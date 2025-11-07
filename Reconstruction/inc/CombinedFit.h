#/**
 * @file CombinedFit.h
 * @brief Combined fitting utilities declaration.
 *
 * Declares the combinedFit helper used to fit multiple TGraphErrors/TF1
 * series together (e.g., high-level fits like Bethe-Bloch curve fits).
 *
 * @details
 * Implementation coordinates multiple TF1 instances and ROOT fit helpers to
 * perform simultaneous fits across different data series (for example,
 * fitting Bethe-Bloch parameters across particle species). It uses ROOT's
 * fitting framework including WrappedMultiTF1 and the ROOT::Fit::Fitter
 * interface and exposes a single helper function `combinedFit` which takes
 * vectors of TGraphErrors and TF1 pointers and runs a global chi2 fit.
 */
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