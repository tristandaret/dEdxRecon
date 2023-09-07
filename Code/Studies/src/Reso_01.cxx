#include "Studies/Reso_01.h"

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

void Reso_01()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Reso_01.pdf" ; 
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
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge1D , PadName , EventNber,EntryNber,ModuleNber ,iX ,iY ); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//
  double Time0 = 0. ;

  int iYTrue = Ny/2 ;
  double YTrue = pModel_ReadOutGeometry->Get_YcPad(0,iYTrue,ModuleNber);
  double SY = 0. ;
  YTrue += SY * Ly ;
  
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
  std::vector < double  >      V_AMax ;
  std::vector < double  >      V_TMax ;
    
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
    double TheAMax = 0. ;
    double TheTMax = 0. ;
    int    Npoints = 4000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
      pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur/40.E-9 , ADC  ) ;
      if (iPoint==0){
        TheAMax = ADC            ;
        TheTMax = TimeCur/40.E-9 ;
      }
      if (TheAMax<ADC){
        TheAMax = ADC            ;
        TheTMax = TimeCur/40.E-9 ;
      }
    }
    V_AMax.push_back( TheAMax ) ;
    V_TMax.push_back( TheTMax ) ;

  }

  std::vector < TH1F*  > V_pTH1F_ADC_unfloored ; std::vector < double > V_AMax_unfloored ; std::vector < double > V_TMax_unfloored ;
  std::vector < TH1F*  > V_pTH1F_ADC_unsmeared ; std::vector < double > V_AMax_unsmeared ; std::vector < double > V_TMax_unsmeared ;
  std::vector < TH1F*  > V_pTH1F_ADC_smeared   ; std::vector < double > V_AMax_smeared   ; std::vector < double > V_TMax_smeared   ;
  std::vector < double > V_YMAX   ;
  std::vector < double > V_YMIN   ;
    
  for (int iTurn = 0 ; iTurn< 2 ; iTurn++){
    Pad* pPad_Cur = 0 ;
    if (iTurn==0) pPad_Cur = pPad_Center ;
    if (iTurn==1) pPad_Cur = pPad_Next   ;

    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "(SY=  "<< std::setw(5) << std::setprecision(3) << SY <<") " ;
    aostringstream << " ADC VS time:" ;
    if (iTurn==0) aostringstream << " Shot Pad/" ;
    if (iTurn==1) aostringstream << " Next Pad/" ;
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
    double TheAMax_unfloored = 0. ;
    double TheTMax_unfloored = 0. ;
    double TheAMax_unsmeared = 0. ;
    double TheTMax_unsmeared = 0. ;
    double TheAMax_smeared   = 0. ;
    double TheTMax_smeared   = 0. ;
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
      
      if (iTimeBin==0){
        TheAMax_unfloored = ADC           ; TheTMax_unfloored = TimeCur/40.E-9 ;
        TheAMax_unsmeared = ADC_unsmeared ; TheTMax_unsmeared = TimeCur/40.E-9 ;
        TheAMax_smeared   = ADC_smeared   ; TheTMax_smeared   = TimeCur/40.E-9 ;
      }
      
      if (TheAMax_unfloored<ADC          ) { TheAMax_unfloored = ADC           ; TheTMax_unfloored = TimeCur/40.E-9 ; }
      if (TheAMax_unsmeared<ADC_unsmeared) { TheAMax_unsmeared = ADC_unsmeared ; TheTMax_unsmeared = TimeCur/40.E-9 ; }
      if (TheAMax_smeared  <ADC_smeared  ) { TheAMax_smeared   = ADC_smeared   ; TheTMax_smeared   = TimeCur/40.E-9 ; }

      
    }
    double TheDiff = YMAX - YMIN ;
    YMAX = YMAX + 0.1 * TheDiff ;
    YMIN = YMIN - 0.1 * TheDiff ;
    V_YMAX.push_back(YMAX);
    V_YMIN.push_back(YMIN);
    
    V_AMax_unfloored.push_back( TheAMax_unfloored ) ; V_TMax_unfloored.push_back( TheTMax_unfloored ) ;
    V_AMax_unsmeared.push_back( TheAMax_unsmeared ) ; V_TMax_unsmeared.push_back( TheTMax_unsmeared ) ;
    V_AMax_smeared  .push_back( TheAMax_smeared   ) ; V_TMax_smeared  .push_back( TheTMax_smeared   ) ;
 
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
        std::ostringstream aostringstream ;
        aostringstream << std::setiosflags(std::ios::fixed) ;
        aostringstream << "(SY=  "<< std::setw(5) << std::setprecision(3) << SY <<") " ;
        aostringstream << " ADC VS time:" ;
        if (iTurn==0) aostringstream << " Shot Pad " ;
        if (iTurn==1) aostringstream << " Next Pad " ;
        std::string TheTitle = aostringstream.str() ;
        V_TMultiGraph_ADC[iTurn]->SetTitle(TheTitle.c_str());
        V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetTitle("Time bins");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitle("ADC");
        V_TMultiGraph_ADC[iTurn]->GetYaxis()->SetTitleOffset(1.5);
        V_TMultiGraph_ADC[iTurn]->GetXaxis()->SetLimits(-0.5 , 99.5);

        V_TMultiGraph_ADC[iTurn]->SetMaximum( V_YMAX[iTurn] );
        V_TMultiGraph_ADC[iTurn]->SetMinimum( V_YMIN[iTurn] );
        
        V_TMultiGraph_ADC[iTurn]->Draw("A") ;

      pTCanvas->Update();
     
        double minV = pTCanvas->cd(iTurn+1)->GetUymin() ;
        double maxV = pTCanvas->cd(iTurn+1)->GetUymax() ;
        TLine* pTLine_TrueV = new TLine( V_TMax[iTurn]  , minV , V_TMax[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_TrueV->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueV->SetLineColor(4) ;
        pTLine_TrueV->SetLineStyle(1); pTLine_TrueV->Draw();  
        
        double xminH = pTCanvas->cd(iTurn+1)->GetUxmin() ;
        double xmaxH = pTCanvas->cd(iTurn+1)->GetUxmax() ;
        TLine* pTLine_TrueH = new TLine( xminH , V_AMax[iTurn] , xmaxH , V_AMax[iTurn] ) ;
        if (iTurn==0) pTLine_TrueH->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueH->SetLineColor(4) ;
        pTLine_TrueH->SetLineStyle(1);  pTLine_TrueH->Draw();  
        
        if (iTurn==1) {
          TPaveText* pTPaveText = new TPaveText(.5,.5,.7,.6,"NDC");
          std::ostringstream aostringstream2 ;
          aostringstream2 << std::setiosflags(std::ios::fixed) ;
          aostringstream2 << "#rho_{True}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax[1]/V_AMax[0]  ;
          pTPaveText->AddText(( aostringstream2.str()).c_str()) ;
          pTPaveText->Draw();
        }
       
      pTCanvas->Update()    ;
      
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
     
        double minV = pTCanvas->cd(iTurn+1)->GetUymin() ;
        double maxV = pTCanvas->cd(iTurn+1)->GetUymax() ;
        TLine* pTLine_TrueV = new TLine( V_TMax[iTurn]  , minV , V_TMax[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_TrueV->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueV->SetLineColor(4) ;
        pTLine_TrueV->SetLineStyle(1); pTLine_TrueV->Draw();  
        
        double xminH = pTCanvas->cd(iTurn+1)->GetUxmin() ;
        double xmaxH = pTCanvas->cd(iTurn+1)->GetUxmax() ;
        TLine* pTLine_TrueH = new TLine( xminH , V_AMax[iTurn] , xmaxH , V_AMax[iTurn] ) ;
        if (iTurn==0) pTLine_TrueH->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueH->SetLineColor(4) ;
        pTLine_TrueH->SetLineStyle(1);  pTLine_TrueH->Draw();  
        
        TLine* pTLine_LV = new TLine( V_TMax_unfloored[iTurn]  , minV , V_TMax_unfloored[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_LV->SetLineColor(2) ;
        if (iTurn==1) pTLine_LV->SetLineColor(4) ;
        pTLine_LV->SetLineStyle(2); pTLine_LV->Draw();  
        
        TLine* pTLine_LH = new TLine( xminH , V_AMax_unfloored[iTurn] , xmaxH , V_AMax_unfloored[iTurn] ) ;
        if (iTurn==0) pTLine_LH->SetLineColor(2) ;
        if (iTurn==1) pTLine_LH->SetLineColor(4) ;
        pTLine_LH->SetLineStyle(2);  pTLine_LH->Draw();  
        
        if (iTurn==1) {
          TPaveText* pTPaveText = new TPaveText(.5,.5,.7,.65,"NDC");
          std::ostringstream aostringstream2 ;
          aostringstream2 << std::setiosflags(std::ios::fixed) ;
          aostringstream2 << "#rho_{True}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax[1]/V_AMax[0]  ;
          pTPaveText->AddText(( aostringstream2.str()).c_str()) ;
          std::ostringstream aostringstream3 ;
          aostringstream3 << std::setiosflags(std::ios::fixed) ;
          aostringstream3 << "#rho_{Rec}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax_unfloored[1]/V_AMax_unfloored[0]  ;
          pTPaveText->AddText(( aostringstream3.str()).c_str()) ;
          std::ostringstream aostringstream4 ;
          aostringstream4 << std::setiosflags(std::ios::fixed) ;
          aostringstream4 << "Diff =  "<< std::setw(5) << std::setprecision(3) <<
          100.* ( (V_AMax_unfloored[1]/V_AMax_unfloored[0]) - (V_AMax[1]/V_AMax[0]) )/(V_AMax[1]/V_AMax[0]) <<"%";
          pTPaveText->AddText(( aostringstream4.str()).c_str()) ;
          pTPaveText->Draw();
        }
       
      pTCanvas->Update()    ;
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
     
        double minV = pTCanvas->cd(iTurn+1)->GetUymin() ;
        double maxV = pTCanvas->cd(iTurn+1)->GetUymax() ;
        TLine* pTLine_TrueV = new TLine( V_TMax[iTurn]  , minV , V_TMax[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_TrueV->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueV->SetLineColor(4) ;
        pTLine_TrueV->SetLineStyle(1); pTLine_TrueV->Draw();  
        
        double xminH = pTCanvas->cd(iTurn+1)->GetUxmin() ;
        double xmaxH = pTCanvas->cd(iTurn+1)->GetUxmax() ;
        TLine* pTLine_TrueH = new TLine( xminH , V_AMax[iTurn] , xmaxH , V_AMax[iTurn] ) ;
        if (iTurn==0) pTLine_TrueH->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueH->SetLineColor(4) ;
        pTLine_TrueH->SetLineStyle(1);  pTLine_TrueH->Draw();  
        
        TLine* pTLine_LV = new TLine( V_TMax_unsmeared[iTurn]  , minV , V_TMax_unsmeared[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_LV->SetLineColor(2) ;
        if (iTurn==1) pTLine_LV->SetLineColor(4) ;
        pTLine_LV->SetLineStyle(2); pTLine_LV->Draw();  
        
        TLine* pTLine_LH = new TLine( xminH , V_AMax_unsmeared[iTurn] , xmaxH , V_AMax_unsmeared[iTurn] ) ;
        if (iTurn==0) pTLine_LH->SetLineColor(2) ;
        if (iTurn==1) pTLine_LH->SetLineColor(4) ;
        pTLine_LH->SetLineStyle(2);  pTLine_LH->Draw();  
        
        if (iTurn==1) {
          TPaveText* pTPaveText = new TPaveText(.5,.5,.7,.65,"NDC");
          std::ostringstream aostringstream2 ;
          aostringstream2 << std::setiosflags(std::ios::fixed) ;
          aostringstream2 << "#rho_{True}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax[1]/V_AMax[0]  ;
          pTPaveText->AddText(( aostringstream2.str()).c_str()) ;
          std::ostringstream aostringstream3 ;
          aostringstream3 << std::setiosflags(std::ios::fixed) ;
          aostringstream3 << "#rho_{Rec}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax_unsmeared[1]/V_AMax_unsmeared[0]  ;
          pTPaveText->AddText(( aostringstream3.str()).c_str()) ;
          std::ostringstream aostringstream4 ;
          aostringstream4 << std::setiosflags(std::ios::fixed) ;
          aostringstream4 << "Diff =  "<< std::setw(5) << std::setprecision(3) <<
          100.* ( (V_AMax_unsmeared[1]/V_AMax_unsmeared[0]) - (V_AMax[1]/V_AMax[0]) )/(V_AMax[1]/V_AMax[0]) <<"%";
          pTPaveText->AddText(( aostringstream4.str()).c_str()) ;
          pTPaveText->Draw();
        }
        
      pTCanvas->Update()    ;
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
     
        double minV = pTCanvas->cd(iTurn+1)->GetUymin() ;
        double maxV = pTCanvas->cd(iTurn+1)->GetUymax() ;
        TLine* pTLine_TrueV = new TLine( V_TMax[iTurn]  , minV , V_TMax[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_TrueV->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueV->SetLineColor(4) ;
        pTLine_TrueV->SetLineStyle(1); pTLine_TrueV->Draw();  
        
        double xminH = pTCanvas->cd(iTurn+1)->GetUxmin() ;
        double xmaxH = pTCanvas->cd(iTurn+1)->GetUxmax() ;
        TLine* pTLine_TrueH = new TLine( xminH , V_AMax[iTurn] , xmaxH , V_AMax[iTurn] ) ;
        if (iTurn==0) pTLine_TrueH->SetLineColor(2) ;
        if (iTurn==1) pTLine_TrueH->SetLineColor(4) ;
        pTLine_TrueH->SetLineStyle(1);  pTLine_TrueH->Draw();  
        
        TLine* pTLine_LV = new TLine( V_TMax_smeared[iTurn]  , minV , V_TMax_smeared[iTurn] , maxV ) ;
        if (iTurn==0) pTLine_LV->SetLineColor(2) ;
        if (iTurn==1) pTLine_LV->SetLineColor(4) ;
        pTLine_LV->SetLineStyle(2); pTLine_LV->Draw();  
        
        TLine* pTLine_LH = new TLine( xminH , V_AMax_smeared[iTurn] , xmaxH , V_AMax_smeared[iTurn] ) ;
        if (iTurn==0) pTLine_LH->SetLineColor(2) ;
        if (iTurn==1) pTLine_LH->SetLineColor(4) ;
        pTLine_LH->SetLineStyle(2);  pTLine_LH->Draw();  
        
        
        if (iTurn==1) {
          TPaveText* pTPaveText = new TPaveText(.5,.5,.7,.65,"NDC");
          std::ostringstream aostringstream2 ;
          aostringstream2 << std::setiosflags(std::ios::fixed) ;
          aostringstream2 << "#rho_{True}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax[1]/V_AMax[0]  ;
          pTPaveText->AddText(( aostringstream2.str()).c_str()) ;
          std::ostringstream aostringstream3 ;
          aostringstream3 << std::setiosflags(std::ios::fixed) ;
          aostringstream3 << "#rho_{Rec}^{L} =  "<< std::setw(5) << std::setprecision(3) << V_AMax_smeared[1]/V_AMax_smeared[0]  ;
          pTPaveText->AddText(( aostringstream3.str()).c_str()) ;
          std::ostringstream aostringstream4 ;
          aostringstream4 << std::setiosflags(std::ios::fixed) ;
          aostringstream4 << "Diff =  "<< std::setw(5) << std::setprecision(3) <<
          100.* ( (V_AMax_smeared[1]/V_AMax_smeared[0]) - (V_AMax[1]/V_AMax[0]) )/(V_AMax[1]/V_AMax[0]) <<"%";
          pTPaveText->AddText(( aostringstream4.str()).c_str()) ;
          pTPaveText->Draw();
        }

      pTCanvas->Update()    ;
    }

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
