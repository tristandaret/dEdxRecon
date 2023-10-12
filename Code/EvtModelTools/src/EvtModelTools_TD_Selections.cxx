#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "Tristan_DESY21/Tristan_DESY21_Fits.h"
#include "Misc/Util.h"


#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"

// CERN22; Get cut values for the time selection (run based)
std::vector<int> SetStage120Cuts(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF)
{
  std::vector<int>                        v_TCut ;

  TH1F* h1f_TLead                       = new TH1F("h1f_TLead",  "T_{leading}", 511,  -0.5 , 510.5 );

  int NEvent                            = pUploader->Get_NberOfEvent() ;
  int TLow                              = 0 ;
  int THigh                             = 0 ;
  for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                    = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    if (!pEvent or pEvent->IsValid() == 0){
      delete pEvent;
      continue;
    }  

    int nMod                  = pEvent->Get_NberOfModule() ;
    if(nMod < 4){ delete pEvent; continue;}
    for (int iMod = 0; iMod < nMod ; iMod++){
      Module* pModule                 = pEvent->Get_Module_InArray(iMod) ;
      if (pEvent->Validity_ForThisModule(iMod) == 0) continue;

      int NClusters                   = pModule->Get_NberOfCluster() ;
      for (int iC = 0; iC < NClusters; iC++){
        Cluster* pCluster             = pModule->Get_Cluster(iC) ;
        double TL               = pCluster->Get_TMaxLeading() ;
        if (TL>5. and TL < 509.) h1f_TLead->Fill( TL ) ;
      }
    }
    delete pEvent ;
  }

  double MaxTLead = h1f_TLead->GetMaximum();
  std::cout << MaxTLead << std::endl;
  int nbin_max                        = h1f_TLead->GetMaximumBin() ;        
  std::cout << nbin_max << std::endl;                       // bin of the highest peak

  std::string OutputFile      = "h1f_test_timing.pdf" ;
  TCanvas* pTCanvas           = new TCanvas("TCanvas_timing", "TCanvas_timing", 1800, 1200) ;
  pTCanvas->                    cd();
  pTCanvas->                    SetLogy(1);
  h1f_TLead->                   SetLineWidth(2) ;
  h1f_TLead->                   Draw();
  pTCanvas->                    SaveAs(OutputFile.c_str());
  delete pTCanvas;

  if(nbin_max <= 0 or nbin_max >= 510){
    std::cout << "Bug with nbin_max: value out of time range | Set cuts to  [0, 510]" << std::endl;
    v_TCut.push_back(0);
    v_TCut.push_back(510);
  }
  else{
    int iDeltaBinP                      = 10 ;                                                            // initialization of increments
    int iDeltaBinM                      = 10 ;
    float y_init                        = h1f_TLead->GetBinContent(nbin_max) ;                       // value of the peak
    float y_0                           = h1f_TLead->GetBinContent(nbin_max) ;                       // value of the peak
    float y_minus1                      = h1f_TLead->GetBinContent(nbin_max-10) ;                    // value of the previous bin
    float y_plus1                       = h1f_TLead->GetBinContent(nbin_max+10) ;                    // value of the next bin
    // Find the lower boundary
    while(y_minus1 - y_0 < y_init/2500 and y_minus1 > 0.001*y_init and iDeltaBinM < 509){
      iDeltaBinM++ ;
      y_0                               = h1f_TLead->GetBinContent(nbin_max-iDeltaBinM) ;
      y_minus1                          = h1f_TLead->GetBinContent(nbin_max-iDeltaBinM-1) ;
    }
    // Find the higher boundary
    y_0                                 = h1f_TLead->GetBinContent(nbin_max) ;                       // reboot y_0 to maximum value peak
    while(y_plus1 - y_0 < y_init/2500 and y_minus1 > 0.001*y_init and iDeltaBinP < 509){
      iDeltaBinP++ ;
      y_0                               = h1f_TLead->GetBinContent(nbin_max+iDeltaBinP) ;
      y_plus1                           = h1f_TLead->GetBinContent(nbin_max+iDeltaBinP+1) ;
    }

    TLow                                = nbin_max - 1 - iDeltaBinM ;
    THigh                               = nbin_max - 1 + iDeltaBinP ;
    if(TLow < 0 or TLow > 510){
      std::cout << "Bug with TLow = " << TLow << ": value out of time range | ";
      TLow = 0;
      std::cout << "Reset to " << TLow << std::endl;
    }
    if(THigh < 0 or THigh > 510){
      std::cout << "Bug with THigh = " << THigh << ": value out of time range | ";
      THigh = 510;
      std::cout << "Reset to " << THigh << std::endl;
    }
    v_TCut.                               push_back(TLow) ;
    v_TCut.                               push_back(THigh) ;
  }
  delete h1f_TLead;
  return v_TCut ;
}







