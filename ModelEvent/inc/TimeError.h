#ifndef TimeError_H
#define TimeError_H

#include "Misc.h"

#include "Pad.h"

double TimeError(const double& DeltaT_in);

double TimeError(
		const Pad* pPad,
		const Pad* pPad_Leading,
		const Pad* pPad_NextLeading,
		const Pad* pPad_NextNextLeading	
);

#endif

