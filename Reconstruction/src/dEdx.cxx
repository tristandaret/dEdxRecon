#include "dEdx.h"
#include "LUTs.h"
#include "ReconTools.h"
#include "SignalTools.h"
#include "Variables.h"
#include "Misc_Functions.h"

#include "Util.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "PRFParameters.h"
#include "TrackRecon.h"
#include "ClusterFitter.h"
#include "TrackFitter.h"
#include "Displays.h"
#include "GiveMe_Uploader.h"
#include "Selector.h"
#include <chrono>

ClassImp(Reconstruction::RecoPad);
ClassImp(Reconstruction::RecoCluster);
ClassImp(Reconstruction::RecoModule);
ClassImp(Reconstruction::RecoEvent);

Reconstruction::dEdx::dEdx() {}

Reconstruction::dEdx::~dEdx() {}

void Reconstruction::dEdx::Reconstruction()
{

   // Redirect Output
   std::cout << std::setprecision(2) << std::fixed;
   std::cout << std::endl;
   std::streambuf *coutbuf = std::cout.rdbuf();  // Save old buf
   std::ofstream OUT_DataFile(log_file.c_str()); // Set output file
   std::cout.rdbuf(OUT_DataFile.rdbuf());        // Redirect to output file

   // Get ERAM maps
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
   pERAMMaps = new Reconstruction::ERAMMaps();

   // Diagonal clustering?
   diag = false;
   if (tag.find("diag") != std::string::npos)
      diag = true;

   // Handle theta case
   costheta = 1;
   v_theta = {-20, 20, 45};
   for (int theta_file : v_theta)
      if (tag.find("theta") != std::string::npos and
          tag.find(std::to_string(theta_file)) != std::string::npos)
         costheta = fabs(cos((float)theta_file / 180 * M_PI));

   /////////////////////////////////////////////////////////////////////////////////////////////////////////

   // Selection stage
   Selector aSelector(selectionSet);
   NEvents = p_uploader->Get_NberOfEvent();
   NEvents = 1000;
   Init_selection(selectionSet, aSelector, tag, p_uploader, moduleCase, 0);
   aSelector.Tell_Selection();

   // Log info
   std::cout << "dataFile:			  " << v_datafiles.back() << std::endl;
   std::cout << "drawoutScanPath:   " << drawoutScanPath << std::endl;
   std::cout << "root output file:   " << rootout_file << std::endl;
   std::cout << "tag:                " << tag << std::endl;
   std::cout << "comment:            " << comment << std::endl;
   std::cout << "selectionSet:       " << selectionSet << std::endl;
   std::cout << "drift method:       " << fcorrectDrift << std::endl;
   std::cout << "WF    correction:   " << fcorrectWF << std::endl;
   std::cout << "Gain  correction:   " << fcorrectGain << std::endl;
   std::cout << "RC    correction:   " << fcorrectRC << std::endl;
   std::cout << "Save only selected: " << fsaveSelectOnly << std::endl;
   std::cout << std::endl;
   std::cout << "Number of entries:  " << NEvents << std::endl;
   std::cout << "drift               " << driftDist << " mm" << std::endl;
   std::cout << "Peaking time:       " << PT << " ns" << std::endl;
   std::cout << "Time bin length     " << TB << " ns" << std::endl;
   std::cout << "PT/TB =             " << PT / TB << " time bins" << std::endl;
   std::cout << "drift method:       " << fcorrectDrift << std::endl;
   std::cout << "minimal length      " << fminLength * 1e3 << " mm" << std::endl;
   std::cout << std::endl;

   // Correction function for WF
   pcorrFunctionWF = corr_func(v_datafiles.back(), tag, fcorrectWF);
   fAref = pcorrFunctionWF->Eval(XPADLENGTH);

   // Track fit initializations
   TrackFitter aTrackFitter("Minuit", fnParamsTrack);
   PRFParameters aPRFParameters;
   ptf1PRF = new TF1("ptf1PRF", aPRFParameters, -2.5 * 1.128, 2.5 * 1.128, 5);
   ptf1PRF->SetParameters(p_uploader->Get_Norm(), p_uploader->Get_a2(),
                          p_uploader->Get_a4(), p_uploader->Get_b2(),
                          p_uploader->Get_b4());
   fcounterFail = 0;
   fcounterFit = 0;

   // Initialization of analysis vectors and histograms
   waveform_cluster.assign(510, 0);
   waveform_pad.assign(510, 0);
   // dE/dx quick access histograms
   int xmax = 1300;
   if (tag.find("Theta") != std::string::npos)
      xmax = 2000;
   ph1f_WF = new TH1F("ph1f_WF", "<dE/dx> with WF;<dE/dx> (ADC count);Number of events",
                      100, 0, xmax);
   ph1f_XP = new TH1F("ph1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events",
                      100, 0, xmax);
   ph1f_GP1 = new TH1F(
      "ph1f_GP1", "<dE/dx> with GP1;<dE/dx> (ADC count);Number of events", 100, 0, xmax);
   ph1f_GP2 = new TH1F(
      "ph1f_GP2", "<dE/dx> with GP2;<dE/dx> (ADC count);Number of events", 100, 0, xmax);
   ph1f_GP3 = new TH1F(
      "ph1f_GP3", "<dE/dx> with GP3;<dE/dx> (ADC count);Number of events", 100, 0, xmax);
   ph1f_GP4 = new TH1F(
      "ph1f_GP4", "<dE/dx> with GP4;<dE/dx> (ADC count);Number of events", 100, 0, xmax);
   ph1f_GP5 = new TH1F(
      "ph1f_GP5", "<dE/dx> with GP5;<dE/dx> (ADC count);Number of events", 100, 0, xmax);

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Compute dE/dx
   aSelector.Reset_StatCounters();
   std::cout << "Processing events:" << std::endl;

   // GP variables
   std::vector<TH1F> vPadWaveforms;
   std::vector<TH1F> vClusterWaveforms;
   std::vector<TH1F> vCrossedWaveforms;
   std::vector<float> v_dEdxCluster;
   std::vector<float> v_dEdxClusterGP3;
   std::vector<float> v_dEdxClusterGP4;
   std::vector<float> v_dEdxCrossedGP5;
   std::vector<float> v_Aclus;    // amplitude of cluster in ADC counts
   std::vector<float> v_Lclus;    // length in clusters in cm
   std::vector<float> v_Lcrossed; // length in crossed pads in cm
   std::vector<float> v_Alead;    // amplitude of leading pads in ADC counts
   std::vector<float> v_AleadGP3; // amplitude of leading pads in ADC counts
   std::vector<float> v_AleadGP4; // amplitude of leading pads in ADC counts
   TH2F *pth2f_paramGP3 = nullptr;
   TF1 *ptf1_paramGP3 = nullptr;

   std::string GP3paramPath =
      v_datafiles.back().substr(0, v_datafiles.back().length() - 5) + "_GP3param.root";
   TFile *testGP3param = TFile::Open(GP3paramPath.c_str(), "READ");
   if (testGP3param && !testGP3param->IsZombie()) {
      ptf1_paramGP3 = testGP3param->Get<TF1>("ptf1_paramGP3");
      pth2f_paramGP3 = testGP3param->Get<TH2F>("pth2f_paramGP3");
      pth2f_paramGP3->SetDirectory(
         fpFile_dEdx); // avoids issue when writing in my output file
      testGP3param->Close();
   } else {
      testGP3param = TFile::Open(GP3paramPath.c_str(), "RECREATE");
      pth2f_paramGP3 = new TH2F("pth2f_paramGP3", "GP3 Parameters;Parameter;Value", 100,
                                -1, 1, 100, 1, 2.5);
      std::cout << "Loop of shame for GP3 parametrisation" << std::endl;
      for (iEvent = 0; iEvent < NEvents; iEvent++) {
         if (iEvent % 1000 == 0 or iEvent == NEvents - 1)
            std::cout << iEvent << "/" << NEvents << std::endl;

         pEvent = p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
         if (!pEvent)
            continue;

         aSelector.ApplySelection(pEvent);
         if (pEvent->IsValid() == 0) {
            delete pEvent;
            continue;
         }

         // Module loop
         NMod = pEvent->Get_NberOfModule();
         for (iMod = 0; iMod < NMod; iMod++) {
            pModule = pEvent->Get_Module_InArray(iMod);
            fmodID = pModule->Get_ModuleNber();
            if (pEvent->Validity_ForThisModule(fmodID) == 0) {
               if (!fsaveSelectOnly)
                  DiscardedModule();
               continue;
            }
            NClusters = pModule->Get_NberOfCluster();
            // Track fitting
            if (!diag) {
               ClusterFitter_Horizontal aClusterFitter_Horizontal("Minuit");
               ClusterFit_Horizontal_Event(pEvent, fmodID, ptf1PRF,
                                           aClusterFitter_Horizontal);
            } else {
               ClusterFitter_Diagonal aClusterFitter_Diagonal("Minuit");
               ClusterFit_Diagonal_Event(-(M_PI_2 - (PHIMAX * M_PI / 180)), pEvent,
                                         fmodID, ptf1PRF, fcounterFit, fcounterFail,
                                         aClusterFitter_Diagonal);
            }
            TrackFitter aTrackFitter("Minuit", fnParamsTrack);
            TrackRecon_Event(aTrackFitter, pEvent, fmodID, fnParamsTrack);

            // Loop On Clusters
            for (iC = 0; iC < NClusters; iC++) {
               pCluster = pModule->Get_Cluster(iC);
               float Aclus = pCluster->Get_Acluster();
               float Alead = pCluster->Get_AMaxLeading();
               float Lclus = 0;
               float ytrack = pCluster->Get_YTrack() * 100;  // in cm
               float ylead = pCluster->Get_YLeading() * 100; // in cm
               pth2f_paramGP3->Fill(ytrack - ylead, Aclus / Alead);
            }
         }
      }
      TGraphErrors *ptge_paramGP3 = Convert_TH2_TGE(pth2f_paramGP3);
      ptf1_paramGP3 = new TF1("ptf1_paramGP3", "[0] + [1]*x*x + [2]*pow(x,4)", -0.6, 0.6);
      ptge_paramGP3->Fit(ptf1_paramGP3, "R");
      delete ptge_paramGP3;
      TFile *ptfileGP3param = new TFile(GP3paramPath.c_str(), "RECREATE");
      ptf1_paramGP3->Write();
      pth2f_paramGP3->Write();
      ptfileGP3param->Close();
   }

   // Output
   fpFile_dEdx = new TFile(rootout_file.c_str(), "RECREATE");
   fpTree_dEdx = new TTree("dEdx_tree", "dEdx TTree");
   p_recoevent = new Reconstruction::RecoEvent();
   fpTree_dEdx->Branch("event_branch", "Reconstruction::RecoEvent", &p_recoevent);

   // Event loop
   for (iEvent = 0; iEvent < NEvents; iEvent++) {
      // if (iEvent % 1000 == 0 or iEvent == NEvents - 1)
      std::cout << iEvent << "/" << NEvents;

      pEvent = p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
      if (!pEvent)
         continue;

      p_recoevent->eventNbr = iEvent;
      NMod = pEvent->Get_NberOfModule();
      p_recoevent->numberOfModules = NMod;

      aSelector.ApplySelection(pEvent);
      if (pEvent->IsValid() == 0) {
         if (!fsaveSelectOnly) {
            for (iMod = 0; iMod < NMod; iMod++) {
               pModule = pEvent->Get_Module_InArray(iMod);
               fmodID = pModule->Get_ModuleNber();
               DiscardedModule();
            }
            fpTree_dEdx->Fill();
         }
         p_recoevent->Clear();
         delete pEvent;
         std::cout << " - Discarded" << std::endl;
         continue;
      }
      p_recoevent->selected = true;
      auto start_time = std::chrono::high_resolution_clock::now();

      // Cluster display
      if (iEvent < 1) {
         std::string clusterDrawOutput = drawoutRunPath + "ClusterDisplay";
         MakeMyDir(clusterDrawOutput);
         NewClusterDisplay(pEvent, clusterDrawOutput, tag, PT, TB);
         NewClusterDisplayMinimal(pEvent, clusterDrawOutput, tag);
      }

      // Reset GP variables
      p_recoevent->peakingTime = PT;
      p_recoevent->timeBinSize = TB;
      vPadWaveforms.clear();
      vClusterWaveforms.clear();
      vCrossedWaveforms.clear();
      v_dEdxCluster.clear();
      v_dEdxClusterGP3.clear();
      v_dEdxClusterGP4.clear();
      v_dEdxCrossedGP5.clear();
      v_Aclus.clear();
      v_Lclus.clear();
      v_Lcrossed.clear();
      v_Alead.clear();
      v_AleadGP3.clear();
      v_AleadGP4.clear();
      int nEventClusters = 0;
      int nEventCrossedPads = 0;
      float Levent = 0; // length of the event in m

      // Module loop
      for (iMod = 0; iMod < NMod; iMod++) {
         pModule = pEvent->Get_Module_InArray(iMod);
         fmodID = pModule->Get_ModuleNber();
         if (pEvent->Validity_ForThisModule(fmodID) == 0) {
            if (!fsaveSelectOnly)
               DiscardedModule();
            continue;
         }

         NClusters = pModule->Get_NberOfCluster();

         p_recomodule = new Reconstruction::RecoModule();
         p_recomodule->position = fmodID;
         p_recoevent->v_modules_position.push_back(fmodID);
         p_recomodule->ID = fERAMs_iD[fmodID];
         p_recomodule->selected = true;

         // Track fitting
         if (!diag) {
            ClusterFitter_Horizontal aClusterFitter_Horizontal("Minuit");
            ClusterFit_Horizontal_Event(pEvent, fmodID, ptf1PRF,
                                        aClusterFitter_Horizontal);
         } else {
            ClusterFitter_Diagonal aClusterFitter_Diagonal("Minuit");
            ClusterFit_Diagonal_Event(-(M_PI_2 - (PHIMAX * M_PI / 180)), pEvent, fmodID,
                                      ptf1PRF, fcounterFit, fcounterFail,
                                      aClusterFitter_Diagonal);
         }
         TrackFitter aTrackFitter("Minuit", fnParamsTrack);
         TrackRecon_Event(aTrackFitter, pEvent, fmodID, fnParamsTrack);

         // Track details
         pTrack = pEvent->GiveMe_Track_ForThisModule(fmodID);
         p_recomodule->phi = std::atan(pTrack->Get_ParameterValue(1)) / M_PI * 180;
         p_recomodule->Track->SetParameters(pTrack->Get_ParameterValue(0),
                                            pTrack->Get_ParameterValue(1));
         Double_t parErrors[2] = {pTrack->Get_ParameterError(0),
                                  pTrack->Get_ParameterError(1)};
         p_recomodule->Track->SetParErrors(parErrors);
         if (fnParamsTrack == 3) {
            p_recomodule->Track->SetParameter(2, pTrack->Get_ParameterValue(2));
            p_recomodule->Track->SetParError(2, pTrack->Get_ParameterError(2));
         }

         // Loop On Clusters
         for (iC = 0; iC < NClusters; iC++) {
            std::fill(waveform_cluster.begin(), waveform_cluster.end(),
                      0); // reset waveform
            pCluster = pModule->Get_Cluster(iC);
            p_recocluster = new Reconstruction::RecoCluster();
            p_recocluster->ADCmax_base = pCluster->Get_Acluster(); // should not trust it
            p_recocluster->ALead_base = pCluster->Get_AMaxLeading();
            p_recocluster->TLead = pCluster->Get_TMaxLeading();
            p_recocluster->yCluster = pCluster->Get_YTrack() * 100; // in cm
            p_recocluster->yWeight = pCluster->Get_YWeight() * 100; // in cm

            // GP variables
            float Lclus = 0;
            float rhoGP4 = 0;

            // Loop On Pads
            int NPads = pCluster->Get_NberOfPads();
            p_recocluster->NPads = NPads;
            for (iP = 0; iP < NPads; iP++) {
               std::fill(waveform_pad.begin(), waveform_pad.end(), 0); // reset waveform
               pPad = pCluster->Get_Pad(iP);
               p_recopad = new Reconstruction::RecoPad();
               if (iP == NPads - 1)
                  p_recopad->leading = true;
               std::vector<int> v_int = pPad->Get_vADC();
               std::vector<float> waveform_pad(
                  v_int.begin(), v_int.end()); // Implicit conversion from int to float
               p_recopad->ix = pPad->Get_iX();
               p_recopad->iy = pPad->Get_iY();
               p_recopad->xPad = pPad->Get_XPad() * 100; // in cm
               p_recopad->yPad = pPad->Get_YPad() * 100; // in cm
               p_recopad->dy = p_recocluster->yCluster - pPad->Get_YPad() * 100;

               // RC correction
               switch (fcorrectRC) {
               case 0: // no RC correction
                  p_recopad->RC = 170;
                  break;
               case 1: // pad per pad RC correction
                  p_recopad->RC =
                     pERAMMaps->RC(fERAMs_pos[fmodID], p_recopad->ix, p_recopad->iy);
                  break;
               case 2: // ERAM per ERAM correction
                  p_recopad->RC = pERAMMaps->MeanRC(fERAMs_pos[fmodID]);
                  break;
               }

               // Gain correction
               switch (fcorrectGain) {
               case 0: // no gain correction
                  p_recopad->GainCorrection = 1.0;
                  break;
               case 1: // pad per pad gain correction
                  p_recopad->gain =
                     pERAMMaps->Gain(fERAMs_pos[fmodID], p_recopad->ix, p_recopad->iy);
                  p_recopad->GainCorrection = AVG_GAIN / p_recopad->gain;
                  for (int i = 0; i < 510; i++)
                     waveform_pad[i] = waveform_pad[i] * p_recopad->GainCorrection;
                  if (iP == 0)
                     p_recocluster->ALead_GCorr = p_recopad->GainCorrection;
                  break;
               case 2: // ERAM per ERAM correction
                  p_recopad->GainCorrection =
                     AVG_GAIN / pERAMMaps->MeanGain(fERAMs_pos[fmodID]);
                  for (int i = 0; i < 510; i++)
                     waveform_pad[i] = waveform_pad[i] * p_recopad->GainCorrection;
                  if (iP == 0)
                     p_recocluster->ALead_GCorr = p_recopad->GainCorrection;
                  break;
               }
               p_recopad->ADCmax_base = pPad->Get_AMax();
               p_recopad->ADCmax = p_recopad->GainCorrection * pPad->Get_AMax();
               for (int i = 0; i < 510; i++)
                  waveform_cluster[i] += waveform_pad[i];

               // Track computations
               local_params(pPad, pTrack, p_recopad->d, p_recopad->dd, p_recopad->phi,
                            p_recopad->length); // impact parameter and length in pad in
                                                // m, angle in degrees
               p_recopad->d *= 1000;            // in mm
               p_recopad->dd *= 1000;           // in mm
               p_recopad->length /= costheta;
               p_recocluster->length += p_recopad->length;

               // GP variables
               Levent += p_recopad->length; // length in pads in cm
               TH1F hPadWF(Form("PadWF_mod%d_clu%d_pad%d", iMod, iC, iP), "Pad waveform",
                           waveform_pad.size(), 0, waveform_pad.size());
               for (size_t ibin = 0; ibin < waveform_pad.size(); ++ibin) {
                  hPadWF.SetBinContent(ibin + 1, waveform_pad[ibin]);
               }
               vPadWaveforms.push_back(hPadWF); // add after gain correction
               Lclus += p_recopad->length;      // length in pad in m

               // Compute drift distance for different time bin sizes and peaking times
               p_recopad->TMax = pPad->Get_TMax();
               if (PT == 412 and TB == 50) {
                  p_recopad->T0 = 45;
                  p_recopad->driftDistance = 3.90 * (p_recopad->TMax - p_recopad->T0);
               } // 45 =	37(time shift) +	8(PT) from David Attié
               else if (PT == 412 and TB == 40) {
                  p_recopad->T0 = 56;
                  p_recopad->driftDistance = 3.12 * (p_recopad->TMax - p_recopad->T0);
               } // 56 =	46(time shift) + 10(PT)
               else if (PT == 200 and TB == 50) {
                  p_recopad->T0 = 39;
                  p_recopad->driftDistance = 3.90 * (p_recopad->TMax - p_recopad->T0);
               } // 39 =	35(time shift) +	4(PT) own computation
               else if (PT == 200 and TB == 40) {
                  p_recopad->T0 = 48;
                  p_recopad->driftDistance = 3.12 * (p_recopad->TMax - p_recopad->T0);
               } // 48 =	44(time shift) +	5(PT)
               p_recopad->driftDistance =
                  std::max(p_recopad->driftDistance,
                           (float)0.0); // avoid negative drift distance

               if (iP == NPads - 1) {
                  float halfdiag =
                     fabs(sqrt(XPADLENGTH * XPADLENGTH + YPADLENGTH * YPADLENGTH) / 2);
                  float impactGP4 = std::min(halfdiag, p_recopad->d);
                  rhoGP4 = p_lut->getRatio(Dt, p_recopad->RC, p_recopad->driftDistance,
                                           fabs(impactGP4), fabs(p_recopad->phi));
               }

               // Min length in pad to avoid fluctuations from small segments
               if (p_recopad->length * costheta <=
                   fminLength) { // cutoff defined in the ERAM's plane -> remove theta
                                 // dependence
                  p_recocluster->v_pads.push_back(p_recopad);
                  continue;
               }

               // Get the LUT ratio
               p_recopad->ratioDrift =
                  p_lut->getRatio(Dt, p_recopad->RC, p_recopad->driftDistance,
                                  fabs(p_recopad->d), fabs(p_recopad->phi));
               p_recopad->ratioFile = p_lut->getRatio(
                  Dt, p_recopad->RC, driftDist, fabs(p_recopad->d), fabs(p_recopad->phi));

               if (fcorrectDrift == 1)
                  p_recopad->ratio = p_recopad->ratioDrift;
               else
                  p_recopad->ratio = p_recopad->ratioFile;

               p_recopad->charge = p_recopad->ADCmax * p_recopad->ratio;
               p_recopad->dEdxXP = p_recopad->charge / p_recopad->length / 100; // m to cm
               v_mod_dE.push_back(p_recopad->charge);
               v_mod_dx.push_back(p_recopad->length);
               v_mod_dEdxXP.push_back(p_recopad->dEdxXP);

               p_recocluster->v_pads.push_back(p_recopad);
               p_recomodule->NCrossedPads++;
               p_recomodule->lengthXP += p_recopad->length;

               // GP5
               nEventCrossedPads++;
               TH1F hCrossedWF(Form("CrossedWF_mod%d_clu%d_pad%d", iMod, iC, iP),
                               "Crossed pad waveform", waveform_pad.size(), 0,
                               waveform_pad.size());
               for (size_t ibin = 0; ibin < waveform_pad.size(); ++ibin) {
                  hCrossedWF.SetBinContent(ibin + 1, waveform_pad[ibin]);
               }
               vCrossedWaveforms.push_back(hCrossedWF); // add after gain correction
            } // Pads

            // Gain correction for the whole cluster (doesn't work really well)

            // WF correction function
            p_recocluster->charge =
               *std::max_element(waveform_cluster.begin(), waveform_cluster.end());
            p_recocluster->ratioCorr =
               fAref / pcorrFunctionWF->Eval(p_recocluster->length * 1000);
            if (diag)
               p_recocluster->dEdxWF =
                  p_recocluster->charge * p_recocluster->ratioCorr / XPADLENGTH * 10;
            else
               p_recocluster->dEdxWF =
                  p_recocluster->charge / (p_recocluster->length * 100);
            v_mod_dEdxWF.push_back(p_recocluster->dEdxWF);

            // Fill cluster information
            p_recomodule->v_clusters.push_back(p_recocluster);
            p_recomodule->NClusters++;
            p_recomodule->lengthWF += p_recocluster->length;
            p_recomodule->NPads += p_recocluster->NPads;

            // GP variables
            float rhoGP3 = ptf1_paramGP3->Eval(
               (pCluster->Get_YTrack() - pCluster->Get_YLeading()) * 100);
            float dE_GP3 = pCluster->Get_AMaxLeading() * rhoGP3;
            float dE_GP4 = pCluster->Get_AMaxLeading() * rhoGP4;
            v_Alead.push_back(pCluster->Get_AMaxLeading());
            v_AleadGP3.push_back(dE_GP3);
            v_AleadGP4.push_back(dE_GP4);
            v_Aclus.push_back(p_recocluster->charge);
            v_Lclus.push_back(Lclus); // length in cm
            v_dEdxCluster.push_back(p_recocluster->charge / (Lclus * 100));
            v_dEdxClusterGP3.push_back(dE_GP3 / (Lclus * 100)); // dE/dx in ADC count
            v_dEdxClusterGP4.push_back(dE_GP4 / (Lclus * 100)); // dE/dx in ADC count
            TH1F hClusterWF(Form("ClusterWF_mod%d_clu%d", iMod, iC), "Cluster waveform",
                            waveform_cluster.size(), 0, waveform_cluster.size());
            for (size_t ibin = 0; ibin < waveform_cluster.size(); ++ibin) {
               hClusterWF.SetBinContent(ibin + 1, waveform_cluster[ibin]);
            }
            vClusterWaveforms.push_back(hClusterWF); // add after gain correction

         } // Clusters

         // Fill module class attributes
         p_recomodule->avg_pad_mult =
            (float)p_recomodule->NPads / p_recomodule->NClusters;

         // Compute dE/dx for the module
         p_recomodule->dEdxWF =
            ComputedEdxWF(v_mod_dEdxWF, p_recomodule->NClusters, falpha);
         p_recomodule->dEdxXP = ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx,
                                              p_recomodule->NCrossedPads, falpha);
         p_recomodule->dEdxWFnoTrunc =
            ComputedEdxWF(v_mod_dEdxWF, p_recomodule->NClusters, 1);
         p_recomodule->dEdxXPnoTrunc = ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx,
                                                     p_recomodule->NCrossedPads, 1);

         // Fill module information into the event class
         p_recoevent->v_modules.push_back(p_recomodule);
         p_recoevent->NCrossedPads += p_recomodule->NCrossedPads;
         p_recoevent->NPads += p_recomodule->NPads;
         p_recoevent->NClusters += p_recomodule->NClusters;
         p_recoevent->lengthXP += p_recomodule->lengthXP;
         p_recoevent->lengthWF += p_recomodule->lengthWF;
         v_evt_dE.insert(v_evt_dE.end(), v_mod_dE.begin(), v_mod_dE.end());
         v_evt_dx.insert(v_evt_dx.end(), v_mod_dx.begin(), v_mod_dx.end());
         v_evt_dEdxXP.insert(v_evt_dEdxXP.end(), v_mod_dEdxXP.begin(),
                             v_mod_dEdxXP.end());
         v_evt_dEdxWF.insert(v_evt_dEdxWF.end(), v_mod_dEdxWF.begin(),
                             v_mod_dEdxWF.end());

         // clear module vectors
         v_mod_dE.clear();
         v_mod_dx.clear();
         v_mod_dEdxXP.clear();
         v_mod_dEdxWF.clear();
      } // Modules

      // Fill event class attributes
      p_recoevent->avg_pad_mult = (float)p_recoevent->NPads / p_recoevent->NClusters;

      // Fill Giga Waveforms
      p_recoevent->GWF = GetGigaWaveform(vClusterWaveforms);
      p_recoevent->GWFtruncatedGP1 = GetTruncatedGigaWaveformGP1(
         vClusterWaveforms, v_dEdxCluster, p_recoevent->NClusters);

      // Compute dE/dx for the event
      p_recoevent->dEdxGP1 = ComputedEdxGP1(vClusterWaveforms, v_dEdxCluster, v_Aclus,
                                            v_Lclus, p_recoevent->NClusters, falpha);
      p_recoevent->dEdxGP2 =
         ComputedEdxGP(p_recoevent->GWF, vClusterWaveforms, v_dEdxCluster, v_Aclus,
                       Levent, v_Lclus, p_recoevent->NClusters, falpha);
      p_recoevent->dEdxGP3 =
         ComputedEdxGP(p_recoevent->GWF, vClusterWaveforms, v_dEdxClusterGP3, v_AleadGP3,
                       Levent, v_Lclus, p_recoevent->NClusters, falpha);
      p_recoevent->dEdxGP4 =
         ComputedEdxGP(p_recoevent->GWF, vClusterWaveforms, v_dEdxClusterGP4, v_AleadGP4,
                       Levent, v_Lclus, p_recoevent->NClusters, falpha);
      p_recoevent->dEdxGP5 =
         ComputedEdxGP(p_recoevent->GWF, vCrossedWaveforms, v_evt_dEdxXP, v_evt_dE,
                       Levent, v_evt_dx, p_recoevent->NCrossedPads, falpha);

      p_recoevent->dEdxWF = ComputedEdxWF(v_evt_dEdxWF, p_recoevent->NClusters, falpha);
      p_recoevent->dEdxXP = ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx,
                                          p_recoevent->NCrossedPads, falpha);
      p_recoevent->dEdxWFnoTrunc = ComputedEdxWF(v_evt_dEdxWF, p_recoevent->NClusters, 1);
      p_recoevent->dEdxXPnoTrunc =
         ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx, p_recoevent->NCrossedPads, 1);

      // Print dE/dx results
      // std::cout << "WF: " << p_recoevent->dEdxWF << " | XP: " << p_recoevent->dEdxXP
      //           << " | GP1: " << p_recoevent->dEdxGP1 << " | GP2: " <<
      //           p_recoevent->dEdxGP2 << " | GP3: " << p_recoevent->dEdxGP3
      //           << std::endl;

      // Make the quick access histograms
      if (testbeam.find("CERN22") == std::string::npos ||
          (testbeam.find("CERN22") != std::string::npos and
           FourModulesInLine(p_recoevent->v_modules_position))) {
         ph1f_WF->Fill(p_recoevent->dEdxWF);
         ph1f_XP->Fill(p_recoevent->dEdxXP);
         ph1f_GP1->Fill(p_recoevent->dEdxGP1);
         ph1f_GP2->Fill(p_recoevent->dEdxGP2);
         ph1f_GP3->Fill(p_recoevent->dEdxGP3);
         ph1f_GP4->Fill(p_recoevent->dEdxGP4);
      }

      // Fill the tree
      fpTree_dEdx->Fill();

      // Clear event vectors
      v_evt_dE.clear();
      v_evt_dx.clear();
      v_evt_dEdxXP.clear();
      v_evt_dEdxWF.clear();
      p_recoevent->Clear();
      delete pEvent;
      auto end_time = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> ms = (end_time - start_time) * 1e3;
      std::cout << " => " << std::setprecision(2) << std::fixed << ms.count()
                << " milliseconds" << std::endl;
   } // Events

   aSelector.PrintStat();

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   // dE/dx histograms fitting
   std::cout << "Fitting dE/dx histograms... ";
   Fit1Gauss(ph1f_WF, 2);
   Fit1Gauss(ph1f_XP, 2);
   Fit1Gauss(ph1f_GP1, 2);
   Fit1Gauss(ph1f_GP2, 2);
   Fit1Gauss(ph1f_GP3, 2);
   Fit1Gauss(ph1f_GP4, 2);
   std::cout << "done!" << std::endl;

   // Save
   ph1f_WF->Write();
   ph1f_XP->Write();
   ph1f_GP1->Write();
   ph1f_GP2->Write();
   ph1f_GP3->Write();
   ph1f_GP4->Write();

   pth2f_paramGP3->Write();
   ptf1_paramGP3->Write();
   fpTree_dEdx->Write();
   fpFile_dEdx->Close();

   // Deleting
   delete fpFile_dEdx;
   delete pcorrFunctionWF;
   delete pERAMMaps;
   delete ptf1PRF;
   std::cout.rdbuf(coutbuf); // Reset to standard output
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* dE/dx RECONSTRUCTION ALGORITHMS */

