/***************************************************************************
 * File: AmplitudeError.h
 * Project: dEdxRecon
 *
 * Brief: Declarations for amplitude error models and containers. Used to
 *        represent uncertainties on amplitude measurements (per-pad or per-
 *        cluster) during fitting and dE/dx estimation.
 *
 * Contents: AmplitudeError class declaration and accessors.
 *
 * Notes: Implementation in AmplitudeError.cxx.
 ***************************************************************************/

#ifndef AmplitudeError_H
#define AmplitudeError_H

#include "Misc.h"

#include "Pad.h"

double AmplitudeError(Pad *pPad, Pad *pPad_Leading, Pad *pPad_NextLeading,
                      Pad *pPad_NextNextLeading);

#endif
