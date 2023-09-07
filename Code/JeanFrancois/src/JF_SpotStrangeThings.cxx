#include "JeanFrancois/JF_SpotStrangeThings.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Sample.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"

#include "EvtModelTools/EvtModelTools_JFL_Histos.h"

#include "EvtModelTools/JFL_Selector.h"

#include "SampleTools/SampleUploader_Data.h"

#include "EvtModelTools/JFL_RecTools.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ChargeI.h"

#include "EvtModelTools/JeanFrancois_DrawOut_Status.h"


#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TVectorD.h"

void JF_SpotStrangeThings()
{ 
  int ModuleNber = 0 ;
//
  
  std::string AnalyseName = "JF_SpotStrangeThings" ;
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
  MyMakeDir(OUTDirName) ;
    
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
  


//Settings -------------------------------------------------------Set Geometry stuff
  int NberOfEvents = aSample.Get_NberOfEvents() ;

//  const  Model_ReadOutGeometry* pModel_ReadOutGeometry =  aSample.Get_Model_ReadOutGeometry() ;
//  double LY       = pModel_ReadOutGeometry->Get_LY()       ;
  
//   double vY_min_DiffPRF = -1.5 * LY *1.E2 ;
//   double vY_max_DiffPRF =  1.5 * LY *1.E2 ;


//-----------------------------------------------------------Hey Ho! Let's Go
//   TCanvas* pTCanvas =  0 ;
//   std::string Str_Out_PDF     = "BIDON" ; 
//   std::string Str_Out_PDF_Beg = "BIDON" ; 
//   std::string Str_Out_PDF_End = "BIDON" ; 

//
//   double Vmin = 0. ;
//   double Vmax = 0. ;


//--------------------------------------------------------------------
//Draw out Initial Status
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"InitialState");


//--------------------------------------------------------------------
//Selection
  std::cout << std::endl ;
  std::cout << "*Do Selection " << std::endl ;
  
  JFL_Selector aJFL_Selector("AN_DESY21_Sample") ;
  
  aJFL_Selector.ApplySelection(aSample, ModuleNber) ;
  
  aJFL_Selector.PrintStat();
  
//-------------------------------------------------------//DBG pad fit
  int iOpt_DBG_PadFit_A = 0 ;
  if (iOpt_DBG_PadFit_A==1){
    int KounterMax = 200 ;
    std::string OUTDIR_DBG_PadFit_A         = OUTDirName + "/DBG_PadFit_A/" ;
    std::string OUTDIR_DBG_PadFit_A_Cluster = OUTDIR_DBG_PadFit_A + "/Cluster/" ;
    MyMakeDir(OUTDIR_DBG_PadFit_A        ) ;
    MyMakeDir(OUTDIR_DBG_PadFit_A_Cluster) ;
    
    int Kounter = 0 ;
    for (int iE = 0 ; iE< NberOfEvents; iE++){
      Event* pEvent =  aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];

        int    NPads      = pCluster->Get_NberOfPads() ;
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);
          
          double DIFF  = pPad->Get_AMax_WF() - pPad->Get_AMax_FIT() ;
          if (std::fabs(DIFF)>40) {
            Kounter += 1 ;
            if(Kounter<=KounterMax){
              DrawOut_JF_WaveFormDisplay(pPad    ,OUTDIR_DBG_PadFit_A        ,"DBG_PadFit_A",1);
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_DBG_PadFit_A_Cluster,"DBG_PadFit_A");
            }
          }
          if (Kounter>KounterMax ) break ;          
        }
        if (Kounter>KounterMax ) break ;          
      }
      if (Kounter>KounterMax ) break ;          
    }
  }
  
