#include "Analysis_JF/ReadRootFile.h"

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
#include "TROOT.h"
#include "TVectorD.h"

ReadRootFile::ReadRootFile()
{
//
  pTF1_rho_VS_YTrackYPad              = 0 ; 
  pTF1_RhoLeading_VS_YTrackYPad       = 0 ; 
  pTF1_3D_LUT_FIX_DD                  = 0 ; 
  pTF1_OK                             = 0 ;

  Input_pTH1F_YTrackYPad              = 0 ; 
  Input_pTH1F_YTrackYPadLeading       = 0 ; 
  
  Output_pTH1F_YTrackYPad             = 0 ;
  Output_pTH1F_YTrackYPadLeading      = 0 ;
  
  Output_pTH1F_PullFitCluster      = 0 ;
  
  Output_pTH1F_PullFitPRF             = 0 ;
  Output_pTH1F_PullFitRC              = 0 ;
  Input_pTH2D_2D_RhoPV2_YTYpad        = 0 ;
  Input_pTH2D_rho_VS_YTrackYPad       = 0 ;
  Output_pTH1F_Ch2MinFitCluster       = 0 ;
  Output_pTH1F_Ch2MinPerDOFFitCluster = 0 ;
  Output_pTH1F_YTrack                 = 0 ;

//
  pTGraphErrors_Residual_Mean_VS_Col  = 0 ; 
  pTGraphErrors_Residual_Sigma_VS_Col = 0 ;
  Final_pTH1F_Pull                    = 0 ;
  Final_pTH1F_Ch2Min_200              = 0 ;
  Final_pTH1F_Ch2Min_400              = 0 ;
  Final_pTH1F_Ch2Min_800              = 0 ;
  Final_pTH1F_Ch2MinPerNODF_5         = 0 ;
  Final_pTH1F_Ch2MinPerNODF_10        = 0 ;
  Final_pTH1F_Ch2MinPerNODF_20        = 0 ;

//
  Final_pTH1F_YFitCol_AllCol      = 0 ;
  Final_pTH1F_YFitCol_AllColLARGE = 0 ;
  
  Final_pTH1F_TrackDeltaTAllCol = 0 ;

  Final_pTH1F_YTrackInTracks_AllCol      = 0 ;
  Final_pTH1F_YTrackInTracks_AllColLARGE = 0 ;

  Final_TH1F_YTrackYPadInTracksAllCol = 0 ;
  Final_TH1F_YTrackYPadLeadingInTracksAllCol = 0 ;
  
}

ReadRootFile::~ReadRootFile()
{
}

void ReadRootFile::Load_Itereration(  
                    const std::string OUTDirName     ,
                    const std::string base_EventFile ,
                    const std::string AnalyseName    ,
                    const std::string& IterString     
){
  std::string TFileName = Get_TFileName_Itereration(OUTDirName,base_EventFile,AnalyseName,IterString);
  Load_Itereration(TFileName) ;
}

