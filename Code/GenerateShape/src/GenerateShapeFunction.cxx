#include "GenerateShape/GenerateShapeFunction.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"

#include "GenerateShape/GenerateShapeFunctionTool.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_Charge1D.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"

void GenerateShapeFunction()
{

//--------------------------------------------------------//
//GenerateShapeFunction_Short(30.00,40.);
//GenerateShapeFunction_Short(40.00,40.);
//GenerateShapeFunction_Short(50.00,40.);
//GenerateShapeFunction_Short(60.00,40.);
//GenerateShapeFunction_Short(70.00,40.);
//GenerateShapeFunction_Short(80.00,40.);
//GenerateShapeFunction_Short(90.00,40.);
//GenerateShapeFunction_Short(100.00,40.);
//GenerateShapeFunction_Short(110.00,40.);
//GenerateShapeFunction_Short(120.00,40.);
//GenerateShapeFunction_Short(130.00,40.);
//GenerateShapeFunction_Short(140.00,40.);
//
//GenerateShapeFunction_Short( 35.00,40.);
//GenerateShapeFunction_Short( 45.00,40.);
//GenerateShapeFunction_Short( 55.00,40.);
//GenerateShapeFunction_Short( 65.00,40.);
//GenerateShapeFunction_Short( 75.00,40.);
//GenerateShapeFunction_Short( 85.00,40.);
//GenerateShapeFunction_Short( 95.00,40.);
//GenerateShapeFunction_Short(105.00,40.);
//GenerateShapeFunction_Short(115.00,40.);
//GenerateShapeFunction_Short(125.00,40.);
//GenerateShapeFunction_Short(135.00,40.);
 
//--------------------------------------------------------//
//GenerateShapeFunction_Short(30.00,60.);
//GenerateShapeFunction_Short(40.00,60.);
//GenerateShapeFunction_Short(50.00,60.);
//GenerateShapeFunction_Short(60.00,60.);
//GenerateShapeFunction_Short(70.00,60.);
//GenerateShapeFunction_Short(80.00,60.);
//GenerateShapeFunction_Short(90.00,60.);
//GenerateShapeFunction_Short(100.00,60.);
//GenerateShapeFunction_Short(110.00,60.);
//GenerateShapeFunction_Short(120.00,60.);
//GenerateShapeFunction_Short(130.00,60.);
//GenerateShapeFunction_Short(140.00,60.);

//GenerateShapeFunction_Short( 35.00,60.);
//GenerateShapeFunction_Short( 45.00,60.);
//GenerateShapeFunction_Short( 55.00,60.);
//GenerateShapeFunction_Short( 65.00,60.);
//GenerateShapeFunction_Short( 75.00,60.);
//GenerateShapeFunction_Short( 85.00,60.);
//GenerateShapeFunction_Short( 95.00,60.);
//GenerateShapeFunction_Short(105.00,60.);
//GenerateShapeFunction_Short(115.00,60.);
//GenerateShapeFunction_Short(125.00,60.);
//GenerateShapeFunction_Short(135.00,60.);
 
//--------------------------------------------------------//
//GenerateShapeFunction_Short(30.00,75);
//GenerateShapeFunction_Short(40.00,75);
//GenerateShapeFunction_Short(50.00,75);
//GenerateShapeFunction_Short(60.00,75);
//GenerateShapeFunction_Short(70.00,75);
//GenerateShapeFunction_Short(80.00,75);
//GenerateShapeFunction_Short(90.00,75);
//GenerateShapeFunction_Short(100.00,75);
//GenerateShapeFunction_Short(110.00,75);
//GenerateShapeFunction_Short(120.00,75);
//GenerateShapeFunction_Short(130.00,75);
//GenerateShapeFunction_Short(140.00,75);

//GenerateShapeFunction_Short( 35.00,75);
//GenerateShapeFunction_Short( 45.00,75);
//GenerateShapeFunction_Short( 55.00,75);
//GenerateShapeFunction_Short( 65.00,75);
//GenerateShapeFunction_Short( 75.00,75);
//GenerateShapeFunction_Short( 85.00,75);
//GenerateShapeFunction_Short( 95.00,75);
//GenerateShapeFunction_Short(105.00,75);
//GenerateShapeFunction_Short(115.00,75);
//GenerateShapeFunction_Short(125.00,75);
//GenerateShapeFunction_Short(135.00,75);

//--------------------------------------------------------//
//GenerateShapeFunction_Short(30.00,160.);
//GenerateShapeFunction_Short(40.00,160.);
//GenerateShapeFunction_Short(50.00,160.);
//GenerateShapeFunction_Short(60.00,160.);
//GenerateShapeFunction_Short(70.00,160.);
//GenerateShapeFunction_Short(80.00,160.);
//GenerateShapeFunction_Short(90.00,160.);
//GenerateShapeFunction_Short(100.00,160.);
//GenerateShapeFunction_Short(110.00,160.);
//GenerateShapeFunction_Short(120.00,160.);
//GenerateShapeFunction_Short(130.00,160.);
//GenerateShapeFunction_Short(140.00,160.);

//GenerateShapeFunction_Short( 35.00,160.);
//GenerateShapeFunction_Short( 45.00,160.);
//GenerateShapeFunction_Short( 55.00,160.);
//GenerateShapeFunction_Short( 65.00,160.);
//GenerateShapeFunction_Short( 75.00,160.);
//GenerateShapeFunction_Short( 85.00,160.);
//GenerateShapeFunction_Short( 95.00,160.);
//GenerateShapeFunction_Short(105.00,160.);
//GenerateShapeFunction_Short(115.00,160.);
//GenerateShapeFunction_Short(125.00,160.);
//GenerateShapeFunction_Short(135.00,160.);
 
//--------------------------------------------------------//
// GenerateShapeFunction_Short(30.00,260.);
// GenerateShapeFunction_Short(40.00,260.);
// GenerateShapeFunction_Short(50.00,260.);
// GenerateShapeFunction_Short(60.00,260.);
// GenerateShapeFunction_Short(70.00,260.);
// GenerateShapeFunction_Short(80.00,260.);
// GenerateShapeFunction_Short(90.00,260.);
// GenerateShapeFunction_Short(100.00,260.);
// GenerateShapeFunction_Short(110.00,260.);
// GenerateShapeFunction_Short(120.00,260.);
// GenerateShapeFunction_Short(130.00,260.);
// GenerateShapeFunction_Short(140.00,260.);
// 
// GenerateShapeFunction_Short( 35.00,260.);
// GenerateShapeFunction_Short( 45.00,260.);
// GenerateShapeFunction_Short( 55.00,260.);
// GenerateShapeFunction_Short( 65.00,260.);
// GenerateShapeFunction_Short( 75.00,260.);
// GenerateShapeFunction_Short( 85.00,260.);
// GenerateShapeFunction_Short( 95.00,260.);
// GenerateShapeFunction_Short(105.00,260.);
// GenerateShapeFunction_Short(115.00,260.);
// GenerateShapeFunction_Short(125.00,260.);
// GenerateShapeFunction_Short(135.00,260.);

//--------------------------------------------------------//
// GenerateShapeFunction_Short(30.00,360.);
// GenerateShapeFunction_Short(40.00,360.);
// GenerateShapeFunction_Short(50.00,360.);
// GenerateShapeFunction_Short(60.00,360.);
// GenerateShapeFunction_Short(70.00,360.);
// GenerateShapeFunction_Short(80.00,360.);
// GenerateShapeFunction_Short(90.00,360.);
// GenerateShapeFunction_Short(100.00,360.);
// GenerateShapeFunction_Short(110.00,360.);
// GenerateShapeFunction_Short(120.00,360.);
// GenerateShapeFunction_Short(130.00,360.);
// GenerateShapeFunction_Short(140.00,360.);
// 
// GenerateShapeFunction_Short( 35.00,360.);
// GenerateShapeFunction_Short( 45.00,360.);
// GenerateShapeFunction_Short( 55.00,360.);
// GenerateShapeFunction_Short( 65.00,360.);
// GenerateShapeFunction_Short( 75.00,360.);
// GenerateShapeFunction_Short( 85.00,360.);
// GenerateShapeFunction_Short( 95.00,360.);
// GenerateShapeFunction_Short(105.00,360.);
// GenerateShapeFunction_Short(115.00,360.);
// GenerateShapeFunction_Short(125.00,360.);
// GenerateShapeFunction_Short(135.00,360.);

//--------------------------------------------------------//
// GenerateShapeFunction_Short(30.00,460.);
// GenerateShapeFunction_Short(40.00,460.);
// GenerateShapeFunction_Short(50.00,460.);
// GenerateShapeFunction_Short(60.00,460.);
// GenerateShapeFunction_Short(70.00,460.);
// GenerateShapeFunction_Short(80.00,460.);
// GenerateShapeFunction_Short(90.00,460.);
// GenerateShapeFunction_Short(100.00,460.);
// GenerateShapeFunction_Short(110.00,460.);
// GenerateShapeFunction_Short(120.00,460.);
// GenerateShapeFunction_Short(130.00,460.);
// GenerateShapeFunction_Short(140.00,460.);
// 
// GenerateShapeFunction_Short( 35.00,460.);
// GenerateShapeFunction_Short( 45.00,460.);
// GenerateShapeFunction_Short( 55.00,460.);
// GenerateShapeFunction_Short( 65.00,460.);
// GenerateShapeFunction_Short( 75.00,460.);
// GenerateShapeFunction_Short( 85.00,460.);
// GenerateShapeFunction_Short( 95.00,460.);
// GenerateShapeFunction_Short(105.00,460.);
// GenerateShapeFunction_Short(115.00,460.);
// GenerateShapeFunction_Short(125.00,460.);
// GenerateShapeFunction_Short(135.00,460.);

//--------------------------------------------------------//
// GenerateShapeFunction_Short(30.00,560.);
// GenerateShapeFunction_Short(40.00,560.);
// GenerateShapeFunction_Short(50.00,560.);
// GenerateShapeFunction_Short(60.00,560.);
// GenerateShapeFunction_Short(70.00,560.);
// GenerateShapeFunction_Short(80.00,560.);
// GenerateShapeFunction_Short(90.00,560.);
// GenerateShapeFunction_Short(100.00,560.);
// GenerateShapeFunction_Short(110.00,560.);
// GenerateShapeFunction_Short(120.00,560.);
// GenerateShapeFunction_Short(130.00,560.);
// GenerateShapeFunction_Short(140.00,560.);
// 
// GenerateShapeFunction_Short( 35.00,560.);
// GenerateShapeFunction_Short( 45.00,560.);
// GenerateShapeFunction_Short( 55.00,560.);
// GenerateShapeFunction_Short( 65.00,560.);
// GenerateShapeFunction_Short( 75.00,560.);
// GenerateShapeFunction_Short( 85.00,560.);
// GenerateShapeFunction_Short( 95.00,560.);
// GenerateShapeFunction_Short(105.00,560.);
// GenerateShapeFunction_Short(115.00,560.);
// GenerateShapeFunction_Short(125.00,560.);
// GenerateShapeFunction_Short(135.00,560.);

//--------------------------------------------------------//
GenerateShapeFunction_Short(30.00,660.);
GenerateShapeFunction_Short(40.00,660.);
GenerateShapeFunction_Short(50.00,660.);
GenerateShapeFunction_Short(60.00,660.);
GenerateShapeFunction_Short(70.00,660.);
GenerateShapeFunction_Short(80.00,660.);
GenerateShapeFunction_Short(90.00,660.);
GenerateShapeFunction_Short(100.00,660.);
GenerateShapeFunction_Short(110.00,660.);
GenerateShapeFunction_Short(120.00,660.);
GenerateShapeFunction_Short(130.00,660.);
GenerateShapeFunction_Short(140.00,660.);

GenerateShapeFunction_Short( 35.00,660.);
GenerateShapeFunction_Short( 45.00,660.);
GenerateShapeFunction_Short( 55.00,660.);
GenerateShapeFunction_Short( 65.00,660.);
GenerateShapeFunction_Short( 75.00,660.);
GenerateShapeFunction_Short( 85.00,660.);
GenerateShapeFunction_Short( 95.00,660.);
GenerateShapeFunction_Short(105.00,660.);
GenerateShapeFunction_Short(115.00,660.);
GenerateShapeFunction_Short(125.00,660.);
GenerateShapeFunction_Short(135.00,660.);

//--------------------------------------------------------//
GenerateShapeFunction_Short(30.00,760.);
GenerateShapeFunction_Short(40.00,760.);
GenerateShapeFunction_Short(50.00,760.);
GenerateShapeFunction_Short(60.00,760.);
GenerateShapeFunction_Short(70.00,760.);
GenerateShapeFunction_Short(80.00,760.);
GenerateShapeFunction_Short(90.00,760.);
GenerateShapeFunction_Short(100.00,760.);
GenerateShapeFunction_Short(110.00,760.);
GenerateShapeFunction_Short(120.00,760.);
GenerateShapeFunction_Short(130.00,760.);
GenerateShapeFunction_Short(140.00,760.);

GenerateShapeFunction_Short( 35.00,760.);
GenerateShapeFunction_Short( 45.00,760.);
GenerateShapeFunction_Short( 55.00,760.);
GenerateShapeFunction_Short( 65.00,760.);
GenerateShapeFunction_Short( 75.00,760.);
GenerateShapeFunction_Short( 85.00,760.);
GenerateShapeFunction_Short( 95.00,760.);
GenerateShapeFunction_Short(105.00,760.);
GenerateShapeFunction_Short(115.00,760.);
GenerateShapeFunction_Short(125.00,760.);
GenerateShapeFunction_Short(135.00,760.);

//--------------------------------------------------------//
GenerateShapeFunction_Short(30.00,860.);
GenerateShapeFunction_Short(40.00,860.);
GenerateShapeFunction_Short(50.00,860.);
GenerateShapeFunction_Short(60.00,860.);
GenerateShapeFunction_Short(70.00,860.);
GenerateShapeFunction_Short(80.00,860.);
GenerateShapeFunction_Short(90.00,860.);
GenerateShapeFunction_Short(100.00,860.);
GenerateShapeFunction_Short(110.00,860.);
GenerateShapeFunction_Short(120.00,860.);
GenerateShapeFunction_Short(130.00,860.);
GenerateShapeFunction_Short(140.00,860.);

GenerateShapeFunction_Short( 35.00,860.);
GenerateShapeFunction_Short( 45.00,860.);
GenerateShapeFunction_Short( 55.00,860.);
GenerateShapeFunction_Short( 65.00,860.);
GenerateShapeFunction_Short( 75.00,860.);
GenerateShapeFunction_Short( 85.00,860.);
GenerateShapeFunction_Short( 95.00,860.);
GenerateShapeFunction_Short(105.00,860.);
GenerateShapeFunction_Short(115.00,860.);
GenerateShapeFunction_Short(125.00,860.);
GenerateShapeFunction_Short(135.00,860.);



}