//-------------------------------------------------------//DBG pad fit
  int iOpt_DBG_PadFit_B = 0 ;
  if (iOpt_DBG_PadFit_B==1){
    int KounterMax = 200 ;
    std::string OUTDIR_DBG_PadFit_B         = OUTDirName + "/DBG_PadFit_B/" ;
    std::string OUTDIR_DBG_PadFit_B_Cluster = OUTDIR_DBG_PadFit_B + "/Cluster/" ;
    MyMakeDir(OUTDIR_DBG_PadFit_B        ) ;
    MyMakeDir(OUTDIR_DBG_PadFit_B_Cluster) ;
    
    int Kounter = 0 ;
    for (int iE = 0 ; iE< NberOfEvents; iE++){
      Event* pEvent =  aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];

        int    NPads      = pCluster->Get_NberOfPads() ;
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);
          
          double DIFF  = pPad->Get_TMax_WF() - pPad->Get_TMax_FIT() ;
          if (std::fabs(DIFF)>40) {
            Kounter += 1 ;
            if(Kounter<=KounterMax){
              DrawOut_JF_WaveFormDisplay(pPad    ,OUTDIR_DBG_PadFit_B        ,"DBG_PadFit_B",2);
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_DBG_PadFit_B_Cluster,"DBG_PadFit_B");
            }
          }
          if (Kounter>KounterMax ) break ;          
        }
        if (Kounter>KounterMax ) break ;          
      }
      if (Kounter>KounterMax ) break ;          
    }
  }
  
//-------------------------------------------------------//DBG pad fit
  int iOpt_DBG_PadFit_C = 0 ;
  if (iOpt_DBG_PadFit_C==1){
    int KounterMax = 200 ;
    std::string OUTDIR_DBG_PadFit_C         = OUTDirName + "/DBG_PadFit_C/" ;
    std::string OUTDIR_DBG_PadFit_C_Cluster = OUTDIR_DBG_PadFit_C + "/Cluster/" ;
    MyMakeDir(OUTDIR_DBG_PadFit_C        ) ;
    MyMakeDir(OUTDIR_DBG_PadFit_C_Cluster) ;
    
    int Kounter = 0 ;
    for (int iE = 0 ; iE< NberOfEvents; iE++){
      Event* pEvent =  aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];

        int    NPads      = pCluster->Get_NberOfPads() ;
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);
          
          if (pPad->Get_TMax_WF_01() < 1 ){
            Kounter += 1 ;
            if(Kounter<=KounterMax){
              DrawOut_JF_WaveFormDisplay(pPad    ,OUTDIR_DBG_PadFit_C        ,"DBG_PadFit_C",2);
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_DBG_PadFit_C_Cluster,"DBG_PadFit_C");
            }
          }
          if (Kounter>KounterMax ) break ;          
        }
        if (Kounter>KounterMax ) break ;          
      }
      if (Kounter>KounterMax ) break ;          
    }
  }
  
//-------------------------------------------------------//DBG pad fit
  int iOpt_DBG_PadFit_D = 1 ;
  if (iOpt_DBG_PadFit_D==1){
    int KounterMax = 200 ;
    std::string OUTDIR_DBG_PadFit_D         = OUTDirName + "/DBG_PadFit_D/" ;
    std::string OUTDIR_DBG_PadFit_D_Cluster = OUTDIR_DBG_PadFit_D + "/Cluster/" ;
    MyMakeDir(OUTDIR_DBG_PadFit_D        ) ;
    MyMakeDir(OUTDIR_DBG_PadFit_D_Cluster) ;
    
    int Kounter = 0 ;
    for (int iE = 0 ; iE< NberOfEvents; iE++){
      Event* pEvent =  aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];

        int    NPads      = pCluster->Get_NberOfPads() ;
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);
          
          if (pPad->Get_AMax() < 100 ){
            Kounter += 1 ;
            if(Kounter<=KounterMax){
              DrawOut_JF_WaveFormDisplay(pPad    ,OUTDIR_DBG_PadFit_D        ,"DBG_PadFit_D",2);
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_DBG_PadFit_D_Cluster,"DBG_PadFit_D");
            }
          }
          if (Kounter>KounterMax ) break ;          
        }
        if (Kounter>KounterMax ) break ;          
      }
      if (Kounter>KounterMax ) break ;          
    }
  }
  
//--------------------------------------------------------------------
//Draw out Status After Selection
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"AfterSelection");


//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

}
