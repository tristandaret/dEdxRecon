#/***************************************************************************
 * File: ParabolaFunctionNG.cxx
 * Project: dEdxRecon
 *
 * Brief: Implementation of a non-symmetric parabola function (NG = non-
 *        gaussian?) used for fits where left/right curvatures differ.
 *
 * Contents: ParabolaFunctionNG operator() and accessor methods.
 *
 * Notes: See ParabolaFunctionNG.h for details on parameters.
 ***************************************************************************/

#include "ParabolaFunctionNG.h"

ParabolaFunctionNG::ParabolaFunctionNG() {
  m_A0P = 1.;
  m_A0M = 1.;
  m_X0 = 0.;
  m_Y0 = 1.;
}

ParabolaFunctionNG::~ParabolaFunctionNG() {}

double ParabolaFunctionNG::operator()(double *x, double *par) {
  double vX = x[0];

  m_A0P = par[0];
  m_A0M = par[1];
  m_X0 = par[2];
  m_Y0 = par[3];

  if (vX >= m_X0) {
    return (-std::fabs(m_A0P) * std::pow(m_X0 - vX, 2) + m_Y0);
  }
  return (-std::fabs(m_A0M) * std::pow(m_X0 - vX, 2) + m_Y0);
}

double ParabolaFunctionNG::Get_A0P() { return m_A0P; }
double ParabolaFunctionNG::Get_A0M() { return m_A0M; }
double ParabolaFunctionNG::Get_X0() { return m_X0; }
double ParabolaFunctionNG::Get_Y0() { return m_Y0; }

void ParabolaFunctionNG::Set_A0P(const double &A0P) { m_A0P = A0P; }
void ParabolaFunctionNG::Set_A0M(const double &A0M) { m_A0M = A0M; }
void ParabolaFunctionNG::Set_X0(const double &X0) { m_X0 = X0; }
void ParabolaFunctionNG::Set_Y0(const double &Y0) { m_Y0 = Y0; }
