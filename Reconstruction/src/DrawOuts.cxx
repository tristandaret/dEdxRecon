#include "DrawOuts.h"
#include "ReconTools.h"
#include "CombinedFit.h"
#include "dEdx.h"

#include "Util.h"
#include <cmath>
#include <numeric>

#include "TFile.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TLegendEntry.h"
#include "TROOT.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLatex.h"

// Constructor delegation
Reconstruction::DrawOuts::DrawOuts(const std::string &inputFile)
    : DrawOuts(std::vector<std::string>{inputFile}) {}

// Main constructor
Reconstruction::DrawOuts::DrawOuts(const std::vector<std::string> &v_inputFiles){

	nscans =				v_tags.size()/v_valint.size();
	nruns =					v_valint.size();
	
	for(int ifile=0; ifile<(int)v_inputFiles.size(); ifile++){
		v_fEvents.				push_back(new RecoEvent());
		v_fFiles.				push_back(TFile::Open(v_inputFiles[ifile].c_str()));
		v_fTrees.				push_back((TTree*)v_fFiles[ifile]->Get("dEdx_tree"));
		v_fTrees[ifile]->		SetBranchAddress("event_branch", &v_fEvents[ifile]);
		v_fnentries.				push_back(v_fTrees[ifile]->GetEntries());
	}

	SetStyle();
}

Reconstruction::DrawOuts::~DrawOuts(){

	for(int i=0;i<(int)v_fEvents.size();i++)	delete v_fEvents[i];
	for(int i=0;i<(int)v_fTrees.size();i++)		delete v_fTrees[i];
	for(int i=0;i<(int)v_fFiles.size();i++)		delete v_fFiles[i];

	delete fpCanvas;

}


void Reconstruction::DrawOuts::SetStyle(){

	fpCanvas = 						new TCanvas("pCanvas", "pCanvas", 1800, 1350);
	TStyle* ptstyle =				SetMyStyle();
	gROOT->							SetStyle(ptstyle->GetName());
	gPad->							UseCurrentStyle();
}

void Reconstruction::DrawOuts::CleanUp(){

	fpCanvas->		Clear();
	delete fpLeg;
	delete fptf1_WF;
	delete fptf1_XP;
	delete fpTGE_mean_WF;
	delete fpTGE_mean_XP;
	delete fpTGE_std_WF;
	delete fpTGE_std_XP;
	delete fpTGE_reso_WF;
	delete fpTGE_reso_XP;

	for(int i=0;i<(int)v_fpTGE_mean_WF.size();i++){
		delete v_fpTGE_mean_WF[i];
		delete v_fpTGE_mean_XP[i];
		delete v_fpTGE_std_WF[i];
		delete v_fpTGE_std_XP[i];
		delete v_fpTGE_reso_WF[i];
		delete v_fpTGE_reso_XP[i];
		delete v_fptf1_WF[i];
		delete v_fptf1_XP[i];
	}
	v_fptf1_WF.			clear();
	v_fptf1_XP.			clear();
	v_fpTGE_mean_WF.	clear();
	v_fpTGE_mean_XP.	clear();
	v_fpTGE_std_WF.		clear();
	v_fpTGE_std_XP.		clear();
	v_fpTGE_reso_WF.	clear();
	v_fpTGE_reso_XP.	clear();

	if(ftype == "multiple") v_valint.clear();
}

