#ifndef SignalTime_Exact_RhoPRF_H
#define SignalTime_Exact_RhoPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SignalTime_Global_Exact.h"

/////////////////////////////////////////////////////////////
class SignalTime_Exact_RhoPRF {
public:
   /** Constructor */
   SignalTime_Exact_RhoPRF(
                        SignalTime_Global_Exact* pSignalTime_Global_Exact 
                       );
   virtual ~SignalTime_Exact_RhoPRF();
   
//Evaluate RhoPRF, vX in cm, vRC in ns mm^-2
   double Eval(const double& vX, const double& vRC  ) ;
   
   double operator() (double *x, double *par) ;

private:

  SignalTime_Global_Exact* p_SignalTime_Global_Exact ;

};



#endif

