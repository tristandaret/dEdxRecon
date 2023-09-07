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
std::vector<int> GetSelection120Cuts(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF)
{
  std::vector<int>                        v_TCut ;

  std::vector<TH1F*>                      v_pTH1_TLeadCut ;
  for (int iMod = 0 ; iMod < 8 ; iMod++){
    TH1F* pTH1_TLead_Cut                = new TH1F(Form("pTH1_TLeadAfter_%i", iMod),  Form("T_{leading} after selection and truncation: Module %i", iMod), 511,  -0.5 , 510.5 );
    v_pTH1_TLeadCut.                      push_back(pTH1_TLead_Cut);
  }

  int NEvent                            = pUploader->Get_NberOfEvent() ;
  int TLow                              = 0 ;
  int THigh                             = 0 ;
    for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
      Event*  pEvent                    = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
      if (!pEvent)                        continue ;
      if (pEvent->IsValid() == 1){ // Valid events only
        //  Loop On Modules
        int NberOfModule                = pEvent->Get_NberOfModule() ;
        for (int iMod = 0; iMod < NberOfModule; iMod++){
          Module* pModule               = pEvent->Get_Module_InArray(iMod) ;
          if (pEvent->Validity_ForThisModule(iMod) == 0) continue ; // Valid modules only
          // Loop On Clusters
          int NClusters                 = pModule->Get_NberOfCluster() ;
          for (int iC = 0; iC < NClusters; iC++){
            Cluster* pCluster           = pModule->Get_Cluster(iC) ;
            double TLeading             = pCluster->Get_TMaxLeading() ;
            if (TLeading>5.) v_pTH1_TLeadCut[iMod]->Fill( TLeading ) ;
          }
        }
      }
      delete pEvent ;
    }
    std::vector<double> v_MaxTLead ;
    for (int iMod = 0; iMod < 8; iMod++){
      std::cout << "module " << iMod << " TLeading max = " << v_pTH1_TLeadCut[iMod]->GetMaximumBin() - 1 << std::endl ;
      v_MaxTLead.                         push_back(v_pTH1_TLeadCut[iMod]->GetMaximum()) ;
    }

    int module_max                      = std::max_element(v_MaxTLead.begin(),v_MaxTLead.end()) - v_MaxTLead.begin() ; // module with biggest peak
    int nbin_max                        = v_pTH1_TLeadCut[module_max]->GetMaximumBin() ;                               // bin of the highest peak
    int iDeltaBinP                      = 10 ;                                                                         // initialization of increments
    int iDeltaBinM                      = 10 ;
    float y_init                        = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max) ;                       // value of the peak
    float y_0                           = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max) ;                       // value of the peak
    float y_minus1                      = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max-10) ;                    // value of the previous bin
    float y_plus1                       = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max+10) ;                    // value of the next bin
    // Find the lower boundary
    while(y_minus1 - y_0 < y_init/2500 and y_minus1 > 0.001*y_init){
      iDeltaBinM++ ;
      y_0                               = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max-iDeltaBinM) ;
      y_minus1                          = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max-iDeltaBinM-1) ;
    }
    // Find the higher boundary
    y_0                                 = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max) ;                       // reboot y_0 to maximum value peak
    while(y_plus1 - y_0 < y_init/2500 and y_minus1 > 0.001*y_init){
      iDeltaBinP++ ;
      y_0                               = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max+iDeltaBinP) ;
      y_plus1                           = v_pTH1_TLeadCut[module_max]->GetBinContent(nbin_max+iDeltaBinP+1) ;
    }

    TLow                                = v_pTH1_TLeadCut[std::max_element(v_MaxTLead.begin(),v_MaxTLead.end()) - v_MaxTLead.begin()]->GetMaximumBin() - 1 - iDeltaBinM ;
    THigh                               = v_pTH1_TLeadCut[std::max_element(v_MaxTLead.begin(),v_MaxTLead.end()) - v_MaxTLead.begin()]->GetMaximumBin() - 1 + iDeltaBinP ;
    v_TCut.                               push_back(TLow) ;
    v_TCut.                               push_back(THigh) ;

    std::cout << "Module whose bin is the most filled: " << std::max_element(v_MaxTLead.begin(),v_MaxTLead.end()) - v_MaxTLead.begin() << std::endl ; std::cout << std::endl ;
    v_MaxTLead.                           clear() ;

  int nmodmax                           = v_pTH1_TLeadCut.size() ;
  for (int iMod = 0 ; iMod < nmodmax ; iMod++){
    delete                                v_pTH1_TLeadCut[iMod] ;
    v_pTH1_TLeadCut[iMod]               = 0 ;
  }
  v_pTH1_TLeadCut.                        clear() ;

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
