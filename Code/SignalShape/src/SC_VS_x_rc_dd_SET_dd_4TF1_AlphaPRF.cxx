#include "SignalShape/SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF.h"

SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF::SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF(
                        SC_VS_x_rc_dd* pSC_VS_x_rc_dd ,
                        const double& vDD
){
  p_SC_VS_x_rc_dd = pSC_VS_x_rc_dd ;
  m_DD = vDD;
}

SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF::~SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF()
{
}

double SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF::Eval(const double& vX, const double& vRC )
{
 return ( p_SC_VS_x_rc_dd->Eval_AlphaPRF(vX,vRC,m_DD) ) ; 
}

double SC_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vRC = par[0] ;
  return Eval(vX,vRC);

}
