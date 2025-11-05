#/***************************************************************************
 * File: ReconTools.cxx
 * Project: dEdxRecon
 *
 * Brief: Implementations of small helper utilities used throughout the
 *        reconstruction code (e.g., corrections, file helpers, selection
 *        initialisation). These functions provide reusable building blocks
 *        for higher-level modules.
 *
 * Contents: implementations of functions declared in ReconTools.h.
 *
 * Notes: Keep these utilities lightweight and well-documented as many
 *        modules depend on them.
 ***************************************************************************/

#include "ReconTools.h"
#include "Displays.h"
#include "LUTs.h"

#include "ClusterFitter.h"
#include "PRFParameters.h"
#include "TrackFitter.h"
#include "TrackRecon.h"

#include "TPaveText.h"
#include "Variables.h"

bool FourModulesInLine(const std::vector<int> &vec) {
  int bitmask = 0;
  // Set corresponding bits for each number in the vector
  for (int num : vec) {
    if (num >= 0 && num <= 7) {
      bitmask |= (1 << num);
    }
  }

  // Define bitmasks for the sets {0,1,2,3} and {4,5,6,7}
  const int set1 = 0b00001111; // 15 (0,1,2,3)
  const int set2 = 0b11110000; // 240 (4,5,6,7)

  // Check if all elements of set1 or set2 are present, regardless of extra
  // elements
  return ((bitmask & set1) == set1) || ((bitmask & set2) == set2);
}

