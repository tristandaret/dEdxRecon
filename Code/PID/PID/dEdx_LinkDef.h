#include <vector>
#include "PID/dEdx.h"
#ifdef __ROOTCLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace PID;
#pragma link C++ class PID::TPad+;
#pragma link C++ class std::vector<PID::TPad*>+;
#pragma link C++ class PID::TCluster+;
#pragma link C++ class std::vector<PID::TCluster*>+;
#pragma link C++ class PID::TModule+;
#pragma link C++ class std::vector<PID::TModule*>+;
#pragma link C++ class PID::TEvent+;

#endif