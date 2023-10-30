#include "Analysis_JF/P_Synthesis_01.h"
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

void P_Synthesis_01()
{

// P_Synthesis_Sp01();


//-------------------//
//   P_Synthesis_02( "ScanZZ_02T_400ns_CERN_1" );
//   P_Synthesis_02( "ScanZZ_02T_200ns_CERN_1" );
//   P_Synthesis_02( "Mag0_CERN_1"             );
 
//   P_Synthesis_02( "ScanZZ_02T_400ns_CERN_2" );
//   P_Synthesis_02( "ScanZZ_02T_200ns_CERN_2" );
//   P_Synthesis_02( "Mag0_CERN_2"             );
 
//   P_Synthesis_02( "ScanZZ_02T_400ns_CERN_3" );
//   P_Synthesis_02( "ScanZZ_02T_200ns_CERN_3" );
//   P_Synthesis_02( "Mag0_CERN_3"             );
 
//   P_Synthesis_02( "ScanZZ_02T_400ns_CERN_4" );
//   P_Synthesis_02( "ScanZZ_02T_200ns_CERN_4" );
//   P_Synthesis_02( "Mag0_CERN_4"             );
  
     P_Synthesis_02( "ScanZZ_02T_400ns_CERN_5" );


}

void P_Synthesis_02(const std::string& SampleSet)
{
    PerSample(SampleSet);
  
    P_Synthesis_DDvsZZ_01(SampleSet);
    P_Synthesis_RCvsZZ_01(SampleSet);
    P_Synthesis_YTYPAD_Iter_01(SampleSet);
    P_Synthesis_YTYPAD_Input_Final_01(SampleSet);
    P_Synthesis_YTYPAD_Final_01(SampleSet);
    P_Synthesis_DumpPRFparameters_01(SampleSet);
    P_Synthesis_ParamPRFLike_Iter_01(SampleSet);
    P_Synthesis_RESO_01(SampleSet);
    P_Synthesis_PULL_01(SampleSet);
    P_Synthesis_QUALITY_01(SampleSet);
    P_Synthesis_CHI2_01(SampleSet);
    P_Synthesis_BEAMPROFILE_01(SampleSet);
}

void P_Synthesis_YTYPAD_Iter_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.20);    
  gStyle->SetStatH(0.20);    
  
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_YTYPAD_Iter_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string PageDeGarde_String = AnalyseName ;
      TText aTText_PageDeGarde ;
      aTText_PageDeGarde.SetNDC();
      aTText_PageDeGarde.SetTextSize(0.06);
      aTText_PageDeGarde.SetTextColor(2);
      aTText_PageDeGarde.DrawText(0.,0.5,PageDeGarde_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;

      int iInOut_start =1 ;
      if (iTurn==0) iInOut_start = 0 ;
      for (int iInOut = iInOut_start ; iInOut< 2; iInOut++){
      
        Analysis_Files aAnalysis_Files(SampleSet) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

          std::string What_ZZ = 
            AnalyseName 
            + " " + aAnalysis_Files.m_VariableName + " " 
            +  aAnalysis_Files.m_VariableValueAsString[iFile];

          ReadRootFile aReadRootFile ;
        
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, IterString);
          
          pTCanvas->cd(iFile+1) ;
          if (iInOut==0){
            if (aReadRootFile.Input_pTH1F_YTrackYPad) aReadRootFile.Input_pTH1F_YTrackYPad->Draw();
          }else{
            if (aReadRootFile.Output_pTH1F_YTrackYPad) aReadRootFile.Output_pTH1F_YTrackYPad->Draw();
          }
          pTCanvas->Update();
          
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

        }
        pTCanvas->Print(Str_Out_PDF.c_str());
      }
    }

    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;

      int iInOut_start =1 ;
      if (iTurn==0) iInOut_start = 0 ;
      for (int iInOut = iInOut_start ; iInOut< 2; iInOut++){
      
        Analysis_Files aAnalysis_Files(SampleSet) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

          std::string What_ZZ = 
            AnalyseName 
            + " " + aAnalysis_Files.m_VariableName + " " 
            +  aAnalysis_Files.m_VariableValueAsString[iFile];

          ReadRootFile aReadRootFile ;
        
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, IterString);
          
          pTCanvas->cd(iFile+1) ;
          if (iInOut==0){
            if (aReadRootFile.Input_pTH1F_YTrackYPadLeading) aReadRootFile.Input_pTH1F_YTrackYPadLeading->Draw();
          }else{
            if (aReadRootFile.Output_pTH1F_YTrackYPadLeading) aReadRootFile.Output_pTH1F_YTrackYPadLeading->Draw();
          }
          pTCanvas->Update();
          
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

        }
        pTCanvas->Print(Str_Out_PDF.c_str());
      }
    }

  }
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_YTYPAD_Final_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.20);    
  gStyle->SetStatH(0.20);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_YTYPAD_Final_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
    pTCanvas->Divide(4,3) ;
      
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, "Turn_9");
        
      pTCanvas->cd(iFile+1) ;
      if(aReadRootFile.Output_pTH1F_YTrackYPad) aReadRootFile.Output_pTH1F_YTrackYPad->Draw();
      pTCanvas->Update();
      
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();

    }
    pTCanvas->Print(Str_Out_PDF.c_str());
   
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
    pTCanvas->Divide(4,3) ;
      
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, "Turn_9");
        
      pTCanvas->cd(iFile+1) ;
      if(aReadRootFile.Output_pTH1F_YTrackYPadLeading) aReadRootFile.Output_pTH1F_YTrackYPadLeading->Draw();
      pTCanvas->Update();
      
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();

    }
    pTCanvas->Print(Str_Out_PDF.c_str());
  }
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}
   
void P_Synthesis_ParamPRFLike_Iter_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_ParamPRFLike_Iter_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    std::vector < std::vector < TGraphErrors* > > V_V_pTGraphErrors      ;
    std::vector < std::vector < std::string   > > V_V_pTGraphErrors_Name ;
    std::vector < std::string                 >   V_pTGraphErrors_Name   ;

//  Determine the number of parameters    
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
    std::string base_EventFile = aAnalysis_Files.m_FilesName[0] ;

    ReadRootFile aReadRootFile ;
    
    aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_0");

    TF1* pTF1_OK = aReadRootFile.pTF1_OK ;

    int NberOfParameters = pTF1_OK->GetNpar();
    for (int iPar = 0; iPar < NberOfParameters; ++iPar) {
      std::vector < TGraphErrors* >    aV_pTGraphErrors ;
      std::vector < std::string > aV_string ;
      V_V_pTGraphErrors      .push_back(aV_pTGraphErrors);
      V_V_pTGraphErrors_Name .push_back(aV_string);
      V_pTGraphErrors_Name   .push_back( std::string(pTF1_OK->GetParName  (iPar)) );
    }

//  Loop on parameters
    for (int iPar = 0; iPar < NberOfParameters; ++iPar) {

//    Loop on Files
      Analysis_Files aAnalysis_Files(SampleSet) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

        std::string What_ZZ = 
          aAnalysis_Files.m_VariableName + " " 
          +  aAnalysis_Files.m_VariableValueAsString[iFile];

        TGraphErrors* pTGraphErrors = new TGraphErrors ;
        
        pTGraphErrors->SetLineColor( 2 ) ; 
        if (iFile==0) pTGraphErrors->SetLineColor( 2 ) ; 
        if (iFile==1) pTGraphErrors->SetLineColor( 2 ) ; 
        if (iFile==2) pTGraphErrors->SetLineColor( 2 ) ;
        if (iFile==3) pTGraphErrors->SetLineColor( 2 ) ;
        if (iFile==4) pTGraphErrors->SetLineColor( 2 ) ;
        if (iFile==5) pTGraphErrors->SetLineColor( 4 ) ;
        if (iFile==6) pTGraphErrors->SetLineColor( 4 ) ;
        if (iFile==7) pTGraphErrors->SetLineColor( 4 ) ;
        if (iFile==8) pTGraphErrors->SetLineColor( 4 ) ;
        if (iFile==9) pTGraphErrors->SetLineColor( 4 ) ;

        pTGraphErrors->SetMarkerStyle(20) ;
        if (iFile==0) pTGraphErrors->SetMarkerStyle(20) ;
        if (iFile==1) pTGraphErrors->SetMarkerStyle(21) ;
        if (iFile==2) pTGraphErrors->SetMarkerStyle(22) ;
        if (iFile==3) pTGraphErrors->SetMarkerStyle(24) ;
        if (iFile==4) pTGraphErrors->SetMarkerStyle(25) ;
        if (iFile==5) pTGraphErrors->SetMarkerStyle(20) ;
        if (iFile==6) pTGraphErrors->SetMarkerStyle(21) ;
        if (iFile==7) pTGraphErrors->SetMarkerStyle(22) ;
        if (iFile==8) pTGraphErrors->SetMarkerStyle(24) ;
        if (iFile==9) pTGraphErrors->SetMarkerStyle(25) ;
        
        pTGraphErrors->SetMarkerSize(1.8) ;
    
//      Loop on Turn
        int iTurn_Max = 10 ;
        for (int iTurn = 0 ; iTurn< iTurn_Max; iTurn++){
          std::string IterString = "BIDON" ;
          if (iTurn==0) IterString = "Turn_0" ;
          if (iTurn==1) IterString = "Turn_1" ;
          if (iTurn==2) IterString = "Turn_2" ;
          if (iTurn==3) IterString = "Turn_3" ;
          if (iTurn==4) IterString = "Turn_4" ;
          if (iTurn==5) IterString = "Turn_5" ;
          if (iTurn==6) IterString = "Turn_6" ;
          if (iTurn==7) IterString = "Turn_7" ;
          if (iTurn==8) IterString = "Turn_8" ;
          if (iTurn==9) IterString = "Turn_9" ;

          ReadRootFile aReadRootFile ;
         
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, IterString);
        
          TF1* pTF1_OK  = aReadRootFile.pTF1_OK ;
          
          double  TheValue =  pTF1_OK->GetParameter(iPar) ;
          double eTheValue =  pTF1_OK->GetParError (iPar) ;
          pTGraphErrors->SetPoint(pTGraphErrors->GetN(), double(iTurn) , TheValue  ) ;
          pTGraphErrors->SetPointError(pTGraphErrors->GetN()-1, 0,eTheValue);

        }
        V_V_pTGraphErrors     [iPar].push_back(pTGraphErrors) ;
        V_V_pTGraphErrors_Name[iPar].push_back(What_ZZ) ;
      }
    }

    int iPar_Max = V_V_pTGraphErrors.size() ;
    for (int iPar = 0; iPar < iPar_Max; ++iPar) {
      TMultiGraph* pTMultiGraph  = new TMultiGraph();  
          
      TLegend* pTLegend = new TLegend(0.88,0.15 ,0.98,0.38  ); 
      pTLegend->SetFillStyle(1001);
      pTLegend->SetBorderSize(1);

      int iFile_Max = V_V_pTGraphErrors     [iPar].size() ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
        pTMultiGraph->Add( (V_V_pTGraphErrors[iPar])[iFile] , "pl" );
        pTLegend->AddEntry( (V_V_pTGraphErrors[iPar])[iFile] , ((V_V_pTGraphErrors_Name[iPar])[iFile]).c_str() , "pl");
      }

      pTMultiGraph->SetTitle(AnalyseName.c_str());
      pTMultiGraph->GetXaxis()->SetTitle("Iterations");
      pTMultiGraph->GetYaxis()->SetTitle(V_pTGraphErrors_Name[iPar].c_str());
      pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
      
      if (iCase==1 || iCase==2){
        std::size_t found = SampleSet.find("ScanZZ");
        if (found!=std::string::npos) {
          pTMultiGraph->SetMinimum(  50.);
          pTMultiGraph->SetMaximum( 105.);
        }
      }
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      
          pTMultiGraph->Draw("A");
          pTCanvas->Update();

          pTLegend->Draw();
          pTCanvas->Update();
      
      pTCanvas->Print(Str_Out_PDF.c_str());

    }
    
  }
  
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
    
}

