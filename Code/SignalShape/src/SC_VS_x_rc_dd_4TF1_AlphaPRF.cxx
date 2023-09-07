#include "SignalShape/SC_VS_x_rc_dd_4TF1_AlphaPRF.h"

SC_VS_x_rc_dd_4TF1_AlphaPRF::SC_VS_x_rc_dd_4TF1_AlphaPRF(
                        SC_VS_x_rc_dd* pSC_VS_x_rc_dd 
){
  p_SC_VS_x_rc_dd = pSC_VS_x_rc_dd ;
}

SC_VS_x_rc_dd_4TF1_AlphaPRF::~SC_VS_x_rc_dd_4TF1_AlphaPRF()
{
}

double SC_VS_x_rc_dd_4TF1_AlphaPRF::Eval(const double& vX, const double& vRC , const double& vDD )
{
 return ( p_SC_VS_x_rc_dd->Eval_AlphaPRF(vX,vRC,vDD) ) ; 
}

double SC_VS_x_rc_dd_4TF1_AlphaPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vRC = par[0] ;
  double vDD = par[1] ;
  return Eval(vX,vRC,vDD);

}
