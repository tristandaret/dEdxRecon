#include "GenerateShape/Shape_Check_Compare.h"
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

void Shape_Check_Compare()
{
  SC_VS_x_rc_dd    aOLD("Data_SignalTime/Load_400ns_VS_x_rc_dd.csv") ;
  SC_1D_VS_x_rc_dd aNEW("Data_SignalTime_1D/Load_1D_400ns_VS_x_rc_dd.csv") ;

  Shape_Check_Compare_Rho_01  (&aNEW,&aOLD) ; 
  Shape_Check_Compare_Alpha_01(&aNEW,&aOLD) ; 
  Shape_Check_Compare_Tau_01  (&aNEW,&aOLD) ; 

}

void Shape_Check_Compare_Rho_01(SC_1D_VS_x_rc_dd_I* pNew,SC_1D_VS_x_rc_dd_I* pOld)
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
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Compare_Rho_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  for (int iDD = 0 ; iDD< 3 ; iDD++){
    double DDcur = 0. ;
    if (iDD== 0) { DDcur =    0. ; WhatDD = " DD    0 " ; }
    if (iDD== 1) { DDcur =  500. ; WhatDD = " DD  500 " ; }
    if (iDD== 2) { DDcur = 1000. ; WhatDD = " DD 1000 " ; }
    
    for (int iRC = 0 ; iRC< 6 ; iRC++){
      double RCcur = 0. ;
      if (iRC==0) { RCcur =  50. ; WhatRC = " RC=  40. " ; }
      if (iRC==1) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
      if (iRC==2) { RCcur = 150. ; WhatRC = " RC= 120. " ; }
      
      TMultiGraph* pTMultiGraph      = new TMultiGraph();
      TMultiGraph* pTMultiGraph_Diff = new TMultiGraph();
     
      std::ostringstream aostringstream ;
      aostringstream << std::setiosflags(std::ios::fixed) ;
      aostringstream << WhatDD ;
      aostringstream << " , " ;
      aostringstream << WhatRC ;

      pTMultiGraph     ->SetTitle( (aostringstream.str()).c_str() )  ; 
      pTMultiGraph_Diff->SetTitle( (aostringstream.str()).c_str() )  ; 
      
      TGraph* pTGraph_New  = new TGraph() ; 
      TGraph* pTGraph_Old  = new TGraph() ; 
      TGraph* pTGraph_Diff = new TGraph() ; 
      
      pTMultiGraph     ->Add( pTGraph_New  ,"l");
      pTMultiGraph     ->Add( pTGraph_Old  ,"l");
      pTMultiGraph_Diff->Add( pTGraph_Diff ,"l");
      
      pTGraph_New->SetLineColor(2) ;
      pTGraph_Old->SetLineColor(4) ;
       
      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        
        double vCur_New  = pNew->Eval_RhoPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_New ->SetPoint( pTGraph_New ->GetN(), vY_cur , vCur_New  ) ;
        
        double vCur_Old  = pOld->Eval_RhoPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_Old ->SetPoint( pTGraph_Old ->GetN(), vY_cur , vCur_Old  ) ;
        
        double Diff = vCur_New - vCur_Old ;
        pTGraph_Diff ->SetPoint( pTGraph_Old ->GetN(), vY_cur , Diff  ) ;
      }  
          
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      
        pTCanvas->Divide(1,2) ;
        
        pTCanvas->cd(1) ;
          pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph->GetYaxis()->SetTitle("#rho = A_{Pad}/A_{Cluster}");  
          pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph->Draw("A");
          
          pTCanvas->Update();

        pTCanvas->cd(2) ;
          pTMultiGraph_Diff->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph_Diff->GetYaxis()->SetTitle("#Delta #rho ");  
          pTMultiGraph_Diff->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph_Diff->Draw("A");
          
          pTMultiGraph_Diff->SetMinimum(-0.001);
          pTMultiGraph_Diff->SetMaximum( 0.001);
          pTCanvas->Update();

          pTCanvas->Update();

      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      
    }
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

