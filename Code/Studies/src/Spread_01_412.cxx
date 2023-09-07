#include "Studies/Spread_01_412.h"

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

void Spread_01_412()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Spread_01_412.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

// 
  TRandom3 aTRandom3 ;

  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 
  
//
  double SamplingFrequency = 25. ; // in MHz

  std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;
  SC_VS_x_rc_dd aSC_VS_x_rc_dd(FILE_SC_VS_x_rc_dd,SamplingFrequency) ;
    
//  
  double YPP =  Ly ;
  double YPM = -Ly ;
  
  TMultiGraph* pTMultiGraph_A2vsYT = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_A3vsYT = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_A2vsA3 = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_A2vsA3onA2 = new TMultiGraph ;

  TMultiGraph* pTMultiGraph_T2vsYT = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_T3vsYT = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_T2vsT3 = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_T2vsT3mT2 = new TMultiGraph ;

  TMultiGraph* pTMultiGraph_A2vsT2        = new TMultiGraph ;
  TMultiGraph* pTMultiGraph_A3onA2vsT3mT2 = new TMultiGraph ;


  TLegend* pTLegend_A = new TLegend(0.15,0.50 ,0.55,0.85  ); 
  pTLegend_A->SetFillStyle(1001);
  pTLegend_A->SetBorderSize(1);

  TLegend* pTLegend_T = new TLegend(0.50,0.50 ,0.85,0.85  ); 
  pTLegend_T->SetFillStyle(1001);
  pTLegend_T->SetBorderSize(1);

  for (int iDD = 0 ; iDD< 2 ; iDD++){
    double DDcur = 0. ;
    std::string WhatDD = "BIDON" ;  
    if (iDD== 0) { DDcur =    0 ; WhatDD = ", DD=  0 " ; }
    if (iDD== 1) { DDcur = 2000 ; WhatDD = ", DD=2000" ; }
      
    for (int iRC = 0 ; iRC< 5 ; iRC++){
      double RCcur = 0. ;
      std::string WhatRC = "BIDON" ;  
      if (iRC==0) { RCcur =  40. ; WhatRC = "RC= 40" ; }
      if (iRC==1) { RCcur =  60. ; WhatRC = "RC= 60" ; }
      if (iRC==2) { RCcur =  80. ; WhatRC = "RC= 80" ; }
      if (iRC==3) { RCcur = 100. ; WhatRC = "RC=100" ; }
      if (iRC==4) { RCcur = 120. ; WhatRC = "RC=120" ; }
      
      std::string Comment = WhatRC + WhatDD ;
      
      std::vector< TGraph* > V_pTGraph ;
      
      TGraph* pTGraph_A2vsYT        = new TGraph ;  V_pTGraph.push_back ( pTGraph_A2vsYT        ) ;
      TGraph* pTGraph_A3vsYT        = new TGraph ;  V_pTGraph.push_back ( pTGraph_A3vsYT        ) ;
      TGraph* pTGraph_A2vsA3        = new TGraph ;  V_pTGraph.push_back ( pTGraph_A2vsA3        ) ;
      TGraph* pTGraph_A2vsA3onA2    = new TGraph ;  V_pTGraph.push_back ( pTGraph_A2vsA3onA2    ) ;
      
      TGraph* pTGraph_T2vsYT        = new TGraph ;  V_pTGraph.push_back ( pTGraph_T2vsYT        ) ;
      TGraph* pTGraph_T3vsYT        = new TGraph ;  V_pTGraph.push_back ( pTGraph_T3vsYT        ) ;
      TGraph* pTGraph_T2vsT3        = new TGraph ;  V_pTGraph.push_back ( pTGraph_T2vsT3        ) ;
      TGraph* pTGraph_T2vsT3mT2     = new TGraph ;  V_pTGraph.push_back ( pTGraph_T2vsT3mT2     ) ;
      
      TGraph* pTGraph_A2vsT2        = new TGraph ;  V_pTGraph.push_back ( pTGraph_A2vsT2        ) ;
      TGraph* pTGraph_A3onA2vsT3mT2 = new TGraph ;  V_pTGraph.push_back ( pTGraph_A3onA2vsT3mT2 ) ;

      int iTem_Max = V_pTGraph.size() ;
      for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
        TGraph* pTGraph = V_pTGraph[iTem] ;
        if (iRC==0) { pTGraph->SetLineColor(1) ; }
        if (iRC==1) { pTGraph->SetLineColor(2) ; }
        if (iRC==2) { pTGraph->SetLineColor(3) ; }
        if (iRC==3) { pTGraph->SetLineColor(4) ; }
        if (iRC==4) { pTGraph->SetLineColor(6) ; }

        if (iDD==0) { pTGraph->SetLineStyle( 1) ; }
        if (iDD==1) { pTGraph->SetLineStyle( 2) ; }
        
      }

      pTMultiGraph_A2vsYT->Add( pTGraph_A2vsYT ) ;
      pTMultiGraph_A3vsYT->Add( pTGraph_A3vsYT ) ;
      pTMultiGraph_A2vsA3->Add( pTGraph_A2vsA3 ) ;
      pTMultiGraph_A2vsA3onA2->Add( pTGraph_A2vsA3onA2 ) ;

      pTMultiGraph_T2vsYT->Add( pTGraph_T2vsYT ) ;
      pTMultiGraph_T3vsYT->Add( pTGraph_T3vsYT ) ;
      pTMultiGraph_T2vsT3->Add( pTGraph_T2vsT3 ) ;
      pTMultiGraph_T2vsT3mT2->Add( pTGraph_T2vsT3mT2 ) ;

      pTMultiGraph_A2vsT2       ->Add( pTGraph_A2vsT2        ) ;
      pTMultiGraph_A3onA2vsT3mT2->Add( pTGraph_A3onA2vsT3mT2 ) ;

      pTLegend_A->AddEntry( pTGraph_A2vsYT , Comment.c_str() , "l");
      
      pTLegend_T->AddEntry( pTGraph_T2vsYT , Comment.c_str() , "l");
      
      double YT_Min = 0. ;
      double YT_Max = 0.9999999*Ly/2. ;
      int iYT_Max = 1000 ;
      for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
        double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
        double YTYPP = YT-YPP ;
        double YTYPM = YT-YPM ;
 
        double nPRFP = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTYPP*1.E2, RCcur,DDcur) ;
        double nPRFM = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTYPM*1.E2, RCcur,DDcur) ;
        
        double nTP = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTYPP*1.E2, RCcur,DDcur) ;
        double nTM = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTYPM*1.E2, RCcur,DDcur) ;
        
        double A2 = std::max(nPRFP, nPRFM) ;
        double A3 = std::min(nPRFP, nPRFM) ;

        double T2 = nTP ;
        double T3 = nTM ;
        if (nPRFP < nPRFM ){
          T2 = nTM ;
          T3 = nTP ;
        }
        
        pTGraph_A2vsYT     ->SetPoint( pTGraph_A2vsYT     ->GetN(), YT*1.E2  , A2  ) ;
        pTGraph_A3vsYT     ->SetPoint( pTGraph_A3vsYT     ->GetN(), YT*1.E2  , A3  ) ;
        pTGraph_A2vsA3     ->SetPoint( pTGraph_A2vsA3     ->GetN(), A3       , A2  ) ;
        pTGraph_A2vsA3onA2 ->SetPoint( pTGraph_A2vsA3onA2 ->GetN(), A3/A2    , A2  ) ;
        
        pTGraph_T2vsYT    ->SetPoint( pTGraph_T2vsYT    ->GetN(), YT*1.E2  , T2  ) ;
        pTGraph_T3vsYT    ->SetPoint( pTGraph_T3vsYT    ->GetN(), YT*1.E2  , T3  ) ;
        pTGraph_T2vsT3    ->SetPoint( pTGraph_T2vsT3    ->GetN(), T3       , T2  ) ;
        pTGraph_T2vsT3mT2 ->SetPoint( pTGraph_T2vsT3mT2 ->GetN(), T3-T2    , T2  ) ;
        
        pTGraph_A2vsT2        ->SetPoint( pTGraph_A2vsT2        ->GetN() , T2    , A2    ) ;
        pTGraph_A3onA2vsT3mT2 ->SetPoint( pTGraph_A3onA2vsT3mT2 ->GetN() , T3-T2 , A3/A2 ) ;
        
      }
    }
  }
   
