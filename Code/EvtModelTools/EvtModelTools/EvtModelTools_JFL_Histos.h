#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"
#include "EvtModel/Cluster.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TF1.h"

//------------------------WF pad Display
//Output waveform of a pad  with tagging string TAG, placed in OUTDIR dir
void DrawOut_JF_WaveFormDisplay(const Pad* pPad,const std::string& OUTDIR,const std::string& TAG,const int& Opt) ;

//Get waveform histo of pad with tagging string TAG
TH1F* GiveMe_JF_WaveFormDisplay(const Pad* pPad,const std::string& TAG, const double& Tmin, const double& Tmax) ;


//Get Histo of Mean Cluster multiplicity
TH1F*     GiveMe_TH1F_MeanClusterMultiplicity ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG 
                               ) ;



//Get Histo of Rho 
TH1F*     GiveMe_pTH1F_Rho                         (Sample& aSample, const int& ModuleNber) ;

//Get Histo of YTrack - YPad
TH1F*     GiveMe_pTH1F_YTrackYPad                  (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
                                                   ) ;
TH1F*     GiveMe_pTH1F_YTrackYPad                  (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& Nbins, 
                                                    const double& Xmin, 
                                                    const double& Xmax
                                                   ) ;

TH1F*     GiveMe_pTH1F_YTrackYPadLeading          (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
                                                   ) ;
TH1F*     GiveMe_pTH1F_YTrackYPadLeading          (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& Nbins, 
                                                    const double& Xmin, 
                                                    const double& Xmax
                                                   ) ;

//Get Profile Rho VS YTrack - YPad
TProfile* GiveMe_pTProfile_rho_VS_YTrackYPad       (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const double& NberOfBinsForProfile, 
                                                    const double& vY_min_DiffPRF , 
                                                    const double& vY_max_DiffPRF
                                                   ) ;
TH2D* GiveMe_pTH2D_rho_VS_YTrackYPad               (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
                                                   ) ;
TH2D* GiveMe_pTH2D_rho_VS_YTrackYPad               (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG ,
                                                    const int& NberOfBins_TH2D_rho_VS_YTrackYPad, 
                                                    const double& vY_min , 
                                                    const double& vY_max  
                                                   ) ;

//Get Profile Rho Leading Pad VS YTrack - YPad
TProfile* GiveMe_pTProfile_RhoLeading_VS_YTrackYPad(Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const double& NberOfBinsForProfile, 
                                                    const double& vY_min_DiffPRF , 
                                                    const double& vY_max_DiffPRF
                                                   ) ;

//Get Profile DeltaT Pad VS YTrack - YPad
TProfile* GiveMe_pTProfile_DT_VS_YTrackYPad        (Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const double& NberOfBinsForProfile, 
                                                    const double& vY_min_DiffPRF , 
                                                    const double& vY_max_DiffPRF
                                                   ) ;

//Get Histo of YTrack    
TH1F*     GiveMe_pTH1F_YTrack                      (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
                                                   ) ;

//Get Profile YTrack_After_fit - YTrack_Before_fit VS YTrack_After_fit
TProfile* GiveMe_pTProfile_DeltaYTrack_VS_YTrack   (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
                                                   ) ;

//Get Profile Error on Track VS YTrack 
TProfile* GiveMe_pTProfile_ErrorOnYTrack_VS_YTrack (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
                                                   ) ;

//-----------------------------------------------------------------------//

TH1F*     GiveMe_pTH1F_DT           (Sample& aSample, const int& ModuleNber) ;
TH1F*     GiveMe_DT                 (Sample& aSample, const int& ModuleNber , const std::string& TAG) ;

TH1F*     GiveMe_pTH1F_Yw  ( 
                            Sample& aSample, const int& ModuleNber ,
                            const std::string& TAG,
                            const int&    Nbin ,
                            const double& Ymin ,
                            const double& Ymax  
                           ) ;
TH1F*     GiveMe_pTH1F_YwYpad  ( 
                            Sample& aSample, const int& ModuleNber ,
                            const std::string& TAG,
                            const int&    Nbin ,
                            const double& Ymin ,
                            const double& Ymax  
                           ) ;