void GenerateShapeFunction_Short(const double& RCin, const double&  DriftDistanceinmm)
{
  GenerateShapeFunction_Standard(1, RCin,DriftDistanceinmm) ;
  
}

void GenerateShapeFunction_Standard(const int& iOptWS, const double& RCin, const double&  DriftDistanceinmm)
{
//
  if ( iOptWS!=1 && iOptWS!=2 ){
    std::cout << "GenerateShapeFunction_Standard: iOptWS!=1 && iOptWS!=2 " << std::endl ;
    std::cout << "iOptWS= " << iOptWS << std::endl ;
    abort();
  }

//
  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  if (iOptWS==1)
  aostringstream << "_400ns"  ;
  if (iOptWS==2)
  aostringstream << "_200ns"  ;
  aostringstream << "_RC" << std::setprecision(2) <<  RCin ;
  aostringstream << "_DD" << std::setprecision(2) <<  DriftDistanceinmm ;
  std::string TAG = aostringstream.str() ;  

  std::cout << " iOptWS              " << iOptWS            << std::endl ;
  std::cout << " RCin                " << RCin              << std::endl ;
  std::cout << " DriftDistanceinmm " << DriftDistanceinmm << std::endl ;
  std::cout << " =>TAG               " << TAG               << std::endl ;

  GenerateShapeFunction_Base(RCin,DriftDistanceinmm,iOptWS,TAG);
  
}

