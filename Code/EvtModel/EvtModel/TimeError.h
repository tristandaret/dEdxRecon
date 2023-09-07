#ifndef TimeError_H
#define TimeError_H

#include "Misc/Misc.h"

#include "EvtModel/Pad.h"

double TimeErrorPV31(
      const Pad* pPad                 ,
      const Pad* pPad_Leading         ,
      const Pad* pPad_NextLeading     ,
      const Pad* pPad_NextNextLeading  
);
double TimeErrorPV31_BASE( const double& Tau);


double TimeError(const double& DeltaT_in);

double TimeError(
      const Pad* pPad                 ,
      const Pad* pPad_Leading         ,
      const Pad* pPad_NextLeading     ,
      const Pad* pPad_NextNextLeading  
);

#endif

