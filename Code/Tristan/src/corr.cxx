#include "Tristan/corr.h"
#include "Tristan/Misc_Functions.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>

#include "TH2.h"
#include "TGraphErrors.h"

#include "AnalysisTools/DoTracksReconstruction.h"
#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Fitters_Track/TheFitterTrack.h"
#include "Misc/Util.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "SignalShape/PRF_param.h"
#include "SampleTools/ReadGainmap.h"

void corr( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use)
{ 
  // Redirect Output
  std::string     OutPut_Analysis = OutDir + "_corr.txt" ;
  std::cout <<    OutPut_Analysis       << std::endl ;
  std::cout <<    std::endl ;
  std::streambuf* coutbuf = std::cout.rdbuf() ;     // Save old buf
  std::ofstream   OUT_DataFile( OutPut_Analysis.c_str() ) ; // Set output file
  std::cout.      rdbuf(OUT_DataFile.rdbuf()) ;             // Redirect std::cout to output file

  // Geometry
  float Lx              = 11.28 ;                               // Length of pad (mm)
  float Ly              = 10.19 ;                               // Height of pad (mm)
  float n_param_trk     = 3 ;                                   // 2 if there is not magnetic field
  float mean_phi        = 0 ;
  int n_events          = 0 ;
  std::cout << OutDir << ".root" << std::endl ;
  std::cout << SelectionSet << std::endl ;
  
  std::string eram_num ;
  if(Tag.find("DESY") != std::string::npos) eram_num = "ERAM01";
  if(Tag.find("CERN22") != std::string::npos) eram_num = "ERAM18";
  ReadGainmap Gainmap(eram_num) ;
  std::cout << "Gain map: loaded" << std::endl ;

  int   status  = 0 ;
  float avg_G   = 0 ;
  float n_pads  = 0 ;
  for(int iX = 0 ; iX < 36 ; iX++){
    for(int iY = 0 ; iY < 32 ; iY++){
      avg_G += Gainmap.GetData(iX, iY, status) ;
      if(Gainmap.GetData(iX, iY, status) != 0) n_pads++ ;
    }
  }
  avg_G /= n_pads ;
  std::cout << "Average Gain in " << eram_num << " = " << avg_G << std::endl ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Selection stage

  JFL_Selector aJFL_Selector(SelectionSet) ;
  int NEvent = pUploader->Get_NberOfEvent() ;
  std::cout << "Number of entries :" << NEvent << std::endl ;
  // Get the correct cut on TLeading
  if(SelectionSet == "T2_CERN22_Event" or SelectionSet == "T_DESY21_Event"){
    int TLow  = 0 ; int THigh = 0 ;
    if(Get120_CSV("../Stage120_Cuts.csv", Tag, TLow, THigh)) std::cout << "TLow = " << TLow << " | THigh = " << THigh << std::endl ;
    else{
      std::cout << "No Stage120 cuts found in CSV. Getting them now..." << std::endl ;
      std::vector<int> v_TCut           = Comp120_Cut(pUploader, NbrOfMod, Data_to_Use, 0) ;
      TLow                              = v_TCut[0] ;
      THigh                             = v_TCut[1] ;
      Set120_CSV("../Stage120_Cuts.csv", Tag, TLow, THigh) ;
      std::cout << "Stage120 cuts are " << TLow << " to " << THigh << ". Values added to CSV file." << std::endl ;
    }
    aJFL_Selector.Set_Cut_Stage120_TLow (TLow) ;
    aJFL_Selector.Set_Cut_Stage120_THigh(THigh) ;
  }
  // Selection for DESY21 phi diagonal clustering
  if(Tag.find("diag") != std::string::npos){
    aJFL_Selector.Set_Cut_Stage5_NCluster_Low(50) ;
    aJFL_Selector.Set_Cut_StageT15_APM_Low(1) ;
    aJFL_Selector.Set_Cut_StageT15_APM_High(3.5) ;
  }
  // Selection for DESY21 theta
  if(Tag.find("theta") != std::string::npos){
    aJFL_Selector.Set_Cut_Stage120_TLow(0) ;
    aJFL_Selector.Set_Cut_Stage120_THigh(510) ;
    aJFL_Selector.Set_Cut_Stage11_EventBased(200) ;
  }
    
  aJFL_Selector.Tell_Selection() ;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  TH2F* h2f_WFvsLength      = new TH2F("h2f_WFvsLength", "WF_{sum} VS length in cluster;Length in cluster (mm);WF_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TF1* A_corr                = new TF1("A_corr", "[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
  A_corr->SetParameters(291, 9.47, -4.04, 1.32, -0.0595) ;
  // TF1* A_corr                = new TF1("A_corr", "[0] + [1]*cos([2]*x-[3])", 0, 17);
  // A_corr->SetParameters(400, 600, M_PI/16, -16) ;
  TheFitterTrack aTheFitterTrack("Minuit", n_param_trk) ;
  PRF_param                   aPRF_param  ;
  TF1* tf1_PRF              = new TF1("tf1_PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5) ;
  tf1_PRF->                   SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4()) ;
  int Kounter_Fit           = 0 ;
  int Kounter_Fail          = 0 ;
  
  aJFL_Selector.                          Reset_StatCounters() ;
  std::cout << "Processing events:" << std::endl ;
  for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 500 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                        = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    if (!pEvent)                            continue ;
    aJFL_Selector.                          ApplySelection(pEvent) ;
    if (pEvent->IsValid() != 1)             continue ;

    // Loop On Modules
    int NberOfModule                      = pEvent->Get_NberOfModule() ;
    for (int iMod = 0 ; iMod < NberOfModule ; iMod++){
      Module* pModule                     = pEvent->Get_Module_InArray(iMod) ;
      if (pEvent->Validity_ForThisModule(iMod) == 0) continue ;

      // PRF procedure & track evaluation
      TheFitterCluster_PV0_PV1            aTheFitterCluster_PV0_PV1("Minuit") ;
      Do_ClusterFit_PV0_PV1_Event(pEvent, iMod, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_PV1) ;
      TheFitterTrack                      aTheFitterTrack("Minuit", n_param_trk) ;
      int reco                          = DoTracksReconstruction_Event(aTheFitterTrack, pEvent, iMod, n_param_trk) ;

      // Track details
      const Track* pTrack                 = pEvent->GiveMe_Track_ForThisModule(iMod) ;
      mean_phi                           += std::atan(pTrack->Get_ParameterValue(1)) ;

      std::vector<TH1F*>                    v_trashbin ;
      float NClusters                     = pModule->Get_NberOfCluster() ;

      // Loop On Clusters
      for (int iC = 0 ; iC < NClusters ; iC++){
        TH1F* h1f_WF_cluster              = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ; // Store this cluster's WFs
        Cluster* pCluster                 = pModule->Get_Cluster(iC) ;
        float trk_len_clus                = 0 ; // in meters

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads() ;
        for(int iP = 0 ; iP < NPads ; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP) ;
          int StatusG  = 0 ;
          double G_pad                    = Gainmap.GetData(pPad->Get_iX(),pPad->Get_iY(), StatusG) ;
          if(G_pad == 0){
            std::cout << "Gain hole in entry " << pEvent->Get_EntryNber() << " | iX = " << pPad->Get_iX() << " | iY = " << pPad->Get_iY() ; 
            float G_left                  = Gainmap.GetData(pPad->Get_iX()-1,pPad->Get_iY(),   StatusG) ;
            float G_right                 = Gainmap.GetData(pPad->Get_iX()+1,pPad->Get_iY(),   StatusG) ;
            float G_low                   = Gainmap.GetData(pPad->Get_iX(),  pPad->Get_iY()-1, StatusG) ;
            float G_top                   = Gainmap.GetData(pPad->Get_iX(),  pPad->Get_iY()+1, StatusG) ;
            G_pad                         = (G_left + G_right + G_low + G_top)/4 ;
            Gainmap.                        SetData(pPad->Get_iX(),pPad->Get_iY(), G_pad) ;
            std::cout << " | Gain value reset at " << G_pad << std::endl ;
          }
          float Gcorr                     = avg_G/G_pad ;

          TH1F* h1f_WF_pad                = GiveMe_WaveFormDisplay(pPad, "main") ;
          h1f_WF_pad->                      Scale(Gcorr) ;
          h1f_WF_cluster->                  Add(h1f_WF_pad) ;
          v_trashbin.                       push_back(h1f_WF_pad) ;

          // Track computations (impact parameter, angle, length in pad)
          std::vector<float> loc_par      = local_params(pPad, pTrack) ;
          trk_len_clus                   += loc_par[2]; 

        }

        if(trk_len_clus > 0) h2f_WFvsLength->Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()) ;
        delete h1f_WF_cluster ;
      }

      for(int i = 0 ; i < (int)v_trashbin.size() ; i++) {delete v_trashbin[i] ; v_trashbin[i] = 0 ; }
      v_trashbin.                           clear() ;
    }
    delete pEvent ;
    n_events++ ;
  }
  mean_phi /= n_events ;
  std::cout << std::setprecision(2) << "mean_phi = " << mean_phi*180/M_PI << std::endl ;
  aJFL_Selector.PrintStat() ;
  delete tf1_PRF ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting 
  float L_phi                           = std::min({Lx/fabs(cos(mean_phi)), Ly/fabs(sin(mean_phi))}) ;
  std::cout << L_phi << std::endl ;
  A_corr->                                SetNameTitle("A_corr", "A_corr") ;
  TGraph* tge_WFvsLength                = Convert_TH2_TGE(h2f_WFvsLength) ;
  tge_WFvsLength->                        SetNameTitle("pTGE_A_corr", "pTGE_A_corr") ;
  tge_WFvsLength->                        Fit(A_corr,"RQ","0", 0, L_phi) ;
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Saving
  // Checks
  TFile* pfileROOT_corr = new TFile(TString(OutDir + "_WFmax_correction.root"), "RECREATE") ;
  h2f_WFvsLength->                        Write() ;
  A_corr->                                Write() ;
  tge_WFvsLength->                        Write() ;
  pfileROOT_corr->                        Close() ;  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting
  delete h2f_WFvsLength ;
  delete tge_WFvsLength ;
  delete A_corr ;
  delete pfileROOT_corr ;

  std::cout.rdbuf(coutbuf) ; // Reset to standard output
}
