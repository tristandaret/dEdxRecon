#ifndef SignalTime_Individual_Exact_H
#define SignalTime_Individual_Exact_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class SignalTime_Individual_Exact {
public:
   /** Constructor */
   SignalTime_Individual_Exact(
                std::string SignalTime_Individual_Exact_File
               );
   virtual ~SignalTime_Individual_Exact();
   
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

  std::string m_SignalTime_Individual_Exact_File ;
  
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

  double m_s2bin ;
  
};



#endif

