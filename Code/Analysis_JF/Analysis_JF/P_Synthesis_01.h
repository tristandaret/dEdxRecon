#include "Misc/Misc.h"

#include "TGraphErrors.h"
#include "TH1F.h"
#include "TF1.h"

void P_Synthesis_01();
void P_Synthesis_02(const std::string& SampleSet);

//
void P_Synthesis_DDvsZZ_01(const std::string& SampleSet);

void P_Synthesis_RCvsZZ_01(const std::string& SampleSet);

void P_Synthesis_YTYPAD_Iter_01(const std::string& SampleSet);
void P_Synthesis_YTYPAD_Input_Final_01(const std::string& SampleSet);
void P_Synthesis_YTYPAD_Final_01(const std::string& SampleSet);

void P_Synthesis_DumpPRFparameters_01(const std::string& SampleSet);

void P_Synthesis_ParamPRFLike_Iter_01(const std::string& SampleSet);

void P_Synthesis_RESO_01(const std::string& SampleSet);

void P_Synthesis_PULL_01(const std::string& SampleSet);

void P_Synthesis_QUALITY_01(const std::string& SampleSet);

void P_Synthesis_CHI2_01(const std::string& SampleSet);

void P_Synthesis_BEAMPROFILE_01(const std::string& SampleSet);

//
void P_Synthesis_Sp01();
