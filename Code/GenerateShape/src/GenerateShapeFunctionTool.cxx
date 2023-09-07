#include "GenerateShape/GenerateShapeFunctionTool.h"
#include "Misc/Util.h"

GenerateShapeFunctionTool::GenerateShapeFunctionTool(
                             std::vector < Pad* >& V_Pad ,
                             const double& YTrue_min ,
                             const double& YTrue_max 
){
  double Time0  = 0. ;

  int iYTrue_Max = 10 ;

  int iPad_Max = V_Pad.size() ;

  double Tmin =     0. * 1E-9 ;
  double Tmax = 20000. * 1E-9 ; 
  int    Npoints = 10000 ;

  std::vector < double > Vtp_X ;
  std::vector < double > Vtp_T ;
  for (int iYTrue = 0 ; iYTrue< iYTrue_Max; iYTrue++){
    double XTrue = 0. ;
    double YTrue = YTrue_min + double(iYTrue)*(YTrue_max -YTrue_min)/double(iYTrue_Max-1) ;
    
    for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      Pad* pPad = V_Pad[iPad] ;
      pPad->SetSignalModel( Time0 , XTrue, YTrue );

      double S_Pad_max = 0. ;
      double T_Pad_max = 0. ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
                
        double S_Pad = pPad->Get_APad(TimeCur) ;
        if (iPoint==0){
          S_Pad_max = S_Pad   ;
          T_Pad_max = TimeCur ;
        }else{
          if (S_Pad_max<=S_Pad) {
            S_Pad_max = S_Pad ;
            T_Pad_max = TimeCur ;
          }else{
            break ;
          }
        }
      }

      Vtp_X.push_back( std::fabs( (YTrue - pPad->Get_YPad())*1.E2 ) );
      Vtp_T.push_back(            T_Pad_max                 *1.E6   );
      
    }
  }

  int iTem_Max = Vtp_X.size() ;
  
  std::vector < RankedValue > V_RankedValue ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    RankedValue aRankedValue ; 
    aRankedValue.Value = Vtp_X[iTem]  ; 
    aRankedValue.Rank  = iTem       ; 
    V_RankedValue.push_back( aRankedValue );
  }
  std::sort(V_RankedValue.begin(), V_RankedValue.end());

  std::vector < double > Vtp_Xn;
  std::vector < double > Vtp_Tn ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    Vtp_Xn.push_back( Vtp_X[ V_RankedValue[iTem].Rank ] );
    Vtp_Tn.push_back( Vtp_T[ V_RankedValue[iTem].Rank ] );
  }
 
  std::vector < int > IsValid ; 
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    if (iTem==0){
      IsValid.push_back(1) ;
      continue ;
    }
    if ( std::fabs( Vtp_Xn[iTem]- Vtp_Xn[iTem-1]) <= 0.000001 ){
      IsValid.push_back(0) ;
    }else{
      IsValid.push_back(1) ;
    }
  }
  
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    if (IsValid[iTem]==1){
      V_X.push_back( Vtp_Xn[ iTem] );
      V_T.push_back( Vtp_Tn[ iTem] );
    }
  }
  
  m_NberOfData = V_X.size() ;
//   for (int iTem = 0 ; iTem< m_NberOfData; iTem++){
//     std::cout 
//       << " iTem "  << iTem
//       << " V_X " << V_X[iTem]
//       << " V_T " << V_T[iTem]
//       << std::endl ;
//   }
  m_vX_Highest = V_X[m_NberOfData-1] ;

}

GenerateShapeFunctionTool::~GenerateShapeFunctionTool()
{
}

double GenerateShapeFunctionTool::TimeEstimate(const double& vXin)
{
  double vX = std::fabs( vXin );
  
  double ToBeReturned = 0.;
  
  if (m_vX_Highest<=vX) {
   ToBeReturned = V_T[m_NberOfData-1] ;
  
  }else{
    double Index = -1 ;
    for (int iData = 0 ; iData< m_NberOfData-1 ; iData++){
      if ( V_X[iData] <= vX ) Index = iData ;
    }   
     
    if (Index==-1){
      std::cout 
        << " GenerateShapeFunctionTool::TimeEstimate "
        << " Index==-1 "
        << " for  vXin " << vXin
        << std::endl;
    }
    
    ToBeReturned = V_T[Index] + ( vX -V_X[Index] ) * ( V_T[Index+1]-V_T[Index] )/( V_X[Index+1]-V_X[Index] );
  
  }
  
  return ToBeReturned - 0.2 ;
}

TGraph*GenerateShapeFunctionTool:: Draw_TimeEstimator(const double& Est_Min, const double& Est_Max)
{
  TGraph* ToBeReturned  = new TGraph ;
  int iEst_Max = 100 ;
  for (int iEst = 0 ; iEst< iEst_Max; iEst++){
    double Xcur = Est_Min + double (iEst)*(Est_Max-Est_Min)/double(iEst_Max-1) ;
    double Ycur = TimeEstimate( Xcur ) ;
    ToBeReturned ->SetPoint( ToBeReturned ->GetN(), Xcur  , Ycur  ) ;
  }
  
  return ToBeReturned ;
}