// 5: Discard clusters misaligned with the tendency of the track
void Selection_TD_TrackAlignement (Sample& aSample, const int& ModuleNber, const std::string& TAG, const float& cutoff, const int& zdrift)
{
  std::cout << std::endl << "Selection Track Alignement: Discard clusters misaligned with the tendency of the track" << std::endl ;

  for(float idy = cutoff+1 ; idy >= cutoff-0.01 ; idy--){
    // std::vector<int> v_NCluster_start ; // List of all clusters in each event before making selection
    int NEvents = aSample.Get_NberOfEvents() ;
    for (int iE = 0 ; iE < NEvents; iE++){
      Event* pEvent           = aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;

      if(NClusters < 20) pEvent->Invalidate_ThisModule(ModuleNber) ; 
      
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

      TH2D* pTH2D_Evt_display = GiveMe_EvtDisplay(pEvent, ModuleNber , TAG) ;
      TGraphErrors* ptge_Evt  = TH2_to_TGE(pTH2D_Evt_display);
      TF1* ptf1_Evt_fit       = GiveMe_FitYwforSelection(ptge_Evt) ;
      
      // v_NCluster_start.push_back(NClusters) ;
      
      std::vector<Cluster*> v_newCluster ; // vector of the clusters kept => is going to become the reconstructed Event
      
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];
        
        int iX                = pCluster->Get_LeadingPad()->Get_iX() ;
        float fresidual       = ptge_Evt->GetPointY(iC) - ptf1_Evt_fit->Eval(iX) ;
        if(std::fabs(fresidual) < idy){
          Cluster* pClusnew   = pEvent->Get_Cluster_Copy(pCluster) ;
          v_newCluster.push_back(pClusnew) ;
        }
      }
      pEvent->Replace_Clusters_ForThisModule(v_newCluster, ModuleNber) ;

      // int nEvtPrint = 0 ;
      // if(idy == cutoff and v_NCluster_start[iE] - pEvent->Get_NberOfCluster() != 0 and nEvtPrint < 200){
      //   TH2D* pTH2D_Evt_displayF = GiveMe_EvtDisplay(pEvent, TAG) ;
      //   TGraphErrors* ptgr_EvtF  = TH2_to_TGE(pTH2D_Evt_displayF);
      //   TF1* ptf1_Evt_fitF       = new TF1("ptf1_Evt_fitF", "[0]*std::pow(x,2)+[1]*x+[2]", 0, 35) ;      
      //   ptf1_Evt_fitF->SetParameter(0, -0.0005) ;
      //   ptf1_Evt_fitF->SetParameter(1, -0.05) ;
      //   ptf1_Evt_fitF->SetParameter(2, 8) ;
      //   ptgr_EvtF->Fit(ptf1_Evt_fitF, "RQ") ;
      //   TCanvas* pCanvasEvt     = new TCanvas(TString("pCanvasEvt_" + pEvent->Get_EventNber()), TString("pCanvasEvt_" + pEvent->Get_EventNber()), 1200, 900) ;
      //   pCanvasEvt->cd() ;
      //   pTH2D_Evt_displayF->DrawClone("colz") ;
      //   ptgr_EvtF->DrawClone("same") ;
      //   ptf1_Evt_fitF->DrawClone("same") ;
      //   pCanvasEvt->SaveAs(TString("OUT_Tristan_pt412/z" + std::to_string(zdrift) + "/Cut_Events/Event_" + std::to_string(pEvent->Get_EventNber()) + ".png")) ;
      //   delete pCanvasEvt ;
      //   nEvtPrint ++ ;
      // }
      
      delete pTH2D_Evt_display ;
      delete ptge_Evt ;
      delete ptf1_Evt_fit ;
    }
  }
  std::cout << "Selection Track Alignement: Completed" << std::endl << std::endl ;
}
