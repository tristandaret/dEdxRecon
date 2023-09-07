#ifndef SC_0D_VS_x_y_rc_dd_SET_rc_dd_H
#define SC_0D_VS_x_y_rc_dd_SET_rc_dd_H

#include "Misc/Misc.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd.h"

/////////////////////////////////////////////////////////////
class SC_0D_VS_x_y_rc_dd_SET_rc_dd {
public:
   /** Constructor */
// vRC in ns mm^-2, vDD in mm
   SC_0D_VS_x_y_rc_dd_SET_rc_dd(
                        SC_0D_VS_x_y_rc_dd* pSC_0D_VS_x_y_rc_dd ,
                        const double& vRC   ,
                        const double& vDD   
                       );
   virtual ~SC_0D_VS_x_y_rc_dd_SET_rc_dd();

//
  int ValidPad(const std::string& Direction) ;
   
//Evaluate Amplitudes, vX in cm, vY in cm
   double Eval_A(const std::string& Direction, const double& vX, const double& vY ) ;
   
   double Eval_A_NN (const double& vX, const double& vY ) ;
   double Eval_A_NW (const double& vX, const double& vY ) ;
   double Eval_A_NE (const double& vX, const double& vY ) ;

   double Eval_A_LE (const double& vX, const double& vY ) ;
   double Eval_A_WW (const double& vX, const double& vY ) ;
   double Eval_A_EE (const double& vX, const double& vY ) ;

   double Eval_A_SS (const double& vX, const double& vY ) ;
   double Eval_A_SW (const double& vX, const double& vY ) ;
   double Eval_A_SE (const double& vX, const double& vY ) ;

//Evaluate Times, vX in cm, vY in cm
   double Eval_T    (const std::string& Direction, const double& vX, const double& vY ) ;
   
   double Eval_T_NN (const double& vX, const double& vY ) ;
   double Eval_T_NW (const double& vX, const double& vY ) ;
   double Eval_T_NE (const double& vX, const double& vY ) ;

   double Eval_T_LE (const double& vX, const double& vY ) ;
   double Eval_T_WW (const double& vX, const double& vY ) ;
   double Eval_T_EE (const double& vX, const double& vY ) ;

   double Eval_T_SS (const double& vX, const double& vY ) ;
   double Eval_T_SW (const double& vX, const double& vY ) ;
   double Eval_T_SE (const double& vX, const double& vY ) ;
   
//Evaluate Times, vX in cm, vY in cm
   double Eval_Tbin    (const std::string& Direction, const double& vX, const double& vY ) ; 
   
   double Eval_Tbin_NN (const double& vX, const double& vY ) ;
   double Eval_Tbin_NW (const double& vX, const double& vY ) ;
   double Eval_Tbin_NE (const double& vX, const double& vY ) ;

   double Eval_Tbin_LE (const double& vX, const double& vY ) ;
   double Eval_Tbin_WW (const double& vX, const double& vY ) ;
   double Eval_Tbin_EE (const double& vX, const double& vY ) ;

   double Eval_Tbin_SS (const double& vX, const double& vY ) ;
   double Eval_Tbin_SW (const double& vX, const double& vY ) ;
   double Eval_Tbin_SE (const double& vX, const double& vY ) ;
   
//Evaluate Amplitudes, vX in cm, vY in cm
   double Eval_Alpha    (const std::string& Direction, const double& vX, const double& vY ) ;
   double Eval_dAlpha   (
                         const std::string& Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min        , 
                         const double& Var_Max        , 
                         const double& Eps            ,
                         const int&    iOptXY
                        );
   void Eval_Alpha_WMat       (
                         const std::string& Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   void Eval_Alpha_WMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   void Eval_Alpha_CMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   
   double Eval_Alpha_NN (const double& vX, const double& vY ) ;
   double Eval_Alpha_NW (const double& vX, const double& vY ) ;
   double Eval_Alpha_NE (const double& vX, const double& vY ) ;

   double Eval_Alpha_LE (const double& vX, const double& vY ) ;
   double Eval_Alpha_WW (const double& vX, const double& vY ) ;
   double Eval_Alpha_EE (const double& vX, const double& vY ) ;

   double Eval_Alpha_SS (const double& vX, const double& vY ) ;
   double Eval_Alpha_SW (const double& vX, const double& vY ) ;
   double Eval_Alpha_SE (const double& vX, const double& vY ) ;

//Evaluate Times, vX in cm, vY in cm
   double Eval_Tau    (const std::string& Direction, const double& vX, const double& vY ) ;
   double Eval_dTau   (
                       const std::string& Direction , 
                       const double& vX             , 
                       const double& vY             ,
                       const double& Var_Min        , 
                       const double& Var_Max        , 
                       const double& Eps            ,
                       const int&    iOptXY
                      );
   void Eval_Tau_WMat       (
                         const std::string& Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   void Eval_Tau_WMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   void Eval_Tau_CMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   
   void Eval_WMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   void Eval_CMat       (
                         const std::vector< std::string >& V_Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min_X      , 
                         const double& Var_Max_X      , 
                         const double& Eps_X          ,
                         const double& Var_Min_Y      , 
                         const double& Var_Max_Y      , 
                         const double& Eps_Y          ,
                         double& A                    ,     
                         double& B                    ,     
                         double& C                      
                        );
   
   double Eval_Tau_NN (const double& vX, const double& vY ) ;
   double Eval_Tau_NW (const double& vX, const double& vY ) ;
   double Eval_Tau_NE (const double& vX, const double& vY ) ;

   double Eval_Tau_LE (const double& vX, const double& vY ) ;
   double Eval_Tau_WW (const double& vX, const double& vY ) ;
   double Eval_Tau_EE (const double& vX, const double& vY ) ;

   double Eval_Tau_SS (const double& vX, const double& vY ) ;
   double Eval_Tau_SW (const double& vX, const double& vY ) ;
   double Eval_Tau_SE (const double& vX, const double& vY ) ;
   

private:

  SC_0D_VS_x_y_rc_dd* p_SC_0D_VS_x_y_rc_dd ;
  double m_RC ;
  double m_DD ;
  
  double m_AmpCut ;

};



#endif

