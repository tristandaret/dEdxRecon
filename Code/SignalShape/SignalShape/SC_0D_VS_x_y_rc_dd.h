#ifndef SC_0D_VS_x_y_rc_dd_H
#define SC_0D_VS_x_y_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_FIX_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd {
public:
   /** Constructor */
   SC_0D_VS_x_y_rc_dd(const std::string& LoadFile);
   SC_0D_VS_x_y_rc_dd(const std::string& LoadFile,const double& s2bin);
   virtual ~SC_0D_VS_x_y_rc_dd();
   
//Evaluate Amplitudes, vX in cm, vY in cm, vRC in ns mm^-2, vDD in mm
   double Eval_A_NN (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_NW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_NE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_A_LE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_WW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_EE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_A_SS (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_SW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_A_SE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

//Evaluate Times, vX in cm, vY in cm, vRC in ns mm^-2, vDD in mm
   double Eval_T_NN (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_NW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_NE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_T_LE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_WW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_EE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_T_SS (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_SW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_T_SE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

//Evaluate Times, vX in cm, vY in cm, vRC in ns mm^-2, vDD in mm
   double Eval_Tbin_NN (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_NW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_NE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_Tbin_LE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_WW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_EE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

   double Eval_Tbin_SS (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_SW (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;
   double Eval_Tbin_SE (const double& vX, const double& vY, const double& vRC , const double& vDDin ) ;

private:
  void Load(const std::string& LoadFile);
  
  int m_NberOfFiles ;               
  std::vector < double >             V_DD  ;
  std::vector < std::string >        V_File ;
  std::vector < SC_0D_VS_x_y_rc_FIX_dd* > V_SC_0D_VS_x_y_rc_FIX_dd      ;
  
  double m_DD_Lowest  ;
  double m_DD_Highest ;
  
  std::string m_LoadFile ;
  double m_s2bin ;
  
};



#endif

