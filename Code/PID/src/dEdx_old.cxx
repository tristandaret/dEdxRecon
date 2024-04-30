#include "PID/dEdx.h"
#include "PID/THATdEdx.hxx"
#include "PID/dEdx_func.h"
#include "PID/Misc_Functions.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "TH2.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"

#include "EvtModelTools/JFL_Selector.h"
#include "AnalysisTools/DoTracksReconstruction.h"
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

void dEdx()
{ 
  std::string OUTDirName                    = OutDir + Tag + "/";
  MakeMyDir(OUTDirName);
  std::string OUTDIR_Display            = OUTDirName + "Evt_Display/";
  MakeMyDir(OUTDIR_Display);
  std::string OUTDIR_WF_Display             = OUTDirName + "WF_Display/";
  MakeMyDir(OUTDIR_WF_Display);

  // Geometry
  float Lx              = 11.28;                               // Length of pad (mm)
  float Ly              = 10.19;                               // Height of pad (mm)
  float L               = sqrt(pow(Lx, 2) + pow(Ly, 2));       // diagonal length of the pad (mm)
  float phi_max         = std::atan(Ly/Lx)*180/M_PI;           // (°) angle of the diagonal

  // Parameters for the dE/dx procedure and for selections
  std::string z_method  = "zcalc";                             // method to get z ("zcalc" to recompute, "zfile" to use value from ntuple)
  float alpha           = 70;                                  // truncation value in %
  float n_param_trk     = 3;                                   // 2 if there is not magnetic field
  float len_cut         = 0.002;                               // minimum length in pad to be considered truncable (m)
  int gain_corr         = 1;                                   // apply XP gain correction
  int nERAMs            = 1;                                    // number of ERAMs to use (CERN22)

  // Parameters for the p_lut
  int   z_step          = 50;                                  // z   increment between LUTs
  int   RC_step         = 5;                                   // RC  increment between LUTs

  // Redirect Output
  std::string     OutPut_Analysis = OUTDirName + "3_" + Tag + "_dEdx_XP.log";
  std::cout <<    OutPut_Analysis       << std::endl;
  std::cout <<    std::setprecision(2)  << std::fixed;
  std::cout <<    std::endl;
  std::streambuf* coutbuf = std::cout.rdbuf();     // Save old buf
  std::ofstream   OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
  std::cout.      rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file

  std::cout << "Tag           : " << Tag          << std::endl;
  std::cout << "Comment       : " << Comment      << std::endl;
  std::cout << "FileName      : " << FileName     << std::endl;
  std::cout << "SelectionSet  : " << SelectionSet << std::endl;
  std::cout << "NbrOfMod      : " << NbrOfMod     << std::endl;
  std::cout << "0   : " << 0  << std::endl;
  std::cout << "OUTDirName    : " << OUTDirName   << std::endl;
  std::cout <<                                       std::endl;

  // Get ERAM ID
  std::vector<std::string>  ERAMS_iD;
  if(Tag.find("DESY") != std::string::npos)         ERAMS_iD.push_back("01");
  if(FileName.find("ERAM18") != std::string::npos)  ERAMS_iD.push_back("18");
  if(FileName.find("All_ERAMS") != std::string::npos){
    ERAMS_iD     = {"07", "01", "23", "02", "16", "15", "10", "12"}; // MockUp CERN22
  }
  // ERAMS_iD = {"24", "30", "28", "19", "21", "13", "09", "02", "26", "17", "23", "29", "01", "10", "11", "03",  /*bottom HATPC*/ 
  //             "47", "16", "14", "15", "42", "45", "37", "36", "20", "38", "07", "44", "43", "39", "41", "46"}; /*top    HATPC*/
  // Get Gain & RC maps
  std::vector<ERAM_map*> RCmaps;
  std::vector<ERAM_map*> Gainmaps;
  for(std::string eram_id : ERAMS_iD){
    Gainmaps. push_back(new ERAM_map(eram_id, "Gain"));
    RCmaps.   push_back(new ERAM_map(eram_id, "RC"));
  } 

  float avg_G = avg_Gain(Gainmaps);
  std::cout << "Average gain in HATPCs: " << avg_G << std::endl;

  float costheta = 1;
  int theta_arr[] = {-45, -20, 20};
  for (int theta_file : theta_arr) if(Tag.find("theta") != std::string::npos and Tag.find(std::to_string(theta_file)) != std::string::npos) costheta = fabs(cos((float)theta_file/180*M_PI));

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Selection stage
  JFL_Selector aJFL_Selector(SelectionSet);
  int NEvent = pUploader->Get_NberOfEvent();
  std::cout << "Number of entries :" << NEvent << std::endl;

  Init_selection(SelectionSet, aJFL_Selector, Tag, pUploader, NbrOfMod, 0);
    
  aJFL_Selector.Tell_Selection();

  std::cout << "zdrift        = " << zdrift << " mm"          << std::endl;
  std::cout << "PT            = " << PT     << " ns"          << std::endl;
  std::cout << "TB            = " << TB     << " ns/time bin" << std::endl;
  std::cout << "PT/TB         = " << PT/TB  << " time bins"   << std::endl;
  std::cout << "z method      : " << z_method                 << std::endl;
  std::cout << "cut length    = " << len_cut*1e3 << " mm"     << std::endl;
  std::cout <<                                                   std::endl;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Check histograms
  TH1F* h1f_dnorm           = new TH1F("h1f_dnorm",           "Normalized impact parameter d/d_{max};d/d_{max};", 100, -1.1, 1.1);
  TH1F* h1f_dist            = new TH1F("h1f_dist",            "distance of track in pad;distance (mm);", 100, 0, 17);
  TH1F* h1f_dist_clus       = new TH1F("h1f_dist_clus",       "Distance of track in cluster;distance (mm);", 100, 0, 17);
  TH1F* h1f_Lnorm           = new TH1F("h1f_Lnorm",           "Length in pad normalized to maximum length in pad for a given #varphi;L_{#cap}/L_{#varphi};Count", 60, 0, 1.2012);
  TH1F* h1f_zdiff           = new TH1F("h1f_zdiff",           Form("z_{file = %.0fmm} - z_{computed};difference (mm);Count", zdrift), 80, -150, 150);
  TH1F* h1f_Tcross          = new TH1F("h1f_Tcross",          "T_{max} of crossed pads;T (timbebin);Count", 511, -0.5, 510.5);
  TH1F* h1f_Ncross          = new TH1F("h1f_Ncross",          "Number of crossed pads;# of crossed pads;Count", 45, 15, 60);
  TH1F* h1f_angle           = new TH1F("h1f_angle",           "Angle #varphi in each pad;#varphi (#circ);Count", 66, -55, 10);
  TH1F* h1f_Gcorr           = new TH1F("h1f_Gcorr",           "Gain correction ratio;ratio;Count", 80, 0, 2);
  TH1F* h1f_WFcorr          = new TH1F("h1f_WFcorr",          "Correction A_{max} ratio;ratio;Count", 80, 0, 3);
  TH2F* h2f_dEdx_Y          = new TH2F("h2f_dEdx_Y",          "Y position VS dE/dx;dE/dx (ADC counts);Y position (cm)", 160, 0, 2000, 80, 0, 10);
  TH2F* h2f_ratiodiffZ      = new TH2F("h2f_ratiodiffZ",      "p_lut(z_{file}) vs p_lut(z_{calc});p_lut(z_{calc});p_lut(z_{file})", 80, 0, 2.1, 80, 0, 2.1);
  TH2F* h2f_AmaxvsLength    = new TH2F("h2f_AmaxvsLength",    "ADC_{max} VS length in pad (before length cut);Length in pad (mm);ADC_{max}", 80, -0.1, 16, 80, 0, 4100);
  TH2F* h2f_QvsLength       = new TH2F("h2f_QvsLength",       "Q^{anode} VS length in pad (before length cut);Length in pad (mm);Q^{anode}", 80, -0.1, 16, 80, 0, 4100);
  TH2F* h2f_LUTvsLength     = new TH2F("h2f_LUTvsLength",     "Q^{anode}/ADC_{max} VS length in pad (before length cut);Length in pad (mm);Q^{anode}/ADC_{max}", 80, -0.1, 16, 80, -0.1, 2.1);
  TH2F* h2f_WFvsLength      = new TH2F("h2f_WFvsLength",      "WF_{sum} VS length in cluster;Length in cluster (mm);WF_{sum} (ADC count)", 80, -0.1, 16, 80, 0, 4100);
  TH2F* h2f_WFsumvsLength   = new TH2F("h2f_WFsumvsLength",   "WF_{sum truncated} VS length in cluster;Length in cluster (mm);WF_{sum trunc} (ADC count)", 80, -0.1, 16, 80, 0, 4100);
  TH1F* h1f_WFoLength       = new TH1F("h1f_WFoLength",       "A_{max}(WF_{sum}) / L_{cluster};A_{max}(WF_{sum}) / L_{cluster} (ADC count/mm);", 80, 0, 4000);
  TH2F* h2f_lenVSd          = new TH2F("h2f_lenVSd",          "impact parameter d vs length in pad;Length in pad (mm);impact parameter (mm)", 80, -0.1, 16, 80, -7.8, 7.8);

  // Method histograms
  TH1F* h1f_WFsum           = new TH1F("h1f_WFsum",           "<dE/dx> with WF_{sum};<dE/dx> (ADC count);Number of events", 90, 0, 1800);
  TH1F* h1f_XP              = new TH1F("h1f_XP",              "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 90, 0, 1800);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Track fit initializations
  int WFversion ;
  if(Comment.find("WF1") != std::string::npos) WFversion = 1;
  if(Comment.find("WF2") != std::string::npos) WFversion = 2;
  TF1* A_corr                 = corr_func(FileName, Tag, WFversion); //1 HATRecon, 2 all correction function
  float A_ref                 = A_corr->Eval(Lx);

  TheFitterTrack aTheFitterTrack("Minuit", n_param_trk);
  PRF_param                     aPRF_param;
  TF1* tf1_PRF                = new TF1("tf1_PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5);
  tf1_PRF->                     SetParameters(pUploader->Get_Norm(), pUploader->Get_a2(), pUploader->Get_a4(), pUploader->Get_b2(), pUploader->Get_b4());
  int Kounter_Fit             = 0;
  int Kounter_Fail            = 0;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Compute dE/dx
  aJFL_Selector.                          Reset_StatCounters();
  std::cout << "Processing events:" << std::endl;
  for (int iEvent = 0; iEvent < NEvent; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl;
    Event*  pEvent                        = pUploader->GiveMe_Event(iEvent, NbrOfMod, 0, 0);
    if (!pEvent)                            continue;
    aJFL_Selector.                          ApplySelection(pEvent);
    if (pEvent->IsValid() != 1)             continue;

    // Initialize event variables
    int                                   N_crossed = 0;
    std::vector<float>                    v_dx, v_dE;
    std::vector<RankedValue>              v_dEdx_ranked;
    int                                   N_clus = 0;
    std::vector<float>                    v_dEdxWF;
    std::vector<TH1F*>                    v_trashbin;

    // Loop On Modules
    int nMod                              = pEvent->Get_NberOfModule();

    // Check if there are is a track in 4 ERAMs that are aligned
    std::vector<double> eram_list;
    for (int iMod = 0; iMod < nMod; iMod++){
      int ModuleNber                      = pEvent->Get_Module_InArray(iMod)->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber)!=0) eram_list.push_back(Gainmaps[ModuleNber]->Get_iD()[ModuleNber]);
    } 

    int nber = 2;
    if(FileName.find("All_ERAMS") != std::string::npos){
      // 1 ERAM
      if(nERAMs == 1 and!is_in(eram_list, nber)){
        delete pEvent;
        continue;
      }
      // 2 ERAMs
      if(nERAMs == 2 and !(is_in(eram_list, 7) and is_in(eram_list, 1)) and !(is_in(eram_list, 16) and is_in(eram_list, 10))){
        delete pEvent;
        continue;
      }
      // 4 ERAMs
      if(nERAMs == 4
        and !(is_in(eram_list,  7) and is_in(eram_list,  1) and is_in(eram_list, 23) and is_in(eram_list,  2))
        and !(is_in(eram_list, 16) and is_in(eram_list, 15) and is_in(eram_list, 10) and is_in(eram_list, 12))){
        delete pEvent;
        continue;
      }
    }

    for (int iMod = 0; iMod < nMod; iMod++){
      Module* pModule                     = pEvent->Get_Module_InArray(iMod);
      int ModuleNber                      = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue;
      if(FileName.find("All_ERAMS") != std::string::npos){
        if(nERAMs == 1 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != nber) continue;
        if(nERAMs == 2 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 7 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 1 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 16 and Gainmaps[ModuleNber]->Get_iD()[ModuleNber] != 10) continue; // 2 ERAMs
      }
      int NC                        = pModule->Get_NberOfCluster();
      if(NC == 0) continue;

      // Track fitting
      if(Tag.find("diag") == std::string::npos){
        TheFitterCluster_PV0_PV1            aTheFitterCluster_PV0_PV1("Minuit");
        Do_ClusterFit_PV0_PV1_Event(pEvent, ModuleNber, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_PV1);
      }
      else{
        TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit");
        Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(phi_max*M_PI/180)), pEvent, ModuleNber, tf1_PRF, Kounter_Fit, Kounter_Fail, aTheFitterCluster_PV0_Diag);
      }
      TheFitterTrack                      aTheFitterTrack("Minuit", n_param_trk);
      DoTracksReconstruction_Event(aTheFitterTrack, pEvent, ModuleNber, n_param_trk);

      // Track details
      const Track* pTrack                 = pEvent->GiveMe_Track_ForThisModule(ModuleNber);
      float phi                           = std::atan(pTrack->Get_ParameterValue(1))/M_PI*180;

      // Loop On Clusters
      for (int iC = 0; iC < NC; iC++){
        TH1F* h1f_WF_cluster              = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5);
        Cluster* pCluster                 = pModule->Get_Cluster(iC);
        float len_clus                    = 0; // in meters

        // Loop On Pads
        int NPads                         = pCluster->Get_NberOfPads();
        for(int iP = 0; iP < NPads; iP ++){
          const Pad* pPad                 = pCluster->Get_Pad(iP);
          TH1F* h1f_WF_pad                = GiveMe_WaveFormDisplay(pPad, "main");
          float A_pad;
          double RC_pad                   = RCmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY());
          if(gain_corr){
            double G_pad                  = Gainmaps[ModuleNber]->GetData(pPad->Get_iX(),pPad->Get_iY());
            float Gcorr                   = avg_G/G_pad;
            h1f_Gcorr->                     Fill(Gcorr);
            A_pad                         = Gcorr*pPad->Get_AMax();
            h1f_WF_pad->                    Scale(Gcorr);
          }
          else A_pad                      = pPad->Get_AMax();

          h1f_WF_cluster->                  Add(h1f_WF_pad);
          v_trashbin.                       push_back(h1f_WF_pad);

          // Track computations (impact parameter in m, angle in degrees, length in pad in m)
          float d=0, dd=0, length_in_pad=0;
          local_params(pPad, pTrack, d, dd, phi, length_in_pad);
          d                              *= 1000; // in mm
          dd                             *= 1000; // in mm
          if(length_in_pad <= 1e-6)         continue;  // ignore non-but-almost-zero tracks
          length_in_pad                  /= costheta;
          
          float L_phi                     = std::min({ 11.28/std::fabs(std::cos(phi*M_PI/180)), 10.19/std::fabs(std::sin(phi*M_PI/180)) });
          float LNorm                     = length_in_pad*1000/L_phi;
          float d_max                     = std::max( std::fabs(L/2*std::sin((phi_max+phi)/180*M_PI)), std::fabs(L/2*std::sin((phi_max-phi)/180*M_PI)) );
          len_clus                       += length_in_pad;

          // Compute Z
          float Tmax                      = pPad->Get_TMax();
          float z_calc                    = 0; // in mm
          float T0;
          if      (PT == 412 and TB == 50){ T0 = 45; z_calc = 3.90*(Tmax-T0); } // 45 = 37(time shift) +  8(PT) from David
          else if (PT == 412 and TB == 40){ T0 = 56; z_calc = 3.12*(Tmax-T0); } // 56 = 46(time shift) + 10(PT)
          else if (PT == 200 and TB == 50){ T0 = 39; z_calc = 3.90*(Tmax-T0); } // 39 = 35(time shift) +  4(PT) own computation
          else if (PT == 200 and TB == 40){ T0 = 48; z_calc = 3.12*(Tmax-T0); } // 48 = 44(time shift) +  5(PT)

          h1f_dist->                        Fill(length_in_pad*1000);
          h1f_Lnorm->                       Fill(LNorm);
          h1f_dnorm->                       Fill(d/d_max);
          h2f_AmaxvsLength->                Fill(length_in_pad*1000, A_pad);
          h2f_lenVSd->                      Fill(length_in_pad*1000, d);
          h1f_angle->                       Fill(phi);
          h1f_Tcross->                      Fill(Tmax);
          h1f_zdiff->                       Fill(zdrift - z_calc);

          if(length_in_pad <= len_cut)      continue;
          
          float ratio_zcalc               = p_lut->ratio(fabs(phi), fabs(d), RC_pad, z_calc);
          float ratio_zfile               = p_lut->ratio(fabs(phi), fabs(d), RC_pad, zdrift);

          float ratio;
          if(z_method == "zcalc") ratio   = ratio_zcalc;
          if(z_method == "zfile") ratio   = ratio_zfile;
          h2f_ratiodiffZ->                  Fill(ratio_zcalc, ratio_zfile);
          h2f_LUTvsLength->                 Fill(length_in_pad*1000, ratio);
          h2f_QvsLength->                   Fill(length_in_pad*1000, A_pad*ratio);

          // std::cout << "row = " << pPad->Get_iY() << " | col = " << pPad->Get_iX() << " : ";
          // std::cout << std::setprecision(3) << "(RC, drift, d, phi, L, ratio) = (" << RC_pad << ", " << z_calc << ", " << d << ", " << phi << ", " << length_in_pad*1000  << ", " << ratio << ")" << std::endl;
          v_dE.                             push_back(A_pad*ratio);
          v_dx.                             push_back(length_in_pad);

          RankedValue dEdx_ranked;  
          dEdx_ranked.Rank                  = N_crossed; 
          dEdx_ranked.Value                 = A_pad*ratio/length_in_pad;
          v_dEdx_ranked.                     push_back(dEdx_ranked);

          N_crossed++;
        }

        if(len_clus <= 0) {delete h1f_WF_cluster; continue;}
        h1f_dist_clus->                   Fill(len_clus*1000);
        h2f_WFvsLength->                  Fill(len_clus*1000, h1f_WF_cluster->GetMaximum());
        h1f_WFoLength->                   Fill(h1f_WF_cluster->GetMaximum()/(len_clus*1000));

        // WF v1 & v2 (v2 has several correction function)
        if(len_clus < len_cut) {delete h1f_WF_cluster; continue;}
        float ratio_corr                = A_ref / A_corr->Eval(len_clus*1000);
        if(Tag.find("diag") != std::string::npos) v_dEdxWF.push_back(h1f_WF_cluster->GetMaximum()*ratio_corr/Lx*10);
        else                                      v_dEdxWF.push_back(h1f_WF_cluster->GetMaximum()/(len_clus*100));
        h1f_WFcorr->                      Fill(ratio_corr);
        N_clus++;
        
        delete h1f_WF_cluster;
        // if(pEvent->Get_EntryNber() < 25) DrawOut_ClusterWFDisplay(pCluster, OUTDIR_WF_Display, Tag, 1, PT, TB);
      } // Cluster

      // if(iEvent < 1000) DrawOut_EventDisplay(pModule, OUTDIR_Display, Tag, "amplitude", pTrack->Get_ParameterValue(2), pTrack->Get_ParameterValue(1), pTrack->Get_ParameterValue(0));
    } // Module

    if(N_crossed > 0){
      h1f_Ncross->                            Fill(N_crossed);
      int N_crossed_trc                     = int(floor(N_crossed * (alpha/100))); 

      // WF
      int N_clus_trc                        = int(floor(N_clus * (alpha/100)));
      std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
      float WFsum                       = std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + N_clus_trc, 0.) / N_clus_trc;
      h1f_WFsum->                         Fill(WFsum);

      // XP
      float dx = 0;
      float dE = 0;
      std::sort(v_dEdx_ranked.begin(), v_dEdx_ranked.end());
      for(int iP = 0; iP < (int)N_crossed_trc; iP++){
        dx += v_dx[v_dEdx_ranked[iP].Rank]*100;
        dE += v_dE[v_dEdx_ranked[iP].Rank];
      }
      h1f_XP-> Fill(dE/dx);
    }


    for(int i = 0; i < (int)v_trashbin.size(); i++)       {delete v_trashbin[i]; v_trashbin[i]       = 0; }
    v_trashbin.clear();
    v_dE.clear(); 
    v_dx.clear(); 
    v_dEdx_ranked.clear();
    v_dEdxWF.clear(); 
    eram_list.clear();
    delete pEvent;
  }
  aJFL_Selector.PrintStat();
  delete tf1_PRF;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fitting //
  std::cout << "Fitting: Started... "; 
  TF1* tf1_WFsum                    = Fit1Gauss(h1f_WFsum, 2);
  tf1_WFsum->                         SetNameTitle("tf1_WFsum", "tf1_WFsum");

  TF1* tf1_XP                   = Fit1Gauss(h1f_XP, 2);
  tf1_XP->                        SetNameTitle("tf1_XP", "tf1_XP");
  std::cout << "done!" << std::endl; 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Saving //
  // Checks
  TFile* pfileROOT_checks = new TFile(TString(OUTDirName + "2_" + Tag + "_Checks" + Comment + ".root"), "RECREATE");
  std::cout << "Checks: " << OUTDirName + "2_" + Tag + "_Checks" + Comment + ".root" << std::endl;
  h1f_Tcross->                              Write();
  h1f_Ncross->                              Write();
  h1f_zdiff->                               Write();
  h1f_angle->                               Write();
  h1f_Lnorm->                               Write();
  h1f_dnorm->                               Write();
  h1f_dist->                                Write();
  h1f_dist_clus->                           Write();
  h1f_WFcorr->                              Write();
  h1f_Gcorr->                               Write();
  h2f_dEdx_Y->                              Write();
  h2f_ratiodiffZ->                          Write();
  h2f_AmaxvsLength->                        Write();
  h2f_QvsLength->                           Write();
  h2f_LUTvsLength->                         Write();
  h2f_WFvsLength->                          Write();
  h2f_WFsumvsLength->                       Write();
  h1f_WFoLength->                           Write();
  h2f_lenVSd->                              Write();
  A_corr->                                  Write();
  pfileROOT_checks->                        Close();  
  
  // Methods
  TFile* pfileROOT_status = new TFile(TString(OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root"), "RECREATE");
  std::cout << "dEdx: " << OUTDirName + "3_" + Tag + "_dEdx" + Comment + ".root" << std::endl;
  tf1_WFsum->                     Write();
  tf1_XP->                        Write();
  h1f_WFsum->                     Write();
  h1f_XP->                        Write();
  pfileROOT_status->              Close();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Deleting //  
  delete h1f_Tcross;
  delete h1f_Ncross;
  delete h1f_zdiff;
  delete h1f_angle;
  delete h1f_Lnorm;
  delete h1f_dnorm;
  delete h1f_dist;
  delete h1f_dist_clus;
  delete h1f_WFcorr;
  delete h1f_Gcorr;
  delete h2f_dEdx_Y;
  delete h2f_ratiodiffZ;
  delete h2f_AmaxvsLength;
  delete h2f_QvsLength;
  delete h2f_LUTvsLength;
  delete h2f_WFvsLength;
  delete h2f_WFsumvsLength;
  delete h1f_WFoLength;
  delete h2f_lenVSd;
  delete pfileROOT_checks;
  delete pfileROOT_status;
  delete A_corr;

  delete tf1_WFsum;
  delete tf1_XP;
  delete h1f_WFsum;
  delete h1f_XP;

  for(int i = 0; i < (int)RCmaps.size(); i++){
    delete RCmaps[i];
    delete Gainmaps[i];
  }
  RCmaps.     clear();
  Gainmaps.   clear();

  std::cout.rdbuf(coutbuf); // Reset to standard output
}