void GenerateShapeFunction_Base(const double& RCin, const double&  DriftDistanceinmm,const int& iOptWS, const std::string& TAG)
{
//
  if ( iOptWS!=1 && iOptWS!=2 ){
    std::cout << "GenerateShapeFunction_Base: iOptWS!=1 && iOptWS!=2 " << std::endl ;
    std::cout << "iOptWS= " << iOptWS << std::endl ;
    abort();
  }

//
  Model_ReadOutGeometry aModel_ReadOutGeometry ;
  Model_Electronics     aModel_Electronics     ;
  Model_Charge1D        aModel_Charge1D        ;
  
  if (iOptWS==2){
    double TheQValue = aModel_Electronics.Get_QValue() ;
    double TheWS     = aModel_Electronics.Get_ws()     ;
    TheWS = TheWS * 2. ;
    aModel_Electronics.Set_TimeShape(TheQValue,TheWS);
  }

  double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
  aModel_Charge1D.Set_RC(LocalRC) ;
  
  aModel_Charge1D.Set_WidthFromDriftDistance(DriftDistanceinmm/10.) ;

//
  double LX= aModel_ReadOutGeometry.Get_LX() ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;
  
  double Time0  = 0. ;

  double Vert_Ymin = 0. ;
  double Vert_Ymax = 0. ;

//
  double YTrue_min =  0. ;
  double YTrue_max =  0.5*LY ;
  int    iYTrue_Max= 100 ;
         iYTrue_Max = int( 0.5*LY*1.E6 ) + 1 ;
         std::cout << " iYTrue_Max " << iYTrue_Max << std::endl ;
//       iYTrue_Max= 5 ;
  
//
  int iPadUpDow_Max   = 1 ;
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  
  std::vector < Pad* > V_Pad ;
  std::vector < TGraph* > V_pTGraph_SPAD   ;
  std::vector < TGraph* > V_pTGraph_nSPAD   ;
  std::vector < TGraph* > V_pTGraph_TPAD   ;
  std::vector < TGraph* > V_pTGraph_T0PAD  ;
  std::vector < TGraph* > V_pTGraph_S0PAD   ;
  for (int iPadUp = 0 ; iPadUp< iPadUpDow_Max; iPadUp++){
    int iPadUpLocal = iPadUpDow_Max-1-iPadUp ;
    std::string PadNameUp ="" ;
    for (int iTem = 0 ; iTem< (iPadUpLocal+1); iTem++){
      PadNameUp +="Up" ;
    }
    Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , PadNameUp , EventNber,EntryNber,ModuleNber , 0 , 16 + iPadUpLocal + 1 , 0. , LX , (double(iPadUpLocal)+0.5)*LY , (double(iPadUpLocal)+1.5)*LY );
    V_Pad.push_back(pPad) ;
    TGraph* pTGraph_SPAD  = new TGraph() ; pTGraph_SPAD ->SetLineColor(2)  ; V_pTGraph_SPAD  .push_back(pTGraph_SPAD  ) ;
    TGraph* pTGraph_nSPAD = new TGraph() ; pTGraph_nSPAD->SetLineColor(2)  ; V_pTGraph_nSPAD .push_back(pTGraph_nSPAD ) ;
    TGraph* pTGraph_TPAD  = new TGraph() ; pTGraph_TPAD ->SetLineColor(2)  ; V_pTGraph_TPAD  .push_back(pTGraph_TPAD  ) ;
    TGraph* pTGraph_T0PAD = new TGraph() ; pTGraph_T0PAD->SetLineColor(2)  ; V_pTGraph_T0PAD .push_back(pTGraph_T0PAD ) ;
    TGraph* pTGraph_S0PAD = new TGraph() ; pTGraph_S0PAD->SetLineColor(2)  ; V_pTGraph_S0PAD .push_back(pTGraph_S0PAD ) ;
  }
  
  Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "Central " , EventNber,EntryNber,ModuleNber , 0 , 16 ,  0. , LX , -0.5*LY ,  0.5*LY );
  V_Pad.push_back(pPad) ;
  TGraph* pTGraph_SPAD  = new TGraph() ; pTGraph_SPAD  ->SetLineColor(1) ; V_pTGraph_SPAD  .push_back(pTGraph_SPAD  ) ;
  TGraph* pTGraph_nSPAD = new TGraph() ; pTGraph_nSPAD ->SetLineColor(1) ; V_pTGraph_nSPAD .push_back(pTGraph_nSPAD ) ;
  TGraph* pTGraph_TPAD  = new TGraph() ; pTGraph_TPAD  ->SetLineColor(1) ; V_pTGraph_TPAD  .push_back(pTGraph_TPAD  ) ;
  TGraph* pTGraph_T0PAD = new TGraph() ; pTGraph_T0PAD ->SetLineColor(1) ; V_pTGraph_T0PAD .push_back(pTGraph_T0PAD ) ;
  TGraph* pTGraph_S0PAD = new TGraph() ; pTGraph_S0PAD ->SetLineColor(1) ; V_pTGraph_S0PAD .push_back(pTGraph_S0PAD ) ;

  for (int iPadDown = 0 ; iPadDown< iPadUpDow_Max; iPadDown++){
    int iPadDownLocal = iPadDown ;
    std::string PadNameDown ="" ;
    for (int iTem = 0 ; iTem< (iPadDownLocal+1); iTem++){
      PadNameDown +="Down" ;
    }
    Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D, PadNameDown , EventNber,EntryNber,ModuleNber , 0 , 16 - iPadDown - 1 ,  0. , LX , (-double(iPadDownLocal)-1.5)*LY , (-double(iPadDownLocal)-0.5)*LY );
    V_Pad.push_back(pPad) ;
    TGraph* pTGraph_SPAD  = new TGraph() ; pTGraph_SPAD ->SetLineColor(4) ; V_pTGraph_SPAD .push_back(pTGraph_SPAD ) ;
    TGraph* pTGraph_nSPAD = new TGraph() ; pTGraph_nSPAD->SetLineColor(4) ; V_pTGraph_nSPAD.push_back(pTGraph_nSPAD) ;
    TGraph* pTGraph_TPAD  = new TGraph() ; pTGraph_TPAD ->SetLineColor(4) ; V_pTGraph_TPAD .push_back(pTGraph_TPAD ) ;
    TGraph* pTGraph_T0PAD = new TGraph() ; pTGraph_T0PAD->SetLineColor(4) ; V_pTGraph_T0PAD.push_back(pTGraph_T0PAD) ;
    TGraph* pTGraph_S0PAD = new TGraph() ; pTGraph_S0PAD->SetLineColor(4) ; V_pTGraph_S0PAD.push_back(pTGraph_S0PAD) ;
  }

  int iPad_Max = V_Pad.size() ;

  GenerateShapeFunctionTool aGenerateShapeFunctionTool(V_Pad,YTrue_min,YTrue_max) ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF = "BIDON" ; 

