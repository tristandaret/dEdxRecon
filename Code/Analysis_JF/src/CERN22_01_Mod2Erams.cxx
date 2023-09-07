#include "Analysis_JF/CERN22_01.h"
#include "Misc/Util.h"

std::string CERN22_01_Mod2Erams(const int& iMod)
{
  std::string ToBeReturned = "NoSe" ;
  if (iMod==0) ToBeReturned = "ERAM07" ; 
  if (iMod==1) ToBeReturned = "ERAM01" ; 
  if (iMod==2) ToBeReturned = "ERAM23" ; 
  if (iMod==3) ToBeReturned = "ERAM02" ; 
  if (iMod==4) ToBeReturned = "ERAM16" ; 
  if (iMod==5) ToBeReturned = "ERAM15" ; 
  if (iMod==6) ToBeReturned = "ERAM10" ; 
  if (iMod==7) ToBeReturned = "ERAM12" ; 
  return  ToBeReturned ;
}