void Reconstruction::WFCorrection(const std::string &OutCorr) {

  // Redirect Output
  std::string out_log = OutCorr.substr(0, OutCorr.length() - 5) + "_corr.log";
  std::cout << out_log << std::endl;
  std::cout << std::endl;
  std::streambuf *coutbuf = std::cout.rdbuf(); // Save old buf
  std::ofstream OUT_DataFile(out_log.c_str()); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf()); // Redirect std::cout to output file

  // Geometry
  float Lx = 11.28;      // Length of pad (mm)
  float Ly = 10.19;      // Height of pad (mm)
  float n_param_trk = 3; // 2 if there is not magnetic field
  float mean_phi = 0;
  int n_events = 0;
  std::cout << selectionSet << std::endl;

  // Get ERAM maps
  std::vector<int> fERAMs_iD;
  std::vector<int> fERAMs_pos;
  if (tag.find("DESY") != std::string::npos) {
    fERAMs_iD.push_back(1);
    fERAMs_pos.push_back(12);
  }
  if (v_datafiles.back().find("ERAM18") != std::string::npos) {
    fERAMs_iD.push_back(18);
    fERAMs_pos.push_back(33);
  }
  if (v_datafiles.back().find("All_ERAMS") != std::string::npos) {
    fERAMs_iD = {7, 1, 23, 2, 16, 15, 10, 12};
    fERAMs_pos = {26, 12, 10, 7, 17, 19, 13, 32};
  }
  Reconstruction::ERAMMaps *pERAMMaps = new Reconstruction::ERAMMaps();
  float AVG_GAIN = 1947.72;
  float PHIMAX = 42.10; // std::atan(10.19/11.28)*180/M_PI;
  float fnParamsTrack = 3;
  int fmodID = 0;

  // Selection stage
  Selector aSelector(selectionSet);
  int NEvents = p_uploader->Get_NberOfEvent();
  Init_selection(selectionSet, aSelector, tag, p_uploader, moduleCase, 0);
  aSelector.Tell_Selection();
  aSelector.Reset_StatCounters();

  TH2F *h2f_WFvsLength = new TH2F("h2f_WFvsLength",
                                  "WF_{sum} VS length in cluster;Length in "
                                  "cluster (mm);WF_{sum} (ADC count)",
                                  80, -0.1, 16, 80, 0, 4100);
  TF1 *A_corr =
      new TF1("A_corr", "[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x", 0,
              17); // values provided by Vlada (2022/10/11)
  A_corr->SetParameters(291, 9.47, -4.04, 1.32, -0.0595);
  TrackFitter aTrackFitter("Minuit", n_param_trk);
  PRFParameters aPRFParameters;
  TF1 *tf1_PRF =
      new TF1("tf1_PRF", aPRFParameters, -2.5 * 1.128, 2.5 * 1.128, 5);
  tf1_PRF->SetParameters(p_uploader->Get_Norm(), p_uploader->Get_a2(),
                         p_uploader->Get_a4(), p_uploader->Get_b2(),
                         p_uploader->Get_b4());
  int counterFail = 0;
  int counterFit = 0;

  std::vector<float> waveform_cluster(510, 0);
  std::vector<float> waveform_pad(510, 0);

  // Event loop
  std::cout << "Processing events:" << std::endl;
  for (int iEvent = 0; iEvent < NEvents; iEvent++) {
    if (iEvent % 1000 == 0 or iEvent == NEvents - 1)
      std::cout << iEvent << "/" << NEvents << std::endl;
    Event *pEvent = p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
    if (!pEvent)
      continue;
    aSelector.ApplySelection(pEvent);
    if (pEvent->IsValid() != 1)
      continue;

    // Loop On Modules
    int NMod = pEvent->Get_NberOfModule();
    for (int iMod = 0; iMod < NMod; iMod++) {
      Module *pModule = pEvent->Get_Module_InArray(iMod);
      fmodID = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(fmodID) == 0)
        continue;

      float NClusters = pModule->Get_NberOfCluster();

      ClusterFitter_Diagonal aClusterFitter_Diagonal("Minuit");
      ClusterFit_Diagonal_Event(-(M_PI_2 - (PHIMAX * M_PI / 180)), pEvent,
                                fmodID, tf1_PRF, counterFit, counterFail,
                                aClusterFitter_Diagonal);
      TrackFitter aTrackFitter("Minuit", fnParamsTrack);
      TrackRecon_Event(aTrackFitter, pEvent, fmodID, fnParamsTrack);

      // Track details
      const Track *pTrack = pEvent->GiveMe_Track_ForThisModule(fmodID);
      mean_phi += std::atan(pTrack->Get_ParameterValue(1));

      // Loop On Clusters
      for (int iC = 0; iC < NClusters; iC++) {
        std::fill(waveform_cluster.begin(), waveform_cluster.end(), 0); // reset
        Cluster *pCluster = pModule->Get_Cluster(iC);
        float trk_len_clus = 0; // in meters
        int NPads = pCluster->Get_NberOfPads();

        // Loop On Pads
        for (int iP = 0; iP < NPads; iP++) {
          std::fill(waveform_pad.begin(), waveform_pad.end(),
                    0); // reset waveform
          Pad *pPad = pCluster->Get_Pad(iP);
          waveform_pad = pPad->Get_vADC();
          double G_pad = pERAMMaps->Gain(fERAMs_pos[fmodID], pPad->Get_iX(),
                                         pPad->Get_iY());
          float Gcorr = AVG_GAIN / G_pad;
          for (int i = 0; i < 510; i++)
            waveform_pad[i] = round(waveform_pad[i] * Gcorr);
          for (int i = 0; i < 510; i++)
            waveform_cluster[i] += waveform_pad[i];

          // Track computations (impact parameter, angle, length in pad)
          float d, dd, phi, trk_len_pad;
          local_params(pPad, pTrack, d, dd, phi, trk_len_pad);
          trk_len_clus += trk_len_pad;
        }

        if (trk_len_clus * 1000 > 0.002) {
          h2f_WFvsLength->Fill(trk_len_clus * 1000,
                               *std::max_element(waveform_cluster.begin(),
                                                 waveform_cluster.end()));
        }
      }
    }
    delete pEvent;
    n_events++;
  }
  mean_phi /= n_events;
  std::cout << std::setprecision(2)
            << "mean_phi	= " << mean_phi * 180 / M_PI << std::endl;
  aSelector.PrintStat();
  delete tf1_PRF;

  // Fitting
  float L_phi = std::min({Lx / fabs(cos(mean_phi)), Ly / fabs(sin(mean_phi))});
  A_corr->SetNameTitle("A_corr", "A_corr");
  TGraph *tge_WFvsLength = Convert_TH2_TGE(h2f_WFvsLength);
  tge_WFvsLength->SetNameTitle("pTGE_A_corr", "pTGE_A_corr");
  tge_WFvsLength->Fit(A_corr, "RQ", "0", 0, L_phi);

  // Saving
  TFile *pfileROOT_corr = new TFile(corrFuncPath.c_str(), "RECREATE");
  h2f_WFvsLength->Write();
  A_corr->Write();
  tge_WFvsLength->Write();
  pfileROOT_corr->Close();

  // Deleting
  delete h2f_WFvsLength;
  delete tge_WFvsLength;
  delete A_corr;
  delete pfileROOT_corr;

  std::cout.rdbuf(coutbuf); // Reset to standard output
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
TF1 *corr_func(const std::string &EventFile, const std::string &Tag,
               const int &correctWF) {
  // Don't overwrite the current directory in case one was opened before using
  // this function
  TDirectory *currdir = gDirectory;

  // correctWF = 3: no correction
  if (correctWF == 3) {
    TF1 *corr_func = new TF1("corr_func", "1", 0, 17);
    return corr_func;
  }

  // Get the name of the files to get the exclusive function
  TF1 *corr_func = new TF1(
      "corr_func",
      "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0,
      17); // values provided by Vlada (2022/10/11)
  std::string filename = EventFile.substr(0, EventFile.length() - 5);

  // correctWF = 2: use the correction function from HATRecon
  if (Tag.find("diag") == std::string::npos or correctWF == 2)
    return corr_func;

  // correctWF = 0: get a single file to apply the same correction for all the
  // files
  if (correctWF == 0 or correctWF == 4) {
    int angle;
    if ((angle = filename.find("30")) != (int)std::string::npos or
        (angle = filename.find("45")) != (int)std::string::npos)
      filename.replace(angle, 2, "40");
    while ((angle = filename.find("460")) != (int)std::string::npos or
           (angle = filename.find("860")) != (int)std::string::npos)
      filename.replace(angle, 3, "m40");
  }

  // correctWF = 1: get the correction function from the file
  std::string filename_corr = filename + "_WFCorrFunc.root";
  TFile *pfile = new TFile(filename_corr.c_str(), "READ");
  corr_func = pfile->Get<TF1>("A_corr");
  pfile->Close();

  // correctWF = 4: get a single file to apply the same correction for all the
  // files and shift it by -100
  if (correctWF == 4) {
    corr_func->SetParameter(0, corr_func->GetParameter(0) - 100);
    corr_func->SetNameTitle("A_corr_shifted", "A_corr_shifted");
  }

  std::cout << "correction function: " << filename_corr << std::endl;
  std::cout << std::setprecision(2)
            << "WF correction parameters: " << corr_func->GetParameter(0)
            << " | " << corr_func->GetParameter(1) << " | "
            << corr_func->GetParameter(2) << " | " << corr_func->GetParameter(3)
            << " | " << corr_func->GetParameter(4) << std::endl;
  delete pfile;

  currdir->cd();
  return corr_func;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Init_selection(const std::string &SelectionSet, Selector &aSelector,
                    const std::string &Tag, Uploader *pUploader,
                    const int &NbrOfMod, const int &Data_to_Use) {
  // Get the correct cut on TLeading
  if (SelectionSet == "Sel_CERN22" or SelectionSet == "Sel_DESY21") {
    int TLow = 0, THigh = 0;
    if (GetStage3Cut_CSV("../TimeSelection_Cuts.csv", Tag, TLow, THigh))
      std::cout << "TLow	= " << TLow << " | THigh	= " << THigh
                << std::endl;
    else {
      std::cout << "No Stage3 cuts found in CSV. Getting them now..."
                << std::endl;
      std::vector<int> v_TCut =
          ComputeCutStage3_Cut(pUploader, NbrOfMod, Data_to_Use);
      TLow = v_TCut[0];
      THigh = v_TCut[1];
      SetStage3Cut_CSV("../TimeSelection_Cuts.csv", Tag, TLow, THigh);
      std::cout << "Stage3 cuts are " << TLow << " to " << THigh
                << ". Values added to CSV file." << std::endl;
    }
    aSelector.Set_Cut_Stage3_TLow(TLow);
    aSelector.Set_Cut_Stage3_THigh(THigh);
  }

  // Selection for DESY21 phi diagonal clustering
  if (Tag.find("diag") != std::string::npos) {
    aSelector.Set_Cut_StageFinal_NCluster_Low(50);
    aSelector.Set_Cut_Stage4_APM_Low(1);
    aSelector.Set_Cut_Stage4_APM_High(3.5);
  }

  // Selection for DESY21 theta
  if (Tag.find("theta") != std::string::npos) {
    aSelector.Set_Cut_Stage3_TLow(0);
    aSelector.Set_Cut_Stage3_THigh(510);
    aSelector.Set_Cut_Stage2_EventBased(200);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// FILE HANDLING
// /////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read CSV file probably rotates by 90° the original CSV file
std::vector<std::vector<float>> readCSV(std::string filename) {
  std::vector<std::vector<float>> data;
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream lineStream(line);
    std::string cell;
    std::vector<float> row;
    while (std::getline(lineStream, cell, ',')) {
      row.push_back(std::stof(cell));
    }
    data.push_back(row);
  }
  return data;
}

// Get cut values for the time selection (run based)
std::vector<int> ComputeCutStage3_Cut(Uploader *pUploader, const int &NbrOfMod,
                                      const int &Data_to_Use) {
  std::vector<int> v_TCut;

  TH1F *h1f_TLead = new TH1F("h1f_TLead", "T_{leading}", 511, -0.5, 510.5);

  int NEvent = pUploader->Get_NberOfEvent();
  int TLow = 0;
  int THigh = 0;
  for (int iEvent = 0; iEvent < NEvent; iEvent++) {
    if (iEvent % 1000 == 0 or iEvent == NEvent - 1)
      std::cout << iEvent << "/" << NEvent << std::endl;
    Event *pEvent = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0);
    if (!pEvent) {
      delete pEvent;
      continue;
    }

    int nMod = pEvent->Get_NberOfModule();
    for (int iMod = 0; iMod < nMod; iMod++) {
      Module *pModule = pEvent->Get_Module_InArray(iMod);
      // if (pEvent->Validity_ForThisModule(iMod) == 0) continue;

      int NClusters = pModule->Get_NberOfCluster();
      for (int iC = 0; iC < NClusters; iC++) {
        Cluster *pCluster = pModule->Get_Cluster(iC);
        double TL = pCluster->Get_TMaxLeading();
        if (TL > 5. and TL < 509.)
          h1f_TLead->Fill(TL);
      }
    }
    delete pEvent;
  }

  double MaxTLead = h1f_TLead->GetMaximum();
  int nbin_max = h1f_TLead->GetMaximumBin();

  if (nbin_max <= 0 or nbin_max >= 510) {
    std::cout << "Bug with nbin_max: value out of time range | Set cuts "
                 "to	[0, 510]"
              << std::endl;
    v_TCut.push_back(0);
    v_TCut.push_back(510);
    delete h1f_TLead;
    return v_TCut;
  }

  int iDeltaBinP = 10; // initialization of increments
  int iDeltaBinM = 10;
  float y_init = h1f_TLead->GetBinContent(nbin_max); // maximum of the peak
  float y_minus1 =
      h1f_TLead->GetBinContent(nbin_max - 10);    // value of the previous bin
  float y_0 = h1f_TLead->GetBinContent(nbin_max); // value of the current bin
  float y_plus1 =
      h1f_TLead->GetBinContent(nbin_max + 10); // value of the next bin
  // Find the lower boundary
  while (y_minus1 - y_0 < y_init / 250 and y_minus1 > 0.001 * y_init and
         iDeltaBinM < 509) { // 2500 for DESY21, 250 for CERN22
    iDeltaBinM++;
    y_0 = h1f_TLead->GetBinContent(nbin_max - iDeltaBinM);
    y_minus1 = h1f_TLead->GetBinContent(nbin_max - iDeltaBinM - 1);
  }
  // Find the higher boundary
  y_0 = h1f_TLead->GetBinContent(nbin_max); // reboot y_0 to maximum value peak
  while (y_plus1 - y_0 < y_init / 250 and y_minus1 > 0.001 * y_init and
         iDeltaBinP < 509) {
    iDeltaBinP++;
    y_0 = h1f_TLead->GetBinContent(nbin_max + iDeltaBinP);
    y_plus1 = h1f_TLead->GetBinContent(nbin_max + iDeltaBinP + 1);
  }

  TLow = nbin_max - 1 - iDeltaBinM;
  THigh = nbin_max - 1 + iDeltaBinP;
  if (TLow < 0 or TLow > 510) {
    std::cout << "Bug with TLow	= " << TLow << ": value out of time range | ";
    TLow = 0;
    std::cout << "Reset to " << TLow << std::endl;
  }
  if (THigh < 0 or THigh > 510) {
    std::cout << "Bug with THigh	= " << THigh
              << ": value out of time range | ";
    THigh = 510;
    std::cout << "Reset to " << THigh << std::endl;
  }
  v_TCut.push_back(TLow);
  v_TCut.push_back(THigh);
  delete h1f_TLead;
  return v_TCut;
}

// Function to search for a word in a CSV file
bool GetStage3Cut_CSV(const std::string &filename,
                      const std::string &targetWord, int &value1, int &value2) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string word;
    double val1, val2;

    // Parse the CSV line
    if (std::getline(ss, word, ',') && ss >> val1 && ss.ignore() &&
        ss >> val2) {
      if (word == targetWord) {
        value1 = val1;
        value2 = val2;
        return true; // Word found
      }
    }
  }

  return false; // Word not found
}

