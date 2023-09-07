#include "Analysis_JF/CERN22_01_ESCAN.h"
#include "Misc/Util.h"
#include "Misc/GaussFunction.h"

#include "Analysis_JF/ReadRootFile.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "Analysis_JF/Analysis_Files.h"

#include "Analysis_JF/PerSample.h"

#include "Analysis_JF/CERN22_01_Mod2Erams.h"

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

void CERN22_01_RC_ESCAN_01()
{
  std::string SampleSet = "C22_ESCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MyMakeDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_RC_ESCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  int iProc_Min = 1 ;
  int iProc_Max = 3 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
    pTCanvas->Divide(4,1) ;

    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    if (iProc==1) Procedure = "PV2" ;
    if (iProc==2) Procedure = "PV2" ;
    
    int iMod_Max = 4 ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      TMultiGraph* pTMultiGraph = new TMultiGraph();      

      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      if (iProc==1) aostringstream_pTMultiGraph << " Standard "   ;
      if (iProc==2) aostringstream_pTMultiGraph << " Improved "   ;
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      aostringstream_pTMultiGraph << " " << CERN22_01_Mod2Erams( ModuleNber ) ;
      pTMultiGraph->SetTitle((aostringstream_pTMultiGraph.str()).c_str());
      
      TLegend* pTLegend = new TLegend(0.56,0.20 ,0.75,0.40); 
      pTLegend->SetFillStyle(1001);
      pTLegend->SetBorderSize(1);

      int iScan_Max = 4 ;
      for (int iScan = 0 ; iScan< iScan_Max; iScan++){
        std::string SampleSet_Specific = "BIDON" ;
        if ( iScan == 0 ) SampleSet_Specific = "C22_PIP_ESCAN" ;
        if ( iScan == 1 ) SampleSet_Specific = "C22_EP_ESCAN"  ;
        if ( iScan == 2 ) SampleSet_Specific = "C22_P_ESCAN"   ;
        if ( iScan == 3 ) SampleSet_Specific = "C22_MUP_ESCAN" ;
        
        Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

        std::string AnalyseName = "BIDON" ;
        
        std::ostringstream aostringstream_AnalyseName ;
        aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
        aostringstream_AnalyseName << Procedure  ;
        aostringstream_AnalyseName << "_"  ;

        if (iScan==0 && iProc!=2) aostringstream_AnalyseName << "C22_PIP_ESCAN" ;
        if (iScan==1 && iProc!=2) aostringstream_AnalyseName << "C22_EP_ESCAN"  ;
        if (iScan==2 && iProc!=2) aostringstream_AnalyseName << "C22_P_ESCAN"   ;
        if (iScan==3 && iProc!=2) aostringstream_AnalyseName << "C22_MUP_ESCAN" ;
        
        if (iScan==0 && iProc==2) aostringstream_AnalyseName << "C22_PIP_ESCAN_Improved" ;
        if (iScan==1 && iProc==2) aostringstream_AnalyseName << "C22_EP_ESCAN_Improved"  ;
        if (iScan==2 && iProc==2) aostringstream_AnalyseName << "C22_P_ESCAN_Improved"   ;
        if (iScan==3 && iProc==2) aostringstream_AnalyseName << "C22_MUP_ESCAN_Improved" ;

        aostringstream_AnalyseName << "_Module_Nber_"   ;
        aostringstream_AnalyseName << ModuleNber   ;

        AnalyseName = aostringstream_AnalyseName.str() ;  ;


        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_E-------------------------//
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        pTGraphErrors->SetMarkerSize(1.8) ;

        if (iScan==0) pTLegend->AddEntry( pTGraphErrors , "#pi^{+}"  , "pl");
        if (iScan==1) pTLegend->AddEntry( pTGraphErrors , "e^{+}  "  , "pl");
        if (iScan==2) pTLegend->AddEntry( pTGraphErrors , "p  "      , "pl");
        if (iScan==3) pTLegend->AddEntry( pTGraphErrors , "#mu^{+}"  , "pl");
        
        if (iScan==0) pTGraphErrors->SetMarkerStyle(20) ;
        if (iScan==1) pTGraphErrors->SetMarkerStyle(24) ;
        if (iScan==2) pTGraphErrors->SetMarkerStyle(21) ;
        if (iScan==3) pTGraphErrors->SetMarkerStyle(25) ;

        if (iScan==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (iScan==1) pTGraphErrors->SetMarkerColor( 4 ) ; 
        if (iScan==2) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (iScan==3) pTGraphErrors->SetMarkerColor( 4 ) ; 

        if (iScan==0) pTGraphErrors->SetLineColor( 2 ) ; 
        if (iScan==1) pTGraphErrors->SetLineColor( 4 ) ; 
        if (iScan==2) pTGraphErrors->SetLineColor( 2 ) ; 
        if (iScan==3) pTGraphErrors->SetLineColor( 4 ) ; 

        pTMultiGraph->Add( pTGraphErrors , "pl" );
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double EScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
    
          TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
    
          double  TheValue =  pTF1_OK->GetParameter(0) ;
          double eTheValue =  pTF1_OK->GetParError (0) ;

          int IptCur_Mean  = pTGraphErrors ->GetN() ;
          pTGraphErrors ->SetPoint     ( IptCur_Mean , EScan , TheValue ) ;
          pTGraphErrors ->SetPointError( IptCur_Mean , 0.    ,eTheValue ) ;
          
        }
//--------------------RC_VS_E-------------------------//
      }
      
//--------------------RC_VS_E-------------------------//
      pTCanvas->cd(iMod+1) ;
 

      pTMultiGraph ->Draw("A");
      
      pTMultiGraph->SetMinimum(  0. );
      pTMultiGraph->SetMaximum(120. );
      pTMultiGraph->GetXaxis()->SetLimits(0.,2.0);
      pTMultiGraph->GetXaxis()->SetTitle("E (GeV)");
      pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
      pTMultiGraph->GetXaxis()->SetLimits(0.,2.0);

      pTCanvas->Update();

      pTLegend->Draw();
      pTCanvas->Update();
//--------------------RC_VS_E-------------------------//
    }
    
    pTCanvas->Print(Str_Out_PDF.c_str());

  }
  
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_RESO_ESCAN_01()
{
  std::string SampleSet = "C22_ESCAN" ;
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MyMakeDir(OUTDirName) ;
//
  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_RESO_ESCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Sigma_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Mean_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Mean_Residual_VS_Col     ;

  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_VS_E ;
  std::vector < TLegend* >     V_pTLegend_Sigma_VS_E     ;

//
  int iProc_Min = 0 ;
  int iProc_Max = 3 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::cout << std::endl ;
    
    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    if (iProc==1) Procedure = "PV2" ;
    if (iProc==2) Procedure = "PV2" ;
    
    int iMod_Max = 4 ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){

      std::cout << std::endl ;

      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      
      if ( iProc == 1 ) aostringstream_pTMultiGraph << " Standard "   ;
      if ( iProc == 2 ) aostringstream_pTMultiGraph << " Improved "   ;
      
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      aostringstream_pTMultiGraph << " " << CERN22_01_Mod2Erams( ModuleNber ) ;

//--------------------Sigma_VS_E-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_VS_E = new TMultiGraph(); 
      pTMultiGraph_Sigma_VS_E->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

      TLegend* pTLegend_Sigma_VS_E = new TLegend(0.56,0.20 ,0.75,0.40); 
      pTLegend_Sigma_VS_E->SetFillStyle(1001);
      pTLegend_Sigma_VS_E->SetBorderSize(1);

      V_pTMultiGraph_Sigma_VS_E.push_back(pTMultiGraph_Sigma_VS_E) ;    
      V_pTLegend_Sigma_VS_E    .push_back(pTLegend_Sigma_VS_E    ) ;    
//--------------------Sigma_VS_E-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = new TMultiGraph(); 
      pTMultiGraph_Sigma_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

      TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.15,0.12 ,0.85,0.30); 
      pTLegend_Sigma_Residual_VS_Col->SetFillStyle(1001);
      pTLegend_Sigma_Residual_VS_Col->SetBorderSize(1);
      pTLegend_Sigma_Residual_VS_Col->SetNColumns(2);

      V_pTMultiGraph_Sigma_Residual_VS_Col.push_back(pTMultiGraph_Sigma_Residual_VS_Col) ;    
      V_pTLegend_Sigma_Residual_VS_Col    .push_back(pTLegend_Sigma_Residual_VS_Col    ) ;    
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Mean_Residual_VS_Col-------------------------//
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = new TMultiGraph(); 
      pTMultiGraph_Mean_Residual_VS_Col->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

      TLegend* pTLegend_Mean_Residual_VS_Col = new TLegend(0.15,0.12 ,0.85,0.30); 
      pTLegend_Mean_Residual_VS_Col->SetFillStyle(1001);
      pTLegend_Mean_Residual_VS_Col->SetBorderSize(1);
      pTLegend_Mean_Residual_VS_Col->SetNColumns(2);

      V_pTMultiGraph_Mean_Residual_VS_Col.push_back(pTMultiGraph_Mean_Residual_VS_Col) ;    
      V_pTLegend_Mean_Residual_VS_Col    .push_back(pTLegend_Mean_Residual_VS_Col    ) ;    
