#include "Misc/Misc.h"

#include "EvtModel/Sample.h"

#include "TF1.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "SignalShape/SC_VS_x_rc_dd_SET_dd.h"

//-------------------------------------PRF like Fits-------------------------------/
TGraphErrors* FitRhoVsYTYP (
                            int& fitStatus ,
                            TH2D* pTH2D, 
                            const std::string& FitName, 
                            const double& vY_min_DiffPRF, 
                            const double& vY_max_DiffPRF
                           );

int  Do_SampleFit_PV2 (Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed);

int  Do_SampleFit_PV3 (Sample& aSample, const int& ModuleNber, SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd);

int  Do_SampleFit_PV31 (Sample& aSample, const int& ModuleNber, SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd);

int  Do_SampleFit_PV4 (Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed);
