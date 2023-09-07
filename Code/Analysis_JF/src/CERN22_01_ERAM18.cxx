#include "Analysis_JF/CERN22_01_ERAM18.h"
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

void CERN22_01_RESO_ERAM18_HS_01()
{ 
  std::string SampleSet = "C22_ERAM18_HS" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_RESO_ERAM18_HS_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  std::vector < std::string >  V_AnalyseName ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Mean_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Mean_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_Residual_VS_Col ;
  std::vector < TLegend* >     V_pTLegend_Sigma_Residual_VS_Col     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Sigma_VS_Type ;
  std::vector < TLegend* >     V_pTLegend_Sigma_VS_Type     ;
  
  std::vector < TMultiGraph* > V_pTMultiGraph_Bias_VS_Type ;
  std::vector < TLegend* >     V_pTLegend_Bias_VS_Type     ;
  
  std::string AnalyseName = "Bidon" ;

  int iAna_Max = 3 ;
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
    if (iAna==0) AnalyseName = "PV0_ERAM18_HS_Module_Nber_0" ;
    if (iAna==1) AnalyseName = "PV2_ERAM18_HS_Module_Nber_0" ;
    if (iAna==2) AnalyseName = "PV2_ERAM18_HS_improved_Module_Nber_0" ;
    
    V_AnalyseName.push_back(AnalyseName);
  
//
    std::string SampleSet_Specific = "ERAM18_HS" ;

    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------Mean_Residual_VS_Col-------------------------//
    TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = new TMultiGraph();   
    

    TLegend* pTLegend_Mean_Residual_VS_Col = new TLegend(0.56,0.20 ,0.75,0.40); 
    pTLegend_Mean_Residual_VS_Col->SetFillStyle(1001);
    pTLegend_Mean_Residual_VS_Col->SetBorderSize(1);

    V_pTMultiGraph_Mean_Residual_VS_Col.push_back(pTMultiGraph_Mean_Residual_VS_Col)  ; 
    V_pTLegend_Mean_Residual_VS_Col    .push_back(pTLegend_Mean_Residual_VS_Col    )  ; 

    for (int iFile = 0 ; iFile< iFile_Max; iFile++){

      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      double Type = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 2 ) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 4 ) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 6 ) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 6 ) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor( 3 ) ;

        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;

        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 2 ) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 4 ) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 6 ) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 6 ) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor( 3 ) ;

         aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerSize(1.8) ;
      
         pTMultiGraph_Mean_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col, "pl") ;
       
         if (Type==0) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , "  e+ 0.5 GeV"  , "pl");
         if (Type==1) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , "  e+ 1.0 GeV"  , "pl");
         if (Type==2) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , " mu- 1.0 GeV"  , "pl");
         if (Type==3) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , " mu+ 0.75 GeV" , "pl");
         if (Type==4) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , " mu+ 1.0 GeV"  , "pl");
         if (Type==5) pTLegend_Mean_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , "  p  1.0 GeV"  , "pl");
      } 
                
    }
//--------------------Mean_Residual_VS_Col-------------------------//

//--------------------Sigma_Residual_VS_Col-------------------------//
    TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = new TMultiGraph();      

    TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.56,0.20 ,0.75,0.40); 
    pTLegend_Sigma_Residual_VS_Col->SetFillStyle(1001);
    pTLegend_Sigma_Residual_VS_Col->SetBorderSize(1);

    V_pTMultiGraph_Sigma_Residual_VS_Col.push_back(pTMultiGraph_Sigma_Residual_VS_Col)  ; 
    V_pTLegend_Sigma_Residual_VS_Col    .push_back(pTLegend_Sigma_Residual_VS_Col    )  ; 

    for (int iFile = 0 ; iFile< iFile_Max; iFile++){

      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      double Type = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 2 ) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 4 ) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 6 ) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 6 ) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor( 3 ) ;

        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;

        if (Type==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
        if (Type==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 2 ) ;
        if (Type==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 4 ) ;
        if (Type==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 6 ) ;
        if (Type==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 6 ) ;
        if (Type==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor( 3 ) ;

         aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerSize(1.8) ;
      
         pTMultiGraph_Sigma_Residual_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col, "pl") ;
       
         if (Type==0) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , "  e+ 0.5 GeV"  , "pl");
         if (Type==1) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , "  e+ 1.0 GeV"  , "pl");
         if (Type==2) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , " mu- 1.0 GeV"  , "pl");
         if (Type==3) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , " mu+ 0.75 GeV" , "pl");
         if (Type==4) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , " mu+ 1.0 GeV"  , "pl");
         if (Type==5) pTLegend_Sigma_Residual_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , "  p  1.0 GeV"  , "pl");
      } 
                
    }
