#include "ParabolaFunction.h"

ParabolaFunction::ParabolaFunction()
{
  m_A0 = 1. ;
  m_X0 = 0. ;
  m_Y0 = 1. ;
}

ParabolaFunction::~ParabolaFunction()
{
}


double ParabolaFunction::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  
  m_A0 = par[0] ;
  m_X0 = par[1] ;
  m_Y0 = par[2] ;
  
  return  ( -std::fabs(m_A0) * std::pow(m_X0-vX,2)+ m_Y0 ) ;

}

double ParabolaFunction::Get_A0(){ return m_A0 ; }
double ParabolaFunction::Get_X0(){ return m_X0 ; }
double ParabolaFunction::Get_Y0(){ return m_Y0 ; }

void ParabolaFunction::Set_A0(const double& A0){ m_A0 = A0 ; }
void ParabolaFunction::Set_X0(const double& X0){ m_X0 = X0 ; }
void ParabolaFunction::Set_Y0(const double& Y0){ m_Y0 = Y0 ; }
