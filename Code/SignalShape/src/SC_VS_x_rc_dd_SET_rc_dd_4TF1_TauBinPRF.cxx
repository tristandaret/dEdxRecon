#include "SignalShape/SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF.h"

SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF::SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF(
                        SC_VS_x_rc_dd* pSC_VS_x_rc_dd ,
                        const double& vRC             ,
                        const double& vDD              
){
  p_SC_VS_x_rc_dd = pSC_VS_x_rc_dd ;
  m_RC = vRC;
  m_DD = vDD;
}

SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF::~SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF()
{
}

double SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF::Eval(const double& vX)
{
 return p_SC_VS_x_rc_dd->Eval_TauBinPRF(vX,m_RC,m_DD)  ; 
}

double SC_VS_x_rc_dd_SET_rc_dd_4TF1_TauBinPRF::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  return Eval(vX);

}
