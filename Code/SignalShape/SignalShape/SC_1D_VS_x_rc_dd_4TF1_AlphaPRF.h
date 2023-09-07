#ifndef SC_1D_VS_x_rc_dd_4TF1_AlphaPRF_H
#define SC_1D_VS_x_rc_dd_4TF1_AlphaPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd_4TF1_AlphaPRF {
public:
   /** Constructor */
   SC_1D_VS_x_rc_dd_4TF1_AlphaPRF(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd 
                       );
   virtual ~SC_1D_VS_x_rc_dd_4TF1_AlphaPRF();
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval(const double& vX, const double& vRC , const double& vDD ) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_1D_VS_x_rc_dd* p_SC_1D_VS_x_rc_dd ;

};



#endif

