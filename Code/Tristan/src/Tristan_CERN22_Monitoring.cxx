#include "Misc/Util.h"

#include "Tristan/Tristan_CERN22_Monitoring.h"
#include "Tristan/Tristan_CERN22_Control.h"
#include "Tristan/Tristan_CERN22_corr.h"
#include "Tristan/Tristan_CERN22_dEdx.h"
#include "Tristan/Tristan_CERN22_DrawOuts.h"
#include "Tristan/Tristan_Displayer.h"
#include "EvtModelTools/EvtModelTools_Histos.h"


#include "EvtModelTools/JFL_Selector.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

void Tristan_CERN22_Monitoring()
{
  gErrorIgnoreLevel = kInfo ;
  std::string SelectionSet ;

  int intUploader ;
  int NbrOfMod ;
  int Data_to_Use     =  0 ; // no WF fit = 0       WF fit = 3 needs DoClosure

  // Run variables
  int PT ;      // Peaking time (ns)
  int Dt ;      // Transverse diffusion coefficient (µm/sqrt(cm))
  int zdrift ;  // drift distance (mm)
  int TB ;      // timebin length (ns)
  int nZ  = 22 ; 
  int nRC = 21 ; 

  std::string Tag    ; 
  std::string Comment = "_zcalc_PRF_4IP_Gain2_WF4" ; 
  std::string prtcle ; 
  std::string EvtFile ;
  std::string OutDir  = "OUT_Tristan/";  
  MyMakeDir(OutDir) ;
  gStyle->                SetOptStat(111111) ;
  gStyle->                SetPadTickX(1);
  gStyle->                SetPadTickY(1);

  // Files to use
  int prototype       = 0 ;
  int DESY_zscan      = 0 ; 
  int DESY_yscan      = 0 ; 
  int DESY_phi        = 1 ; 
  int DESY_theta      = 0 ; 

  // Computations
  int Control         = 0 ;
  int dEdx            = 1 ;
  int WFcorr          = 0 ;

  // DrawOuts
  int DO_Displayer    = 0 ;
  int DO_Control      = 0 ;
  int DO_Checks       = 0 ;
  int DO_Methods      = 0 ;
  int DO_Resolution   = 0 ;
  int DO_Global       = 0 ;
  int DO_Scans        = 0 ;
  int DO_Separation   = 0 ;

  // Energy scan using the prototype (ERAM 18)
  if(prototype){
    SelectionSet          = "T2_CERN22_Event" ;
    std::string OutDir    = "OUT_Tristan/CERN22_ERAM18/" ;  
    MyMakeDir(OutDir) ;
    intUploader     =  3 ;
    NbrOfMod        = -1 ;
    PT              = 412 ; Dt = 350 ; zdrift = 415 ; TB = 40 ;
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int part_arr[] = {1,2,5,6} ;
    for (int iFile : part_arr) {
      if (iFile == 0) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_e+_0p5GeV_iter0.root" ;    Tag = "CERN22_ERAM18_e+_0p5GeV" ;    prtcle = "e^{+} 0.5GeV_" ;   }
      if (iFile == 1) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_e+_1GeV_iter0.root" ;      Tag = "CERN22_ERAM18_e+_1GeV" ;      prtcle = "e^{+} 1GeV_" ;     }
      if (iFile == 2) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_p_1GeV_iter0.root" ;       Tag = "CERN22_ERAM18_p_1GeV" ;       prtcle = "protons 1GeV_" ;   } 
      if (iFile == 3) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu-_1GeV_iter0.root" ;     Tag = "CERN22_ERAM18_mu-_1GeV" ;     prtcle = "#mu^{-} 1GeV_" ;   }
      if (iFile == 4) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ;  Tag = "CERN22_ERAM18_mu+_0p75GeV" ;  prtcle = "#mu^{+} 0.75GeV_" ;}
      if (iFile == 5) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_mu+_1GeV_iter0.root" ;     Tag = "CERN22_ERAM18_mu+_1GeV" ;     prtcle = "#mu^{+} 1GeV_" ;   }
      if (iFile == 6) { EvtFile = "../Data_CERN22_vD/ERAM18_350V_412ns_pi+_0p5GeV_iter0.root" ;   Tag = "CERN22_ERAM18_pi+_0p5GeV" ;   prtcle = "#pi^{+} 0.5GeV_" ; }
      if (Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
      delete pUpld ;
    } 
    // if(DO_Separation)         DrawOut_Separation(OutDir, Comment) ;
    if(DO_Separation)         DrawOut_Separation_Reduced(OutDir, Comment) ;
  }
  


  // z scan scan with DESY21
  if(DESY_zscan){
    SelectionSet    = "T_DESY21_Event" ;
    intUploader     =  2 ;
    NbrOfMod        =  0 ; 
    Dt              = 310 ; TB = 50 ;
    // int PT_arr[] = {200, 412} ;
    int PT_arr[] = {200} ;
    for (int PT : PT_arr){
      OutDir        = Form("OUT_Tristan/DESY21_zscan/DESY21_zscan_PT%i/", PT) ; 
      MyMakeDir(OutDir) ; 
      Uploader* pUpld ; Interpol4 LUT ;
      if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
      // int         z_val[]   = {50, 150, 250, 350, 450, 550, 650, 750, 850, 950} ;
      // std::string z_arr[]   = {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"} ;
      int         z_val[]   = {50, 550, 950} ;
      std::string z_arr[]   = {"m40", "460", "860"} ;
      for (int iz = 0 ; iz < (int)std::size(z_arr) ; iz++){
        const char* z       = z_arr[iz].c_str() ;
        EvtFile             = Form("../Data_DESY21/zscan_PT%i/z_360_275_%i_02T_26_%s_iter0.root", PT, PT, z) ; Tag = Form("DESY21_z%s_PT%i", z, PT) ; prtcle = Form("electron_z%s", z) ;
        if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
        if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
        if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
        if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, z_val[iz]) ;
        if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
        if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
        // DrawOut_systematics(OutDir, EvtFile, Tag, Comment) ;
      }
      if(DO_Resolution)   DrawOut_Zscan  (Form("OUT_Tristan/DESY21_zscan/DESY21_zscan_PT%i", PT), Comment, PT) ;
    }
    if(DO_Global) DrawOut_Zscan_PT("OUT_Tristan/DESY21_zscan", Comment) ;
  }
  


  // y scan scan with DESY21
  if(DESY_yscan){
    SelectionSet    = "T_DESY21_Event" ;
    intUploader     =  2 ;
    NbrOfMod        =  0 ; 
    Dt              = 310 ; TB = 50 ; PT = 412 ; zdrift = 90 ;
    OutDir          = "OUT_Tristan/DESY21_yscan/" ; 
    MyMakeDir(OutDir) ; 
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int NFiles      = 12 ;
    std::string Y_arr[] = {"m150", "m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"} ;
    for (int y = 0 ; y < NFiles ; y++){
      EvtFile  = Form("../Data_DESY21/yscan/Y%s_Z0_iter0.root", Y_arr[y].c_str()) ; Tag = Form("DESY21_y%s", Y_arr[y].c_str()) ; prtcle = Form("electron_y%s", Y_arr[y].c_str()) ; 
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
    }
    if(DO_Resolution)   DrawOut_Yscan  ("OUT_Tristan/DESY21_yscan", Comment) ;
  }
  


  // Phi scan with DESY21
  if(DESY_phi){
    SelectionSet                = "T_DESY21_Event" ;
    intUploader                 =  2 ;
    NbrOfMod                    =  0 ;
    PT                          = 200 ; Dt = 310 ; TB = 40 ;
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int         z_val[]         = {50, 550, 950} ;
    std::string z_arr[]         = {"m40", "460", "860"} ;
    int         phi_arr[]       = {0, 5, 10, 20, 30, 30, 40, 45} ;
    for (int iz = 0 ; iz < (int)std::size(z_arr) ; iz++){
      const char* z             = z_arr[iz].c_str() ;
      OutDir                    = Form("OUT_Tristan/DESY21_phi/DESY21_phi_z%s/", z) ;  
      MyMakeDir(OutDir) ; 
      for (int ifile = 0 ; ifile < (int)std::size(phi_arr) ; ifile++){
        int phi                 = phi_arr[ifile] ;
        if(ifile < 5) {EvtFile  = Form("../Data_DESY21/Phi_scan_z%s/phi_200_%i_z%s_ym60_iter0.root", z, phi, z) ; Tag = Form("DESY21_phi%i_z%s", phi, z) ; prtcle = Form("electron_phi%i_z%s", phi, z) ; }
        else          {EvtFile  = Form("../Data_DESY21/Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter0.root", z, phi, z) ; Tag = Form("DESY21_phi%i_diag_z%s", phi, z) ; prtcle = Form("electron_phi%i_diag_z%s", phi, z) ; }

        if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
        if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
        if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
        if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, z_val[iz]) ;
        if (DO_Checks)    DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
        if (DO_Methods)   DrawOut_Methods           (OutDir, Tag, Comment, 1, prtcle) ;
        delete pUpld ;
      }
      if(DO_Resolution)   DrawOut_Phiscan (Form("OUT_Tristan/DESY21_phi/DESY21_phi_z%s", z), Comment, z) ;
    }
    // DrawOut_TGE_WFsum_L("OUT_Tristan/DESY21_phi/", Comment) ;
    if(DO_Global) DrawOut_Phiscan_Z("OUT_Tristan/DESY21_phi", Comment) ;
  }
  


  // Theta scan with DESY21
  if(DESY_theta){
    SelectionSet    = "T_DESY21_Event" ;
    OutDir          = "OUT_Tristan/DESY21_theta/" ;  
    MyMakeDir(OutDir) ; 
    intUploader     =  2 ;
    NbrOfMod        =  0 ;
    PT              = 200 ; Dt = 310 ; zdrift = 350 ; TB = 40 ;
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int theta_arr[] = {-45, -20, 20} ;
    for (int theta : theta_arr){
      if(theta == 20){EvtFile    = "../Data_DESY21/Theta_scan/theta_20_02T_z360_y80_iter0.root" ; Tag = Form("DESY21_theta%i", theta) ; prtcle = Form("electron_theta%i", theta) ; }
      else           {EvtFile    = Form("../Data_DESY21/Theta_scan/theta_m%i_02T_z260_y80_iter0.root", -theta) ; Tag = Form("DESY21_theta%i", theta) ; prtcle = Form("electron_theta%i", theta) ; }
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
    }
    if(DO_Resolution)   DrawOut_Thetascan ("OUT_Tristan/DESY21_theta",   Comment) ;
  }



  if(DO_Scans) DrawOut_Scans("OUT_Tristan", Comment);
  // DrawOut_Versions("OUT_Tristan/", "XP", "_zcalc_PRF_4IP_WF1", "_zcalc_PRF_4IP_Gain_WFoffdiag") ;
  // DrawOut_verif("OUT_Tristan/DESY21_phi/DESY21_phi_", Comment) ;
    // DrawOut_corrections() ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  int MC_zscan        = 0 ; 
  int DESY19_phi      = 0 ; 
  int DESY_mag        = 0 ;
  int MockUp_energy   = 0 ; 
  int MockUp_zdist    = 0 ; 
  int DESY_zscan_139V = 0 ; 


  // Phi scan with DESY19
  if(DESY19_phi){
    SelectionSet    = "T_DESY21_Event" ;
    intUploader     =  2 ;
    NbrOfMod        =  0 ;
    PT              = 412 ; Dt = 310 ; TB = 40 ; zdrift = 430 ;
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int phi_arr[]   = {0, 10, 20, 30, 40, 45} ;
    int NFiles      = 6 ;
    OutDir          = "OUT_Tristan/DESY19_phi/" ;
    MyMakeDir(OutDir) ; 
    for (int ifile = 3 ; ifile < NFiles ; ifile++){
      int phi                   = phi_arr[ifile] ;
      if(ifile < 3) { EvtFile  = Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi) ;       Tag = Form("DESY19_phi%i", phi) ;      prtcle = Form("electron_phi%i", phi) ; }
      else          { EvtFile  = Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi) ;  Tag = Form("DESY19_phi%i_diag", phi) ; prtcle = Form("electron_phi%i_diag", phi) ; }
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
      delete pUpld ;
    }
  }


  // z scan with MC
  if(MC_zscan){
    SelectionSet = "TMC_CERN22_Event" ;
    OutDir = "OUT_Tristan/CERN23_MC/MC_zscan/" ;  
    MyMakeDir(OutDir) ; 
    intUploader     =  3 ;
    NbrOfMod        = -1 ;
    PT              = 412 ; Dt = 286 ; TB = 40 ; // PT = 400 actually
    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int zmax = 9 ;
    for (int zDrift = 0 ; zDrift < zmax ; zDrift++){
      // EvtFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v2_iter4.root", zDrift+1) ; Tag = Form("CERN23_MC_z%i00", zDrift+1) ; prtcle = Form("MC %i0cm", zDrift+1) ;      
      EvtFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root", zDrift+1) ; Tag = Form("CERN23_MC_z%i00", zDrift+1) ; prtcle = Form("MC %i0cm NoOpt", zDrift+1) ;      
      // EvtFile = Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root", zDrift+1) ; Tag = Form("CERN23_MC_z%i00_old", zDrift+1) ; prtcle = Form("MC %i0cm old", zDrift+1) ;      
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, (zDrift+1)*100) ;
      if (DO_Checks)     DrawOut_Checks            (OutDir, EvtFile, Tag, Comment) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
    }
  }






  // Magnetic field scan scan with DESY21
  if(DESY_mag){
    SelectionSet    = "T_DESY21_Event" ;
    OutDir          = "OUT_Tristan/DESY21_mag/" ;  
    MyMakeDir(OutDir) ; 
    intUploader     =  2 ;
    NbrOfMod        =  0 ;
    PT              = 200 ; Dt = 310 ; zdrift = 950 ; TB = 40 ;

    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    // int mag_arr[] = {0, 2, 4, 10} ;
    int mag_arr[] = {2} ;
    for (int mag : mag_arr){
      EvtFile                    = Form            ("../Data_DESY21/mag_%i_200_ym60_iter0.root", mag) ; Tag = Form("DESY21_mag%i", mag) ; prtcle = Form("electron_mag%i", mag) ;
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
    }
  }



  // Energy scan using the mockup
  if(MockUp_energy){
    SelectionSet    = "T2_CERN22_Event" ;
    OutDir          = "OUT_Tristan/CERN22_Energy_Scan/" ;  
    MyMakeDir(OutDir) ;
    intUploader     =  3 ;
    NbrOfMod        = -1 ;
    PT              = 412 ; Dt = 350 ; zdrift = 415 ; TB = 40 ;

    Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
    int NFiles = 14 ;
    for (int iFile = 0 ; iFile < NFiles ; iFile++){
      if (iFile == 0)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root" ;       Tag = "CERN22_Mockup_Energy_e+_0p5GeV" ;  prtcle = "e^{+} 0p5GeV" ;}
      if (iFile == 1)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root" ;      Tag = "CERN22_Mockup_Energy_e+_0p75GeV" ; prtcle = "e^{+} 0p75GeV" ;}
      if (iFile == 2)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root" ;       Tag = "CERN22_Mockup_Energy_e+_1GeV" ;    prtcle = "e^{+} 1GeV" ;}  
      if (iFile == 3)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root" ;  Tag = "CERN22_Mockup_Energy_e+_1p25GeV" ; prtcle = "e^{+} 1p25GeV" ;}
      if (iFile == 4)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root" ;     Tag = "CERN22_Mockup_Energy_e+_1p5GeV" ;  prtcle = "e^{+} 1p5GeV" ;}
      if (iFile == 5)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root" ;     Tag = "CERN22_Mockup_Energy_mu_0p75GeV" ; prtcle = "#mu^{+} 0p75GeV" ;}
      if (iFile == 6)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root" ;      Tag = "CERN22_Mockup_Energy_mu_1GeV" ;    prtcle = "#mu^{+} 1GeV" ;}
      if (iFile == 7)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root" ;    Tag = "CERN22_Mockup_Energy_mu_1p5GeV" ;  prtcle = "#mu^{+} 1p5GeV" ;}
      if (iFile == 8)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root" ;     Tag = "CERN22_Mockup_Energy_pi_0p75GeV" ; prtcle = "#pi^{+} 0p75GeV" ;}
      if (iFile == 9)  { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root" ; Tag = "CERN22_Mockup_Energy_pi_1p25GeV" ; prtcle = "#pi^{+} 1p25GeV" ;}
      if (iFile == 10) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root" ;    Tag = "CERN22_Mockup_Energy_pi_1p5GeV" ;  prtcle = "#pi^{+} 1p5GeV" ;}
      if (iFile == 11) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root" ;        Tag = "CERN22_Mockup_Energy_p+_1GeV" ;    prtcle = "protons 1GeV" ;}
      if (iFile == 12) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root" ;     Tag = "CERN22_Mockup_Energy_p+_1p25GeV" ; prtcle = "protons 1p25GeV" ;}
      if (iFile == 13) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root" ;      Tag = "CERN22_Mockup_Energy_p+_1p5GeV" ;  prtcle = "protons 1p5GeV" ;}
      if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
      if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
      if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift) ;
      if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
      delete pUpld ;
    }
  }
  

  // z scan scan using the mockup
  if(MockUp_zdist){
    SelectionSet    = "T2_CERN22_Event" ;
    OutDir          = "OUT_Tristan/CERN22_Mockup_zscan/" ;  
    MyMakeDir(OutDir) ; 
    intUploader     =  3 ;
    NbrOfMod        = -1 ;
    PT              = 412 ; Dt = 350 ; zdrift = 415 ; TB = 40 ;

    int NFiles = 10 ;
    for (int iFile = 0 ; iFile < NFiles ; iFile++){
      if (iFile == 0) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root" ;       Tag = "CERN22_Mockup_e+_14V_z060" ;}
      if (iFile == 1) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root" ;  Tag = "CERN22_Mockup_e+_14V_z218" ;}
      if (iFile == 2) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root" ;      Tag = "CERN22_Mockup_e+_14V_z415" ;}
      if (iFile == 3) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root" ;      Tag = "CERN22_Mockup_e+_14V_z572" ;}
      if (iFile == 5) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root" ;       Tag = "CERN22_Mockup_e+_25V_z060" ;}
      if (iFile == 6) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root" ;    Tag = "CERN22_Mockup_e+_25V_z218" ;}
      if (iFile == 7) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root" ;      Tag = "CERN22_Mockup_e+_25V_z415" ;}
      if (iFile == 8) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root" ;      Tag = "CERN22_Mockup_e+_25V_z572" ;}
      if (iFile == 9) { EvtFile = "../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root" ;      Tag = "CERN22_Mockup_e+_25V_z925" ;}
      if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 8) ;
    }
  }
  


  // z scan scan with DESY21 at 139 V/cm
  if(DESY_zscan_139V){
    SelectionSet    = "T_DESY21_Event" ;
    intUploader     =  2 ;
    NbrOfMod        =  0 ; 
    Dt              = 310 ; TB = 50 ;
    int PT_arr[] = {200} ;
    for (int PT : PT_arr){
      OutDir        = Form("OUT_Tristan/DESY21_zscan/DESY21_zscan_PT%i_139V/", PT) ; 
      MyMakeDir(OutDir) ; 
      Uploader* pUpld ; Interpol4 LUT ;
    if (Control or dEdx) LUT = GiveMe_LUT(Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi200_nd200/", Dt, PT), nZ, nRC) ;
      int NFiles = 9 ;
      for (int zDrift = -1 ; zDrift < NFiles ; zDrift++){
        if(zDrift == -1) {EvtFile  = Form("../Data_DESY21/zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT) ;          Tag = Form("DESY21_zm40_PT%i", PT) ;          prtcle = "electron_z-40" ; }
        else {EvtFile              = Form("../Data_DESY21/zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift) ; Tag = Form("DESY21_z%i60_PT%i", zDrift, PT) ; prtcle = Form("electron_z%i60", zDrift) ; }
        if(Control or dEdx) pUpld = GiveMe_Uploader (intUploader, EvtFile) ;
        if (Control)      Tristan_CERN22_Control    (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, PT, TB, prtcle) ;
        if (DO_Control)   DrawOut_Control           (OutDir, Tag, Comment, SelectionSet, 1) ;
        if (dEdx)         Tristan_CERN22_dEdx       (OutDir, Tag, Comment, EvtFile, SelectionSet, pUpld, NbrOfMod, Data_to_Use, LUT, PT, TB, zdrift*100+150) ;
        if (DO_Methods)  DrawOut_Methods        (OutDir, Tag, Comment, 1, prtcle) ;
      }
      if(DO_Resolution)   DrawOut_Zscan  (Form("OUT_Tristan/DESY21_zscan/DESY21_zscan_PT%i_139V", PT), Comment, PT) ;
    }
  }




  OutDir = "OUT_Tristan/CERN22_ERAM18" ;
  if(DO_Displayer){
    SelectionSet          = "T2_CERN22_Event" ;
    EvtFile                = "../Data_CERN22_vD/ERAM18_350V_412ns_e+_1GeV_iter0.root" ; Tag = "CERN22_ERAM18_e+_1GeV" ; prtcle = "e^{+} 1GeV" ;
    Uploader* pUpld       = GiveMe_Uploader(intUploader, EvtFile) ; 
    Tristan_Displayer(OutDir, Tag, SelectionSet, pUpld, 0, Data_to_Use, prtcle) ;
    delete                  pUpld ;
  }

  if(WFcorr){
    SelectionSet          = "T_DESY21_Event" ;
    intUploader           =  2 ;
    NbrOfMod              =  0 ;
    std::string z_index[] = {"m40", "460", "860"} ;
    int phi_arr[]         = {30, 40, 45} ;
    for (int iz = 0 ; iz < (int)std::size(z_index) ; iz++){
      for (int iphi = 0 ; iphi < (int)std::size(phi_arr) ; iphi++){
        OutDir            = Form("../Data_DESY21/Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter0", z_index[iz].c_str(), phi_arr[iphi], z_index[iz].c_str()) ; 
        Tag               = Form("DESY21_phi%i_diag_z%s", phi_arr[iphi], z_index[iz].c_str()) ;
        Uploader* pUpld   = GiveMe_Uploader (intUploader, OutDir+".root") ;  
        Tristan_CERN22_corr(OutDir, Tag, SelectionSet, pUpld, NbrOfMod, Data_to_Use) ;
        delete pUpld ;
      }
    }
  }

}