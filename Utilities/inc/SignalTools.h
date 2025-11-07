
/**
 * @file SignalTools.h
 * @brief Signal-processing helper declarations (baseline, pulse finding, ...)
 *
 * Provides utility declarations used by reconstruction and fitting code for
 * basic signal processing tasks such as building ETF pulse shapes.
 */
#
#/**
 * @details
 * The corresponding source file SignalTools.cxx implements ETF and other
 * electronics-response helpers used to simulate and normalise pulse shapes.
 * These utilities are used by reconstruction and testing code that needs a
 * reproducible electronics impulse response (ETF) and related helpers.
 */

#ifndef MISC_SIGNALTOOLS_H
#define MISC_SIGNALTOOLS_H

#include "TH1F.h"

// Electronics pulse response function
TH1F *ETF(std::string name, const float &tmin, float const &tmax,
          const float &t_0, const int &nbins, const int &iC, const float &PT,
          const float &TB);

#endif