// Function to update a CSV file
void SetStage3Cut_CSV(const std::string &filename,
                      const std::string &targetWord, int value1, int value2) {
  std::ofstream file(filename, std::ios_base::app); // Open in append mode

  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return;
  }

  // Append a new line with the provided values
  file << targetWord << ", " << value1 << ", " << value2 << std::endl;

  file.close();
}

// General Math
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TF1 *Fit2Gauss(TH1F *h1F) { return Fit2Gauss(h1F, -5, 1, -1, 2); }
TF1 *Fit2Gauss(TH1F *h1F, const float &x1min, const float &x1max,
               const float &x2min, const float &x2max) {
  double par[6];
  float min1 = h1F->GetMean() + x1min * h1F->GetStdDev();
  float max1 = h1F->GetMean() + x1max * h1F->GetStdDev();
  float min2 = h1F->GetMean() + x2min * h1F->GetStdDev();
  float max2 = h1F->GetMean() + x2max * h1F->GetStdDev();

  TF1 *g1 = new TF1("g1", "gausn");
  h1F->Fit(g1, "QR", "0", min1, max1);
  g1->GetParameters(&par[0]);
  delete g1;

  TF1 *g2 = new TF1("g2", "gausn");
  h1F->Fit(g2, "QR", "0", min2, max2);
  g2->GetParameters(&par[3]);
  delete g2;

  TF1 *g3 = new TF1("g3", "gausn(0)+gausn(3)");
  g3->SetParameters(par);
  h1F->Fit(g3, "QRS", "0", min1, max2);

  TF1 *tf1 = h1F->GetFunction("g3");
  tf1->SetParName(0, "A_1");
  tf1->SetParName(1, "#mu_1");
  tf1->SetParName(2, "#sigma_1");
  tf1->SetParName(3, "A_2");
  tf1->SetParName(4, "#mu_2");
  tf1->SetParName(5, "#sigma_2");
  return tf1;
}

