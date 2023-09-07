#ifndef SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX_H
#define SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX {
public:
   /** Constructor */
   SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        const std::vector< std::string >& V_Direction , 
                        const double& Var_Min_X      , 
                        const double& Var_Max_X      , 
                        const double& Eps_X          ,
                        const double& Var_Min_Y      , 
                        const double& Var_Max_Y      , 
                        const double& Eps_Y          
                       );
   virtual ~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX();
   
//Evaluate Amplitudes derivative
   double Eval(const double& vX,const double& vY) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_0D_VS_x_y_rc_dd_SET_rc_dd* p_SC_0D_VS_x_y_rc_dd_SET_rc_dd ;
  std::vector < std::string > m_V_Direction ;
  double m_Var_Min_X  ;
  double m_Var_Max_X  ;
  double m_Eps_X      ;
  double m_Var_Min_Y  ;
  double m_Var_Max_Y  ;
  double m_Eps_Y      ;

};



#endif