void Shape_Check_Compare_Alpha_01(SC_1D_VS_x_rc_dd_I* pNew,SC_1D_VS_x_rc_dd_I* pOld)
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
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Compare_Alpha_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  for (int iDD = 0 ; iDD< 3 ; iDD++){
    double DDcur = 0. ;
    if (iDD== 0) { DDcur =    0. ; WhatDD = " DD    0 " ; }
    if (iDD== 1) { DDcur =  500. ; WhatDD = " DD  500 " ; }
    if (iDD== 2) { DDcur = 1000. ; WhatDD = " DD 1000 " ; }
    
    for (int iRC = 0 ; iRC< 6 ; iRC++){
      double RCcur = 0. ;
      if (iRC==0) { RCcur =  50. ; WhatRC = " RC=  40. " ; }
      if (iRC==1) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
      if (iRC==2) { RCcur = 150. ; WhatRC = " RC= 120. " ; }
      
      TMultiGraph* pTMultiGraph      = new TMultiGraph();
      TMultiGraph* pTMultiGraph_Diff = new TMultiGraph();
     
      std::ostringstream aostringstream ;
      aostringstream << std::setiosflags(std::ios::fixed) ;
      aostringstream << WhatDD ;
      aostringstream << " , " ;
      aostringstream << WhatRC ;

      pTMultiGraph     ->SetTitle( (aostringstream.str()).c_str() )  ; 
      pTMultiGraph_Diff->SetTitle( (aostringstream.str()).c_str() )  ; 
      
      TGraph* pTGraph_New  = new TGraph() ; 
      TGraph* pTGraph_Old  = new TGraph() ; 
      TGraph* pTGraph_Diff = new TGraph() ; 
      
      pTMultiGraph     ->Add( pTGraph_New  ,"l");
      pTMultiGraph     ->Add( pTGraph_Old  ,"l");
      pTMultiGraph_Diff->Add( pTGraph_Diff ,"l");
      
      pTGraph_New->SetLineColor(2) ;
      pTGraph_Old->SetLineColor(4) ;
       
      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        
        double vCur_New  = pNew->Eval_AlphaPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_New ->SetPoint( pTGraph_New ->GetN(), vY_cur , vCur_New  ) ;
        
        double vCur_Old  = pOld->Eval_AlphaPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_Old ->SetPoint( pTGraph_Old ->GetN(), vY_cur , vCur_Old  ) ;
        
        double Diff = vCur_New - vCur_Old ;
        pTGraph_Diff ->SetPoint( pTGraph_Old ->GetN(), vY_cur , Diff  ) ;
      }  
          
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      
        pTCanvas->Divide(1,2) ;
        
        pTCanvas->cd(1) ;
          pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph->GetYaxis()->SetTitle("#alpha = A_{Pad}/A_{Leading}");
          pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph->Draw("A");
          
          pTCanvas->Update();

        pTCanvas->cd(2) ;
          pTMultiGraph_Diff->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph_Diff->GetYaxis()->SetTitle("#Delta #alpha ");
          pTMultiGraph_Diff->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph_Diff->Draw("A");
          
          pTCanvas->Update();

          pTMultiGraph_Diff->SetMinimum(-0.001);
          pTMultiGraph_Diff->SetMaximum( 0.001);
          pTCanvas->Update();

      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      
    }
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}

void Shape_Check_Compare_Tau_01(SC_1D_VS_x_rc_dd_I* pNew,SC_1D_VS_x_rc_dd_I* pOld)
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
  Str_Out_PDF =  "OUT_SignalShape/Shape_Check_Compare_Tau_01.pdf" ; 
  Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(Str_Out_PDF_Beg.c_str());
  
//
  for (int iDD = 0 ; iDD< 3 ; iDD++){
    double DDcur = 0. ;
    if (iDD== 0) { DDcur =    0. ; WhatDD = " DD    0 " ; }
    if (iDD== 1) { DDcur =  500. ; WhatDD = " DD  500 " ; }
    if (iDD== 2) { DDcur = 1000. ; WhatDD = " DD 1000 " ; }
    
    for (int iRC = 0 ; iRC< 6 ; iRC++){
      double RCcur = 0. ;
      if (iRC==0) { RCcur =  50. ; WhatRC = " RC=  40. " ; }
      if (iRC==1) { RCcur = 100. ; WhatRC = " RC= 100. " ; }
      if (iRC==2) { RCcur = 150. ; WhatRC = " RC= 120. " ; }
      
      TMultiGraph* pTMultiGraph      = new TMultiGraph();
      TMultiGraph* pTMultiGraph_Diff = new TMultiGraph();
     
      std::ostringstream aostringstream ;
      aostringstream << std::setiosflags(std::ios::fixed) ;
      aostringstream << WhatDD ;
      aostringstream << " , " ;
      aostringstream << WhatRC ;

      pTMultiGraph     ->SetTitle( (aostringstream.str()).c_str() )  ; 
      pTMultiGraph_Diff->SetTitle( (aostringstream.str()).c_str() )  ; 
      
      TGraph* pTGraph_New  = new TGraph() ; 
      TGraph* pTGraph_Old  = new TGraph() ; 
      TGraph* pTGraph_Diff = new TGraph() ; 
      
      pTMultiGraph     ->Add( pTGraph_New  ,"l");
      pTMultiGraph     ->Add( pTGraph_Old  ,"l");
      pTMultiGraph_Diff->Add( pTGraph_Diff ,"l");
      
      pTGraph_New->SetLineColor(2) ;
      pTGraph_Old->SetLineColor(4) ;
       
      int Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
        
        double vCur_New  = pNew->Eval_TauPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_New ->SetPoint( pTGraph_New ->GetN(), vY_cur , vCur_New  ) ;
        
        double vCur_Old  = pOld->Eval_TauPRF(vY_cur, RCcur,DDcur) ; 
        pTGraph_Old ->SetPoint( pTGraph_Old ->GetN(), vY_cur , vCur_Old  ) ;
        
        double Diff = vCur_New - vCur_Old ;
        pTGraph_Diff ->SetPoint( pTGraph_Old ->GetN(), vY_cur , Diff  ) ;
      }  
          
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      
        pTCanvas->Divide(1,2) ;
        
        pTCanvas->cd(1) ;
          pTMultiGraph->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph->GetYaxis()->SetTitle("#tau = T_{Pad}-T_{Leading} (#mu s)");  
          pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph->Draw("A");
          
          pTCanvas->Update();

        pTCanvas->cd(2) ;
          pTMultiGraph_Diff->GetXaxis()->SetTitle("Y_{Track}-Y_{Pad} (cm)");

          pTMultiGraph_Diff->GetYaxis()->SetTitle("#Delta #tau (#mu s)");  
          pTMultiGraph_Diff->GetYaxis()->SetTitleOffset(1.5);
  
          pTMultiGraph_Diff->Draw("A");
          
          pTCanvas->Update();

          pTMultiGraph_Diff->SetMinimum(-0.001);
          pTMultiGraph_Diff->SetMaximum( 0.001);
          pTCanvas->Update();

      pTCanvas->SaveAs(Str_Out_PDF.c_str());
      
    }
    
  }
  
  pTCanvas->SaveAs(Str_Out_PDF_End.c_str());

}