// General Physics
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bethe-Bloch relativistic for heavy charged particles
TF1 *BetheBloch(const float &Pmin, const float &Pmax, const double &M,
                const std::string &particle) {
  /*	Input: Pmin & Pmax GeV | m GeV
     Output: keV/cm */
  // float n						= 5.357e20; e/cm^3
  // float alpha2					= pow(1/137, 2);
  // float hbar2c2					= pow(1.973e-14, 2) (GeV
  // cm)^2
  double coeff = 0.2723; // 4 pi n alpha^2 hbar^2 c^2 / (m_e c^2) (keV^2/cm)
  double me = 511e-6;    // GeV
  double I = 188e-9;     // GeV

  // PDG general
  const char *formula =
      "[1]/(x*x)*(x*x+[0]*[0]) * (log(2*[2]/[3]*x*x/([0]*[0])) - 0.5*log(1 + "
      "2*sqrt(x*x+[0]*[0])/([0]*[0])*[2]/[0] + [2]*[2]/([0]*[0])) - "
      "x*x/(x*x+[0]*[0]))";
  TF1 *dEdx =
      new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->SetParameters(M, coeff, me, I);
  dEdx->FixParameter(0, M);
  dEdx->FixParameter(2, me);
  dEdx->FixParameter(3, I);

  dEdx->SetTitle(
      Form("Bethe-Bloch for%s;Energy (GeV);mean (keV/cm)", particle.c_str()));
  return dEdx; // keV/cm
}

