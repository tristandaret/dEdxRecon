#include "JeanFrancois/DIA_RC_Gain_Dump.h"
#include "Misc/Util.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

#include "Fitters_Cluster/TheFitterCluster_PV0_PV1.h"
#include "Fitters_Cluster/TheFitterCluster_PV0_Diag.h"

#include "SignalShape/PRF_param.h"

#include "EvtModelTools/JFL_Do_ClusterFit.h"

#include "AnalysisTools/DoTracksReconstruction.h"

#include "SampleTools/ReadRCmap.h"
#include "SampleTools/ReadGainmap.h"

void DIA_RC_Gain_Dump()
{   
  
  std::string Tag = "DIA_RC_Gain_Dump" ;

  int intUploader         =  2 ; // DESY21
  int ModuleNber          =  0 ;
  int Data_to_Use         =  0 ; // Use ntuple Maxs
  int Verbose             =  1 ; // Verbose mode
  
  int NberOfEvent_Max = 2 ;  

//Loop on files
  std::string EventFile = "BIDON" ;
  int iFile_Min = 0 ;
  int iFile_Max = 2 ;
  for (int iFile = iFile_Min ; iFile< iFile_Max; iFile++){
    if (iFile==0)  EventFile = "../Real_Data/DIAG/phi_200_30_z460_ym60_diag_iter0.root" ;
    
//  Set output dir name
    std::string AnalyseName = Tag ;
    std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;

    std::ostringstream aostringstream_OutPutDir ;
    aostringstream_OutPutDir << "OUT_JeanFrancois/Dir_" << base_EventFile << "_" << AnalyseName << "/";
    std::string OUTDirName = aostringstream_OutPutDir.str() ;

    MakeMyDir(OUTDirName) ;
    std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
    std::cout << std::endl ;

//  Redirect Output
    std::cout << std::endl ;
    std::ostringstream aostringstream_OutPut_Analysis ;
    aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
    aostringstream_OutPut_Analysis << OUTDirName <<AnalyseName<< ".txt" ;
    
    std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
    
    std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
    std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
    std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file
    
//
    std::cout << std::endl ;
    std::cout << " DIA_RC_Gain_Dump " << std::endl ;
    std::cout << "      Tag          : " << Tag          << std::endl ;
    std::cout << "      EventFile    : " << EventFile    << std::endl ;
    std::cout << "      intUploader  : " << intUploader  << std::endl ;
    std::cout << "      ModuleNber   : " << ModuleNber   << std::endl ;
    std::cout << "      Data_to_Use  : " << Data_to_Use   << std::endl ;
    std::cout << std::endl ;
    std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
    std::cout << std::endl ;


//
    ReadRCmap aReadRCmap_ERAM01("ERAM01"); ReadGainmap aReadGainmap_ERAM01("ERAM01");
    ReadRCmap aReadRCmap_ERAM02("ERAM02"); ReadGainmap aReadGainmap_ERAM02("ERAM02");
    ReadRCmap aReadRCmap_ERAM03("ERAM03"); ReadGainmap aReadGainmap_ERAM03("ERAM03");
    ReadRCmap aReadRCmap_ERAM07("ERAM07"); ReadGainmap aReadGainmap_ERAM07("ERAM07");
    ReadRCmap aReadRCmap_ERAM09("ERAM09"); ReadGainmap aReadGainmap_ERAM09("ERAM09");
    ReadRCmap aReadRCmap_ERAM10("ERAM10"); ReadGainmap aReadGainmap_ERAM10("ERAM10");
    ReadRCmap aReadRCmap_ERAM11("ERAM11"); ReadGainmap aReadGainmap_ERAM11("ERAM11");
    ReadRCmap aReadRCmap_ERAM13("ERAM13"); ReadGainmap aReadGainmap_ERAM13("ERAM13");
    ReadRCmap aReadRCmap_ERAM12("ERAM12"); ReadGainmap aReadGainmap_ERAM12("ERAM12");
    ReadRCmap aReadRCmap_ERAM14("ERAM14"); ReadGainmap aReadGainmap_ERAM14("ERAM14");
    ReadRCmap aReadRCmap_ERAM15("ERAM15"); ReadGainmap aReadGainmap_ERAM15("ERAM15");
    ReadRCmap aReadRCmap_ERAM16("ERAM16"); ReadGainmap aReadGainmap_ERAM16("ERAM16");
    ReadRCmap aReadRCmap_ERAM17("ERAM17"); ReadGainmap aReadGainmap_ERAM17("ERAM17");
    ReadRCmap aReadRCmap_ERAM18("ERAM18"); ReadGainmap aReadGainmap_ERAM18("ERAM18");
    ReadRCmap aReadRCmap_ERAM19("ERAM19"); ReadGainmap aReadGainmap_ERAM19("ERAM19");
    ReadRCmap aReadRCmap_ERAM20("ERAM20"); ReadGainmap aReadGainmap_ERAM20("ERAM20");
    ReadRCmap aReadRCmap_ERAM21("ERAM21"); ReadGainmap aReadGainmap_ERAM21("ERAM21");
    ReadRCmap aReadRCmap_ERAM23("ERAM23"); ReadGainmap aReadGainmap_ERAM23("ERAM23");
    ReadRCmap aReadRCmap_ERAM24("ERAM24"); ReadGainmap aReadGainmap_ERAM24("ERAM24");
    ReadRCmap aReadRCmap_ERAM26("ERAM26"); ReadGainmap aReadGainmap_ERAM26("ERAM26");
    ReadRCmap aReadRCmap_ERAM28("ERAM28"); ReadGainmap aReadGainmap_ERAM28("ERAM28");
    ReadRCmap aReadRCmap_ERAM29("ERAM29"); ReadGainmap aReadGainmap_ERAM29("ERAM29");
    ReadRCmap aReadRCmap_ERAM30("ERAM30"); ReadGainmap aReadGainmap_ERAM30("ERAM30");
  
//  Events Uploader  
    Uploader* pUploader = GiveMe_Uploader(intUploader,EventFile) ;

//  Loop on events
    int NberOfEvent = pUploader->Get_NberOfEvent() ;
    if (Verbose==1) std::cout << "      NberOfEvent on file =  " << NberOfEvent << std::endl ;
    if ( NberOfEvent>NberOfEvent_Max )  NberOfEvent = NberOfEvent_Max ;
    if (Verbose==1) std::cout << "      NberOfEvent set to  =  " << NberOfEvent << std::endl ;

 
    for (int iEvent = 0; iEvent < NberOfEvent; iEvent++) {
      if (Verbose==1) std::cout << "============================================================"<< std::endl ; 
      if (Verbose==1) std::cout << "->iEvent " << iEvent << std::endl ; 

//
      Event*  pEvent = pUploader->GiveMe_Event( iEvent, ModuleNber, Data_to_Use );
      if (pEvent==0) {
        std::cout << " DIA_DAVID_01 "
                  << " pEvent==0 "
                  << " for iEvent " << iEvent
                  << std::endl; 
      }
      
      if (!pEvent) abort() ;

//
      int NberOfModule = pEvent->Get_NberOfModule() ;
      if (Verbose==1) std::cout << " ->NberOfModule " << NberOfModule << std::endl ; 
      
//    Loop On Modules
      for (int iModule = 0; iModule < NberOfModule; iModule++) {
        Module* pModule = pEvent->Get_Module_InArray(iModule) ;
        
        int ModuleNber = pModule->Get_ModuleNber() ;
        if (Verbose==1) std::cout << "  ->ModuleNber " << ModuleNber << std::endl ; 

//      Loop On Clusters
        int NClusters = pModule->Get_NberOfCluster() ;
        if (Verbose==1) std::cout << "  ->NClusters " << NClusters << std::endl ; 
        for (int iCluster = 0; iCluster < NClusters; iCluster++) {
          Cluster* pCluster = pModule->Get_Cluster(iCluster) ;
          if (Verbose==1) std::cout << "   ->iCluster " << iCluster << std::endl ; 

//        Loop On Pads
          std::ostringstream aostringstream_iPad ; aostringstream_iPad << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_iX   ; aostringstream_iX   << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_iY   ; aostringstream_iY   << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_RC   ; aostringstream_RC   << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_Gain ; aostringstream_Gain << std::setiosflags(std::ios::fixed) ;
         
           aostringstream_iPad << "       iPad " ;
           aostringstream_iX   << "         iX " ;
           aostringstream_iY   << "         iY " ;
           aostringstream_RC   << "         RC " ;
           aostringstream_Gain << "       Gain " ;
 
          int NberOfPads = pCluster->Get_NberOfPads() ;
          if (Verbose==1) std::cout << "   ->NberOfPads " << NberOfPads << std::endl ; 
          for (int iPad = 0; iPad < NberOfPads; iPad++) {
            const Pad* pPad = pCluster->Get_Pad(iPad) ;
            aostringstream_iPad << std::setw(8) << std::setprecision(0) << iPad ;
            aostringstream_iX   << std::setw(8) << std::setprecision(0) << pPad->Get_iX() ;
            aostringstream_iY   << std::setw(8) << std::setprecision(0) << pPad->Get_iY() ;

            int StatusRC = 0 ;
            aostringstream_RC   << std::setw(8) << std::setprecision(2) << aReadRCmap_ERAM01.GetData(pPad->Get_iX(),pPad->Get_iY(), StatusRC) ;

            int StatusGain = 0 ;
            aostringstream_Gain << std::setw(8) << std::setprecision(1) << aReadGainmap_ERAM01.GetData(pPad->Get_iX(),pPad->Get_iY(), StatusGain) ;

          }
          if (Verbose==1)std::cout << aostringstream_iPad.str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_iX  .str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_iY  .str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_RC  .str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_Gain.str() << std::endl ;
          
        }
      }
    }
//
    std::cout.rdbuf(coutbuf); // Reset to standard output again


  }
}
