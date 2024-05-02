#include "PID/dEdx.h"
#include "PID/Tools.h"
#include "PID/ReadLUT_vROOT.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>

#include "TH2.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"

#include "Fitters/DoTracksReconstruction.h"
#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Fitters/TheFitterTrack.h"
#include "Misc/Util.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "SampleTools/ReadRCmap.h"
#include "SampleTools/ReadGainmap.h"
#include "Fitters/PRF_param.h"

void dEdx( const std::string& OutDir,
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
  MakeMyDir(OUTDirName) ;
  std::string OUTDIR_Evt_Display            = OUTDirName + "Evt_Display/" ;
  MakeMyDir(OUTDIR_Evt_Display) ;
  std::string OUTDIR_WF_Display             = OUTDirName + "WF_Display/" ;
  MakeMyDir(OUTDIR_WF_Display) ;

  // int nZ = 22 ;
  // Interpol3 LUT3(Form("~/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/LUT_RC115/", 310, PT), nZ ) ;

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
  std::string     OutPut_Analysis = OUTDirName + "3_" + Tag + "_dEdx_XP.log" ;
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

  // Get ERAM ID
  std::vector<std::string> eram_id ;
  if(Tag.find("DESY") != std::string::npos) eram_id.push_back("ERAM01");
  if(EventFile.find("ERAM18") != std::string::npos) eram_id.push_back("ERAM18");
  if(EventFile.find("All_ERAMS") != std::string::npos) eram_id = {"ERAM07", "ERAM01", "ERAM23", "ERAM02"};

  // Get Gain & RC maps
  std::vector<ReadRCmap*> RCmaps;
  std::vector<ReadGainmap*> Gainmaps;
  ReadRCmap *RCmap0, *RCmap1, *RCmap2, *RCmap3;
  ReadGainmap *Gainmap0, *Gainmap1, *Gainmap2, *Gainmap3;
  RCmaps.push_back(RCmap0);
  Gainmaps.push_back(Gainmap0);
  int n_maps = 1;
  if(EventFile.find("All_ERAMS") != std::string::npos){
    n_maps = 4;
    RCmaps.push_back(RCmap1); RCmaps.push_back(RCmap2); RCmaps.push_back(RCmap3); 
    Gainmaps.push_back(Gainmap1); Gainmaps.push_back(Gainmap2); Gainmaps.push_back(Gainmap3); 
  }
  for(int i = 0 ; i < n_maps ; i++){
    RCmaps[i] = new ReadRCmap(eram_id[i]);
    Gainmaps[i] = new ReadGainmap(eram_id[i]);
  }
  std::cout << "Gain & RC maps: loaded" << std::endl ;

  // Get average gain value
  std::vector<float> v_avg_G;
  int lowest, status;
  float avg_G, n_pads;
  for(int i = 0; i < (int)eram_id.size(); i++){
    status        = 0 ;
    n_pads        = 0 ;
    avg_G         = 0 ;
    lowest        = 9999 ;
    for(int iX = 0 ; iX < 36 ; iX++){
      for(int iY = 0 ; iY < 32 ; iY++){
        if(Gainmaps[i]->GetData(iX, iY, status) != 0){
          avg_G        += Gainmaps[i]->GetData(iX, iY, status) ;
          n_pads++ ;
        } 
        if(Gainmaps[i]->GetData(iX, iY, status) < lowest and Gainmaps[i]->GetData(iX, iY, status) > 0) lowest = Gainmaps[i]->GetData(iX, iY, status) ;
      }
    }
    // DrawOut_ERAMmaps(eram_id[i], Gainmaps[i]);
    avg_G /= n_pads ;
    v_avg_G.push_back(avg_G);
    std::cout << "Average Gain in " << eram_id[i] << " = " << avg_G << std::endl ;
  }
  avg_G = mean(v_avg_G);
  if(EventFile.find("All_ERAMS") != std::string::npos) std::cout << "Average Gain accross 4 modules = " << avg_G << std::endl ;


  // Fill holes in maps
  for(int i = 0; i < (int)eram_id.size(); i++){
    for(int iX = 0 ; iX < 36 ; iX++){
      for(int iY = 0 ; iY < 32 ; iY++){
        double RC_pad                   = RCmaps[i]->GetData(iX,iY, status) ;
        double G_pad                    = Gainmaps[i]->GetData(iX,iY, status) ;
        if(RC_pad == 0){
          std::cout << eram_id[i] << ": RC hole in   " <<  " iX = " << iX << " | iY = " << iY ; 
          float RC_left                 = RCmaps[i]->GetData(iX-1,iY,   status) ;
          float RC_right                = RCmaps[i]->GetData(iX+1,iY,   status) ;
          float RC_low                  = RCmaps[i]->GetData(iX,  iY-1, status) ;
          float RC_top                  = RCmaps[i]->GetData(iX,  iY+1, status) ;
          RC_pad                        = (RC_left + RC_right + RC_low + RC_top)/4 ;
          RCmaps[i]->                          SetData(iX,iY, RC_pad) ;
          std::cout << " | value reset at " << RC_pad << std::endl ;
        }
        if(G_pad == 0){
          std::cout << eram_id[i] << ": Gain hole in " << " iX = " << iX << " | iY = " << iY ; 
          float G_left                  = Gainmaps[i]->GetData(iX-1,iY,   status) ;
          float G_right                 = Gainmaps[i]->GetData(iX+1,iY,   status) ;
          float G_low                   = Gainmaps[i]->GetData(iX,  iY-1, status) ;
          float G_top                   = Gainmaps[i]->GetData(iX,  iY+1, status) ;
          G_pad                         = (G_left + G_right + G_low + G_top)/4 ;
          Gainmaps[i]->                        SetData(iX,iY, G_pad) ;
          std::cout << " | value reset at " << G_pad << std::endl ;
        }
      }
    }
  }


  
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

  std::cout << "zdrift        = " << zdrift << " mm"          << std::endl ;
  std::cout << "PT            = " << PT     << " ns"          << std::endl ;
  std::cout << "TB            = " << TB     << " ns/time bin" << std::endl ;
  std::cout << "PT/TB         = " << PT/TB  << " time bins"   << std::endl ;
  std::cout << "Track fitting : " << fitting                  << std::endl ;
  std::cout << "z method      : " << z_method                 << std::endl ;
  std::cout << "cut length    = " << len_cut*1e3 << " mm"     << std::endl ;
  std::cout <<                                                   std::endl ;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Check histograms
  TH1F* h1f_dnorm           = new TH1F("h1f_dnorm",           "Normalized impact parameter d/d_{max};d/d_{max};", 100, -1.1, 1.1) ;
  TH1F* h1f_dist            = new TH1F("h1f_dist",            "distance of track in pad;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_dist_clus       = new TH1F("h1f_dist_clus",       "Distance of track in cluster;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_Lnorm           = new TH1F("h1f_Lnorm",           "Length in pad normalized to maximum length in pad for a given #varphi;L_{#cap}/L_{#varphi};Count", 60, 0, 1.2012) ;
  TH1F* h1f_zdiff           = new TH1F("h1f_zdiff",           Form("z_{file = %.0fmm} - z_{computed};difference (mm);Count", zdrift), 80, -150, 150) ;
  TH1F* h1f_XPdiff          = new TH1F("h1f_XPdiff",          "#Sigma(Q)/#Sigma(length) - mean{Q_{i}/length_{i}};difference (ADC count);Count", 100, -150, 150) ;
  TH1F* h1f_Tcross          = new TH1F("h1f_Tcross",          "T_{max} of crossed pads;T (timbebin);Count", 511, -0.5, 510.5) ;
  TH1F* h1f_Ncross          = new TH1F("h1f_Ncross",          "Number of crossed pads;# of crossed pads;Count", 45, 15, 60) ;
  TH1F* h1f_angle           = new TH1F("h1f_angle",           "Angle #varphi in each pad;#varphi (#circ);Count", 66, -55, 10) ;
  TH1F* h1f_WFcorr          = new TH1F("h1f_WFcorr",          "Correction A_{max} ratio;ratio;Count", 80, 0, 3) ;
  TH1F* h1f_Lmod1VScl       = new TH1F("h1f_Lmod1VScl",       "L_{ERAM}*0.7 - #Sigma L_{clus>2mm};difference (mm);Count", 80, -60, 60) ;
  TH1F* h1f_Lmod2VScl       = new TH1F("h1f_Lmod2VScl",       "L_{ERAM}*(N_{trunc cross}/N_{clus cross>2mm}) - #Sigma L_{clus>2mm};difference (mm);Count", 80, -60, 60) ;
  TH1F* h1f_LallVScl        = new TH1F("h1f_LallVScl",        "L_{clusters} - L_{clusters > 2mm};difference (mm);Count", 80, -60, 60) ;
  TH2F* h2f_ratiodiffZ      = new TH2F("h2f_ratiodiffZ",      "LUT(z_{file}) vs LUT(z_{calc});LUT(z_{calc});LUT(z_{file})", 80, 0, 2.1, 80, 0, 2.1) ;
  TH2F* h2f_AmaxvsLength    = new TH2F("h2f_AmaxvsLength",    "ADC_{max} VS length in pad (before length cut);Length in pad (mm);ADC_{max}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_QvsLength       = new TH2F("h2f_QvsLength",       "Q^{anode} VS length in pad (before length cut);Length in pad (mm);Q^{anode}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_LUTvsLength     = new TH2F("h2f_LUTvsLength",     "Q^{anode}/ADC_{max} VS length in pad (before length cut);Length in pad (mm);Q^{anode}/ADC_{max}", 80, -0.1, 16, 80, -0.1, 2.1) ;
  TH2F* h2f_QclvsLength     = new TH2F("h2f_QclvsLength",     "Q_{cluster} VS length in cluster;Length in cluster (mm);Q_{cluster} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFvsLength      = new TH2F("h2f_WFvsLength",      "WF_{sum} VS length in cluster;Length in cluster (mm);WF_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFsumvsLength = new TH2F("h2f_WFsumvsLength", "WF_{sum truncated} VS length in cluster;Length in cluster (mm);WF_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstarvsLen     = new TH2F("h2f_WFstarvsLen",     "WF*_{sum} VS length in cluster;Length in cluster (mm);WF*_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstartrcvsLen  = new TH2F("h2f_WFstartrcvsLen",  "WF*_{sum truncated} VS length in cluster;Length in cluster (mm);WF*_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH1F* h1f_WFoLength       = new TH1F("h1f_WFoLength",       "A_{max}(WF_{sum}) / L_{cluster};A_{max}(WF_{sum}) / L_{cluster} (ADC count/mm);", 80, 0, 4000) ;
  TH2F* h2f_lenVSd          = new TH2F("h2f_lenVSd",          "impact parameter d vs length in pad;Length in pad (mm);impact parameter (mm)", 80, -0.1, 16, 80, -7.8, 7.8) ;

  // Method histograms
  TH1F* h1f_Qsel            = new TH1F("h1f_Qsel",            "<dE/dx> with Q_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_Qtrunc          = new TH1F("h1f_Qtrunc",          "<dE/dx> with Q_{cluster} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_WFsel           = new TH1F("h1f_WFsel",           "<dE/dx> with WF_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_WFsum         = new TH1F("h1f_WFsum",         "<dE/dx> with WF_{cluster} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  // TH1F* h1f_GPsel           = new TH1F("h1f_GPsel",           "<dE/dx> with GP_{not trunc} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  // TH1F* h1f_GPv3            = new TH1F("h1f_GPv3",            "<dE/dx> with GPv3 (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  // TH1F* h1f_GPv6            = new TH1F("h1f_GPv6",            "<dE/dx> with GPv6 (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_XP              = new TH1F("h1f_XP",              "<dE/dx> with XP (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // correction functoin & Track fit initializations
  TF1* A_corr                 = new TF1("A_corr", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
  if(Tag.find("diag") != std::string::npos){
    std::string filename      = EventFile.substr(0, EventFile.length()-5) ;
    int angle ;
    if( (angle = filename.find("30"))  != (int)std::string::npos or (angle = filename.find("45"))  != (int)std::string::npos) filename.replace(angle, 2, "40") ;
    while( (angle = filename.find("460")) != (int)std::string::npos or (angle = filename.find("860")) != (int)std::string::npos) filename.replace(angle, 3, "m40") ;
    TFile* pfile              = new TFile((filename + "_WFmax_correction.root").c_str(), "READ") ;
    std::cout << (filename + "_WFmax_correction.root").c_str() << std::endl ;
    A_corr                    = pfile->Get<TF1>("A_corr") ;
    pfile->                     Close() ;
    A_corr->                    SetParameter(0, A_corr->GetParameter(0)-100) ;
    std::cout << std::setprecision(2) << "WF correction parameters: " << A_corr->GetParameter(0) << " | " << A_corr->GetParameter(1) << " | " << A_corr->GetParameter(2) << " | " << A_corr->GetParameter(3) << " | " << A_corr->GetParameter(4) << std::endl ;
  }
  float A_ref                 = A_corr->Eval(Lx) ;

  TheFitterTrack aTheFitterTrack("Minuit", n_param_trk) ;
  PRF_param                     aPRF_param  ;
  TF1* tf1_PRF                = new TF1("tf1_PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5) ;
  tf1_PRF->                     SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4()) ;
  int Kounter_Fit             = 0 ;
  int Kounter_Fail            = 0 ;




  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Compute dE/dx
  aJFL_Selector.                          Reset_StatCounters() ;
  std::cout << "Processing events:" << std::endl ;
  for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 500 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                        = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    if (!pEvent)                            continue ;
    aJFL_Selector.                          ApplySelection(pEvent) ;
    if (pEvent->IsValid() != 1)             continue ;

    // Initialize event variables
    std::vector<TH1F*>                    v_h1f_DPRcluster, v_WF_cross_Evt, v_trashbin;
    int                                   Qsel,             Qtrunc,           WFsel,          WFsum ;
    int                                   N_cross_Evt = 0,      N_cross_trc_Evt = 0,  N_clust_Evt = 0,    N_clust_trc_Evt = 0,  N_cross_clust_Evt = 0,  N_cross_clust_trc_Evt = 0;
    std::vector<float>                    v_len_cross_Evt,  v_len_clust_Evt;
    float                                 len_track_Evt ;
    std::vector<float>                    v_Q,              v_WF,             v_WFmax ; 
    std::vector<RankedValue>              v_rank_WF,        v_rank_Cross ;
    std::vector<float>                    v_ratio_Evt ; 
    // std::vector<RankedValue>           v_LP;            

    // Loop On Modules
    int NberOfModule                      = pEvent->Get_NberOfModule() ;
    for (int iMod = 0 ; iMod < NberOfModule ; iMod++){
      Module* pModule                     = pEvent->Get_Module_InArray(iMod) ;
      if (pEvent->Validity_ForThisModule(iMod) == 0) continue ;

      // Track fitting (barycenter or PRF)
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
      std::vector<float>                    v_len_cross ; 
      std::vector<float>                    v_len_clust ; 
      float len_track                     = trk_len(pModule, pTrack)*100 ; // in cm from left side  of cluster #0 to right side of last cluster
      len_track_Evt                      += len_track;

      // Initialize module variables
      // TH1F* h1f_GWF_mod                   = new TH1F("h1f_GWF_mod", "h1f_GWF_mod", 510, -0.5, 509.5) ; // GWF in module
      float N_clus                        = pModule->Get_NberOfCluster() ;
      N_clust_Evt                        += N_clus;

      // Loop On Clusters
      for (int iC = 0 ; iC < N_clus ; iC++){
        TH1F* h1f_WF_cluster              = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ; // Store this cluster's WFs
        Cluster* pCluster                 = pModule->Get_Cluster(iC) ;
        Qsel                             += pCluster->Get_Acluster()/len_track ;
        v_Q.                                push_back(pCluster->Get_Acluster()) ;
        float trk_len_clus                = 0 ; // in meters

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads() ;
        for(int iP = 0 ; iP < NPads ; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP) ;
          int StatusRC = 0 ;
          int StatusG  = 0 ;
          double RC_pad                   = RCmaps[iMod]->GetData(pPad->Get_iX(),pPad->Get_iY(), StatusRC) ;
          double G_pad                    = Gainmaps[iMod]->GetData(pPad->Get_iX(),pPad->Get_iY(), StatusG) ;
          float Gcorr                     = avg_G/G_pad ;
          // float PadAmaxCorr               = pPad->Get_AMax() ;
          float PadAmaxCorr               = Gcorr*pPad->Get_AMax() ;

          TH1F* h1f_WF_pad                = GiveMe_WaveFormDisplay(pPad, "main") ;
          h1f_WF_pad->                      Scale(Gcorr) ;
          h1f_WF_cluster->                  Add(h1f_WF_pad) ;
          // h1f_GWF_mod->                     Add(h1f_WF_pad) ;
          v_trashbin.                       push_back(h1f_WF_pad) ;
          
          // // GPv3: List of pads to truncate (leading pads wrt ADC_max)
          // if(pPad == pCluster->Get_LeadingPad()){
          //   RankedValue rank_iC ;  
          //   rank_iC.Rank                  = iC ; 
          //   rank_iC.Value                 = PadAmaxCorr ;
          //   v_LP.                           push_back(rank_iC) ;
          // }

          // Track computations (impact parameter, angle, length in pad)
          std::vector<float> loc_par      = local_params(pPad, pTrack) ;
          float d                         = loc_par[0]*1000 ;       // in mm
          if(pTrack->GetNberOfParameters() > 2) phi = loc_par[1] ;  // in degrees, recompute only for curved tracks
          float trk_len_pad               = loc_par[2];             // in m
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
          v_WF_cross_Evt.                 push_back(h1f_WF_pad) ;
          v_ratio_Evt.                          push_back(ratio) ;
          v_len_cross.                      push_back(trk_len_pad) ;
          v_len_cross_Evt.                  push_back(trk_len_pad) ;

          h1f_Tcross->                      Fill(Tmax) ;
          h1f_angle->                       Fill(phi) ;
          h1f_zdiff->                       Fill(zdrift - z_calc) ;
          h2f_ratiodiffZ->                  Fill(ratio_zcalc, ratio_zfile) ;


          // GPv6 & XP: List of pads to truncate (crossed pads wrt to DPR_max)
          RankedValue rank_DPR ;  
          rank_DPR.Rank                   = N_cross_Evt ; 
          rank_DPR.Value                  = PadAmaxCorr*ratio/trk_len_pad ;  // DPR amplitude divided by track length in pad
          v_rank_Cross.                            push_back(rank_DPR) ;

          N_cross_Evt++ ;
        }

        if(trk_len_clus > 0){
          h1f_dist_clus->                   Fill(trk_len_clus*1000) ;
          h2f_QclvsLength->                 Fill(trk_len_clus*1000, pCluster->Get_Acluster()) ;
          h2f_WFvsLength->                  Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()) ;
          h1f_WFoLength->                   Fill(h1f_WF_cluster->GetMaximum()/(trk_len_clus*1000)) ;
        }

        WFsel                            += h1f_WF_cluster->GetMaximum()/len_track ;

        // // WF v1: Sum(WFmax)/track length
        // v_WF.                               push_back(h1f_WF_cluster->GetMaximum()) ;

        // // WF2: sum(WFmax)/sum(L_cluster)
        // if(trk_len_clus > len_cut){
        //   RankedValue rank_WF ;
        //   rank_WF.Rank                    = N_cross_clust_Evt ;
        //   rank_WF.Value                   = h1f_WF_cluster->GetMaximum()/(trk_len_clus*100) ;
        //   v_rank_WF.                        push_back(rank_WF) ;
        //   v_WF.                             push_back(h1f_WF_cluster->GetMaximum()) ;
        //   v_len_clust.                        push_back(trk_len_clus) ;
        //   N_cross_clust_Evt++ ;
        // }

        // // WF v3: Mean(WFmax/cluster length)
        // if(trk_len_clus > len_cut){
        //   v_WF.                              push_back(h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
        //   N_cross_clust_Evt++ ;
        // }

        // WFoff: Mean(WFmax*ratio_corr)
        if(trk_len_clus > len_cut){
          float ratio_corr                = A_ref / A_corr->Eval(trk_len_clus*1000) ;
          h1f_WFcorr->                      Fill(ratio_corr) ;
          if(Tag.find("diag") != std::string::npos) v_WF.push_back(h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10) ;
          else                                      v_WF.push_back(h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
          v_len_clust.                        push_back(trk_len_clus) ;
          v_len_clust_Evt.                    push_back(trk_len_clus) ;
          v_WFmax.                          push_back(h1f_WF_cluster->GetMaximum()) ;
          RankedValue rank_WF ;
          rank_WF.Rank                    = N_cross_clust_Evt ;
          if(Tag.find("diag") != std::string::npos) rank_WF.Value = h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10 ;
          else                                      rank_WF.Value = h1f_WF_cluster->GetMaximum()/(trk_len_clus*100) ;
          v_rank_WF.                        push_back(rank_WF) ;
          N_cross_clust_Evt++ ;
          if(Tag.find("diag") != std::string::npos) h2f_WFstarvsLen->Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10) ;
          else                                      h2f_WFstarvsLen->Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()/(trk_len_clus*100)) ;
        }

        // GPv3: cluster-based DPR
        TH1F* h1f_DPRcluster              = DPR("h1f_DPRcluster", -0.5, 509.5, h1f_WF_cluster->GetMaximumBin()-PT_TB, 510, iC, PT, TB) ;
        h1f_DPRcluster->                    Scale(h1f_WF_cluster->GetMaximum()) ;
        v_h1f_DPRcluster.                   push_back(h1f_DPRcluster) ;
        
        // if(pEvent->Get_EntryNber() == 25) DrawOut_ClusterWFDisplay(pCluster, OUTDIR_WF_Display, Tag, 1, PT, TB) ;
        delete                              h1f_WF_cluster ;
      } // Cluster

      // float Tmax_GP                       = h1f_GWF_mod->GetMaximumBin() ;
      // TH1F* h1f_DPR                       = DPR("h1f_DPR", -0.5, 509.5, Tmax_GP-PT_TB, 510, (size-1), PT, TB) ;
    } // Module

    h1f_Ncross->                            Fill(N_cross_Evt) ;

    N_cross_trc_Evt                       = int(floor(N_cross_Evt * (alpha/100))) ; 
    N_clust_trc_Evt                       = int(floor(N_clust_Evt * (alpha/100))) ; 


    // Filling dE/dx histograms for each method
    // Qsel
    h1f_Qsel->                    Fill(Qsel) ;

    // Qtrunc
    std::sort(v_Q.begin(), v_Q.end()) ;
    v_Q.resize(N_clust_trc_Evt) ;
    Qtrunc                              = accumulate(v_Q.begin(), v_Q.end(), 0.)/(len_track_Evt*(alpha/100)) ;
    h1f_Qtrunc->                  Fill(Qtrunc) ;


    // WFsel
    h1f_WFsel->                   Fill(WFsel) ;

    // // WF v1: Sum(WFmax)/track length
    // std::sort(v_WF.begin(), v_WF.end()) ;
    // v_WF.                                 resize(N_clus_trc) ;
    // WFsum                             = accumulate(v_WF.begin(), v_WF.end(), 0.)/(len_track*(N_clus_trc/N_clus)) ;
    // v_h1f_WFsum[iMod]->                 Fill(WFsum) ;

    // // WF2
    // int N_cross_clust_trc_Evt                 = int(floor(N_cross_clust_Evt * (alpha/100))) ;
    // std::sort(v_rank_WF.begin(), v_rank_WF.end()) ;
    // float lentrunc = 0 ;
    // for(int i = 0 ; i < N_cross_clust_trc_Evt ; i++){
    //   WFsum                          += v_WF[v_rank_WF[i].Rank] ;
    //   lentrunc                         +=v_len_clust[v_rank_WF[i].Rank] ;
    // }
    // WFsum                            /= (lentrunc*100) ;
    // v_h1f_WFsum[iMod]->                 Fill(WFsum) ;

    // WF3 & WFoff & WF4
    N_cross_clust_trc_Evt                 = int(floor(N_cross_clust_Evt * (alpha/100))) ;
    std::sort(v_WF.begin(), v_WF.end()) ;
    std::sort(v_rank_WF.begin(), v_rank_WF.end()) ;
    WFsum                             = std::accumulate(v_WF.begin(), v_WF.begin() + N_cross_clust_trc_Evt, 0.) / N_cross_clust_trc_Evt ;
    h1f_WFsum->                         Fill(WFsum) ;
    for(int i=0; i<N_cross_clust_trc_Evt; i++)  h2f_WFsumvsLength->Fill(v_len_clust_Evt[v_rank_WF[i].Rank]*1000, v_WFmax[v_rank_WF[i].Rank]) ;
    for(int i=0; i<N_cross_clust_trc_Evt; i++)  h2f_WFstartrcvsLen->Fill(v_len_clust_Evt[v_rank_WF[i].Rank]*1000,  v_rank_WF[v_rank_WF[i].Rank].Value) ;

    // // GPsel
    // float GPsel                         = h1f_GWF_mod->GetMaximum()/len_track_Evt ;
    // h1f_GPsel->                           Fill(GPsel) ;

    // // GPv3
    // std::sort(v_LP.begin(), v_LP.end()) ;
    // TH1F* h1f_GPv3                      = new TH1F("h1f_GPv3", "h1f_GPv3", 510, -0.5, 509.5) ;
    // h1f_GPv3->                            Add(h1f_GWF_mod) ;
    // for(int iC = (int)N_clus_trc ; iC < N_clus ; iC++) h1f_GPv3->Add(v_h1f_DPRcluster[v_LP[iC].Rank], -1) ;
    // float GPv3                          = h1f_GPv3->GetMaximum()/(len_track*(alpha/100)) ;
    // h1f_GPv3->                             Fill(GPv3) ;

    std::sort(v_rank_Cross.begin(), v_rank_Cross.end()) ;
    // // GPv6
    // float track_len_trc_v6              = len_track ;
    // TH1F* h1f_GPv6                      = new TH1F("h1f_GPv6", "h1f_GPv6", 510, -0.5, 509.5) ;
    // h1f_GPv6->                            Add(h1f_GWF_mod) ;
    // for(int iP = (int)N_cross_trc_Evt ; iP < (int)v_rank_Cross.size() ; iP++){
    //   track_len_trc_v6                 -= v_len_cross[v_rank_Cross[iP].Rank]*100 ;
    //   float amax                        = v_WF_cross_Evt[v_rank_Cross[iP].Rank]->GetMaximum() ;
    //   h1f_GPv6->                          Add(h1f_DPR, -amax*v_ratio_Evt[v_rank_Cross[iP].Rank]) ;
    // }
    // float GPv6                          = h1f_GPv6->GetMaximum()/(track_len_trc_v6) ;
    // h1f_GPv6->                             Fill(GPv6) ;

    // XP
    float track_len_trc_XP              = 0 ;
    float XP                            = 0 ;
    for(int iP = 0 ; iP < (int)N_cross_trc_Evt ; iP++){
      track_len_trc_XP                 += v_len_cross_Evt[v_rank_Cross[iP].Rank]*100 ;
      XP                               += v_WF_cross_Evt[v_rank_Cross[iP].Rank]->GetMaximum()*v_ratio_Evt[v_rank_Cross[iP].Rank] ;
    }
    XP                                 /= track_len_trc_XP ;
    h1f_XP->                               Fill(XP) ;

    // float XP_avg                        = 0 ;
    // for(int iP = 0 ; iP < (int)N_cross_trc_Evt ; iP++) XP_avg += v_rank_Cross[iP].Value/100 ; // /100 to get it in ADC/cm and not ADC/m
    // XP_avg                             /= N_cross_trc_Evt ;
    // // v_h1f_XP[iMod]->                      Fill(XP_avg) ;
    // h1f_XPdiff->                          Fill(XP - XP_avg) ;


    // if(iEvent < 50) DrawOut_EventDisplay(pModule, OUTDIR_Evt_Display, Tag, "amplitude", pTrack->Get_ParameterValue(2), pTrack->Get_ParameterValue(1), pTrack->Get_ParameterValue(0)) ;


    for(int i = 0 ; i < (int)v_h1f_DPRcluster.size() ; i++) {delete v_h1f_DPRcluster[i] ; v_h1f_DPRcluster[i] = 0 ; }
    for(int i = 0 ; i < (int)v_trashbin.size() ; i++)       {delete v_trashbin[i]       ; v_trashbin[i]       = 0 ; }
    v_h1f_DPRcluster.                     clear() ;
    v_trashbin.                           clear() ;
    v_WF_cross_Evt.                     clear() ;
    // delete                                h1f_GPv3 ;
    // delete                                h1f_GPv6 ;
    v_Q.clear() ; 
    v_WF.clear() ; 
    v_WFmax.clear() ; 
    // v_LP.clear() ;
    v_rank_Cross.clear() ;
    v_ratio_Evt.clear() ; 
    v_len_cross_Evt.clear() ; 
    v_len_clust_Evt.clear() ; 
    delete                                  pEvent ;
  }
  aJFL_Selector.PrintStat() ;
  delete tf1_PRF ;




  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting 
  std::cout << "Fitting: Started... " ; 

  TF1* tf1_Qsel                       = Fit1Gauss(h1f_Qsel, 2) ;
  tf1_Qsel->                            SetNameTitle("tf1_Qsel", "tf1_Qsel") ;

  TF1* tf1_Qtrunc                     = Fit1Gauss(h1f_Qtrunc, 2) ;
  tf1_Qtrunc->                          SetNameTitle("tf1_Qtrunc", "tf1_Qtrunc") ;

  TF1* tf1_WFsel                      = Fit1Gauss(h1f_WFsel, 2) ;
  tf1_WFsel->                           SetNameTitle("tf1_WFsel", "tf1_WFsel") ;

  TF1* tf1_WFsum                    = Fit1Gauss(h1f_WFsum, 2) ;
  tf1_WFsum->                         SetNameTitle("tf1_WFsum", "tf1_WFsum") ;

  // TF1* tf1_GPsel                      = Fit1Gauss(h1f_GPsel, 2) ;
  // tf1_GPsel->                           SetNameTitle("tf1_GPsel", "tf1_GPsel") ;

  // TF1* tf1_GPv3                       = Fit1Gauss(h1f_GPv3, 2) ;
  // tf1_GPv3->                            SetNameTitle("tf1_GPv3", "tf1_GPv3") ;

  // TF1* tf1_GPv6                       = Fit1Gauss(h1f_GPv6, 2) ;
  // tf1_GPv6->                            SetNameTitle("tf1_GPv6", "tf1_GPv6") ;

  TF1* tf1_XP                         = Fit1Gauss(h1f_XP, 2) ;
  tf1_XP->                              SetNameTitle("tf1_XP", "tf1_XP") ;

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
  h2f_WFsumvsLength->                     Write() ;
  h2f_WFstarvsLen->                         Write() ;
  h2f_WFstartrcvsLen->                      Write() ;
  h1f_WFoLength->                           Write() ;
  h2f_lenVSd->                              Write() ;
  A_corr->                                  Write() ;
  pfileROOT_checks->                        Close() ;  
  
  // Methods
  TFile* pfileROOT_status = new TFile(TString(OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root"), "RECREATE") ;
  std::cout << "dEdx: " << OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root" << std::endl ;
  tf1_Qsel->                      Write() ;
  tf1_Qtrunc->                    Write() ; 
  tf1_WFsel->                     Write() ; 
  tf1_WFsum->                   Write() ;
  // tf1_GPsel->                     Write() ;
  // tf1_GPv3->                      Write() ;
  // tf1_GPv6->                      Write() ;
  tf1_XP->                        Write() ;
  h1f_Qsel->                      Write() ;
  h1f_Qtrunc->                    Write() ;  
  h1f_WFsel->                     Write() ;
  h1f_WFsum->                   Write() ;
  // h1f_GPsel->                     Write() ;
  // h1f_GPv3->                      Write() ;
  // h1f_GPv6->                      Write() ;
  h1f_XP->                        Write() ;

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
  delete h2f_WFsumvsLength ;
  delete h2f_WFstarvsLen ;
  delete h2f_WFstartrcvsLen ;
  delete h1f_WFoLength ;
  delete h2f_lenVSd ;
  delete pfileROOT_checks ;
  delete pfileROOT_status ;
  delete A_corr ;

  delete tf1_Qsel ;
  delete tf1_Qtrunc ;
  delete tf1_WFsel ;
  delete tf1_WFsum ;
  // delete tf1_GPsel ;
  // delete tf1_GPv3 ;
  // delete tf1_GPv6 ;
  delete tf1_XP ;
  delete h1f_Qsel ;
  delete h1f_Qtrunc ;
  delete h1f_WFsel ;
  delete h1f_WFsum ;
  // delete h1f_GPsel ;
  // delete h1f_GPv3 ;
  // delete h1f_GPv6 ;
  delete h1f_XP ;

  for(int i = 0; i < (int)RCmaps.size(); i++){
    delete RCmaps[i];
    delete Gainmaps[i];
  }
  RCmaps.     clear();
  Gainmaps.   clear();

  std::cout.rdbuf(coutbuf) ; // Reset to standard output
}
