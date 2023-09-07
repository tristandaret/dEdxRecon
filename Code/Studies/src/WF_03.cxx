#include "Studies/WF_03.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ReadOutGeometry.h"

#include "Misc/FuncFromTGraph.h"

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

void WF_03()
{
// 
  double SY = 0.0 ;
  
  double RCin = 100. ;

  double G_SigmaT =  40.E-9 ;
  double G_T0     =   0.E-9 ;
  int    G_NberOfSigmas = 5 ;
  int    G_iT_max = 10*2*G_NberOfSigmas ;

//
  double G_Tmin = -double(G_NberOfSigmas)*G_SigmaT + G_T0 ;
  double G_Tmax =  double(G_NberOfSigmas)*G_SigmaT + G_T0 ;
  double G_WidthInterval = (G_Tmax-G_Tmin)/double(G_iT_max) ;
  double G_GLWeight = TMath::Erf( double(G_NberOfSigmas)/std::sqrt(2) )  ;
 
//  
  TCanvas* pTCanvas =  0 ;

  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*         pModel_Charge1D        = new Model_Charge1D        ;

  double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge1D->Set_RC(LocalRC) ;

  pModel_Charge1D->Set_Width(0.);
  
  double TheQValue = pModel_Electronics->Get_QValue() ;
  double TheWS     = pModel_Electronics->Get_ws()     ;
//TheWS = TheWS * 2. ;
  pModel_Electronics->Set_TimeShape(TheQValue,TheWS);
  
  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ; 
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 

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
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge1D , PadName , EventNber,EntryNber,ModuleNber   ,iX ,iY ); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//Compute A^o responses, signal for instantaneous deposit a t=0
  double Time0 = 0. ;
//       Time0 = 5000. * 1E-9 ; 

  int iYTrue = Ny/2 ;
  double YTrue = pModel_ReadOutGeometry->Get_YcPad(0,iYTrue,ModuleNber);

  YTrue += SY * Ly/2. ;
 
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,0., YTrue ); 
  }

  Pad* pPad_Center = aROBoard.Get_Pad(0,iYTrue  ) ;
  Pad* pPad_NextU  = aROBoard.Get_Pad(0,iYTrue+1) ;
  Pad* pPad_NextD  = aROBoard.Get_Pad(0,iYTrue-1) ;
  
//  
  TMultiGraph* pTMultiGraph_ADC = new TMultiGraph ;
  std::vector< TGraph* > V_TGraph ;
  std::vector< FuncFromTGraph* > V_FuncFromTGraph ;

  TLegend* pTLegend = new TLegend(0.4,0.65 ,0.85,0.55  ); 
  pTLegend->SetFillStyle(1001);
  pTLegend->SetBorderSize(1);
  
  std::vector < double > V_AMAX ;
  std::vector < double > V_TMAX ;
  double AMIN_all = 0. ;
  double AMAX_all = 0. ;
  for (int iTurn = 0 ; iTurn< 1 ; iTurn++){
    Pad* pPad_Cur = 0 ;
    if (iTurn==0) pPad_Cur = pPad_Center ;
    if (iTurn==1) pPad_Cur = pPad_NextU  ;
    if (iTurn==2) pPad_Cur = pPad_NextD  ;

    TGraph* pTGraph_ADC = new TGraph ;
    if (iTurn==0) pTGraph_ADC->SetLineColor( 2 ) ;
    if (iTurn==1) pTGraph_ADC->SetLineColor( 4 ) ;
    if (iTurn==2) pTGraph_ADC->SetLineColor( 8 ) ;

    pTMultiGraph_ADC->Add(pTGraph_ADC) ;
    V_TGraph.push_back(pTGraph_ADC);
    
    int    Tmin_in_ns =    0 ;
    int    Tmax_in_ns = 5000 ;
    double Tmin =  double(Tmin_in_ns) * 1E-9 ; 
    double Tmax =  double(Tmax_in_ns) * 1E-9 ; 
    int    Npoints = (Tmax_in_ns-Tmin_in_ns) + 1  ;
    double AMAX = 0. ;
    double TMAX = 0. ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      
      double ADC = pPad_Cur->Get_APad(TimeCur) ;
      
      pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur , ADC  ) ;
      
      if (iPoint==0){
        AMAX = 0. ;
        TMAX = 0. ;
      } 
      if (AMAX<ADC){
        AMAX = ADC ;
        TMAX = TimeCur  ;
      }
      if (iTurn==0&&iPoint==0){
        AMIN_all = ADC ;
        AMAX_all = ADC ;
      }
      if (AMIN_all>ADC) AMIN_all=ADC;
      if (AMAX_all<ADC) AMAX_all=ADC;
   
      
    }
    V_AMAX.push_back(AMAX);
    V_TMAX.push_back(TMAX);
    
    FuncFromTGraph* pFuncFromTGraph = new FuncFromTGraph(pTGraph_ADC) ;
    V_FuncFromTGraph.push_back(pFuncFromTGraph) ;

    std::ostringstream aostringstream_pTGraph_ADC ;
    aostringstream_pTGraph_ADC << std::setiosflags(std::ios::fixed) ;
    aostringstream_pTGraph_ADC << std::setw(8) << "A_{max}="    << std::setw(6) << std::setprecision(1) << AMAX ;
    aostringstream_pTGraph_ADC << std::setw(8) << ", T_{max}="  << std::setw(6) << std::setprecision(3) << TMAX*1E6 << " #mu s";
    aostringstream_pTGraph_ADC << std::setw(1) << "=" << std::setw(6) << std::setprecision(1) << TMAX/40.E-9 << " tb" ;

    pTLegend->AddEntry( pTGraph_ADC , (aostringstream_pTGraph_ADC.str()).c_str() , "l");
    
  }
  double TheDIFF = AMAX_all - AMIN_all ;
  AMAX_all = AMAX_all + 0.1*TheDIFF ;
  AMIN_all = AMIN_all - 0.1*TheDIFF ;


