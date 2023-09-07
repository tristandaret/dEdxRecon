#include "JeanFrancois/JeanFrancois.h"

#include "JeanFrancois/PV0.h"
#include "JeanFrancois/PV1.h"
#include "JeanFrancois/PV2.h"
#include "JeanFrancois/PV3.h"
#include "JeanFrancois/PV4.h"

#include "JeanFrancois/PV0_CERN22.h"
#include "JeanFrancois/PV1_CERN22.h"
#include "JeanFrancois/PV2_CERN22.h"
#include "JeanFrancois/PV3_CERN22.h"
#include "JeanFrancois/PV31_CERN22.h"
#include "JeanFrancois/PV4_CERN22.h"

#include "JeanFrancois/PV0_COSMICS23.h"
 
#include "JeanFrancois/JF_SelectionValidation.h"

#include "JeanFrancois/JF_SpotStrangeThings.h"

#include "JeanFrancois/JF_SpotStrangeThings.h"

#include "JeanFrancois/DIA_CERN22_01.h"

#include "JeanFrancois/DIA_DAVID_01.h"

#include "JeanFrancois/DIA_DiagNoDiag.h"

#include "JeanFrancois/DIA_RC_Gain_Dump.h"

#include "JeanFrancois/EVT_CERN22_01.h"
#include "JeanFrancois/EVT_DESY21_01.h"

void JeanFrancois()
{

//
//JF_SelectionValidation();

//
//JF_SpotStrangeThings();
  
//Procedures

//------------------------------DESY 21----------------------------/
//PV0() ;   
//-----------------------------------  PV1() ;  

//PV2() ;  

//PV3() ; 

//PV4() ; 

//------------------------------CERN 22----------------------------/
//PV0_CERN22() ;   
//PV1_CERN22() ;   
//PV2_CERN22() ;   
//PV3_CERN22() ;   
//PV31_CERN22() ;   
//PV4_CERN22() ;   

//DIA_CERN22_01() ;

//EVT_CERN22_01() ;
//EVT_DESY21_01() ;

//------------------------------DAVID----------------------------/
//DIA_DAVID_01() ;

//------------------------------DAVID----------------------------/
//PV0_COSMICS23() ; 

//------------------------------Studies diag/nodiag------------------/
  DIA_DiagNoDiag() ; 

//------------------------------RC/Gain access------------------/
//DIA_RC_Gain_Dump() ; 

}
