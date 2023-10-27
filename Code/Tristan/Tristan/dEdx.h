#include "Misc/Misc.h"

#include "Tristan/ReadLUT.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"


void dEdx( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& Comment,
                          std::string const& EventFile,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use,
                          Interpol4          LUT,
                          int         const& PT,
                          int         const& TB,
                          float       const& zdrift) ;