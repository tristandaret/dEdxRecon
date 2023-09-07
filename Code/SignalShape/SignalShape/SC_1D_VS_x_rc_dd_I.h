#ifndef SC_1D_VS_x_rc_dd_I_H
#define SC_1D_VS_x_rc_dd_I_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd_I {
public:
   /** Constructor */
   SC_1D_VS_x_rc_dd_I(const std::string& LoadFile);
   SC_1D_VS_x_rc_dd_I(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_1D_VS_x_rc_dd_I();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   virtual double Eval_RhoPRF    (const double& vX, const double& vRC , const double& vDDin ) = 0 ;
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   virtual double Eval_AlphaPRF  (const double& vX, const double& vRC , const double& vDDin ) = 0 ;
   
//Evaluate TauPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   virtual double Eval_TauPRF    (const double& vX, const double& vRC , const double& vDDin ) = 0 ;
   virtual double Eval_TauBinPRF (const double& vX, const double& vRC , const double& vDDin ) = 0 ;
   
//Evaluate TimeAbsPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   virtual double Eval_TimeAbsPRF(const double& vX, const double& vRC , const double& vDDin ) = 0 ;
   
//Evaluate Amplitude, vX in cm, vRC in ns mm^-2, vDD in mm
   virtual double Eval_Amplitude(const double& vX, const double& vRC , const double& vDDin ) = 0 ;

};



#endif