//--------------------Sigma_Residual_VS_Col-------------------------//

//--------------------Sigma_VS_Type-------------------------//
    TMultiGraph* pTMultiGraph_Sigma_VS_Type = new TMultiGraph();      

    TLegend* pTLegend_Sigma_VS_Type = new TLegend(0.56,0.20 ,0.75,0.40); 
    pTLegend_Sigma_VS_Type->SetFillStyle(1001);
    pTLegend_Sigma_VS_Type->SetBorderSize(1);

    V_pTMultiGraph_Sigma_VS_Type.push_back(pTMultiGraph_Sigma_VS_Type)  ; 
    V_pTLegend_Sigma_VS_Type    .push_back(pTLegend_Sigma_VS_Type    )  ; 

    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      double Type = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
        double Mean = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetMean(2) ;
        double RMS  = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetRMS(2)  ;
        double TheN = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()  ;
                                                  
        double RMSLocal = RMS ;
        RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
        
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        if (Type==0) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==1) pTGraphErrors->SetMarkerStyle(24) ;
        if (Type==2) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==3) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==4) pTGraphErrors->SetMarkerStyle(24) ;
        if (Type==5) pTGraphErrors->SetMarkerStyle(20) ;

        if (Type==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (Type==1) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (Type==2) pTGraphErrors->SetMarkerColor( 4 ) ; 
        if (Type==3) pTGraphErrors->SetMarkerColor( 6 ) ; 
        if (Type==4) pTGraphErrors->SetMarkerColor( 6 ) ; 
        if (Type==5) pTGraphErrors->SetMarkerColor( 3 ) ; 

        pTGraphErrors ->SetPoint     ( 0 , Type , Mean      ) ;
        pTGraphErrors ->SetPointError( 0 , 0.   , RMSLocal  ) ;
        
        if (iAna==0){
          std::cout 
            << " iAna "        << std::setw(3) << std::setprecision(0) << iAna
            << " AnalyseName " << AnalyseName
            << " Type "        << std::setw(3) << std::setprecision(0) << Type 
            ;
            
          if (Type==0) std::cout << std::setw(20) << "  e+ 0.5 GeV"  ;
          if (Type==1) std::cout << std::setw(20) << "  e+ 1.0 GeV"  ;
          if (Type==2) std::cout << std::setw(20) << " mu- 1.0 GeV"  ;
          if (Type==3) std::cout << std::setw(20) << " mu+ 0.75 GeV" ;
          if (Type==4) std::cout << std::setw(20) << " mu+ 1.0 GeV"  ;
          if (Type==5) std::cout << std::setw(20) << "  p  1.0 GeV"  ;
            
          std::cout
            << " Reso " << std::setw(3) << std::setprecision(0) << Mean     
            << " +/- "  << std::setw(3) << std::setprecision(0) << RMSLocal 
            << std::endl ;
        }
        
        pTMultiGraph_Sigma_VS_Type->Add( pTGraphErrors , "p" );
        
        pTGraphErrors->SetMarkerSize(1.8) ;
        
        if (Type==0) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , "  e+ 0.5 GeV" , "p");
        if (Type==1) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , "  e+ 1.0 GeV" , "p");
        if (Type==2) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , " mu- 1.0 GeV" , "p");
        if (Type==3) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , " mu+ 0.75 GeV" , "p");
        if (Type==4) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , " mu+ 1.0 GeV" , "p");
        if (Type==5) pTLegend_Sigma_VS_Type->AddEntry( pTGraphErrors , "  p  1.0 GeV" , "p");
      } 
                
    }
//--------------------Sigma_VS_Type-------------------------//
       
//--------------------Bias_VS_Type-------------------------//
    TMultiGraph* pTMultiGraph_Bias_VS_Type = new TMultiGraph();      

    TLegend* pTLegend_Bias_VS_Type = new TLegend(0.56,0.60 ,0.75,0.80); 
    pTLegend_Bias_VS_Type->SetFillStyle(1001);
    pTLegend_Bias_VS_Type->SetBorderSize(1);

    V_pTMultiGraph_Bias_VS_Type.push_back(pTMultiGraph_Bias_VS_Type)  ; 
    V_pTLegend_Bias_VS_Type    .push_back(pTLegend_Bias_VS_Type    )  ; 

    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
 
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      double Type = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
        double Mean = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetMean(2) ;
        double RMS  = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetRMS(2)  ;
        double TheN = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN()     ;