void P_Synthesis_DumpPRFparameters_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

//Redirect Output
  std::cout << std::endl ;
  std::ostringstream aostringstream_OutPut_Analysis ;
  aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
  aostringstream_OutPut_Analysis << OUTDirName << SampleSet << "_" << "P_Synthesis_DumpPRFparameters_01"<< ".txt" ;
  
  std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
  
  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file
 
//
  std::cout << "List of the values of the parameters of the shape function at the end of the last iteration " << std::endl ;
  std::cout << std::endl ;
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    std::cout << std::endl ;
    
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
        
      TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
      
      std::cout 
        << " "  << std::setw(10)  << AnalyseName 
        << " | " ;
      std::cout 
        << " "  << std::setw(10)  << What_ZZ 
        << " | " ;
      for (int iPar = 0; iPar < pTF1_OK->GetNpar(); ++iPar) {
        std::cout 
          << " "  << std::setw(4 )                         << pTF1_OK->GetParName  (iPar) 
          << "="  << std::setw(10) << std::setprecision(7) << pTF1_OK->GetParameter(iPar) 
          << " | " ;
      }
      std::cout << std::endl ;
      std::cout 
        << " "  << std::setw(10)  << "" 
        << " | " ;
      std::cout 
        << " "  << std::setw(10)  << "" 
        << " | " ;
      for (int iPar = 0; iPar < pTF1_OK->GetNpar(); ++iPar) {
        std::cout 
          << " "  << std::setw(4 )                         << "" 
          << " "  << std::setw(10) << std::setprecision(7) << pTF1_OK->GetParError(iPar) 
          << " | " ;
      }
      std::cout << std::endl ;
    }

  }

//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

}

void P_Synthesis_YTYPAD_Input_Final_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.20);    
  gStyle->SetStatH(0.20);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_YTYPAD_Input_Final_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string PageDeGarde_String = AnalyseName ;
      TText aTText_PageDeGarde ;
      aTText_PageDeGarde.SetNDC();
      aTText_PageDeGarde.SetTextSize(0.06);
      aTText_PageDeGarde.SetTextColor(2);
      aTText_PageDeGarde.DrawText(0.,0.5,PageDeGarde_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
    
    int CDpos = 3 ;
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile_Turn_0 ;
     
      aReadRootFile_Turn_0.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_0");

      ReadRootFile aReadRootFile_Turn_9;
     
      aReadRootFile_Turn_9.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
        
//  
      if (CDpos==3){
        pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
        pTCanvas->Divide(3,2) ;
        CDpos = 0 ;
      }
//
          CDpos += 1 ;
          pTCanvas->cd(CDpos) ;
          if(aReadRootFile_Turn_0.Input_pTH1F_YTrackYPad) aReadRootFile_Turn_0.Input_pTH1F_YTrackYPad->Draw();
          pTCanvas->Update();
          
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();
         
          pTCanvas->cd(CDpos+3) ;
          if(aReadRootFile_Turn_9.Output_pTH1F_YTrackYPad) aReadRootFile_Turn_9.Output_pTH1F_YTrackYPad->Draw();
          pTCanvas->Update();
          
          TText aTText_cd4 ;
          aTText_cd4.SetNDC();
          aTText_cd4.SetTextSize(0.06);
          aTText_cd4.SetTextColor(2);
          aTText_cd4.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

      if (CDpos==3){
        pTCanvas->Print(Str_Out_PDF.c_str());
      }

    }
    if (CDpos!=3){
      pTCanvas->Print(Str_Out_PDF.c_str());
    }
    
    CDpos = 3 ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile_Turn_0 ;
     
      aReadRootFile_Turn_0.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_0");

      ReadRootFile aReadRootFile_Turn_9;
     
      aReadRootFile_Turn_9.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
        
//  
      if (CDpos==3){
        pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
        pTCanvas->Divide(3,2) ;
        CDpos = 0 ;
      }
//
          CDpos += 1 ;
          pTCanvas->cd(CDpos) ;
          if(aReadRootFile_Turn_0.Input_pTH1F_YTrackYPadLeading) aReadRootFile_Turn_0.Input_pTH1F_YTrackYPadLeading->Draw();
          pTCanvas->Update();
          
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();
         
          pTCanvas->cd(CDpos+3) ;
          if(aReadRootFile_Turn_9.Output_pTH1F_YTrackYPadLeading) aReadRootFile_Turn_9.Output_pTH1F_YTrackYPadLeading->Draw();
          pTCanvas->Update();
          
          TText aTText_cd4 ;
          aTText_cd4.SetNDC();
          aTText_cd4.SetTextSize(0.06);
          aTText_cd4.SetTextColor(2);
          aTText_cd4.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

      if (CDpos==3){
        pTCanvas->Print(Str_Out_PDF.c_str());
      }

    }
    if (CDpos!=3){
      pTCanvas->Print(Str_Out_PDF.c_str());
    }


  }
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_RESO_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kFALSE) ;
  gStyle->SetStatW(0.10);    
  gStyle->SetStatH(0.10);    
  
  std::string TAG         = "FINAL"            ; 
  
//Redirect Output
  std::cout << std::endl ;
  std::ostringstream aostringstream_OutPut_Analysis ;
  aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
  aostringstream_OutPut_Analysis << OUTDirName << SampleSet << "_" << "P_Synthesis_RESO_01.txt" ;
  
  std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
  
  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file
 
//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_RESO_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//
  TLegend* pTLegend_Residual_Sigma = new TLegend(0.30,0.15 ,0.85,0.25  ); 
  pTLegend_Residual_Sigma->SetFillStyle(1001);
  pTLegend_Residual_Sigma->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_Residual_Sigma = new TMultiGraph();      

  TLegend* pTLegend_Residual_Mean = new TLegend(0.40,0.75 ,0.68,0.85  ); 
  pTLegend_Residual_Mean->SetFillStyle(1001);
  pTLegend_Residual_Mean->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_Residual_Mean = new TMultiGraph();      

  std::vector < TMultiGraph* > V_pTMultiGraph_Residual_Mean_VS_Col  ;
  std::vector < TLegend*     > V_pTLegend_Residual_Mean_VS_Col      ;
  std::vector < TMultiGraph* > V_pTMultiGraph_Residual_Sigma_VS_Col ;
  std::vector < TLegend*     > V_pTLegend_Residual_Sigma_VS_Col     ;
  
//  
  std::string TheNameVariable = "BIDON" ;  
  int iCase_max = 7 ;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }
    
    
    std::cout << std::endl ;

