#include "SignalShape/SC_1D_VS_x_FIX_rc_dd.h"

SC_1D_VS_x_FIX_rc_dd::SC_1D_VS_x_FIX_rc_dd(const std::string& LoadFile)
{
  m_s2bin = 1000./40. ;
  m_FirstCall = 1 ;
  m_LoadFile = LoadFile ;
//Load(LoadFile) ;
}
SC_1D_VS_x_FIX_rc_dd::SC_1D_VS_x_FIX_rc_dd(const std::string& LoadFile,const double& s2bin)
{
  m_s2bin = s2bin ;
  m_FirstCall = 1 ;
  m_LoadFile = LoadFile ;
//Load(LoadFile) ;
}
void SC_1D_VS_x_FIX_rc_dd::Load(const std::string& LoadFile)
{
  if (m_FirstCall==0) return ;
  m_FirstCall = 0 ;
//m_LoadFile = LoadFile ;

//std::cout <<"  SC_1D_VS_x_FIX_rc_dd::SC_1D_VS_x_FIX_rc_dd : " << LoadFile << std::endl ;

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

    if (linenum_Sample_File==1){
      m_LY      = atof(line.c_str())*1E6 ;
      m_LY_incm = atof(line.c_str())*1E2 ;
    }else{
      std::istringstream linestream(line);
      std::string item;
      int itemnum = 0 ;
      while (getline (linestream, item, ';'))
      {
        itemnum++;

        if ( itemnum == 1 ) V_Y    .push_back( atof(item.c_str())*1E6 ) ; 
        if ( itemnum == 2 ) V_A_DW .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 3 ) V_A_LE .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 4 ) V_A_UP .push_back( atof(item.c_str())     ) ; 
        if ( itemnum == 5 ) V_T_DW .push_back( atof(item.c_str())*1E6 ) ; 
        if ( itemnum == 6 ) V_T_LE .push_back( atof(item.c_str())*1E6 ) ; 
        if ( itemnum == 7 ) V_T_UP .push_back( atof(item.c_str())*1E6 ) ; 
      }
    }
  }

  int MaxIndex = V_Y.size() - 1 ;
  m_Y_Lowest        =  0.                ;
  m_Y_Highest       =  V_Y    [MaxIndex] ;
  m_A_DW_Default    =  V_A_DW [MaxIndex] ;
  m_A_LE_Default    =  V_A_LE [MaxIndex] ;
  m_A_UP_Default    =  V_A_UP [MaxIndex] ;
  m_T_DW_Default    =  V_T_DW [MaxIndex] ;
  m_T_LE_Default    =  V_T_LE [MaxIndex] ;
  m_T_UP_Default    =  V_T_UP [MaxIndex] ;
  
}

SC_1D_VS_x_FIX_rc_dd::~SC_1D_VS_x_FIX_rc_dd()
{
  V_Y.clear() ;
  
  V_A_DW.clear()   ;
  V_A_LE.clear()   ;
  V_A_UP.clear()   ;
  V_T_DW.clear()   ;
  V_T_LE.clear()   ;
  V_T_UP.clear()   ;
}

//---------------------------------------------------------------------//

double SC_1D_VS_x_FIX_rc_dd::Eval_RhoPRF    (const double& vX ){ return Rho_PRF(vX)    ; }

double SC_1D_VS_x_FIX_rc_dd::Eval_AlphaPRF  (const double& vX ){ return Alpha_PRF(vX)  ; }

double SC_1D_VS_x_FIX_rc_dd::Eval_TauPRF    (const double& vX ){ return Tau_PRF(vX)    ; }

double SC_1D_VS_x_FIX_rc_dd::Eval_TauBinPRF (const double& vX ){ return TauBin_PRF(vX) ; }

double SC_1D_VS_x_FIX_rc_dd::Eval_TimeAbsPRF(const double& vX ){ return T_PRF(vX)      ; }

double SC_1D_VS_x_FIX_rc_dd::Eval_Amplitude (const double& vX ){ return A_PRF(vX)      ; }

//---------------------------------------------------------------------//

double  SC_1D_VS_x_FIX_rc_dd::Rho_PRF(const double& vX )
{
  if(vX < -m_LY_incm/2. ) return Rho_UP(vX+m_LY_incm) ;
  if(vX >  m_LY_incm/2. ) return Rho_DW(vX-m_LY_incm) ;
  return Rho_LE(vX) ;
}
double  SC_1D_VS_x_FIX_rc_dd::A_PRF(const double& vX )
{
  if(vX < -m_LY_incm/2. ) return Am_UP(vX+m_LY_incm) ;
  if(vX >  m_LY_incm/2. ) return Am_DW(vX-m_LY_incm) ;
  return Am_LE(vX) ;
}

double  SC_1D_VS_x_FIX_rc_dd::T_PRF(const double& vX )
{
  if(vX < -m_LY_incm/2. ) return Ti_UP(vX+m_LY_incm) ;
  if(vX >  m_LY_incm/2. ) return Ti_DW(vX-m_LY_incm) ;
  return Ti_LE(vX) ;
}

