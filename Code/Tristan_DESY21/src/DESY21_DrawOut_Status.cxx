#include "Tristan_DESY21/DESY21_DrawOut_Status.h"
#include "Tristan_DESY21/DESY21_Fits.h"

#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "EvtModelTools/EvtModelTools_TD_Histos.h"


#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"

using namespace std;

void DESY21_DrawOut_Status(Sample& aSample, const int& ModuleNber ,const string& OUTDIR,const string& TAG, std::string Stage, const int Procedure, const int& iIter, TF1* ptf1_PRF_fit)
{
  // PDF File
  string OutputFile     = OUTDIR + "Histograms/" + TAG + "_DESY21_DrawOut_Status_iter_" + std::to_string(iIter) + ".pdf" ;
  string OutputFile_Beg = OutputFile + "(" ;
  string OutputFile_End = OutputFile + ")" ;
  
  // Page 1 Basic control plots on position, number of clusters
  TH1I* pTH1I_PadPerCluster             = GiveMe_PadPerCluster            (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_LeadingiY                 = GiveMe_LeadingiY                (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_Cluster_Verticality       = GiveMe_Cluster_Verticality      (aSample, ModuleNber , TAG) ;

  TH1I* pTH1I_Cluster_Density           = GiveMe_NClusPerCol              (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_Event_Density             = GiveMe_ClusterPerEvent          (aSample, ModuleNber , TAG) ;
  TH1D* pTH1D_Rho                       = GiveMe_Rho                      (aSample, ModuleNber , TAG) ;
  TH1D* pTH1D_Rho2                      = GiveMe_Rho                      (aSample, ModuleNber , TAG + "_2", 2, 2) ;
  TH1D* pTH1D_Rho3                      = GiveMe_Rho                      (aSample, ModuleNber , TAG + "_3", 3, 3) ;
  TH1D* pTH1D_Rho4                      = GiveMe_Rho                      (aSample, ModuleNber , TAG + "_4", 4, 4) ;

  // Page 2 Amplitude 
  TH1I* pTH1I_Amax_AmaxWF               = GiveMe_Amax_AmaxWF              (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_AmaxLead                  = GiveMe_AmaxLeading              (aSample, ModuleNber , TAG) ;

  TH1I* pTH1I_AClus_ASumPads            = GiveMe_AClus_ASumPads           (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_AmaxSubLead               = GiveMe_AmaxSubLeading           (aSample, ModuleNber , TAG) ;

  TH2D* pTH2D_Amax_iX                   = GiveMe_2D_ALeading_iX           (aSample, ModuleNber , TAG) ;
  TH2D* pTH2D_Amaxsubrelative_iX        = GiveMe_2D_AsubRelative_iX       (aSample, ModuleNber , TAG) ;

  // Page 3 Timing #DeltaT
  TH1I* pTH1I_Tmax_TmaxWF               = GiveMe_Tmax_TmaxWF              (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_Tleading                  = GiveMe_LeadTmax                 (aSample, ModuleNber , TAG) ;
  TH1I* pTH1I_TLeadTEvent               = GiveMe_TLeading_TEvent          (aSample, ModuleNber , TAG) ;

  TH2I* pTH2_DeltaT21vs31               = GiveMe_DeltaT21vs31             (aSample, ModuleNber , TAG, Stage) ;
  TProfile* tp_DeltaT_YTYpad            = GiveMe_DeltaT_YTYpad            (aSample, ModuleNber , TAG) ;
  TH2I* pTH2I_TmaxPadvLead              = GiveMe_Tmax_PadvLead            (aSample, ModuleNber , TAG) ;


  // Page 4  Y_T , Y_W - Y_pad & #rho
  TH1D* pTH1D_YT                        = GiveMe_YT                       (aSample, ModuleNber , TAG) ;
  TProfile* tp_Rho_YTYpad               = GiveMe_Rho_YTYpad               (aSample, ModuleNber , TAG, 250, -3, 3, -0.1, 1.1) ;

  TH1D* pTH1D_RelativY                  = GiveMe_YTYpad                   (aSample, ModuleNber , TAG) ;
  TH2D* pTH2D_Rho_YTYpad = 0;
  if(Procedure == 2) pTH2D_Rho_YTYpad   = GiveMe_2D_RhoPV2_YTYpad         (aSample, ModuleNber , TAG) ;
  if(Procedure == 1) pTH2D_Rho_YTYpad   = GiveMe_2D_Rho_YTYpad            (aSample, ModuleNber , TAG, 1) ;
  if(Procedure == 0) pTH2D_Rho_YTYpad   = GiveMe_2D_Rho_YTYpad            (aSample, ModuleNber , TAG, 0) ;   

  TH2D* pTH2D_Rho_YT                    = GiveMe_2D_Rho_YT                (aSample, ModuleNber , TAG) ;
  TProfile* tp_Rho_YT                   = GiveMe_Rho_YT                   (aSample, ModuleNber , TAG) ;

  // Page 5 DeltaT and Rho VS DeltaT from neighbours

  // Page 6 Again amplitude stuff
  TH1D* pTH1D_AclusNPads                = GiveMe_AclusNPads               (aSample, ModuleNber , TAG, Stage) ;
  TH2D* pTH2D_A21vsA31                  = GiveMe_2D_A21vsA31              (aSample, ModuleNber , TAG, Stage) ;
  TH1D* pTH1D_TrackAlignSelectResiduals = GiveMe_TrackAlignResiduals      (aSample, ModuleNber , TAG, Stage, iIter) ;

//
  int OPTSTAT_IN = gStyle->GetOptStat() ;
  gStyle->SetOptStat(111111) ;

  // Canvas for png files
  TCanvas* Canvaspng = new TCanvas("Canvaspng"," ",1200, 900) ;
  TCanvas* pTCanvas =  new TCanvas("", "", 3200, 1800) ;

// page 1 Geometry
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->Divide(3,2) ; 

  pTCanvas->cd(1) ;
  gPad->SetLogy() ;
  pTH1I_PadPerCluster->Draw() ;
  if(iIter == 0) pTCanvas->cd(1)->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_PadsPerCluster_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(2) ;
  gPad->SetLogy(0) ;
  pTH1I_LeadingiY->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1I_LeadingiY->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_Lead_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(3) ;
  gPad->SetLogy() ;
  pTH1I_Cluster_Verticality->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_Cluster_Verticality->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_Verticality_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(4) ;
  gPad->SetLogy(0) ;
  pTH1I_Cluster_Density->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1I_Cluster_Density->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_ClustersPerColumn_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(5) ;
  gPad->SetLogy() ;
  pTH1I_Event_Density->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_Event_Density->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_ClustersPerEvent_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(6) ;
  gPad->SetLogy(0) ;
  pTH1D_Rho->Scale(1./double(pTH1D_Rho->GetEntries() ) ) ;
  pTH1D_Rho->SetStats(0) ;
  pTH1D_Rho3->Scale(1./double(pTH1D_Rho3->GetEntries() ) ) ;
  pTH1D_Rho3->SetStats(0) ;
  if(pTH1D_Rho2 != 0){
    pTH1D_Rho2->Scale(1./double(pTH1D_Rho2->GetEntries() ) ) ;
    pTH1D_Rho2->SetStats(0) ;
  }
  if(pTH1D_Rho4 != 0){
    pTH1D_Rho4->Scale(1./double(pTH1D_Rho4->GetEntries() ) ) ;
    pTH1D_Rho4->SetStats(0) ;
  }
  pTH1D_Rho->SetLineColor(1) ;
  pTH1D_Rho3->SetLineColor(3) ;
  if(pTH1D_Rho2 != 0) pTH1D_Rho2->SetLineColor(2) ;
  if(pTH1D_Rho4 != 0) pTH1D_Rho4->SetLineColor(4) ;
  TLegend* leg = new TLegend(0.15,0.55,0.4,0.85) ; 
  leg->AddEntry(pTH1D_Rho,"All clusters","l") ;
  if(pTH1D_Rho2 != 0) leg->AddEntry(pTH1D_Rho2,"Clusters with 2 pads","l") ;
  leg->AddEntry(pTH1D_Rho3,"Clusters with 3 pads","l") ;
  if(pTH1D_Rho4 != 0) leg->AddEntry(pTH1D_Rho4,"Clusters with 4 pads","l") ;

  pTH1D_Rho3->Draw("HIST") ;
  pTH1D_Rho->Draw("SAME, HIST") ;
  if(pTH1D_Rho2 != 0) pTH1D_Rho2->Draw("SAME, HIST") ;
  if(pTH1D_Rho4 != 0) pTH1D_Rho4->Draw("SAME, HIST") ;
  leg->DrawClone("SAME") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1D_Rho3->Draw("HIST") ;
  pTH1D_Rho->Draw("SAME, HIST") ;
  if(pTH1D_Rho2 != 0) pTH1D_Rho2->Draw("SAME, HIST") ;
  if(pTH1D_Rho4 != 0) pTH1D_Rho4->Draw("SAME, HIST") ;
  leg->Draw("SAME") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_1_Hist_Rho_nPads_Distribution_iter_" + std::to_string(iIter) + ".png")) ;
  delete leg ;

  pTCanvas->SaveAs(OutputFile_Beg.c_str()) ;
// page 2 Amplitude
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->Divide(3,2) ; 

  pTCanvas->cd(1) ;
  gPad->SetLogy() ;
  pTH1I_Amax_AmaxWF->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_Amax_AmaxWF->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist_Amax_WFvsPad_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(2) ;
  gPad->SetLogy() ;
  pTH1I_AmaxLead->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_AmaxLead->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist_Amax_Leading_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(3) ;
  gPad->SetLogy(0) ;
  pTH2D_Amax_iX->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2D_Amax_iX->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist2D_Amax_iX_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(4) ;
  gPad->SetLogy() ;
  pTH1I_AClus_ASumPads->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_AClus_ASumPads->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist_AClus_ASumPads_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(5) ;
  gPad->SetLogy() ;
  pTH1I_AmaxSubLead->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_AmaxSubLead->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist_Amax_SubLeading_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(6) ;
  gPad->SetLogy(0) ;
  pTH2D_Amaxsubrelative_iX->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2D_Amaxsubrelative_iX->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_2_Hist2D_AmaxSubOverLead_iX_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(6) ;

  pTCanvas->SaveAs(OutputFile.c_str()) ;
// page 3 Timing
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->Divide(3,2) ; 

  pTCanvas->cd(1) ;
  gPad->SetLogy() ;
  pTH1I_Tmax_TmaxWF->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_Tmax_TmaxWF->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_Hist_Tmax_WFvsPad_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(2) ;
  gPad->SetLogy() ;
  pTH1I_Tleading->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy() ;
  pTH1I_Tleading->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_Hist_Tmax_Leading_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(3) ;
  gPad->SetLogy() ;
  pTH1I_TLeadTEvent->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1I_TLeadTEvent->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_Hist_TLead_TEvent_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(4) ;
  gPad->SetLogy(0) ;
  pTH2_DeltaT21vs31->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2_DeltaT21vs31->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_Hist_T21vsT31_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(5) ;
  gPad->SetLogy(0) ;
  tp_DeltaT_YTYpad->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  tp_DeltaT_YTYpad->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_TProf_DeltaT_YTYpad_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(6) ;
  gPad->SetLogy(0) ;
  pTH2I_TmaxPadvLead->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2I_TmaxPadvLead->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_3_Hist2D_DeltaT_TmaxLead_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->SaveAs(OutputFile.c_str()) ;
// page 4 #rho & y_{w} - y_{pad}
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->Divide(3,2) ; 

  pTCanvas->cd(1) ;
  gPad->SetLogy(0) ;
  pTH1D_YT->Draw() ;
  // double Ymin = pTCanvas->cd(1)->GetUymin() ;
  // double Ymax = pTCanvas->cd(1)->GetUymax() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1D_YT->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_Hist_YT_Distribution_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(2) ;
  gPad->SetLogy(0) ;
  tp_Rho_YTYpad->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  tp_Rho_YTYpad->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_TProf_Rho_YTYpad_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(3) ;
  gPad->SetLogy(0) ;
  tp_Rho_YT->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  tp_Rho_YT->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_TProf_Rho_YT_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(4) ;
  gPad->SetLogy(0) ;
  pTH1D_RelativY->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1D_RelativY->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_Hist_YT-Ypad_Distribution_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(5) ;
  gPad->SetLogy(0) ;
  pTH2D_Rho_YTYpad->Draw("colz") ;
  if(ptf1_PRF_fit != 0) ptf1_PRF_fit->Draw("same") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2D_Rho_YTYpad->Draw("colz") ;
  if(ptf1_PRF_fit != 0){
    ptf1_PRF_fit->Draw("same") ;
    ptf1_PRF_fit->SetLineWidth(2) ;
  }
  Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_Hist2D_Rho_YTYpad_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(6) ;
  gPad->SetLogy(0) ;
  pTH2D_Rho_YT->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2D_Rho_YT->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_4_Hist2D_Rho_YT_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->SaveAs(OutputFile.c_str()) ;
  // page 5 #DeltaT & #rho
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->SetWindowSize(3150,1950) ;
  pTCanvas->Divide(4,3) ;

  // TH1 #DeltaT
  for(int  i = 1 ; i <= 4 ; i++){
    TH1I* pTH1I_DeltaT = GiveMe_DeltaT(aSample, ModuleNber , TAG, Stage, i+1, 450, -150, 300) ;
    pTCanvas->cd(i) ;
    gPad->SetLogy() ;
    pTH1I_DeltaT->DrawClone() ;
    Canvaspng->cd() ;
    Canvaspng->SetLogy() ;
    pTH1I_DeltaT->Draw() ;
    if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_5_Hist_DeltaT"+int(i+1)+"_iter_" + std::to_string(iIter) + ".png")) ;
    delete pTH1I_DeltaT ;
  }

  // TProfile #rho VS #DeltaT
  for(int  i = 1 ; i <= 4 ; i++){
    TProfile* tp_Rho_DeltaT = GiveMe_Rho_DeltaT(aSample, ModuleNber , TAG, Stage, i+1) ;
    pTCanvas->cd(i+4) ;
    gPad->SetLogy(0) ;
    tp_Rho_DeltaT->DrawClone() ;
    Canvaspng->cd() ;
    Canvaspng->SetLogy(0) ;
    tp_Rho_DeltaT->Draw() ;
    if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_5_TProf_Rho_DeltaT"+int(i+1)+"_iter_" + std::to_string(iIter) + ".png")) ;
    delete tp_Rho_DeltaT ;
  }

  // TH2 #rho VS #DeltaT
  for(int  i = 1 ; i <= 4 ; i++){
    TH2F* pTH2F_Rho_DeltaT = GiveMe_2D_Rho_DeltaT(aSample, ModuleNber , TAG, Stage, i+1) ;
    pTH2F_Rho_DeltaT->SetStats(0) ;
    pTCanvas->cd(i+8) ;
    gPad->SetLogy(0) ;
    pTH2F_Rho_DeltaT->DrawClone("colz") ;
    Canvaspng->cd() ;
    Canvaspng->SetLogy(0) ;
    pTH2F_Rho_DeltaT->Draw("colz") ;
    if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_5_Hist2D_Rho_DeltaT"+int(i+1)+"_iter_" + std::to_string(iIter) + ".png")) ;
    delete pTH2F_Rho_DeltaT ;
  }

  pTCanvas->SaveAs(OutputFile.c_str()) ;
  // page 6 Again amplitude stuff
  Canvaspng->Clear() ;
  pTCanvas->Clear() ;
  pTCanvas->SetWindowSize(3200, 1800) ;
  pTCanvas->Divide(3,2) ; 

  pTCanvas->cd(1) ;
  gPad->SetLogy(0) ;
  pTH1D_AclusNPads->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1D_AclusNPads->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_6_Hist2D_AclusNPads_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(2) ;
  gPad->SetLogy(0) ;
  pTH2D_A21vsA31->Draw("colz") ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH2D_A21vsA31->Draw("colz") ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_6_Hist2D_A21_A31_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->cd(3) ;
  gPad->SetLogy(0) ;
  pTH1D_TrackAlignSelectResiduals->Draw() ;
  Canvaspng->cd() ;
  Canvaspng->SetLogy(0) ;
  pTH1D_TrackAlignSelectResiduals->Draw() ;
  if(iIter == 0) Canvaspng->SaveAs(TString(OUTDIR + "Histograms/" + TAG + "_6_Hist_TrackAlignSelectResiduals_iter_" + std::to_string(iIter) + ".png")) ;

  pTCanvas->Update() ;
  pTCanvas->SaveAs(OutputFile_End.c_str()) ;

  //  
  delete pTH1I_PadPerCluster              ;
  delete pTH1I_LeadingiY                  ;
  delete pTH1I_Cluster_Verticality        ;
  delete pTH1I_Cluster_Density            ;
  delete pTH1I_Event_Density              ;
  delete pTH1D_Rho                        ;
  delete pTH1D_Rho2                       ;
  delete pTH1D_RelativY                   ;
  delete pTH1D_Rho3                       ;
  delete pTH1D_Rho4                       ;
  delete pTH2D_Amax_iX                    ;
  delete pTH1I_Amax_AmaxWF                ;
  delete pTH1I_Tmax_TmaxWF                ;
  delete pTH1D_YT                         ;
  delete pTH2D_Amaxsubrelative_iX         ;
  delete pTH1I_Tleading                   ;
  delete pTH2I_TmaxPadvLead               ;
  delete tp_Rho_YTYpad                    ;
  delete tp_DeltaT_YTYpad                 ;
  delete pTH1I_TLeadTEvent                ;
  delete pTH1I_AmaxLead                   ;
  delete pTH1I_AmaxSubLead                ;
  delete pTH1I_AClus_ASumPads             ;
  delete pTH2D_Rho_YT                     ;
  delete pTH2D_Rho_YTYpad                 ;
  delete pTH2_DeltaT21vs31                ;
  delete pTH1D_AclusNPads                 ;
  delete pTH2D_A21vsA31                   ;
  delete pTH1D_TrackAlignSelectResiduals  ;


  delete Canvaspng                        ; 
  delete pTCanvas                         ;

//
  gStyle->SetOptStat(OPTSTAT_IN) ;
}