//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,2) ;
  
    pTCanvas->cd(1) ;
      pTMultiGraph_A2vsYT->Draw("A");
  
      pTMultiGraph_A2vsYT->SetTitle("#alpha_{2}=A2/A1 VS Track Position");
      pTMultiGraph_A2vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_A2vsYT->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_A2vsYT->SetMaximum( 1. );
      pTMultiGraph_A2vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
 
      pTLegend_A->Draw();       
      pTCanvas->Update();
      
    pTCanvas->cd(2) ;
      pTMultiGraph_A3vsYT->Draw("A");
  
      pTMultiGraph_A3vsYT->SetTitle("#alpha_{3}=A3/A1 VS Track Position");
      pTMultiGraph_A3vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_A3vsYT->GetYaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTMultiGraph_A3vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A3vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_A3vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
 
    pTCanvas->cd(3) ;
      pTMultiGraph_A2vsA3->Draw("A");
  
      pTMultiGraph_A2vsA3->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}=A3/A1");
      pTMultiGraph_A2vsA3->GetXaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTMultiGraph_A2vsA3->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsA3->GetYaxis()->SetTitleOffset(1.5);
      double Xmax_pTMultiGraph_A2vsA3 = pTMultiGraph_A2vsA3->GetXaxis()->GetXmax() ;
      pTMultiGraph_A2vsA3->GetXaxis()->SetLimits(0. , Xmax_pTMultiGraph_A2vsA3);
      pTMultiGraph_A2vsA3->SetMaximum( 1. );
      pTMultiGraph_A2vsA3->SetMinimum( 0. );
  
      pTCanvas->Update();
 
 
    pTCanvas->cd(4) ;
      pTMultiGraph_A2vsA3onA2->Draw("A");
  
      pTMultiGraph_A2vsA3onA2->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetLimits(0. , 1.);
      pTMultiGraph_A2vsA3onA2->SetMaximum( 1. );
      pTMultiGraph_A2vsA3onA2->SetMinimum( 0. );
  
      pTCanvas->Update();
 
  pTCanvas->Print(Str_Out_PDF.c_str());
  

