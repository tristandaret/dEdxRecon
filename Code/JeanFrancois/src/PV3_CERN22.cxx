#include "JeanFrancois/PV3_CERN22.h"
#include "Misc/Util.h"

#include "Procedures/PV3_BASE.h"

void PV3_CERN22()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 3 ; // CERN22 Data 
  int StartFrom           = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "AN_CERN22_Sample" ;
  int Data_to_Use = 0 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min =  0 ;
  int ISAMPLE_Max = 25 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
    if(
// //        ISAMPLE!=10
//           ISAMPLE!=22
// //      &&ISAMPLE!=23
//         &&ISAMPLE!=24
//    &&   
// //        ISAMPLE!=7
//           ISAMPLE!=8
//         &&ISAMPLE!=18
//    &&
// //        ISAMPLE!=19
//           ISAMPLE!=20
//         &&ISAMPLE!=21
// &&   
//     ISAMPLE!=5
//   &&ISAMPLE!=6
       ISAMPLE!=17

    ) continue ;
  
//--------------------------------------------------------//ISAMPLE 5
    if(ISAMPLE==5){
      Tag = "C22_ZSCAN_25V" ;  

      Data_to_Use = 3 ;
      
      SelectionSet = "AN_CERN22_Sample" ;
      
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

          double ZZ = 0. ;
          if (iFl==0) ZZ =  60.0 ;
          if (iFl==1) ZZ = 218.5 ;
          if (iFl==2) ZZ = 415.0 ;
          if (iFl==3) ZZ = 427.0 ;
          if (iFl==4) ZZ = 572.0 ;
          if (iFl==5) ZZ = 572.0 ;
          if (iFl==6) ZZ = 925.0 ;

          double DD_Sample = ZZ + 40. ;
          
          double SamplingFrequency = 25. ; // in MHz

          std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

          PV3_BASE(
                   iOptQuiet               ,
                   Tag                     ,
                   OutDir                  ,
                   EvtFil                  ,
                   FILE_SC_VS_x_rc_dd      ,
                   DD_Sample               ,
                   intUploader             ,
                   StartFrom               ,
                   iOpt_TrackType          ,
                   SamplingFrequency       ,
                   ModuleNber              ,
                   SelectionSet            ,
                   Data_to_Use
                  );
        }
      }
    }
//--------------------------------------------------------//ISAMPLE 5

//--------------------------------------------------------//ISAMPLE 6
    if(ISAMPLE==6){
      Tag = "C22_ZSCAN_14V" ;  

      Data_to_Use = 3 ;
      
      SelectionSet = "AN_CERN22_Sample" ;
      
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

          double ZZ = 0. ;
          if (iFl==0) ZZ =  60.0 ;
          if (iFl==1) ZZ = 218.5 ;
          if (iFl==2) ZZ = 218.5 ;
          if (iFl==3) ZZ = 218.5 ;
          if (iFl==4) ZZ = 427.0 ;
          if (iFl==5) ZZ = 572.0 ;
          if (iFl==6) ZZ = 572.0 ;
          if (iFl==7) ZZ = 925.0 ;

          double DD_Sample = ZZ + 40. ;
          
          double SamplingFrequency = 25. ; // in MHz

          std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

          PV3_BASE(
                   iOptQuiet               ,
                   Tag                     ,
                   OutDir                  ,
                   EvtFil                  ,
                   FILE_SC_VS_x_rc_dd      ,
                   DD_Sample               ,
                   intUploader             ,
                   StartFrom               ,
                   iOpt_TrackType          ,
                   SamplingFrequency       ,
                   ModuleNber              ,
                   SelectionSet            ,
                   Data_to_Use
                  );
        }
      }
    }
//--------------------------------------------------------//ISAMPLE 6

