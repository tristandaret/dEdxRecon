#include "SignalShape/SignalTime_Exact_AlphaPRF.h"

SignalTime_Exact_AlphaPRF::SignalTime_Exact_AlphaPRF(
                        SignalTime_Global_Exact* pSignalTime_Global_Exact 
){
  p_SignalTime_Global_Exact = pSignalTime_Global_Exact ;
}

SignalTime_Exact_AlphaPRF::~SignalTime_Exact_AlphaPRF()
{
}

double SignalTime_Exact_AlphaPRF::Eval(const double& vX, const double& vRC )
{
 return ( p_SignalTime_Global_Exact->Eval_AlphaPRF(vX,vRC) ) ; 
}

double SignalTime_Exact_AlphaPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vRC = par[0] ;
  return Eval(vX,vRC) ;

}
