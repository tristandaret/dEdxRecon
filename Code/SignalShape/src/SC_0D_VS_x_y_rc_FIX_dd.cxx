#include "SignalShape/SC_0D_VS_x_y_rc_FIX_dd.h"
#include "Misc/Util.h"

SC_0D_VS_x_y_rc_FIX_dd::SC_0D_VS_x_y_rc_FIX_dd(const std::string& LoadFile)
{
  Load(LoadFile) ;
}
void SC_0D_VS_x_y_rc_FIX_dd::Load(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

  std::cout <<" =>SC_0D_VS_x_y_rc_FIX_dd::SC_0D_VS_x_y_rc_FIX_dd : " << m_LoadFile << std::endl ;

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
    SC_0D_VS_x_y_FIX_rc_dd* pSC_0D_VS_x_y_FIX_rc_dd = new SC_0D_VS_x_y_FIX_rc_dd(V_File[iTem]) ;
    V_SC_0D_VS_x_y_FIX_rc_dd.push_back(pSC_0D_VS_x_y_FIX_rc_dd);
  }
  
  m_RC_Lowest  = V_RC[0] ;
  m_RC_Highest = V_RC[m_NberOfFiles-1] ;
  
}

SC_0D_VS_x_y_rc_FIX_dd::~SC_0D_VS_x_y_rc_FIX_dd()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SC_0D_VS_x_y_FIX_rc_dd[iTem] ;
    V_SC_0D_VS_x_y_FIX_rc_dd[iTem] = 0 ;
  }
  V_SC_0D_VS_x_y_FIX_rc_dd.clear() ;
  V_RC.clear()  ;
  V_File.clear()  ; 
}

double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_NN(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_NN(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_NN(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_NN(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_NN(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_NW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_NW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_NW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_NW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_NW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_NE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_NE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_NE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_NE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_NE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_LE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_LE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_LE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_LE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_LE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_WW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_WW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_WW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_WW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_WW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_EE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_EE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_EE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_EE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_EE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_SS(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_SS(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_SS(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_SS(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_SS(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_SW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_SW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_SW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_SW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_SW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_A_SE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_A_SE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_A_SE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_A_SE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_A_SE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_NN(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_NN(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_NN(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_NN(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_NN(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_NW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_NW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_NW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_NW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_NW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_NE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_NE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_NE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_NE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_NE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_LE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_LE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_LE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_LE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_LE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_WW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_WW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_WW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_WW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_WW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_EE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_EE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_EE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_EE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_EE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_SS(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_SS(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_SS(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_SS(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_SS(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_SW(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_SW(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_SW(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_SW(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_SW(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_FIX_dd::Eval_T_SE(const double& vX, const double& vY, const double& vRCin)
{
  double vRC = std::fabs(vRCin) ;
  if ( vRC <= m_RC_Lowest ) return V_SC_0D_VS_x_y_FIX_rc_dd[0]              ->Eval_T_SE(vX,vY) ;
  if ( vRC >= m_RC_Highest) return V_SC_0D_VS_x_y_FIX_rc_dd[m_NberOfFiles-1]->Eval_T_SE(vX,vY) ;
  
  int Index = int( (vRC - m_RC_Lowest)/50. ) ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SC_0D_VS_x_y_FIX_rc_dd[Index]  ->Eval_T_SE(vX,vY) ;
  double YH = V_SC_0D_VS_x_y_FIX_rc_dd[Index+1]->Eval_T_SE(vX,vY) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
