#/**
 * @file AmplitudeError.h
 * @brief Amplitude error model utilities.
 *
 * Declares helpers to compute amplitude uncertainties for pads and clusters
 * used during fitting and dE/dx estimation.
 */
#ifndef AmplitudeError_H
#define AmplitudeError_H

#include "Misc.h"

#include "Pad.h"

double AmplitudeError(Pad *pPad, Pad *pPad_Leading, Pad *pPad_NextLeading,
                      Pad *pPad_NextNextLeading);

#endif
