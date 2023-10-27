#include "Tristan_DESY21/DESY21_EventsWF.h"

#include "Misc/Util.h"
#include <numeric>

// Draw Waveforms and Event Display if DeltaT < -100 bins
void DESY21_EventsWF(Sample& aSample, const int& ModuleNber , const std::string& TAG, std::string Stage, const int& pt, const int& zDrift)
{
  // gErrorIgnoreLevel           = kPrint ;
  // gErrorIgnoreLevel           = kWarning ;
  // Directory for Event Displays
  std::ostringstream dirEvtDisp ;
  dirEvtDisp << std::setiosflags(std::ios::fixed) ;
  dirEvtDisp << "OUT_pt" << pt << "/z" << zDrift << "/" << Stage << "/Event_Displays/" ;
  std::string OUTDIREvt       = dirEvtDisp.str() ;

int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < 100 ; iE++){
    Event* pEvent           =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    DrawOut_EventDisplay(pEvent, ModuleNber , OUTDIREvt, TAG) ;
  }

  //Directory for Waveforms
  std::ostringstream dirWFstring ;
  dirWFstring << std::setiosflags(std::ios::fixed) ;
  dirWFstring << "OUT_pt" << pt << "/z" << zDrift << "/" << Stage << "/Waveforms/" ;
  std::string OUTDIRWF        =  dirWFstring.str() ;  

  std::vector<int> vEvent(NEvents, 0) ;
  // int nDeltaTnegtotal         = 0 ;
  // int nDeltaTpostotal         = 0 ;
  // int isCLusNeg               = 0 ;
  // int isClusPos               = 0 ;
  // int isEvtBoth               = 0 ;
  // for (int iE = 0 ; iE < NEvents ; iE++){
  //   Event* pEvent             =  aSample.Get_Event(iE) ;
  //   // int nDeltaTneg            = 0 ;
  //   // int nDeltaTpos            = 0 ;
  //   if (pEvent->Validity_ForThisModule(ModuleNber)     == 0) continue ;
    
  //   int NClusters             = pEvent->Get_NberOfCluster() ;
  //   isCLusNeg                 = 0 ;
  //   isClusPos                 = 0 ;
  //   for (int iC = 0 ; iC< NClusters ; iC++){
  //     Cluster* pCluster       = pEvent->Get_Cluster(iC) ;
  //     int TLeading            = pCluster->Get_TMaxLeading() ;

  //     int NPads               = pCluster->Get_NberOfPads() ;
  //     if(NPads >= 2){
  //       const Pad* pPad       = pCluster->Get_Pad(NPads-2) ; // get 2nd highest pad, which should be a neighbour  
  //       int Tneigh            = pPad->Get_TMax() ;
  //       int DeltaT            = Tneigh - TLeading ;
  //       if (DeltaT < -100){
  //         isCLusNeg           = 1 ;
  //         // DrawOut_ClusterWFDisplay(pCluster, OUTDIRWF, TAG) ;
  //         // nDeltaTneg         += 1 ;
  //         // nDeltaTnegtotal    += 1 ;
  //         vEvent[iE]          = 1 ;
  //       }
  //       if (DeltaT > 100){
  //         isClusPos           = 1 ;
  //         // DrawOut_ClusterWFDisplay(pCluster, OUTDIRWF, TAG) ;
  //         // nDeltaTpos         += 1 ;
  //         // nDeltaTpostotal    += 1 ;
  //         vEvent[iE]          = 1 ;
  //       }
  //     }       
  //   }
  //   if(isCLusNeg == 1 and isClusPos == 1) isEvtBoth += 1 ;
  //   if (vEvent[iE] != 0) DrawOut_EventDisplay(pEvent, OUTDIREvt, TAG) ;
  // } 
  // // std::cout << "Drift distance:    " << zDrift << " mm" << std::endl ;
  // // int nEvent = accumulate(vEvent.begin(), vEvent.end(), 0) ;
  // // std::cout << "Events neg or pos: " << nEvent << std::endl ;
  // // std::cout << "Events neg & pos:  " << isEvtBoth << std::endl ;
} 
