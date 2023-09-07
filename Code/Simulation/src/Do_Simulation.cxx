#include "Simulation/Do_Simulation.h"

#include "SampleTools/Generate_Simulation1D.h"
#include "SampleTools/Generate_Simulation0D.h"

void Do_Simulation()
{
//   Generate_Simulation1D(1,  std::string("Sim_02_NoSmearing" ), 0 , 100. , 1000. , 0. , 10000 ) ;
//   Generate_Simulation1D(1,  std::string("Sim_02_CteSigma"   ), 1 , 100. , 1000. ,  0. , 10000 ) ;
//   Generate_Simulation1D(1,  std::string("Sim_02_SqrtASigma" ), 2 , 100. , 1000. ,  0. , 10000 ) ;

//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_NoSmearing" ), 0 , 83. , 1000. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_CteSigma"   ), 1 , 83. , 1000. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_SqrtASigma" ), 2 , 83. , 1000. ,  0. , 1000 ) ;
// 
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC050_G1000_SqrtASigma" ), 2 ,  50. , 1000. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC100_G1000_SqrtASigma" ), 2 , 100. , 1000. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC150_G1000_SqrtASigma" ), 2 , 150. , 1000. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC200_G1000_SqrtASigma" ), 2 , 200. , 1000. ,  0. , 1000 ) ;
//   
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC100_G1500_SqrtASigma" ), 2 , 100. , 1500. ,  0. , 1000 ) ;
//   Generate_Simulation0D(1,  std::string("Sim_4Shiwam_03_RC100_G2000_SqrtASigma" ), 2 , 100. , 2000. ,  0. , 1000 ) ;
//
  Generate_Simulation0D(1,  std::string("Sim_4Shiwam_04_RC050_G1000_SqrtASigma" ), 2 ,  50. , 1000. ,  0. , 10000 ) ;
  Generate_Simulation0D(1,  std::string("Sim_4Shiwam_04_RC100_G1000_SqrtASigma" ), 2 , 100. , 1000. ,  0. , 10000 ) ;
  Generate_Simulation0D(1,  std::string("Sim_4Shiwam_04_RC150_G1000_SqrtASigma" ), 2 , 150. , 1000. ,  0. , 10000 ) ;
  Generate_Simulation0D(1,  std::string("Sim_4Shiwam_04_RC200_G1000_SqrtASigma" ), 2 , 200. , 1000. ,  0. , 10000 ) ;
//   
}