//      Work on Absolute value of the means!!!                                                
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
        
        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        if (Type==0) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==1) pTGraphErrors->SetMarkerStyle(24) ;
        if (Type==2) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==3) pTGraphErrors->SetMarkerStyle(20) ;
        if (Type==4) pTGraphErrors->SetMarkerStyle(24) ;
        if (Type==5) pTGraphErrors->SetMarkerStyle(20) ;

        if (Type==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (Type==1) pTGraphErrors->SetMarkerColor( 2 ) ; 
        if (Type==2) pTGraphErrors->SetMarkerColor( 4 ) ; 
        if (Type==3) pTGraphErrors->SetMarkerColor( 6 ) ; 
        if (Type==4) pTGraphErrors->SetMarkerColor( 6 ) ; 
        if (Type==5) pTGraphErrors->SetMarkerColor( 3 ) ; 

        pTGraphErrors ->SetPoint     ( 0 , Type , Mean      ) ;
        pTGraphErrors ->SetPointError( 0 , 0.   , RMSLocal  ) ;
                   
        pTMultiGraph_Bias_VS_Type->Add( pTGraphErrors , "p" );
        
        pTGraphErrors->SetMarkerSize(1.8) ;
        
        if (Type==0) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , "  e+ 0.5 GeV"  , "p");
        if (Type==1) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , "  e+ 1.0 GeV"  , "p");
        if (Type==2) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , " mu- 1.0 GeV"  , "p");
        if (Type==3) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , " mu+ 0.75 GeV" , "p");
        if (Type==4) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , " mu+ 1.0 GeV"  , "p");
        if (Type==5) pTLegend_Bias_VS_Type->AddEntry( pTGraphErrors , "  p  1.0 GeV"  , "p");
      } 
                
    }
//--------------------Bias_VS_Type-------------------------//

  }

     
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
     std::string  AnalyseName                       = V_AnalyseName[iAna] ;

//--------------------Mean_Residual_VS_Col-------------------------//
    TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col = V_pTMultiGraph_Mean_Residual_VS_Col[iAna] ;
    TLegend*     pTLegend_Mean_Residual_VS_Col     = V_pTLegend_Mean_Residual_VS_Col    [iAna] ;
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitle("Mean of Residuals (#mu m)");
      pTMultiGraph_Mean_Residual_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Mean_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-100.);
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 100.);

      pTCanvas->Update();

      pTLegend_Mean_Residual_VS_Col->Draw();
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------Mean_Residual_VS_Col-------------------------//
  }  
     
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
     std::string  AnalyseName                       = V_AnalyseName[iAna] ;

//--------------------Bias_VS_Type-------------------------//
    TMultiGraph* pTMultiGraph_Bias_VS_Type = V_pTMultiGraph_Bias_VS_Type[iAna] ;
    TLegend*     pTLegend_Bias_VS_Type     = V_pTLegend_Bias_VS_Type    [iAna] ;
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTMultiGraph_Bias_VS_Type ->Draw("A");
      
      pTMultiGraph_Bias_VS_Type->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Bias_VS_Type->GetXaxis()->SetTitle("Type");
      pTMultiGraph_Bias_VS_Type->GetYaxis()->SetTitle("Mean of Absolute Values of Mean of Residuals (#mu m)");
      pTMultiGraph_Bias_VS_Type->GetYaxis()->SetTitleOffset(1.5);
      
      pTMultiGraph_Bias_VS_Type->SetMinimum(  0. );
      pTMultiGraph_Bias_VS_Type->SetMaximum( 30. );
      pTMultiGraph_Bias_VS_Type->GetXaxis()->SetLimits(-0.5,5.5);

      pTCanvas->Update();

      pTLegend_Bias_VS_Type->Draw();
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------Bias_VS_Type-------------------------//
  }  
     
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
     std::string  AnalyseName                       = V_AnalyseName[iAna] ;

//--------------------Sigma_Residual_VS_Col-------------------------//
    TMultiGraph* pTMultiGraph_Sigma_Residual_VS_Col = V_pTMultiGraph_Sigma_Residual_VS_Col[iAna] ;
    TLegend*     pTLegend_Sigma_Residual_VS_Col     = V_pTLegend_Sigma_Residual_VS_Col    [iAna] ;
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTMultiGraph_Sigma_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Sigma_Residual_VS_Col->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitle("Sigma of Residuals (#mu m)");
      pTMultiGraph_Sigma_Residual_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Sigma_Residual_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_Sigma_Residual_VS_Col->SetMinimum(   0.);
      pTMultiGraph_Sigma_Residual_VS_Col->SetMaximum( 500.);

      pTCanvas->Update();

      pTLegend_Sigma_Residual_VS_Col->Draw();
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------Sigma_Residual_VS_Col-------------------------//
  }  
     
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
     std::string  AnalyseName                       = V_AnalyseName[iAna] ;

