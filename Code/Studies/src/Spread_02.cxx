#include "Studies/Spread_02.h"

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

void Spread_02()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Spread_02.pdf" ; 
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
  
  TMultiGraph* pTMultiGraph_Alpha   = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_TauBin  = new TMultiGraph ;

  TMultiGraph* pTMultiGraph_D_Alpha   = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_D_TauBin  = new TMultiGraph ;

  TMultiGraph* pTMultiGraph_E_Alpha   = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_E_Alpha_2 = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_E_TauBin  = new TMultiGraph ;

  TMultiGraph* pTMultiGraph_EC_Alpha          = new TMultiGraph ;  
  TMultiGraph* pTMultiGraph_EC_Alpha_Compare  = new TMultiGraph ;  

  TLegend* pTLegend_A = new TLegend(0.25,0.65 ,0.75,0.85  ); 
  pTLegend_A->SetFillStyle(1001);
  pTLegend_A->SetBorderSize(1);

  TLegend* pTLegend_E = new TLegend(0.15,0.65 ,0.55,0.85  ); 
  pTLegend_E->SetFillStyle(1001);
  pTLegend_E->SetBorderSize(1);

  TLegend* pTLegend_EC = new TLegend(0.15,0.65 ,0.85,0.85  ); 
  pTLegend_EC->SetFillStyle(1001);
  pTLegend_EC->SetBorderSize(1);

  TLegend* pTLegend_EC_Compare = new TLegend(0.15,0.65 ,0.855,0.85  ); 
  pTLegend_EC_Compare->SetFillStyle(1001);
  pTLegend_EC_Compare->SetBorderSize(1);

  TLegend* pTLegend_ET = new TLegend(0.3,0.15 ,0.75,0.25  ); 
  pTLegend_ET->SetFillStyle(1001);
  pTLegend_ET->SetBorderSize(1);

//
  for (int iDD = 0 ; iDD< 1 ; iDD++){
    double DDcur = 0. ;
    std::string WhatDD = "BIDON" ;  
    if (iDD== 0) { DDcur =    0 ; WhatDD = ", DD=  0 " ; }
    if (iDD== 1) { DDcur = 1000 ; WhatDD = ", DD=1000" ; }
      
  for (int iRC = 0 ; iRC< 1 ; iRC++){
    double RCcur = 0. ;
    std::string WhatRC = "BIDON" ;  
    if (iRC==0) { RCcur =  50. ; WhatRC = "RC= 50" ; }
    if (iRC==1) { RCcur = 100. ; WhatRC = "RC=100" ; }

//      
      std::string Comment = WhatRC + WhatDD ;
      
//
      TGraph* pTGraph_Alpha_PU  = new TGraph ;  pTGraph_Alpha_PU  ->SetLineColor(2) ;  
      TGraph* pTGraph_Alpha_PD  = new TGraph ;  pTGraph_Alpha_PD  ->SetLineColor(4) ; 
      TGraph* pTGraph_TauBin_PU = new TGraph ;  pTGraph_TauBin_PU ->SetLineColor(2) ; 
      TGraph* pTGraph_TauBin_PD = new TGraph ;  pTGraph_TauBin_PD ->SetLineColor(4) ; 
      
      pTMultiGraph_Alpha  ->Add( pTGraph_Alpha_PU  ) ;
      pTMultiGraph_Alpha  ->Add( pTGraph_Alpha_PD  ) ;
      pTMultiGraph_TauBin ->Add( pTGraph_TauBin_PU ) ;
      pTMultiGraph_TauBin ->Add( pTGraph_TauBin_PD ) ;

      pTLegend_A->AddEntry( pTGraph_Alpha_PU  ,std::string( "Up Pad, "     + WhatRC + WhatDD ).c_str() , "l");
      pTLegend_A->AddEntry( pTGraph_Alpha_PD  ,std::string( "Down Pad, "   + WhatRC + WhatDD ).c_str() , "l");
      
      pTGraph_Alpha_PU  ->SetLineStyle(1) ;
      pTGraph_Alpha_PD  ->SetLineStyle(1) ;
      pTGraph_TauBin_PU  ->SetLineStyle(1) ;
      pTGraph_TauBin_PD  ->SetLineStyle(1) ;
      if (iRC==1 || iDD==1){
      pTGraph_Alpha_PU  ->SetLineStyle(2) ;
      pTGraph_Alpha_PD  ->SetLineStyle(2) ;
      pTGraph_TauBin_PU ->SetLineStyle(2) ;
      pTGraph_TauBin_PD ->SetLineStyle(2) ;
      }
      
      double YT_Min =-0.9999999*Ly/2. ;
      double YT_Max = 0.9999999*Ly/2. ;
      int iYT_Max = 1000 ;
      for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
        double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
        double YTY_PU = YT-Y_PU ;
        double YTY_PD = YT-Y_PD ;
 
        double Alpha_PU  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PU*1.E2, RCcur,DDcur) ;
        double Alpha_PD  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PD*1.E2, RCcur,DDcur) ;
        double TauBin_PU = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PU*1.E2, RCcur,DDcur) ;
        double TauBin_PD = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PD*1.E2, RCcur,DDcur) ;

        pTGraph_Alpha_PU     ->SetPoint( pTGraph_Alpha_PU  ->GetN(), YT*1.E2  , Alpha_PU   ) ;
        pTGraph_Alpha_PD     ->SetPoint( pTGraph_Alpha_PD  ->GetN(), YT*1.E2  , Alpha_PD   ) ;
        pTGraph_TauBin_PU    ->SetPoint( pTGraph_TauBin_PU ->GetN(), YT*1.E2  , TauBin_PU  ) ;
        pTGraph_TauBin_PD    ->SetPoint( pTGraph_TauBin_PD ->GetN(), YT*1.E2  , TauBin_PD  ) ;
        
      }

