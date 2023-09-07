#include "JeanFrancois/EVT_CERN22_01.h"
#include "Misc/Util.h"

#include "Procedures/EVT_01_BASE.h"

void EVT_CERN22_01()
{ 
//
  std::string OutDir       = "OUT_JeanFrancois/" ;

  int iOptQuiet            = 0 ;

  std::string SelectionSet = "BN_CERN22_Event" ;

  int intUploader          =  3 ; // CERN22 Data 
  int ModuleNber           = -1 ; // Work on all modules nbers
  int Data_to_Use          = 0  ; // Use ntuple Maxs

//
  std::string Tag = "BIDON" ;

  std::string EvtFil = "BIDON" ;

  double Stg120_L  = -1. ;
  double Stg120_H = 1000. ;
  
//
  int ISAMPLE_Min = 5 ;
  int ISAMPLE_Max = 6 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
  
//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "EVT_CERN22_01" ;  
      
      Stg120_L = 160. ; Stg120_H = 220. ; EvtFil = "../Real_Data/CERN22_V6/All_ERAMS_350V_412ns_e+_0p5GeV_1_iter2.root" ;
      
      EVT_01_BASE(
                         iOptQuiet ,
                         Tag ,
                         OutDir ,
                         EvtFil ,
                         SelectionSet ,
                         intUploader ,
                         ModuleNber  ,
                         Data_to_Use ,
                         Stg120_L ,
                         Stg120_H 
                        ) ;
    }
//--------------------------------------------------------//ISAMPLE 0

