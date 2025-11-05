#/***************************************************************************
 * File: TrackRecon.h
 * Project: dEdxRecon
 *
 * Brief: Declarations for track reconstruction utilities and helpers.
 *        This header exposes the TrackRecon top-level functions that run
 *        reconstruction over a Sample or an Event using a TrackFitter.
 *
 * Contents: function declarations: TrackRecon, TrackRecon_Event
 *
 * Notes: Implementation is in TrackRecon.cxx. This header intentionally keeps
 *        a minimal API surface and expects callers to provide samples/events
 *        and a configured TrackFitter.
 ***************************************************************************/

#include "Misc.h"

#include "Sample.h"

#include "TrackFitter.h"

void TrackRecon(Sample &aSample, const int &ModuleNber,
                const int &NberOfParameters = 3);

int TrackRecon_Event(TrackFitter &aTrackFitter, Event *pEvent,
                     const int &ModuleNber, const int &NberOfParameters = 3);