//--------------------Mean_Residual_VS_Col-------------------------//

      int iScan_Max = 4 ;
      for (int iScan = 0 ; iScan< iScan_Max; iScan++){
    
        std::cout << std::endl ;
    
        std::string SampleSet_Specific = "BIDON" ;
        
        if (iScan==0) SampleSet_Specific = "C22_PIP_ESCAN" ;
        if (iScan==1) SampleSet_Specific = "C22_EP_ESCAN"  ;
        if (iScan==2) SampleSet_Specific = "C22_P_ESCAN"   ;
        if (iScan==3) SampleSet_Specific = "C22_MUP_ESCAN" ;

        Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

        std::string AnalyseName = "BIDON" ;
        
        std::ostringstream aostringstream_AnalyseName ;
        aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
        aostringstream_AnalyseName << Procedure  ;
        aostringstream_AnalyseName << "_"  ;

        if (iScan==0 && iProc!=2) aostringstream_AnalyseName << "C22_PIP_ESCAN" ;
        if (iScan==1 && iProc!=2) aostringstream_AnalyseName << "C22_EP_ESCAN"  ;
        if (iScan==2 && iProc!=2) aostringstream_AnalyseName << "C22_P_ESCAN"   ;
        if (iScan==3 && iProc!=2) aostringstream_AnalyseName << "C22_MUP_ESCAN" ;
        
        if (iScan==0 && iProc==2) aostringstream_AnalyseName << "C22_PIP_ESCAN_Improved" ;
        if (iScan==1 && iProc==2) aostringstream_AnalyseName << "C22_EP_ESCAN_Improved"  ;
        if (iScan==2 && iProc==2) aostringstream_AnalyseName << "C22_P_ESCAN_Improved"   ;
        if (iScan==3 && iProc==2) aostringstream_AnalyseName << "C22_MUP_ESCAN_Improved" ;

        aostringstream_AnalyseName << "_Module_Nber_"   ;
        aostringstream_AnalyseName << ModuleNber   ;

        AnalyseName = aostringstream_AnalyseName.str() ;  ;

        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------Sigma_VS_E-------------------------//
        TGraphErrors* pTGraphErrors_Sigma_VS_E = new TGraphErrors ;
        pTGraphErrors_Sigma_VS_E->SetMarkerSize(1.8) ;

        if (iScan==0) pTLegend_Sigma_VS_E->AddEntry( pTGraphErrors_Sigma_VS_E , "#pi^{+}"  , "pl");
        if (iScan==1) pTLegend_Sigma_VS_E->AddEntry( pTGraphErrors_Sigma_VS_E , "e^{+}  "  , "pl");
        if (iScan==2) pTLegend_Sigma_VS_E->AddEntry( pTGraphErrors_Sigma_VS_E , "p  "      , "pl");
        if (iScan==3) pTLegend_Sigma_VS_E->AddEntry( pTGraphErrors_Sigma_VS_E , "#mu^{+}"  , "pl");
        
        if (iScan==0) pTGraphErrors_Sigma_VS_E->SetMarkerStyle(20) ;
        if (iScan==1) pTGraphErrors_Sigma_VS_E->SetMarkerStyle(24) ;
        if (iScan==2) pTGraphErrors_Sigma_VS_E->SetMarkerStyle(21) ;
        if (iScan==3) pTGraphErrors_Sigma_VS_E->SetMarkerStyle(25) ;

        if (iScan==0) pTGraphErrors_Sigma_VS_E->SetMarkerColor( 2 ) ; 
        if (iScan==1) pTGraphErrors_Sigma_VS_E->SetMarkerColor( 4 ) ; 
        if (iScan==2) pTGraphErrors_Sigma_VS_E->SetMarkerColor( 2 ) ; 
        if (iScan==3) pTGraphErrors_Sigma_VS_E->SetMarkerColor( 4 ) ; 

        if (iScan==0) pTGraphErrors_Sigma_VS_E->SetLineColor( 2 ) ; 
        if (iScan==1) pTGraphErrors_Sigma_VS_E->SetLineColor( 4 ) ; 
        if (iScan==2) pTGraphErrors_Sigma_VS_E->SetLineColor( 2 ) ; 
        if (iScan==3) pTGraphErrors_Sigma_VS_E->SetLineColor( 4 ) ; 

        pTMultiGraph_Sigma_VS_E->Add( pTGraphErrors_Sigma_VS_E , "pl" );
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double EScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

          if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
            double Mean = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetMean(2) ;
            double RMS  = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetRMS(2)  ;
            double TheN = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()  ;
                                                      
            double RMSLocal = RMS ;
            RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
            
            int IptCur_Mean  = pTGraphErrors_Sigma_VS_E ->GetN() ;
            pTGraphErrors_Sigma_VS_E ->SetPoint     ( IptCur_Mean , EScan , Mean     ) ;
            pTGraphErrors_Sigma_VS_E ->SetPointError( IptCur_Mean , 0.    , RMSLocal ) ;

            if ( iScan==1 || iScan==3 ){
              std::cout 
                << " iMod    " << iMod ;
               std::cout 
                << " iProc    " << iProc ;
               std::cout 
                << " iScan    " << iScan ;
              if (iScan==0) std::cout << std::setw(5) << " pi " ;
              if (iScan==1) std::cout << std::setw(5) << " e+ " ;
              if (iScan==2) std::cout << std::setw(5) << " p  " ;
              if (iScan==3) std::cout << std::setw(5) << " mu+" ;
              std::cout
                << " EScan    " << std::setw(5) << std::setprecision(2) << EScan
              ;
              std::cout
                << " Reso " << std::setw(3) << std::setprecision(0) << Mean     
                << " +/- "  << std::setw(3) << std::setprecision(0) << RMSLocal 
                ;
              std::cout
                << "->Mean " << std::setw(3) << std::setprecision(0) << pTGraphErrors_Sigma_VS_E->GetMean(2) 
                << " +/- "   << std::setw(3) << std::setprecision(0) <<
                ( TMath::MaxElement(pTGraphErrors_Sigma_VS_E ->GetN(),pTGraphErrors_Sigma_VS_E->GetY())
                 - 
                  TMath::MinElement(pTGraphErrors_Sigma_VS_E ->GetN(),pTGraphErrors_Sigma_VS_E->GetY()))/2.
                << std::endl ;     
            }


          }
        }
