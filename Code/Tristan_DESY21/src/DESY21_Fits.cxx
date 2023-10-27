#include "Tristan_DESY21/DESY21_Fits.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "Fitters_Cluster/TheFitterCluster_PV0_PV1.h"

#include "EvtModel/Cluster.h"
#include "EvtModel/Event.h"
#include "EvtModel/Track.h"

#include "AnalysisTools/DoTracksReconstruction.h"
#include "SignalShape/SignalTime_Exact_RhoPRF.h"
#include "SignalShape/SignalTime_Exact_AlphaPRF.h"

#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "EvtModelTools/JFL_Do_SampleFit.h"

#include "TF1.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TGraphErrors.h"

#include <cmath>    // powers
#include <numeric>  // sum elements of vector

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PRF for PV2
TF1* GiveMe_PRF_PV2(Sample& aSample, const int& ModuleNber , SignalTime_Exact_AlphaPRF& aSignalTime_Exact_AlphaPRF, double& RC, double& err_RC, const int EditParam)
{
  TF1* ptf1_PRF_PV2 = new TF1("ptf1_PRF_PV2", aSignalTime_Exact_AlphaPRF, -1.5*1.019, 1.5*1.019, 1) ;
  ptf1_PRF_PV2->SetParameter(0, RC) ;
  ptf1_PRF_PV2->SetParName(0, "RC") ;
  Do_SampleFit_PV2(aSample, ModuleNber , ptf1_PRF_PV2) ;

  if(EditParam == 1){
    RC              = ptf1_PRF_PV2->GetParameter(0) ;
    err_RC          = ptf1_PRF_PV2->GetParError(0) ;
  }
  return ptf1_PRF_PV2 ; 
}


// Setting the newly found YT values with ROOT minimization
TF1* PRF_PV2_iter(Sample& aSample, const int& ModuleNber , const std::string& TAG, SignalTime_Exact_AlphaPRF& aSignalTime_Exact_AlphaPRF, double& RC, double& err_RC, TGraphErrors* ptge_RC, const int& zDrift, const int& iIter)
{
  if(iIter != 0){
    TF1* PRF_fit = GiveMe_PRF_PV2(aSample, ModuleNber ,aSignalTime_Exact_AlphaPRF, RC, err_RC, 1) ;
    Do_ClusterFit_PV2( aSample, ModuleNber , PRF_fit ) ;
    ptge_RC->SetPoint(zDrift, zDrift*100+60, RC);
    ptge_RC->SetPointError(zDrift, 0, err_RC);
  }

  TF1* PRF_fit = GiveMe_PRF_PV2(aSample, ModuleNber,  aSignalTime_Exact_AlphaPRF, RC, err_RC, 0) ;
  return PRF_fit ;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//PV1

// PRF for PV1
TF1* GiveMe_PRF_PV1(TGraph* tgrRhoYTYpad, SignalTime_Exact_RhoPRF& aSignalTime_Exact_RhoPRF, double& RC, double& err_RC, const int EditParam)
{
  TF1* ptf1_PRF_RC = new TF1("ptf1_PRF_RC" ,aSignalTime_Exact_RhoPRF, 2.5,  2.5, 1);
  ptf1_PRF_RC->SetParameter(0,RC);
  ptf1_PRF_RC->SetParName(0,"RC");
  tgrRhoYTYpad->Fit(ptf1_PRF_RC, "RQ") ; // R fit in specified range ; Q quiet printing

  if(EditParam == 1){
    RC              = ptf1_PRF_RC->GetParameter(0) ;
    err_RC          = ptf1_PRF_RC->GetParError(0) ;
  }
  return ptf1_PRF_RC ;
}



// Setting the newly found YT values with ROOT minimization
TF1* PRF_PV1_iter(Sample& aSample, const int& ModuleNber , const std::string& TAG, SignalTime_Exact_RhoPRF& aSignalTime_Exact_RhoPRF, double& RC, double& err_RC, TGraphErrors* ptge_RC, const int& zDrift, const int& iIter)
{
  if(iIter != 0){
    TH2D* ph2_Rho_YTYpad          = GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , std::string(TAG + "PRF1"), 1) ;
    TGraphErrors* ptgr_Rho_YTYpad = Convert_TH2_TGE(ph2_Rho_YTYpad) ;
    TF1* PRF_fit                  = GiveMe_PRF_PV1(ptgr_Rho_YTYpad, aSignalTime_Exact_RhoPRF, RC, err_RC, 1) ;
    TheFitterCluster_PV0_PV1 aTheFitterCluster_PV0_PV1("Minuit") ;

    int NEvents                   = aSample.Get_NberOfEvents() ;
    for(int iE = 0 ; iE < NEvents ; iE++){
      Event* pEvent               = aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      
      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];    
      
        pCluster->SetEval_PV0_PV1(PRF_fit) ;
        aTheFitterCluster_PV0_PV1.Set_Cluster(pCluster) ; 
        aTheFitterCluster_PV0_PV1.DoMinimisation() ;       // Minimizing and updating the values YT
      }
    }
    ptge_RC->SetPoint(zDrift, zDrift*100+60, RC);
    ptge_RC->SetPointError(zDrift, 0, err_RC);
  }

  TH2D* ph2_Rho_YTYpad          = GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , std::string(TAG + "PRFnull1"), 1) ;
  TGraphErrors* ptgr_Rho_YTYpad = Convert_TH2_TGE(ph2_Rho_YTYpad) ;
  TF1* PRF_fit                  = GiveMe_PRF_PV1(ptgr_Rho_YTYpad, aSignalTime_Exact_RhoPRF, RC, err_RC, 0) ;  
  delete ph2_Rho_YTYpad ;
  delete ptgr_Rho_YTYpad ;
  return PRF_fit ;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// PV0

