#ifndef SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha_H
#define SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha {
public:
   /** Constructor */
   SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        const std::string& Direction ,
                        const double& Var_Min , 
                        const double& Var_Max , 
                        const double& Eps     ,
                        const int&    iOptXY
                       );
   virtual ~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha();
   
//Evaluate Amplitudes derivative
   double Eval(const double& vX,const double& vY) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_0D_VS_x_y_rc_dd_SET_rc_dd* p_SC_0D_VS_x_y_rc_dd_SET_rc_dd ;
  std::string m_Direction ;
  double m_Eps     ;
  double m_Var_Min ;
  double m_Var_Max ;
  int    m_iOptXY  ;

};



#endif