//
    TGraphErrors* pTGraphErrors_Residual_Mean = new TGraphErrors ;

    if (iCase==0){ pTGraphErrors_Residual_Mean->SetLineColor(2); }
    if (iCase==1){ pTGraphErrors_Residual_Mean->SetLineColor(4); }
    if (iCase==2){ pTGraphErrors_Residual_Mean->SetLineColor(8); }
    if (iCase==3){ pTGraphErrors_Residual_Mean->SetLineColor(2); }
    if (iCase==4){ pTGraphErrors_Residual_Mean->SetLineColor(4); }
    if (iCase==5){ pTGraphErrors_Residual_Mean->SetLineColor(8); }
    if (iCase==6){ pTGraphErrors_Residual_Mean->SetLineColor(2); }
    if (iCase==7){ pTGraphErrors_Residual_Mean->SetLineColor(4); }
    if (iCase==8){ pTGraphErrors_Residual_Mean->SetLineColor(8); }

    if (iCase==0){ pTGraphErrors_Residual_Mean->SetMarkerStyle(24); }
    if (iCase==1){ pTGraphErrors_Residual_Mean->SetMarkerStyle(24); }
    if (iCase==2){ pTGraphErrors_Residual_Mean->SetMarkerStyle(24); }
    if (iCase==3){ pTGraphErrors_Residual_Mean->SetMarkerStyle(25); }
    if (iCase==4){ pTGraphErrors_Residual_Mean->SetMarkerStyle(25); }
    if (iCase==5){ pTGraphErrors_Residual_Mean->SetMarkerStyle(25); }
    if (iCase==6){ pTGraphErrors_Residual_Mean->SetMarkerStyle(26); }
    if (iCase==7){ pTGraphErrors_Residual_Mean->SetMarkerStyle(26); }
    if (iCase==8){ pTGraphErrors_Residual_Mean->SetMarkerStyle(26); }

    if (iCase==0){ pTGraphErrors_Residual_Mean->SetMarkerColor(2); }
    if (iCase==1){ pTGraphErrors_Residual_Mean->SetMarkerColor(4); }
    if (iCase==2){ pTGraphErrors_Residual_Mean->SetMarkerColor(8); }
    if (iCase==3){ pTGraphErrors_Residual_Mean->SetMarkerColor(2); }
    if (iCase==4){ pTGraphErrors_Residual_Mean->SetMarkerColor(4); }
    if (iCase==5){ pTGraphErrors_Residual_Mean->SetMarkerColor(8); }
    if (iCase==6){ pTGraphErrors_Residual_Mean->SetMarkerColor(2); }
    if (iCase==7){ pTGraphErrors_Residual_Mean->SetMarkerColor(4); }
    if (iCase==8){ pTGraphErrors_Residual_Mean->SetMarkerColor(8); }


    pTGraphErrors_Residual_Mean->SetMarkerSize(1.8) ;
    
    TGraphErrors* pTGraphErrors_Residual_Sigma = new TGraphErrors ;

    if (iCase==0){ pTGraphErrors_Residual_Sigma->SetLineColor(2); }
    if (iCase==1){ pTGraphErrors_Residual_Sigma->SetLineColor(4); }
    if (iCase==2){ pTGraphErrors_Residual_Sigma->SetLineColor(8); }
    if (iCase==3){ pTGraphErrors_Residual_Sigma->SetLineColor(2); }
    if (iCase==4){ pTGraphErrors_Residual_Sigma->SetLineColor(4); }
    if (iCase==5){ pTGraphErrors_Residual_Sigma->SetLineColor(8); }
    if (iCase==6){ pTGraphErrors_Residual_Sigma->SetLineColor(2); }
    if (iCase==7){ pTGraphErrors_Residual_Sigma->SetLineColor(4); }
    if (iCase==8){ pTGraphErrors_Residual_Sigma->SetLineColor(8); }

    if (iCase==0){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(24); }
    if (iCase==1){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(24); }
    if (iCase==2){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(24); }
    if (iCase==3){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(25); }
    if (iCase==4){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(25); }
    if (iCase==5){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(25); }
    if (iCase==6){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(26); }
    if (iCase==7){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(26); }
    if (iCase==8){ pTGraphErrors_Residual_Sigma->SetMarkerStyle(26); }

    if (iCase==0){ pTGraphErrors_Residual_Sigma->SetMarkerColor(2); }
    if (iCase==1){ pTGraphErrors_Residual_Sigma->SetMarkerColor(4); }
    if (iCase==2){ pTGraphErrors_Residual_Sigma->SetMarkerColor(8); }
    if (iCase==3){ pTGraphErrors_Residual_Sigma->SetMarkerColor(2); }
    if (iCase==4){ pTGraphErrors_Residual_Sigma->SetMarkerColor(4); }
    if (iCase==5){ pTGraphErrors_Residual_Sigma->SetMarkerColor(8); }
    if (iCase==6){ pTGraphErrors_Residual_Sigma->SetMarkerColor(2); }
    if (iCase==7){ pTGraphErrors_Residual_Sigma->SetMarkerColor(4); }
    if (iCase==8){ pTGraphErrors_Residual_Sigma->SetMarkerColor(8); }

    pTGraphErrors_Residual_Sigma->SetMarkerSize(1.8) ;
    
    TMultiGraph* pTMultiGraph_Residual_Mean_VS_Col = new TMultiGraph();      
 
    TLegend* pTLegend_Residual_Mean_VS_Col = new TLegend(0.50,0.15 ,0.60,0.38  ); 
    pTLegend_Residual_Mean_VS_Col->SetFillStyle(1001);
    pTLegend_Residual_Mean_VS_Col->SetBorderSize(1);

    TMultiGraph* pTMultiGraph_Residual_Sigma_VS_Col = new TMultiGraph();      
 
    TLegend* pTLegend_Residual_Sigma_VS_Col = new TLegend(0.25,0.65 ,0.35,0.88  ); 
    pTLegend_Residual_Sigma_VS_Col->SetFillStyle(1001);
    pTLegend_Residual_Sigma_VS_Col->SetBorderSize(1);

    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
    
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = aAnalysis_Files.m_VariableName + " " +  aAnalysis_Files.m_VariableValueAsString[iFile];
      
      double ZZ = aAnalysis_Files.m_VariableValue[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);
      
      TheNameVariable = aAnalysis_Files.m_VariableName ;
            
      if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){   
        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(2) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(2) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(2) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(2) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(2) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(4) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(4) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(4) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(4) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetLineColor(4) ;

        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(2) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(2) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(2) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(2) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(2) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(4) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(4) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(4) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(4) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerColor(4) ;

        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(22) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(25) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(20) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(21) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(22) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(24) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerStyle(25) ;

        aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->SetMarkerSize(1.8) ;

        pTMultiGraph_Residual_Mean_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col, "pl") ;
  
        pTLegend_Residual_Mean_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col , What_ZZ.c_str() , "pl");
      }            
      
      if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){   
        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(2) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(2) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(2) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(2) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(2) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(4) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(4) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(4) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(4) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetLineColor(4) ;

        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(2) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(4) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(4) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(4) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerColor(4) ;

        if (iFile==0) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
        if (iFile==1) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
        if (iFile==2) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(22) ;
        if (iFile==3) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
        if (iFile==4) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(25) ;
        if (iFile==5) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(20) ;
        if (iFile==6) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(21) ;
        if (iFile==7) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(22) ;
        if (iFile==8) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(24) ;
        if (iFile==9) aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(25) ;

        aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerSize(1.8) ;

        pTMultiGraph_Residual_Sigma_VS_Col->Add( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col, "pl") ;
  
        pTLegend_Residual_Sigma_VS_Col->AddEntry( aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col , What_ZZ.c_str() , "pl");
      }            
                  
      if (aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col){                           
        double Mean = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetMean(2) ;
        double RMS  = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetRMS(2)  ;
        double TheN = aReadRootFile.pTGraphErrors_Residual_Mean_VS_Col->GetN()     ;

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
        
        std::cout 
           << " "  << std::setw(10)  << AnalyseName 
           << " | "  
           << What_ZZ                                           
           << " | "  
           << " MeanMean "   << std::setw(5) << std::setprecision(0) << Mean   
           << " MeanRMS  "   << std::setw(5) << std::setprecision(0) << RMS    
           << " MeanN    "   << std::setw(5) << std::setprecision(0) << TheN    
           << std::endl ;
                   
        double RMSLocal = RMS ;
        RMSLocal =  RMS/std::sqrt( double(TheN) ) ;
        
        int IptCur_Mean  = pTGraphErrors_Residual_Mean ->GetN() ;
        pTGraphErrors_Residual_Mean ->SetPoint     ( IptCur_Mean , ZZ , Mean      ) ;
        pTGraphErrors_Residual_Mean ->SetPointError( IptCur_Mean , 0. , RMSLocal  ) ;
      }           
                  
      if (aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col){                           
        double Mean = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetMean(2) ;
        double RMS  = aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetRMS(2)  ;
                                                  
        std::cout 
           << " "  << std::setw(10)  << AnalyseName 
           << " | "  
           << What_ZZ                                           
           << " | "  
           << " SigMean "   << std::setw(5) << std::setprecision(0) << Mean   
           << " SigRMS  "   << std::setw(5) << std::setprecision(0) << RMS    
           << " SigN    "   << std::setw(5) << std::setprecision(0) << aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()    
           << std::endl ;
                   
        double RMSLocal = RMS ;
        RMSLocal =  RMS/std::sqrt( double(aReadRootFile.pTGraphErrors_Residual_Sigma_VS_Col->GetN()) ) ;
        
        int IptCur_Mean  = pTGraphErrors_Residual_Sigma ->GetN() ;
        pTGraphErrors_Residual_Sigma ->SetPoint     ( IptCur_Mean , ZZ , Mean      ) ;
        pTGraphErrors_Residual_Sigma ->SetPointError( IptCur_Mean , 0. , RMSLocal  ) ;
      }           
    }

    V_pTMultiGraph_Residual_Mean_VS_Col .push_back(pTMultiGraph_Residual_Mean_VS_Col ) ;
    V_pTLegend_Residual_Mean_VS_Col     .push_back(pTLegend_Residual_Mean_VS_Col     ) ;
    V_pTMultiGraph_Residual_Sigma_VS_Col.push_back(pTMultiGraph_Residual_Sigma_VS_Col) ;
    V_pTLegend_Residual_Sigma_VS_Col    .push_back(pTLegend_Residual_Sigma_VS_Col    ) ;
    
      pTMultiGraph_Residual_Mean_VS_Col->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Residual_Mean_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Residual_Mean_VS_Col->GetYaxis()->SetTitle("Mean Residual (#mu m)");
      pTMultiGraph_Residual_Mean_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Residual_Mean_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_Residual_Mean_VS_Col->SetMinimum(-400.);
      pTMultiGraph_Residual_Mean_VS_Col->SetMaximum( 400.);

      pTMultiGraph_Residual_Sigma_VS_Col->SetTitle(AnalyseName.c_str());
      pTMultiGraph_Residual_Sigma_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Residual_Sigma_VS_Col->GetYaxis()->SetTitle("Sigma Residual (#mu m)");
      pTMultiGraph_Residual_Sigma_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Residual_Sigma_VS_Col->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_Residual_Sigma_VS_Col->SetMinimum( 150.);
      pTMultiGraph_Residual_Sigma_VS_Col->SetMaximum( 600.);

   
    if (aAnalysis_Files.m_VariableName=="ZZ"){
      pTMultiGraph_Residual_Mean->Add( pTGraphErrors_Residual_Mean , "p" );
      pTMultiGraph_Residual_Sigma->Add( pTGraphErrors_Residual_Sigma , "p" );
    }else{
      pTMultiGraph_Residual_Mean->Add( pTGraphErrors_Residual_Mean , "pl" );
      pTMultiGraph_Residual_Sigma->Add( pTGraphErrors_Residual_Sigma , "pl" );
    }

    if (aAnalysis_Files.m_VariableName=="ZZ"){
      std::ostringstream aostringstream_pTLegend_Residual_Mean ;
      aostringstream_pTLegend_Residual_Mean << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Residual_Mean << AnalyseName;

      pTLegend_Residual_Mean ->AddEntry( pTGraphErrors_Residual_Mean  , (aostringstream_pTLegend_Residual_Mean.str()).c_str() , "pl");
                                             
      TF1* pTF1 = new TF1("fit1","sqrt([0]*[0] + 237.*237.*((x+137.46)/10.) /[1])",-137.46, 900.);
      pTF1->SetParName(0,"sigma0");
      pTF1->SetParName(1,"Neff");    
      pTF1->SetParameter(0,100.);
      pTF1->SetParameter(1,100.);    
      if (iCase==0) pTF1->SetLineColor(2) ;
      if (iCase==1) pTF1->SetLineColor(4) ;
      if (iCase==2) pTF1->SetLineColor(8) ;
      pTGraphErrors_Residual_Sigma->Fit(pTF1, "R");
      
      std::ostringstream aostringstream_pTLegend_Residual_Sigma ;
      aostringstream_pTLegend_Residual_Sigma << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Residual_Sigma << AnalyseName 
                                             << " ( #sigma_{0} = " << std::setprecision(0) << pTF1->GetParameter(0)
                                             << " +/- "        << std::setprecision(0) << pTF1->GetParError(0)
                                             << "  N_{eff} = " << std::setprecision(0) << pTF1->GetParameter(1)
                                             << " +/- "        << std::setprecision(0) << pTF1->GetParError(1)
                                             << "  #chi^{2}_{min}/dof = " 
                                                               << std::setprecision(0) << pTF1->GetChisquare()
                                             << "/"            << std::setprecision(0) << pTF1->GetNDF()
                                             << " ) " ;
                                             
      pTLegend_Residual_Sigma->AddEntry( pTGraphErrors_Residual_Sigma , (aostringstream_pTLegend_Residual_Sigma.str()).c_str() , "pl");
 
    }else{
      std::ostringstream aostringstream_pTLegend_Residual_Mean ;
      aostringstream_pTLegend_Residual_Mean << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Residual_Mean << AnalyseName;
                                             
      pTLegend_Residual_Mean ->AddEntry( pTGraphErrors_Residual_Mean  , (aostringstream_pTLegend_Residual_Mean.str()).c_str() , "pl");

      std::ostringstream aostringstream_pTLegend_Residual_Sigma ;
      aostringstream_pTLegend_Residual_Sigma << std::setiosflags(std::ios::fixed) ;
      aostringstream_pTLegend_Residual_Sigma << AnalyseName;
                                             
      pTLegend_Residual_Sigma->AddEntry( pTGraphErrors_Residual_Sigma , (aostringstream_pTLegend_Residual_Sigma.str()).c_str() , "pl");

    }
    
  }
  
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      V_pTMultiGraph_Residual_Mean_VS_Col[iCase]->Draw("A");
      pTCanvas->Update();

      V_pTLegend_Residual_Mean_VS_Col[iCase]->Draw();
      pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
   
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
      pTMultiGraph_Residual_Mean->GetXaxis()->SetTitle(TheNameVariable.c_str());
      pTMultiGraph_Residual_Mean->GetYaxis()->SetTitle("Bias (#mu m)");
      pTMultiGraph_Residual_Mean->GetYaxis()->SetTitleOffset(1.5);
      
      pTMultiGraph_Residual_Mean->SetMinimum( 0. );
      
      std::size_t found = SampleSet.find("ScanZZ");
      
      if (found!=std::string::npos) {
         pTMultiGraph_Residual_Mean->GetXaxis()->SetLimits(-200.,900.);
      }
      
      pTMultiGraph_Residual_Mean->Draw("A");
      pTCanvas->Update();
      
      pTLegend_Residual_Mean->Draw();
      pTCanvas->Update();
      
      if (found!=std::string::npos) {
        double Vmin = pTCanvas->GetUymin() ;
        double Vmax = pTCanvas->GetUymax() ;
        TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
  pTCanvas->Print(Str_Out_PDF.c_str());

  
//
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      V_pTMultiGraph_Residual_Sigma_VS_Col[iCase]->Draw("A");
      pTCanvas->Update();

      V_pTLegend_Residual_Sigma_VS_Col[iCase]->Draw();
      pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
   
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
      pTMultiGraph_Residual_Sigma->GetXaxis()->SetTitle(TheNameVariable.c_str());
      pTMultiGraph_Residual_Sigma->GetYaxis()->SetTitle("Resolution (#mu m)");
      pTMultiGraph_Residual_Sigma->GetYaxis()->SetTitleOffset(1.5);
      
      if (found!=std::string::npos) {
        pTMultiGraph_Residual_Sigma->SetMinimum( 150. );
        pTMultiGraph_Residual_Sigma->SetMaximum( 450. );
        pTMultiGraph_Residual_Sigma->GetXaxis()->SetLimits(-200.,900.);
      }
      
      pTMultiGraph_Residual_Sigma->Draw("A");
      pTCanvas->Update();
      
      pTLegend_Residual_Sigma->Draw();
      pTCanvas->Update();
      
      if (found!=std::string::npos) {
        double Vmin = pTCanvas->GetUymin() ;
        double Vmax = pTCanvas->GetUymax() ;
        TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
  pTCanvas->Print(Str_Out_PDF.c_str());

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

}

void P_Synthesis_RCvsZZ_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_RCvsZZ_01_" + ".pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph  = new TMultiGraph();  
  TLegend* pTLegend = new TLegend(0.56,0.20 ,0.75,0.38); 
  pTLegend->SetFillStyle(1001);
  pTLegend->SetBorderSize(1);

  std::string TheNameVariable = "BIDON" ;  
  int iPV_max = 6 ;
  if (iPV_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iPV_max=2 ;
  for (int iPV = 0 ; iPV < iPV_max; iPV ++){

    TMultiGraph* pTMultiGraph_PV  = new TMultiGraph();  
    TLegend* pTLegend_PV = new TLegend(0.56,0.20 ,0.75,0.38); 
    pTLegend_PV->SetFillStyle(1001);
    pTLegend_PV->SetBorderSize(1);

    int iFlavor_max = 1 ;
    for (int iFlavor = 0 ; iFlavor < iFlavor_max; iFlavor ++){
      std::string Flavor = "BIDON" ;
      
      Flavor = "T3_w2" ;
      
      std::string AnalyseName = "BIDON" ;
      if (iPV==0) AnalyseName = "PV2" ;
      if (iPV==1) AnalyseName = "PV21" ;
      if (iPV==2) AnalyseName = "PV3" ;
      if (iPV==3) AnalyseName = "PV31" ;
      if (iPV==4) AnalyseName = "PV4" ;
      if (iPV==5) AnalyseName = "PV41" ;
      
      AnalyseName = AnalyseName + "_" + Flavor ;
      
      TGraphErrors* pTGraphErrors = new TGraphErrors ;

      if (iPV==0) pTGraphErrors->SetLineColor( 2 ) ; 
      if (iPV==1) pTGraphErrors->SetLineColor( 2 ) ;
      if (iPV==2) pTGraphErrors->SetLineColor( 2 ) ; 
      if (iPV==3) pTGraphErrors->SetLineColor( 4 ) ;
      if (iPV==4) pTGraphErrors->SetLineColor( 4 ) ; 
      if (iPV==5) pTGraphErrors->SetLineColor( 4 ) ;
      
      if (iPV==0) pTGraphErrors->SetMarkerStyle( 24 ) ; 
      if (iPV==1) pTGraphErrors->SetMarkerStyle( 25 ) ;
      if (iPV==2) pTGraphErrors->SetMarkerStyle( 26 ) ;
      if (iPV==3) pTGraphErrors->SetMarkerStyle( 24 ) ; 
      if (iPV==4) pTGraphErrors->SetMarkerStyle( 25 ) ;
      if (iPV==5) pTGraphErrors->SetMarkerStyle( 26 ) ;
      
      if (iPV==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
      if (iPV==1) pTGraphErrors->SetMarkerColor( 2 ) ;
      if (iPV==2) pTGraphErrors->SetMarkerColor( 2 ) ;
      if (iPV==3) pTGraphErrors->SetMarkerColor( 4 ) ; 
      if (iPV==4) pTGraphErrors->SetMarkerColor( 4 ) ;
      if (iPV==5) pTGraphErrors->SetMarkerColor( 4 ) ;
      
     pTGraphErrors->SetMarkerSize(1.8) ;
      
      Analysis_Files aAnalysis_Files(SampleSet) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
          
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        
        double ZZ = aAnalysis_Files.m_VariableValue[iFile];
        
        TheNameVariable = aAnalysis_Files.m_VariableName ;
 
        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
        
        TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
        
        double  TheValue =  pTF1_OK->GetParameter(0) ;
        double eTheValue =  pTF1_OK->GetParError (0) ;
        pTGraphErrors->SetPoint(pTGraphErrors->GetN(), ZZ , TheValue  ) ;
        pTGraphErrors->SetPointError(pTGraphErrors->GetN()-1, 0,eTheValue);
        
      }
      
      pTMultiGraph->Add( pTGraphErrors , "pl" );
      pTLegend->AddEntry( pTGraphErrors , AnalyseName.c_str() , "pl");
       
      pTMultiGraph_PV->Add( pTGraphErrors , "pl" );
      pTLegend_PV->AddEntry( pTGraphErrors , AnalyseName.c_str() , "pl");
   
    }

    std::ostringstream aostringstream_Title ;
    aostringstream_Title << std::setiosflags(std::ios::fixed) ;
    aostringstream_Title << "RC VS " << TheNameVariable ;
    
    pTMultiGraph_PV->SetTitle((aostringstream_Title.str()).c_str());
    pTMultiGraph_PV->GetXaxis()->SetTitle(TheNameVariable.c_str());
    pTMultiGraph_PV->GetYaxis()->SetTitle("RC (ns mm^{-2})");
    pTMultiGraph_PV->GetYaxis()->SetTitleOffset(1.5);
    std::size_t found = SampleSet.find("ScanZZ");
    if (found!=std::string::npos) {
      pTMultiGraph_PV->SetMinimum(  50. );
      pTMultiGraph_PV->SetMaximum( 120. );
      pTMultiGraph_PV->GetXaxis()->SetLimits(-200.,900.);
    }

    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    
        pTMultiGraph_PV->Draw("A");
        pTCanvas->Update();

        if (found!=std::string::npos) {
          double Vmin = pTCanvas->GetUymin() ;
          double Vmax = pTCanvas->GetUymax() ;
          TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
          pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
          pTCanvas->Update()    ;
        }
        
        pTLegend_PV->Draw();
        pTCanvas->Update();
    
    pTCanvas->Print(Str_Out_PDF.c_str());
 
  }

//
  std::ostringstream aostringstream_Title ;
  aostringstream_Title << std::setiosflags(std::ios::fixed) ;
  aostringstream_Title << "RC VS " << TheNameVariable ;
  
  pTMultiGraph->SetTitle((aostringstream_Title.str()).c_str());
  pTMultiGraph->GetXaxis()->SetTitle(TheNameVariable.c_str());
  pTMultiGraph->GetYaxis()->SetTitle("RC (ns mm^{-2})");
  pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
  std::size_t found = SampleSet.find("ScanZZ");
  if (found!=std::string::npos) {
    pTMultiGraph->SetMinimum(  50. );
    pTMultiGraph->SetMaximum( 120. );
    pTMultiGraph->GetXaxis()->SetLimits(-200.,900.);
  }
  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
      pTMultiGraph->Draw("A");
      pTCanvas->Update();

      if (found!=std::string::npos) {
        double Vmin = pTCanvas->GetUymin() ;
        double Vmax = pTCanvas->GetUymax() ;
        TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      pTLegend->Draw();
      pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
 
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}

void P_Synthesis_PULL_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.30);    
  gStyle->SetStatH(0.30);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_PULL_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string PageDeGarde_String = AnalyseName ;
      TText aTText_PageDeGarde ;
      aTText_PageDeGarde.SetNDC();
      aTText_PageDeGarde.SetTextSize(0.06);
      aTText_PageDeGarde.SetTextColor(2);
      aTText_PageDeGarde.DrawText(0.,0.5,PageDeGarde_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
  
//  PRF fit - Beg
    if (iCase==0){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

        std::string RCFit_String = AnalyseName + " : PRF fit";
        TText aTText_RCFit ;
        aTText_RCFit.SetNDC();
        aTText_RCFit.SetTextSize(0.06);
        aTText_RCFit.SetTextColor(2);
        aTText_RCFit.DrawText(0.,0.5,RCFit_String.c_str());
        pTCanvas->Update();
        
      pTCanvas->Print(Str_Out_PDF.c_str());
      
      for (int iTurn = 0 ; iTurn< 10; iTurn++){
        std::string IterString = "BIDON" ;
        if (iTurn==0) IterString = "Turn_0" ;
        if (iTurn==1) IterString = "Turn_1" ;
        if (iTurn==2) IterString = "Turn_2" ;
        if (iTurn==3) IterString = "Turn_3" ;
        if (iTurn==4) IterString = "Turn_4" ;
        if (iTurn==5) IterString = "Turn_5" ;
        if (iTurn==6) IterString = "Turn_6" ;
        if (iTurn==7) IterString = "Turn_7" ;
        if (iTurn==8) IterString = "Turn_8" ;
        if (iTurn==9) IterString = "Turn_9" ;

        pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
        pTCanvas->Divide(4,3) ;

          Analysis_Files aAnalysis_Files(SampleSet) ;
          std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
          int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
          for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
            std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

            std::string What_ZZ = 
              AnalyseName 
              + " " + aAnalysis_Files.m_VariableName + " " 
              +  aAnalysis_Files.m_VariableValueAsString[iFile];

            ReadRootFile aReadRootFile ;
        
            aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, IterString);
        
            GaussFunction aGaussFunction ;
            TF1 *pTF1 = new TF1("GaussFunction",aGaussFunction ,-3. ,3.,3);

            pTF1->SetParameter(0,100.);
            pTF1->SetParameter(1,0.  );
            pTF1->SetParameter(2,0.3 );
  
            pTF1->SetParName(0,"Norm");
            pTF1->SetParName(1,"mean");
            pTF1->SetParName(2,"sigma");
  

            pTCanvas->cd(iFile+1) ;
            pTCanvas->cd(iFile+1)->SetLogy(1) ;

              if(aReadRootFile.Output_pTH1F_PullFitPRF) aReadRootFile.Output_pTH1F_PullFitPRF->Fit("GaussFunction","Q","",-3.,3.) ;
              if(aReadRootFile.Output_pTH1F_PullFitPRF) aReadRootFile.Output_pTH1F_PullFitPRF->Draw();

            pTCanvas->Update();
                if (aReadRootFile.Output_pTH1F_PullFitPRF) aReadRootFile.Output_pTH1F_PullFitPRF->UseCurrentStyle() ;
           
            pTCanvas->cd(iFile+1) ;
            TText aTText_cd1 ;
            aTText_cd1.SetNDC();
            aTText_cd1.SetTextSize(0.06);
            aTText_cd1.SetTextColor(2);
            aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
            pTCanvas->Update();

        }
        pTCanvas->Print(Str_Out_PDF.c_str());
      }
    }
