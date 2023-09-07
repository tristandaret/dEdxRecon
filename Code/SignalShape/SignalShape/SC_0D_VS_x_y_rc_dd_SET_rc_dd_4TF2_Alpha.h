#ifndef SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_H
#define SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha {
public:
   /** Constructor */
   SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha(
                        SC_0D_VS_x_y_rc_dd_SET_rc_dd* pSC_0D_VS_x_y_rc_dd_SET_rc_dd,
                        std::string Direction
                       );
   virtual ~SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha();
   
//Evaluate Amplitudes
   double Eval(const double& vX,const double& vY) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_0D_VS_x_y_rc_dd_SET_rc_dd* p_SC_0D_VS_x_y_rc_dd_SET_rc_dd ;
  std::string m_Direction ;

};



#endif

