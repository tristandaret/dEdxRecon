#ifndef GenerateShapeFunctionTool_H
#define GenerateShapeFunctionTool_H

#include "Misc/Misc.h"

#include "EvtModel/Pad.h"

#include "TGraph.h"

/////////////////////////////////////////////////////////////
class GenerateShapeFunctionTool {
public:
   /** Constructor */
   GenerateShapeFunctionTool(
                             std::vector < Pad* >& V_Pad ,
                             const double& YTrue_min ,
                             const double& YTrue_max 
                            ) ;
   virtual ~GenerateShapeFunctionTool();
   
   double TimeEstimate(const double& vXin) ;
   
   TGraph* Draw_TimeEstimator(const double& Est_Min, const double& Est_Max);
   
private:
  
  int m_NberOfData ;
  std::vector < double > V_X ;
  std::vector < double > V_T ;
  double m_vX_Highest;
  
};



#endif

