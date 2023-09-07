#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX.h"

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        const std::vector< std::string >& V_Direction , 
                        const double& Var_Min_X      , 
                        const double& Var_Max_X      , 
                        const double& Eps_X          ,
                        const double& Var_Min_Y      , 
                        const double& Var_Max_Y      , 
                        const double& Eps_Y          
 ){
  p_SC_0D_VS_x_y_rc_dd_SET_rc_dd = pSC_0D_VS_x_y_rc_dd_SET_rc_dd ;

  int iTem_Max= V_Direction.size() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    std::string Direction = V_Direction[iTem] ;
    m_V_Direction.push_back(Direction) ;
    if (pSC_0D_VS_x_y_rc_dd_SET_rc_dd->ValidPad(Direction)==0){
      std::cout 
        << " SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX::SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX "
        << std::endl ;
      std::cout 
        << " Unknown m_Direction = " << Direction
        << std::endl ;
      abort() ;
    }
  }

  m_Var_Min_X = Var_Min_X ;
  m_Var_Max_X = Var_Max_X ;
  m_Eps_X     = Eps_X     ;
  m_Var_Min_Y = Var_Min_Y ;
  m_Var_Max_Y = Var_Max_Y ;
  m_Eps_Y     = Eps_Y     ;
} 

SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX::~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX()
{
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX::Eval(const double& vX,const double& vY)
{
 double A = 0. ;
 double B = 0. ;
 double C = 0. ;
 p_SC_0D_VS_x_y_rc_dd_SET_rc_dd->Eval_CMat(
                                             m_V_Direction    , 
                                             vX               , 
                                             vY               ,
                                             m_Var_Min_X      , 
                                             m_Var_Max_X      , 
                                             m_Eps_X          ,
                                             m_Var_Min_Y      , 
                                             m_Var_Max_Y      , 
                                             m_Eps_Y          ,
                                             A                ,     
                                             B                ,     
                                             C                  
                                            );

  return ( std::sqrt(A)*1E-2/1E-6 ) ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX::operator() (double *x, double *par)
{
  double vX  = x[0]   ;
  double vY  = x[1]   ;
  return Eval(vX,vY) ;

}
