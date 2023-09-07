#ifndef SC_1D_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF_H
#define SC_1D_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF {
public:
   /** Constructor */
// vDD in mm
   SC_1D_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd ,
                        const double& vDD
                       );
   virtual ~SC_1D_VS_x_rc_dd_SET_dd_4TF1_AlphaPRF();
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2
   double Eval(const double& vX, const double& vRC  ) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_1D_VS_x_rc_dd* p_SC_1D_VS_x_rc_dd ;
  double m_DD ;

};



#endif

