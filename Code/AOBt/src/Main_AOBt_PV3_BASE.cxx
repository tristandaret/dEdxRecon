#include "Misc/Misc.h"
#include "Misc/Util.h"
#include "Procedures/PV3_BASE.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_AOBt_PV3_BASE" << std::endl ;

  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_Flavor                  (argv[1]) ;
  std::string str_OutDir                  (argv[2]) ;
  std::string str_EventFile               (argv[3]) ;
  std::string str_FILE_SC_VS_x_rc_dd      (argv[4]) ;
  std::string str_DD_Sample               (argv[5]) ;
  std::string str_Uploader                (argv[6]) ;
  std::string str_StartFrom               (argv[7]) ;
  std::string str_iOpt_TrackType          (argv[8]) ;
  std::string str_SamplingFrequency       (argv[9]) ;
  std::string str_ModuleNber             (argv[10]) ;
  std::string str_SelectionSet           (argv[11]) ;
  std::string str_Data_to_Use            (argv[12]) ;

  std::string Flavor             = str_Flavor             ;
  std::string OutDir             = str_OutDir             ;
  std::string EventFile          = str_EventFile          ;
  std::string FILE_SC_VS_x_rc_dd = str_FILE_SC_VS_x_rc_dd ;
  
  double      DD_Sample = atof( str_DD_Sample.data() ) ;
  
  int Uploader                = atoi( str_Uploader          .data() ) ;
  int StartFrom               = atoi( str_StartFrom         .data() ) ;
  int iOpt_TrackType          = atoi( str_iOpt_TrackType    .data() ) ;
  int ModuleNber              = atoi( str_ModuleNber        .data() ) ;
  int Data_to_Use             = atoi( str_Data_to_Use       .data() ) ;

  double      SamplingFrequency = atof( str_SamplingFrequency.data() ) ;
  
  std::string SelectionSet        = str_SelectionSet ;

  PV3_BASE(
           0                  ,
           Flavor             ,
           OutDir             ,
           EventFile          ,
           FILE_SC_VS_x_rc_dd ,
           DD_Sample          ,
           Uploader           ,
           StartFrom          ,
           iOpt_TrackType     ,
           SamplingFrequency  ,
           ModuleNber         ,
           SelectionSet       ,
           Data_to_Use
          );

}
