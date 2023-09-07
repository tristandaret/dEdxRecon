#include "Studies/WF_04.h"

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

void WF_04()
{
// 
  double SY = 0.0 ;
  
  double RCin = 100. ;

//
  TCanvas* pTCanvas =  0 ;

  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*         pModel_Charge1D        = new Model_Charge1D        ;

  double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge1D->Set_RC(LocalRC) ;

  pModel_Charge1D->Set_Width(0.);
  
  double TheQValue = pModel_Electronics->Get_QValue() ;
  double TheWS     = pModel_Electronics->Get_ws()     ;
  pModel_Electronics->Set_TimeShape(TheQValue,TheWS);
  
  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ; 
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 

//  
  ROBoard aROBoard(
                   pModel_ReadOutGeometry  ,
                   pModel_Electronics      ,
                   pModel_Charge1D
                  );
                  
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  for (int iX = 0 ; iX< Nx; iX++){
    for (int iY = 0 ; iY< Ny; iY++){
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << "( " << iX << " , " << iY << " )" ;
       std::string PadName = aostringstream.str() ; 
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge1D , PadName , EventNber,EntryNber,ModuleNber   ,iX ,iY ); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//Compute A^o responses, signal for instantaneous deposit a t=0
  double Time0 = 0. ;
//       Time0 = 5000. * 1E-9 ; 

  int iYTrue = Ny/2 ;
  double YTrue = pModel_ReadOutGeometry->Get_YcPad(0,iYTrue,ModuleNber);

  YTrue += SY * Ly/2. ;
 
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,0., YTrue ); 
  }

  Pad* pPad_Center = aROBoard.Get_Pad(0,iYTrue  ) ;
//   Pad* pPad_NextU  = aROBoard.Get_Pad(0,iYTrue+1) ;
//   Pad* pPad_NextD  = aROBoard.Get_Pad(0,iYTrue-1) ;
  
//  
  TMultiGraph* pTMultiGraph_AmaxVSRC = new TMultiGraph ;

  std::vector < double > V_AMAX ;

  TGraph* pTGraph_AmaxVSRC = new TGraph ;
  pTMultiGraph_AmaxVSRC->Add(pTGraph_AmaxVSRC) ;

  double RCin_min =  50. ;
  double RCin_max = 150. ;
  int iTurn_Max = 40 ;
  for (int iTurn = 0 ; iTurn< iTurn_Max ; iTurn++){

    double RCin = RCin_min + double(iTurn)*(RCin_max-RCin_min)/double(iTurn_Max-1) ;

    double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
    pModel_Charge1D->Set_RC(LocalRC) ;

    pModel_Charge1D->Set_Width(0.);
    
    double TheQValue = pModel_Electronics->Get_QValue() ;
    double TheWS     = pModel_Electronics->Get_ws()     ;
    pModel_Electronics->Set_TimeShape(TheQValue,TheWS);

    Pad* pPad_Cur = pPad_Center ;

    int    Tmin_in_ns =    0 ;
    int    Tmax_in_ns = 5000 ;
    double Tmin =  double(Tmin_in_ns) * 1E-9 ; 
    double Tmax =  double(Tmax_in_ns) * 1E-9 ; 
    int    Npoints = (Tmax_in_ns-Tmin_in_ns) + 1  ;
    double AMAX = 0. ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
            
      if (iPoint==0){
        AMAX = 0. ;
      } 
      if (AMAX<ADC){
        AMAX = ADC ;
      }
    }
    pTGraph_AmaxVSRC->SetPoint( pTGraph_AmaxVSRC->GetN(), RCin , AMAX  ) ;
    
    V_AMAX.push_back(AMAX);
    
  }

//
  TMultiGraph* pTMultiGraph_DERIV_AmaxVSRC = new TMultiGraph ;

  TGraph* pTGraph_DERIV_AmaxVSRC = new TGraph ;
  pTMultiGraph_DERIV_AmaxVSRC->Add(pTGraph_DERIV_AmaxVSRC) ;

  double DeltaRCin =  (RCin_max-RCin_min)/double(iTurn_Max-1) ;
  for (int iTurn = 0 ; iTurn< iTurn_Max-1 ; iTurn++){
    double RCin = RCin_min + double(iTurn)*(RCin_max-RCin_min)/double(iTurn_Max-1) ;
    double Deriv = (V_AMAX[iTurn+1]-V_AMAX[iTurn])/DeltaRCin ;
    Deriv = 100.*Deriv/((V_AMAX[iTurn+1]+V_AMAX[iTurn])/2.);
    pTGraph_DERIV_AmaxVSRC->SetPoint( pTGraph_DERIV_AmaxVSRC->GetN(), RCin , Deriv  ) ;
  }

//Page 1
  std::ostringstream aostringstream_PDF ;
  aostringstream_PDF << std::setiosflags(std::ios::fixed) ;
  aostringstream_PDF << "OUT_Studies/WF_04_";
  aostringstream_PDF << ".pdf";
  std::string Str_Out_PDF = aostringstream_PDF.str() ; 

  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());


//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_AmaxVSRC->SetTitle("ADC_{max}(RC)");
    pTMultiGraph_AmaxVSRC->GetXaxis()->SetTitle("RC  ");
    pTMultiGraph_AmaxVSRC->GetYaxis()->SetTitle("ADC_{max}");
    pTMultiGraph_AmaxVSRC->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_AmaxVSRC->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_DERIV_AmaxVSRC->SetTitle("d(ADC_{max})/dRC");
    pTMultiGraph_DERIV_AmaxVSRC->GetXaxis()->SetTitle("RC  ");
    pTMultiGraph_DERIV_AmaxVSRC->GetYaxis()->SetTitle("d(ADC_{max})/dRC in %");
    pTMultiGraph_DERIV_AmaxVSRC->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_DERIV_AmaxVSRC->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//Page LAST
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
