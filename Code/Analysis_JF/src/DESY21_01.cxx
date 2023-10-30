#include "Analysis_JF/DESY21_01.h"
#include "Misc/Util.h"
#include "Misc/GaussFunction.h"

#include "Analysis_JF/ReadRootFile.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "Analysis_JF/Analysis_Files.h"

#include "Analysis_JF/PerSample.h"

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
#include "TFile.h"
#include "TROOT.h"
#include "TRandom3.h"

void DESY21_01()
{ 
    DESY21_01_RC_PV3_ZSCAN_01 () ;
  
    DESY21_01_RESO_ZSCAN_01() ;
    DESY21_01_RC_ZSCAN_01  () ;

//  DESY21_01_RESO_ZSCAN_200_01() ;
    DESY21_01_RC_ZSCAN_200_01  () ;

}

void DESY21_01_RC_PV3_ZSCAN_01()
{
  std::string SampleSet = "D21_ZSCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "DESY21_01_RC_PV3_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph_All = new TMultiGraph();      
  pTMultiGraph_All->SetTitle("PV3 improved setting");
  
  TLegend* pTLegend_All = new TLegend(0.12,0.15 ,0.87,0.50); 
  pTLegend_All->SetBorderSize(1);
  pTLegend_All->SetMargin(0.1) ;
  
  int    DIFF_Min_First = 1  ;
  double DIFF_Min       = 0. ;
  double RC_Min         = 0. ;

//
  std::vector< double > v_AVG_Mod0 ; std::vector< double > v_DIFF_Mod0 ;
  std::vector< double > v_CTD      ;  

  int iProc_Min =  1 ;
  int iProc_Max = 11 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    if ( iProc ==  1 ) Procedure = "PV3" ;
    if ( iProc ==  2 ) Procedure = "PV3" ;
    
    if ( iProc ==  3 ) Procedure = "PV3" ;
    if ( iProc ==  4 ) Procedure = "PV3" ;
    if ( iProc ==  5 ) Procedure = "PV3" ;
    if ( iProc ==  6 ) Procedure = "PV3" ;
    if ( iProc ==  7 ) Procedure = "PV3" ;
    if ( iProc ==  8 ) Procedure = "PV3" ;
    if ( iProc ==  9 ) Procedure = "PV3" ;
    if ( iProc == 10 ) Procedure = "PV3" ;
    
    if ( iProc ==  3 ) v_CTD.push_back( 260. ) ;
    if ( iProc ==  4 ) v_CTD.push_back( 270. ) ;
    if ( iProc ==  5 ) v_CTD.push_back( 280. ) ;
    if ( iProc ==  6 ) v_CTD.push_back( 290. ) ;
    if ( iProc ==  7 ) v_CTD.push_back( 300. ) ;
    if ( iProc ==  8 ) v_CTD.push_back( 310. ) ;
    if ( iProc ==  9 ) v_CTD.push_back( 320. ) ;
    if ( iProc == 10 ) v_CTD.push_back( 330. ) ;
        
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      TMultiGraph* pTMultiGraph = new TMultiGraph();      

      TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
      pTLegend->SetBorderSize(1);
      pTLegend->SetMargin(0.1) ;
  
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;

      if ( iProc ==  1 ) aostringstream_pTMultiGraph << "Standard "   ;
      if ( iProc ==  2 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=300 #mu m/#sqrt{cm}"   ;

      if ( iProc ==  3 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=260 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  4 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=270 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  5 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=280 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  6 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=290 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  7 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=300 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  8 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=310 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  9 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=320 #mu m/#sqrt{cm}"   ;   
      if ( iProc == 10 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=330 #mu m/#sqrt{cm}"   ;

      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      pTMultiGraph->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
        std::string SampleSet_Specific = "D21_ZSCAN" ;
        
        Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

        std::string AnalyseName = "BIDON" ;
        
        std::ostringstream aostringstream_AnalyseName ;
        aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
        aostringstream_AnalyseName << Procedure  ;
        aostringstream_AnalyseName << "_"  ;
        
        if ( iProc ==  1 ) aostringstream_AnalyseName <<  "D21_ZSCAN" ;
        if ( iProc ==  2 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_300_00" ;
        
        if ( iProc ==  3 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_260_00" ;
        if ( iProc ==  4 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_270_00" ;
        if ( iProc ==  5 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_280_00" ;
        if ( iProc ==  6 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_290_00" ;
        if ( iProc ==  7 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_300_00" ;
        if ( iProc ==  8 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_310_00" ;
        if ( iProc ==  9 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_320_00" ;
        if ( iProc == 10 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_330_00" ;
        
        aostringstream_AnalyseName << "_Module_Nber_"   ;
        aostringstream_AnalyseName << ModuleNber   ;

        AnalyseName = aostringstream_AnalyseName.str() ;  ;


        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        pTGraphErrors->SetMarkerSize(1.8) ;

        pTGraphErrors->SetMarkerStyle(24) ;

        pTGraphErrors->SetMarkerColor( 2 ) ; 

        pTGraphErrors->SetLineColor( 2 ) ; 

        if (iProc ==  3 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetLineColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetLineColor( 3 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerStyle( 21 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerColor( 3 ) ;

        double RC_min   = 0 ;
        double RC_max   = 0 ;
        int    RC_first = 1 ;

        pTMultiGraph    ->Add( pTGraphErrors , "pl" );
        if ( iProc >=3 )  
        pTMultiGraph_All->Add( pTGraphErrors , "pl" );
        
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double ZScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
    
          TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
    
          double  TheValue =  pTF1_OK->GetParameter(0) ;
          double eTheValue =  pTF1_OK->GetParError (0) ;

          int IptCur_Mean  = pTGraphErrors ->GetN() ;
          pTGraphErrors ->SetPoint     ( IptCur_Mean , ZScan , TheValue ) ;
          pTGraphErrors ->SetPointError( IptCur_Mean , 0.    ,eTheValue ) ;
          
          if (RC_first==1){
            RC_first = 0 ;
            RC_min = TheValue ;
            RC_max = TheValue ;
          }
          if (RC_min>TheValue) RC_min = TheValue ;
          if (RC_max<TheValue) RC_max = TheValue ;
          
        }
        
        double AVG =  (RC_max+RC_min)/2. ;
        double DIFF = 100.*(RC_max-RC_min)/AVG ;
        DIFF = DIFF/2. ;
        
        if ( iProc >=  3 ){
          if (iMod==0) { v_AVG_Mod0.push_back(AVG ) ; v_DIFF_Mod0.push_back(DIFF ) ; }
          if (DIFF_Min_First==1){
            DIFF_Min_First = 0    ;
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          if (DIFF_Min>DIFF){
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          
        }

        std::ostringstream aostringstream_pTLegend ;
        aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

        if ( iProc ==  1 ) aostringstream_pTLegend << "Standard "   ;
        if ( iProc ==  2 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;

        if ( iProc ==  3 ) aostringstream_pTLegend << "C_{TD}=260 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  4 ) aostringstream_pTLegend << "C_{TD}=270 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  5 ) aostringstream_pTLegend << "C_{TD}=280 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  6 ) aostringstream_pTLegend << "C_{TD}=290 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  7 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  8 ) aostringstream_pTLegend << "C_{TD}=310 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  9 ) aostringstream_pTLegend << "C_{TD}=320 #mu m/#sqrt{cm}"   ;   
        if ( iProc == 10 ) aostringstream_pTLegend << "C_{TD}=330 #mu m/#sqrt{cm}"   ;

        aostringstream_pTLegend << ", RC = "   ; 
        aostringstream_pTLegend << std::setprecision(0) << AVG   ;
        aostringstream_pTLegend << " ns mm^{-2} "  ;
        aostringstream_pTLegend << " +/- "   ;
        aostringstream_pTLegend << std::setprecision(0) << DIFF    ;
        aostringstream_pTLegend << " %"   ;

        pTLegend    ->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl " );
        if ( iProc >=3 )  
        pTLegend_All->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl" );

//--------------------RC_VS_Z-------------------------//
      
//--------------------RC_VS_Z-------------------------// 

      pTMultiGraph ->Draw("A");
      
      pTMultiGraph->SetMinimum(  0. );
      pTMultiGraph->SetMaximum(150. );
      pTMultiGraph->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update()    ;

      double Vmin = pTCanvas->GetUymin() ;
      double Vmax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
      pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

      pTCanvas->Update()    ;

      pTLegend ->Draw();
      
      pTCanvas->Update();

//--------------------RC_VS_Z-------------------------//
    }
    
    pTCanvas->Print(Str_Out_PDF.c_str());

  }
  
//--------------------RC_VS_Z-------------------------// 

  std::cout << " DIFF_Min " << DIFF_Min << std::endl ;
  std::cout << " RC_Min   " << RC_Min   << std::endl ;
  pTMultiGraph_All ->Draw("A");
  
  pTMultiGraph_All->SetMinimum(  0. );
  pTMultiGraph_All->SetMaximum(150. );
  pTMultiGraph_All->GetXaxis()->SetLimits(-200.,900.);
  
  pTMultiGraph_All->GetXaxis()->SetTitle("Z (mm)");
  pTMultiGraph_All->GetYaxis()->SetTitle("RC (ns mm^{-2})");
  pTMultiGraph_All->GetYaxis()->SetTitleOffset(1.5);
      
  pTCanvas->Update()    ;

  double Vmin = pTCanvas->GetUymin() ;
  double Vmax = pTCanvas->GetUymax() ;
  TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
  pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

  pTCanvas->Update()    ;

  double Hmin = pTCanvas->GetUxmin() ;
  double Hmax = pTCanvas->GetUxmax() ;
  TLine* pTLineH = new TLine( Hmin  , RC_Min , Hmax , RC_Min ) ;
  pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
  pTCanvas->Update()    ;

  pTLegend_All ->Draw();
  
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------RC_VS_Z-------------------------//

//
  int iTem_Max = v_CTD.size()  ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::cout 
      << " v_CTD " << v_CTD[iTem]
      << " v_AVG_Mod0 " << std::setw(4) << std::setprecision(0) << v_AVG_Mod0[iTem] << " v_DIFF_Mod0 " << std::setw(4) << std::setprecision(0) << v_DIFF_Mod0[iTem]
      << std::endl ;
  }
  
//
   pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
   pTCanvas->Divide(2,1) ;
   
   int iMod_Min = 0 ;
   int iMod_Max = 1 ;
   for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      TMultiGraph* pTMultiGraph_AVG  = new TMultiGraph();      
      TMultiGraph* pTMultiGraph_DIFF = new TMultiGraph();      

      std::ostringstream aostringstream_pTMultiGraph_AVG ;
      aostringstream_pTMultiGraph_AVG << " Module "   ;
      aostringstream_pTMultiGraph_AVG << iMod   ;
      pTMultiGraph_AVG->SetTitle((aostringstream_pTMultiGraph_AVG.str()).c_str());

      std::ostringstream aostringstream_pTMultiGraph_DIFF ;
      aostringstream_pTMultiGraph_DIFF << " Module "   ;
      aostringstream_pTMultiGraph_DIFF << iMod   ;
      pTMultiGraph_DIFF->SetTitle((aostringstream_pTMultiGraph_DIFF.str()).c_str());

//
      TGraph* pTGraph_AVG  = new TGraph ;
      TGraph* pTGraph_DIFF = new TGraph ;

      pTGraph_AVG ->SetMarkerSize(1.8) ;
      pTGraph_DIFF->SetMarkerSize(1.8) ;
      
      pTGraph_AVG ->SetMarkerColor( 2 ) ; 
      pTGraph_DIFF->SetMarkerColor( 2 ) ; 

      pTGraph_AVG ->SetMarkerStyle(24) ;
      pTGraph_DIFF->SetMarkerStyle(24) ;
      
      pTGraph_AVG ->SetLineColor( 2 ) ; 
      pTGraph_DIFF->SetLineColor( 2 ) ; 

      pTMultiGraph_AVG ->Add( pTGraph_AVG  , "pl" );
      pTMultiGraph_DIFF->Add( pTGraph_DIFF , "pl" );
      
      int iTem_Max = v_CTD.size()  ;
      for (int iTem = 0 ; iTem< iTem_Max; iTem++){
        double AVG = 0 ; 
        double DIFF = 0 ; 
        if (iMod==0) { AVG = v_AVG_Mod0[iTem] ; DIFF = v_DIFF_Mod0[iTem] ; }

        int IptCur_AVG  = pTGraph_AVG ->GetN() ;
        pTGraph_AVG ->SetPoint ( IptCur_AVG , v_CTD[iTem] , AVG     ) ;
   
        int IptCur_DIFF  = pTGraph_DIFF ->GetN() ;
        pTGraph_DIFF ->SetPoint ( IptCur_DIFF , v_CTD[iTem] , DIFF     ) ;
      }
      
      pTCanvas->cd(iMod+1) ;
 
      pTMultiGraph_AVG ->Draw("A");
      
      pTMultiGraph_AVG->SetMinimum(  0. );
      pTMultiGraph_AVG->SetMaximum(150. );
      pTMultiGraph_AVG->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_AVG->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_AVG->GetYaxis()->SetTitle("(Max+Min)/2 (ns mm^{-2})");
      pTMultiGraph_AVG->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

//
      pTCanvas->cd(iMod+2) ;
 
      pTMultiGraph_DIFF ->Draw("A");
      
      pTMultiGraph_DIFF->SetMinimum(  0. );
      pTMultiGraph_DIFF->SetMaximum( 10. );
      pTMultiGraph_DIFF->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_DIFF->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_DIFF->GetYaxis()->SetTitle("(Max-Min)/2 (%)");
      pTMultiGraph_DIFF->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

   }
   pTCanvas->Print(Str_Out_PDF.c_str());
   
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void DESY21_01_RC_ZSCAN_01()
{
  std::string SampleSet = "D21_ZSCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "DESY21_01_RC_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph_All = new TMultiGraph();      
  pTMultiGraph_All->SetTitle("PV2 improved setting");
  
  TLegend* pTLegend_All = new TLegend(0.12,0.15 ,0.87,0.50); 
  pTLegend_All->SetBorderSize(1);
  pTLegend_All->SetMargin(0.1) ;
  
  int    DIFF_Min_First = 1  ;
  double DIFF_Min       = 0. ;
  double RC_Min         = 0. ;

//
  std::vector< double > v_AVG_Mod0 ; std::vector< double > v_DIFF_Mod0 ;
  std::vector< double > v_CTD      ;  

  int iProc_Min =  1 ;
  int iProc_Max = 11 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    if ( iProc ==  1 ) Procedure = "PV2" ;
    if ( iProc ==  2 ) Procedure = "PV2" ;
    
    if ( iProc ==  3 ) Procedure = "PV2" ;
    if ( iProc ==  4 ) Procedure = "PV2" ;
    if ( iProc ==  5 ) Procedure = "PV2" ;
    if ( iProc ==  6 ) Procedure = "PV2" ;
    if ( iProc ==  7 ) Procedure = "PV2" ;
    if ( iProc ==  8 ) Procedure = "PV2" ;
    if ( iProc ==  9 ) Procedure = "PV2" ;
    if ( iProc == 10 ) Procedure = "PV2" ;
    
    if ( iProc ==  3 ) v_CTD.push_back( 260. ) ;
    if ( iProc ==  4 ) v_CTD.push_back( 270. ) ;
    if ( iProc ==  5 ) v_CTD.push_back( 280. ) ;
    if ( iProc ==  6 ) v_CTD.push_back( 290. ) ;
    if ( iProc ==  7 ) v_CTD.push_back( 300. ) ;
    if ( iProc ==  8 ) v_CTD.push_back( 310. ) ;
    if ( iProc ==  9 ) v_CTD.push_back( 320. ) ;
    if ( iProc == 10 ) v_CTD.push_back( 330. ) ;
        
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      TMultiGraph* pTMultiGraph = new TMultiGraph();      

      TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
      pTLegend->SetBorderSize(1);
      pTLegend->SetMargin(0.1) ;
  
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;

      if ( iProc ==  1 ) aostringstream_pTMultiGraph << "Standard "   ;
      if ( iProc ==  2 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=310 #mu m/#sqrt{cm}"   ;

      if ( iProc ==  3 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=260 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  4 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=270 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  5 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=280 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  6 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=290 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  7 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=300 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  8 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=310 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  9 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=320 #mu m/#sqrt{cm}"   ;   
      if ( iProc == 10 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=330 #mu m/#sqrt{cm}"   ;

      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      pTMultiGraph->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
        std::string SampleSet_Specific = "D21_ZSCAN" ;
        
        Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

        std::string AnalyseName = "BIDON" ;
        
        std::ostringstream aostringstream_AnalyseName ;
        aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
        aostringstream_AnalyseName << Procedure  ;
        aostringstream_AnalyseName << "_"  ;
        
        if ( iProc ==  1 ) aostringstream_AnalyseName <<  "D21_ZSCAN" ;
        if ( iProc ==  2 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_310_00" ;
        
        if ( iProc ==  3 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_260_00" ;
        if ( iProc ==  4 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_270_00" ;
        if ( iProc ==  5 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_280_00" ;
        if ( iProc ==  6 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_290_00" ;
        if ( iProc ==  7 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_300_00" ;
        if ( iProc ==  8 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_310_00" ;
        if ( iProc ==  9 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_320_00" ;
        if ( iProc == 10 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_330_00" ;
        
        aostringstream_AnalyseName << "_Module_Nber_"   ;
        aostringstream_AnalyseName << ModuleNber   ;

        AnalyseName = aostringstream_AnalyseName.str() ;  ;


        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        pTGraphErrors->SetMarkerSize(1.8) ;

        pTGraphErrors->SetMarkerStyle(24) ;

        pTGraphErrors->SetMarkerColor( 2 ) ; 

        pTGraphErrors->SetLineColor( 2 ) ; 

        if (iProc ==  3 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetLineColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetLineColor( 3 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerStyle( 21 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerColor( 3 ) ;

        double RC_min   = 0 ;
        double RC_max   = 0 ;
        int    RC_first = 1 ;

        pTMultiGraph    ->Add( pTGraphErrors , "pl" );
        if ( iProc >=3 )  
        pTMultiGraph_All->Add( pTGraphErrors , "pl" );
        
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double ZScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
    
          TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
    
          double  TheValue =  pTF1_OK->GetParameter(0) ;
          double eTheValue =  pTF1_OK->GetParError (0) ;

          int IptCur_Mean  = pTGraphErrors ->GetN() ;
          pTGraphErrors ->SetPoint     ( IptCur_Mean , ZScan , TheValue ) ;
          pTGraphErrors ->SetPointError( IptCur_Mean , 0.    ,eTheValue ) ;
          
          if (RC_first==1){
            RC_first = 0 ;
            RC_min = TheValue ;
            RC_max = TheValue ;
          }
          if (RC_min>TheValue) RC_min = TheValue ;
          if (RC_max<TheValue) RC_max = TheValue ;
          
        }
        
        double AVG =  (RC_max+RC_min)/2. ;
        double DIFF = 100.*(RC_max-RC_min)/AVG ;
        DIFF = DIFF/2. ;
        
        if ( iProc >=  3 ){
          if (iMod==0) { v_AVG_Mod0.push_back(AVG ) ; v_DIFF_Mod0.push_back(DIFF ) ; }
          if (DIFF_Min_First==1){
            DIFF_Min_First = 0    ;
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          if (DIFF_Min>DIFF){
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          
        }

        std::ostringstream aostringstream_pTLegend ;
        aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

        if ( iProc ==  1 ) aostringstream_pTLegend << "Standard "   ;
        if ( iProc ==  2 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;

        if ( iProc ==  3 ) aostringstream_pTLegend << "C_{TD}=260 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  4 ) aostringstream_pTLegend << "C_{TD}=270 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  5 ) aostringstream_pTLegend << "C_{TD}=280 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  6 ) aostringstream_pTLegend << "C_{TD}=290 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  7 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  8 ) aostringstream_pTLegend << "C_{TD}=310 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  9 ) aostringstream_pTLegend << "C_{TD}=320 #mu m/#sqrt{cm}"   ;   
        if ( iProc == 10 ) aostringstream_pTLegend << "C_{TD}=330 #mu m/#sqrt{cm}"   ;

        aostringstream_pTLegend << ", RC = "   ; 
        aostringstream_pTLegend << std::setprecision(0) << AVG   ;
        aostringstream_pTLegend << " ns mm^{-2} "  ;
        aostringstream_pTLegend << " +/- "   ;
        aostringstream_pTLegend << std::setprecision(0) << DIFF    ;
        aostringstream_pTLegend << " %"   ;

        pTLegend    ->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl " );
        if ( iProc >=3 )  
        pTLegend_All->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl" );


//--------------------RC_VS_Z-------------------------//
      
//--------------------RC_VS_Z-------------------------// 

      pTMultiGraph ->Draw("A");
      
      pTMultiGraph->SetMinimum(  0. );
      pTMultiGraph->SetMaximum(150. );
      pTMultiGraph->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update()    ;

      double Vmin = pTCanvas->GetUymin() ;
      double Vmax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
      pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

      pTCanvas->Update()    ;

      pTLegend ->Draw();
      
      pTCanvas->Update();

//--------------------RC_VS_Z-------------------------//
    }
    
    pTCanvas->Print(Str_Out_PDF.c_str());

  }
  
//--------------------RC_VS_Z-------------------------// 
  pTMultiGraph_All ->Draw("A");
  
  pTMultiGraph_All->SetMinimum(  0. );
  pTMultiGraph_All->SetMaximum(150. );
  pTMultiGraph_All->GetXaxis()->SetLimits(-200.,900.);
  
  pTMultiGraph_All->GetXaxis()->SetTitle("Z (mm)");
  pTMultiGraph_All->GetYaxis()->SetTitle("RC (ns mm^{-2})");
  pTMultiGraph_All->GetYaxis()->SetTitleOffset(1.5);
      
  pTCanvas->Update()    ;

  double Vmin = pTCanvas->GetUymin() ;
  double Vmax = pTCanvas->GetUymax() ;
  TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
  pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

  pTCanvas->Update()    ;

  double Hmin = pTCanvas->GetUxmin() ;
  double Hmax = pTCanvas->GetUxmax() ;
  TLine* pTLineH = new TLine( Hmin  , RC_Min , Hmax , RC_Min ) ;
  pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
  pTCanvas->Update()    ;

  pTLegend_All ->Draw();
  
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------RC_VS_Z-------------------------//

//
  int iTem_Max = v_CTD.size()  ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::cout 
      << " v_CTD " << v_CTD[iTem]
      << " v_AVG_Mod0 " << std::setw(4) << std::setprecision(0) << v_AVG_Mod0[iTem] << " v_DIFF_Mod0 " << std::setw(4) << std::setprecision(0) << v_DIFF_Mod0[iTem]
      << std::endl ;
  }
  
//
   pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
   pTCanvas->Divide(2,1) ;
   
   int iMod_Min = 0 ;
   int iMod_Max = 1 ;
   for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      TMultiGraph* pTMultiGraph_AVG  = new TMultiGraph();      
      TMultiGraph* pTMultiGraph_DIFF = new TMultiGraph();      

      std::ostringstream aostringstream_pTMultiGraph_AVG ;
      aostringstream_pTMultiGraph_AVG << " Module "   ;
      aostringstream_pTMultiGraph_AVG << iMod   ;
      pTMultiGraph_AVG->SetTitle((aostringstream_pTMultiGraph_AVG.str()).c_str());

      std::ostringstream aostringstream_pTMultiGraph_DIFF ;
      aostringstream_pTMultiGraph_DIFF << " Module "   ;
      aostringstream_pTMultiGraph_DIFF << iMod   ;
      pTMultiGraph_DIFF->SetTitle((aostringstream_pTMultiGraph_DIFF.str()).c_str());

//
      TGraph* pTGraph_AVG  = new TGraph ;
      TGraph* pTGraph_DIFF = new TGraph ;

      pTGraph_AVG ->SetMarkerSize(1.8) ;
      pTGraph_DIFF->SetMarkerSize(1.8) ;
      
      pTGraph_AVG ->SetMarkerColor( 2 ) ; 
      pTGraph_DIFF->SetMarkerColor( 2 ) ; 

      pTGraph_AVG ->SetMarkerStyle(24) ;
      pTGraph_DIFF->SetMarkerStyle(24) ;
      
      pTGraph_AVG ->SetLineColor( 2 ) ; 
      pTGraph_DIFF->SetLineColor( 2 ) ; 

      pTMultiGraph_AVG ->Add( pTGraph_AVG  , "pl" );
      pTMultiGraph_DIFF->Add( pTGraph_DIFF , "pl" );
      
      int iTem_Max = v_CTD.size()  ;
      for (int iTem = 0 ; iTem< iTem_Max; iTem++){
        double AVG = 0 ; 
        double DIFF = 0 ; 
        if (iMod==0) { AVG = v_AVG_Mod0[iTem] ; DIFF = v_DIFF_Mod0[iTem] ; }

        int IptCur_AVG  = pTGraph_AVG ->GetN() ;
        pTGraph_AVG ->SetPoint ( IptCur_AVG , v_CTD[iTem] , AVG     ) ;
   
        int IptCur_DIFF  = pTGraph_DIFF ->GetN() ;
        pTGraph_DIFF ->SetPoint ( IptCur_DIFF , v_CTD[iTem] , DIFF     ) ;
      }
      
      pTCanvas->cd(iMod+1) ;
 
      pTMultiGraph_AVG ->Draw("A");
      
      pTMultiGraph_AVG->SetMinimum(  0. );
      pTMultiGraph_AVG->SetMaximum(150. );
      pTMultiGraph_AVG->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_AVG->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_AVG->GetYaxis()->SetTitle("(Max+Min)/2 (ns mm^{-2})");
      pTMultiGraph_AVG->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

//
      pTCanvas->cd(iMod+2) ;
 
      pTMultiGraph_DIFF ->Draw("A");
      
      pTMultiGraph_DIFF->SetMinimum(  0. );
      pTMultiGraph_DIFF->SetMaximum( 10. );
      pTMultiGraph_DIFF->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_DIFF->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_DIFF->GetYaxis()->SetTitle("(Max-Min)/2 (%)");
      pTMultiGraph_DIFF->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

   }
   pTCanvas->Print(Str_Out_PDF.c_str());
   
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void DESY21_01_RESO_ZSCAN_01()
{
  std::string SampleSet = "D21_ZSCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "DESY21_01_RESO_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_VS_Z ;
  std::vector < TLegend* >     V_pTLegend_Sigma_VS_Z     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Sigma_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Mean_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Mean_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Bias_VS_Z ;
  
  int iAna_Min = 0 ;
  int iAna_Max = 5 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    std::string Procedure = "BIDON" ;
    if (iAna==0) Procedure = "PV0" ;
    if (iAna==1) Procedure = "PV2" ;
    if (iAna==2) Procedure = "PV2" ;
    if (iAna==3) Procedure = "PV3" ;
    if (iAna==4) Procedure = "PV3" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      if ( iAna == 1) aostringstream_pTMultiGraph << " Standard "   ;
      if ( iAna == 2) aostringstream_pTMultiGraph << " Improved "   ;
      if ( iAna == 3) aostringstream_pTMultiGraph << " Standard "   ;
      if ( iAna == 4) aostringstream_pTMultiGraph << " Improved "   ;
      aostringstream_pTMultiGraph << " "   ;
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "D21_ZSCAN" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
        
      if ( iAna == 0 ) aostringstream_AnalyseName <<  "D21_ZSCAN" ;
      if ( iAna == 1 ) aostringstream_AnalyseName <<  "D21_ZSCAN" ;
      if ( iAna == 2 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_310_00" ;
      if ( iAna == 3 ) aostringstream_AnalyseName <<  "D21_ZSCAN" ;
      if ( iAna == 4 ) aostringstream_AnalyseName <<  "D21_ZSCAN_TD_300_00" ;
        
      aostringstream_AnalyseName << "_Module_Nber_"   ;
      aostringstream_AnalyseName << ModuleNber   ;

      AnalyseName = aostringstream_AnalyseName.str() ;  ;

      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------Sigma_VS_Z-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_VS_Z = new TMultiGraph();      
      pTMultiGraph_Sigma_VS_Z->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Sigma_VS_Z = new TLegend(0.16,0.15 ,0.88,0.20); 
      pTLegend_Sigma_VS_Z->SetBorderSize(0);
      pTLegend_Sigma_VS_Z->SetMargin(0.1) ;

      V_pTMultiGraph_Sigma_VS_Z.push_back(pTMultiGraph_Sigma_VS_Z)  ; 
      V_pTLegend_Sigma_VS_Z    .push_back(pTLegend_Sigma_VS_Z    )  ; 
 
      TGraphErrors* pTGraphErrors_Sigma_VS_Z = new TGraphErrors ;
      pTGraphErrors_Sigma_VS_Z->SetMarkerSize(1.8) ;

      pTGraphErrors_Sigma_VS_Z->SetMarkerStyle(24) ;

      pTGraphErrors_Sigma_VS_Z->SetMarkerColor( 2 ) ; 

      pTGraphErrors_Sigma_VS_Z->SetLineColor( 2 ) ; 

      pTMultiGraph_Sigma_VS_Z->Add( pTGraphErrors_Sigma_VS_Z , "p" );
      
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
          double Mean = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetMean(2) ;
          double RMS  = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetRMS(2)  ;
          double TheN = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()  ;
                                                    
          double RMSLocal = RMS ;
          RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
          
          std::cout 
            << " ZScan    " << ZScan
            << " Mean     " << Mean 
            << " RMS      " << RMS  
            << " TheN     " << TheN 
            << " RMSLocal " << RMSLocal
            << " " << std::setw(40) << base_EventFile
            << std::endl ;
            
          int IptCur_Mean  = pTGraphErrors_Sigma_VS_Z ->GetN() ;
          pTGraphErrors_Sigma_VS_Z ->SetPoint     ( IptCur_Mean , ZScan , Mean     ) ;
          pTGraphErrors_Sigma_VS_Z ->SetPointError( IptCur_Mean , 0.    , RMSLocal ) ;
        }
      }
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*((x+40)/10.) /[1])",-137.46, 1000.);
      pTF1->SetParName(0,"sigma0");
      pTF1->SetParName(1,"Neff");    
      pTF1->SetParameter(0,100.);
      pTF1->SetParameter(1,100.);    
      pTF1->SetLineColor(2) ;
      pTGraphErrors_Sigma_VS_Z->Fit(pTF1, "R");

      std::ostringstream aostringstream_pTLegend_Sigma_VS_Z ;
      aostringstream_pTLegend_Sigma_VS_Z << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Sigma_VS_Z 
                              << "#sigma_{0} = " << std::setprecision(0) << pTF1->GetParameter(0)
                              << " +/- "        << std::setprecision(0) << pTF1->GetParError(0)
                              << "  N_{eff} = " << std::setprecision(0) << pTF1->GetParameter(1)
                              << " +/- "        << std::setprecision(0) << pTF1->GetParError(1)
                              << "  #chi^{2}_{min}/dof = " 
                                                << std::setprecision(0) << pTF1->GetChisquare()
                              << "/"            << std::setprecision(0) << pTF1->GetNDF()
                              ;
                                             
      pTLegend_Sigma_VS_Z->AddEntry( pTGraphErrors_Sigma_VS_Z , (aostringstream_pTLegend_Sigma_VS_Z.str()).c_str() , "pl");
      
//--------------------Sigma_VS_Z-------------------------//
      
//--------------------Sigma_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = new TMultiGraph();      
      pTMultiGraph_Sigma_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.16,0.15 ,0.88,0.30); 
      pTLegend_Sigma_Residual_VS_Col->SetBorderSize(0);
      pTLegend_Sigma_Residual_VS_Col->SetMargin(0.1) ;

      V_pTMultiGraph_Sigma_Residual_VS_Col.push_back(pTMultiGraph_Sigma_Residual_VS_Col)  ; 
      V_pTLegend_Sigma_Residual_VS_Col    .push_back(pTLegend_Sigma_Residual_VS_Col    )  ; 
 
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineStyle( 1 ) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerSize(1.4) ;
        
          pTMultiGraph_Sigma_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col, "pl") ;
         
          std::ostringstream aostringstream_pTLegend_Sigma_Residual_VS_Col ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << "Z ="  ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << std::setprecision(2) << ZScan  ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << " (mm)"   ;

          std::string pTLegend_Sigma_Residual_VS_Col_String = aostringstream_pTLegend_Sigma_Residual_VS_Col.str() ;  ;

          pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , pTLegend_Sigma_Residual_VS_Col_String.c_str()  , "pl");
        } 
                  
      }
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Mean_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = new TMultiGraph();      
      pTMultiGraph_Mean_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Mean_Residual_VS_Col = new TLegend(0.16,0.15 ,0.88,0.30); 
      pTLegend_Mean_Residual_VS_Col->SetBorderSize(0);
      pTLegend_Mean_Residual_VS_Col->SetMargin(0.1) ;

      V_pTMultiGraph_Mean_Residual_VS_Col.push_back(pTMultiGraph_Mean_Residual_VS_Col)  ; 
      V_pTLegend_Mean_Residual_VS_Col    .push_back(pTLegend_Mean_Residual_VS_Col    )  ; 
 
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineStyle( 1 ) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerSize(1.1) ;
        
          pTMultiGraph_Mean_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col, "pl") ;
         
          std::ostringstream aostringstream_pTLegend_Mean_Residual_VS_Col ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << "Z ="  ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << std::setprecision(2) << ZScan  ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << " (mm)"   ;

          std::string pTLegend_Mean_Residual_VS_Col_String = aostringstream_pTLegend_Mean_Residual_VS_Col.str() ;  ;

          pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , pTLegend_Mean_Residual_VS_Col_String.c_str()  , "pl");
        } 
                  
      }
//--------------------Mean_Residual_VS_Col-------------------------//

//--------------------Bias_VS_Z-------------------------//
    TMultiGraph* pTMultiGraph_Bias_VS_Z = new TMultiGraph();      
    pTMultiGraph_Bias_VS_Z->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

    V_pTMultiGraph_Bias_VS_Z.push_back(pTMultiGraph_Bias_VS_Z)  ; 

    TGraphErrors* pTGraphErrors_Bias_VS_Z = new TGraphErrors ;
    pTGraphErrors_Bias_VS_Z->SetMarkerStyle(20) ;
    pTGraphErrors_Bias_VS_Z->SetMarkerColor( 2 ) ; 
    pTGraphErrors_Bias_VS_Z->SetMarkerSize(1.8) ;
    pTMultiGraph_Bias_VS_Z->Add( pTGraphErrors_Bias_VS_Z , "p" );

      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);
        
        if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
          double Mean = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetMean(2) ;
          double RMS  = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetRMS(2)  ;
          double TheN = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN()     ;

//        Work on Absolute value of the means!!!                                                
          int     Npoints_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN() ;
          double*       X_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetX() ;
          double*       Y_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetY() ;
          double*      EY_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetEY() ;
          TGraphErrors* pTGraphErrors_Copy = new TGraphErrors ;
          for (int ipt = 0; ipt< Npoints_in_Original; ipt++){
            int IptCur_Mean  = pTGraphErrors_Copy ->GetN() ;
            pTGraphErrors_Copy ->SetPoint     ( IptCur_Mean , X_in_Original[ipt] , std::fabs(Y_in_Original[ipt] )    ) ;
            pTGraphErrors_Copy ->SetPointError( IptCur_Mean , 0.                 ,           EY_in_Original[ipt]     ) ;
          }
          Mean = pTGraphErrors_Copy->GetMean(2) ;
          RMS  = pTGraphErrors_Copy->GetRMS(2)  ;
          TheN = pTGraphErrors_Copy->GetN()     ;
          
          double RMSLocal = RMS ;
          RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
          
          int IptCur_Mean  = pTGraphErrors_Bias_VS_Z ->GetN() ;
          pTGraphErrors_Bias_VS_Z ->SetPoint     ( IptCur_Mean , ZScan , Mean      ) ;
          pTGraphErrors_Bias_VS_Z ->SetPointError( IptCur_Mean , 0.    , RMSLocal  ) ;
                     
          pTMultiGraph_Bias_VS_Z->Add( pTGraphErrors_Bias_VS_Z , "p" );
          
        } 
        
      }
//--------------------Bias_VS_Z-------------------------//
    }
    
  }
 
  int Kounter = 0;

//--------------------Mean_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-300. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 300. );
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
      
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitle("Mean of Residuals (#mu m)");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      
      pTLegend_Mean_Residual_VS_Col->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Mean_Residual_VS_Col-------------------------//

//--------------------Bias_VS_Z-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum( 100. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Bias_VS_Z->GetYaxis()->SetTitle("Mean of Absolute Values of Mean of Residuals (#mu m)");
      pTMultiGraph_Bias_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Bias_VS_Z-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col  = V_pTMultiGraph_Sigma_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Sigma_Residual_VS_Col      = V_pTLegend_Sigma_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Sigma_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Sigma_Residual_VS_Col->SetMinimum(   0. );
      pTMultiGraph_Sigma_Residual_VS_Col->SetMaximum( 500. );
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
      
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitle("Sigma of Residuals (#mu m)");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      
      pTLegend_Sigma_Residual_VS_Col->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Sigma_Residual_VS_Col-------------------------//


//--------------------Sigma_VS_Z-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->GetUymin() ;
      double Vmax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
      pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
      pTCanvas->Update()    ;

      pTLegend_Sigma_VS_Z->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Sigma_VS_Z-------------------------//
  
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void DESY21_01_RC_ZSCAN_200_01()
{
  std::string SampleSet = "D21_ZSCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "DESY21_01_RC_ZSCAN_200_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph_All = new TMultiGraph();      
  pTMultiGraph_All->SetTitle("PV2 improved setting");
  
  TLegend* pTLegend_All = new TLegend(0.12,0.15 ,0.87,0.50); 
  pTLegend_All->SetBorderSize(1);
  pTLegend_All->SetMargin(0.1) ;
  
  int    DIFF_Min_First = 1  ;
  double DIFF_Min       = 0. ;
  double RC_Min         = 0. ;

//
  std::vector< double > v_AVG_Mod0 ; std::vector< double > v_DIFF_Mod0 ;
  std::vector< double > v_CTD      ;  

  int iProc_Min =  1 ;
  int iProc_Max = 11 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    if ( iProc ==  1 ) Procedure = "PV2" ;
    if ( iProc ==  2 ) Procedure = "PV2" ;
    
    if ( iProc ==  3 ) Procedure = "PV2" ;
    if ( iProc ==  4 ) Procedure = "PV2" ;
    if ( iProc ==  5 ) Procedure = "PV2" ;
    if ( iProc ==  6 ) Procedure = "PV2" ;
    if ( iProc ==  7 ) Procedure = "PV2" ;
    if ( iProc ==  8 ) Procedure = "PV2" ;
    if ( iProc ==  9 ) Procedure = "PV2" ;
    if ( iProc == 10 ) Procedure = "PV2" ;
    
    if ( iProc ==  3 ) v_CTD.push_back( 260. ) ;
    if ( iProc ==  4 ) v_CTD.push_back( 270. ) ;
    if ( iProc ==  5 ) v_CTD.push_back( 280. ) ;
    if ( iProc ==  6 ) v_CTD.push_back( 290. ) ;
    if ( iProc ==  7 ) v_CTD.push_back( 300. ) ;
    if ( iProc ==  8 ) v_CTD.push_back( 310. ) ;
    if ( iProc ==  9 ) v_CTD.push_back( 320. ) ;
    if ( iProc == 10 ) v_CTD.push_back( 330. ) ;
        
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      TMultiGraph* pTMultiGraph = new TMultiGraph();      

      TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
      pTLegend->SetBorderSize(1);
      pTLegend->SetMargin(0.1) ;
  
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;

      if ( iProc ==  1 ) aostringstream_pTMultiGraph << "Standard "   ;
      if ( iProc ==  2 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=310 #mu m/#sqrt{cm}"   ;

      if ( iProc ==  3 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=260 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  4 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=270 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  5 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=280 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  6 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=290 #mu m/#sqrt{cm}"   ;   
      if ( iProc ==  7 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=300 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  8 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=310 #mu m/#sqrt{cm}"   ;
      if ( iProc ==  9 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=320 #mu m/#sqrt{cm}"   ;   
      if ( iProc == 10 ) aostringstream_pTMultiGraph << "Fit peak + C_{TD}=330 #mu m/#sqrt{cm}"   ;

      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      pTMultiGraph->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
        std::string SampleSet_Specific = "D21_ZSCAN_200" ;
        
        Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

        std::string AnalyseName = "BIDON" ;
        
        std::ostringstream aostringstream_AnalyseName ;
        aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
        aostringstream_AnalyseName << Procedure  ;
        aostringstream_AnalyseName << "_"  ;
        
        if ( iProc ==  1 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200" ;
        if ( iProc ==  2 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_310_00" ;
        
        if ( iProc ==  3 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_260_00" ;
        if ( iProc ==  4 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_270_00" ;
        if ( iProc ==  5 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_280_00" ;
        if ( iProc ==  6 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_290_00" ;
        if ( iProc ==  7 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_300_00" ;
        if ( iProc ==  8 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_310_00" ;
        if ( iProc ==  9 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_320_00" ;
        if ( iProc == 10 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_330_00" ;
        
        aostringstream_AnalyseName << "_Module_Nber_"   ;
        aostringstream_AnalyseName << ModuleNber   ;

        AnalyseName = aostringstream_AnalyseName.str() ;  ;


        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        pTGraphErrors->SetMarkerSize(1.8) ;

        pTGraphErrors->SetMarkerStyle(24) ;

        pTGraphErrors->SetMarkerColor( 2 ) ; 

        pTGraphErrors->SetLineColor( 2 ) ; 

        if (iProc ==  3 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetLineColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetLineColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetLineColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetLineColor( 3 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerStyle( 21 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerStyle( 24 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerStyle( 20 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerStyle( 21 ) ;

        if (iProc ==  3 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  4 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  5 ) pTGraphErrors->SetMarkerColor( 2 ) ;
        if (iProc ==  6 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  7 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  8 ) pTGraphErrors->SetMarkerColor( 4 ) ;
        if (iProc ==  9 ) pTGraphErrors->SetMarkerColor( 3 ) ;
        if (iProc == 10 ) pTGraphErrors->SetMarkerColor( 3 ) ;

        double RC_min   = 0 ;
        double RC_max   = 0 ;
        int    RC_first = 1 ;

        pTMultiGraph->Add( pTGraphErrors , "pl" );
        if ( iProc >=3 )  
        pTMultiGraph_All->Add( pTGraphErrors , "pl" );
        
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double ZScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
    
          TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
    
          double  TheValue =  pTF1_OK->GetParameter(0) ;
          double eTheValue =  pTF1_OK->GetParError (0) ;

          int IptCur_Mean  = pTGraphErrors ->GetN() ;
          pTGraphErrors ->SetPoint     ( IptCur_Mean , ZScan , TheValue ) ;
          pTGraphErrors ->SetPointError( IptCur_Mean , 0.    ,eTheValue ) ;
          
          if (RC_first==1){
            RC_first = 0 ;
            RC_min = TheValue ;
            RC_max = TheValue ;
          }
          if (RC_min>TheValue) RC_min = TheValue ;
          if (RC_max<TheValue) RC_max = TheValue ;
          
        }
        
        double AVG =  (RC_max+RC_min)/2. ;
        double DIFF = 100.*(RC_max-RC_min)/AVG ;
        DIFF = DIFF/2. ;
        
        if ( iProc >=  3 ){
          if (iMod==0) { v_AVG_Mod0.push_back(AVG ) ; v_DIFF_Mod0.push_back(DIFF ) ; }
          if (DIFF_Min_First==1){
            DIFF_Min_First = 0    ;
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          if (DIFF_Min>DIFF){
            DIFF_Min       = DIFF ;
            RC_Min         = AVG  ;
          }
          
        }

        std::ostringstream aostringstream_pTLegend ;
        aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

        if ( iProc ==  1 ) aostringstream_pTLegend << "Standard "   ;
        if ( iProc ==  2 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;

        if ( iProc ==  3 ) aostringstream_pTLegend << "C_{TD}=260 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  4 ) aostringstream_pTLegend << "C_{TD}=270 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  5 ) aostringstream_pTLegend << "C_{TD}=280 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  6 ) aostringstream_pTLegend << "C_{TD}=290 #mu m/#sqrt{cm}"   ;   
        if ( iProc ==  7 ) aostringstream_pTLegend << "C_{TD}=300 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  8 ) aostringstream_pTLegend << "C_{TD}=310 #mu m/#sqrt{cm}"   ;
        if ( iProc ==  9 ) aostringstream_pTLegend << "C_{TD}=320 #mu m/#sqrt{cm}"   ;   
        if ( iProc == 10 ) aostringstream_pTLegend << "C_{TD}=330 #mu m/#sqrt{cm}"   ;

        aostringstream_pTLegend << ", RC = "   ; 
        aostringstream_pTLegend << std::setprecision(0) << AVG   ;
        aostringstream_pTLegend << " ns mm^{-2} "  ;
        aostringstream_pTLegend << " +/- "   ;
        aostringstream_pTLegend << std::setprecision(0) << DIFF    ;
        aostringstream_pTLegend << " %"   ;

        pTLegend    ->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl " );
        if ( iProc >=3 )  
        pTLegend_All->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str(), "pl" );

//--------------------RC_VS_Z-------------------------//
      
//--------------------RC_VS_Z-------------------------// 

      pTMultiGraph ->Draw("A");
      
      pTMultiGraph->SetMinimum(  0. );
      pTMultiGraph->SetMaximum(150. );
      pTMultiGraph->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update()    ;

      double Vmin = pTCanvas->GetUymin() ;
      double Vmax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
      pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

      pTCanvas->Update()    ;

      pTLegend ->Draw();
      
      pTCanvas->Update();

//--------------------RC_VS_Z-------------------------//
    }
    
    pTCanvas->Print(Str_Out_PDF.c_str());

  }

  
//--------------------RC_VS_Z-------------------------// 

  std::cout << " DIFF_Min " << DIFF_Min << std::endl ;
  std::cout << " RC_Min   " << RC_Min   << std::endl ;
  pTMultiGraph_All ->Draw("A");
  
  pTMultiGraph_All->SetMinimum(-30. );
  pTMultiGraph_All->SetMaximum(120. );
  pTMultiGraph_All->GetXaxis()->SetLimits(-200.,900.);
  
  pTMultiGraph_All->GetXaxis()->SetTitle("Z (mm)");
  pTMultiGraph_All->GetYaxis()->SetTitle("RC (ns mm^{-2})");
  pTMultiGraph_All->GetYaxis()->SetTitleOffset(1.5);
      
  pTCanvas->Update()    ;

  double Vmin = pTCanvas->GetUymin() ;
  double Vmax = pTCanvas->GetUymax() ;
  TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
  pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  

  pTCanvas->Update()    ;

  double Hmin = pTCanvas->GetUxmin() ;
  double Hmax = pTCanvas->GetUxmax() ;
  TLine* pTLineH = new TLine( Hmin  , RC_Min , Hmax , RC_Min ) ;
  pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
  pTCanvas->Update()    ;

  pTLegend_All ->Draw();
  
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------RC_VS_Z-------------------------//

//
  int iTem_Max = v_CTD.size()  ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::cout 
      << " v_CTD " << v_CTD[iTem]
      << " v_AVG_Mod0 " << std::setw(4) << std::setprecision(0) << v_AVG_Mod0[iTem] << " v_DIFF_Mod0 " << std::setw(4) << std::setprecision(0) << v_DIFF_Mod0[iTem]
      << std::endl ;
  }
  
//
   pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
   pTCanvas->Divide(2,1) ;
   
   int iMod_Min = 0 ;
   int iMod_Max = 1 ;
   for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      TMultiGraph* pTMultiGraph_AVG  = new TMultiGraph();      
      TMultiGraph* pTMultiGraph_DIFF = new TMultiGraph();      

      std::ostringstream aostringstream_pTMultiGraph_AVG ;
      aostringstream_pTMultiGraph_AVG << " Module "   ;
      aostringstream_pTMultiGraph_AVG << iMod   ;
      pTMultiGraph_AVG->SetTitle((aostringstream_pTMultiGraph_AVG.str()).c_str());

      std::ostringstream aostringstream_pTMultiGraph_DIFF ;
      aostringstream_pTMultiGraph_DIFF << " Module "   ;
      aostringstream_pTMultiGraph_DIFF << iMod   ;
      pTMultiGraph_DIFF->SetTitle((aostringstream_pTMultiGraph_DIFF.str()).c_str());

//
      TGraph* pTGraph_AVG  = new TGraph ;
      TGraph* pTGraph_DIFF = new TGraph ;

      pTGraph_AVG ->SetMarkerSize(1.8) ;
      pTGraph_DIFF->SetMarkerSize(1.8) ;
      
      pTGraph_AVG ->SetMarkerColor( 2 ) ; 
      pTGraph_DIFF->SetMarkerColor( 2 ) ; 

      pTGraph_AVG ->SetMarkerStyle(24) ;
      pTGraph_DIFF->SetMarkerStyle(24) ;
      
      pTGraph_AVG ->SetLineColor( 2 ) ; 
      pTGraph_DIFF->SetLineColor( 2 ) ; 

      pTMultiGraph_AVG ->Add( pTGraph_AVG  , "pl" );
      pTMultiGraph_DIFF->Add( pTGraph_DIFF , "pl" );
      
      int iTem_Max = v_CTD.size()  ;
      for (int iTem = 0 ; iTem< iTem_Max; iTem++){
        double AVG = 0 ; 
        double DIFF = 0 ; 
        if (iMod==0) { AVG = v_AVG_Mod0[iTem] ; DIFF = v_DIFF_Mod0[iTem] ; }

        int IptCur_AVG  = pTGraph_AVG ->GetN() ;
        pTGraph_AVG ->SetPoint ( IptCur_AVG , v_CTD[iTem] , AVG     ) ;
   
        int IptCur_DIFF  = pTGraph_DIFF ->GetN() ;
        pTGraph_DIFF ->SetPoint ( IptCur_DIFF , v_CTD[iTem] , DIFF     ) ;
      }
      
      pTCanvas->cd(iMod+1) ;
 
      pTMultiGraph_AVG ->Draw("A");
      
      pTMultiGraph_AVG->SetMinimum(  0. );
      pTMultiGraph_AVG->SetMaximum(150. );
      pTMultiGraph_AVG->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_AVG->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_AVG->GetYaxis()->SetTitle("(Max+Min)/2 (ns mm^{-2})");
      pTMultiGraph_AVG->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

//
      pTCanvas->cd(iMod+2) ;
 
      pTMultiGraph_DIFF ->Draw("A");
      
      pTMultiGraph_DIFF->SetMinimum(  0. );
      pTMultiGraph_DIFF->SetMaximum( 10. );
      pTMultiGraph_DIFF->GetXaxis()->SetLimits(250.,340.);
      
      pTMultiGraph_DIFF->GetXaxis()->SetTitle("C_{TD} (#mu m/#sqrt{cm})");
      pTMultiGraph_DIFF->GetYaxis()->SetTitle("(Max-Min)/2 (%)");
      pTMultiGraph_DIFF->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();

   }
   pTCanvas->Print(Str_Out_PDF.c_str());
   
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void DESY21_01_RESO_ZSCAN_200_01()
{
  std::string SampleSet = "D21_ZSCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "DESY21_01_RESO_ZSCAN_200_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_VS_Z ;
  std::vector < TLegend* >     V_pTLegend_Sigma_VS_Z     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Sigma_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Mean_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Mean_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Bias_VS_Z ;
  
  int iAna_Min = 0 ;
  int iAna_Max = 3 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    std::string Procedure = "BIDON" ;
    if (iAna==0) Procedure = "PV0" ;
    if (iAna==1) Procedure = "PV2" ;
    if (iAna==2) Procedure = "PV2" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      if ( iAna == 1) aostringstream_pTMultiGraph << " Standard "   ;
      if ( iAna == 2) aostringstream_pTMultiGraph << " Improved "   ;
      aostringstream_pTMultiGraph << " "   ;
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "D21_ZSCAN_200" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
        
      if ( iAna == 0 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200" ;
      if ( iAna == 1 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200" ;
      if ( iAna == 2 ) aostringstream_AnalyseName <<  "D21_ZSCAN_200_TD_310_00" ;
        
      aostringstream_AnalyseName << "_Module_Nber_"   ;
      aostringstream_AnalyseName << ModuleNber   ;

      AnalyseName = aostringstream_AnalyseName.str() ;  ;

      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------Sigma_VS_Z-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_VS_Z = new TMultiGraph();      
      pTMultiGraph_Sigma_VS_Z->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Sigma_VS_Z = new TLegend(0.16,0.15 ,0.88,0.20); 
      pTLegend_Sigma_VS_Z->SetBorderSize(0);
      pTLegend_Sigma_VS_Z->SetMargin(0.1) ;

      V_pTMultiGraph_Sigma_VS_Z.push_back(pTMultiGraph_Sigma_VS_Z)  ; 
      V_pTLegend_Sigma_VS_Z    .push_back(pTLegend_Sigma_VS_Z    )  ; 
 
      TGraphErrors* pTGraphErrors_Sigma_VS_Z = new TGraphErrors ;
      pTGraphErrors_Sigma_VS_Z->SetMarkerSize(1.8) ;

      pTGraphErrors_Sigma_VS_Z->SetMarkerStyle(24) ;

      pTGraphErrors_Sigma_VS_Z->SetMarkerColor( 2 ) ; 

      pTGraphErrors_Sigma_VS_Z->SetLineColor( 2 ) ; 

      pTMultiGraph_Sigma_VS_Z->Add( pTGraphErrors_Sigma_VS_Z , "p" );
      
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
          double Mean = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetMean(2) ;
          double RMS  = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetRMS(2)  ;
          double TheN = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()  ;
                                                    
          double RMSLocal = RMS ;
          RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
          
          std::cout 
            << " ZScan    " << ZScan
            << " Mean     " << Mean 
            << " RMS      " << RMS  
            << " TheN     " << TheN 
            << " RMSLocal " << RMSLocal
            << " " << std::setw(40) << base_EventFile
            << std::endl ;
            
          int IptCur_Mean  = pTGraphErrors_Sigma_VS_Z ->GetN() ;
          pTGraphErrors_Sigma_VS_Z ->SetPoint     ( IptCur_Mean , ZScan , Mean     ) ;
          pTGraphErrors_Sigma_VS_Z ->SetPointError( IptCur_Mean , 0.    , RMSLocal ) ;
        }
      }
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*((x+40)/10.) /[1])",-137.46, 1000.);
      pTF1->SetParName(0,"sigma0");
      pTF1->SetParName(1,"Neff");    
      pTF1->SetParameter(0,100.);
      pTF1->SetParameter(1,100.);    
      pTF1->SetLineColor(2) ;
      pTGraphErrors_Sigma_VS_Z->Fit(pTF1, "R");

      std::ostringstream aostringstream_pTLegend_Sigma_VS_Z ;
      aostringstream_pTLegend_Sigma_VS_Z << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Sigma_VS_Z 
                              << "#sigma_{0} = " << std::setprecision(0) << pTF1->GetParameter(0)
                              << " +/- "        << std::setprecision(0) << pTF1->GetParError(0)
                              << "  N_{eff} = " << std::setprecision(0) << pTF1->GetParameter(1)
                              << " +/- "        << std::setprecision(0) << pTF1->GetParError(1)
                              << "  #chi^{2}_{min}/dof = " 
                                                << std::setprecision(0) << pTF1->GetChisquare()
                              << "/"            << std::setprecision(0) << pTF1->GetNDF()
                              ;
                                             
      pTLegend_Sigma_VS_Z->AddEntry( pTGraphErrors_Sigma_VS_Z , (aostringstream_pTLegend_Sigma_VS_Z.str()).c_str() , "pl");
      
//--------------------Sigma_VS_Z-------------------------//
      
//--------------------Sigma_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = new TMultiGraph();      
      pTMultiGraph_Sigma_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.16,0.15 ,0.88,0.30); 
      pTLegend_Sigma_Residual_VS_Col->SetBorderSize(0);
      pTLegend_Sigma_Residual_VS_Col->SetMargin(0.1) ;

      V_pTMultiGraph_Sigma_Residual_VS_Col.push_back(pTMultiGraph_Sigma_Residual_VS_Col)  ; 
      V_pTLegend_Sigma_Residual_VS_Col    .push_back(pTLegend_Sigma_Residual_VS_Col    )  ; 
 
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineStyle( 1 ) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerSize(1.4) ;
        
          pTMultiGraph_Sigma_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col, "pl") ;
         
          std::ostringstream aostringstream_pTLegend_Sigma_Residual_VS_Col ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << "Z ="  ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << std::setprecision(2) << ZScan  ;
          aostringstream_pTLegend_Sigma_Residual_VS_Col << " (mm)"   ;

          std::string pTLegend_Sigma_Residual_VS_Col_String = aostringstream_pTLegend_Sigma_Residual_VS_Col.str() ;  ;

          pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , pTLegend_Sigma_Residual_VS_Col_String.c_str()  , "pl");
        } 
                  
      }
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Mean_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = new TMultiGraph();      
      pTMultiGraph_Mean_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend_Mean_Residual_VS_Col = new TLegend(0.16,0.15 ,0.88,0.30); 
      pTLegend_Mean_Residual_VS_Col->SetBorderSize(0);
      pTLegend_Mean_Residual_VS_Col->SetMargin(0.1) ;

      V_pTMultiGraph_Mean_Residual_VS_Col.push_back(pTMultiGraph_Mean_Residual_VS_Col)  ; 
      V_pTLegend_Mean_Residual_VS_Col    .push_back(pTLegend_Mean_Residual_VS_Col    )  ; 
 
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

        if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineStyle( 1 ) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;

          if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
          if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
          if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;
          if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;

          aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerSize(1.1) ;
        
          pTMultiGraph_Mean_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col, "pl") ;
         
          std::ostringstream aostringstream_pTLegend_Mean_Residual_VS_Col ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << "Z ="  ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << std::setprecision(2) << ZScan  ;
          aostringstream_pTLegend_Mean_Residual_VS_Col << " (mm)"   ;

          std::string pTLegend_Mean_Residual_VS_Col_String = aostringstream_pTLegend_Mean_Residual_VS_Col.str() ;  ;

          pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , pTLegend_Mean_Residual_VS_Col_String.c_str()  , "pl");
        } 
                  
      }
//--------------------Mean_Residual_VS_Col-------------------------//

//--------------------Bias_VS_Z-------------------------//
    TMultiGraph* pTMultiGraph_Bias_VS_Z = new TMultiGraph();      
    pTMultiGraph_Bias_VS_Z->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

    V_pTMultiGraph_Bias_VS_Z.push_back(pTMultiGraph_Bias_VS_Z)  ; 

    TGraphErrors* pTGraphErrors_Bias_VS_Z = new TGraphErrors ;
    pTGraphErrors_Bias_VS_Z->SetMarkerStyle(20) ;
    pTGraphErrors_Bias_VS_Z->SetMarkerColor( 2 ) ; 
    pTGraphErrors_Bias_VS_Z->SetMarkerSize(1.8) ;
    pTMultiGraph_Bias_VS_Z->Add( pTGraphErrors_Bias_VS_Z , "p" );

      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        double ZScan = aAnalysis_Files.m_VariableValue[iFile];

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);
        
        if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
          double Mean = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetMean(2) ;
          double RMS  = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetRMS(2)  ;
          double TheN = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN()     ;

//        Work on Absolute value of the means!!!                                                
          int     Npoints_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN() ;
          double*       X_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetX() ;
          double*       Y_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetY() ;
          double*      EY_in_Original = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetEY() ;
          TGraphErrors* pTGraphErrors_Copy = new TGraphErrors ;
          for (int ipt = 0; ipt< Npoints_in_Original; ipt++){
            int IptCur_Mean  = pTGraphErrors_Copy ->GetN() ;
            pTGraphErrors_Copy ->SetPoint     ( IptCur_Mean , X_in_Original[ipt] , std::fabs(Y_in_Original[ipt] )    ) ;
            pTGraphErrors_Copy ->SetPointError( IptCur_Mean , 0.                 ,           EY_in_Original[ipt]     ) ;
          }
          Mean = pTGraphErrors_Copy->GetMean(2) ;
          RMS  = pTGraphErrors_Copy->GetRMS(2)  ;
          TheN = pTGraphErrors_Copy->GetN()     ;
          
          double RMSLocal = RMS ;
          RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
          
          int IptCur_Mean  = pTGraphErrors_Bias_VS_Z ->GetN() ;
          pTGraphErrors_Bias_VS_Z ->SetPoint     ( IptCur_Mean , ZScan , Mean      ) ;
          pTGraphErrors_Bias_VS_Z ->SetPointError( IptCur_Mean , 0.    , RMSLocal  ) ;
                     
          pTMultiGraph_Bias_VS_Z->Add( pTGraphErrors_Bias_VS_Z , "p" );
          
        } 
        
      }
//--------------------Bias_VS_Z-------------------------//
    }
    
  }
 
  int Kounter = 0;

//--------------------Mean_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-300. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 300. );
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
      
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitle("Mean of Residuals (#mu m)");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      
      pTLegend_Mean_Residual_VS_Col->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Mean_Residual_VS_Col-------------------------//

//--------------------Bias_VS_Z-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum( 100. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Bias_VS_Z->GetYaxis()->SetTitle("Mean of Absolute Values of Mean of Residuals (#mu m)");
      pTMultiGraph_Bias_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Bias_VS_Z-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col  = V_pTMultiGraph_Sigma_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Sigma_Residual_VS_Col      = V_pTLegend_Sigma_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Sigma_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Sigma_Residual_VS_Col->SetMinimum(   0. );
      pTMultiGraph_Sigma_Residual_VS_Col->SetMaximum( 500. );
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
      
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitle("Sigma of Residuals (#mu m)");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      
      pTLegend_Sigma_Residual_VS_Col->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Sigma_Residual_VS_Col-------------------------//


//--------------------Sigma_VS_Z-------------------------//
  Kounter = -1 ;
  for (int iAna = iAna_Min ; iAna< iAna_Max; iAna++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-200.,900.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->GetUymin() ;
      double Vmax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
      pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
      pTCanvas->Update()    ;

      pTLegend_Sigma_VS_Z->Draw();

      pTCanvas->Update();
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------Sigma_VS_Z-------------------------//
  
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}
