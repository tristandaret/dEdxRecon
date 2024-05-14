#include "Misc.h"

#include "Sample.h"

#include "TrackFitter.h"

void TrackRecon(
                            Sample& aSample, const int& ModuleNber,
                            const int& NberOfParameters = 3
);

int TrackRecon_Event(
                            TrackFitter& aTrackFitter , 
                            Event* pEvent, const int& ModuleNber ,
                            const int& NberOfParameters = 3
);
