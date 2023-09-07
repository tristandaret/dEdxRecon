#include "SignalShape/SC_VS_x_rc_dd.h"
#include "Misc/Util.h"

SC_VS_x_rc_dd::SC_VS_x_rc_dd(const std::string& LoadFile)
:SC_1D_VS_x_rc_dd_I(LoadFile)
{
  m_s2bin = 1000./40. ;
  Load(LoadFile) ;
}
SC_VS_x_rc_dd::SC_VS_x_rc_dd(const std::string& LoadFile,const double& s2bin)
:SC_1D_VS_x_rc_dd_I(LoadFile,s2bin)
{
  m_s2bin = s2bin ;
  Load(LoadFile) ;
}
void SC_VS_x_rc_dd::Load(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

  std::cout <<" =>SC_VS_x_rc_dd::SC_VS_x_rc_dd : " << m_LoadFile << std::endl ;

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

      if ( itemnum == 1 )  V_DD  .push_back( atof(item.c_str()) ) ;
      if ( itemnum == 2 )  {
        Free_trim(item) ;
        V_File.push_back( item ) ;
      }
    }
    
  }
  
  m_NberOfFiles = V_DD.size() ;
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    SC_VS_x_rc_FIX_dd* pSC_VS_x_rc_FIX_dd = new SC_VS_x_rc_FIX_dd(V_File[iTem]) ;
    V_SC_VS_x_rc_FIX_dd.push_back(pSC_VS_x_rc_FIX_dd);
  }
  
  m_DD_Lowest  = V_DD[0] ;
  m_DD_Highest = V_DD[m_NberOfFiles-1] ;
  
}

SC_VS_x_rc_dd::~SC_VS_x_rc_dd()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SC_VS_x_rc_FIX_dd[iTem] ;
    V_SC_VS_x_rc_FIX_dd[iTem] = 0 ;
  }
  V_SC_VS_x_rc_FIX_dd.clear() ;
  V_DD.clear()  ;
  V_File.clear()  ; 
}

double SC_VS_x_rc_dd::Eval_RhoPRF(const double& vX, const double& vRC , const double& vDDin  )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_VS_x_rc_FIX_dd[0]              ->Eval_RhoPRF(vX,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_VS_x_rc_FIX_dd[m_NberOfFiles-1]->Eval_RhoPRF(vX,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_VS_x_rc_FIX_dd[Index]  ->Eval_RhoPRF(vX,vRC) ;
  double YH = V_SC_VS_x_rc_FIX_dd[Index+1]->Eval_RhoPRF(vX,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_VS_x_rc_dd::Eval_AlphaPRF(const double& vX, const double& vRC , const double& vDDin  )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_VS_x_rc_FIX_dd[0]              ->Eval_AlphaPRF(vX,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_VS_x_rc_FIX_dd[m_NberOfFiles-1]->Eval_AlphaPRF(vX,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_VS_x_rc_FIX_dd[Index]  ->Eval_AlphaPRF(vX,vRC) ;
  double YH = V_SC_VS_x_rc_FIX_dd[Index+1]->Eval_AlphaPRF(vX,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_VS_x_rc_dd::Eval_TauPRF(const double& vX, const double& vRC , const double& vDDin  )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_VS_x_rc_FIX_dd[0]              ->Eval_TauPRF(vX,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_VS_x_rc_FIX_dd[m_NberOfFiles-1]->Eval_TauPRF(vX,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_VS_x_rc_FIX_dd[Index]  ->Eval_TauPRF(vX,vRC) ;
  double YH = V_SC_VS_x_rc_FIX_dd[Index+1]->Eval_TauPRF(vX,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_VS_x_rc_dd::Eval_TauBinPRF(const double& vX, const double& vRC , const double& vDDin  )
{
 return ( Eval_TauPRF(vX,vRC,vDDin) * m_s2bin ) ; 
}

double SC_VS_x_rc_dd::Eval_TimeAbsPRF(const double& vX, const double& vRC , const double& vDDin  )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_VS_x_rc_FIX_dd[0]              ->Eval_TimeAbsPRF(vX,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_VS_x_rc_FIX_dd[m_NberOfFiles-1]->Eval_TimeAbsPRF(vX,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_VS_x_rc_FIX_dd[Index]  ->Eval_TimeAbsPRF(vX,vRC) ;
  double YH = V_SC_VS_x_rc_FIX_dd[Index+1]->Eval_TimeAbsPRF(vX,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_VS_x_rc_dd::Eval_Amplitude(const double& vX, const double& vRC , const double& vDDin  )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_VS_x_rc_FIX_dd[0]              ->Eval_Amplitude(vX,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_VS_x_rc_FIX_dd[m_NberOfFiles-1]->Eval_Amplitude(vX,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_VS_x_rc_FIX_dd[Index]  ->Eval_Amplitude(vX,vRC) ;
  double YH = V_SC_VS_x_rc_FIX_dd[Index+1]->Eval_Amplitude(vX,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
