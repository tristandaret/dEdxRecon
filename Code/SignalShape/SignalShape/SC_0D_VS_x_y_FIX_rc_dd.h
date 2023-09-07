#ifndef SC_0D_VS_x_y_FIX_rc_dd_H
#define SC_0D_VS_x_y_FIX_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_FIX_y_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_FIX_rc_dd {
public:
   /** Constructor */
   SC_0D_VS_x_y_FIX_rc_dd(const std::string& LoadFile);
   SC_0D_VS_x_y_FIX_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_0D_VS_x_y_FIX_rc_dd();
   
//Evaluate Amplitudes, vX in cm, vY in cm
   double Eval_A_NN (const double& vX, const double& vY) ;
   double Eval_A_NW (const double& vX, const double& vY) ;
   double Eval_A_NE (const double& vX, const double& vY) ;

   double Eval_A_LE (const double& vX, const double& vY) ;
   double Eval_A_WW (const double& vX, const double& vY) ;
   double Eval_A_EE (const double& vX, const double& vY) ;

   double Eval_A_SS (const double& vX, const double& vY) ;
   double Eval_A_SW (const double& vX, const double& vY) ;
   double Eval_A_SE (const double& vX, const double& vY) ;
   
//Evaluate Time, vX in cm, vY in cm
   double Eval_T_NN (const double& vX, const double& vY) ;
   double Eval_T_NW (const double& vX, const double& vY) ;
   double Eval_T_NE (const double& vX, const double& vY) ;

   double Eval_T_LE (const double& vX, const double& vY) ;
   double Eval_T_WW (const double& vX, const double& vY) ;
   double Eval_T_EE (const double& vX, const double& vY) ;

   double Eval_T_SS (const double& vX, const double& vY) ;
   double Eval_T_SW (const double& vX, const double& vY) ;
   double Eval_T_SE (const double& vX, const double& vY) ;

private:
  void Load(const std::string& LoadFile);
  
  int m_NberOfFiles ;               
  std::vector < double >             V_Y  ;
  std::vector < std::string >        V_File ;
  std::vector < SC_0D_VS_x_FIX_y_rc_dd* > V_SC_0D_VS_x_FIX_y_rc_dd      ;
  
  double m_Y_Lowest  ;
  double m_Y_Highest ;
  
  std::string m_LoadFile ;
  
};



#endif