//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,2) ;
  
    pTCanvas->cd(1) ;
      pTMultiGraph_A2vsYT->Draw("A");
  
      pTMultiGraph_A2vsYT->SetTitle("#alpha_{2}=A2/A1 VS Track Position");
      pTMultiGraph_A2vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_A2vsYT->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_A2vsYT->SetMaximum( 0.2 );
      pTMultiGraph_A2vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
    pTCanvas->cd(2) ;
      pTMultiGraph_A3vsYT->Draw("A");
  
      pTMultiGraph_A3vsYT->SetTitle("#alpha_{3}=A3/A1 VS Track Position");
      pTMultiGraph_A3vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_A3vsYT->GetYaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTMultiGraph_A3vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A3vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_A3vsYT->SetMaximum( 0.1 );
      pTMultiGraph_A3vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
  
    pTCanvas->cd(4) ;
      pTMultiGraph_A2vsA3onA2->Draw("A");
  
      pTMultiGraph_A2vsA3onA2->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetLimits(0. , 0.2);
      pTMultiGraph_A2vsA3onA2->SetMaximum( 1. );
      pTMultiGraph_A2vsA3onA2->SetMinimum( 0.9 );
  
      pTCanvas->Update();
  pTCanvas->Print(Str_Out_PDF.c_str());
  