//
      TGraph* pTGraph_D_Alpha_PU  = new TGraph ;  pTGraph_D_Alpha_PU  ->SetLineColor(2) ;  
      TGraph* pTGraph_D_Alpha_PD  = new TGraph ;  pTGraph_D_Alpha_PD  ->SetLineColor(4) ; 

      pTMultiGraph_D_Alpha  ->Add( pTGraph_D_Alpha_PU  ) ;
      pTMultiGraph_D_Alpha  ->Add( pTGraph_D_Alpha_PD  ) ;

      TGraph* pTGraph_E_Alpha_Sa   = new TGraph ;  pTGraph_E_Alpha_Sa   ->SetLineColor(2) ; 
      TGraph* pTGraph_E_Alpha_Sa_U = new TGraph ;  pTGraph_E_Alpha_Sa_U ->SetLineColor(1) ; 
      TGraph* pTGraph_E_Alpha_Sa_D = new TGraph ;  pTGraph_E_Alpha_Sa_D ->SetLineColor(1) ; 
      TGraph* pTGraph_E_Alpha_Sb   = new TGraph ;  pTGraph_E_Alpha_Sb   ->SetLineColor(4) ; 
      TGraph* pTGraph_E_Alpha_Sb_U = new TGraph ;  pTGraph_E_Alpha_Sb_U ->SetLineColor(1) ; 
      TGraph* pTGraph_E_Alpha_Sb_D = new TGraph ;  pTGraph_E_Alpha_Sb_D ->SetLineColor(1) ; 
      
      TGraph* pTGraph_EC_Alpha_Sa          = new TGraph ;  pTGraph_EC_Alpha_Sa          ->SetLineColor(2) ; 
      TGraph* pTGraph_EC_Alpha_Sb          = new TGraph ;  pTGraph_EC_Alpha_Sb          ->SetLineColor(4) ; 
      TGraph* pTGraph_E_Alpha_Sa_Compare   = new TGraph ;  pTGraph_E_Alpha_Sa_Compare   ->SetLineColor(2) ; 
      TGraph* pTGraph_E_Alpha_Sb_Compare   = new TGraph ;  pTGraph_E_Alpha_Sb_Compare   ->SetLineColor(4) ; 
      
      pTMultiGraph_EC_Alpha_Compare  ->Add( pTGraph_EC_Alpha_Sa   ) ;
      pTMultiGraph_EC_Alpha_Compare  ->Add( pTGraph_EC_Alpha_Sb   ) ;
      pTMultiGraph_EC_Alpha_Compare  ->Add( pTGraph_E_Alpha_Sa_Compare   ) ;
      pTMultiGraph_EC_Alpha_Compare  ->Add( pTGraph_E_Alpha_Sb_Compare   ) ;
      
      pTMultiGraph_EC_Alpha  ->Add( pTGraph_EC_Alpha_Sa   ) ;
      pTMultiGraph_EC_Alpha  ->Add( pTGraph_EC_Alpha_Sb   ) ;
      
      pTMultiGraph_E_Alpha  ->Add( pTGraph_E_Alpha_Sa   ) ;
      pTMultiGraph_E_Alpha  ->Add( pTGraph_E_Alpha_Sb   ) ;
      
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sa   ) ;
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sa_U ) ;
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sa_D ) ;
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sb   ) ;
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sb_U ) ;
      pTMultiGraph_E_Alpha_2 ->Add( pTGraph_E_Alpha_Sb_D ) ;
      
      pTLegend_E->AddEntry( pTGraph_E_Alpha_Sa  , std::string( WhatRC + WhatDD +", Sc a").c_str()   , "l");
      pTLegend_E->AddEntry( pTGraph_E_Alpha_Sb  , std::string( WhatRC + WhatDD +", Sc b").c_str() , "l");

      pTLegend_EC->AddEntry( pTGraph_EC_Alpha_Sa  , std::string( WhatRC + WhatDD +", Sc a").c_str()   , "l");
      pTLegend_EC->AddEntry( pTGraph_EC_Alpha_Sb  , std::string( WhatRC + WhatDD +", Sc b").c_str() , "l");

      pTLegend_EC_Compare->AddEntry( pTGraph_EC_Alpha_Sa        , std::string( "Proper method: " + WhatRC + WhatDD +", Sc a").c_str() , "l");
      pTLegend_EC_Compare->AddEntry( pTGraph_EC_Alpha_Sb        , std::string( "Proper method: " + WhatRC + WhatDD +", Sc b").c_str() , "l");
      pTLegend_EC_Compare->AddEntry( pTGraph_E_Alpha_Sa_Compare , std::string( "Approx method: " + WhatRC + WhatDD +", Sc a").c_str() , "l");
      pTLegend_EC_Compare->AddEntry( pTGraph_E_Alpha_Sb_Compare , std::string( "Approx method: " + WhatRC + WhatDD +", Sc b").c_str() , "l");

      pTGraph_D_Alpha_PU  ->SetLineStyle(1) ;
      pTGraph_D_Alpha_PD  ->SetLineStyle(1) ;
      pTGraph_E_Alpha_Sa  ->SetLineStyle(1) ;
      pTGraph_E_Alpha_Sb  ->SetLineStyle(1) ;
      pTGraph_EC_Alpha_Sa ->SetLineStyle(1) ;
      pTGraph_EC_Alpha_Sb ->SetLineStyle(1) ;
      pTGraph_E_Alpha_Sa_Compare ->SetLineStyle(9) ;
      pTGraph_E_Alpha_Sb_Compare ->SetLineStyle(9) ;
      if (iRC==1 || iDD==1 ){
      pTGraph_D_Alpha_PU  ->SetLineStyle(2) ;
      pTGraph_D_Alpha_PD  ->SetLineStyle(2) ;
      pTGraph_E_Alpha_Sa  ->SetLineStyle(2) ;
      pTGraph_E_Alpha_Sb  ->SetLineStyle(2) ;
      pTGraph_EC_Alpha_Sa ->SetLineStyle(2) ;
      pTGraph_EC_Alpha_Sb ->SetLineStyle(2) ;
      }
      
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
          pTGraph_D_Alpha_PU     ->SetPoint( pTGraph_D_Alpha_PU  ->GetN(), YT*1.E2  , D_Alpha_PU    ) ;
          pTGraph_D_Alpha_PD     ->SetPoint( pTGraph_D_Alpha_PD  ->GetN(), YT*1.E2  , D_Alpha_PD    ) ;

