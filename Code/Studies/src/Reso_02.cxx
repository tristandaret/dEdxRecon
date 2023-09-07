#include "Studies/Reso_02.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/SC_VS_x_rc_dd.h"
#include "SignalShape/SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF.h"
#include "SignalShape/SC_VS_x_rc_dd_SET_rc_dd_4TF1_RhoPRF.h"

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

void Reso_02()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Reso_02.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

// 
  TRandom3 aTRandom3 ;

  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*         pModel_Charge1D        = new Model_Charge1D        ;

  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ; 
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 
  
  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 
  
  double RC_in_ns_per_mm2 = 100 ;
  pModel_Charge1D->Set_RC(RC_in_ns_per_mm2 * 1E-9 / ( 1E-3 * 1E-3)) ;
  
  double DD_inmm = 0. ;
  pModel_Charge1D->Set_WidthFromDriftDistance(DD_inmm/10.) ;

//
  std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;
  SC_VS_x_rc_dd aSC_VS_x_rc_dd(FILE_SC_VS_x_rc_dd) ;
  SC_VS_x_rc_dd_SET_rc_dd_4TF1_RhoPRF aSC_VS_x_rc_dd_SET_rc_dd_4TF1_RhoPRF(&aSC_VS_x_rc_dd,RC_in_ns_per_mm2,DD_inmm) ;
  SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF aSC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF(&aSC_VS_x_rc_dd,RC_in_ns_per_mm2,DD_inmm) ;
    
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
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge1D , PadName , EventNber,EntryNber,ModuleNber  ,iX ,iY ); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//
  double Time0 = 0. ;

  int iYTrue = Ny/2 ;
  double YTrue = pModel_ReadOutGeometry->Get_YcPad(0,iYTrue,ModuleNber);
  double SY = 0.1 ;
  YTrue += SY * Ly/2. ;
  std::cout << std::endl ;
  std::cout << " YTrue " << YTrue << std::endl ;
  std::cout << std::endl ;
  
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,0., YTrue ); 
  }

  Pad* pPad_Center = aROBoard.Get_Pad(0,iYTrue  ) ;
  Pad* pPad_Up     = aROBoard.Get_Pad(0,iYTrue+1) ;
  Pad* pPad_Dw     = aROBoard.Get_Pad(0,iYTrue-1) ;
  
//  
  TGraph* pTGraph_RhoCurv = new TGraph ;
  TMultiGraph* pTMultiGraph_RhoCurv = new TMultiGraph ;
  TGraph* pTGraph_RhoCurv_Data = new TGraph ;

  std::vector < TGraph*      > V_pTGraph_ADC     ;
  std::vector < TMultiGraph* > V_TMultiGraph_ADC ;
  std::vector < double  >      V_AMax ;
  std::vector < double  >      V_TMax ;
  std::vector < double > V_YPAD     ;
  std::vector < double > V_YTYPAD   ;
  std::vector < double > V_RHO      ;
  std::vector < double > V_eRHO     ;
  std::vector < double > V_eYTYPAD_Found ;
  std::vector < double > V_YTYPAD_Found ;
  std::vector < double > V_YTYPADSlope_Found ;
  std::vector < double > V_YMAX   ;
  std::vector < double > V_YMIN   ;
    
  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    Pad* pPad_Cur = 0 ;
    if (iTurn==0) pPad_Cur = pPad_Dw     ;
    if (iTurn==1) pPad_Cur = pPad_Center ;
    if (iTurn==2) pPad_Cur = pPad_Up     ;

    TGraph* pTGraph_ADC = new TGraph ;
    if (iTurn==0) pTGraph_ADC->SetLineColor( 2 ) ;
    if (iTurn==1) pTGraph_ADC->SetLineColor( 3 ) ;
    if (iTurn==2) pTGraph_ADC->SetLineColor( 4 ) ;
    V_YPAD.push_back  ( 1.E2*pPad_Cur->Get_YPad() ) ;
    V_YTYPAD.push_back( 1.E2*(YTrue - pPad_Cur->Get_YPad()) ) ;

    V_pTGraph_ADC.push_back(pTGraph_ADC) ;

    TMultiGraph* pTMultiGraph_ADC = new TMultiGraph ;
    pTMultiGraph_ADC->Add(pTGraph_ADC) ;
    V_TMultiGraph_ADC.push_back(pTMultiGraph_ADC) ;
    
    double Tmin =     0.        ;
    double Tmax =  4000. * 1E-9 ; 
    double TheAMax = 0. ;
    double TheTMax = 0. ;
    double YMAX = 0. ;
    double YMIN = 0. ;
    int    Npoints = 4000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
      pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur/1.E-6 , ADC  ) ;
      
      if (iPoint==0){
        TheAMax = ADC            ;
        TheTMax = TimeCur/1.E-6 ;
      }
      if (TheAMax<ADC){
        TheAMax = ADC            ;
        TheTMax = TimeCur/1.E-6 ;
      }

      if (iPoint==0){
        YMAX = ADC ;
        YMIN = ADC ;
      }
      if (YMAX<ADC) YMAX = ADC ;
      if (YMIN>ADC) YMIN = ADC ;

    }

    V_AMax.push_back( TheAMax ) ;
    V_TMax.push_back( TheTMax ) ;

    double TheDiff = YMAX - YMIN ;
    YMAX = YMAX + 0.1 * TheDiff ;
    YMIN = YMIN - 0.1 * TheDiff ;
    V_YMAX.push_back(YMAX);
    V_YMIN.push_back(YMIN);

  }

  double A_Leading = 0. ;
  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    if ( A_Leading < V_AMax[iTurn] ) A_Leading = V_AMax[iTurn] ;
  }
  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    V_RHO.push_back(V_AMax[iTurn]/A_Leading) ;
    pTGraph_RhoCurv_Data->SetPoint( pTGraph_RhoCurv_Data->GetN(), V_YTYPAD[iTurn] , V_RHO[iTurn]  ) ;
  } 
  pTGraph_RhoCurv_Data->SetMarkerStyle(20) ;
  pTGraph_RhoCurv_Data->SetMarkerColor(2) ;
  pTGraph_RhoCurv_Data->SetMarkerSize(1.8) ;

  pTMultiGraph_RhoCurv->Add(pTGraph_RhoCurv_Data,"p");

