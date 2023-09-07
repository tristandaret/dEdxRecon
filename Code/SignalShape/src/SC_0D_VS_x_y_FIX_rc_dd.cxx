#include "SignalShape/SC_0D_VS_x_y_FIX_rc_dd.h"
#include "Misc/Util.h"

SC_0D_VS_x_y_FIX_rc_dd::SC_0D_VS_x_y_FIX_rc_dd(const std::string& LoadFile)
{
  Load(LoadFile) ;
}
void SC_0D_VS_x_y_FIX_rc_dd::Load(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

  std::cout <<" =>SC_0D_VS_x_y_FIX_rc_dd::SC_0D_VS_x_y_FIX_rc_dd : " << m_LoadFile << std::endl ;

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

      if ( itemnum == 1 )  V_Y  .push_back( atof(item.c_str()) ) ;
      if ( itemnum == 2 )  {
        Free_trim(item) ;
        V_File.push_back( item ) ;
      }
    }
    
  }
  
  m_NberOfFiles = V_Y.size() ;
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    SC_0D_VS_x_FIX_y_rc_dd* pSC_0D_VS_x_FIX_y_rc_dd = new SC_0D_VS_x_FIX_y_rc_dd(V_File[iTem]) ;
    V_SC_0D_VS_x_FIX_y_rc_dd.push_back(pSC_0D_VS_x_FIX_y_rc_dd);
  }
  
  m_Y_Lowest  = V_Y[0] ;
  m_Y_Highest = V_Y[m_NberOfFiles-1] ;
  
}

SC_0D_VS_x_y_FIX_rc_dd::~SC_0D_VS_x_y_FIX_rc_dd()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SC_0D_VS_x_FIX_y_rc_dd[iTem] ;
    V_SC_0D_VS_x_FIX_y_rc_dd[iTem] = 0 ;
  }
  V_SC_0D_VS_x_FIX_y_rc_dd.clear() ;
  V_Y.clear()  ;
  V_File.clear()  ; 
}

double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_NN(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_NN(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_NN(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_NN(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_NN(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_NW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_NW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_NW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_NW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_NW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_NE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_NE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_NE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_NE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_NE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_LE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_LE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_LE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_LE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_LE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_WW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_WW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_WW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_WW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_WW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_EE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_EE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_EE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_EE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_EE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_SS(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_SS(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_SS(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_SS(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_SS(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_SW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_SW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_SW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_SW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_SW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_A_SE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_A_SE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_A_SE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_A_SE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_A_SE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_NN(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_NN(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_NN(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_NN(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_NN(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_NW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_NW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_NW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_NW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_NW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_NE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_NE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_NE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_NE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_NE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_LE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_LE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_LE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_LE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_LE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_WW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_WW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_WW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_WW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_WW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_EE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_EE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_EE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_EE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_EE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_SS(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_SS(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_SS(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_SS(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_SS(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_SW(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_SW(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_SW(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_SW(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_SW(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_FIX_rc_dd::Eval_T_SE(const double& vX, const double& vYin)
{
  double vY = std::fabs(vYin)*1.E-2; ;
  if ( vY <= m_Y_Lowest ) return V_SC_0D_VS_x_FIX_y_rc_dd[0]              ->Eval_T_SE(vX) ;
  if ( vY >= m_Y_Highest) return V_SC_0D_VS_x_FIX_y_rc_dd[m_NberOfFiles-1]->Eval_T_SE(vX) ;
  
  int Index = int( (vY - m_Y_Lowest)/10.E-6) ;
  double XL = V_Y[Index] ;
  double XH = V_Y[Index+1] ;
  double YL = V_SC_0D_VS_x_FIX_y_rc_dd[Index]  ->Eval_T_SE(vX) ;
  double YH = V_SC_0D_VS_x_FIX_y_rc_dd[Index+1]->Eval_T_SE(vX) ;

  return ( YL + (vY -XL)*(YH-YL)/(XH-XL) ) ;
    
}
