#include "SignalShape/SC_1D_VS_x_rc_dd_SET_dd.h"

SC_1D_VS_x_rc_dd_SET_dd::SC_1D_VS_x_rc_dd_SET_dd(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd ,
                        const double& vDD
){
  p_SC_1D_VS_x_rc_dd = pSC_1D_VS_x_rc_dd ;
  m_DD = vDD;
}

SC_1D_VS_x_rc_dd_SET_dd::~SC_1D_VS_x_rc_dd_SET_dd()
{
}

double SC_1D_VS_x_rc_dd_SET_dd::Eval_RhoPRF     (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_RhoPRF    (vX,vRC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_dd::Eval_AlphaPRF   (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_AlphaPRF  (vX,vRC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_dd::Eval_TauPRF     (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TauPRF    (vX,vRC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_dd::Eval_TauBinPRF  (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TauBinPRF (vX,vRC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_dd::Eval_TimeAbsPRF (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TimeAbsPRF(vX,vRC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_dd::Eval_Amplitude  (const double& vX, const double& vRC ) { return ( p_SC_1D_VS_x_rc_dd->Eval_Amplitude (vX,vRC,m_DD) ) ; }
