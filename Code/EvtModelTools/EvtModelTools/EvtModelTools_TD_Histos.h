#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"
#include "EvtModel/Cluster.h"

#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TProfile.h"
#include "TGraphErrors.h"

TGraphErrors* TH2_to_TGE(const TH2D* pTH2) ;

//////////////////////////////////////////////////// TH1I /////////////////////////////////////////////////////

// vertical position of the leading pad
TH1I* GiveMe_LeadingiY              (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// Verticality of a Cluster
TH1I* GiveMe_Cluster_Verticality    (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// number of Clusters per pad column (should be 1, possibly 0 for column #0 and #35)
TH1I* GiveMe_NClusPerCol        (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// Number of Pads per Cluster
TH1I* GiveMe_PadPerCluster          (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// number of Clusters per Event
TH1I* GiveMe_ClusterPerEvent        (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// difference of maximum Amplitude using pads or ADCs
TH1I* GiveMe_Amax_AmaxWF            (Sample& aSample, const int& ModuleNber, const std::string& TAG) ; 

// Time difference of the timestamp of the maximum amplitude using pads or ADCs
TH1I* GiveMe_Tmax_TmaxWF            (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// DeltaT
TH1I* GiveMe_DeltaT                 (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber) ;
TH1I* GiveMe_DeltaT                 (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber, int Nbin, int DTmin, int DTmax) ;

// Tmax of Leading
TH1I* GiveMe_LeadTmax               (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// Difference of time between mean of Tleading and each TLeading
TH1I* GiveMe_TLeading_TEvent        (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1I* GiveMe_TLeading_TEvent        (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Nbins, const int& Diffmin, const int& Diffmax) ;

// Amplitude of Leading pad
TH1I* GiveMe_AmaxLeading            (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1I* GiveMe_AmaxLeading            (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& nbins, const int& ADCmin, const int& ADCmax) ;

// Amplitude of Subleading pad
TH1I* GiveMe_AmaxSubLeading         (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1I* GiveMe_AmaxSubLeading         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& nbins, const int& ADCmin, const int& ADCmax) ;

// Difference of amplitude from Cluster - Sum of amplitudes from pads
TH1I* GiveMe_AClus_ASumPads         (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1I* GiveMe_AClus_ASumPads         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Nbins, const int& ADCmin, const int& ADCmax) ;


//////////////////////////////////////////////////// TH1D /////////////////////////////////////////////////////

// Rho distribution of leading pads
TH1D* GiveMe_Rho                    (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// Rho distribution of leading pads for given range of number of pads
TH1D* GiveMe_Rho                    (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Npadmin, const int& Npadmax) ;

// #y_T distribution
TH1D* GiveMe_YT                     (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// y_T - y_pad
TH1D* GiveMe_YTYpad                 (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH1D* GiveMe_YTYpad                 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const int& Ymin, const int& Ymax) ;

// y_T - y_T(before minimization)
TH1D* GiveMe_DeltaYT                (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax) ;

// Ampllitude of a cluster divide by its number of pads
TH1D* GiveMe_AclusNPads             (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage) ;
TH1D* GiveMe_AclusNPads             (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& nbins, const int& Xmin, const int& Xmax) ;

// Discard clusters misaligned with the tendency of the track
TH1D* GiveMe_TrackAlignResiduals    (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage, const int& iIter) ;


//////////////////////////////////////////////////// TH2I /////////////////////////////////////////////////////

// 2D: T_max_pads VS T_max_leading
TH2I* GiveMe_Tmax_PadvLead          (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;

// (T2max - T1max) VS (T3max - T1max)
TH2I* GiveMe_DeltaT21vs31           (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage) ;
TH2I* GiveMe_DeltaT21vs31           (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, int Nbin21, int DTmin21, int DTmax21, int Nbin31, int DTmin31, int DTmax31) ;

//////////////////////////////////////////////////// TH2F /////////////////////////////////////////////////////

// 2D: Rho VS DeltaT
TH2F* GiveMe_2D_Rho_DeltaT          (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2F* GiveMe_2D_Rho_DeltaT          (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber) ;
TH2F* GiveMe_2D_Rho_DeltaT          (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber, int DTbins, int DTmin, int DTmax, int rhobin, float rhomin, float rhomax) ;

//////////////////////////////////////////////////// TH2D /////////////////////////////////////////////////////

// 2D: rho VS YT
TH2D* GiveMe_2D_Rho_YT              (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2D* GiveMe_2D_Rho_YT              (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;

// 2D: rho VS YT for PV2 (rho = A_i / A_leading)
TH2D* GiveMe_2D_RhoPV2_YT           (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2D* GiveMe_2D_RhoPV2_YT           (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;

// 2D: rho VS YTYpad
TH2D* GiveMe_2D_Rho_YTYpad          (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int PV) ;
TH2D* GiveMe_2D_Rho_YTYpad          (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;

// 2D: rho VS YTYpad for PV2 (rho = A_i / A_leading)
TH2D* GiveMe_2D_RhoPV2_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2D* GiveMe_2D_RhoPV2_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax) ;

// 2D: DeltaYT VS Y_T - Y-pad
TH2D* GiveMe_2D_DeltaY_YTYpad       (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& ytypadbin, const double& ytypadmin, const double& ytypadmax, const int& DYbins, const double& DYmin, const double& DYmax) ;

// 2D: A_leading VS iX
TH2D* GiveMe_2D_ALeading_iX         (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2D* GiveMe_2D_ALeading_iX         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Abin, const double& Amin, const double& Amax, const int& iXbin, const double& iXmin, const double& iXmax) ;

// 2D: A_subleading / A_leading VS iX
TH2D* GiveMe_2D_AsubRelative_iX     (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TH2D* GiveMe_2D_AsubRelative_iX     (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Abin, const double& Amin, const double& Amax, const int& iXbin, const double& iXmin, const double& iXmax) ;

// 2D: A2/A_leading VS A3/A_leading
TH2D* GiveMe_2D_A21vsA31            (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage) ;
TH2D* GiveMe_2D_A21vsA31            (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage, const int& A31bin, const double& A31min, const double& A31max, const int& A21bin, const double& A21min, const double& A21max) ;


//////////////////////////////////////////////////// TProfile /////////////////////////////////////////////////

// Rho vs YT
TProfile* GiveMe_Rho_YT             (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TProfile* GiveMe_Rho_YT             (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax) ;

// Rho VS YTYpad for PV2 (rho = A_i / A_leading)
TProfile* GiveMe_RhoPV2_YT          (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TProfile* GiveMe_RhoPV2_YT          (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax) ;

// Rho VS YT - Ypad
TProfile* GiveMe_Rho_YTYpad         (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TProfile* GiveMe_Rho_YTYpad         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax) ;

// Rho VS YTYpad for PV2 (rho = A_i / A_leading)
TProfile* GiveMe_RhoPV2_YTYpad      (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
TProfile* GiveMe_RhoPV2_YTYpad      (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax) ;

// Rho VS DeltaT
TProfile* GiveMe_Rho_DeltaT         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Procedure) ;
TProfile* GiveMe_Rho_DeltaT         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Procedure, const int& NeighbourNumber) ;
TProfile* GiveMe_Rho_DeltaT         (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Procedure, const int& NeighbourNumber, const int& DTbin, const int& DTmin, const int& DTmax, const double& rhomin, const double& rhomax) ;

// DeltaT VS Yrelat
TProfile* GiveMe_DeltaT_YTYpad      (Sample& aSample, const int& ModuleNber, const std::string& TAG) ;
