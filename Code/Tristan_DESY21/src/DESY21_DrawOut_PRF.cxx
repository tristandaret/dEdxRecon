#include "Tristan_DESY21/DESY21_DrawOut_PRF.h"
#include "Tristan_DESY21/DESY21_Fits.h"

#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TPaveStats.h"

void DESY21_DrawOut_PRF(Sample& aSample, const int& ModuleNber , const std::string& OUTDIR, const std::string& TAG, std::string& Stage, const int Procedure, const int& iIter, const int& NIter, TF1* ptf1_PRF_fit)
{

  // PDF File
  std::string OutputFile     = OUTDIR + "Fits/" + TAG + "_DESY21_DrawOut_PRF.pdf" ;
  std::string OutputFile_Beg = OutputFile + "(" ;
  std::string OutputFile_End = OutputFile + ")" ;

  gStyle->SetOptStat(111111) ;
  TCanvas* pCanvasPRF = new TCanvas("pCanvasPRF","", 3100, 500) ;
  TCanvas* pCanvasPNG = new TCanvas("Canvaspng","",1200, 900) ;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  TH1D* ph1_YTYpad                  = GiveMe_YTYpad                         (aSample, ModuleNber , std::string(TAG + "PRF")) ;
  TH2D* ph2_Rho_YTYpad = 0;
  if(Procedure == 2) ph2_Rho_YTYpad = GiveMe_2D_RhoPV2_YTYpad               (aSample, ModuleNber , TAG) ;
  if(Procedure == 1) ph2_Rho_YTYpad = GiveMe_2D_Rho_YTYpad                  (aSample, ModuleNber , TAG, 1) ;
  if(Procedure == 0) ph2_Rho_YTYpad = GiveMe_2D_Rho_YTYpad                  (aSample, ModuleNber , TAG, 0) ;  
  TGraphErrors* tgr_Rho_YTYpad      = Convert_TH2_TGE                  (ph2_Rho_YTYpad) ;
  TH1D* ph1_DeltaYT                 = GiveMe_DeltaYT                        (aSample, ModuleNber , TAG, 200, -20, 20) ;
  TH2D* ph2_DeltaY_YTYpad           = GiveMe_2D_DeltaY_YTYpad               (aSample, ModuleNber , TAG, 350, -3.5, 3.5, 200, -20, 20) ;
  TH1D* pth1_chi2min_residual       = GiveMe_chi2min_residual_distribution  (aSample, ModuleNber , TAG, iIter) ;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////

  // Drawing the plots
  pCanvasPRF->Divide(6,1) ;

  pCanvasPRF->cd(1) ;
  tgr_Rho_YTYpad->DrawClone() ;

  pCanvasPRF->cd(2) ;
  ph2_Rho_YTYpad->SetStats(0) ;
  ph2_Rho_YTYpad->DrawClone("colz") ;
  ptf1_PRF_fit->Draw("same") ;
  pCanvasPNG->cd() ;
  ph2_Rho_YTYpad->DrawClone("colz") ;
  ptf1_PRF_fit->Draw("same") ;
  ptf1_PRF_fit->SetLineWidth(2) ;
  pCanvasPNG->SaveAs(TString(OUTDIR + "Fits/" + TAG + "_Hist2D_PRF_Iter" + iIter + ".png")) ;

  pCanvasPRF->cd(3) ;
  ph1_YTYpad->DrawClone() ;
  pCanvasPNG->cd() ;
  ph1_YTYpad->Draw() ;
  pCanvasPNG->SaveAs(TString(OUTDIR + "Fits/" + TAG + "_Hist_YTYpad_Iter" + iIter + ".png")) ;

  pCanvasPRF->cd(4) ;
  gPad->SetLogy() ;
  ph1_DeltaYT->Draw() ;

  pCanvasPRF->cd(5) ;
  gPad->SetLogy(0) ;
  gStyle->SetOptStat("eou") ;
  ph2_DeltaY_YTYpad->DrawClone("colz") ;

  pCanvasPRF->cd(6) ;
  gPad->SetLogy(1) ;
  pth1_chi2min_residual->DrawClone() ;

  pCanvasPRF->Update() ;
  if(iIter == 0) pCanvasPRF->SaveAs(OutputFile_Beg.c_str()) ;
  if(iIter == NIter) pCanvasPRF->SaveAs(OutputFile_End.c_str()) ;
  if(iIter != 0 and iIter != NIter) pCanvasPRF->SaveAs(OutputFile.c_str()) ;
  gStyle->SetOptStat(111111) ;

  pCanvasPRF->Clear() ;

  // Save in ROOT file the histograms
  std::ostringstream filestr ;
  filestr << std::setiosflags(std::ios::fixed) ;
  filestr << OUTDIR << "ROOT_files/Iterations_fit/" << TAG << "_Fits_iter" << iIter << ".root" ;
  std::string FILEstring =  filestr.str() ;
  TFile* pfileROOT = new TFile(TString(FILEstring), "RECREATE");

  ph1_YTYpad->Write();
  ph2_Rho_YTYpad->Write();
  tgr_Rho_YTYpad->Write() ;
  ph2_DeltaY_YTYpad->Write();
  pth1_chi2min_residual->Write() ;
  ptf1_PRF_fit->Write() ;

  pfileROOT->Close() ;
  delete pfileROOT ;

  delete pCanvasPRF ;
  delete pCanvasPNG ;

  delete ph1_YTYpad ;
  delete ph2_Rho_YTYpad ;
  delete tgr_Rho_YTYpad ;
  delete ph1_DeltaYT ;
  delete ph2_DeltaY_YTYpad ;
  delete pth1_chi2min_residual ;
  delete ptf1_PRF_fit ;
}
