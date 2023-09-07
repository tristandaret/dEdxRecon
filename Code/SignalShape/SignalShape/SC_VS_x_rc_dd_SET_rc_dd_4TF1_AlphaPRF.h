#ifndef SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF_H
#define SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SC_VS_x_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF {
public:
   /** Constructor */
// vRC in ns mm^-2, vDD in mm
   SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF(
                        SC_VS_x_rc_dd* pSC_VS_x_rc_dd ,
                        const double& vRC             ,
                        const double& vDD
                       );
   virtual ~SC_VS_x_rc_dd_SET_rc_dd_4TF1_AlphaPRF();
   
//Evaluate AlphaPRF, vX in cm
   double Eval(const double& vX) ;
   
   double operator() (double *x, double *par) ;

private:

  SC_VS_x_rc_dd* p_SC_VS_x_rc_dd ;
  double m_RC ;
  double m_DD ;

};



#endif

