#include "Tristan/dEdx.h"
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
#include "SampleTools/ReadRCmap.h"
#include "SampleTools/ReadGainmap.h"
#include "SignalShape/PRF_param.h"

void dEdx( const std::string& OutDir,
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

  // int nZ = 21 ;
  // Interpol3 LUT3(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/LUT_RC115/", 310, PT), nZ ) ;

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
  int   dsize           = 200 ;                                 // LUT definition along d
  int   phisize         = 200 ;                                 // LUT definition along phi
  float d_step          = L/(dsize-1) ;                          // d   increment between LUTs
  float phi_step        = (90-2e-6)/(phisize-1) ;                  // phi increment between LUTs
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
  std::cout << "FileName     : " << FileName    << std::endl ;
  std::cout << "SelectionSet  : " << SelectionSet << std::endl ;
  std::cout << "NbrOfMod      : " << NbrOfMod     << std::endl ;
  std::cout << "Data_to_Use   : " << Data_to_Use  << std::endl ;
  std::cout << "OUTDirName    : " << OUTDirName   << std::endl ;
  std::cout <<                                       std::endl ;

  // Get ERAM ID
  std::vector<std::string>  eram_id ;
  std::vector<double>       eram_number ;
  if(Tag.find("DESY") != std::string::npos){        eram_id.push_back("ERAM01"); eram_number.push_back(1); }
  if(FileName.find("ERAM18") != std::string::npos){ eram_id.push_back("ERAM18"); eram_number.push_back(18);}
  if(FileName.find("All_ERAMS") != std::string::npos){
    eram_id     = {"ERAM07", "ERAM01", "ERAM23", "ERAM02", "ERAM16", "ERAM15", "ERAM10", "ERAM12"};
    eram_number = {7, 1, 23, 2, 16, 15, 10, 12};
  }
  // Get Gain & RC maps
  std::vector<ReadRCmap*> RCmaps;
  std::vector<ReadGainmap*> Gainmaps;
  LoadMaps(FileName, RCmaps, Gainmaps, eram_id);

  // Get average gain value
  float avg_G = avg_Gain(FileName, eram_id, Gainmaps);


  // Fill holes in maps
  Fill_Maps(RCmaps, Gainmaps, eram_id);

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

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Check histograms
  TH1F* h1f_dnorm           = new TH1F("h1f_dnorm",           "Normalized impact parameter d/d_{max};d/d_{max};", 100, -1.1, 1.1) ;
  TH1F* h1f_dist            = new TH1F("h1f_dist",            "distance of track in pad;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_dist_clus       = new TH1F("h1f_dist_clus",       "Distance of track in cluster;distance (mm);", 100, 0, 17) ;
  TH1F* h1f_Lnorm           = new TH1F("h1f_Lnorm",           "Length in pad normalized to maximum length in pad for a given #varphi;L_{#cap}/L_{#varphi};Count", 60, 0, 1.2012) ;
  TH1F* h1f_zdiff           = new TH1F("h1f_zdiff",           Form("z_{file = %.0fmm} - z_{computed};difference (mm);Count", zdrift), 80, -150, 150) ;
  TH1F* h1f_Tcross          = new TH1F("h1f_Tcross",          "T_{max} of crossed pads;T (timbebin);Count", 511, -0.5, 510.5) ;
  TH1F* h1f_Ncross          = new TH1F("h1f_Ncross",          "Number of crossed pads;# of crossed pads;Count", 45, 15, 60) ;
  TH1F* h1f_angle           = new TH1F("h1f_angle",           "Angle #varphi in each pad;#varphi (#circ);Count", 66, -55, 10) ;
  TH1F* h1f_Gcorr           = new TH1F("h1f_Gcorr",           "Gain correction ratio;ratio;Count", 80, 0, 2) ;
  TH1F* h1f_WFcorr          = new TH1F("h1f_WFcorr",          "Correction A_{max} ratio;ratio;Count", 80, 0, 3) ;
  TH2F* h2f_dEdx_Y          = new TH2F("h2f_dEdx_Y",          "Y position VS dE/dx;dE/dx (ADC counts);Y position (cm)", 160, 0, 2000, 80, 0, 10) ;
  TH2F* h2f_ratiodiffZ      = new TH2F("h2f_ratiodiffZ",      "LUT(z_{file}) vs LUT(z_{calc});LUT(z_{calc});LUT(z_{file})", 80, 0, 2.1, 80, 0, 2.1) ;
  TH2F* h2f_AmaxvsLength    = new TH2F("h2f_AmaxvsLength",    "ADC_{max} VS length in pad (before length cut);Length in pad (mm);ADC_{max}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_QvsLength       = new TH2F("h2f_QvsLength",       "Q^{anode} VS length in pad (before length cut);Length in pad (mm);Q^{anode}", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_LUTvsLength     = new TH2F("h2f_LUTvsLength",     "Q^{anode}/ADC_{max} VS length in pad (before length cut);Length in pad (mm);Q^{anode}/ADC_{max}", 80, -0.1, 16, 80, -0.1, 2.1) ;
  TH2F* h2f_WFvsLength      = new TH2F("h2f_WFvsLength",      "WF_{sum} VS length in cluster;Length in cluster (mm);WF_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFsumvsLength   = new TH2F("h2f_WFsumvsLength",   "WF_{sum truncated} VS length in cluster;Length in cluster (mm);WF_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstarvsLen     = new TH2F("h2f_WFstarvsLen",     "WF*_{sum} VS length in cluster;Length in cluster (mm);WF*_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH2F* h2f_WFstartrcvsLen  = new TH2F("h2f_WFstartrcvsLen",  "WF*_{sum truncated} VS length in cluster;Length in cluster (mm);WF*_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100) ;
  TH1F* h1f_WFoLength       = new TH1F("h1f_WFoLength",       "A_{max}(WF_{sum}) / L_{cluster};A_{max}(WF_{sum}) / L_{cluster} (ADC count/mm);", 80, 0, 4000) ;
  TH2F* h2f_lenVSd          = new TH2F("h2f_lenVSd",          "impact parameter d vs length in pad;Length in pad (mm);impact parameter (mm)", 80, -0.1, 16, 80, -7.8, 7.8) ;

  // Method histograms
  TH1F* h1f_WFsel           = new TH1F("h1f_WFsel",           "<dE/dx> with WF_{sum} not truncated (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_WFsum           = new TH1F("h1f_WFsum",           "<dE/dx> with WF_{sum} (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;
  TH1F* h1f_XP              = new TH1F("h1f_XP",              "<dE/dx> with XP (module %i);<dE/dx> (ADC count);Number of events", 90, 0, 1800) ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // correction function & Track fit initializations
  TF1* A_corr                 = corr_func(FileName, Tag);
  float A_ref                 = A_corr->Eval(Lx) ;

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
  int n_real_evt = 0 ;
  for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                        = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    if (!pEvent)                            continue ;
    n_real_evt++;
    aJFL_Selector.                          ApplySelection(pEvent) ;
    if (pEvent->IsValid() != 1)             continue ;

    // Initialize event variables
    std::vector<TH1F*>                    v_WF_cross_Evt,   v_trashbin;
    int                                   WFsel = 0,        WFsum = 0;
    int                                   N_cross_Evt = 0,  N_cross_trc_Evt = 0,  N_clust_Evt = 0,    N_clust_trc_Evt = 0,  N_cross_clust_Evt = 0,  N_cross_clust_trc_Evt = 0;
    std::vector<float>                    v_len_cross_Evt,  v_len_clust_Evt;
    float                                 len_track_Evt = 0;
    std::vector<float>                    v_WF,             v_WFmax ; 
    std::vector<RankedValue>              v_rank_WF,        v_rank_Cross ;
    std::vector<float>                    v_ratio_Evt ; 
    std::vector<float>                    v_ordo_origin ; 

    // Loop On Modules
    int nMod                              = pEvent->Get_NberOfModule() ;

    // Check if there are is a track in 4 ERAMs that are aligned
    std::vector<double> eram_list;
    for (int iMod = 0 ; iMod < nMod ; iMod++){
      int ModuleNber                      = pEvent->Get_Module_InArray(iMod)->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber)!=0) eram_list.push_back(eram_number[ModuleNber]) ;
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

    for (int iMod = 0 ; iMod < nMod ; iMod++){
      Module* pModule                     = pEvent->Get_Module_InArray(iMod) ;
      int ModuleNber                      = pModule->Get_ModuleNber() ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      if(FileName.find("All_ERAMS") != std::string::npos){
        if(eram_number[ModuleNber] != nber) continue; // 1 ERAM
        // if(eram_number[ModuleNber] != 7 and eram_number[ModuleNber] != 1 and eram_number[ModuleNber] != 16 and eram_number[ModuleNber] != 10) continue; // 2 ERAMs
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
      std::vector<float>                    v_len_cross ; 
      std::vector<float>                    v_len_clust ; 
      float len_track                     = trk_len(pModule, pTrack)*100/costheta ; // in cm from left side  of cluster #0 to right side of last cluster
      len_track_Evt                      += len_track;
      v_ordo_origin.                        push_back(pTrack->Get_ParameterValue(0)*100);

      // Loop On Clusters
      for (int iC = 0 ; iC < N_clus ; iC++){
        TH1F* h1f_WF_cluster              = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ; // Store this cluster's WFs
        Cluster* pCluster                 = pModule->Get_Cluster(iC) ;
        float trk_len_clus                = 0 ; // in meters

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads() ;
        for(int iP = 0 ; iP < NPads ; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP) ;
          TH1F* h1f_WF_pad                = GiveMe_WaveFormDisplay(pPad, "main") ;
          float A_pad;
          int StatusRC = 0, StatusG  = 0 ;
          double RC_pad                   = RCmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY(), StatusRC) ;
          if(gain_corr){
            double G_pad                  = Gainmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY(), StatusG) ;
            float Gcorr                   = avg_G/G_pad ;
            h1f_Gcorr->                     Fill(Gcorr);
            A_pad                         = Gcorr*pPad->Get_AMax() ;
            h1f_WF_pad->                    Scale(Gcorr) ;
          }
          else A_pad                      = pPad->Get_AMax() ;

          h1f_WF_cluster->                  Add(h1f_WF_pad) ;
          v_trashbin.                       push_back(h1f_WF_pad) ;

          // Track computations (impact parameter in m, angle in degrees, length in pad in m)
          float d=0, dd=0, trk_len_pad=0;
          local_params(pPad, pTrack, d, dd, phi, trk_len_pad);
          d                              *= 1000 ; // in mm
          dd                             *= 1000 ; // in mm
          if(trk_len_pad <= 1e-6)           continue ;  // ignore non-but-almost-zero tracks
          trk_len_pad                    /= costheta;
          
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
          // d                              += dd;
          if(d >  L/2) d                  =  L/2 ;
          if(d < -L/2) d                  = -L/2 ;
          // float dconv                     = (d+L/2)/d_step ;  // +L/2 shift because LUT indices have to be > 0 but d can be < 0
          float absd                      = fabs(d) ;
          // Interpolation Z
          // z_calc                         += 15;
          if(z_calc < 0)    z_calc        = 0 ;
          if(z_calc > 1000) z_calc        = 1e3 ;
          float zfile                     = zdrift/z_step ;
          float zconv                     = z_calc/z_step ;
          // Interpolation RC
          // RC_pad                         *= 1.02;
          if(RC_pad < 50)  RC_pad         = 50 ;
          if(RC_pad > 150) RC_pad         = 150 ;
          float RCconv                    = (RC_pad-50)/RC_step ;

          float absphi                    = fabs(phi);
          float ratio_zcalc               = LUT.Interpolate(absd, absphi, zconv, RCconv) ;
          float ratio_zfile               = LUT.Interpolate(absd, absphi, zfile, RCconv) ;
          // float ratio_zfile               = LUT3.Interpolate(dconv, phiconv, zfile) ;
          // float ratio_zcalc               = LUT3.Interpolate(dconv, phiconv, zconv) ;

          float ratio ;
          if(z_method == "zcalc") ratio   = ratio_zcalc ;
          if(z_method == "zfile") ratio   = ratio_zfile ;

          h1f_dist->                        Fill(trk_len_pad*1000) ;
          h1f_Lnorm->                       Fill(LNorm) ;
          h1f_dnorm->                       Fill(d/d_max) ;
          h2f_AmaxvsLength->                Fill(trk_len_pad*1000, A_pad) ;
          h2f_QvsLength->                   Fill(trk_len_pad*1000, A_pad*ratio) ;
          h2f_LUTvsLength->                 Fill(trk_len_pad*1000, ratio) ;
          h2f_lenVSd->                      Fill(trk_len_pad*1000, d) ;
          if(trk_len_pad <= len_cut)        continue ;
          v_WF_cross_Evt.                   push_back(h1f_WF_pad) ;
          v_ratio_Evt.                      push_back(ratio) ;
          v_len_cross.                      push_back(trk_len_pad) ;
          v_len_cross_Evt.                  push_back(trk_len_pad) ;

          h1f_Tcross->                      Fill(Tmax) ;
          h1f_angle->                       Fill(phi) ;
          h1f_zdiff->                       Fill(zdrift - z_calc) ;
          h2f_ratiodiffZ->                  Fill(ratio_zcalc, ratio_zfile) ;


          // XP: List of pads to truncate (crossed pads wrt to DPR_max)
          RankedValue rank_Qanode ;  
          rank_Qanode.Rank                = N_cross_Evt ; 
          rank_Qanode.Value               = A_pad*ratio/trk_len_pad ;  // DPR amplitude divided by track length in pad
          v_rank_Cross.                     push_back(rank_Qanode) ;

          N_cross_Evt++ ;
        }

        if(trk_len_clus > 0){
          h1f_dist_clus->                   Fill(trk_len_clus*1000) ;
          h2f_WFvsLength->                  Fill(trk_len_clus*1000, h1f_WF_cluster->GetMaximum()) ;
          h1f_WFoLength->                   Fill(h1f_WF_cluster->GetMaximum()/(trk_len_clus*1000)) ;
        }

        WFsel                            += h1f_WF_cluster->GetMaximum() ;

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

        // WFoff & WF4: Mean(WFmax*ratio_corr) (difference between off & v4: correction function)
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

        // if(pEvent->Get_EntryNber() < 25) DrawOut_ClusterWFDisplay(pCluster, OUTDIR_WF_Display, Tag, 1, PT, TB) ;
        delete                              h1f_WF_cluster ;
      } // Cluster

      // if(iEvent < 50) DrawOut_EventDisplay(pModule, OUTDIR_Evt_Display, Tag, "amplitude", pTrack->Get_ParameterValue(2), pTrack->Get_ParameterValue(1), pTrack->Get_ParameterValue(0)) ;
    } // Module

    if(N_cross_Evt > 0){
      h1f_Ncross->                            Fill(N_cross_Evt) ;

      N_cross_trc_Evt                       = int(floor(N_cross_Evt * (alpha/100))) ; 
      N_clust_trc_Evt                       = int(floor(N_clust_Evt * (alpha/100))) ; 


      // Filling dE/dx histograms for each method

      // WFsel
      h1f_WFsel->                   Fill(WFsel/len_track_Evt) ;

      // // WF v1: Sum(WFmax)/track length
      // std::sort(v_WF.begin(), v_WF.end()) ;
      // v_WF.                                 resize(N_clust_trc_Evt) ;
      // WFsum                             = accumulate(v_WF.begin(), v_WF.end(), 0.)/(len_track_Evt*(N_clust_trc_Evt/N_clust_Evt)) ;
      // h1f_WFsum->                         Fill(WFsum) ;

      // // WF2
      // int N_cross_clust_trc_Evt                 = int(floor(N_cross_clust_Evt * (alpha/100))) ;
      // std::sort(v_rank_WF.begin(), v_rank_WF.end()) ;
      // float lentrunc = 0 ;
      // for(int i = 0 ; i < N_cross_clust_trc_Evt ; i++){
      //   WFsum                          += v_WF[v_rank_WF[i].Rank] ;
      //   lentrunc                         +=v_len_clust_Evt[v_rank_WF[i].Rank] ;
      // }
      // WFsum                            /= (lentrunc*100) ;
      // h1f_WFsum->                 Fill(WFsum) ;

      // WF3 & WFoff & WF4
      N_cross_clust_trc_Evt                 = int(floor(N_cross_clust_Evt * (alpha/100))) ;
      std::sort(v_WF.begin(), v_WF.end()) ;
      std::sort(v_rank_WF.begin(), v_rank_WF.end()) ;
      WFsum                             = std::accumulate(v_WF.begin(), v_WF.begin() + N_cross_clust_trc_Evt, 0.) / N_cross_clust_trc_Evt ;
      h1f_WFsum->                         Fill(WFsum) ;
      for(int i=0; i<N_cross_clust_trc_Evt; i++)  h2f_WFsumvsLength->Fill(v_len_clust_Evt[v_rank_WF[i].Rank]*1000, v_WFmax[v_rank_WF[i].Rank]) ;
      for(int i=0; i<N_cross_clust_trc_Evt; i++)  h2f_WFstartrcvsLen->Fill(v_len_clust_Evt[v_rank_WF[i].Rank]*1000,  v_rank_WF[v_rank_WF[i].Rank].Value) ;

      // XP
      float track_len_trc_XP              = 0 ;
      float XP                            = 0 ;
      std::sort(v_rank_Cross.begin(), v_rank_Cross.end()) ;
      for(int iP = 0 ; iP < (int)N_cross_trc_Evt ; iP++){
        track_len_trc_XP                 += v_len_cross_Evt[v_rank_Cross[iP].Rank]*100 ;
        XP                               += v_WF_cross_Evt[v_rank_Cross[iP].Rank]->GetMaximum()*v_ratio_Evt[v_rank_Cross[iP].Rank] ;
      }
      XP                                 /= track_len_trc_XP ;
      h1f_XP->                               Fill(XP) ;

      for(int i = 0 ; i<(int)v_ordo_origin.size();i++) h2f_dEdx_Y->Fill(XP, v_ordo_origin[i]);
    }


    for(int i = 0 ; i < (int)v_trashbin.size() ; i++)       {delete v_trashbin[i]       ; v_trashbin[i]       = 0 ; }
    v_trashbin.                           clear() ;
    v_WF_cross_Evt.                     clear() ;
    v_WF.clear() ; 
    v_WFmax.clear() ; 
    v_rank_Cross.clear() ;
    v_ratio_Evt.clear() ; 
    v_len_cross_Evt.clear() ; 
    v_len_clust_Evt.clear() ; 
    eram_list.clear();
    delete                                  pEvent ;
  }
  std::cout << n_real_evt << " real events" << std::endl;
  aJFL_Selector.PrintStat() ;
  delete tf1_PRF ;


  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting 
  std::cout << "Fitting: Started... " ; 

  TF1* tf1_WFsel                      = Fit1Gauss(h1f_WFsel, 2) ;
  tf1_WFsel->                           SetNameTitle("tf1_WFsel", "tf1_WFsel") ;

  TF1* tf1_WFsum                    = Fit1Gauss(h1f_WFsum, 2) ;
  tf1_WFsum->                         SetNameTitle("tf1_WFsum", "tf1_WFsum") ;

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
  h1f_dist->                                Write() ;
  h1f_dist_clus->                           Write() ;
  h1f_WFcorr->                              Write() ;
  h1f_Gcorr->                               Write() ;
  h2f_dEdx_Y->                              Write() ;
  h2f_ratiodiffZ->                          Write() ;
  h2f_AmaxvsLength->                        Write() ;
  h2f_QvsLength->                           Write() ;
  h2f_LUTvsLength->                         Write() ;
  h2f_WFvsLength->                          Write() ;
  h2f_WFsumvsLength->                       Write() ;
  h2f_WFstarvsLen->                         Write() ;
  h2f_WFstartrcvsLen->                      Write() ;
  h1f_WFoLength->                           Write() ;
  h2f_lenVSd->                              Write() ;
  A_corr->                                  Write() ;
  pfileROOT_checks->                        Close() ;  
  
  // Methods
  TFile* pfileROOT_status = new TFile(TString(OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root"), "RECREATE") ;
  std::cout << "dEdx: " << OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root" << std::endl ;
  tf1_WFsel->                     Write() ; 
  tf1_WFsum->                     Write() ;
  tf1_XP->                        Write() ;
  h1f_WFsel->                     Write() ;
  h1f_WFsum->                     Write() ;
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
  delete h1f_dist ;
  delete h1f_dist_clus ;
  delete h1f_WFcorr ;
  delete h1f_Gcorr ;
  delete h2f_dEdx_Y ;
  delete h2f_ratiodiffZ ;
  delete h2f_AmaxvsLength ;
  delete h2f_QvsLength ;
  delete h2f_LUTvsLength ;
  delete h2f_WFvsLength ;
  delete h2f_WFsumvsLength ;
  delete h2f_WFstarvsLen ;
  delete h2f_WFstartrcvsLen ;
  delete h1f_WFoLength ;
  delete h2f_lenVSd ;
  delete pfileROOT_checks ;
  delete pfileROOT_status ;
  delete A_corr ;

  delete tf1_WFsel ;
  delete tf1_WFsum ;
  delete tf1_XP ;
  delete h1f_WFsel ;
  delete h1f_WFsum ;
  delete h1f_XP ;

  for(int i = 0; i < (int)RCmaps.size(); i++){
    delete RCmaps[i];
    delete Gainmaps[i];
  }
  RCmaps.     clear();
  Gainmaps.   clear();

  std::cout.rdbuf(coutbuf) ; // Reset to standard output
}
