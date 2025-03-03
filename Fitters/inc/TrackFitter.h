#ifndef TrackFitter_H
#define TrackFitter_H

#include "Misc.h"

#include "Track.h"

#include "TMinuit.h"
#include "TMath.h"
#include "TMatrixD.h"
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