// Bethe-Bloch relativistic for positrons with Bhabha scattering
TF1 *BetheBlochBhabha(const float &Pmin, const float &Pmax, const double &m,
                      const std::string &particle) {
  /*	Input: Pmin & Pmax GeV | M GeV
     Output: keV/cm */
  // float n						= 5.357e20; e/cm^3
  // float alpha2					= pow(1/137, 2);
  // float hbar2c2					= pow(1.973e-14, 2) (GeV
  // cm)^2
  double coeff = 0.2723; // 4 pi n alpha^2 hbar^2 c^2 / (m_e c^2) (keV^2/cm)
  double me = 511e-6;    // GeV
  double M2 = pow(m, 2); // GeV^2
  double I = 188e-9;     // GeV

  // PDG Bhabha
  const char *formula =
      "[0]/(x*x)*(x*x+[2]) * (log(2*[1]/[3]*x*x/[2]) - 0.5*log(1 + "
      "2*sqrt(x*x+[2])/[2]*[1]/sqrt([2]) + [1]*[1]/[2]) - x*x/(x*x+[2])/24. * "
      "(23 + "
      "14/(sqrt(x*x+[2])/[2]-1) + 10/pow(sqrt(x*x+[2])/[2]-1, 2) + "
      "4/pow(sqrt(x*x+[2])/[2]-1, 3)) )";
  TF1 *dEdx =
      new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->SetParameters(coeff, me, M2, I);

  dEdx->SetTitle(
      Form("Bethe-Bloch with Bhabha Xsec for%s;Energy (GeV);mean (keV/cm)",
           particle.c_str()));
  return dEdx; // keV/cm
}

// Bethe-Bloch with experimental parametrisation
TF1 *BetheBlochExp(const float &Pmin, const float &Pmax, const double &M,
                   const std::string &particle) {
  /*	Input: Pmin & Pmax GeV | m GeV
     Output: keV/cm */

  const char *formula = "[0]/pow(x/sqrt(x*x+[5]*[5]),[3]) * ( [1] - "
                        "pow(x/sqrt(x*x+[5]*[5]),[3]) - log([2] "
                        "+ 1 / pow(x/[5], [4])) )";
  TF1 *dEdx =
      new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->FixParameter(5, M);

  // double dedxexp = (par0 / pow(beta, par3)) * (par1 - pow(beta, par3) -
  // TMath::Log(par2 + (1 / pow(bg, par4))));

  dEdx->SetTitle(
      Form("Experimental Bethe-Bloch for%s;Energy (GeV);mean (keV/cm)",
           particle.c_str()));
  return dEdx; // keV/cm
}