void ReadRootFile::Load_Itereration(  
                    const std::string& TFileName   
){
  TFile* pTFile = TFile::Open(TFileName.c_str(),"READ");
  if (!pTFile) {
    std::cout <<" ERROR. Couldn't open root file"<<std::endl;
    abort() ;
  }
  
  gROOT->ForceStyle();
  
  pTF1_rho_VS_YTrackYPad              = 0 ; 
  pTF1_RhoLeading_VS_YTrackYPad       = 0 ; 
  pTF1_3D_LUT_FIX_DD                  = 0 ; 
  pTF1_OK                             = 0 ;
  
  Input_pTH1F_YTrackYPad              = 0 ; 
  Input_pTH1F_YTrackYPadLeading       = 0 ; 
  
  Output_pTH1F_YTrackYPad             = 0 ;
  Output_pTH1F_YTrackYPadLeading      = 0 ;

  Output_pTH1F_PullFitCluster         = 0 ;

  Output_pTH1F_PullFitPRF             = 0 ;
  Output_pTH1F_PullFitRC              = 0 ;

  Input_pTH2D_2D_RhoPV2_YTYpad        = 0 ;
  Input_pTH2D_rho_VS_YTrackYPad       = 0 ;
  Input_pTH2F_Alpha_VS_YTYpad         = 0 ;

  Output_pTH1F_Ch2MinFitCluster       = 0 ;
  
  Output_pTH1F_Ch2MinPerDOFFitCluster = 0 ;
  
  Output_pTH1F_YTrack                 = 0 ;
  
  TF1* pTF1_rho_VS_YTrackYPad_Infile = (TF1*)pTFile->Get("pTF1_rho_VS_YTrackYPad") ;
  if (pTF1_rho_VS_YTrackYPad_Infile) pTF1_rho_VS_YTrackYPad = (TF1*)pTF1_rho_VS_YTrackYPad_Infile->Clone() ;
  
  TF1* pTF1_RhoLeading_VS_YTrackYPad_Infile = (TF1*)pTFile->Get("pTF1_RhoLeading_VS_YTrackYPad") ;
  if (pTF1_RhoLeading_VS_YTrackYPad_Infile) pTF1_RhoLeading_VS_YTrackYPad = (TF1*)pTF1_RhoLeading_VS_YTrackYPad_Infile->Clone() ;
  
  TF1* pTF1_3D_LUT_FIX_DD_Infile = (TF1*)pTFile->Get("pTF1_3D_LUT_FIX_DD") ;
  if (pTF1_3D_LUT_FIX_DD_Infile) pTF1_3D_LUT_FIX_DD = (TF1*)pTF1_3D_LUT_FIX_DD_Infile->Clone() ;

//  
  TH1F* Input_pTH1F_YTrackYPad_Infile = (TH1F*)pTFile->Get("Input_pTH1F_YTrackYPad") ;
  if (Input_pTH1F_YTrackYPad_Infile) Input_pTH1F_YTrackYPad = (TH1F*)Input_pTH1F_YTrackYPad_Infile->Clone() ;
  if (Input_pTH1F_YTrackYPad       ) Input_pTH1F_YTrackYPad->SetDirectory(0);

  TH1F* Input_pTH1F_YTrackYPadLeading_Infile = (TH1F*)pTFile->Get("Input_pTH1F_YTrackYPadLeading") ;
  if (Input_pTH1F_YTrackYPadLeading_Infile) Input_pTH1F_YTrackYPadLeading = (TH1F*)Input_pTH1F_YTrackYPadLeading_Infile->Clone() ;
  if (Input_pTH1F_YTrackYPadLeading       ) Input_pTH1F_YTrackYPadLeading->SetDirectory(0);

//  
  TH1F* Output_pTH1F_YTrackYPad_Infile = (TH1F*)pTFile->Get("Output_pTH1F_YTrackYPad") ;
  if (Output_pTH1F_YTrackYPad_Infile) Output_pTH1F_YTrackYPad = (TH1F*)Output_pTH1F_YTrackYPad_Infile->Clone() ;
  if (Output_pTH1F_YTrackYPad       ) Output_pTH1F_YTrackYPad->SetDirectory(0);

  TH1F* Output_pTH1F_YTrackYPadLeading_Infile = (TH1F*)pTFile->Get("Output_pTH1F_YTrackYPadLeading") ;
  if (Output_pTH1F_YTrackYPadLeading_Infile) Output_pTH1F_YTrackYPadLeading = (TH1F*)Output_pTH1F_YTrackYPadLeading_Infile->Clone() ;
  if (Output_pTH1F_YTrackYPadLeading       ) Output_pTH1F_YTrackYPadLeading->SetDirectory(0);

//  
  if (Output_pTH1F_PullFitCluster==0){
  TH1F* Output_pTH1F_PullFitCluster_Infile = (TH1F*)pTFile->Get("Output_pTH1F_PullFitCluster") ;
  if (Output_pTH1F_PullFitCluster_Infile) Output_pTH1F_PullFitCluster = (TH1F*)Output_pTH1F_PullFitCluster_Infile->Clone() ;
  if (Output_pTH1F_PullFitCluster       ) Output_pTH1F_PullFitCluster->SetDirectory(0);
  }
  
  if (Output_pTH1F_PullFitCluster==0){
  TH1F* Output_pTH1F_PullFitCluster_1D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_PullFitCluster_1D") ;
  if (Output_pTH1F_PullFitCluster_1D_Infile) Output_pTH1F_PullFitCluster = (TH1F*)Output_pTH1F_PullFitCluster_1D_Infile->Clone() ;
  if (Output_pTH1F_PullFitCluster       ) Output_pTH1F_PullFitCluster->SetDirectory(0);
  }
  
  if (Output_pTH1F_PullFitCluster==0){
  TH1F* Output_pTH1F_PullFitCluster_2D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_PullFitCluster_2D") ;
  if (Output_pTH1F_PullFitCluster_2D_Infile) Output_pTH1F_PullFitCluster = (TH1F*)Output_pTH1F_PullFitCluster_2D_Infile->Clone() ;
  if (Output_pTH1F_PullFitCluster       ) Output_pTH1F_PullFitCluster->SetDirectory(0);
  }

//  
  TH1F* Output_pTH1F_PullFitRC_Infile = (TH1F*)pTFile->Get("Output_pTH1F_PullFitRC") ;
  if (Output_pTH1F_PullFitRC_Infile) Output_pTH1F_PullFitRC = (TH1F*)Output_pTH1F_PullFitRC_Infile->Clone() ;
  if (Output_pTH1F_PullFitRC       ) Output_pTH1F_PullFitRC->SetDirectory(0);
  
  TH1F* Output_pTH1F_PullFitPRF_Infile = (TH1F*)pTFile->Get("Output_pTH1F_PullFitPRF") ;
  if (Output_pTH1F_PullFitPRF_Infile) Output_pTH1F_PullFitPRF = (TH1F*)Output_pTH1F_PullFitPRF_Infile->Clone() ;
  if (Output_pTH1F_PullFitPRF       ) Output_pTH1F_PullFitPRF->SetDirectory(0);

//  
  TH2D* Input_pTH2D_2D_RhoPV2_YTYpad_Infile = (TH2D*)pTFile->Get("Input_pTH2D_2D_RhoPV2_YTYpad") ;
  if (Input_pTH2D_2D_RhoPV2_YTYpad_Infile) Input_pTH2D_2D_RhoPV2_YTYpad = (TH2D*)Input_pTH2D_2D_RhoPV2_YTYpad_Infile->Clone() ;
  if (Input_pTH2D_2D_RhoPV2_YTYpad       ) Input_pTH2D_2D_RhoPV2_YTYpad->SetDirectory(0);
  
  TH2D* Input_pTH2D_rho_VS_YTrackYPad_Infile = (TH2D*)pTFile->Get("Input_pTH2D_rho_VS_YTrackYPad") ;
  if (Input_pTH2D_rho_VS_YTrackYPad_Infile) Input_pTH2D_rho_VS_YTrackYPad = (TH2D*)Input_pTH2D_rho_VS_YTrackYPad_Infile->Clone() ;
  if (Input_pTH2D_rho_VS_YTrackYPad       ) Input_pTH2D_rho_VS_YTrackYPad->SetDirectory(0);
  
  TH2F* Input_pTH2F_Alpha_VS_YTYpad_Infile = (TH2F*)pTFile->Get("Input_pTH2F_Alpha_VS_YTYpad") ;
  if (Input_pTH2F_Alpha_VS_YTYpad_Infile) Input_pTH2F_Alpha_VS_YTYpad = (TH2F*)Input_pTH2F_Alpha_VS_YTYpad_Infile->Clone() ;
  if (Input_pTH2F_Alpha_VS_YTYpad       ) Input_pTH2F_Alpha_VS_YTYpad->SetDirectory(0);

//
  pTF1_OK = 0 ;
  if (pTF1_rho_VS_YTrackYPad       ) pTF1_OK = pTF1_rho_VS_YTrackYPad        ;
  if (pTF1_RhoLeading_VS_YTrackYPad) pTF1_OK = pTF1_RhoLeading_VS_YTrackYPad ;
  if (pTF1_3D_LUT_FIX_DD           ) pTF1_OK = pTF1_3D_LUT_FIX_DD            ;
  if (pTF1_OK==0){
    std::cout << std::endl ;
    std::cout << " ReadRootFile::Load_Itereration : pTF1_OK==0 K1 " << std::endl ;
    abort() ;
  }

//----------------------------------------------------------------//
  Chi2Min_PRFLikeFit       = 0 ;
  Chi2Min_PRFLikeFit_Valid = 0 ;
  NODF_PRFLikeFit_Valid    = 0 ;
  NODF_PRFLikeFit          = 0 ;

  TVectorD* pTVectorD_Chi2Min_PRFLikeFit = 0 ;
  TVectorD* pTVectorD_NODF_PRFLikeFit =   0 ;

//-----PV0  
  pTVectorD_Chi2Min_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_Chi2Min_PRFfit_PV0") ;
  if (pTVectorD_Chi2Min_PRFLikeFit){
    Chi2Min_PRFLikeFit_Valid = 1 ;
    Chi2Min_PRFLikeFit = (*pTVectorD_Chi2Min_PRFLikeFit)[0] ;
  }

  pTVectorD_NODF_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_NODF_PRFfit_PV0") ;
  if (pTVectorD_NODF_PRFLikeFit){
    NODF_PRFLikeFit_Valid = 1 ;
    NODF_PRFLikeFit = (*pTVectorD_NODF_PRFLikeFit)[0] ;
  }

//-----PV2  
  pTVectorD_Chi2Min_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_Chi2Min_RCfit_PV2") ;
  if (pTVectorD_Chi2Min_PRFLikeFit){
    Chi2Min_PRFLikeFit_Valid = 1 ;
    Chi2Min_PRFLikeFit = (*pTVectorD_Chi2Min_PRFLikeFit)[0] ;
  }

  pTVectorD_NODF_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_NODF_RCfit_PV2") ;
  if (pTVectorD_NODF_PRFLikeFit){
    NODF_PRFLikeFit_Valid = 1 ;
    NODF_PRFLikeFit = (*pTVectorD_NODF_PRFLikeFit)[0] ;
  }

//-----PV3  
  pTVectorD_Chi2Min_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_Chi2Min_RCfit_PV3") ;
  if (pTVectorD_Chi2Min_PRFLikeFit){
    Chi2Min_PRFLikeFit_Valid = 1 ;
    Chi2Min_PRFLikeFit = (*pTVectorD_Chi2Min_PRFLikeFit)[0] ;
  }

  pTVectorD_NODF_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_NODF_RCfit_PV3") ;
  if (pTVectorD_NODF_PRFLikeFit){
    NODF_PRFLikeFit_Valid = 1 ;
    NODF_PRFLikeFit = (*pTVectorD_NODF_PRFLikeFit)[0] ;
  }

//-----PV4  
  pTVectorD_Chi2Min_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_Chi2Min_RCfit_PV4") ;
  if (pTVectorD_Chi2Min_PRFLikeFit){
    Chi2Min_PRFLikeFit_Valid = 1 ;
    Chi2Min_PRFLikeFit = (*pTVectorD_Chi2Min_PRFLikeFit)[0] ;
  }

  pTVectorD_NODF_PRFLikeFit = (TVectorD*)pTFile->Get("aTVectorD_NODF_RCfit_PV4") ;
  if (pTVectorD_NODF_PRFLikeFit){
    NODF_PRFLikeFit_Valid = 1 ;
    NODF_PRFLikeFit = (*pTVectorD_NODF_PRFLikeFit)[0] ;
  }


//   std::cout << " Chi2Min_PRFLikeFit_Valid " << Chi2Min_PRFLikeFit_Valid << std::endl ;
//   std::cout << " Chi2Min_PRFLikeFit       " << Chi2Min_PRFLikeFit << std::endl ;
// 
//   std::cout << " NODF_PRFLikeFit_Valid " << NODF_PRFLikeFit_Valid << std::endl ;
//   std::cout << " NODF_PRFLikeFit       " << NODF_PRFLikeFit << std::endl ;

//----------------------------------------------------------------//

//  
  if (Output_pTH1F_Ch2MinFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinFitCluster_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinFitCluster") ;
  if (Output_pTH1F_Ch2MinFitCluster_Infile) Output_pTH1F_Ch2MinFitCluster = (TH1F*)Output_pTH1F_Ch2MinFitCluster_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinFitCluster          ) Output_pTH1F_Ch2MinFitCluster->SetDirectory(0);
  }
  
  if (Output_pTH1F_Ch2MinFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinFitCluster_1D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinFitCluster_1D") ;
  if (Output_pTH1F_Ch2MinFitCluster_1D_Infile) Output_pTH1F_Ch2MinFitCluster = (TH1F*)Output_pTH1F_Ch2MinFitCluster_1D_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinFitCluster          ) Output_pTH1F_Ch2MinFitCluster->SetDirectory(0);
  }

  if (Output_pTH1F_Ch2MinFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinFitCluster_2D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinFitCluster_2D") ;
  if (Output_pTH1F_Ch2MinFitCluster_2D_Infile) Output_pTH1F_Ch2MinFitCluster = (TH1F*)Output_pTH1F_Ch2MinFitCluster_2D_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinFitCluster          ) Output_pTH1F_Ch2MinFitCluster->SetDirectory(0);
  }
    
