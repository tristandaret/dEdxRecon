#include "GenerateShape/GenerateShape0DFunction.h"

int main(int argc, char *argv[])
{
//
  std::cout << std::endl ;
  std::cout << "Entering Main_AOBt_GenerateShape0DFunction" << std::endl ;
//
  std::cout << std::setiosflags(std::ios::fixed);

  std::string str_iOpt              (argv[1]) ;
  std::string str_YDeposit          (argv[2]) ;
  std::string str_RCin              (argv[3]) ;
  std::string str_DriftDistanceinmm (argv[4]) ;
  
  int    iOptWS            =  atoi( str_iOpt.data() ) ;
  
  double YDeposit          =  atof( str_YDeposit.data() ) ;
  double RCin              =  atof( str_RCin.data() ) ;
  double DriftDistanceinmm =  atof( str_DriftDistanceinmm.data() ) ;

  std::cout << " iOptWS            " << iOptWS            << std::endl ;
  std::cout << " YDeposit          " << YDeposit          << std::endl ;
  std::cout << " RCin              " << RCin              << std::endl ;
  std::cout << " DriftDistanceinmm " << DriftDistanceinmm << std::endl ;
  
  GenerateShapeFunction0D_Standard(
                                   iOptWS            , 
                                   YDeposit          ,
                                   RCin              , 
                                   DriftDistanceinmm
                                  ) ;

}
