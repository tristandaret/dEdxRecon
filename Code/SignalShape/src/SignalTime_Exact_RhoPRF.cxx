#include "SignalShape/SignalTime_Exact_RhoPRF.h"

SignalTime_Exact_RhoPRF::SignalTime_Exact_RhoPRF(
                        SignalTime_Global_Exact* pSignalTime_Global_Exact 
){
  p_SignalTime_Global_Exact = pSignalTime_Global_Exact ;
}

SignalTime_Exact_RhoPRF::~SignalTime_Exact_RhoPRF()
{
}

double SignalTime_Exact_RhoPRF::Eval(const double& vX, const double& vRC )
{
 return ( p_SignalTime_Global_Exact->Eval_RhoPRF(vX,vRC) ) ; 
}

double SignalTime_Exact_RhoPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vRC = par[0] ;
  return Eval(vX,vRC) ;

}
