#include "Misc/Misc.h"

#include "Tristan/ReadLUT_vROOT.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"


void dEdx_XPonly( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& Comment,
                          std::string const& EventFile,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use,
                          LUT4               LUT,
                          int         const& PT,
                          int         const& TB,
                          float       const& zdrift) ;