/***************************************************************************
 * File: TimeError.h
 * Project: dEdxRecon
 *
 * Brief: Declarations for time/temporal error models used to represent
 *        uncertainties in time measurements (per-sample or per-pad).
 *
 * Contents: TimeError class/utility declarations.
 *
 * Notes: Implementation in TimeError.cxx.
 ***************************************************************************/

#ifndef TimeError_H
#define TimeError_H

#include "Misc.h"

#include "Pad.h"

double TimeError(const double &DeltaT_in);

double TimeError(Pad *pPad, Pad *pPad_Leading, Pad *pPad_NextLeading,
                 Pad *pPad_NextNextLeading);

#endif
