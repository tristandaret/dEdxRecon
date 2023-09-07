#include "Misc/Misc.h"
#include "Misc/Util.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Cluster.h"

#include "SignalShape/SignalTime_Exact_RhoPRF.h"
#include "SignalShape/SignalTime_Exact_AlphaPRF.h"


// PRF PV2: LUT
TF1* GiveMe_PRF_PV2 (Sample& aSample, const int& ModuleNber , SignalTime_Exact_AlphaPRF& aSignalTime_Exact_AlphaPRF, double& RC, double& err_RC, const int EditParam) ;
TF1* PRF_PV2_iter   (Sample& aSample, const int& ModuleNber , const std::string& TAG, SignalTime_Exact_AlphaPRF& aSignalTime_Exact_AlphaPRF, double& RC, double& err_RC, TGraphErrors* ptge_RC, const int& zDrift, const int& iIter) ;

// PRF PV1: RC
TF1* GiveMe_PRF_PV1 (TGraph* tgrRhoYTYpad, SignalTime_Exact_RhoPRF& aSignalTime_Exact_RhoPRF, double& RC, double& err_RC, const int EditParam) ;
TF1* PRF_PV1_iter   (Sample& aSample, const int& ModuleNber , const std::string& TAG, SignalTime_Exact_RhoPRF& aSignalTime_Exact_RhoPRF, double& RC, double& err_RC, TGraphErrors* ptge_RC, const int& zDrift, const int& iIter) ;

// PRF PV0: polynomial function
TF1* GiveMe_PRF_PV0 (TGraph* tgrRhoYTYpad, double& Norm, double& a2, double& a4, double& b2, double& b4, const int EditParam) ;
TF1* PRF_PV0_iter   (Sample& aSample, const int& ModuleNber , const std::string& TAG, double& Norm, double& a2, double& a4, double& b2, double& b4, const int& iIter) ;


// TH1 of residual distribution for a given column 
TH1D* GiveMe_Residual_Distribution          (Sample& aSample, const int& ModuleNber , const std::string& TAG, const int& iIter, const int& iX) ;

// Get chi² min distribution of residuals
TH1D* GiveMe_chi2min_residual_distribution  (Sample& aSample, const int& ModuleNber , const std::string& TAG, const int& iIter) ;

// Fit column residual 
TF1* GiveMe_Residual_Fit                    (TH1D* ph1Residual) ;

// Fit resolution as a function of drifting distance
void MakeMe_Resolution_Fit                  (TGraphErrors* ptge_resolution) ;

// Fit (straight line) of Yw for the Track Alignment selection
TF1* GiveMe_FitYwforSelection                   (TGraphErrors* ptge) ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get chi^{2}
// double GiveMe_Chi2 (Cluster* pCluster, const TF1* PRF_func) ;

// // TH2D to make vary chi^{2} to visualize 
// TH2D* GiveMe_Chi2_Variations (Cluster* pCluster, const TF1* PRF_func, const double& deltaY, const int& Nsteps) ;
// TH2D* GiveMe_Chi2_Variations (Cluster* pCluster, const TF1* PRF_func, const double& deltaY, const int& Nsteps, const int& chibin, const double& chimin, const double& chimax) ;

// // Finding Yw that minimizes chi²
// double GiveMe_YT_minimized (Cluster* pCluster, const TF1* PRF_func, const double& deltaY, const int& Nsteps) ;

// // Setting the newly found YT values
// void PRF_polynom_iter_TD(Sample& aSample, const int& ModuleNber , const std::string& TAG, const double& deltaY, const int& Nsteps, double& Norm, double& a2, double& a4, double& b2, double& b4) ;
