#include "EvtModel/TimeError.h"

//
double TimeError_Leading()          ; 
double TimeError_NextLeading()      ;
double TimeError_NextNextLeading()  ;

double TimeError_BASE(const double& DeltaT_in);


//
double TimeError(
      const Pad* pPad                 ,
      const Pad* pPad_Leading         ,
      const Pad* pPad_NextLeading     ,
      const Pad* pPad_NextNextLeading  
){
  double ToBeReturned = TimeError( pPad->Get_TMax() - pPad_Leading->Get_TMax()) ;
  
  if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) ToBeReturned = TimeError_Leading() ;
  
  if ( pPad_NextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) ToBeReturned = TimeError_NextLeading() ;
  }
  
  if ( pPad_NextNextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) ToBeReturned = TimeError_NextNextLeading() ;
  }
  
  return ToBeReturned ;
}


double TimeError(const double& DeltaT_in)
{
 return 3. ;
 return (TimeError_BASE(DeltaT_in))/5. ;
}

double TimeError_BASE(const double& DeltaT_in)
{
 double DeltaT = std::fabs(DeltaT_in) ;
 
 double X1 = 25. ; double Y1 =  4. ;
 if (DeltaT <= X1 ) return Y1 ;
 
 double X2 = 40. ; double Y2 = 10. ;
 if (DeltaT_in <= X2) return ( (DeltaT_in-X1)*(Y2-Y1)/(X2-X1) + Y1 ) ;

 double X3 = 50. ; double Y3 = 25. ;
 return ( (DeltaT_in-X2)*(Y3-Y2)/(X3-X2) + Y2 ) ;
 
}

double TimeError_Leading()         {  return   2. * 10000000. ; }
double TimeError_NextLeading()     {  return   2. * 10000000. ; }
double TimeError_NextNextLeading() {  return   2. * 10000000. ; }
double TimeError_BASE(const double& DeltaT_in);


double TimeErrorPV31(
      const Pad* pPad                 ,
      const Pad* pPad_Leading         ,
      const Pad* pPad_NextLeading     ,
      const Pad* pPad_NextNextLeading  
){
  double ToBeReturned = 1 ;

  if ( pPad_NextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) {
      double Tau = pPad_NextLeading->Get_TMax() - pPad_Leading->Get_TMax() ; ;
      ToBeReturned = TimeErrorPV31_BASE(Tau) ;
      ToBeReturned = std::sqrt(2.) ;
    }
  }
  
  if ( pPad_NextNextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) {
      double Tau = pPad_NextNextLeading->Get_TMax() - pPad_Leading->Get_TMax() ; ;
      ToBeReturned = TimeErrorPV31_BASE(Tau) ;
      ToBeReturned = std::sqrt(1+std::pow(4.5,2)) ;
    }
  }
  
  return ToBeReturned ;
}
double TimeErrorPV31_BASE( const double& Tau)
{
  double          ToBeReturned =   1. ; 
  if (Tau >=  1.) ToBeReturned =   1. ; 
  if (Tau >=  2.) ToBeReturned =   2. ; 
  if (Tau >=  3.) ToBeReturned =   2. ; 
  if (Tau >=  4.) ToBeReturned =   2. ; 
  if (Tau >=  5.) ToBeReturned =   2. ; 
  if (Tau >=  6.) ToBeReturned =   3. ; 
  if (Tau >=  7.) ToBeReturned =   3. ; 
  if (Tau >=  8.) ToBeReturned =   6. ; 
  if (Tau >=  9.) ToBeReturned =   6. ; 
  if (Tau >= 10.) ToBeReturned =  10. ; 
  if (Tau >= 11.) ToBeReturned =  12. ; 
  if (Tau >= 12.) ToBeReturned =  16. ; 
  if (Tau >= 13.) ToBeReturned =  16. ; 
  if (Tau >= 14.) ToBeReturned =  18. ; 
  if (Tau >= 15.) ToBeReturned =  20. ; 
  if (Tau >= 16.) ToBeReturned =  20. ; 
  if (Tau >= 17.) ToBeReturned =  20. ; 
  if (Tau >= 18.) ToBeReturned =  20. ; 
  if (Tau >= 19.) ToBeReturned =  20. ; 
  if (Tau >= 20.) ToBeReturned =  20. ; 
  if (Tau >= 21.) ToBeReturned =  20. ; 
  if (Tau >= 22.) ToBeReturned =  20. ; 
  if (Tau >= 23.) ToBeReturned =  20. ; 
  if (Tau >= 24.) ToBeReturned =  20. ; 
  if (Tau >= 25.) ToBeReturned =  20. ; 
  if (Tau >= 26.) ToBeReturned =  20. ; 
  if (Tau >= 27.) ToBeReturned =  20. ; 
  if (Tau >= 28.) ToBeReturned =  20. ; 
  if (Tau >= 29.) ToBeReturned =  20. ; 
  ToBeReturned =   1. ; 
  if (Tau >=  6.) ToBeReturned =   std::sqrt(1+std::pow(4.5,2)) ; 
  return ToBeReturned ;
}
