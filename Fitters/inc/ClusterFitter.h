#/***************************************************************************
 * File: ClusterFitter.h
 * Project: dEdxRecon
 *
 * Brief: Declarations for cluster-level fitters used to fit pad-cluster
 *        charge distributions. This header defines both horizontal and
 *        diagonal cluster fitter classes and related convenience functions
 *        to run fits on Samples, Events and individual Clusters.
 *
 * Contents: ClusterFitter_Horizontal, ClusterFitter_Diagonal, and helper
 *           functions ClusterFit_* for different geometries.
 *
 * Notes: Implementations live in ClusterFitter.cxx. Fitting uses ROOT's
 *        TVirtualFitter/TMinuit and TF1 for model functions.
 ***************************************************************************/

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
  ClusterFitter_Horizontal(const std::string &FitterName);
  virtual ~ClusterFitter_Horizontal();

public:
  ///////////////////////////////////

  void Set_Cluster(Cluster *pCluster);

  int DoMinimisation();

  double Chi2(double par[]);

protected:
  ///////////////////////////////////

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
  ClusterFitter_Diagonal(const std::string &FitterName);
  virtual ~ClusterFitter_Diagonal();

public:
  ///////////////////////////////////

  void Set_Cluster(Cluster *pCluster);

  int DoMinimisation();

  double Chi2(double par[]);

protected:
  ///////////////////////////////////

  std::string m_FitterName;

  Cluster *p_Cluster;

  TVirtualFitter *p_TVirtualFitter; //!< Fitter
};

//-------------------------------------Cluster
//Fits-------------------------------/
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