//  PRF fit - End
  
//  RC fit - Beg
    if (iCase==1 || iCase==2){
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

        std::string RCFit_String = AnalyseName + " : RC fit";
        TText aTText_RCFit ;
        aTText_RCFit.SetNDC();
        aTText_RCFit.SetTextSize(0.06);
        aTText_RCFit.SetTextColor(2);
        aTText_RCFit.DrawText(0.,0.5,RCFit_String.c_str());
        pTCanvas->Update();
        
      pTCanvas->Print(Str_Out_PDF.c_str());
      
      for (int iTurn = 0 ; iTurn< 10; iTurn++){
        std::string IterString = "BIDON" ;
        if (iTurn==0) IterString = "Turn_0" ;
        if (iTurn==1) IterString = "Turn_1" ;
        if (iTurn==2) IterString = "Turn_2" ;
        if (iTurn==3) IterString = "Turn_3" ;
        if (iTurn==4) IterString = "Turn_4" ;
        if (iTurn==5) IterString = "Turn_5" ;
        if (iTurn==6) IterString = "Turn_6" ;
        if (iTurn==7) IterString = "Turn_7" ;
        if (iTurn==8) IterString = "Turn_8" ;
        if (iTurn==9) IterString = "Turn_9" ;

        pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
        pTCanvas->Divide(4,3) ;

          Analysis_Files aAnalysis_Files(SampleSet) ;
          std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
          int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
          for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
            std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

            std::string What_ZZ = 
              AnalyseName 
              + " " + aAnalysis_Files.m_VariableName + " " 
              +  aAnalysis_Files.m_VariableValueAsString[iFile];

            ReadRootFile aReadRootFile ;
        
            aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, IterString);
        
            GaussFunction aGaussFunction ;
            TF1 *pTF1 = new TF1("GaussFunction",aGaussFunction ,-3. ,3.,3);

            pTF1->SetParameter(0,100.);
            pTF1->SetParameter(1,0.  );
            pTF1->SetParameter(2,1.  );
  
            pTF1->SetParName(0,"Norm");
            pTF1->SetParName(1,"mean");
            pTF1->SetParName(2,"sigma");
  

            pTCanvas->cd(iFile+1) ;
            pTCanvas->cd(iFile+1)->SetLogy(1) ;

              if(aReadRootFile.Output_pTH1F_PullFitRC) aReadRootFile.Output_pTH1F_PullFitRC->Fit("GaussFunction","Q","",-3.,3.) ;
              if(aReadRootFile.Output_pTH1F_PullFitRC) aReadRootFile.Output_pTH1F_PullFitRC->Draw();

            pTCanvas->Update();
                if (aReadRootFile.Output_pTH1F_PullFitRC) aReadRootFile.Output_pTH1F_PullFitRC->UseCurrentStyle() ;
           
            pTCanvas->cd(iFile+1) ;
            TText aTText_cd1 ;
            aTText_cd1.SetNDC();
            aTText_cd1.SetTextSize(0.06);
            aTText_cd1.SetTextColor(2);
            aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
            pTCanvas->Update();

        }
        pTCanvas->Print(Str_Out_PDF.c_str());
      }
    }
