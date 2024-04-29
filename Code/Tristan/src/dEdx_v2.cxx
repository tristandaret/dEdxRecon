#include "Tristan/dEdx.h"
#include "Tristan/THATdEdx.hxx"
#include "Tristan/dEdx_func.h"
#include "Tristan/Misc_Functions.h"

#include "Misc/Util.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "EvtModelTools/JFL_Selector.h"
#include "AnalysisTools/DoTracksReconstruction.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Fitters_Track/TheFitterTrack.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "SignalShape/PRF_param.h"


void PID::dEdx::Reconstruction(){
  std::string OUTDirName                = outDir + tag + "/";
  MakeMyDir(OUTDirName);
  std::string OUTDIR_Display            = OUTDirName + "Evt_Display/";
  MakeMyDir(OUTDIR_Display);
  std::string OUTDIR_WF_Display         = OUTDirName + "WF_Display/";
  MakeMyDir(OUTDIR_WF_Display);

  // Redirect Output
  std::string     OutPut_Analysis       = OUTDirName + "3_" + tag + "_dEdx_XP.log";
  std::cout <<    OutPut_Analysis       << std::endl;
  std::cout <<    std::setprecision(2)  << std::fixed;
  std::cout <<    std::endl;
  std::streambuf* coutbuf               = std::cout.rdbuf();  // Save old buf
  std::ofstream   OUT_DataFile( OutPut_Analysis.c_str() );    // Set output file
  std::cout.      rdbuf(OUT_DataFile.rdbuf());                // Redirect to output file

  std::cout << "tag              : " << tag          << std::endl;
  std::cout << "comment          : " << comment      << std::endl;
  std::cout << "dataFile         : " << dataFile     << std::endl;
  std::cout << "selectionSet     : " << selectionSet << std::endl;
  std::cout << "OUTDirName       : " << OUTDirName   << std::endl;
  std::cout <<                                          std::endl;

  // Get ERAM ID
  if(tag.find("DESY") != std::string::npos)         {fERAMs_iD.push_back(1);  fERAMs_pos.push_back(12);}
  if(dataFile.find("ERAM18") != std::string::npos)  {fERAMs_iD.push_back(18); fERAMs_pos.push_back(18);} // 14 in position #18 because 18 is not mounted but they are similar
  if(dataFile.find("All_ERAMS") != std::string::npos){fERAMs_iD = {7, 1, 23, 2, 16, 15, 10, 12}; fERAMs_pos = {26, 7, 12, 10, 7, 17, 19, 13, 14};} // 12 replace with 11 in pos #14

  for (int theta_file : theta_arr) if(tag.find("theta") != std::string::npos and tag.find(std::to_string(theta_file)) != std::string::npos) costheta = fabs(cos((float)theta_file/180*M_PI));

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Selection stage
  JFL_Selector aJFL_Selector(selectionSet);
  int NEvent = pUploader->Get_NberOfEvent();
  std::cout << "Number of entries :" << NEvent << std::endl;

  Init_selection(selectionSet, aJFL_Selector, tag, pUploader, moduleCase, 0);
    
  aJFL_Selector.Tell_Selection();

  std::cout << "drift distance : " << driftDist       << " mm"        << std::endl;
  std::cout << "Peaking time   : " << PT              << " ns"        << std::endl;
  std::cout << "Time bin length: " << TB              << " ns"        << std::endl;
  std::cout << "PT/TB          = " << PT/TB           << " time bins" << std::endl;
  std::cout << "drift method   : " << driftMethod                     << std::endl;
  std::cout << "minimal length : " << fminLength*1e3  << " mm"        << std::endl;
  std::cout <<                                                           std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Track fit initializations
  if(comment.find("WF2")     != std::string::npos) fWFupdated = true;
  pcorrFunctionWF             = corr_func(dataFile, tag, fWFupdated);
  fAref                       = pcorrFunctionWF->Eval(10.19);

  TheFitterTrack aTheFitterTrack("Minuit", fnParamsTrack);
  PRF_param                     aPRF_param;
  ptf1PRF                     = new TF1("ptf1PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5);
  ptf1PRF->                     SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4());
  fcounterFail                = 0;
  fcounterFit                 = 0;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Compute dE/dx
  aJFL_Selector.                Reset_StatCounters();
  std::cout << "Processing events:" << std::endl;
  for (feventNbr = 0; feventNbr < NEvent; feventNbr++){
    EventReset();
    if(feventNbr % 1000 == 0 or feventNbr == NEvent-1) std::cout << feventNbr << "/" << NEvent << std::endl;
    pEvent->                    Clear_Modules();

    pEvent                    = pUploader->GiveMe_Event(feventNbr, moduleCase, 0, 0);
    if (!pEvent)                continue;

    aJFL_Selector.              ApplySelection(pEvent);
    if (pEvent->IsValid() != 1) continue;

    // Initialize variables
    fnumberOfModules          = pEvent->Get_NberOfModule();

    // Check if there is a track in 4 ERAMs that are aligned
    std::vector<double> eram_list;
    for (fModulePos = 0; fModulePos < fnumberOfModules; fModulePos++){
      fModuleID                 = pEvent->Get_Module_InArray(fModulePos)->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(fModuleID)!=0) eram_list.push_back(fERAMs_iD[fModuleID]);
    } 

    // int nber = 2;
    // if(dataFile.find("All_ERAMS") != std::string::npos){
    //   // 1 ERAM
    //   if(fnERAMs == 1 and!is_in(eram_list, nber)){
    //     delete pEvent;
    //     continue;
    //   }
    //   // 2 ERAMs
    //   if(fnERAMs == 2 and !(is_in(eram_list, 7) and is_in(eram_list, 1)) and !(is_in(eram_list, 16) and is_in(eram_list, 10))){
    //     delete pEvent;
    //     continue;
    //   }
    //   // 4 ERAMs
    //   if(fnERAMs == 4
    //     and !(is_in(eram_list,  7) and is_in(eram_list,  1) and is_in(eram_list, 23) and is_in(eram_list,  2))
    //     and !(is_in(eram_list, 16) and is_in(eram_list, 15) and is_in(eram_list, 10) and is_in(eram_list, 12))){
    //     delete pEvent;
    //     continue;
    //   }
    // }

    for (int fModulePos = 0; fModulePos < fnumberOfModules; fModulePos++){
      ModuleReset();
      Module* pModule         = pEvent->Get_Module_InArray(fModulePos);
      fModuleID                 = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(fModuleID) == 0) continue;
      // if(dataFile.find("All_ERAMS") != std::string::npos){
      //   if(fnERAMs == 1 and Gainmaps[fModuleID]->Get_iD()[fModuleID] != nber) continue;
      //   if(fnERAMs == 2 and Gainmaps[fModuleID]->Get_iD()[fModuleID] != 7 and Gainmaps[fModuleID]->Get_iD()[fModuleID] != 1 and Gainmaps[fModuleID]->Get_iD()[fModuleID] != 16 and Gainmaps[fModuleID]->Get_iD()[fModuleID] != 10) continue; // 2 ERAMs
      // }
      int NC                  = pModule->Get_NberOfCluster();
      if(NC == 0) continue;

      // Track fitting
      if(tag.find("diag") == std::string::npos){
        TheFitterCluster_PV0_PV1  aTheFitterCluster_PV0_PV1("Minuit");
        Do_ClusterFit_PV0_PV1_Event(pEvent, fModuleID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_PV1);
      }
      else{
        TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit");
        Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(PHIMAX*M_PI/180)), pEvent, fModuleID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_Diag);
      }
      TheFitterTrack              aTheFitterTrack("Minuit", fnParamsTrack);
      DoTracksReconstruction_Event(aTheFitterTrack, pEvent, fModuleID, fnParamsTrack);

      // Track details
      const Track* pTrack     = pEvent->GiveMe_Track_ForThisModule(fModuleID);
      fphi                    = std::atan(pTrack->Get_ParameterValue(1))/M_PI*180;

      // Loop On Clusters
      for (int iC = 0; iC < NC; iC++){
        ClusterReset();
        ph1f_WF_cluster       = new TH1F("ph1f_WF_cluster", "ph1f_WF_cluster", 510, -0.5, 509.5);
        Cluster* pCluster     = pModule->Get_Cluster(iC);

        // Loop On Pads
        int NPads             = pCluster->Get_NberOfPads();
        for(int iP = 0; iP < NPads; iP ++){
          PadReset();
          const Pad* pPad     = pCluster->Get_Pad(iP);
          ph1f_WF_pad         = GiveMe_WaveFormDisplay(pPad, "main");
          fAPad;
          fRCPad              = pERAMMaps->RC(fERAMs_pos[fModulePos], pPad->Get_iX(), pPad->Get_iY());
          if(fgainCorrection){
            fGainPad          = pERAMMaps->Gain(fERAMs_pos[fModulePos], pPad->Get_iX(), pPad->Get_iY());
            fgainCorrection   = AVG_GAIN/fGainPad;
            fAPad             = fgainCorrection*pPad->Get_AMax();
            ph1f_WF_pad->       Scale(fgainCorrection);
          }
          else fAPad          = pPad->Get_AMax();

          ph1f_WF_cluster->     Add(ph1f_WF_pad);
          v_trash.              push_back(ph1f_WF_pad);
          
          // Track computations (impact parameter in m, angle in degrees, length in pad in m)
          local_params(pPad, pTrack, fd, fdd, fphi, fpadLength);
          fd                 *= 1000; // in mm
          fdd                *= 1000; // in mm
          if(fpadLength <= 1e-6)  continue;  // ignore non-but-almost-zero tracks
          fpadLength         /= costheta;
          fclusterLength     += fpadLength;

          // Compute Z
          fTPad               = pPad->Get_TMax();
          if      (PT == 412 and TB == 50){ T0 = 45; fdriftDistance = 3.90*(fTPad-T0); } // 45 = 37(time shift) +  8(PT) from David
          else if (PT == 412 and TB == 40){ T0 = 56; fdriftDistance = 3.12*(fTPad-T0); } // 56 = 46(time shift) + 10(PT)
          else if (PT == 200 and TB == 50){ T0 = 39; fdriftDistance = 3.90*(fTPad-T0); } // 39 = 35(time shift) +  4(PT) own computation
          else if (PT == 200 and TB == 40){ T0 = 48; fdriftDistance = 3.12*(fTPad-T0); } // 48 = 44(time shift) +  5(PT)

          if(fpadLength <= fminLength) continue;
          
          float ratio_zcalc   = p_lut->getRatio(fabs(fphi), fabs(fd), fRCPad, fdriftDistance);
          float ratio_zfile   = p_lut->getRatio(fabs(fphi), fabs(fd), fRCPad, driftDist);

          float ratio;
          if(driftMethod == "zcalc") ratio  = ratio_zcalc;
          if(driftMethod == "zfile") ratio  = ratio_zfile;

          v_dE.                   push_back(fAPad*ratio);
          v_dx.                   push_back(fpadLength);
          v_dEdxXP.               push_back(fAPad*ratio/fpadLength*10); // mm to cm

          fNCrossedPads++;
        }

        // WF method
        if(fclusterLength < fminLength) {delete ph1f_WF_cluster; continue;}
        fratioCorr            = fAref / pcorrFunctionWF->Eval(fclusterLength*1000);
        if(tag.find("diag") != std::string::npos) v_dEdxWF.push_back(ph1f_WF_cluster->GetMaximum()*fratioCorr/10.19*10);
        else                    v_dEdxWF.push_back(ph1f_WF_cluster->GetMaximum()/(fclusterLength*100));
        fNClusters++;
        
        delete ph1f_WF_cluster;
      } // Cluster
    } // Module

    if(fNCrossedPads > 0){
      // WF
      fdEdxWF                 = ComputedEdxWF(v_dEdxWF, fNClusters);
      ph1f_WF->                 Fill(fdEdxWF);

      // XP
      fdEdxXP                 = ComputedEdxXP(v_dEdxXP, v_dE, v_dx, fNCrossedPads);
      ph1f_XP->                 Fill(fdEdxXP);
    }


    for(int i = 0; i < (int)v_trash.size(); i++) {delete v_trash[i]; v_trash[i]       = 0; }
    v_trash.clear();
    v_dE.clear(); 
    v_dx.clear(); 
    v_dEdxWF.clear();
    v_dEdxXP.clear(); 
    eram_list.clear();
  }
  delete pEvent;
  aJFL_Selector.PrintStat();
  delete ptf1PRF;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting //
  std::cout << "Fitting: Started... "; 
  ptf1_WF                     = Fit1Gauss(ph1f_WF, 2);
  ptf1_WF->                     SetNameTitle("ptf1_WF", "ptf1_WF");

  ptf1_XP                     = Fit1Gauss(ph1f_XP, 2);
  ptf1_XP->                     SetNameTitle("ptf1_XP", "ptf1_XP");
  std::cout << "done!" << std::endl; 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Methods
  TFile* pfileROOT_status = new TFile(TString(OUTDirName + "3_" + tag + "_dEdx" + comment + ".root"), "RECREATE");
  std::cout << "dEdx: " << OUTDirName + "3_" + tag + "_dEdx" + comment + ".root" << std::endl;
  ptf1_WF->                     Write();
  ptf1_XP->                     Write();
  ph1f_WF->                     Write();
  ph1f_XP->                     Write();
  pfileROOT_status->            Close();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting
  delete pfileROOT_status;
  delete pcorrFunctionWF;
  delete ptf1_WF;
  delete ptf1_XP;
  delete ph1f_WF;
  delete ph1f_XP;
  delete pERAMMaps;

  std::cout.rdbuf(coutbuf); // Reset to standard output
}



