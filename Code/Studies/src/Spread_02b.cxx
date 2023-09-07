#include "Studies/Spread_02b.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/SC_VS_x_rc_dd.h"

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

void Spread_02b()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Spread_02b.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

// 
  TRandom3 aTRandom3 ;

  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 
  
//
  double Conv_MicroSec_2_TimeBin = 1000./40. ;

  std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;
  SC_VS_x_rc_dd aSC_VS_x_rc_dd(FILE_SC_VS_x_rc_dd,Conv_MicroSec_2_TimeBin) ;
    
//  
  double Y_PU =  Ly ;
  double Y_PD = -Ly ;
  
  TMultiGraph* pTMultiGraph_E_Alpha   = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_E_TauBin  = new TMultiGraph ;

  TLegend* pTLegend_E = new TLegend(0.15,0.65 ,0.40,0.85  ); 
  pTLegend_E->SetFillStyle(1001);
  pTLegend_E->SetBorderSize(1);

//
    int iDD = 0 ;
    double DDcur = 0. ;
    std::string WhatDD = "BIDON" ;  
    if (iDD== 0) { DDcur =    0 ; WhatDD = ", DD=  0 " ; }
    if (iDD== 1) { DDcur = 1000 ; WhatDD = ", DD=1000" ; }
      
  for (int iRC = 0 ; iRC< 2 ; iRC++){
    double RCcur = 0. ;
    std::string WhatRC = "BIDON" ;  
    if (iRC==0) { RCcur =  40. ; WhatRC = "RC= 40" ; }
    if (iRC==1) { RCcur = 100. ; WhatRC = "RC=100" ; }

//      
      double YT_Min =-0.9999999*Ly/2. ;
      double YT_Max = 0.9999999*Ly/2. ;
      int iYT_Max = 1000 ;
//
      TGraph* pTGraph_E_Alpha_Sa = new TGraph ;  pTGraph_E_Alpha_Sa ->SetLineColor(2) ; 
      TGraph* pTGraph_E_Alpha_Sb = new TGraph ;  pTGraph_E_Alpha_Sb ->SetLineColor(4) ; 
      
      pTGraph_E_Alpha_Sa ->SetLineStyle(1) ;
      pTGraph_E_Alpha_Sb ->SetLineStyle(1) ;
      if (iRC==1){
        pTGraph_E_Alpha_Sa ->SetLineStyle(2) ;
        pTGraph_E_Alpha_Sb ->SetLineStyle(2) ;
      }
      
      pTMultiGraph_E_Alpha ->Add( pTGraph_E_Alpha_Sa ) ;
      pTMultiGraph_E_Alpha ->Add( pTGraph_E_Alpha_Sb ) ;
      
      pTLegend_E->AddEntry( pTGraph_E_Alpha_Sa  , std::string( WhatRC +", Sc a").c_str()   , "l");
      pTLegend_E->AddEntry( pTGraph_E_Alpha_Sb  , std::string( WhatRC +", Sc b").c_str() , "l");

      double Alpha_PU_Last  = 0. ;
      double Alpha_PD_Last  = 0. ;
      iYT_Max = 1000 ;
      for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
        double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
        double YTY_PU = YT-Y_PU ;
        double YTY_PD = YT-Y_PD ;
        
        double Alpha_PU  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PU*1.E2, RCcur,DDcur) ;
        double Alpha_PD  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PD*1.E2, RCcur,DDcur) ;
        
        double D_Alpha_PU  = ( Alpha_PU  - Alpha_PU_Last  )/( 1.E2*(YT_Max-YT_Min)/double(iYT_Max-1) );
        double D_Alpha_PD  = ( Alpha_PD  - Alpha_PD_Last  )/( 1.E2*(YT_Max-YT_Min)/double(iYT_Max-1) );
        
        if (iYT!=0){
//        Scenario A
          double A1 = 700. ;
          double Delta_Alpha_Sa_PU = std::sqrt( Alpha_PU*(1.+Alpha_PU))/std::sqrt(A1) ;
          double Delta_Alpha_Sa_PD = std::sqrt( Alpha_PD*(1.+Alpha_PD))/std::sqrt(A1) ;
          double E_Alpha_Sa = 
          1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sa_PU , 2 ) +  std::pow( D_Alpha_PD/Delta_Alpha_Sa_PD , 2 ) ) ;

