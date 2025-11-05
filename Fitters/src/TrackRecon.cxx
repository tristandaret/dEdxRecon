#/***************************************************************************
 * File: TrackRecon.cxx
 * Project: dEdxRecon
 *
 * Brief: Implementation for the top-level track reconstruction routines.
 *        Implements TrackRecon and TrackRecon_Event which orchestrate
 *        fitting over Samples and Events using the TrackFitter API.
 *
 * Contents: TrackRecon(Sample&, ...), TrackRecon_Event(...)
 *
 * Notes: Keep high-level control flow here; heavy numerical code is in
 *        the fitter classes.
 ***************************************************************************/

#include "TrackRecon.h"

#include "Cluster.h"
#include "Event.h"
#include "Track.h"

void TrackRecon(Sample &aSample, const int &ModuleNber,
                const int &NberOfParameters) {

  TrackFitter aTrackFitter("Minuit", NberOfParameters);

  int Kounter_Fit = 0;
  int Kounter_Failure = 0;

  int NberOfEvents = aSample.Get_NberOfEvents();
  for (int iE = 0; iE < NberOfEvents; iE++) {
    Event *pEvent = aSample.Get_Event(iE);

    if (pEvent->Validity_ForThisModule(ModuleNber) == 0)
      continue;

    int FitResult =
        TrackRecon_Event(aTrackFitter, pEvent, ModuleNber, NberOfParameters);

    Kounter_Fit += 1;
    if (FitResult != 0)
      Kounter_Failure += 1;
  }
  std::cout << std::endl;
  std::cout << " TrackRecon " << std::endl;
  std::cout << "	Nber of Fits		" << std::setw(20)
            << Kounter_Fit << std::endl;
  std::cout << "	Nber of Failed Fits " << std::setw(20)
            << Kounter_Failure << " ( " << std::setw(10) << std::setprecision(4)
            << 100. * double(Kounter_Failure) / double(Kounter_Fit) << " % ) "
            << std::endl;
}

int TrackRecon_Event(TrackFitter &aTrackFitter, Event *pEvent,
                     const int &ModuleNber, const int &NberOfParameters) {

  Track *pTrack_ForEvent = 0;
  pEvent->Set_Track_ForThisModule(pTrack_ForEvent, ModuleNber);

  pTrack_ForEvent = pEvent->GiveMe_AnUnfittedTrack_ForThisModule(ModuleNber);

  aTrackFitter.Set_Track(pTrack_ForEvent);

  int ToBeReturned = aTrackFitter.DoMinimisation();

  pEvent->Set_Track_ForThisModule(pTrack_ForEvent, ModuleNber);

  // pTrack_ForEvent->DumpRec();

  return ToBeReturned;
}