void Reconstruction::DrawOuts::ScanFill(){

	int istartscan = 0;
	if(ftype == "single") istartscan = scanindex;
	for(int iscan=0;iscan<nscans;iscan++){

		v_fpTGE_mean_WF.			push_back(new TGraphErrors());
		v_fpTGE_mean_XP.			push_back(new TGraphErrors());
		v_fpTGE_std_WF.				push_back(new TGraphErrors());
		v_fpTGE_std_XP.				push_back(new TGraphErrors());
		v_fpTGE_reso_WF.			push_back(new TGraphErrors());
		v_fpTGE_reso_XP.			push_back(new TGraphErrors());

		if(iscan < istartscan) continue;
		for(int irun=0;irun<nruns;irun++){

			int index;
			if(ftype == "single") index = irun;
			else index = 				iscan*nruns+irun;

			v_fptf1_WF.					push_back((TF1*)v_fFiles[iscan*nruns+irun]->Get<TH1F>("ph1f_WF")->GetFunction("gausn"));
			v_fptf1_XP.					push_back((TF1*)v_fFiles[iscan*nruns+irun]->Get<TH1F>("ph1f_XP")->GetFunction("gausn"));

			float mean_WF =				v_fptf1_WF[index]->GetParameter(1);
			float mean_XP =				v_fptf1_XP[index]->GetParameter(1);
			float dmean_WF =			v_fptf1_WF[index]->GetParError(1);
			float dmean_XP =			v_fptf1_XP[index]->GetParError(1);

			float std_WF =				v_fptf1_WF[index]->GetParameter(2);
			float std_XP =				v_fptf1_XP[index]->GetParameter(2);
			float dstd_WF =				v_fptf1_WF[index]->GetParError(2);
			float dstd_XP =				v_fptf1_XP[index]->GetParError(2);

			float reso_WF =				std_WF/mean_WF*100;
			float reso_XP =				std_XP/mean_XP*100;
			float dreso_WF =			GetResoError(v_fptf1_WF[index]);
			float dreso_XP =			GetResoError(v_fptf1_XP[index]);

			v_fpTGE_mean_WF[iscan]->	SetPoint(irun, v_valint[irun], mean_WF);
			v_fpTGE_mean_XP[iscan]->	SetPoint(irun, v_valint[irun], mean_XP);
			v_fpTGE_mean_WF[iscan]->	SetPointError(irun, 0, dmean_WF);
			v_fpTGE_mean_XP[iscan]->	SetPointError(irun, 0, dmean_XP);

			v_fpTGE_std_WF[iscan]->		SetPoint(irun, v_valint[irun], std_WF);
			v_fpTGE_std_XP[iscan]->		SetPoint(irun, v_valint[irun], std_XP); 
			v_fpTGE_std_WF[iscan]->		SetPointError(irun, 0, dstd_WF);
			v_fpTGE_std_XP[iscan]->		SetPointError(irun, 0, dstd_XP);

			v_fpTGE_reso_WF[iscan]->	SetPoint(irun, v_valint[irun], reso_WF);
			v_fpTGE_reso_XP[iscan]->	SetPoint(irun, v_valint[irun], reso_XP);
			v_fpTGE_reso_WF[iscan]->	SetPointError(irun, 0, dreso_WF);
			v_fpTGE_reso_XP[iscan]->	SetPointError(irun, 0, dreso_XP);
		}
	}
}