//        Scenario B
          double DeltaA = 6 ;
          double Delta_Alpha_Sb_PU = DeltaA *std::sqrt(1.+Alpha_PU*Alpha_PU)/A1 ;
          double Delta_Alpha_Sb_PD = DeltaA *std::sqrt(1.+Alpha_PD*Alpha_PD)/A1 ;
          double E_Alpha_Sb = 
          1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sb_PU , 2 ) +  std::pow( D_Alpha_PD/Delta_Alpha_Sb_PD , 2 ) ) ;
          
          pTGraph_E_Alpha_Sa        ->SetPoint( pTGraph_E_Alpha_Sa ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sa )  ;
          pTGraph_E_Alpha_Sb        ->SetPoint( pTGraph_E_Alpha_Sb ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sb )  ;
        }
        
        Alpha_PU_Last  = Alpha_PU  ;
        Alpha_PD_Last  = Alpha_PD  ;
      }

//
      TGraph* pTGraph_E_TauBin = new TGraph ;  pTGraph_E_TauBin ->SetLineColor(2) ; 

      pTGraph_E_TauBin ->SetLineStyle(1) ;
      if (iRC==1){
        pTGraph_E_TauBin ->SetLineStyle(2) ;
      }

      pTMultiGraph_E_TauBin ->Add( pTGraph_E_TauBin ) ;
      
      double TauBin_PU_Last = 0. ;
      double TauBin_PD_Last = 0. ;
      iYT_Max = 50 ;
      for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
        double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
        double YTY_PU = YT-Y_PU ;
        double YTY_PD = YT-Y_PD ;
        
        double TauBin_PU = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PU*1.E2, RCcur,DDcur) ;
        double TauBin_PD = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PD*1.E2, RCcur,DDcur) ;
        
        double D_TauBin_PU = ( TauBin_PU - TauBin_PU_Last )/( 1.E2*(YT_Max-YT_Min)/double(iYT_Max-1) );
        double D_TauBin_PD = ( TauBin_PD - TauBin_PD_Last )/( 1.E2*(YT_Max-YT_Min)/double(iYT_Max-1) );
        
        if (iYT!=0){
          double DeltaTauBin = 1. ;
          double E_TauBin = 
          1./std::sqrt( std::pow( D_TauBin_PU/DeltaTauBin , 2 ) +  std::pow( D_TauBin_PD/DeltaTauBin , 2 ) ) ;
          
          pTGraph_E_TauBin       ->SetPoint( pTGraph_E_TauBin    ->GetN(), YT*1.E2 , 1.E4 * E_TauBin )  ;
        }
        
        TauBin_PU_Last = TauBin_PU ;
        TauBin_PD_Last = TauBin_PD ;
      }
  }

//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
 
//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_E_Alpha->Draw("A");
      pTMultiGraph_E_Alpha->SetTitle("Error from #alpha VS Track Position");
      pTMultiGraph_E_Alpha->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_Alpha->GetYaxis()->SetTitle("Error from #alpha (#mu m)");
      pTMultiGraph_E_Alpha->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_Alpha->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_Alpha->SetMaximum( 500. );
      pTMultiGraph_E_Alpha->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_E->Draw();       
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_E_TauBin->Draw("A");
      pTMultiGraph_E_TauBin->SetTitle("Error from #tau VS Track Position");
      pTMultiGraph_E_TauBin->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitle("Error from #tau (#mu m)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_TauBin->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_TauBin->SetMaximum( 500. );
      pTMultiGraph_E_TauBin->SetMinimum( 0. );
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
//Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
