#ifndef SC_VS_x_FIX_rc_dd_H
#define SC_VS_x_FIX_rc_dd_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class SC_VS_x_FIX_rc_dd {
public:
   /** Constructor */
   SC_VS_x_FIX_rc_dd(const std::string& LoadFile);
   SC_VS_x_FIX_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_VS_x_FIX_rc_dd();
   
//Evaluate RhoPRF, vX in cm
   double Eval_RhoPRF    (const double& vX ) ;
   
//Evaluate AlphaPRF, vX in cm
   double Eval_AlphaPRF  (const double& vX ) ;
   
//Evaluate TauPRF, vX in cm
   double Eval_TauPRF    (const double& vX ) ;
   double Eval_TauBinPRF (const double& vX ) ;
   
//Evaluate TimeAbsPRF, vX in cm
   double Eval_TimeAbsPRF(const double& vX ) ;
   
//Evaluate Amplitude, vX in cm
   double Eval_Amplitude(const double& vX ) ;
   
private:
  void Load(const std::string& LoadFile);
  
  std::vector< double > V_X ;
  
  std::vector< double > V_SignalPRF   ;
  std::vector< double > V_SignalnPRF  ;
  std::vector< double > V_SignalTime  ;
  std::vector< double > V_SignalTime0 ;
  std::vector< double > V_SignalAmpli ;

  double m_X_Lowest            ;
  double m_X_Highest           ;
  
  double m_SignalPRF_Default   ;
  double m_SignalnPRF_Default  ;
  double m_SignalTime_Default  ;
  double m_SignalTime0_Default ;
  double m_SignalAmpli_Default ;

  std::string m_LoadFile ;
  double m_s2bin ;
  
  int m_FirstCall ;
  
};



#endif

