#include "Studies/Spread_03.h"

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
#include "TArrow.h"
#include "TRandom3.h"

void Spread_03()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PLF     = "OUT_Studies/Spread_03.pdf" ; 
  std::string Str_Out_PLF_Beg = Str_Out_PLF + "(" ;
  std::string Str_Out_PLF_End = Str_Out_PLF + ")" ;

// 
  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 

//
  double Conv_MicroSec_2_TimeBin = 1000./40. ;

  std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;
  SC_VS_x_rc_dd aSC_VS_x_rc_dd(FILE_SC_VS_x_rc_dd,Conv_MicroSec_2_TimeBin) ;
    
//  
  double Y_PR =  Ly ;
  double Y_PC =  0  ;
  double Y_PL = -Ly ;
  
//
  std::vector < TGraph* > V_pTGraph_Alpha_PR  ;
  std::vector < TGraph* > V_pTGraph_Alpha_PL  ;
  std::vector < TGraph* > V_pTGraph_TauBin_PR ;
  std::vector < TGraph* > V_pTGraph_TauBin_PL ;
  
  std::vector < TGraph* > V_pTGraph_Amplitude_PR ;
  std::vector < TGraph* > V_pTGraph_Amplitude_PC ;
  std::vector < TGraph* > V_pTGraph_Amplitude_PL ;

  std::vector < TGraph* > V_pTGraph_D_Alpha_PR  ;
  std::vector < TGraph* > V_pTGraph_D_Alpha_PL  ;
  std::vector < TGraph* > V_pTGraph_D_TauBin_PR ;
  std::vector < TGraph* > V_pTGraph_D_TauBin_PL ;

  std::vector < TGraph* > V_pTGraph_E_Alpha_PR  ;
  std::vector < TGraph* > V_pTGraph_E_Alpha_PL  ;
  std::vector < TGraph* > V_pTGraph_E_TauBin_PR ;
  std::vector < TGraph* > V_pTGraph_E_TauBin_PL ;

  std::vector < TGraph* > V_pTGraph_EC_Alpha       ;
  std::vector < TGraph* > V_pTGraph_EC_TauBin      ;
  std::vector < TGraph* > V_pTGraph_EC_AlphaTauBin ;


  std::vector < double > V_EC_Alpha_Cb     ;
  std::vector < double > V_EC_Alpha_Cb_Max ;
  std::vector < double > V_EC_Alpha_Cb_Min ;
  std::vector < double > V_EC_TauBin_Cb     ;
  std::vector < double > V_EC_TauBin_Cb_Max ;
  std::vector < double > V_EC_TauBin_Cb_Min ;
  std::vector < double > V_EC_AlphaTauBin_Cb     ;
  std::vector < double > V_EC_AlphaTauBin_Cb_Max ;
  std::vector < double > V_EC_AlphaTauBin_Cb_Min ;
  
//
  double DDcur = 0. ;

  for (int iRC = 0 ; iRC< 3 ; iRC++){
    double RCcur = 0. ;
    std::string WhatRC = "BIDON" ;  
    if (iRC==0) { RCcur =  50. ; WhatRC = "RC= 50" ; }
    if (iRC==1) { RCcur = 200. ; WhatRC = "RC=200" ; }
    if (iRC==2) { RCcur = 100. ; WhatRC = "RC=100" ; }

//      
    std::string Comment = WhatRC ;
    
//
    TGraph* pTGraph_Alpha_PR  = new TGraph ;  pTGraph_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_Alpha_PL  = new TGraph ;  pTGraph_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_TauBin_PR = new TGraph ;  pTGraph_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_TauBin_PL = new TGraph ;  pTGraph_TauBin_PL ->SetLineColor(4) ; 
    
    pTGraph_Alpha_PR ->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_Alpha_PL ->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_TauBin_PR->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_TauBin_PL->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    
    pTGraph_Alpha_PR  ->SetLineStyle(1) ;
    pTGraph_Alpha_PL  ->SetLineStyle(1) ;
    pTGraph_TauBin_PR ->SetLineStyle(1) ;
    pTGraph_TauBin_PL ->SetLineStyle(1) ;

    V_pTGraph_Alpha_PR .push_back( pTGraph_Alpha_PR  ) ;
    V_pTGraph_Alpha_PL .push_back( pTGraph_Alpha_PL  ) ;
    V_pTGraph_TauBin_PR.push_back( pTGraph_TauBin_PR ) ;
    V_pTGraph_TauBin_PL.push_back( pTGraph_TauBin_PL ) ;

//
    TGraph* pTGraph_Amplitude_PR = new TGraph ;  pTGraph_Amplitude_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_Amplitude_PC = new TGraph ;  pTGraph_Amplitude_PC ->SetLineColor(1) ; 
    TGraph* pTGraph_Amplitude_PL = new TGraph ;  pTGraph_Amplitude_PL ->SetLineColor(4) ; 

    pTGraph_Amplitude_PR ->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_Amplitude_PC ->SetTitle( std::string( "C Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_Amplitude_PL ->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;

    pTGraph_Amplitude_PR ->SetLineStyle(1) ;
    pTGraph_Amplitude_PC ->SetLineStyle(1) ;
    pTGraph_Amplitude_PL ->SetLineStyle(1) ;
    
    V_pTGraph_Amplitude_PR.push_back( pTGraph_Amplitude_PR ) ;
    V_pTGraph_Amplitude_PC.push_back( pTGraph_Amplitude_PC ) ;
    V_pTGraph_Amplitude_PL.push_back( pTGraph_Amplitude_PL ) ;

//
    TGraph* pTGraph_D_Alpha_PR  = new TGraph ;  pTGraph_D_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_D_Alpha_PL  = new TGraph ;  pTGraph_D_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_D_TauBin_PR = new TGraph ;  pTGraph_D_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_D_TauBin_PL = new TGraph ;  pTGraph_D_TauBin_PL ->SetLineColor(4) ; 
    
    pTGraph_D_Alpha_PR ->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_D_Alpha_PL ->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_D_TauBin_PR->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_D_TauBin_PL->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;

    V_pTGraph_D_Alpha_PR .push_back( pTGraph_D_Alpha_PR  ) ;
    V_pTGraph_D_Alpha_PL .push_back( pTGraph_D_Alpha_PL  ) ;
    V_pTGraph_D_TauBin_PR.push_back( pTGraph_D_TauBin_PR ) ;
    V_pTGraph_D_TauBin_PL.push_back( pTGraph_D_TauBin_PL ) ;

//
    TGraph* pTGraph_E_Alpha_PR  = new TGraph ;  pTGraph_E_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_E_Alpha_PL  = new TGraph ;  pTGraph_E_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_E_TauBin_PR = new TGraph ;  pTGraph_E_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_E_TauBin_PL = new TGraph ;  pTGraph_E_TauBin_PL ->SetLineColor(4) ; 
    
    pTGraph_E_Alpha_PR ->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_E_Alpha_PL ->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_E_TauBin_PR->SetTitle( std::string( "R Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_E_TauBin_PL->SetTitle( std::string( "L Pad, " + WhatRC + " ns mm^{-2} " ).c_str() ) ;

    V_pTGraph_E_Alpha_PR .push_back( pTGraph_E_Alpha_PR  ) ;
    V_pTGraph_E_Alpha_PL .push_back( pTGraph_E_Alpha_PL  ) ;
    V_pTGraph_E_TauBin_PR.push_back( pTGraph_E_TauBin_PR ) ;
    V_pTGraph_E_TauBin_PL.push_back( pTGraph_E_TauBin_PL ) ;

//
    TGraph* pTGraph_EC_Alpha       = new TGraph ;  pTGraph_EC_Alpha       ->SetLineColor(1) ;  
    TGraph* pTGraph_EC_TauBin      = new TGraph ;  pTGraph_EC_TauBin      ->SetLineColor(1) ; 
    TGraph* pTGraph_EC_AlphaTauBin = new TGraph ;  pTGraph_EC_AlphaTauBin ->SetLineColor(1) ; 
    
    pTGraph_EC_Alpha      ->SetTitle( std::string( " L+R (#alpha) " + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_EC_TauBin     ->SetTitle( std::string( " L+R (#tau) "   + WhatRC + " ns mm^{-2} " ).c_str() ) ;
    pTGraph_EC_AlphaTauBin->SetTitle( std::string( "#alpha+#tau "   + WhatRC + " ns mm^{-2} " ).c_str() ) ;

    V_pTGraph_EC_Alpha      .push_back( pTGraph_EC_Alpha       ) ;
    V_pTGraph_EC_TauBin     .push_back( pTGraph_EC_TauBin      ) ;
    V_pTGraph_EC_AlphaTauBin.push_back( pTGraph_EC_AlphaTauBin ) ;

//
    double YT_Min =-0.9999999*Ly/2. ;
    double YT_Max = 0.9999999*Ly/2. ;
    int iYT_Max = 51 ;
    for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
      double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
//      
      double YTY_PR = YT-Y_PR ;
      double YTY_PC = YT-Y_PC ;
      double YTY_PL = YT-Y_PL ;
 
      double Alpha_PR  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PR*1.E2, RCcur,DDcur) ;
      double Alpha_PL  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PL*1.E2, RCcur,DDcur) ;
      double TauBin_PR = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PR*1.E2, RCcur,DDcur) ;
      double TauBin_PL = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PL*1.E2, RCcur,DDcur) ;

      pTGraph_Alpha_PR     ->SetPoint( pTGraph_Alpha_PR  ->GetN(), YT*1.E2  , Alpha_PR   ) ;
      pTGraph_Alpha_PL     ->SetPoint( pTGraph_Alpha_PL  ->GetN(), YT*1.E2  , Alpha_PL   ) ;
      pTGraph_TauBin_PR    ->SetPoint( pTGraph_TauBin_PR ->GetN(), YT*1.E2  , TauBin_PR  ) ;
      pTGraph_TauBin_PL    ->SetPoint( pTGraph_TauBin_PL ->GetN(), YT*1.E2  , TauBin_PL  ) ;
      
//
      double Amplitude_PR = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PR*1.E2, RCcur,DDcur) ;
      double Amplitude_PC = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PC*1.E2, RCcur,DDcur) ;
      double Amplitude_PL = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PL*1.E2, RCcur,DDcur) ;
      
      pTGraph_Amplitude_PR ->SetPoint( pTGraph_Amplitude_PR ->GetN(), YT*1.E2  , Amplitude_PR  ) ;
      pTGraph_Amplitude_PC ->SetPoint( pTGraph_Amplitude_PC ->GetN(), YT*1.E2  , Amplitude_PC  ) ;
      pTGraph_Amplitude_PL ->SetPoint( pTGraph_Amplitude_PL ->GetN(), YT*1.E2  , Amplitude_PL  ) ;

    }

    double* X_pTGraph_Alpha_PR  = pTGraph_Alpha_PR ->GetX() ;
    
    double* Y_pTGraph_Alpha_PR  = pTGraph_Alpha_PR ->GetY() ;
    double* Y_pTGraph_Alpha_PL  = pTGraph_Alpha_PL ->GetY() ;
    double* Y_pTGraph_TauBin_PR = pTGraph_TauBin_PR->GetY() ;
    double* Y_pTGraph_TauBin_PL = pTGraph_TauBin_PL->GetY() ;

    double* Y_pTGraph_Amplitude_PC = pTGraph_Amplitude_PC->GetY() ;

    double EC_Alpha_Cb            = 0. ;
    double EC_Alpha_Cb_Max        = 0. ;
    double EC_Alpha_Cb_Min        = 0. ;
    double EC_TauBin_Cb           = 0. ;
    double EC_TauBin_Cb_Max       = 0. ;
    double EC_TauBin_Cb_Min       = 0. ;
    double EC_AlphaTauBin_Cb      = 0. ;
    double EC_AlphaTauBin_Cb_Max  = 0. ;
    double EC_AlphaTauBin_Cb_Min  = 0. ;
    
    for (int iYT = 1 ; iYT< iYT_Max-1 ; iYT++){
      double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
      double Delta_pTGraph_Alpha_PR  = Y_pTGraph_Alpha_PR [ iYT + 1 ] - Y_pTGraph_Alpha_PR [ iYT - 1 ] ;
      double Delta_pTGraph_Alpha_PL  = Y_pTGraph_Alpha_PL [ iYT + 1 ] - Y_pTGraph_Alpha_PL [ iYT - 1 ] ;
      double Delta_pTGraph_TauBin_PR = Y_pTGraph_TauBin_PR[ iYT + 1 ] - Y_pTGraph_TauBin_PR[ iYT - 1 ] ;
      double Delta_pTGraph_TauBin_PL = Y_pTGraph_TauBin_PL[ iYT + 1 ] - Y_pTGraph_TauBin_PL[ iYT - 1 ] ;
      
      double Delta_X = X_pTGraph_Alpha_PR [ iYT + 1 ] - X_pTGraph_Alpha_PR [ iYT - 1 ] ;
      
      double D_pTGraph_Alpha_PR  = Delta_pTGraph_Alpha_PR  / Delta_X ;
      double D_pTGraph_Alpha_PL  = Delta_pTGraph_Alpha_PL  / Delta_X ;
      double D_pTGraph_TauBin_PR = Delta_pTGraph_TauBin_PR / Delta_X ;
      double D_pTGraph_TauBin_PL = Delta_pTGraph_TauBin_PL / Delta_X ;

      pTGraph_D_Alpha_PR      ->SetPoint( pTGraph_D_Alpha_PR   ->GetN(), YT*1.E2 , D_pTGraph_Alpha_PR  ) ;
      pTGraph_D_Alpha_PL      ->SetPoint( pTGraph_D_Alpha_PL   ->GetN(), YT*1.E2 , D_pTGraph_Alpha_PL  ) ;
      pTGraph_D_TauBin_PR     ->SetPoint( pTGraph_D_TauBin_PR  ->GetN(), YT*1.E2 , D_pTGraph_TauBin_PR ) ;
      pTGraph_D_TauBin_PL     ->SetPoint( pTGraph_D_TauBin_PL  ->GetN(), YT*1.E2 , D_pTGraph_TauBin_PL ) ;
      
//      
      double A_PC = Y_pTGraph_Amplitude_PC[iYT] ;
      double A_PR = Y_pTGraph_Alpha_PR [iYT ] * A_PC ;
      double A_PL = Y_pTGraph_Alpha_PL [iYT ] * A_PC ;
      
      double E_A_PC = sqrt( A_PC ) ;
      double E_A_PR = sqrt( A_PR ) ;
      double E_A_PL = sqrt( A_PL ) ;
      
      double Delta_Alpha_PR = (A_PR/A_PC) *  std::sqrt( std::pow(E_A_PC/A_PC,2) + std::pow(E_A_PR/A_PR,2) ) ;
      double Delta_Alpha_PL = (A_PL/A_PC) *  std::sqrt( std::pow(E_A_PC/A_PC,2) + std::pow(E_A_PL/A_PL,2) ) ;

      double E_pTGraph_Alpha_PR  = Delta_Alpha_PR * 1.E4/std::fabs(D_pTGraph_Alpha_PR)  ;
      double E_pTGraph_Alpha_PL  = Delta_Alpha_PL * 1.E4/std::fabs(D_pTGraph_Alpha_PL)  ;
      
      double E_Time =  1. ;
      double E_Tau  =  std::sqrt(2.*E_Time) ;
      double E_pTGraph_TauBin_PR = E_Tau          * 1.E4/std::fabs(D_pTGraph_TauBin_PR) ;
      double E_pTGraph_TauBin_PL = E_Tau          * 1.E4/std::fabs(D_pTGraph_TauBin_PL) ;
      
      pTGraph_E_Alpha_PR      ->SetPoint( pTGraph_E_Alpha_PR   ->GetN(), YT*1.E2 , E_pTGraph_Alpha_PR  ) ;
      pTGraph_E_Alpha_PL      ->SetPoint( pTGraph_E_Alpha_PL   ->GetN(), YT*1.E2 , E_pTGraph_Alpha_PL  ) ;
      pTGraph_E_TauBin_PR     ->SetPoint( pTGraph_E_TauBin_PR  ->GetN(), YT*1.E2 , E_pTGraph_TauBin_PR ) ;
      pTGraph_E_TauBin_PL     ->SetPoint( pTGraph_E_TauBin_PL  ->GetN(), YT*1.E2 , E_pTGraph_TauBin_PL ) ;

//
      double EC_pTGraph_Alpha = 
      std::sqrt( 
         std::pow(1./E_pTGraph_Alpha_PR,2) 
       + std::pow(1./E_pTGraph_Alpha_PL,2) 
      )  ;
      EC_pTGraph_Alpha = 1./EC_pTGraph_Alpha ;
      
      pTGraph_EC_Alpha ->SetPoint( pTGraph_EC_Alpha ->GetN(), YT*1.E2 , EC_pTGraph_Alpha  ) ;
      EC_Alpha_Cb += EC_pTGraph_Alpha ;
      
//
      double EC_pTGraph_TauBin = 
      std::sqrt( 
         std::pow(1./E_pTGraph_TauBin_PR,2) 
       + std::pow(1./E_pTGraph_TauBin_PL,2) 
      )  ;
      EC_pTGraph_TauBin = 1./EC_pTGraph_TauBin ;
      
      pTGraph_EC_TauBin ->SetPoint( pTGraph_EC_TauBin ->GetN(), YT*1.E2 , EC_pTGraph_TauBin  ) ;
      EC_TauBin_Cb += EC_pTGraph_TauBin ;
      
//
      double EC_pTGraph_AlphaTauBin = 
      std::sqrt( 
         std::pow(1./E_pTGraph_Alpha_PR,2) 
       + std::pow(1./E_pTGraph_Alpha_PL,2) 
       + std::pow(1./E_pTGraph_TauBin_PR,2) 
       + std::pow(1./E_pTGraph_TauBin_PL,2) 
      )  ;
      EC_pTGraph_AlphaTauBin = 1./EC_pTGraph_AlphaTauBin ;

      pTGraph_EC_AlphaTauBin ->SetPoint( pTGraph_EC_AlphaTauBin ->GetN(), YT*1.E2 , EC_pTGraph_AlphaTauBin  ) ;
      EC_AlphaTauBin_Cb += EC_pTGraph_AlphaTauBin ;
      
//
      if (iYT==1){
        EC_Alpha_Cb_Max       = EC_pTGraph_Alpha       ;
        EC_Alpha_Cb_Min       = EC_pTGraph_Alpha       ;
        EC_TauBin_Cb_Max      = EC_pTGraph_TauBin      ;
        EC_TauBin_Cb_Min      = EC_pTGraph_TauBin      ;
        EC_AlphaTauBin_Cb_Max = EC_pTGraph_AlphaTauBin ;
        EC_AlphaTauBin_Cb_Min = EC_pTGraph_AlphaTauBin ;
      }
      if ( EC_Alpha_Cb_Max       < EC_pTGraph_Alpha       ) EC_Alpha_Cb_Max       = EC_pTGraph_Alpha       ;
      if ( EC_Alpha_Cb_Min       > EC_pTGraph_Alpha       ) EC_Alpha_Cb_Min       = EC_pTGraph_Alpha       ;
      if ( EC_TauBin_Cb_Max      < EC_pTGraph_TauBin      ) EC_TauBin_Cb_Max      = EC_pTGraph_TauBin      ;
      if ( EC_TauBin_Cb_Min      > EC_pTGraph_TauBin      ) EC_TauBin_Cb_Min      = EC_pTGraph_TauBin      ;
      if ( EC_AlphaTauBin_Cb_Max < EC_pTGraph_AlphaTauBin ) EC_AlphaTauBin_Cb_Max = EC_pTGraph_AlphaTauBin ;
      if ( EC_AlphaTauBin_Cb_Min > EC_pTGraph_AlphaTauBin ) EC_AlphaTauBin_Cb_Min = EC_pTGraph_AlphaTauBin ;

    }
    EC_Alpha_Cb       = EC_Alpha_Cb      /double(pTGraph_EC_Alpha       ->GetN()) ;
    EC_TauBin_Cb      = EC_TauBin_Cb     /double(pTGraph_EC_TauBin      ->GetN()) ;
    EC_AlphaTauBin_Cb = EC_AlphaTauBin_Cb/double(pTGraph_EC_AlphaTauBin ->GetN()) ;
    