//
  double Tmin =     0. * 1E-9 ;
  double Tmax = 20000. * 1E-9 ; 
  int    Npoints = 100000 ;

  
  TMultiGraph* pTMultiGraph_SPAD = new TMultiGraph();

  TLegend* pTLegend_SPAD = new TLegend(0.65,0.65 ,0.85,0.80  ); 
  pTLegend_SPAD->SetFillStyle(1001);
  pTLegend_SPAD->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_nSPAD = new TMultiGraph();

  TLegend* pTLegend_nSPAD = new TLegend(0.65,0.65 ,0.85,0.80  ); 
  pTLegend_nSPAD->SetFillStyle(1001);
  pTLegend_nSPAD->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_TPAD = new TMultiGraph();

  TLegend* pTLegend_TPAD = new TLegend(0.45,0.65 ,0.65,0.80  ); 
  pTLegend_TPAD->SetFillStyle(1001);
  pTLegend_TPAD->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_T0PAD = new TMultiGraph();

  TLegend* pTLegend_T0PAD = new TLegend(0.45,0.65 ,0.65,0.80  ); 
  pTLegend_T0PAD->SetFillStyle(1001);
  pTLegend_T0PAD->SetBorderSize(1);

  for (int iYTrue = 0 ; iYTrue< iYTrue_Max; iYTrue++){
    double XTrue = 0. ;
    double YTrue = YTrue_min + double(iYTrue)*(YTrue_max -YTrue_min)/double(iYTrue_Max-1) ;

    std::vector < double > VS_PAD  ;
    std::vector < double > VT_PAD ;
    
    double S_Cluster = 0. ;
    double T_Cluster = 0. ;
    double Max_S_Pad_max = 0. ;
    for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      Pad* pPad = V_Pad[iPad] ;
      pPad->SetSignalModel( Time0 , XTrue, YTrue );

      double S_Pad_max = 0. ;
      double T_Pad_max = 0. ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
        
        if ( TimeCur <  aGenerateShapeFunctionTool.TimeEstimate( (YTrue - pPad->Get_YPad())*1.E2  ) *1.E-6 ) continue ;
                
        double S_Pad = pPad->Get_APad(TimeCur) ;
        if (iPoint==0){
          S_Pad_max = S_Pad   ;
          T_Pad_max = TimeCur ;
        }else{
          if (S_Pad_max<=S_Pad) {
            S_Pad_max = S_Pad ;
            T_Pad_max = TimeCur ;
          }else{
            break ;
          }
        }
      }

      if (iPad==0){
        Max_S_Pad_max = S_Pad_max ;
      }
      if (Max_S_Pad_max<=S_Pad_max) Max_S_Pad_max = S_Pad_max ;
      
      VS_PAD .push_back( S_Pad_max ) ;
      VT_PAD .push_back( T_Pad_max ) ;
      S_Cluster += S_Pad_max ;
      if (iPad==0) T_Cluster = T_Pad_max ;
      if (T_Cluster>T_Pad_max) T_Cluster = T_Pad_max ;
      
    }

    for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      Pad* pPad = V_Pad[iPad] ;
      
      TGraph* pTGraph_SPAD  = V_pTGraph_SPAD[iPad] ;
      pTGraph_SPAD ->SetPoint( pTGraph_SPAD ->GetN(), (YTrue - pPad->Get_YPad())*1.E2 , VS_PAD[iPad]/S_Cluster  ) ;
      
      TGraph* pTGraph_nSPAD  = V_pTGraph_nSPAD[iPad] ;
      pTGraph_nSPAD ->SetPoint( pTGraph_nSPAD ->GetN(), (YTrue - pPad->Get_YPad())*1.E2 , VS_PAD[iPad]/Max_S_Pad_max  ) ;
      
      TGraph* pTGraph_TPAD  = V_pTGraph_TPAD[iPad] ;
      pTGraph_TPAD ->SetPoint( pTGraph_TPAD ->GetN(), (YTrue - pPad->Get_YPad())*1.E2 , (VT_PAD[iPad]-T_Cluster)*1.E6  ) ;
      
      TGraph* pTGraph_T0PAD = V_pTGraph_T0PAD[iPad] ;
      pTGraph_T0PAD->SetPoint( pTGraph_T0PAD->GetN(), (YTrue - pPad->Get_YPad())*100. , (VT_PAD[iPad]-Time0)*1.E6  ) ;
 
      TGraph* pTGraph_S0PAD = V_pTGraph_S0PAD[iPad] ;
      pTGraph_S0PAD->SetPoint( pTGraph_S0PAD->GetN(), (YTrue - pPad->Get_YPad())*1.E2 , VS_PAD[iPad] ) ;
      
    }

  }

