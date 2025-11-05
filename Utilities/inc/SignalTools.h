/***************************************************************************
 * File: SignalTools.h
 * Project: dEdxRecon
 *
 * Brief: Declarations for signal-processing helper functions used by
 *        reconstruction and fitting code (e.g., baseline subtraction,
 *        pulse-finding, integration routines).
 *
 * Contents: signal processing helper declarations.
 *
 * Notes: Implementations are in SignalTools.cxx.
 ***************************************************************************/

#ifndef MISC_SIGNALTOOLS_H
#define MISC_SIGNALTOOLS_H

#include "TH1F.h"

/* SIGNAL MODELISATION*/
// Electronics pulse response function
TH1F *ETF(std::string name, const float &tmin, float const &tmax,
          const float &t_0, const int &nbins, const int &iC, const float &PT,
          const float &TB);

#endif