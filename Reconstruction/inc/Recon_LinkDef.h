#include <vector>
#include "dEdx.h"
#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// For dEdx output
#pragma link C++ namespace Reconstruction;

#pragma link C++ namespace Reconstruction;
#pragma link C++ class Reconstruction::RecoPad + ;
#pragma link C++ class std::vector < Reconstruction::RecoPad *> + ;
#pragma link C++ class Reconstruction::RecoCluster + ;
#pragma link C++ class std::vector < Reconstruction::RecoCluster *> + ;
#pragma link C++ class Reconstruction::RecoModule + ;
#pragma link C++ class std::vector < Reconstruction::RecoModule *> + ;
#pragma link C++ class Reconstruction::RecoEvent + ;

// For ERAM maps reading
#pragma link C++ class EramInfo + ;
#endif