// Specific Math
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// impact parameter d (in m) & track angle phi (in degrees) computed locally at
// the level of the pad
void local_params(Pad *pPad, const Track *pTrack, float &d, float &dd,
                  float &phi, float &trk_len_pad) {

  int dd_compute =
      1; // compute error on impact parameter, involve matrix	=> slow

  // Get geometry of pad
  float x0 = pPad->Get_XL();   // in meters
  float xc = pPad->Get_XPad(); // in meters
  float x1 = pPad->Get_XH();   // in meters
  float y0 = pPad->Get_YL();   // in meters
  float yc = pPad->Get_YPad(); // in meters
  float y1 = pPad->Get_YH();   // in meters

  // Parameters of the fitted Track
  double a = pTrack->Get_ParameterValue(0);  // in meters	(intercept)
  double da = pTrack->Get_ParameterError(0); // in meters	(intercept)
  double b = pTrack->Get_ParameterValue(1);  // no dimension (slope)
  double db = pTrack->Get_ParameterError(1); // no dimension (slope)
  double c;
  double dc;

  phi = TMath::ATan(b) / TMath::Pi() * 180;

  // Impact parameter in the pad
  d = (b * xc - yc + a) /
      sqrt(pow(b, 2) + 1); // if pTrack is a line, then get d from it directly
  float da_d = 1 / sqrt(b * b + 1);
  float db_d =
      (xc * sqrt(b * b + 1) - (b * xc - yc + a) * b / sqrt(b * b + 1)) /
      (b * b + 1);
  dd = sqrt(da_d * da_d * da * da + db_d * db_d * db * db);

  // Y position of the track at the left & right borders of the pad and X
  // position at top and bottom borders
  float x_y0 = (y0 - a) / b; // in meters
  float x_y1 = (y1 - a) / b; // in meters
  float y_x0 = b * x0 + a;   // in meters
  float y_x1 = b * x1 + a;   // in meters

  float dx_y0 = sqrt(1 / b * b * da * da +
                     pow((y0 - a) / b * b, 2) * db * db); // in meters
  float dx_y1 = sqrt(1 / b * b * da * da +
                     pow((y1 - a) / b * b, 2) * db * db); // in meters
  float dy_x0 = sqrt(da * da + x0 * x0 * db * db);        // in meters
  float dy_x1 = sqrt(da * da + x1 * x1 * db * db);        // in meters

  // If parabolic fit <=> Magnetic field
  if (pTrack->GetNberOfParameters() == 3) {
    c = pTrack->Get_ParameterValue(2);  // in 1/meters	(curvature)
    dc = pTrack->Get_ParameterError(2); // in 1/meters	(curvature)

    float Delta_y0 = sqrt(b * b - 4 * (a - y0) * c);
    float Delta_y1 = sqrt(b * b - 4 * (a - y1) * c);

    // Roots of Pol2 to get X position of track at top and bottom borders of the
    // pad (with parabolic track)
    float xroot1_y0 = (-b - Delta_y0) / (2 * c);
    float da_xroot1_y0 = 1 / Delta_y0;
    float db_xroot1_y0 = 1 / (2 * c) * (-b / Delta_y0 - 1);
    float dc_xroot1_y0 =
        (a - y0) / (c * Delta_y0) + (b + Delta_y0) / (2 * c * c);
    float dxroot1_y0 = sqrt(da_xroot1_y0 * da_xroot1_y0 * da * da +
                            db_xroot1_y0 * db_xroot1_y0 * db * db +
                            dc_xroot1_y0 * dc_xroot1_y0 * dc * dc);

    float xroot2_y0 = (-b + Delta_y0) / (2 * c);
    float da_xroot2_y0 = -1 / Delta_y0;
    float db_xroot2_y0 = 1 / (2 * c) * (+b / Delta_y0 - 1);
    float dc_xroot2_y0 =
        -(a - y0) / (c * Delta_y0) + (b - Delta_y0) / (2 * c * c);
    float dxroot2_y0 = sqrt(da_xroot2_y0 * da_xroot2_y0 * da * da +
                            db_xroot2_y0 * db_xroot2_y0 * db * db +
                            dc_xroot2_y0 * dc_xroot2_y0 * dc * dc);

    float xroot1_y1 = (-b - Delta_y1) / (2 * c);
    float da_xroot1_y1 = 1 / Delta_y1;
    float db_xroot1_y1 = 1 / (2 * c) * (-b / Delta_y1 - 1);
    float dc_xroot1_y1 =
        (a - y1) / (c * Delta_y1) + (b + Delta_y1) / (2 * c * c);
    float dxroot1_y1 = sqrt(da_xroot1_y1 * da_xroot1_y1 * da * da +
                            db_xroot1_y1 * db_xroot1_y1 * db * db +
                            dc_xroot1_y1 * dc_xroot1_y1 * dc * dc);

    float xroot2_y1 = (-b + Delta_y1) / (2 * c);
    float da_xroot2_y1 = -1 / Delta_y1;
    float db_xroot2_y1 = 1 / (2 * c) * (+b / Delta_y1 - 1);
    float dc_xroot2_y1 =
        -(a - y1) / (c * Delta_y1) + (b - Delta_y1) / (2 * c * c);
    float dxroot2_y1 = sqrt(da_xroot2_y1 * da_xroot2_y1 * da * da +
                            db_xroot2_y1 * db_xroot2_y1 * db * db +
                            dc_xroot2_y1 * dc_xroot2_y1 * dc * dc);

    // Select correct root	= closest to the center of the pad
    if (fabs(xroot1_y0 - xc) < fabs((xroot2_y0 - xc))) {
      x_y0 = xroot1_y0;
      dx_y0 = dxroot1_y0;
    } else {
      x_y0 = xroot2_y0;
      dx_y0 = dxroot2_y0;
    }
    if (fabs(xroot1_y1 - xc) < fabs((xroot2_y1 - xc))) {
      x_y1 = xroot1_y1;
      dx_y1 = dxroot1_y1;
    } else {
      x_y1 = xroot2_y1;
      dx_y1 = dxroot2_y1;
    }

    // Y position of the track at level of left & right borders of the pad (with
    // parabolic track)
    y_x0 = a + b * x0 + c * x0 * x0;
    float da_y_x0 = 1.;
    float db_y_x0 = x0;
    float dc_y_x0 = x0 * x0;

    y_x1 = a + b * x1 + c * x1 * x1;
    float da_y_x1 = 1.;
    float db_y_x1 = x1;
    float dc_y_x1 = x1 * x1;

    // Get error on Y
    std::vector<double> errorsy_x0 = {da_y_x0, db_y_x0, dc_y_x0};
    std::vector<double> errorsy_x1 = {da_y_x1, db_y_x1, dc_y_x1};

    double cov_dy_x0 = 0;
    double cov_dy_x1 = 0;
    TMatrixD pCovMat = pTrack->Get_CovMatrix();
    for (int i = 0; i < (int)pCovMat.GetNrows(); i++) {
      for (int j = 0; j < (int)pCovMat.GetNcols(); j++) {
        cov_dy_x0 += errorsy_x0[i] * pCovMat(i, j) * errorsy_x0[j];
        cov_dy_x1 += errorsy_x1[i] * pCovMat(i, j) * errorsy_x1[j];
      }
    }
    dy_x0 = sqrt(cov_dy_x0);
    dy_x1 = sqrt(cov_dy_x1);
  }

  int wall = 0; // count how many walls were crossed
  float x[3] = {0};
  float y[3] = {0};
  float dx[3] = {0};
  float dy[3] = {0};

  // Determine which walls were crossed
  if (y0 <= y_x0 && y_x0 <= y1) { // left
    x[wall] = x0;
    y[wall] = y_x0;
    dx[wall] = 0;
    dy[wall] = dy_x0;
    wall++;
  }
  if (y0 <= y_x1 && y_x1 <= y1) { // right
    x[wall] = x1;
    y[wall] = y_x1;
    dx[wall] = 0;
    dy[wall] = dy_x1;
    wall++;
  }
  if (x0 <= x_y1 && x_y1 <= x1) { // top
    x[wall] = x_y1;
    y[wall] = y1;
    dx[wall] = dx_y1;
    dy[wall] = 0;
    wall++;
  }
  if (x0 <= x_y0 && x_y0 <= x1) { // bottom
    x[wall] = x_y0;
    y[wall] = y0;
    dx[wall] = dx_y0;
    dy[wall] = 0;
    wall++;
  }

  if (wall != 2 and wall > 0) {
    std::cout << "ALERT: " << wall << " walls crossed for a pad in entry #"
              << pPad->Get_EntryNber() << std::endl;
    std::cout << x0 * 100 << " " << x1 * 100 << " " << y0 * 100 << " "
              << y1 * 100 << std::endl;
    std::cout << x_y0 * 100 << " " << x_y1 * 100 << " " << y_x0 * 100 << " "
              << y_x1 * 100 << std::endl;
    std::cout << a * 100 << " " << b << " "
              << pTrack->Get_ParameterValue(2) / 100 << std::endl;
  }

  trk_len_pad =
      sqrt(pow(y[1] - y[0], 2) +
           pow(x[1] - x[0], 2)); // in meters (track length in the pad)

  float m = 0, q = 0;
  float dm = 0, dq = 0;
  // If parabolic fit <=> Magnetic field
  if (pTrack->GetNberOfParameters() == 3) {
    TMatrixD pCovMat = pTrack->Get_CovMatrix();
    // Get intercept & slope from entrance & exit points
    // q						= (y[0]*x[1]-y[1]*x[0])
    // / (x[1]-x[0]); m						=
    // (y[1]-y[0]) / (x[1]-x[0]);
    q = a - c * x[0] * x[1];
    m = b + c * (x[1] + x[0]);

    if (dd_compute) {
      // float da_q				= 1;
      // float db_q				= 0;
      float dc_q = -x[0] * x[1];
      // double errorsq[]		= {da_q, db_q, dc_q};

      // Get slope from entrance & exit points
      // float da_m				= 0;
      // float db_m				= 1;
      float dc_m = x[1] + x[0];
      // double errorsm[]		= {da_m, db_m, dc_m};

      // Get impact parameter from entrance & exit points
      float da_d = 1 / sqrt(m * m + 1);
      float db_d =
          xc / sqrt(m * m + 1) - (m * xc + q - yc) * m / pow(m * m + 1, 1.5);
      float dc_d = (dc_m * xc + dc_q) / sqrt(m * m + 1) -
                   (m * xc + q - yc) * dc_m * m / pow(m * m + 1, 1.5);
      double errorsd[] = {da_d, db_d, dc_d};

      TMatrixD row_err_d(1, 3, errorsd);
      TMatrixD d_mat_temp = row_err_d * pCovMat;
      TMatrixD dd_matrix = d_mat_temp * row_err_d.Transpose(row_err_d);

      // TMatrixD row_err_q(1, 3, errorsq);
      // d_mat_temp				= row_err_q * pCovMat;
      // TMatrixD dq_matrix		= d_mat_temp *
      // row_err_q.Transpose(row_err_q);

      // TMatrixD row_err_m(1, 3, errorsm);
      // d_mat_temp				= row_err_m * pCovMat;
      // TMatrixD dm_matrix		= d_mat_temp *
      // row_err_m.Transpose(row_err_m);

      // dd						= sqrt(dd_matrix(0,0));
      // // in meters dq						=
      // sqrt(dq_matrix(0,0)); // in meters dm
      // = sqrt(dm_matrix(0,0)); // no dimension
    }

    phi = TMath::ATan(m) / TMath::Pi() * 180;                      // in degrees
    trk_len_pad = sqrt(pow(y[1] - y[0], 2) + pow(x[1] - x[0], 2)); // in meters
    d = (m * xc - yc + q) / sqrt(pow(m, 2) + 1);                   // in meters
  }

  // if(x[0] and dd_compute){
  //	std::cout << std::setprecision(3) << "x_in: "	<< x[0]*1000 << " +- "
  //<< dx[0]*1000
  //<< " | "; 	std::cout << std::setprecision(3) << "y_in: "	<< y[0]*1000 <<
  //" +- "
  //<< dy[0]*1000 << " | "; 	std::cout << std::setprecision(3) << "x_out: "
  //<< x[1]*1000
  // << "
  //+- " << dx[1]*1000 << " | "; 	std::cout << std::setprecision(3) <<
  //"y_out: " <<
  // y[1]*1000 << " +- " << dy[1]*1000 << " | "; 	std::cout <<
  // std::setprecision(3) << "q: "	<< q*1000	<< " +- " << dq*1000
  // << " | "; 	std::cout << std::setprecision(3)
  //<< "m: "	<< m		<< " +- " << dm		<< " | ";
  //std::cout << std::setprecision(3)
  //<< "d: "	<< d*1000	<< " +- " << dd*1000	<< std::endl;
  // }
}