//--------------------------------------------------------//ISAMPLE 17
    if(ISAMPLE==17){
    
      Data_to_Use = 3 ;

      SelectionSet = "AN_CERN22_Sample" ;
      
      intUploader = 3 ; // CERN22 Data
      
      int iCTD_Min = 0 ;
      int iCTD_Max = 8 ;
      for (int iCTD = iCTD_Min ; iCTD< iCTD_Max; iCTD++){
      
          if (
              iCTD!=4
             ) continue ;

        double CTD = 0. ;
        if (iCTD==0) CTD = 300. ;
        if (iCTD==1) CTD = 310. ;
        if (iCTD==2) CTD = 320. ;
        if (iCTD==3) CTD = 330. ;
        if (iCTD==4) CTD = 340. ;
        if (iCTD==5) CTD = 350. ;
        if (iCTD==6) CTD = 360. ;
        if (iCTD==7) CTD = 370. ;
        
        Tag = "C22_ZSCAN_25V_TD_" ;  

        std::ostringstream aostringstream_CTD ;
        aostringstream_CTD << std::setiosflags(std::ios::fixed) ;
        aostringstream_CTD << Tag  ;
        aostringstream_CTD << std::setprecision(0) << CTD   ;
        aostringstream_CTD << "_00"   ;       
        Tag = aostringstream_CTD.str() ;  ;

        Data_to_Use = 3 ;

        SelectionSet = "AN_CERN22_Sample" ;
        
        int iMod_Min = 4 ;
        int iMod_Max = 8 ;
        for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

          if (
              iMod!=4
             ) continue ;

          ModuleNber = iMod ;
          
          int iFl_Min = 0 ;
          int iFl_Max = 7 ;
          for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          
            if (
              iFl!=6
            ) continue ;
            
            if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root" ; }
            if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_noCollim_iter0.root" ; }
            if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root" ; }
            if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z427_noCollim_iter0.root" ; }
            if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root" ; }
            if (iFl==5) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_noCollim_iter0.root" ; }
            if (iFl==6) { EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root" ; }

            double ZZ = 0. ;
            if (iFl==0) ZZ =  60.0 ;
            if (iFl==1) ZZ = 218.5 ;
            if (iFl==2) ZZ = 415.0 ;
            if (iFl==3) ZZ = 427.0 ;
            if (iFl==4) ZZ = 572.0 ;
            if (iFl==5) ZZ = 572.0 ;
            if (iFl==6) ZZ = 925.0 ;

            double DD_Sample = ZZ + 40. ;
            DD_Sample *= std::pow((CTD/237.00),2) ;
            
            double SamplingFrequency = 25. ; // in MHz

            std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

            PV3_BASE(
                     iOptQuiet               ,
                     Tag                     ,
                     OutDir                  ,
                     EvtFil                  ,
                     FILE_SC_VS_x_rc_dd      ,
                     DD_Sample               ,
                     intUploader             ,
                     StartFrom               ,
                     iOpt_TrackType          ,
                     SamplingFrequency       ,
                     ModuleNber              ,
                     SelectionSet            ,
                     Data_to_Use
                    );
          }
        }
      }
    }
//--------------------------------------------------------//ISAMPLE 17

//--------------------------------------------------------//ISAMPLE 6
    if(ISAMPLE==6){
      Tag = "C22_ZSCAN_14V" ;  

      Data_to_Use = 3 ;
      
      SelectionSet = "AN_CERN22_Sample" ;
      
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

          double ZZ = 0. ;
          if (iFl==0) ZZ =  60.0 ;
          if (iFl==1) ZZ = 218.5 ;
          if (iFl==2) ZZ = 218.5 ;
          if (iFl==3) ZZ = 218.5 ;
          if (iFl==4) ZZ = 427.0 ;
          if (iFl==5) ZZ = 572.0 ;
          if (iFl==6) ZZ = 572.0 ;
          if (iFl==7) ZZ = 925.0 ;

          double DD_Sample = ZZ + 40. ;
          
          double SamplingFrequency = 25. ; // in MHz

          std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

          PV3_BASE(
                   iOptQuiet               ,
                   Tag                     ,
                   OutDir                  ,
                   EvtFil                  ,
                   FILE_SC_VS_x_rc_dd      ,
                   DD_Sample               ,
                   intUploader             ,
                   StartFrom               ,
                   iOpt_TrackType          ,
                   SamplingFrequency       ,
                   ModuleNber              ,
                   SelectionSet            ,
                   Data_to_Use
                  );
        }
      }
    }
//--------------------------------------------------------//ISAMPLE 6

