#ifndef TheFitterCluster_PV3_H
#define TheFitterCluster_PV3_H

#include "Misc/Misc.h"

#include "EvtModel/Cluster.h"

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
class TheFitterCluster_PV3 {
public:
   TheFitterCluster_PV3(
               const std::string& FitterName
            );
   virtual ~TheFitterCluster_PV3();

public:
///////////////////////////////////

  void Set_Cluster (Cluster* pCluster) ;
  
  int DoMinimisation();

  double Chi2(double par[]); 


protected:
///////////////////////////////////

  std::string m_FitterName    ; 

  Cluster* p_Cluster    ; 
   
  TVirtualFitter* p_TVirtualFitter ; //!< Fitter

};



#endif

