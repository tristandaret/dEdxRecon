#include "SignalShape/SC_VS_x_rc_FIX_dd_4TF1_RhoPRF.h"

SC_VS_x_rc_FIX_dd_4TF1_RhoPRF::SC_VS_x_rc_FIX_dd_4TF1_RhoPRF(
                        SC_VS_x_rc_FIX_dd* pSC_VS_x_rc_FIX_dd 
){
  p_SC_VS_x_rc_FIX_dd = pSC_VS_x_rc_FIX_dd ;
}

SC_VS_x_rc_FIX_dd_4TF1_RhoPRF::~SC_VS_x_rc_FIX_dd_4TF1_RhoPRF()
{
}

double SC_VS_x_rc_FIX_dd_4TF1_RhoPRF::Eval(const double& vX, const double& vRC )
{
 return ( p_SC_VS_x_rc_FIX_dd->Eval_RhoPRF(vX,vRC) ) ; 
}

double SC_VS_x_rc_FIX_dd_4TF1_RhoPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vRC = par[0] ;
  return Eval(vX,vRC) ;

}
