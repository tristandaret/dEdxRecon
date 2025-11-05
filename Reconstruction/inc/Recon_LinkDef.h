#/**
 * @file Recon_LinkDef.h
 * @brief ROOT linkdef file for Reconstruction module classes.
 *
 * Contains #pragma link directives used by rootcling to generate dictionaries
 * for I/O and interactive ROOT usage.
 */
#include "dEdx.h"
#include <vector>
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