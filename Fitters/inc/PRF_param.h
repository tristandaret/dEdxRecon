#ifndef PRF_param_H
#define PRF_param_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class PRF_param {
public:
   /** Constructor */
   PRF_param();
   virtual ~PRF_param();
   
//Evaluate PRF, Xin in cm
   double Eval(
                   const double& Xin
                  );
   
   double operator() (double *x, double *par) ;

   void SetPRF(
               const double&  Norm ,
               const double&    A2 ,
               const double&    A4 ,
               const double&    B2 ,
               const double&    B4 
              );
              
private:
   double m_Norm ;
   double m_A2   ;
   double m_A4   ;
   double m_B2   ;
   double m_B4   ;
};



#endif

