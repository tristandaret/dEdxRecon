#ifndef AmplitudeError_H
#define AmplitudeError_H

#include "Misc.h"

#include "Pad.h"

double AmplitudeError(
		const Pad* pPad,
		const Pad* pPad_Leading,
		const Pad* pPad_NextLeading,
		const Pad* pPad_NextNextLeading	
);

#endif