//
    V_EC_Alpha_Cb          .push_back(EC_Alpha_Cb          ) ;
    V_EC_Alpha_Cb_Max      .push_back(EC_Alpha_Cb_Max      ) ;
    V_EC_Alpha_Cb_Min      .push_back(EC_Alpha_Cb_Min      ) ;
    V_EC_TauBin_Cb         .push_back(EC_TauBin_Cb         ) ;
    V_EC_TauBin_Cb_Max     .push_back(EC_TauBin_Cb_Max     ) ;
    V_EC_TauBin_Cb_Min     .push_back(EC_TauBin_Cb_Min     ) ;
    V_EC_AlphaTauBin_Cb    .push_back(EC_AlphaTauBin_Cb    ) ;
    V_EC_AlphaTauBin_Cb_Max.push_back(EC_AlphaTauBin_Cb_Max) ;
    V_EC_AlphaTauBin_Cb_Min.push_back(EC_AlphaTauBin_Cb_Min) ;
    
  }
  
//
  TGraph* pTGraph_Alpha_EC_Cb     = new TGraph ;  
          pTGraph_Alpha_EC_Cb     ->SetLineColor(2) ;  
          pTGraph_Alpha_EC_Cb     ->SetLineStyle(1) ;  
          pTGraph_Alpha_EC_Cb     ->SetMarkerStyle(20) ;
          pTGraph_Alpha_EC_Cb     ->SetMarkerColor(2) ;
          pTGraph_Alpha_EC_Cb     ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_Alpha_EC_Cb_Max = new TGraph ;  
          pTGraph_Alpha_EC_Cb_Max ->SetLineColor(2) ;  
          pTGraph_Alpha_EC_Cb_Max ->SetLineStyle(2) ;  
          pTGraph_Alpha_EC_Cb_Max ->SetMarkerStyle(20) ;
          pTGraph_Alpha_EC_Cb_Max ->SetMarkerColor(2) ;
          pTGraph_Alpha_EC_Cb_Max ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_Alpha_EC_Cb_Min = new TGraph ;  
          pTGraph_Alpha_EC_Cb_Min ->SetLineColor(2) ;  
          pTGraph_Alpha_EC_Cb_Min ->SetLineStyle(2) ;  
          pTGraph_Alpha_EC_Cb_Min ->SetMarkerStyle(20) ;
          pTGraph_Alpha_EC_Cb_Min ->SetMarkerColor(2) ;
          pTGraph_Alpha_EC_Cb_Min ->SetMarkerSize(1.8) ;

  pTGraph_Alpha_EC_Cb     ->SetTitle( std::string( "Averaged" ).c_str() ) ;
  pTGraph_Alpha_EC_Cb_Max ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;
  pTGraph_Alpha_EC_Cb_Min ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;

  TGraph* pTGraph_TauBin_EC_Cb     = new TGraph ;  
          pTGraph_TauBin_EC_Cb     ->SetLineColor(2) ;  
          pTGraph_TauBin_EC_Cb     ->SetLineStyle(1) ;  
          pTGraph_TauBin_EC_Cb     ->SetMarkerStyle(20) ;
          pTGraph_TauBin_EC_Cb     ->SetMarkerColor(2) ;
          pTGraph_TauBin_EC_Cb     ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_TauBin_EC_Cb_Max = new TGraph ;  
          pTGraph_TauBin_EC_Cb_Max ->SetLineColor(2) ;  
          pTGraph_TauBin_EC_Cb_Max ->SetLineStyle(2) ;  
          pTGraph_TauBin_EC_Cb_Max ->SetMarkerStyle(20) ;
          pTGraph_TauBin_EC_Cb_Max ->SetMarkerColor(2) ;
          pTGraph_TauBin_EC_Cb_Max ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_TauBin_EC_Cb_Min = new TGraph ;  
          pTGraph_TauBin_EC_Cb_Min ->SetLineColor(2) ;  
          pTGraph_TauBin_EC_Cb_Min ->SetLineStyle(2) ;  
          pTGraph_TauBin_EC_Cb_Min ->SetMarkerStyle(20) ;
          pTGraph_TauBin_EC_Cb_Min ->SetMarkerColor(2) ;
          pTGraph_TauBin_EC_Cb_Min ->SetMarkerSize(1.8) ;

  pTGraph_TauBin_EC_Cb     ->SetTitle( std::string( "Averaged" ).c_str() ) ;
  pTGraph_TauBin_EC_Cb_Max ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;
  pTGraph_TauBin_EC_Cb_Min ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;
  
  TGraph* pTGraph_AlphaTauBin_EC_Cb     = new TGraph ;  
          pTGraph_AlphaTauBin_EC_Cb     ->SetLineColor(2) ;  
          pTGraph_AlphaTauBin_EC_Cb     ->SetLineStyle(1) ;  
          pTGraph_AlphaTauBin_EC_Cb     ->SetMarkerStyle(20) ;
          pTGraph_AlphaTauBin_EC_Cb     ->SetMarkerColor(2) ;
          pTGraph_AlphaTauBin_EC_Cb     ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_AlphaTauBin_EC_Cb_Max = new TGraph ;  
          pTGraph_AlphaTauBin_EC_Cb_Max ->SetLineColor(2) ;  
          pTGraph_AlphaTauBin_EC_Cb_Max ->SetLineStyle(2) ;  
          pTGraph_AlphaTauBin_EC_Cb_Max ->SetMarkerStyle(20) ;
          pTGraph_AlphaTauBin_EC_Cb_Max ->SetMarkerColor(2) ;
          pTGraph_AlphaTauBin_EC_Cb_Max ->SetMarkerSize(1.8) ;
  TGraph* pTGraph_AlphaTauBin_EC_Cb_Min = new TGraph ;  
          pTGraph_AlphaTauBin_EC_Cb_Min ->SetLineColor(2) ;  
          pTGraph_AlphaTauBin_EC_Cb_Min ->SetLineStyle(2) ;  
          pTGraph_AlphaTauBin_EC_Cb_Min ->SetMarkerStyle(20) ;
          pTGraph_AlphaTauBin_EC_Cb_Min ->SetMarkerColor(2) ;
          pTGraph_AlphaTauBin_EC_Cb_Min ->SetMarkerSize(1.8) ;

  pTGraph_AlphaTauBin_EC_Cb     ->SetTitle( std::string( "Averaged" ).c_str() ) ;
  pTGraph_AlphaTauBin_EC_Cb_Max ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;
  pTGraph_AlphaTauBin_EC_Cb_Min ->SetTitle( std::string( "Max/Min"  ).c_str() ) ;

  TGraph* pTGraph_Amplitude_LR_CentralTrack = new TGraph ;  
          pTGraph_Amplitude_LR_CentralTrack ->SetLineColor(2) ;  
          pTGraph_Amplitude_LR_CentralTrack ->SetLineStyle(1) ;  
          pTGraph_Amplitude_LR_CentralTrack ->SetMarkerStyle(20) ;
          pTGraph_Amplitude_LR_CentralTrack ->SetMarkerColor(2) ;
          pTGraph_Amplitude_LR_CentralTrack ->SetMarkerSize(1.8) ;

  pTGraph_Amplitude_LR_CentralTrack     ->SetTitle( std::string( "Signal on L/R pads for central track" ).c_str() ) ;
  
  for (int iRC = 0 ; iRC< 16 ; iRC++){
    double RCcur = 0. ;
    if (iRC== 0) { RCcur =  50. ; }
    if (iRC== 1) { RCcur =  60. ; }
    if (iRC== 2) { RCcur =  70. ; }
    if (iRC== 3) { RCcur =  80. ; }
    if (iRC== 4) { RCcur =  90. ; }
    if (iRC== 5) { RCcur = 100. ; }
    if (iRC== 6) { RCcur = 110. ; }
    if (iRC== 7) { RCcur = 120. ; }
    if (iRC== 8) { RCcur = 130. ; }
    if (iRC== 9) { RCcur = 140. ; }
    if (iRC==10) { RCcur = 150. ; }
    if (iRC==11) { RCcur = 160. ; }
    if (iRC==12) { RCcur = 170. ; }
    if (iRC==13) { RCcur = 180. ; }
    if (iRC==14) { RCcur = 190. ; }
    if (iRC==15) { RCcur = 200. ; }

//      
    TGraph* pTGraph_Alpha_PR  = new TGraph ;  pTGraph_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_Alpha_PL  = new TGraph ;  pTGraph_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_TauBin_PR = new TGraph ;  pTGraph_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_TauBin_PL = new TGraph ;  pTGraph_TauBin_PL ->SetLineColor(4) ; 

//
    TGraph* pTGraph_Amplitude_PR = new TGraph ;  pTGraph_Amplitude_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_Amplitude_PC = new TGraph ;  pTGraph_Amplitude_PC ->SetLineColor(1) ; 
    TGraph* pTGraph_Amplitude_PL = new TGraph ;  pTGraph_Amplitude_PL ->SetLineColor(4) ; 

//
    TGraph* pTGraph_D_Alpha_PR  = new TGraph ;  pTGraph_D_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_D_Alpha_PL  = new TGraph ;  pTGraph_D_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_D_TauBin_PR = new TGraph ;  pTGraph_D_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_D_TauBin_PL = new TGraph ;  pTGraph_D_TauBin_PL ->SetLineColor(4) ; 
    
//
    TGraph* pTGraph_E_Alpha_PR  = new TGraph ;  pTGraph_E_Alpha_PR  ->SetLineColor(2) ;  
    TGraph* pTGraph_E_Alpha_PL  = new TGraph ;  pTGraph_E_Alpha_PL  ->SetLineColor(4) ; 
    TGraph* pTGraph_E_TauBin_PR = new TGraph ;  pTGraph_E_TauBin_PR ->SetLineColor(2) ; 
    TGraph* pTGraph_E_TauBin_PL = new TGraph ;  pTGraph_E_TauBin_PL ->SetLineColor(4) ; 
    
//
    TGraph* pTGraph_EC_Alpha       = new TGraph ;  pTGraph_EC_Alpha       ->SetLineColor(1) ;  
    TGraph* pTGraph_EC_TauBin      = new TGraph ;  pTGraph_EC_TauBin      ->SetLineColor(1) ; 
    TGraph* pTGraph_EC_AlphaTauBin = new TGraph ;  pTGraph_EC_AlphaTauBin ->SetLineColor(1) ; 

//
    double YT_Min =-0.9999999*Ly/2. ;
    double YT_Max = 0.9999999*Ly/2. ;
    int iYT_Max = 51 ;
    for (int iYT = 0 ; iYT< iYT_Max ; iYT++){
      double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
//
      double YTY_PR = YT-Y_PR ;
      double YTY_PC = YT-Y_PC ;
      double YTY_PL = YT-Y_PL ;
 
      double Alpha_PR  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PR*1.E2, RCcur,DDcur) ;
      double Alpha_PL  = aSC_VS_x_rc_dd.Eval_AlphaPRF(YTY_PL*1.E2, RCcur,DDcur) ;
      double TauBin_PR = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PR*1.E2, RCcur,DDcur) ;
      double TauBin_PL = aSC_VS_x_rc_dd.Eval_TauBinPRF(YTY_PL*1.E2, RCcur,DDcur) ;

      pTGraph_Alpha_PR     ->SetPoint( pTGraph_Alpha_PR  ->GetN(), YT*1.E2  , Alpha_PR   ) ;
      pTGraph_Alpha_PL     ->SetPoint( pTGraph_Alpha_PL  ->GetN(), YT*1.E2  , Alpha_PL   ) ;
      pTGraph_TauBin_PR    ->SetPoint( pTGraph_TauBin_PR ->GetN(), YT*1.E2  , TauBin_PR  ) ;
      pTGraph_TauBin_PL    ->SetPoint( pTGraph_TauBin_PL ->GetN(), YT*1.E2  , TauBin_PL  ) ;
      
