#include "PID/CombinedFit.h"
#include "PID/Tools.h"
 
// definition of parameters (0 to 4 shared, last one is particle mass)
int npar = 6;
int iparmuon[]      = {5, 0, 1, 2, 3, 4};
int iparpion[]      = {6, 0, 1, 2, 3, 4};
int iparproton[]    = {7, 0, 1, 2, 3, 4};
int iparpositron[]  = {8, 0, 1, 2, 3, 4};
 
// Create the GlobalCHi2 structure
struct GlobalChi2_4 {
   GlobalChi2_4(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2, ROOT::Math::IMultiGenFunction &f3, ROOT::Math::IMultiGenFunction &f4) : fChi2_1(&f1), fChi2_2(&f2), fChi2_3(&f3), fChi2_4(&f4) {}
 
   double operator()(const double *par) const
   {
      double p1[npar];
      for (int i = 0; i < npar; ++i) p1[i] = par[iparmuon[i]];

      double p2[npar];
      for (int i = 0; i < npar; ++i) p2[i] = par[iparpion[i]];

      double p3[npar];
      for (int i = 0; i < npar; ++i) p3[i] = par[iparproton[i]];

      double p4[npar];
      for (int i = 0; i < npar; ++i) p4[i] = par[iparpositron[i]];

      return (*fChi2_1)(p1) + (*fChi2_2)(p2) + (*fChi2_3)(p3) + (*fChi2_4)(p4) ;
   }
 
   const ROOT::Math::IMultiGenFunction *fChi2_1;
   const ROOT::Math::IMultiGenFunction *fChi2_2;
   const ROOT::Math::IMultiGenFunction *fChi2_3;
   const ROOT::Math::IMultiGenFunction *fChi2_4;
};
 



