#include "PRFParameters.h"

PRFParameters::PRFParameters() {}

PRFParameters::~PRFParameters() {}

double PRFParameters::Eval(const double &Xin) {
  double ToBeReturned = 0.;
  double X2 = Xin * Xin;
  double X4 = X2 * X2;
  ToBeReturned =
      m_Norm * (1. + m_A2 * X2 + m_A4 * X4) / (1. + m_B2 * X2 + m_B4 * X4);

  return ToBeReturned;
}

void PRFParameters::SetPRF(const double &Norm, const double &A2,
                           const double &A4, const double &B2,
                           const double &B4) {
  m_Norm = Norm;
  m_A2 = A2;
  m_A4 = A4;
  m_B2 = B2;
  m_B4 = B4;
}

double PRFParameters::operator()(double *x, double *par) {
  double xx = x[0];
  SetPRF(par[0], par[1], par[2], par[3], par[4]);
  return Eval(xx);
}
