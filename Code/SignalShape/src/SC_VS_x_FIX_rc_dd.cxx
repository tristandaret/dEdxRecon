#include "SignalShape/SC_VS_x_FIX_rc_dd.h"

SC_VS_x_FIX_rc_dd::SC_VS_x_FIX_rc_dd(const std::string& LoadFile)
{
  m_s2bin = 1000./40. ;
  m_FirstCall = 1 ;
  m_LoadFile = LoadFile ;
//Load(LoadFile) ;
}
SC_VS_x_FIX_rc_dd::SC_VS_x_FIX_rc_dd(const std::string& LoadFile,const double& s2bin)
{
  m_s2bin = s2bin ;
  m_FirstCall = 1 ;
  m_LoadFile = LoadFile ;
//Load(LoadFile) ;
}
void SC_VS_x_FIX_rc_dd::Load(const std::string& LoadFile)
{
  if (m_FirstCall==0) return ;
  m_FirstCall = 0 ;
//m_LoadFile = LoadFile ;

//std::cout <<"  SC_VS_x_FIX_rc_dd::SC_VS_x_FIX_rc_dd : " << LoadFile << std::endl ;

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

        if ( itemnum == 1 ) V_X.push_back( atof(item.c_str())*1E6 ) ; 
        if ( itemnum == 2 ) V_SignalPRF  .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 3 ) V_SignalnPRF .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 4 ) V_SignalTime .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 5 ) V_SignalTime0.push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 6 ) V_SignalAmpli.push_back( atof(item.c_str())     ) ; 
      }
  }

  int MaxIndex = V_X.size() - 1 ;
  m_X_Lowest             =  0.                      ;
  m_X_Highest            =  V_X          [MaxIndex] ;
  m_SignalPRF_Default    =  V_SignalPRF  [MaxIndex] ;
  m_SignalnPRF_Default   =  V_SignalnPRF [MaxIndex] ;
  m_SignalTime_Default   =  V_SignalTime [MaxIndex] ;
  m_SignalTime0_Default  =  V_SignalTime0[MaxIndex] ;  
  m_SignalAmpli_Default  =  V_SignalAmpli[MaxIndex] ;  
  
}

SC_VS_x_FIX_rc_dd::~SC_VS_x_FIX_rc_dd()
{
  V_X.clear() ;
  
  V_SignalPRF.clear()   ;
  V_SignalnPRF.clear()  ;
  V_SignalTime.clear()  ;
  V_SignalTime0.clear() ;
  V_SignalAmpli.clear() ;
}

double SC_VS_x_FIX_rc_dd::Eval_RhoPRF(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_SignalPRF_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_X_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_X_Lowest)  ;
  double XL = V_X[Index] ;
  double YL = V_SignalPRF[Index] ;
  double YH = V_SignalPRF[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_VS_x_FIX_rc_dd::Eval_AlphaPRF(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_SignalnPRF_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_X_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_X_Lowest)  ;
  double XL = V_X[Index] ;
  double YL = V_SignalnPRF[Index] ;
  double YH = V_SignalnPRF[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_VS_x_FIX_rc_dd::Eval_TauPRF(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_SignalTime_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_X_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_X_Lowest)  ;
  double XL = V_X[Index] ;
  double YL = V_SignalTime[Index] ;
  double YH = V_SignalTime[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_VS_x_FIX_rc_dd::Eval_TauBinPRF(const double& vX )
{
 return ( Eval_TauPRF(vX) * m_s2bin ) ; 
}

double SC_VS_x_FIX_rc_dd::Eval_TimeAbsPRF(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_SignalTime0_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_X_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_X_Lowest)  ;
  double XL = V_X[Index] ;
  double YL = V_SignalTime0[Index] ;
  double YH = V_SignalTime0[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_VS_x_FIX_rc_dd::Eval_Amplitude(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_SignalAmpli_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_X_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_X_Lowest)  ;
  double XL = V_X[Index] ;
  double YL = V_SignalAmpli[Index] ;
  double YH = V_SignalAmpli[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

