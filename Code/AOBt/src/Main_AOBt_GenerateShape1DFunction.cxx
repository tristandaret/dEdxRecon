#include "GenerateShape/GenerateShape1DFunction.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_AOBt_GenerateShape1DFunction" << std::endl ;
//
  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_iOpt              (argv[1]) ;
  std::string str_RCin              (argv[2]) ;
  std::string str_DriftDistanceinmm (argv[3]) ;
  
  int    iOptWS            =  atoi( str_iOpt.data() ) ;
  
  double RCin              =  atof( str_RCin.data() ) ;
  double DriftDistanceinmm =  atof( str_DriftDistanceinmm.data() ) ;

  std::cout << " iOptWS            " << iOptWS            << std::endl ;
  std::cout << " RCin              " << RCin              << std::endl ;
  std::cout << " DriftDistanceinmm " << DriftDistanceinmm << std::endl ;
  
  GenerateShapeFunction1D_Standard(
                                   iOptWS            , 
                                   RCin              , 
                                   DriftDistanceinmm
                                  ) ;

}
