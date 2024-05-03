#ifndef FuncFromTGraph_H
#define FuncFromTGraph_H

#include "Misc.h"

#include "TGraph.h"

// 
/////////////////////////////////////////////////////////////
class FuncFromTGraph{
public:
   /** Constructor */
   FuncFromTGraph(TGraph* pTGraph);
   virtual ~FuncFromTGraph();
   
   double Get_Y(const double& vX) ;
   
   void Dump();

   double operator() (double *x, double *par) ;

private:

  TGraph* p_TGraph  ;
  double  m_StepX ;
  double  m_X_Lowest ;
  double  m_X_Highest ;
  
};



#endif