// PRF for PV0
TF1* GiveMe_PRF_PV0(TGraph* tgrRhoYTYpad,  double& Norm,  double& a2,  double& a4,  double& b2,  double& b4, const int EditParam)
{
  TF1* PRF_Polynom = new TF1("PRF_Polynom","[0]*(1+[1]*x*x+[2]*x*x*x*x)/(1+[3]*x*x+[4]*x*x*x*x)", -2.5, 2.5) ;
  PRF_Polynom->SetParameter(0,Norm);
  PRF_Polynom->SetParameter(1,a2);
  PRF_Polynom->SetParameter(2,a4);
  PRF_Polynom->SetParameter(3,b2);
  PRF_Polynom->SetParameter(4,b4);
  PRF_Polynom->SetParName(0,"Norm");
  PRF_Polynom->SetParName(1,"a2");
  PRF_Polynom->SetParName(2,"a4");
  PRF_Polynom->SetParName(3,"b2");
  PRF_Polynom->SetParName(4,"b4");
  tgrRhoYTYpad->Fit(PRF_Polynom, "RQ") ; // R fit in specified range ; Q quiet printing
  if(EditParam == 1){
    Norm  = PRF_Polynom->GetParameter(0) ;
    a2    = PRF_Polynom->GetParameter(1) ;
    a4    = PRF_Polynom->GetParameter(2) ;
    b2    = PRF_Polynom->GetParameter(3) ;
    b4    = PRF_Polynom->GetParameter(4) ;
  }
  return PRF_Polynom ;
}


// Setting the newly found YT values with ROOT minimization
TF1* PRF_PV0_iter(Sample& aSample, const int& ModuleNber , const std::string& TAG, double& Norm, double& a2, double& a4, double& b2, double& b4, const int& iIter)
{
  if(iIter != 0){
    TH2D* ph2_Rho_YTYpad          = GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , std::string(TAG + "PRF0"), 0) ;
    TGraphErrors* ptgr_Rho_YTYpad = Convert_TH2_TGE(ph2_Rho_YTYpad) ;
    TF1* PRF_fit                  = GiveMe_PRF_PV0(ptgr_Rho_YTYpad, Norm, a2, a4, b2, b4, 1) ;
    TheFitterCluster_PV0_PV1 aTheFitterCluster_PV0_PV1("Minuit") ;

    int NEvents                   = aSample.Get_NberOfEvents() ;
    for(int iE = 0 ; iE < NEvents ; iE++){
      Event* pEvent               = aSample.Get_Event(iE) ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      
      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];
      
        pCluster->SetEval_PV0_PV1(PRF_fit) ;
        aTheFitterCluster_PV0_PV1.Set_Cluster(pCluster) ; 
        aTheFitterCluster_PV0_PV1.DoMinimisation() ;       // Minimizing and updating the values YT
      }
    }
  }

  TH2D* ph2_Rho_YTYpad          = GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , std::string(TAG + "PRFnull0"), 0) ;
  TGraphErrors* ptgr_Rho_YTYpad = Convert_TH2_TGE(ph2_Rho_YTYpad) ;
  TF1* PRF_fit                  = GiveMe_PRF_PV0(ptgr_Rho_YTYpad, Norm, a2, a4, b2, b4, 0) ;  
  
  delete ph2_Rho_YTYpad ;
  delete ptgr_Rho_YTYpad ;

  return PRF_fit ;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Plotting results (Residual, Bias, Resolution)

