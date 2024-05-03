#include "EvtModelTools_TD_Selections.h"
#include "EvtModelTools_TD_Histos.h"
#include "EvtModelTools_Histos.h"
#include "Util.h"


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
std::vector<int> Comp120_Cut(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF)
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
    // if(nMod < 4){ delete pEvent; continue;}
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
