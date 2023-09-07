#include "SignalShape/SC_1D_VS_x_rc_FIX_dd.h"
#include "Misc/Util.h"

SC_1D_VS_x_rc_FIX_dd::SC_1D_VS_x_rc_FIX_dd(const std::string& LoadFile)
{
  m_s2bin = 1000./40. ;
  Load(LoadFile) ;
}
SC_1D_VS_x_rc_FIX_dd::SC_1D_VS_x_rc_FIX_dd(const std::string& LoadFile,const double& s2bin)
{
  m_s2bin = s2bin ;
  Load(LoadFile) ;
}

void SC_1D_VS_x_rc_FIX_dd::Load(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

//std::cout <<" =>SC_1D_VS_x_rc_FIX_dd::SC_1D_VS_x_rc_FIX_dd : " << m_LoadFile << std::endl ;

  std::ifstream InputFile_LoadFile ; 
  InputFile_LoadFile.open(m_LoadFile.c_str()); 
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

      if ( itemnum == 1 )  V_RC  .push_back( atof(item.c_str()) ) ;
      if ( itemnum == 2 )  {
        Free_trim(item) ;
        V_File.push_back( item ) ;
      }
    }
    
  }
  
  m_NberOfFiles = V_RC.size() ;
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    SC_1D_VS_x_FIX_rc_dd* pSC_1D_VS_x_FIX_rc_dd = new SC_1D_VS_x_FIX_rc_dd(V_File[iTem]) ;
    V_SC_1D_VS_x_FIX_rc_dd.push_back(pSC_1D_VS_x_FIX_rc_dd);
  }
  
  m_RC_Lowest  = V_RC[0] ;
  m_RC_Highest = V_RC[m_NberOfFiles-1] ;
  
}

SC_1D_VS_x_rc_FIX_dd::~SC_1D_VS_x_rc_FIX_dd()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SC_1D_VS_x_FIX_rc_dd[iTem] ;
    V_SC_1D_VS_x_FIX_rc_dd[iTem] = 0 ;
  }
  V_SC_1D_VS_x_FIX_rc_dd.clear() ;
  V_RC.clear()  ;
  V_File.clear()  ;
  
}

double SC_1D_VS_x_rc_FIX_dd::Eval_RhoPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Eval_RhoPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Eval_RhoPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Eval_RhoPRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Eval_RhoPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_1D_VS_x_rc_FIX_dd::Eval_AlphaPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Eval_AlphaPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Eval_AlphaPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Eval_AlphaPRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Eval_AlphaPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_1D_VS_x_rc_FIX_dd::Eval_TauPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Eval_TauPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Eval_TauPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Eval_TauPRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Eval_TauPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_1D_VS_x_rc_FIX_dd::Eval_TauBinPRF(const double& vX, const double& vRC  )
{
 return ( Eval_TauPRF(vX,vRC) * m_s2bin ) ; 
}

double SC_1D_VS_x_rc_FIX_dd::Eval_TimeAbsPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Eval_TimeAbsPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Eval_TimeAbsPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Eval_TimeAbsPRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Eval_TimeAbsPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_1D_VS_x_rc_FIX_dd::Eval_Amplitude(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Eval_Amplitude(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Eval_Amplitude(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Eval_Amplitude(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Eval_Amplitude(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_1D_VS_x_rc_FIX_dd::Am_DW_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_DW_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_DW_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_DW_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_DW_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Am_LE_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_LE_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_LE_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_LE_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_LE_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Am_UP_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_UP_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_UP_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_UP_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_UP_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_DW_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_DW_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_DW_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_DW_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_DW_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_LE_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_LE_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_LE_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_LE_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_LE_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_UP_Base(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_UP_Base(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_UP_Base(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_UP_Base(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_UP_Base(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Am_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Am_LE(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_LE(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_LE(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_LE(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_LE(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Am_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Am_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Am_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Am_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Am_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_LE(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_LE(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_LE(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_LE(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_LE(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Ti_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Ti_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Ti_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Ti_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Ti_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Alpha_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Alpha_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Alpha_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Alpha_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Alpha_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Alpha_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Alpha_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Alpha_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Alpha_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Alpha_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Tau_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Tau_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Tau_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Tau_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Tau_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Tau_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Tau_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Tau_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Tau_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Tau_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::TauBin_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->TauBin_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->TauBin_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->TauBin_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->TauBin_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::TauBin_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->TauBin_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->TauBin_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->TauBin_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->TauBin_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Rho_DW(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Rho_DW(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Rho_DW(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Rho_DW(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Rho_DW(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Rho_LE(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Rho_LE(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Rho_LE(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Rho_LE(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Rho_LE(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Rho_UP(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Rho_UP(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Rho_UP(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Rho_UP(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Rho_UP(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::TauBin_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->TauBin_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->TauBin_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->TauBin_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->TauBin_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Tau_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Tau_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Tau_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Tau_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Tau_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Alpha_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Alpha_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Alpha_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Alpha_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Alpha_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::Rho_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->Rho_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->Rho_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->Rho_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->Rho_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::T_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->T_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->T_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->T_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->T_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_1D_VS_x_rc_FIX_dd::A_PRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SC_1D_VS_x_FIX_rc_dd[0]              ->A_PRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SC_1D_VS_x_FIX_rc_dd[m_NberOfFiles-1]->A_PRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_1D_VS_x_FIX_rc_dd[Index]  ->A_PRF(vX) ;
  double YH = V_SC_1D_VS_x_FIX_rc_dd[Index+1]->A_PRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
