#include "Misc/Misc.h"

void GenerateShapeFunction1D_BaseTest();

void GenerateShapeFunction1D_Standard(
  const int&    iOptWS            , 
  const double& RCin              , 
  const double& DriftDistanceinmm
);

void GenerateShapeFunction1D_Base(
  const double&      RCin              , 
  const double&      DriftDistanceinmm ,
  const int&         iOptWS            , 
  const std::string& TAG
);
