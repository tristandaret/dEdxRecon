#include "Misc/Misc.h"
#include "Misc/Util.h"
#include "Procedures/PV0_BASE.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_AOBt_PV0_BASE" << std::endl ;

  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_Flavor              (argv[1]) ;
  std::string str_OutDir              (argv[2]) ;
  std::string str_EventFile           (argv[3]) ;
  std::string str_PRFfile_Fixed_RC_DD (argv[4]) ;
  std::string str_Uploader            (argv[5]) ;
  std::string str_StartFrom           (argv[6]) ;
  std::string str_iOpt_Initial_YT_fit (argv[7]) ;
  std::string str_iOpt_TrackType      (argv[8]) ;
  std::string str_ModuleNber          (argv[9]) ;
  std::string str_SelectionSet       (argv[10]) ;
  std::string str_Data_to_Use        (argv[11]) ;

  std::string Flavor              = str_Flavor      ;
  std::string OutDir              = str_OutDir      ;
  std::string EventFile           = str_EventFile   ;
  std::string PRFfile_Fixed_RC_DD = str_PRFfile_Fixed_RC_DD ;

  int Uploader                    = atoi( str_Uploader           .data() ) ;
  int StartFrom                   = atoi( str_StartFrom          .data() ) ;
  int iOpt_Initial_YT_fit         = atoi( str_iOpt_Initial_YT_fit.data() ) ;
  int iOpt_TrackType              = atoi( str_iOpt_TrackType     .data() ) ;
  int ModuleNber                  = atoi( str_ModuleNber         .data() ) ;
  int Data_to_Use                 = atoi( str_Data_to_Use         .data() ) ;

  std::string SelectionSet        = str_SelectionSet ;

  PV0_BASE(
           0                   ,
           Flavor              ,
           OutDir              ,
           EventFile           ,
           PRFfile_Fixed_RC_DD ,
           Uploader            ,
           StartFrom           ,
           iOpt_Initial_YT_fit ,
           iOpt_TrackType      ,
           ModuleNber          ,
           SelectionSet        ,
           Data_to_Use
         );

}