double  SC_1D_VS_x_FIX_rc_dd::Alpha_PRF(const double& vX )
{
  if(vX < -m_LY_incm/2. ) return Alpha_UP(vX+m_LY_incm) ;
  if(vX >  m_LY_incm/2. ) return Alpha_DW(vX-m_LY_incm) ;
  return 1. ;
}

double  SC_1D_VS_x_FIX_rc_dd::Tau_PRF(const double& vX )
{
  if(vX < -m_LY_incm/2. ) return Tau_UP(vX+m_LY_incm) ;
  if(vX >  m_LY_incm/2. ) return Tau_DW(vX-m_LY_incm) ;
  return 0. ;
}

double  SC_1D_VS_x_FIX_rc_dd::TauBin_PRF(const double& vX ){ return ( Tau_PRF(vX) * m_s2bin ) ; } 

//---------------------------------------------------------------------//

double SC_1D_VS_x_FIX_rc_dd::Rho_DW    (const double& vX ){ return ( Am_DW(vX)/(Am_LE(vX)+Am_DW(vX)+Am_UP(vX))  ) ; }
double SC_1D_VS_x_FIX_rc_dd::Rho_LE    (const double& vX ){ return ( Am_LE(vX)/(Am_LE(vX)+Am_DW(vX)+Am_UP(vX))  ) ; }
double SC_1D_VS_x_FIX_rc_dd::Rho_UP    (const double& vX ){ return ( Am_UP(vX)/(Am_LE(vX)+Am_DW(vX)+Am_UP(vX))  ) ; }

double SC_1D_VS_x_FIX_rc_dd::Alpha_DW  (const double& vX ){ return ( Am_DW(vX)/Am_LE(vX)  ) ; }
double SC_1D_VS_x_FIX_rc_dd::Alpha_UP  (const double& vX ){ return ( Am_UP(vX)/Am_LE(vX)  ) ; }

double SC_1D_VS_x_FIX_rc_dd::Tau_DW    (const double& vX ){ return ( Ti_DW(vX)-Ti_LE(vX)  ) ; }
double SC_1D_VS_x_FIX_rc_dd::Tau_UP    (const double& vX ){ return ( Ti_UP(vX)-Ti_LE(vX)  ) ; }
double SC_1D_VS_x_FIX_rc_dd::TauBin_DW (const double& vX ){ return ( Tau_DW(vX) * m_s2bin ) ; } 
double SC_1D_VS_x_FIX_rc_dd::TauBin_UP (const double& vX ){ return ( Tau_UP(vX) * m_s2bin ) ; }

double SC_1D_VS_x_FIX_rc_dd::Am_LE(const double& vX ){ return Am_LE_Base(vX) ; }
double SC_1D_VS_x_FIX_rc_dd::Ti_LE(const double& vX ){ return Ti_LE_Base(vX) ; }

double SC_1D_VS_x_FIX_rc_dd::Am_DW(const double& vX )
{ 
 if (vX>0.){  return Am_DW_Base( vX) ; }
              return Am_UP_Base(-vX) ; 
}

double SC_1D_VS_x_FIX_rc_dd::Am_UP(const double& vX )
{ 
 if (vX>0.){ return Am_UP_Base( vX) ; }
             return Am_DW_Base(-vX) ; 
}

double SC_1D_VS_x_FIX_rc_dd::Ti_DW(const double& vX )
{ 
 if (vX>0.){ return Ti_DW_Base( vX) ; } 
             return Ti_UP_Base(-vX) ; 
}

double SC_1D_VS_x_FIX_rc_dd::Ti_UP(const double& vX )
{ 
 if (vX>0.){ return Ti_UP_Base( vX) ; } 
             return Ti_DW_Base(-vX) ; 
}

double SC_1D_VS_x_FIX_rc_dd::Am_DW_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_A_DW_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_A_DW[Index] ;
  double YH = V_A_DW[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_1D_VS_x_FIX_rc_dd::Am_LE_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_A_LE_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_A_LE[Index] ;
  double YH = V_A_LE[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_1D_VS_x_FIX_rc_dd::Am_UP_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_A_UP_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_A_UP[Index] ;
  double YH = V_A_UP[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_1D_VS_x_FIX_rc_dd::Ti_DW_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_T_DW_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_T_DW[Index] ;
  double YH = V_T_DW[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_1D_VS_x_FIX_rc_dd::Ti_LE_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_T_LE_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_T_LE[Index] ;
  double YH = V_T_LE[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}

double SC_1D_VS_x_FIX_rc_dd::Ti_UP_Base(const double& vX )
{
  Load(m_LoadFile) ;
  double ToBeReturned = m_T_UP_Default ;
  double vXL = std::fabs(vX)*1E4 ;

  if ( vXL >= m_Y_Highest ) return ToBeReturned ;
  
  int Index = int(vXL - m_Y_Lowest)  ;
  double XL = V_Y[Index] ;
  double YL = V_T_UP[Index] ;
  double YH = V_T_UP[Index+1] ;
  
  ToBeReturned = YL + (vXL -XL)*(YH-YL) ;
  
  return ToBeReturned ;
    
}