//
  if (Output_pTH1F_Ch2MinPerDOFFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinPerDOFFitCluster_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinPerDOFFitCluster") ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster_Infile) Output_pTH1F_Ch2MinPerDOFFitCluster = (TH1F*)Output_pTH1F_Ch2MinPerDOFFitCluster_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster          ) Output_pTH1F_Ch2MinPerDOFFitCluster->SetDirectory(0);
  }
  
  if (Output_pTH1F_Ch2MinPerDOFFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinPerDOFFitCluster_1D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinPerDOFFitCluster_1D") ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster_1D_Infile) Output_pTH1F_Ch2MinPerDOFFitCluster = (TH1F*)Output_pTH1F_Ch2MinPerDOFFitCluster_1D_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster          ) Output_pTH1F_Ch2MinPerDOFFitCluster->SetDirectory(0);
  }
  
  if (Output_pTH1F_Ch2MinPerDOFFitCluster==0) {
  TH1F* Output_pTH1F_Ch2MinPerDOFFitCluster_2D_Infile = (TH1F*)pTFile->Get("Output_pTH1F_Ch2MinPerDOFFitCluster_2D") ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster_2D_Infile) Output_pTH1F_Ch2MinPerDOFFitCluster = (TH1F*)Output_pTH1F_Ch2MinPerDOFFitCluster_2D_Infile->Clone() ;
  if (Output_pTH1F_Ch2MinPerDOFFitCluster          ) Output_pTH1F_Ch2MinPerDOFFitCluster->SetDirectory(0);
  }
  
