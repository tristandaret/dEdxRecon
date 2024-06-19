#include "Util.h"
#include "Misc.h"
// #include "SetT2KStyle.h"

#include "Monitoring.h"
#include "ReconTools.h"
#include "Variables.h"
#include "LUTs.h"
#include "dEdx.h"

#include "Control.h"
#include "DrawOuts.h"

#include "Selector.h"

#include "Uploader.h"
#include "GiveMe_Uploader.h"

namespace Reconstruction{
  Reconstruction::dEdx      *p_dEdx;
  Reconstruction::DrawOuts  *p_DrawOuts;
  Reconstruction::LUT       *p_lut;
  Uploader                  *p_uploader;
}

void Reconstruction::Monitoring()
{
  // SetT2KStyle(1) ;

  comment = "";
  gErrorIgnoreLevel = kInfo;

  MakeMyDir(outDir);
  gStyle->                SetOptStat(111111);
  gStyle->                SetPadTickX(1);
  gStyle->                SetPadTickY(1);

  WFupdated = true;
  if(WFupdated) WFversion = 1;
  else WFversion = 0;

  
  // Files to use
  int prototype =       0;
  int CERN_Escan =      0; 

  int DESY_zscan =      1; 
  int DESY_yscan =      0; 
  int DESY_phi =        0; 
  int DESY_theta =      0; 

  // Computations
  int control =         0;
  int dedx =            0;

  // DrawOuts
  int DO_control =      0;
  int DO_Checks =       0;
  int DO_dEdx =         1;
  int DO_Resolution =   0;
  int DO_Global =       0;
  int DO_Scans =        0;

  int DO_Separation =   0;
  int DO_Systematics =  0;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Energy scan using the prototype (ERAM 18)
  if(prototype){
    selectionSet = "T2_CERN22_Event";
    outDir = "../OUT_Reconstruction/CERN22_ERAM18/";
    MakeMyDir(outDir);
    intUploader = 2;
    moduleCase = -1;
    PT = 412; Dt = 350; driftDist = 415; TB = 40;
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    int part_arr[] = {1,2,5,6};
    // int part_arr[] = {1};
    for (int iFile : part_arr) {
      if (iFile == 0) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_e+_0p5GeV_iter0.root";    tag = "CERN22_ERAM18_e+_0p5GeV";    prtcle = "e^{+} 0.5GeV";   }
      if (iFile == 1) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_e+_1GeV_iter0.root";      tag = "CERN22_ERAM18_e+_1GeV";      prtcle = "e^{+} 1GeV";     }
      if (iFile == 2) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_p_1GeV_iter0.root";       tag = "CERN22_ERAM18_p_1GeV";       prtcle = "protons 1GeV";   } 
      if (iFile == 3) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu-_1GeV_iter0.root";     tag = "CERN22_ERAM18_mu-_1GeV";     prtcle = "#mu^{-} 1GeV";   }
      if (iFile == 4) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root";  tag = "CERN22_ERAM18_mu+_0p75GeV";  prtcle = "#mu^{+} 0.75GeV";}
      if (iFile == 5) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu+_1GeV_iter0.root";     tag = "CERN22_ERAM18_mu+_1GeV";     prtcle = "#mu^{+} 1GeV";   }
      if (iFile == 6) { dataFile = "../Data_CERN22_vD/ERAM18_350V_412ns_pi+_0p5GeV_iter0.root";   tag = "CERN22_ERAM18_pi+_0p5GeV";   prtcle = "#pi^{+} 0.5GeV"; }
      if (control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
      if (dedx)         p_dEdx->Reconstruction();
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
      delete p_uploader;
    } 
    // if(DO_Separation)         DrawOut_Separation(outDir, comment);
    if(DO_Separation)         DrawOut_Separation_Reduced(outDir, comment, "1p5");
  }

  // Energy scan using the mockup
  if(CERN_Escan){
    selectionSet = "T2_CERN22_Event";
    outDir = "../OUT_Reconstruction/CERN22_Energy_Scan/";
    MakeMyDir(outDir);
    intUploader = 3;
    moduleCase = -1;
    PT = 412; Dt = 350; driftDist = 415; TB = 40;

    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    // int NFiles = 14;
    // for (int iFile = 0; iFile < NFiles; iFile++){
    int part_arr[] = {4, 7, 10, 13};
    for (int iFile : part_arr) {
      if (iFile == 0)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root";       tag = "CERN22_Escan_e+_0p5GeV";  prtcle = "e^{+} 0p5GeV (Mockup)";}
      if (iFile == 1)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root";      tag = "CERN22_Escan_e+_0p75GeV"; prtcle = "e^{+} 0p75GeV (Mockup)";}
      if (iFile == 2)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root";       tag = "CERN22_Escan_e+_1GeV";    prtcle = "e^{+} 1GeV (Mockup)";}  
      if (iFile == 3)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root";  tag = "CERN22_Escan_e+_1p25GeV"; prtcle = "e^{+} 1p25GeV (Mockup)";}
      if (iFile == 4)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root";     tag = "CERN22_Escan_e+_1p5GeV";  prtcle = "e^{+} 1p5GeV (Mockup)";}
      if (iFile == 5)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root";     tag = "CERN22_Escan_mu_0p75GeV"; prtcle = "#mu^{+} 0p75GeV (Mockup)";}
      if (iFile == 6)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root";      tag = "CERN22_Escan_mu_1GeV";    prtcle = "#mu^{+} 1GeV (Mockup)";}
      if (iFile == 7)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root";    tag = "CERN22_Escan_mu_1p5GeV";  prtcle = "#mu^{+} 1p5GeV (Mockup)";}
      if (iFile == 8)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root";     tag = "CERN22_Escan_pi_0p75GeV"; prtcle = "#pi^{+} 0p75GeV (Mockup)";}
      if (iFile == 9)  { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root"; tag = "CERN22_Escan_pi_1p25GeV"; prtcle = "#pi^{+} 1p25GeV (Mockup)";}
      if (iFile == 10) { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root";    tag = "CERN22_Escan_pi_1p5GeV";  prtcle = "#pi^{+} 1p5GeV (Mockup)";}
      if (iFile == 11) { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root";        tag = "CERN22_Escan_p+_1GeV";    prtcle = "protons 1GeV (Mockup)";}
      if (iFile == 12) { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root";     tag = "CERN22_Escan_p+_1p25GeV"; prtcle = "protons 1p25GeV (Mockup)";}
      if (iFile == 13) { dataFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root";      tag = "CERN22_Escan_p+_1p5GeV";  prtcle = "protons 1p5GeV (Mockup)";}
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 4);
      if (dedx)          p_dEdx->Reconstruction();
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
      delete p_uploader;
    }
    if(DO_Resolution) DrawOut_Escan(outDir, comment);
    if(DO_Separation) DrawOut_Separation_Reduced(outDir, comment, "1p5");
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // z scan scan with DESY21
  if(DESY_zscan){
    selectionSet = "T_DESY21_Event";
    intUploader = 1;
    moduleCase =  0; 
    Dt = 310; TB = 50;
    int PT_arr[] = {412}; // 200, 412
    for (int iPT : PT_arr){
      PT = iPT;
      outDir = Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i/", iPT); 
      MakeMyDir(outDir); 
      if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, iPT));
      // int         z_val[] = {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
      // std::string z_arr[] = {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};
      int         z_val[] = {550};
      std::string z_arr[] = {"460"};
      for (int iz = 0; iz < (int)std::size(z_arr); iz++){
        const char* z = z_arr[iz].c_str();
        driftDist = z_val[iz];
        dataFile = Form("../Data_DESY21_dev_v9/zscan_PT%i/z_360_275_%i_02T_26_%s_iter9.root", iPT, iPT, z); tag = Form("DESY21_z%s_PT%i", z, iPT); prtcle = Form("electron_z%s", z);
        if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
        // if (control)      p_dEdx->Control();
        outFile_dEdx = (outDir + tag + "/" + "dEdx_" + tag + comment + ".root").c_str();
        if (dedx)         p_dEdx->      Reconstruction();
        p_DrawOuts = new DrawOuts(outFile_dEdx);
        // if (DO_control)   p_DrawOuts->Control();
        if (DO_Checks)    p_DrawOuts->  Checks();
        if (DO_dEdx)      p_DrawOuts->  EnergyLoss();
      }
      // if(DO_Systematics)  p_DrawOuts->Systematics();
      // if(DO_Resolution)   p_DrawOuts->ResolutionDriftScan();
    }
    // if(DO_Global)         p_DrawOuts->ResolutionDriftScanPT();
  }
  


  // y scan scan with DESY21
  if(DESY_yscan){
    selectionSet = "T_DESY21_Event";
    intUploader = 1;
    moduleCase =  0; 
    Dt = 310; TB = 50; PT = 412; driftDist = 90;
    outDir = "../OUT_Reconstruction/DESY21_yscan/"; 
    MakeMyDir(outDir); 
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    std::string Y_arr[] = {"m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"};
    for (int y = 0; y < (int)std::size(Y_arr); y++){
      dataFile = Form("../Data_DESY21_dev_v9/yscan/Y%s_Z0_iter9.root", Y_arr[y].c_str()); tag = Form("DESY21_y%s", Y_arr[y].c_str()); prtcle = Form("electron_y%s", Y_arr[y].c_str()); 
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
      if (dedx)          p_dEdx->Reconstruction();
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
    }
    if(DO_Resolution)   DrawOut_Yscan  ("../OUT_Reconstruction/DESY21_yscan", comment);
  }
  


  // Phi scan with DESY21
  if(WFupdated) WFversion = 1;
  if(DESY_phi){
    selectionSet = "T_DESY21_Event";
    intUploader = 1;
    moduleCase =  0;
    PT = 200; Dt = 310; TB = 40;
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    int         z_val[] = {50, 550, 950};
    std::string z_arr[] = {"m40", "460", "860"};
    // int         z_val[] = {950};
    // std::string z_arr[] = {"860"};
    int         phi_val[] = {0, 5, 10, 20, 30, 30, 40, 45};
    for (int iz = 0; iz < (int)std::size(z_arr); iz++){
      const char* z = z_arr[iz].c_str();
      driftDist = z_val[iz];
      outDir = Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s/", z);  
      MakeMyDir(outDir); 
      std::string comment_phi = comment;
      // for (int ifile = 0; ifile < (int)std::size(phi_val); ifile++){
      for (int ifile = 5; ifile < 8; ifile++){
        int phi = phi_val[ifile];
        // if(ifile ==0){       outDir = "../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/"; 
        //                     dataFile = Form("../Data_DESY21_dev_v9/zscan_PT200/z_360_275_200_02T_26_%s_iter9.root", z); tag = Form("DESY21_z%s_PT200", z); prtcle = Form("electron_z%s", z); }
        if(ifile < 5) {dataFile = Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_iter9.root", z, phi, z);            tag = Form("DESY21_phi%i_z%s", phi, z); prtcle = Form("electron_phi%i_z%s", phi, z); }
        else               {dataFile = Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter9.root", z, phi, z);       tag = Form("DESY21_phi%i_diag_z%s", phi, z); prtcle = Form("electron_phi%i_diag_z%s", phi, z);
                            comment_phi = comment + "_WF" + WFversion;}

        if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
        if (control)      Control                   (outDir, tag, comment_phi, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
        if (DO_control)   DrawOut_Control           (outDir, tag, comment_phi, selectionSet, 1);
        if (dedx)          p_dEdx->Reconstruction();
        if (DO_Checks)    DrawOut_Checks            (outDir, dataFile, tag, comment);
        if (DO_dEdx)   DrawOut_Methods           (outDir, tag, comment_phi, 1, prtcle);
        delete p_uploader;
        if(ifile ==0)outDir = Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s/", z); 
      }
      if(DO_Resolution)   DrawOut_Phiscan (Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s", z), comment, "_WF" + WFversion, z);
      if(DO_Systematics)  DrawOut_Systematics(outDir, comment, "phi");
    }
    // DrawOut_TGE_WFsum_L("../OUT_Reconstruction/DESY21_phi/", comment);
    if(DO_Global) DrawOut_Phiscan_Z("../OUT_Reconstruction/DESY21_phi", comment, "_WF" + WFversion);
  }
  


  // Theta scan with DESY21
  if(DESY_theta){
    selectionSet = "T_DESY21theta_Event";
    outDir = "../OUT_Reconstruction/DESY21_theta/";  
    MakeMyDir(outDir); 
    intUploader = 1;
    moduleCase =  0;
    PT = 200; Dt = 310; driftDist = 350; TB = 40;
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    std::string theta_arr[] = {"m45", "m20", "20"};
    for (std::string theta : theta_arr){
      dataFile = Form("../Data_DESY21_dev_v9/Theta_scan/theta_%s_02T_z460_ym60_iter9.root", theta.c_str()); tag = Form("DESY21_theta%s", theta.c_str()); prtcle = Form("electron_theta%s", theta.c_str());
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)                Control(outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)             DrawOut_Control(outDir, tag, comment, selectionSet, 1);
      if (dedx)                    p_dEdx->Reconstruction();
      if (DO_Checks)              DrawOut_Checks(outDir, dataFile, tag, comment);
      if (DO_dEdx)             DrawOut_Methods(outDir, tag, comment, 1, prtcle);
    }
    if(DO_Resolution)             DrawOut_Thetascan ("../OUT_Reconstruction/DESY21_theta",   comment);
  }



  if(DO_Scans) DrawOut_Scans("../OUT_Reconstruction", comment, "_WF" + WFversion);
  // DrawOut_Versions("../OUT_Reconstruction/", "XP", "_zcalc_PRF_4IP_WF1", "_zcalc_PRF_4IP_Gain_WFoffdiag");
  // DrawOut_verif("../OUT_Reconstruction/DESY21_phi/DESY21_phi_", comment);
  // DrawOut_corrections();





  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  int DESY_mag = 0;
  int DESY_ExB = 0;
  int DESY_zscan_139V = 0; 
  int MC_zscan = 0; 
  int DESY19_phi = 0; 
  int MockUp_zdist = 0; 


  // Magnetic field scan scan with DESY21
  if(DESY_mag){
    selectionSet = "T_DESY21_Event";
    outDir = "../OUT_Reconstruction/DESY21_mag/";  
    MakeMyDir(outDir); 
    intUploader = 1;
    moduleCase =  0;
    PT = 200; Dt = 310; driftDist = 950; TB = 40;

    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    // int mag_arr[] = {0, 2, 4, 10};
    int mag_arr[] = {0,2,4};
    for (int mag : mag_arr){
      dataFile = Form            ("../Data_DESY21_dev_v9/Mag_scan/mag_%i_200_ym60_iter0.root", mag); tag = Form("DESY21_mag%i", mag); prtcle = Form("electron_mag%i", mag);
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
      if (dedx)          p_dEdx->Reconstruction();
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
    }
  }





  // ExB scan scan with DESY21
  if(DESY_ExB){
    selectionSet = "T_DESY21_Event";
    outDir = "../OUT_Reconstruction/DESY21_ExB/";  
    MakeMyDir(outDir); 
    intUploader = 1;
    moduleCase =  0;
    PT = 200; Dt = 310;; TB = 40;
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    std::string B_arr[] = {"0", "04", "06"};
    std::string Z_arr[] = {"60", "460", "860"};
    int z_val[] = {150, 550, 950};
    for(int j = 0; j < 3; j++){
      const char* B = B_arr[j].c_str();
      for(int i = 0; i < 3; i++){
        const char* z = Z_arr[i].c_str();
        driftDist = z_val[i];
        // dataFile = Form            ("../Data_DESY21_dev_v9/ExB_scan/ExB_360_412ns_B02_ym10_z%s_iter9.root", z); tag = Form("DESY21_ExB02_Z%s", z); prtcle = Form("electron_ExB02_Z%s", z);
        dataFile = Form            ("../Data_DESY21_dev_v9/ExB_scan/ExB_360_phim3_200ns_B%s_ym60_z%s_iter0.root", B, z); tag = Form("DESY21_ExB%s_Z%s", B, z); prtcle = Form("electron_ExB%s_Z%s", B, z);
        if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
        if (control)                Control (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
        if (DO_control)             DrawOut_Control (outDir, tag, comment, selectionSet, 1);
        if (dedx)                    p_dEdx->Reconstruction();
        if (DO_dEdx)             DrawOut_Methods (outDir, tag, comment, 1, prtcle);
        if (DO_Checks)              DrawOut_Checks (outDir, dataFile, tag, comment);
      }
    }
  }





  // Phi scan with DESY19
  if(DESY19_phi){
    selectionSet = "T_DESY21_Event";
    intUploader = 1;
    moduleCase =  0;
    PT = 412; Dt = 310; TB = 40; driftDist = 430;
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    int phi_val[] = {0, 10, 20, 30, 40, 45};
    int NFiles = 6;
    outDir = "../OUT_Reconstruction/DESY19_phi/";
    MakeMyDir(outDir); 
    for (int ifile = 3; ifile < NFiles; ifile++){
      int phi = phi_val[ifile];
      if(ifile < 3) { dataFile = Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi);       tag = Form("DESY19_phi%i", phi);      prtcle = Form("electron_phi%i", phi); }
      else          { dataFile = Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi);  tag = Form("DESY19_phi%i_diag", phi); prtcle = Form("electron_phi%i_diag", phi); }
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
      if (dedx)          p_dEdx->Reconstruction();
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
      delete p_uploader;
    }
  }




  // z scan with MC
  if(MC_zscan){
    selectionSet = "TMC_CERN22_Event";
    outDir = "../OUT_Reconstruction/CERN23_MC/MC_zscan/";  
    MakeMyDir(outDir); 
    intUploader = 2;
    moduleCase = -1;
    PT = 412; Dt = 286; TB = 40; // PT = 400 actually
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
    int zmax = 9;
    for (int zDrift = 0; zDrift < zmax; zDrift++){
      // dataFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v2_iter4.root", zDrift+1); tag = Form("CERN23_MC_z%i00", zDrift+1); prtcle = Form("MC %i0cm", zDrift+1);      
      dataFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root", zDrift+1); tag = Form("CERN23_MC_z%i00", zDrift+1); prtcle = Form("MC %i0cm NoOpt", zDrift+1);      
      // dataFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root", zDrift+1); tag = Form("CERN23_MC_z%i00_old", zDrift+1); prtcle = Form("MC %i0cm old", zDrift+1);      
      if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
      if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
      if (dedx)          p_dEdx->Reconstruction();
      if (DO_Checks)     DrawOut_Checks            (outDir, dataFile, tag, comment);
      if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
    }
  }
  



  // z scan scan using the mockup
  if(MockUp_zdist){
    selectionSet = "T2_CERN22_Event";
    outDir = "../OUT_Reconstruction/CERN22_Mockup_zscan/";  
    MakeMyDir(outDir); 
    intUploader = 2;
    moduleCase = -1;
    PT = 412; Dt = 350; driftDist = 415; TB = 40;

    int NFiles = 10;
    for (int iFile = 0; iFile < NFiles; iFile++){
      if (iFile == 0) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root";       tag = "CERN22_Mockup_e+_14V_z060";}
      if (iFile == 1) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root";  tag = "CERN22_Mockup_e+_14V_z218";}
      if (iFile == 2) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root";      tag = "CERN22_Mockup_e+_14V_z415";}
      if (iFile == 3) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root";      tag = "CERN22_Mockup_e+_14V_z572";}
      if (iFile == 5) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root";       tag = "CERN22_Mockup_e+_25V_z060";}
      if (iFile == 6) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root";    tag = "CERN22_Mockup_e+_25V_z218";}
      if (iFile == 7) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root";      tag = "CERN22_Mockup_e+_25V_z415";}
      if (iFile == 8) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root";      tag = "CERN22_Mockup_e+_25V_z572";}
      if (iFile == 9) { dataFile = "../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root";      tag = "CERN22_Mockup_e+_25V_z925";}
      if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 8);
    }
  }
  


  // z scan scan with DESY21 at 139 V/cm
  if(DESY_zscan_139V){
    selectionSet = "T_DESY21_Event";
    intUploader = 1;
    moduleCase =  0; 
    Dt = 310; TB = 50;
    int PT_arr[] = {200};
    for (int PT : PT_arr){
      outDir = Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i_139V/", PT); 
      MakeMyDir(outDir); 
    if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
      int NFiles = 9;
      for (int zDrift = -1; zDrift < NFiles; zDrift++){
        if(zDrift == -1) {dataFile = Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT);          tag = Form("DESY21_zm40_PT%i", PT);          prtcle = "electron_z-40"; }
        else {dataFile = Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift); tag = Form("DESY21_z%i60_PT%i", zDrift, PT); prtcle = Form("electron_z%i60", zDrift); }
        if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, dataFile);
        if (control)      Control       (outDir, tag, comment, dataFile, selectionSet, p_uploader, moduleCase, 0, PT, TB, prtcle);
        if (DO_control)   DrawOut_Control           (outDir, tag, comment, selectionSet, 1);
        if (dedx)          p_dEdx->Reconstruction();
        if (DO_dEdx)  DrawOut_Methods        (outDir, tag, comment, 1, prtcle);
      }
      if(DO_Resolution)   DrawOut_Zscan  (Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i_139V", PT), comment, PT);
    }
  }
}