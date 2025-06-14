#include "LUTs.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end

ClassImp(EramInfo)

   /* ERAM MAPS READING
      ------------------------------------------------------------------------------------------------------------------
    */
   // Default constructor

   Reconstruction::ERAMMaps::ERAMMaps(const std::string &file)
{
   fFile = file;
   fmean_gain.assign(34, 0);
   fmean_RC.assign(34, 0);

   std::cout << "ERAM maps: LOADING " << fFile << std::endl;
   Load();
   std::cout << "ERAM maps: LOADED" << std::endl;
}

// Destructor
Reconstruction::ERAMMaps::~ERAMMaps()
{
   pFile->Close();
   delete pFile;
   for (int i = 0; i < (int)fID.size(); i++)
      fID[i] = 0;
   fID.clear();
   for (int i = 0; i < 32; i++) {
      for (int j = 0; j < 36; j++) {
         for (int k = 0; k < 32; k++) {
            fGain[i][j][k] = 0;
            fRC[i][j][k] = 0;
            fResolution[i][j][k] = 0;
         }
      }
   }
}

// Load the ERAM maps
void Reconstruction::ERAMMaps::Load()
{
   pFile = TFile::Open(fFile.c_str(), "READ");
   pTree = (TTree *)pFile->Get("Infotree");
   pBranch = pTree->GetBranch("EramInfo");
   int nentries = pTree->GetEntries();
   pTree->SetMakeClass(1); // Because ROOT version in ND280 is old
   pTree->SetBranchAddress("Id", &fid, &pBranch);
   pTree->SetBranchAddress("Position", &fpos, &pBranch);
   pTree->SetBranchAddress("XX", &fx, &pBranch);
   pTree->SetBranchAddress("YY", &fy, &pBranch);
   pTree->SetBranchAddress("RC", &frc, &pBranch);
   pTree->SetBranchAddress("Gain", &fgain, &pBranch);
   pTree->SetBranchAddress("Resolution", &fres, &pBranch);
   pTree->GetVal(0);

   // Initializing all values to 0
   for (int i = 0; i < 32; i++)
      for (int j = 0; j < 36; j++)
         for (int k = 0; k < 32; k++) {
            setGain(i, j, k, 0);
            setRC(i, j, k, 0);
            setResolution(i, j, k, 0);
         }

   // Filling the maps
   for (int i = 0; i < nentries; i++) {
      pTree->GetEntry(i);
      if (fid == 12) { // Not mounted @ JPARC but used in CERN22 Mockup
         setGain(32, fx, fy, fgain);
         setRC(32, fx, fy, frc);
         setResolution(32, fx, fy, fres);
      }
      if (fid == 18) { // Not mounted @ JPARC but used for CERN22 prototype
         setGain(33, fx, fy, fgain);
         setRC(33, fx, fy, frc);
         setResolution(33, fx, fy, fres);
      }
      if (fpos > 31)
         continue;
      setGain(fpos, fx, fy, fgain);
      setRC(fpos, fx, fy, frc);
      setResolution(fpos, fx, fy, fres);
   }

   // Fill holes in the maps
   FillHoles();

   float meanGain = 0;
   float meanRC = 0;
   for (int i = 0; i < 34; i++) {
      for (int j = 0; j < 36; j++) {
         for (int k = 0; k < 32; k++) {
            meanGain += Gain(i, j, k);
            meanRC += RC(i, j, k);
         }
      }
      meanGain /= 1152;
      meanRC /= 1152;
      setMeanGain(i, meanGain);
      setMeanRC(i, meanRC);
      if (verbose)
         std::cout << "ERAM#" << ID(i) << ": mean Gain	= " << MeanGain(i)
                   << " | mean RC	= " << MeanRC(i) << std::endl;
   }
}

/* Public functions */
// Getters
int Reconstruction::ERAMMaps::ID(const int &position)
{
   return channel2iD[position];
}

float Reconstruction::ERAMMaps::RC(const int &position, const int &iX, const int &iY)
{
   return fRC[position][iX][iY];
}

float Reconstruction::ERAMMaps::Gain(const int &position, const int &iX, const int &iY)
{
   return fGain[position][iX][iY];
}

float Reconstruction::ERAMMaps::Resolution(const int &position, const int &iX,
                                           const int &iY)
{
   return fResolution[position][iX][iY];
}
float Reconstruction::ERAMMaps::MeanGain(const int &position)
{
   return fmean_gain[position];
}
float Reconstruction::ERAMMaps::MeanRC(const int &position)
{
   return fmean_RC[position];
}

/* Private functions */
// Setters
// ERAMs on endplates 1&3 are X-flipped compared to their maps in the file
void Reconstruction::ERAMMaps::setGain(const int &position, const int &iX, const int &iY,
                                       const float &gain)
{
   fGain[position][iX][iY] = gain;
}