// TH1 of residual distribution for a given column 
TH1D* GiveMe_Residual_Distribution(Sample& aSample, const int& ModuleNber , const std::string& TAG, const int& iIter, const int& iX)
{
  std::string Title       = TAG + ": Residuals distribution for iX = " + std::to_string(iX) = ";Residual (#mum);Count" ;
  TH1D* ph1_residue       = new TH1D(TString(Form("ph1_residue_%02i_", iX) + TAG + iIter) ,Title.c_str(), 50, -1500 , 1500 ) ; 

  int NEvents             = aSample.Get_NberOfEvents() ;
  for(int iE = 0 ; iE < NEvents ; iE++){
    Event* pEvent         = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    const Track* pTrack   = pEvent->GiveMe_Track_ForThisModule(ModuleNber) ;
    int NClustersTrack    = pTrack->Get_NberOfCluster() ;
    if(NClustersTrack < 29) continue ;

    for(int iC = 0 ; iC < NClustersTrack ; iC++){
      const Cluster* pCluster= pTrack->Get_Cluster(iC) ;
      int iX_cluster         = pCluster->Get_LeadingPad()->Get_iX() ;
      if(iX_cluster == iX){
         double residual     = pTrack->Get_Residual(iC)*1000000 ; // m to µm
         ph1_residue->Fill(residual) ;
      } 
    }
  }
  return ph1_residue ;
}



// TH1 of chi2min distribution of residual
TH1D* GiveMe_chi2min_residual_distribution(Sample& aSample, const int& ModuleNber , const std::string& TAG, const int& iIter)
{
  std::string Title       = TAG + ": #chi^{2} residuals distribution;chi^{2};Count" ;
  TH1D* ph1_chi2min       = new TH1D(TString("ph1_chi2min" + TAG + iIter),Title.c_str(), 50, -100 , 2500 ) ; 

  int NEvents             = aSample.Get_NberOfEvents() ;
  for(int iE = 0 ; iE < NEvents ; iE++){
    Event* pEvent         = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    if(iIter !=0){
      const Track* pTrack = pEvent->GiveMe_Track_ForThisModule(ModuleNber) ;
      ph1_chi2min->Fill(pTrack->Get_Chi2Min()) ; 
    } 
  }
  return ph1_chi2min ;
}



// TH1 of residual distribution for a given column 
TF1* GiveMe_Residual_Fit(TH1D* ph1Residual)
{
  TF1* resi_gauss = new TF1("resi_gauss","gaus", -1000, 1000) ;
  ph1Residual->Fit(resi_gauss,"RQ") ;
  gStyle->SetOptFit(kTRUE) ;
  return resi_gauss ;
}



// Fit resolution as a function of drifting distance
void MakeMe_Resolution_Fit(TGraphErrors* ptge_resolution)
{
  TF1* ptf1_reso_fit  = new TF1("ptf1_reso_fit", "std::sqrt(std::pow([0],2)+std::pow(280,2)*(x+137.46)*0.1/[1])", 50, 870) ;
  ptf1_reso_fit->SetParName(0, "sigma0") ;
  ptf1_reso_fit->SetParName(1, "N_eff") ;
  ptf1_reso_fit->SetParameter(1, 100) ;
  ptge_resolution->Fit(ptf1_reso_fit, "R") ;
  gStyle->SetOptFit(kTRUE) ;
}



// Fit (parabola line) of Yw for the Track Alignment selection
TF1* GiveMe_FitYwforSelection(TGraphErrors* ptge)
{
  TF1* ptf1_Evt_fit = new TF1("ptf1_Evt_fit", "[0]*std::pow(x,2)+[1]*x+[2]", 0, 35) ;      
  ptf1_Evt_fit->SetParameter(0, -0.0005) ;
  ptf1_Evt_fit->SetParameter(1, -0.05) ;
  ptf1_Evt_fit->SetParameter(2, 8) ;
  ptge->Fit(ptf1_Evt_fit, "RQ") ;
  return ptf1_Evt_fit ;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
