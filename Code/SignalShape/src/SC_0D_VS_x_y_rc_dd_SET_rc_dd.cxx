#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

SC_0D_VS_x_y_rc_dd_SET_rc_dd::SC_0D_VS_x_y_rc_dd_SET_rc_dd(
                        SC_0D_VS_x_y_rc_dd* pSC_0D_VS_x_y_rc_dd ,
                        const double& vRC   ,
                        const double& vDD   
){
  p_SC_0D_VS_x_y_rc_dd = pSC_0D_VS_x_y_rc_dd ;
  m_RC = vRC;
  m_DD = vDD;
  m_AmpCut = 70. ;
}

SC_0D_VS_x_y_rc_dd_SET_rc_dd::~SC_0D_VS_x_y_rc_dd_SET_rc_dd()
{
}

int SC_0D_VS_x_y_rc_dd_SET_rc_dd::ValidPad(const std::string& Direction)
{
  if ( 
    Direction != "SW"
  &&Direction != "SS"
  &&Direction != "SE"
  &&Direction != "WW"
  &&Direction != "LE"
  &&Direction != "EE"
  &&Direction != "NW"
  &&Direction != "NN"
  &&Direction != "NE"
  ) return 0;
  return 1 ;

}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A    (const std::string& Direction, const double& vX, const double& vY )
{
  if ( Direction == "SW" ) return Eval_A_SW(vX,vY) ;
  if ( Direction == "SS" ) return Eval_A_SS(vX,vY) ;
  if ( Direction == "SE" ) return Eval_A_SE(vX,vY) ;
  if ( Direction == "WW" ) return Eval_A_WW(vX,vY) ;
  if ( Direction == "LE" ) return Eval_A_LE(vX,vY) ;
  if ( Direction == "EE" ) return Eval_A_EE(vX,vY) ;
  if ( Direction == "NW" ) return Eval_A_NW(vX,vY) ;
  if ( Direction == "NN" ) return Eval_A_NN(vX,vY) ;
  if ( Direction == "NE" ) return Eval_A_NE(vX,vY) ;
  return 0. ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T    (const std::string& Direction, const double& vX, const double& vY )
{
  if ( Direction == "SW" ) return Eval_T_SW(vX,vY) ;
  if ( Direction == "SS" ) return Eval_T_SS(vX,vY) ;
  if ( Direction == "SE" ) return Eval_T_SE(vX,vY) ;
  if ( Direction == "WW" ) return Eval_T_WW(vX,vY) ;
  if ( Direction == "LE" ) return Eval_T_LE(vX,vY) ;
  if ( Direction == "EE" ) return Eval_T_EE(vX,vY) ;
  if ( Direction == "NW" ) return Eval_T_NW(vX,vY) ;
  if ( Direction == "NN" ) return Eval_T_NN(vX,vY) ;
  if ( Direction == "NE" ) return Eval_T_NE(vX,vY) ;
  return 0. ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin(const std::string& Direction, const double& vX, const double& vY )
{
  if ( Direction == "SW" ) return Eval_Tbin_SW(vX,vY) ;
  if ( Direction == "SS" ) return Eval_Tbin_SS(vX,vY) ;
  if ( Direction == "SE" ) return Eval_Tbin_SE(vX,vY) ;
  if ( Direction == "WW" ) return Eval_Tbin_WW(vX,vY) ;
  if ( Direction == "LE" ) return Eval_Tbin_LE(vX,vY) ;
  if ( Direction == "EE" ) return Eval_Tbin_EE(vX,vY) ;
  if ( Direction == "NW" ) return Eval_Tbin_NW(vX,vY) ;
  if ( Direction == "NN" ) return Eval_Tbin_NN(vX,vY) ;
  if ( Direction == "NE" ) return Eval_Tbin_NE(vX,vY) ;
  return 0. ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha    (const std::string& Direction, const double& vX, const double& vY )
{
  if ( Direction == "SW" ) return Eval_Alpha_SW(vX,vY) ;
  if ( Direction == "SS" ) return Eval_Alpha_SS(vX,vY) ;
  if ( Direction == "SE" ) return Eval_Alpha_SE(vX,vY) ;
  if ( Direction == "WW" ) return Eval_Alpha_WW(vX,vY) ;
  if ( Direction == "LE" ) return Eval_Alpha_LE(vX,vY) ;
  if ( Direction == "EE" ) return Eval_Alpha_EE(vX,vY) ;
  if ( Direction == "NW" ) return Eval_Alpha_NW(vX,vY) ;
  if ( Direction == "NN" ) return Eval_Alpha_NN(vX,vY) ;
  if ( Direction == "NE" ) return Eval_Alpha_NE(vX,vY) ;
  return 0. ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau    (const std::string& Direction, const double& vX, const double& vY )
{
  if ( Direction == "SW" ) return Eval_Tau_SW(vX,vY) ;
  if ( Direction == "SS" ) return Eval_Tau_SS(vX,vY) ;
  if ( Direction == "SE" ) return Eval_Tau_SE(vX,vY) ;
  if ( Direction == "WW" ) return Eval_Tau_WW(vX,vY) ;
  if ( Direction == "LE" ) return Eval_Tau_LE(vX,vY) ;
  if ( Direction == "EE" ) return Eval_Tau_EE(vX,vY) ;
  if ( Direction == "NW" ) return Eval_Tau_NW(vX,vY) ;
  if ( Direction == "NN" ) return Eval_Tau_NN(vX,vY) ;
  if ( Direction == "NE" ) return Eval_Tau_NE(vX,vY) ;
  return 0. ;
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_NN (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_NN(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_NW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_NW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_NE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_NE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_LE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_LE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_WW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_WW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_EE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_EE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_SS (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_SS(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_SW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_SW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_A_SE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_A_SE(vX,vY,m_RC,m_DD) ) ; }

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_NN (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_NN(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_NW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_NW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_NE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_NE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_LE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_LE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_WW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_WW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_EE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_EE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_SS (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_SS(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_SW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_SW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_T_SE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_T_SE(vX,vY,m_RC,m_DD) ) ; }


double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_NN (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_NN(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_NW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_NW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_NE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_NE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_LE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_LE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_WW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_WW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_EE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_EE(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_SS (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_SS(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_SW (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_SW(vX,vY,m_RC,m_DD) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tbin_SE (const double& vX, const double& vY) { return ( p_SC_0D_VS_x_y_rc_dd->Eval_Tbin_SE(vX,vY,m_RC,m_DD) ) ; }


double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_NN (const double& vX, const double& vY) { return ( Eval_A_NN(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_NW (const double& vX, const double& vY) { return ( Eval_A_NW(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_NE (const double& vX, const double& vY) { return ( Eval_A_NE(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_LE (const double& vX, const double& vY) { return ( Eval_A_LE(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_WW (const double& vX, const double& vY) { return ( Eval_A_WW(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_EE (const double& vX, const double& vY) { return ( Eval_A_EE(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_SS (const double& vX, const double& vY) { return ( Eval_A_SS(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_SW (const double& vX, const double& vY) { return ( Eval_A_SW(vX,vY)/Eval_A_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_SE (const double& vX, const double& vY) { return ( Eval_A_SE(vX,vY)/Eval_A_LE(vX,vY) ) ; }

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_NN (const double& vX, const double& vY) { return ( Eval_Tbin_NN(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_NW (const double& vX, const double& vY) { return ( Eval_Tbin_NW(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_NE (const double& vX, const double& vY) { return ( Eval_Tbin_NE(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_LE (const double& vX, const double& vY) { return ( Eval_Tbin_LE(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WW (const double& vX, const double& vY) { return ( Eval_Tbin_WW(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_EE (const double& vX, const double& vY) { return ( Eval_Tbin_EE(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_SS (const double& vX, const double& vY) { return ( Eval_Tbin_SS(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_SW (const double& vX, const double& vY) { return ( Eval_Tbin_SW(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }
double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_SE (const double& vX, const double& vY) { return ( Eval_Tbin_SE(vX,vY)-Eval_Tbin_LE(vX,vY) ) ; }


double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_dAlpha(
                         const std::string& Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min        , 
                         const double& Var_Max        , 
                         const double& Eps            ,
                         const int&    iOptXY
)
{
  double Xcur = 0. ; double Ycur = 0.;
  
  Xcur = vX ; Ycur = vY;
  double FC = Eval_Alpha(Direction,Xcur,Ycur) ;
   
  double FD   = 0.    ;
  double dVar = Eps ;
  if (iOptXY==0){
    Xcur = vX + Eps ; Ycur = vY ;
    if ( Xcur > Var_Max ) Xcur = vX - Eps ;
    if ( Xcur < Var_Min ) Xcur = vX + Eps ;
    dVar = Xcur - vX ;
    FD = Eval_Alpha(Direction,Xcur,Ycur) ;
  }
  if (iOptXY==1){
    Xcur = vX ; Ycur = vY + Eps ;
    if ( Ycur > Var_Max ) Ycur = vY - Eps ;
    if ( Ycur < Var_Min ) Ycur = vY + Eps ;
    dVar = Ycur - vY ;
    FD = Eval_Alpha(Direction,Xcur,Ycur) ;
  }
  
  return ( (FD-FC)/dVar );
}

double SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_dTau(
                         const std::string& Direction , 
                         const double& vX             , 
                         const double& vY             ,
                         const double& Var_Min        , 
                         const double& Var_Max        , 
                         const double& Eps            ,
                         const int&    iOptXY
)
{
  double Xcur = 0. ; double Ycur = 0.;
  
  Xcur = vX ; Ycur = vY;
  double FC = Eval_Tau(Direction,Xcur,Ycur) ;
   
  double FD   = 0.    ;
  double dVar = Eps*100.*2./50. ;
  if (iOptXY==0){
    Xcur = vX + Eps*100.*2./50. ; Ycur = vY ;
    if ( Xcur > Var_Max ) Xcur = vX - Eps*100.*2./50.  ;
    if ( Xcur < Var_Min ) Xcur = Var_Min ;
    dVar = Xcur - vX ;
    FD = Eval_Tau(Direction,Xcur,Ycur) ;
  }
  if (iOptXY==1){
    Xcur = vX ; Ycur = vY + Eps*100.*2./50. ;
    if ( Ycur > Var_Max ) Ycur = vY - Eps*100.*2./50. ;
    if ( Ycur < Var_Min ) Ycur = Var_Min ;
    dVar = Ycur - vY ;
    FD = Eval_Tau(Direction,Xcur,Ycur) ;
  }
  
  return ( (FD-FC)/dVar );
}

void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_WMat       (
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
){
  double Dx = Eval_dAlpha (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,0);
  double Dy = Eval_dAlpha (Direction, vX, vY,Var_Min_Y,Var_Max_Y,Eps_Y,1);
  
  double Alpha = Eval_Alpha (Direction,vX,vY ) ;
  double NUM   = Eval_A     (Direction,vX,vY ) ;
  double DEN   = Eval_A     ("LE"     ,vX,vY ) ;
  
  double Er = Alpha *std::sqrt( std::fabs(1./NUM) + std::fabs(1./DEN) );
  
  Er = Er/4. ;
  
  Dx = Dx /Er ;
  Dy = Dy /Er ;
  
  if (Dx!=Dx) std::cout << " SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_WMat Dx NAN!!" ;
  if (Dy!=Dy) std::cout << " SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_WMat Dy NAN!!" ;
  
  A = Dx*Dx ;
  B = Dy*Dy ;
  C = Dx*Dy ;
}
void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_WMat       (
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
){
  A = 0. ;
  B = 0. ;
  C = 0. ;
  
  int iTem_Max= V_Direction.size() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    std::string Direction = V_Direction[iTem] ;
    double Amplitude = Eval_A(Direction,vX,vY) ;
    if (Amplitude<m_AmpCut) continue ;
    double AL = 0. ;
    double BL = 0. ;
    double CL = 0. ;
    Eval_Alpha_WMat (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
    A += AL ;
    B += BL ;
    C += CL ;
  }
  A += 12. ;
  B += 12. ;
  
}

void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Alpha_CMat       (
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
){
  double AL = 0. ;
  double BL = 0. ;
  double CL = 0. ;
  Eval_Alpha_WMat (V_Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
  
  double Det = AL*BL-CL*CL ;
  
  A = BL/Det ;
  B = AL/Det ;
  C =-CL/Det ;

}

void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WMat       (
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
){
  double Dx = Eval_dTau (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,0);
  double Dy = Eval_dTau (Direction, vX, vY,Var_Min_Y,Var_Max_Y,Eps_Y,1);
  
//   std::cout << "SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WMat" << std::endl;
//   std::cout << " Direction " << Direction << std::endl;
//   std::cout << " Dx " << Dx << std::endl;
//   std::cout << " Dy " << Dy << std::endl;

  double Er = std::sqrt(2.); ;
    
  Dx = Dx /Er ;
  Dy = Dy /Er ;
  
  if (Dx!=Dx) std::cout << " SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WMat Dx NAN!!" ;
  if (Dy!=Dy) std::cout << " SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WMat Dy NAN!!" ;
  
  A = Dx*Dx ;
  B = Dy*Dy ;
  C = Dx*Dy ;
}
void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_WMat       (
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
){
  A = 0. ;
  B = 0. ;
  C = 0. ;
  
  int iTem_Max= V_Direction.size() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    std::string Direction = V_Direction[iTem] ;
    double Amplitude = Eval_A(Direction,vX,vY) ;
    if (Amplitude<m_AmpCut) continue ;
    double AL = 0. ;
    double BL = 0. ;
    double CL = 0. ;
    Eval_Tau_WMat (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
    A += AL ;
    B += BL ;
    C += CL ;
  }
  A += 12. ;
  B += 12. ;
  
  
}

void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_CMat       (
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
){
  double AL = 0. ;
  double BL = 0. ;
  double CL = 0. ;
  Eval_Tau_WMat (V_Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
  
  double Det = AL*BL-CL*CL ;
  
//   std::cout << "SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_Tau_CMat" << std::endl;
//   std::cout << " AL " << AL << std::endl;
//   std::cout << " BL " << BL << std::endl;
//   std::cout << " CL " << CL << std::endl;

  A = BL/Det ;
  B = AL/Det ;
  C =-CL/Det ;

}
void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_WMat       (
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
){
  A = 0. ;
  B = 0. ;
  C = 0. ;
  
  int iTem_Max= V_Direction.size() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    std::string Direction = V_Direction[iTem] ;
    double Amplitude = Eval_A(Direction,vX,vY) ;
    if (Amplitude<m_AmpCut) continue ;
    double AL = 0. ;
    double BL = 0. ;
    double CL = 0. ;
    Eval_Alpha_WMat (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
    A += AL ;
    B += BL ;
    C += CL ;
  }
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    std::string Direction = V_Direction[iTem] ;
    double Amplitude = Eval_A(Direction,vX,vY) ;
    if (Amplitude<m_AmpCut) continue ;
    double AL = 0. ;
    double BL = 0. ;
    double CL = 0. ;
    Eval_Tau_WMat (Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
    A += AL ;
    B += BL ;
    C += CL ;
  }
  A += 12. ;
  B += 12. ;
  
}

void SC_0D_VS_x_y_rc_dd_SET_rc_dd::Eval_CMat       (
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
){
  double AL = 0. ;
  double BL = 0. ;
  double CL = 0. ;
  Eval_WMat (V_Direction, vX, vY,Var_Min_X,Var_Max_X,Eps_X,Var_Min_Y,Var_Max_Y,Eps_Y,AL,BL,CL);
  
  double Det = AL*BL-CL*CL ;
  
  A = BL/Det ;
  B = AL/Det ;
  C =-CL/Det ;

}

