#/**
 * @file ParabolaFunction.h
 * @brief Parabola helper function declaration.
 *
 * Declares ParabolaFunction which implements a simple parabola wrapper used
 * for fitting and curve approximation. A TF1-callable operator() is provided
 * for integration with ROOT if needed.
 */
#ifndef ParabolaFunction_H
#define ParabolaFunction_H

#include "Misc.h"

//
// Fit parabola -std::fabs(A0) * std::pow(X0-vX,2)+ Y0 );

/////////////////////////////////////////////////////////////
class ParabolaFunction {
public:
  /** Constructor */
  ParabolaFunction();
  virtual ~ParabolaFunction();

  double Get_A0();
  double Get_X0();
  double Get_Y0();
  void Set_A0(const double &A0);
  void Set_X0(const double &X0);
  void Set_Y0(const double &Y0);

  double operator()(double *x, double *par);

private:
  double m_A0;
  double m_X0;
  double m_Y0;
};

#endif
