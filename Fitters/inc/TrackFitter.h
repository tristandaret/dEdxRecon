/**
 * @file TrackFitter.h
 * @brief Declaration of the TrackFitter class used to perform track fits.
 *
 * This header defines the TrackFitter interface which wraps a minimiser
 * (ROOT's TVirtualFitter/TMinuit) and exposes methods to set the track,
 * run minimisation and compute the chi2. The implementation lives in
 * TrackFitter.cxx.
 *
 * @details
 * Implementation notes: the corresponding source file, TrackFitter.cxx,
 * contains the concrete interfacing code to ROOT's minimisers and the
 * static bridge used by Minuit (a file-local helper class that forwards
 * the minimiser callbacks to a TrackFitter instance). The Chi2 evaluation
 * routine and Minuit callback registration are performed in the source
 * implementation. Care must be taken not to change the global/static
 * bridge semantics as Minuit requires a static callback wrapper.
 */

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
  /**
   * @brief Construct a TrackFitter
   * @param FitterName name used to identify the fitter instance
   * @param NberOfParameters number of fit parameters (default: 2)
   */
  TrackFitter(const std::string &FitterName, const int &NberOfParameters = 2);

  /**
   * @brief Virtual destructor
   */
  virtual ~TrackFitter();

public:
  ///////////////////////////////////

  /**
   * @brief Set the track to be fitted
   * @param pTrack pointer to a Track object (ownership not transferred)
   */
  void Set_Track(Track *pTrack);

  /**
   * @brief Run the minimisation routine
   * @return status code (0 on success, non-zero on failure)
   */
  int DoMinimisation();

  /**
   * @brief Compute chi2 for the current parameter vector
   * @param par parameter array used for the chi2 evaluation
   * @return computed chi2 value
   */
  double Chi2(double par[]);

protected:
  ///////////////////////////////////

  std::string m_FitterName;

  int m_NberOfParameters;

  Track *p_Track;

  TVirtualFitter *p_TVirtualFitter; //!< Fitter
};

#endif
