#ifndef SC_0D_VS_x_FIX_y_rc_dd_H
#define SC_0D_VS_x_FIX_y_rc_dd_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_FIX_y_rc_dd {
public:
   /** Constructor */
   SC_0D_VS_x_FIX_y_rc_dd(const std::string& LoadFile);
   virtual ~SC_0D_VS_x_FIX_y_rc_dd();
   
//Evaluate Amplitudes, vX in cm 
   double Eval_A_NN (const double& vX ) ;
   double Eval_A_NW (const double& vX ) ;
   double Eval_A_NE (const double& vX ) ;

   double Eval_A_LE (const double& vX ) ;
   double Eval_A_WW (const double& vX ) ;
   double Eval_A_EE (const double& vX ) ;

   double Eval_A_SS (const double& vX ) ;
   double Eval_A_SW (const double& vX ) ;
   double Eval_A_SE (const double& vX ) ;

//Evaluate Times, vX in cm 
   double Eval_T_NN (const double& vX ) ;
   double Eval_T_NW (const double& vX ) ;
   double Eval_T_NE (const double& vX ) ;

   double Eval_T_LE (const double& vX ) ;
   double Eval_T_WW (const double& vX ) ;
   double Eval_T_EE (const double& vX ) ;

   double Eval_T_SS (const double& vX ) ;
   double Eval_T_SW (const double& vX ) ;
   double Eval_T_SE (const double& vX ) ;

private:
  void Load(const std::string& LoadFile);
  
  std::vector< double > V_X ;
  
  std::vector< double > V_A_SW ;
  std::vector< double > V_A_SS ;
  std::vector< double > V_A_SE ;
  std::vector< double > V_A_WW ;
  std::vector< double > V_A_LE ;
  std::vector< double > V_A_EE ;
  std::vector< double > V_A_NW ;
  std::vector< double > V_A_NN ;
  std::vector< double > V_A_NE ;

  std::vector< double > V_T_SW ;
  std::vector< double > V_T_SS ;
  std::vector< double > V_T_SE ;
  std::vector< double > V_T_WW ;
  std::vector< double > V_T_LE ;
  std::vector< double > V_T_EE ;
  std::vector< double > V_T_NW ;
  std::vector< double > V_T_NN ;
  std::vector< double > V_T_NE ;

  double m_X_Lowest            ;
  double m_X_Highest           ;
  
  double m_V_A_SW_Default ;
  double m_V_A_SS_Default ;
  double m_V_A_SE_Default ;
  double m_V_A_WW_Default ;
  double m_V_A_LE_Default ;
  double m_V_A_EE_Default ;
  double m_V_A_NW_Default ;
  double m_V_A_NN_Default ;
  double m_V_A_NE_Default ;
  
  double m_V_T_SW_Default ;
  double m_V_T_SS_Default ;
  double m_V_T_SE_Default ;
  double m_V_T_WW_Default ;
  double m_V_T_LE_Default ;
  double m_V_T_EE_Default ;
  double m_V_T_NW_Default ;
  double m_V_T_NN_Default ;
  double m_V_T_NE_Default ;

  std::string m_LoadFile ;
  
  int m_FirstCall ;
  
};



#endif

