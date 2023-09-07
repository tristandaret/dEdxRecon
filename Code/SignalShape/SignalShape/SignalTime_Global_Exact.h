#ifndef SignalTime_Global_Exact_H
#define SignalTime_Global_Exact_H

#include "Misc/Misc.h"
#include "SignalShape/SignalTime_Individual_Exact.h"

/////////////////////////////////////////////////////////////
class SignalTime_Global_Exact {
public:
   /** Constructor */
   SignalTime_Global_Exact(const std::string& LoadFile);
   virtual ~SignalTime_Global_Exact();
   
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
  int m_NberOfFiles ;
  std::vector < double >                       V_RC   ;
  std::vector < std::string >                  V_File ;
  std::vector < SignalTime_Individual_Exact* > V_SignalTime_Individual_Exact      ;
  
  double m_RC_Lowest  ;
  double m_RC_Highest ;
  
  std::string m_LoadFile ;
  double m_s2bin ;
  
};



#endif