//
          double AL = 700. ;
          double AU = Alpha_PU * AL ;
          double AD = Alpha_PD * AL ;

//        Scenario A
          double SigmaL_Sa = sqrt( AL ) ;
          double SigmaU_Sa = sqrt( AU ) ;
          double SigmaD_Sa = sqrt( AD ) ;
          
//        Scenario B
          double DeltaA = 6 ;
          double SigmaL_Sb = DeltaA ;
          double SigmaU_Sb = DeltaA ;
          double SigmaD_Sb = DeltaA ;
          
//Appropriate method
//        Scenario A
          double EC_Sa = std::pow( (D_Alpha_PU * Alpha_PD) / (SigmaU_Sa * SigmaD_Sa ) - (D_Alpha_PD * Alpha_PU) / (SigmaD_Sa * SigmaU_Sa ) , 2 ) ;
          EC_Sa  = EC_Sa + ( std::pow( (D_Alpha_PU/SigmaU_Sa) ,2 ) +  std::pow( (D_Alpha_PD/SigmaD_Sa) ,2 ) )* std::pow( 1./SigmaL_Sa , 2 ) ;
          EC_Sa  =         ( std::pow( (  Alpha_PU/SigmaU_Sa) ,2 ) +  std::pow( (  Alpha_PD/SigmaD_Sa) ,2 )  + std::pow( 1./SigmaL_Sa , 2 ) )/EC_Sa ;
          EC_Sa  =  EC_Sa/std::pow(AL,2) ;
          EC_Sa  =  std::sqrt(EC_Sa) ;
          
