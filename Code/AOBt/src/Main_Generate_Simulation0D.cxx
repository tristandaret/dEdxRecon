#include "Misc/Misc.h"
#include "Misc/Util.h"
#include "SampleTools/Generate_Simulation0D.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_Generate_Simulation0D" << std::endl ;

  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_Output_RootFile   (argv[1]) ;
  std::string str_iOptSmear         (argv[2]) ;
  std::string str_RC_ns_mmM2        (argv[3]) ;
  std::string str_Gain              (argv[4]) ;
  std::string str_DD_mm             (argv[5]) ;
  std::string str_NberOfEvents      (argv[6]) ;

  std::string Output_RootFile     = str_Output_RootFile          ;

  int iOptSmear    = atoi( str_iOptSmear.data()    ) ;
  int NberOfEvents = atoi( str_NberOfEvents.data() ) ;

  double RC_ns_mmM2 = atof( str_RC_ns_mmM2.data() ) ;
  double Gain       = atof( str_Gain.data()       ) ;
  double DD_mm      = atof( str_DD_mm.data()      ) ;
  
  Generate_Simulation0D( 
                         0       ,
                         Output_RootFile ,
                         iOptSmear       ,
                         RC_ns_mmM2      ,
                         Gain            ,
                         DD_mm           ,
                         NberOfEvents
                       );

}
