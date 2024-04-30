#ifndef CORR_H
#define CORR_H

#include "Misc/Misc.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

void corr(const std::string& OutDir,
          std::string const& Tag,
          std::string const& SelectionSet,
          Uploader* pUploader,
          int const& NbrOfMod,
          int const& Data_to_Use);

#endif // CORR_H