//PRF curve  
  double Ymax_RhoCurv =  1.E2*3*Ly/2.   ;
  double Ymin_RhoCurv = -Ymax_RhoCurv ;
  int iTem_RhoCurv_Max = int ( (Ymax_RhoCurv-Ymin_RhoCurv)/(1.E2*1.E-6) ) ;
  for (int iTem_RhoCurv = 0 ; iTem_RhoCurv< iTem_RhoCurv_Max; iTem_RhoCurv++){
    double YCur = Ymin_RhoCurv + double(iTem_RhoCurv)*(Ymax_RhoCurv-Ymin_RhoCurv)/double(iTem_RhoCurv_Max-1);
    double TheValue = aSC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF.Eval(YCur) ;
    pTGraph_RhoCurv->SetPoint( pTGraph_RhoCurv->GetN(), YCur , TheValue  ) ;
  }
  pTMultiGraph_RhoCurv->Add(pTGraph_RhoCurv,"l");

//
  int     Npoints_in_pTGraph_RhoCurv = pTGraph_RhoCurv->GetN() ;
  double*       X_in_pTGraph_RhoCurv = pTGraph_RhoCurv->GetX() ;
  double*       Y_in_pTGraph_RhoCurv = pTGraph_RhoCurv->GetY() ;

  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    double TheRho = V_RHO[iTurn] ;
    int TheSign = 0. ;
    int First   = 1  ;
    double TheYTYPAD_Found = 0. ;
    double TheYTYPADSlope_Found = 0. ;
    int iCurve_Max = Npoints_in_pTGraph_RhoCurv ;
    for (int iCurve = 0 ; iCurve< iCurve_Max ; iCurve++){
      double TheX=X_in_pTGraph_RhoCurv[iCurve] ;
      if (TheX<0.) continue ;
      double TheY=Y_in_pTGraph_RhoCurv[iCurve] ;
      double TheDiff = TheY - TheRho ;
      if (First==1){
        First   = 0  ;
        TheSign = 1. ;
        if ( TheDiff < 0.) TheSign = -1. ;
      }
      TheDiff *= TheSign ;
      if (TheDiff<0) {
        double TheX1 = X_in_pTGraph_RhoCurv[iCurve-1] ;
        double TheX2 = X_in_pTGraph_RhoCurv[iCurve  ] ;
        double TheY1 = Y_in_pTGraph_RhoCurv[iCurve-1] ;
        double TheY2 = Y_in_pTGraph_RhoCurv[iCurve  ] ;
        TheYTYPAD_Found = TheX1 +  (TheRho-TheY1)*(TheX2-TheX1)/(TheY2-TheY1) ;
        TheYTYPADSlope_Found = (TheY2-TheY1)/(TheX2-TheX1) ;
      }
      if (TheDiff<0) break ;
    }
    V_YTYPAD_Found.push_back( TheYTYPAD_Found ) ;
    V_YTYPADSlope_Found.push_back( TheYTYPADSlope_Found ) ;

  }

  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    double error = 0. ;
    for (int iTurn2 = 0 ; iTurn2< 3 ; iTurn2++){
      if (iTurn2==iTurn) continue ;
      error += std::pow( V_RHO[iTurn2]/std::sqrt(V_AMax[iTurn2]), 2 ) ;
    }
    error +=  std::pow( (1.-V_RHO[iTurn])/std::sqrt(V_AMax[iTurn]) , 2 ) ;
    V_eRHO.push_back( V_RHO[iTurn]*std::sqrt(error) );
    V_eYTYPAD_Found.push_back ( V_eRHO[iTurn] /V_YTYPADSlope_Found[iTurn] );
  }
  
  std::cout << std::endl ;
  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    std::cout 
      << " iTurn "     << iTurn 
      << " YTYPAD "    << V_YTYPAD[iTurn] 
      << " rho "       << V_RHO[iTurn] 
      << " erho "      << V_eRHO[iTurn] 
      << " TheYTYPAD_Found " << V_YTYPAD_Found[iTurn]
      << " TheYTYPADSlope_Found " << V_YTYPADSlope_Found[iTurn]
      << "  " << 1./V_YTYPADSlope_Found[iTurn]
      << " EYT " << V_eYTYPAD_Found[iTurn]/(1.E2*1E-6 ) 
      << " TheSol+   " << ( ( V_YTYPAD_Found[iTurn] + V_YPAD[iTurn])/1E2 - YTrue )*1E6 
      << " TheSol-   " << ( (-V_YTYPAD_Found[iTurn] + V_YPAD[iTurn])/1E2 - YTrue )*1E6
      << std::endl ;
  }
  std::cout << std::endl ;
 
  std::cout << std::endl ;
  for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
    double error = 0. ;
    for (int iTurn2 = 0 ; iTurn2< 3 ; iTurn2++){
      if (iTurn2==iTurn) continue ;
      error += std::pow( 1./V_eYTYPAD_Found[iTurn2] , 2) ;
    }
    error = std::sqrt(1./error) ;
    std::cout 
      << " iTurn " << iTurn
      << " error " << error/(1.E2*1E-6 )
      << std::endl ;
  }
  double error = 0. ;
  for (int iTurn2 = 0 ; iTurn2< 3 ; iTurn2++){
    error += std::pow( 1./V_eYTYPAD_Found[iTurn2] , 2) ;
  }
  error = std::sqrt(1./error) ;
  std::cout 
    << " TOT "  
    << " error " << error/(1.E2*1E-6 )
    << std::endl ;
  std::cout << std::endl ;
  
 
 
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,1) ;
    
    for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
      pTCanvas->cd(iTurn+1) ;
        
        std::ostringstream aostringstream ;
        aostringstream << std::setiosflags(std::ios::fixed) ;
        aostringstream << "(SY=  "<< std::setw(5) << std::setprecision(3) << SY <<") " ;
        aostringstream << " ADC VS time:" ;
        if (iTurn==0) aostringstream << " Dw Pad " ;
        if (iTurn==1) aostringstream << " Shot Pad " ;
        if (iTurn==2) aostringstream << " Up Pad " ;
        std::string TheTitle = aostringstream.str() ;

        V_TMultiGraph_ADC[iTurn]->SetTitle(TheTitle.c_str());
        V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetTitle("Time (#mu s)");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitle("ADC");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitleOffset(1.5);
