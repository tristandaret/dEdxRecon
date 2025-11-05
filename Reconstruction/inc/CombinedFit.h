#/***************************************************************************
 * File: CombinedFit.h
 * Project: dEdxRecon
 *
 * Brief: Declaration for combined fitting utilities used for higher-level
 *        fits, e.g., fitting Bethe-Bloch curves or combining multiple data
 *        series into a single fit procedure.
 *
 * Contents: combinedFit function declaration.
 *
 * Notes: Implementation lives in CombinedFit.cxx.
 ***************************************************************************/

#ifndef COMBINEDFIT_H
#define COMBINEDFIT_H

#include <Fit/BinData.h>
#include <Fit/Chi2FCN.h>
#include <Fit/Fitter.h>
#include <HFitInterface.h>
#include <Math/WrappedMultiTF1.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TStyle.h>

void combinedFit(std::vector<TGraphErrors *> &v_tge, std::vector<TF1 *> &v_tf1);

#endif // COMBINEDFIT_H