//  RC fit - End
  
//  Cluster Position fit - Beg
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string ClusterPositionFit_String = AnalyseName + " : Cluster Position fit";
      TText aTText_ClusterPositionFit ;
      aTText_ClusterPositionFit.SetNDC();
      aTText_ClusterPositionFit.SetTextSize(0.06);
      aTText_ClusterPositionFit.SetTextColor(2);
      aTText_ClusterPositionFit.DrawText(0.,0.5,ClusterPositionFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
  
    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;

      
        Analysis_Files aAnalysis_Files(SampleSet) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
        
        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

          std::string What_ZZ = 
            AnalyseName 
            + " " + aAnalysis_Files.m_VariableName + " " 
            +  aAnalysis_Files.m_VariableValueAsString[iFile];

          ReadRootFile aReadRootFile ;
        
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, IterString);
        
          TH1F* Output_pTH1F_PullFitCluster = aReadRootFile.Output_pTH1F_PullFitCluster ;

          GaussFunction aGaussFunction ;
          TF1 *pTF1 = new TF1("GaussFunction",aGaussFunction ,-3. ,3.,3);

          pTF1->SetParameter(0,100.);
          pTF1->SetParameter(1,0.  );
          pTF1->SetParameter(2,1.  );
  
          pTF1->SetParName(0,"Norm");
          pTF1->SetParName(1,"mean");
          pTF1->SetParName(2,"sigma");
  
          pTCanvas->cd(iFile+1) ;
          pTCanvas->cd(iFile+1)->SetLogy(1) ;

            if (Output_pTH1F_PullFitCluster) Output_pTH1F_PullFitCluster->Fit("GaussFunction","Q","",-3.,3.) ;
            if (Output_pTH1F_PullFitCluster) Output_pTH1F_PullFitCluster->Draw();
            
          pTCanvas->Update();
             if (Output_pTH1F_PullFitCluster) Output_pTH1F_PullFitCluster->UseCurrentStyle() ;
         
          pTCanvas->cd(iFile+1) ;
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

      }
      pTCanvas->Print(Str_Out_PDF.c_str());
      
    }
//  Cluster Position fit - Beg

//  Track Fit - Beg   
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string TrackFit_String = AnalyseName + " : Track fit";
      TText aTText_TrackFit ;
      aTText_TrackFit.SetNDC();
      aTText_TrackFit.SetTextSize(0.06);
      aTText_TrackFit.SetTextColor(2);
      aTText_TrackFit.DrawText(0.,0.5,TrackFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
    pTCanvas->Divide(4,3) ;
    
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      GaussFunction aGaussFunction ;
      TF1 *pTF1 = new TF1("GaussFunction",aGaussFunction ,-5. ,5.,3);

      pTF1->SetParameter(0,100.);
      pTF1->SetParameter(1,0.  );
      pTF1->SetParameter(2,1.  );
  
      pTF1->SetParName(0,"Norm");
      pTF1->SetParName(1,"mean");
      pTF1->SetParName(2,"sigma");

      pTCanvas->cd(iFile+1) ;
      pTCanvas->cd(iFile+1)->SetLogy(1);    

              
        if (aReadRootFile.Final_pTH1F_Pull) aReadRootFile.Final_pTH1F_Pull->Fit("GaussFunction","Q","",-5.,5.) ;
        if (aReadRootFile.Final_pTH1F_Pull) aReadRootFile.Final_pTH1F_Pull->Draw();
     
      pTCanvas->Update();
        if (aReadRootFile.Final_pTH1F_Pull) aReadRootFile.Final_pTH1F_Pull->UseCurrentStyle() ;
      
      
      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
//  Track Fit - End   
    
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_QUALITY_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
//gStyle->SetOptStat(111111) ;
  gStyle->SetOptStat(0) ;
//gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.30);    
  gStyle->SetStatH(0.30);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_QUALITY_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string PageDeGarde_String = AnalyseName ;
      TText aTText_PageDeGarde ;
      aTText_PageDeGarde.SetNDC();
      aTText_PageDeGarde.SetTextSize(0.06);
      aTText_PageDeGarde.SetTextColor(2);
      aTText_PageDeGarde.DrawText(0.,0.5,PageDeGarde_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
  
//  PRF-like fit - Beg
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;

      std::string RCFit_String = AnalyseName + " : PRF-like fit";
      TText aTText_RCFit ;
      aTText_RCFit.SetNDC();
      aTText_RCFit.SetTextSize(0.06);
      aTText_RCFit.SetTextColor(2);
      aTText_RCFit.DrawText(0.,0.5,RCFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
    
    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;

        Analysis_Files aAnalysis_Files(SampleSet) ;
        std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
        int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
        for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
          std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

          std::string What_ZZ = 
            AnalyseName 
            + " " + aAnalysis_Files.m_VariableName + " " 
            +  aAnalysis_Files.m_VariableValueAsString[iFile];

          ReadRootFile aReadRootFile ;
        
          aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile ,AnalyseName, IterString);
        
          TH2D* Input_pTH2D_2D_RhoLike_YTYpad = aReadRootFile.Input_pTH2D_2D_RhoPV2_YTYpad ;
          if (aReadRootFile.Input_pTH2D_rho_VS_YTrackYPad) Input_pTH2D_2D_RhoLike_YTYpad = aReadRootFile.Input_pTH2D_rho_VS_YTrackYPad ;

          TH2F* Input_pTH2F_Alpha_VS_YTYpad = aReadRootFile.Input_pTH2F_Alpha_VS_YTYpad ;

          TF1* pTF1_rhoLike_VS_YTrackYPad = aReadRootFile.pTF1_OK ;
          
          pTCanvas->cd(iFile+1) ;

            if(Input_pTH2D_2D_RhoLike_YTYpad) Input_pTH2D_2D_RhoLike_YTYpad->Draw("colz");
            if(Input_pTH2F_Alpha_VS_YTYpad  ) Input_pTH2F_Alpha_VS_YTYpad  ->Draw("colz");
            
            if(pTF1_rhoLike_VS_YTrackYPad) pTF1_rhoLike_VS_YTrackYPad->Draw("SAME");

          pTCanvas->Update();
            if (Input_pTH2D_2D_RhoLike_YTYpad) Input_pTH2D_2D_RhoLike_YTYpad->UseCurrentStyle() ;
            if (Input_pTH2F_Alpha_VS_YTYpad  ) Input_pTH2F_Alpha_VS_YTYpad  ->UseCurrentStyle() ;
           
          pTCanvas->cd(iFile+1) ;
          TText aTText_cd1 ;
          aTText_cd1.SetNDC();
          aTText_cd1.SetTextSize(0.06);
          aTText_cd1.SetTextColor(2);
          aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
          pTCanvas->Update();

        }
        pTCanvas->Print(Str_Out_PDF.c_str());
    }
//  PRF-like fit - End
  
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_Sp01()
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" ;
  MakeMyDir(OUTDirName) ;

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "P_Synthesis_Sp01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  std::vector <double > v_ZZ ;
  std::vector <double > v_RC ; std::vector <double > v_eRC ;
  std::vector <double > v_DD ; std::vector <double > v_eDD ;

  v_ZZ.push_back( 60.) ; v_RC.push_back(100.520) ; v_eRC.push_back(0.022) ; v_DD.push_back( 458.780) ; v_eDD.push_back(  0.562) ;
  v_ZZ.push_back(160.) ; v_RC.push_back( 99.016) ; v_eRC.push_back(0.019) ; v_DD.push_back( 581.659) ; v_eDD.push_back(  0.526) ;
  v_ZZ.push_back(260.) ; v_RC.push_back( 97.818) ; v_eRC.push_back(0.020) ; v_DD.push_back( 720.834) ; v_eDD.push_back(  0.538) ;
  v_ZZ.push_back(360.) ; v_RC.push_back( 97.161) ; v_eRC.push_back(0.021) ; v_DD.push_back( 868.607) ; v_eDD.push_back(  0.532) ;
  v_ZZ.push_back(460.) ; v_RC.push_back( 96.627) ; v_eRC.push_back(0.022) ; v_DD.push_back(1021.253) ; v_eDD.push_back(0.534) ;
  v_ZZ.push_back(560.) ; v_RC.push_back( 95.081) ; v_eRC.push_back(0.021) ; v_DD.push_back(1105.045) ; v_eDD.push_back(  0.504) ;
  v_ZZ.push_back(660.) ; v_RC.push_back( 93.677) ; v_eRC.push_back(0.021) ; v_DD.push_back(1210.989) ; v_eDD.push_back(  0.559) ;
  v_ZZ.push_back(760.) ; v_RC.push_back( 91.448) ; v_eRC.push_back(0.023) ; v_DD.push_back(1258.712) ; v_eDD.push_back(  0.599) ;
  v_ZZ.push_back(860.) ; v_RC.push_back( 91.924) ; v_eRC.push_back(0.027) ; v_DD.push_back(1433.983) ; v_eDD.push_back(  0.670) ;

//
  TGraphErrors* pTGraphErrors_RC = new TGraphErrors ;
  TGraphErrors* pTGraphErrors_W  = new TGraphErrors ;
  
    pTGraphErrors_RC->SetMarkerStyle(24) ;
    pTGraphErrors_W ->SetMarkerStyle(25) ;
    
    pTGraphErrors_RC->SetLineColor( 2 ) ;
    pTGraphErrors_W ->SetLineColor( 4 ) ;
    
    pTGraphErrors_RC->SetMarkerColor(2) ;
    pTGraphErrors_W ->SetMarkerColor(4) ;  

  TMultiGraph* pTMultiGraph_RC = new TMultiGraph();  
  TMultiGraph* pTMultiGraph_W  = new TMultiGraph();  

  TLegend* pTLegend = new TLegend(0.15,0.20 ,0.85,0.30  ); 
  pTLegend->SetFillStyle(1001);
  pTLegend->SetBorderSize(1);

//
  int iTem_max = v_ZZ.size();
  for (int iTem = 0 ; iTem< iTem_max; iTem++){
    double TheZZ  = v_ZZ[iTem] ;
    
    double TheDD  = v_DD[iTem] /10. ;
    double TheeDD = v_eDD[iTem]/10. ;
    double Thew =  237. * std::sqrt(TheDD ) ;
    double eThew = Thew * (TheeDD/TheDD) * 0.5 ;
    
    pTGraphErrors_RC->SetPoint     (pTGraphErrors_RC->GetN()  , TheZZ , v_RC[iTem]  ) ;
    pTGraphErrors_RC->SetPointError(pTGraphErrors_RC->GetN()-1,     0 , v_eRC[iTem] ) ;
    
    pTGraphErrors_W ->SetPoint     (pTGraphErrors_W ->GetN()  , TheZZ , Thew  ) ;
    pTGraphErrors_W ->SetPointError(pTGraphErrors_W ->GetN()-1,     0 , eThew ) ;
  }

