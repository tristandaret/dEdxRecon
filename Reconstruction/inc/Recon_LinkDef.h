#/***************************************************************************
 * File: Recon_LinkDef.h
 * Project: dEdxRecon
 *
 * Brief: ROOT dictionary/linkdef file for classes and namespaces used in the
 *        Reconstruction module and for external types like EramInfo. This file
 *        is processed by rootcling to generate dictionaries for I/O and
 *        interactive use.
 *
 * Contents: #pragma link directives for Reconstruction classes and containers.
 *
 * Notes: This file is only meaningful to ROOT's dictionary generation tools.
 ***************************************************************************/

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