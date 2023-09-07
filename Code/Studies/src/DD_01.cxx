#include "Studies/DD_01.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ReadOutGeometry.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TRandom3.h"

void DD_01()
{
  DD_01_Base(60.00, 40.);
  DD_01_Base(60.00, 60.);
  DD_01_Base(60.00,160.);
  DD_01_Base(60.00,260.);
  DD_01_Base(60.00,460.);
  DD_01_Base(60.00,560.);
  DD_01_Base(60.00,660.);
  DD_01_Base(60.00,760.);
  DD_01_Base(60.00,860.);

}

void DD_01_Base(const double& RCin, const double&  DriftDistanceinmm)
{
  Model_ReadOutGeometry aModel_ReadOutGeometry ;
  Model_Electronics     aModel_Electronics     ;
  Model_Charge1D        aModel_Charge1D        ;

  double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
  aModel_Charge1D.Set_RC(LocalRC) ;
  
  aModel_Charge1D.Set_WidthFromDriftDistance(DriftDistanceinmm/10.) ;
  double TheWidth = aModel_Charge1D.Get_Width() ;
  
  std::cout 
     << " RCin "            << RCin
     << " DriftDistancein " << DriftDistanceinmm
     << " TheWidth "        << TheWidth*1.E3
     << std::endl ;
  
}
