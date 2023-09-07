#include "SignalShape/SignalTime_Individual_Exact.h"

SignalTime_Individual_Exact::SignalTime_Individual_Exact(
                std::string SignalTime_Individual_Exact_File
){
  m_SignalTime_Individual_Exact_File = SignalTime_Individual_Exact_File ;

  m_s2bin = 1000./40. ;

  std::cout <<"  SignalTime_Individual_Exact::SignalTime_Individual_Exact : " << SignalTime_Individual_Exact_File << std::endl ;

  std::ifstream InputFile_SignalTime_Individual_Exact_File ; 
  InputFile_SignalTime_Individual_Exact_File.open(SignalTime_Individual_Exact_File.c_str()); 
  if(!InputFile_SignalTime_Individual_Exact_File) { // file couldn't be opened
    std::cout << "Error: file " << SignalTime_Individual_Exact_File << "could not be opened" << std::endl;
    abort() ;
  }
  
  std::string line;
  int linenum_Sample_File = 0. ;
  while (getline (InputFile_SignalTime_Individual_Exact_File, line)){
  
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

SignalTime_Individual_Exact::~SignalTime_Individual_Exact()
{
  V_X.clear() ;
  
  V_SignalPRF.clear()   ;
  V_SignalnPRF.clear()  ;
  V_SignalTime.clear()  ;
  V_SignalTime0.clear() ;
  V_SignalAmpli.clear() ;
}

double SignalTime_Individual_Exact::Eval_RhoPRF(const double& vX )
{
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

double SignalTime_Individual_Exact::Eval_AlphaPRF(const double& vX )
{
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

double SignalTime_Individual_Exact::Eval_TauPRF(const double& vX )
{
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

double SignalTime_Individual_Exact::Eval_TauBinPRF(const double& vX )
{
 return ( Eval_TauPRF(vX) * m_s2bin ) ; 
}

double SignalTime_Individual_Exact::Eval_TimeAbsPRF(const double& vX )
{
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

double SignalTime_Individual_Exact::Eval_Amplitude(const double& vX )
{
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

