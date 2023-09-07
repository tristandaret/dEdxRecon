#include "SignalShape/SC_0D_VS_x_FIX_y_rc_dd.h"

SC_0D_VS_x_FIX_y_rc_dd::SC_0D_VS_x_FIX_y_rc_dd(const std::string& LoadFile)
{
  m_FirstCall = 1 ;
  m_LoadFile = LoadFile ;
//Load(LoadFile) ;
}
void SC_0D_VS_x_FIX_y_rc_dd::Load(const std::string& LoadFile)
{
  if (m_FirstCall==0) return ;
  m_FirstCall = 0 ;
//m_LoadFile = LoadFile ;

//std::cout <<"  SC_0D_VS_x_FIX_y_rc_dd::SC_0D_VS_x_FIX_y_rc_dd : " << LoadFile << std::endl ;

  std::ifstream InputFile_LoadFile ; 
  InputFile_LoadFile.open(LoadFile.c_str()); 
  if(!InputFile_LoadFile) { // file couldn't be opened
    std::cout << "Error: file " << LoadFile << "could not be opened" << std::endl;
    abort() ;
  }
  
  std::string line;
  int linenum_Sample_File = 0. ;
  while (getline (InputFile_LoadFile, line)){
  
    linenum_Sample_File++;

      std::istringstream linestream(line);
      std::string item;
      int itemnum = 0 ;
      while (getline (linestream, item, ';'))
      {
        itemnum++;

        if ( itemnum ==  1 ) V_X.push_back( atof(item.c_str()) ) ; 

        if ( itemnum ==  2 ) V_A_SW  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  3 ) V_A_SS  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  4 ) V_A_SE  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  5 ) V_A_WW  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  6 ) V_A_LE  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  7 ) V_A_EE  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  8 ) V_A_NW  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum ==  9 ) V_A_NN  .push_back( 2.*atof(item.c_str())     ) ; 
        if ( itemnum == 10 ) V_A_NE  .push_back( 2.*atof(item.c_str())     ) ; 

        if ( itemnum == 11 ) V_T_SW  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 12 ) V_T_SS  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 13 ) V_T_SE  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 14 ) V_T_WW  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 15 ) V_T_LE  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 16 ) V_T_EE  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 17 ) V_T_NW  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 18 ) V_T_NN  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 19 ) V_T_NE  .push_back( atof(item.c_str())     ) ; 
      }
  }

  int MaxIndex = V_X.size() - 1 ;
  m_X_Lowest             =  0.                      ;
  m_X_Highest            =  V_X          [MaxIndex] ;
 
  m_V_A_SW_Default    =  V_A_SW  [MaxIndex] ;
  m_V_A_SS_Default    =  V_A_SS  [MaxIndex] ;
  m_V_A_SE_Default    =  V_A_SE  [MaxIndex] ;
  m_V_A_WW_Default    =  V_A_WW  [MaxIndex] ;
  m_V_A_LE_Default    =  V_A_LE  [MaxIndex] ;
  m_V_A_EE_Default    =  V_A_EE  [MaxIndex] ;
  m_V_A_NW_Default    =  V_A_NW  [MaxIndex] ;
  m_V_A_NN_Default    =  V_A_NN  [MaxIndex] ;
  m_V_A_NE_Default    =  V_A_NE  [MaxIndex] ;
  
  m_V_T_SW_Default    =  V_T_SW  [MaxIndex] ;
  m_V_T_SS_Default    =  V_T_SS  [MaxIndex] ;
  m_V_T_SE_Default    =  V_T_SE  [MaxIndex] ;
  m_V_T_WW_Default    =  V_T_WW  [MaxIndex] ;
  m_V_T_LE_Default    =  V_T_LE  [MaxIndex] ;
  m_V_T_EE_Default    =  V_T_EE  [MaxIndex] ;
  m_V_T_NW_Default    =  V_T_NW  [MaxIndex] ;
  m_V_T_NN_Default    =  V_T_NN  [MaxIndex] ;
  m_V_T_NE_Default    =  V_T_NE  [MaxIndex] ;

}

SC_0D_VS_x_FIX_y_rc_dd::~SC_0D_VS_x_FIX_y_rc_dd()
{
  V_X.clear() ;
  
  V_A_SW.clear() ;
  V_A_SS.clear() ;
  V_A_SE.clear() ;
  V_A_WW.clear() ;
  V_A_LE.clear() ;
  V_A_EE.clear() ;
  V_A_NW.clear() ;
  V_A_NN.clear() ;
  V_A_NE.clear() ;

  V_T_SW.clear() ;
  V_T_SS.clear() ;
  V_T_SE.clear() ;
  V_T_WW.clear() ;
  V_T_LE.clear() ;
  V_T_EE.clear() ;
  V_T_NW.clear() ;
  V_T_NN.clear() ;
  V_T_NE.clear() ;
}

double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_NN(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_NN_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_NN[Index] ;
  double YH = V_A_NN[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_NW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_NW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_NW[Index] ;
  double YH = V_A_NW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_NE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_NE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_NE[Index] ;
  double YH = V_A_NE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_LE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_LE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_LE[Index] ;
  double YH = V_A_LE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_WW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_WW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_WW[Index] ;
  double YH = V_A_WW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_EE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_EE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_EE[Index] ;
  double YH = V_A_EE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_SS(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_SS_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_SS[Index] ;
  double YH = V_A_SS[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_SW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_SW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_SW[Index] ;
  double YH = V_A_SW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}

double SC_0D_VS_x_FIX_y_rc_dd::Eval_A_SE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_A_SE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_A_SE[Index] ;
  double YH = V_A_SE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_NN(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_NN_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_NN[Index] ;
  double YH = V_T_NN[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_NW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_NW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_NW[Index] ;
  double YH = V_T_NW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_NE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_NE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_NE[Index] ;
  double YH = V_T_NE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_LE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_LE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_LE[Index] ;
  double YH = V_T_LE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_WW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_WW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_WW[Index] ;
  double YH = V_T_WW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_EE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_EE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_EE[Index] ;
  double YH = V_T_EE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_SS(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_SS_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_SS[Index] ;
  double YH = V_T_SS[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_SW(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_SW_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_SW[Index] ;
  double YH = V_T_SW[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}

double SC_0D_VS_x_FIX_y_rc_dd::Eval_T_SE(const double& vX )
{
  Load(m_LoadFile) ;
  double vXL = std::fabs(vX)*1E-2 ;
  if ( vXL >= m_X_Highest ) return m_V_T_SE_Default ;
  
  int Index = int((vXL - m_X_Lowest)/10.E-6)  ;
  double XL = V_X[Index] ;
  double XH = V_X[Index+1] ;
  double YL = V_T_SE[Index] ;
  double YH = V_T_SE[Index+1] ;
  
  return ( YL + (vXL -XL)*(YH-YL)/(XH-XL) )  ;
    
}