//--------------------------------------------------------//ISAMPLE 7
    if(ISAMPLE==7){
      Tag = "SIM_221006_Data_to_Use_0" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

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

        double ZZ = 0. ;
        if (iFl==0) ZZ = 100.0 ;
        if (iFl==1) ZZ = 200.0 ;
        if (iFl==2) ZZ = 300.0 ;
        if (iFl==3) ZZ = 400.0 ;
        if (iFl==4) ZZ = 500.0 ;
        if (iFl==5) ZZ = 600.0 ;
        if (iFl==6) ZZ = 700.0 ;
        if (iFl==7) ZZ = 800.0 ;
        if (iFl==8) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 7

//--------------------------------------------------------//ISAMPLE 8
    if(ISAMPLE==8){
      Tag = "SIM_221006_Data_to_Use_3_DT_Unscaled" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

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

        double ZZ = 0. ;
        if (iFl==0) ZZ = 100.0 ;
        if (iFl==1) ZZ = 200.0 ;
        if (iFl==2) ZZ = 300.0 ;
        if (iFl==3) ZZ = 400.0 ;
        if (iFl==4) ZZ = 500.0 ;
        if (iFl==5) ZZ = 600.0 ;
        if (iFl==6) ZZ = 700.0 ;
        if (iFl==7) ZZ = 800.0 ;
        if (iFl==8) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 8

//--------------------------------------------------------//ISAMPLE 18
    if(ISAMPLE==18){
      Tag = "SIM_221006_Data_to_Use_3_DT_311_00" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

      intUploader = 3 ; // CERN22 Data
      
      int iFl_Min = 0 ;
      int iFl_Max = 9 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
//         if(
//            iFl!=0
//          &&iFl!=8
//         ) continue ;
        if (iFl==0) { EvtFil = "../Sim_Data/z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==1) { EvtFil = "../Sim_Data/z_400_nomDrift_20cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==2) { EvtFil = "../Sim_Data/z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==3) { EvtFil = "../Sim_Data/z_400_nomDrift_40cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==4) { EvtFil = "../Sim_Data/z_400_nomDrift_50cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==5) { EvtFil = "../Sim_Data/z_400_nomDrift_60cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==6) { EvtFil = "../Sim_Data/z_400_nomDrift_70cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==7) { EvtFil = "../Sim_Data/z_400_nomDrift_80cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==8) { EvtFil = "../Sim_Data/z_400_nomDrift_90cm_MD_RC100_v2_iter4.root" ; }

        double ZZ = 0. ;
        if (iFl==0) ZZ = 100.0 ;
        if (iFl==1) ZZ = 200.0 ;
        if (iFl==2) ZZ = 300.0 ;
        if (iFl==3) ZZ = 400.0 ;
        if (iFl==4) ZZ = 500.0 ;
        if (iFl==5) ZZ = 600.0 ;
        if (iFl==6) ZZ = 700.0 ;
        if (iFl==7) ZZ = 800.0 ;
        if (iFl==8) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        DD_Sample *= std::pow((311.00/237.00),2) ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 18

//--------------------------------------------------------//ISAMPLE 19
    if(ISAMPLE==19){
      Tag = "SIM_RcUnknown_221006_Data_to_Use_0" ;  

      ModuleNber = 0 ;

      Data_to_Use = 0 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

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

        double ZZ = 0. ;
        if (iFl==0) ZZ =  50.0 ;
        if (iFl==1) ZZ = 100.0 ;
        if (iFl==2) ZZ = 200.0 ;
        if (iFl==3) ZZ = 300.0 ;
        if (iFl==4) ZZ = 400.0 ;
        if (iFl==5) ZZ = 500.0 ;
        if (iFl==6) ZZ = 600.0 ;
        if (iFl==7) ZZ = 700.0 ;
        if (iFl==8) ZZ = 800.0 ;
        if (iFl==9) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 19

//--------------------------------------------------------//ISAMPLE 20
    if(ISAMPLE==20){
      Tag = "SIM_RcUnknown_221006_Data_to_Use_3_DT_Unscaled" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

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

        double ZZ = 0. ;
        if (iFl==0) ZZ =  50.0 ;
        if (iFl==1) ZZ = 100.0 ;
        if (iFl==2) ZZ = 200.0 ;
        if (iFl==3) ZZ = 300.0 ;
        if (iFl==4) ZZ = 400.0 ;
        if (iFl==5) ZZ = 500.0 ;
        if (iFl==6) ZZ = 600.0 ;
        if (iFl==7) ZZ = 700.0 ;
        if (iFl==8) ZZ = 800.0 ;
        if (iFl==9) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 20

//--------------------------------------------------------//ISAMPLE 21
    if(ISAMPLE==21){
      Tag = "SIM_RcUnknown_221006_Data_to_Use_3_DT_311_00" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;

      SelectionSet = "AN_CERN22_Sample_Sim" ;

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

        double ZZ = 0. ;
        if (iFl==0) ZZ =  50.0 ;
        if (iFl==1) ZZ = 100.0 ;
        if (iFl==2) ZZ = 200.0 ;
        if (iFl==3) ZZ = 300.0 ;
        if (iFl==4) ZZ = 400.0 ;
        if (iFl==5) ZZ = 500.0 ;
        if (iFl==6) ZZ = 600.0 ;
        if (iFl==7) ZZ = 700.0 ;
        if (iFl==8) ZZ = 800.0 ;
        if (iFl==9) ZZ = 900.0 ;

        double DD_Sample = ZZ + 0. ;
        DD_Sample *= std::pow((311.00/237.00),2) ;
        
        double SamplingFrequency = 25. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      }
    }
//--------------------------------------------------------//ISAMPLE 21

//--------------------------------------------------------//ISAMPLE 10
    if(ISAMPLE==10){
      Tag = "SIMHM_Data_to_Use_0" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "AN_CERN22_SimHomeMade" ;

      intUploader = 100 ; // Simulation Home Made

//    EvtFil = "../Sim_Data/Sim_02_NoSmearing.root" ;
//    EvtFil = "../Sim_Data/Sim_02_CteSigma.root" ;
      EvtFil = "../Sim_Data/Sim_02_SqrtASigma.root" ;
      
      double DD_Sample = 0. ;
        
      double SamplingFrequency = 25. ; // in MHz

      std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

      PV3_BASE(
               iOptQuiet               ,
               Tag                     ,
               OutDir                  ,
               EvtFil                  ,
               FILE_SC_VS_x_rc_dd      ,
               DD_Sample               ,
               intUploader             ,
               StartFrom               ,
               iOpt_TrackType          ,
               SamplingFrequency       ,
               ModuleNber              ,
               SelectionSet            ,
               Data_to_Use
              );
      
    }
//--------------------------------------------------------//ISAMPLE  10

//--------------------------------------------------------//ISAMPLE 22
    if(ISAMPLE==22){
      Tag = "SIMHM_Data_to_Use_3" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;
      
      SelectionSet = "AN_CERN22_SimHomeMade" ;

      intUploader = 100 ; // Simulation Home Made

//    EvtFil = "../Sim_Data/Sim_02_NoSmearing.root" ;
//    EvtFil = "../Sim_Data/Sim_02_CteSigma.root" ;
      EvtFil = "../Sim_Data/Sim_02_SqrtASigma.root" ;
      
      double DD_Sample = 0. ;
        
      double SamplingFrequency = 25. ; // in MHz

      std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

      PV3_BASE(
               iOptQuiet               ,
               Tag                     ,
               OutDir                  ,
               EvtFil                  ,
               FILE_SC_VS_x_rc_dd      ,
               DD_Sample               ,
               intUploader             ,
               StartFrom               ,
               iOpt_TrackType          ,
               SamplingFrequency       ,
               ModuleNber              ,
               SelectionSet            ,
               Data_to_Use
              );
      
    }
//--------------------------------------------------------//ISAMPLE  22

//--------------------------------------------------------//ISAMPLE 23
    if(ISAMPLE==23){
      Tag = "SIMHM_Data_to_Use_0" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "AN_CERN22_SimHomeMade" ;

      intUploader = 100 ; // Simulation Home Made

      EvtFil = "../Sim_Data/Sim_02_NoSmearing.root" ;
//    EvtFil = "../Sim_Data/Sim_02_CteSigma.root" ;
//    EvtFil = "../Sim_Data/Sim_02_SqrtASigma.root" ;
      
      double DD_Sample = 0. ;
        
      double SamplingFrequency = 25. ; // in MHz

      std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

      PV3_BASE(
               iOptQuiet               ,
               Tag                     ,
               OutDir                  ,
               EvtFil                  ,
               FILE_SC_VS_x_rc_dd      ,
               DD_Sample               ,
               intUploader             ,
               StartFrom               ,
               iOpt_TrackType          ,
               SamplingFrequency       ,
               ModuleNber              ,
               SelectionSet            ,
               Data_to_Use
              );
      
    }
//--------------------------------------------------------//ISAMPLE  23

//--------------------------------------------------------//ISAMPLE 24
    if(ISAMPLE==24){
      Tag = "SIMHM_Data_to_Use_3" ;  

      ModuleNber = 0 ;
      
      Data_to_Use = 3 ;
      
      SelectionSet = "AN_CERN22_SimHomeMade" ;

      intUploader = 100 ; // Simulation Home Made

      EvtFil = "../Sim_Data/Sim_02_NoSmearing.root" ;
//    EvtFil = "../Sim_Data/Sim_02_CteSigma.root" ;
//    EvtFil = "../Sim_Data/Sim_02_SqrtASigma.root" ;
      
      double DD_Sample = 0. ;
        
      double SamplingFrequency = 25. ; // in MHz

      std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

      PV3_BASE(
               iOptQuiet               ,
               Tag                     ,
               OutDir                  ,
               EvtFil                  ,
               FILE_SC_VS_x_rc_dd      ,
               DD_Sample               ,
               intUploader             ,
               StartFrom               ,
               iOpt_TrackType          ,
               SamplingFrequency       ,
               ModuleNber              ,
               SelectionSet            ,
               Data_to_Use
              );
      
    }
//--------------------------------------------------------//ISAMPLE  24

  }
}
