#include "Studies/WF_01.h"

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

void WF_01()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/WF_01.pdf" ; 
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

  pModel_Charge1D->Set_Width(0.);
  
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
  
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,0., YTrue ); 
  }

  Pad* pPad_Center = aROBoard.Get_Pad(0,iYTrue  ) ;
  Pad* pPad_Next   = aROBoard.Get_Pad(0,iYTrue+1) ;
  
//  
  std::vector < TGraph*      > V_pTGraph_ADC     ;
  std::vector < TMultiGraph* > V_TMultiGraph_ADC ;
    
  for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
    Pad* pPad_Cur = 0 ;
    if (iTurn==0) pPad_Cur = pPad_Center ;
    if (iTurn==1) pPad_Cur = pPad_Next   ;

    TGraph* pTGraph_ADC = new TGraph ;
    if (iTurn==0) pTGraph_ADC->SetLineColor( 2 ) ;
    if (iTurn==1) pTGraph_ADC->SetLineColor( 4 ) ;

    V_pTGraph_ADC.push_back(pTGraph_ADC) ;

    TMultiGraph* pTMultiGraph_ADC = new TMultiGraph ;
    pTMultiGraph_ADC->Add(pTGraph_ADC) ;
    V_TMultiGraph_ADC.push_back(pTMultiGraph_ADC) ;
    
    double Tmin =     0.        ;
    double Tmax =  4000. * 1E-9 ; 
    int    Npoints = 1000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
      pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur/40.E-9 , ADC  ) ;
    }

  }

  std::vector < TH1F* > V_pTH1F_ADC_unfloored ;
  std::vector < TH1F* > V_pTH1F_ADC_unsmeared ;
  std::vector < TH1F* > V_pTH1F_ADC_smeared   ;
  std::vector < double > V_YMAX   ;
  std::vector < double > V_YMIN   ;
    
  for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
    Pad* pPad_Cur = 0 ;
    if (iTurn==0) pPad_Cur = pPad_Center ;
    if (iTurn==1) pPad_Cur = pPad_Next   ;

    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << " ADC VS time:" ;
    if (iTurn==0) aostringstream << " Leading Pad/" ;
    if (iTurn==1) aostringstream << " SubLeading Pad/" ;
    std::string Title_unfloored = aostringstream.str() + "Unfloored "         + " ; Time bins ; ADC" ;
    std::string Title_unsmeared = aostringstream.str() + "Floored/Unsmeared " + " ; Time bins ; ADC" ;
    std::string Title_smeared   = aostringstream.str() + "Floored/Smeared "   + " ; Time bins ; ADC" ;
    
    
    TH1F* pTH1F_ADC_unfloored = new TH1F(Title_unfloored.c_str(),Title_unfloored.c_str(),100, -0.5 , 99.5 );
    V_pTH1F_ADC_unfloored.push_back( pTH1F_ADC_unfloored );
    
    TH1F* pTH1F_ADC_unsmeared = new TH1F(Title_unsmeared.c_str(),Title_unsmeared.c_str(),100, -0.5 , 99.5 );
    V_pTH1F_ADC_unsmeared.push_back( pTH1F_ADC_unsmeared );
    
    TH1F* pTH1F_ADC_smeared   = new TH1F(Title_smeared  .c_str(),Title_smeared  .c_str(),100, -0.5 , 99.5 );
    V_pTH1F_ADC_smeared  .push_back( pTH1F_ADC_smeared );
    
    double YMAX = 0. ;
    double YMIN = 0. ;
    int iTimeBin_Max = 100 ;
    for (int iTimeBin = 0 ; iTimeBin < iTimeBin_Max ; iTimeBin++){
      double TimeCur = double(iTimeBin)*40.E-9 ;
     
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
      pTH1F_ADC_unfloored->SetBinContent( iTimeBin+1 ,ADC);     
      
      double ADC_unsmeared = std::floor( ADC  ) ;
      pTH1F_ADC_unsmeared->SetBinContent( iTimeBin+1 ,ADC_unsmeared);     
      
      double ADC_smeared = std::floor( ADC + aTRandom3.Gaus(0.,6.) ) ;
      pTH1F_ADC_smeared->SetBinContent( iTimeBin+1 ,ADC_smeared);     
      
      if (iTimeBin==0){
        YMAX = ADC ;
        YMIN = ADC ;
      }
      if (YMAX<ADC          ) YMAX = ADC ;
      if (YMIN>ADC          ) YMIN = ADC ;
      if (YMAX<ADC_unsmeared) YMAX = ADC_unsmeared ;
      if (YMIN>ADC_unsmeared) YMIN = ADC_unsmeared ;
      if (YMAX<ADC_smeared  ) YMAX = ADC_smeared ;
      if (YMIN>ADC_smeared  ) YMIN = ADC_smeared ;
      
    }
    double TheDiff = YMAX - YMIN ;
    YMAX = YMAX + 0.1 * TheDiff ;
    YMIN = YMIN - 0.1 * TheDiff ;
    V_YMAX.push_back(YMAX);
    V_YMIN.push_back(YMIN);
 
  }
  
  for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
    double YMAX = V_YMAX[iTurn] ;
    double YMIN = V_YMIN[iTurn] ;
    V_pTH1F_ADC_unfloored[iTurn]->SetMaximum(YMAX);V_pTH1F_ADC_unfloored[iTurn]->SetMinimum(YMIN);
    V_pTH1F_ADC_unsmeared[iTurn]->SetMaximum(YMAX);V_pTH1F_ADC_unsmeared[iTurn]->SetMinimum(YMIN);
    V_pTH1F_ADC_smeared  [iTurn]->SetMaximum(YMAX);V_pTH1F_ADC_smeared  [iTurn]->SetMinimum(YMIN);
  }

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,1) ;
    
    for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
      pTCanvas->cd(iTurn+1) ;
        if (iTurn==0) V_TMultiGraph_ADC[iTurn]->SetTitle("Leading Pad: ADC VS time");
        if (iTurn==1) V_TMultiGraph_ADC[iTurn]->SetTitle("Subleading Pad: ADC VS time");
        V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetTitle("Time bins");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitle("ADC");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitleOffset(1.5);
        V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetLimits(-0.5 , 99.5);

        V_TMultiGraph_ADC[iTurn]->Draw("A") ;
        pTCanvas->Update();

        V_TMultiGraph_ADC[iTurn]->SetMaximum( V_YMAX[iTurn] );
        V_TMultiGraph_ADC[iTurn]->SetMinimum( V_YMIN[iTurn] );
      pTCanvas->Update();
    }
 
  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,1) ;
  
    for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
      pTCanvas->cd(iTurn+1) ;
        V_pTH1F_ADC_unfloored  [iTurn]->GetYaxis()->SetTitleOffset(1.5);
        V_pTH1F_ADC_unfloored  [iTurn]->Draw("HIST") ;
        V_pTGraph_ADC[iTurn]->Draw("SAME") ;
      pTCanvas->Update();
    }

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,1) ;
  
    for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
      pTCanvas->cd(iTurn+1) ;
        V_pTH1F_ADC_unsmeared  [iTurn]->GetYaxis()->SetTitleOffset(1.5);
        V_pTH1F_ADC_unsmeared  [iTurn]->Draw("HIST") ;
        V_pTGraph_ADC[iTurn]->Draw("SAME") ;
      pTCanvas->Update();
    }

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,1) ;
  
    for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
      pTCanvas->cd(iTurn+1) ;
        V_pTH1F_ADC_smeared  [iTurn]->GetYaxis()->SetTitleOffset(1.5);
        V_pTH1F_ADC_smeared  [iTurn]->Draw("HIST") ;
        V_pTGraph_ADC[iTurn]->Draw("SAME") ;
      pTCanvas->Update();
    }

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
