#ifndef SignalTime_Exact_AlphaPRF_H
#define SignalTime_Exact_AlphaPRF_H

#include "Misc/Misc.h"
#include "SignalShape/SignalTime_Global_Exact.h"

/////////////////////////////////////////////////////////////
class SignalTime_Exact_AlphaPRF {
public:
   /** Constructor */
   SignalTime_Exact_AlphaPRF(
                        SignalTime_Global_Exact* pSignalTime_Global_Exact 
                       );
   virtual ~SignalTime_Exact_AlphaPRF();
   
//Evaluate AlphaPRF, vX in cm, vRC in ns mm^-2
   double Eval(const double& vX, const double& vRC  ) ;
   
   double operator() (double *x, double *par) ;

private:

  SignalTime_Global_Exact* p_SignalTime_Global_Exact ;

};



#endif

