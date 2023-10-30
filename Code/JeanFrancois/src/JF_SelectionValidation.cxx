#include "JeanFrancois/JF_SelectionValidation.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Sample.h"

#include "EvtModelTools/JFL_Selector.h"

#include "SampleTools/SampleUploader_Data.h"

#include "EvtModelTools/JeanFrancois_DrawOut_Status.h"

void JF_SelectionValidation()
{ 
  int ModuleNber = 0 ;
//
  
  std::string AnalyseName = "JF_SelectionValidation" ;
  std::string EventFile = "../Real_Data/z_360_275_412_02T_26_260_iter0.root" ;


  std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;
  std::string OUTDirName = "OUT_JeanFrancois/Dir_" + base_EventFile + "_" + AnalyseName + "/";

  std::cout << std::endl ;
  std::cout << " AnalyseName = " << AnalyseName << std::endl ;
  std::cout << std::endl ;
  std::cout << " EventFile               = " << EventFile          << std::endl ;
  std::cout << std::endl ;
  std::cout << " OUTDirName              = " << OUTDirName  << std::endl ;
  
//
  MakeMyDir(OUTDirName) ;
    
//Redirect Output
  std::cout << std::endl ;
  std::ostringstream aostringstream_OutPut_Analysis ;
  aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
  aostringstream_OutPut_Analysis << OUTDirName <<AnalyseName<< ".txt" ;
  
  std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
  
  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file

  
//-----------------------------------------------------------//
//Load Sample
  std::cout << std::endl ;
  std::cout << "*Load Sample " << EventFile << std::endl ;
  
  int Uploader = 2 ;
  int Data_to_Use = 3 ;
  Sample aSample ;
  SampleUploader_Data(Uploader,aSample, ModuleNber , EventFile , Data_to_Use );


//--------------------------------------------------------------------
//Draw out Initial Status
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"Start");


//--------------------------------------------------------------------
//Selection
  std::cout << std::endl ;
  std::cout << "*Do Selection " << std::endl ;
  
  JFL_Selector aJFL_Selector("AN_DESY21_Sample") ;

  aJFL_Selector.Reset_StatCounters() ;

  int iTem_Max = aJFL_Selector.NberOfSelections() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){   
  
    aJFL_Selector.Apply_ASelection(aSample, ModuleNber ,iTem) ;
    
    std::ostringstream aostringstream_TAG ;
    aostringstream_TAG << std::setiosflags(std::ios::fixed) ;
    aostringstream_TAG <<"Cut_" << iTem  ;
    aostringstream_TAG <<"_After_" << aJFL_Selector.Get_SelectionName(iTem)  ;
    std::string TAG = aostringstream_TAG.str() ; 
    
    JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,TAG);
    
  }

  aJFL_Selector.PrintStat();
  
//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

}
