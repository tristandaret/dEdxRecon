#include "Analysis_JF/Analysis_Files.h"
#include "Misc/Util.h"

Analysis_Files::Analysis_Files(const std::string& SampleSet)
{

  m_SampleSet = SampleSet ;
  
//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "D21_ZSCAN_200" ){
    m_FilesDir = "OUT_Prod/Tristan_DESY21/ZSCAN_200/" ; 
    m_NberOfFiles = 10 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
          if (iFl==0) EvtFil = "z_360_275_200_02T_26_m40_iter0.root" ;
          if (iFl==1) EvtFil = "z_360_275_200_02T_26_60_iter0.root"  ;
          if (iFl==2) EvtFil = "z_360_275_200_02T_26_160_iter0.root" ;
          if (iFl==3) EvtFil = "z_360_275_200_02T_26_260_iter0.root" ;
          if (iFl==4) EvtFil = "z_360_275_200_02T_26_360_iter0.root" ;
          if (iFl==5) EvtFil = "z_360_275_200_02T_26_460_iter0.root" ;
          if (iFl==6) EvtFil = "z_360_275_200_02T_26_560_iter0.root" ;
          if (iFl==7) EvtFil = "z_360_275_200_02T_26_660_iter0.root" ;
          if (iFl==8) EvtFil = "z_360_275_200_02T_26_760_iter0.root" ;
          if (iFl==9) EvtFil = "z_360_275_200_02T_26_860_iter0.root" ;

       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Z" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = -40.00  ;
       if (iFl==1) VariableValue =  60.00  ;
       if (iFl==2) VariableValue = 160.00  ;
       if (iFl==3) VariableValue = 260.00  ;
       if (iFl==4) VariableValue = 360.00  ;
       if (iFl==5) VariableValue = 460.00  ;
       if (iFl==6) VariableValue = 560.00  ;
       if (iFl==7) VariableValue = 660.00  ;
       if (iFl==8) VariableValue = 760.00  ;
       if (iFl==9) VariableValue = 860.00  ;

       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "D21_ZSCAN" ){
    m_FilesDir = "OUT_Prod/Tristan_DESY21/ZSCAN/" ; 
    m_NberOfFiles = 10 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
          if (iFl==0) EvtFil = "z_360_275_412_02T_26_m40_iter0.root" ;
          if (iFl==1) EvtFil = "z_360_275_412_02T_26_60_iter0.root"  ;
          if (iFl==2) EvtFil = "z_360_275_412_02T_26_160_iter0.root" ;
          if (iFl==3) EvtFil = "z_360_275_412_02T_26_260_iter0.root" ;
          if (iFl==4) EvtFil = "z_360_275_412_02T_26_360_iter0.root" ;
          if (iFl==5) EvtFil = "z_360_275_412_02T_26_460_iter0.root" ;
          if (iFl==6) EvtFil = "z_360_275_412_02T_26_560_iter0.root" ;
          if (iFl==7) EvtFil = "z_360_275_412_02T_26_660_iter0.root" ;
          if (iFl==8) EvtFil = "z_360_275_412_02T_26_760_iter0.root" ;
          if (iFl==9) EvtFil = "z_360_275_412_02T_26_860_iter0.root" ;

       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Z" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = -40.00  ;
       if (iFl==1) VariableValue =  60.00  ;
       if (iFl==2) VariableValue = 160.00  ;
       if (iFl==3) VariableValue = 260.00  ;
       if (iFl==4) VariableValue = 360.00  ;
       if (iFl==5) VariableValue = 460.00  ;
       if (iFl==6) VariableValue = 560.00  ;
       if (iFl==7) VariableValue = 660.00  ;
       if (iFl==8) VariableValue = 760.00  ;
       if (iFl==9) VariableValue = 860.00  ;

       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "SIM_221006" ){
    m_FilesDir = "OUT_Prod/SIM/" ;
    m_NberOfFiles = 9 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==1) { EvtFil = "z_400_nomDrift_20cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==2) { EvtFil = "z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==3) { EvtFil = "z_400_nomDrift_40cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==4) { EvtFil = "z_400_nomDrift_50cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==5) { EvtFil = "z_400_nomDrift_60cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==6) { EvtFil = "z_400_nomDrift_70cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==7) { EvtFil = "z_400_nomDrift_80cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==8) { EvtFil = "z_400_nomDrift_90cm_MD_RC100_v2_iter4.root" ; }

       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Z" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 100.0 ;
       if (iFl==1) VariableValue = 200.0 ;
       if (iFl==2) VariableValue = 300.0 ;
       if (iFl==3) VariableValue = 400.0 ;
       if (iFl==4) VariableValue = 500.0 ;
       if (iFl==5) VariableValue = 600.0 ;
       if (iFl==6) VariableValue = 700.0 ;
       if (iFl==7) VariableValue = 800.0 ;
       if (iFl==8) VariableValue = 900.0 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "SIM_RcUnknown_221006" ){
    m_FilesDir = "OUT_Prod/SIM/" ;
    m_NberOfFiles = 10 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "z_400_nomDrift_5cm_iter4.root"  ; }
        if (iFl==1) { EvtFil = "z_400_nomDrift_10cm_iter4.root" ; }
        if (iFl==2) { EvtFil = "z_400_nomDrift_20cm_iter4.root" ; }
        if (iFl==3) { EvtFil = "z_400_nomDrift_30cm_iter4.root" ; }
        if (iFl==4) { EvtFil = "z_400_nomDrift_40cm_iter4.root" ; }
        if (iFl==5) { EvtFil = "z_400_nomDrift_50cm_iter4.root" ; }
        if (iFl==6) { EvtFil = "z_400_nomDrift_60cm_iter4.root" ; }
        if (iFl==7) { EvtFil = "z_400_nomDrift_70cm_iter4.root" ; }
        if (iFl==8) { EvtFil = "z_400_nomDrift_80cm_iter4.root" ; }
        if (iFl==9) { EvtFil = "z_400_nomDrift_90cm_iter4.root" ; }

       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Z" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue =  50.0 ;
       if (iFl==1) VariableValue = 100.0 ;
       if (iFl==2) VariableValue = 200.0 ;
       if (iFl==3) VariableValue = 300.0 ;
       if (iFl==4) VariableValue = 400.0 ;
       if (iFl==5) VariableValue = 500.0 ;
       if (iFl==6) VariableValue = 600.0 ;
       if (iFl==7) VariableValue = 700.0 ;
       if (iFl==8) VariableValue = 800.0 ;
       if (iFl==9) VariableValue = 900.0 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "C22_ZSCAN_25V" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ZSCAN/" ; 
    m_NberOfFiles = 7 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root" ; }
        if (iFl==1) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_noCollim_iter0.root" ; }
        if (iFl==2) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root" ; }
        if (iFl==3) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z427_noCollim_iter0.root" ; }
        if (iFl==4) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root" ; }
        if (iFl==5) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_noCollim_iter0.root" ; }
        if (iFl==6) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root" ; }

       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Z" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue =  60.0 ;
       if (iFl==1) VariableValue = 218.5 ;
       if (iFl==2) VariableValue = 415.0 ;
       if (iFl==3) VariableValue = 427.0 ;
       if (iFl==4) VariableValue = 572.0 ;
       if (iFl==5) VariableValue = 572.0 ;
       if (iFl==6) VariableValue = 925.0 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "C22_MUP_ESCAN" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ESCAN/" ; 
    m_NberOfFiles = 3 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
        if (iFl==1) { EvtFil = "All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root" ; }
        if (iFl==2) { EvtFil = "All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }
       
       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "E" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 0.75 ;
       if (iFl==1) VariableValue = 1.00 ;
       if (iFl==2) VariableValue = 1.50 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "C22_P_ESCAN" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ESCAN/" ; 
    m_NberOfFiles = 3 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root" ; }
        if (iFl==1) { EvtFil = "All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root" ; }
        if (iFl==2) { EvtFil = "All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root" ; }
       
       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "E" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 1.00 ;
       if (iFl==1) VariableValue = 1.25 ;
       if (iFl==2) VariableValue = 1.50 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "C22_EP_ESCAN" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ESCAN/" ; 
    m_NberOfFiles = 5 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root" ; }
        if (iFl==1) { EvtFil = "All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
        if (iFl==2) { EvtFil = "All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root" ; }
        if (iFl==3) { EvtFil = "All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root" ; }
        if (iFl==4) { EvtFil = "All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }
       
       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "E" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 0.50 ;
       if (iFl==1) VariableValue = 0.75 ;
       if (iFl==2) VariableValue = 1.00 ;
       if (iFl==3) VariableValue = 1.25 ;
       if (iFl==4) VariableValue = 1.50 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "C22_PIP_ESCAN" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ESCAN/" ; 
    m_NberOfFiles = 3 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
        if (iFl==0) { EvtFil = "All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root" ; }
        if (iFl==1) { EvtFil = "All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root" ; }
        if (iFl==2) { EvtFil = "All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root" ; }
       
       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "E" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 0.75 ;
       if (iFl==1) VariableValue = 1.25 ;
       if (iFl==2) VariableValue = 1.50 ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ERAM18_HS" ){
    m_FilesDir = "OUT_Prod/CERN22_V6/ERAM18_HS/" ; 
    m_NberOfFiles = 6 ;
    for (int iFl = 0 ; iFl< m_NberOfFiles; iFl++){
       std::string EvtFil = "BIDON" ;
       if (iFl==0) { EvtFil = "ERAM18_350V_412ns_e+_0p5GeV_iter0.root" ; }
       if (iFl==1) { EvtFil = "ERAM18_350V_412ns_e+_1GeV_iter0.root" ; }
       if (iFl==2) { EvtFil = "ERAM18_350V_412ns_mu-_1GeV_iter0.root" ; }
       if (iFl==3) { EvtFil = "ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ; }
       if (iFl==4) { EvtFil = "ERAM18_350V_412ns_mu+_1GeV_iter0.root" ; }
       if (iFl==5) { EvtFil = "ERAM18_350V_412ns_p_1GeV_iter0.root" ; }
       
       m_FilesName.push_back(EvtFil) ;

       m_VariableName = "Type" ;
       
       double VariableValue = 0. ;
       if (iFl==0) VariableValue = 0. ;
       if (iFl==1) VariableValue = 1. ;
       if (iFl==2) VariableValue = 2. ;
       if (iFl==3) VariableValue = 3. ;
       if (iFl==4) VariableValue = 4. ;
       if (iFl==5) VariableValue = 5. ;
             
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ScanZZ_02T_400ns_CERN_1" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_05_CERN_1/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_412_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_412_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_412_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_412_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_412_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_412_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_412_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_412_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_412_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_412_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "ScanZZ_02T_200ns_CERN_1" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_05_CERN_1/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_200_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_200_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_200_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_200_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_200_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_200_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_200_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_200_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_200_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_200_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "Mag0_CERN_1" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_05_CERN_1/" ; 
    m_NberOfFiles = 8 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "mag_0_200_y80_iter0.root"   ;
       if (iFile==1) base_EventFile = "mag_0_200_y40_iter0.root"   ;
       if (iFile==2) base_EventFile = "mag_0_200_ym20_iter0.root"  ;
       if (iFile==3) base_EventFile = "mag_0_200_ym40_iter0.root"  ;
       if (iFile==4) base_EventFile = "mag_0_200_ym60_iter0.root"  ;
       if (iFile==5) base_EventFile = "mag_0_200_ym80_iter0.root"  ;
       if (iFile==6) base_EventFile = "mag_0_200_ym120_iter0.root" ;
       if (iFile==7) base_EventFile = "mag_0_200_ym160_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "YY" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue =  80  ;
       if (iFile==1) VariableValue =  40  ;
       if (iFile==2) VariableValue = -20  ;
       if (iFile==3) VariableValue = -40  ;
       if (iFile==4) VariableValue = -60  ;
       if (iFile==5) VariableValue = -80  ;
       if (iFile==6) VariableValue = -120 ;
       if (iFile==7) VariableValue = -160 ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ScanZZ_02T_400ns_CERN_2" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_13_CERN_2/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_412_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_412_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_412_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_412_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_412_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_412_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_412_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_412_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_412_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_412_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "ScanZZ_02T_200ns_CERN_2" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_13_CERN_2/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_200_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_200_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_200_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_200_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_200_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_200_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_200_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_200_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_200_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_200_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "Mag0_CERN_2" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_13_CERN_2/" ; 
    m_NberOfFiles = 8 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "mag_0_200_y80_iter0.root"   ;
       if (iFile==1) base_EventFile = "mag_0_200_y40_iter0.root"   ;
       if (iFile==2) base_EventFile = "mag_0_200_ym20_iter0.root"  ;
       if (iFile==3) base_EventFile = "mag_0_200_ym40_iter0.root"  ;
       if (iFile==4) base_EventFile = "mag_0_200_ym60_iter0.root"  ;
       if (iFile==5) base_EventFile = "mag_0_200_ym80_iter0.root"  ;
       if (iFile==6) base_EventFile = "mag_0_200_ym120_iter0.root" ;
       if (iFile==7) base_EventFile = "mag_0_200_ym160_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "YY" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue =  80  ;
       if (iFile==1) VariableValue =  40  ;
       if (iFile==2) VariableValue = -20  ;
       if (iFile==3) VariableValue = -40  ;
       if (iFile==4) VariableValue = -60  ;
       if (iFile==5) VariableValue = -80  ;
       if (iFile==6) VariableValue = -120 ;
       if (iFile==7) VariableValue = -160 ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ScanZZ_02T_400ns_CERN_3" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_21_CERN_3/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_412_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_412_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_412_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_412_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_412_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_412_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_412_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_412_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_412_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_412_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "ScanZZ_02T_200ns_CERN_3" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_21_CERN_3/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_200_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_200_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_200_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_200_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_200_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_200_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_200_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_200_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_200_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_200_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "Mag0_CERN_3" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_07_21_CERN_3/" ; 
    m_NberOfFiles = 8 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "mag_0_200_y80_iter0.root"   ;
       if (iFile==1) base_EventFile = "mag_0_200_y40_iter0.root"   ;
       if (iFile==2) base_EventFile = "mag_0_200_ym20_iter0.root"  ;
       if (iFile==3) base_EventFile = "mag_0_200_ym40_iter0.root"  ;
       if (iFile==4) base_EventFile = "mag_0_200_ym60_iter0.root"  ;
       if (iFile==5) base_EventFile = "mag_0_200_ym80_iter0.root"  ;
       if (iFile==6) base_EventFile = "mag_0_200_ym120_iter0.root" ;
       if (iFile==7) base_EventFile = "mag_0_200_ym160_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "YY" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue =  80  ;
       if (iFile==1) VariableValue =  40  ;
       if (iFile==2) VariableValue = -20  ;
       if (iFile==3) VariableValue = -40  ;
       if (iFile==4) VariableValue = -60  ;
       if (iFile==5) VariableValue = -80  ;
       if (iFile==6) VariableValue = -120 ;
       if (iFile==7) VariableValue = -160 ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ScanZZ_02T_400ns_CERN_4" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_08_03_CERN_4/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_412_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_412_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_412_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_412_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_412_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_412_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_412_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_412_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_412_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_412_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "ScanZZ_02T_200ns_CERN_4" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_08_03_CERN_4/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_200_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_200_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_200_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_200_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_200_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_200_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_200_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_200_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_200_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_200_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//
  if ( m_SampleSet == "Mag0_CERN_4" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_08_03_CERN_4/" ; 
    m_NberOfFiles = 8 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "mag_0_200_y80_iter0.root"   ;
       if (iFile==1) base_EventFile = "mag_0_200_y40_iter0.root"   ;
       if (iFile==2) base_EventFile = "mag_0_200_ym20_iter0.root"  ;
       if (iFile==3) base_EventFile = "mag_0_200_ym40_iter0.root"  ;
       if (iFile==4) base_EventFile = "mag_0_200_ym60_iter0.root"  ;
       if (iFile==5) base_EventFile = "mag_0_200_ym80_iter0.root"  ;
       if (iFile==6) base_EventFile = "mag_0_200_ym120_iter0.root" ;
       if (iFile==7) base_EventFile = "mag_0_200_ym160_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "YY" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue =  80  ;
       if (iFile==1) VariableValue =  40  ;
       if (iFile==2) VariableValue = -20  ;
       if (iFile==3) VariableValue = -40  ;
       if (iFile==4) VariableValue = -60  ;
       if (iFile==5) VariableValue = -80  ;
       if (iFile==6) VariableValue = -120 ;
       if (iFile==7) VariableValue = -160 ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
  if ( m_SampleSet == "ScanZZ_02T_400ns_CERN_5" ){
    m_FilesDir = "../FromCERN/OUT_JeanFrancois_Prod_22_08_14_CERN_5/" ; 
    m_NberOfFiles = 10 ;
    for (int iFile = 0 ; iFile< m_NberOfFiles; iFile++){
       std::string base_EventFile = "BIDON" ;
       if (iFile==0) base_EventFile = "z_360_275_412_02T_26_m40_iter0.root" ;
       if (iFile==1) base_EventFile = "z_360_275_412_02T_26_60_iter0.root"  ;
       if (iFile==2) base_EventFile = "z_360_275_412_02T_26_160_iter0.root" ;
       if (iFile==3) base_EventFile = "z_360_275_412_02T_26_260_iter0.root" ;
       if (iFile==4) base_EventFile = "z_360_275_412_02T_26_360_iter0.root" ;
       if (iFile==5) base_EventFile = "z_360_275_412_02T_26_460_iter0.root" ;
       if (iFile==6) base_EventFile = "z_360_275_412_02T_26_560_iter0.root" ;
       if (iFile==7) base_EventFile = "z_360_275_412_02T_26_660_iter0.root" ;
       if (iFile==8) base_EventFile = "z_360_275_412_02T_26_760_iter0.root" ;
       if (iFile==9) base_EventFile = "z_360_275_412_02T_26_860_iter0.root" ;
       
       m_FilesName.push_back(base_EventFile) ;

       m_VariableName = "ZZ" ;
       
       double VariableValue = 0. ;
       if (iFile==0) VariableValue = -40. ;
       if (iFile==1) VariableValue =  60. ;
       if (iFile==2) VariableValue = 160. ;
       if (iFile==3) VariableValue = 260. ;
       if (iFile==4) VariableValue = 360. ;
       if (iFile==5) VariableValue = 460. ;
       if (iFile==6) VariableValue = 560. ;
       if (iFile==7) VariableValue = 660. ;
       if (iFile==8) VariableValue = 760. ;
       if (iFile==9) VariableValue = 860. ;
              
       m_VariableValue.push_back(VariableValue) ;
       
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << std::setprecision(0) << VariableValue ;
       m_VariableValueAsString.push_back(aostringstream.str()) ;
       
    }
  }

//----------------------------------------------------------------------------------------------//

}

Analysis_Files::~Analysis_Files(){}
