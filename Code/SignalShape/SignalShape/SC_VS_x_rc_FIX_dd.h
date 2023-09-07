#ifndef SC_VS_x_rc_FIX_dd_H
#define SC_VS_x_rc_FIX_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_VS_x_FIX_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_VS_x_rc_FIX_dd {
public:
   /** Constructor */
   SC_VS_x_rc_FIX_dd(const std::string& LoadFile);
   SC_VS_x_rc_FIX_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_VS_x_rc_FIX_dd();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2
   double Eval_RhoPRF    (const double& vX, const double& vRC ) ;
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2
   double Eval_AlphaPRF  (const double& vX, const double& vRC ) ;
   
//Evaluate TauPRF, vX in cm, vRC in ns mm^-2
   double Eval_TauPRF    (const double& vX, const double& vRC ) ;
   double Eval_TauBinPRF (const double& vX, const double& vRC ) ;
   
//Evaluate TimeAbsPRF, vX in cm, vRC in ns mm^-2
   double Eval_TimeAbsPRF(const double& vX, const double& vRC ) ;
   
//Evaluate Amplitude, vX in cm, vRC in ns mm^-2
   double Eval_Amplitude(const double& vX, const double& vRC ) ;

private:
  void Load(const std::string& LoadFile);
  
  int m_NberOfFiles ;               
  std::vector < double >             V_RC   ;
  std::vector < std::string >        V_File ;
  std::vector < SC_VS_x_FIX_rc_dd* > V_SC_VS_x_FIX_rc_dd      ;
  
  double m_RC_Lowest  ;
  double m_RC_Highest ;
  
  std::string m_LoadFile ;
  double m_s2bin ;
  
};



#endif

