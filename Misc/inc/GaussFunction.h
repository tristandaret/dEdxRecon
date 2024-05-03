#ifndef GaussFunction_H
#define GaussFunction_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class GaussFunction{
public:
   /** Constructor */
   GaussFunction();
   virtual ~GaussFunction();
   
   double Get_Norm () ;
   double Get_Mean () ;
   double Get_Sigma() ;
   void   Set_Norm (const double& Norm ) ;
   void   Set_Mean (const double& Mean ) ;
   void   Set_Sigma(const double& Sigma) ;
  
   double operator() (double *x, double *par) ;

private:

  double m_Norm  ;
  double m_Mean  ;
  double m_Sigma ;
  
};



#endif

