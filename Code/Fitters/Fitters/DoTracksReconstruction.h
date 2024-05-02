#include "Misc/Misc.h"

#include "EvtModel/Sample.h"

#include "Fitters/TheFitterTrack.h"

void DoTracksReconstruction(
                            Sample& aSample, const int& ModuleNber,
                            const int& NberOfParameters = 3
);

int DoTracksReconstruction_Event(
                            TheFitterTrack& aTheFitterTrack , 
                            Event* pEvent, const int& ModuleNber ,
                            const int& NberOfParameters = 3
);