//        Scenario B
          double EC_Sb = std::pow( (D_Alpha_PU * Alpha_PD) / (SigmaU_Sb * SigmaD_Sb ) - (D_Alpha_PD * Alpha_PU) / (SigmaD_Sb * SigmaU_Sb ) , 2 ) ;
          EC_Sb  = EC_Sb + ( std::pow( (D_Alpha_PU/SigmaU_Sb) ,2 ) +  std::pow( (D_Alpha_PD/SigmaD_Sb) ,2 ) )* std::pow( 1./SigmaL_Sb , 2 ) ;
          EC_Sb  =         ( std::pow( (  Alpha_PU/SigmaU_Sb) ,2 ) +  std::pow( (  Alpha_PD/SigmaD_Sb) ,2 )  + std::pow( 1./SigmaL_Sb , 2 ) )/EC_Sb ;
          EC_Sb  =  EC_Sb/std::pow(AL,2) ;
          EC_Sb  =  std::sqrt(EC_Sb) ;

//Approximate method
//        Scenario A
          double Delta_Alpha_Sa_PU = Alpha_PU * std::sqrt( std::pow((SigmaU_Sa/AU),2) + std::pow(SigmaL_Sa/AL,2) ) ;
          double Delta_Alpha_Sa_PD = Alpha_PD * std::sqrt( std::pow((SigmaD_Sa/AD),2) + std::pow(SigmaL_Sa/AL,2) ) ;
          
          double E_Alpha_Sa   = 1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sa_PU , 2 ) +  std::pow( D_Alpha_PD/Delta_Alpha_Sa_PD , 2 ) ) ;
          double E_Alpha_Sa_U = 1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sa_PU , 2 )                                                 ) ;
          double E_Alpha_Sa_D = 1./std::sqrt(                                                 std::pow( D_Alpha_PD/Delta_Alpha_Sa_PD , 2 ) ) ;

//        Scenario B
          double Delta_Alpha_Sb_PU = Alpha_PU * std::sqrt( std::pow((SigmaU_Sb/AU),2) + std::pow(SigmaL_Sb/AL,2) ) ;
          double Delta_Alpha_Sb_PD = Alpha_PD * std::sqrt( std::pow((SigmaD_Sb/AD),2) + std::pow(SigmaL_Sb/AL,2) ) ;
          
          double E_Alpha_Sb   = 1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sb_PU , 2 ) +  std::pow( D_Alpha_PD/Delta_Alpha_Sb_PD , 2 ) ) ;
          double E_Alpha_Sb_U = 1./std::sqrt( std::pow( D_Alpha_PU/Delta_Alpha_Sb_PU , 2 )                                                 ) ;
          double E_Alpha_Sb_D = 1./std::sqrt(                                                 std::pow( D_Alpha_PD/Delta_Alpha_Sb_PD , 2 ) ) ;
            
