#ifndef Util_H
#define Util_H

#include "Misc.h"

#include "TH2.h"
#include "TGraphErrors.h"

int MakeMyDir(const std::string& DirName);

TGraphErrors* Convert_TH2_TGE     (const TH2* pTH2) ;

struct RankedValue{
  double Value ;
  int    Rank  ;
 
  bool operator<(const RankedValue& rhs) const
  {
    return Value < rhs.Value ;
  }

};

#endif