//      V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetLimits(-0.5 , 99.5);

        V_TMultiGraph_ADC[iTurn]->SetMaximum( V_YMAX[iTurn] );
        V_TMultiGraph_ADC[iTurn]->SetMinimum( V_YMIN[iTurn] );
        
        V_TMultiGraph_ADC[iTurn]->Draw("A") ;

      pTCanvas->Update();
     
        double minV = pTCanvas->cd(iTurn+1)->GetUymin() ;
        double maxV = pTCanvas->cd(iTurn+1)->GetUymax() ;
        TLine* pTLine_TrueV = new TLine( V_TMax[iTurn]  , minV , V_TMax[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_TrueV->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueV->SetLineColor(3) ;
        if (iTurn==2) pTLine_TrueV->SetLineColor(4) ;
        pTLine_TrueV->SetLineStyle(1); pTLine_TrueV->Draw();  
        
        double xminH = pTCanvas->cd(iTurn+1)->GetUxmin() ;
        double xmaxH = pTCanvas->cd(iTurn+1)->GetUxmax() ;
        TLine* pTLine_TrueH = new TLine( xminH , V_AMax[iTurn] , xmaxH , V_AMax[iTurn] ) ;
        if (iTurn==0) pTLine_TrueH->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueH->SetLineColor(3) ;
        if (iTurn==2) pTLine_TrueH->SetLineColor(4) ;
        pTLine_TrueH->SetLineStyle(1);  pTLine_TrueH->Draw();  
        
        TPaveText* pTPaveText = new TPaveText(.5,.5,.7,.6,"NDC");
        std::ostringstream aostringstream2 ;
        aostringstream2 << std::setiosflags(std::ios::fixed) ;
        aostringstream2 << "#rho =  "<< std::setw(5) << std::setprecision(3) << V_AMax[iTurn]/(V_AMax[0]+V_AMax[1]+V_AMax[2])  ;
        pTPaveText->AddText(( aostringstream2.str()).c_str()) ;
        pTPaveText->Draw();
       
      pTCanvas->Update()    ;
      
    }
 
  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTMultiGraph_RhoCurv->Draw("A") ;
    
    pTCanvas->Update();
    
  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