//
          pTGraph_EC_Alpha_Sa        ->SetPoint( pTGraph_EC_Alpha_Sa        ->GetN(), YT*1.E2 , 1.E4 * EC_Sa        )  ;
          pTGraph_EC_Alpha_Sb        ->SetPoint( pTGraph_EC_Alpha_Sb        ->GetN(), YT*1.E2 , 1.E4 * EC_Sb        )  ;
          pTGraph_E_Alpha_Sa_Compare ->SetPoint( pTGraph_E_Alpha_Sa_Compare ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sa   )  ;
          pTGraph_E_Alpha_Sb_Compare ->SetPoint( pTGraph_E_Alpha_Sb_Compare ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sb   )  ;
          
          pTGraph_E_Alpha_Sa        ->SetPoint( pTGraph_E_Alpha_Sa   ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sa   )  ;
          pTGraph_E_Alpha_Sa_U      ->SetPoint( pTGraph_E_Alpha_Sa_U ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sa_U )  ;
          pTGraph_E_Alpha_Sa_D      ->SetPoint( pTGraph_E_Alpha_Sa_D ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sa_D )  ;
          
          pTGraph_E_Alpha_Sb        ->SetPoint( pTGraph_E_Alpha_Sb   ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sb   )  ;
          pTGraph_E_Alpha_Sb_U      ->SetPoint( pTGraph_E_Alpha_Sb_U ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sb_U )  ;
          pTGraph_E_Alpha_Sb_D      ->SetPoint( pTGraph_E_Alpha_Sb_D ->GetN(), YT*1.E2 , 1.E4 * E_Alpha_Sb_D )  ;
          
        }
        
        Alpha_PU_Last  = Alpha_PU  ;
        Alpha_PD_Last  = Alpha_PD  ;
      }

//
      TGraph* pTGraph_D_TauBin_PU = new TGraph ;  pTGraph_D_TauBin_PU ->SetLineColor(2) ; 
      TGraph* pTGraph_D_TauBin_PD = new TGraph ;  pTGraph_D_TauBin_PD ->SetLineColor(4) ; 

      pTMultiGraph_D_TauBin ->Add( pTGraph_D_TauBin_PU ) ;
      pTMultiGraph_D_TauBin ->Add( pTGraph_D_TauBin_PD ) ;

      TGraph* pTGraph_E_TauBin = new TGraph ;  pTGraph_E_TauBin ->SetLineColor(2) ; 
      pTMultiGraph_E_TauBin ->Add( pTGraph_E_TauBin ) ;
      
      pTLegend_ET->AddEntry( pTGraph_E_TauBin  , std::string( WhatRC + WhatDD ).c_str() , "l");

      pTGraph_D_TauBin_PU ->SetLineStyle(1) ;
      pTGraph_D_TauBin_PD ->SetLineStyle(1) ;
      pTGraph_E_TauBin    ->SetLineStyle(1) ;
      if (iRC==1 || iDD==1){
      pTGraph_D_TauBin_PU ->SetLineStyle(2) ;
      pTGraph_D_TauBin_PD ->SetLineStyle(2) ;
      pTGraph_E_TauBin    ->SetLineStyle(2) ;
      }
      
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
          pTGraph_D_TauBin_PU    ->SetPoint( pTGraph_D_TauBin_PU ->GetN(), YT*1.E2  , D_TauBin_PU  ) ;
          pTGraph_D_TauBin_PD    ->SetPoint( pTGraph_D_TauBin_PD ->GetN(), YT*1.E2  , D_TauBin_PD  ) ;
          
          double DeltaTauBin = 1. ;
          double E_TauBin = 
          1./std::sqrt( std::pow( D_TauBin_PU/DeltaTauBin , 2 ) +  std::pow( D_TauBin_PD/DeltaTauBin , 2 ) ) ;
          
          pTGraph_E_TauBin       ->SetPoint( pTGraph_E_TauBin    ->GetN(), YT*1.E2 , 1.E4 * E_TauBin )  ;
        }
        
        TauBin_PU_Last = TauBin_PU ;
        TauBin_PD_Last = TauBin_PD ;
      }
  }
  }

//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTMultiGraph_Alpha->Draw("A");
      pTMultiGraph_Alpha->SetTitle("#alpha VS Track Position");
      pTMultiGraph_Alpha->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_Alpha->GetYaxis()->SetTitle("#alpha (Tbin)");
      pTMultiGraph_Alpha->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_Alpha->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_Alpha->SetMaximum( 1. );
      pTMultiGraph_Alpha->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_A->Draw();       
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_TauBin->Draw("A");
      pTMultiGraph_TauBin->SetTitle("#tau VS Track Position");
      pTMultiGraph_TauBin->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_TauBin->GetYaxis()->SetTitle("#tau");
      pTMultiGraph_TauBin->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_TauBin->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_TauBin->SetMaximum( 38. );
      pTMultiGraph_TauBin->SetMinimum(  0. );
      pTCanvas->Update();
 
  pTCanvas->Print(Str_Out_PDF.c_str());
 