//--------------------Sigma_VS_E-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double EScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);


          if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
            
            if (iScan==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
            if (iScan==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;
            if (iScan==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
            if (iScan==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 6 ) ;
            
            if (iScan==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
            if (iScan==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;
            if (iScan==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
            if (iScan==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 6 ) ;
            
            if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ; 
            if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ; 
            if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ; 
            if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(25) ; 
            if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(22) ; 
            if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(26) ; 
            if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(23) ; 

            pTMultiGraph_Sigma_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col, "pl") ;

            std::ostringstream aostringstream_pTLegend_Sigma_Residual_VS_Col ;
            aostringstream_pTLegend_Sigma_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;

            if (iScan==0) aostringstream_pTLegend_Sigma_Residual_VS_Col << "#pi^{+}" ;
            if (iScan==1) aostringstream_pTLegend_Sigma_Residual_VS_Col << "e^{+}  " ;
            if (iScan==2) aostringstream_pTLegend_Sigma_Residual_VS_Col << "p " ;
            if (iScan==3) aostringstream_pTLegend_Sigma_Residual_VS_Col << "#mu^{+}" ;

            aostringstream_pTLegend_Sigma_Residual_VS_Col << ", E=" <<std::setprecision(2) << EScan  << " GeV" ;

            std::string pTLegend_Sigma_Residual_VS_Col_String = aostringstream_pTLegend_Sigma_Residual_VS_Col.str() ;  ;

            pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , pTLegend_Sigma_Residual_VS_Col_String.c_str()  , "pl");
          }
        }
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Mean_Residual_VS_Col-------------------------//
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
          double EScan = aAnalysis_Files.m_VariableValue[iFile];

          ReadRootFile aReadRootFile ;
          
          aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);


          if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
            
            if (iScan==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
            if (iScan==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;
            if (iScan==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
            if (iScan==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 6 ) ;
            
            if (iScan==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
            if (iScan==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;
            if (iScan==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
            if (iScan==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 6 ) ;
            
            if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ; 
            if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ; 
            if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ; 
            if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(25) ; 
            if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(22) ; 
            if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(26) ; 
            if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(23) ; 

            pTMultiGraph_Mean_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col, "pl") ;

            std::ostringstream aostringstream_pTLegend_Mean_Residual_VS_Col ;
            aostringstream_pTLegend_Mean_Residual_VS_Col << std::setiosflags(std::ios::fixed) ;

            if (iScan==0) aostringstream_pTLegend_Mean_Residual_VS_Col << "#pi^{+}" ;
            if (iScan==1) aostringstream_pTLegend_Mean_Residual_VS_Col << "e^{+}  " ;
            if (iScan==2) aostringstream_pTLegend_Mean_Residual_VS_Col << "p " ;
            if (iScan==3) aostringstream_pTLegend_Mean_Residual_VS_Col << "#mu^{+}" ;

            aostringstream_pTLegend_Mean_Residual_VS_Col << ", E=" <<std::setprecision(2) << EScan  << " GeV" ;

            std::string pTLegend_Mean_Residual_VS_Col_String = aostringstream_pTLegend_Mean_Residual_VS_Col.str() ;  ;

            pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , pTLegend_Mean_Residual_VS_Col_String.c_str()  , "pl");
          }
        }
//--------------------Mean_Residual_VS_Col-------------------------//

      }
    }
  }
      
  int Kounter = -1 ;
  