//
      double Amplitude_PR = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PR*1.E2, RCcur,DDcur) ;
      double Amplitude_PC = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PC*1.E2, RCcur,DDcur) ;
      double Amplitude_PL = aSC_VS_x_rc_dd.Eval_Amplitude(YTY_PL*1.E2, RCcur,DDcur) ;
      
      pTGraph_Amplitude_PR ->SetPoint( pTGraph_Amplitude_PR ->GetN(), YT*1.E2  , Amplitude_PR  ) ;
      pTGraph_Amplitude_PC ->SetPoint( pTGraph_Amplitude_PC ->GetN(), YT*1.E2  , Amplitude_PC  ) ;
      pTGraph_Amplitude_PL ->SetPoint( pTGraph_Amplitude_PL ->GetN(), YT*1.E2  , Amplitude_PL  ) ;

    }

    double* X_pTGraph_Alpha_PR  = pTGraph_Alpha_PR ->GetX() ;
    
    double* Y_pTGraph_Alpha_PR  = pTGraph_Alpha_PR ->GetY() ;
    double* Y_pTGraph_Alpha_PL  = pTGraph_Alpha_PL ->GetY();
    double* Y_pTGraph_TauBin_PR = pTGraph_TauBin_PR->GetY() ;
    double* Y_pTGraph_TauBin_PL = pTGraph_TauBin_PL->GetY() ;

    double* Y_pTGraph_Amplitude_PC = pTGraph_Amplitude_PC->GetY() ;

    double EC_Alpha_Cb            = 0. ;
    double EC_Alpha_Cb_Max        = 0. ;
    double EC_Alpha_Cb_Min        = 0. ;
    double EC_TauBin_Cb           = 0. ;
    double EC_TauBin_Cb_Max       = 0. ;
    double EC_TauBin_Cb_Min       = 0. ;
    double EC_AlphaTauBin_Cb      = 0. ;
    double EC_AlphaTauBin_Cb_Max  = 0. ;
    double EC_AlphaTauBin_Cb_Min  = 0. ;
     
    for (int iYT = 1 ; iYT< iYT_Max-1 ; iYT++){
      double YT = YT_Min + double(iYT)*(YT_Max-YT_Min)/double(iYT_Max-1) ;
      double Delta_pTGraph_Alpha_PR  = Y_pTGraph_Alpha_PR [ iYT + 1 ] - Y_pTGraph_Alpha_PR [ iYT - 1 ] ;
      double Delta_pTGraph_Alpha_PL  = Y_pTGraph_Alpha_PL [ iYT + 1 ] - Y_pTGraph_Alpha_PL [ iYT - 1 ] ;
      double Delta_pTGraph_TauBin_PR = Y_pTGraph_TauBin_PR[ iYT + 1 ] - Y_pTGraph_TauBin_PR[ iYT - 1 ] ;
      double Delta_pTGraph_TauBin_PL = Y_pTGraph_TauBin_PL[ iYT + 1 ] - Y_pTGraph_TauBin_PL[ iYT - 1 ] ;
      
      double Delta_X = X_pTGraph_Alpha_PR [ iYT + 1 ] - X_pTGraph_Alpha_PR [ iYT - 1 ] ;
      
      double D_pTGraph_Alpha_PR  = Delta_pTGraph_Alpha_PR  / Delta_X ;
      double D_pTGraph_Alpha_PL  = Delta_pTGraph_Alpha_PL  / Delta_X ;
      double D_pTGraph_TauBin_PR = Delta_pTGraph_TauBin_PR / Delta_X ;
      double D_pTGraph_TauBin_PL = Delta_pTGraph_TauBin_PL / Delta_X ;

      pTGraph_D_Alpha_PR      ->SetPoint( pTGraph_D_Alpha_PR   ->GetN(), YT*1.E2 , D_pTGraph_Alpha_PR  ) ;
      pTGraph_D_Alpha_PL      ->SetPoint( pTGraph_D_Alpha_PL   ->GetN(), YT*1.E2 , D_pTGraph_Alpha_PL  ) ;
      pTGraph_D_TauBin_PR     ->SetPoint( pTGraph_D_TauBin_PR  ->GetN(), YT*1.E2 , D_pTGraph_TauBin_PR ) ;
      pTGraph_D_TauBin_PL     ->SetPoint( pTGraph_D_TauBin_PL  ->GetN(), YT*1.E2 , D_pTGraph_TauBin_PL ) ;
      
//      
      double A_PC = Y_pTGraph_Amplitude_PC[iYT] ;
      double A_PR = Y_pTGraph_Alpha_PR [iYT ] * A_PC ;
      double A_PL = Y_pTGraph_Alpha_PL [iYT ] * A_PC ;
      
      if (iYT==( (iYT_Max+1)/2 ) )
      pTGraph_Amplitude_LR_CentralTrack     ->SetPoint( pTGraph_Amplitude_LR_CentralTrack  ->GetN(), RCcur , A_PR ) ;

      double E_A_PC = sqrt( A_PC ) ;
      double E_A_PR = sqrt( A_PR ) ;
      double E_A_PL = sqrt( A_PL ) ;
      
      double Delta_Alpha_PR = (A_PR/A_PC) *  std::sqrt( std::pow(E_A_PC/A_PC,2) + std::pow(E_A_PR/A_PR,2) ) ;
      double Delta_Alpha_PL = (A_PL/A_PC) *  std::sqrt( std::pow(E_A_PC/A_PC,2) + std::pow(E_A_PL/A_PL,2) ) ;

      double E_pTGraph_Alpha_PR  = Delta_Alpha_PR * 1.E4/std::fabs(D_pTGraph_Alpha_PR)  ;
      double E_pTGraph_Alpha_PL  = Delta_Alpha_PL * 1.E4/std::fabs(D_pTGraph_Alpha_PL)  ;
      
      double E_Time =  1. ;
      double E_Tau  =  std::sqrt(2.*E_Time) ;
      double E_pTGraph_TauBin_PR = E_Tau          * 1.E4/std::fabs(D_pTGraph_TauBin_PR) ;
      double E_pTGraph_TauBin_PL = E_Tau          * 1.E4/std::fabs(D_pTGraph_TauBin_PL) ;
       
      pTGraph_E_Alpha_PR      ->SetPoint( pTGraph_E_Alpha_PR   ->GetN(), YT*1.E2 , E_pTGraph_Alpha_PR  ) ;
      pTGraph_E_Alpha_PL      ->SetPoint( pTGraph_E_Alpha_PL   ->GetN(), YT*1.E2 , E_pTGraph_Alpha_PL  ) ;
      pTGraph_E_TauBin_PR     ->SetPoint( pTGraph_E_TauBin_PR  ->GetN(), YT*1.E2 , E_pTGraph_TauBin_PR ) ;
      pTGraph_E_TauBin_PL     ->SetPoint( pTGraph_E_TauBin_PL  ->GetN(), YT*1.E2 , E_pTGraph_TauBin_PL ) ;

//
      double EC_pTGraph_Alpha = 
      std::sqrt( 
          std::pow(1./E_pTGraph_Alpha_PR,2) 
        + std::pow(1./E_pTGraph_Alpha_PL,2) 
      )  ;
      EC_pTGraph_Alpha = 1./EC_pTGraph_Alpha ;
      
      pTGraph_EC_Alpha ->SetPoint( pTGraph_EC_Alpha ->GetN(), YT*1.E2 , EC_pTGraph_Alpha  ) ;
      EC_Alpha_Cb += EC_pTGraph_Alpha ;
      
//
      double EC_pTGraph_TauBin = 
      std::sqrt( 
          std::pow(1./E_pTGraph_TauBin_PR,2) 
        + std::pow(1./E_pTGraph_TauBin_PL,2) 
        
      )  ;
      EC_pTGraph_TauBin = 1./EC_pTGraph_TauBin ;
      
      pTGraph_EC_TauBin ->SetPoint( pTGraph_EC_TauBin ->GetN(), YT*1.E2 , EC_pTGraph_TauBin  ) ;
      EC_TauBin_Cb += EC_pTGraph_TauBin ;
      
//
      double EC_pTGraph_AlphaTauBin = 
      std::sqrt( 
         std::pow(1./E_pTGraph_Alpha_PR,2) 
       + std::pow(1./E_pTGraph_Alpha_PL,2) 
       + std::pow(1./E_pTGraph_TauBin_PR,2) 
       + std::pow(1./E_pTGraph_TauBin_PL,2) 
      )  ;
      EC_pTGraph_AlphaTauBin = 1./EC_pTGraph_AlphaTauBin ;

      pTGraph_EC_AlphaTauBin ->SetPoint( pTGraph_EC_AlphaTauBin ->GetN(), YT*1.E2 , EC_pTGraph_AlphaTauBin  ) ;
      EC_AlphaTauBin_Cb += EC_pTGraph_AlphaTauBin ;
      
//
      if (iYT==1){
        EC_Alpha_Cb_Max       = EC_pTGraph_Alpha       ;
        EC_Alpha_Cb_Min       = EC_pTGraph_Alpha       ;
        EC_TauBin_Cb_Max      = EC_pTGraph_TauBin      ;
        EC_TauBin_Cb_Min      = EC_pTGraph_TauBin      ;
        EC_AlphaTauBin_Cb_Max = EC_pTGraph_AlphaTauBin ;
        EC_AlphaTauBin_Cb_Min = EC_pTGraph_AlphaTauBin ;
      }
      if ( EC_Alpha_Cb_Max       < EC_pTGraph_Alpha       ) EC_Alpha_Cb_Max       = EC_pTGraph_Alpha       ;
      if ( EC_Alpha_Cb_Min       > EC_pTGraph_Alpha       ) EC_Alpha_Cb_Min       = EC_pTGraph_Alpha       ;
      if ( EC_TauBin_Cb_Max      < EC_pTGraph_TauBin      ) EC_TauBin_Cb_Max      = EC_pTGraph_TauBin      ;
      if ( EC_TauBin_Cb_Min      > EC_pTGraph_TauBin      ) EC_TauBin_Cb_Min      = EC_pTGraph_TauBin      ;
      if ( EC_AlphaTauBin_Cb_Max < EC_pTGraph_AlphaTauBin ) EC_AlphaTauBin_Cb_Max = EC_pTGraph_AlphaTauBin ;
      if ( EC_AlphaTauBin_Cb_Min > EC_pTGraph_AlphaTauBin ) EC_AlphaTauBin_Cb_Min = EC_pTGraph_AlphaTauBin ;

    }
    EC_Alpha_Cb       = EC_Alpha_Cb      /double(pTGraph_EC_Alpha       ->GetN()) ;
    EC_TauBin_Cb      = EC_TauBin_Cb     /double(pTGraph_EC_TauBin      ->GetN()) ;
    EC_AlphaTauBin_Cb = EC_AlphaTauBin_Cb/double(pTGraph_EC_AlphaTauBin ->GetN()) ;