TH1F*     GiveMe_ClusterTypology (Sample& aSample, const int& ModuleNber,const std::string& TAG) ;


TH1F*     GiveMe_pTH1F_Tmax                   (Sample& aSample, const int& ModuleNber , const std::string& TAG) ;

TH1F*     GiveMe_pTH1F_Amax_Leading           (Sample& aSample, const int& ModuleNber , const std::string& TAG) ;



TProfile* GiveMe_pTProfile_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string & TAG);
TProfile* GiveMe_pTProfile_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string & TAG, const int& DTbin, const int& DTmin, const int& DTmax, const double& rhomin, const double& rhomax);


TH2F* GiveMe_pTH2F_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax);

TH2F* GiveMe_pTH2F_DeltaT_YwYpad (
                                  Sample& aSample, const int& ModuleNber, 
                                  const std::string& TAG
                                 );
TH2F* GiveMe_pTH2F_DeltaT_YwYpad (
                                  Sample& aSample, const int& ModuleNber, 
                                  const std::string& TAG,
                                  const double& ywypad_min ,
                                  const double& ywypad_max
                                 );

TH1F* GiveMe_pTH1F_YwYpad(
                          Sample& aSample, const int& ModuleNber, 
                          const std::string& TAG
                         ) ;
TH1F* GiveMe_pTH1F_YwYpad(
                          Sample& aSample, const int& ModuleNber, 
                          const std::string& TAG ,
                          const double ywpad_min ,
                          const double ywpad_max  
                         ) ;


TProfile* GiveMe_pTProfile_DeltaT_YwYpad (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG
                                         );
TProfile* GiveMe_pTProfile_DeltaT_YwYpad (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG,
                                          const double& ywypad_min ,
                                          const double& ywypad_max
                                         );
                                         
TH1F* GiveMe_pTH1F_Rho (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Rho (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Npadmin, const int& Npadmax) ;

TH1F* GiveMe_pTH1F_Yw (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

TH1I* GiveMe_pTH1I_LeadingiY (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

TProfile* GiveMe_pTProfile_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TProfile* GiveMe_pTProfile_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax) ;

TH1F* GiveMe_pTH1F_PullFitRC_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitRC_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitRC_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitRC_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitRC_PV4 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitRC_PV4 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitPRF (TGraphErrors* pTGraphErrors, TF1* pTF1_rho_VS_YTrackYPad, double& Chi2Min_PRFfit, int& NODF_PRFfit , const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitPRF (TGraphErrors* pTGraphErrors, TF1* pTF1_rho_VS_YTrackYPad, double& Chi2Min_PRFfit, int& NODF_PRFfit , const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;



TH1F* GiveMe_pTH1F_PullFitRC_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitRC_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_PullFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_PullFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) ;


//----------------------  Alpha2_VS_Alpha3 ----------------------//
TGraph* GiveMe_pTGraph_Alpha2_VS_Alpha3  (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F*   GiveMe_pTH2F_Alpha2_VS_Alpha3 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GiveMe_pTH2F_Alpha2_VS_Alpha3 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

//----------------------  Tau2_VS_Tau3  ----------------------//
TGraph* GiveMe_pTGraph_Tau2_VS_Tau3  (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F*   GimeMe_pTH2F_Tau2_VS_Tau3 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GimeMe_pTH2F_Tau2_VS_Tau3 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

//----------------------  Alpha2_VS_Alpha3onAlpha2  ----------------------//
TGraph* GiveMe_pTGraph_Alpha2_VS_Alpha3onAlpha2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F*   GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

//----------------------  Tau2_VS_Tau3mTau2  ----------------------//
TGraph* GiveMe_pTGraph_Tau2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F*   GiveMe_pTH2F_Tau2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GiveMe_pTH2F_Tau2_VS_Tau3mTau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F*   GiveMe_pTH2F_Alpha2_VS_Tau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GiveMe_pTH2F_Alpha2_VS_Tau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F*   GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*   GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);


TH1F* GiveMe_pTH1F_LeadTmax (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH1F* GiveMe_pTH1F_LeadTmax (Sample& aSample, const int& ModuleNber, const std::string& TAG,const int& NbinX,const double& Xmin, const double& Xmax);

TH1F* GiveMe_pTH1F_DTLeading (Sample& aSample, const int& ModuleNber,const std::string& TAG) ;
TH1F* GiveMe_pTH1F_DTLeading (Sample& aSample, const int& ModuleNber,const std::string& TAG,const int& NbinX,const double& Xmin, const double& Xmax) ;

TH1F* GiveMe_pTH1F_TauNotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1F* GiveMe_pTH1F_TauNotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Nbin,const double& DTmin, const double& DTmax) ;

TH2F* GiveMe_pTH2F_Alpha_VS_Tau_NotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F* GiveMe_pTH2F_Alpha_VS_Tau_NotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, int DTbins, int DTmin, int DTmax, int rhobin, float rhomin, float rhomax) ;

TH2F* GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F* GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
) ;

TH1F* GiveMe_pTH1F_Amax (Sample& aSample, const int& ModuleNber , const std::string& TAG) ;
TH1F* GiveMe_pTH1F_Amax (
                         Sample& aSample, const int& ModuleNber, const std::string& TAG,
                         const int& NbinX, const double& Xmin, const double& Xmax
);

TH1F* GiveMe_pTH1F_AlphaNotLeading(Sample& aSample, const int& ModuleNber , const std::string& TAG) ;
TH1F* GiveMe_pTH1F_AlphaNotLeading(
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax
) ;

TH1F* GiveMe_pTH1F_AMax_WFmAMax_FIT(Sample& aSample, const int& ModuleNber , const std::string& TAG) ;
TH1F* GiveMe_pTH1F_TMax_WFmTMax_FIT(Sample& aSample, const int& ModuleNber , const std::string& TAG) ;

TH2F* GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);
TH2F* GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH1F* GiveMe_pTH1F_AMaxmAMax_FIT(Sample& aSample, const int& ModuleNber , const std::string& TAG) ;
TH1F* GiveMe_pTH1F_TMaxmTMax_FIT(Sample& aSample, const int& ModuleNber , const std::string& TAG) ;

TH2F* GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);
TH2F* GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F* GiveMe_pTH2F_Alpha_VS_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F* GiveMe_pTH2F_Alpha_VS_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;

