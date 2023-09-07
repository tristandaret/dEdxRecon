#include "Studies/DEDX_cor_01.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/SC_1D_VS_x_rc_dd.h"

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

void DEDX_cor_01()
{
  SC_1D_VS_x_rc_dd aSC_1D_VS_x_rc_dd("Data_SignalTime_1D/Load_1D_400ns_VS_x_rc_dd.csv") ;

//
  Model_ReadOutGeometry       aModel_ReadOutGeometry         ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;
  double LX= aModel_ReadOutGeometry.Get_LY() ;

  double vY_min = -0.5 * LY * 1.E2 ;
  double vY_max =  0.5 * LY * 1.E2 ;

//
  double Primary_Charge = 200. * 1.E2 * LX * 1.602176565E-19 * 1E3  ;
  double ADC_Pulse = Primary_Charge * 4096./ 120.E-15 ;
  double T_Pulse = 412. ;
  std::cout 
    << " ADC_Pulse = " << std::setw(5) << std::setprecision(0) << ADC_Pulse  
    << " T_Pulse   = " << std::setw(5) << std::setprecision(0) << T_Pulse 
    << std::endl ;

  double Cor_A_Ref = 1.20 ;  
  double Cor_T_Ref =  30. ;  
//
  TMultiGraph* pTMultiGraph_A   = new TMultiGraph();
  TMultiGraph* pTMultiGraph_ANP = new TMultiGraph();
  pTMultiGraph_A   ->SetTitle( "ADC_{Leading} VS Track position" )  ; 
  pTMultiGraph_ANP ->SetTitle( "ADC_{Pulse}/ADC_{Leading} VS Track position" )  ; 
  
  TMultiGraph* pTMultiGraph_T   = new TMultiGraph();
  TMultiGraph* pTMultiGraph_TNP = new TMultiGraph();
  pTMultiGraph_T   ->SetTitle( "T_{Leading} VS Track position" )  ; 
  pTMultiGraph_TNP ->SetTitle( "T_{Pulse}-T_{Leading} VS Track position" )  ; 
  
//
  for (int iRC = 0 ; iRC< 3 ; iRC++){
  for (int iDD = 0 ; iDD< 3 ; iDD++){
    double RCin = 100. ; // in ns mm^-2 
    double DDin =   0. ; // in mm 
    
    if (iRC==0) RCin = 50. ;
    if (iRC==1) RCin =100. ;
    if (iRC==2) RCin =150. ;

    if (iDD==0) DDin =   0. ;
    if (iDD==1) DDin = 500. ;
    if (iDD==2) DDin =1000. ;

//
    TGraph* pTGraph_A   = new TGraph() ; 
    TGraph* pTGraph_ANP = new TGraph() ; 

    pTMultiGraph_A   ->Add( pTGraph_A   ,"l");
    pTMultiGraph_ANP ->Add( pTGraph_ANP ,"l");

//
    TGraph* pTGraph_T   = new TGraph() ; 
    TGraph* pTGraph_TNP = new TGraph() ; 
    
    pTMultiGraph_T   ->Add( pTGraph_T   ,"l");
    pTMultiGraph_TNP ->Add( pTGraph_TNP ,"l");

//
    if (iRC==0) { pTGraph_A->SetLineColor(2) ; pTGraph_ANP->SetLineColor(2) ; pTGraph_T->SetLineColor(2) ; pTGraph_TNP->SetLineColor(2) ; }
    if (iRC==1) { pTGraph_A->SetLineColor(4) ; pTGraph_ANP->SetLineColor(4) ; pTGraph_T->SetLineColor(4) ; pTGraph_TNP->SetLineColor(4) ; }
    if (iRC==2) { pTGraph_A->SetLineColor(3) ; pTGraph_ANP->SetLineColor(3) ; pTGraph_T->SetLineColor(3) ; pTGraph_TNP->SetLineColor(3) ; }
 
    int Npoints = 1000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double vY_cur = vY_min + double (iPoint)*(vY_max-vY_min)/double(Npoints-1) ;
      
      double vCur = aSC_1D_VS_x_rc_dd.Am_LE(vY_cur, RCin,DDin) ; 
      pTGraph_A   ->SetPoint( pTGraph_A   ->GetN(), vY_cur , vCur            ) ;
      pTGraph_ANP ->SetPoint( pTGraph_ANP ->GetN(), vY_cur , ADC_Pulse/vCur  ) ;
      
      vCur = aSC_1D_VS_x_rc_dd.Ti_LE_Base(vY_cur, RCin,DDin) ; 
      pTGraph_T   ->SetPoint( pTGraph_T   ->GetN(), vY_cur , vCur*1000          ) ;
      pTGraph_TNP ->SetPoint( pTGraph_TNP ->GetN(), vY_cur , T_Pulse-vCur*1000  ) ;

    }
    std::cout 
      << " RC = " << std::setw(6) << std::setprecision(0) << RCin 
      << " DD = " << std::setw(6) << std::setprecision(0) << DDin
      << " Avg_ANP = " << std::setw(6) << std::setprecision(2) << pTGraph_ANP->GetMean(2)  
      << " ( " << std::setw(6) << std::setprecision(2) << pTGraph_ANP->GetMean(2)-Cor_A_Ref << ")" 
      << " Avg_TNP = " << std::setw(6) << std::setprecision(2) << pTGraph_TNP->GetMean(2) 
      << " ( " << std::setw(6) << std::setprecision(2) << pTGraph_TNP->GetMean(2)-Cor_T_Ref << ")" 
      << std::endl ;
      
  }
  }
  
//
  TCanvas* pTCanvas =  0 ;
  
  std::ostringstream aostringstream_PDF ;
  aostringstream_PDF << std::setiosflags(std::ios::fixed) ;
  aostringstream_PDF << "OUT_Studies/DEDX_cor_01_";
  aostringstream_PDF << ".pdf";
  std::string Str_Out_PDF = aostringstream_PDF.str() ; 

  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());


//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_A->GetXaxis()->SetTitle("Y_{Track} (cm)");

    pTMultiGraph_A->GetYaxis()->SetTitle("ADC_{Leading}");  
    pTMultiGraph_A->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph_A->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_ANP->GetXaxis()->SetTitle("Y_{Track} (cm)");

    pTMultiGraph_ANP->GetYaxis()->SetTitle("ADC_{Pulse}/ADC_{Leading}");  
    pTMultiGraph_ANP->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph_ANP->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//Page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_T->GetXaxis()->SetTitle("Y_{Track} (cm)");

    pTMultiGraph_T->GetYaxis()->SetTitle("T_{Leading} (ns) ");  
    pTMultiGraph_T->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph_T->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    pTMultiGraph_TNP->GetXaxis()->SetTitle("Y_{Track} (cm)");

    pTMultiGraph_TNP->GetYaxis()->SetTitle("T_{Pulse}-T_{Leading} (ns) ");  
    pTMultiGraph_TNP->GetYaxis()->SetTitleOffset(1.5);
    
    pTMultiGraph_TNP->Draw("A") ;

    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//Page LAST
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