//Page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,2) ;
  
    pTCanvas->cd(1) ;
      pTMultiGraph_T2vsYT->Draw("A");
  
      pTMultiGraph_T2vsYT->SetTitle("#tau_{2}=T2-T1 VS Track Position");
      pTMultiGraph_T2vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_T2vsYT->GetYaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_T2vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_T2vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_T2vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
 
      pTLegend_T->Draw();       
      pTCanvas->Update();
      
    pTCanvas->cd(2) ;
      pTMultiGraph_T3vsYT->Draw("A");
  
      pTMultiGraph_T3vsYT->SetTitle("#tau_{3}=T3-T1 VS Track Position");
      pTMultiGraph_T3vsYT->GetXaxis()->SetTitle("Y_{T}^{Cluster}-Y_{pad}^{Leading} (cm)");
      pTMultiGraph_T3vsYT->GetYaxis()->SetTitle("#tau_{3}=T3-T1 (Time bins)");
      pTMultiGraph_T3vsYT->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_T3vsYT->GetXaxis()->SetLimits(0. , Ly/2. *1.E2);
      pTMultiGraph_T3vsYT->SetMinimum( 0. );
    
      pTCanvas->Update();
 
    pTCanvas->cd(3) ;
      pTMultiGraph_T2vsT3->Draw("A");
  
      pTMultiGraph_T2vsT3->SetTitle("#tau_{2}=T2-T1 VS #tau_{3}=T3-T1");
      pTMultiGraph_T2vsT3->GetXaxis()->SetTitle("#tau_{3}=T3-T1 (Time bins)");
      pTMultiGraph_T2vsT3->GetYaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_T2vsT3->GetYaxis()->SetTitleOffset(1.5);
      double Xmax_pTMultiGraph_T2vsT3 = pTMultiGraph_T2vsT3->GetXaxis()->GetXmax() ;
      pTMultiGraph_T2vsT3->GetXaxis()->SetLimits(0. , Xmax_pTMultiGraph_T2vsT3);
      pTMultiGraph_T2vsT3->SetMinimum( 0. );
 
    pTCanvas->cd(4) ;
      pTMultiGraph_T2vsT3mT2->Draw("A");
  
      pTMultiGraph_T2vsT3mT2->SetTitle("#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2");
      pTMultiGraph_T2vsT3mT2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2 (Time bins)");
      pTMultiGraph_T2vsT3mT2->GetYaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_T2vsT3mT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_T2vsT3mT2->GetXaxis()->SetLimits(0. , 100.);
      pTMultiGraph_T2vsT3mT2->SetMaximum( 30. );
      pTMultiGraph_T2vsT3mT2->SetMinimum( 0. );
  
      pTCanvas->Update();
 
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,2) ;
  
    pTCanvas->cd(1) ;
      pTMultiGraph_A2vsT2->Draw("A");

      pTMultiGraph_A2vsT2->SetTitle("#alpha_{2}=A2/A1 VS #tau_{2}=T2-T1");
      pTMultiGraph_A2vsT2->GetXaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_A2vsT2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsT2->SetMaximum( 1. );
      pTMultiGraph_A2vsT2->SetMinimum( 0. );
      pTMultiGraph_A2vsT2->GetXaxis()->SetLimits(0. , 30.);

      pTCanvas->Update();
 
      pTLegend_T->Draw();       
      pTCanvas->Update();
  
    pTCanvas->cd(2) ;
      pTMultiGraph_A3onA2vsT3mT2->Draw("A");

      pTMultiGraph_A3onA2vsT3mT2->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2 VS #tau_{3}-#tau_{2}=T3-T2");
      pTMultiGraph_A3onA2vsT3mT2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2 (Time bins)");
      pTMultiGraph_A3onA2vsT3mT2->GetYaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A3onA2vsT3mT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A3onA2vsT3mT2->SetMaximum( 1. );
      pTMultiGraph_A3onA2vsT3mT2->SetMinimum( 0. );
      pTMultiGraph_A3onA2vsT3mT2->GetXaxis()->SetLimits(0. , 100.);

      pTCanvas->Update();

  
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//Page 6
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2) ;
 
    pTCanvas->cd(2) ;
      pTMultiGraph_A2vsA3onA2->Draw("A");
  
      pTMultiGraph_A2vsA3onA2->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsA3onA2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsA3onA2->GetXaxis()->SetLimits(0. , 1.);
      pTMultiGraph_A2vsA3onA2->SetMaximum( 1. );
      pTMultiGraph_A2vsA3onA2->SetMinimum( 0. );
  
      pTCanvas->Update();
 
      pTLegend_T->Draw();       
      pTCanvas->Update();
  
    pTCanvas->cd(3) ;
      pTMultiGraph_A2vsT2->Draw("A");

      pTMultiGraph_A2vsT2->SetTitle("#alpha_{2}=A2/A1 VS #tau_{2}=T2-T1");
      pTMultiGraph_A2vsT2->GetXaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_A2vsT2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTMultiGraph_A2vsT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A2vsT2->SetMaximum( 1. );
      pTMultiGraph_A2vsT2->SetMinimum( 0. );
      pTMultiGraph_A2vsT2->GetXaxis()->SetLimits(-3.5,30.5);

      pTCanvas->Update();
 
    pTCanvas->cd(5) ;
      pTMultiGraph_T2vsT3mT2->Draw("A");
  
      pTMultiGraph_T2vsT3mT2->SetTitle("#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2");
      pTMultiGraph_T2vsT3mT2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2 (Time bins)");
      pTMultiGraph_T2vsT3mT2->GetYaxis()->SetTitle("#tau_{2}=T2-T1 (Time bins)");
      pTMultiGraph_T2vsT3mT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_T2vsT3mT2->GetXaxis()->SetLimits(-3.5,100.5);
      pTMultiGraph_T2vsT3mT2->SetMaximum( 30.5 );
      pTMultiGraph_T2vsT3mT2->SetMinimum( -3.5 );
  
    pTCanvas->cd(6) ;
      pTMultiGraph_A3onA2vsT3mT2->Draw("A");

      pTMultiGraph_A3onA2vsT3mT2->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2 VS #tau_{3}-#tau_{2}=T3-T2");
      pTMultiGraph_A3onA2vsT3mT2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2 (Time bins)");
      pTMultiGraph_A3onA2vsT3mT2->GetYaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTMultiGraph_A3onA2vsT3mT2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_A3onA2vsT3mT2->GetXaxis()->SetLimits(-3.5,100.5);
      pTMultiGraph_A3onA2vsT3mT2->SetMaximum( 1. );
      pTMultiGraph_A3onA2vsT3mT2->SetMinimum( 0. );

      pTCanvas->Update();

  
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//Page 7
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