float PID::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF, const int& nClusters){
    float NClustersTrunc                        = int(floor(nClusters*falpha));
    std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
    return std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + NClustersTrunc, 0.) / NClustersTrunc;
}




float PID::dEdx::ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads) {
    /// dEdx = sum(dE)/Sum(dx) and not average(dE/dx) of each pad => less sensitive to statistical fluctuations
    float NCrossedTrunc                        = int(floor(nCrossedPads*falpha));

    // Few steps to order v_dE & v_dx similarly to v_dEdx
    std::vector<int> indices(v_dEdx.size());
    std::iota(indices.begin(), indices.end(), 0); // make list for 1 to v_dEdx.size()

    std::sort(indices.begin(), indices.end(), [&](int i, int j){ // sorting wrt v_dEdx
        return v_dEdx[i] < v_dEdx[j];
    });
    std::vector<float> v_dE_sort(v_dEdx.size());
    std::vector<float> v_dx_sort(v_dEdx.size());
    for (int i = 0; i < (int)v_dEdx.size(); ++i) {
        v_dE_sort[i] = v_dE[indices[i]];
        v_dx_sort[i] = v_dx[indices[i]];
    }

    float DE            = std::accumulate(v_dE_sort.begin(),v_dE_sort.begin()+NCrossedTrunc, 0);
    float DX            = std::accumulate(v_dx_sort.begin(),v_dx_sort.begin()+NCrossedTrunc, 0);
    return DE/DX*10; // mm->cm normalization
}

