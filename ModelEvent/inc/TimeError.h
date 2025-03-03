#ifndef TimeError_H
#define TimeError_H

#include "Misc.h"

#include "Pad.h"

double TimeError(const double &DeltaT_in);

double TimeError(Pad *pPad, Pad *pPad_Leading, Pad *pPad_NextLeading, Pad *pPad_NextNextLeading);

#endif
