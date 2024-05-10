#include <vector>
#include "dEdx.h"
#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// For dEdx output
#pragma link C++ namespace Reconstruction;

#pragma link C++ namespace Reconstruction;
#pragma link C++ class Reconstruction::TPad+;
#pragma link C++ class std::vector<Reconstruction::TPad*>+;
#pragma link C++ class Reconstruction::TCluster+;
#pragma link C++ class std::vector<Reconstruction::TCluster*>+;
#pragma link C++ class Reconstruction::TModule+;
#pragma link C++ class std::vector<Reconstruction::TModule*>+;
#pragma link C++ class Reconstruction::TEvent+;

// For ERAM maps reading 
#pragma link C++ class EramInfo+;
#endif