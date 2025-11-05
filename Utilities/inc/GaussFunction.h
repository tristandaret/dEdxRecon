#/**
 * @file GaussFunction.h
 * @brief Gaussian function wrapper declaration.
 *
 * Declares GaussFunction which provides getters/setters and a TF1-compatible
 * operator() to evaluate a Gaussian with configurable normalization,
 * mean and sigma.
 */
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