float Reconstruction::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF,
                                          const int &NClusters, const float &alpha)
{
   float NClustersTrunc = int(floor(NClusters * alpha));
   std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
   return std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + NClustersTrunc, 0.) /
          NClustersTrunc;
}

float Reconstruction::dEdx::ComputedEdxXP(const std::vector<float> &v_dEdx,
                                          const std::vector<float> &v_dE,
                                          const std::vector<float> &v_dx,
                                          const int &nCrossedPads, const float &alpha)
{
   /// dEdx =	sum(dE)/Sum(dx) and not average(dE/dx) of each pad = > less sensitive to
   /// statistical fluctuations
   float NCrossedTrunc = int(floor(nCrossedPads * alpha));
   float DE = 0, DX = 0;

   // Few steps to order v_dE & v_dx similarly to v_dEdx
   std::vector<int> indices(v_dEdx.size());
   std::iota(indices.begin(), indices.end(), 0); // make list from 0 to v_dEdx.size()

   std::sort(indices.begin(), indices.end(), [&](int i, int j) { // sorting wrt v_dEdx
      return v_dEdx[i] < v_dEdx[j];
   });
   for (int i = 0; i < NCrossedTrunc; ++i) {
      DE += v_dE[indices[i]];
      DX += v_dx[indices[i]] * 100; // m->cm normalization
   }
   return DE / DX;
}

