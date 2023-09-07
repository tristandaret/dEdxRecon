#include "EvtModelTools/JFL_RecTools.h"
#include "Misc/Util.h"

#include "EvtModel/Cluster.h"

void  Set_Starting_YT_Sample (Sample& aSample, const int& ModuleNber, const int& StartFrom)
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;

  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   
    Set_Starting_YT_Event(pEvent,ModuleNber,StartFrom) ;
  }
}

void  Set_Starting_YT_Event (Event* pEvent, const int& ModuleNber, const int& StartFrom)
{
  if ( StartFrom == 0 || StartFrom == 1 ){
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      if ( StartFrom == 0 ) pCluster->Set_YTrack( pCluster->Get_YWeight () ) ;
      if ( StartFrom == 1 ) pCluster->Set_YTrack( pCluster->Get_YLeading() ) ;
    }
  }
  if ( StartFrom == 2 ){
    const Track* pTrack = pEvent->GiveMe_Track_ForThisModule(ModuleNber) ; 
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      double ThePosition_X = (pCluster->Get_Pad(0))->Get_XPad();
      pCluster->Set_YTrack(  pTrack->Y_Position(ThePosition_X)  )  ;
    }
  }
}