//
  TH1F* Output_pTH1F_YTrack_Infile = (TH1F*)pTFile->Get("Output_pTH1F_YTrack") ;
  if (Output_pTH1F_YTrack_Infile) Output_pTH1F_YTrack = (TH1F*)Output_pTH1F_YTrack_Infile->Clone() ;
  if (Output_pTH1F_YTrack       ) Output_pTH1F_YTrack->SetDirectory(0);
  
//
  pTFile->Close();
  delete pTFile;
  
}

void ReadRootFile::Load_AnalyseReconstruction(
                              const std::string OUTDirName     ,
                              const std::string base_EventFile ,
                              const std::string AnalyseName  
){
  std::string TFileName = Get_TFileName_AnalyseReconstruction(OUTDirName,base_EventFile,AnalyseName);
  Load_AnalyseReconstruction(TFileName);
}

void ReadRootFile::Load_AnalyseReconstruction(
                              const std::string& TFileName 
){
  TFile* pTFile = TFile::Open(TFileName.c_str(),"READ");
  if (!pTFile) {
    std::cout <<" ERROR. Couldn't open root file"<<std::endl;
    abort() ;
  }
  
  gROOT->ForceStyle();

  pTGraphErrors_Residual_Mean_VS_Col  = 0 ; 
  pTGraphErrors_Residual_Sigma_VS_Col = 0 ;
  Final_pTH1F_Pull                    = 0 ;
  Final_pTH1F_Ch2Min_200              = 0 ;
  Final_pTH1F_Ch2Min_400              = 0 ;
  Final_pTH1F_Ch2Min_800              = 0 ;
  Final_pTH1F_Ch2MinPerNODF_5         = 0 ;
  Final_pTH1F_Ch2MinPerNODF_10        = 0 ;
  Final_pTH1F_Ch2MinPerNODF_20        = 0 ;

//
  Final_pTH1F_YFitCol_AllCol      = 0 ;
  Final_pTH1F_YFitCol_AllColLARGE = 0 ;
  
  Final_pTH1F_TrackDeltaTAllCol = 0 ;

  Final_pTH1F_YTrackInTracks_AllCol      = 0 ;
  Final_pTH1F_YTrackInTracks_AllColLARGE = 0 ;

  Final_TH1F_YTrackYPadInTracksAllCol = 0 ;
  Final_TH1F_YTrackYPadLeadingInTracksAllCol = 0 ;
  
//
  TH1F* Final_pTH1F_YFitCol_AllCol_Infile = (TH1F*)pTFile->Get("Final_pTH1F_YFitCol_AllCol") ;
  if (Final_pTH1F_YFitCol_AllCol_Infile) Final_pTH1F_YFitCol_AllCol = (TH1F*)Final_pTH1F_YFitCol_AllCol_Infile->Clone() ;
  if (Final_pTH1F_YFitCol_AllCol       ) Final_pTH1F_YFitCol_AllCol->SetDirectory(0);

  TH1F* Final_pTH1F_YFitCol_AllColLARGE_Infile = (TH1F*)pTFile->Get("Final_pTH1F_YFitCol_AllColLARGE") ;
  if (Final_pTH1F_YFitCol_AllColLARGE_Infile) Final_pTH1F_YFitCol_AllColLARGE = (TH1F*)Final_pTH1F_YFitCol_AllColLARGE_Infile->Clone() ;
  if (Final_pTH1F_YFitCol_AllColLARGE       ) Final_pTH1F_YFitCol_AllColLARGE->SetDirectory(0);

//
  TH1F* Final_pTH1F_TrackDeltaTAllCol_Infile = (TH1F*)pTFile->Get("Final_pTH1F_TrackDeltaTAllCol") ;
  if (Final_pTH1F_TrackDeltaTAllCol_Infile) Final_pTH1F_TrackDeltaTAllCol = (TH1F*)Final_pTH1F_TrackDeltaTAllCol_Infile->Clone() ;
  if (Final_pTH1F_TrackDeltaTAllCol       ) Final_pTH1F_TrackDeltaTAllCol->SetDirectory(0);

//
  TH1F* Final_pTH1F_YTrackInTracks_AllCol_Infile = (TH1F*)pTFile->Get("Final_pTH1F_YTrackInTracks_AllCol") ;
  if (Final_pTH1F_YTrackInTracks_AllCol_Infile) Final_pTH1F_YTrackInTracks_AllCol = (TH1F*)Final_pTH1F_YTrackInTracks_AllCol_Infile->Clone() ;
  if (Final_pTH1F_YTrackInTracks_AllCol       ) Final_pTH1F_YTrackInTracks_AllCol->SetDirectory(0);

  TH1F* Final_pTH1F_YTrackInTracks_AllColLARGE_Infile = (TH1F*)pTFile->Get("Final_pTH1F_YTrackInTracks_AllColLARGE") ;
  if (Final_pTH1F_YTrackInTracks_AllColLARGE_Infile) Final_pTH1F_YTrackInTracks_AllColLARGE = (TH1F*)Final_pTH1F_YTrackInTracks_AllColLARGE_Infile->Clone() ;
  if (Final_pTH1F_YTrackInTracks_AllColLARGE       ) Final_pTH1F_YTrackInTracks_AllColLARGE->SetDirectory(0);

//
  TH1F* Final_TH1F_YTrackYPadInTracksAllCol_Infile = (TH1F*)pTFile->Get("Final_TH1F_YTrackYPadInTracksAllCol") ;
  if (Final_TH1F_YTrackYPadInTracksAllCol_Infile) Final_TH1F_YTrackYPadInTracksAllCol = (TH1F*)Final_TH1F_YTrackYPadInTracksAllCol_Infile->Clone() ;
  if (Final_TH1F_YTrackYPadInTracksAllCol       ) Final_TH1F_YTrackYPadInTracksAllCol->SetDirectory(0);

//
  TH1F* Final_TH1F_YTrackYPadLeadingInTracksAllCol_Infile = (TH1F*)pTFile->Get("Final_TH1F_YTrackYPadLeadingInTracksAllCol") ;
  if (Final_TH1F_YTrackYPadLeadingInTracksAllCol_Infile) Final_TH1F_YTrackYPadLeadingInTracksAllCol = (TH1F*)Final_TH1F_YTrackYPadLeadingInTracksAllCol_Infile->Clone() ;
  if (Final_TH1F_YTrackYPadLeadingInTracksAllCol       ) Final_TH1F_YTrackYPadLeadingInTracksAllCol->SetDirectory(0);

//
  V_pTH1F_YFitCol     .clear() ;
  V_pTH1F_YFitColLARGE.clear() ;

  V_pTH1F_TrackDeltaT.clear() ;

  V_pTH1F_YTrackInTracks     .clear() ;
  V_pTH1F_YTrackInTracksLARGE.clear() ;

  V_pTH1F_YTrackYPadInTracks.clear() ;
  V_pTH1F_YTrackYPadLeadingInTracks.clear() ;

  V_TGraphErrors_Mean_perCol .clear() ;
  V_TGraphErrors_Sigma_perCol.clear() ;

//
  TGraphErrors* pTGraphErrors_Residual_Mean_VS_Col_Infile = (TGraphErrors*)pTFile->Get("pTGraphErrors_Residual_Mean_VS_Col") ;
  pTGraphErrors_Residual_Mean_VS_Col = (TGraphErrors*)pTGraphErrors_Residual_Mean_VS_Col_Infile->Clone() ;
  
  TGraphErrors* pTGraphErrors_Residual_Sigma_VS_Col_Infile = (TGraphErrors*)pTFile->Get("pTGraphErrors_Residual_Sigma_VS_Col") ;
  pTGraphErrors_Residual_Sigma_VS_Col = (TGraphErrors*)pTGraphErrors_Residual_Sigma_VS_Col_Infile->Clone() ;

  TH1F* Final_pTH1F_Pull_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Pull") ;
  if (Final_pTH1F_Pull_Infile) Final_pTH1F_Pull = (TH1F*)Final_pTH1F_Pull_Infile->Clone() ;
  if (Final_pTH1F_Pull       ) Final_pTH1F_Pull->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2Min_200_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2Min_200") ;
  if (Final_pTH1F_Ch2Min_200_Infile) Final_pTH1F_Ch2Min_200 = (TH1F*)Final_pTH1F_Ch2Min_200_Infile->Clone() ;
  if (Final_pTH1F_Ch2Min_200       ) Final_pTH1F_Ch2Min_200->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2Min_400_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2Min_400") ;
  if (Final_pTH1F_Ch2Min_400_Infile) Final_pTH1F_Ch2Min_400 = (TH1F*)Final_pTH1F_Ch2Min_400_Infile->Clone() ;
  if (Final_pTH1F_Ch2Min_400       ) Final_pTH1F_Ch2Min_400->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2Min_800_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2Min_800") ;
  if (Final_pTH1F_Ch2Min_800_Infile) Final_pTH1F_Ch2Min_800 = (TH1F*)Final_pTH1F_Ch2Min_800_Infile->Clone() ;
  if (Final_pTH1F_Ch2Min_800       ) Final_pTH1F_Ch2Min_800->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2MinPerNODF_5_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2MinPerNODF_5") ;
  if (Final_pTH1F_Ch2MinPerNODF_5_Infile) Final_pTH1F_Ch2MinPerNODF_5 = (TH1F*)Final_pTH1F_Ch2MinPerNODF_5_Infile->Clone() ;
  if (Final_pTH1F_Ch2MinPerNODF_5       ) Final_pTH1F_Ch2MinPerNODF_5->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2MinPerNODF_10_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2MinPerNODF_10") ;
  if (Final_pTH1F_Ch2MinPerNODF_10_Infile) Final_pTH1F_Ch2MinPerNODF_10 = (TH1F*)Final_pTH1F_Ch2MinPerNODF_10_Infile->Clone() ;
  if (Final_pTH1F_Ch2MinPerNODF_10       ) Final_pTH1F_Ch2MinPerNODF_10->SetDirectory(0);

  TH1F* Final_pTH1F_Ch2MinPerNODF_20_Infile = (TH1F*)pTFile->Get("Final_pTH1F_Ch2MinPerNODF_20") ;
  if (Final_pTH1F_Ch2MinPerNODF_20_Infile) Final_pTH1F_Ch2MinPerNODF_20 = (TH1F*)Final_pTH1F_Ch2MinPerNODF_20_Infile->Clone() ;
  if (Final_pTH1F_Ch2MinPerNODF_20       ) Final_pTH1F_Ch2MinPerNODF_20->SetDirectory(0);

//
  int iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YFitCol_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YFitCol.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YFitColLARGE_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YFitColLARGE.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_TrackDeltaT_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_TrackDeltaT.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackInTracks_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YTrackInTracks.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackInTracksLARGE_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YTrackInTracksLARGE.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackYPadInTracks_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YTrackYPadInTracks.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackYPadLeadingInTracks_" << iTem ;
    TH1F* pTH1F = 0 ;
    TH1F* pTH1F_Infile = (TH1F*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTH1F_Infile) pTH1F = (TH1F*)pTH1F_Infile->Clone() ;
    if (pTH1F       ) pTH1F->SetDirectory(0);
    V_pTH1F_YTrackYPadLeadingInTracks.push_back( pTH1F ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTGraphErrors_Sigma_" << iTem ;
    TGraphErrors* pTGraphErrors = 0 ;
    TGraphErrors* pTGraphErrors_Infile = (TGraphErrors*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTGraphErrors_Infile) pTGraphErrors = (TGraphErrors*)pTGraphErrors_Infile->Clone() ;
    V_TGraphErrors_Sigma_perCol.push_back( pTGraphErrors ) ;
  }

  iTem_Max = 36 ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTGraphErrors_Mean_" << iTem ;
    TGraphErrors* pTGraphErrors = 0 ;
    TGraphErrors* pTGraphErrors_Infile = (TGraphErrors*)pTFile->Get( (aostringstream.str()).c_str() ) ;
    if (pTGraphErrors_Infile) pTGraphErrors = (TGraphErrors*)pTGraphErrors_Infile->Clone() ;
    V_TGraphErrors_Mean_perCol.push_back( pTGraphErrors ) ;
  }

