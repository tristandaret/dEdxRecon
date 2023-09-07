#include "SignalShape/SignalTime_Global_Exact.h"
#include "Misc/Util.h"

SignalTime_Global_Exact::SignalTime_Global_Exact(const std::string& LoadFile)
{
  m_LoadFile = LoadFile ;

  m_s2bin = 1000./40. ;

  std::cout <<" =>SignalTime_Global_Exact::SignalTime_Global_Exact : " << m_LoadFile << std::endl ;

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
    SignalTime_Individual_Exact* pSignalTime_Individual_Exact = new SignalTime_Individual_Exact(V_File[iTem]) ;
    V_SignalTime_Individual_Exact.push_back(pSignalTime_Individual_Exact);
  }
  
  m_RC_Lowest  = V_RC[0] ;
  m_RC_Highest = V_RC[m_NberOfFiles-1] ;
  
}

SignalTime_Global_Exact::~SignalTime_Global_Exact()
{
  for (int iTem = 0 ; iTem< m_NberOfFiles; iTem++){
    delete V_SignalTime_Individual_Exact[iTem] ;
    V_SignalTime_Individual_Exact[iTem] = 0 ;
  }
  V_SignalTime_Individual_Exact.clear() ;
  V_RC.clear()  ;
  V_File.clear()  ;
  
}

double SignalTime_Global_Exact::Eval_RhoPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SignalTime_Individual_Exact[0]              ->Eval_RhoPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SignalTime_Individual_Exact[m_NberOfFiles-1]->Eval_RhoPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SignalTime_Individual_Exact[Index]  ->Eval_RhoPRF(vX) ;
  double YH = V_SignalTime_Individual_Exact[Index+1]->Eval_RhoPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SignalTime_Global_Exact::Eval_AlphaPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SignalTime_Individual_Exact[0]              ->Eval_AlphaPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SignalTime_Individual_Exact[m_NberOfFiles-1]->Eval_AlphaPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SignalTime_Individual_Exact[Index]  ->Eval_AlphaPRF(vX) ;
  double YH = V_SignalTime_Individual_Exact[Index+1]->Eval_AlphaPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SignalTime_Global_Exact::Eval_TauPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SignalTime_Individual_Exact[0]              ->Eval_TauPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SignalTime_Individual_Exact[m_NberOfFiles-1]->Eval_TauPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SignalTime_Individual_Exact[Index]  ->Eval_TauPRF(vX) ;
  double YH = V_SignalTime_Individual_Exact[Index+1]->Eval_TauPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SignalTime_Global_Exact::Eval_TauBinPRF(const double& vX, const double& vRC  )
{
 return ( Eval_TauPRF(vX,vRC) * m_s2bin ) ; 
}

double SignalTime_Global_Exact::Eval_TimeAbsPRF(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SignalTime_Individual_Exact[0]              ->Eval_TimeAbsPRF(vX) ;
  if ( vRC >= m_RC_Highest) return V_SignalTime_Individual_Exact[m_NberOfFiles-1]->Eval_TimeAbsPRF(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SignalTime_Individual_Exact[Index]  ->Eval_TimeAbsPRF(vX) ;
  double YH = V_SignalTime_Individual_Exact[Index+1]->Eval_TimeAbsPRF(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}

double SignalTime_Global_Exact::Eval_Amplitude(const double& vX, const double& vRC  )
{
  if ( vRC <= m_RC_Lowest ) return V_SignalTime_Individual_Exact[0]              ->Eval_Amplitude(vX) ;
  if ( vRC >= m_RC_Highest) return V_SignalTime_Individual_Exact[m_NberOfFiles-1]->Eval_Amplitude(vX) ;
  
  int Index = int( (vRC - m_RC_Lowest)/5. )  ;
  double XL = V_RC[Index] ;
  double XH = V_RC[Index+1] ;
  double YL = V_SignalTime_Individual_Exact[Index]  ->Eval_Amplitude(vX) ;
  double YH = V_SignalTime_Individual_Exact[Index+1]->Eval_Amplitude(vX) ;

  return ( YL + (vRC -XL)*(YH-YL)/(XH-XL) ) ;
    
}