float Reconstruction::dEdx::ComputedEdxGP1(const std::vector<TH1F> &vClusWF,
                                           const std::vector<float> &v_dEdx,
                                           const std::vector<float> &v_Aclus,
                                           const std::vector<float> &v_Lclus,
                                           const int &nClusters, const float &alpha)
{
   float nTruncatedClusters = int(floor(nClusters * alpha));

   // Prepare the GigaWaveform
   TH1F GWFtruncatedGP1(
      ("GWFtruncatedGP1" + std::to_string(iEvent) + std::to_string(iMod)).c_str(),
      "Truncated Giga Waveform GP1", 510, 0, 510);
   float LtruncatedTrack = 0;

   // Few steps to order v_Aclus & v_Lclus similarly to v_dEdx
   std::vector<int> indices(v_dEdx.size());
   std::iota(indices.begin(), indices.end(), 0); // make list from 0 to v_dEdx.size()
   std::sort(indices.begin(), indices.end(), [&](int i, int j) { // sorting wrt v_dEdx
      return v_dEdx[i] < v_dEdx[j];
   });

   for (int i = 0; i < nTruncatedClusters; ++i) {
      GWFtruncatedGP1.Add(&vClusWF[indices[i]]);
      LtruncatedTrack += v_Lclus[indices[i]];
   }

   int amplitudeGWF = GWFtruncatedGP1.GetMaximum();
   GWFtruncatedGP1.Delete();
   return amplitudeGWF / (LtruncatedTrack * 100); // dE/dx in ADC count/ cm
}