//Compute A response, landding times gaussian spread
  TMultiGraph* pTMultiGraph_ADC_CV = new TMultiGraph ;
  std::vector< TGraph* > V_TGraph_CV ;

  TLegend* pTLegend_CV = new TLegend(0.4,0.65 ,0.85,0.55  ); 
  pTLegend_CV->SetFillStyle(1001);
  pTLegend_CV->SetBorderSize(1);
  
  std::vector < double > V_AMAX_CV ;
  std::vector < double > V_TMAX_CV ;
  for (int iTurn = 0 ; iTurn< 1 ; iTurn++){
    FuncFromTGraph* pFuncFromTGraph = V_FuncFromTGraph[iTurn] ;

    TGraph* pTGraph_ADC = new TGraph ;
    if (iTurn==0) pTGraph_ADC->SetLineColor( 2 ) ;
    if (iTurn==1) pTGraph_ADC->SetLineColor( 4 ) ;
    if (iTurn==2) pTGraph_ADC->SetLineColor( 8 ) ;

    pTMultiGraph_ADC_CV->Add(pTGraph_ADC) ;
    V_TGraph_CV.push_back(pTGraph_ADC);

    int    Tmin_in_ns = -200 ;
    int    Tmax_in_ns = 5000 ;
    double Tmin =  double(Tmin_in_ns) * 1E-9 + G_T0 ; 
    double Tmax =  double(Tmax_in_ns) * 1E-9 + G_T0 ; 
    int    Npoints = (Tmax_in_ns-Tmin_in_ns) + 1  ;
    double AMAX = 0. ;
    double TMAX = 0. ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      
      double ADC= 0. ; 
      for (int G_iT = 0 ; G_iT< G_iT_max ; G_iT++){
        double TheT = G_Tmin + G_WidthInterval/2. + double (G_iT)*G_WidthInterval ;
        double Weight = ( TMath::Erf( (TheT-G_T0+G_WidthInterval/2.)/(std::sqrt(2.)*G_SigmaT)) 
                        - TMath::Erf( (TheT-G_T0-G_WidthInterval/2.)/(std::sqrt(2.)*G_SigmaT)) )/2. ;
        ADC += Weight* pFuncFromTGraph->Get_Y(TimeCur-TheT)/G_GLWeight ;
      }
     
      pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur , ADC  ) ;
      
      if (iPoint==0){
        AMAX = 0. ;
        TMAX = 0. ;
      } 
      if (AMAX<ADC){
        AMAX = ADC ;
        TMAX = TimeCur  ;
      }
      
    }
    V_AMAX_CV.push_back(AMAX);
    V_TMAX_CV.push_back(TMAX);

    std::ostringstream aostringstream_pTGraph_ADC ;
    aostringstream_pTGraph_ADC << std::setiosflags(std::ios::fixed) ;
    aostringstream_pTGraph_ADC << std::setw(8) << "A_{max}="    << std::setw(6) << std::setprecision(1) << AMAX ;
    aostringstream_pTGraph_ADC << std::setw(8) << ", T_{max}="  << std::setw(6) << std::setprecision(3) << TMAX*1E6 << " #mu s";
    aostringstream_pTGraph_ADC << std::setw(1) << "=" << std::setw(6) << std::setprecision(1) << TMAX/40.E-9 << " tb" ;

    pTLegend_CV->AddEntry( pTGraph_ADC , (aostringstream_pTGraph_ADC.str()).c_str() , "l");
    
  }

