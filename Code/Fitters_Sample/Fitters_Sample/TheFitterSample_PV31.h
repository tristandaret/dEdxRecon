#ifndef TheFitterSample_PV31_H
#define TheFitterSample_PV31_H

#include "Misc/Misc.h"

#include "EvtModel/Sample.h"

#include "TMinuit.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TVirtualFitter.h"

//
// Fit Basic:  MIGRAD/MINOS
//
// Fit Rescues:
//   1: MIGRAD->MINMIZE/MIGRAD
//   2: MIGRAD->SIMPLEX/MIGRAD
//
// Fit failure -> Failure procedure called
//
class TheFitterSample_PV31 {
public:
   TheFitterSample_PV31(
               const std::string& FitterName
            );
   virtual ~TheFitterSample_PV31();

public:
///////////////////////////////////

  void Set_Sample (Sample* pSample) ;
  
  int DoMinimisation();

  double Chi2(double par[]); 


protected:
///////////////////////////////////

  std::string m_FitterName    ; 

  Sample* p_Sample    ; 
   
  TVirtualFitter* p_TVirtualFitter ; //!< Fitter

};



#endif

