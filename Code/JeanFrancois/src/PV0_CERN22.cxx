#include "JeanFrancois/PV0_CERN22.h"
#include "Misc/Util.h"

#include "Procedures/PV0_BASE.h"

void PV0_CERN22()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 1 ;
  
  int intUploader         = 3 ; // CERN22 Data 
  int StartFrom           = 0 ;
  int iOpt_Initial_YT_fit = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 4 ;
  
  std::string SelectionSet = "BN_CERN22_Sample" ;
  int Data_to_Use          = 0 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 25 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
    if (
//      ISAMPLE!=  7
        
        ISAMPLE!= 19
        
    ) continue ;

//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "ERAM18_HS" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iFl_Min = 0 ;
      int iFl_Max = 6 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_1GeV_iter0.root" ; }
        if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ; }
        if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu-_1GeV_iter0.root" ; }
        if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_1GeV_iter0.root" ; }
        if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_0p5GeV_iter0.root" ; }
        if (iFl==5) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_p_1GeV_iter0.root" ; }

        std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
                 iOptQuiet           ,
                 Tag                 ,
                 OutDir              ,
                 EvtFil              ,
                 PRFfile_Fixed_RC_DD ,
                 intUploader         ,
                 StartFrom           ,
                 iOpt_Initial_YT_fit ,
                 iOpt_TrackType      ,
                 ModuleNber          ,
                 SelectionSet        ,
                 Data_to_Use
                );
      }
      
    }
    
//--------------------------------------------------------//ISAMPLE 0

//--------------------------------------------------------//ISAMPLE 1
    if(ISAMPLE==1){
      Tag = "C22_PIP_ESCAN" ;  

      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 0 ;
      int iMod_Max = 4 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;

        int iFl_Min = 0 ;
        int iFl_Max = 3 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 1

//--------------------------------------------------------//ISAMPLE 2
    if(ISAMPLE==2){
      Tag = "C22_EP_ESCAN" ;  

      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 0 ;
      int iMod_Max = 4 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;

        int iFl_Min = 0 ;
        int iFl_Max = 5 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root" ; }
          if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root" ; }
          if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 2

//--------------------------------------------------------//ISAMPLE 3
    if(ISAMPLE==3){
      Tag = "C22_P_ESCAN" ;  

      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 0 ;
      int iMod_Max = 4 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;

        int iFl_Min = 0 ;
        int iFl_Max = 3 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 3

//--------------------------------------------------------//ISAMPLE 4
    if(ISAMPLE==4){
      Tag = "C22_MUP_ESCAN" ;  
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 0 ;
      int iMod_Max = 4 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;

        int iFl_Min = 0 ;
        int iFl_Max = 3 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ESCAN/All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 4

//--------------------------------------------------------//ISAMPLE 5
    if(ISAMPLE==5){
      Tag = "C22_ZSCAN_25V" ;  

      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 4 ;
      int iMod_Max = 8 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;
      
        int iFl_Min = 0 ;
        int iFl_Max = 7 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_noCollim_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root" ; }
          if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z427_noCollim_iter0.root" ; }
          if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root" ; }
          if (iFl==5) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_noCollim_iter0.root" ; }
          if (iFl==6) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 5

//--------------------------------------------------------//ISAMPLE 6
    if(ISAMPLE==6){
      Tag = "C22_ZSCAN_14V" ;  

      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iMod_Min = 4 ;
      int iMod_Max = 8 ;
      for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
        ModuleNber = iMod ;
      
        int iFl_Min = 0 ;
        int iFl_Max = 8 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root" ; }
          if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root" ; }
          if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_2_iter0.root" ; }
          if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_noCollim_iter0.root" ; }
          if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z427_noCollim_iter0.root" ; }
          if (iFl==5) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root" ; }
          if (iFl==6) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_noCollim_iter0.root" ; }
          if (iFl==7) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z925_iter0.root" ; }

          std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

          PV0_BASE(
                   iOptQuiet           ,
                   Tag                 ,
                   OutDir              ,
                   EvtFil              ,
                   PRFfile_Fixed_RC_DD ,
                   intUploader         ,
                   StartFrom           ,
                   iOpt_Initial_YT_fit ,
                   iOpt_TrackType      ,
                   ModuleNber          ,
                   SelectionSet        ,
                   Data_to_Use
                  );
        }
      }
    }
    
//--------------------------------------------------------//ISAMPLE 6

//--------------------------------------------------------//ISAMPLE 7
    if(ISAMPLE==7){
      Tag = "SIM_221006" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample_Sim" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iFl_Min = 0 ;
      int iFl_Max = 9 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { EvtFil = "../Sim_Data/z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==1) { EvtFil = "../Sim_Data/z_400_nomDrift_20cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==2) { EvtFil = "../Sim_Data/z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==3) { EvtFil = "../Sim_Data/z_400_nomDrift_40cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==4) { EvtFil = "../Sim_Data/z_400_nomDrift_50cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==5) { EvtFil = "../Sim_Data/z_400_nomDrift_60cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==6) { EvtFil = "../Sim_Data/z_400_nomDrift_70cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==7) { EvtFil = "../Sim_Data/z_400_nomDrift_80cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==8) { EvtFil = "../Sim_Data/z_400_nomDrift_90cm_MD_RC100_v2_iter4.root" ; }

        std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
                 iOptQuiet           ,
                 Tag                 ,
                 OutDir              ,
                 EvtFil              ,
                 PRFfile_Fixed_RC_DD ,
                 intUploader         ,
                 StartFrom           ,
                 iOpt_Initial_YT_fit ,
                 iOpt_TrackType      ,
                 ModuleNber          ,
                 SelectionSet        ,
                 Data_to_Use
                );
      }
    }
    
//--------------------------------------------------------//ISAMPLE 7

//--------------------------------------------------------//ISAMPLE 19
    if(ISAMPLE==19){
      Tag = "SIM_RcUnknown_221006" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_CERN22_Sample_Sim" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { EvtFil = "../Sim_Data/z_400_nomDrift_5cm_iter4.root"  ; }
        if (iFl==1) { EvtFil = "../Sim_Data/z_400_nomDrift_10cm_iter4.root" ; }
        if (iFl==2) { EvtFil = "../Sim_Data/z_400_nomDrift_20cm_iter4.root" ; }
        if (iFl==3) { EvtFil = "../Sim_Data/z_400_nomDrift_30cm_iter4.root" ; }
        if (iFl==4) { EvtFil = "../Sim_Data/z_400_nomDrift_40cm_iter4.root" ; }
        if (iFl==5) { EvtFil = "../Sim_Data/z_400_nomDrift_50cm_iter4.root" ; }
        if (iFl==6) { EvtFil = "../Sim_Data/z_400_nomDrift_60cm_iter4.root" ; }
        if (iFl==7) { EvtFil = "../Sim_Data/z_400_nomDrift_70cm_iter4.root" ; }
        if (iFl==8) { EvtFil = "../Sim_Data/z_400_nomDrift_80cm_iter4.root" ; }
        if (iFl==9) { EvtFil = "../Sim_Data/z_400_nomDrift_90cm_iter4.root" ; }

        std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
                 iOptQuiet           ,
                 Tag                 ,
                 OutDir              ,
                 EvtFil              ,
                 PRFfile_Fixed_RC_DD ,
                 intUploader         ,
                 StartFrom           ,
                 iOpt_Initial_YT_fit ,
                 iOpt_TrackType      ,
                 ModuleNber          ,
                 SelectionSet        ,
                 Data_to_Use
                );
      }
    }
    
//--------------------------------------------------------//ISAMPLE 19

  }

}
