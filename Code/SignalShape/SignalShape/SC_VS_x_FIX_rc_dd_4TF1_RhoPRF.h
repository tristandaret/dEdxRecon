#ifndef SC_VS_x_FIX_rc_dd_4TF1_RhoPRF_H
#define SC_VS_x_FIX_rc_dd_4TF1_RhoPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SC_VS_x_FIX_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_VS_x_FIX_rc_dd_4TF1_RhoPRF {
public:
   /** Constructor */
   SC_VS_x_FIX_rc_dd_4TF1_RhoPRF(
                        SC_VS_x_FIX_rc_dd* pSC_VS_x_FIX_rc_dd 
                       );
   virtual ~SC_VS_x_FIX_rc_dd_4TF1_RhoPRF();
   
//Evaluate RhoPRF, vX in cm
   double Eval(const double& vX) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_VS_x_FIX_rc_dd* p_SC_VS_x_FIX_rc_dd ;

};



#endif

