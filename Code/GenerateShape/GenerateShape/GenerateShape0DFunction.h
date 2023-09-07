#include "Misc/Misc.h"


void GenerateShapeFunction0D_BaseTest();

void GenerateShapeFunction0D_Standard(
  const int&    iOptWS            , 
  const double& YDeposit          ,
  const double& RCin              , 
  const double& DriftDistanceinmm
);

void GenerateShapeFunction0D_Base(
  const double&      YDeposit          ,
  const double&      RCin              , 
  const double&      DriftDistanceinmm ,
  const int&         iOptWS            , 
  const std::string& TAG
);
