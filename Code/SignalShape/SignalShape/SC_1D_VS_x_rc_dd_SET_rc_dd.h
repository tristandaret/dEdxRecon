#ifndef SC_1D_VS_x_rc_dd_SET_rc_dd_H
#define SC_1D_VS_x_rc_dd_SET_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd_SET_rc_dd {
public:
   /** Constructor */
// vRC in ns mm^-2, vDD in mm
   SC_1D_VS_x_rc_dd_SET_rc_dd(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd ,
                        const double& vRC   ,
                        const double& vDD   
                       );
   virtual ~SC_1D_VS_x_rc_dd_SET_rc_dd();
   
//Evaluate RhoPRF, vX in cm
   double Eval_RhoPRF    (const double& vX ) ;
   
//Evaluate AlphaPRF, vX in cm
   double Eval_AlphaPRF   (const double& vX ) ;
   
//Evaluate TauPRF, vX in cm
   double Eval_TauPRF   (const double& vX ) ;
   double Eval_TauBinPRF(const double& vX ) ;
   
//Evaluate TimeAbsPRF, vX in cm
   double Eval_TimeAbsPRF  (const double& vX ) ;
   
//Evaluate Amplitude, vX in cm
   double Eval_Amplitude  (const double& vX ) ;
   

private:

  SC_1D_VS_x_rc_dd* p_SC_1D_VS_x_rc_dd ;
  double m_RC ;
  double m_DD ;

};



#endif