void PID::dEdx::TreeInitilization(TTree* pTree){
    pTree->Branch("eventNbr", &feventNbr, "eventNbr/I");
    pTree->Branch("numberOfModules", &fnumberOfModules, "numberOfModules/I");
    pTree->Branch("ModuleID", &fModuleID, "ModuleID/I");
    pTree->Branch("ModulePos", &fModulePos, "ModulePos/I");
    pTree->Branch("dEdxWF", &fdEdxWF, "dEdxWF/F");
    pTree->Branch("dEdxXP", &fdEdxXP, "dEdxXP/F");
    pTree->Branch("NCrossedPads", &fNCrossedPads, "NCrossedPads/I");
    pTree->Branch("NClusters", &fNClusters, "NClusters/I");
    pTree->Branch("lengthWF", &flengthWF, "lengthWF/F");
    pTree->Branch("lengthXP", &flengthXP, "lengthXP/F");
    pTree->Branch("ClusterLength", &fclusterLength, "ClusterLength/F");
    pTree->Branch("APad", &fAPad, "APad/F");
    pTree->Branch("phi", &fphi, "phi/F");
    pTree->Branch("impactParameter", &fd, "d/F");
    pTree->Branch("PadLength", &fpadLength, "PadLength/F");
    pTree->Branch("TPad", &fTPad, "TPad/F");
    pTree->Branch("driftDistance", &fdriftDistance, "driftDistance/F");
    pTree->Branch("ratio", &ratio, "ratio/F");
}

void PID::dEdx::PadReset(){
    ph1f_WF_pad->Clear();
    fAPad = 0;
    fRCPad = 0;
    fGainPad = 0;
    fGainCorrection = 0;
    fphi = 0;
    fd = 0;
    fdd = 0;
    fpadLength = 0;
    fTPad = 0;
    T0 = 0;
    fdriftDistance = 0;
    fratioDrift = 0;
    fratioFile = 0;
    ratio = 0;
}

void PID::dEdx::ClusterReset(){
    PadReset();
    ph1f_WF_cluster->Clear();
    fclusterLength = 0;
    fNPads = 0;
    fratioCorr = 0;
}

void PID::dEdx::ModuleReset(){
    ClusterReset();
    fModuleID = 0;
    fModulePos = 0;
}

void PID::dEdx::EventReset(){
    ModuleReset();
    v_dE.clear();
    v_dx.clear();
    v_dEdxXP.clear();
    v_dEdxWF.clear();
    v_trash.clear();
    feventNbr = 0;
    fdEdxWF = 0;
    fdEdxXP = 0;
    fNCrossedPads = 0;
    fNClusters = 0;
    flengthWF = 0;
    flengthXP = 0;
    fnumberOfModules = 0;
}