// Track length in ERAM in m
float trk_len(Module *pModule, const Track *pTrack) {
  int n_param = pTrack->GetNberOfParameters();
  float a = 0;
  if (n_param == 3)
    a = pTrack->Get_ParameterValue(2);     // curvature
  float b = pTrack->Get_ParameterValue(1); // slope
  int NC = pModule->Get_NberOfCluster();
  float start = pModule->Get_Cluster(0)->Get_LeadingPad()->Get_XL();
  float end = pModule->Get_Cluster(NC - 1)->Get_LeadingPad()->Get_XH();

  float L = 0;
  if (n_param == 2)
    L = sqrt(pow(b, 2) + 1) * (end - start);

  if (n_param == 3) {
    // computing arc length of P[2] polynomial between start and end points
    float in = ((b + 2 * a * start) * sqrt(1 + pow(b + 2 * a * start, 2)) +
                TMath::ASinH(b + 2 * a * start)) /
               (4 * a);
    float out = ((b + 2 * a * end) * sqrt(1 + pow(b + 2 * a * end, 2)) +
                 TMath::ASinH(b + 2 * a * end)) /
                (4 * a);
    L = out - in;
  }

  return L; // in m
}

// ROOT
// /////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw TH2
void DrawTH2(const std::string &OutDir, TH2 *h2) {
  TCanvas *pCanTH2 = new TCanvas("pCanTH2", "pCanTH2", 1800, 1200);
  pCanTH2->cd();
  h2->SetMaximum(2.1);
  h2->Draw("colz");
  pCanTH2->SaveAs(TString(OutDir + h2->GetName() + ".png"));
  delete pCanTH2;
}

// From TH1 get TGraph
TGraph *hist_to_graph(TH1 *h1) {
  TGraph *gr = new TGraph(h1);
  Int_t nbins = gr->GetN();
  Double_t x, y;
  for (int n = 0; n < nbins; ++n) {
    gr->GetPoint(n, x, y);
    gr->SetPoint(n, x, y);
  }
  return gr;
}

// From TH1 get swapped TGraph (transpose x and y)
TGraph *Swapped_graph(TH1 *h1) {
  TGraph *gr = new TGraph(h1);
  Int_t nbins = gr->GetN();
  Double_t x, y;
  for (int n = 0; n < nbins; ++n) {
    gr->GetPoint(n, x, y);
    gr->SetPoint(n, y, x);
  }
  return gr;
}