//
  
  for (int iPad = 0 ; iPad< iPad_Max; iPad++){
    Pad* pPad = V_Pad[iPad] ;
    
    TGraph* pTGraph_SPAD = V_pTGraph_SPAD[iPad] ;
    pTMultiGraph_SPAD->Add( pTGraph_SPAD  ,"l");
    pTLegend_SPAD->AddEntry( pTGraph_SPAD       ,(pPad->Get_PadName()).c_str()  , "l");

    TGraph* pTGraph_nSPAD = V_pTGraph_nSPAD[iPad] ;
    pTMultiGraph_nSPAD->Add( pTGraph_nSPAD  ,"l");
    pTLegend_nSPAD->AddEntry( pTGraph_nSPAD       ,(pPad->Get_PadName()).c_str()  , "l");

    TGraph* pTGraph_TPAD = V_pTGraph_TPAD[iPad] ;
    pTMultiGraph_TPAD->Add( pTGraph_TPAD  ,"l");
    pTLegend_TPAD->AddEntry( pTGraph_TPAD       ,(pPad->Get_PadName()).c_str()  , "l");

    TGraph* pTGraph_T0PAD = V_pTGraph_T0PAD[iPad] ;
    pTMultiGraph_T0PAD->Add( pTGraph_T0PAD  ,"l");
    pTLegend_T0PAD->AddEntry( pTGraph_TPAD       ,(pPad->Get_PadName()).c_str()  , "l");

  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    pTMultiGraph_SPAD->SetTitle("S_{Pad}/S_{Cluster} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_SPAD->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_SPAD->GetYaxis()->SetTitle("S_{Pad}/S_{Cluster}");  

    pTMultiGraph_SPAD->SetMinimum(0.);
    pTMultiGraph_SPAD->SetMaximum(1.);

    pTMultiGraph_SPAD->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 0 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }

    pTLegend_SPAD->Draw();       
    pTCanvas->Update();

  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_01" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    pTMultiGraph_nSPAD->SetTitle("S_{Pad}/S_{Max} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_nSPAD->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_nSPAD->GetYaxis()->SetTitle("S_{Pad}/S_{Max}");  

    pTMultiGraph_nSPAD->SetMinimum(0.);
//  pTMultiGraph_nSPAD->SetMaximum(1.);

    pTMultiGraph_nSPAD->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 0 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }

    pTLegend_nSPAD->Draw();       
    pTCanvas->Update();

  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_02" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    pTMultiGraph_TPAD->SetTitle("T_{Pad}-T_{Earlier Pad} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_TPAD->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_TPAD->GetYaxis()->SetTitle("T_{Pad}-T_{Earlier Pad} (#mu s)");  
    pTMultiGraph_TPAD->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_TPAD->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 0 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }

    pTLegend_TPAD->Draw();       
    pTCanvas->Update();

  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_03" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    pTMultiGraph_T0PAD->SetTitle("T_{Pad}-T_{0} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_T0PAD->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_T0PAD->GetYaxis()->SetTitle("T_{Pad}-T_{0} (#mu s)");  
    pTMultiGraph_T0PAD->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_T0PAD->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 0 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }
 
    pTLegend_T0PAD->Draw();       
    pTCanvas->Update();

  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_04" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  std::vector < double > V_X     ;
  std::vector < double > V_SPAD  ;
  std::vector < double > V_nSPAD  ;
  std::vector < double > V_TPAD  ;
  std::vector < double > V_T0PAD ;
  std::vector < double > V_S0PAD ;

  for (int iPad = 0 ; iPad< iPad_Max; iPad++){
    TGraph* pTGraph_SPAD  = V_pTGraph_SPAD [iPad] ;
    TGraph* pTGraph_nSPAD  = V_pTGraph_nSPAD [iPad] ;
    TGraph* pTGraph_TPAD  = V_pTGraph_TPAD [iPad] ;
    TGraph* pTGraph_T0PAD = V_pTGraph_T0PAD[iPad] ;
    TGraph* pTGraph_S0PAD = V_pTGraph_S0PAD[iPad] ;
    
    int iTem_First = 0                   ;
    int iTem_Last  = pTGraph_SPAD->GetN();
    if ( (iPad-iPadUpDow_Max) < 0 ){
//  if ( iPad == 0 ){
      iTem_First = 1            ;
      iTem_Last  = iTem_Last -1 ;
    } 
    double* X_SPAD  = pTGraph_SPAD->GetX ();
    
    double* Y_SPAD  = pTGraph_SPAD ->GetY ();
    double* Y_nSPAD = pTGraph_nSPAD->GetY ();
    double* Y_TPAD  = pTGraph_TPAD ->GetY ();
    double* Y_T0PAD = pTGraph_T0PAD->GetY ();
    double* Y_S0PAD = pTGraph_S0PAD->GetY ();
    
    for (int iTem = iTem_First ; iTem< iTem_Last; iTem++){
      V_X    .push_back( std::fabs( X_SPAD  [iTem] ) ) ;
      V_SPAD .push_back(            Y_SPAD  [iTem]   ) ;
      V_nSPAD.push_back(            Y_nSPAD [iTem]   ) ;
      V_TPAD .push_back(            Y_TPAD  [iTem]   ) ;
      V_T0PAD.push_back(            Y_T0PAD [iTem]   ) ;
      V_S0PAD.push_back(            Y_S0PAD [iTem]   ) ;
//       std::cout 
//         <<           std::setw(15) << std::setprecision(9) <<  std::fabs( X_SPAD  [iTem] ) /1.E2
//         << " ; "  << std::setw(15) << std::setprecision(9) <<             Y_SPAD  [iTem]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<             Y_nSPAD  [iTem]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<             Y_TPAD  [iTem]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<             Y_T0PAD [iTem]   
//         << std::endl ;
    }
    
  }

  std::vector < RankedValue > V_RankedValue ;
  int iTem_Max = V_X.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    RankedValue aRankedValue ; 
    aRankedValue.Value = V_X[iTem] ; 
    aRankedValue.Rank  = iTem       ; 
    V_RankedValue.push_back( aRankedValue );
  }

  std::sort(V_RankedValue.begin(), V_RankedValue.end());

  TGraph* pTGraph_SPAD_All  = new TGraph() ; 
  TGraph* pTGraph_nSPAD_All = new TGraph() ; 
  TGraph* pTGraph_TPAD_All  = new TGraph() ; 
  TGraph* pTGraph_T0PAD_All = new TGraph() ; 
  TGraph* pTGraph_S0PAD_All = new TGraph() ; 

  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    int TheRank = V_RankedValue[iTem].Rank ;
    pTGraph_SPAD_All ->SetPoint( pTGraph_SPAD_All ->GetN(), V_X[TheRank] , V_SPAD [TheRank]  ) ;
    pTGraph_nSPAD_All->SetPoint( pTGraph_nSPAD_All->GetN(), V_X[TheRank] , V_nSPAD[TheRank]  ) ;
    pTGraph_TPAD_All ->SetPoint( pTGraph_TPAD_All ->GetN(), V_X[TheRank] , V_TPAD [TheRank]  ) ;
    pTGraph_T0PAD_All->SetPoint( pTGraph_T0PAD_All->GetN(), V_X[TheRank] , V_T0PAD[TheRank]  ) ;
    pTGraph_S0PAD_All->SetPoint( pTGraph_S0PAD_All->GetN(), V_X[TheRank] , V_S0PAD[TheRank]  ) ;
