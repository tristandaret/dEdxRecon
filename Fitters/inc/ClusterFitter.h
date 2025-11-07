/**
 * @file ClusterFitter.h
 * @brief Declarations for cluster-level fitters used to fit pad-cluster
 *        charge distributions.
 *
 * This header defines both horizontal and diagonal cluster fitter classes
 * and related helper functions to run fits on Samples, Events and Clusters.
 * Implementations live in ClusterFitter.cxx.
 *
 * @details
 * The corresponding implementation uses ROOT's TVirtualFitter/TMinuit and
 * registers file-local static bridge classes that forward Minuit callbacks
 * to the instance methods (Chi2 evaluation). The source contains both the
 * horizontal and diagonal fitter implementations and several helper
 * functions to apply fitters to Samples, Events and individual Clusters.
 */
#ifndef CLUSTER_FIT_H
#define CLUSTER_FIT_H

#include "Misc.h"

#include "Cluster.h"
#include "Event.h"
#include "Sample.h"

#include "TMath.h"
#include "TMatrixD.h"
#include "TMinuit.h"
#include "TVirtualFitter.h"

#include "TF1.h"

// HORIZONTAL FITTER
// Fit Basic:	MIGRAD/MINOS
//
// Fit Rescues:
//	1: MIGRAD->MINMIZE/MIGRAD
//	2: MIGRAD->SIMPLEX/MIGRAD
//
// Fit failure -> Failure procedure called
//
class ClusterFitter_Horizontal {
public:
  /**
   * @brief Construct a horizontal cluster fitter
   * @param FitterName name used to identify the fitter instance
   */
  ClusterFitter_Horizontal(const std::string &FitterName);

  /**
   * @brief Virtual destructor
   */
  virtual ~ClusterFitter_Horizontal();

public:
  /**
   * @brief Set the cluster to be fitted
   * @param pCluster pointer to the Cluster (ownership not transferred)
   */
  void Set_Cluster(Cluster *pCluster);

  /**
   * @brief Run minimisation for the current cluster
   * @return 0 on success, non-zero on failure
   */
  int DoMinimisation();

  /**
   * @brief Compute chi2 for given parameters
   * @param par parameter array for chi2 evaluation
   * @return chi2 value
   */
  double Chi2(double par[]);

protected:
  std::string m_FitterName;
  Cluster *p_Cluster;
  TVirtualFitter *p_TVirtualFitter; //!< Fitter
};

void ClusterFit_Horizontal(Sample &aSample, const int &ModuleNber,
                           TF1 *pTF1_ToBeUsed);
void ClusterFit_Horizontal_Event(
    Event *pEvent, const int &ModuleNber, TF1 *pTF1_ToBeUsed,
    ClusterFitter_Horizontal &aClusterFitter_Horizontal);
void ClusterFit_Horizontal_Cluster(
    Cluster *pCluster, const int &ModuleNber, TF1 *pTF1_ToBeUsed,
    ClusterFitter_Horizontal &aClusterFitter_Horizontal);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DIAGONAL FITTER
// Fit Basic:	MIGRAD/MINOS
//
// Fit Rescues:
//	1: MIGRAD->MINMIZE/MIGRAD
//	2: MIGRAD->SIMPLEX/MIGRAD
//
// Fit failure -> Failure procedure called
//
class ClusterFitter_Diagonal {
public:
  /**
   * @brief Construct a diagonal cluster fitter
   * @param FitterName name used to identify the fitter instance
   */
  ClusterFitter_Diagonal(const std::string &FitterName);

  /**
   * @brief Virtual destructor
   */
  virtual ~ClusterFitter_Diagonal();

public:
  /**
   * @brief Set the cluster to be fitted
   * @param pCluster pointer to the Cluster (ownership not transferred)
   */
  void Set_Cluster(Cluster *pCluster);

  /**
   * @brief Run minimisation for the current diagonal cluster
   * @return 0 on success, non-zero on failure
   */
  int DoMinimisation();

  /**
   * @brief Compute chi2 for given parameters
   * @param par parameter array for chi2 evaluation
   * @return chi2 value
   */
  double Chi2(double par[]);

protected:
  std::string m_FitterName;
  Cluster *p_Cluster;
  TVirtualFitter *p_TVirtualFitter; //!< Fitter
};

//-------------------------------------Cluster
// Fits-------------------------------/
// Cluster Fits

void ClusterFit_Diagonal(const double &AngleRot, Sample &aSample,
                         const int &ModuleNber, TF1 *pTF1_ToBeUsed);
void ClusterFit_Diagonal_Event(const double &AngleRot, Event *pEvent,
                               const int &ModuleNber, TF1 *pTF1_ToBeUsed,
                               int &Kounter_Fit, int &Kounter_Failure,
                               ClusterFitter_Diagonal &aClusterFitter_Diagonal);
void ClusterFit_Diagonal_Cluster(
    const double &AngleRot, Cluster *pCluster, const int &ModuleNber,
    TF1 *pTF1_ToBeUsed, int &Kounter_Fit, int &Kounter_Failure,
    ClusterFitter_Diagonal &aClusterFitter_Diagonal);

#endif