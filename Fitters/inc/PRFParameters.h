#/***************************************************************************
 * File: PRFParameters.h
 * Project: dEdxRecon
 *
 * Brief: Parameter container and evaluator for the Pad Response Function (PRF).
 *        Declares the PRFParameters class which stores polynomial coefficients
 *        used to evaluate a PRF model used by cluster/track fitting.
 *
 * Contents: class PRFParameters { Eval, SetPRF, operator() }
 *
 * Notes: Implementation is in PRFParameters.cxx. This is a lightweight POD-like
 *        helper used by fitters.
 ***************************************************************************/

#ifndef PRFParameters_H
#define PRFParameters_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class PRFParameters {
public:
  /** Constructor */
  PRFParameters();
  virtual ~PRFParameters();

  // Evaluate PRF, Xin in cm
  double Eval(const double &Xin);

  double operator()(double *x, double *par);

  void SetPRF(const double &Norm, const double &A2, const double &A4,
              const double &B2, const double &B4);

private:
  double m_Norm;
  double m_A2;
  double m_A4;
  double m_B2;
  double m_B4;
};

#endif
