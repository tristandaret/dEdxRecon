#include "Misc/Misc.h"

#include "Tristan/ReadLUT_vROOT.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "Tristan/ReadLUT_vTTree.h"


void dEdx( const std::string& OutDir,
                          std::string const& Tag,
                          std::string const& Comment,
                          std::string const& EventFile,
                          std::string const& SelectionSet,
                          Uploader*          pUploader,
                          int         const& NbrOfMod,
                          int         const& Data_to_Use,
                          LUT*               p_lut,
                          int         const& PT,
                          int         const& TB,
                          float       const& zdrift) ;