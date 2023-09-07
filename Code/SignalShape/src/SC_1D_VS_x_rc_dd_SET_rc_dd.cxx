#include "SignalShape/SC_1D_VS_x_rc_dd_SET_rc_dd.h"

SC_1D_VS_x_rc_dd_SET_rc_dd::SC_1D_VS_x_rc_dd_SET_rc_dd(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd ,
                        const double& vRC   ,
                        const double& vDD   
){
  p_SC_1D_VS_x_rc_dd = pSC_1D_VS_x_rc_dd ;
  m_RC = vRC;
  m_DD = vDD;
}

SC_1D_VS_x_rc_dd_SET_rc_dd::~SC_1D_VS_x_rc_dd_SET_rc_dd()
{
}

double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_RhoPRF     (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_RhoPRF    (vX,m_RC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_AlphaPRF   (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_AlphaPRF  (vX,m_RC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_TauPRF     (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TauPRF    (vX,m_RC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_TauBinPRF  (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TauBinPRF (vX,m_RC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_TimeAbsPRF (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_TimeAbsPRF(vX,m_RC,m_DD) ) ; }
double SC_1D_VS_x_rc_dd_SET_rc_dd::Eval_Amplitude  (const double& vX ) { return ( p_SC_1D_VS_x_rc_dd->Eval_Amplitude (vX,m_RC,m_DD) ) ; }