void combinedFit(std::vector<TGraphErrors*>& v_tge, std::vector<TF1*>& v_tf1)
{
   int Npar = 9;
   // double par0[Npar] = {1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // THATPID raw
   // double par0[Npar] = {0.301580,    3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // THATPID raw normalized
   // double par0[Npar] = {0.186543, 5.382656, 0.004234, 2.028548, -0.994807, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // THATPID fit

   // double par0[Npar] = {785,      6.047, 0.00064, 2.308, -1.359, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // Claudio raw
   // double par0[Npar] = {0.148800, 6.047, 0.00064, 2.308, -1.359, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // Claudio raw normalized
   // double par0[Npar] = {1, 6, 0.0005, 2, -1.5, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // Claudio init
   // double par0[Npar] = {0.186524, 5.382954, 0.004232, 2.028659, -0.994959, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // Claudio fit

   double par0[Npar] = {7, 3, 5, 0.5, -0.5, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // PID init
   // double par0[Npar] = {8.001897, 2.916423, 5.291697, 0.377516, -0.274286, 105.658e-3, 139.570e-3, 938.272e-3, 0.511e-3}; // PID fit

   v_tf1[1]->               SetParameters(par0[5], par0[0], par0[1], par0[2], par0[3], par0[4]);
   v_tf1[1]->               FixParameter(0, par0[5]);

   v_tf1[2]->               SetParameters(par0[6], par0[0], par0[1], par0[2], par0[3], par0[4]);
   v_tf1[2]->               FixParameter(0, par0[6]);

   v_tf1[3]->               SetParameters(par0[7], par0[0], par0[1], par0[2], par0[3], par0[4]);
   v_tf1[3]->               FixParameter(0, par0[7]);

   v_tf1[0]->               SetParameters(par0[8], par0[0], par0[1], par0[2], par0[3], par0[4]);
   v_tf1[0]->               FixParameter(0, par0[8]);


 
   // perform now global fit
  
   ROOT::Math::WrappedMultiTF1 wfmuon(*v_tf1[1], 1);
   ROOT::Math::WrappedMultiTF1 wfpion(*v_tf1[2], 1);
   ROOT::Math::WrappedMultiTF1 wfproton(*v_tf1[3], 1);
   ROOT::Math::WrappedMultiTF1 wfpositron(*v_tf1[0], 1);
 
   ROOT::Fit::DataOptions opt;

   ROOT::Fit::DataRange rangemuon;
   rangemuon.SetRange(0, 100);
   ROOT::Fit::BinData datamuon(opt, rangemuon);
   ROOT::Fit::FillData(datamuon, v_tge[1]);

   ROOT::Fit::DataRange rangepion;
   rangepion.SetRange(0, 100);
   ROOT::Fit::BinData datapion(opt, rangepion);
   ROOT::Fit::FillData(datapion, v_tge[2]);
 
   ROOT::Fit::DataRange rangeproton;
   rangeproton.SetRange(0, 100);
   ROOT::Fit::BinData dataproton(opt, rangeproton);
   ROOT::Fit::FillData(dataproton, v_tge[3]);
 
   ROOT::Fit::DataRange rangepositron;
   rangepositron.SetRange(0, 100);
   ROOT::Fit::BinData datapositron(opt, rangepositron);
   ROOT::Fit::FillData(datapositron, v_tge[0]);
 
   ROOT::Fit::Chi2Function chi2_muon(datamuon, wfmuon);
   ROOT::Fit::Chi2Function chi2_pion(datapion, wfpion);
   ROOT::Fit::Chi2Function chi2_proton(dataproton, wfproton);
   ROOT::Fit::Chi2Function chi2_positron(datapositron, wfpositron);
 
   GlobalChi2_4 GlobalChi2_4(chi2_muon, chi2_pion, chi2_proton, chi2_positron);
 
   ROOT::Fit::Fitter fitter;
 
   // create before the parameter settings in order to fix or set range on them
   fitter.Config().SetParamsSettings(9, par0);

   // fitter.Config().ParSettings(0).SetLimits(-0.5, -0.2);
   // fitter.Config().ParSettings(1).Fix();
   // fitter.Config().ParSettings(2).Fix();
   // fitter.Config().ParSettings(3).Fix();
   // fitter.Config().ParSettings(4).Fix();
   fitter.Config().ParSettings(5).Fix();
   fitter.Config().ParSettings(6).Fix();
   fitter.Config().ParSettings(7).Fix();
   fitter.Config().ParSettings(8).Fix();
   fitter.Config().MinimizerOptions().SetPrintLevel(1);
   fitter.Config().SetMinimizer("Minuit2", "Migrad");
 
   // fit FCN function directly
   // (specify optionally data size and flag to indicate that is a chi2 fit)
   fitter.FitFCN(9, GlobalChi2_4, 0, datamuon.Size() + datapion.Size() + dataproton.Size() + datapositron.Size(), true);
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);
 

   TCanvas *c1 = new TCanvas("Simfit", "Simultaneous fit", 1800, 1200);
   c1->cd();
   gStyle->SetOptFit(0);

   v_tf1[1]->SetFitResult(result, iparmuon);
   v_tf1[1]->SetLineColor(kBlue);
   Graphic_setup(v_tge[1], 3, 21, kBlue, 1, kBlue) ;
   v_tf1[1]->SetTitle("Approximated Bethe-Bloch;Energy (GeV);dE/dx (keV/cm)");
   v_tf1[1]->GetXaxis()->SetLimits(0, 20) ;
   v_tf1[1]->SetMinimum(0.5);
   v_tf1[1]->SetMaximum(3);
   gPad->SetLogx();
   v_tf1[1]->Draw();
   v_tge[1]->Draw("p same");
 
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
 
   v_tf1[0]->SetFitResult(result, iparpositron);
   v_tf1[0]->SetLineColor(kMagenta+1);
   Graphic_setup(v_tge[0], 3, 20, kMagenta+1, 1, kMagenta+1) ;
   v_tge[0]->Draw("p same");
   v_tf1[0]->Draw("same");
   
   c1->SaveAs("Test_Simultaneous_fit_4approx.pdf");
   delete c1;
}