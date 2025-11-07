#/**
 * @file PRFParameters.h
 * @brief Parameter container and evaluator for the Pad Response Function (PRF).
 *
 * Declares the PRFParameters class which stores polynomial coefficients used
 * to evaluate a PRF model used by cluster/track fitting. Implementation is in
 * PRFParameters.cxx.
 *
 * @details
 * The implementation provides Eval, SetPRF and operator() so the object can
 * be used directly as a functor with ROOT's TF1. It stores the polynomial
 * coefficients and provides a small, stable interface for fitters and
 * waveform utilities.
 */
#ifndef PRFParameters_H
#define PRFParameters_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class PRFParameters {
public:
  /**
   * @brief Default constructor
   */
  PRFParameters();

  /**
   * @brief Virtual destructor
   */
  virtual ~PRFParameters();

  /**
   * @brief Evaluate the PRF at a given position
   * @param Xin input position in cm
   * @return evaluated PRF value
   */
  double Eval(const double &Xin);

  /**
   * @brief Function-call operator used by ROOT TF1-style callbacks
   */
  double operator()(double *x, double *par);

  /**
   * @brief Set PRF polynomial parameters
   * @param Norm normalization
   * @param A2 coefficient A2
   * @param A4 coefficient A4
   * @param B2 coefficient B2
   * @param B4 coefficient B4
   */
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
