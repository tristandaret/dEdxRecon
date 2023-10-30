#include "JeanFrancois/DIA_DiagNoDiag.h"
#include "Misc/Util.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

#include "Fitters_Cluster/TheFitterCluster_PV0_PV1.h"
#include "Fitters_Cluster/TheFitterCluster_PV0_Diag.h"

#include "SignalShape/PRF_param.h"

#include "EvtModelTools/JFL_Do_ClusterFit.h"

#include "AnalysisTools/DoTracksReconstruction.h"

void DIA_DiagNoDiag()
{   
  std::string Tag = "DIA_DiagNoDiag" ;

  int intUploader         =  2 ; // DESY21
  int ModuleNber          =  0 ;
  int Data_to_Use         =  0 ; // Use ntuple Maxs
  int Verbose             =  1 ; // Verbose mode
  
  int NberOfEvent_Max = 10 ;  

//Loop on files
  std::string EventFile = "BIDON" ;
  int iFile_Min = 1 ;
  int iFile_Max = 4 ;
  for (int iFile = iFile_Min ; iFile< iFile_Max; iFile++){
    if (iFile==0)  EventFile = "../Real_Data/DIAG/phi_200_30_z460_ym60_iter0.root" ;
    if (iFile==1)  EventFile = "../Real_Data/DIAG/phi_200_30_z460_ym60_diag_iter0.root" ;
    if (iFile==2)  EventFile = "../Real_Data/DIAG/phi_200_30_z460_ym60_diag_iter0.root" ;
    if (iFile==3)  EventFile = "../Real_Data/DIAG/phi_200_30_z460_ym60_diag_iter0.root" ;
    
    int IsVertical = 0 ;
    if (iFile==0) IsVertical =  1 ; // Vertical cl.
    if (iFile==1) IsVertical =  1 ; // Diagonal cl. treated as Vertical cl.
    if (iFile==2) IsVertical =  0 ; // Diagonal cl. treated as Diagonal cl. + AngleRot =  M_PI/4.
    if (iFile==3) IsVertical = -1 ; // Diagonal cl. treated as Diagonal cl. + AngleRot = -M_PI/4.
    
//  Set output dir name
    std::string AnalyseName = Tag ;
    std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;

    std::ostringstream aostringstream_OutPutDir ;
    aostringstream_OutPutDir << "OUT_JeanFrancois/Dir_" << IsVertical << "_" << base_EventFile << "_" << AnalyseName << "/";
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
    std::cout << " DIA_DiagNoDiag " << std::endl ;
    std::cout << "      Tag          : " << Tag          << std::endl ;
    std::cout << "      EventFile    : " << EventFile    << std::endl ;
    std::cout << "      intUploader  : " << intUploader  << std::endl ;
    std::cout << "      ModuleNber   : " << ModuleNber   << std::endl ;
    std::cout << "      Data_to_Use  : " << Data_to_Use   << std::endl ;
    std::cout << "      IsVertical   : " << IsVertical   << std::endl ;
    std::cout << std::endl ;
    std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
    std::cout << std::endl ;


//  Events Uploader  
    Uploader* pUploader = GiveMe_Uploader(intUploader,EventFile) ;

//
    double vY_min_DiffPRF = -2.5 * 0.011280 *1.E2 ;
    double vY_max_DiffPRF =  2.5 * 0.011280 *1.E2 ;
    PRF_param  aPRF_param_rho_VS_YTrackYPad_FromTFile  ;
    TF1 * pTF1_FromTFile = 0;
    int PRF_exist = pUploader->Get_PRF_exist() ;
    if (PRF_exist==1){
      std::cout << "   PRF does exist! " << std::endl ;
      double L_Norm   = pUploader->Get_Norm () ;
      double L_a2     = pUploader->Get_a2   () ;
      double L_a4     = pUploader->Get_a4   () ;
      double L_b2     = pUploader->Get_b2   () ;
      double L_b4     = pUploader->Get_b4   () ;
      pTF1_FromTFile = new TF1("pTF1_FromTFile",aPRF_param_rho_VS_YTrackYPad_FromTFile ,vY_min_DiffPRF , vY_max_DiffPRF,5);
      pTF1_FromTFile->SetParameter(0, L_Norm );
      pTF1_FromTFile->SetParameter(1, L_a2   );
      pTF1_FromTFile->SetParameter(2, L_a4   );
      pTF1_FromTFile->SetParameter(3, L_b2   );
      pTF1_FromTFile->SetParameter(4, L_b4   );
      std::cout << "   Set PRF from TFile with  "  <<  std::endl;
      std::cout << "     L_Norm  "  << std::setw(20) << std::setprecision(6) <<  L_Norm    << std::endl;
      std::cout << "     L_a2    "  << std::setw(20) << std::setprecision(6) <<  L_a2      << std::endl;
      std::cout << "     L_a4    "  << std::setw(20) << std::setprecision(6) <<  L_a4      << std::endl;
      std::cout << "     L_b2    "  << std::setw(20) << std::setprecision(6) <<  L_b2      << std::endl;
      std::cout << "     L_b4    "  << std::setw(20) << std::setprecision(6) <<  L_b4      << std::endl;
   }else{
     std::cout << " PRF does NOT exist! " << std::endl ;
   }
   
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
          std::ostringstream aostringstream_I ; aostringstream_I << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_X ; aostringstream_X << std::setiosflags(std::ios::fixed) ;
          std::ostringstream aostringstream_Y ; aostringstream_Y << std::setiosflags(std::ios::fixed) ;
         
           aostringstream_I << "            " ;
           aostringstream_X << "            " ;
           aostringstream_Y << "            " ;
 
          int NberOfPads = pCluster->Get_NberOfPads() ;
          if (Verbose==1) std::cout << "   ->NberOfPads " << NberOfPads << std::endl ; 
          for (int iPad = 0; iPad < NberOfPads; iPad++) {
            const Pad* pPad = pCluster->Get_Pad(iPad) ;
            aostringstream_I << std::setw(3) << std::setprecision(0) << iPad ;
            aostringstream_X << std::setw(3) << std::setprecision(0) << pPad->Get_iX() ;
            aostringstream_Y << std::setw(3) << std::setprecision(0) << pPad->Get_iY() ;
          }
          if (Verbose==1)std::cout << aostringstream_I.str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_X.str() << std::endl ;
          if (Verbose==1)std::cout << aostringstream_Y.str() << std::endl ;
          
//        Clusters Fit
          if (Verbose==1) 
          std::cout << "   ->Avant Cluster fit  "  
                    << " XTrack " << pCluster->Get_XTrack ()  
                    << " YTrack " << pCluster->Get_YTrack ()  
                    << std::endl ;
          int Kounter_Fit     = 0 ;          
          int Kounter_Failure = 0 ;  
          
          if (IsVertical==1){        
            TheFitterCluster_PV0_PV1 aTheFitterCluster_PV0_PV1("Minuit") ;
            Do_ClusterFit_PV0_PV1_Cluster(pCluster,ModuleNber, pTF1_FromTFile, Kounter_Fit, Kounter_Failure, aTheFitterCluster_PV0_PV1) ;
          }
//
          if (IsVertical==0||IsVertical==-1){   
            double  AngleRot = M_PI/4. ;     
            if (IsVertical==-1) AngleRot = -M_PI/4. ;   
            TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit") ;
            Do_ClusterFit_PV0_Diag_Cluster(AngleRot,pCluster,ModuleNber, pTF1_FromTFile, Kounter_Fit, Kounter_Failure, aTheFitterCluster_PV0_Diag) ;
          }

          if (Verbose==1) 
          std::cout << "   ->Apres Cluster fit  "  
                    << " XTrack " << pCluster->Get_XTrack ()  
                    << " YTrack " << pCluster->Get_YTrack ()  
                    << std::endl ;
        }

//      Track fit   
        int NberOfParameters = 3 ;      
        TheFitterTrack aTheFitterTrack("Minuit",NberOfParameters) ;
        int FitResult = DoTracksReconstruction_Event(
                                                     aTheFitterTrack , 
                                                     pEvent , ModuleNber ,
                                                     NberOfParameters 
                                                    ) ; 

        const  Track* pTrack = pEvent->GiveMe_Track_ForThisModule(ModuleNber) ;
        std::cout << "  Track Fit  " << std::endl ;  
        std::cout << "     Get_EventNber        " << pTrack->Get_EventNber ()      << std::endl ;  
        std::cout << "     Get_EntryNber        " << pTrack->Get_EntryNber ()      << std::endl ;  
        std::cout << "     Get_ModuleNber       " << pTrack->Get_ModuleNber()      << std::endl ;  
        std::cout << "     GetNberOfParameters  " << pTrack->GetNberOfParameters() << std::endl ;  
        int iPar_Max = pTrack->GetNberOfParameters() ;
        for (int iPar = 0 ; iPar< iPar_Max; iPar++){
          std::cout << std::setw(3)  << std::setprecision(0) << iPar
                    << std::setw(4)                          << pTrack->Get_ParameterName (iPar)
                    << std::setw(20) << std::setprecision(6) << pTrack->Get_ParameterValue(iPar)
                    << std::setw(20) << std::setprecision(6) << pTrack->Get_ParameterError(iPar)
                    << std::endl ;  
        }
        
        
        
      }
    }
//
    std::cout.rdbuf(coutbuf); // Reset to standard output again


  }
}
