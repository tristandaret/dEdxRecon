#include <Fit/Fitter.h>
#include <Fit/BinData.h>
#include <Fit/Chi2FCN.h>
#include <TH1.h>
#include <Math/WrappedMultiTF1.h>
#include <HFitInterface.h>
#include <TCanvas.h>
#include <TStyle.h>

#include "Tristan/Misc_Functions.h"
 
// definition of shared parameter
// protons
int npar = 6;
int iparmuon[] = {105.658e-3, 1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01};
 
// muons
int iparproton[] = {938.272e-3, 1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01};
 
// Create the GlobalCHi2 structure
 
struct GlobalChi2 {
   GlobalChi2(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2) : fChi2_1(&f1), fChi2_2(&f2) {}
 
   // parameter vector is first background (in common 1 and 2)
   // and then is signal (only in 2)
   double operator()(const double *par) const
   {
      double p1[npar];
      for (int i = 0; i < npar; ++i)
         p1[i] = par[iparmuon[i]];
 
      double p2[npar];
      for (int i = 0; i < npar; ++i)
         p2[i] = par[iparproton[i]];
 
      return (*fChi2_1)(p1) + (*fChi2_2)(p2);
   }
 
   const ROOT::Math::IMultiGenFunction *fChi2_1;
   const ROOT::Math::IMultiGenFunction *fChi2_2;
};
 
void combinedFit()
{
 
   TH1D *hmuon = new TH1D("hmuon", "histo muon", 100, 0, 100);
   TH1D *hproton = new TH1D("hproton", "histo proton", 100, 0, 100);
 
   TF1 *fmuon = new TF1("fmuon", "expo", 0, 100);
   TF1 *fmuon = BetheBlochExp(0, 2, iparmuon[0], "muon");
   hmuon->FillRandom("fmuon");

   TF1 *fproton = BetheBlochExp(0, 2, iparproton[0], "proton");
 
   hproton->FillRandom("fproton", 1000);
 
   // perform now global fit
 
   TF1 *fproton = new TF1("fproton", "expo + gaus(2)", 0, 100);
 
   ROOT::Math::WrappedMultiTF1 wfmuon(*fmuon, 1);
   ROOT::Math::WrappedMultiTF1 wfproton(*fproton, 1);
 
   ROOT::Fit::DataOptions opt;
   ROOT::Fit::DataRange rangemuon;
   // set the data range
   rangemuon.SetRange(10, 90);
   ROOT::Fit::BinData datamuon(opt, rangemuon);
   ROOT::Fit::FillData(datamuon, hmuon);
 
   ROOT::Fit::DataRange rangeproton;
   rangeproton.SetRange(10, 50);
   ROOT::Fit::BinData dataproton(opt, rangeproton);
   ROOT::Fit::FillData(dataproton, hproton);
 
   ROOT::Fit::Chi2Function chi2_muon(datamuon, wfmuon);
   ROOT::Fit::Chi2Function chi2_proton(dataproton, wfproton);
 
   GlobalChi2 globalChi2(chi2_muon, chi2_proton);
 
   ROOT::Fit::Fitter fitter;
 
   const int Npar = 6;
   double par0[Npar] = {5, 5, -0.1, 100, 30, 10};
 
   // create before the parameter settings in order to fix or set range on them
   fitter.Config().SetParamsSettings(6, par0);
   // fix 5-th parameter
   fitter.Config().ParSettings(4).Fix();
   // set limits on the third and 4-th parameter
   fitter.Config().ParSettings(2).SetLimits(-10, -1.E-4);
   fitter.Config().ParSettings(3).SetLimits(0, 10000);
   fitter.Config().ParSettings(3).SetStepSize(5);
 
   fitter.Config().MinimizerOptions().SetPrintLevel(0);
   fitter.Config().SetMinimizer("Minuit2", "Migrad");
 
   // fit FCN function directly
   // (specify optionally data size and flag to indicate that is a chi2 fit)
   fitter.FitFCN(6, globalChi2, 0, datamuon.Size() + dataproton.Size(), true);
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);
 
   TCanvas *c1 = new TCanvas("Simfit", "Simultaneous fit of two histograms", 10, 10, 700, 700);
   c1->Divide(1, 2);
   c1->cd(1);
   gStyle->SetOptFit(1111);
 
   fmuon->SetFitResult(result, iparmuon);
   fmuon->SetRange(rangemuon().first, rangemuon().second);
   fmuon->SetLineColor(kBlue);
   hmuon->GetListOfFunctions()->Add(fmuon);
   hmuon->Draw();
 
   c1->cd(2);
   fproton->SetFitResult(result, iparproton);
   fproton->SetRange(rangeproton().first, rangeproton().second);
   fproton->SetLineColor(kRed);
   hproton->GetListOfFunctions()->Add(fproton);
   hproton->Draw();
}