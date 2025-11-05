#/***************************************************************************
# * File: ParabolaFunction.h
# * Project: dEdxRecon
# *
# * Brief: Declaration of a simple parabola function helper used for fitting
# *        and curve approximation. Provides a small callable wrapper compatible
# *        with ROOT TF1 if needed.
# *
# * Contents: ParabolaFunction class declaration.
# *
# * Notes: Implementation in ParabolaFunction.cxx.
# ***************************************************************************/

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