//
    pTGraph_Alpha_EC_Cb    ->SetPoint( pTGraph_Alpha_EC_Cb    ->GetN(), RCcur , EC_Alpha_Cb     ) ;
    pTGraph_Alpha_EC_Cb_Max->SetPoint( pTGraph_Alpha_EC_Cb_Max->GetN(), RCcur , EC_Alpha_Cb_Max ) ;
    pTGraph_Alpha_EC_Cb_Min->SetPoint( pTGraph_Alpha_EC_Cb_Min->GetN(), RCcur , EC_Alpha_Cb_Min ) ;
    
    pTGraph_TauBin_EC_Cb    ->SetPoint( pTGraph_TauBin_EC_Cb    ->GetN(), RCcur , EC_TauBin_Cb     ) ;
    pTGraph_TauBin_EC_Cb_Max->SetPoint( pTGraph_TauBin_EC_Cb_Max->GetN(), RCcur , EC_TauBin_Cb_Max ) ;
    pTGraph_TauBin_EC_Cb_Min->SetPoint( pTGraph_TauBin_EC_Cb_Min->GetN(), RCcur , EC_TauBin_Cb_Min ) ;
    
    pTGraph_AlphaTauBin_EC_Cb    ->SetPoint( pTGraph_AlphaTauBin_EC_Cb    ->GetN(), RCcur , EC_AlphaTauBin_Cb     ) ;
    pTGraph_AlphaTauBin_EC_Cb_Max->SetPoint( pTGraph_AlphaTauBin_EC_Cb_Max->GetN(), RCcur , EC_AlphaTauBin_Cb_Max ) ;
    pTGraph_AlphaTauBin_EC_Cb_Min->SetPoint( pTGraph_AlphaTauBin_EC_Cb_Min->GetN(), RCcur , EC_AlphaTauBin_Cb_Min ) ;
    
    
  }

//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PLF_Beg.c_str());

