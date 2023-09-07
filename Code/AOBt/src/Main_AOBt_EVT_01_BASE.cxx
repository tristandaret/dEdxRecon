#include "Misc/Misc.h"
#include "Misc/Util.h"
#include "Procedures/EVT_01_BASE.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_AOBt_EVT_01_BASE" << std::endl ;

  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_Tag                 (argv[1]) ;
  std::string str_OutDir              (argv[2]) ;
  std::string str_EvtFil              (argv[3]) ;
  std::string str_SelectionSet        (argv[4]) ;
  std::string str_intUploader         (argv[5]) ;
  std::string str_ModuleNber          (argv[6]) ;
  std::string str_Data_to_Use         (argv[7]) ;
  std::string str_Stg120_L            (argv[8]) ;
  std::string str_Stg120_H            (argv[9]) ;

  std::string Tag                 = str_Tag          ;
  std::string OutDir              = str_OutDir       ;
  std::string EvtFil              = str_EvtFil       ;
  std::string SelectionSet        = str_SelectionSet ;

  int intUploader = atoi( str_intUploader.data() ) ;
  int ModuleNber  = atoi( str_ModuleNber .data() ) ;
  int Data_to_Use = atoi( str_Data_to_Use.data() ) ;

  double Stg120_L = atof( str_Stg120_L.data() ) ;
  double Stg120_H = atof( str_Stg120_H.data() ) ;
  
  EVT_01_BASE(
                     0 ,
                     Tag ,
                     OutDir ,
                     EvtFil ,
                     SelectionSet ,
                     intUploader ,
                     ModuleNber  ,
                     Data_to_Use ,
                     Stg120_L ,
                     Stg120_H 
                    ) ;

}
