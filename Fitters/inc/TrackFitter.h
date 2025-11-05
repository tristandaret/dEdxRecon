#/***************************************************************************
 * File: TrackFitter.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of the TrackFitter class used to perform track fits.
 *        This header defines the TrackFitter interface which wraps a
 *        minimiser (ROOT's TVirtualFitter/TMinuit) and exposes methods to
 *        set the track, run minimisation and compute the chi2. It contains
 *        configuration comments about the fitting strategy and parameter
 *        ownership expectations.
 *
 * Contents: class TrackFitter { ... }
 *
 * Notes: This file only contains the public API for track fitting. The
 *        implementation is in TrackFitter.cxx.
 ***************************************************************************/

#ifndef TrackFitter_H
#define TrackFitter_H

#include "Misc.h"

#include "Track.h"

#include "TMath.h"
#include "TMatrixD.h"
#include "TMinuit.h"
#include "TVirtualFitter.h"

//
// Parameters of fit should be unconstrained
//
// Fit Basic:	MIGRAD/MINOS
//
// Fit Rescues:
//	1: MIGRAD->MINMIZE/MIGRAD
//	2: MIGRAD->SIMPLEX/MIGRAD
//
// Fit failure -> abort
//
class TrackFitter {
public:
  TrackFitter(const std::string &FitterName, const int &NberOfParameters = 2);
  virtual ~TrackFitter();

public:
  ///////////////////////////////////

  void Set_Track(Track *pTrack);

  int DoMinimisation();

  double Chi2(double par[]);

protected:
  ///////////////////////////////////

  std::string m_FitterName;

  int m_NberOfParameters;

  Track *p_Track;

  TVirtualFitter *p_TVirtualFitter; //!< Fitter
};

#endif
