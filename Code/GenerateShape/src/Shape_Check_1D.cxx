#include "GenerateShape/Shape_Check_1D.h"
#include "Misc/Util.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLine.h"
#include "TF1.h"

void Shape_Check_1D()
{
  SC_1D_VS_x_rc_dd aNEW("Data_SignalTime_1D/Load_1D_400ns_VS_x_rc_dd.csv") ;

  Shape_Check_A_T_Base_01                    (&aNEW) ; 
  Shape_Check_1D_A_T_01                      (&aNEW) ; 
  Shape_Check_1D_Alpha_Tau_TauBin_Rho_01     (&aNEW) ; 
  Shape_Check_1D_Alpha_Tau_TauBin_Rho_PRF_01 (&aNEW) ; 
  Shape_Check_A_T_PRF_01                     (&aNEW) ; 

}

void Shape_Check_A_T_Base_01(SC_1D_VS_x_rc_dd* pNew)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min =  0. ;
  double vY_max =  0.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_A_T_Base_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  double DDcur = 2000. ; WhatDD = " DD= 2000. " ; 
  double RCcur =  100. ; WhatRC = " RC= 100."   ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << WhatDD ;
  aostringstream << " , " ;
  aostringstream << WhatRC ;

  TMultiGraph* pTMultiGraph_Am = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Ti = new TMultiGraph();
  pTMultiGraph_Am ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Ti ->SetTitle( (aostringstream.str()).c_str() )  ; 
  
  TLegend* pTLegend_Am = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Am->SetFillStyle(1001);
  pTLegend_Am->SetBorderSize(1);

  TLegend* pTLegend_Ti = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Ti->SetFillStyle(1001);
  pTLegend_Ti->SetBorderSize(1);

  TGraph* pTGraph_Am_DW_Base = new TGraph() ; 
  TGraph* pTGraph_Am_LE_Base = new TGraph() ; 
  TGraph* pTGraph_Am_UP_Base = new TGraph() ; 
  TGraph* pTGraph_Ti_DW_Base = new TGraph() ; 
  TGraph* pTGraph_Ti_LE_Base = new TGraph() ; 
  TGraph* pTGraph_Ti_UP_Base = new TGraph() ; 
  
  pTGraph_Am_DW_Base->SetLineColor(1) ;
  pTGraph_Am_LE_Base->SetLineColor(2) ;
  pTGraph_Am_UP_Base->SetLineColor(4) ;
  pTGraph_Ti_DW_Base->SetLineColor(1) ;
  pTGraph_Ti_LE_Base->SetLineColor(2) ;
  pTGraph_Ti_UP_Base->SetLineColor(4) ;

  pTMultiGraph_Am ->Add( pTGraph_Am_DW_Base ,"l");
  pTMultiGraph_Am ->Add( pTGraph_Am_LE_Base ,"l");
  pTMultiGraph_Am ->Add( pTGraph_Am_UP_Base ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_DW_Base ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_LE_Base ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_UP_Base ,"l");
  
  pTLegend_Am->AddEntry( pTGraph_Am_DW_Base , "DW", "l");
  pTLegend_Am->AddEntry( pTGraph_Am_LE_Base , "LE", "l");
  pTLegend_Am->AddEntry( pTGraph_Am_UP_Base , "UP", "l");

  pTLegend_Ti->AddEntry( pTGraph_Ti_DW_Base , "DW", "l");
  pTLegend_Ti->AddEntry( pTGraph_Ti_LE_Base , "LE", "l");
  pTLegend_Ti->AddEntry( pTGraph_Ti_UP_Base , "UP", "l");

  int Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
    
    double vCur_Am_DW_Base = pNew->Am_DW_Base(vY_cur, RCcur,DDcur) ; 
    double vCur_Am_LE_Base = pNew->Am_LE_Base(vY_cur, RCcur,DDcur) ; 
    double vCur_Am_UP_Base = pNew->Am_UP_Base(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_DW_Base = pNew->Ti_DW_Base(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_LE_Base = pNew->Ti_LE_Base(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_UP_Base = pNew->Ti_UP_Base(vY_cur, RCcur,DDcur) ; 
    
    pTGraph_Am_DW_Base ->SetPoint( pTGraph_Am_DW_Base ->GetN(), vY_cur , vCur_Am_DW_Base ) ;
    pTGraph_Am_LE_Base ->SetPoint( pTGraph_Am_LE_Base ->GetN(), vY_cur , vCur_Am_LE_Base ) ;
    pTGraph_Am_UP_Base ->SetPoint( pTGraph_Am_UP_Base ->GetN(), vY_cur , vCur_Am_UP_Base ) ;
    pTGraph_Ti_DW_Base ->SetPoint( pTGraph_Ti_DW_Base ->GetN(), vY_cur , vCur_Ti_DW_Base ) ;
    pTGraph_Ti_LE_Base ->SetPoint( pTGraph_Ti_LE_Base ->GetN(), vY_cur , vCur_Ti_LE_Base ) ;
    pTGraph_Ti_UP_Base ->SetPoint( pTGraph_Ti_UP_Base ->GetN(), vY_cur , vCur_Ti_UP_Base ) ;
  }  
   
      
//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Am->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Am->GetYaxis()->SetTitle("A");  
    pTMultiGraph_Am->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Am->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Am->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Ti->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Ti->GetYaxis()->SetTitle("T (#mu s)");  
    pTMultiGraph_Ti->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Ti->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Ti->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
    

//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

void Shape_Check_1D_A_T_01(SC_1D_VS_x_rc_dd* pNew)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -0.5 * LY * 1.E2 ;
  double vY_max =  0.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_1D_A_T_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  double DDcur = 2000. ; WhatDD = " DD= 2000. " ; 
  double RCcur =  100. ; WhatRC = " RC= 100."   ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << WhatDD ;
  aostringstream << " , " ;
  aostringstream << WhatRC ;

  TMultiGraph* pTMultiGraph_Am = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Ti = new TMultiGraph();
  pTMultiGraph_Am ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Ti ->SetTitle( (aostringstream.str()).c_str() )  ; 
  
  TLegend* pTLegend_Am = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Am->SetFillStyle(1001);
  pTLegend_Am->SetBorderSize(1);

  TLegend* pTLegend_Ti = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Ti->SetFillStyle(1001);
  pTLegend_Ti->SetBorderSize(1);

  TGraph* pTGraph_Am_DW = new TGraph() ; 
  TGraph* pTGraph_Am_LE = new TGraph() ; 
  TGraph* pTGraph_Am_UP = new TGraph() ; 
  TGraph* pTGraph_Ti_DW = new TGraph() ; 
  TGraph* pTGraph_Ti_LE = new TGraph() ; 
  TGraph* pTGraph_Ti_UP = new TGraph() ; 
  
  pTGraph_Am_DW->SetLineColor(1) ;
  pTGraph_Am_LE->SetLineColor(2) ;
  pTGraph_Am_UP->SetLineColor(4) ;
  pTGraph_Ti_DW->SetLineColor(1) ;
  pTGraph_Ti_LE->SetLineColor(2) ;
  pTGraph_Ti_UP->SetLineColor(4) ;

  pTMultiGraph_Am ->Add( pTGraph_Am_DW ,"l");
  pTMultiGraph_Am ->Add( pTGraph_Am_LE ,"l");
  pTMultiGraph_Am ->Add( pTGraph_Am_UP ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_DW ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_LE ,"l");
  pTMultiGraph_Ti ->Add( pTGraph_Ti_UP ,"l");
  
  pTLegend_Am->AddEntry( pTGraph_Am_DW , "DW", "l");
  pTLegend_Am->AddEntry( pTGraph_Am_LE , "LE", "l");
  pTLegend_Am->AddEntry( pTGraph_Am_UP , "UP", "l");

  pTLegend_Ti->AddEntry( pTGraph_Ti_DW , "DW", "l");
  pTLegend_Ti->AddEntry( pTGraph_Ti_LE , "LE", "l");
  pTLegend_Ti->AddEntry( pTGraph_Ti_UP , "UP", "l");

  int Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
    
    double vCur_Am_DW = pNew->Am_DW(vY_cur, RCcur,DDcur) ; 
    double vCur_Am_LE = pNew->Am_LE(vY_cur, RCcur,DDcur) ; 
    double vCur_Am_UP = pNew->Am_UP(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_DW = pNew->Ti_DW(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_LE = pNew->Ti_LE(vY_cur, RCcur,DDcur) ; 
    double vCur_Ti_UP = pNew->Ti_UP(vY_cur, RCcur,DDcur) ; 
    
    pTGraph_Am_DW ->SetPoint( pTGraph_Am_DW ->GetN(), vY_cur , vCur_Am_DW ) ;
    pTGraph_Am_LE ->SetPoint( pTGraph_Am_LE ->GetN(), vY_cur , vCur_Am_LE ) ;
    pTGraph_Am_UP ->SetPoint( pTGraph_Am_UP ->GetN(), vY_cur , vCur_Am_UP ) ;
    pTGraph_Ti_DW ->SetPoint( pTGraph_Ti_DW ->GetN(), vY_cur , vCur_Ti_DW ) ;
    pTGraph_Ti_LE ->SetPoint( pTGraph_Ti_LE ->GetN(), vY_cur , vCur_Ti_LE ) ;
    pTGraph_Ti_UP ->SetPoint( pTGraph_Ti_UP ->GetN(), vY_cur , vCur_Ti_UP ) ;
  }  
   
      
//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Am->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Am->GetYaxis()->SetTitle("A");  
    pTMultiGraph_Am->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Am->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Am->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Ti->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Ti->GetYaxis()->SetTitle("T (#mu s)");  
    pTMultiGraph_Ti->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Ti->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Ti->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
    

//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

void Shape_Check_1D_Alpha_Tau_TauBin_Rho_01(SC_1D_VS_x_rc_dd* pNew)
{
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -0.5 * LY * 1.E2 ;
  double vY_max =  0.5 * LY * 1.E2 ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     ="BIDON" ;
  std::string Str_Out_PDF_Beg ="BIDON" ;
  std::string Str_Out_PDF_End ="BIDON" ;

//
  std::string WhatDD = "BIDON" ;  
  std::string WhatRC = "BIDON" ;  

//
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_1D_Alpha_Tau_TauBin_Rho_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  double DDcur = 2000. ; WhatDD = " DD= 2000. " ; 
  double RCcur =  100. ; WhatRC = " RC= 100."   ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << WhatDD ;
  aostringstream << " , " ;
  aostringstream << WhatRC ;

  TMultiGraph* pTMultiGraph_Alpha  = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Tau    = new TMultiGraph();
  TMultiGraph* pTMultiGraph_TauBin = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Rho    = new TMultiGraph();
  pTMultiGraph_Alpha  ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Tau    ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_TauBin ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Rho    ->SetTitle( (aostringstream.str()).c_str() )  ; 
  
  TLegend* pTLegend_Alpha = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Alpha->SetFillStyle(1001);
  pTLegend_Alpha->SetBorderSize(1);

  TLegend* pTLegend_Tau = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Tau->SetFillStyle(1001);
  pTLegend_Tau->SetBorderSize(1);

  TLegend* pTLegend_TauBin = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_TauBin->SetFillStyle(1001);
  pTLegend_TauBin->SetBorderSize(1);

  TLegend* pTLegend_Rho = new TLegend(0.60,0.50 ,0.70,0.60  ); 
  pTLegend_Rho->SetFillStyle(1001);
  pTLegend_Rho->SetBorderSize(1);

  TGraph* pTGraph_Alpha_DW  = new TGraph() ; 
  TGraph* pTGraph_Alpha_UP  = new TGraph() ; 
  TGraph* pTGraph_Tau_DW    = new TGraph() ; 
  TGraph* pTGraph_Tau_UP    = new TGraph() ; 
  TGraph* pTGraph_TauBin_DW = new TGraph() ; 
  TGraph* pTGraph_TauBin_UP = new TGraph() ; 
  TGraph* pTGraph_Rho_DW    = new TGraph() ; 
  TGraph* pTGraph_Rho_LE    = new TGraph() ; 
  TGraph* pTGraph_Rho_UP    = new TGraph() ; 
  
  pTGraph_Alpha_DW ->SetLineColor(1) ;
  pTGraph_Alpha_UP ->SetLineColor(4) ;
  pTGraph_Tau_DW   ->SetLineColor(1) ;
  pTGraph_Tau_UP   ->SetLineColor(4) ;
  pTGraph_TauBin_DW->SetLineColor(1) ;
  pTGraph_TauBin_UP->SetLineColor(4) ;
  pTGraph_Rho_DW   ->SetLineColor(1) ;
  pTGraph_Rho_LE   ->SetLineColor(1) ;
  pTGraph_Rho_UP   ->SetLineColor(4) ;

  pTMultiGraph_Alpha  ->Add( pTGraph_Alpha_DW  ,"l");
  pTMultiGraph_Alpha  ->Add( pTGraph_Alpha_UP  ,"l");
  pTMultiGraph_Tau    ->Add( pTGraph_Tau_DW    ,"l");
  pTMultiGraph_Tau    ->Add( pTGraph_Tau_UP    ,"l");
  pTMultiGraph_TauBin ->Add( pTGraph_TauBin_DW ,"l");
  pTMultiGraph_TauBin ->Add( pTGraph_TauBin_UP ,"l");
  pTMultiGraph_Rho    ->Add( pTGraph_Rho_DW    ,"l");
  pTMultiGraph_Rho    ->Add( pTGraph_Rho_LE    ,"l");
  pTMultiGraph_Rho    ->Add( pTGraph_Rho_UP    ,"l");
  
  pTLegend_Alpha->AddEntry( pTGraph_Alpha_DW , "DW", "l");
  pTLegend_Alpha->AddEntry( pTGraph_Alpha_UP , "UP", "l");

  pTLegend_Tau->AddEntry( pTGraph_Tau_DW , "DW", "l");
  pTLegend_Tau->AddEntry( pTGraph_Tau_UP , "UP", "l");

  pTLegend_TauBin->AddEntry( pTGraph_TauBin_DW , "DW", "l");
  pTLegend_TauBin->AddEntry( pTGraph_TauBin_UP , "UP", "l");

  pTLegend_Rho->AddEntry( pTGraph_Rho_DW , "DW", "l");
  pTLegend_Rho->AddEntry( pTGraph_Rho_LE , "LE", "l");
  pTLegend_Rho->AddEntry( pTGraph_Rho_UP , "UP", "l");

  int Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
    
    double vCur_Alpha_DW  = pNew->Alpha_DW (vY_cur, RCcur,DDcur) ; 
    double vCur_Alpha_UP  = pNew->Alpha_UP (vY_cur, RCcur,DDcur) ; 
    double vCur_Tau_DW    = pNew->Tau_DW   (vY_cur, RCcur,DDcur) ; 
    double vCur_Tau_UP    = pNew->Tau_UP   (vY_cur, RCcur,DDcur) ; 
    double vCur_TauBin_DW = pNew->TauBin_DW(vY_cur, RCcur,DDcur) ; 
    double vCur_TauBin_UP = pNew->TauBin_UP(vY_cur, RCcur,DDcur) ; 
    double vCur_Rho_DW    = pNew->Rho_DW   (vY_cur, RCcur,DDcur) ; 
    double vCur_Rho_LE    = pNew->Rho_LE   (vY_cur, RCcur,DDcur) ; 
    double vCur_Rho_UP    = pNew->Rho_UP   (vY_cur, RCcur,DDcur) ; 
    
    pTGraph_Alpha_DW  ->SetPoint( pTGraph_Alpha_DW  ->GetN(), vY_cur , vCur_Alpha_DW  ) ;
    pTGraph_Alpha_UP  ->SetPoint( pTGraph_Alpha_UP  ->GetN(), vY_cur , vCur_Alpha_UP  ) ;
    pTGraph_Tau_DW    ->SetPoint( pTGraph_Tau_DW    ->GetN(), vY_cur , vCur_Tau_DW    ) ;
    pTGraph_Tau_UP    ->SetPoint( pTGraph_Tau_UP    ->GetN(), vY_cur , vCur_Tau_UP    ) ;
    pTGraph_TauBin_DW ->SetPoint( pTGraph_TauBin_DW ->GetN(), vY_cur , vCur_TauBin_DW ) ;
    pTGraph_TauBin_UP ->SetPoint( pTGraph_TauBin_UP ->GetN(), vY_cur , vCur_TauBin_UP ) ;
    pTGraph_Rho_DW    ->SetPoint( pTGraph_Rho_DW    ->GetN(), vY_cur , vCur_Rho_DW    ) ;
    pTGraph_Rho_LE    ->SetPoint( pTGraph_Rho_LE    ->GetN(), vY_cur , vCur_Rho_LE    ) ;
    pTGraph_Rho_UP    ->SetPoint( pTGraph_Rho_UP    ->GetN(), vY_cur , vCur_Rho_UP    ) ;
  }  
   
      
//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Alpha->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Alpha->GetYaxis()->SetTitle("#alpha");  
    pTMultiGraph_Alpha->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Alpha->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Alpha->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Tau->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Tau->GetYaxis()->SetTitle("#tau (#mu s)");  
    pTMultiGraph_Tau->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Tau->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Tau->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
      
      
//page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_TauBin->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_TauBin->GetYaxis()->SetTitle("#tau  (tb)");  
    pTMultiGraph_TauBin->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_TauBin->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_TauBin->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Rho->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Rho->GetYaxis()->SetTitle("#rho ");  
    pTMultiGraph_Rho->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Rho->Draw("A");
    
    pTCanvas->Update();
  
    pTLegend_Rho->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
    

//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}


void Shape_Check_1D_Alpha_Tau_TauBin_Rho_PRF_01(SC_1D_VS_x_rc_dd* pNew)
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
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_1D_Alpha_Tau_TauBin_Rho_PRF_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  double DDcur = 2000. ; WhatDD = " DD= 2000. " ; 
  double RCcur =  100. ; WhatRC = " RC= 100."   ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << WhatDD ;
  aostringstream << " , " ;
  aostringstream << WhatRC ;

  TMultiGraph* pTMultiGraph_Rho    = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Alpha  = new TMultiGraph();
  TMultiGraph* pTMultiGraph_Tau    = new TMultiGraph();
  TMultiGraph* pTMultiGraph_TauBin = new TMultiGraph();
  pTMultiGraph_Rho    ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Alpha  ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_Tau    ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_TauBin ->SetTitle( (aostringstream.str()).c_str() )  ; 
  
  TGraph* pTGraph_Rho    = new TGraph() ; 
  TGraph* pTGraph_Alpha  = new TGraph() ; 
  TGraph* pTGraph_Tau    = new TGraph() ; 
  TGraph* pTGraph_TauBin = new TGraph() ; 
  
  pTGraph_Rho    ->SetLineColor(2) ;
  pTGraph_Alpha  ->SetLineColor(2) ;
  pTGraph_Tau    ->SetLineColor(2) ;
  pTGraph_TauBin ->SetLineColor(2) ;

  pTMultiGraph_Rho    ->Add( pTGraph_Rho    ,"l");
  pTMultiGraph_Alpha  ->Add( pTGraph_Alpha  ,"l");
  pTMultiGraph_Tau    ->Add( pTGraph_Tau    ,"l");
  pTMultiGraph_TauBin ->Add( pTGraph_TauBin ,"l");
  
  int Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
    
    double vCur_Rho_PRF    = pNew->Rho_PRF   (vY_cur, RCcur,DDcur) ; 
    double vCur_Alpha_PRF  = pNew->Alpha_PRF (vY_cur, RCcur,DDcur) ; 
    double vCur_Tau_PRF    = pNew->Tau_PRF   (vY_cur, RCcur,DDcur) ; 
    double vCur_TauBin_PRF = pNew->TauBin_PRF(vY_cur, RCcur,DDcur) ; 
    
    pTGraph_Rho    ->SetPoint( pTGraph_Rho    ->GetN(), vY_cur , vCur_Rho_PRF    ) ;
    pTGraph_Alpha  ->SetPoint( pTGraph_Alpha  ->GetN(), vY_cur , vCur_Alpha_PRF  ) ;
    pTGraph_Tau    ->SetPoint( pTGraph_Tau    ->GetN(), vY_cur , vCur_Tau_PRF    ) ;
    pTGraph_TauBin ->SetPoint( pTGraph_TauBin ->GetN(), vY_cur , vCur_TauBin_PRF ) ;
  }  
   
      
//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Rho->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Rho->GetYaxis()->SetTitle("#rho ");  
    pTMultiGraph_Rho->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Rho->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
    

//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Alpha->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Alpha->GetYaxis()->SetTitle("#alpha");  
    pTMultiGraph_Alpha->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Alpha->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_Tau->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_Tau->GetYaxis()->SetTitle("#tau (#mu s)");  
    pTMultiGraph_Tau->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_Tau->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
      
      
//page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_TauBin->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_TauBin->GetYaxis()->SetTitle("#tau  (tb)");  
    pTMultiGraph_TauBin->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_TauBin->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}
void Shape_Check_A_T_PRF_01(SC_1D_VS_x_rc_dd* pNew)
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
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_A_T_PRF_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  double DDcur = 2000. ; WhatDD = " DD= 2000. " ; 
  double RCcur =  100. ; WhatRC = " RC= 100."   ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << WhatDD ;
  aostringstream << " , " ;
  aostringstream << WhatRC ;

  TMultiGraph* pTMultiGraph_A = new TMultiGraph();
  TMultiGraph* pTMultiGraph_T = new TMultiGraph();
  pTMultiGraph_A ->SetTitle( (aostringstream.str()).c_str() )  ; 
  pTMultiGraph_T ->SetTitle( (aostringstream.str()).c_str() )  ; 
  
  TGraph* pTGraph_A_PRF = new TGraph() ; 
  TGraph* pTGraph_T_PRF = new TGraph() ; 
  
  pTGraph_A_PRF->SetLineColor(1) ;
  pTGraph_T_PRF->SetLineColor(1) ;

  pTMultiGraph_A ->Add( pTGraph_A_PRF ,"l");
  pTMultiGraph_T ->Add( pTGraph_T_PRF ,"l");

  int Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
    
    double vCur_A_PRF = pNew->A_PRF(vY_cur, RCcur,DDcur) ; 
    double vCur_T_PRF = pNew->T_PRF(vY_cur, RCcur,DDcur) ; 
    
    pTGraph_A_PRF ->SetPoint( pTGraph_A_PRF ->GetN(), vY_cur , vCur_A_PRF ) ;
    pTGraph_T_PRF ->SetPoint( pTGraph_T_PRF ->GetN(), vY_cur , vCur_T_PRF ) ;
  }  
   
      
//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_A->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_A->GetYaxis()->SetTitle("A");  
    pTMultiGraph_A->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_A->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

      
//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
   
    pTMultiGraph_T->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

    pTMultiGraph_T->GetYaxis()->SetTitle("T (#mu s)");  
    pTMultiGraph_T->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_T->Draw("A");
    
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());
    

//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

