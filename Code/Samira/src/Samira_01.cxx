#include "Samira/Samira_01.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Sample.h"

#include "EvtModelTools/EvtModelTools_Histos.h"

#include "SampleTools/SampleUploader_Data.h"

void Samira_01()
{
  std::cout << std::endl ;
  std::cout << " Samira_01 " << std::endl ;
  std::cout << std::endl ;
  
  int ModuleNber = 0 ;
  
  int iFile_Max = 9 ;
      iFile_Max = 1 ;
  for (int iFile = 0 ; iFile< iFile_Max; iFile++){

    std::string EventFile = "BIDON" ;
    if (iFile==0) EventFile = "../Real_Data/z_360_275_412_02T_26_60_iter0.root"  ;
    if (iFile==1) EventFile = "../Real_Data/z_360_275_412_02T_26_160_iter0.root" ;
    if (iFile==2) EventFile = "../Real_Data/z_360_275_412_02T_26_260_iter0.root" ;
    if (iFile==3) EventFile = "../Real_Data/z_360_275_412_02T_26_360_iter0.root" ;
    if (iFile==4) EventFile = "../Real_Data/z_360_275_412_02T_26_460_iter0.root" ;
    if (iFile==5) EventFile = "../Real_Data/z_360_275_412_02T_26_560_iter0.root" ;
    if (iFile==6) EventFile = "../Real_Data/z_360_275_412_02T_26_660_iter0.root" ;
    if (iFile==7) EventFile = "../Real_Data/z_360_275_412_02T_26_760_iter0.root" ;
    if (iFile==8) EventFile = "../Real_Data/z_360_275_412_02T_26_860_iter0.root" ;

    std::cout << " EventFile:    " << EventFile << std::endl ;

    std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;

//
    std::string OUTDirName = "OUT_Samira/Dir_" + base_EventFile + "/";
    MyMakeDir(OUTDirName) ;
    std::cout << " OUTDirName:    " << OUTDirName << std::endl ;

//
    int NberOfEvents = 0 ;

//
    int Uploader = 2 ;
    int Data_to_Use = 3 ;
    Sample aSample ;
    SampleUploader_Data(Uploader,aSample, ModuleNber , EventFile , Data_to_Use );
    
    std::cout << " NberOfEvent: " << aSample.Get_NberOfEvents() << std::endl ;
    std::cout << std::endl ;

//------------------------------------------// Draw out displays
//  Loop on events
    int Kounter_DrawOut_EventDisplay     =  0 ;
    int Kounter_DrawOut_EventDisplay_Max =  5 ;
    
    int Kounter_DrawOut_ClusterWFDisplay     =  0 ;
    int Kounter_DrawOut_ClusterWFDisplay_Max =  5 ;
    
    int Kounter_DrawOut_WaveFormDisplay     =  0 ;
    int Kounter_DrawOut_WaveFormDisplay_Max =  5 ;
    
//  Loop on events
    NberOfEvents = aSample.Get_NberOfEvents() ;
    for (int iE = 0 ; iE< NberOfEvents; iE++){
      Event* pEvent =  aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

//    Draw out event display   
      if (Kounter_DrawOut_EventDisplay < Kounter_DrawOut_EventDisplay_Max ){
        Kounter_DrawOut_EventDisplay += 1 ;
        DrawOut_EventDisplay(pEvent, ModuleNber ,OUTDirName,"Test1") ;
      }

//    Loop on clusters in current event
      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];

//      Draw out cluster display   
        if (Kounter_DrawOut_ClusterWFDisplay < Kounter_DrawOut_ClusterWFDisplay_Max ){
          Kounter_DrawOut_ClusterWFDisplay += 1 ;
          DrawOut_ClusterWFDisplay(pCluster,OUTDirName,"Test1") ;
         }
        
//      Loop on pads in current cluster
        int NPads = pCluster->Get_NberOfPads();
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);
         
//        Draw out WF display   
          if (Kounter_DrawOut_WaveFormDisplay < Kounter_DrawOut_WaveFormDisplay_Max ){
            Kounter_DrawOut_WaveFormDisplay += 1 ;
            DrawOut_WaveFormDisplay(pPad,OUTDirName,"Test1");
          }
          
          
        }
      }
    }    

  }
  
  
  
}