//--------------------Sigma_VS_Type-------------------------//
    TMultiGraph* pTMultiGraph_Sigma_VS_Type = V_pTMultiGraph_Sigma_VS_Type[iAna] ;
    TLegend*     pTLegend_Sigma_VS_Type     = V_pTLegend_Sigma_VS_Type    [iAna] ;
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTMultiGraph_Sigma_VS_Type ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Type->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Sigma_VS_Type->GetXaxis()->SetTitle("Type");
      pTMultiGraph_Sigma_VS_Type->GetYaxis()->SetTitle("Mean of the Sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Type->GetYaxis()->SetTitleOffset(1.5);
      
      pTMultiGraph_Sigma_VS_Type->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Type->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Type->GetXaxis()->SetLimits(-0.5,5.5);

      pTCanvas->Update();

      pTLegend_Sigma_VS_Type->Draw();
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------Sigma_VS_Type-------------------------//
  }
  
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_RC_ERAM18_HS_01()
{ 
  std::string SampleSet = "C22_ERAM18_HS" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_RC_ERAM18_HS_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

  int iAna_Max = 2 ;
  for (int iAna = 0 ; iAna< iAna_Max; iAna++){
    std::string AnalyseName = "Bidon" ;
    if (iAna==0) AnalyseName = "PV2_ERAM18_HS_Module_Nber_0" ;
    if (iAna==1) AnalyseName = "PV2_ERAM18_HS_improved_Module_Nber_0" ;
    
//
    std::string SampleSet_Specific = "ERAM18_HS" ;
    
    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//
    TMultiGraph* pTMultiGraph  = new TMultiGraph();  
    
    std::ostringstream aostringstream_pTMultiGraph ;
    aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
    aostringstream_pTMultiGraph << AnalyseName  ;

    pTMultiGraph->SetTitle((aostringstream_pTMultiGraph.str()).c_str());

    TLegend* pTLegend = new TLegend(0.56,0.20 ,0.75,0.38); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    for (int iFile = 0 ; iFile< iFile_Max; iFile++){

      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      double Type = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
      
      TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
      
      double  TheValue =  pTF1_OK->GetParameter(0) ;
      double eTheValue =  pTF1_OK->GetParError (0) ;
      
      TGraphErrors* pTGraphErrors = new TGraphErrors ;
      if (Type==0) pTGraphErrors->SetMarkerStyle(20) ;
      if (Type==1) pTGraphErrors->SetMarkerStyle(24) ;
      if (Type==2) pTGraphErrors->SetMarkerStyle(20) ;
      if (Type==3) pTGraphErrors->SetMarkerStyle(20) ;
      if (Type==4) pTGraphErrors->SetMarkerStyle(24) ;
      if (Type==5) pTGraphErrors->SetMarkerStyle(20) ;

      if (Type==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
      if (Type==1) pTGraphErrors->SetMarkerColor( 2 ) ; 
      if (Type==2) pTGraphErrors->SetMarkerColor( 4 ) ; 
      if (Type==3) pTGraphErrors->SetMarkerColor( 6 ) ; 
      if (Type==4) pTGraphErrors->SetMarkerColor( 6 ) ; 
      if (Type==5) pTGraphErrors->SetMarkerColor( 3 ) ; 
       
      pTGraphErrors->SetPoint     (0, Type , TheValue ) ;
      pTGraphErrors->SetPointError(0, 0    ,eTheValue ) ;
          
      pTMultiGraph->Add( pTGraphErrors , "p" );

      pTGraphErrors->SetMarkerSize(1.8) ;

      if (Type==0) pTLegend->AddEntry( pTGraphErrors , "  e+ 0.5 GeV"  , "p");
      if (Type==1) pTLegend->AddEntry( pTGraphErrors , "  e+ 1.0 GeV"  , "p");
      if (Type==2) pTLegend->AddEntry( pTGraphErrors , " mu- 1.0 GeV"  , "p");
      if (Type==3) pTLegend->AddEntry( pTGraphErrors , " mu+ 0.75 GeV" , "p");
      if (Type==4) pTLegend->AddEntry( pTGraphErrors , " mu+ 1.0 GeV"  , "p");
      if (Type==5) pTLegend->AddEntry( pTGraphErrors , "  p  1.0 GeV"  , "p");

    }

//--------------------RC_VS_Type-------------------------//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTMultiGraph ->Draw("A");
      
      pTMultiGraph->GetXaxis()->SetTitle("Type");
      pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
      
      pTMultiGraph->SetMinimum(  0. );
      pTMultiGraph->SetMaximum(100. );
      pTMultiGraph->GetXaxis()->SetLimits(-0.5,5.5);

      pTCanvas->Update();

      pTLegend->Draw();
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------RC_VS_Type-------------------------//
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
  
}




