#include "Tristan/CombinedFit.h"
#include "Tristan/Misc_Functions.h"
 
int npar = 4;
int iparmuon[]      = {0, 3, 4, 5};
int iparpion[]      = {1, 3, 4, 5};
int iparproton[]    = {2, 3, 4, 5};
 
// Create the GlobalCHi2 structure
struct GlobalChi2_3 {
   GlobalChi2_3(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2, ROOT::Math::IMultiGenFunction &f3) : fChi2_1(&f1), fChi2_2(&f2), fChi2_3(&f3) {}
 
   double operator()(const double *par) const
   {
      double p1[npar];
      for (int i = 0; i < npar; ++i) p1[i] = par[iparmuon[i]];

      double p2[npar];
      for (int i = 0; i < npar; ++i) p2[i] = par[iparpion[i]];

      double p3[npar];
      for (int i = 0; i < npar; ++i) p3[i] = par[iparproton[i]];

      return (*fChi2_1)(p1) + (*fChi2_2)(p2) + (*fChi2_3)(p3) ;
   }
 
   const ROOT::Math::IMultiGenFunction *fChi2_1;
   const ROOT::Math::IMultiGenFunction *fChi2_2;
   const ROOT::Math::IMultiGenFunction *fChi2_3;
};
 



void combinedFit(std::vector<TGraphErrors*>& v_tge, std::vector<TF1*>& v_tf1)
{
   int Npar = 6;
   double par0[Npar] = {105.658e-3, 139.570e-3, 938.272e-3, 0.5, 0.511e-3, 188e-9};
 

   v_tf1[1]->               SetParameters(par0[0], par0[3], par0[4], par0[5]);
   v_tf1[1]->               FixParameter(0, 105.658e-3);

   v_tf1[2]->               SetParameters(par0[1], par0[3], par0[4], par0[5]);
   v_tf1[2]->               FixParameter(0, 105.658e-3);

   v_tf1[3]->               SetParameters(par0[2], par0[3], par0[4], par0[5]);
   v_tf1[3]->               FixParameter(0, 938.272e-3);

   v_tge[0]->               Fit(v_tf1[0], "R", "", 0, 2);


 
   // perform now global fit
  
   ROOT::Math::WrappedMultiTF1 wfmuon(*v_tf1[1], 1);
   ROOT::Math::WrappedMultiTF1 wfpion(*v_tf1[2], 1);
   ROOT::Math::WrappedMultiTF1 wfproton(*v_tf1[3], 1);
 
   ROOT::Fit::DataOptions opt;

   ROOT::Fit::DataRange rangemuon;
   rangemuon.SetRange(0, 2);
   ROOT::Fit::BinData datamuon(opt, rangemuon);
   ROOT::Fit::FillData(datamuon, v_tge[1]);

   ROOT::Fit::DataRange rangepion;
   rangepion.SetRange(0, 2);
   ROOT::Fit::BinData datapion(opt, rangepion);
   ROOT::Fit::FillData(datapion, v_tge[2]);
 
   ROOT::Fit::DataRange rangeproton;
   rangeproton.SetRange(0, 2);
   ROOT::Fit::BinData dataproton(opt, rangeproton);
   ROOT::Fit::FillData(dataproton, v_tge[3]);
 
   ROOT::Fit::Chi2Function chi2_muon(datamuon, wfmuon);
   ROOT::Fit::Chi2Function chi2_pion(datapion, wfpion);
   ROOT::Fit::Chi2Function chi2_proton(dataproton, wfproton);
 
   GlobalChi2_3 globalChi2_3(chi2_muon, chi2_pion, chi2_proton);
 
   ROOT::Fit::Fitter fitter;
 
   // create before the parameter settings in order to fix or set range on them
   fitter.Config().SetParamsSettings(6, par0);
   // fix parameters
   fitter.Config().ParSettings(0).Fix();
   fitter.Config().ParSettings(1).Fix();
   fitter.Config().ParSettings(2).Fix();
  //  fitter.Config().ParSettings(3).SetLimits(0, 10);
   fitter.Config().ParSettings(4).Fix();
   fitter.Config().ParSettings(5).SetLimits(180e-9, 200e-9); 
   fitter.Config().MinimizerOptions().SetPrintLevel(1);
   fitter.Config().SetMinimizer("Minuit2", "Migrad");
 
   // fit FCN function directly
   // (specify optionally data size and flag to indicate that is a chi2 fit)
   fitter.FitFCN(6, globalChi2_3, 0, datamuon.Size() + datapion.Size() + dataproton.Size(), true);
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);
 

   TCanvas *c1 = new TCanvas("Simfit", "Simultaneous fit", 1800, 1200);
   c1->cd();
   gStyle->SetOptFit(0);
 
   v_tf1[1]->SetFitResult(result, iparmuon);
   v_tf1[1]->SetLineColor(kBlue);
   Graphic_setup(v_tge[1], 3, 21, kBlue, 1, kBlue) ;
   v_tge[1]->SetTitle("Approximated Bethe-Bloch;Energy (GeV);dE/dx (keV/cm)");
   v_tge[1]->GetXaxis()->SetLimits(0, 2) ;
   v_tge[1]->SetMinimum(0.5);
   v_tge[1]->SetMaximum(5);
   v_tge[1]->Draw("ap");
   v_tf1[1]->Draw("same");
 
   v_tf1[2]->SetFitResult(result, iparpion);
   v_tf1[2]->SetLineColor(kGreen+1);
   Graphic_setup(v_tge[2], 3, 24, kGreen+1, 1, kGreen+1) ;
   v_tge[2]->Draw("p same");
   v_tf1[2]->Draw("same");
 
   v_tf1[3]->SetFitResult(result, iparproton);
   v_tf1[3]->SetLineColor(kRed);
   Graphic_setup(v_tge[3], 3, 25, kRed, 1, kRed) ;
   v_tge[3]->Draw("p same");
   v_tf1[3]->Draw("same");
 
   v_tf1[0]->SetLineColor(kMagenta+1);
   Graphic_setup(v_tge[0], 3, 20, kMagenta+1, 1, kMagenta+1) ;
   v_tge[0]->Draw("p same");
   v_tf1[0]->Draw("same");
   
   c1->SaveAs("Test_Simultaneous_fit_3_BB.pdf");
}