TH2F* GiveMe_pTH2F_Tau_VS_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F* GiveMe_pTH2F_Tau_VS_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;


TH1F* GiveMe_pTH1F_PadMultiplicityPerEvent (
                                            Sample& aSample, const int& ModuleNber, 
                                            const std::string& TAG, 
                                            const int& NbinX, const double& Xmin, const double& Xmax
                                           ) ;

//-----------------------------------//
TH1F*     GiveMe_pTH1F_AMax_FITmAMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
);

TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH1F*     GiveMe_pTH1F_TMax_FITmTMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
);

TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH1F*     GiveMe_pTH1F_TMax_FromSetmTMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
);

TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH1F*     GiveMe_pTH1F_AMax_FromSetmAMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
);

TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);


TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);


TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TProfile* GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG
);
TProfile* GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG,
                                          const int&    NbinX ,
                                          const double& X_min ,
                                          const double& X_max
);

TH1F* GiveMe_pTH1F_SlopeXY     (Sample& aSample, const int& ModuleNber , const std::string&  TAG) ;
TH1F* GiveMe_pTH1F_SlopeXZ     (Sample& aSample, const int& ModuleNber , const std::string&  TAG) ;
TH1F* GiveMe_pTH1F_InterCeptXZ (Sample& aSample, const int& ModuleNber , const std::string&  TAG) ;

//------------------Samira Histos----------------------------------------------------------------
TH1F* GiveMe_pTH1F_SlopeXYZ0     (Sample& aSample, const int& ModuleNber , const std::string&  TAG, const double& v_drift) ;
TH1F* GiveMe_pTH1F_SlopeXYZ1     (Sample& aSample, const int& ModuleNber , const std::string&  TAG) ;

TH2F*  GiveMe_pTH2F_XZ_display (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*  GiveMe_pTH2F_XZ_display (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);

TH2F*  GiveMe_pTH2F_XY_display (Sample& aSample, const int& ModuleNber, const std::string& TAG);
TH2F*  GiveMe_pTH2F_XY_display (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
);
