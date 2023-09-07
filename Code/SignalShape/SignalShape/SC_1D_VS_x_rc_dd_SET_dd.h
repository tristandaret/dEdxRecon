#ifndef SC_1D_VS_x_rc_dd_SET_dd_H
#define SC_1D_VS_x_rc_dd_SET_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd_SET_dd {
public:
   /** Constructor */
// vDD in mm
   SC_1D_VS_x_rc_dd_SET_dd(
                        SC_1D_VS_x_rc_dd* pSC_1D_VS_x_rc_dd ,
                        const double& vDD
                       );
   virtual ~SC_1D_VS_x_rc_dd_SET_dd();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2
   double Eval_RhoPRF    (const double& vX, const double& vRC ) ;
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2
   double Eval_AlphaPRF   (const double& vX, const double& vRC ) ;
   
//Evaluate TauPRF, vX in cm, vRC in ns mm^-2
   double Eval_TauPRF   (const double& vX, const double& vRC ) ;
   double Eval_TauBinPRF(const double& vX, const double& vRC ) ;
   
//Evaluate TimeAbsPRF, vX in cm, vRC in ns mm^-2
   double Eval_TimeAbsPRF  (const double& vX, const double& vRC ) ;
    
//Evaluate Amplitude, vX in cm, vRC in ns mm^-2
   double Eval_Amplitude  (const double& vX, const double& vRC ) ;
  

private:

  SC_1D_VS_x_rc_dd* p_SC_1D_VS_x_rc_dd ;
  double m_DD ;

};



#endif

