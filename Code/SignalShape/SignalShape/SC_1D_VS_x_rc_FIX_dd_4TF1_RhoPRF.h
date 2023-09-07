#ifndef SC_1D_VS_x_rc_FIX_dd_4TF1_RhoPRF_H
#define SC_1D_VS_x_rc_FIX_dd_4TF1_RhoPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_FIX_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_FIX_dd_4TF1_RhoPRF {
public:
   /** Constructor */
   SC_1D_VS_x_rc_FIX_dd_4TF1_RhoPRF(
                        SC_1D_VS_x_rc_FIX_dd* pSC_1D_VS_x_rc_FIX_dd 
                       );
   virtual ~SC_1D_VS_x_rc_FIX_dd_4TF1_RhoPRF();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2
   double Eval(const double& vX, const double& vRC  ) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_1D_VS_x_rc_FIX_dd* p_SC_1D_VS_x_rc_FIX_dd ;

};



#endif

