#include "Misc/Misc.h"

void GenerateShapeFunction();


void GenerateShapeFunction_Short   (const double& RCin, const double&  DriftDistanceinmm) ;
void GenerateShapeFunction_Standard(const int& iOptWS, const double& RCin, const double&  DriftDistanceinmm);

void GenerateShapeFunction_Base(const double& RCin, const double&  DriftDistanceinmm,const int& iOptWS, const std::string& TAG);
