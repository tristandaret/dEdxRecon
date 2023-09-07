#include "Studies/Toy_01.h"

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

void Toy_01()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Toy_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

// 
  TRandom3 aTRandom3 ;

  gStyle->SetOptStat(0);
  
  int Nrange = 5 ;
  double DeltaYT = 10. ;
  
  int iEvent_Max = Nrange*100000 ;
  
  double YTmax  = DeltaYT * double (Nrange+1)/2. ;
  double YTmin  = -YTmax ;
  int    YTNbin = int(YTmax-YTmin)*5 ;
  
  std::string Name_Gene  = "pTH1F_Gene";
  std::string Title_Gene = "Generated Y_{T}";
  TH1F* pTH1F_Gene = new TH1F(Name_Gene.c_str(),Title_Gene.c_str(),YTNbin, YTmin , YTmax );

  std::string Name_Smeared  = "pTH1F_Smeared";
  std::string Title_Smeared = "Smeared Y_{T}";
  TH1F* pTH1F_Smeared = new TH1F(Name_Smeared.c_str(),Title_Smeared.c_str(),YTNbin, YTmin , YTmax );

  for (int iEvent = 0 ; iEvent< iEvent_Max; iEvent++){
    double TheRndm =  aTRandom3.Rndm(0) ;
    double TheYT_Gene = (TheRndm-0.5)*2.*DeltaYT*double(Nrange)/2. ;
    pTH1F_Gene->Fill( TheYT_Gene ) ;
    double TheSigma = (  3.*(std::cos(2.*M_PI*TheYT_Gene/10.)-1.) )/10.;
    double TheYT_Smeared = TheYT_Gene + aTRandom3.Gaus(0.,TheSigma) ;
    pTH1F_Smeared->Fill( TheYT_Smeared ) ;
  }

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  
    pTH1F_Gene->Draw("HIST") ;

    pTCanvas->Update();
    
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    for (int iV = -3 ; iV< 3; iV++){
      double YCur = ( double(iV) + 0.5 )* DeltaYT   ;        
      TLine* pTLineV = new TLine( YCur , Vmin , YCur, Vmax ) ;
      pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
      pTCanvas->Update()    ;
    }
    
  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  
    pTH1F_Smeared->Draw("HIST") ;

    pTCanvas->Update();
    
   Vmin = pTCanvas->GetUymin() ;
   Vmax = pTCanvas->GetUymax() ;
    for (int iV = -3 ; iV< 3; iV++){
      double YCur = ( double(iV) + 0.5 )* DeltaYT   ;        
      TLine* pTLineV = new TLine( YCur , Vmin , YCur, Vmax ) ;
      pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
      pTCanvas->Update()    ;
    }
    
  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