//
  TF1* pTF1a = new TF1("funca","[0]*sqrt( (x+137.46)/10.)",50., 900.);
  pTF1a->SetParName(0,"C");
  pTF1a->SetParameter(0,100.);
  pTF1a->SetLineColor(2) ;
  pTGraphErrors_W->Fit(pTF1a, "R");

//TF1* pTF1c = new TF1("funcc","[0]*sqrt( (x+137.46)/10.) + [1]",50., 900.);
  TF1* pTF1c = new TF1("funcc","sqrt( [0]*[0]* (x+137.46)/10. + [1]*[1])",50., 900.);
  pTF1c->SetParName(0,"C");
  pTF1c->SetParName(0,"w0");
  pTF1c->SetParameter(0,100.);
  pTF1c->SetParameter(1,500.);
  pTF1c->SetLineColor(4) ;
  pTGraphErrors_W->Fit(pTF1c, "R");

  TF1* pTF1b = new TF1("funcb","[0]*sqrt( (x+137.46)/10.)",50, 900.);
  pTF1b->SetParName(0,"C");
  pTF1b->SetParameter(0,237.);
  pTF1b->SetLineColor(1) ;
  pTF1b->SetLineStyle(2) ;

//
  pTMultiGraph_RC->Add( pTGraphErrors_RC , "p" );
  pTMultiGraph_W ->Add( pTGraphErrors_W  , "p" );
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      
          pTMultiGraph_RC->GetXaxis()->SetTitle("ZZ (mm)");
          pTMultiGraph_RC->GetYaxis()->SetTitle("RC (ns mm^{-2})");
          pTMultiGraph_RC->GetYaxis()->SetTitleOffset(1.5);
          pTMultiGraph_RC->SetMinimum(  50. );
          pTMultiGraph_RC->SetMaximum( 105. );
       
          pTMultiGraph_RC->Draw("A");
          pTCanvas->Update();
          
      pTCanvas->Print(Str_Out_PDF.c_str());
      
//
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      
          pTMultiGraph_W ->GetXaxis()->SetTitle("ZZ (mm)");
          pTMultiGraph_W ->GetYaxis()->SetTitle("w (#mu m) ");
          pTMultiGraph_W ->GetYaxis()->SetTitleOffset(1.5);
          pTMultiGraph_W->SetMinimum(    0. );
          pTMultiGraph_W->SetMaximum( 3000. );
           
          pTMultiGraph_W ->Draw("A");
          pTCanvas->Update();

//           Vmin = pTCanvas->GetUymin() ;
//           Vmax = pTCanvas->GetUymax() ;
//           TLine* pTLineV2 = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
//           pTLineV2->SetLineStyle(2); pTLineV2->SetLineWidth(1); pTLineV2->SetLineColor(1) ; pTLineV2->Draw();  
//           pTCanvas->Update()    ;
         
          std::ostringstream aostringstream_b ;
          aostringstream_b << std::setiosflags(std::ios::fixed) ;
          aostringstream_b  
                           << " w = C sqrt(DD) with C = 237 " 
                           << " #mu m cm^{-1/2}" 
                           ;
                                                 
          pTLegend->AddEntry( pTF1b , (aostringstream_b.str()).c_str() , "l");
          pTF1b->Draw("SAME");

          std::ostringstream aostringstream_a ;
          aostringstream_a << std::setiosflags(std::ios::fixed) ;
          aostringstream_a  
                           << " w = C sqrt(DD) with C = " << std::setprecision(2) << pTF1a->GetParameter(0)
                           << " +/- " << std::setprecision(2) << pTF1a->GetParError(0)
                           << " #mu m cm^{-1/2}" 
                           ;
                                                 
          pTLegend->AddEntry( pTF1a , (aostringstream_a.str()).c_str() , "l");
          pTF1a->Draw("SAME");

          std::ostringstream aostringstream_c ;
          aostringstream_c << std::setiosflags(std::ios::fixed) ;
          aostringstream_c  
                           << " w = C sqrt(DD) + w0 with "
                           << " C = " << std::setprecision(1) << pTF1c->GetParameter(0)
                           << " +/- " << std::setprecision(1) << pTF1c->GetParError(0)
                           << " #mu m cm^{-1/2}" 
                           << " w0 = " << std::setprecision(0) << pTF1c->GetParameter(1)
                           << " +/- " << std::setprecision(0) << pTF1c->GetParError(1)
                           << " #mu m"
                           ;
                                                 
          pTLegend->AddEntry( pTF1c , (aostringstream_c.str()).c_str() , "l");
          pTF1c->Draw("SAME");

          pTCanvas->Update();
          
          pTLegend->Draw();
          pTCanvas->Update();

      pTCanvas->Print(Str_Out_PDF.c_str());
      
//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
} 

void P_Synthesis_CHI2_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.30);    
  gStyle->SetStatH(0.30);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_CHI2_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string PageDeGarde_String = AnalyseName ;
      TText aTText_PageDeGarde ;
      aTText_PageDeGarde.SetNDC();
      aTText_PageDeGarde.SetTextSize(0.06);
      aTText_PageDeGarde.SetTextColor(2);
      aTText_PageDeGarde.DrawText(0.,0.5,PageDeGarde_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());
  
  

//  PRF Fit - Beg   
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string PRFLike_String = AnalyseName + " : PRF Like fit Ch2min";
      TText aTText_PRFLike_String ;
      aTText_PRFLike_String.SetNDC();
      aTText_PRFLike_String.SetTextSize(0.06);
      aTText_PRFLike_String.SetTextColor(2);
      aTText_PRFLike_String.DrawText(0.,0.5,PRFLike_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//
    TMultiGraph* pTMultiGraph_CHI2            = new TMultiGraph();  
    TMultiGraph* pTMultiGraph_CHI2perDOF      = new TMultiGraph(); 
     
    TMultiGraph* pTMultiGraph_CHI2perDOF_Last = new TMultiGraph();  
    TGraph* pTGraph_CHI2perDOF_Last = new TGraph ;
    pTMultiGraph_CHI2perDOF_Last->Add( pTGraph_CHI2perDOF_Last , "pl" );
    
    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      double ZZ = aAnalysis_Files.m_VariableValue[iFile];

      TGraph* pTGraph_CHI2       = new TGraph ;
      TGraph* pTGraph_CHI2perDOF = new TGraph ;
      
      pTMultiGraph_CHI2      ->Add( pTGraph_CHI2       , "pl" );
      pTMultiGraph_CHI2perDOF->Add( pTGraph_CHI2perDOF , "pl" );
      
      for (int iTurn = 0 ; iTurn< 10; iTurn++){
        std::string IterString = "BIDON" ;
        if (iTurn==0) IterString = "Turn_0" ;
        if (iTurn==1) IterString = "Turn_1" ;
        if (iTurn==2) IterString = "Turn_2" ;
        if (iTurn==3) IterString = "Turn_3" ;
        if (iTurn==4) IterString = "Turn_4" ;
        if (iTurn==5) IterString = "Turn_5" ;
        if (iTurn==6) IterString = "Turn_6" ;
        if (iTurn==7) IterString = "Turn_7" ;
        if (iTurn==8) IterString = "Turn_8" ;
        if (iTurn==9) IterString = "Turn_9" ;

        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, IterString);
        
        if (!isinf(aReadRootFile.Chi2Min_PRFLikeFit)) pTGraph_CHI2      ->SetPoint( pTGraph_CHI2      ->GetN(), double(iTurn) , aReadRootFile.Chi2Min_PRFLikeFit  ) ;
        if (!isinf(aReadRootFile.Chi2Min_PRFLikeFit)) pTGraph_CHI2perDOF->SetPoint( pTGraph_CHI2perDOF->GetN(), double(iTurn) , aReadRootFile.Chi2Min_PRFLikeFit/double(aReadRootFile.NODF_PRFLikeFit)  ) ;

        if (iTurn==9){
          if (!isinf(aReadRootFile.Chi2Min_PRFLikeFit)) pTGraph_CHI2perDOF_Last->SetPoint( pTGraph_CHI2perDOF_Last->GetN(), ZZ , aReadRootFile.Chi2Min_PRFLikeFit/double(aReadRootFile.NODF_PRFLikeFit)  ) ;
        }
                
      }
    }

//    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
     
      pTCanvas->cd(1) ;
        pTMultiGraph_CHI2->SetTitle(AnalyseName.c_str());
        pTMultiGraph_CHI2->GetXaxis()->SetTitle("Iterations");
        pTMultiGraph_CHI2->GetYaxis()->SetTitle("Chi2 Min");
        pTMultiGraph_CHI2->GetYaxis()->SetTitleOffset(1.5);
        pTMultiGraph_CHI2->Draw("A");
      pTCanvas->Update();
     
      pTCanvas->cd(2) ;
        pTMultiGraph_CHI2perDOF->SetTitle(AnalyseName.c_str());
        pTMultiGraph_CHI2perDOF->GetXaxis()->SetTitle("Iterations");
        pTMultiGraph_CHI2perDOF->GetYaxis()->SetTitle("Chi2 Min per dof");
        pTMultiGraph_CHI2perDOF->GetYaxis()->SetTitleOffset(1.5);
        pTMultiGraph_CHI2perDOF->Draw("A");
      pTCanvas->Update();
      
      pTCanvas->cd(3) ;
        pTMultiGraph_CHI2perDOF_Last->SetTitle(AnalyseName.c_str());
        pTMultiGraph_CHI2perDOF_Last->GetXaxis()->SetTitle("ZZ");
        pTMultiGraph_CHI2perDOF_Last->GetYaxis()->SetTitle("Chi2 Min per dof");
        pTMultiGraph_CHI2perDOF_Last->GetYaxis()->SetTitleOffset(1.5);
        pTMultiGraph_CHI2perDOF_Last->Draw("A");
      pTCanvas->Update();
     
    pTCanvas->Print(Str_Out_PDF.c_str());
//  PRF Fit - End  

//  Cluster Fit - Beg  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string ClusFit_String = AnalyseName + " : Cluster fit Ch2min";
      TText aTText_ClusFit_String ;
      aTText_ClusFit_String.SetNDC();
      aTText_ClusFit_String.SetTextSize(0.06);
      aTText_ClusFit_String.SetTextColor(2);
      aTText_ClusFit_String.DrawText(0.,0.5,ClusFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//
    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;
      
      Analysis_Files aAnalysis_Files(SampleSet) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

        std::string What_ZZ = 
          AnalyseName 
          + " " + aAnalysis_Files.m_VariableName + " " 
          +  aAnalysis_Files.m_VariableValueAsString[iFile];

        ReadRootFile aReadRootFile ;
      
        aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, IterString);
      
        pTCanvas->cd(iFile+1) ;
        pTCanvas->cd(iFile+1)->SetLogy(1) ;

          if(aReadRootFile.Output_pTH1F_Ch2MinFitCluster) aReadRootFile.Output_pTH1F_Ch2MinFitCluster->Draw();

        pTCanvas->Update();
          
          if (aReadRootFile.Output_pTH1F_Ch2MinFitCluster) aReadRootFile.Output_pTH1F_Ch2MinFitCluster->UseCurrentStyle() ;
        
        pTCanvas->cd(iFile+1) ;
        TText aTText_cd1 ;
        aTText_cd1.SetNDC();
        aTText_cd1.SetTextSize(0.06);
        aTText_cd1.SetTextColor(2);
        aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
        pTCanvas->Update();
      }
        pTCanvas->Print(Str_Out_PDF.c_str());
              
    }
