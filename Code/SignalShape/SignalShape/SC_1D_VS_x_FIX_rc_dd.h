#ifndef SC_1D_VS_x_FIX_rc_dd_H
#define SC_1D_VS_x_FIX_rc_dd_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_FIX_rc_dd {
public:
   /** Constructor */
   SC_1D_VS_x_FIX_rc_dd(const std::string& LoadFile);
   SC_1D_VS_x_FIX_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_1D_VS_x_FIX_rc_dd();
   
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


//vX in cm
   double A_PRF(const double& vX );
   double T_PRF(const double& vX );

   double Rho_PRF   (const double& vX );
   double Alpha_PRF (const double& vX );
   double Tau_PRF   (const double& vX );
   double TauBin_PRF(const double& vX );

//
   double Rho_DW (const double& vX );
   double Rho_LE (const double& vX );
   double Rho_UP (const double& vX );

   double Alpha_DW (const double& vX );
   double Alpha_UP (const double& vX );

   double Tau_DW (const double& vX );
   double Tau_UP (const double& vX );
   double TauBin_DW (const double& vX );
   double TauBin_UP (const double& vX );

   double Am_DW (const double& vX );
   double Am_LE (const double& vX );
   double Am_UP (const double& vX );
   double Ti_DW (const double& vX );
   double Ti_LE (const double& vX );
   double Ti_UP (const double& vX );

   double Am_DW_Base(const double& vX );
   double Am_LE_Base(const double& vX );
   double Am_UP_Base(const double& vX );
   double Ti_DW_Base(const double& vX );
   double Ti_LE_Base(const double& vX );
   double Ti_UP_Base(const double& vX );
//
private:
  void Load(const std::string& LoadFile);
  
  std::vector< double > V_Y ;
  
  std::vector< double > V_A_DW ;
  std::vector< double > V_A_LE ;
  std::vector< double > V_A_UP ;
  std::vector< double > V_T_DW ;
  std::vector< double > V_T_LE ;
  std::vector< double > V_T_UP ;

  double m_LY        ;
  double m_LY_incm   ;
  double m_Y_Lowest  ;
  double m_Y_Highest ;
  
  double m_A_DW_Default ;
  double m_A_LE_Default ;
  double m_A_UP_Default ;
  double m_T_DW_Default ;
  double m_T_LE_Default ;
  double m_T_UP_Default ;

  std::string m_LoadFile ;
  double m_s2bin ;
  
  int m_FirstCall ;
  
};



#endif

