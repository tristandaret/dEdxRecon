#include "Analysis_JF/CERN22_01_SIM.h"
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

void CERN22_01_SIM()
{ 
  CERN22_01_SIM_PV2_RESO_ZSCAN_01() ;
  CERN22_01_SIM_PV2_RC_ZSCAN_01  () ;

  CERN22_01_SIM_PV3_RESO_ZSCAN_01() ;
  CERN22_01_SIM_PV3_RC_ZSCAN_01  () ;

  CERN22_01_SIM_RcUnknown_PV2_RESO_ZSCAN_01() ;
  CERN22_01_SIM_RcUnknown_PV2_RC_ZSCAN_01  () ;

  CERN22_01_SIM_RcUnknown_PV3_RESO_ZSCAN_01() ;
  CERN22_01_SIM_RcUnknown_PV3_RC_ZSCAN_01  () ;

}

void CERN22_01_SIM_PV3_RC_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_PV3_RC_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph = new TMultiGraph();   
  std::vector < TGraphErrors* > V_pTGraphErrors ;

  TLegend* pTLegend = new TLegend(0.12,0.15 ,0.89,0.35); 
  pTLegend->SetBorderSize(1);
  pTLegend->SetMargin(0.1) ;
  
  int iProc_Min = 1 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;

    if (iProc==1) Procedure = "PV3" ;
    if (iProc==2) Procedure = "PV3" ;
    if (iProc==3) Procedure = "PV3" ;

    int ModuleNber = 0 ;
    
    std::string SampleSet_Specific = "SIM_221006" ;
      
    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

//TAGADA 
//    if (iProc==1) SampleSet_Specific = "SIM_221006_TEST10_25MHZ" ;           ;

    std::string AnalyseName = "BIDON" ;
    
    std::ostringstream aostringstream_AnalyseName ;
    aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
    aostringstream_AnalyseName << Procedure  ;
    aostringstream_AnalyseName << "_"  ;
    aostringstream_AnalyseName << SampleSet_Specific   ;
    
    if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
    if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
    if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
    
    aostringstream_AnalyseName << "_Module_Nber_"   ;
    aostringstream_AnalyseName << ModuleNber   ;

    AnalyseName = aostringstream_AnalyseName.str() ;  ;

    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
    TGraphErrors* pTGraphErrors = new TGraphErrors ;

    pTGraphErrors->SetMarkerSize(1.8) ;

    pTGraphErrors->SetMarkerStyle(24) ;
    if (iProc==1) pTGraphErrors->SetMarkerStyle(21) ;
    if (iProc==2) pTGraphErrors->SetMarkerStyle(25) ;
    if (iProc==3) pTGraphErrors->SetMarkerStyle(24) ;

    pTGraphErrors->SetMarkerColor( 2 ) ; 
    if (iProc==1) pTGraphErrors->SetMarkerColor( 2 ) ;
    if (iProc==2) pTGraphErrors->SetMarkerColor( 4 ) ;
    if (iProc==3) pTGraphErrors->SetMarkerColor( 3 ) ;

    pTGraphErrors->SetLineColor( 1 ) ; 
    if (iProc==1) pTGraphErrors->SetLineColor( 2 ) ; 
    if (iProc==2) pTGraphErrors->SetLineColor( 4 ) ; 
    if (iProc==3) pTGraphErrors->SetLineColor( 3 ) ; 

    V_pTGraphErrors.push_back(pTGraphErrors) ;
    
    pTMultiGraph->Add( pTGraphErrors , "pl" );
    
    double RC_min   = 0 ;
    double RC_max   = 0 ;
    int    RC_first = 1 ;

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
    
    double AVG  = (RC_max+RC_min)/2. ;
    double Diff = 100.*(RC_max-RC_min)/AVG ;
    
    std::ostringstream aostringstream_pTLegend ;
    aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

    if (iProc==1) aostringstream_pTLegend << "PV3 Standard " ;
    if (iProc==2) aostringstream_pTLegend << "PV3 Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;   
    if (iProc==3) aostringstream_pTLegend << "PV3 Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
    
    aostringstream_pTLegend << ", RC = "   ;
    aostringstream_pTLegend << std::setprecision(0) << AVG   ;
    aostringstream_pTLegend << " ns mm^{-2} "  ;
    aostringstream_pTLegend << "+/- "   ;
    aostringstream_pTLegend << std::setprecision(0) << Diff/2   ;
    aostringstream_pTLegend << " %"   ;
    pTGraphErrors->SetTitle((aostringstream_pTLegend.str()).c_str() );
    pTLegend->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str() , "pl");
        
