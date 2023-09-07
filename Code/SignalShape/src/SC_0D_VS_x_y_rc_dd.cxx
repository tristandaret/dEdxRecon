#include "SignalShape/SC_0D_VS_x_y_rc_dd.h"
#include "Misc/Util.h"

SC_0D_VS_x_y_rc_dd::SC_0D_VS_x_y_rc_dd(const std::string& LoadFile)
{
  m_s2bin = 1000./40. ;
  Load(LoadFile) ;
}
SC_0D_VS_x_y_rc_dd::SC_0D_VS_x_y_rc_dd(const std::string& LoadFile,const double& s2bin)
{
  m_s2bin = s2bin ;
  Load(LoadFile) ;
}
void SC_0D_VS_x_y_rc_dd::Load(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

  std::cout <<" =>SC_0D_VS_x_y_rc_dd::SC_0D_VS_x_y_rc_dd : " << m_LoadFile << std::endl ;

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
    SC_0D_VS_x_y_rc_FIX_dd* pSC_0D_VS_x_y_rc_FIX_dd = new SC_0D_VS_x_y_rc_FIX_dd(V_File[iTem]) ;
    V_SC_0D_VS_x_y_rc_FIX_dd.push_back(pSC_0D_VS_x_y_rc_FIX_dd);
  }
  
  m_DD_Lowest  = V_DD[0] ;
  m_DD_Highest = V_DD[m_NberOfFiles-1] ;
  
}

SC_0D_VS_x_y_rc_dd::~SC_0D_VS_x_y_rc_dd()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SC_0D_VS_x_y_rc_FIX_dd[iTem] ;
    V_SC_0D_VS_x_y_rc_FIX_dd[iTem] = 0 ;
  }
  V_SC_0D_VS_x_y_rc_FIX_dd.clear() ;
  V_DD.clear()  ;
  V_File.clear()  ; 
}

double SC_0D_VS_x_y_rc_dd::Eval_A_NN(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_NN(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_NN(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_NN(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_NN(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_NW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_NW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_NW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_NW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_NW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_NE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_NE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_NE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_NE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_NE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_LE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_LE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_LE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_LE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_LE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_WW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_WW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_WW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_WW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_WW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_EE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_EE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_EE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_EE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_EE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_SS(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_SS(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_SS(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_SS(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_SS(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_SW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_SW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_SW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_SW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_SW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_A_SE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_A_SE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_A_SE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_A_SE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_A_SE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) ) ;
    
}


double SC_0D_VS_x_y_rc_dd::Eval_T_NN(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_NN(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_NN(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_NN(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_NN(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )   ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_NW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_NW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_NW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_NW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_NW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_NE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_NE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_NE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_NE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_NE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_LE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_LE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_LE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_LE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_LE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_WW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_WW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_WW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_WW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_WW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_EE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_EE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_EE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_EE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_EE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_SS(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_SS(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_SS(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_SS(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_SS(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_SW(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_SW(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_SW(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_SW(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_SW(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_T_SE(const double& vX, const double& vY, const double& vRC , const double& vDDin )
{
  double vDD = std::fabs(vDDin) ;
  if ( vDD <= m_DD_Lowest ) return V_SC_0D_VS_x_y_rc_FIX_dd[0]              ->Eval_T_SE(vX,vY,vRC) ;
  if ( vDD >= m_DD_Highest) return V_SC_0D_VS_x_y_rc_FIX_dd[m_NberOfFiles-1]->Eval_T_SE(vX,vY,vRC) ;
  
  int Index = int( (vDD - m_DD_Lowest)/50. ) ;
  double XL = V_DD[Index] ;
  double XH = V_DD[Index+1] ;
  double YL = V_SC_0D_VS_x_y_rc_FIX_dd[Index]  ->Eval_T_SE(vX,vY,vRC) ;
  double YH = V_SC_0D_VS_x_y_rc_FIX_dd[Index+1]->Eval_T_SE(vX,vY,vRC) ;

  return ( YL + (vDD -XL)*(YH-YL)/(XH-XL) )  ;
    
}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_NN (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_NN(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_NW (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_NW(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_NE (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_NE(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_LE (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_LE(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_WW (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_WW(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_EE (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_EE(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_SS (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_SS(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_SW (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_SW(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
double SC_0D_VS_x_y_rc_dd::Eval_Tbin_SE (const double& vX, const double& vY, const double& vRC , const double& vDDin ){ return ( Eval_T_SE(vX,vY,vRC,vDDin) *1E6*m_s2bin ) ;}
