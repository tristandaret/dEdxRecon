#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau.h"

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        const std::string& Direction ,
                        const double& Var_Min , 
                        const double& Var_Max , 
                        const double& Eps     ,
                        const int&    iOptXY
){
  p_SC_0D_VS_x_y_rc_dd_SET_rc_dd = pSC_0D_VS_x_y_rc_dd_SET_rc_dd ;

  m_Direction = Direction ;
  if (pSC_0D_VS_x_y_rc_dd_SET_rc_dd->ValidPad(Direction)==0){
    std::cout 
      << " SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau "
      << std::endl ;
    std::cout 
      << " Unknown m_Direction = " << m_Direction
      << std::endl ;
    abort() ;
  }

  m_Var_Min = Var_Min ;
  m_Var_Max = Var_Max ;
  m_Eps     = Eps     ;
  m_iOptXY  = iOptXY  ;
  if ( m_iOptXY!=0 && m_iOptXY!=1 ){
    std::cout << " SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2 " << std::endl ;
    std::cout << " Unknown  m_iOptXY = " << m_iOptXY << std::endl ;
    abort() ;
  }
} 

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau()
{
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::Eval(const double& vX,const double& vY)
{
  return ( 
          p_SC_0D_VS_x_y_rc_dd_SET_rc_dd->Eval_dTau(
                                                      m_Direction , 
                                                      vX        , 
                                                      vY        ,
                                                      m_Var_Min , 
                                                      m_Var_Max , 
                                                      m_Eps     ,
                                                      m_iOptXY
                                                     )
  );
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vY  = x[1]   ;
  return Eval(vX,vY) ;

}