void Reconstruction::ERAMMaps::setRC(const int &position, const int &iX, const int &iY,
                                     const float &RC)
{
   fRC[position][iX][iY] = RC;
}

void Reconstruction::ERAMMaps::setResolution(const int &position, const int &iX,
                                             const int &iY, const float &resolution)
{
   fResolution[position][iX][iY] = resolution;
}
void Reconstruction::ERAMMaps::setMeanGain(const int &position, const float &meanGain)
{
   fmean_gain[position] = meanGain;
}
void Reconstruction::ERAMMaps::setMeanRC(const int &position, const float &meanRC)
{
   fmean_RC[position] = meanRC;
}

void Reconstruction::ERAMMaps::FillHoles()
{
   for (int i = 0; i < 32; i++) {
      for (int iY = 0; iY < 32; iY++) {
         for (int iX = 0; iX < 36; iX++) {
            float gain = Gain(i, iX, iY);
            float rc = RC(i, iX, iY);

            v_sides.assign(4, 0);
            if (gain == 0) {
               if (iX > 0)
                  v_sides[0] = Gain(i, iX - 1, iY);
               if (iX < 35)
                  v_sides[1] = Gain(i, iX + 1, iY);
               if (iY > 0)
                  v_sides[2] = Gain(i, iX, iY - 1);
               if (iY < 31)
                  v_sides[3] = Gain(i, iX, iY + 1);
               float n_sides = 0;
               for (int i = 0; i < 4; i++)
                  if (v_sides[i] != 0) { // additionnal step to discard empty neighbours
                     gain += v_sides[i];
                     n_sides++;
                  }
               gain /= n_sides;
               setGain(i, iX, iY, gain);
               if (verbose)
                  std::cout << "ERAM#" << std::setw(2) << ID(i) << std::setw(2) << " ("
                            << std::setw(2) << i << "): "
                            << "Gain hole in (iX,iY)	= (" << iX << "," << iY
                            << ") | value reset at " << Gain(i, iX, iY) << std::endl;
            }

            v_sides.assign(4, 0);
            if (rc == 0) {
               if (iX > 0)
                  v_sides[0] = RC(i, iX - 1, iY);
               if (iX < 35)
                  v_sides[1] = RC(i, iX + 1, iY);
               if (iY > 0)
                  v_sides[2] = RC(i, iX, iY - 1);
               if (iY < 31)
                  v_sides[3] = RC(i, iX, iY + 1);
               float n_sides = 0;
               for (int i = 0; i < 4; i++)
                  if (v_sides[i] != 0) { // additionnal step to discard empty neighbours
                     rc += v_sides[i];
                     n_sides++;
                  }
               rc /= n_sides;
               setRC(i, iX, iY, rc);
               if (verbose)
                  std::cout << "ERAM#" << std::setw(2) << ID(i) << std::setw(2) << " ("
                            << std::setw(2) << i << "): "
                            << "RC	hole in (iX,iY)	= (" << iX << "," << iY
                            << ") | value reset at " << RC(i, iX, iY) << std::endl;
            }
            v_sides.clear();
         } // iY
      } // iX
   }
}

/* Look Up Tables for XP method
 * ------------------------------------------------------------------------------------------------------------------
 */

float Reconstruction::LUT::LUTValues[Reconstruction::LUT::SNSTEPS_TRANS]
                                    [Reconstruction::LUT::SNSTEPS_RC]
                                    [Reconstruction::LUT::SNSTEPS_DRIFT]
                                    [Reconstruction::LUT::SNSTEPS_IMPACT]
                                    [Reconstruction::LUT::SNSTEPS_PHI];

// Default constructor
Reconstruction::LUT::LUT(const int &transDiffCoeffB, const int &transDiffCoeffnoB,
                         const int &peakingTime)
{
   DtwithB = transDiffCoeffB;
   DtwithoutB = transDiffCoeffnoB;
   stepSizeTrans = fabs(DtwithB - DtwithoutB);
   std::cout << "dEdx LUT: Dt with B = " << DtwithB
             << " | Dt without B = " << DtwithoutB
             << " | step size = " << stepSizeTrans << std::endl;
   fFile_LUT = Form("~/Documents/Code/CPP/CPP_Projects/LUT_Maker/Output_LUT/"
                    "dEdx_XP_LUT_Dt%d_%d_PT%d.root",
                    transDiffCoeffB, transDiffCoeffnoB, peakingTime);
   std::cout << "dEdx LUT: LOADING " << fFile_LUT << std::endl;
   Load();
   std::cout << "dEdx LUT: LOADED" << std::endl;
}

Reconstruction::LUT::LUT(const std::string &file)
{
   fFile_LUT = file;
   std::cout << "dEdx LUT: LOADING " << fFile_LUT << std::endl;
   Load();
   std::cout << "dEdx LUT: LOADED" << std::endl;
}

// Destructor
Reconstruction::LUT::~LUT()
{
   pFile_LUT->Close();
   delete pFile_LUT;
}

