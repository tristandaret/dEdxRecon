#include "PID/Monitoring.h"
#include "PID/dEdx.h"
#include "PID/Tools.h"
#include "PID/LUTs.hxx"

#include "PID/Misc_Functions.h"
#include "PID/dEdx_func.h"

#include "Misc/Util.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "SignalShape/PRF_param.h"
#include "AnalysisTools/DoTracksReconstruction.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Fitters_Track/TheFitterTrack.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "EvtModelTools/JFL_Selector.h"

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
  // Output
  pFile = new TFile((OUTDirName + "2_" + tag + "_dEdx" + comment + ".root").c_str(), "RECREATE");
  pTree = new TTree("dEdx_tree", "dEdx TTree");
  pFile->cd();
  pTree->Branch("tevent", &tevent);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Track fit initializations
  if(comment.find("WF2")     != std::string::npos) WFupdated = true;
  pcorrFunctionWF             = corr_func(dataFile, tag, WFupdated);
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
  for (int iEvent = 0; iEvent < NEvent; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl;
    pEvent->                    Clear_Modules();

    pEvent                    = pUploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
    if (!pEvent)                continue;

    aJFL_Selector.              ApplySelection(pEvent);
    if (pEvent->IsValid() != 1) continue;

    tevent = PID::TEvent();
    tevent.eventNbr = iEvent;

    tevent.numberOfModules          = pEvent->Get_NberOfModule();

    // Check if there is a track in 4 ERAMs that are aligned
    for (int iMod = 0; iMod < tevent.numberOfModules; iMod++){
      fmodID                 = pEvent->Get_Module_InArray(iMod)->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(fmodID)!=0) v_erams.push_back(fERAMs_iD[fmodID]);
    } 

    // int nber = 2;
    // if(dataFile.find("All_ERAMS") != std::string::npos){
    //   // 1 ERAM
    //   if(fnERAMs == 1 and!is_in(v_erams, nber)){
    //     delete pEvent;
    //     continue;
    //   }
    //   // 2 ERAMs
    //   if(fnERAMs == 2 and !(is_in(v_erams, 7) and is_in(v_erams, 1)) and !(is_in(v_erams, 16) and is_in(v_erams, 10))){
    //     delete pEvent;
    //     continue;
    //   }
    //   // 4 ERAMs
    //   if(fnERAMs == 4
    //     and !(is_in(v_erams,  7) and is_in(v_erams,  1) and is_in(v_erams, 23) and is_in(v_erams,  2))
    //     and !(is_in(v_erams, 16) and is_in(v_erams, 15) and is_in(v_erams, 10) and is_in(v_erams, 12))){
    //     delete pEvent;
    //     continue;
    //   }
    // }

    for (int iMod = 0; iMod < tevent.numberOfModules; iMod++){
      Module* pModule         = pEvent->Get_Module_InArray(iMod);
      fmodID                 = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(fmodID) == 0) continue;

      // if(dataFile.find("All_ERAMS") != std::string::npos){
      //   if(fnERAMs == 1 and Gainmaps[fmodID]->Get_iD()[fmodID] != nber) continue;
      //   if(fnERAMs == 2 and Gainmaps[fmodID]->Get_iD()[fmodID] != 7 and Gainmaps[fmodID]->Get_iD()[fmodID] != 1 and Gainmaps[fmodID]->Get_iD()[fmodID] != 16 and Gainmaps[fmodID]->Get_iD()[fmodID] != 10) continue; // 2 ERAMs
      // }

      NC                  = pModule->Get_NberOfCluster();
      if(NC == 0) continue;

      PID::TModule tmodule;
      tmodule.position       = iMod;
      tmodule.ID        = fmodID;

      // Track fitting
      if(tag.find("diag") == std::string::npos){
        TheFitterCluster_PV0_PV1  aTheFitterCluster_PV0_PV1("Minuit");
        Do_ClusterFit_PV0_PV1_Event(pEvent, fmodID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_PV1);
      }
      else{
        TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit");
        Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(PHIMAX*M_PI/180)), pEvent, fmodID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_Diag);
      }
      TheFitterTrack              aTheFitterTrack("Minuit", fnParamsTrack);
      DoTracksReconstruction_Event(aTheFitterTrack, pEvent, fmodID, fnParamsTrack);

      // Track details
      const Track* pTrack     = pEvent->GiveMe_Track_ForThisModule(fmodID);
      tmodule.phi             = std::atan(pTrack->Get_ParameterValue(1))/M_PI*180;
      tmodule.Track->SetParameters(pTrack->Get_ParameterValue(0), pTrack->Get_ParameterValue(1));
      Double_t parErrors[2] = {pTrack->Get_ParameterError(0), pTrack->Get_ParameterError(1)};
      tmodule.Track->SetParErrors(parErrors);
      if(fnParamsTrack == 3){
        tmodule.Track->SetParameter(2, pTrack->Get_ParameterValue(2));
        tmodule.Track->SetParError(2, pTrack->Get_ParameterError(2));
      }


      // Loop On Clusters
      for (int iC = 0; iC < NC; iC++){
        Cluster* pCluster           = pModule->Get_Cluster(iC);
        PID::TCluster tcluster;

        // Loop On Pads
        int NPads                   = pCluster->Get_NberOfPads();
        for(int iP = 0; iP < NPads; iP ++){
          const Pad* pPad           = pCluster->Get_Pad(iP);
          PID::TPad tpad;
          tpad.ix                   = pPad->Get_iX();
          tpad.iy                   = pPad->Get_iY();
          tpad.ph1f_WF_pad          = GiveMe_WaveFormDisplay(pPad, "main");
          tpad.RC                   = pERAMMaps->RC(fERAMs_pos[iMod], pPad->Get_iX(), pPad->Get_iY());
          if(fgainCorrection){
            tpad.gain               = pERAMMaps->Gain(fERAMs_pos[iMod], pPad->Get_iX(), pPad->Get_iY());
            tpad.GainCorrection     = AVG_GAIN/tpad.gain;
            tpad.ADC                = fgainCorrection*pPad->Get_AMax();
            tpad.ph1f_WF_pad->       Scale(fgainCorrection);
          }
          else tpad.ADC             = pPad->Get_AMax();
          tcluster.ph1f_WF_cluster->  Add(tpad.ph1f_WF_pad);
          
          // Track computations (impact parameter in m, angle in degrees, length in pad in m)
          local_params(pPad, pTrack, tpad.d, tpad.dd, tpad.phi, tpad.length);
          tpad.d               *= 1000; // in mm
          tpad.dd              *= 1000; // in mm
          if(tpad.length <= 1e-6) continue;  // ignore non-but-almost-zero tracks
          tpad.length          /= costheta;
          tcluster.length      += tpad.length;

          // Compute Z
          tpad.TMax             = pPad->Get_TMax();
          if      (PT == 412 and TB == 50){ tpad.T0 = 45; tpad.driftDistance = 3.90*(tpad.TMax-tpad.T0); } // 45 = 37(time shift) +  8(PT) from David
          else if (PT == 412 and TB == 40){ tpad.T0 = 56; tpad.driftDistance = 3.12*(tpad.TMax-tpad.T0); } // 56 = 46(time shift) + 10(PT)
          else if (PT == 200 and TB == 50){ tpad.T0 = 39; tpad.driftDistance = 3.90*(tpad.TMax-tpad.T0); } // 39 = 35(time shift) +  4(PT) own computation
          else if (PT == 200 and TB == 40){ tpad.T0 = 48; tpad.driftDistance = 3.12*(tpad.TMax-tpad.T0); } // 48 = 44(time shift) +  5(PT)

          if(tpad.length <= fminLength) continue;
          
          tpad.ratioDrift       = p_lut->getRatio(fabs(tpad.phi), fabs(tpad.d), tpad.RC, tpad.driftDistance);
          tpad.ratioFile        = p_lut->getRatio(fabs(tpad.phi), fabs(tpad.d), tpad.RC, driftDist);

          if(driftMethod == "zcalc") tpad.ratio  = tpad.ratioDrift;
          if(driftMethod == "zfile") tpad.ratio  = tpad.ratioFile;

          v_dE.                   push_back(tpad.ADC*tpad.ratio);
          v_dx.                   push_back(tpad.length);
          v_dEdxXP.               push_back(tpad.ADC*tpad.ratio/tpad.length*10); // mm to cm

          tevent.NCrossedPads++;
          tcluster.v_pads.push_back(tpad);
        } // Pads

        // WF method
        tcluster.ratioCorr            = fAref / pcorrFunctionWF->Eval(tcluster.length*1000);
        if(tag.find("diag") != std::string::npos) v_dEdxWF.push_back(tcluster.ph1f_WF_cluster->GetMaximum()*tcluster.ratioCorr/10.19*10);
        else                    v_dEdxWF.push_back(tcluster.ph1f_WF_cluster->GetMaximum()/(tcluster.length*100));
        tevent.NClusters++;
        
        tmodule.v_clusters.push_back(tcluster);
      } // Clusters
      tevent.v_modules.push_back(tmodule);
    } // Modules

    if(tevent.NCrossedPads > 0){
      // WF
      tevent.dEdxWF                 = ComputedEdxWF(v_dEdxWF, tevent.NClusters);
      ph1f_WF->                       Fill(tevent.dEdxWF);

      // XP
      tevent.dEdxXP                 = ComputedEdxXP(v_dEdxXP, v_dE, v_dx, tevent.NCrossedPads);
      ph1f_XP->                       Fill(tevent.dEdxXP);
    }

    pTree->                     Fill();
    v_erams.                    clear();
    v_dE.                       clear();
    v_dx.                       clear();
    v_dEdxXP.                   clear();
    v_dEdxWF.                   clear();
  } // Events

  aJFL_Selector.PrintStat();

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
  ptf1_WF->                     Write();
  ptf1_XP->                     Write();
  ph1f_WF->                     Write();
  ph1f_XP->                     Write();
  pFile->                       Close();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting
  delete pFile;
  delete pcorrFunctionWF;
  delete ptf1_WF;
  delete ptf1_XP;
  delete ph1f_WF;
  delete ph1f_XP;
  delete pERAMMaps;
  delete pEvent;
  delete ptf1PRF;
  std::cout.rdbuf(coutbuf); // Reset to standard output
}



float PID::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters){
    float NClustersTrunc                        = int(floor(NClusters*falpha));
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

