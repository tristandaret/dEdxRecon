#include "Tristan/dEdx_XP.h"
#include "Tristan/dEdx_func.h"
#include "Tristan/Misc_Functions.h"
#include "Tristan/ReadLUT_vROOT.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "TH2.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"

#include "AnalysisTools/DoTracksReconstruction.h"
#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Fitters_Track/TheFitterTrack.h"
#include "Misc/Util.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "SampleTools/THATERAMMaps.h"
#include "SampleTools/ReadGainmap.h"
#include "SignalShape/PRF_param.h"

void dEdx_XPonly( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& Comment,
                          std::string const& FileName,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use,
                          LUT4               LUT,
                          int         const& PT,
                          int         const& TB,
                          float       const& zdrift)

{ 
  std::string OUTDirName                    = OutDir + Tag + "/" ;
  MakeMyDir(OUTDirName) ;
  std::string OUTDIR_Evt_Display            = OUTDirName + "Evt_Display/" ;
  MakeMyDir(OUTDIR_Evt_Display) ;
  std::string OUTDIR_WF_Display             = OUTDirName + "WF_Display/" ;
  MakeMyDir(OUTDIR_WF_Display) ;

  // Geometry
  float Lx              = 11.28 ;                               // Length of pad (mm)
  float Ly              = 10.19 ;                               // Height of pad (mm)
  float L               = sqrt(pow(Lx, 2) + pow(Ly, 2)) ;       // diagonal length of the pad (mm)
  float phi_max         = std::atan(Ly/Lx)*180/M_PI ;           // (°) angle of the diagonal

  // Parameters for the dE/dx procedure and for selections
  std::string z_method  = "zcalc" ;                             // method to get z ("zcalc" to recompute, "zfile" to use value from ntuple)
  float alpha           = 70 ;                                  // truncation value in %
  float n_param_trk     = 3 ;                                   // 2 if there is not magnetic field
  float len_cut         = 0.002 ;                               // minimum length in pad to be considered truncable (m)
  int gain_corr         = 1 ;                                   // apply XP gain correction

  // Parameters for the LUT
  int   z_step          = 50 ;                                  // z   increment between LUTs
  int   RC_step         = 5 ;                                   // RC  increment between LUTs

  // Redirect Output
  std::string     OutPut_Analysis = OUTDirName + "3_" + Tag + "_dEdx_XP.log" ;
  std::cout <<    OutPut_Analysis       << std::endl ;
  std::cout <<    std::setprecision(2)  << std::fixed ;
  std::cout <<    std::endl ;
  std::streambuf* coutbuf = std::cout.rdbuf() ;     // Save old buf
  std::ofstream   OUT_DataFile( OutPut_Analysis.c_str() ) ; // Set output file
  std::cout.      rdbuf(OUT_DataFile.rdbuf()) ;             // Redirect std::cout to output file

  std::cout << "Tag           : " << Tag          << std::endl ;
  std::cout << "Comment       : " << Comment      << std::endl ;
  std::cout << "FileName      : " << FileName    << std::endl ;
  std::cout << "SelectionSet  : " << SelectionSet << std::endl ;
  std::cout << "NbrOfMod      : " << NbrOfMod     << std::endl ;
  std::cout << "Data_to_Use   : " << Data_to_Use  << std::endl ;
  std::cout << "OUTDirName    : " << OUTDirName   << std::endl ;
  std::cout <<                                       std::endl ;

  // Get ERAM ID
  std::vector<std::string>  ERAMS_iD ;
  if(Tag.find("DESY") != std::string::npos)         ERAMS_iD.push_back("01");
  if(FileName.find("ERAM18") != std::string::npos)  ERAMS_iD.push_back("18");
  if(FileName.find("All_ERAMS") != std::string::npos){
    ERAMS_iD     = {"07", "01", "23", "02", "16", "15", "10", "12"}; // MockUp CERN22
  }
  ERAMS_iD = {"24", "30", "28", "19", "21", "13", "09", "02", "29", "23", "17", "26", "03", "11", "10", "01"}; // bottom HATPC
  ERAMS_iD = {"01", "02", "03",                   "07",       "09", "10",
              "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
              "21",       "23", "24",       "26",       "28", "29", "30"}; // All ERAMs scanned
  // Get Gain & RC maps
  std::vector<ERAM_map*> RCmaps;
  std::vector<ERAM_map*> Gainmaps;
  for(std::string eram_id : ERAMS_iD){
    Gainmaps. push_back(new ERAM_map(eram_id, "Gain"));
    RCmaps.   push_back(new ERAM_map(eram_id, "RC"));
  }

  float avg_G = avg_Gain(Gainmaps);
  std::cout << "Average gain in bottom HATPC: " << avg_G << std::endl;


  // // Fill holes in maps
  // Fill_Maps(RCmaps, Gainmaps, ERAMS_iD);

  float costheta = 1;
  int theta_arr[] = {-45, -20, 20} ;
  for (int theta_file : theta_arr) if(Tag.find("theta") != std::string::npos and Tag.find(std::to_string(theta_file)) != std::string::npos) costheta = fabs(cos((float)theta_file/180*M_PI));

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Selection stage
  JFL_Selector aJFL_Selector(SelectionSet) ;
  int NEvent = pUploader->Get_NberOfEvent() ;
  std::cout << "Number of entries :" << NEvent << std::endl ;

  Init_selection(SelectionSet, aJFL_Selector, Tag, pUploader, NbrOfMod, Data_to_Use);
    
  aJFL_Selector.Tell_Selection() ;

  std::cout << "zdrift        = " << zdrift << " mm"          << std::endl ;
  std::cout << "PT            = " << PT     << " ns"          << std::endl ;
  std::cout << "TB            = " << TB     << " ns/time bin" << std::endl ;
  std::cout << "PT/TB         = " << PT/TB  << " time bins"   << std::endl ;
  std::cout << "z method      : " << z_method                 << std::endl ;
  std::cout << "cut length    = " << len_cut*1e3 << " mm"     << std::endl ;
  std::cout <<                                                   std::endl ;

  TH1F* h1f_XP              = new TH1F("h1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_ratio           = new TH1F("h1f_ratio", "mean ratio per event;mean ratio;Number of events", 90, 0, 2) ;

  // Track fit initializations
  TheFitterTrack aTheFitterTrack("Minuit", n_param_trk) ;
  PRF_param                     aPRF_param;
  TF1* tf1_PRF                = new TF1("tf1_PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5) ;
  tf1_PRF->                     SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4()) ;
  int Kounter_Fit             = 0 ;
  int Kounter_Fail            = 0 ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Compute dE/dx
  aJFL_Selector.                          Reset_StatCounters() ;
  std::cout << "Processing events:" << std::endl ;
  for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                        = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    std::cout << "Entry " << pEvent->Get_EntryNber() << " Event " << pEvent->Get_EventNber() <<  ": ";
    if (!pEvent)                            continue ;
    aJFL_Selector.                          ApplySelection(pEvent) ;
    if (pEvent->IsValid() != 1)             continue ;
    // std::cout << "Event " << pEvent->Get_EventNber() << std::endl;

    // Initialize event variables
    int                                   N_crossed = 0,  N_crossed_trc = 0;
    std::vector<float>                    v_dx, v_dE;
    std::vector<RankedValue>              v_dEdx_ranked ;

    // Loop On Modules
    int nMod                              = pEvent->Get_NberOfModule() ;

    // Check if there are is a track in 4 ERAMs that are aligned
    std::vector<double> eram_list;
    for (int iMod = 0 ; iMod < nMod ; iMod++){
      int ModuleNber                      = pEvent->Get_Module_InArray(iMod)->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber)!=0) eram_list.push_back(Gainmaps[ModuleNber]->Get_iD()[ModuleNber]) ;
    } 

    int nber ;
    if(FileName.find("All_ERAMS") != std::string::npos){
      // 1 ERAM
      nber = 2;
      if(!is_in(eram_list, nber)){
        delete pEvent;
        continue;
      }

      // // 2 ERAMs
      // if(!(is_in(eram_list, 7) and is_in(eram_list, 1)) and !(is_in(eram_list, 16) and is_in(eram_list, 10))){
      //   delete pEvent;
      //   continue;
      // }

      // // 4 ERAMs
      // if(   !(is_in(eram_list,  7) and is_in(eram_list,  1) and is_in(eram_list, 23) and is_in(eram_list,  2))
      //   and !(is_in(eram_list, 16) and is_in(eram_list, 15) and is_in(eram_list, 10) and is_in(eram_list, 12))){
      //   delete pEvent;
      //   continue;
      // }
    }

    float avg_ratio = 0;
    for (int iMod = 0 ; iMod < nMod ; iMod++){
      Module* pModule                     = pEvent->Get_Module_InArray(iMod) ;
      int ModuleNber                      = pModule->Get_ModuleNber() ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      if(FileName.find("All_ERAMS") != std::string::npos){
        if(Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != nber) continue; // 1 ERAM
        // if(Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 7 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 1 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 16 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 10) continue; // 2 ERAMs
      }
      float N_clus                        = pModule->Get_NberOfCluster() ;
      if(N_clus == 0) continue;

      // Track fitting
      if(Tag.find("diag") == std::string::npos){
        TheFitterCluster_PV0_PV1            aTheFitterCluster_PV0_PV1("Minuit") ;
        Do_ClusterFit_PV0_PV1_Event(pEvent, ModuleNber, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_PV1) ;
      }
      else{
        TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit") ;
        Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(phi_max*M_PI/180)), pEvent, ModuleNber, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_Diag) ;
      }
      TheFitterTrack                      aTheFitterTrack("Minuit", n_param_trk) ;
      DoTracksReconstruction_Event(aTheFitterTrack, pEvent, ModuleNber, n_param_trk) ;

      // Track details
      const Track* pTrack                 = pEvent->GiveMe_Track_ForThisModule(ModuleNber) ;
      float phi                           = std::atan(pTrack->Get_ParameterValue(1))/M_PI*180 ;

      // Loop On Clusters
      for (int iC = 0 ; iC < N_clus ; iC++){
        Cluster* pCluster                 = pModule->Get_Cluster(iC) ;

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads() ;
        for(int iP = 0 ; iP < NPads ; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP) ;
          float A_pad;
          double RC_pad                   = RCmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY()) ;
          if(gain_corr){
            double G_pad                  = Gainmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY()) ;
            float Gcorr                   = avg_G/G_pad ;
            A_pad                         = Gcorr*pPad->Get_AMax() ;
          }
          else A_pad                      = pPad->Get_AMax() ;

          // Track computations (impact parameter in m, angle in degrees, length in pad in m)
          float d=0, dd=0, length_in_pad=0;
          local_params(pPad, pTrack, d, dd, phi, length_in_pad);
          d                              *= 1000 ; // in mm
          dd                             *= 1000 ; // in mm
          if(length_in_pad <= 1e-6)         continue ;  // ignore non-but-almost-zero tracks
          length_in_pad                  /= costheta;
          
          // Compute Z
          float Tmax                      = pPad->Get_TMax() ;
          float z_calc                    = 0 ; // in mm
          float T0 ;
          if      (PT == 412 and TB == 50){ T0 = 45 ; z_calc = 3.90*(Tmax-T0) ; } // 45 = 37(time shift) +  8(PT) from David
          else if (PT == 412 and TB == 40){ T0 = 56 ; z_calc = 3.12*(Tmax-T0) ; } // 56 = 46(time shift) + 10(PT)
          else if (PT == 200 and TB == 50){ T0 = 39 ; z_calc = 3.90*(Tmax-T0) ; } // 39 = 35(time shift) +  4(PT) own computation
          else if (PT == 200 and TB == 40){ T0 = 48 ; z_calc = 3.12*(Tmax-T0) ; } // 48 = 44(time shift) +  5(PT)
          
          // Interpolation phi
          if(phi < -90) phi               = -90-2e+6 ;
          if(phi >  90) phi               =  90-2e-6 ;
          // Interpolation d
          if(d >  L/2) d                  =  L/2 ;
          if(d < -L/2) d                  = -L/2 ;
          float absd                      = fabs(d) ;
          // Interpolation Z
          if(z_calc < 0)    z_calc        = 0 ;
          if(z_calc > 1000) z_calc        = 1e3 ;
          float zfile                     = zdrift/z_step ;
          float zconv                     = z_calc/z_step ;
          // Interpolation RC
          if(RC_pad < 50)  RC_pad         = 50 ;
          if(RC_pad > 150) RC_pad         = 150 ;
          float RCconv                    = (RC_pad-50)/RC_step ;

          float absphi                    = fabs(phi);
          float ratio_zcalc               = LUT.Interpolate(absd, absphi, zconv, RCconv) ;
          float ratio_zfile               = LUT.Interpolate(absd, absphi, zfile, RCconv) ;

          float ratio ;
          if(z_method == "zcalc") ratio   = ratio_zcalc ;
          if(z_method == "zfile") ratio   = ratio_zfile ;

          std::cout << "row = " << pPad->Get_iY() << " | col = " << pPad->Get_iX() << " : ";
          std::cout << std::setprecision(3) << "(RC, drift, d, phi, L, ratio) = (" << RC_pad << ", " << z_calc << ", " << d << ", " << phi << ", " << length_in_pad*1000  << ", " << ratio << ")" << std::endl;

          if(length_in_pad <= len_cut)        continue ;
          v_dE.                             push_back(A_pad*ratio);
          v_dx.                  push_back(length_in_pad) ;

          RankedValue dEdx_ranked ;  
          dEdx_ranked.Rank                  = N_crossed ; 
          dEdx_ranked.Value                 = A_pad*ratio/length_in_pad ;
          v_dEdx_ranked.                     push_back(dEdx_ranked) ;
          avg_ratio += ratio;

          N_crossed++ ;
        }
      } // Cluster
      // if(iEvent < 1000) DrawOut_EventDisplay(pModule, OUTDIR_Evt_Display, Tag, "amplitude", pTrack->Get_ParameterValue(2), pTrack->Get_ParameterValue(1), pTrack->Get_ParameterValue(0)) ;
    } // Module

    if(N_crossed > 0){
      avg_ratio /= N_crossed;
      h1f_ratio->Fill(avg_ratio);
      N_crossed_trc                     = int(floor(N_crossed * (alpha/100))) ; 

      // XP
      float dx_trunc            = 0 ;
      float XP                            = 0 ;
      std::sort(v_dEdx_ranked.begin(), v_dEdx_ranked.end()) ;
      for(int iP = 0 ; iP < (int)N_crossed_trc ; iP++){
        dx_trunc               += v_dx[v_dEdx_ranked[iP].Rank]*100 ;
        XP                               += v_dE[v_dEdx_ranked[iP].Rank] ;
      }
      XP                                 /= dx_trunc ;
      h1f_XP->                              Fill(XP) ;
    }

    v_dEdx_ranked.clear() ;
    v_dx.clear() ; 
    eram_list.clear();
    delete                                  pEvent ;
  }
  aJFL_Selector.PrintStat() ;
  delete tf1_PRF ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting //
  TF1* tf1_XP                   = Fit1Gauss(h1f_XP, 2) ;
  tf1_XP->                        SetNameTitle("tf1_XP", "tf1_XP") ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Saving //
  TFile* pfileROOT_status       = new TFile(TString(OUTDirName + "3_" + Tag + "_dEdx" + Comment + "_XPonly.root"), "RECREATE") ;
  std::cout << "dEdx: " << OUTDirName + "3_" + Tag + "_dEdx" + Comment + "_XPonly.root" << std::endl ;
  tf1_XP->                        Write() ;
  h1f_XP->                        Write() ;
  h1f_ratio->                        Write() ;
  pfileROOT_status->              Close() ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting //
  delete tf1_XP ;
  delete h1f_XP ;

  for(int i = 0; i < (int)RCmaps.size(); i++){
    delete RCmaps[i];
    delete Gainmaps[i];
  }
  RCmaps.     clear();
  Gainmaps.   clear();

  std::cout.rdbuf(coutbuf) ; // Reset to standard output
}
