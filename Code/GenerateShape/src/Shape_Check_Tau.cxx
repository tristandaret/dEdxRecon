#include "GenerateShape/Shape_Check_Tau.h"
#include "Misc/Util.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/SC_VS_x_rc_dd.h"
#include "SignalShape/SC_1D_VS_x_rc_dd.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLine.h"
#include "TF1.h"

void Shape_Check_Tau()
{
  SC_VS_x_rc_dd aSC("Data_SignalTime/Load_400ns_VS_x_rc_dd.csv") ;
//SC_1D_VS_x_rc_dd aSC("Data_SignalTime_1D/Load_1D_400ns_VS_x_rc_dd.csv") ;

  Shape_Check_Tau_Fixed_DD_01(&aSC) ; 
  Shape_Check_Tau_Fixed_RC_01(&aSC) ; 
  Shape_Check_Tau_Fixed_RC_02(&aSC) ;

}

void Shape_Check_Tau_Fixed_DD_01(SC_1D_VS_x_rc_dd_I* pSC_1D_VS_x_rc_dd_I)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -1.5 * LY * 1.E2 ;
  double vY_max =  1.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Tau_Fixed_DD_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
  int Kounter_CD = 0 ;
  
  for (int iDD = 0 ; iDD< 11 ; iDD++){
    double DDcur = 0. ;
    if (iDD== 0) { DDcur =  40 ; WhatDD = " DD  40 " ; }
    if (iDD== 1) { DDcur =  60 ; WhatDD = " DD  60 " ; }
    if (iDD== 2) { DDcur =  75 ; WhatDD = " DD  75 " ; }
    if (iDD== 3) { DDcur = 160 ; WhatDD = " DD 160 " ; }
    if (iDD== 4) { DDcur = 260 ; WhatDD = " DD 260 " ; }
    if (iDD== 5) { DDcur = 360 ; WhatDD = " DD 360 " ; }
    if (iDD== 6) { DDcur = 460 ; WhatDD = " DD 460 " ; }
    if (iDD== 7) { DDcur = 560 ; WhatDD = " DD 560 " ; }
    if (iDD== 8) { DDcur = 660 ; WhatDD = " DD 660 " ; }
    if (iDD== 9) { DDcur = 760 ; WhatDD = " DD 760 " ; }
    if (iDD==10) { DDcur = 860 ; WhatDD = " DD 860 " ; }
    
    Kounter_CD += 1 ;
    
    if (Kounter_CD==1 ){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }

    if ( Kounter_CD==7 ){
      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      Kounter_CD = 1;
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }
    
    pTCanvas->cd( Kounter_CD );
    pTCanvas->cd( Kounter_CD )->SetGrid();
    
    TLegend* pTLegend = new TLegend(0.40,0.60 ,0.65,0.88  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    TMultiGraph* pTMultiGraph = new TMultiGraph();
    pTMultiGraph->SetTitle( WhatDD.c_str() )  ; 

    for (int iRC = 0 ; iRC< 6 ; iRC++){
      double RCcur = 0. ;
      if (iRC==0) { RCcur =  40. ; WhatRC = " RC=  40. " ; }
      if (iRC==1) { RCcur =  60. ; WhatRC = " RC=  60. " ; }
      if (iRC==2) { RCcur =  80. ; WhatRC = " RC=  80. " ; }
      if (iRC==3) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
      if (iRC==4) { RCcur = 120. ; WhatRC = " RC= 120. " ; }
      if (iRC==5) { RCcur = 140. ; WhatRC = " RC= 140. " ; }
      
      TGraph* pTGraph  = new TGraph() ; 
      pTGraph->SetLineColor(4) ;
      if (RCcur <= 40.) pTGraph->SetLineColor(2) ;
       
      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        double vCur  = pSC_1D_VS_x_rc_dd_I->Eval_TauPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph ->SetPoint( pTGraph ->GetN(), vY_cur , vCur  ) ;
      }
      
      pTLegend->AddEntry( pTGraph , WhatRC.c_str() , "l");
      
      pTMultiGraph->Add( pTGraph ,"l");
      
    }
    
    pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph->GetYaxis()->SetTitle("#tau = T_{Pad}-T_{Leading} (#mu s)");  
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

//     pTMultiGraph->SetMinimum(-0.1);
//     pTMultiGraph->SetMaximum( 2.2);

    pTMultiGraph->Draw("A");
    pTCanvas->Update();
    
    pTLegend->Draw();       
    pTCanvas->Update();
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

void Shape_Check_Tau_Fixed_RC_01(SC_1D_VS_x_rc_dd_I* pSC_1D_VS_x_rc_dd_I)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -1.5 * LY * 1.E2 ;
  double vY_max =  1.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Tau_Fixed_RC_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
  int Kounter_CD = 0 ;
  
  for (int iRC = 0 ; iRC< 6 ; iRC++){
    double RCcur = 0. ;
    if (iRC==0) { RCcur =  40. ; WhatRC = " RC=  40. " ; }
    if (iRC==1) { RCcur =  60. ; WhatRC = " RC=  60. " ; }
    if (iRC==2) { RCcur =  80. ; WhatRC = " RC=  80. " ; }
    if (iRC==3) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
    if (iRC==4) { RCcur = 120. ; WhatRC = " RC= 120. " ; }
    if (iRC==5) { RCcur = 140. ; WhatRC = " RC= 140. " ; }
    
    Kounter_CD += 1 ;
    
    if (Kounter_CD==1 ){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }

    if ( Kounter_CD==7 ){
      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      Kounter_CD = 1;
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }
    
    pTCanvas->cd( Kounter_CD );
    pTCanvas->cd( Kounter_CD )->SetGrid();
    
    TLegend* pTLegend = new TLegend(0.40,0.30 ,0.60,0.78  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    TMultiGraph* pTMultiGraph = new TMultiGraph();
    pTMultiGraph->SetTitle( WhatRC.c_str() )  ; 

    for (int iDD = 0 ; iDD< 11 ; iDD++){
      double DDcur = 0. ;
      if (iDD== 0) { DDcur =  40 ; WhatDD = " DD  40 " ; }
      if (iDD== 1) { DDcur =  60 ; WhatDD = " DD  60 " ; }
      if (iDD== 2) { DDcur =  75 ; WhatDD = " DD  75 " ; }
      if (iDD== 3) { DDcur = 160 ; WhatDD = " DD 160 " ; }
      if (iDD== 4) { DDcur = 260 ; WhatDD = " DD 260 " ; }
      if (iDD== 5) { DDcur = 360 ; WhatDD = " DD 360 " ; }
      if (iDD== 6) { DDcur = 460 ; WhatDD = " DD 460 " ; }
      if (iDD== 7) { DDcur = 560 ; WhatDD = " DD 560 " ; }
      if (iDD== 8) { DDcur = 660 ; WhatDD = " DD 660 " ; }
      if (iDD== 9) { DDcur = 760 ; WhatDD = " DD 760 " ; }
      if (iDD==10) { DDcur = 860 ; WhatDD = " DD 860 " ; }
      
      TGraph* pTGraph  = new TGraph() ;
      pTGraph ->SetLineColor(4) ;  
      if (iDD==0) pTGraph ->SetLineColor(2) ; 

      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        double vCur  = pSC_1D_VS_x_rc_dd_I->Eval_TauPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph ->SetPoint( pTGraph ->GetN(), vY_cur , vCur  ) ;
      }
    
      pTLegend->AddEntry( pTGraph , WhatDD.c_str() , "l");

      pTMultiGraph->Add( pTGraph ,"l");
    
    }
    
    pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph->GetYaxis()->SetTitle("#tau = T_{Pad}-T_{Leading} (#mu s)");  
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph->SetMinimum(-0.1);
    pTMultiGraph->SetMaximum( 2.2);

    pTMultiGraph->Draw("A");
    pTCanvas->Update();
    
    pTLegend->Draw();       
    pTCanvas->Update();
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());
  
}


