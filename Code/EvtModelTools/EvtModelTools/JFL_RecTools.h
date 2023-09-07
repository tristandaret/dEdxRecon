#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"

//
// StartFrom:
// =0 set YT from YWeight
// =1 set YT from YLeading
// =2 set YT from YLeading
//
void  Set_Starting_YT_Sample (Sample& aSample, const int& ModuleNber, const int& StartFrom) ;
void  Set_Starting_YT_Event  (Event*  pEvent , const int& ModuleNber, const int& StartFrom) ;
