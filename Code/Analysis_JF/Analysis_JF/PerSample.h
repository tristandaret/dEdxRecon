#include "Misc/Misc.h"

#include "TGraphErrors.h"
#include "TH1F.h"
#include "TF1.h"

void PerSample(const std::string& SampleSet);

void PerRun(
   const std::string& OUTDirNameIN,
   const std::string& TheOutDirName,
   const std::string& base_EventFile,
   const std::string& AnalyseName 
);