//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string ClusPERdofFit_String = AnalyseName + " : Cluster fit Ch2min per DOF";
      TText aTText_ClusPERdofFit_String ;
      aTText_ClusPERdofFit_String.SetNDC();
      aTText_ClusPERdofFit_String.SetTextSize(0.06);
      aTText_ClusPERdofFit_String.SetTextColor(2);
      aTText_ClusPERdofFit_String.DrawText(0.,0.5,ClusPERdofFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//
    for (int iTurn = 0 ; iTurn< 10; iTurn++){
      std::string IterString = "BIDON" ;
      if (iTurn==0) IterString = "Turn_0" ;
      if (iTurn==1) IterString = "Turn_1" ;
      if (iTurn==2) IterString = "Turn_2" ;
      if (iTurn==3) IterString = "Turn_3" ;
      if (iTurn==4) IterString = "Turn_4" ;
      if (iTurn==5) IterString = "Turn_5" ;
      if (iTurn==6) IterString = "Turn_6" ;
      if (iTurn==7) IterString = "Turn_7" ;
      if (iTurn==8) IterString = "Turn_8" ;
      if (iTurn==9) IterString = "Turn_9" ;

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1800) ;
      pTCanvas->Divide(4,3) ;
      
      Analysis_Files aAnalysis_Files(SampleSet) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
     
      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

        std::string What_ZZ = 
          AnalyseName 
          + " " + aAnalysis_Files.m_VariableName + " " 
          +  aAnalysis_Files.m_VariableValueAsString[iFile];

        ReadRootFile aReadRootFile ;
      
        aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, IterString);
      
        pTCanvas->cd(iFile+1) ;
        pTCanvas->cd(iFile+1)->SetLogy(1) ;

          if(aReadRootFile.Output_pTH1F_Ch2MinPerDOFFitCluster) aReadRootFile.Output_pTH1F_Ch2MinPerDOFFitCluster->Draw();

        pTCanvas->Update();
          
          if (aReadRootFile.Output_pTH1F_Ch2MinPerDOFFitCluster) aReadRootFile.Output_pTH1F_Ch2MinPerDOFFitCluster->UseCurrentStyle() ;
        
        pTCanvas->cd(iFile+1) ;
        TText aTText_cd1 ;
        aTText_cd1.SetNDC();
        aTText_cd1.SetTextSize(0.06);
        aTText_cd1.SetTextColor(2);
        aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
        pTCanvas->Update();
      }
      pTCanvas->Print(Str_Out_PDF.c_str());
              
    }
//  Cluster Fit - End  

//  Track Fit - Beg   
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      std::string TrackFit_String = AnalyseName + " : Track fit Ch2min";
      TText aTText_TrackFit ;
      aTText_TrackFit.SetNDC();
      aTText_TrackFit.SetTextSize(0.06);
      aTText_TrackFit.SetTextColor(2);
      aTText_TrackFit.DrawText(0.,0.5,TrackFit_String.c_str());
      pTCanvas->Update();
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//   
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){

      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];


      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(3,2) ;
      
      pTCanvas->cd(1) ;
        if (aReadRootFile.Final_pTH1F_Ch2Min_200) aReadRootFile.Final_pTH1F_Ch2Min_200  ->Draw() ;
      pTCanvas->Update();
        
      pTCanvas->cd(2) ;
        if (aReadRootFile.Final_pTH1F_Ch2Min_400) aReadRootFile.Final_pTH1F_Ch2Min_400 ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(3) ;
        if (aReadRootFile.Final_pTH1F_Ch2Min_800) aReadRootFile.Final_pTH1F_Ch2Min_800->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(4) ;
        if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_5) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_5->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(5) ;
        if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_10) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_10->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(6) ;
        if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_20) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_20->Draw() ;
      pTCanvas->Update();

      if (aReadRootFile.Final_pTH1F_Ch2Min_200 ) aReadRootFile.Final_pTH1F_Ch2Min_200 ->UseCurrentStyle() ;
      if (aReadRootFile.Final_pTH1F_Ch2Min_400 ) aReadRootFile.Final_pTH1F_Ch2Min_400 ->UseCurrentStyle() ;
      if (aReadRootFile.Final_pTH1F_Ch2Min_800 ) aReadRootFile.Final_pTH1F_Ch2Min_800 ->UseCurrentStyle() ;
      if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_5 ) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_5 ->UseCurrentStyle() ;
      if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_10) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_10->UseCurrentStyle() ;
      if (aReadRootFile.Final_pTH1F_Ch2MinPerNODF_20) aReadRootFile.Final_pTH1F_Ch2MinPerNODF_20->UseCurrentStyle() ;
      
      pTCanvas->cd(1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      pTCanvas->Print(Str_Out_PDF.c_str());
      
    }
    
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_BEAMPROFILE_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(1111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.30);    
  gStyle->SetStatH(0.10);    

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_BEAMPROFILE_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//  
  Model_ReadOutGeometry* pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  double LY       = pModel_ReadOutGeometry->Get_LY      () ;
  double Ypad_min = pModel_ReadOutGeometry->Get_Ypad_min() ;
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ;

  double YBeam0 = Ypad_min + 0.5 * LY + double (Ny/2) * LY ;
  
  
  TRandom3 aTRandom3 ;
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide (4,4) ;

  double YBeamSigma_min = 1.E-3 ;
  double YBeamSigma_max = 1.E-2 ;
  int iYBeamSigma_Max = 10 ;
  for (int iYBeamSigma = 0 ; iYBeamSigma< iYBeamSigma_Max; iYBeamSigma++){

    double YBeamSigma = YBeamSigma_min + (YBeamSigma_max-YBeamSigma_min)*double(iYBeamSigma)/double(iYBeamSigma_Max-1) ;

    double YBeamShift_min = 0.   ;
    double YBeamShift_max = 0.99 ;
    int iYBeamShift_Max = 16 ;
    for (int iYBeamShift = 0 ; iYBeamShift< iYBeamShift_Max; iYBeamShift++){
      double YBeamShift = YBeamShift_min + (YBeamShift_max-YBeamShift_min)*double(iYBeamShift)/double(iYBeamShift_Max-1) ;
      
      double YBeam = YBeam0 + YBeamShift * LY/2. ;
      
      std::ostringstream aostringstreamEssaiAuVol0 ;
      aostringstreamEssaiAuVol0 << std::setiosflags(std::ios::fixed) ;
      aostringstreamEssaiAuVol0 << "EssaiAuVol" << iYBeamSigma ;
      aostringstreamEssaiAuVol0 << "_"          << iYBeamShift ;
      std::string Name = aostringstreamEssaiAuVol0.str() ;  ;

      std::string Title = "#Delta_{T} (cm)" ;

      double Ymax = LY/2. ;
      double Ymin =-LY/2. ;
      double DiffY = Ymax - Ymin ;
      Ymax = Ymax + 0.1*DiffY ;
      Ymin = Ymin - 0.1*DiffY ;

      TH1F* pTH1F = new TH1F(Name.c_str() ,Title.c_str() ,50,  Ymin*1.E2 ,Ymax*1.E2 );

      int iT_Max = 10000 ;
      for (int iT = 0 ; iT< iT_Max; iT++){
        double YTtrack   = aTRandom3.Gaus(0.,YBeamSigma) + YBeam ;
        int iY0 = (YTtrack-Ypad_min)/LY ;
        double Y0 = pModel_ReadOutGeometry->Get_YcPad(0,iY0,0) ; //WARNING: ModuleNber Set to 0
        double TheDiff = (YTtrack - Y0)*1.E2 ;
        
        pTH1F->Fill( TheDiff ) ;
      }
      
      pTCanvas->cd(iYBeamShift+1) ;
        pTH1F->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iYBeamShift+1) ;
        
        
//       TText aTText_cd1 ;
//       aTText_cd1.SetNDC();
//       aTText_cd1.SetTextSize(0.06);
//       aTText_cd1.SetTextColor(2);
//       aTText_cd1.DrawText(0.3,0.2,StringaostringstreamEssaiAuVol.c_str());

        std::ostringstream aostringstreamEssaiAuVol1;
        aostringstreamEssaiAuVol1<< std::setiosflags(std::ios::fixed) ;
        aostringstreamEssaiAuVol1<< "YBeamSigma = " 
        << std::setw(6) << std::setprecision(3) << YBeamSigma*1.E2 <<" cm " ;
        std::string StringaostringstreamEssaiAuVol1= aostringstreamEssaiAuVol1.str() ; 
        
        std::ostringstream aostringstreamEssaiAuVol2;
        aostringstreamEssaiAuVol2<< std::setiosflags(std::ios::fixed) ;
        aostringstreamEssaiAuVol2<< "YBeamShift = " 
        << std::setw(6) << std::setprecision(3) << YBeamShift ;
        std::string StringaostringstreamEssaiAuVol2= aostringstreamEssaiAuVol2.str() ; 
        
       TPaveText* pTPaveText = new TPaveText(.2,.2,.7,.4,"NDC");
       pTPaveText->AddText( StringaostringstreamEssaiAuVol1.c_str() ) ;
       pTPaveText->AddText( StringaostringstreamEssaiAuVol2.c_str() ) ;
       pTPaveText->Draw();

      pTCanvas->Update();
    }
 
    pTCanvas->Print(Str_Out_PDF.c_str());
  }

//  
  int iCase_max = 7;
  if (iCase_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iCase_max=2 ;
  for (int iCase = 0 ; iCase < iCase_max; iCase ++){
    std::string AnalyseName = "BIDON" ;
    if (iCase==0){ AnalyseName = "PV0_T3_Std" ; }
    if (iCase==1){ AnalyseName = "PV2_T3_w2"  ; }
    if (iCase==2){ AnalyseName = "PV21_T3_w2" ; }
    if (iCase==3){ AnalyseName = "PV3_T3_w2"  ; }
    if (iCase==4){ AnalyseName = "PV31_T3_w2" ; }
    if (iCase==5){ AnalyseName = "PV4_T3_w2"  ; }
    if (iCase==6){ AnalyseName = "PV41_T3_w2" ; }

    Analysis_Files aAnalysis_Files(SampleSet) ;
    std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
   
//  Track Fit - Beg   
//  Track YT at col. 1
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YFitCol[1]) aReadRootFile.V_pTH1F_YFitCol[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Track YT at col. 1
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;

    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YFitColLARGE[1]) aReadRootFile.V_pTH1F_YFitColLARGE[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  DeltaTrack Distribution at col. 1
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_TrackDeltaT[1]) aReadRootFile.V_pTH1F_TrackDeltaT[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Cluster YT at col. 1  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YTrackInTracks[1]) aReadRootFile.V_pTH1F_YTrackInTracks[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Cluster YT at col. 1  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YTrackInTracksLARGE[1]) aReadRootFile.V_pTH1F_YTrackInTracksLARGE[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Cluster YT-YPad at col. 1 
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YTrackYPadInTracks[1]) aReadRootFile.V_pTH1F_YTrackYPadInTracks[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.V_pTH1F_YTrackYPadLeadingInTracks[1]) aReadRootFile.V_pTH1F_YTrackYPadLeadingInTracks[1]  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Track YT all col.
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_pTH1F_YFitCol_AllCol) aReadRootFile.Final_pTH1F_YFitCol_AllCol  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Track YT all col.
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;

    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_pTH1F_YFitCol_AllColLARGE) aReadRootFile.Final_pTH1F_YFitCol_AllColLARGE  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  DeltaTrack Distribution all col.
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;
      
      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_pTH1F_TrackDeltaTAllCol) aReadRootFile.Final_pTH1F_TrackDeltaTAllCol  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
    
