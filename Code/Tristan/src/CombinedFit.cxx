#include "Tristan/CombinedFit.h"
#include "Tristan/Misc_Functions.h"
 
// definition of parameters (0 to 4 shared, last one is particle mass)
int npar = 6;
int iparmuon[]    = {0, 1, 2, 3, 4, 5};
int iparproton[]  = {0, 1, 2, 3, 4, 6};
 

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
 



void combinedFit(std::vector<TGraphErrors*> v_tge)
{
   const int Npar = 7;
   double par0[Npar] = {1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01, 105.658e-3, 938.272e-3};

   TGraphErrors *tge_muon = v_tge[1];
   TGraphErrors *tge_proton = v_tge[3];
 
   const char* formula    = "[1]/pow(x/sqrt(x*x+[0]*[0]),[4]) * ( [2] - pow(x/sqrt(x*x+[0]*[0]),[4]) - log([3]+ pow(x*x/[0]*[0], [5])) )" ;

   TF1* fmuon             = new TF1("dEdx_muon", formula, 0, 2, "");
   fmuon->                  SetParameters(par0[5], par0[0], par0[1], par0[2], par0[3], par0[4]);
   fmuon->                  FixParameter(0, 105.658e-3);
   TF1* fproton           = new TF1("dEdx_muon", formula, 0, 2, "");
   fproton->                SetParameters(par0[6], par0[0], par0[1], par0[2], par0[3], par0[4]);
   fproton->                FixParameter(0, 938.272e-3);

 
   // perform now global fit
  
   ROOT::Math::WrappedMultiTF1 wfmuon(*fmuon, 1);
   ROOT::Math::WrappedMultiTF1 wfproton(*fproton, 1);
 
   ROOT::Fit::DataOptions opt;

   ROOT::Fit::DataRange rangemuon;
   rangemuon.SetRange(0, 2);
   ROOT::Fit::BinData datamuon(opt, rangemuon);
   ROOT::Fit::FillData(datamuon, tge_muon);
 
   ROOT::Fit::DataRange rangeproton;
   rangeproton.SetRange(0, 2);
   ROOT::Fit::BinData dataproton(opt, rangeproton);
   ROOT::Fit::FillData(dataproton, tge_proton);
 
   ROOT::Fit::Chi2Function chi2_muon(datamuon, wfmuon);
   ROOT::Fit::Chi2Function chi2_proton(dataproton, wfproton);
 
   GlobalChi2 globalChi2(chi2_muon, chi2_proton);
 
   ROOT::Fit::Fitter fitter;
 
   // create before the parameter settings in order to fix or set range on them
   fitter.Config().SetParamsSettings(7, par0);
   // fix parameters
   fitter.Config().ParSettings(5).Fix();
   fitter.Config().ParSettings(6).Fix();
   // // set limits on the third and 4-th parameter
   // fitter.Config().ParSettings(2).SetLimits(-10, -1.E-4);
   // fitter.Config().ParSettings(3).SetLimits(0, 10000);
   // fitter.Config().ParSettings(3).SetStepSize(5);
 
   fitter.Config().MinimizerOptions().SetPrintLevel(1);
   fitter.Config().SetMinimizer("Minuit2", "Migrad");
 
   // fit FCN function directly
   // (specify optionally data size and flag to indicate that is a chi2 fit)
   fitter.FitFCN(7, globalChi2, 0, datamuon.Size() + dataproton.Size(), true);
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);
 

   TCanvas *c1 = new TCanvas("Simfit", "Simultaneous of muons & protons", 1800, 1200);
   c1->cd();
   gStyle->SetOptFit(1111);
 
   fmuon->SetFitResult(result, iparmuon);
   fmuon->SetLineColor(kBlue);
   Graphic_setup(tge_muon, 3, 21, kBlue, 1, kBlue) ;
   tge_muon->GetXaxis()->SetLimits(0.45, 1.55) ;
   tge_muon->SetMinimum(0.5);
   tge_muon->SetMaximum(5);
   tge_muon->Draw("ap");
   fmuon->Draw();
 
   fproton->SetFitResult(result, iparproton);
   fproton->SetLineColor(kRed);
   Graphic_setup(tge_proton, 3, 25, kRed, 1, kRed) ;
   tge_proton->Draw("p same");
  //  fproton->Draw();
   
   c1->SaveAs("Test_Simultaneous_fit.pdf");
}