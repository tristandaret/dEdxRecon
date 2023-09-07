#ifndef SC_1D_VS_x_rc_dd_H
#define SC_1D_VS_x_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_1D_VS_x_rc_dd_I.h"
#include "SignalShape/SC_1D_VS_x_rc_FIX_dd.h"

/////////////////////////////////////////////////////////////
class SC_1D_VS_x_rc_dd : public SC_1D_VS_x_rc_dd_I  {
public:
   /** Constructor */
   SC_1D_VS_x_rc_dd(const std::string& LoadFile);
   SC_1D_VS_x_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_1D_VS_x_rc_dd();
   
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

//vX in cm
   double A_PRF(const double& vX, const double& vRC , const double& vDDin );
   double T_PRF(const double& vX, const double& vRC , const double& vDDin );

   double Rho_PRF   (const double& vX, const double& vRC , const double& vDDin );
   double Alpha_PRF (const double& vX, const double& vRC , const double& vDDin );
   double Tau_PRF   (const double& vX, const double& vRC , const double& vDDin );
   double TauBin_PRF(const double& vX, const double& vRC , const double& vDDin );

//
   double Rho_DW (const double& vX, const double& vRC , const double& vDDin );
   double Rho_LE (const double& vX, const double& vRC , const double& vDDin );
   double Rho_UP (const double& vX, const double& vRC , const double& vDDin );

   double Alpha_DW (const double& vX, const double& vRC , const double& vDDin );
   double Alpha_UP (const double& vX, const double& vRC , const double& vDDin );

   double Tau_DW (const double& vX, const double& vRC , const double& vDDin );
   double Tau_UP (const double& vX, const double& vRC , const double& vDDin );
   double TauBin_DW (const double& vX, const double& vRC , const double& vDDin );
   double TauBin_UP (const double& vX, const double& vRC , const double& vDDin );

   double Am_DW (const double& vX, const double& vRC , const double& vDDin );
   double Am_LE (const double& vX, const double& vRC , const double& vDDin );
   double Am_UP (const double& vX, const double& vRC , const double& vDDin );
   double Ti_DW (const double& vX, const double& vRC , const double& vDDin );
   double Ti_LE (const double& vX, const double& vRC , const double& vDDin );
   double Ti_UP (const double& vX, const double& vRC , const double& vDDin );

   double Am_DW_Base(const double& vX, const double& vRC , const double& vDDin );
   double Am_LE_Base(const double& vX, const double& vRC , const double& vDDin );
   double Am_UP_Base(const double& vX, const double& vRC , const double& vDDin );
   double Ti_DW_Base(const double& vX, const double& vRC , const double& vDDin );
   double Ti_LE_Base(const double& vX, const double& vRC , const double& vDDin );
   double Ti_UP_Base(const double& vX, const double& vRC , const double& vDDin );

private:
  void Load(const std::string& LoadFile);
  
  int m_NberOfFiles ;               
  std::vector < double >             V_DD  ;
  std::vector < std::string >        V_File ;
  std::vector < SC_1D_VS_x_rc_FIX_dd* > V_SC_1D_VS_x_rc_FIX_dd      ;
  
  double m_DD_Lowest  ;
  double m_DD_Highest ;
  
  std::string m_LoadFile ;
  double m_s2bin ;
  
};



#endif

