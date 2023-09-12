#include "Tristan/Tristan_CERN22_dEdx.h"
#include "Tristan/Tristan_Misc_Functions.h"
#include "Tristan/Tristan_ReadLUT.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>

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
#include "SampleTools/ReadRCmap.h"
#include "SampleTools/ReadGainmap.h"
#include "SignalShape/PRF_param.h"

void Tristan_CERN22_dEdx( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& Comment,
                          std::string const& EventFile,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use,
                          Interpol4          LUT,
                          int         const& PT,
                          int         const& TB,
                          float       const& zdrift)

{ 
  std::string OUTDirName                    = OutDir + Tag + "/" ;
  MyMakeDir(OUTDirName) ;
  std::string OUTDIR_Evt_Display            = OUTDirName + "Evt_Display/" ;
  MyMakeDir(OUTDIR_Evt_Display) ;
  std::string OUTDIR_WF_Display             = OUTDirName + "WF_Display/" ;
  MyMakeDir(OUTDIR_WF_Display) ;

  // int nZ = 22 ;
  // Interpol3 LUT3(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/LUT_RC115/", 310, PT), nZ ) ;

  // Geometry
  float Lx              = 11.28 ;                               // Length of pad (mm)
  float Ly              = 10.19 ;                               // Height of pad (mm)
  float L               = sqrt(pow(Lx, 2) + pow(Ly, 2)) ;       // diagonal length of the pad (mm)
  float phi_max         = std::atan(Ly/Lx)*180/M_PI ;  // (°) angle of the diagonal

  // Parameters for the dE/dx procedure and for selections
  float PT_TB           = float(PT)/float(TB) ;                 // peaking time in time bins
  std::string z_method  = "zcalc" ;                             // method to get z ("zcalc" to recompute, "zfile" to use value from ntuple)
  std::string fitting ;                                         // type of track fitting, either yw or PRF
  if(Comment.find("yw") != std::string::npos)  fitting = "yw" ;
  if(Comment.find("PRF") != std::string::npos) fitting = "PRF" ;
  float alpha           = 70 ;                                  // truncation value in %
  float n_param_trk     = 3 ;                                   // 2 if there is not magnetic field
  float len_cut         = 0.002 ;                               // minimum length in pad to be considered truncable (m)

  // Parameters for the LUT
  int   size            = 200 ;                                 // LUT definition along d & phi
  float d_step          = L/(size-1) ;                          // d   increment between LUTs
  float phi_step        = (90-2e-6)/(size-1) ;                  // phi increment between LUTs
  int   z_step          = 50 ;                                  // z   increment between LUTs
  int   RC_step         = 5 ;                                   // RC  increment between LUTs

  // Redirect Output
  std::string     OutPut_Analysis = OUTDirName + "3_" + Tag + "_dEdx_XP.txt" ;
  std::cout <<    OutPut_Analysis       << std::endl ;
  std::cout <<    std::setprecision(2)  << std::fixed ;
  std::cout <<    std::endl ;
  std::streambuf* coutbuf = std::cout.rdbuf() ;     // Save old buf
  std::ofstream   OUT_DataFile( OutPut_Analysis.c_str() ) ; // Set output file
  std::cout.      rdbuf(OUT_DataFile.rdbuf()) ;             // Redirect std::cout to output file

  std::cout << "Tag           : " << Tag          << std::endl ;
  std::cout << "Comment       : " << Comment      << std::endl ;
  std::cout << "EventFile     : " << EventFile    << std::endl ;
  std::cout << "SelectionSet  : " << SelectionSet << std::endl ;
  std::cout << "NbrOfMod      : " << NbrOfMod     << std::endl ;
  std::cout << "Data_to_Use   : " << Data_to_Use  << std::endl ;
  std::cout << "OUTDirName    : " << OUTDirName   << std::endl ;
  std::cout <<                                       std::endl ;

  std::string eram_num ;
  if(Tag.find("DESY") != std::string::npos) eram_num = "ERAM01";
  if(Tag.find("CERN22") != std::string::npos) eram_num = "ERAM18";
  ReadRCmap RCmap(eram_num) ;
  std::cout << "RC map: loaded" << std::endl ;
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
    if(Get120_CSV("../Data_DESY21/Stage120_Cuts.csv", Tag, TLow, THigh)) std::cout << "TLow = " << TLow << " | THigh = " << THigh << std::endl ;
    else{
      std::cout << "No Stage120 cuts found in CSV. Getting them now..." << std::endl ;
      std::vector<int> v_TCut           = SetStage120Cuts(pUploader, NbrOfMod, Data_to_Use, 0) ;
      TLow                              = v_TCut[0] ;
      THigh                             = v_TCut[1] ;
      Set120_CSV("../Data_DESY21/Stage120_Cuts.csv", Tag, TLow, THigh) ;
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

  std::cout << "zdrift        = " << zdrift << " mm"          << std::endl ;
  std::cout << "PT            = " << PT     << " ns"          << std::endl ;
  std::cout << "TB            = " << TB     << " ns/time bin" << std::endl ;
  std::cout << "PT/TB         = " << PT/TB  << " time bins"   << std::endl ;
  std::cout << "Track fitting : " << fitting                  << std::endl ;
  std::cout << "z method      : " << z_method                 << std::endl ;
  std::cout << "cut length    = " << len_cut*1e3 << " mm"     << std::endl ;
  std::cout <<                                                   std::endl ;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Methods

  std::vector<TH1F*>                      v_h1f_Qsel  ;
  std::vector<TH1F*>                      v_h1f_Qtrunc  ;
  std::vector<TH1F*>                      v_h1f_WFsel  ;
  std::vector<TH1F*>                      v_h1f_WFtrunc  ;
  std::vector<TH1F*>                      v_h1f_GPsel  ;
  std::vector<TH1F*>                      v_h1f_GPv3 ;
  std::vector<TH1F*>                      v_h1f_GPv6  ;
  std::vector<TH1F*>                      v_h1f_XP  ;
  std::vector<TF1*>                       v_tf1_Qsel ;
  std::vector<TF1*>                       v_tf1_Qtrunc ;
  std::vector<TF1*>                       v_tf1_WFsel ;
  std::vector<TF1*>                       v_tf1_WFtrunc ;
  std::vector<TF1*>                       v_tf1_GPsel ;
  std::vector<TF1*>                       v_tf1_GPv3 ;
  std::vector<TF1*>                       v_tf1_GPv6 ;
  std::vector<TF1*>                       v_tf1_XP ;
  // Comparisons 
  std::vector<TH2F*>                      v_h2f_XPvsWF  ;
  std::vector<TH2F*>                      v_h2f_WFmWFvsWF ;

  TH1F* h1f_dnorm           = new TH1F("h1f_dnorm","Normalized impact parameter d/d_{max};d/d_{max};", 100, -1.1, 1.1) ;
  TH1F* h1f_dist            = new TH1F("h1f_dist","distance of track in pad;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_dist_clus       = new TH1F("h1f_dist_clus","distance of track in cluster;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_Lnorm           = new TH1F("h1f_Lnorm","Length in pad normalized to maximum length in pad for a given #varphi;L_{#cap}/L_{#varphi};Count", 60, 0, 1.2012) ;
  TH1F* h1f_zdiff           = new TH1F("h1f_zdiff",Form("z_{file = %.0fmm} - z_{computed};difference (mm);Count", zdrift), 80, -150, 150) ;
  TH1F* h1f_XPdiff          = new TH1F("h1f_XPdiff","#Sigma(Q)/#Sigma(length) - mean{Q_{i}/length_{i}};difference (ADC count);Count", 100, -150, 150) ;
  TH1F* h1f_Tcross          = new TH1F("h1f_Tcross","T_{max} of crossed pads;T (timbebin);Count", 511, -0.5, 510.5) ;
  TH1F* h1f_Ncross          = new TH1F("h1f_Ncross","Number of crossed pads;# of crossed pads;Count", 45, 15, 60) ;
  TH1F* h1f_angle           = new TH1F("h1f_angle","Angle #varphi in each pad;#varphi (#circ);Count", 66, -55, 10) ;
  TH1F* h1f_WFcorr          = new TH1F("h1f_WFcorr","Correction A_{max} ratio;ratio;Count", 80, 0, 3) ;
  TH1F* h1f_Lmod1VScl       = new TH1F("h1f_Lmod1VScl","L_{ERAM}*0.7 - #Sigma L_{clus>2mm};difference (mm);Count", 80, -60, 60) ;
  TH1F* h1f_Lmod2VScl       = new TH1F("h1f_Lmod2VScl","L_{ERAM}*(N_{trunc cross}/N_{clus cross>2mm}) - #Sigma L_{clus>2mm};difference (mm);Count", 80, -60, 60) ;
  TH1F* h1f_LallVScl        = new TH1F("h1f_LallVScl","L_{clusters} - L_{clusters > 2mm};difference (mm);Count", 80, -60, 60) ;
  TH2F* h2f_ratiodiffZ      = new TH2F("h2f_ratiodiffZ", "LUT(z_{file}) vs LUT(z_{calc});LUT(z_{calc});LUT(z_{file})", 80, 0, 2.1, 80, 0, 2.1) ;
  TH2F* h2f_AmaxvsLength    = new TH2F("h2f_AmaxvsLength", "ADC_{max} VS length in pad (before length cut);Length in pad (mm);ADC_{max}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_QvsLength       = new TH2F("h2f_QvsLength", "Q^{anode} VS length in pad (before length cut);Length in pad (mm);Q^{anode}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_LUTvsLength     = new TH2F("h2f_LUTvsLength", "Q^{anode}/ADC_{max} VS length in pad (before length cut);Length in pad (mm);Q^{anode}/ADC_{max}", 80, -0.1, 16, 80, -0.1, 2.1) ;
  TH2F* h2f_QclvsLength     = new TH2F("h2f_QclvsLength", "Q_{cluster} VS length in cluster;Length in cluster (mm);Q_{cluster} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFvsLength      = new TH2F("h2f_WFvsLength", "WF_{sum} VS length in cluster;Length in cluster (mm);WF_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFtruncvsLength = new TH2F("h2f_WFtruncvsLength", "WF_{sum truncated} VS length in cluster;Length in cluster (mm);WF_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstarvsLen     = new TH2F("h2f_WFstarvsLen", "WF*_{sum} VS length in cluster;Length in cluster (mm);WF*_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstartrcvsLen  = new TH2F("h2f_WFstartrcvsLen", "WF*_{sum truncated} VS length in cluster;Length in cluster (mm);WF*_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH1F* h1f_WFoLength       = new TH1F("h1f_WFoLength","A_{max}(WF_{sum}) / L_{cluster};A_{max}(WF_{sum}) / L_{cluster} (ADC count/mm);", 80, 0, 4000) ;
  TH2F* h2f_lenVSd          = new TH2F("h2f_lenVSd", "impact parameter d vs length in pad;Length in pad (mm);impact parameter (mm)", 80, -0.1, 16, 80, -7.8, 7.8) ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // correction functoin & Track fit initializations

  TF1* A_corr                = new TF1("A_corr", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
  if(Tag.find("diag") != std::string::npos){
    std::string filename = EventFile.substr(0, EventFile.length()-5) ;
    // int angle ;
    // if( (angle = filename.find("30")) != std::string::npos or (angle = filename.find("45")) != std::string::npos) filename.replace(angle, 2, "40") ;
    // TFile* pfile = new TFile((filename + "_WFmax_correction.root").c_str(), "READ") ;
    // std::cout << (filename + "_WFmax_correction.root").c_str() << std::endl ;
    TFile* pfile = new TFile((filename + "_WFmax_correction_full_err100.root").c_str(), "READ") ;
    std::cout << (filename + "_WFmax_correction_full_err100.root").c_str() << std::endl ;
    A_corr                   = pfile->Get<TF1>("A_corr") ;
    pfile->Close() ;
    std::cout << std::setprecision(2) << "WF correction parameters: " << A_corr->GetParameter(0) << " | " << A_corr->GetParameter(1) << " | " << A_corr->GetParameter(2) << " | " << A_corr->GetParameter(3) << " | " << A_corr->GetParameter(4) << std::endl ;
  }
  float A_ref               = A_corr->Eval(Lx) ;

  TheFitterTrack aTheFitterTrack("Minuit", n_param_trk) ;
  PRF_param                   aPRF_param  ;
  TF1* tf1_PRF              = new TF1("tf1_PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5) ;
  tf1_PRF->                   SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4()) ;
  int Kounter_Fit           = 0 ;
  int Kounter_Fail          = 0 ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Make dE/dx histogram for each method

  aJFL_Selector.                          Reset_StatCounters() ;
  for (int iMod = 0 ; iMod < 8 ; iMod++){
    TH1F* h1f_Qsel          = new TH1F(Form("h1f_Qsel_%i", iMod),       Form("<dE/dx> with Q_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_Qtrunc        = new TH1F(Form("h1f_Qtrunc_%i", iMod),     Form("<dE/dx> with Q_{cluster} (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_WFsel         = new TH1F(Form("h1f_WFsel_%i", iMod),      Form("<dE/dx> with WF_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_WFtrunc       = new TH1F(Form("h1f_WFtrunc_%i", iMod),    Form("<dE/dx> with WF_{cluster} (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_GPsel         = new TH1F(Form("h1f_GPsel_%i", iMod),      Form("<dE/dx> with GP_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_GPv3          = new TH1F(Form("h1f_GPv3_%i", iMod),       Form("<dE/dx> with GPv3 (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_GPv6          = new TH1F(Form("h1f_GPv6_%i", iMod),       Form("<dE/dx> with GPv6 (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH1F* h1f_XP            = new TH1F(Form("h1f_XP_%i", iMod),         Form("<dE/dx> with XP (module %i);<dE/dx> (ADC count);Number of events", iMod), 90, 0, 1800) ;
    TH2F* h2f_XPvsWF        = new TH2F(Form("h2f_XPvsWF_%i", iMod),     Form("XP vs WF_{cluster} <dE/dx> (selected data, module %i);WF_{cluster} <dE/dx> (ADC count);XP <dE/dx> (ADC count)", iMod), 90, 0, 1800, 90, 0, 1800) ;
    TH2F* h2f_WFmWFvsWF     = new TH2F(Form("h2f_WFmWFvsWF_%i", iMod),  Form("WF_{not trunc} - WF_{trunc} vs WF_{not trunc} (module %i);WF_{not trunc} <dE/dx> (ADC count);WF_{not trunc} - WF_{trunc} <dE/dx> (ADC count)", iMod), 90, 0, 1800, 200, 0, 500) ;
    v_h1f_Qsel.               push_back(h1f_Qsel);
    v_h1f_Qtrunc.             push_back(h1f_Qtrunc);
    v_h1f_WFsel.              push_back(h1f_WFsel);
    v_h1f_WFtrunc.            push_back(h1f_WFtrunc);
    v_h1f_GPsel.              push_back(h1f_GPsel);
    v_h1f_GPv3.               push_back(h1f_GPv3);
    v_h1f_GPv6.               push_back(h1f_GPv6);
    v_h1f_XP.                 push_back(h1f_XP);
    v_h2f_XPvsWF.             push_back(h2f_XPvsWF);
    v_h2f_WFmWFvsWF.          push_back(h2f_WFmWFvsWF);
  }

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

      int reco ;
      if(fitting == "yw") reco            = DoTracksReconstruction_Event(aTheFitterTrack, pEvent, iMod, n_param_trk) ;
      if(fitting == "PRF"){
        // PRF procedure & track evaluation
        if(Tag.find("diag") == std::string::npos){
          TheFitterCluster_PV0_PV1            aTheFitterCluster_PV0_PV1("Minuit") ;
          Do_ClusterFit_PV0_PV1_Event(pEvent, iMod, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_PV1) ;
        }
        else{
          TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit") ;
          Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(phi_max*M_PI/180)), pEvent, iMod, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_Diag) ;
        }
        TheFitterTrack                      aTheFitterTrack("Minuit", n_param_trk) ;
        reco                              = DoTracksReconstruction_Event(aTheFitterTrack, pEvent, iMod, n_param_trk) ;
      }

      // Track details
      const Track* pTrack                 = pEvent->GiveMe_Track_ForThisModule(iMod) ;
      double phi                          = std::atan(pTrack->Get_ParameterValue(1))/M_PI*180 ;

      std::vector<TH1F*>                    v_h1f_DPRcluster ;
      TH1F* h1f_GWF_mod                   = new TH1F("h1f_GWF_mod", "h1f_GWF_mod", 510, -0.5, 509.5) ; // GWF in module
      std::vector<TH1F*>                    v_h1f_WF_crossed ;        // waveforms of pads crossed by the track
      std::vector<TH1F*>                    v_trashbin ;              // collect all TH1F to delete them eventually
      int Qsel                            = 0 ;
      int Qtrunc                          = 0 ;
      int WFsel                           = 0 ;
      int WFtrunc                         = 0 ;
      int N_crossed                       = 0 ;
      int N_crosclus                      = 0 ;
      std::vector<float>                    v_Q ; 
      std::vector<float>                    v_WF ; 
      std::vector<float>                    v_WFmax ; 
      std::vector<RankedValue>              v_rank_WF ;
      std::vector<RankedValue>              v_LP ;          // list of A_max    of leading pads
      std::vector<RankedValue>              v_DPR ;         // list of DPR_max  of crossed pads
      std::vector<float>                    v_ratio ; 
      std::vector<float>                    v_length ; 
      std::vector<float>                    v_lenclus ; 
      float NClusters                     = pModule->Get_NberOfCluster() ;
      float NClus_trunc                   = int(floor(NClusters * (alpha/100))) ;
      float track_len                     = trk_len(pModule, pTrack)*100 ; // in cm from left side  of cluster #0 to right side of last cluster
      float trk_len_sum_2mm               = 0 ;
      float trk_len_sum                   = 0 ;

      // Loop On Clusters
      for (int iC = 0 ; iC < NClusters ; iC++){
        TH1F* h1f_WF_cluster              = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ; // Store this cluster's WFs
        Cluster* pCluster                 = pModule->Get_Cluster(iC) ;
        Qsel                             += pCluster->Get_Acluster()/track_len ;
        v_Q.                                push_back(pCluster->Get_Acluster()) ;
        float trk_len_clus                = 0 ; // in meters

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads() ;
        for(int iP = 0 ; iP < NPads ; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP) ;
          int StatusRC = 0 ;
          int StatusG  = 0 ;
          double RC_pad                   = RCmap.GetData(pPad->Get_iX(),pPad->Get_iY(), StatusRC) ;
          double G_pad                    = Gainmap.GetData(pPad->Get_iX(),pPad->Get_iY(), StatusG) ;
          if(RC_pad == 0){
            std::cout << "RC hole in entry   " << pEvent->Get_EntryNber() << " | iX = " << pPad->Get_iX() << " | iY = " << pPad->Get_iY() ; 
            float RC_left                 = RCmap.GetData(pPad->Get_iX()-1,pPad->Get_iY(),   StatusRC) ;
            float RC_right                = RCmap.GetData(pPad->Get_iX()+1,pPad->Get_iY(),   StatusRC) ;
            float RC_low                  = RCmap.GetData(pPad->Get_iX(),  pPad->Get_iY()-1, StatusRC) ;
            float RC_top                  = RCmap.GetData(pPad->Get_iX(),  pPad->Get_iY()+1, StatusRC) ;
            RC_pad                        = (RC_left + RC_right + RC_low + RC_top)/4 ;
            RCmap.                          SetData(pPad->Get_iX(),pPad->Get_iY(), RC_pad) ;
            std::cout << " |  RC  value reset at " << RC_pad << std::endl ;
          }
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
          float PadAmaxCorr               = Gcorr*pPad->Get_AMax() ;

          TH1F* h1f_WF_pad                = GiveMe_WaveFormDisplay(pPad, "main") ;
          h1f_WF_pad->                      Scale(Gcorr) ;
          h1f_WF_cluster->                  Add(h1f_WF_pad) ;
          h1f_GWF_mod->                     Add(h1f_WF_pad) ;
          v_trashbin.                       push_back(h1f_WF_pad) ;
          
          // WF & GPv3: List of pads to truncate (leading pads wrt ADC_max)
          if(pPad == pCluster->Get_LeadingPad()){
            RankedValue rank_iC ;  
            rank_iC.Rank                  = iC ; 
            rank_iC.Value                 = PadAmaxCorr ;
            v_LP.                           push_back(rank_iC) ;
          }

          // Track computations (impact parameter, angle, length in pad)
          std::vector<float> loc_par      = local_params(pPad, pTrack) ;
          float d                         = loc_par[0]*1000 ;       // in mm
          if(pTrack->GetNberOfParameters() > 2) phi = loc_par[1] ;  // in degrees, recompute only for curved tracks
          float trk_len_pad               = loc_par[2];             // in m
          trk_len_sum                    += loc_par[2]*1000 ;
          if(trk_len_pad <= 1e-6)           continue ;
          
          float L_phi                     = std::min({ 11.28/std::fabs(std::cos(phi*M_PI/180)), 10.19/std::fabs(std::sin(phi*M_PI/180)) }) ;
          float LNorm                     = trk_len_pad*1000/L_phi ;
          float d_max                     = std::max( std::fabs(L/2*std::sin((phi_max+phi)/180*M_PI)), std::fabs(L/2*std::sin((phi_max-phi)/180*M_PI)) );
          trk_len_clus                   += trk_len_pad ;

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
          float phiconv                   = fabs(phi)/phi_step ;
          // Interpolation d
          if(d < -L/2) d                  = -L/2 ;
          if(d >  L/2) d                  =  L/2 ;
          float dconv                     = (d+L/2)/d_step ;  // +L/2 shift because LUT indices have to be > 0 but d can be < 0
          // Interpolation Z
          if(z_calc < 0)    z_calc        = 0 ;
          if(z_calc > 1000) z_calc        = 1e3 ;
          float zfile                     = zdrift/z_step ;
          float zconv                     = z_calc/z_step ;
          // Interpolation RC
          if(RC_pad < 50)  RC_pad         = 50 ;
          if(RC_pad > 150) RC_pad         = 150 ;
          float RCconv                    = (RC_pad-50)/RC_step ;

          float ratio_zfile               = LUT.Interpolated(dconv, phiconv, zfile, RCconv) ;
          float ratio_zcalc               = LUT.Interpolated(dconv, phiconv, zconv, RCconv) ;
          // float ratio_zfile               = LUT3.Interpolated(dconv, phiconv, zfile) ;
          // float ratio_zcalc               = LUT3.Interpolated(dconv, phiconv, zconv) ;

          float ratio ;
          if(z_method == "zcalc") ratio   = ratio_zcalc ;
          if(z_method == "zfile") ratio   = ratio_zfile ;

          h1f_dist->                        Fill(trk_len_pad*1000) ;
          h1f_Lnorm->                       Fill(LNorm) ;
          h1f_dnorm->                       Fill(d/d_max) ;
          h2f_AmaxvsLength->                Fill(trk_len_pad*1000, PadAmaxCorr) ;
          h2f_QvsLength->                   Fill(trk_len_pad*1000, PadAmaxCorr*ratio) ;
          h2f_LUTvsLength->                 Fill(trk_len_pad*1000, ratio) ;
          h2f_lenVSd->                      Fill(trk_len_pad*1000, d) ;
          if(trk_len_pad <= len_cut)        continue ;
          v_h1f_WF_crossed.                 push_back(h1f_WF_pad) ;
          v_ratio.                          push_back(ratio) ;
          v_length.                         push_back(trk_len_pad) ;

          h1f_Tcross->                      Fill(Tmax) ;
          h1f_angle->                       Fill(phi) ;
          h1f_zdiff->                       Fill(zdrift - z_calc) ;
          h2f_ratiodiffZ->                  Fill(ratio_zcalc, ratio_zfile) ;


          // GPv6 & XP: List of pads to truncate (crossed pads wrt to DPR_max)
          RankedValue rank_DPR ;  
          rank_DPR.Rank                   = N_crossed ; 
          rank_DPR.Value                  = PadAmaxCorr*ratio/trk_len_pad ;  // DPR amplitude divided by track length in pad
          v_DPR.                            push_back(rank_DPR) ;

          N_crossed++ ;
        }

        if(trk_len_clus > 0){
          h1f_dist_clus->                   Fill(trk_len_clus*1000) ;
          h2f_QclvsLength->                 Fill(trk_len_clus*1000, pCluster->Get_Acluster()) ;
          h2f_WFvsLength->                  Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()) ;
          h1f_WFoLength->                   Fill(h1f_WF_cluster->GetMaximum()/(trk_len_clus*1000)) ;
        }

        WFsel                            += h1f_WF_cluster->GetMaximum()/track_len ;

        // // WF v1: Sum(WFmax)/track length
        // v_WF.                               push_back(h1f_WF_cluster->GetMaximum()) ;

        // // WF v3: Mean(WFmax/cluster length)
        // if(trk_len_clus > len_cut){
        //   v_WF.                              push_back(h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
        //   N_crosclus++ ;
        // }

        // WFoff: Mean(WFmax*ratio_corr)
        if(trk_len_clus > len_cut){
          float ratio_corr                = A_ref / A_corr->Eval(trk_len_clus*1000) ;
          h1f_WFcorr->                      Fill(ratio_corr) ;
          if(Tag.find("diag") != std::string::npos) v_WF.push_back(h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10) ;
          else                                      v_WF.push_back(h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
          v_lenclus.                        push_back(trk_len_clus) ;
          v_WFmax.                          push_back(h1f_WF_cluster->GetMaximum()) ;
          RankedValue rank_WF ;
          rank_WF.Rank                    = N_crosclus ;
          if(Tag.find("diag") != std::string::npos) rank_WF.Value = h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10 ;
          else                                      rank_WF.Value = h1f_WF_cluster->GetMaximum()/(trk_len_clus*100) ;
          // rank_WF.Value                   = h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10 ;
          v_rank_WF.                        push_back(rank_WF) ;
          N_crosclus++ ;
          // h2f_WFvsLength->                  Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10) ;
          if(Tag.find("diag") != std::string::npos) h2f_WFstarvsLen->Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10) ;
          else                                      h2f_WFstarvsLen->Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
        }

        // GPv3: cluster-based DPR
        TH1F* h1f_DPRcluster              = DPR("h1f_DPRcluster", -0.5, 509.5, h1f_WF_cluster->GetMaximumBin()-PT_TB, 510, iC, PT, TB) ;
        h1f_DPRcluster->                    Scale(h1f_WF_cluster->GetMaximum()) ;
        v_h1f_DPRcluster.                   push_back(h1f_DPRcluster) ;
        
        // if(pEvent->Get_EntryNber() == 25) DrawOut_ClusterWFDisplay(pCluster, OUTDIR_WF_Display, Tag, 1, PT, TB) ;
        delete                              h1f_WF_cluster ;
      }
      h1f_Ncross->                          Fill(N_crossed) ;

      int N_crossed_trc                   = int(floor(N_crossed * (alpha/100))) ;
      float Tmax_GP                       = h1f_GWF_mod->GetMaximumBin() ;
      TH1F* h1f_DPR                       = DPR("h1f_DPR", -0.5, 509.5, Tmax_GP-PT_TB, 510, (size-1), PT, TB) ;


      // Filling dE/dx histograms for each method
      // Qsel
      v_h1f_Qsel[iMod]->                    Fill(Qsel) ;

      // Qtrunc
      std::sort(v_Q.begin(), v_Q.end()) ;
      v_Q.resize(NClus_trunc) ;
      Qtrunc                              = accumulate(v_Q.begin(), v_Q.end(), 0.)/(track_len*(alpha/100)) ;
      v_h1f_Qtrunc[iMod]->                  Fill(Qtrunc) ;


      // WFsel
      v_h1f_WFsel[iMod]->                   Fill(WFsel) ;

      // // WF v1: Sum(WFmax)/track length
      // std::sort(v_WF.begin(), v_WF.end()) ;
      // v_WF.                                 resize(NClus_trunc) ;
      // WFtrunc                             = accumulate(v_WF.begin(), v_WF.end(), 0.)/(track_len*(NClus_trunc/NClusters)) ;
      // v_h1f_WFtrunc[iMod]->                 Fill(WFtrunc) ;

      // WF3 & WFoff
      int N_crosclustrunc                 = int(floor(N_crosclus * (alpha/100))) ;
      std::sort(v_WF.begin(), v_WF.end()) ;
      std::sort(v_rank_WF.begin(), v_rank_WF.end()) ;
      WFtrunc                             = std::accumulate(v_WF.begin(), v_WF.begin() + N_crosclustrunc, 0.) / N_crosclustrunc ;
      v_h1f_WFtrunc[iMod]->                 Fill(WFtrunc) ;
      for(int i=0; i<N_crosclustrunc; i++)  h2f_WFtruncvsLength->Fill(v_lenclus[v_rank_WF[i].Rank]*1000, v_WFmax[v_rank_WF[i].Rank]) ;
      for(int i=0; i<N_crosclustrunc; i++)  h2f_WFstartrcvsLen->Fill(v_lenclus[v_rank_WF[i].Rank]*1000,  v_rank_WF[v_rank_WF[i].Rank].Value) ;
      for(int i=0; i<N_crosclustrunc; i++)  trk_len_sum_2mm += v_lenclus[v_rank_WF[i].Rank]*1000 ;

      // GPsel
      float GPsel                         = h1f_GWF_mod->GetMaximum()/track_len ;
      v_h1f_GPsel[iMod]->                   Fill(GPsel) ;

      // GPv3
      std::sort(v_LP.begin(), v_LP.end()) ;
      TH1F* h1f_GPv3                      = new TH1F("h1f_GPv3", "h1f_GPv3", 510, -0.5, 509.5) ;
      h1f_GPv3->                            Add(h1f_GWF_mod) ;
      for(int iC = (int)NClus_trunc ; iC < NClusters ; iC++) h1f_GPv3->Add(v_h1f_DPRcluster[v_LP[iC].Rank], -1) ;
      float GPv3                          = h1f_GPv3->GetMaximum()/(track_len*(alpha/100)) ;
      v_h1f_GPv3[iMod]->                    Fill(GPv3) ;

      std::sort(v_DPR.begin(), v_DPR.end()) ;
      // GPv6
      float track_len_trc_v6              = track_len ;
      TH1F* h1f_GPv6                      = new TH1F("h1f_GPv6", "h1f_GPv6", 510, -0.5, 509.5) ;
      h1f_GPv6->                            Add(h1f_GWF_mod) ;
      for(int iP = (int)N_crossed_trc ; iP < (int)v_DPR.size() ; iP++){
        track_len_trc_v6                 -= v_length[v_DPR[iP].Rank]*100 ;
        float amax                        = v_h1f_WF_crossed[v_DPR[iP].Rank]->GetMaximum() ;
        h1f_GPv6->                          Add(h1f_DPR, -amax*v_ratio[v_DPR[iP].Rank]) ;
      }
      float GPv6                          = h1f_GPv6->GetMaximum()/(track_len_trc_v6) ;
      v_h1f_GPv6[iMod]->                    Fill(GPv6) ;

      // XP
      float track_len_trc_XP              = 0 ;
      float XP                            = 0 ;
      for(int iP = 0 ; iP < (int)N_crossed_trc ; iP++){
        track_len_trc_XP                 += v_length[v_DPR[iP].Rank]*100 ;
        XP                               += v_h1f_WF_crossed[v_DPR[iP].Rank]->GetMaximum()*v_ratio[v_DPR[iP].Rank] ;
      }
      XP                                 /= track_len_trc_XP ;
      v_h1f_XP[iMod]->                      Fill(XP) ;

      float XP_avg                        = 0 ;
      for(int iP = 0 ; iP < (int)N_crossed_trc ; iP++) XP_avg += v_DPR[iP].Value/100 ; // /100 to get it in ADC/cm and not ADC/m
      XP_avg                             /= N_crossed_trc ;
      // v_h1f_XP[iMod]->                      Fill(XP_avg) ;
      h1f_XPdiff->                          Fill(XP - XP_avg) ;


      // if(iEvent < 50) DrawOut_EventDisplay(pModule, OUTDIR_Evt_Display, Tag, "amplitude", pTrack->Get_ParameterValue(2), pTrack->Get_ParameterValue(1), pTrack->Get_ParameterValue(0)) ;

      // Methods comparisons
      h1f_Lmod1VScl->                       Fill(track_len*10*(alpha/100)                         - trk_len_sum_2mm) ;
      h1f_Lmod2VScl->                       Fill(track_len*10*((float)N_crosclustrunc/N_crosclus) - trk_len_sum_2mm) ;
      h1f_LallVScl->                        Fill(trk_len_sum                                      - 1000*std::accumulate(v_lenclus.begin(), v_lenclus.end(), 0.)) ;
      // v_h2f_XPvsWF[iMod]->                  Fill(WFtrunc, XP) ;
      // v_h2f_WFmWFvsWF[iMod]->               Fill(WFsel,   WFsel-WFtrunc) ;

      for(int i = 0 ; i < (int)v_h1f_DPRcluster.size() ; i++) {delete v_h1f_DPRcluster[i] ; v_h1f_DPRcluster[i] = 0 ; }
      for(int i = 0 ; i < (int)v_trashbin.size() ; i++)       {delete v_trashbin[i]       ; v_trashbin[i]       = 0 ; }
      v_h1f_DPRcluster.                     clear() ;
      v_trashbin.                           clear() ;
      v_h1f_WF_crossed.                     clear() ;
      delete                                h1f_DPR ;
      delete                                h1f_GWF_mod ;
      delete                                h1f_GPv3 ;
      delete                                h1f_GPv6 ;
      v_Q.clear() ; 
      v_WF.clear() ; 
      v_WFmax.clear() ; 
      v_LP.clear() ;
      v_DPR.clear() ;
      v_ratio.clear() ; 
      v_length.clear() ; 
      v_lenclus.clear() ; 
    }
    delete                                  pEvent ;
  }  
  aJFL_Selector.PrintStat() ;
  delete tf1_PRF ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting 
  std::cout << "Fitting: Started... " ; 

  // Get number of modules in this run (prototype or mockup?)
  Event* pEvent                         = pUploader->GiveMe_Event(0, NbrOfMod, Data_to_Use, 0) ;
  int nMod                              = pEvent->Get_NberOfModule() ;
  if (nMod != 1) nMod                   = 8 ;
  delete                                pEvent ;

  for (int iMod = 0 ; iMod < nMod ; iMod++){
    TF1* tf1_Qsel                       = Fit1Gauss(v_h1f_Qsel[iMod], 2) ;
    tf1_Qsel->                            SetNameTitle(Form("tf1_Qsel_%i", iMod), Form("tf1_Qsel_%i", iMod)) ;
    v_tf1_Qsel.                           push_back(tf1_Qsel) ;

    TF1* tf1_Qtrunc                     = Fit1Gauss(v_h1f_Qtrunc[iMod], 2) ;
    tf1_Qtrunc->                          SetNameTitle(Form("tf1_Qtrunc_%i", iMod), Form("tf1_Qtrunc_%i", iMod)) ;
    v_tf1_Qtrunc.                         push_back(tf1_Qtrunc) ;

    TF1* tf1_WFsel                      = Fit1Gauss(v_h1f_WFsel[iMod], 2) ;
    tf1_WFsel->                           SetNameTitle(Form("tf1_WFsel_%i", iMod), Form("tf1_WFsel_%i", iMod)) ;
    v_tf1_WFsel.                          push_back(tf1_WFsel) ;

    TF1* tf1_WFtrunc                    = Fit1Gauss(v_h1f_WFtrunc[iMod], 2) ;
    tf1_WFtrunc->                         SetNameTitle(Form("tf1_WFtrunc_%i", iMod), Form("tf1_WFtrunc_%i", iMod)) ;
    v_tf1_WFtrunc.                        push_back(tf1_WFtrunc) ;

    TF1* tf1_GPsel                      = Fit1Gauss(v_h1f_GPsel[iMod], 2) ;
    tf1_GPsel->                           SetNameTitle(Form("tf1_GPsel_%i", iMod), Form("tf1_GPsel_%i", iMod)) ;
    v_tf1_GPsel.                          push_back(tf1_GPsel) ;

    TF1* tf1_GPv3                       = Fit1Gauss(v_h1f_GPv3[iMod], 2) ;
    tf1_GPv3->                            SetNameTitle(Form("tf1_GPv3_%i", iMod), Form("tf1_GPv3_%i", iMod)) ;
    v_tf1_GPv3.                           push_back(tf1_GPv3) ;

    TF1* tf1_GPv6                       = Fit1Gauss(v_h1f_GPv6[iMod], 2) ;
    tf1_GPv6->                            SetNameTitle(Form("tf1_GPv6_%i", iMod), Form("tf1_GPv6_%i", iMod)) ;
    v_tf1_GPv6.                           push_back(tf1_GPv6) ;

    TF1* tf1_XP                         = Fit1Gauss(v_h1f_XP[iMod], 2) ;
    tf1_XP->                              SetNameTitle(Form("tf1_XP_%i", iMod), Form("tf1_XP_%i", iMod)) ;
    v_tf1_XP.                             push_back(tf1_XP) ;
  }
  std::cout << "done!" << std::endl ; 

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Saving //
  // Checks
  TFile* pfileROOT_checks = new TFile(TString(OUTDirName + "2_" + Tag + "_Checks" + Comment + ".root"), "RECREATE") ;
  std::cout << "Checks: " << OUTDirName + "2_" + Tag + "_Checks" + Comment + ".root" << std::endl ;
  h1f_Tcross->                              Write() ;
  h1f_Ncross->                              Write() ;
  h1f_zdiff->                               Write() ;
  h1f_angle->                               Write() ;
  h1f_Lnorm->                               Write() ;
  h1f_dnorm->                               Write() ;
  h1f_XPdiff->                              Write() ;
  h1f_dist->                                Write() ;
  h1f_dist_clus->                           Write() ;
  h1f_WFcorr->                              Write() ;
  h1f_Lmod1VScl->                           Write() ;
  h1f_Lmod2VScl->                           Write() ;
  h1f_LallVScl->                            Write() ;
  h2f_ratiodiffZ->                          Write() ;
  h2f_AmaxvsLength->                        Write() ;
  h2f_QvsLength->                           Write() ;
  h2f_LUTvsLength->                         Write() ;
  h2f_QclvsLength->                         Write() ;
  h2f_WFvsLength->                          Write() ;
  h2f_WFtruncvsLength->                     Write() ;
  h2f_WFstarvsLen->                         Write() ;
  h2f_WFstartrcvsLen->                      Write() ;
  h1f_WFoLength->                           Write() ;
  h2f_lenVSd->                              Write() ;
  A_corr->                                  Write() ;
  pfileROOT_checks->                        Close() ;  
  
  // Methods
  TFile* pfileROOT_status = new TFile(TString(OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root"), "RECREATE") ;
  std::cout << "dEdx: " << OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root" << std::endl ;
  for (int iMod = 0 ; iMod < 1 ; iMod++){
    v_tf1_Qsel[iMod]->                      Write() ;
    v_tf1_Qtrunc[iMod]->                    Write() ; 
    v_tf1_WFsel[iMod]->                     Write() ; 
    v_tf1_WFtrunc[iMod]->                   Write() ;
    v_tf1_GPsel[iMod]->                     Write() ;
    v_tf1_GPv3[iMod]->                      Write() ;
    v_tf1_GPv6[iMod]->                      Write() ;
    v_tf1_XP[iMod]->                        Write() ;
    v_h1f_Qsel[iMod]->                      Write() ;
    v_h1f_Qtrunc[iMod]->                    Write() ;  
    v_h1f_WFsel[iMod]->                     Write() ;
    v_h1f_WFtrunc[iMod]->                   Write() ;
    v_h1f_GPsel[iMod]->                     Write() ;
    v_h1f_GPv3[iMod]->                      Write() ;
    v_h1f_GPv6[iMod]->                      Write() ;
    v_h1f_XP[iMod]->                        Write() ;
    v_h2f_XPvsWF[iMod]->                    Write() ;
    v_h2f_WFmWFvsWF[iMod]->                 Write() ;
  }
  pfileROOT_status->Close() ;

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting //  
  delete h1f_Tcross ;
  delete h1f_Ncross ;
  delete h1f_zdiff ;
  delete h1f_angle ;
  delete h1f_Lnorm ;
  delete h1f_dnorm ;
  delete h1f_XPdiff ;
  delete h1f_dist ;
  delete h1f_dist_clus ;
  delete h1f_WFcorr ;
  delete h1f_Lmod1VScl ;
  delete h1f_Lmod2VScl ;
  delete h1f_LallVScl ;
  delete h2f_ratiodiffZ ;
  delete h2f_AmaxvsLength ;
  delete h2f_QvsLength ;
  delete h2f_LUTvsLength ;
  delete h2f_QclvsLength ;
  delete h2f_WFvsLength ;
  delete h2f_WFtruncvsLength ;
  delete h2f_WFstarvsLen ;
  delete h2f_WFstartrcvsLen ;
  delete h1f_WFoLength ;
  delete h2f_lenVSd ;
  delete pfileROOT_checks ;
  delete pfileROOT_status ;
  delete A_corr ;
  for(int iMod = 0 ; iMod < (int)v_tf1_Qsel.size() ; iMod++){
    delete v_tf1_Qsel[iMod] ;               v_tf1_Qsel[iMod]             = 0 ;
    delete v_tf1_Qtrunc[iMod] ;             v_tf1_Qtrunc[iMod]           = 0 ;
    delete v_tf1_WFsel[iMod] ;              v_tf1_WFsel[iMod]            = 0 ;
    delete v_tf1_WFtrunc[iMod] ;            v_tf1_WFtrunc[iMod]          = 0 ;
    delete v_tf1_GPsel[iMod] ;              v_tf1_GPsel[iMod]            = 0 ;
    delete v_tf1_GPv3[iMod] ;               v_tf1_GPv3[iMod]             = 0 ;
    delete v_tf1_GPv6[iMod] ;               v_tf1_GPv6[iMod]             = 0 ;
    delete v_tf1_XP[iMod] ;                 v_tf1_XP[iMod]               = 0 ;
  }
  for(int iMod = 0 ; iMod < (int)v_h1f_Qsel.size() ; iMod++){
    delete v_h1f_Qsel[iMod] ;               v_h1f_Qsel[iMod]             = 0 ;
    delete v_h1f_Qtrunc[iMod] ;             v_h1f_Qtrunc[iMod]           = 0 ;
    delete v_h1f_WFsel[iMod] ;              v_h1f_WFsel[iMod]            = 0 ;
    delete v_h1f_WFtrunc[iMod] ;            v_h1f_WFtrunc[iMod]          = 0 ;
    delete v_h1f_GPsel[iMod] ;              v_h1f_GPsel[iMod]            = 0 ;
    delete v_h1f_GPv3[iMod] ;               v_h1f_GPv3[iMod]             = 0 ;
    delete v_h1f_GPv6[iMod] ;               v_h1f_GPv6[iMod]             = 0 ;
    delete v_h1f_XP[iMod] ;                 v_h1f_XP[iMod]               = 0 ;
    
    delete v_h2f_XPvsWF[iMod] ;             v_h2f_XPvsWF[iMod]           = 0 ;
    delete v_h2f_WFmWFvsWF[iMod] ;          v_h2f_WFmWFvsWF[iMod]        = 0 ;
  }

  v_tf1_Qsel.                               clear() ; 
  v_tf1_Qtrunc.                             clear() ;
  v_tf1_WFsel.                              clear() ;
  v_tf1_WFtrunc.                            clear() ;
  v_tf1_GPsel.                              clear() ;
  v_tf1_GPv3.                               clear() ;
  v_tf1_GPv6.                               clear() ;
  v_tf1_XP.                                 clear() ;
  v_h1f_Qsel.                               clear() ; 
  v_h1f_Qtrunc.                             clear() ;
  v_h1f_WFsel.                              clear() ;
  v_h1f_WFtrunc.                            clear() ;
  v_h1f_GPsel.                              clear() ;
  v_h1f_GPv3.                               clear() ;
  v_h1f_GPv6.                               clear() ;
  v_h1f_XP.                                 clear() ;

  v_h2f_XPvsWF.                             clear() ;
  v_h2f_WFmWFvsWF.                          clear() ;


  std::cout.rdbuf(coutbuf) ; // Reset to standard output
}