float Reconstruction::dEdx::ComputedEdxGP(const TH1F *GWF, const std::vector<TH1F> &vWF,
                                          const std::vector<float> &v_dEdx,
                                          const std::vector<float> &v_Amax,
                                          const float &eventLength,
                                          const std::vector<float> &v_Length,
                                          const int &nElements, const float &alpha)
{
   TH1F *localCopyGWF = dynamic_cast<TH1F *>(GWF->Clone("localCopyGWF"));
   float nElementsTruncated = int(floor(nElements * alpha));
   float LtruncatedTrack = eventLength;
   float TmaxGWF = localCopyGWF->GetMaximumBin();

   // Few steps to order v_Amax & v_Length similarly to v_dEdx
   std::vector<int> indices(v_dEdx.size());
   std::iota(indices.begin(), indices.end(), 0);
   std::sort(indices.begin(), indices.end(), [&](int i, int j) { // sorting wrt v_dEdx
      return v_dEdx[i] < v_dEdx[j];
   });

   // Start from the complete GigaWaveform and subtract top 30% ETFs
   for (int i = nElementsTruncated; i < nElements; ++i) {
      LtruncatedTrack -= v_Length[indices[i]];
      float Ascale = v_Amax[indices[i]];
      TH1F *etf = ETF("pTH1F_ETFtrunc_" + std::to_string(i) + std::to_string(iEvent) +
                         std::to_string(iMod),
                      0, 510, TmaxGWF - PT / TB, 510, 999, PT, TB);
      etf->Scale(Ascale);
      localCopyGWF->Add(etf, -1); // subtract ETF from the GigaWaveform
      delete etf;
   }

   int amplitudeGWF = localCopyGWF->GetMaximum();
   delete localCopyGWF;
   return amplitudeGWF / (LtruncatedTrack * 100); // dE/dx in ADC count/cm
}

