#include "GaussFunction.h"

GaussFunction::GaussFunction()
{
   m_Norm = 1.;
   m_Mean = 0.;
   m_Sigma = 1.;
}

GaussFunction::~GaussFunction() {}

double GaussFunction::operator()(double *x, double *par)
{
   double vX = x[0];

   m_Norm = par[0];
   m_Mean = par[1];
   m_Sigma = par[2];

   return (m_Norm * exp(-0.5 * (pow((vX - m_Mean) / m_Sigma, 2))) /
           (m_Sigma * std::sqrt(2 * M_PI)));
}

double GaussFunction::Get_Norm()
{
   return m_Norm;
}
double GaussFunction::Get_Mean()
{
   return m_Mean;
}
double GaussFunction::Get_Sigma()
{
   return m_Sigma;
}

void GaussFunction::Set_Norm(const double &Norm)
{
   m_Norm = Norm;
}
void GaussFunction::Set_Mean(const double &Mean)
{
   m_Mean = Mean;
}
void GaussFunction::Set_Sigma(const double &Sigma)
{
   m_Sigma = Sigma;
}