//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page2   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page2->Add( V_pTGraph_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page2->Draw("A");
    pTMultiGraph_Alpha_page2->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page2->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page2->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page2->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page2->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page2 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page2->SetFillStyle(1001);
    pTLegend_page2->SetBorderSize(1);

    pTLegend_page2->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page2->Draw(); 
          
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page2   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page2->Add( V_pTGraph_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page2->Draw("A");
    pTMultiGraph_TauBin_page2->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page2->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page2->GetYaxis()->SetTitle("#tau (tbin)");
    pTMultiGraph_TauBin_page2->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page2->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page2->SetMaximum( 38. );
    pTMultiGraph_TauBin_page2->SetMinimum(  0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page2_2 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page2_2->SetFillStyle(1001);
    pTLegend_page2_2->SetBorderSize(1);

    pTLegend_page2_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page2_2->Draw(); 
          
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page3   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page3->Add( V_pTGraph_D_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page3->Draw("A");
    pTMultiGraph_Alpha_page3->SetTitle("d#alpha/dX_{T} VS Track Position");
    pTMultiGraph_Alpha_page3->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page3->GetYaxis()->SetTitle("d#alpha/dX_{T} (cm^{-1}) ");
    pTMultiGraph_Alpha_page3->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page3->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page3 = new TLegend(0.3,0.55 ,0.75,0.65  ); 
    pTLegend_page3->SetFillStyle(1001);
    pTLegend_page3->SetBorderSize(1);

    pTLegend_page3->AddEntry( V_pTGraph_D_Alpha_PR[0]  , V_pTGraph_D_Alpha_PR[0]->GetTitle() , "l");

    pTLegend_page3->Draw();       
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page3   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page3->Add( V_pTGraph_D_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page3->Draw("A");
    pTMultiGraph_TauBin_page3->SetTitle("d#tau/dX_{T} VS Track Position");
    pTMultiGraph_TauBin_page3->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page3->GetYaxis()->SetTitle("d#tau/dX_{T} (tbin cm^{-1}) ");
    pTMultiGraph_TauBin_page3->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page3->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page3_2 = new TLegend(0.3,0.75 ,0.75,0.85  ); 
    pTLegend_page3_2->SetFillStyle(1001);
    pTLegend_page3_2->SetBorderSize(1);

    pTLegend_page3_2->AddEntry( V_pTGraph_D_TauBin_PR[0]  , V_pTGraph_D_TauBin_PR[0]->GetTitle() , "l");

    pTLegend_page3_2->Draw();       
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page4   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page4->Add( V_pTGraph_E_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page4->Draw("A");
    pTMultiGraph_Alpha_page4->SetTitle("#sigma_{Y}^{#alpha} VS Track Position");
    pTMultiGraph_Alpha_page4->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page4->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha} (#mu m) ");
    pTMultiGraph_Alpha_page4->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page4->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page4->SetMaximum( 500. );
    pTMultiGraph_Alpha_page4->SetMinimum( 0. );
   
    pTCanvas->Update();
     
    TLegend* pTLegend_page4 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page4->SetFillStyle(1001);
    pTLegend_page4->SetBorderSize(1);

    pTLegend_page4->AddEntry( V_pTGraph_E_Alpha_PR[0]  , V_pTGraph_E_Alpha_PR[0]->GetTitle() , "l");

    pTLegend_page4->Draw();       
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page4   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page4->Add( V_pTGraph_E_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page4->Draw("A");
    pTMultiGraph_TauBin_page4->SetTitle("#sigma_{Y}^{#tau} VS Track Position");
    pTMultiGraph_TauBin_page4->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page4->GetYaxis()->SetTitle("#sigma_{Y}^{#tau} (#mu m) ");
    pTMultiGraph_TauBin_page4->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page4->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page4->SetMaximum( 500. );
    pTMultiGraph_TauBin_page4->SetMinimum( 0. );
   
    pTCanvas->Update();

    TLegend* pTLegend_page4_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page4_2->SetFillStyle(1001);
    pTLegend_page4_2->SetBorderSize(1);

    pTLegend_page4_2->AddEntry( V_pTGraph_E_TauBin_PR[0]  , V_pTGraph_E_TauBin_PR[0]->GetTitle() , "l");

    pTLegend_page4_2->Draw();       
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page5   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page5->Add( V_pTGraph_Alpha_PR[0] ) ;
    pTMultiGraph_Alpha_page5->Add( V_pTGraph_Alpha_PL[0] ) ;

    pTMultiGraph_Alpha_page5->Draw("A");
    pTMultiGraph_Alpha_page5->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page5->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page5->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page5->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page5->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    
    pTCanvas->Update();

    TLegend* pTLegend_page5 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page5->SetFillStyle(1001);
    pTLegend_page5->SetBorderSize(1);

    pTLegend_page5->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page5->AddEntry( V_pTGraph_Alpha_PL[0]  , V_pTGraph_Alpha_PL[0]->GetTitle() , "l");
   
    pTLegend_page5->Draw();       
    pTCanvas->Update();

 
    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page5   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page5->Add( V_pTGraph_TauBin_PR[0] ) ;
    pTMultiGraph_TauBin_page5->Add( V_pTGraph_TauBin_PL[0] ) ;

    pTMultiGraph_TauBin_page5->Draw("A");
    pTMultiGraph_TauBin_page5->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page5->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page5->GetYaxis()->SetTitle("#tau (tbin) ");
    pTMultiGraph_TauBin_page5->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page5->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    
    pTCanvas->Update();

    TLegend* pTLegend_page5_2 = new TLegend(0.3,0.75 ,0.75,0.85  ); 
    pTLegend_page5_2->SetFillStyle(1001);
    pTLegend_page5_2->SetBorderSize(1);

    pTLegend_page5_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page5_2->AddEntry( V_pTGraph_TauBin_PL[0]  , V_pTGraph_TauBin_PL[0]->GetTitle() , "l");
   
    pTLegend_page5_2->Draw();       
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 6
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page6   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page6->Add( V_pTGraph_D_Alpha_PR[0] ) ;
    pTMultiGraph_Alpha_page6->Add( V_pTGraph_D_Alpha_PL[0] ) ;

    pTMultiGraph_Alpha_page6->Draw("A");
    pTMultiGraph_Alpha_page6->SetTitle("d#alpha/dX_{T} VS Track Position");
    pTMultiGraph_Alpha_page6->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page6->GetYaxis()->SetTitle("d#alpha/dX_{T} (cm^{-1}) ");
    pTMultiGraph_Alpha_page6->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page6->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page6 = new TLegend(0.3,0.15 ,0.7,0.25  ); 
    pTLegend_page6->SetFillStyle(1001);
    pTLegend_page6->SetBorderSize(1);

    pTLegend_page6->AddEntry( V_pTGraph_D_Alpha_PR[0]  , V_pTGraph_D_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page6->AddEntry( V_pTGraph_D_Alpha_PL[0]  , V_pTGraph_D_Alpha_PL[0]->GetTitle() , "l");
 
    pTLegend_page6->Draw();       
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page6   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page6->Add( V_pTGraph_D_TauBin_PR[0] ) ;
    pTMultiGraph_TauBin_page6->Add( V_pTGraph_D_TauBin_PL[0] ) ;

    pTMultiGraph_TauBin_page6->Draw("A");
    pTMultiGraph_TauBin_page6->SetTitle("d#tau/dX_{T} VS Track Position");
    pTMultiGraph_TauBin_page6->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page6->GetYaxis()->SetTitle("d#tau/dX_{T} (tbin cm^{-1}) ");
    pTMultiGraph_TauBin_page6->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page6->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page6_2 = new TLegend(0.3,0.45 ,0.7,0.55  ); 
    pTLegend_page6_2->SetFillStyle(1001);
    pTLegend_page6_2->SetBorderSize(1);

    pTLegend_page6_2->AddEntry( V_pTGraph_D_TauBin_PR[0]  , V_pTGraph_D_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page6_2->AddEntry( V_pTGraph_D_TauBin_PL[0]  , V_pTGraph_D_TauBin_PL[0]->GetTitle() , "l");
 
    pTLegend_page6_2->Draw();       
    pTCanvas->Update();
 
   pTCanvas->Print(Str_Out_PLF.c_str());

//Page 7
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page7   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page7->Add( V_pTGraph_E_Alpha_PR[0] ) ;
    pTMultiGraph_Alpha_page7->Add( V_pTGraph_E_Alpha_PL[0] ) ;
    pTMultiGraph_Alpha_page7->Add( V_pTGraph_EC_Alpha  [0] ) ;

    pTMultiGraph_Alpha_page7->Draw("A");
    pTMultiGraph_Alpha_page7->SetTitle("#sigma_{Y}^{#alpha} VS Track Position");
    pTMultiGraph_Alpha_page7->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page7->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha} (#mu m) ");
    pTMultiGraph_Alpha_page7->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page7->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page7->SetMaximum( 500. );
    pTMultiGraph_Alpha_page7->SetMinimum(   0. );

    pTCanvas->Update();
    
    TLegend* pTLegend_page7 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page7->SetFillStyle(1001);
    pTLegend_page7->SetBorderSize(1);

    pTLegend_page7->AddEntry( V_pTGraph_E_Alpha_PR[0]  , V_pTGraph_E_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page7->AddEntry( V_pTGraph_E_Alpha_PL[0]  , V_pTGraph_E_Alpha_PL[0]->GetTitle() , "l");
    pTLegend_page7->AddEntry( V_pTGraph_EC_Alpha  [0]  , V_pTGraph_EC_Alpha  [0]->GetTitle() , "l");
   
    pTLegend_page7->Draw();       
    pTCanvas->Update();
 
    double Xmin_page7 = pTCanvas->cd(1)->GetUxmin() ;
    double Xmax_page7 = pTCanvas->cd(1)->GetUxmax() ;
    TLine* pTLineH_page7 = new TLine( Xmin_page7  , V_EC_Alpha_Cb[0] , Xmax_page7 , V_EC_Alpha_Cb[0] ) ;
    pTLineH_page7 ->SetLineStyle( 9 ); 
    pTLineH_page7 ->SetLineColor( 1 ) ;
    pTLineH_page7->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page7_1_2 = new TLine( Xmin_page7  , V_EC_Alpha_Cb_Max[0] , Xmax_page7 , V_EC_Alpha_Cb_Max[0] ) ;
    pTLineH_page7_1_2 ->SetLineStyle( 9 ); 
    pTLineH_page7_1_2 ->SetLineColor( 1 ) ;
    pTLineH_page7_1_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page7_1_3 = new TLine( Xmin_page7  , V_EC_Alpha_Cb_Min[0] , Xmax_page7 , V_EC_Alpha_Cb_Min[0] ) ;
    pTLineH_page7_1_3 ->SetLineStyle( 9 ); 
    pTLineH_page7_1_3 ->SetLineColor( 1 ) ;
    pTLineH_page7_1_3->Draw();  
    pTCanvas->Update()    ;
    
    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page7   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page7->Add( V_pTGraph_E_TauBin_PR[0] ) ;
    pTMultiGraph_TauBin_page7->Add( V_pTGraph_E_TauBin_PL[0] ) ;
    pTMultiGraph_TauBin_page7->Add( V_pTGraph_EC_TauBin  [0] ) ;

    pTMultiGraph_TauBin_page7->Draw("A");
    pTMultiGraph_TauBin_page7->SetTitle("#sigma_{Y}^{#tau} VS Track Position");
    pTMultiGraph_TauBin_page7->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page7->GetYaxis()->SetTitle("#sigma_{Y}^{#tau} (#mu m) ");
    pTMultiGraph_TauBin_page7->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page7->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page7->SetMaximum( 500. );
    pTMultiGraph_TauBin_page7->SetMinimum(   0. );

    pTCanvas->Update();
    
    TLegend* pTLegend_page7_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page7_2->SetFillStyle(1001);
    pTLegend_page7_2->SetBorderSize(1);

    pTLegend_page7_2->AddEntry( V_pTGraph_E_TauBin_PR[0]  , V_pTGraph_E_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page7_2->AddEntry( V_pTGraph_E_TauBin_PL[0]  , V_pTGraph_E_TauBin_PL[0]->GetTitle() , "l");
    pTLegend_page7_2->AddEntry( V_pTGraph_EC_TauBin  [0]  , V_pTGraph_EC_TauBin  [0]->GetTitle() , "l");
   
    pTLegend_page7_2->Draw();       
    pTCanvas->Update();
    
    TLine* pTLineH_page7_2_1 = new TLine( Xmin_page7  , V_EC_TauBin_Cb[0] , Xmax_page7 , V_EC_TauBin_Cb[0] ) ;
    pTLineH_page7_2_1 ->SetLineStyle( 9 ); 
    pTLineH_page7_2_1 ->SetLineColor( 1 ) ;
    pTLineH_page7_2_1->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page7_2_2 = new TLine( Xmin_page7  , V_EC_TauBin_Cb_Max[0] , Xmax_page7 , V_EC_TauBin_Cb_Max[0] ) ;
    pTLineH_page7_2_2 ->SetLineStyle( 9 ); 
    pTLineH_page7_2_2 ->SetLineColor( 1 ) ;
    pTLineH_page7_2_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page7_2_3 = new TLine( Xmin_page7  , V_EC_TauBin_Cb_Min[0] , Xmax_page7 , V_EC_TauBin_Cb_Min[0] ) ;
    pTLineH_page7_2_3 ->SetLineStyle( 9 ); 
    pTLineH_page7_2_3 ->SetLineColor( 1 ) ;
    pTLineH_page7_2_3->Draw();  
    pTCanvas->Update()    ;
    
  pTCanvas->Print(Str_Out_PLF.c_str());


//Page 8
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page8   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page8->Add( V_pTGraph_Alpha_PR[0] ) ;
    pTMultiGraph_Alpha_page8->Add( V_pTGraph_Alpha_PL[0] ) ;
    pTMultiGraph_Alpha_page8->Add( V_pTGraph_Alpha_PR[1] ) ;
    pTMultiGraph_Alpha_page8->Add( V_pTGraph_Alpha_PL[1] ) ;

    V_pTGraph_Alpha_PR[0] ->SetLineStyle(1) ;
    V_pTGraph_Alpha_PL[0] ->SetLineStyle(1) ;
    V_pTGraph_Alpha_PR[1] ->SetLineStyle(2) ;
    V_pTGraph_Alpha_PL[1] ->SetLineStyle(2) ;

    pTMultiGraph_Alpha_page8->Draw("A");
    pTMultiGraph_Alpha_page8->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page8->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page8->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page8->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page8->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    
    pTCanvas->Update();

    TLegend* pTLegend_page8 = new TLegend(0.3,0.55 ,0.75,0.70  ); 
    pTLegend_page8->SetFillStyle(1001);
    pTLegend_page8->SetBorderSize(1);

    pTLegend_page8->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page8->AddEntry( V_pTGraph_Alpha_PL[0]  , V_pTGraph_Alpha_PL[0]->GetTitle() , "l");
    pTLegend_page8->AddEntry( V_pTGraph_Alpha_PR[1]  , V_pTGraph_Alpha_PR[1]->GetTitle() , "l");
    pTLegend_page8->AddEntry( V_pTGraph_Alpha_PL[1]  , V_pTGraph_Alpha_PL[1]->GetTitle() , "l");
    pTLegend_page8->Draw();       
    pTCanvas->Update();

 
    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page8   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page8->Add( V_pTGraph_TauBin_PR[0] ) ;
    pTMultiGraph_TauBin_page8->Add( V_pTGraph_TauBin_PL[0] ) ;
    pTMultiGraph_TauBin_page8->Add( V_pTGraph_TauBin_PR[1] ) ;
    pTMultiGraph_TauBin_page8->Add( V_pTGraph_TauBin_PL[1] ) ;

    V_pTGraph_TauBin_PR[0] ->SetLineStyle(1) ;
    V_pTGraph_TauBin_PL[0] ->SetLineStyle(1) ;
    V_pTGraph_TauBin_PR[1] ->SetLineStyle(2) ;
    V_pTGraph_TauBin_PL[1] ->SetLineStyle(2) ;

    pTMultiGraph_TauBin_page8->Draw("A");
    pTMultiGraph_TauBin_page8->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page8->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page8->GetYaxis()->SetTitle("#tau (tbin) ");
    pTMultiGraph_TauBin_page8->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page8->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    
    pTCanvas->Update();

    TLegend* pTLegend_page8_2 = new TLegend(0.3,0.70 ,0.75,0.85  ); 
    pTLegend_page8_2->SetFillStyle(1001);
    pTLegend_page8_2->SetBorderSize(1);

    pTLegend_page8_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page8_2->AddEntry( V_pTGraph_TauBin_PL[0]  , V_pTGraph_TauBin_PL[0]->GetTitle() , "l");
    pTLegend_page8_2->AddEntry( V_pTGraph_TauBin_PR[1]  , V_pTGraph_TauBin_PR[1]->GetTitle() , "l");
    pTLegend_page8_2->AddEntry( V_pTGraph_TauBin_PL[1]  , V_pTGraph_TauBin_PL[1]->GetTitle() , "l");
    pTLegend_page8_2->Draw();       
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 9
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page9   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page9->Add( V_pTGraph_D_Alpha_PR[0] ) ;
    pTMultiGraph_Alpha_page9->Add( V_pTGraph_D_Alpha_PL[0] ) ;
    pTMultiGraph_Alpha_page9->Add( V_pTGraph_D_Alpha_PR[1] ) ;
    pTMultiGraph_Alpha_page9->Add( V_pTGraph_D_Alpha_PL[1] ) ;

    V_pTGraph_D_Alpha_PR[0] ->SetLineStyle(1) ;
    V_pTGraph_D_Alpha_PL[0] ->SetLineStyle(1) ;
    V_pTGraph_D_Alpha_PR[1] ->SetLineStyle(2) ;
    V_pTGraph_D_Alpha_PL[1] ->SetLineStyle(2) ;

    pTMultiGraph_Alpha_page9->Draw("A");
    pTMultiGraph_Alpha_page9->SetTitle("d#alpha/dX_{T} VS Track Position");
    pTMultiGraph_Alpha_page9->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page9->GetYaxis()->SetTitle("d#alpha/dX_{T} (cm^{-1}) ");
    pTMultiGraph_Alpha_page9->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page9->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page9 = new TLegend(0.3,0.15 ,0.7,0.25  ); 
    pTLegend_page9->SetFillStyle(1001);
    pTLegend_page9->SetBorderSize(1);
    pTLegend_page9->AddEntry( V_pTGraph_D_Alpha_PR[0]  , V_pTGraph_D_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page9->AddEntry( V_pTGraph_D_Alpha_PL[0]  , V_pTGraph_D_Alpha_PL[0]->GetTitle() , "l");
    pTLegend_page9->AddEntry( V_pTGraph_D_Alpha_PR[1]  , V_pTGraph_D_Alpha_PR[1]->GetTitle() , "l");
    pTLegend_page9->AddEntry( V_pTGraph_D_Alpha_PL[1]  , V_pTGraph_D_Alpha_PL[1]->GetTitle() , "l");

    pTLegend_page9->Draw();       
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page9   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page9->Add( V_pTGraph_D_TauBin_PR[0] ) ;
    pTMultiGraph_TauBin_page9->Add( V_pTGraph_D_TauBin_PL[0] ) ;
    pTMultiGraph_TauBin_page9->Add( V_pTGraph_D_TauBin_PR[1] ) ;
    pTMultiGraph_TauBin_page9->Add( V_pTGraph_D_TauBin_PL[1] ) ;

    V_pTGraph_D_TauBin_PR[0] ->SetLineStyle(1) ;
    V_pTGraph_D_TauBin_PL[0] ->SetLineStyle(1) ;
    V_pTGraph_D_TauBin_PR[1] ->SetLineStyle(2) ;
    V_pTGraph_D_TauBin_PL[1] ->SetLineStyle(2) ;

    pTMultiGraph_TauBin_page9->Draw("A");
    pTMultiGraph_TauBin_page9->SetTitle("d#tau/dX_{T} VS Track Position");
    pTMultiGraph_TauBin_page9->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page9->GetYaxis()->SetTitle("d#tau/dX_{T} (tbin cm^{-1}) ");
    pTMultiGraph_TauBin_page9->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page9->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page9_2 = new TLegend(0.3,0.45 ,0.7,0.55  ); 
    pTLegend_page9_2->SetFillStyle(1001);
    pTLegend_page9_2->SetBorderSize(1);
    pTLegend_page9_2->AddEntry( V_pTGraph_D_TauBin_PR[0]  , V_pTGraph_D_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page9_2->AddEntry( V_pTGraph_D_TauBin_PL[0]  , V_pTGraph_D_TauBin_PL[0]->GetTitle() , "l");
    pTLegend_page9_2->AddEntry( V_pTGraph_D_TauBin_PR[1]  , V_pTGraph_D_TauBin_PR[1]->GetTitle() , "l");
    pTLegend_page9_2->AddEntry( V_pTGraph_D_TauBin_PL[1]  , V_pTGraph_D_TauBin_PL[1]->GetTitle() , "l");

    pTLegend_page9_2->Draw();       
    pTCanvas->Update();

 
  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 10
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page10   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page10->Add( V_pTGraph_EC_Alpha  [0] ) ; V_pTGraph_EC_Alpha[0]->SetLineColor(2) ;
    pTMultiGraph_Alpha_page10->Add( V_pTGraph_EC_Alpha  [1] ) ; V_pTGraph_EC_Alpha[1]->SetLineColor(4) ;

    pTMultiGraph_Alpha_page10->Draw("A");
    pTMultiGraph_Alpha_page10->SetTitle("#sigma_{Y}^{#alpha} VS Track Position");
    pTMultiGraph_Alpha_page10->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page10->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha} (#mu m) ");
    pTMultiGraph_Alpha_page10->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page10->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page10->SetMaximum( 500. );
    pTMultiGraph_Alpha_page10->SetMinimum( 0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page10 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page10->SetFillStyle(1001);
    pTLegend_page10->SetBorderSize(1);

    pTLegend_page10->AddEntry( V_pTGraph_EC_Alpha  [0]  , V_pTGraph_EC_Alpha  [0]->GetTitle() , "l");
    pTLegend_page10->AddEntry( V_pTGraph_EC_Alpha  [1]  , V_pTGraph_EC_Alpha  [1]->GetTitle() , "l");

    pTLegend_page10->Draw();       
    pTCanvas->Update();
 
    double Xmin_page10 = pTCanvas->cd(1)->GetUxmin() ;
    double Xmax_page10 = pTCanvas->cd(1)->GetUxmax() ;
    TLine* pTLineH_0_page10 = new TLine( Xmin_page10  , V_EC_Alpha_Cb[0] , Xmax_page10 , V_EC_Alpha_Cb[0] ) ;
    pTLineH_0_page10 ->SetLineStyle(9); 
    pTLineH_0_page10 ->SetLineColor( 2 ) ;
    pTLineH_0_page10->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_1_page10 = new TLine( Xmin_page10  , V_EC_Alpha_Cb[1] , Xmax_page10 , V_EC_Alpha_Cb[1] ) ;
    pTLineH_1_page10 ->SetLineStyle(9); 
    pTLineH_1_page10 ->SetLineColor( 4 ) ;
    pTLineH_1_page10->Draw();  
    pTCanvas->Update()    ;


    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page10   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page10->Add( V_pTGraph_EC_TauBin  [0] ) ; V_pTGraph_EC_TauBin[0]->SetLineColor(2) ;
    pTMultiGraph_TauBin_page10->Add( V_pTGraph_EC_TauBin  [1] ) ; V_pTGraph_EC_TauBin[1]->SetLineColor(4) ;

    pTMultiGraph_TauBin_page10->Draw("A");
    pTMultiGraph_TauBin_page10->SetTitle("#sigma_{Y}^{#tau} VS Track Position");
    pTMultiGraph_TauBin_page10->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page10->GetYaxis()->SetTitle("#sigma_{Y}^{#tau} (#mu m) ");
    pTMultiGraph_TauBin_page10->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page10->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page10->SetMaximum( 500. );
    pTMultiGraph_TauBin_page10->SetMinimum( 0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page10_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page10_2->SetFillStyle(1001);
    pTLegend_page10_2->SetBorderSize(1);

    pTLegend_page10_2->AddEntry( V_pTGraph_EC_TauBin  [0]  , V_pTGraph_EC_TauBin  [0]->GetTitle() , "l");
    pTLegend_page10_2->AddEntry( V_pTGraph_EC_TauBin  [1]  , V_pTGraph_EC_TauBin  [1]->GetTitle() , "l");

    pTLegend_page10_2->Draw();       
    pTCanvas->Update();
    
    TLine* pTLineH_0_page10_2 = new TLine( Xmin_page10  , V_EC_TauBin_Cb[0] , Xmax_page10 , V_EC_TauBin_Cb[0] ) ;
    pTLineH_0_page10_2 ->SetLineStyle(9); 
    pTLineH_0_page10_2 ->SetLineColor( 2 ) ;
    pTLineH_0_page10_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_1_page10_2 = new TLine( Xmin_page10  , V_EC_TauBin_Cb[1] , Xmax_page10 , V_EC_TauBin_Cb[1] ) ;
    pTLineH_1_page10_2 ->SetLineStyle(9); 
    pTLineH_1_page10_2 ->SetLineColor( 4 ) ;
    pTLineH_1_page10_2->Draw();  
    pTCanvas->Update()    ;

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 11
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page11   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page11->Add( V_pTGraph_EC_Alpha  [0] ) ; V_pTGraph_EC_Alpha[0]->SetLineColor(2) ;
    pTMultiGraph_Alpha_page11->Add( V_pTGraph_EC_Alpha  [1] ) ; V_pTGraph_EC_Alpha[1]->SetLineColor(4) ;
    pTMultiGraph_Alpha_page11->Add( V_pTGraph_EC_Alpha  [2] ) ; V_pTGraph_EC_Alpha[2]->SetLineColor(3) ;

    pTMultiGraph_Alpha_page11->Draw("A");
    pTMultiGraph_Alpha_page11->SetTitle("#sigma_{Y}^{#alpha} VS Track Position");
    pTMultiGraph_Alpha_page11->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page11->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha} (#mu m) ");
    pTMultiGraph_Alpha_page11->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page11->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page11->SetMaximum( 500. );
    pTMultiGraph_Alpha_page11->SetMinimum( 0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page11 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page11->SetFillStyle(1001);
    pTLegend_page11->SetBorderSize(1);

    pTLegend_page11->AddEntry( V_pTGraph_EC_Alpha  [0]  , V_pTGraph_EC_Alpha  [0]->GetTitle() , "l");
    pTLegend_page11->AddEntry( V_pTGraph_EC_Alpha  [1]  , V_pTGraph_EC_Alpha  [1]->GetTitle() , "l");
    pTLegend_page11->AddEntry( V_pTGraph_EC_Alpha  [2]  , V_pTGraph_EC_Alpha  [2]->GetTitle() , "l");
 
    pTLegend_page11->Draw();       
    pTCanvas->Update();

    double Xmin_page11 = pTCanvas->cd(1)->GetUxmin() ;
    double Xmax_page11 = pTCanvas->cd(1)->GetUxmax() ;
    TLine* pTLineH_0_page11 = new TLine( Xmin_page11  , V_EC_Alpha_Cb[0] , Xmax_page11 , V_EC_Alpha_Cb[0] ) ;
    pTLineH_0_page11 ->SetLineStyle(9); 
    pTLineH_0_page11 ->SetLineColor( 2 ) ;
    pTLineH_0_page11->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_1_page11 = new TLine( Xmin_page11  , V_EC_Alpha_Cb[1] , Xmax_page11 , V_EC_Alpha_Cb[1] ) ;
    pTLineH_1_page11 ->SetLineStyle(9); 
    pTLineH_1_page11 ->SetLineColor( 4 ) ;
    pTLineH_1_page11->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_2_page11 = new TLine( Xmin_page11  , V_EC_Alpha_Cb[2] , Xmax_page11 , V_EC_Alpha_Cb[2] ) ;
    pTLineH_2_page11 ->SetLineStyle(9); 
    pTLineH_2_page11 ->SetLineColor( 3 ) ;
    pTLineH_2_page11->Draw();  
    pTCanvas->Update()    ;


    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page11   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page11->Add( V_pTGraph_EC_TauBin  [0] ) ; V_pTGraph_EC_TauBin[0]->SetLineColor(2) ;
    pTMultiGraph_TauBin_page11->Add( V_pTGraph_EC_TauBin  [1] ) ; V_pTGraph_EC_TauBin[1]->SetLineColor(4) ;
    pTMultiGraph_TauBin_page11->Add( V_pTGraph_EC_TauBin  [2] ) ; V_pTGraph_EC_TauBin[2]->SetLineColor(3) ;

    pTMultiGraph_TauBin_page11->Draw("A");
    pTMultiGraph_TauBin_page11->SetTitle("#sigma_{Y}^{#tau} VS Track Position");
    pTMultiGraph_TauBin_page11->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page11->GetYaxis()->SetTitle("#sigma_{Y}^{#tau} (#mu m) ");
    pTMultiGraph_TauBin_page11->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page11->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page11->SetMaximum( 500. );
    pTMultiGraph_TauBin_page11->SetMinimum( 0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page11_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page11_2->SetFillStyle(1001);
    pTLegend_page11_2->SetBorderSize(1);

    pTLegend_page11_2->AddEntry( V_pTGraph_EC_TauBin  [0]  , V_pTGraph_EC_TauBin  [0]->GetTitle() , "l");
    pTLegend_page11_2->AddEntry( V_pTGraph_EC_TauBin  [1]  , V_pTGraph_EC_TauBin  [1]->GetTitle() , "l");
    pTLegend_page11_2->AddEntry( V_pTGraph_EC_TauBin  [2]  , V_pTGraph_EC_TauBin  [2]->GetTitle() , "l");
 
    pTLegend_page11_2->Draw();       
    pTCanvas->Update();
    

    TLine* pTLineH_0_page11_2 = new TLine( Xmin_page11  , V_EC_TauBin_Cb[0] , Xmax_page11 , V_EC_TauBin_Cb[0] ) ;
    pTLineH_0_page11_2 ->SetLineStyle(9); 
    pTLineH_0_page11_2 ->SetLineColor( 2 ) ;
    pTLineH_0_page11_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_1_page11_2 = new TLine( Xmin_page11  , V_EC_TauBin_Cb[1] , Xmax_page11 , V_EC_TauBin_Cb[1] ) ;
    pTLineH_1_page11_2 ->SetLineStyle(9); 
    pTLineH_1_page11_2 ->SetLineColor( 4 ) ;
    pTLineH_1_page11_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_2_page11_2 = new TLine( Xmin_page11  , V_EC_TauBin_Cb[2] , Xmax_page11 , V_EC_TauBin_Cb[2] ) ;
    pTLineH_2_page11_2 ->SetLineStyle(9); 
    pTLineH_2_page11_2 ->SetLineColor( 3 ) ;
    pTLineH_2_page11_2->Draw();  
    pTCanvas->Update()    ;

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 12
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    
    TMultiGraph* pTMultiGraph_Alpha_page12   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page12->Add( pTGraph_Alpha_EC_Cb    ,"l" ) ;
    pTMultiGraph_Alpha_page12->Add( pTGraph_Alpha_EC_Cb_Max,"l" ) ;
    pTMultiGraph_Alpha_page12->Add( pTGraph_Alpha_EC_Cb_Min,"l" ) ;

    pTMultiGraph_Alpha_page12->Draw("A");
    pTMultiGraph_Alpha_page12->SetTitle("#sigma_{Y}^{#alpha} VS RC");
    pTMultiGraph_Alpha_page12->GetXaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_Alpha_page12->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha} (#mu m) ");
    pTMultiGraph_Alpha_page12->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page12->GetXaxis()->SetLimits( 0., 250.);
    pTMultiGraph_Alpha_page12->SetMaximum( 450. );
    pTMultiGraph_Alpha_page12->SetMinimum( 0. );

    pTCanvas->Update();
    
    TLegend* pTLegend_page12 = new TLegend(0.2,0.15 ,0.4,0.25  ); 
    pTLegend_page12->SetFillStyle(1001);
    pTLegend_page12->SetBorderSize(1);
    pTLegend_page12->AddEntry( pTGraph_Alpha_EC_Cb      , pTGraph_Alpha_EC_Cb    ->GetTitle() , "l");
    pTLegend_page12->AddEntry( pTGraph_Alpha_EC_Cb_Max  , pTGraph_Alpha_EC_Cb_Max->GetTitle() , "l");
    pTLegend_page12->Draw(); 

    pTCanvas->Update();

    pTCanvas->cd(2) ;
    
    TMultiGraph* pTMultiGraph_TauBin_page12   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page12->Add( pTGraph_TauBin_EC_Cb    ,"l" ) ;
    pTMultiGraph_TauBin_page12->Add( pTGraph_TauBin_EC_Cb_Max,"l" ) ;
    pTMultiGraph_TauBin_page12->Add( pTGraph_TauBin_EC_Cb_Min,"l" ) ;

    pTMultiGraph_TauBin_page12->Draw("A");
    pTMultiGraph_TauBin_page12->SetTitle("#sigma_{Y}^{#tau} VS RC");
    pTMultiGraph_TauBin_page12->GetXaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_TauBin_page12->GetYaxis()->SetTitle("#sigma_{Y}^{#tau} (#mu m) ");
    pTMultiGraph_TauBin_page12->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page12->GetXaxis()->SetLimits( 0., 250.);
    pTMultiGraph_TauBin_page12->SetMaximum( 450. );
    pTMultiGraph_TauBin_page12->SetMinimum( 0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page12_2 = new TLegend(0.2,0.15 ,0.4,0.25  ); 
    pTLegend_page12_2->SetFillStyle(1001);
    pTLegend_page12_2->SetBorderSize(1);
    pTLegend_page12_2->AddEntry( pTGraph_TauBin_EC_Cb      , pTGraph_TauBin_EC_Cb    ->GetTitle() , "l");
    pTLegend_page12_2->AddEntry( pTGraph_TauBin_EC_Cb_Max  , pTGraph_TauBin_EC_Cb_Max->GetTitle() , "l");
    pTLegend_page12_2->Draw(); 

    pTCanvas->Update();

 pTCanvas->Print(Str_Out_PLF.c_str());

//Page 13
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page13   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page13->Add( V_pTGraph_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page13->Draw("A");
    pTMultiGraph_Alpha_page13->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page13->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page13->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page13->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page13->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page13 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page13->SetFillStyle(1001);
    pTLegend_page13->SetBorderSize(1);

    pTLegend_page13->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page13->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_Alpha_H = new TArrow( 0                                  , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                         (V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],0.01,"|>");
    pTArrow_Alpha_H->SetLineColor( 2 ) ;
    pTArrow_Alpha_H->Draw();
    
    TArrow* pTArrow_Alpha_V = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                         (V_pTGraph_Alpha_PR[0]->GetX())[40] , pTCanvas->cd(1)->GetUymin()        ,0.01,"|>");
    pTArrow_Alpha_V->SetLineColor( 2 ) ;
    pTArrow_Alpha_V->Draw();
  
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page13   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page13->Add( V_pTGraph_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page13->Draw("A");
    pTMultiGraph_TauBin_page13->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page13->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page13->GetYaxis()->SetTitle("#tau (tbin)");
    pTMultiGraph_TauBin_page13->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page13->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page13->SetMaximum( 38. );
    pTMultiGraph_TauBin_page13->SetMinimum(  0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page13_2 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page13_2->SetFillStyle(1001);
    pTLegend_page13_2->SetBorderSize(1);

    pTLegend_page13_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page13_2->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_TauBin_H = new TArrow( 0                                  , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                          (V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],0.01,"|>");
    pTArrow_TauBin_H->SetLineColor( 2 ) ;
    pTArrow_TauBin_H->Draw();
    
    TArrow* pTArrow_TauBin_V = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                          (V_pTGraph_TauBin_PR[0]->GetX())[40] , pTCanvas->cd(2)->GetUymin()        ,0.01,"|>");
    pTArrow_TauBin_V->SetLineColor( 2 ) ;
    pTArrow_TauBin_V->Draw();
  
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 14
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page14   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page14->Add( V_pTGraph_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page14->Draw("A");
    pTMultiGraph_Alpha_page14->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page14->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page14->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page14->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page14->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page14 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page14->SetFillStyle(1001);
    pTLegend_page14->SetBorderSize(1);

    pTLegend_page14->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page14->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_Alpha_H_C = new TArrow( 0                                  , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],0.01,"|>");
    pTArrow_Alpha_H_C->SetLineColor( 2 ) ;
    pTArrow_Alpha_H_C->Draw();
    
    TArrow* pTArrow_Alpha_V_C = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[40] , pTCanvas->cd(1)->GetUymin()        ,0.01,"|>");
    pTArrow_Alpha_V_C->SetLineColor( 2 ) ;
    pTArrow_Alpha_V_C->Draw();
  
    TArrow* pTArrow_Alpha_H_U = new TArrow( 0                                  , (V_pTGraph_Alpha_PR[0]->GetY())[41],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[41] , (V_pTGraph_Alpha_PR[0]->GetY())[41],0.01,"|>");
    pTArrow_Alpha_H_U->SetLineColor( 2 ) ;
    pTArrow_Alpha_H_U->Draw();
    
    TArrow* pTArrow_Alpha_V_U = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[41] , (V_pTGraph_Alpha_PR[0]->GetY())[41],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[41] , pTCanvas->cd(1)->GetUymin()        ,0.01,"|>");
    pTArrow_Alpha_V_U->SetLineColor( 2 ) ;
    pTArrow_Alpha_V_U->Draw();
  
    TArrow* pTArrow_Alpha_H_D = new TArrow( 0                                  , (V_pTGraph_Alpha_PR[0]->GetY())[39],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[39] , (V_pTGraph_Alpha_PR[0]->GetY())[39],0.01,"|>");
    pTArrow_Alpha_H_D->SetLineColor( 2 ) ;
    pTArrow_Alpha_H_D->Draw();
    
    TArrow* pTArrow_Alpha_V_D = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[39] , (V_pTGraph_Alpha_PR[0]->GetY())[39],
                                           (V_pTGraph_Alpha_PR[0]->GetX())[39] , pTCanvas->cd(1)->GetUymin()        ,0.01,"|>");
    pTArrow_Alpha_V_D->SetLineColor( 2 ) ;
    pTArrow_Alpha_V_D->Draw();
          
    pTCanvas->Update();
  
    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page14   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page14->Add( V_pTGraph_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page14->Draw("A");
    pTMultiGraph_TauBin_page14->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page14->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page14->GetYaxis()->SetTitle("#tau (tbin)");
    pTMultiGraph_TauBin_page14->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page14->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page14->SetMaximum( 38. );
    pTMultiGraph_TauBin_page14->SetMinimum(  0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page14_2 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page14_2->SetFillStyle(1001);
    pTLegend_page14_2->SetBorderSize(1);

    pTLegend_page14_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page14_2->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_TauBin_H_C = new TArrow( 0                                  , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],0.01,"|>");
    pTArrow_TauBin_H_C->SetLineColor( 2 ) ;
    pTArrow_TauBin_H_C->Draw();
    
    TArrow* pTArrow_TauBin_V_C = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[40] , pTCanvas->cd(2)->GetUymin()        ,0.01,"|>");
    pTArrow_TauBin_V_C->SetLineColor( 2 ) ;
    pTArrow_TauBin_V_C->Draw();
  
    TArrow* pTArrow_TauBin_H_U = new TArrow( 0                                  , (V_pTGraph_TauBin_PR[0]->GetY())[41],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[41] , (V_pTGraph_TauBin_PR[0]->GetY())[41],0.01,"|>");
    pTArrow_TauBin_H_U->SetLineColor( 2 ) ;
    pTArrow_TauBin_H_U->Draw();
    
    TArrow* pTArrow_TauBin_V_U = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[41] , (V_pTGraph_TauBin_PR[0]->GetY())[41],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[41] , pTCanvas->cd(2)->GetUymin()        ,0.01,"|>");
    pTArrow_TauBin_V_U->SetLineColor( 2 ) ;
    pTArrow_TauBin_V_U->Draw();
  
    TArrow* pTArrow_TauBin_H_D = new TArrow( 0                                  , (V_pTGraph_TauBin_PR[0]->GetY())[39],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[39] , (V_pTGraph_TauBin_PR[0]->GetY())[39],0.01,"|>");
    pTArrow_TauBin_H_D->SetLineColor( 2 ) ;
    pTArrow_TauBin_H_D->Draw();
    
    TArrow* pTArrow_TauBin_V_D = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[39] , (V_pTGraph_TauBin_PR[0]->GetY())[39],
                                            (V_pTGraph_TauBin_PR[0]->GetX())[39] , pTCanvas->cd(2)->GetUymin()        ,0.01,"|>");
    pTArrow_TauBin_V_D->SetLineColor( 2 ) ;
    pTArrow_TauBin_V_D->Draw();
          
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());


//Page 15
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page15   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page15->Add( V_pTGraph_Amplitude_PR[0] ) ;
    pTMultiGraph_Alpha_page15->Add( V_pTGraph_Amplitude_PC[0] ) ;
    pTMultiGraph_Alpha_page15->Add( V_pTGraph_Amplitude_PL[0] ) ;

    pTMultiGraph_Alpha_page15->Draw("A");
    pTMultiGraph_Alpha_page15->SetTitle("Amplitude VS Track Position");
    pTMultiGraph_Alpha_page15->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page15->GetYaxis()->SetTitle("Amplitude (ADC) ");
    pTMultiGraph_Alpha_page15->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page15->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page15->SetMaximum( 600. );
    pTMultiGraph_Alpha_page15->SetMinimum(   0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page15 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page15->SetFillStyle(1001);
    pTLegend_page15->SetBorderSize(1);

    pTLegend_page15->AddEntry( V_pTGraph_Amplitude_PR[0]  , V_pTGraph_Amplitude_PR[0]->GetTitle() , "l");
    pTLegend_page15->AddEntry( V_pTGraph_Amplitude_PC[0]  , V_pTGraph_Amplitude_PC[0]->GetTitle() , "l");
    pTLegend_page15->AddEntry( V_pTGraph_Amplitude_PL[0]  , V_pTGraph_Amplitude_PL[0]->GetTitle() , "l");
    pTLegend_page15->Draw(); 
          
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 16
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page16   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PR[0] ) ;
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PC[0] ) ;
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PL[0] ) ;
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PR[1] ) ;
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PC[1] ) ;
    pTMultiGraph_Alpha_page16->Add( V_pTGraph_Amplitude_PL[1] ) ;

    V_pTGraph_Amplitude_PR[0] ->SetLineStyle(1) ;
    V_pTGraph_Amplitude_PC[0] ->SetLineStyle(1) ;
    V_pTGraph_Amplitude_PL[0] ->SetLineStyle(1) ;
    V_pTGraph_Amplitude_PR[1] ->SetLineStyle(2) ;
    V_pTGraph_Amplitude_PC[1] ->SetLineStyle(2) ;
    V_pTGraph_Amplitude_PL[1] ->SetLineStyle(2) ;

    pTMultiGraph_Alpha_page16->Draw("A");
    pTMultiGraph_Alpha_page16->SetTitle("Amplitude VS Track Position");
    pTMultiGraph_Alpha_page16->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page16->GetYaxis()->SetTitle("Amplitude (ADC) ");
    pTMultiGraph_Alpha_page16->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page16->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_Alpha_page16->SetMaximum( 650. );
    pTMultiGraph_Alpha_page16->SetMinimum(   0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page16 = new TLegend(0.3,0.45 ,0.75,0.65  ); 
    pTLegend_page16->SetFillStyle(1001);
    pTLegend_page16->SetBorderSize(1);

    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PR[0]  , V_pTGraph_Amplitude_PR[0]->GetTitle() , "l");
    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PC[0]  , V_pTGraph_Amplitude_PC[0]->GetTitle() , "l");
    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PL[0]  , V_pTGraph_Amplitude_PL[0]->GetTitle() , "l");
    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PR[1]  , V_pTGraph_Amplitude_PR[1]->GetTitle() , "l");
    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PC[1]  , V_pTGraph_Amplitude_PC[1]->GetTitle() , "l");
    pTLegend_page16->AddEntry( V_pTGraph_Amplitude_PL[1]  , V_pTGraph_Amplitude_PL[1]->GetTitle() , "l");
    pTLegend_page16->Draw(); 
          
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 17
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    
    TMultiGraph* pTMultiGraph_AlphaTauBin_page17   = new TMultiGraph ;  
    pTMultiGraph_AlphaTauBin_page17->Add( pTGraph_AlphaTauBin_EC_Cb    ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page17->Add( pTGraph_AlphaTauBin_EC_Cb_Max,"l" ) ;
    pTMultiGraph_AlphaTauBin_page17->Add( pTGraph_AlphaTauBin_EC_Cb_Min,"l" ) ;

    pTMultiGraph_AlphaTauBin_page17->Draw("A");
    pTMultiGraph_AlphaTauBin_page17->SetTitle("#sigma_{Y}^{#alpha+#tau} VS RC");
    pTMultiGraph_AlphaTauBin_page17->GetXaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_AlphaTauBin_page17->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha+#tau} (#mu m) ");
    pTMultiGraph_AlphaTauBin_page17->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_AlphaTauBin_page17->GetXaxis()->SetLimits( 0., 250.);
    pTMultiGraph_AlphaTauBin_page17->SetMaximum( 450. );
    pTMultiGraph_AlphaTauBin_page17->SetMinimum( 0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page17_2 = new TLegend(0.2,0.15 ,0.4,0.25  ); 
    pTLegend_page17_2->SetFillStyle(1001);
    pTLegend_page17_2->SetBorderSize(1);
    pTLegend_page17_2->AddEntry( pTGraph_AlphaTauBin_EC_Cb      , pTGraph_AlphaTauBin_EC_Cb    ->GetTitle() , "l");
    pTLegend_page17_2->AddEntry( pTGraph_AlphaTauBin_EC_Cb_Max  , pTGraph_AlphaTauBin_EC_Cb_Max->GetTitle() , "l");
    pTLegend_page17_2->Draw(); 

    pTCanvas->Update();

 pTCanvas->Print(Str_Out_PLF.c_str());

//Page 18
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_AlphaTauBin_page18   = new TMultiGraph ;  
    pTMultiGraph_AlphaTauBin_page18->Add( V_pTGraph_EC_AlphaTauBin  [0] ) ; V_pTGraph_EC_AlphaTauBin[0]->SetLineColor(2) ;
    pTMultiGraph_AlphaTauBin_page18->Add( V_pTGraph_EC_AlphaTauBin  [1] ) ; V_pTGraph_EC_AlphaTauBin[1]->SetLineColor(4) ;

    pTMultiGraph_AlphaTauBin_page18->Draw("A");
    pTMultiGraph_AlphaTauBin_page18->SetTitle("#sigma_{Y}^{#alpha+#tau} VS Track Position");
    pTMultiGraph_AlphaTauBin_page18->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_AlphaTauBin_page18->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha+#tau} (#mu m) ");
    pTMultiGraph_AlphaTauBin_page18->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_AlphaTauBin_page18->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_AlphaTauBin_page18->SetMaximum( 500. );
    pTMultiGraph_AlphaTauBin_page18->SetMinimum( 0. );
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page18_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page18_2->SetFillStyle(1001);
    pTLegend_page18_2->SetBorderSize(1);

    pTLegend_page18_2->AddEntry( V_pTGraph_EC_AlphaTauBin  [0]  , V_pTGraph_EC_AlphaTauBin  [0]->GetTitle() , "l");
    pTLegend_page18_2->AddEntry( V_pTGraph_EC_AlphaTauBin  [1]  , V_pTGraph_EC_AlphaTauBin  [1]->GetTitle() , "l");

    pTLegend_page10_2->Draw();       
    pTCanvas->Update();
    
    double Xmin_page18 = pTCanvas->cd(1)->GetUxmin() ;
    double Xmax_page18 = pTCanvas->cd(1)->GetUxmax() ;
    TLine* pTLineH_0_page18_2 = new TLine( Xmin_page18  , V_EC_AlphaTauBin_Cb[0] , Xmax_page18 , V_EC_AlphaTauBin_Cb[0] ) ;
    pTLineH_0_page18_2 ->SetLineStyle(9); 
    pTLineH_0_page18_2 ->SetLineColor( 2 ) ;
    pTLineH_0_page18_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_1_page18_2 = new TLine( Xmin_page18  , V_EC_AlphaTauBin_Cb[1] , Xmax_page18 , V_EC_AlphaTauBin_Cb[1] ) ;
    pTLineH_1_page18_2 ->SetLineStyle(9); 
    pTLineH_1_page18_2 ->SetLineColor( 4 ) ;
    pTLineH_1_page18_2->Draw();  
    pTCanvas->Update()    ;

  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 19
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_AlphaTauBin_page19   = new TMultiGraph ;  
    pTMultiGraph_AlphaTauBin_page19->Add( V_pTGraph_EC_AlphaTauBin  [0] ) ;

    pTMultiGraph_AlphaTauBin_page19->Draw("A");
    pTMultiGraph_AlphaTauBin_page19->SetTitle("#sigma_{Y}^{#alpha+#tau} VS Track Position");
    pTMultiGraph_AlphaTauBin_page19->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_AlphaTauBin_page19->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha+#tau} (#mu m) ");
    pTMultiGraph_AlphaTauBin_page19->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_AlphaTauBin_page19->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_AlphaTauBin_page19->SetMaximum( 500. );
    pTMultiGraph_AlphaTauBin_page19->SetMinimum(   0. );

    pTCanvas->Update();
    
    TLegend* pTLegend_page19_2 = new TLegend(0.3,0.15 ,0.75,0.25  ); 
    pTLegend_page19_2->SetFillStyle(1001);
    pTLegend_page19_2->SetBorderSize(1);

    pTLegend_page19_2->AddEntry( V_pTGraph_EC_AlphaTauBin  [0]  , V_pTGraph_EC_AlphaTauBin  [0]->GetTitle() , "l");
   
    pTLegend_page19_2->Draw();       
    pTCanvas->Update();
     
    double Xmin_page19 = pTCanvas->cd(1)->GetUxmin() ;
    double Xmax_page19 = pTCanvas->cd(1)->GetUxmax() ;
    TLine* pTLineH_page19_2_1 = new TLine( Xmin_page19  , V_EC_AlphaTauBin_Cb[0] , Xmax_page19 , V_EC_AlphaTauBin_Cb[0] ) ;
    pTLineH_page19_2_1 ->SetLineStyle( 9 ); 
    pTLineH_page19_2_1 ->SetLineColor( 1 ) ;
    pTLineH_page19_2_1->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page19_2_2 = new TLine( Xmin_page19  , V_EC_AlphaTauBin_Cb_Max[0] , Xmax_page19 , V_EC_AlphaTauBin_Cb_Max[0] ) ;
    pTLineH_page19_2_2 ->SetLineStyle( 9 ); 
    pTLineH_page19_2_2 ->SetLineColor( 1 ) ;
    pTLineH_page19_2_2->Draw();  
    pTCanvas->Update()    ;

    TLine* pTLineH_page19_2_3 = new TLine( Xmin_page19  , V_EC_AlphaTauBin_Cb_Min[0] , Xmax_page19 , V_EC_AlphaTauBin_Cb_Min[0] ) ;
    pTLineH_page19_2_3 ->SetLineStyle( 9 ); 
    pTLineH_page19_2_3 ->SetLineColor( 1 ) ;
    pTLineH_page19_2_3->Draw();  
    pTCanvas->Update()    ;
    
  pTCanvas->Print(Str_Out_PLF.c_str());

//Page 20
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    
    TMultiGraph* pTMultiGraph_AlphaTauBin_page20   = new TMultiGraph ;  
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_TauBin_EC_Cb         ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_TauBin_EC_Cb_Max     ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_TauBin_EC_Cb_Min     ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_Alpha_EC_Cb          ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_Alpha_EC_Cb_Max      ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_Alpha_EC_Cb_Min      ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_AlphaTauBin_EC_Cb    ,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_AlphaTauBin_EC_Cb_Max,"l" ) ;
    pTMultiGraph_AlphaTauBin_page20->Add( pTGraph_AlphaTauBin_EC_Cb_Min,"l" ) ;
    
    pTGraph_TauBin_EC_Cb    ->SetLineColor(18) ;
    pTGraph_TauBin_EC_Cb_Max->SetLineColor(18) ;
    pTGraph_TauBin_EC_Cb_Min->SetLineColor(18) ;
    pTGraph_Alpha_EC_Cb     ->SetLineColor(18) ;
    pTGraph_Alpha_EC_Cb_Max ->SetLineColor(18) ;
    pTGraph_Alpha_EC_Cb_Min ->SetLineColor(18) ;

    pTMultiGraph_AlphaTauBin_page20->Draw("A");
    pTMultiGraph_AlphaTauBin_page20->SetTitle("#sigma_{Y}^{#alpha+#tau} VS RC");
    pTMultiGraph_AlphaTauBin_page20->GetXaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_AlphaTauBin_page20->GetYaxis()->SetTitle("#sigma_{Y}^{#alpha+#tau} (#mu m) ");
    pTMultiGraph_AlphaTauBin_page20->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_AlphaTauBin_page20->GetXaxis()->SetLimits( 0., 250.);
    pTMultiGraph_AlphaTauBin_page20->SetMaximum( 450. );
    pTMultiGraph_AlphaTauBin_page20->SetMinimum( 0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page20_2 = new TLegend(0.2,0.15 ,0.4,0.25  ); 
    pTLegend_page20_2->SetFillStyle(1001);
    pTLegend_page20_2->SetBorderSize(1);
    pTLegend_page20_2->AddEntry( pTGraph_AlphaTauBin_EC_Cb      , pTGraph_AlphaTauBin_EC_Cb    ->GetTitle() , "l");
    pTLegend_page20_2->AddEntry( pTGraph_AlphaTauBin_EC_Cb_Max  , pTGraph_AlphaTauBin_EC_Cb_Max->GetTitle() , "l");
    pTLegend_page20_2->Draw(); 

    pTCanvas->Update();

 pTCanvas->Print(Str_Out_PLF.c_str());

//Page 21
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    
    TMultiGraph* pTMultiGraph_page21   = new TMultiGraph ;  
    pTMultiGraph_page21->Add( pTGraph_Amplitude_LR_CentralTrack         ,"l" ) ;
    
    pTMultiGraph_page21->Draw("A");
    pTMultiGraph_page21->SetTitle("A_{L/R} for Central track VS RC");
    pTMultiGraph_page21->GetXaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_page21->GetYaxis()->SetTitle("A_{L/R} (ADC)");
    pTMultiGraph_page21->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_page21->GetXaxis()->SetLimits( 0., 250.);
    pTMultiGraph_page21->SetMaximum( 100. );
    pTMultiGraph_page21->SetMinimum( 0. );
    
    pTCanvas->Update();

 pTCanvas->Print(Str_Out_PLF.c_str());

//Page 22
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1) ;

    pTCanvas->cd(1) ;
    pTCanvas->cd( 1 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_Alpha_page22   = new TMultiGraph ;  
    pTMultiGraph_Alpha_page22->Add( V_pTGraph_Alpha_PR[0] ) ;

    pTMultiGraph_Alpha_page22->Draw("A");
    pTMultiGraph_Alpha_page22->SetTitle("#alpha VS Track Position");
    pTMultiGraph_Alpha_page22->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_Alpha_page22->GetYaxis()->SetTitle("#alpha ");
    pTMultiGraph_Alpha_page22->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_Alpha_page22->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
   
    pTCanvas->Update();
    
    TLegend* pTLegend_page22 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page22->SetFillStyle(1001);
    pTLegend_page22->SetBorderSize(1);

    pTLegend_page22->AddEntry( V_pTGraph_Alpha_PR[0]  , V_pTGraph_Alpha_PR[0]->GetTitle() , "l");
    pTLegend_page22->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_page22_Alpha_H = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                          0                                  , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                         0.01,"|>");
    pTArrow_page22_Alpha_H->SetLineColor( 2 ) ;
    pTArrow_page22_Alpha_H->Draw();
    
    TArrow* pTArrow_page22_Alpha_V = new TArrow((V_pTGraph_Alpha_PR[0]->GetX())[40] , pTCanvas->cd(1)->GetUymin()        ,
                                         (V_pTGraph_Alpha_PR[0]->GetX())[40] , (V_pTGraph_Alpha_PR[0]->GetY())[40],
                                        0.01,"|>");
    pTArrow_page22_Alpha_V->SetLineColor( 2 ) ;
    pTArrow_page22_Alpha_V->Draw();
  
    pTCanvas->Update();

    pTCanvas->cd(2) ;
    pTCanvas->cd( 2 )->SetGrid();
  
    TMultiGraph* pTMultiGraph_TauBin_page22   = new TMultiGraph ;  
    pTMultiGraph_TauBin_page22->Add( V_pTGraph_TauBin_PR[0] ) ;

    pTMultiGraph_TauBin_page22->Draw("A");
    pTMultiGraph_TauBin_page22->SetTitle("#tau VS Track Position");
    pTMultiGraph_TauBin_page22->GetXaxis()->SetTitle("X_{T} (cm)");
    pTMultiGraph_TauBin_page22->GetYaxis()->SetTitle("#tau (tbin)");
    pTMultiGraph_TauBin_page22->GetYaxis()->SetTitleOffset(1.5);
    pTMultiGraph_TauBin_page22->GetXaxis()->SetLimits( -Ly/2. *1.E2  , Ly/2. *1.E2);
    pTMultiGraph_TauBin_page22->SetMaximum( 38. );
    pTMultiGraph_TauBin_page22->SetMinimum(  0. );
    
    pTCanvas->Update();
    
    TLegend* pTLegend_page22_2 = new TLegend(0.3,0.65 ,0.75,0.55  ); 
    pTLegend_page22_2->SetFillStyle(1001);
    pTLegend_page22_2->SetBorderSize(1);

    pTLegend_page22_2->AddEntry( V_pTGraph_TauBin_PR[0]  , V_pTGraph_TauBin_PR[0]->GetTitle() , "l");
    pTLegend_page22_2->Draw(); 
          
    pTCanvas->Update();

    TArrow* pTArrow_page22_TauBin_H = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                            0                                  , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                          0.01,"|>");
    pTArrow_page22_TauBin_H->SetLineColor( 2 ) ;
    pTArrow_page22_TauBin_H->Draw();
    
    TArrow* pTArrow_page22_TauBin_V = new TArrow((V_pTGraph_TauBin_PR[0]->GetX())[40] , pTCanvas->cd(2)->GetUymin()        ,
                                          (V_pTGraph_TauBin_PR[0]->GetX())[40] , (V_pTGraph_TauBin_PR[0]->GetY())[40],
                                          0.01,"|>");
    pTArrow_page22_TauBin_V->SetLineColor( 2 ) ;
    pTArrow_page22_TauBin_V->Draw();
  
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PLF.c_str());


//Page 22
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PLF_End.c_str());

}
