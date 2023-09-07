#include "Analysis_JF/PerSample.h"
#include "Misc/Util.h"

#include "Analysis_JF/Analysis_Files.h"

#include "Analysis_JF/ReadRootFile.h"

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
#include "TFile.h"
#include "TROOT.h"
#include "TRandom3.h"

void PerSample(const std::string& SampleSet)
{
  std::string OUTDirName  = "OUT_JeanFrancois/P_Synthesis/" + SampleSet ;
  MyMakeDir(OUTDirName) ;

  gROOT->ForceStyle();
  gStyle->SetOptStat(111111) ;
  gStyle->SetOptFit(kTRUE) ;
  gStyle->SetStatW(0.20);    
  gStyle->SetStatH(0.20);    
  
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
    
    int iFile_Max = aAnalysis_Files.m_NberOfFiles ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
      
//    if (iFile!=3) continue ;   
         
      std::string base_EventFile = aAnalysis_Files.m_FilesName[iFile] ;
      
      PerRun(OUTDirName ,TheFilesDir,base_EventFile ,AnalyseName);
      
    }
  }
}

void PerRun(
   const std::string& OUTDirNameIN,
   const std::string& TheFilesDir,
   const std::string& base_EventFile,
   const std::string& AnalyseName)
{
  std::string OUTDirName  = OUTDirNameIN + "/" + AnalyseName + "/" + base_EventFile + "/" ;
  MyMakeDir(OUTDirName) ;

  gStyle->SetOptStat(0);

//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  int    Ny = pModel_ReadOutGeometry->Get_Ny() ;

//  
  TCanvas* pTCanvas = 0 ;
  std::string Str_Out_PDF     = OUTDirName + "Reconstruction.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
 
  ReadRootFile aReadRootFile ;
  aReadRootFile.Load_AnalyseReconstruction(TheFilesDir,base_EventFile, AnalyseName);

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());
  
//-------------------------//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  if (aReadRootFile.Final_TH1F_YTrackYPadInTracksAllCol) aReadRootFile.Final_TH1F_YTrackYPadInTracksAllCol->Draw("HIST");
  pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  int iTem_Max = aReadRootFile.V_pTH1F_YTrackYPadInTracks.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    if (aReadRootFile.V_pTH1F_YTrackYPadInTracks[iTem]) aReadRootFile.V_pTH1F_YTrackYPadInTracks[iTem]->Draw();
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  if (aReadRootFile.Final_TH1F_YTrackYPadLeadingInTracksAllCol) aReadRootFile.Final_TH1F_YTrackYPadLeadingInTracksAllCol->Draw("HIST");
  pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_YTrackYPadLeadingInTracks.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    if (aReadRootFile.V_pTH1F_YTrackYPadLeadingInTracks[iTem]) aReadRootFile.V_pTH1F_YTrackYPadLeadingInTracks[iTem]->Draw();
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  aReadRootFile.Final_pTH1F_YTrackInTracks_AllCol->Draw("HIST");
  pTCanvas->Update();
  
  iTem_Max = aReadRootFile.V_pTH1F_YTrackInTracks.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->Scale(20.) ;
    aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->SetLineColor(16) ;
    if (iTem==1 || iTem==34 ) aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->Draw("Hist Same");
    pTCanvas->Update();
  }
  aReadRootFile.Final_pTH1F_YTrackInTracks_AllCol->Draw("HIST same");
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_YTrackInTracks.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->SetLineColor(1) ;
    aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->Scale(1./20.) ;
    aReadRootFile.V_pTH1F_YTrackInTracks[iTem]->Draw("HIST");
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  aReadRootFile.Final_pTH1F_YTrackInTracks_AllColLARGE->Draw("HIST");
  pTCanvas->Update();
  
  iTem_Max = aReadRootFile.V_pTH1F_YTrackInTracksLARGE.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->Scale(20.) ;
    aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->SetLineColor(16) ;
    if (iTem==1 || iTem==34 ) aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->Draw("Hist Same");
    pTCanvas->Update();
  }
  aReadRootFile.Final_pTH1F_YTrackInTracks_AllColLARGE->Draw("HIST same");
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_YTrackInTracksLARGE.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->SetLineColor(1) ;
    aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->Scale(1./20.) ;
    aReadRootFile.V_pTH1F_YTrackInTracksLARGE[iTem]->Draw("HIST");
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------// page 10 and 11
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  aReadRootFile.Final_pTH1F_YFitCol_AllCol->Draw("HIST");
  pTCanvas->Update();
  
  iTem_Max = aReadRootFile.V_pTH1F_YFitCol.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    aReadRootFile.V_pTH1F_YFitCol[iTem]->Scale(20.) ;
    aReadRootFile.V_pTH1F_YFitCol[iTem]->SetLineColor(16) ;
    if (iTem==1 || iTem==34 ) aReadRootFile.V_pTH1F_YFitCol[iTem]->Draw("Hist Same");
    pTCanvas->Update();
  }
  aReadRootFile.Final_pTH1F_YFitCol_AllCol->Draw("HIST same");
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_YFitCol.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    aReadRootFile.V_pTH1F_YFitCol[iTem]->SetLineColor(1) ;
    aReadRootFile.V_pTH1F_YFitCol[iTem]->Scale(1./20.) ;
    aReadRootFile.V_pTH1F_YFitCol[iTem]->Draw("HIST");
    pTCanvas->Update();

    double Hmin = (pTCanvas->cd( iTem+1 ))->GetUxmin() ;
    double Hmax = (pTCanvas->cd( iTem+1 ))->GetUxmax() ;
    double Vmin = (pTCanvas->cd( iTem+1 ))->GetUymin() ;
    double Vmax = (pTCanvas->cd( iTem+1 ))->GetUymax() ;
    for (int iY = 0 ; iY< Ny; iY++){
      double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iY,0) *1E2 ; // WARNING: ModuleNber set to 0 
      if (  Hmin < YCur &&  YCur< Hmax){ 
        TLine* pTLineV = new TLine( YCur  , Vmin , YCur , Vmax ) ;
        pTLineV->SetLineStyle(2); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
    }

  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------// page 12 and 13
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  aReadRootFile.Final_pTH1F_YFitCol_AllColLARGE->Draw("HIST");
  pTCanvas->Update();
  
  iTem_Max = aReadRootFile.V_pTH1F_YFitColLARGE.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->Scale(20.) ;
    aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->SetLineColor(16) ;
    if (iTem==1 || iTem==34 ) aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->Draw("Hist Same");
    pTCanvas->Update();
  }
  aReadRootFile.Final_pTH1F_YFitCol_AllColLARGE->Draw("HIST same");
  pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_YFitColLARGE.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->SetLineColor(1) ;
    aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->Scale(1./20.) ;
    aReadRootFile.V_pTH1F_YFitColLARGE[iTem]->Draw("HIST");
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//-------------------------// page 14 and 15
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  
  aReadRootFile.Final_pTH1F_TrackDeltaTAllCol->Draw("HIST");
  pTCanvas->Update();
  
  pTCanvas->Print(Str_Out_PDF.c_str());
    
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Divide(9,4) ;
  
  iTem_Max = aReadRootFile.V_pTH1F_TrackDeltaT.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    pTCanvas->cd(iTem+1) ;
    aReadRootFile.V_pTH1F_TrackDeltaT[iTem]->Draw();
    pTCanvas->Update();
  }
  pTCanvas->Print(Str_Out_PDF.c_str());

//  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 2400, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());
  
}