TH1F *Reconstruction::dEdx::GetGigaWaveform(const std::vector<TH1F> &vClusWF)
{
   // Prepare the GigaWaveform
   TH1F *GWF =
      new TH1F(("getGWF" + std::to_string(iEvent) + std::to_string(iMod)).c_str(),
               "Giga Waveform", 510, 0, 510);
   for (int i = 0; i < vClusWF.size(); ++i) {
      GWF->Add(&vClusWF[i]);
   }
   return GWF;
}

TH1F *Reconstruction::dEdx::GetTruncatedGigaWaveformGP1(const std::vector<TH1F> &vClusWF,
                                                        const std::vector<float> &v_dEdx,
                                                        const int &nClusters)
{
   float nTruncatedClusters = int(floor(nClusters * 0.7));

   // Prepare the GigaWaveform
   TH1F *GWFtruncatedGP1 =
      new TH1F(("getGWFtruncGP1" + std::to_string(iEvent) + std::to_string(iMod)).c_str(),
               "Truncated Giga Waveform GP1", 510, 0, 510);
   // Few steps to order v_dEdx similarly to v_dEdx
   std::vector<int> indices(v_dEdx.size());
   std::iota(indices.begin(), indices.end(), 0); // make list from 0 to v_dEdx.size()
   std::sort(indices.begin(), indices.end(), [&](int i, int j) { // sorting wrt v_dEdx
      return v_dEdx[i] < v_dEdx[j];
   });

   for (int i = 0; i < nTruncatedClusters; ++i) {
      GWFtruncatedGP1->Add(&vClusWF[indices[i]]);
   }

   return GWFtruncatedGP1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Events discarded by selection steps
void Reconstruction::dEdx::DiscardedModule()
{
   NClusters = pModule->Get_NberOfCluster();

   p_recomodule = new Reconstruction::RecoModule();
   p_recomodule->position = fmodID;
   p_recomodule->ID = fERAMs_iD[fmodID];

   // Loop On Clusters
   for (iC = 0; iC < NClusters; iC++) {
      pCluster = pModule->Get_Cluster(iC);
      p_recocluster = new Reconstruction::RecoCluster();
      p_recocluster->ADCmax_base = pCluster->Get_Acluster();
      p_recocluster->ALead_base = pCluster->Get_AMaxLeading();
      p_recocluster->TLead = pCluster->Get_TMaxLeading();

      // Loop On Pads
      int NPads = pCluster->Get_NberOfPads();
      p_recocluster->NPads = NPads;
      for (iP = 0; iP < NPads; iP++) {
         pPad = pCluster->Get_Pad(iP);
         p_recopad = new Reconstruction::RecoPad();
         if (iP == 0)
            p_recopad->leading = true;
         p_recopad->ix = pPad->Get_iX();
         p_recopad->iy = pPad->Get_iY();
         p_recopad->ADCmax_base = pPad->Get_AMax();
         p_recopad->TMax = pPad->Get_TMax();

         p_recocluster->v_pads.push_back(p_recopad);
         p_recomodule->NCrossedPads++;

      } // Pads

      // Fill cluster information
      p_recomodule->v_clusters.push_back(p_recocluster);
      p_recomodule->NClusters++;
      p_recomodule->NPads += p_recocluster->NPads;

   } // Clusters

   // Fill module class attributes
   p_recomodule->avg_pad_mult = (float)p_recomodule->NPads / p_recomodule->NClusters;

   // Fill module information into the event class
   p_recoevent->v_modules.push_back(p_recomodule);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ANALYSIS CLASSES DEFINITION */

Reconstruction::RecoPad::~RecoPad()
{
   // nothing to clear
}

Reconstruction::RecoCluster::~RecoCluster()
{
   for (auto p_recopad : v_pads)
      delete p_recopad;
   v_pads.clear();
}

Reconstruction::RecoModule::~RecoModule()
{
   delete Track;
   for (auto p_recocluster : v_clusters)
      delete p_recocluster;
   v_clusters.clear();
}

Reconstruction::RecoEvent::RecoEvent() {}
Reconstruction::RecoEvent::~RecoEvent()
{
   for (auto p_recomodule : v_modules)
      delete p_recomodule;
   v_modules.clear();
   v_modules_position.clear();
   delete GWF;
   delete GWFtruncatedGP1;
   GWF = nullptr;
   GWFtruncatedGP1 = nullptr;
}

void Reconstruction::RecoEvent::Clear()
{
   for (auto p_recomodule : v_modules)
      delete p_recomodule;
   v_modules.clear();
   v_modules_position.clear();
   selected = false;
   eventNbr = 0;
   dEdxXP = 0;
   dEdxWF = 0;
   dEdxXPnoTrunc = 0;
   dEdxWFnoTrunc = 0;
   NCrossedPads = 0;
   NPads = 0;
   NClusters = 0;
   lengthXP = 0;
   lengthWF = 0;
   numberOfModules = 0;
   avg_pad_mult = 0;
   delete GWF;
   delete GWFtruncatedGP1;
   GWF = nullptr;
   GWFtruncatedGP1 = nullptr;
}