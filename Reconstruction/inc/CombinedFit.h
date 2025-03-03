#ifndef COMBINEDFIT_H
#define COMBINEDFIT_H

#include <Fit/Fitter.h>
#include <Fit/BinData.h>
#include <Fit/Chi2FCN.h>
#include <TH1.h>
#include <Math/WrappedMultiTF1.h>
#include <HFitInterface.h>
#include <TCanvas.h>
#include <TStyle.h>

void combinedFit(std::vector<TGraphErrors *> &v_tge, std::vector<TF1 *> &v_tf1);

#endif // COMBINEDFIT_H