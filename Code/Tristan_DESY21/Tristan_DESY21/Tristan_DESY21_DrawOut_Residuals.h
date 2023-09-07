#include "Misc/Misc.h"

#include "EvtModel/Sample.h"

#include "TGraphErrors.h"

void Tristan_DESY21_DrawOut_Residuals      (Sample& aSample, const int& ModuleNber , const std::string& OUTDIR, const std::string& TAG, TGraphErrors* ptge_std_mean, TGraphErrors* ptge_bias_mean, const int& zDrift, const int& iIter) ;

void Tristan_DrawOut_DD_functions   (const std::string& Stage, TGraphErrors* ptge_std_mean, TGraphErrors* ptge_bias_mean, TGraphErrors* ptge_RC, const int& pt) ;
