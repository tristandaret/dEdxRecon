#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T.h"

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        std::string Direction
){
  p_SC_0D_VS_x_y_rc_dd_SET_rc_dd = pSC_0D_VS_x_y_rc_dd_SET_rc_dd ;
  m_Direction = Direction ;
  if (pSC_0D_VS_x_y_rc_dd_SET_rc_dd->ValidPad(Direction)==0){
    std::cout 
      << " SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T " << " WARNING! "
      << std::endl ;
    std::cout 
      << " Unknown m_Direction = " << m_Direction
      << std::endl ;
    abort() ;
  }
}

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T::~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T()
{
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T::Eval(const double& vX,const double& vY)
{
  return p_SC_0D_VS_x_y_rc_dd_SET_rc_dd->Eval_T(m_Direction,vX,vY) ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_T::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vY  = x[1]   ;
  return Eval(vX,vY) ;

}
