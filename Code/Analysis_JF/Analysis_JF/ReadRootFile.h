#ifndef ReadRootFile_H
#define ReadRootFile_H

#include "Misc/Misc.h"

#include "TGraphErrors.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2D.h"


/////////////////////////////////////////////////////////////
class ReadRootFile {
public:
   /** Constructor */
   ReadRootFile();
   virtual ~ReadRootFile();

   void Load_Itereration(  
                    const std::string OUTDirName     ,
                    const std::string base_EventFile ,
                    const std::string AnalyseName    ,
                    const std::string& IterString     
                   ) ;
   void Load_Itereration(  
                    const std::string& TFileName   
                   ) ;
              
   void Load_AnalyseReconstruction(  
                              const std::string OUTDirName     ,
                              const std::string base_EventFile ,
                              const std::string AnalyseName  
                             ) ;
   void Load_AnalyseReconstruction(  
                              const std::string& TFileName 
                             ) ;
              
public:
//   Load_Itereration 
  TF1*  pTF1_rho_VS_YTrackYPad              ; 
  TF1*  pTF1_RhoLeading_VS_YTrackYPad       ; 
  TF1*  pTF1_3D_LUT_FIX_DD                  ; 
  TF1*  pTF1_OK                             ;
  
  TH1F* Input_pTH1F_YTrackYPad              ; 
  TH1F* Input_pTH1F_YTrackYPadLeading       ; 
  
  TH1F* Output_pTH1F_YTrackYPad             ;
  TH1F* Output_pTH1F_YTrackYPadLeading      ;
  
  TH1F* Output_pTH1F_PullFitCluster         ;
  
  TH1F* Output_pTH1F_PullFitPRF             ;
  TH1F* Output_pTH1F_PullFitRC              ;
  
  TH2D* Input_pTH2D_2D_RhoPV2_YTYpad        ;
  TH2D* Input_pTH2D_rho_VS_YTrackYPad       ;
  TH2F* Input_pTH2F_Alpha_VS_YTYpad         ;
  
  TH1F* Output_pTH1F_Ch2MinFitCluster       ;
  TH1F* Output_pTH1F_Ch2MinPerDOFFitCluster ;
  
  TH1F* Output_pTH1F_YTrack                 ;
  
  int    Chi2Min_PRFLikeFit_Valid ;
  double Chi2Min_PRFLikeFit       ;
  
  int    NODF_PRFLikeFit_Valid ;
  int    NODF_PRFLikeFit       ;

//   Load_AnalyseReconstruction 
  TGraphErrors* pTGraphErrors_Residual_Mean_VS_Col  ; 
  TGraphErrors* pTGraphErrors_Residual_Sigma_VS_Col ;
  TH1F*         Final_pTH1F_Pull                    ;
  TH1F*         Final_pTH1F_Ch2Min_200              ;
  TH1F*         Final_pTH1F_Ch2Min_400              ;
  TH1F*         Final_pTH1F_Ch2Min_800              ;
  TH1F*         Final_pTH1F_Ch2MinPerNODF_5         ;
  TH1F*         Final_pTH1F_Ch2MinPerNODF_10        ;
  TH1F*         Final_pTH1F_Ch2MinPerNODF_20        ;
  
  TH1F* Final_pTH1F_YFitCol_AllCol        ;
  TH1F* Final_pTH1F_YFitCol_AllColLARGE   ;

  TH1F* Final_pTH1F_TrackDeltaTAllCol   ;

  TH1F* Final_pTH1F_YTrackInTracks_AllCol        ;
  TH1F* Final_pTH1F_YTrackInTracks_AllColLARGE   ;

  TH1F* Final_TH1F_YTrackYPadInTracksAllCol ;
  TH1F* Final_TH1F_YTrackYPadLeadingInTracksAllCol ;

  std::vector < TH1F* > V_pTH1F_YFitCol      ;
  std::vector < TH1F* > V_pTH1F_YFitColLARGE ;

  std::vector < TH1F* > V_pTH1F_TrackDeltaT ;

  std::vector < TH1F* > V_pTH1F_YTrackInTracks      ;
  std::vector < TH1F* > V_pTH1F_YTrackInTracksLARGE ;

  std::vector < TH1F* > V_pTH1F_YTrackYPadInTracks ;
  std::vector < TH1F* > V_pTH1F_YTrackYPadLeadingInTracks ;

  std::vector < TGraphErrors* > V_TGraphErrors_Mean_perCol  ;
  std::vector < TGraphErrors* > V_TGraphErrors_Sigma_perCol ;

private:
  std::string Get_DirAnalysisName(
                                  const std::string OUTDirName      ,
                                  const std::string& base_EventFile ,
                                  const std::string& AnalyseName    
                                 );
  std::string Get_TFileName_Itereration(
                                        const std::string OUTDirName      ,
                                        const std::string& base_EventFile ,
                                        const std::string& AnalyseName    ,
                                        const std::string& IterString 
                                       );
  std::string Get_TFileName_AnalyseReconstruction(
                                                  const std::string OUTDirName      ,
                                                  const std::string& base_EventFile ,
                                                  const std::string& AnalyseName 
                                                 );


};

#endif

