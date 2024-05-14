#include "AmplitudeError.h"

//
double AmplitudeError_Leading()          ; 
double AmplitudeError_NextLeading()      ;
double AmplitudeError_NextNextLeading()  ;


//
double AmplitudeError(
      const Pad* pPad                 ,
      const Pad* pPad_Leading         ,
      const Pad* pPad_NextLeading     ,
      const Pad* pPad_NextNextLeading  
){
  double ToBeReturned = std::sqrt(pPad->Get_AMax()) ;
  
  if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) ToBeReturned = ToBeReturned*AmplitudeError_Leading() ;
  
  if ( pPad_NextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) ToBeReturned = ToBeReturned*AmplitudeError_NextLeading() ;
  }
  
  if ( pPad_NextNextLeading ){
    if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) ToBeReturned = ToBeReturned*AmplitudeError_NextNextLeading() ;
  }
  
  return ToBeReturned ;
}



double AmplitudeError_Leading()         {  return   1. ;}
double AmplitudeError_NextLeading()     {  return   1. ;}
double AmplitudeError_NextNextLeading() {  return   1. ;}

