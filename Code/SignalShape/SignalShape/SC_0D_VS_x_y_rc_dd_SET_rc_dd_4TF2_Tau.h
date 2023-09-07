#ifndef SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_H
#define SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau {
public:
   /** Constructor */
   SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        std::string Direction
                       );
   virtual ~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau();
   
//Evaluate Amplitudes
   double Eval(const double& vX,const double& vY) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_0D_VS_x_y_rc_dd_SET_rc_dd* p_SC_0D_VS_x_y_rc_dd_SET_rc_dd ;
  std::string m_Direction ;

};



#endif