//
  pTFile->Close();
  delete pTFile;
  
}

std::string ReadRootFile::Get_DirAnalysisName(
                                const std::string OUTDirName      ,
                                const std::string& base_EventFile ,
                                const std::string& AnalyseName    
){
//std::string OUTDirName  = "../FromCERN/OUT_JeanFrancois_Prod_22_07_05/" ;
//std::string OUTDirName  = "OUT_JeanFrancois/" ;

  std::string DirAnalysisName = OUTDirName + "Dir_" + base_EventFile + "_" + AnalyseName + "/" ;

  return DirAnalysisName ;
}

std::string ReadRootFile::Get_TFileName_Itereration(
                                        const std::string OUTDirName     ,
                                        const std::string& base_EventFile ,
                                        const std::string& AnalyseName    ,
                                        const std::string& IterString 
){
  std::string DirAnalysisName = Get_DirAnalysisName(OUTDirName,base_EventFile,AnalyseName) ;
  
  std::string TFileName = DirAnalysisName + IterString + ".root";

  return TFileName ;
}

std::string ReadRootFile::Get_TFileName_AnalyseReconstruction(
                                                  const std::string OUTDirName     ,
                                                  const std::string& base_EventFile ,
                                                  const std::string& AnalyseName 
){
  std::string DirAnalysisName = Get_DirAnalysisName(OUTDirName,base_EventFile,AnalyseName) ;

  std::string TAG         = "FINAL"            ; 

  std::string TFileName = DirAnalysisName + "AnalyseReconstruction_" + TAG + ".root";

  return TFileName ;  
}