//       std::cout 
//         << " * "  << std::setw(15) << std::setprecision(9) <<  V_X[TheRank] /1.E2
//         << " ; "  << std::setw(15) << std::setprecision(9) <<  V_SPAD [TheRank]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<  V_nSPAD [TheRank]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<  V_TPAD [TheRank]   
//         << " ; "  << std::setw(15) << std::setprecision(9) <<  V_T0PAD[TheRank]   
//         << std::endl ;
   }

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    TMultiGraph* pTMultiGraph_SPAD_ALL = new TMultiGraph();
    pTMultiGraph_SPAD_ALL->Add( pTGraph_SPAD_All  ,"l");
    
    pTMultiGraph_SPAD_ALL->SetTitle("S_{Pad}/S_{Cluster} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_SPAD_ALL->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_SPAD_ALL->GetYaxis()->SetTitle("S_{Pad}/S_{Cluster}");  

    pTMultiGraph_SPAD_ALL->SetMinimum(0.);
    pTMultiGraph_SPAD_ALL->SetMaximum(1.);

    pTMultiGraph_SPAD_ALL->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 3 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }
  
  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_04_SPAD_ALL" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    TMultiGraph* pTMultiGraph_nSPAD_ALL = new TMultiGraph();
    pTMultiGraph_nSPAD_ALL->Add( pTGraph_nSPAD_All  ,"l");
    
    pTMultiGraph_nSPAD_ALL->SetTitle("S_{Pad}/S_{Max} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_nSPAD_ALL->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_nSPAD_ALL->GetYaxis()->SetTitle("S_{Pad}/S_{Max}");  

    pTMultiGraph_nSPAD_ALL->SetMinimum(0.);
//   pTMultiGraph_nSPAD_ALL->SetMaximum(1.);

    pTMultiGraph_nSPAD_ALL->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 3 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }
  
  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_04_nSPAD_ALL" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    TMultiGraph* pTMultiGraph_TPAD_All = new TMultiGraph();
    pTMultiGraph_TPAD_All->Add( pTGraph_TPAD_All  ,"l");
    
    pTMultiGraph_TPAD_All->SetTitle("T_{Pad}-T_{Earlier Pad} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_TPAD_All->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_TPAD_All->GetYaxis()->SetTitle("T_{Pad}-T_{Earlier Pad} (#mu s)");  
    pTMultiGraph_TPAD_All->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_TPAD_All->Draw("A");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 3 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }

  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_04_TPAD_All" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
  
//
  double Est_Min = 0. ;
  double Est_Max = 1.6  ;
  TGraph* pTGraph_GenerateShapeFunctionTool  = aGenerateShapeFunctionTool.Draw_TimeEstimator(Est_Min,Est_Max) ;
  pTGraph_GenerateShapeFunctionTool->SetLineStyle(10) ;
  pTGraph_GenerateShapeFunctionTool->SetLineColor(4)  ;
   
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    TMultiGraph* pTMultiGraph_T0PAD_All = new TMultiGraph();
    pTMultiGraph_T0PAD_All->Add( pTGraph_T0PAD_All  ,"l");
    
    pTMultiGraph_T0PAD_All->SetTitle("T_{Pad}-T_{0} VS Y_{Track}-Y_{Pad}");  

    pTMultiGraph_T0PAD_All->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_T0PAD_All->GetYaxis()->SetTitle("T_{Pad}-T_{0} (#mu s)");  
    pTMultiGraph_T0PAD_All->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_T0PAD_All->Draw("A");
    pTCanvas->Update();

    pTGraph_GenerateShapeFunctionTool->Draw("Same");
    pTCanvas->Update();

    Vert_Ymin = pTCanvas->GetUymin() ;
    Vert_Ymax = pTCanvas->GetUymax() ;
    for (int iV = 3 ; iV< 7; iV++){
      double YCur =   LY/2. + double (iV-3) * LY ;
      TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
      pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
      pTCanvas->Update()    ;
    }
    
  Str_Out_PDF = "OUT_SignalShape/GenerateShapeFunction_04_T0PAD_All" + TAG + ".pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

//
  std::string Out_PRF =  "OUT_SignalShape/SignalTime_True" + TAG + ".csv" ;

  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( Out_PRF.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file

    int     Npoints_in_pTGraph_SPAD_All = pTGraph_SPAD_All->GetN() ;
    double*       X_in_pTGraph_SPAD_All = pTGraph_SPAD_All->GetX() ;
    
    double*       Y_in_pTGraph_SPAD_All  = pTGraph_SPAD_All ->GetY() ;
    double*       Y_in_pTGraph_nSPAD_All = pTGraph_nSPAD_All->GetY() ;
    double*       Y_in_pTGraph_TPAD_All  = pTGraph_TPAD_All ->GetY() ;
    double*       Y_in_pTGraph_T0PAD_All = pTGraph_T0PAD_All->GetY() ;
    double*       Y_in_pTGraph_S0PAD_All = pTGraph_S0PAD_All->GetY() ;
    for (int iTem = 0 ; iTem< Npoints_in_pTGraph_SPAD_All; iTem++){
      std::cout 
        << std::setw(15) << std::setprecision(9) << X_in_pTGraph_SPAD_All[iTem]/1.E2
        << " ; "  << std::setw(15) << std::setprecision(9) << Y_in_pTGraph_SPAD_All [iTem]
        << " ; "  << std::setw(15) << std::setprecision(9) << Y_in_pTGraph_nSPAD_All[iTem]
        << " ; "  << std::setw(15) << std::setprecision(9) << Y_in_pTGraph_TPAD_All [iTem]
        << " ; "  << std::setw(15) << std::setprecision(9) << Y_in_pTGraph_T0PAD_All[iTem]
        << " ; "  << std::setw(15) << std::setprecision(9) << Y_in_pTGraph_S0PAD_All[iTem]
        << std::endl ;
    }

  std::cout.rdbuf(coutbuf); // Reset to standard output again

}