//--------------------Sigma_VS_E-------------------------//
  Kounter = -1 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
    pTCanvas->Divide(4,1) ;

    int iMod_Max = 4 ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;
      
      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_E = V_pTMultiGraph_Sigma_VS_E[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_E     = V_pTLegend_Sigma_VS_E    [Kounter] ;
 

      pTMultiGraph_Sigma_VS_E ->Draw("A");
      
      pTMultiGraph_Sigma_VS_E->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_E->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_E->GetXaxis()->SetTitle("E (GeV)");
      pTMultiGraph_Sigma_VS_E->GetYaxis()->SetTitle("Mean of the Sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_E->GetXaxis()->SetLimits(0.,2.0);

      pTCanvas->Update();

      pTLegend_Sigma_VS_E->Draw();
      pTCanvas->Update();
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());

 }
//--------------------Sigma_VS_E-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
    pTCanvas->Divide(4,1) ;

    int iMod_Max = 4 ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;
      
      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = V_pTMultiGraph_Sigma_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Sigma_Residual_VS_Col     = V_pTLegend_Sigma_Residual_VS_Col    [Kounter] ;
 

      pTMultiGraph_Sigma_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Sigma_Residual_VS_Col->SetMinimum(   0. );
      pTMultiGraph_Sigma_Residual_VS_Col->SetMaximum( 500. );
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
      
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitle("Sigma of Residuals (#mu m)");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);

      pTCanvas->Update();

      pTLegend_Sigma_Residual_VS_Col->Draw();
      pTCanvas->Update();
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());

 }
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Mean_Residual_VS_Col-------------------------//
  Kounter = -1 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
    pTCanvas->Divide(4,1) ;

    int iMod_Max = 4 ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;
      
      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col     = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
 

      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-300. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 300. );
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetLimits(-0.5,35.5);
     
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitle("Mean of Residuals (#mu m)");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);

      pTCanvas->Update();

      pTLegend_Mean_Residual_VS_Col->Draw();
      pTCanvas->Update();
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());

 }
//--------------------Mean_Residual_VS_Col-------------------------//

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 600) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}
