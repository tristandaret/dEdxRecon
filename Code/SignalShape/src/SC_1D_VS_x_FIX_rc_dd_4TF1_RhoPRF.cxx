#include "SignalShape/SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF.h"

SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF::SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF(
                        SC_1D_VS_x_FIX_rc_dd* pSC_1D_VS_x_FIX_rc_dd 
){
  p_SC_1D_VS_x_FIX_rc_dd = pSC_1D_VS_x_FIX_rc_dd ;
}

SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF::~SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF()
{
}

double SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF::Eval(const double& vX)
{
 return ( p_SC_1D_VS_x_FIX_rc_dd->Eval_RhoPRF(vX) ) ; 
}

double SC_1D_VS_x_FIX_rc_dd_4TF1_RhoPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  return Eval(vX) ;

}
