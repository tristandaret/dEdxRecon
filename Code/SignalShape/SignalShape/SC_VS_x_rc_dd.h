#ifndef SC_VS_x_rc_dd_H
#define SC_VS_x_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd_I.h"
#include "SignalShape/SC_VS_x_rc_FIX_dd.h"

/////////////////////////////////////////////////////////////
class SC_VS_x_rc_dd : public SC_1D_VS_x_rc_dd_I  {
public:
   /** Constructor */
   SC_VS_x_rc_dd(const std::string& LoadFile);
   SC_VS_x_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_VS_x_rc_dd();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval_RhoPRF    (const double& vX, const double& vRC , const double& vDDin ) ;
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval_AlphaPRF  (const double& vX, const double& vRC , const double& vDDin ) ;
   
//Evaluate TauPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval_TauPRF    (const double& vX, const double& vRC , const double& vDDin ) ;
   double Eval_TauBinPRF (const double& vX, const double& vRC , const double& vDDin ) ;
   
//Evaluate TimeAbsPRF, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval_TimeAbsPRF(const double& vX, const double& vRC , const double& vDDin ) ;
   
//Evaluate Amplitude, vX in cm, vRC in ns mm^-2, vDD in mm
   double Eval_Amplitude(const double& vX, const double& vRC , const double& vDDin ) ;

private:
  void Load(const std::string& LoadFile);
  
  int m_NberOfFiles ;               
  std::vector < double >             V_DD  ;
  std::vector < std::string >        V_File ;
  std::vector < SC_VS_x_rc_FIX_dd* > V_SC_VS_x_rc_FIX_dd      ;
  
  double m_DD_Lowest  ;
  double m_DD_Highest ;
  
  std::string m_LoadFile ;
  double m_s2bin ;
  
};



#endif

