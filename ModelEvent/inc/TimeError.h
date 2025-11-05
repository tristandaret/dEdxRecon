#/**
 * @file TimeError.h
 * @brief Time error model utilities.
 *
 * Declares helpers to compute temporal uncertainties used during signal
 * processing and fitting.
 */
#ifndef TimeError_H
#define TimeError_H

#include "Misc.h"

#include "Pad.h"

double TimeError(const double &DeltaT_in);

double TimeError(Pad *pPad, Pad *pPad_Leading, Pad *pPad_NextLeading,
                 Pad *pPad_NextNextLeading);

#endif
