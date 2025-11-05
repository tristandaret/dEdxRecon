#/**
 * @file TrackRecon.h
 * @brief Declarations for track reconstruction utilities and helpers.
 *
 * This header exposes the TrackRecon top-level functions that run
 * reconstruction over a Sample or an Event using a TrackFitter. Implementation
 * is in TrackRecon.cxx.
 */
#include "Misc.h"

#include "Sample.h"

#include "TrackFitter.h"

/**
 * @brief Run track reconstruction over a sample
 * @param aSample reference to the input Sample
 * @param ModuleNber module index
 * @param NberOfParameters number of fit parameters (default: 3)
 */
void TrackRecon(Sample &aSample, const int &ModuleNber,
                const int &NberOfParameters = 3);

/**
 * @brief Run track reconstruction for a single event using a TrackFitter
 * @param aTrackFitter reference to an existing TrackFitter instance
 * @param pEvent pointer to the Event to process
 * @param ModuleNber module index
 * @param NberOfParameters number of fit parameters (default: 3)
 * @return integer status (0 on success)
 */
int TrackRecon_Event(TrackFitter &aTrackFitter, Event *pEvent,
                     const int &ModuleNber, const int &NberOfParameters = 3);
