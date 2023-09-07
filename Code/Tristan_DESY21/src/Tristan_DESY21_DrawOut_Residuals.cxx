#include "Tristan_DESY21/Tristan_DESY21_DrawOut_Residuals.h"
#include "Tristan_DESY21/Tristan_DESY21_Monitoring.h"
#include "Tristan_DESY21/Tristan_DESY21_Fits.h"

#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Track.h"
#include "AnalysisTools/DoTracksReconstruction.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TPaveStats.h"

#include "EvtModelTools/JFL_Selections.h"

void Tristan_DESY21_DrawOut_Residuals(Sample& aSample, const int& ModuleNber , const std::string& OUTDIR, const std::string& TAG, TGraphErrors* ptge_std_mean, TGraphErrors* ptge_bias_mean, const int& zDrift, const int& iIter)
{
  // PDF File
  std::string OutputFile     = OUTDIR + "Fits/" + TAG + "_Residuals_iter" + std::to_string(iIter) +".pdf" ;
  std::string OutputFile_Beg = OutputFile + "(" ;
  std::string OutputFile_End = OutputFile + ")" ;

  TCanvas*
  pCanvasresidual  = new TCanvas("","",3500, 2800) ;
  pCanvasresidual->Divide(6,6) ;

  TGraphErrors* ptge_sigmapercol  = new TGraphErrors() ;
  TGraphErrors* ptge_meanpercol   = new TGraphErrors() ;
  for (int iX = 1 ; iX < 35; iX++){
    TH1D* ph1_Residual  = GiveMe_Residual_Distribution(aSample, ModuleNber , TAG, iIter, iX) ;
    pCanvasresidual->cd(iX+1) ;
    std::ostringstream strcol ;
    strcol << std::setiosflags(std::ios::fixed) ;
    strcol << "Column " << iX ;
    std::string title_str =  strcol.str() ; 
    ph1_Residual->SetTitle(title_str.c_str()) ;

    ph1_Residual->DrawClone() ;
    TF1* ptf1_Residual    = GiveMe_Residual_Fit(ph1_Residual) ;

    double pmean          = ptf1_Residual->GetParameter(1) ;
    double pmeanErr       = ptf1_Residual->GetParError(1) ;
    ptge_meanpercol->SetPoint(ptge_meanpercol->GetN(), iX*1.019, pmean);
    ptge_meanpercol->SetPointError(ptge_meanpercol->GetN()-1, 0, pmeanErr);

    double psigmaErr      = ptf1_Residual->GetParError(2) ;
    double psigma         = ptf1_Residual->GetParameter(2) ;
    ptge_sigmapercol->SetPoint(ptge_sigmapercol->GetN(), iX*1.019, psigma);
    ptge_sigmapercol->SetPointError(ptge_meanpercol->GetN()-1, 0, psigmaErr);
  }
  ptge_std_mean->SetPoint       (zDrift, zDrift*100+60, ptge_sigmapercol->GetMean(2)) ;
  ptge_std_mean->SetPointError  (zDrift, 0, ptge_sigmapercol->GetRMS(2)/ptge_sigmapercol->GetN()) ;
  ptge_bias_mean->SetPoint      (zDrift, zDrift*100+60, ptge_meanpercol->GetMean(2)) ;
  ptge_bias_mean->SetPointError (zDrift, 0, ptge_meanpercol->GetRMS(2)/ptge_meanpercol->GetN()) ;

  pCanvasresidual->SaveAs(OutputFile_Beg.c_str()) ;
  pCanvasresidual->SaveAs(TString(OUTDIR + "Fits/" + TAG + "_Residuals_per_col_iter" + std::to_string(iIter) + ".png")) ;

  gStyle->SetOptStat(111111) ;
  pCanvasresidual->Clear() ;
  pCanvasresidual->Divide(1,1) ;
  ptge_meanpercol->SetMarkerStyle(4) ;
  ptge_meanpercol->SetMarkerSize(5) ;
  ptge_meanpercol->SetNameTitle("TGE_Bias", TString(TAG + " : Bias per column iter " + std::to_string(iIter))) ;
  ptge_meanpercol->GetXaxis()->SetTitle("x (cm)") ;
  ptge_meanpercol->GetYaxis()->SetTitle("bias (#mum)") ;
  pCanvasresidual->cd() ;
  ptge_meanpercol->Draw("AP") ;  
  pCanvasresidual->SaveAs(TString(OUTDIR + "Fits/" + TAG + "_Bias_iter" + std::to_string(iIter) + ".png")) ;

  pCanvasresidual->SaveAs(OutputFile.c_str()) ;

  pCanvasresidual->Clear() ;
  ptge_sigmapercol->SetMarkerStyle(4) ;
  ptge_sigmapercol->SetMarkerSize(5) ;
  ptge_sigmapercol->SetNameTitle("TGE_Resolution", TString(TAG + " : Resolution per column iter " + std::to_string(iIter))) ;
  ptge_sigmapercol->GetXaxis()->SetTitle("x (cm)") ;
  ptge_sigmapercol->GetYaxis()->SetTitle("resolution (#mum)") ;
  pCanvasresidual->cd() ;
  ptge_sigmapercol->Draw("AP") ; 
  pCanvasresidual->SaveAs(TString(OUTDIR + "Fits/" + TAG + "_Resolution_iter" + std::to_string(iIter) + ".png")) ;

  pCanvasresidual->SaveAs(OutputFile_End.c_str()) ;

  TFile* ptfile_zresi = new TFile(TString(OUTDIR + "ROOT_files/Residuals/" + TAG + "_Residuals_iter" + std::to_string(iIter) + ".root"), "RECREATE") ;
  ptge_meanpercol->Write() ;
  ptge_sigmapercol->Write() ;
  ptfile_zresi->Close() ;
}