void Reconstruction::DrawOuts::ScanDraw(){
	int base = 0;
	int nlegentries = nscans;
	if(ftype == "single"){
		base = scanindex;
		nlegentries = 0;
	}

	if(ftype == "multiple") foutputFile = drawout_metascanpath + testbeam + "_" + metascan + comment + ".pdf";
	else foutputFile = 				drawout_scanpath + scan + comment + ".pdf";
	gPad->							SetRightMargin(0.04);
	gPad->							SetTopMargin(0.04);
	fpCanvas->						Clear();
	fpCanvas->						cd();
	fpLeg =							new TLegend(0.42,0.7-0.03*nlegentries,0.93,0.93);
	fpLeg->							SetTextSize(0.05);
	fpLeg->							SetFillStyle(0);
	fpLeg->							SetTextColor(kBlue-1);
	for(int i=base;i<(int)v_fpTGE_mean_WF.size();i++){
		fpLeg->						AddEntry(v_fpTGE_reso_WF[i], Form("Sum of waveforms %s", v_scanspec[i].c_str()), "ep"); 
		fpLeg->						AddEntry(v_fpTGE_reso_XP[i], Form("Crossed pads %s", v_scanspec[i].c_str()), "ep");	
	}
	
	v_fpTGE_reso_WF[base]->			SetMinimum(YRESOMIN);
	v_fpTGE_reso_WF[base]->			SetMaximum(YRESOMAX);
	v_fpTGE_reso_WF[base]->			SetNameTitle("fpTGE_reso_WF", Form(";%s;resolution (%%)", runvarstr.c_str()));
	if (v_fpTGE_reso_WF[base]->GetXaxis()->GetXmin() == 0)
	{	
		v_fpTGE_reso_WF[base]->		GetXaxis()->SetLimits(-3, v_fpTGE_reso_WF[base]->GetXaxis()->GetXmax());
	}
	for(int i=base;i<(int)v_fpTGE_reso_WF.size();i++){
		Graphic_setup(v_fpTGE_reso_WF[i], 4, markers[2*i], colors[2*i], 1, kBlack);
		Graphic_setup(v_fpTGE_reso_XP[i], 4, markers[2*i+1], colors[2*i+1], 1, kBlack);
		if(i==base) v_fpTGE_reso_WF[i]->	DrawClone("AP");
		else v_fpTGE_reso_WF[i]->	DrawClone("P same");
		v_fpTGE_reso_XP[i]->		DrawClone("P same");
		v_fpTGE_reso_WF[i]->		SetMarkerSize(5);
		v_fpTGE_reso_XP[i]->		SetMarkerSize(5);
	}
	fpLeg->							Draw();
	fpCanvas->						SaveAs((foutputFile + "(").c_str());

	v_fpTGE_mean_WF[base]->			SetMinimum(YMEANMIN);
	v_fpTGE_mean_WF[base]->			SetMaximum(YMEANMAX);
	v_fpTGE_mean_WF[base]->			SetNameTitle("fpTGE_mean_WF", Form(";%s;Mean (ADC count)", runvarstr.c_str()));
	for(int i=base;i<(int)v_fpTGE_mean_WF.size();i++){
		Graphic_setup(v_fpTGE_mean_WF[i], 4, markers[2*i], colors[2*i], 1, kBlack);
		Graphic_setup(v_fpTGE_mean_XP[i], 4, markers[2*i+1], colors[2*i+1], 1, kBlack);
		if(i==base) v_fpTGE_mean_WF[i]->	Draw("AP");
		else v_fpTGE_mean_WF[i]->	Draw("P same");
		v_fpTGE_mean_XP[i]->		Draw("P same");
	}
	fpLeg->							Draw();
	fpCanvas->						SaveAs(foutputFile.c_str());

	v_fpTGE_std_WF[base]->			SetMinimum(YSTDMIN);
	v_fpTGE_std_WF[base]->			SetMaximum(YSTDMAX);
	v_fpTGE_std_WF[base]->			SetNameTitle("fpTGE_std_WF", Form(";%s;std (ADC count)", runvarstr.c_str()));
	for(int i=base;i<(int)v_fpTGE_std_WF.size();i++){
		Graphic_setup(v_fpTGE_std_WF[i], 4, markers[2*i], colors[2*i], 1, kBlack);
		Graphic_setup(v_fpTGE_std_XP[i], 4, markers[2*i+1], colors[2*i+1], 1, kBlack);
		if(i==base) v_fpTGE_std_WF[i]->	Draw("AP");
		else v_fpTGE_std_WF[i]->	Draw("P same");
		v_fpTGE_std_XP[i]->			Draw("P same");
	}
	fpLeg->							Draw();
	fpCanvas->						SaveAs((foutputFile + ")").c_str());

}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reconstruction::DrawOuts::Control(){

	// Prepare histograms
	TH2I *ph2i_heatmap =		new TH2I("ph2i_heatmap", "Events heatmap;iX;iY;", 144, 0, 143, 64, 0, 63);
	TH1I *ph1i_TLead = 			new TH1I("ph1i_TLead", "T_{max} of leading pad;T_{max};Counts", 511, 0, 510);
	TH1I *ph1i_TLeadSel = 		new TH1I("ph1i_TLeadSel", "T_{max} of leading pad (after cut);T_{max};Counts", 511, 0, 510);
	TH1I *ph1i_PadMult = 		new TH1I("ph1i_PadMult", "Pad multiplicity;N_{pads};Counts", 11, 0, 10);
	TH1I *ph1i_PadMultSel = 	new TH1I("ph1i_PadMultSel", "Pad multiplicity (after cut);N_{pads};Counts", 11, 0, 10);
	TH1F *ph1f_AvgPadMult = 	new TH1F("ph1f_AvgPadMult", "Average pad multiplicity;N_{pads};Counts", 100, 0, 10);
	TH1F *ph1f_AvgPadMultSel = 	new TH1F("ph1f_AvgPadMultSel", "Average pad multiplicity (after cut);N_{pads};Counts", 100, 0, 10);
	TH1F *ph1f_ADCmax = 		new TH1F("ph1f_ADCmax", "ADC max;ADC counts;Counts", 400, 0, 4000);
	TH1F *ph1f_ADCmaxSel = 		new TH1F("ph1f_ADCmaxSel", "ADC max (after cut);ADC counts;Counts", 400, 0, 4000);
	TH1F *ph1f_ALead = 			new TH1F("ph1f_ALead", "Leading pad amplitude;ADC counts;Counts", 400, 0, 4000);
	TH1F *ph1f_ALeadSel = 		new TH1F("ph1f_ALeadSel", "Leading pad amplitude (after cut);ADC counts;Counts", 400, 0, 4000);
	TH1F *ph1f_ANeighbour = 	new TH1F("ph1f_ANeighbour", "Neighbouring pad amplitude;ADC counts;Counts", 400, 0, 4000);
	TH1F *ph1f_ANeighbourSel = 	new TH1F("ph1f_ANeighbourSel", "Neighbouring pad amplitude (after cut);ADC counts;Counts", 400, 0, 4000);
	std::vector<TH1F*> 			v_AvgPadMult;
	std::vector<TH1F*> 			v_AvgPadMultSel;
	std::vector<TH1I*> 			v_PadMult;
	std::vector<TH1I*> 			v_PadMultSel;
	std::vector<TH1I*> 			v_TLead;
	std::vector<TH1I*> 			v_TLeadSel;
	std::vector<TH1F*> 			v_ADCmax;
	std::vector<TH1F*> 			v_ADCmaxSel;
	std::vector<TH1F*> 			v_ALead;
	std::vector<TH1F*> 			v_ALeadSel;
	std::vector<TH1F*> 			v_ANeighbour;
	std::vector<TH1F*> 			v_ANeighbourSel;

	for(int i=0;i<8;i++){
		v_TLead.push_back					(new TH1I(Form("ph1i_TLead_%i", i), Form("T_{max} of leading pad (module %i);T_{max};Counts", i), 511, 0, 510));
		v_TLeadSel.push_back				(new TH1I(Form("ph1i_TLeadSel_%i", i), Form("T_{max} of leading pad (module %i) (after cut);T_{max};Counts", i), 511, 0, 510));
		v_PadMult.push_back					(new TH1I(Form("ph1i_PadMult_%i", i), Form("Pad multiplicity (module %i);N_{pads};Counts", i), 100, 0, 100));
		v_PadMultSel.push_back				(new TH1I(Form("ph1i_PadMultSel_%i", i), Form("Pad multiplicity (module %i) (after cut);N_{pads};Counts", i), 100, 0, 100));
		v_AvgPadMult.push_back				(new TH1F(Form("ph1f_AvgPadMult_%i", i), Form("Average pad multiplicity (module %i);N_{pads};Counts", i), 100, 0, 100));
		v_AvgPadMultSel.push_back			(new TH1F(Form("ph1f_AvgPadMultSel_%i", i), Form("Average pad multiplicity (module %i) (after cut);N_{pads};Counts", i), 100, 0, 100));
		v_ADCmax.push_back					(new TH1F(Form("ph1f_ADCmax_%i", i), Form("ADC max (module %i);ADC counts;Counts", i), 400, 0, 4000));
		v_ADCmaxSel.push_back				(new TH1F(Form("ph1f_ADCmaxSel_%i", i), Form("ADC max (module %i) (after cut);ADC counts;Counts", i), 400, 0, 4000));
		v_ALead.push_back					(new TH1F(Form("ph1f_ALead_%i", i), Form("Leading pad amplitude (module %i);ADC counts;Counts", i), 400, 0, 4000));
		v_ALeadSel.push_back				(new TH1F(Form("ph1f_ALeadSel_%i", i), Form("Leading pad amplitude (module %i) (after cut);ADC counts;Counts", i), 400, 0, 4000));
		v_ANeighbour.push_back				(new TH1F(Form("ph1f_ANeighbour_%i", i), Form("Neighbouring pad amplitude (module %i);ADC counts;Counts", i), 400, 0, 4000));
		v_ANeighbourSel.push_back			(new TH1F(Form("ph1f_ANeighbourSel_%i", i), Form("Neighbouring pad amplitude (module %i) (after cut);ADC counts;Counts", i), 400, 0, 4000));
	}

	// Get entries and fill histograms
	for(int i=0;i<v_fnentries.back();i++){
		v_fTrees.back()->				GetEntry(i);
		p_recoevent =					v_fEvents.back();
		NMod =							p_recoevent->v_modules.size();

		for(int iMod=0;iMod<NMod;iMod++){
			p_recomodule =				p_recoevent->v_modules[iMod];
			position =					p_recomodule->position;
			NClusters =					p_recomodule->NClusters;


			for(int iC=0;iC<NClusters;iC++){
				p_recocluster =			p_recomodule->v_clusters[iC];
				NPads =					p_recocluster->NPads;


				for(int iP=0;iP<NPads;iP++){
					// Heatmap
					ix =				p_recocluster->v_pads[iP]->ix;
					iy =				p_recocluster->v_pads[iP]->iy;
					if(position<4) iy+=32;
					ix += 36*(position%4);
					ph2i_heatmap->		Fill(ix, iy);
					
					p_recopad =			p_recocluster->v_pads[iP];
					ph1f_ADCmax->		Fill(p_recopad->ADCmax);
					v_ADCmax[position]->Fill(p_recopad->ADCmax);
					if(!p_recopad->leading){
						ph1f_ANeighbour->Fill(p_recopad->ADCmax);
						v_ANeighbour[position]->Fill(p_recopad->ADCmax);
					}

					if(!p_recomodule->selected) continue;
					ph1f_ADCmaxSel->	Fill(p_recopad->ADCmax);
					v_ADCmaxSel[position]->Fill(p_recopad->ADCmax);
					if(!p_recopad->leading){
						ph1f_ANeighbourSel->Fill(p_recopad->ADCmax);
						v_ANeighbourSel[position]->Fill(p_recopad->ADCmax);
					}
				} // Pads

				ph1i_TLead->			Fill(p_recocluster->TLead);
				ph1i_PadMult->			Fill(NPads);
				ph1f_ALead->			Fill(p_recocluster->ALead_base);
				v_TLead[position]->		Fill(p_recocluster->TLead);
				v_PadMult[position]->	Fill(NPads);
				v_ALead[position]->		Fill(p_recocluster->ALead_base);

				if(!p_recomodule->selected) continue;
				ph1i_TLeadSel->			Fill(p_recocluster->TLead);
				ph1i_PadMultSel->		Fill(NPads);
				ph1f_ALeadSel->			Fill(p_recocluster->ALead_base);
				v_TLeadSel[position]->	Fill(p_recocluster->TLead);
				v_PadMultSel[position]->Fill(NPads);
				v_ALeadSel[position]->	Fill(p_recocluster->ALead_base);
			} // Clusters

			ph1f_AvgPadMult->			Fill(p_recomodule->avg_pad_mult);
			v_AvgPadMult[position]->	Fill(p_recomodule->avg_pad_mult);

			if(!p_recomodule->selected) continue;
			ph1f_AvgPadMultSel->Fill(p_recomodule->avg_pad_mult);
			v_AvgPadMultSel[position]->Fill(p_recomodule->avg_pad_mult);
		} // Modules
		delete p_recoevent;
	}

	// Draw
	gStyle->						SetOptStat(0);
	gStyle->						SetOptFit(0);

	// Get maximum of module plots


	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();
	ph1i_TLead->					Draw("HIST");
	ph1i_TLeadSel->					SetLineColor(kRed);
	ph1i_TLeadSel->					Draw("HIST sames");
	fpCanvas->						SaveAs((drawout_file + "(").c_str());

	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();
	fpCanvas->						Divide(4, 2);
	for(int i=0;i<8;i++){
		fpCanvas->					cd(i+1);
		v_TLeadSel[i]->				SetLineColor(kRed);
		v_TLead[i]->				Draw("HIST");
		v_TLeadSel[i]->				Draw("HIST sames");
	}
	fpCanvas->						SaveAs(drawout_file.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						Clear();
	gPad->							SetRightMargin(0.12);
	gPad->							SetLeftMargin(0.12);
	ph2i_heatmap->					GetYaxis()->SetTitleOffset(1.1);
	gStyle->						SetTitleX((1.-gPad->GetRightMargin()+gPad->GetLeftMargin())/2);
	ph2i_heatmap->					Draw("COLZ");
	fpCanvas->						SaveAs((drawout_file + ")").c_str());


	fpCanvas->						Clear();
}

void Reconstruction::DrawOuts::EnergyLoss(){

	int xmax =									1300;
	if(tag.find("Theta") != std::string::npos) xmax = 2000;
	// Prepare histograms
	TH1F *ph1f_XP =				new TH1F("ph1f_XP", "Energy loss;dE/dx (ADC counts);Counts", 100, 0, xmax);
	TH1F *ph1f_WF =				new TH1F("ph1f_WF", "Energy loss;dE/dx (ADC counts);Counts", 100, 0, xmax);
	TH1F *ph1f_XPnoTrunc =		new TH1F("ph1f_XPnoTrunc", "Energy loss (no truncation);dE/dx (ADC counts);Counts", 100, 0, xmax);
	TH1F *ph1f_WFnoTrunc =		new TH1F("ph1f_WFnoTrunc", "Energy loss (no truncation);dE/dx (ADC counts);Counts", 100, 0, xmax);
	TH2F *ph2f_XPWF =			new TH2F("ph2f_XPWF", "Energy loss with XP vs with WF;dE/dx (WF);dE/dx (XP)", 100, 0, xmax, 100, 0, xmax);
	std::vector<TH1F*> 			v_h1f_XP_mod;
	std::vector<TH1F*> 			v_h1f_WF_mod;
	std::vector<TH1F*> 			v_h1f_XP_modnoTrunc;
	std::vector<TH1F*> 			v_h1f_WF_modnoTrunc;
	
	for(int i=0;i<8;i++){
		v_h1f_XP_mod.push_back(			new TH1F(Form("ph1f_XP_%i", i), Form("Energy loss (module %i);dE/dx (ADC counts);Counts", i), 100, 0, xmax));
		v_h1f_WF_mod.push_back(			new TH1F(Form("ph1f_WF_%i", i), Form("Energy loss (module %i);dE/dx (ADC counts);Counts", i), 100, 0, xmax));
		v_h1f_XP_modnoTrunc.push_back(	new TH1F(Form("ph1f_XPnoTrunc_%i", i), Form("Energy loss (module %i) (no truncation);dE/dx (ADC counts);Counts", i), 100, 0, xmax));
		v_h1f_WF_modnoTrunc.push_back(	new TH1F(Form("ph1f_WFnoTrunc_%i", i), Form("Energy loss (module %i) (no truncation);dE/dx (ADC counts);Counts", i), 100, 0, xmax));
	}

	// Get entries and fill histograms
	for(int i=0;i<v_fnentries.back();i++){
		v_fTrees.back()->					GetEntry(i);
		p_recoevent =						v_fEvents.back();
		NMod =								p_recoevent->v_modules.size();
		if(!p_recoevent->selected) 			continue;
		for(int iMod=0;iMod<NMod;iMod++){
			p_recomodule = 					p_recoevent->v_modules[iMod];
			if(!p_recomodule->selected) 	continue;
			position =						p_recomodule->position;
			v_h1f_XP_mod[position]->		Fill(p_recomodule->dEdxXP);
			v_h1f_WF_mod[position]->		Fill(p_recomodule->dEdxWF);
			v_h1f_XP_modnoTrunc[position]->	Fill(p_recomodule->dEdxXPnoTrunc);
			v_h1f_WF_modnoTrunc[position]->	Fill(p_recomodule->dEdxWFnoTrunc);
		}
		ph1f_WF->							Fill(p_recoevent->dEdxWF);
		ph1f_XP->							Fill(p_recoevent->dEdxXP);
		ph1f_WFnoTrunc->					Fill(p_recoevent->dEdxWFnoTrunc);
		ph1f_XPnoTrunc->					Fill(p_recoevent->dEdxXPnoTrunc);
		ph2f_XPWF->							Fill(p_recoevent->dEdxWF, p_recoevent->dEdxXP);
	}

	// Fitting
	Fit1Gauss(ph1f_WF, 2);
	Fit1Gauss(ph1f_XP, 2);
	Fit1Gauss(ph1f_WFnoTrunc, 2);
	Fit1Gauss(ph1f_XPnoTrunc, 2);

	// Display settings
	Graphic_setup(ph1f_WF, 0.5, 1, kCyan+1, 2, kCyan-2, kCyan, 0.2);
	Graphic_setup(ph1f_XP, 0.5, 1, kMagenta+2, 2, kMagenta-2, kMagenta, 0.2);
	Graphic_setup(ph1f_WFnoTrunc, 0.5, 1, kCyan+1, 2, kCyan-2, kCyan, 0.2);
	Graphic_setup(ph1f_XPnoTrunc, 0.5, 1, kMagenta+2, 2, kMagenta-2, kMagenta, 0.2);

	for(int i=0;i<8;i++){
		Graphic_setup(v_h1f_WF_mod[i], 0.5, 1, kCyan+1, 1, kCyan-2, kCyan, 0.2);
		Graphic_setup(v_h1f_XP_mod[i], 0.5, 1, kMagenta+2, 1, kMagenta-2, kMagenta, 0.2);
		Graphic_setup(v_h1f_WF_modnoTrunc[i], 0.5, 1, kCyan+1, 1, kCyan-2, kCyan, 0.2);
		Graphic_setup(v_h1f_XP_modnoTrunc[i], 0.5, 1, kMagenta+2, 1, kMagenta-2, kMagenta, 0.2);
		if(v_h1f_WF_mod[i]->GetEntries() < 100) continue;
		Fit1Gauss(v_h1f_WF_mod[i], 2);
		Fit1Gauss(v_h1f_XP_mod[i], 2);
		Fit1Gauss(v_h1f_WF_modnoTrunc[i], 2);
		Fit1Gauss(v_h1f_XP_modnoTrunc[i], 2);	
	}


	// Draw
	bool disp_trunc = true;
	gStyle->						SetOptStat(0);
	gStyle->						SetOptFit(0);

	// Get maximum of event plots
	int WFMax =						ph1f_WF->GetMaximum();
	int XPMax =						ph1f_XP->GetMaximum();

	// Get maximum of module plots
	int overallMax = 0, overallMaxMod = 0, maxVal_WF = 0, maxVal_XP = 0;
	overallMax =					std::max({WFMax, XPMax});
	for(int i=0;i<8;i++){
		maxVal_WF =					v_h1f_WF_mod[i]->GetMaximum();
		maxVal_XP =					v_h1f_XP_mod[i]->GetMaximum();
		overallMaxMod =				std::max({overallMaxMod, maxVal_WF, maxVal_XP});
	}
	float invX, invY;
	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();

	ph1f_WF->						SetAxisRange(0, 1.1 * overallMax,	"Y");
	invX = 0, invY = 0;
	if(ph1f_WF->GetMean() > 650) invX = 0.5;
	ph1f_WF->						Draw("HIST");
	ph1f_XP->						Draw("HIST sames");

	PrintResolution(ph1f_XP, fpCanvas, 0.65-invX, 0.58, kMagenta+2, "XP");
	PrintResolution(ph1f_WF, fpCanvas, 0.65-invX, 0.25, kCyan+2, "WF");
	fpCanvas->						SaveAs((drawout_file + "(").c_str());

		//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();
	fpCanvas->						Divide(4, 2);

	for(int i=0;i<8;i++){
		v_h1f_WF_mod[i]->				SetAxisRange(0, 1.1 * overallMaxMod, "Y");
		fpCanvas->					cd(i+1);
		invX=0;
		if(v_h1f_WF_mod[i]->GetMean() > 650) invX = 0.5;
		gPad->						SetRightMargin(0);
		v_h1f_WF_mod[i]->				Draw("HIST");
		v_h1f_XP_mod[i]->				Draw("HIST sames");
		if(v_h1f_WF_mod[i]->GetEntries() < 100) continue;
		PrintResolution(v_h1f_XP_mod[i], fpCanvas, 0.65-invX, 0.58, kMagenta+2, "XP");
		PrintResolution(v_h1f_WF_mod[i], fpCanvas, 0.65-invX, 0.25, kCyan+2, "WF");
	}
	fpCanvas->						SaveAs(drawout_file.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();

	ph1f_WFnoTrunc->				SetAxisRange(0, 1.1 * overallMax, "Y");
	invX = 0, invY = 0;
	if(ph1f_WFnoTrunc->GetMean(1) > 650) invX = 0.5;
	ph1f_WFnoTrunc->				Draw("HIST");
	ph1f_XPnoTrunc->				Draw("HIST sames");

	PrintResolution(ph1f_XPnoTrunc, fpCanvas, 0.65-invX, 0.58, kMagenta+2, "XP");
	PrintResolution(ph1f_WFnoTrunc, fpCanvas, 0.65-invX, 0.25, kCyan+2, "WF");
	fpCanvas->						SaveAs(drawout_file.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();
	fpCanvas->						Divide(4, 2);

	for(int i=0;i<8;i++){
		v_h1f_WF_modnoTrunc[i]->		SetAxisRange(0, 1.1 * overallMaxMod, "Y");
		fpCanvas->					cd(i+1);
		invX = 0;
		if(v_h1f_WF_modnoTrunc[i]->GetMean() > 650) invX = 0.5;
		gPad->						SetRightMargin(0);
		v_h1f_WF_modnoTrunc[i]->		Draw("HIST");
		v_h1f_XP_modnoTrunc[i]->		Draw("HIST sames");
		if(v_h1f_WF_mod[i]->GetEntries() < 100) continue;
		PrintResolution(v_h1f_XP_modnoTrunc[i], fpCanvas, 0.65-invX, 0.58, kMagenta+2, "XP");
		PrintResolution(v_h1f_WF_modnoTrunc[i], fpCanvas, 0.65-invX, 0.25, kCyan+2, "WF");
	}
	fpCanvas->						SaveAs(drawout_file.c_str());


		//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd(); 
	fpCanvas->						Clear();
	invX, invY = 0;
	if(ph2f_XPWF->GetMean(1) > 650) invX = 0.5;
	if(ph2f_XPWF->GetMean(2) > 650) invY = 0.5;
	gStyle->						SetOptStat("merou");
	gStyle->						SetStatX(0.36);
	gStyle->						SetStatY(0.88);
	gPad->							SetRightMargin(0.12);
	gPad->							SetLeftMargin(0.12);
	ph2f_XPWF->						GetYaxis()->SetTitleOffset(1.1);
	gStyle->						SetTitleX((1.-gPad->GetRightMargin()+gPad->GetLeftMargin())/2);
	ph2f_XPWF->						Draw("colz");
	fpCanvas->						SaveAs((drawout_file + ")").c_str());

	fpCanvas->						Clear();
	// Delete
	delete ph2f_XPWF;
}

void Reconstruction::DrawOuts::FileComparison(){
	// Set output
	std::string comment_list;
	for(int i=0;i<(int)v_comments.size();i++){
		if(v_comments[i] != "") comment_list += v_comments[i] + "_";
		else						comment_list += "Default_";
	}
	drawout_file =							foutputDir + "Comparison_" + comment_list + ".pdf";

	// Prepare histograms
	std::vector<TH1F*> v_h1f_XP;
	std::vector<TH1F*> v_h1f_WF;
	std::vector<std::vector<TH1F*>> v_h1f_XP_mod;
	std::vector<std::vector<TH1F*>> v_h1f_WF_mod;
	for(int i=0;i<(int)v_comments.size();i++){
		v_h1f_XP.							push_back(new TH1F(Form("ph1f_XP_%i", i), Form("Energy loss (%s);dE/dx (ADC counts);Counts", v_comments[i].c_str()), 100, 0, 1300));
		v_h1f_WF.							push_back(new TH1F(Form("ph1f_WF_%i", i), Form("Energy loss (%s);dE/dx (ADC counts);Counts", v_comments[i].c_str()), 100, 0, 1300));
		v_h1f_XP_mod.						push_back(std::vector<TH1F*>());
		v_h1f_WF_mod. 						push_back(std::vector<TH1F*>());
		for(int j=0;j<8;j++){
			v_h1f_XP_mod[i].				push_back(new TH1F(Form("ph1f_XP_%i_%i", i, j), Form("Energy loss (%s, module %i);dE/dx (ADC counts);Counts", v_comments[i].c_str(), j), 100, 0, 1300));
			v_h1f_WF_mod[i].				push_back(new TH1F(Form("ph1f_WF_%i_%i", i, j), Form("Energy loss (%s, module %i);dE/dx (ADC counts);Counts", v_comments[i].c_str(), j), 100, 0, 1300));
		}
	}

	int NMod = 0,	position = 0;
	// Get entries and fill histograms
	for(int ifile=0;ifile<(int)v_comments.size();ifile++){
		for(int i=0;i<v_fnentries[ifile];i++){
			v_fTrees[ifile]->			GetEntry(i);
			NMod =							v_fEvents[ifile]->v_modules.size();

			v_h1f_WF[i]->					Fill(v_fEvents[ifile]->dEdxWF);
			v_h1f_XP[i]->					Fill(v_fEvents[ifile]->dEdxXP);
			for(int iMod=0;iMod<NMod;iMod++){
				position =					v_fEvents[ifile]->v_modules[iMod]->position;
				v_h1f_WF_mod[i][position]->	Fill(v_fEvents[ifile]->v_modules[iMod]->dEdxWF);
				v_h1f_XP_mod[i][position]->	Fill(v_fEvents[ifile]->v_modules[iMod]->dEdxXP);
			}
		}
	}

	for(int i=0;i<(int)v_comments.size();i++){
		Fit1Gauss(v_h1f_WF[i], 2);
		Fit1Gauss(v_h1f_XP[i], 2);
		Graphic_setup(v_h1f_WF[i], 0.5, 1, 2+i, 2, 2+i, 2+i, 0.2);
		Graphic_setup(v_h1f_XP[i], 0.5, 1, 2+i, 2, 2+i, 2+i, 0.2);
		for(int j=0;j<8;j++){
			Fit1Gauss(v_h1f_WF_mod[i][j], 2);
			Fit1Gauss(v_h1f_XP_mod[i][j], 2);
			Graphic_setup(v_h1f_WF_mod[i][j], 0.5, 1, 2+i, 1, 2+i, 2+i, 0.2);
			Graphic_setup(v_h1f_XP_mod[i][j], 0.5, 1, 2+i, 1, 2+i, 2+i, 0.2);
		}
	}


	// Draw
	gStyle->						SetOptStat(0);
	gStyle->						SetOptFit(0);

	// Get maximum of event plots
	int overallMax = 0, overallMaxMod = 0, maxVal_WF = 0, maxVal_XP = 0;
	for(int i=0;i<(int)v_comments.size();i++){
		maxVal_WF =					v_h1f_WF[i]->GetMaximum();
		maxVal_XP =					v_h1f_XP[i]->GetMaximum();
		overallMax =				std::max({overallMax, maxVal_WF, maxVal_XP});
		for(int j=0;j<8;j++){
			maxVal_WF =				v_h1f_WF_mod[i][j]->GetMaximum();
			maxVal_XP =				v_h1f_XP_mod[i][j]->GetMaximum();
			overallMaxMod =			std::max({overallMaxMod, maxVal_WF, maxVal_XP});
		}
	}

	float invX;
	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();
	fpCanvas->						Clear();

	v_h1f_WF[0]->					SetAxisRange(0, 1.1 * overallMax,	"Y");
	invX = 0;
	if(v_h1f_WF[0]->GetMean() > 650) invX = 0.5;
	v_h1f_WF[0]->					Draw("HIST");
	for(int i=0;i<(int)v_comments.size();i++){
		if(i!=0) v_h1f_WF[i]->		Draw("HIST sames");
		PrintResolution(v_h1f_WF[i], fpCanvas, 0.65-invX, 0.58-0.33, 2+i, v_comments[i].c_str());
	}
	fpCanvas->						SaveAs((drawout_file + "(").c_str());

	//////////////////////////////////////////////////////////////////////////////////////////
	fpCanvas->						cd();	
	fpCanvas->						Clear();

	v_h1f_XP[0]->					SetAxisRange(0, 1.1 * overallMax,	"Y");
	invX = 0;
	if(v_h1f_XP[0]->GetMean() > 650) invX = 0.5;
	v_h1f_XP[0]->					Draw("HIST");
	for(int i=0;i<(int)v_comments.size();i++){
		if(i!=0) v_h1f_XP[i]->		Draw("HIST sames");
		PrintResolution(v_h1f_XP[i], fpCanvas, 0.65-invX, 0.58-0.33, 2+i, v_comments[i].c_str());
	}	
	fpCanvas->						SaveAs((drawout_file + ")").c_str());

	// Delete
	for(int i=0;i<(int)v_comments.size();i++){
		delete v_h1f_XP[i];
		delete v_h1f_WF[i];
		for(int j=0;j<8;j++){
			delete v_h1f_XP_mod[i][j];
			delete v_h1f_WF_mod[i][j];
		}
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reconstruction::DrawOuts::DESY21SingleScan(){
	
	ScanFill();
	// SingleScanDraw();
	// ScanFill();
	ScanDraw();
	CleanUp();
}

void Reconstruction::DrawOuts::DESY21Scan(const std::string &drawtype){

	ftype = drawtype;
	ScanFill();
	ScanDraw();
	// CleanUp();
}