//Page 1
  std::ostringstream aostringstream_PDF ;
  aostringstream_PDF << std::setiosflags(std::ios::fixed) ;
  aostringstream_PDF << "OUT_Studies/WF_03_";
  aostringstream_PDF << "SY_"<< std::setprecision(1) << SY ;
  aostringstream_PDF << "_RC_"<< std::setprecision(0) << RCin ;
  aostringstream_PDF << ".pdf";
  std::string Str_Out_PDF = aostringstream_PDF.str() ; 

  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());


//Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    std::ostringstream aostringstream_pTMultiGraph_ADC ;
    aostringstream_pTMultiGraph_ADC << std::setiosflags(std::ios::fixed) ;
    aostringstream_pTMultiGraph_ADC << "ADC vs Time" ;
//    aostringstream_pTMultiGraph_ADC << " (" ;
//     aostringstream_pTMultiGraph_ADC << "SY =" << std::setw(3) << std::setprecision(1) << SY ;
//     aostringstream_pTMultiGraph_ADC << ", RC =" << std::setw(5) << std::setprecision(0) << RCin ;
//     aostringstream_pTMultiGraph_ADC << ")";

    pTMultiGraph_ADC->SetTitle((aostringstream_pTMultiGraph_ADC.str()).c_str());
    pTMultiGraph_ADC->GetXaxis()->SetTitle("Time (s)");
    pTMultiGraph_ADC->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_ADC->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_ADC->Draw("A") ;

    pTCanvas->Update();

    pTMultiGraph_ADC->SetMaximum(AMAX_all);
    pTMultiGraph_ADC->SetMinimum(AMIN_all);
    pTMultiGraph_ADC->GetXaxis()->SetLimits( -0.5E-6 , 3.E-6);
    
    pTMultiGraph_ADC->Draw("S") ;
    pTCanvas->Update();

    double Xmin = pTCanvas->GetUxmin() ;
    double Xmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Xmin  , 0. , Xmax , 0. ) ;
    pTLineH ->SetLineStyle(1); 
    pTLineH ->SetLineWidth(1); 
    pTLineH ->SetLineColor( 1 ) ;
    pTLineH->Draw();  
    pTCanvas->Update()    ;
    
    double Ymin = pTCanvas->GetUymin() ;
    double Ymax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Ymin , 0. , Ymax ) ;
    pTLineV ->SetLineStyle(1); 
    pTLineV ->SetLineWidth(1); 
    pTLineV ->SetLineColor( 1 ) ;
    pTLineV->Draw();  
    pTCanvas->Update()    ;
    
    for (int iTurn = 0 ; iTurn< 1; iTurn++){
      double XCur =  V_TMAX[iTurn] ;        
      double YCur =  V_AMAX[iTurn] ;        
      TLine* pTLineV = new TLine( XCur  , 0. , XCur , YCur ) ;
      pTLineV->SetLineStyle(2); 
      pTLineV->SetLineWidth(1); 
      if (iTurn==0) pTLineV->SetLineColor( 2 ) ;
      if (iTurn==1) pTLineV->SetLineColor( 4 ) ;
      if (iTurn==2) pTLineV->SetLineColor( 8 ) ;
      pTLineV->Draw();  
      pTCanvas->Update()    ;
    }

//     pTLegend->Draw(); 
          
    pTCanvas->Update();
    
  pTCanvas->Print(Str_Out_PDF.c_str());

  std::ostringstream aostringstream_PNG_Page_2 ;
  aostringstream_PNG_Page_2 << std::setiosflags(std::ios::fixed) ;
  aostringstream_PNG_Page_2 << "OUT_Studies/WF_03_Page_2_";
  aostringstream_PNG_Page_2 << "SY_"<< std::setprecision(1) << SY ;
  aostringstream_PNG_Page_2 << "_RC_"<< std::setprecision(0) << RCin ;
  aostringstream_PNG_Page_2 << ".svg";
  std::string Str_Out_PNG_Page_2 = aostringstream_PNG_Page_2.str() ; 
  pTCanvas->Print(Str_Out_PNG_Page_2.c_str());

//Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

    std::ostringstream aostringstream_pTMultiGraph_ADC_CV ;
    aostringstream_pTMultiGraph_ADC_CV << std::setiosflags(std::ios::fixed) ;
    aostringstream_pTMultiGraph_ADC_CV << "ADC vs Time (" ;
    aostringstream_pTMultiGraph_ADC_CV << "SY =" << std::setw(3) << std::setprecision(1) << SY ;
    aostringstream_pTMultiGraph_ADC_CV << ", RC =" << std::setw(5) << std::setprecision(0) << RCin  ;
    aostringstream_pTMultiGraph_ADC_CV << ", #sigma_{T} =" << std::setw(5) << std::setprecision(0) << G_SigmaT/1.E-9 << "ns";
    aostringstream_pTMultiGraph_ADC_CV << ")";

    pTMultiGraph_ADC_CV->SetTitle((aostringstream_pTMultiGraph_ADC_CV.str()).c_str());
    pTMultiGraph_ADC_CV->GetXaxis()->SetTitle("Time (s)");
    pTMultiGraph_ADC_CV->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_ADC_CV->GetYaxis()->SetTitleOffset(1.5);

    pTMultiGraph_ADC_CV->Draw("A") ;

    pTCanvas->Update();

    pTMultiGraph_ADC_CV->SetMaximum(AMAX_all);
    pTMultiGraph_ADC_CV->SetMinimum(AMIN_all);
    pTMultiGraph_ADC_CV->GetXaxis()->SetLimits( -1.E-6 , 5.E-6);
    
    pTCanvas->Update();

    pTLineH->Draw();  
    pTCanvas->Update()    ;

    pTLineV->Draw();  
    pTCanvas->Update()    ;
    
    for (int iTurn = 0 ; iTurn< 1; iTurn++){
      double XCur =  V_TMAX_CV[iTurn] ;        
      double YCur =  V_AMAX_CV[iTurn] ;        
      TLine* pTLineV = new TLine( XCur  , 0. , XCur , YCur ) ;
      pTLineV->SetLineStyle(2); 
      pTLineV->SetLineWidth(1); 
      if (iTurn==0) pTLineV->SetLineColor( 2 ) ;
      if (iTurn==1) pTLineV->SetLineColor( 4 ) ;
      if (iTurn==2) pTLineV->SetLineColor( 8 ) ;
      pTLineV->Draw();  
      pTCanvas->Update()    ;
    }

    pTLegend_CV->Draw(); 
          
    pTCanvas->Update();
    
  pTCanvas->Print(Str_Out_PDF.c_str());

  std::ostringstream aostringstream_PNG_Page_3 ;
  aostringstream_PNG_Page_3 << std::setiosflags(std::ios::fixed) ;
  aostringstream_PNG_Page_3 << "OUT_Studies/WF_03_Page_3_";
  aostringstream_PNG_Page_3 << "SY_"<< std::setprecision(1) << SY ;
  aostringstream_PNG_Page_3 << "_RC_"<< std::setprecision(0) << RCin ;
  aostringstream_PNG_Page_3 << ".svg";
  std::string Str_Out_PNG_Page_3 = aostringstream_PNG_Page_3.str() ; 
  pTCanvas->Print(Str_Out_PNG_Page_3.c_str());


//Page 4, 5, 6
    for (int iTurn = 0 ; iTurn< 1 ; iTurn++){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;

      TMultiGraph* pTMultiGraph = new TMultiGraph ;
      pTMultiGraph->Add(V_TGraph[iTurn]) ;
      pTMultiGraph->Add(V_TGraph_CV[iTurn]) ;
      
      pTMultiGraph->GetXaxis()->SetTitle("Time (s)");
      pTMultiGraph->GetYaxis()->SetTitle("ADC");
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

      pTMultiGraph->Draw("A") ;

      pTCanvas->Update();
      
      pTMultiGraph->GetXaxis()->SetLimits( -1.E-6 , 4.E-6);
      pTCanvas->Update();
      
      pTCanvas->Print(Str_Out_PDF.c_str());
    }
    

//Page LAST
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 3600, 2400) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
