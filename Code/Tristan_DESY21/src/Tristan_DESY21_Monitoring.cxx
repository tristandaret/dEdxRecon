#include "Tristan_DESY21/Tristan_DESY21_Monitoring.h"
#include "Tristan_DESY21/Tristan_DESY21_DrawOut_Status.h"
#include "Tristan_DESY21/Tristan_DESY21_DrawOut_PRF.h"
#include "Tristan_DESY21/Tristan_DESY21_DrawOut_Residuals.h"
#include "Tristan_DESY21/Tristan_DESY21_Fits.h"
#include "Tristan_DESY21/Tristan_DESY21_EventsWF.h"

#include "SignalShape/SignalTime_Exact_RhoPRF.h"
#include "SignalShape/SignalTime_Exact_AlphaPRF.h"
#include "SignalShape/SignalTime_Global_Exact.h"

#include "AnalysisTools/DoTracksReconstruction.h"

#include "EvtModelTools/JFL_Selections.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "SampleTools/SampleUploader_Data.h"

void Tristan_DESY21_Monitoring()
{
  int ModuleNber = 0 ;
  // Residuals and RC from each DD made in DrawOut_Residuals
  TGraphErrors* ptge_std_mean   = new TGraphErrors() ;
  TGraphErrors* ptge_bias_mean  = new TGraphErrors() ;
  TGraphErrors* ptge_RC         = new TGraphErrors() ;

  // PRF Stage
  int nPV = 3 ;
  int nw  = 3 ;
  for(int iPV = 0  ; iPV < nPV ; iPV+= 2){
    for(int iw = 0 ; iw  < nw ; iw+= 2){
      if(iPV == 0 and iw == 2) continue ;
      if(iPV == 2 and iw == 0) continue ;

      std::string Procedure = std::string("PV" + std::to_string(iPV)) ;
      std::string wdrift    = std::string("_w" + std::to_string(iw)) ;
      std::string Stage     = std::string(Procedure + wdrift) ;

      // Peaking time
      int pt = 412 ;

      // Iterate on drifting distance (9 files)
      for(int DD = 0 ; DD <= 8 ; DD++){
        int zdrift = 100*DD+60 ;

        // Indicate output directory
        std::ostringstream dirstring ;
        dirstring << std::setiosflags(std::ios::fixed) ;
        dirstring << "OUT_Tristan_pt" << pt << "/z" << zdrift << "/" << Stage << "/" ;
        std::string OUTDIR =  dirstring.str() ;

        // Indicate TAG (peaking time - drift distance - stage)
        std::ostringstream tagstring ;
        tagstring << std::setiosflags(std::ios::fixed) ;
        tagstring << "pt" << pt << "_z" << zdrift << " _" << Stage ;
        std::string TAG =  tagstring.str() ;

        // Load PRF pre-existing file
        std::ostringstream prffilestr ;
        prffilestr << std::setiosflags(std::ios::fixed) ;
        if(wdrift == "_w2") prffilestr << "Data_SignalTime/LoadRC_400ns_DD" << DD+1 << "97.46_Desy21_ZZ" << zdrift << ".00.csv" ; 
        if(wdrift == "_w1") prffilestr << "Data_SignalTime/LoadRC_400ns_DD_" << zdrift << ".00.csv" ; 
        if(wdrift == "_w0") prffilestr << "Data_SignalTime/LoadRC_400ns_DD_00.00.csv" ; 
        std::string prfFILEstring = prffilestr.str() ;
        SignalTime_Global_Exact     aSignalTime_Global_Exact(prfFILEstring) ;
        SignalTime_Exact_RhoPRF        aSignalTime_Exact_RhoPRF(&aSignalTime_Global_Exact) ;  // PV1
        SignalTime_Exact_AlphaPRF       aSignalTime_Exact_AlphaPRF(&aSignalTime_Global_Exact) ; // PV2

        // Load Sample
        std::ostringstream samplestr ;
        samplestr << std::setiosflags(std::ios::fixed) ;
        samplestr << "Data_Real/z_360_275_" << pt << "_02T_26_" << zdrift << "_iter0.root" ;
        std::string SAMPname =  samplestr.str() ;

        int Uploader = 2 ;
        int Data_to_Use = 3 ;
        Sample aSample ;
        SampleUploader_Data(Uploader,aSample, ModuleNber , SAMPname , Data_to_Use );
    
        // Selection of suitable clusters & events
        Tristan_DESY21_DrawOut_Status        (aSample, ModuleNber ,OUTDIR, std::string(TAG + "_0Raw"), Stage, iPV, 0, 0) ;
        Selection_TD_TrackAlignement  (aSample, ModuleNber , TAG, 0.5, zdrift) ;
        Tristan_DESY21_DrawOut_Status        (aSample, ModuleNber ,OUTDIR, std::string(TAG + "_5Align"), Stage, iPV, 0, 0) ;
        if(Procedure == "PV0")  Selection_JFL_B(aSample, ModuleNber ) ;
        if(Procedure != "PV0")  Selection_JFL_A(aSample, ModuleNber ) ;

        // Initialization of parameters of fit 
        int NIter       = 8 ;
        // PV0 parameters
        double a2       = -0.87 ; 
        double a4       = 0.24 ; 
        double b2       = 0.73 ; 
        double b4       = 0.14 ;
        double Norm     = 0.74 ; 
        // PV2 parameters
        double RC       = 90 ; 
        double err_RC   = 0 ; 

        // Iterate the PRF & Draw controls plots and PRF plots
        for(int iIter = 0 ; iIter <= NIter ; iIter++){
          std::cout << std::endl << "Stage " << Stage << " | Drift distance " << zdrift << " | Iteration " << iIter << std::endl << std::endl ;
          TF1* ptf1_PRF_fit = 0 ;

          // Minimisation
          if(Procedure == "PV0") ptf1_PRF_fit = PRF_PV0_iter(aSample, ModuleNber , TAG, Norm, a2, a4, b2, b4, iIter) ;
          if(Procedure == "PV1") ptf1_PRF_fit = PRF_PV1_iter(aSample, ModuleNber , TAG, aSignalTime_Exact_RhoPRF, RC, err_RC, ptge_RC, DD, iIter) ;
          if(Procedure == "PV2") ptf1_PRF_fit = PRF_PV2_iter(aSample, ModuleNber , TAG, aSignalTime_Exact_AlphaPRF, RC, err_RC, ptge_RC, DD, iIter) ;

          // Draw Status and PRF plots
          if(iIter == 0 or iIter == NIter) Tristan_DESY21_DrawOut_Status(aSample, ModuleNber ,OUTDIR, TAG, Stage, iPV, iIter, ptf1_PRF_fit) ;  // Control plots (iter0 and iter8)
          Tristan_DESY21_DrawOut_PRF(aSample, ModuleNber , OUTDIR, TAG, Stage, iPV, iIter, NIter, ptf1_PRF_fit) ;                // PRF plots (each iteration)

          // Get residuals, bias and resolution
          DoTracksReconstruction(aSample, ModuleNber ) ;
          Tristan_DESY21_DrawOut_Residuals(aSample, ModuleNber , OUTDIR, TAG, ptge_std_mean, ptge_bias_mean, DD, iIter) ;
        }

        // Draw Event Displays and Waveforms if DeltaT < -100 bins
        Tristan_DESY21_EventsWF(aSample, ModuleNber , TAG, Stage, pt, zdrift) ;
      }
      Tristan_DrawOut_DD_functions(Stage, ptge_std_mean, ptge_bias_mean, ptge_RC, pt) ;
    }
  }
}
