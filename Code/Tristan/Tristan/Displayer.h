#include "Misc/Misc.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

void Displayer(
                        const std::string& OutDir ,
                        std::string const& Tag ,
                        std::string const& SelectionSet ,
                        Uploader*          pUploader ,
                        int         const& Moduleslist  ,
                        int         const& Data_to_Use,
                        std::string const& particle) ;


// Draw pulse electronics response
TH1F* Displayer_DPR(const std::string OUTDirName, const float& tmin, float const& tmax, const float& t_0, const int& step = 1) ;
