#include "JeanFrancois/PV4.h"
#include "Misc/Util.h"

#include "Procedures/PV4_BASE.h"

void PV4_CERN22()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 3 ; // CERN22 Data 
  int StartFrom           = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "AN_CERN22_Sample" ;
  int Data_to_Use          = 0 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 1 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
  
//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "ERAM18_HS" ;
      
      ModuleNber = 0 ;
      
      int iFl_Min = 0 ;
      int iFl_Max = 1 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_1GeV_iter0.root" ; }
        if (iFl==1) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ; }
        if (iFl==2) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu-_1GeV_iter0.root" ; }
        if (iFl==3) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_1GeV_iter0.root" ; }
        if (iFl==4) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_0p5GeV_iter0.root" ; }
        if (iFl==5) { EvtFil = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_p_1GeV_iter0.root" ; }


        double ZZ = 415. ;

        double DD_Sample = ZZ + 40. ;
        
        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV4_BASE(
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
//--------------------------------------------------------//
    }// ISAMPLE 0  


  }

}