void Shape_Check_Tau_Fixed_RC_02(SC_1D_VS_x_rc_dd_I* pSC_1D_VS_x_rc_dd_I)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -1.5 * LY * 1.E2 ;
  double vY_max =  1.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Tau_Fixed_RC_02.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
  int Kounter_CD = 0 ;
  
  for (int iRC = 0 ; iRC< 6 ; iRC++){
    double RCcur = 0. ;
    if (iRC==0) { RCcur =  40. ; WhatRC = " RC=  40. " ; }
    if (iRC==1) { RCcur =  60. ; WhatRC = " RC=  60. " ; }
    if (iRC==2) { RCcur =  80. ; WhatRC = " RC=  80. " ; }
    if (iRC==3) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
    if (iRC==4) { RCcur = 120. ; WhatRC = " RC= 120. " ; }
    if (iRC==5) { RCcur = 140. ; WhatRC = " RC= 140. " ; }
    
    Kounter_CD += 1 ;
    
    if (Kounter_CD==1 ){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }

    if ( Kounter_CD==7 ){
      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      Kounter_CD = 1;
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
    }
    
    pTCanvas->cd( Kounter_CD );
    pTCanvas->cd( Kounter_CD )->SetGrid();
    
    TLegend* pTLegend = new TLegend(0.40,0.30 ,0.60,0.78  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    TMultiGraph* pTMultiGraph = new TMultiGraph();
    pTMultiGraph->SetTitle( WhatRC.c_str() )  ; 

    for (int iDD = 0 ; iDD< 11 ; iDD++){
      double DDref = 40. ;
      double DDcur = 0. ;
      if (iDD== 0) { DDcur =  40 ; WhatDD = " DD  40 " ; }
      if (iDD== 1) { DDcur =  60 ; WhatDD = " DD  60 " ; }
      if (iDD== 2) { DDcur =  75 ; WhatDD = " DD  75 " ; }
      if (iDD== 3) { DDcur = 160 ; WhatDD = " DD 160 " ; }
      if (iDD== 4) { DDcur = 260 ; WhatDD = " DD 260 " ; }
      if (iDD== 5) { DDcur = 360 ; WhatDD = " DD 360 " ; }
      if (iDD== 6) { DDcur = 460 ; WhatDD = " DD 460 " ; }
      if (iDD== 7) { DDcur = 560 ; WhatDD = " DD 560 " ; }
      if (iDD== 8) { DDcur = 660 ; WhatDD = " DD 660 " ; }
      if (iDD== 9) { DDcur = 760 ; WhatDD = " DD 760 " ; }
      if (iDD==10) { DDcur = 860 ; WhatDD = " DD 860 " ; }
      
      TGraph* pTGraph  = new TGraph() ;
      pTGraph ->SetLineColor(4) ;  
      if (iDD==0) pTGraph ->SetLineColor(2) ; 

      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        double vCur  = pSC_1D_VS_x_rc_dd_I->Eval_TauPRF(vY_cur, RCcur,DDcur)
                     - pSC_1D_VS_x_rc_dd_I->Eval_TauPRF(vY_cur, RCcur,DDref); 
        pTGraph ->SetPoint( pTGraph ->GetN(), vY_cur , vCur  ) ;
      }
    
      pTLegend->AddEntry( pTGraph , WhatDD.c_str() , "l");

      pTMultiGraph->Add( pTGraph ,"l");
    
    }
    
    pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph->GetYaxis()->SetTitle("#Delta #tau (#mu s)");  
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph->SetMinimum(-0.35);
    pTMultiGraph->SetMaximum( 0.010);

    pTMultiGraph->Draw("A");
    pTCanvas->Update();
    
    pTLegend->Draw();       
    pTCanvas->Update();
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());
  
}