//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_D_Alpha->Draw("A");
      pTMultiGraph_D_Alpha->SetTitle("d#alpha/dY_{T} VS Track Position");
      pTMultiGraph_D_Alpha->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_D_Alpha->GetYaxis()->SetTitle("d#alpha/dY_{T} ( cm^{-1})");
      pTMultiGraph_D_Alpha->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_D_Alpha->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      
      pTCanvas->Update();
 
      pTLegend_A->Draw();       
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_D_TauBin->Draw("A");
      pTMultiGraph_D_TauBin->SetTitle("d#tau/dY_{T} VS Track Position");
      pTMultiGraph_D_TauBin->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_D_TauBin->GetYaxis()->SetTitle("d#tau/dY_{T} (Tbin cm^{-1} )");
      pTMultiGraph_D_TauBin->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_D_TauBin->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
 
//Page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_E_Alpha->Draw("A");
      pTMultiGraph_E_Alpha->SetTitle("Error from Amplitudes VS Track Position");
      pTMultiGraph_E_Alpha->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_Alpha->GetYaxis()->SetTitle("Error from Amplitudes (#mu m)");
      pTMultiGraph_E_Alpha->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_Alpha->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_Alpha->SetMaximum( 500. );
      pTMultiGraph_E_Alpha->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_E->Draw();       
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_E_TauBin->Draw("A");
      pTMultiGraph_E_TauBin->SetTitle("Error from Times VS Track Position");
      pTMultiGraph_E_TauBin->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitle("Error from Times (#mu m)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_TauBin->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_TauBin->SetMaximum( 500. );
      pTMultiGraph_E_TauBin->SetMinimum( 0. );
      pTCanvas->Update();

      pTLegend_ET->Draw();       
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
 
//Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_E_Alpha_2->Draw("A");
      pTMultiGraph_E_Alpha_2->SetTitle("Error from Amplitudes VS Track Position");
      pTMultiGraph_E_Alpha_2->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_Alpha_2->GetYaxis()->SetTitle("Error from Amplitudes (#mu m)");
      pTMultiGraph_E_Alpha_2->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_Alpha_2->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_Alpha_2->SetMaximum( 500. );
      pTMultiGraph_E_Alpha_2->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_E->Draw();       
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
//Page 6
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_EC_Alpha_Compare->Draw("A");
      pTMultiGraph_EC_Alpha_Compare->SetTitle("Error from Amplitudes VS Track Position");
      pTMultiGraph_EC_Alpha_Compare->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_EC_Alpha_Compare->GetYaxis()->SetTitle("Error from Amplitudes (#mu m)");
      pTMultiGraph_EC_Alpha_Compare->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_EC_Alpha_Compare->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_EC_Alpha_Compare->SetMaximum( 500. );
      pTMultiGraph_EC_Alpha_Compare->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_EC_Compare->Draw();       
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
 
//Page 7
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
      pTCanvas->cd( 1 )->SetGrid();
      pTMultiGraph_EC_Alpha->Draw("A");
      pTMultiGraph_EC_Alpha->SetTitle("Error from Amplitudes VS Track Position");
      pTMultiGraph_EC_Alpha->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_EC_Alpha->GetYaxis()->SetTitle("Error from Amplitudes (#mu m)");
      pTMultiGraph_EC_Alpha->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_EC_Alpha->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_EC_Alpha->SetMaximum( 500. );
      pTMultiGraph_EC_Alpha->SetMinimum( 0. );
      
      pTCanvas->Update();
 
      pTLegend_EC->Draw();       
      pTCanvas->Update();


    pTCanvas->cd(2) ;
      pTMultiGraph_E_TauBin->Draw("A");
      pTMultiGraph_E_TauBin->SetTitle("Error from Times VS Track Position");
      pTMultiGraph_E_TauBin->GetXaxis()->SetTitle("Y_{T}-Y_{pad}^{Shoot} (cm)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitle("Error from Times (#mu m)");
      pTMultiGraph_E_TauBin->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_E_TauBin->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
      pTMultiGraph_E_TauBin->SetMaximum( 500. );
      pTMultiGraph_E_TauBin->SetMinimum( 0. );
      pTCanvas->Update();

      pTLegend_ET->Draw();       
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
 
 
//Page 8
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