void Tristan_DrawOut_DD_functions(const std::string& Stage, TGraphErrors* ptge_std_mean, TGraphErrors* ptge_bias_mean, TGraphErrors* ptge_RC, const int& pt)
{
  std::ostringstream residu_str ;
  residu_str << std::setiosflags(std::ios::fixed) ;
  residu_str << "sidupt" << pt << "/Residuals/" << Stage << "/pt" << pt << "_" << Stage;
  std::string OUTDIR_mean_residuals =  residu_str.str() ;

  // Resolution
  MakeMe_Resolution_Fit(ptge_std_mean) ;
  gStyle->SetOptStat(1) ;
  ptge_std_mean->SetNameTitle("ptge_resolution_mean", TString("Resolution for each drifting distance (" + Stage + ", " + pt + " ns)")) ;
  ptge_std_mean->GetXaxis()->SetTitle("Drifting distance (mm)") ;
  ptge_std_mean->GetYaxis()->SetTitle("Resolution (#mum)") ;
  TCanvas* pCanvasSigmean = new TCanvas("pCanvasSigmean", "pCanvasSigmean", 1200, 900) ;
  pCanvasSigmean->cd() ;
  ptge_std_mean->SetMaximum(450) ;
  ptge_std_mean->SetMarkerStyle(4) ;
  ptge_std_mean->SetMarkerSize(2) ;
  ptge_std_mean->Draw("AP") ;
  pCanvasSigmean->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_Resolution_mean.png")) ;
  pCanvasSigmean->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_Resolution_mean.pdf")) ;

  // Bias
  ptge_bias_mean->SetNameTitle("ptge_bias_mean", "Bias for each drifting distance (PV2, 412 ns)") ;
  ptge_bias_mean->GetXaxis()->SetTitle("Drifting distance (mm)") ;
  ptge_bias_mean->GetYaxis()->SetTitle("Bias (#mum)") ;
  TCanvas* pCanvasBiasmean = new TCanvas("pCanvasBiasmean", "pCanvasBiasmean", 1400, 1100) ;
  pCanvasBiasmean->cd() ;
  ptge_bias_mean->SetMarkerStyle(4) ;
  ptge_bias_mean->SetMarkerSize(2) ;
  ptge_bias_mean->Draw("AP") ;
  pCanvasBiasmean->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_Bias_mean.png")) ;
  pCanvasBiasmean->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_Bias_mean.pdf")) ;

  // RC
  ptge_RC->SetNameTitle("ptge_RC", "RC for each drifting distance (PV2, 412 ns)") ;
  ptge_RC->GetXaxis()->SetTitle("Drifting distance (mm)") ;
  ptge_RC->GetYaxis()->SetTitle("RC (ns/mm^{2})") ;
  TCanvas* pCanvasRC = new TCanvas("pCanvasRC", "pCanvasRC", 1200, 900) ;
  pCanvasRC->cd() ;
  ptge_RC->SetMarkerStyle(4) ;
  ptge_RC->SetMarkerSize(2) ;
  ptge_RC->Draw("AP") ;
  pCanvasRC->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_RC.png")) ;
  pCanvasRC->SaveAs(TString(OUTDIR_mean_residuals + "pt" + pt + "_RC.pdf")) ;

  // Save in ROOT file the histograms
  TFile* pfileROOT_RC = new TFile(TString(OUTDIR_mean_residuals + "_Residuals.root"), "RECREATE");
  ptge_std_mean->Write();
  ptge_bias_mean->Write();  
  ptge_RC->Write();
  pfileROOT_RC->Close() ;
}