//  Cluster YT all col.  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_pTH1F_YTrackInTracks_AllCol) aReadRootFile.Final_pTH1F_YTrackInTracks_AllCol  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Cluster YT all col.  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_pTH1F_YTrackInTracks_AllColLARGE) aReadRootFile.Final_pTH1F_YTrackInTracks_AllColLARGE  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
//  Cluster YT-YPad all col. 
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_TH1F_YTrackYPadInTracksAllCol) aReadRootFile.Final_TH1F_YTrackYPadInTracksAllCol  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
    pTCanvas->Divide(4,3) ;
    
    iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
            
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;

      std::string What_ZZ = 
        AnalyseName 
        + " " + aAnalysis_Files.m_VariableName + " " 
        +  aAnalysis_Files.m_VariableValueAsString[iFile];

      ReadRootFile aReadRootFile ;

      aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

      pTCanvas->cd(iFile+1) ;
        if (aReadRootFile.Final_TH1F_YTrackYPadLeadingInTracksAllCol) aReadRootFile.Final_TH1F_YTrackYPadLeadingInTracksAllCol  ->Draw() ;
      pTCanvas->Update();

      pTCanvas->cd(iFile+1) ;
      TText aTText_cd1 ;
      aTText_cd1.SetNDC();
      aTText_cd1.SetTextSize(0.06);
      aTText_cd1.SetTextColor(2);
      aTText_cd1.DrawText(0.3,0.2,What_ZZ.c_str());
      pTCanvas->Update();
      
      
    }
    pTCanvas->Print(Str_Out_PDF.c_str());
    
  }

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}

void P_Synthesis_DDvsZZ_01(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet + "/" ;
  MakeMyDir(OUTDirName) ;

//
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + SampleSet +"_" + "P_Synthesis_DDvsZZ_01_" + ".pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//
//
  TF1* pTF1b = new TF1("funcb","[0]*sqrt( (x+137.46)/10.)",50, 900.);
  pTF1b->SetParName(0,"C");
  pTF1b->SetParameter(0,237.);
  pTF1b->SetLineColor(1) ;
  pTF1b->SetLineStyle(2) ;

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph  = new TMultiGraph();  
  TLegend* pTLegend = new TLegend(0.56,0.20 ,0.75,0.38); 
  pTLegend->SetFillStyle(1001);
  pTLegend->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_W  = new TMultiGraph();  
  
  std::string TheNameVariable = "BIDON" ;  
  int iPV_max = 2 ;
  if (iPV_max>2 
     && SampleSet.find("CERN_4") == std::string::npos 
     && SampleSet.find("CERN_5") == std::string::npos) iPV_max=2 ;
  for (int iPV = 0 ; iPV < iPV_max; iPV ++){

    TMultiGraph* pTMultiGraph_PV  = new TMultiGraph();  
    TLegend* pTLegend_PV = new TLegend(0.56,0.20 ,0.75,0.38); 
    pTLegend_PV->SetFillStyle(1001);
    pTLegend_PV->SetBorderSize(1);

    TMultiGraph* pTMultiGraph_PV_W  = new TMultiGraph();  

    int iFlavor_max = 1 ;
    for (int iFlavor = 0 ; iFlavor < iFlavor_max; iFlavor ++){
      std::string Flavor = "BIDON" ;
      
      Flavor = "T3_w2" ;
      
      std::string AnalyseName = "BIDON" ;
      if (iPV==0) AnalyseName = "PV4" ;
      if (iPV==1) AnalyseName = "PV41" ;
      
      AnalyseName = AnalyseName + "_" + Flavor ;
      
      TGraphErrors* pTGraphErrors = new TGraphErrors ;

      if (iPV==0) pTGraphErrors->SetLineColor( 2 ) ; 
      if (iPV==1) pTGraphErrors->SetLineColor( 4 ) ;
      
      if (iPV==0) pTGraphErrors->SetMarkerStyle( 24 ) ; 
      if (iPV==1) pTGraphErrors->SetMarkerStyle( 25 ) ;
      
      if (iPV==0) pTGraphErrors->SetMarkerColor( 2 ) ; 
      if (iPV==1) pTGraphErrors->SetMarkerColor( 4 ) ;
      
      pTGraphErrors->SetMarkerSize(1.8) ;
 
      TGraphErrors* pTGraphErrors_W  = new TGraphErrors ;
      
      if (iPV==0) pTGraphErrors_W->SetLineColor( 2 ) ; 
      if (iPV==1) pTGraphErrors_W->SetLineColor( 4 ) ;
      
      if (iPV==0) pTGraphErrors_W->SetMarkerStyle( 24 ) ; 
      if (iPV==1) pTGraphErrors_W->SetMarkerStyle( 25 ) ;
      
      if (iPV==0) pTGraphErrors_W->SetMarkerColor( 2 ) ; 
      if (iPV==1) pTGraphErrors_W->SetMarkerColor( 4 ) ;
      
      Analysis_Files aAnalysis_Files(SampleSet) ;
      std::string TheFilesDir = aAnalysis_Files.m_FilesDir ;
    
      int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
          
        std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
        
        double ZZ = aAnalysis_Files.m_VariableValue[iFile];
        
        TheNameVariable = aAnalysis_Files.m_VariableName ;
 
        ReadRootFile aReadRootFile ;
        
        aReadRootFile.Load_Itereration(TheFilesDir,base_EventFile, AnalyseName, "Turn_9");
        
        TF1* pTF1_OK = aReadRootFile.pTF1_OK ;
        
        double  TheValue =  pTF1_OK->GetParameter(1) ;
        double eTheValue =  pTF1_OK->GetParError (1) ;
        pTGraphErrors->SetPoint(pTGraphErrors->GetN(), ZZ , TheValue  ) ;
        pTGraphErrors->SetPointError(pTGraphErrors->GetN()-1, 0,eTheValue);

        double TheDD  = TheValue /10. ;
        double TheeDD = eTheValue/10. ;
        double Thew =  237. * std::sqrt(TheDD ) ;
        double eThew = Thew * (TheeDD/TheDD) * 0.5 ;
        pTGraphErrors_W ->SetPoint     (pTGraphErrors_W ->GetN()  , ZZ , Thew  ) ;
        pTGraphErrors_W ->SetPointError(pTGraphErrors_W ->GetN()-1,  0 , eThew ) ;
        
      }
      
      pTMultiGraph->Add( pTGraphErrors , "pl" );
      pTLegend->AddEntry( pTGraphErrors , AnalyseName.c_str() , "pl");
       
      pTMultiGraph_PV->Add( pTGraphErrors , "pl" );
      pTLegend_PV->AddEntry( pTGraphErrors , AnalyseName.c_str() , "pl");
   
      pTMultiGraph_W->Add( pTGraphErrors_W , "pl" );
       
      pTMultiGraph_PV_W->Add( pTGraphErrors_W , "pl" );
   
    }

    std::ostringstream aostringstream_Title ;
    aostringstream_Title << std::setiosflags(std::ios::fixed) ;
    aostringstream_Title << "DD VS " << TheNameVariable ;
    
    pTMultiGraph_PV->SetTitle((aostringstream_Title.str()).c_str());
    pTMultiGraph_PV->GetXaxis()->SetTitle(TheNameVariable.c_str());
    pTMultiGraph_PV->GetYaxis()->SetTitle("DD (mm)");
    pTMultiGraph_PV->GetYaxis()->SetTitleOffset(1.5);
    std::size_t found = SampleSet.find("ScanZZ");
    if (found!=std::string::npos) {
      pTMultiGraph_PV->SetMinimum(    0. );
      pTMultiGraph_PV->SetMaximum( 1200. );
      pTMultiGraph_PV->GetXaxis()->SetLimits(-200.,900.);
    }

    std::ostringstream aostringstream_Title2 ;
    aostringstream_Title2 << std::setiosflags(std::ios::fixed) ;
    aostringstream_Title2 << "w VS " << TheNameVariable ;
    
    pTMultiGraph_PV_W->SetTitle((aostringstream_Title2.str()).c_str());
    pTMultiGraph_PV_W->GetXaxis()->SetTitle(TheNameVariable.c_str());
    pTMultiGraph_PV_W->GetYaxis()->SetTitle("w (#mu m)");
    pTMultiGraph_PV_W->GetYaxis()->SetTitleOffset(1.5);
    if (found!=std::string::npos) {
      pTMultiGraph_PV_W->SetMinimum(    0. );
      pTMultiGraph_PV_W->SetMaximum( 3000. );
      pTMultiGraph_PV_W->GetXaxis()->SetLimits(-200.,900.);
    }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    
        pTMultiGraph_PV->Draw("A");
        pTCanvas->Update();

        if (found!=std::string::npos) {
          double Vmin = pTCanvas->GetUymin() ;
          double Vmax = pTCanvas->GetUymax() ;
          TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
          pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
          pTCanvas->Update()    ;

          Vmin = pTCanvas->GetUxmin() ;
          Vmax = pTCanvas->GetUxmax() ;
          TLine* pTLineO = new TLine( Vmin , Vmin + 137.46 , Vmax , Vmax + 137.46 ) ;
          pTLineO->SetLineStyle(2); pTLineO->SetLineWidth(1); pTLineO->SetLineColor(1) ; pTLineO->Draw();  
          pTCanvas->Update()    ;
        }
        
        pTLegend_PV->Draw();
        pTCanvas->Update();
    
    pTCanvas->Print(Str_Out_PDF.c_str());

//  
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    
        pTMultiGraph_PV_W->Draw("A");
        pTCanvas->Update();

        if (found!=std::string::npos) {
          double Vmin = pTCanvas->GetUymin() ;
          double Vmax = pTCanvas->GetUymax() ;
          TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
          pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
          pTCanvas->Update()    ;
        }
        
        pTLegend_PV->Draw();
        pTCanvas->Update();

        pTF1b->Draw("SAME");
        pTCanvas->Update();
    
    pTCanvas->Print(Str_Out_PDF.c_str());

  }

//
  std::ostringstream aostringstream_Title ;
  aostringstream_Title << std::setiosflags(std::ios::fixed) ;
  aostringstream_Title << "DD VS " << TheNameVariable ;
  
  pTMultiGraph->SetTitle((aostringstream_Title.str()).c_str());
  pTMultiGraph->GetXaxis()->SetTitle(TheNameVariable.c_str());
  pTMultiGraph->GetYaxis()->SetTitle("DD (mm)");
  pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);
  std::size_t found = SampleSet.find("ScanZZ");
  if (found!=std::string::npos) {
    pTMultiGraph->SetMinimum(    0. );
    pTMultiGraph->SetMaximum( 1200. );
    pTMultiGraph->GetXaxis()->SetLimits(-200.,900.);
  }
  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
      pTMultiGraph->Draw("A");
      pTCanvas->Update();

      if (found!=std::string::npos) {
        double Vmin = pTCanvas->GetUymin() ;
        double Vmax = pTCanvas->GetUymax() ;
        TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
        pTCanvas->Update()    ;

        Vmin = pTCanvas->GetUxmin() ;
        Vmax = pTCanvas->GetUxmax() ;
        TLine* pTLineO = new TLine( Vmin , Vmin + 137.46 , Vmax , Vmax + 137.46 ) ;
        pTLineO->SetLineStyle(2); pTLineO->SetLineWidth(1); pTLineO->SetLineColor(1) ; pTLineO->Draw();  
        pTCanvas->Update()    ;
      }

      pTLegend->Draw();
      pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
 
//
  std::ostringstream aostringstream_Title2 ;
  aostringstream_Title2 << std::setiosflags(std::ios::fixed) ;
  aostringstream_Title2 << "w VS " << TheNameVariable ;
  
  pTMultiGraph_W->SetTitle((aostringstream_Title2.str()).c_str());
  pTMultiGraph_W->GetXaxis()->SetTitle(TheNameVariable.c_str());
  pTMultiGraph_W->GetYaxis()->SetTitle("w (#mu m)");
  pTMultiGraph_W->GetYaxis()->SetTitleOffset(1.5);
  if (found!=std::string::npos) {
    pTMultiGraph_W->SetMinimum(    0. );
    pTMultiGraph_W->SetMaximum( 3000. );
    pTMultiGraph_W->GetXaxis()->SetLimits(-200.,900.);
  }
  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
      pTMultiGraph_W->Draw("A");
      pTCanvas->Update();

      if (found!=std::string::npos) {
        double Vmin = pTCanvas->GetUymin() ;
        double Vmax = pTCanvas->GetUymax() ;
        TLine* pTLineV = new TLine( -137.46  , Vmin , -137.46 , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(1) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

        pTF1b->Draw("SAME");
        pTCanvas->Update();
    
      pTLegend->Draw();
      pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
 
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
