#/***************************************************************************
 * File: GaussFunction.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of a Gaussian function wrapper compatible with ROOT's
 *        TF1. Provides parameter accessors and a callable operator that
 *        returns the Gaussian value for a given x and parameter set.
 *
 * Contents: class GaussFunction with getters/setters and operator().
 *
 * Notes: Implementation in GaussFunction.cxx.
 ***************************************************************************/

#ifndef GaussFunction_H
#define GaussFunction_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class GaussFunction {
public:
  /** Constructor */
  GaussFunction();
  virtual ~GaussFunction();

  double Get_Norm();
  double Get_Mean();
  double Get_Sigma();
  void Set_Norm(const double &Norm);
  void Set_Mean(const double &Mean);
  void Set_Sigma(const double &Sigma);

  double operator()(double *x, double *par);

private:
  double m_Norm;
  double m_Mean;
  double m_Sigma;
};

#endif