//--------------------RC_VS_Z-------------------------//
    
  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

//--------------------RC_VS_Z-------------------------//
 
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum(120. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
 
    pTCanvas->Update();
    
    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 100. , Hmax , 100. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();
    
    pTLegend ->Draw();
 
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------RC_VS_Z-------------------------//
    
//--------------------RC_VS_Z-------------------------//
  int iView_Min = 0 ;
  int iView_Max = 3 ;
  for (int iView = iView_Min ; iView< iView_Max; iView++){
    TMultiGraph* pTMultiGraph = new TMultiGraph();   

    TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
    pTLegend->SetBorderSize(1);
    pTLegend->SetMargin(0.1) ;
    
    if (iView>=0) pTMultiGraph->Add( V_pTGraphErrors[0] , "pl" );
    if (iView>=1) pTMultiGraph->Add( V_pTGraphErrors[1] , "pl" );
    if (iView>=2) pTMultiGraph->Add( V_pTGraphErrors[2] , "pl" );
    if (iView>=3) pTMultiGraph->Add( V_pTGraphErrors[3] , "pl" );

    if (iView>=0) pTLegend->AddEntry( V_pTGraphErrors[0] , V_pTGraphErrors[0]->GetTitle() , "pl");
    if (iView>=1) pTLegend->AddEntry( V_pTGraphErrors[1] , V_pTGraphErrors[1]->GetTitle() , "pl");
    if (iView>=2) pTLegend->AddEntry( V_pTGraphErrors[2] , V_pTGraphErrors[2]->GetTitle() , "pl");
    if (iView>=3) pTLegend->AddEntry( V_pTGraphErrors[3] , V_pTGraphErrors[3]->GetTitle() , "pl");
    
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum(120. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
    
    pTCanvas->Update();

    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 100. , Hmax , 100. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();

    pTLegend ->Draw();
    
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------RC_VS_Z-------------------------//
  

//
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_SIM_PV3_RESO_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_PV3_RESO_ZSCAN_01.pdf" ; 
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
  
  int iProc_Min = 0 ;
  int iProc_Max = 5 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    
    if (iProc==1) Procedure = "PV3" ;
    if (iProc==2) Procedure = "PV3" ;
    if (iProc==3) Procedure = "PV3" ;
    if (iProc==4) Procedure = "PV3" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      
      if (iProc==1) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
      if (iProc==2) aostringstream_pTMultiGraph << " Standard " ;
      if (iProc==3) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;
      if (iProc==4) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
      
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "SIM_221006" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
      aostringstream_AnalyseName << SampleSet_Specific   ;
        
      if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
      if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
      if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
      if (iProc==4) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
        
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
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*(x/10.) /[1])", 0.0, 1000.);
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-100. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 100. );
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum(  30. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->cd(iMod+1)->GetUymin() ;
      double Vmax = pTCanvas->cd(iMod+1)->GetUymax() ;
      TLine* pTLineV = new TLine(  0.  , Vmin ,  0. , Vmax ) ;
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

void CERN22_01_SIM_PV2_RC_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_PV2_RC_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph = new TMultiGraph();   
  std::vector < TGraphErrors* > V_pTGraphErrors ;

  TLegend* pTLegend = new TLegend(0.12,0.15 ,0.89,0.35); 
  pTLegend->SetBorderSize(1);
  pTLegend->SetMargin(0.1) ;
  
  int iProc_Min = 1 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;

    if (iProc==1) Procedure = "PV2" ;
    if (iProc==2) Procedure = "PV2" ;
    if (iProc==3) Procedure = "PV2" ;

    int ModuleNber = 0 ;
    
    std::string SampleSet_Specific = "SIM_221006" ;
      
    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

    std::string AnalyseName = "BIDON" ;
    
    std::ostringstream aostringstream_AnalyseName ;
    aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
    aostringstream_AnalyseName << Procedure  ;
    aostringstream_AnalyseName << "_"  ;
    aostringstream_AnalyseName << SampleSet_Specific   ;
    
    if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
    if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
    if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
    
    aostringstream_AnalyseName << "_Module_Nber_"   ;
    aostringstream_AnalyseName << ModuleNber   ;

    AnalyseName = aostringstream_AnalyseName.str() ;  ;

    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
    TGraphErrors* pTGraphErrors = new TGraphErrors ;

    pTGraphErrors->SetMarkerSize(1.8) ;

    pTGraphErrors->SetMarkerStyle(24) ;
    if (iProc==1) pTGraphErrors->SetMarkerStyle(21) ;
    if (iProc==2) pTGraphErrors->SetMarkerStyle(25) ;
    if (iProc==3) pTGraphErrors->SetMarkerStyle(24) ;

    pTGraphErrors->SetMarkerColor( 2 ) ; 
    if (iProc==1) pTGraphErrors->SetMarkerColor( 2 ) ;
    if (iProc==2) pTGraphErrors->SetMarkerColor( 4 ) ;
    if (iProc==3) pTGraphErrors->SetMarkerColor( 3 ) ;

    pTGraphErrors->SetLineColor( 1 ) ; 
    if (iProc==1) pTGraphErrors->SetLineColor( 2 ) ; 
    if (iProc==2) pTGraphErrors->SetLineColor( 4 ) ; 
    if (iProc==3) pTGraphErrors->SetLineColor( 3 ) ; 

    V_pTGraphErrors.push_back(pTGraphErrors) ;
    
    pTMultiGraph->Add( pTGraphErrors , "pl" );
    
    double RC_min   = 0 ;
    double RC_max   = 0 ;
    int    RC_first = 1 ;

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
    
    double AVG  = (RC_max+RC_min)/2. ;
    double Diff = 100.*(RC_max-RC_min)/AVG ;
    
    std::ostringstream aostringstream_pTLegend ;
    aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

    if (iProc==1) aostringstream_pTLegend << "PV2 Standard " ;
    if (iProc==2) aostringstream_pTLegend << "PV2 Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;   
    if (iProc==3) aostringstream_pTLegend << "PV2 Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
    
    aostringstream_pTLegend << ": RC = "   ;
    aostringstream_pTLegend << std::setprecision(0) << AVG   ;
    aostringstream_pTLegend << " ns mm^{-2} "  ;
    aostringstream_pTLegend << "+/- "   ;
    aostringstream_pTLegend << std::setprecision(0) << Diff/2   ;
    aostringstream_pTLegend << " %"   ;
    pTGraphErrors->SetTitle((aostringstream_pTLegend.str()).c_str() );
    pTLegend->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str() , "pl");
        
//--------------------RC_VS_Z-------------------------//
    
  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

//--------------------RC_VS_Z-------------------------//
 
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum(120. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
 
    pTCanvas->Update();
    
    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 100. , Hmax , 100. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();
    
    pTLegend ->Draw();
 
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------RC_VS_Z-------------------------//
    
//--------------------RC_VS_Z-------------------------//
  int iView_Min = 0 ;
  int iView_Max = 3 ;
  for (int iView = iView_Min ; iView< iView_Max; iView++){
    TMultiGraph* pTMultiGraph = new TMultiGraph();   

    TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
    pTLegend->SetBorderSize(1);
    pTLegend->SetMargin(0.1) ;
    
    if (iView>=0) pTMultiGraph->Add( V_pTGraphErrors[0] , "pl" );
    if (iView>=1) pTMultiGraph->Add( V_pTGraphErrors[1] , "pl" );
    if (iView>=2) pTMultiGraph->Add( V_pTGraphErrors[2] , "pl" );
    if (iView>=3) pTMultiGraph->Add( V_pTGraphErrors[3] , "pl" );

    if (iView>=0) pTLegend->AddEntry( V_pTGraphErrors[0] , V_pTGraphErrors[0]->GetTitle() , "pl");
    if (iView>=1) pTLegend->AddEntry( V_pTGraphErrors[1] , V_pTGraphErrors[1]->GetTitle() , "pl");
    if (iView>=2) pTLegend->AddEntry( V_pTGraphErrors[2] , V_pTGraphErrors[2]->GetTitle() , "pl");
    if (iView>=3) pTLegend->AddEntry( V_pTGraphErrors[3] , V_pTGraphErrors[3]->GetTitle() , "pl");
    
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum(120. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
    
    pTCanvas->Update();
    
    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 100. , Hmax , 100. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();

    pTLegend ->Draw();
    
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------RC_VS_Z-------------------------//
  

//
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_SIM_PV2_RESO_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_PV2_RESO_ZSCAN_01.pdf" ; 
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
  
  int iProc_Min = 0 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;

    if (iProc==1) Procedure = "PV2" ;
    if (iProc==2) Procedure = "PV2" ;
    if (iProc==3) Procedure = "PV2" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      
      if (iProc==1) aostringstream_pTMultiGraph << " Standard " ;
      if (iProc==2) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;
      if (iProc==3) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
      
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "SIM_221006" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
      aostringstream_AnalyseName << SampleSet_Specific   ;
        
      if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
      if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
      if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
        
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
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*(x/10.) /[1])", 0.0, 1000.);
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-100. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 100. );
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum(  30. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->cd(iMod+1)->GetUymin() ;
      double Vmax = pTCanvas->cd(iMod+1)->GetUymax() ;
      TLine* pTLineV = new TLine(  0.  , Vmin ,  0. , Vmax ) ;
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

void CERN22_01_SIM_RcUnknown_PV2_RC_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_RcUnknown_PV2_RC_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph = new TMultiGraph();   
  std::vector < TGraphErrors* > V_pTGraphErrors ;

  TLegend* pTLegend = new TLegend(0.12,0.15 ,0.89,0.35); 
  pTLegend->SetBorderSize(1);
  pTLegend->SetMargin(0.1) ;
  
  int iProc_Min = 1 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    
    if (iProc==1) Procedure = "PV2" ;    
    if (iProc==2) Procedure = "PV2" ;
    if (iProc==3) Procedure = "PV2" ;

    int ModuleNber = 0 ;
    
    std::string SampleSet_Specific = "SIM_RcUnknown_221006" ;
      
    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

    std::string AnalyseName = "BIDON" ;
    
    std::ostringstream aostringstream_AnalyseName ;
    aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
    aostringstream_AnalyseName << Procedure  ;
    aostringstream_AnalyseName << "_"  ;
    aostringstream_AnalyseName << SampleSet_Specific   ;
    
    if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
    if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
    if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
    
    aostringstream_AnalyseName << "_Module_Nber_"   ;
    aostringstream_AnalyseName << ModuleNber   ;

    AnalyseName = aostringstream_AnalyseName.str() ;  ;

    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
    TGraphErrors* pTGraphErrors = new TGraphErrors ;

    pTGraphErrors->SetMarkerSize(1.8) ;

    pTGraphErrors->SetMarkerStyle(24) ;
    if (iProc==1) pTGraphErrors->SetMarkerStyle(21) ;
    if (iProc==2) pTGraphErrors->SetMarkerStyle(25) ;
    if (iProc==3) pTGraphErrors->SetMarkerStyle(24) ;

    pTGraphErrors->SetMarkerColor( 2 ) ; 
    if (iProc==1) pTGraphErrors->SetMarkerColor( 2 ) ;
    if (iProc==2) pTGraphErrors->SetMarkerColor( 4 ) ;
    if (iProc==3) pTGraphErrors->SetMarkerColor( 3 ) ;

    pTGraphErrors->SetLineColor( 1 ) ; 
    if (iProc==1) pTGraphErrors->SetLineColor( 2 ) ; 
    if (iProc==2) pTGraphErrors->SetLineColor( 4 ) ; 
    if (iProc==3) pTGraphErrors->SetLineColor( 3 ) ; 

    V_pTGraphErrors.push_back(pTGraphErrors) ;
    
    pTMultiGraph->Add( pTGraphErrors , "pl" );
    
    double RC_min   = 0 ;
    double RC_max   = 0 ;
    int    RC_first = 1 ;

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
    
    double AVG  = (RC_max+RC_min)/2. ;
    double Diff = 100.*(RC_max-RC_min)/AVG ;
    
    std::ostringstream aostringstream_pTLegend ;
    aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

    if (iProc==1) aostringstream_pTLegend << "PV2 Standard " ;
    if (iProc==2) aostringstream_pTLegend << "PV2 Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;   
    if (iProc==3) aostringstream_pTLegend << "PV2 Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
    
    aostringstream_pTLegend << ": RC = "   ;
    aostringstream_pTLegend << std::setprecision(0) << AVG   ;
    aostringstream_pTLegend << " ns mm^{-2} "  ;
    aostringstream_pTLegend << "+/- "   ;
    aostringstream_pTLegend << std::setprecision(0) << Diff/2   ;
    aostringstream_pTLegend << " %"   ;
    pTGraphErrors->SetTitle((aostringstream_pTLegend.str()).c_str() );
    pTLegend->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str() , "pl");
        
//--------------------RC_VS_Z-------------------------//
    
  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

//--------------------RC_VS_Z-------------------------//
 
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum( 65. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    pTCanvas->Update();
    
    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 55. , Hmax , 55. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();
    
    pTLegend ->Draw();
 
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------RC_VS_Z-------------------------//
    
//--------------------RC_VS_Z-------------------------//
  int iView_Min = 0 ;
  int iView_Max = 3 ;
  for (int iView = iView_Min ; iView< iView_Max; iView++){
    TMultiGraph* pTMultiGraph = new TMultiGraph();   

    TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
    pTLegend->SetBorderSize(1);
    pTLegend->SetMargin(0.1) ;
    
    if (iView>=0) pTMultiGraph->Add( V_pTGraphErrors[0] , "pl" );
    if (iView>=1) pTMultiGraph->Add( V_pTGraphErrors[1] , "pl" );
    if (iView>=2) pTMultiGraph->Add( V_pTGraphErrors[2] , "pl" );
    if (iView>=3) pTMultiGraph->Add( V_pTGraphErrors[3] , "pl" );

    if (iView>=0) pTLegend->AddEntry( V_pTGraphErrors[0] , V_pTGraphErrors[0]->GetTitle() , "pl");
    if (iView>=1) pTLegend->AddEntry( V_pTGraphErrors[1] , V_pTGraphErrors[1]->GetTitle() , "pl");
    if (iView>=2) pTLegend->AddEntry( V_pTGraphErrors[2] , V_pTGraphErrors[2]->GetTitle() , "pl");
    if (iView>=3) pTLegend->AddEntry( V_pTGraphErrors[3] , V_pTGraphErrors[3]->GetTitle() , "pl");
    
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum( 65. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    pTCanvas->Update();

    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 55. , Hmax , 55. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
  
    pTCanvas->Update();
    
    pTLegend ->Draw();
    
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------RC_VS_Z-------------------------//
  

//
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_SIM_RcUnknown_PV2_RESO_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_RcUnknown_PV2_RESO_ZSCAN_01.pdf" ; 
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
  
  int iProc_Min = 0 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    
    if (iProc==1) Procedure = "PV2" ;
    if (iProc==2) Procedure = "PV2" ;
    if (iProc==3) Procedure = "PV2" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      
      if (iProc==1) aostringstream_pTMultiGraph << " Standard " ;
      if (iProc==2) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ; 
      if (iProc==3) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
      
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "SIM_RcUnknown_221006" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
      aostringstream_AnalyseName << SampleSet_Specific   ;
        
      if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
      if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
      if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
        
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
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*(x/10.) /[1])", 0.0, 1000.);
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
      
      TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.16,0.70 ,0.88,0.85); 
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-100. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 100. );
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum(  30. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->cd(iMod+1)->GetUymin() ;
      double Vmax = pTCanvas->cd(iMod+1)->GetUymax() ;
      TLine* pTLineV = new TLine(  0.  , Vmin ,  0. , Vmax ) ;
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

void CERN22_01_SIM_RcUnknown_PV3_RC_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_RcUnknown_PV3_RC_ZSCAN_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph = new TMultiGraph();   
  std::vector < TGraphErrors* > V_pTGraphErrors ;

  TLegend* pTLegend = new TLegend(0.12,0.15 ,0.89,0.35); 
  pTLegend->SetBorderSize(1);
  pTLegend->SetMargin(0.1) ;
  
  int iProc_Min = 1 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    
    if (iProc==1) Procedure = "PV3" ;    
    if (iProc==2) Procedure = "PV3" ;
    if (iProc==3) Procedure = "PV3" ;

    int ModuleNber = 0 ;
    
    std::string SampleSet_Specific = "SIM_RcUnknown_221006" ;
      
    Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

    std::string AnalyseName = "BIDON" ;
    
    std::ostringstream aostringstream_AnalyseName ;
    aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
    aostringstream_AnalyseName << Procedure  ;
    aostringstream_AnalyseName << "_"  ;
    aostringstream_AnalyseName << SampleSet_Specific   ;
    
    if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
    if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
    if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
    
    aostringstream_AnalyseName << "_Module_Nber_"   ;
    aostringstream_AnalyseName << ModuleNber   ;

    AnalyseName = aostringstream_AnalyseName.str() ;  ;

    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;

//--------------------RC_VS_Z-------------------------//
    TGraphErrors* pTGraphErrors = new TGraphErrors ;

    pTGraphErrors->SetMarkerSize(1.8) ;

    pTGraphErrors->SetMarkerStyle(24) ;
    if (iProc==1) pTGraphErrors->SetMarkerStyle(21) ;
    if (iProc==2) pTGraphErrors->SetMarkerStyle(25) ;
    if (iProc==3) pTGraphErrors->SetMarkerStyle(24) ;

    pTGraphErrors->SetMarkerColor( 2 ) ; 
    if (iProc==1) pTGraphErrors->SetMarkerColor( 2 ) ;
    if (iProc==2) pTGraphErrors->SetMarkerColor( 4 ) ;
    if (iProc==3) pTGraphErrors->SetMarkerColor( 3 ) ;

    pTGraphErrors->SetLineColor( 1 ) ; 
    if (iProc==1) pTGraphErrors->SetLineColor( 2 ) ; 
    if (iProc==2) pTGraphErrors->SetLineColor( 4 ) ; 
    if (iProc==3) pTGraphErrors->SetLineColor( 3 ) ; 

    V_pTGraphErrors.push_back(pTGraphErrors) ;
    
    pTMultiGraph->Add( pTGraphErrors , "pl" );
    
    double RC_min   = 0 ;
    double RC_max   = 0 ;
    int    RC_first = 1 ;

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
    
    double AVG  = (RC_max+RC_min)/2. ;
    double Diff = 100.*(RC_max-RC_min)/AVG ;
    
    std::ostringstream aostringstream_pTLegend ;
    aostringstream_pTLegend << std::setiosflags(std::ios::fixed) ;

    if (iProc==1) aostringstream_pTLegend << "PV3 Standard " ;
    if (iProc==2) aostringstream_pTLegend << "PV3 Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ;   
    if (iProc==3) aostringstream_pTLegend << "PV3 Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
    
    aostringstream_pTLegend << ": RC = "   ;
    aostringstream_pTLegend << std::setprecision(0) << AVG   ;
    aostringstream_pTLegend << " ns mm^{-2} "  ;
    aostringstream_pTLegend << "+/- "   ;
    aostringstream_pTLegend << std::setprecision(0) << Diff/2   ;
    aostringstream_pTLegend << " %"   ;
    pTGraphErrors->SetTitle((aostringstream_pTLegend.str()).c_str() );
    pTLegend->AddEntry( pTGraphErrors , (aostringstream_pTLegend.str()).c_str() , "pl");
        
//--------------------RC_VS_Z-------------------------//
    
  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

//--------------------RC_VS_Z-------------------------//
 
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum( 65. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    pTCanvas->Update();
    
    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 55. , Hmax , 55. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
 
    pTCanvas->Update();
    
    pTLegend ->Draw();
 
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
//--------------------RC_VS_Z-------------------------//
    
//--------------------RC_VS_Z-------------------------//
  int iView_Min = 0 ;
  int iView_Max = 3 ;
  for (int iView = iView_Min ; iView< iView_Max; iView++){
    TMultiGraph* pTMultiGraph = new TMultiGraph();   

    TLegend* pTLegend = new TLegend(0.12,0.15 ,0.88,0.30); 
    pTLegend->SetBorderSize(1);
    pTLegend->SetMargin(0.1) ;
    
    if (iView>=0) pTMultiGraph->Add( V_pTGraphErrors[0] , "pl" );
    if (iView>=1) pTMultiGraph->Add( V_pTGraphErrors[1] , "pl" );
    if (iView>=2) pTMultiGraph->Add( V_pTGraphErrors[2] , "pl" );
    if (iView>=3) pTMultiGraph->Add( V_pTGraphErrors[3] , "pl" );

    if (iView>=0) pTLegend->AddEntry( V_pTGraphErrors[0] , V_pTGraphErrors[0]->GetTitle() , "pl");
    if (iView>=1) pTLegend->AddEntry( V_pTGraphErrors[1] , V_pTGraphErrors[1]->GetTitle() , "pl");
    if (iView>=2) pTLegend->AddEntry( V_pTGraphErrors[2] , V_pTGraphErrors[2]->GetTitle() , "pl");
    if (iView>=3) pTLegend->AddEntry( V_pTGraphErrors[3] , V_pTGraphErrors[3]->GetTitle() , "pl");
    
    pTMultiGraph ->Draw("A");
    
    pTMultiGraph->SetMinimum(  0. );
    pTMultiGraph->SetMaximum( 65. );
    pTMultiGraph->GetXaxis()->SetLimits(-100.,1000.);
    
    pTMultiGraph->GetXaxis()->SetTitle("Z (mm)");
    pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    pTCanvas->Update();

    double Hmin = pTCanvas->GetUxmin() ;
    double Hmax = pTCanvas->GetUxmax() ;
    TLine* pTLineH = new TLine( Hmin  , 55. , Hmax , 55. ) ;
    pTLineH->SetLineStyle(2); pTLineH->SetLineWidth(1); pTLineH->SetLineColor(1) ; pTLineH->Draw();  
 
    double Vmin = pTCanvas->GetUymin() ;
    double Vmax = pTCanvas->GetUymax() ;
    TLine* pTLineV = new TLine( 0.  , Vmin , 0. , Vmax ) ;
    pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
  
    pTCanvas->Update();
    
    pTLegend ->Draw();
    
    pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//--------------------RC_VS_Z-------------------------//
  

//
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void CERN22_01_SIM_RcUnknown_PV3_RESO_ZSCAN_01()
{
  std::string SampleSet = "ZScan_SIM" ;
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
  std::string Str_Out_PDF     = OUTDirName + "CERN22_01_SIM_RcUnknown_PV3_RESO_ZSCAN_01.pdf" ; 
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
  
  int iProc_Min = 0 ;
  int iProc_Max = 4 ;
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    std::string Procedure = "BIDON" ;
    if (iProc==0) Procedure = "PV0" ;
    
    if (iProc==1) Procedure = "PV3" ;
    if (iProc==2) Procedure = "PV3" ;
    if (iProc==3) Procedure = "PV3" ;
    
    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
    
      int ModuleNber = iMod ;
      
      std::ostringstream aostringstream_pTMultiGraph ;
      aostringstream_pTMultiGraph << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTMultiGraph << Procedure   ;
      aostringstream_pTMultiGraph << " "   ;
      
      if (iProc==1) aostringstream_pTMultiGraph << " Standard " ;
      if (iProc==2) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=237 #mu m/#sqrt{cm}"   ; 
      if (iProc==3) aostringstream_pTMultiGraph << " Fit peak + C_{TD}=311 #mu m/#sqrt{cm}"   ;
      
      aostringstream_pTMultiGraph << " Module "   ;
      aostringstream_pTMultiGraph << ModuleNber   ;
      
//
      std::string SampleSet_Specific = "SIM_RcUnknown_221006" ;

      Analysis_Files aAnalysis_Files(SampleSet_Specific) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;

      std::string AnalyseName = "BIDON" ;
      
      std::ostringstream aostringstream_AnalyseName ;
      aostringstream_AnalyseName << std::setiosflags(std::ios::fixed) ;
      aostringstream_AnalyseName << Procedure  ;
      aostringstream_AnalyseName << "_"  ;
      aostringstream_AnalyseName << SampleSet_Specific   ;
        
      if (iProc==1) aostringstream_AnalyseName << "_" << "Data_to_Use_0"             ;
      if (iProc==2) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_Unscaled" ;
      if (iProc==3) aostringstream_AnalyseName << "_" << "Data_to_Use_3_DT_311_00"   ;
        
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
                                           
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*(x/10.) /[1])", 0.0, 1000.);
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
      
      TLegend* pTLegend_Sigma_Residual_VS_Col = new TLegend(0.4,0.73 ,0.6,0.88); 
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Mean_Residual_VS_Col  = V_pTMultiGraph_Mean_Residual_VS_Col[Kounter] ;
      TLegend*     pTLegend_Mean_Residual_VS_Col      = V_pTLegend_Mean_Residual_VS_Col    [Kounter] ;
      
      pTMultiGraph_Mean_Residual_VS_Col ->Draw("A");
      
      pTMultiGraph_Mean_Residual_VS_Col->SetMinimum(-100. );
      pTMultiGraph_Mean_Residual_VS_Col->SetMaximum( 100. );
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Bias_VS_Z  = V_pTMultiGraph_Bias_VS_Z[Kounter] ;
      
      pTMultiGraph_Bias_VS_Z ->Draw("A");
      
      pTMultiGraph_Bias_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Bias_VS_Z->SetMaximum(  30. );
      pTMultiGraph_Bias_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

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
  for (int iProc = iProc_Min ; iProc< iProc_Max; iProc++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,1) ;

    int iMod_Min = 0 ;
    int iMod_Max = 1 ;
    for (int iMod = iMod_Min ; iMod< iMod_Max; iMod++){
      pTCanvas->cd(iMod+1) ;

      Kounter +=1 ;
      TMultiGraph* pTMultiGraph_Sigma_VS_Z  = V_pTMultiGraph_Sigma_VS_Z[Kounter] ;
      TLegend*     pTLegend_Sigma_VS_Z      = V_pTLegend_Sigma_VS_Z    [Kounter] ;
      
      pTMultiGraph_Sigma_VS_Z ->Draw("A");
      
      pTMultiGraph_Sigma_VS_Z->SetMinimum(   0. );
      pTMultiGraph_Sigma_VS_Z->SetMaximum( 500. );
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetLimits(-100.,1000.);
      
      pTMultiGraph_Sigma_VS_Z->GetXaxis()->SetTitle("Z (mm)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitle("Mean of the sigmas of Residuals (#mu m)");
      pTMultiGraph_Sigma_VS_Z->GetYaxis()->SetTitleOffset(1.5);
      
      double Vmin = pTCanvas->cd(iMod+1)->GetUymin() ;
      double Vmax = pTCanvas->cd(iMod+1)->GetUymax() ;
      TLine* pTLineV = new TLine(  0.  , Vmin ,  0. , Vmax ) ;
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