// Load the LUTs
void Reconstruction::LUT::Load()
{

   // Initializing all values to 0
   for (int i = 0; i < SNSTEPS_TRANS; i++)
      for (int j = 0; j < SNSTEPS_RC; j++)
         for (int k = 0; k < SNSTEPS_DRIFT; k++)
            for (int l = 0; l < SNSTEPS_IMPACT; l++)
               for (int m = 0; m < SNSTEPS_PHI; m++)
                  LUTValues[i][j][k][l][m] = 0;

   pFile_LUT = TFile::Open(fFile_LUT.c_str(), "READ");
   TTree &ptree = *(TTree *)pFile_LUT->Get("treeLUTdEdx");
   ptree.SetBranchAddress("scalefactor", &fweight);
   ptree.SetBranchAddress("transvDiff", &fDt);
   ptree.SetBranchAddress("RC", &fRC);
   ptree.SetBranchAddress("drift", &fdrift);
   ptree.SetBranchAddress("angle", &fphi);
   ptree.SetBranchAddress("impact", &fd);

   int nentries = ptree.GetEntries();
   int iDt, iRC, idrift, id, iphi;
   for (int i = 0; i < nentries; i++) {
      ptree.GetEntry(i);
      iDt = (int)std::round((fDt * pow(10, 3.5) - DtwithB) / stepSizeTrans);
      iRC = (int)std::round((fRC - 112) / sSTEP_RC);
      idrift = (int)std::round(fdrift / sSTEP_DRIFT);
      id = (int)std::round(fd / sSTEP_IMPACT);
      iphi = (int)std::round((fphi - 1e-6) / sSTEP_PHI);
      LUTValues[iDt][iRC][idrift][id][iphi] = fweight;
   }
}

float Reconstruction::LUT::getRatio(const int &Dt, const int &RC, const float &drift,
                                    const float &impact, const float &angle)
{
   // keep double
   int itransvDiff = (int)(Dt - DtwithB) / stepSizeTrans;
   int iRC = (int)(RC - 112) / sSTEP_RC;
   float idrift = drift / sSTEP_DRIFT;
   float idrift_min =
      std::min((double)std::floor(drift / sSTEP_DRIFT), (double)SNSTEPS_DRIFT - 1);
   float idrift_max = std::max((double)std::ceil(drift / sSTEP_DRIFT), 0.);
   float iphi = angle / sSTEP_PHI;
   float iphi_min =
      std::min((double)std::floor(angle / sSTEP_PHI), (double)SNSTEPS_PHI - 1);
   float iphi_max = std::max((double)std::ceil(angle / sSTEP_PHI), 0.);
   float id = impact / sSTEP_IMPACT;
   float id_min =
      std::min((double)std::floor(impact / sSTEP_IMPACT), (double)SNSTEPS_IMPACT - 1);
   float id_max = std::max((double)std::ceil(impact / sSTEP_IMPACT), 0.);

   // weights
   double w_drift, w_d, w_phi;
   if (idrift_min == idrift_max)
      w_drift = 1;
   else
      w_drift = 1 - (idrift - idrift_min) / (idrift_max - idrift_min);
   if (id_min == id_max)
      w_d = 1;
   else
      w_d = 1 - (id - id_min) / (id_max - id_min);
   if (iphi_min == iphi_max)
      w_phi = 1;
   else
      w_phi = 1 - (iphi - iphi_min) / (iphi_max - iphi_min);

   // Interpolation
   float factor = 0;
   factor += w_drift * w_d * w_phi *
             LUTValues[itransvDiff][iRC][(int)idrift_min][(int)id_min][(int)iphi_min];
   factor += w_drift * w_d * (1 - w_phi) *
             LUTValues[itransvDiff][iRC][(int)idrift_min][(int)id_min][(int)iphi_max];
   factor += w_drift * (1 - w_d) * w_phi *
             LUTValues[itransvDiff][iRC][(int)idrift_min][(int)id_max][(int)iphi_min];
   factor += w_drift * (1 - w_d) * (1 - w_phi) *
             LUTValues[itransvDiff][iRC][(int)idrift_min][(int)id_max][(int)iphi_max];
   factor += (1 - w_drift) * w_d * w_phi *
             LUTValues[itransvDiff][iRC][(int)idrift_max][(int)id_min][(int)iphi_min];
   factor += (1 - w_drift) * w_d * (1 - w_phi) *
             LUTValues[itransvDiff][iRC][(int)idrift_max][(int)id_min][(int)iphi_max];
   factor += (1 - w_drift) * (1 - w_d) * w_phi *
             LUTValues[itransvDiff][iRC][(int)idrift_max][(int)id_max][(int)iphi_min];
   factor += (1 - w_drift) * (1 - w_d) * (1 - w_phi) *
             LUTValues[itransvDiff][iRC][(int)idrift_max][(int)id_max][(int)iphi_max];

   return factor;
}