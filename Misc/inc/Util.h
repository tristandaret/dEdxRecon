#ifndef Util_H
#define Util_H

#include "Misc.h"


#include "TGraph.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TGraphErrors.h"

int MakeMyDir(const std::string& DirName);

int ListDir(std::string DirName, std::vector < std::string >& VectorOfStrings);

TGraphErrors* Convert_TH2_TGE     (const TH2* pTH2) ;
TGraphErrors* Convert_TH2_TGE_v2  (const TH2* pTH2) ; // several values for a given x

void Free_trim(std::string& s) ;

struct RankedValue{
  double Value ;
  int    Rank  ;
 
  bool operator<(const RankedValue& rhs) const
  {
    return Value < rhs.Value ;
  }

};

// Set graphic settings of a TGraphErrors
void Graphic_setup( TGraphErrors* ptge, 
                    Size_t markersize, 
                    Style_t markerstyle, 
                    Color_t markercolor, 
                    Width_t linewidth, 
                    Color_t linecolor) ;

// Set graphic settings of an histogram
void Graphic_setup(TH1* th1, Size_t markersize, Style_t markerstyle, Color_t markercolor, Width_t linewidth, Color_t linecolor) ;

// Set graphic settings of an TH2
void Graphic_setup(TH2* th2, Size_t markersize, Style_t markerstyle, Color_t markercolor) ;

// Set the stats box position
void SetStatBoxPosition(TH1* pTH, const double& xmin, const double& xmax, const double& ymin, const double& ymax) ;

#endif
