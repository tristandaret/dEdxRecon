#include "AnalysisTools/DoTracksReconstruction.h"

#include "EvtModel/Track.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Event.h"


void DoTracksReconstruction(
                            Sample& aSample, const int& ModuleNber,
                            const int& NberOfParameters
){
  
  TheFitterTrack aTheFitterTrack("Minuit",NberOfParameters) ;

  int Kounter_Fit     = 0 ; 
  int Kounter_Failure = 0 ; 

  int NberOfEvents = aSample.Get_NberOfEvents();
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; 
      
    int FitResult = DoTracksReconstruction_Event(
                                                 aTheFitterTrack , 
                                                 pEvent , ModuleNber ,
                                                 NberOfParameters 
                                                ) ; 

    Kounter_Fit += 1 ;
    if (FitResult!=0) Kounter_Failure += 1 ;
    
  }
  std::cout << std::endl ;
  std::cout << " DoTracksReconstruction " << std::endl ;
  std::cout << "   Nber of Fits        " << std::setw(20) << Kounter_Fit     << std::endl ;
  std::cout << "   Nber of Failed Fits " << std::setw(20) << Kounter_Failure 
     << " ( " 
     << std::setw(10) << std::setprecision(4) 
     << 100.* double (Kounter_Failure)/double (Kounter_Fit) 
     << " % ) "
     << std::endl ;

}

int DoTracksReconstruction_Event(
                            TheFitterTrack& aTheFitterTrack , 
                            Event* pEvent, const int& ModuleNber ,
                            const int& NberOfParameters  
){
  
  Track* pTrack_ForEvent = 0 ;
  pEvent->Set_Track_ForThisModule(pTrack_ForEvent,ModuleNber) ;
 
  pTrack_ForEvent = pEvent->GiveMe_AnUnfittedTrack_ForThisModule(ModuleNber) ;
      
  aTheFitterTrack.Set_Track(pTrack_ForEvent) ;
  
  int ToBeReturned = aTheFitterTrack.DoMinimisation() ;
 
  pEvent->Set_Track_ForThisModule(pTrack_ForEvent,ModuleNber) ;
  
//pTrack_ForEvent->DumpRec();
  
  return ToBeReturned ;
}