//--------------------------------------------------------//ISAMPLE 1
    if(ISAMPLE==1){
      Tag = "EVT_CERN22_01_ZSCAN_25V" ;  

      int iFl_Min = 0 ;
      int iFl_Max = 7 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { Stg120_L = 68.-40. ; Stg120_H = 68.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root" ; }
        if (iFl==1) { Stg120_L =127.-40. ; Stg120_H =127.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_noCollim_iter0.root" ; }
        if (iFl==2) { Stg120_L =185.-40. ; Stg120_H =185.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root" ; }
        if (iFl==3) { Stg120_L =192.-40. ; Stg120_H =192.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z427_noCollim_iter0.root" ; }
        if (iFl==4) { Stg120_L =244.-40. ; Stg120_H =244.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root" ; }
        if (iFl==5) { Stg120_L =243.-40. ; Stg120_H =243.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_noCollim_iter0.root" ; }
        if (iFl==6) { Stg120_L =370.-40. ; Stg120_H =370.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_25V/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root" ; }
  
        EVT_01_BASE(
                           iOptQuiet ,
                           Tag ,
                           OutDir ,
                           EvtFil ,
                           SelectionSet ,
                           intUploader ,
                           ModuleNber  ,
                           Data_to_Use ,
                           Stg120_L ,
                           Stg120_H 
                          ) ;
      }
    }
//--------------------------------------------------------//ISAMPLE 1

//--------------------------------------------------------//ISAMPLE 2
    if(ISAMPLE==2){
      Tag = "EVT_CERN22_01_ZSCAN_14V" ;  

      int iFl_Min = 0 ;
      int iFl_Max = 8 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { Stg120_L = 83.-40. ; Stg120_H = 83.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root" ; }
        if (iFl==1) { Stg120_L =165.-40. ; Stg120_H =165.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root" ; }
        if (iFl==2) { Stg120_L =162.-40. ; Stg120_H =162.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_2_iter0.root" ; }
        if (iFl==3) { Stg120_L =160.-40. ; Stg120_H =160.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_noCollim_iter0.root" ; }
        if (iFl==4) { Stg120_L =257.-40. ; Stg120_H =257.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z427_noCollim_iter0.root" ; }
        if (iFl==5) { Stg120_L =331.-40. ; Stg120_H =331.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root" ; }
        if (iFl==6) { Stg120_L =332.-40. ; Stg120_H =332.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_noCollim_iter0.root" ; }
        if (iFl==7) { Stg120_L =504.-40. ; Stg120_H =504.+40. ; EvtFil = "../Real_Data/CERN22_V6/ZSCAN_14V/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z925_iter0.root" ; }

        EVT_01_BASE(
                           iOptQuiet ,
                           Tag ,
                           OutDir ,
                           EvtFil ,
                           SelectionSet ,
                           intUploader ,
                           ModuleNber  ,
                           Data_to_Use ,
                           Stg120_L ,
                           Stg120_H 
                          ) ;
      }
      
    }
//--------------------------------------------------------//ISAMPLE 2

//--------------------------------------------------------//ISAMPLE 3
    if(ISAMPLE==3){
      Tag = "EVT_CERN22_01_ERAM18_HS" ;  

      int iFl_Min = 0 ;
      int iFl_Max = 1 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_1GeV_iter0.root" ; }
        if (iFl==1) { Stg120_L =180.-40. ; Stg120_H =180.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ; }
        if (iFl==2) { Stg120_L =176.-40. ; Stg120_H =176.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu-_1GeV_iter0.root" ; }
        if (iFl==4) { Stg120_L =183.-40. ; Stg120_H =183.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_1GeV_iter0.root" ; }
        if (iFl==5) { Stg120_L =182.-40. ; Stg120_H =182.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_0p5GeV_iter0.root" ; }
        if (iFl==6) { Stg120_L =179.-40. ; Stg120_H =179.+40. ; EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_p_1GeV_iter0.root" ; }

        EVT_01_BASE(
                           iOptQuiet ,
                           Tag ,
                           OutDir ,
                           EvtFil ,
                           SelectionSet ,
                           intUploader ,
                           ModuleNber  ,
                           Data_to_Use ,
                           Stg120_L ,
                           Stg120_H 
                          ) ;
      }
      
    }
//--------------------------------------------------------//ISAMPLE 3

//--------------------------------------------------------//ISAMPLE 4
    if(ISAMPLE==4){
      Tag = "EVT_CERN22_01_SIM_221006" ;  

      int iFl_Min = 0 ;
      int iFl_Max = 1 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==1) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_20cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==2) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==3) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_40cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==4) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_50cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==5) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_60cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==6) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_70cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==7) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_80cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==8) { Stg120_L =184.-40. ; Stg120_H =184.+40. ; EvtFil = "../Sim_Data/z_400_nomDrift_90cm_MD_RC100_v2_iter4.root" ; }

        EVT_01_BASE(
                           iOptQuiet ,
                           Tag ,
                           OutDir ,
                           EvtFil ,
                           SelectionSet ,
                           intUploader ,
                           ModuleNber  ,
                           Data_to_Use ,
                           Stg120_L ,
                           Stg120_H 
                          ) ;
      }
      
    }
//--------------------------------------------------------//ISAMPLE 4

//--------------------------------------------------------//ISAMPLE 5
    if(ISAMPLE==5){
      Tag = "EVT_SIMHM" ;  

      Data_to_Use = 0 ;
      
      intUploader = 100 ; // Simulation Home Made

      std::string SelectionSet = "AN_CERN22_SimHomeMade" ;

      ModuleNber = 0 ;
      
      Stg120_L =0.; // NB there no stage120 in AN_CERN22_SimHomeMade
      Stg120_H =0.; 
      
      EvtFil = "../Sim_Data/Sim_02_SqrtASigma.root" ;

      EVT_01_BASE(
                         iOptQuiet ,
                         Tag ,
                         OutDir ,
                         EvtFil ,
                         SelectionSet ,
                         intUploader ,
                         ModuleNber  ,
                         Data_to_Use ,
                         Stg120_L ,
                         Stg120_H 
                        ) ;
      
    }
//--------------------------------------------------------//ISAMPLE 5

//--------------------------------------------------------//ISAMPLE 6
    if(ISAMPLE==6){
      Tag = "SIM_221006_30cm" ;  

      Data_to_Use = 3 ;
      
      intUploader         = 3 ; // CERN22 Data 

      std::string SelectionSet = "AN_CERN22_Sample_Sim" ;

      ModuleNber = 0 ;
      
      Stg120_L =0.; // NB there no stage120 in AN_CERN22_Sample_Sim
      Stg120_H =1000.; 
      
      EvtFil = "../Sim_Data/z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ;  

      EVT_01_BASE(
                         iOptQuiet ,
                         Tag ,
                         OutDir ,
                         EvtFil ,
                         SelectionSet ,
                         intUploader ,
                         ModuleNber  ,
                         Data_to_Use ,
                         Stg120_L ,
                         Stg120_H 
                        ) ;
      
    }
//--------------------------------------------------------//ISAMPLE 6

//--------------------------------------------------------//ISAMPLE 7
    if(ISAMPLE==7){
      Tag = "SIM_221006_30cm" ;  

      Data_to_Use = 3 ;
      
      intUploader         = 3 ; // CERN22 Data 

      std::string SelectionSet = "AN_CERN22_Sample_Sim" ;

      ModuleNber = 0 ;
      
      Stg120_L =0.; // NB there no stage120 in AN_CERN22_Sample_Sim
      Stg120_H =1000.; 
      
      EvtFil = "../Sim_Data/z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ;  

      EVT_01_BASE(
                         iOptQuiet ,
                         Tag ,
                         OutDir ,
                         EvtFil ,
                         SelectionSet ,
                         intUploader ,
                         ModuleNber  ,
                         Data_to_Use ,
                         Stg120_L ,
                         Stg120_H 
                        ) ;
      
    }
//--------------------------------------------------------//ISAMPLE 7

  }



}
