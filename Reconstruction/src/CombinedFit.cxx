#include "CombinedFit.h"
#include "ReconTools.h"
#include "SetStyle.h"
 
// definition of parameters (0 to 4 shared, last one is particle mass)
int npar =				6;
int iparpositron[] =	{5, 0, 1, 2, 3, 4};
int iparmuon[] =		{6, 0, 1, 2, 3, 4};
int iparpion[] =		{7, 0, 1, 2, 3, 4};
int iparproton[] =		{8, 0, 1, 2, 3, 4};
 
// Create the GlobalCHi2 structure
struct GlobalChi2_4 {
	GlobalChi2_4(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2, ROOT::Math::IMultiGenFunction &f3, ROOT::Math::IMultiGenFunction &f4) : fChi2_1(&f1), fChi2_2(&f2), fChi2_3(&f3), fChi2_4(&f4) {}
 
	double operator()(const double *par) const
	{
		double p1[npar];
		for (int i	= 0; i < npar; ++i) p1[i]	= par[iparpositron[i]];

		double p2[npar];
		for (int i	= 0; i < npar; ++i) p2[i]	= par[iparmuon[i]];

		double p3[npar];
		for (int i	= 0; i < npar; ++i) p3[i]	= par[iparpion[i]];

		double p4[npar];
		for (int i	= 0; i < npar; ++i) p4[i]	= par[iparproton[i]];

		return (*fChi2_1)(p1) + (*fChi2_2)(p2) + (*fChi2_3)(p3) + (*fChi2_4)(p4);
	}
 
	const ROOT::Math::IMultiGenFunction *fChi2_1;
	const ROOT::Math::IMultiGenFunction *fChi2_2;
	const ROOT::Math::IMultiGenFunction *fChi2_3;
	const ROOT::Math::IMultiGenFunction *fChi2_4;
};
 



void combinedFit(std::vector<TGraphErrors*>& v_tge, std::vector<TF1*>& v_tf1)
{
	int Npar	= 9;

	double par0[Npar] = 	{2, 3.5, 0.03, 2, 0.7, 0.511e-3, 105.658e-3, 139.570e-3, 938.272e-3}; 											// From hatRecon (initialization) => amplitude reset
	// double par0[Npar] = 	{1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, 7.14413e-01, 0.511e-3, 105.658e-3, 139.570e-3, 938.272e-3};// From hatRecon

	v_tf1[0]->				SetParameters(par0[5], par0[0], par0[1], par0[2], par0[3], par0[4]);
	v_tf1[0]->				FixParameter(0, par0[5]);

	v_tf1[1]->				SetParameters(par0[6], par0[0], par0[1], par0[2], par0[3], par0[4]);
	v_tf1[1]->				FixParameter(0, par0[6]);

	v_tf1[2]->				SetParameters(par0[7], par0[0], par0[1], par0[2], par0[3], par0[4]);
	v_tf1[2]->				FixParameter(0, par0[7]);

	v_tf1[3]->				SetParameters(par0[8], par0[0], par0[1], par0[2], par0[3], par0[4]);
	v_tf1[3]->				FixParameter(0, par0[8]);


 
	// perform now global fit
	
	ROOT::Math::WrappedMultiTF1 wfpositron(*v_tf1[0], 1);
	ROOT::Math::WrappedMultiTF1 wfmuon(*v_tf1[1], 1);
	ROOT::Math::WrappedMultiTF1 wfpion(*v_tf1[2], 1);
	ROOT::Math::WrappedMultiTF1 wfproton(*v_tf1[3], 1);
 
	ROOT::Fit::DataOptions opt;
 
	ROOT::Fit::DataRange rangepositron;
	rangepositron.SetRange(0, 100);
	ROOT::Fit::BinData datapositron(opt, rangepositron);
	ROOT::Fit::FillData(datapositron, v_tge[0]);

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
 
	ROOT::Fit::Chi2Function chi2_positron(datapositron, wfpositron);
	ROOT::Fit::Chi2Function chi2_muon(datamuon, wfmuon);
	ROOT::Fit::Chi2Function chi2_pion(datapion, wfpion);
	ROOT::Fit::Chi2Function chi2_proton(dataproton, wfproton);
 
	GlobalChi2_4 GlobalChi2_4(chi2_positron, chi2_muon, chi2_pion, chi2_proton);
 
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
	fitter.FitFCN(9, GlobalChi2_4, 0, datapositron.Size() + datamuon.Size() + datapion.Size() + dataproton.Size(), true);
	ROOT::Fit::FitResult result	= fitter.Result();
	result.Print(std::cout);
 

	TCanvas *c1	= new TCanvas("Simfit", "Simultaneous fit", 1800, 1350);
	c1->cd();
	gStyle->SetOptFit(0);
	std::vector<int> markersCERN = 	{20, 21, 33, 34};
	std::vector<int> colorsCERN = 	{kOrange+7, kAzure+2, kGreen+3, kRed+1};

	v_tf1[0]->SetFitResult(result, iparpositron);
	v_tf1[0]->SetLineColor(colorsCERN[0]);
	Graphic_setup(v_tge[0], 4, markersCERN[0], colorsCERN[0], 1, kBlack);
	v_tf1[0]->SetTitle(";Energy (GeV);dE/dx (keV/cm)");
	v_tf1[0]->GetXaxis()->SetLimits(0, 20);
	v_tf1[0]->SetMinimum(0.5);
	v_tf1[0]->SetMaximum(3);
	gPad->SetLogx();
	v_tf1[0]->Draw();
	v_tge[0]->Draw("p same");
 
	v_tf1[1]->SetFitResult(result, iparmuon);
	v_tf1[1]->SetLineColor(colorsCERN[1]);
	Graphic_setup(v_tge[1], 4, markersCERN[1], colorsCERN[1], 1, kBlack);
	v_tge[1]->Draw("p same");
	v_tf1[1]->Draw("same");
 
	v_tf1[2]->SetFitResult(result, iparpion);
	v_tf1[2]->SetLineColor(colorsCERN[2]);
	Graphic_setup(v_tge[2], 4, markersCERN[2], colorsCERN[2], 1, kBlack);
	v_tge[2]->Draw("p same");
	v_tf1[2]->Draw("same");
 
	v_tf1[3]->SetFitResult(result, iparproton);
	v_tf1[3]->SetLineColor(colorsCERN[3]);
	Graphic_setup(v_tge[3], 4, markersCERN[3], colorsCERN[3], 1, kBlack);
	v_tge[3]->Draw("p same");
	v_tf1[3]->Draw("same");
	
	c1->SaveAs("../OUT_Reconstruction/CERN22_Energy/Test_Simultaneous_fit_4approx.pdf");
	delete c1;
}