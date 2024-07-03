#include "DrawOuts.h"
#include "ReconTools.h"
#include "CombinedFit.h"

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

Reconstruction::DrawOuts::DrawOuts(const std::string &inputFile){
	// Get Tree
	p_recoevent =						new RecoEvent();
	fpFile =						TFile::Open(inputFile.c_str());
	fpTree =						(TTree*)fpFile->Get("dEdx_tree");
	fpTree->						SetBranchAddress("event_branch", &p_recoevent);
	fnentries =						fpTree->GetEntries();

	SetStyle();
}

Reconstruction::DrawOuts::DrawOuts(const std::vector<std::string> &v_inputFiles){
	// Get Tree
	for(int ifile=0; ifile<(int)v_inputFiles.size(); ifile++){
		finputEvents.				push_back(new RecoEvent());
		finputFiles.				push_back(TFile::Open(v_inputFiles[ifile].c_str()));
		finputTrees.				push_back((TTree*)finputFiles[ifile]->Get("dEdx_tree"));
		finputTrees[ifile]->		SetBranchAddress("event_branch", &finputEvents[ifile]);
		finputnEntries.				push_back(finputTrees[ifile]->GetEntries());
	}

	SetStyle();
}


void Reconstruction::DrawOuts::SetStyle(){
	// Set Style
	fpCanvas = 						new TCanvas("pCanvas", "pCanvas", 1800, 1350);
	TStyle* ptstyle =				SetMyStyle();
	gROOT->							SetStyle(ptstyle->GetName());
	gPad->							UseCurrentStyle();
}



Reconstruction::DrawOuts::~DrawOuts(){
	delete p_recoevent;
	for(int i=0;i<(int)finputEvents.size();i++) 	delete finputEvents[i];
	delete fpTree;
	for(int i=0;i<(int)finputTrees.size();i++) 		delete finputTrees[i];
	delete fpFile;
	for(int i=0;i<(int)finputFiles.size();i++) 		delete finputFiles[i];

	delete fpCanvas;
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
	for(int i=0;i<fnentries;i++){
		fpTree->						GetEntry(i);
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

				if(p_recocluster->TLead !=0) std::cout << "TLead: " << p_recocluster->TLead << std::endl;
				if(p_recocluster->TLead !=0) ph1i_TLead->			Fill(p_recocluster->TLead);
				ph1i_PadMult->			Fill(NPads);
				ph1f_ALead->			Fill(p_recocluster->ALead_base);
				v_TLead[position]->		Fill(p_recocluster->TLead);
				v_PadMult[position]->	Fill(NPads);
				v_ALead[position]->		Fill(p_recocluster->ALead_base);

				if(!p_recomodule->selected) continue;
				if(p_recocluster->TLead !=0) ph1i_TLeadSel->			Fill(p_recocluster->TLead);
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
	fpCanvas->						SaveAs((drawout_file + ")").c_str());


	fpCanvas->						Clear();
}









////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reconstruction::DrawOuts::EnergyLoss(){

	// Prepare histograms
	TH1F *ph1f_XP =				new TH1F("ph1f_XP", "Energy loss;dE/dx (ADC counts);Counts", 100, 0, 1300);
	TH1F *ph1f_WF =				new TH1F("ph1f_WF", "Energy loss;dE/dx (ADC counts);Counts", 100, 0, 1300);
	TH1F *ph1f_XPnoTrunc =		new TH1F("ph1f_XPnoTrunc", "Energy loss (no truncation);dE/dx (ADC counts);Counts", 100, 0, 1300);
	TH1F *ph1f_WFnoTrunc =		new TH1F("ph1f_WFnoTrunc", "Energy loss (no truncation);dE/dx (ADC counts);Counts", 100, 0, 1300);
	TH2F *ph2f_XPWF =			new TH2F("ph2f_XPWF", "Energy loss with XP vs with WF;dE/dx (WF);dE/dx (XP)", 100, 0, 1300, 100, 0, 1300);
	std::vector<TH1F*> 			v_h1f_XP_mod;
	std::vector<TH1F*> 			v_h1f_WF_mod;
	std::vector<TH1F*> 			v_h1f_XP_modnoTrunc;
	std::vector<TH1F*> 			v_h1f_WF_modnoTrunc;
	
	for(int i=0;i<8;i++){
		v_h1f_XP_mod.push_back(			new TH1F(Form("ph1f_XP_%i", i), Form("Energy loss (module %i);dE/dx (ADC counts);Counts", i), 100, 0, 1300));
		v_h1f_WF_mod.push_back(			new TH1F(Form("ph1f_WF_%i", i), Form("Energy loss (module %i);dE/dx (ADC counts);Counts", i), 100, 0, 1300));
		v_h1f_XP_modnoTrunc.push_back(	new TH1F(Form("ph1f_XPnoTrunc_%i", i), Form("Energy loss (module %i) (no truncation);dE/dx (ADC counts);Counts", i), 100, 0, 1300));
		v_h1f_WF_modnoTrunc.push_back(	new TH1F(Form("ph1f_WFnoTrunc_%i", i), Form("Energy loss (module %i) (no truncation);dE/dx (ADC counts);Counts", i), 100, 0, 1300));
	}

	// Get entries and fill histograms
	for(int i=0;i<fnentries;i++){
		fpTree->						GetEntry(i);
		NMod =							p_recoevent->v_modules.size();

		for(int iMod=0;iMod<NMod;iMod++){
			position =					p_recoevent->v_modules[iMod]->position;
			// Histograms
			v_h1f_XP_mod[position]->		Fill(p_recoevent->v_modules[iMod]->dEdxXP);
			v_h1f_WF_mod[position]->		Fill(p_recoevent->v_modules[iMod]->dEdxWF);
			v_h1f_XP_modnoTrunc[position]->	Fill(p_recoevent->v_modules[iMod]->dEdxXPnoTrunc);
			v_h1f_WF_modnoTrunc[position]->	Fill(p_recoevent->v_modules[iMod]->dEdxWFnoTrunc);
		}
		if(NMod != 4) continue;
		ph1f_WF->						Fill(p_recoevent->dEdxWF);
		ph1f_XP->						Fill(p_recoevent->dEdxXP);
		ph1f_WFnoTrunc->				Fill(p_recoevent->dEdxWFnoTrunc);
		ph1f_XPnoTrunc->				Fill(p_recoevent->dEdxXPnoTrunc);
		ph2f_XPWF->						Fill(p_recoevent->dEdxWF, p_recoevent->dEdxXP);
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
		for(int i=0;i<finputnEntries[ifile];i++){
			std::cout << "number of entries" << finputTrees[ifile]->GetEntries() << std::endl;
			finputTrees[ifile]->			GetEntry(i);
			NMod =							finputEvents[ifile]->v_modules.size();

			v_h1f_WF[i]->					Fill(finputEvents[ifile]->dEdxWF);
			v_h1f_XP[i]->					Fill(finputEvents[ifile]->dEdxXP);
			for(int iMod=0;iMod<NMod;iMod++){
				position =					finputEvents[ifile]->v_modules[iMod]->position;
				v_h1f_WF_mod[i][position]->	Fill(finputEvents[ifile]->v_modules[iMod]->dEdxWF);
				v_h1f_XP_mod[i][position]->	Fill(finputEvents[ifile]->v_modules[iMod]->dEdxXP);
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











































void DrawOut_Control(const std::string& inputDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod)
{
	std::string inputDirName	= inputDir + Tag + "/";

	// Get input file
	TFile* pTFile_Control	= TFile::Open(TString(inputDir + "/" + Tag + "/" + "1_" + Tag + "_Control" + Comment + ".root"));

	// Get histograms & functions
	std::vector<TH1F*>			v_h1f_TL_Raw;
	std::vector<TH1F*>			v_h1f_TL_Sel;
	std::vector<TH1F*>			v_h1f_PM_Raw;
	std::vector<TH1F*>			v_h1f_PM_Sel;
	std::vector<TH1F*>			v_h1f_APM_Raw;
	std::vector<TH1F*>			v_h1f_APM_Sel;
	std::vector<TH1F*>			v_h1f_API_Raw;
	std::vector<TH1F*>			v_h1f_API_Sel;
	std::vector<TH1F*>			v_h1f_Qpad_Raw;
	std::vector<TH1F*>			v_h1f_Qpad_Sel;
	std::vector<TH1F*>			v_h1f_Qlead_Raw;
	std::vector<TH1F*>			v_h1f_Qlead_Sel;
	std::vector<TH1F*>			v_h1f_Qneigh_Raw;
	std::vector<TH1F*>			v_h1f_Qneigh_Sel;
	std::vector<TH1F*>			v_h1f_CM_Raw;
	std::vector<TH1F*>			v_h1f_CM_Sel;
	std::vector<TH1F*>			v_h1f_Eff_Raw;
	std::vector<TH1F*>			v_h1f_Eff_Sel;
	std::vector<TH2F*>			v_h2f_LeadPos_Raw;
	std::vector<TH2F*>			v_h2f_LeadPos_Sel;
	std::vector<TH2F*>			v_h2f_Theta_Raw;
	std::vector<TH2F*>			v_h2f_Theta_Sel;
	std::vector<TH2F*>			v_h2f_rho_DT_Raw;
	std::vector<TH2F*>			v_h2f_rho_DT_Sel;

	for (int iMod = 0; iMod < nMod; iMod++){
	v_h1f_TL_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_TL_Raw_%i", iMod)));
	v_h1f_TL_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_TL_Sel_%i", iMod)));
	v_h1f_PM_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_PM_Raw_%i", iMod)));
	v_h1f_PM_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_PM_Sel_%i", iMod)));
	v_h1f_APM_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_APM_Raw_%i", iMod)));
	v_h1f_APM_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_APM_Sel_%i", iMod)));
	v_h1f_API_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_API_Raw_%i", iMod)));
	v_h1f_API_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_API_Sel_%i", iMod)));
	v_h1f_Qpad_Raw.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qpad_Raw_%i", iMod)));
	v_h1f_Qpad_Sel.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qpad_Sel_%i", iMod)));
	v_h1f_Qlead_Raw.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qlead_Raw_%i", iMod)));
	v_h1f_Qlead_Sel.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qlead_Sel_%i", iMod)));
	v_h1f_Qneigh_Raw.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qneigh_Raw_%i", iMod)));
	v_h1f_Qneigh_Sel.		push_back(pTFile_Control->Get<TH1F>(Form("h1f_Qneigh_Sel_%i", iMod)));
	v_h1f_CM_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_CM_Raw_%i", iMod)));
	v_h1f_CM_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_CM_Sel_%i", iMod)));
	v_h1f_Eff_Raw.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_Eff_Raw_%i", iMod)));
	v_h1f_Eff_Sel.			push_back(pTFile_Control->Get<TH1F>(Form("h1f_Eff_Sel_%i", iMod)));
	v_h2f_LeadPos_Raw.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_LeadPos_Raw_%i", iMod)));
	v_h2f_LeadPos_Sel.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_LeadPos_Sel_%i", iMod)));
	v_h2f_Theta_Raw.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_Theta_Raw_%i", iMod)));
	v_h2f_Theta_Sel.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_Theta_Sel_%i", iMod)));
	v_h2f_rho_DT_Raw.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_rho_DT_Raw_%i", iMod)));
	v_h2f_rho_DT_Sel.		push_back(pTFile_Control->Get<TH2F>(Form("h2f_rho_DT_Sel_%i", iMod)));
	}

	// Draw out
	std::string OutputFile	= inputDirName + "1_" + Tag + "_Control" + Comment + ".pdf";
	std::string OutputFile_Beg = OutputFile + "(";
	std::string OutputFile_End = OutputFile + ")";
	gStyle->							SetOptStat(111111);
	gStyle->							SetStatX(0.89);
	gStyle->							SetStatY(0.89);

	// Page 1 TL Raw
	TCanvas* pTCanvas =	new TCanvas("TCanvas_Control", "TCanvas_Control", 1800, 1200);
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	int TLow = 0, THigh = 0;
	GetStage3Cut_CSV("../TimeSelection_Cuts.csv", Tag, TLow, THigh);
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_TL_Raw[iMod]->SetLineWidth(2);
	v_h1f_TL_Raw[iMod]->Draw("HIST");
	
	TGraph* pTGraph_1 = new TGraph;
	pTGraph_1->SetPoint( pTGraph_1->GetN(), TLow, 1.E4*v_h1f_TL_Raw[iMod]->GetMaximum() );
	pTGraph_1->SetPoint( pTGraph_1->GetN(), TLow, 0.1	);
	pTGraph_1->SetLineWidth(2);
	pTGraph_1->SetLineColor(2);
	pTGraph_1->Draw("l");	

	TGraph* pTGraph_2 = new TGraph;
	pTGraph_2->SetPoint( pTGraph_2->GetN(), THigh , 1.E4*v_h1f_TL_Raw[iMod]->GetMaximum() );
	pTGraph_2->SetPoint( pTGraph_2->GetN(), THigh , 0.1	);
	pTGraph_2->SetLineColor(2);
	pTGraph_2->SetLineWidth(2);
	pTGraph_2->Draw("l");	

	pTCanvas->Update();

	pTCanvas->cd(iMod+1);
	v_h1f_TL_Sel[iMod]->SetLineWidth(2);
	v_h1f_TL_Sel[iMod]->SetLineColor(kRed);
	v_h1f_TL_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile_Beg.c_str());

	// Page 3 Pad Multiplicity in Clusters Raw
	pTCanvas->Clear();
	gStyle->SetOptStat(111111);
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_PM_Raw[iMod]->SetLineWidth(2);
	v_h1f_PM_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();

	pTCanvas->cd(iMod+1);
	v_h1f_PM_Sel[iMod]->SetLineWidth(2);
	v_h1f_PM_Sel[iMod]->SetLineColor(kRed);
	v_h1f_PM_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 5 Average Pad Multiplicity in Events (per module) Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_APM_Raw[iMod]->SetLineWidth(2);
	v_h1f_APM_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();

	v_h1f_APM_Sel[iMod]->SetLineWidth(2);
	v_h1f_APM_Sel[iMod]->SetLineColor(kRed);
	v_h1f_APM_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 9 Average Pad integral Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_API_Raw[iMod]->SetLineWidth(2);
	v_h1f_API_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	
	v_h1f_API_Sel[iMod]->SetLineWidth(2);
	v_h1f_API_Sel[iMod]->SetLineColor(kRed);
	v_h1f_API_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 Maximum ampitude for leading pad Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(0);
	v_h1f_Qlead_Raw[iMod]->SetLineWidth(2);
	v_h1f_Qlead_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	
	v_h1f_Qlead_Sel[iMod]->SetLineWidth(2);
	v_h1f_Qlead_Sel[iMod]->SetLineColor(kRed);
	v_h1f_Qlead_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 Maximum ampitude for neighbour pads Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_Qneigh_Raw[iMod]->SetLineWidth(2);
	v_h1f_Qneigh_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	
	v_h1f_Qneigh_Sel[iMod]->SetLineWidth(2);
	v_h1f_Qneigh_Sel[iMod]->SetLineColor(kRed);
	v_h1f_Qneigh_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 Maximum ampitude for each pad Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_Qpad_Raw[iMod]->SetLineWidth(2);
	v_h1f_Qpad_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	
	v_h1f_Qpad_Sel[iMod]->SetLineWidth(2);
	v_h1f_Qpad_Sel[iMod]->SetLineColor(kRed);
	v_h1f_Qpad_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 Cluster multipicity Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(1);
	v_h1f_CM_Raw[iMod]->SetLineWidth(2);
	v_h1f_CM_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	SetStatBoxPosition(v_h1f_CM_Raw[iMod], 0.15, 0.35, 0.6, 0.85);
	pTCanvas->Update();
	
	v_h1f_CM_Sel[iMod]->SetLineWidth(2);
	v_h1f_CM_Sel[iMod]->SetLineColor(kRed);
	v_h1f_CM_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	// SetStatBoxPosition(v_h1f_CM_Sel[iMod], 0.15, 0.35, 0.6, 0.85);
	pTCanvas->Update();
	}

	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 13 Cluster efficiency Raw
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	pTCanvas->cd(iMod+1)->SetLogy(0);
	v_h1f_Eff_Raw[iMod]->SetLineWidth(2);
	v_h1f_Eff_Raw[iMod]->Draw("HIST");
	pTCanvas->Update();
	SetStatBoxPosition(v_h1f_Eff_Raw[iMod], 0.15, 0.35, 0.3, 0.55);
	pTCanvas->Update();
	
	v_h1f_Eff_Sel[iMod]->SetLineWidth(2);
	v_h1f_Eff_Sel[iMod]->SetLineColor(kRed);
	v_h1f_Eff_Sel[iMod]->Draw("HIST same");
	pTCanvas->Update();
	// SetStatBoxPosition(v_h1f_Eff_Sel[iMod], 0.15, 0.35, 0.3, 0.55);
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_LeadPos_Raw[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_LeadPos_Sel[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_Theta_Raw[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_Theta_Sel[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_rho_DT_Raw[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// 
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2); 

	for (int iMod = 0; iMod < nMod; iMod++) {
	pTCanvas->cd(iMod+1);
	v_h2f_rho_DT_Sel[iMod]->Draw("colz");
	pTCanvas->Update();
	}
	pTCanvas->SaveAs(OutputFile_End.c_str());
	delete pTCanvas;

	for (int iMod = 0; iMod < nMod; iMod++) {
	delete v_h1f_PM_Raw[iMod];		v_h1f_PM_Raw[iMod] = 0;
	delete v_h1f_PM_Sel[iMod];		v_h1f_PM_Sel[iMod] = 0;
	delete v_h1f_TL_Raw[iMod];		v_h1f_TL_Raw[iMod] = 0;
	delete v_h1f_TL_Sel[iMod];		v_h1f_TL_Sel[iMod] = 0;
	delete v_h1f_APM_Raw[iMod];		v_h1f_APM_Raw[iMod] = 0;
	delete v_h1f_APM_Sel[iMod];		v_h1f_APM_Sel[iMod] = 0;
	delete v_h1f_API_Raw[iMod];		v_h1f_API_Raw[iMod] = 0;
	delete v_h1f_API_Sel[iMod];		v_h1f_API_Sel[iMod] = 0;
	delete v_h1f_CM_Raw[iMod];		v_h1f_CM_Raw[iMod] = 0;
	delete v_h1f_CM_Sel[iMod];		v_h1f_CM_Sel[iMod] = 0;
	delete v_h1f_Qpad_Raw[iMod];		v_h1f_Qpad_Raw[iMod] = 0;
	delete v_h1f_Qpad_Sel[iMod];		v_h1f_Qpad_Sel[iMod] = 0;
	delete v_h1f_Qlead_Raw[iMod];		v_h1f_Qlead_Raw[iMod] = 0;
	delete v_h1f_Qlead_Sel[iMod];		v_h1f_Qlead_Sel[iMod] = 0;
	delete v_h1f_Qneigh_Raw[iMod];	v_h1f_Qneigh_Raw[iMod] = 0;
	delete v_h1f_Qneigh_Sel[iMod];	v_h1f_Qneigh_Sel[iMod] = 0;
	delete v_h1f_Eff_Raw[iMod];		v_h1f_Eff_Raw[iMod] = 0;
	delete v_h1f_Eff_Sel[iMod];		v_h1f_Eff_Sel[iMod] = 0;
	delete v_h2f_LeadPos_Raw[iMod];	v_h2f_LeadPos_Raw[iMod] = 0;
	delete v_h2f_LeadPos_Sel[iMod];	v_h2f_LeadPos_Sel[iMod] = 0;
	delete v_h2f_Theta_Raw[iMod];	v_h2f_Theta_Raw[iMod] = 0;
	delete v_h2f_Theta_Sel[iMod];	v_h2f_Theta_Sel[iMod] = 0;
	delete v_h2f_rho_DT_Raw[iMod];	v_h2f_rho_DT_Raw[iMod] = 0;
	delete v_h2f_rho_DT_Sel[iMod];	v_h2f_rho_DT_Sel[iMod] = 0;
	}
	v_h1f_TL_Raw.						clear();
	v_h1f_TL_Sel.						clear();
	v_h1f_PM_Raw.						clear();
	v_h1f_PM_Sel.						clear();
	v_h1f_APM_Raw.						clear();
	v_h1f_APM_Sel.						clear();
	v_h1f_API_Raw.						clear();
	v_h1f_API_Sel.						clear();
	v_h1f_CM_Raw.						clear();
	v_h1f_CM_Sel.						clear();
	v_h1f_Qpad_Raw.						clear();
	v_h1f_Qpad_Sel.						clear();
	v_h1f_Qlead_Raw.						clear();
	v_h1f_Qlead_Sel.						clear();
	v_h1f_Qneigh_Raw.					clear();
	v_h1f_Qneigh_Sel.					clear();
	v_h1f_Eff_Raw.						clear();
	v_h1f_Eff_Sel.						clear();
	v_h2f_LeadPos_Raw.					clear();
	v_h2f_LeadPos_Sel.					clear();
	v_h2f_Theta_Raw.					clear();
	v_h2f_Theta_Sel.					clear();
	v_h2f_rho_DT_Raw.					clear();
	v_h2f_rho_DT_Sel.					clear();
}














void DrawOut_Checks(const std::string& OutDir, const std::string EvtFile, const std::string& Tag, const std::string& Comment){

	std::cout										<< std::endl;
	std::cout << " DrawOuts: Checks"			<< std::endl;
	std::cout << " OUTDirName: " << OutDir		<< std::endl;
	std::cout										<< std::endl;
	int statXin					= gStyle->GetStatX();
	int statYin					= gStyle->GetStatY();
	gStyle->						SetOptStat(111111);
	gStyle->						SetStatX(0.33);
	gStyle->						SetStatY(0.89);

	// Get histograms
	TFile* pTFile_Checks			= TFile::Open(TString(OutDir + Tag + "/2_" + Tag + "_Checks" + Comment + ".root"));

	// Get histograms & functions
	TH1F* h1f_Tcross				= pTFile_Checks->Get<TH1F>("h1f_Tcross");
	TH1F* h1f_Ncross				= pTFile_Checks->Get<TH1F>("h1f_Ncross");
	TH1F* h1f_angle				= pTFile_Checks->Get<TH1F>("h1f_angle");
	TH1F* h1f_Lnorm				= pTFile_Checks->Get<TH1F>("h1f_Lnorm");
	TH1F* h1f_dnorm				= pTFile_Checks->Get<TH1F>("h1f_dnorm");
	TH1F* h1f_zdiff				= pTFile_Checks->Get<TH1F>("h1f_zdiff");
	TH1F* h1f_dist				= pTFile_Checks->Get<TH1F>("h1f_dist");
	TH1F* h1f_dist_clus			= pTFile_Checks->Get<TH1F>("h1f_dist_clus");
	TH1F* h1f_WFcorr				= pTFile_Checks->Get<TH1F>("h1f_WFcorr");
	TH1F* h1f_Gcorr				= pTFile_Checks->Get<TH1F>("h1f_Gcorr");
	TH2F* h2f_dEdx_Y				= pTFile_Checks->Get<TH2F>("h2f_dEdx_Y");
	TH2F* h2f_ratiodiffZ			= pTFile_Checks->Get<TH2F>("h2f_ratiodiffZ");
	TH2F* h2f_AmaxvsLength		= pTFile_Checks->Get<TH2F>("h2f_AmaxvsLength");
	TH2F* h2f_LUTvsLength		= pTFile_Checks->Get<TH2F>("h2f_LUTvsLength");
	TH2F* h2f_WFvsLength			= pTFile_Checks->Get<TH2F>("h2f_WFvsLength");
	TH2F* h2f_WFsumvsLength		= pTFile_Checks->Get<TH2F>("h2f_WFsumvsLength");
	TH2F* h2f_WFstarvsLen		= pTFile_Checks->Get<TH2F>("h2f_WFstarvsLen");
	TH2F* h2f_WFstartrcvsLen		= pTFile_Checks->Get<TH2F>("h2f_WFstartrcvsLen");
	TF1* A_corr					= new TF1;
	//	if(Tag.find("diag") != std::string::npos){
	//	std::string filename = EvtFile.substr(0, EvtFile.length()-5);
	//	TFile* pfile = new TFile((filename + "_WFmax_correction.root").c_str(), "READ");
	//	A_corr						= pfile->Get<TF1>("A_corr");
	//	pfile->Close();
	// }
	float pivot					= 0;
	// float pivot					= A_corr->Eval(11.28);
	TF1* A_corr_HATRecon			= new TF1("A_corr_Vlada", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
	TH1F* h1f_WFoLength			= pTFile_Checks->Get<TH1F>("h1f_WFoLength");
	TH2F* h2f_lenVSd				= pTFile_Checks->Get<TH2F>("h2f_lenVSd");

	// Draw out
	std::string OutputFile		= OutDir + Tag + "/2_" + Tag + "_Checks" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";

	TCanvas* pTCanvas			= new TCanvas("TCanvas_Control", "TCanvas_Control", 1800, 1200);
	pTCanvas->						cd();

	pTCanvas->						SetLogy(1);	
	h1f_Tcross->					SetLineWidth(4);
	h1f_Tcross->					Draw();
	pTCanvas->						SaveAs(OutputFile_Beg.c_str());
	pTCanvas->						SetLogy(0);	

	pTCanvas->						Clear();
	h1f_Ncross->					SetLineWidth(4);
	h1f_Ncross->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h1f_zdiff->					SetLineWidth(4);
	h1f_zdiff->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	gStyle->						SetStatX(0.89);
	pTCanvas->						Clear();
	h1f_angle->					SetLineWidth(4);
	h1f_angle->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	gStyle->						SetStatX(0.33);
	gStyle->						SetStatY(0.89);
	pTCanvas->						SetLogy(1);	
	pTCanvas->						Clear();
	h1f_Lnorm->					SetLineWidth(4);
	h1f_Lnorm->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	gStyle->						SetStatX(0.35);
	gStyle->						SetStatY(0.45);
	pTCanvas->						SetLogy(1);	
	pTCanvas->						Clear();
	h1f_dnorm->					SetLineWidth(4);
	h1f_dnorm->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h1f_dist->						SetLineWidth(4);
	h1f_dist->						Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h1f_dist_clus->				SetLineWidth(4);
	h1f_dist_clus->				Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	pTCanvas->						SetLogy(0);	

	gStyle->						SetStatX(0.89);
	gStyle->						SetStatY(0.89);

	pTCanvas->						Clear();
	h1f_WFcorr->					SetLineWidth(4);
	h1f_WFcorr->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	pTCanvas->						SetLogy(0); 

	pTCanvas->						Clear();
	h1f_Gcorr->					SetLineWidth(4);
	h1f_Gcorr->					Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_dEdx_Y->					Draw("colz");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_ratiodiffZ->				Draw("colz");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_AmaxvsLength->				Draw("colz");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_LUTvsLength->				Draw("colz");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_WFvsLength->				Draw("colz");
	if(Tag.find("diag") != std::string::npos){
	A_corr->						SetLineWidth(4);
	A_corr->						Draw("same");
	A_corr_HATRecon->				SetLineWidth(4);
	A_corr_HATRecon->				SetLineColor(kBlack);
	A_corr_HATRecon->				Draw("same");
	}
	h2f_WFvsLength->				GetYaxis()->SetRangeUser(0, 2000);
	gPad->							Update();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_WFsumvsLength->			Draw("colz");
	if(Tag.find("diag") != std::string::npos){
	A_corr->						SetLineWidth(4);
	A_corr->						Draw("same");
	// A_corr_HATRecon->				SetLineWidth(4);
	// A_corr_HATRecon->				SetLineColor(kBlack);
	// A_corr_HATRecon->				Draw("same");
	}
	h2f_WFsumvsLength->			GetYaxis()->SetRangeUser(0, 2000);
	gPad->							Update();
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_WFstarvsLen->				Draw("colz");
	h2f_WFstarvsLen->				GetYaxis()->SetRangeUser(0, 2000);
	gPad->							Update();
	TLine *pline					= new TLine(pTCanvas->GetUxmin(), pivot, pTCanvas->GetUxmax(), pivot);
	pline->						SetLineStyle(9);
	pline->						SetLineWidth(4);
	pline->						SetLineColor(kRed);
	pline->						Draw("same");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						Clear();
	h2f_WFstartrcvsLen->			Draw("colz");
	h2f_WFstartrcvsLen->			GetYaxis()->SetRangeUser(0, 2000);
	gPad->							Update();
	pline->						Draw("same");
	pTCanvas->						SaveAs(OutputFile.c_str());

	pTCanvas->						SetLogy(1);	
	pTCanvas->						Clear();
	gStyle->						SetStatX(0.88);
	gStyle->						SetStatY(0.88);
	h1f_WFoLength->				SetLineWidth(4);
	h1f_WFoLength->				Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	pTCanvas->						SetLogy(0);	

	gStyle->						SetStatX(0.25);
	gStyle->						SetStatY(0.65);
	pTCanvas->						Clear();
	TGraph* low_tg				= hist_to_graph(h2f_lenVSd->ProjectionX());
	TGraph* left_tg				= Swapped_graph(h2f_lenVSd->ProjectionY());
	TPad* center_pad				= new TPad("center_pad", "center_pad", 0.20, 0.29, 1,	1);
	TPad* low_pad				= new TPad("low_pad",	"low_pad",	0.11, 0.03, 1,	0.3);
	TPad* left_pad				= new TPad("left_pad",	"left_pad",	0.00, 0.26, 0.195, 1);
	center_pad->					SetMargin(0.0, 0.11, 0.02, 0.1);
	low_pad->						SetMargin(0.101, 0.099, 0.25, 0);
	left_pad->						SetMargin(0.25, 0.00, 0.06,	0.095);
	low_pad->						SetFillStyle(4000);
	center_pad->					Draw(); 
	low_pad->						Draw();
	left_pad->						Draw();
	low_pad->						SetFillStyle(4000);
	low_tg->						SetLineWidth(2);
	low_tg->						SetTitle(";Length in pad (mm);");
	low_tg->						GetXaxis()->SetTitleSize(0.12);
	low_tg->						GetXaxis()->SetLabelSize(0.1);
	low_tg->						GetYaxis()->SetLabelSize(0.1);
	low_tg->						GetYaxis()->SetLabelOffset(-0.005);
	left_tg->						SetLineWidth(2);
	left_tg->						SetTitle(";;Impact parameter d (mm)");
	left_tg->						GetYaxis()->SetTitleSize(0.1);
	left_tg->						GetYaxis()->SetLabelSize(0.09);
	left_tg->						GetXaxis()->SetLabelSize(0.09);
	left_tg->						GetXaxis()->SetLabelOffset(-0.05);
	center_pad->					cd();
	h2f_lenVSd->					GetXaxis()->SetLabelSize(0);
	h2f_lenVSd->					GetYaxis()->SetTitleSize(0);
	h2f_lenVSd->					Draw("colz");
	low_pad->						cd();
	low_pad->						SetLogy();
	low_tg->						GetXaxis()->SetLimits(h2f_lenVSd->GetXaxis()->GetXmin(), h2f_lenVSd->GetXaxis()->GetXmax());
	low_tg->						Draw("apl Y+");
	left_pad->						cd();
	left_pad->						SetLogx();
	left_tg->						SetMinimum(h2f_lenVSd->GetYaxis()->GetXmin());
	left_tg->						SetMaximum(h2f_lenVSd->GetYaxis()->GetXmax());
	left_tg->						Draw("apl");
	pTCanvas->						SaveAs(OutputFile_End.c_str());

	delete h1f_Tcross;
	delete h1f_Ncross;
	delete h1f_zdiff;
	delete h1f_angle;
	delete h1f_Lnorm;
	delete h1f_dnorm;
	delete h1f_dist;
	delete h1f_WFcorr;
	delete h1f_Gcorr;
	delete h2f_dEdx_Y;
	delete h2f_ratiodiffZ;
	delete h2f_AmaxvsLength;
	delete h2f_LUTvsLength;
	delete h2f_WFvsLength;
	delete h2f_WFsumvsLength;
	delete h2f_lenVSd;
	delete pTCanvas;

	gStyle->							SetStatX(statXin);
	gStyle->							SetStatY(statYin);
}













void DrawOut_dEdx(const std::string& OutDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod)
{
	// Get input file
	TFile* pTFile_dEdx	= TFile::Open(TString(OutDir + "/" + Tag + "/" + "3_" + Tag + "_dEdx_GPv3.root"));

	// Get histograms & functions
	std::vector<TH1F*>	v_h1f_Qsel;
	std::vector<TH1F*>	v_h1f_Qtrunc;
	std::vector<TH1F*>	v_h1f_WFsel;
	std::vector<TH1F*>	v_h1f_WFsum;
	std::vector<TH1F*>	v_h1f_GPsel;
	std::vector<TH1F*>	v_h1f_GPv4;

	std::vector<TF1*>	v_tf1_Qsel;
	std::vector<TF1*>	v_tf1_Qtrunc;
	std::vector<TF1*>	v_tf1_WFsel;
	std::vector<TF1*>	v_tf1_WFsum;
	std::vector<TF1*>	v_tf1_GPsel;
	std::vector<TF1*>	v_tf1_GPv4;

	for (int iMod = 0; iMod < nMod; iMod++){
	v_h1f_Qsel.		push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_Qsel_%i", iMod)));
	v_h1f_Qtrunc.	push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_Qtrunc_%i", iMod)));
	v_h1f_WFsel.		push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_WFsel_%i", iMod)));
	v_h1f_WFsum.	push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_WFsum_%i", iMod)));
	v_h1f_GPsel.		push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_GPsel_%i", iMod)));
	v_h1f_GPv4.		push_back(pTFile_dEdx->Get<TH1F>(Form("h1f_GPv4_%i", iMod)));

	v_tf1_Qsel.		push_back(pTFile_dEdx->Get<TF1>(Form("tf1_Qsel_%i", iMod)));
	v_tf1_Qtrunc.		push_back(pTFile_dEdx->Get<TF1>(Form("tf1_Qtrunc_%i", iMod)));
	v_tf1_WFsel.		push_back(pTFile_dEdx->Get<TF1>(Form("tf1_WFsel_%i", iMod)));
	v_tf1_WFsum.	push_back(pTFile_dEdx->Get<TF1>(Form("tf1_WFsum_%i", iMod)));
	v_tf1_GPsel.		push_back(pTFile_dEdx->Get<TF1>(Form("tf1_GPsel_%i", iMod)));
	v_tf1_GPv4.		push_back(pTFile_dEdx->Get<TF1>(Form("tf1_GPv4_%i", iMod)));
	}

	std::string OutputFile	= OutDir + "/" + Tag + "/3_" + Tag + "_dEdx_GPv3.pdf";
	std::string OutputFile_Beg = OutputFile + "(";
	std::string OutputFile_End = OutputFile + ")";
	gStyle->SetOptStat(111111);
	gStyle->SetOptFit(111);
	TCanvas* pTCanvas =	new TCanvas("TCanvas_Status", "TCanvas_Status", 1800, 1200);

	// Page 1 Q raw
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_Qsel[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_Qsel[iMod]->SetLineColor(kBlack);
	v_tf1_Qsel[iMod]->SetLineColor(kMagenta+2);
	v_h1f_Qsel[iMod]->SetLineWidth(2);
	v_tf1_Qsel[iMod]->SetLineWidth(2);
	v_h1f_Qsel[iMod]->Draw();
	v_tf1_Qsel[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_Qsel[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_Qsel[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_Qsel[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_Qsel[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile_Beg.c_str());

	// Page 3 Q trunc
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_Qtrunc[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_Qtrunc[iMod]->SetLineColor(kBlue+3);
	v_tf1_Qtrunc[iMod]->SetLineColor(kBlue-6);
	v_h1f_Qtrunc[iMod]->SetLineWidth(2);
	v_tf1_Qtrunc[iMod]->SetLineWidth(2);
	v_h1f_Qtrunc[iMod]->Draw();
	v_tf1_Qtrunc[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_Qtrunc[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_Qtrunc[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_Qtrunc[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_Qtrunc[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 5 WRsel
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_WFsel[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_WFsel[iMod]->SetLineColor(kGreen+3);
	v_tf1_WFsel[iMod]->SetLineColor(kGreen-6);
	v_h1f_WFsel[iMod]->SetLineWidth(2);
	v_tf1_WFsel[iMod]->SetLineWidth(2);
	v_h1f_WFsel[iMod]->Draw();
	v_tf1_WFsel[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_WFsel[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_WFsel[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_WFsel[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_WFsel[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 7 WF trunc
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_WFsum[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_WFsum[iMod]->SetLineColor(kCyan+2);
	v_tf1_WFsum[iMod]->SetLineColor(kCyan-3);
	v_h1f_WFsum[iMod]->SetLineWidth(2);
	v_tf1_WFsum[iMod]->SetLineWidth(2);
	v_h1f_WFsum[iMod]->Draw();
	v_tf1_WFsum[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_WFsum[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_WFsum[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_WFsum[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_WFsum[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 9 GPsel
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_GPsel[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_GPsel[iMod]->SetLineColor(kGreen+2);
	v_tf1_GPsel[iMod]->SetLineColor(kGreen-2);
	v_h1f_GPsel[iMod]->SetLineWidth(2);
	v_tf1_GPsel[iMod]->SetLineWidth(2);
	v_h1f_GPsel[iMod]->Draw();
	v_tf1_GPsel[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_GPsel[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_GPsel[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_GPsel[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_GPsel[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 GPtrunc
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);
	v_h1f_GPv4[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_GPv4[iMod]->SetLineColor(kMagenta+3);
	v_tf1_GPv4[iMod]->SetLineColor(kMagenta-6);
	v_h1f_GPv4[iMod]->SetLineWidth(2);
	v_tf1_GPv4[iMod]->SetLineWidth(2);
	v_h1f_GPv4[iMod]->Draw();
	v_tf1_GPv4[iMod]->Draw("same");
	gPad->Update();
	if (v_h1f_GPv4[iMod]->GetMean() > (pTCanvas->GetUxmax() - pTCanvas->GetUxmin())/2) SetStatBoxPosition(v_h1f_GPv4[iMod], 0.12, 0.42, 0.52, 0.87);
	else SetStatBoxPosition(v_h1f_GPv4[iMod], 0.58, 0.88, 0.52, 0.87);
	PrintResolution(v_h1f_GPv4[iMod], pTCanvas, 0.05, 0.3, kBlack , "");
	}
	pTCanvas->SaveAs(OutputFile.c_str());

	// Page 11 Summary selected & truncated Q & WF & GP
	pTCanvas->Clear();
	if (nMod != 1) pTCanvas->Divide(2,2);
	for (int iMod = 0; iMod < nMod; iMod++){
	pTCanvas->cd(iMod+1);

	int QselMax	= v_h1f_Qsel[iMod]->GetMaximum();
	int QtruncMax	= v_h1f_Qtrunc[iMod]->GetMaximum();
	int WFselMax	= v_h1f_WFsel[iMod]->GetMaximum();
	int WFsumMax	= v_h1f_WFsum[iMod]->GetMaximum();
	int GPselMax	= v_h1f_GPsel[iMod]->GetMaximum();
	int GPtruncMax	= v_h1f_GPv4[iMod]->GetMaximum();

	v_h1f_Qsel[iMod]->SetStats(0);
	v_h1f_Qsel[iMod]->SetAxisRange(0, 1500, "X");
	v_h1f_Qsel[iMod]->SetAxisRange(0, 1.1 * std::max({QselMax, QtruncMax, WFselMax, WFsumMax, GPselMax, GPtruncMax}),	"Y");
	v_h1f_Qsel[iMod]->SetTitle("Mean dE/dx using Q & WF & GP");
	v_h1f_Qsel[iMod]->Draw();
	v_tf1_Qsel[iMod]->Draw("same");

	v_h1f_Qtrunc[iMod]->SetStats(0);
	v_h1f_Qtrunc[iMod]->Draw("same");
	v_tf1_Qtrunc[iMod]->Draw("same");

	v_h1f_WFsel[iMod]->SetStats(0);
	v_h1f_WFsel[iMod]->Draw("same");
	v_tf1_WFsel[iMod]->Draw("same");

	v_h1f_WFsum[iMod]->SetStats(0);
	v_h1f_WFsum[iMod]->Draw("same");
	v_tf1_WFsum[iMod]->Draw("same");

	v_h1f_GPsel[iMod]->SetStats(0);
	v_h1f_GPsel[iMod]->Draw("same");
	v_tf1_GPsel[iMod]->Draw("same");

	v_h1f_GPv4[iMod]->SetStats(0);
	v_h1f_GPv4[iMod]->Draw("same");
	v_tf1_GPv4[iMod]->Draw("same");


	TLegend* leg = new TLegend(0.6,0.60,0.87,0.87); 
	leg->AddEntry(v_h1f_Qsel[iMod], "Q not truncated ", "l");	
	leg->AddEntry(v_h1f_Qtrunc[iMod], "Q truncated ", "l");	
	leg->AddEntry(v_h1f_WFsel[iMod], "WF not truncated ", "l");	
	leg->AddEntry(v_h1f_WFsum[iMod], "WF truncated ", "l");	
	leg->AddEntry(v_h1f_GPsel[iMod], "GP not truncated ", "l");	
	leg->AddEntry(v_h1f_GPv4[iMod], "GP truncated ", "l");	
	leg->DrawClone();
	pTCanvas->Update();
	delete leg;
	}

	pTCanvas->SaveAs(OutputFile_End.c_str());
	delete pTCanvas;
}









void DrawOut_Methods(const std::string& OutDir, const std::string& Tag, const std::string& Comment, const int& nMod){

	std::cout										<< std::endl;
	std::cout << " DrawOuts: Methods comparison" << std::endl;
	std::cout << " OUTDirName: " << OutDir		<< std::endl;
	std::cout										<< std::endl;
	int statXin				= gStyle->GetStatX();
	int statYin				= gStyle->GetStatY();

	// Get histograms
	TFile* pTFile_dEdx			= TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + ".root"));

	std::vector<TH1F*>				v_h1f_WFsum;
	std::vector<TH1F*>				v_h1f_XP;
	std::vector<TF1*>				v_tf1_WFsum;
	std::vector<TF1*>				v_tf1_XP;

	for (int iMod = 0; iMod < nMod; iMod++){
	v_h1f_WFsum.					push_back(pTFile_dEdx->Get<TH1F>("h1f_WFsum"));
	v_h1f_XP.						push_back(pTFile_dEdx->Get<TH1F>("h1f_XP"));
	v_tf1_WFsum.					push_back(pTFile_dEdx->Get<TF1> ("tf1_WFsum"));
	v_tf1_XP.						push_back(pTFile_dEdx->Get<TF1> ("tf1_XP"));

	// Set Display
	v_h1f_WFsum[iMod]->		SetTitle(std::string("Mean dE/dx;mean dE/dx of each event;Counts").c_str());
	// Set line color
	v_h1f_WFsum[iMod]->		SetLineColor(kCyan+2);
	v_h1f_XP[iMod]->			SetLineColor(kMagenta+2);
	v_tf1_WFsum[iMod]->		SetLineColor(kCyan-3);
	v_tf1_XP[iMod]->			SetLineColor(kMagenta+1);
	// Set line width
	v_h1f_WFsum[iMod]->		SetLineWidth(2);
	v_h1f_XP[iMod]->			SetLineWidth(2);
	v_tf1_WFsum[iMod]->		SetLineWidth(2);
	v_tf1_XP[iMod]->			SetLineWidth(2);
	}

	// Make canvas
	std::string OutputFile			= OutDir + Tag + "/3_" + Tag + "_Comparison" + Comment + ".pdf";
	TCanvas* pTCanvas				= new TCanvas("pTCanvas", "pTCanvas", 1800, 1200);

	//	Draw
	TPaveStats* pStat_WF;
	TPaveStats* pStat_XP;
	for(int iMod = 0; iMod < nMod; iMod++){
	int WFsumMax					= v_h1f_WFsum[iMod]->GetMaximum();
	int XPMax						= v_h1f_XP[iMod]->GetMaximum();
	v_h1f_WFsum[iMod]->SetAxisRange(0, 1.1 * std::max({WFsumMax, XPMax}),	"Y");

	float invX						= 0;
	if(v_h1f_WFsum[iMod]->GetMean() > 800) invX = 0.4;

	pTCanvas->cd(iMod+1);
	gStyle->							SetOptStat(11);
	gStyle->							SetOptFit(111);	

	v_h1f_WFsum[iMod]->				Draw("HIST");
	v_tf1_WFsum[iMod]->				Draw("same");
	gPad->								Update();
	SetStatBoxPosition(v_h1f_WFsum[iMod], 0.76, 0.99, 0.52, 0.72);
	pStat_WF							= (TPaveStats*)v_h1f_WFsum[iMod]->FindObject("stats");
	pStat_WF->							SetTextColor(kCyan+2);

	v_h1f_XP[iMod]->			Draw("HIST sames");
	v_tf1_XP[iMod]->			Draw("same");
	gPad->								Update();
	SetStatBoxPosition(v_h1f_XP[iMod], 0.76, 0.99, 0.72, 0.92);	
	pStat_XP						= (TPaveStats*)v_h1f_XP[iMod]->FindObject("stats");
	pStat_XP->						SetTextColor(kMagenta+2);

	PrintResolution(v_h1f_XP[iMod],	pTCanvas, 0.8-invX, 0.93, kMagenta+2, "XP");
	PrintResolution(v_h1f_WFsum[iMod], pTCanvas, 0.8-invX, 0.83, kCyan+2, "WF");

	}
	pTCanvas->					SaveAs(OutputFile.c_str());

	delete						pTCanvas;
	v_h1f_WFsum.				clear();
	v_h1f_XP.				clear();
	v_tf1_WFsum.				clear();
	v_tf1_XP.				clear();
	delete						pTFile_dEdx;
	gStyle->					SetStatX(statXin);
	gStyle->					SetStatY(statYin);
	gStyle->					SetOptStat(111111);
}





// Draw resolution of 2 different procedures
void DrawOut_Versions(const std::string& inputDir, const std::string& Method, const std::string& Comment1, const std::string& Comment2)
{
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	// Draw
	std::string OutputFile		= inputDir + "/Comparison_" + Method + Comment1 + "_" + Comment2 + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.65,0.75,0.98,0.93);
	std::string leg1			= "XP w/o Gain correction";
	std::string leg2			= "XP w/ Gain correction";

	std::string method;
	Color_t kBefore;
	Color_t kAfter;
	if(Method == "WF"){
	method = "tf1_WFsum";
	kBefore = kCyan+2;
	kAfter	= kOrange-3;
	}
	if(Method == "XP"){
	method = "tf1_XP";
	kBefore = kGreen+3;
	kAfter	= kMagenta+2;
	}

	int ipoint				= 0;
	int npoint				= 11;
	std::string name_arr[]	= {"m140", "m120", "m0", "m80", "m60", "m40", "0", "20", "40", "60", "80"};
	int		Row_arr[]	= {27, 26, 24, 22, 20, 18, 12, 10, 8, 6, 4};

	std::vector<std::vector<TF1*>>	arr_1; // vector for method 1 => 1 vector per scan => TF1
	std::vector<std::vector<TF1*>>	arr_2;
	std::vector<TGraphErrors*>		v_reso1;
	std::vector<TGraphErrors*>		v_reso2;
	std::vector<TGraphErrors*>		v_mean1;
	std::vector<TGraphErrors*>		v_mean2;
	std::vector<TGraphErrors*>		v_std1;
	std::vector<TGraphErrors*>		v_std2;


	// Get mean & std
	float mean_1[npoint];
	float mean_2[npoint];
	float dmean_1[npoint];
	float dmean_2[npoint];
	float std_1[npoint];
	float std_2[npoint];
	float dstd_1[npoint];
	float dstd_2[npoint];

	// Y scan
	std::vector<TF1*>				v_1_y;
	std::vector<TF1*>				v_2_y;
	for(int i = 0; i < 6; i++){
	v_reso1.						push_back(new TGraphErrors());
	v_reso2.						push_back(new TGraphErrors());
	v_mean1.						push_back(new TGraphErrors());
	v_mean2.						push_back(new TGraphErrors());
	v_std1.						push_back(new TGraphErrors());
	v_std2.						push_back(new TGraphErrors());
	}
	for(int iy = 0; iy < npoint; iy++){
	TFile* file1				= TFile::Open(TString(inputDir + "DESY21_yscan/DESY21_y" + name_arr[iy] + "/3_DESY21_y" + name_arr[iy] + "_dEdx" + Comment1 + ".root"));
	TFile* file2				= TFile::Open(TString(inputDir + "DESY21_yscan/DESY21_y" + name_arr[iy] + "/3_DESY21_y" + name_arr[iy] + "_dEdx" + Comment2 + ".root"));
	v_1_y.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_y.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_y);
	arr_2.push_back(v_2_y);

	for(int i = 0; i < npoint; i++){
	mean_1[i]				= arr_1[0][i]->	GetParameter(1);
	mean_2[i]				= arr_2[0][i]->	GetParameter(1);
	std_1[i]					= arr_1[0][i]->	GetParameter(2);
	std_2[i]					= arr_2[0][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[0][i]->	GetParError(1);
	dmean_2[i]				= arr_2[0][i]->	GetParError(1);
	dstd_1[i]				= arr_1[0][i]->	GetParError(2);
	dstd_2[i]				= arr_2[0][i]->	GetParError(2);

	v_reso1[0]->	SetPoint		(ipoint, Row_arr[i], std_1[i]/mean_1[i]*100);
	v_reso2[0]->	SetPoint		(ipoint, Row_arr[i], std_2[i]/mean_2[i]*100);
	v_reso1[0]->	SetPointError (ipoint, 0,			GetResoError(arr_1[0][i]));
	v_reso2[0]->	SetPointError (ipoint, 0,			GetResoError(arr_2[0][i]));

	v_mean1[0]->	SetPoint		(ipoint, Row_arr[i], mean_1[i]);
	v_mean2[0]->	SetPoint		(ipoint, Row_arr[i], mean_2[i]);
	v_mean1[0]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[0]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[0]->	SetPoint		(ipoint, Row_arr[i], std_1[i]);
	v_std2[0]->	SetPoint		(ipoint, Row_arr[i], std_2[i]);
	v_std1[0]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[0]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[0]->				GetXaxis()->SetLimits(-0.5, 31.5);
	v_reso1[0]->				SetMinimum(4);
	v_reso1[0]->				SetMaximum(12);
	v_reso1[0]->				SetNameTitle("v_reso1[0]", "Resolution vs Y position;Pad row;resolution (%)");
	Graphic_setup(v_reso1[0], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[0], 3, 20, kAfter,	1, kBlack);
	v_reso1[0]->				Draw("ap");
	v_reso2[0]->				Draw("p same");
	leg->						AddEntry(v_reso1[0], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[0], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	v_mean1[0]->				GetXaxis()->SetLimits(-0.5, 31.5);
	v_mean1[0]->				SetMinimum(600);
	v_mean1[0]->				SetMaximum(1200);
	v_mean1[0]->				SetNameTitle("v_mean1[0]", "Mean vs Y position;Pad row;Mean (ADC count)");
	Graphic_setup(v_mean1[0], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[0], 3, 20, kAfter,	1, kBlack);
	v_mean1[0]->				Draw("ap");
	v_mean2[0]->				Draw("p same"); 
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[0]->				GetXaxis()->SetLimits(-0.5, 31.5);
	v_std1[0]->				SetMinimum(40);
	v_std1[0]->				SetMaximum(121);
	v_std1[0]->				SetNameTitle("v_std1[0]", "Std vs Y position;Pad row;std (ADC count)");
	Graphic_setup(v_std1[0], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[0], 3, 20, kAfter,	1, kBlack);
	v_std1[0]->				Draw("ap");
	v_std2[0]->				Draw("p same"); 
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());
	leg->Clear();




	// Z 200
	std::vector<TF1*>				v_1_z200;
	std::vector<TF1*>				v_2_z200;
	npoint = 9;
	for(int i = 0; i < 6; i++){
	v_reso1.	clear();
	v_reso2.	clear();
	v_mean1.	clear();
	v_mean2.	clear();
	v_std1.	clear();
	v_std2.	clear();
	}
	for(int i = -1; i < npoint; i++){
	TFile* file1;
	TFile* file2;
	if(i == -1){
		file1		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT200/DESY21_zm40_PT200/3_DESY21_zm40_PT200_dEdx" + Comment1 + ".root"));
		file2		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT200/DESY21_zm40_PT200/3_DESY21_zm40_PT200_dEdx" + Comment2 + ".root"));
	}
	else{
		file1		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT200/DESY21_z" + i + "60_PT200/3_DESY21_z" + i + "60_PT200_dEdx" + Comment1 + ".root"));
		file2		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT200/DESY21_z" + i + "60_PT200/3_DESY21_z" + i + "60_PT200_dEdx" + Comment2 + ".root"));
	}
	v_1_z200.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_z200.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_z200);
	arr_2.push_back(v_2_z200);

	ipoint = 0;
	for(int i = 0; i <= npoint; i++){
	mean_1[i]				= arr_1[1][i]->	GetParameter(1);
	mean_2[i]				= arr_2[1][i]->	GetParameter(1);
	std_1[i]					= arr_1[1][i]->	GetParameter(2);
	std_2[i]					= arr_2[1][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[1][i]->	GetParError(1);
	dmean_2[i]				= arr_2[1][i]->	GetParError(1);
	dstd_1[i]				= arr_1[1][i]->	GetParError(2);
	dstd_2[i]				= arr_2[1][i]->	GetParError(2);

	v_reso1[1]->	SetPoint		(ipoint, i*100+50, std_1[i]/mean_1[i]*100);
	v_reso2[1]->	SetPoint		(ipoint, i*100+50, std_2[i]/mean_2[i]*100);
	v_reso1[1]->	SetPointError (ipoint, 0,			GetResoError(arr_1[1][i]));
	v_reso2[1]->	SetPointError (ipoint, 0,			GetResoError(arr_2[1][i]));

	v_mean1[1]->	SetPoint		(ipoint, i*100+50, mean_1[i]);
	v_mean2[1]->	SetPoint		(ipoint, i*100+50, mean_2[i]);
	v_mean1[1]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[1]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[1]->	SetPoint		(ipoint, i*100+50, std_1[i]);
	v_std2[1]->	SetPoint		(ipoint, i*100+50, std_2[i]);
	v_std1[1]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[1]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[1]->				GetXaxis()->SetLimits(0, 1000);
	v_reso1[1]->				SetMinimum(4);
	v_reso1[1]->				SetMaximum(12);
	v_reso1[1]->				SetNameTitle("v_reso1[1]", "Resolution vs Z_{drift} (200 ns);Z_{drift} (mm);resolution (%)");
	Graphic_setup(v_reso1[1], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[1], 3, 20, kAfter,	1, kBlack);
	v_reso1[1]->				Draw("ap");
	v_reso2[1]->				Draw("p same");
	leg->						AddEntry(v_reso1[1], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[1], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Mean
	v_mean1[1]->				GetXaxis()->SetLimits(0, 1000);
	v_mean1[1]->				SetMinimum(600);
	v_mean1[1]->				SetMaximum(1200);
	v_mean1[1]->				SetNameTitle("v_mean1[1]", "Mean vs Z_{drift} (200 ns);Z_{drift} (mm);Mean (ADC count)");
	Graphic_setup(v_mean1[1], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[1], 3, 20, kAfter,	1, kBlack);
	v_mean1[1]->				Draw("ap");
	v_mean2[1]->				Draw("p same"); 
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[1]->				GetXaxis()->SetLimits(0, 1000);
	v_std1[1]->				SetMinimum(40);
	v_std1[1]->				SetMaximum(121);
	v_std1[1]->				SetNameTitle("v_std1[1]", "Std vs Z_{drift} (200 ns);Z_{drift} (mm);std (ADC count)");
	Graphic_setup(v_std1[1], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[1], 3, 20, kAfter,	1, kBlack);
	v_std1[1]->				Draw("ap");
	v_std2[1]->				Draw("p same");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());
	leg->Clear();




	// Z 412
	std::vector<TF1*>				v_1_z412;
	std::vector<TF1*>				v_2_z412;
	npoint = 9;
	for(int i = 0; i < 6; i++){
	v_reso1.	clear();
	v_reso2.	clear();
	v_mean1.	clear();
	v_mean2.	clear();
	v_std1.	clear();
	v_std2.	clear();
	}
	for(int i = -1; i < npoint; i++){
	TFile* file1;
	TFile* file2;
	if(i == -1){
		file1		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT412/DESY21_zm40_PT412/3_DESY21_zm40_PT412_dEdx" + Comment1 + ".root"));
		file2		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT412/DESY21_zm40_PT412/3_DESY21_zm40_PT412_dEdx" + Comment2 + ".root"));
	}
	else{
		file1		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT412/DESY21_z" + i + "60_PT412/3_DESY21_z" + i + "60_PT412_dEdx" + Comment1 + ".root"));
		file2		= TFile::Open(TString(inputDir + "DESY21_zscan/DESY21_zscan_PT412/DESY21_z" + i + "60_PT412/3_DESY21_z" + i + "60_PT412_dEdx" + Comment2 + ".root"));
	}
	v_1_z412.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_z412.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_z412);
	arr_2.push_back(v_2_z412);


	ipoint = 0;
	for(int i = 0; i <= npoint; i++){
	mean_1[i]				= arr_1[2][i]->	GetParameter(1);
	mean_2[i]				= arr_2[2][i]->	GetParameter(1);
	std_1[i]					= arr_1[2][i]->	GetParameter(2);
	std_2[i]					= arr_2[2][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[2][i]->	GetParError(1);
	dmean_2[i]				= arr_2[2][i]->	GetParError(1);
	dstd_1[i]				= arr_1[2][i]->	GetParError(2);
	dstd_2[i]				= arr_2[2][i]->	GetParError(2);

	v_reso1[2]->	SetPoint		(ipoint, i*100+50, std_1[i]/mean_1[i]*100);
	v_reso2[2]->	SetPoint		(ipoint, i*100+50, std_2[i]/mean_2[i]*100);
	v_reso1[2]->	SetPointError (ipoint, 0,			GetResoError(arr_1[2][i]));
	v_reso2[2]->	SetPointError (ipoint, 0,			GetResoError(arr_2[2][i]));

	v_mean1[2]->	SetPoint		(ipoint, i*100+50, mean_1[i]);
	v_mean2[2]->	SetPoint		(ipoint, i*100+50, mean_2[i]);
	v_mean1[2]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[2]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[2]->	SetPoint		(ipoint, i*100+50, std_1[i]);
	v_std2[2]->	SetPoint		(ipoint, i*100+50, std_2[i]);
	v_std1[2]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[2]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[2]->				GetXaxis()->SetLimits(0, 1000);
	v_reso1[2]->				SetMinimum(4);
	v_reso1[2]->				SetMaximum(12);
	v_reso1[2]->				SetNameTitle("v_reso1[2]", "Resolution vs Z_{drift} (412 ns);Z_{drift} (mm);resolution (%)");
	Graphic_setup(v_reso1[2], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[2], 3, 20, kAfter,	1, kBlack);
	v_reso1[2]->				Draw("ap");
	v_reso2[2]->				Draw("p same");
	leg->						AddEntry(v_reso1[2], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[2], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Mean
	v_mean1[2]->				GetXaxis()->SetLimits(0, 1000);
	v_mean1[2]->				SetMinimum(600);
	v_mean1[2]->				SetMaximum(1200);
	v_mean1[2]->				SetNameTitle("v_mean1[2]", "Mean vs Z_{drift} (412 ns);Z_{drift} (mm);Mean (ADC count)");
	Graphic_setup(v_mean1[2], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[2], 3, 20, kAfter,	1, kBlack);
	v_mean1[2]->				Draw("ap");
	v_mean2[2]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[2]->				GetXaxis()->SetLimits(0, 1000);
	v_std1[2]->				SetMinimum(40);
	v_std1[2]->				SetMaximum(121);
	v_std1[2]->				SetNameTitle("v_std1[2]", "Std vs Z_{drift} (412 ns);Z_{drift} (mm);std (ADC count)");
	Graphic_setup(v_std1[2], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[2], 3, 20, kAfter,	1, kBlack);
	v_std1[2]->				Draw("ap");
	v_std2[2]->				Draw("p same"); 
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());
	leg->Clear();





	// phi 50
	std::vector<TF1*>				v_1_phi50;
	std::vector<TF1*>				v_2_phi50;
	npoint = 8;
	for(int i = 0; i < 6; i++){
	v_reso1.	clear();
	v_reso2.	clear();
	v_mean1.	clear();
	v_mean2.	clear();
	v_std1.	clear();
	v_std2.	clear();
	}
	int phi_arr[] = {0, 5, 10, 20, 30, 30, 40, 45};
	for(int i = 0; i < npoint; i++){
	TFile* file1;
	TFile* file2;
	if(i == 0)		file1	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_zm40_PT200/3_DESY21_zm40_PT200_dEdx" + Comment1 + ".root"));
	else if(i < 5)	file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_zm40/DESY21_phi" + phi_arr[i] + "_zm40/3_DESY21_phi" + phi_arr[i] + "_zm40_dEdx" + Comment1 + ".root"));
	else			file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_zm40/DESY21_phi" + phi_arr[i] + "_diag_zm40/3_DESY21_phi" + phi_arr[i] + "_diag_zm40_dEdx" + Comment1 + ".root"));
	if(i == 0)		file2	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_zm40_PT200/3_DESY21_zm40_PT200_dEdx" + Comment2 + ".root"));
	else if(i < 5)	file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_zm40/DESY21_phi" + phi_arr[i] + "_zm40/3_DESY21_phi" + phi_arr[i] + "_zm40_dEdx" + Comment2 + ".root"));
	else			file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_zm40/DESY21_phi" + phi_arr[i] + "_diag_zm40/3_DESY21_phi" + phi_arr[i] + "_diag_zm40_dEdx" + Comment2 + ".root"));
	v_1_phi50.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_phi50.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_phi50);
	arr_2.push_back(v_2_phi50);

	ipoint = 0;
	int phi_arr_shift[] = {0, 5, 10, 20, 29, 31, 40, 45};
	for(int i = 0; i < npoint; i++){
	mean_1[i]				= arr_1[3][i]->	GetParameter(1);
	mean_2[i]				= arr_2[3][i]->	GetParameter(1);
	std_1[i]					= arr_1[3][i]->	GetParameter(2);
	std_2[i]					= arr_2[3][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[3][i]->	GetParError(1);
	dmean_2[i]				= arr_2[3][i]->	GetParError(1);
	dstd_1[i]				= arr_1[3][i]->	GetParError(2);
	dstd_2[i]				= arr_2[3][i]->	GetParError(2);

	v_reso1[3]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]/mean_1[i]*100);
	v_reso2[3]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]/mean_2[i]*100);
	v_reso1[3]->	SetPointError (ipoint, 0,			GetResoError(arr_1[3][i]));
	v_reso2[3]->	SetPointError (ipoint, 0,			GetResoError(arr_2[3][i]));

	v_mean1[3]->	SetPoint		(ipoint, phi_arr_shift[i], mean_1[i]);
	v_mean2[3]->	SetPoint		(ipoint, phi_arr_shift[i], mean_2[i]);
	v_mean1[3]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[3]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[3]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]);
	v_std2[3]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]);
	v_std1[3]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[3]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[3]->				GetXaxis()->SetLimits(-3, 48);
	v_reso1[3]->				SetMinimum(4);
	v_reso1[3]->				SetMaximum(12);
	v_reso1[3]->				SetNameTitle("v_reso1[3]", "Resolution vs #varphi angle (Z_{drift} = 50 mm);#varphi angle (#circ);resolution (%)");
	Graphic_setup(v_reso1[3], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[3], 3, 20, kAfter,	1, kBlack);
	v_reso1[3]->				Draw("ap");
	v_reso2[3]->				Draw("p same");
	leg->						AddEntry(v_reso1[3], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[3], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Mean
	v_mean1[3]->				GetXaxis()->SetLimits(-3, 48);
	v_mean1[3]->				SetMinimum(600);
	v_mean1[3]->				SetMaximum(1200);
	v_mean1[3]->				SetNameTitle("v_mean1[3]", "Mean vs #varphi angle (Z_{drift} = 50 mm);#varphi angle (#circ);Mean (ADC count)");
	Graphic_setup(v_mean1[3], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[3], 3, 20, kAfter,	1, kBlack);
	v_mean1[3]->				Draw("ap");
	v_mean2[3]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[3]->				GetXaxis()->SetLimits(-3, 48);
	v_std1[3]->				SetMinimum(40);
	v_std1[3]->				SetMaximum(121);
	v_std1[3]->				SetNameTitle("v_std1[3]", "Std vs #varphi angle (Z_{drift} = 50 mm);#varphi angle (#circ);std (ADC count)");
	Graphic_setup(v_std1[3], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[3], 3, 20, kAfter,	1, kBlack);
	v_std1[3]->				Draw("ap");
	v_std2[3]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());
	leg->Clear();






	// phi 550
	std::vector<TF1*>				v_1_phi550;
	std::vector<TF1*>				v_2_phi550;
	npoint = 8;
	for(int i = 0; i < 6; i++){
	v_reso1.	clear();
	v_reso2.	clear();
	v_mean1.	clear();
	v_mean2.	clear();
	v_std1.	clear();
	v_std2.	clear();
	}
	for(int i = 0; i < npoint; i++){
	TFile* file1;
	TFile* file2;
	if(i == 0)		file1	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment1 + ".root"));
	else if(i < 5)	file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z460/DESY21_phi" + phi_arr[i] + "_z460/3_DESY21_phi" + phi_arr[i] + "_z460_dEdx" + Comment1 + ".root"));
	else			file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z460/DESY21_phi" + phi_arr[i] + "_diag_z460/3_DESY21_phi" + phi_arr[i] + "_diag_z460_dEdx" + Comment1 + ".root"));
	if(i == 0)		file2	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment2 + ".root"));
	else if(i < 5)	file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z460/DESY21_phi" + phi_arr[i] + "_z460/3_DESY21_phi" + phi_arr[i] + "_z460_dEdx" + Comment2 + ".root"));
	else			file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z460/DESY21_phi" + phi_arr[i] + "_diag_z460/3_DESY21_phi" + phi_arr[i] + "_diag_z460_dEdx" + Comment2 + ".root"));
	v_1_phi550.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_phi550.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_phi550);
	arr_2.push_back(v_2_phi550);

	ipoint = 0;
	for(int i = 0; i < npoint; i++){
	mean_1[i]				= arr_1[4][i]->	GetParameter(1);
	mean_2[i]				= arr_2[4][i]->	GetParameter(1);
	std_1[i]					= arr_1[4][i]->	GetParameter(2);
	std_2[i]					= arr_2[4][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[4][i]->	GetParError(1);
	dmean_2[i]				= arr_2[4][i]->	GetParError(1);
	dstd_1[i]				= arr_1[4][i]->	GetParError(2);
	dstd_2[i]				= arr_2[4][i]->	GetParError(2);

	v_reso1[4]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]/mean_1[i]*100);
	v_reso2[4]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]/mean_2[i]*100);
	v_reso1[4]->	SetPointError (ipoint, 0,			GetResoError(arr_1[4][i]));
	v_reso2[4]->	SetPointError (ipoint, 0,			GetResoError(arr_2[4][i]));

	v_mean1[4]->	SetPoint		(ipoint, phi_arr_shift[i], mean_1[i]);
	v_mean2[4]->	SetPoint		(ipoint, phi_arr_shift[i], mean_2[i]);
	v_mean1[4]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[4]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[4]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]);
	v_std2[4]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]);
	v_std1[4]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[4]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[4]->				GetXaxis()->SetLimits(-3, 48);
	v_reso1[4]->				SetMinimum(4);
	v_reso1[4]->				SetMaximum(12);
	v_reso1[4]->				SetNameTitle("v_reso1[4]", "Resolution vs #varphi angle (Z_{drift} = 550 mm);#varphi angle (#circ);resolution (%)");
	Graphic_setup(v_reso1[4], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[4], 3, 20, kAfter,	1, kBlack);
	v_reso1[4]->				Draw("ap");
	v_reso2[4]->				Draw("p same");
	leg->						AddEntry(v_reso1[4], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[4], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Mean
	v_mean1[4]->				GetXaxis()->SetLimits(-3, 48);
	v_mean1[4]->				SetMinimum(600);
	v_mean1[4]->				SetMaximum(1200);
	v_mean1[4]->				SetNameTitle("v_mean1[4]", "Mean vs #varphi angle (Z_{drift} = 550 mm);#varphi angle (#circ);Mean (ADC count)");
	Graphic_setup(v_mean1[4], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[4], 3, 20, kAfter,	1, kBlack);
	v_mean1[4]->				Draw("ap");
	v_mean2[4]->				Draw("p same"); 
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[4]->				GetXaxis()->SetLimits(-3, 48);
	v_std1[4]->				SetMinimum(550);
	v_std1[4]->				SetMaximum(121);
	v_std1[4]->				SetNameTitle("v_std1[4]", "Std vs #varphi angle (Z_{drift} = 550 mm);#varphi angle (#circ);std (ADC count)");
	Graphic_setup(v_std1[4], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[4], 3, 20, kAfter,	1, kBlack);
	v_std1[4]->				Draw("ap");
	v_std2[4]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());
	leg->Clear();






	// phi 950
	std::vector<TF1*>				v_1_phi950;
	std::vector<TF1*>				v_2_phi950;
	npoint = 8;
	for(int i = 0; i < 6; i++){
	v_reso1.	clear();
	v_reso2.	clear();
	v_mean1.	clear();
	v_mean2.	clear();
	v_std1.	clear();
	v_std2.	clear();
	}
	for(int i = 0; i < npoint; i++){
	TFile* file1;
	TFile* file2;
	if(i == 0)		file1	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_z860_PT200/3_DESY21_z860_PT200_dEdx" + Comment1 + ".root"));
	else if(i < 5)	file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z860/DESY21_phi" + phi_arr[i] + "_z860/3_DESY21_phi" + phi_arr[i] + "_z860_dEdx" + Comment1 + ".root"));
	else			file1	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z860/DESY21_phi" + phi_arr[i] + "_diag_z860/3_DESY21_phi" + phi_arr[i] + "_diag_z860_dEdx" + Comment1 + ".root"));
	if(i == 0)		file2	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT200/DESY21_z860_PT200/3_DESY21_z860_PT200_dEdx" + Comment2 + ".root"));
	else if(i < 5)	file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z860/DESY21_phi" + phi_arr[i] + "_z860/3_DESY21_phi" + phi_arr[i] + "_z860_dEdx" + Comment2 + ".root"));
	else			file2	= TFile::Open(TString(inputDir + "DESY21_phi/DESY21_phi_z860/DESY21_phi" + phi_arr[i] + "_diag_z860/3_DESY21_phi" + phi_arr[i] + "_diag_z860_dEdx" + Comment2 + ".root"));
	v_1_phi950.						push_back(file1->Get<TF1>(method.c_str()));
	v_2_phi950.						push_back(file2->Get<TF1>(method.c_str()));
	delete file1;
	delete file2;
	}
	arr_1.push_back(v_1_phi950);
	arr_2.push_back(v_2_phi950);

	ipoint = 0;
	for(int i = 0; i < npoint; i++){
	mean_1[i]				= arr_1[5][i]->	GetParameter(1);
	mean_2[i]				= arr_2[5][i]->	GetParameter(1);
	std_1[i]					= arr_1[5][i]->	GetParameter(2);
	std_2[i]					= arr_2[5][i]->	GetParameter(2);
	dmean_1[i]				= arr_1[5][i]->	GetParError(1);
	dmean_2[i]				= arr_2[5][i]->	GetParError(1);
	dstd_1[i]				= arr_1[5][i]->	GetParError(2);
	dstd_2[i]				= arr_2[5][i]->	GetParError(2);

	v_reso1[5]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]/mean_1[i]*100);
	v_reso2[5]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]/mean_2[i]*100);
	v_reso1[5]->	SetPointError (ipoint, 0,			GetResoError(arr_1[5][i]));
	v_reso2[5]->	SetPointError (ipoint, 0,			GetResoError(arr_2[5][i]));

	v_mean1[5]->	SetPoint		(ipoint, phi_arr_shift[i], mean_1[i]);
	v_mean2[5]->	SetPoint		(ipoint, phi_arr_shift[i], mean_2[i]);
	v_mean1[5]->	SetPointError (ipoint, 0,			dmean_1[i]);
	v_mean2[5]->	SetPointError (ipoint, 0,			dmean_2[i]);

	v_std1[5]->	SetPoint		(ipoint, phi_arr_shift[i], std_1[i]);
	v_std2[5]->	SetPoint		(ipoint, phi_arr_shift[i], std_2[i]);
	v_std1[5]->	SetPointError	(ipoint, 0,			dstd_1[i]);
	v_std2[5]->	SetPointError	(ipoint, 0,			dstd_2[i]);

	ipoint++;
	}

	// Resolution
	v_reso1[5]->				GetXaxis()->SetLimits(-3, 48);
	v_reso1[5]->				SetMinimum(4);
	v_reso1[5]->				SetMaximum(12);
	v_reso1[5]->				SetNameTitle("v_reso1[5]", "Resolution vs #varphi angle (Z_{drift} = 950 mm);#varphi angle (#circ);resolution (%)");
	Graphic_setup(v_reso1[5], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_reso2[5], 3, 20, kAfter,	1, kBlack);
	v_reso1[5]->				Draw("ap");
	v_reso2[5]->				Draw("p same");
	leg->						AddEntry(v_reso1[5], leg1.c_str(), "ep"); 
	leg->						AddEntry(v_reso2[5], leg2.c_str(), "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Mean
	v_mean1[5]->				GetXaxis()->SetLimits(-3, 48);
	v_mean1[5]->				SetMinimum(600);
	v_mean1[5]->				SetMaximum(1200);
	v_mean1[5]->				SetNameTitle("v_mean1[5]", "Mean vs #varphi angle (Z_{drift} = 950 mm);#varphi angle (#circ);Mean (ADC count)");
	Graphic_setup(v_mean1[5], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_mean2[5], 3, 20, kAfter,	1, kBlack);
	v_mean1[5]->				Draw("ap");
	v_mean2[5]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Std
	v_std1[5]->				GetXaxis()->SetLimits(-3, 48);
	v_std1[5]->				SetMinimum(950);
	v_std1[5]->				SetMaximum(121);
	v_std1[5]->				SetNameTitle("v_std1[5]", "Std vs #varphi angle (Z_{drift} = 950 mm);#varphi angle (#circ);std (ADC count)");
	Graphic_setup(v_std1[5], 3, 21, kBefore, 1, kBlack);
	Graphic_setup(v_std2[5], 3, 20, kAfter,	1, kBlack);
	v_std1[5]->				Draw("ap");
	v_std2[5]->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







void DrawOut_Separation(const std::string& inputDir, const std::string& Comment)
{
	int nParticles				= 4;

	// Get input files
	std::vector<TFile*>		v_pTFile_dEdx;
	v_pTFile_dEdx	.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_e+_1GeV"	+ "/" + "3_CERN22_ERAM18_e+_1GeV"	+ "_dEdx" + Comment + ".root")));
	v_pTFile_dEdx	.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_p_1GeV"		+ "/" + "3_CERN22_ERAM18_p_1GeV"		+ "_dEdx" + Comment + ".root")));
	v_pTFile_dEdx	.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_mu+_1GeV"	+ "/" + "3_CERN22_ERAM18_mu+_1GeV"	+ "_dEdx" + Comment + ".root")));
	v_pTFile_dEdx	.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_pi+_0p5GeV"	+ "/" + "3_CERN22_ERAM18_pi+_0p5GeV"	+ "_dEdx" + Comment + ".root")));

	// Get functions
	std::vector<TH1F*>		v_h1f_Qtrunc;
	std::vector<TH1F*>		v_h1f_WFsum;
	std::vector<TH1F*>		v_h1f_GPv3;
	std::vector<TH1F*>		v_h1f_GPv6;
	std::vector<TH1F*>		v_h1f_XP;
	std::vector<TF1*>		v_tf1_Qtrunc;
	std::vector<TF1*>		v_tf1_WFsum;
	std::vector<TF1*>		v_tf1_GPv3;
	std::vector<TF1*>		v_tf1_GPv6;
	std::vector<TF1*>		v_tf1_XP;
	for(int iPar = 0; iPar < nParticles; iPar++){
	v_h1f_Qtrunc			.push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_Qtrunc_0"));
	v_h1f_WFsum		.push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_WFsum"));
	v_h1f_GPv3		.push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_GPv3_0"));
	v_h1f_GPv6		.push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_GPv6_0"));
	v_h1f_XP		.push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_XP_0"));
	v_tf1_Qtrunc			.push_back(v_h1f_Qtrunc[iPar]->GetFunction("tf1_Qtrunc_0"));
	v_tf1_WFsum			.push_back(v_h1f_WFsum[iPar]->GetFunction("tf1_WFsum"));
	v_tf1_GPv3			.push_back(v_h1f_GPv3[iPar]->GetFunction("tf1_GPv3_0"));
	v_tf1_GPv6			.push_back(v_h1f_GPv6[iPar]->GetFunction("tf1_GPv6_0"));
	v_tf1_XP			.push_back(v_h1f_XP[iPar]->GetFunction("tf1_XP"));
	}


	// Get mean & std
	float mean_Qtrunc[4];
	float mean_WFsum[4];
	float mean_GPv3[4];
	float mean_GPv6[4];
	float mean_XP[4];
	float dmean_Qtrunc[4];
	float dmean_WFsum[4];
	float dmean_GPv3[4];
	float dmean_GPv6[4];
	float dmean_XP[4];
	float std_Qtrunc[4];
	float std_WFsum[4];
	float std_GPv3[4];
	float std_GPv6[4];
	float std_XP[4];
	float dstd_Qtrunc[4];
	float dstd_WFsum[4];
	float dstd_GPv3[4];
	float dstd_GPv6[4];
	float dstd_XP[4];
	for(int iPar = 0; iPar < nParticles; iPar++){
	mean_Qtrunc[iPar]		= v_tf1_Qtrunc[iPar]->	GetParameter(1);
	mean_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParameter(1);
	mean_GPv3[iPar]		= v_tf1_GPv3[iPar]->	GetParameter(1);
	mean_GPv6[iPar]		= v_tf1_GPv6[iPar]->	GetParameter(1);
	mean_XP[iPar]		= v_tf1_XP[iPar]->	GetParameter(1);
	dmean_Qtrunc[iPar]		= v_tf1_Qtrunc[iPar]->	GetParError(1);
	dmean_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParError(1);
	dmean_GPv3[iPar]		= v_tf1_GPv3[iPar]->	GetParError(1);
	dmean_GPv6[iPar]		= v_tf1_GPv6[iPar]->	GetParError(1);
	dmean_XP[iPar]		= v_tf1_XP[iPar]->	GetParError(1);
	std_Qtrunc[iPar]			= v_tf1_Qtrunc[iPar]->	GetParameter(2);
	std_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParameter(2);
	std_GPv3[iPar]		= v_tf1_GPv3[iPar]->	GetParameter(2);
	std_GPv6[iPar]		= v_tf1_GPv6[iPar]->	GetParameter(2);
	std_XP[iPar]		= v_tf1_XP[iPar]->	GetParameter(2);
	dstd_Qtrunc[iPar]			= v_tf1_Qtrunc[iPar]->	GetParError(2);
	dstd_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParError(2);
	dstd_GPv3[iPar]		= v_tf1_GPv3[iPar]->	GetParError(2);
	dstd_GPv6[iPar]		= v_tf1_GPv6[iPar]->	GetParError(2);
	dstd_XP[iPar]		= v_tf1_XP[iPar]->	GetParError(2);
	}

	// Do the table
	TH2F* h2_separation		= new TH2F("h2_separation",	"Separation power",		6, -0.5, 5.5, 6, -0.5, 5.5);
	TH2F* h2_err_separation	= new TH2F("h2_err_separation", "Error on separation power", 6, -0.5, 5.5, 6, -0.5, 5.5);
	const char* particles[6]	= {"e^{+}-p","e^{+}-#mu^{+}","e^{+}-#pi^{+}", "p-#mu^{+}", "p-#pi^{+}", "#mu^{+}-#pi^{+}"};
	const char* methods[5]		= {"Q", "WF", "GPv3", "GPv6", "XP"};
	
	// histogram of values
	int k = 0;
	for(int i = 0; i < nParticles; i++){
	for(int j = i+1; j < nParticles; j++){
		h2_separation		->Fill(methods[0],	particles[k], std::floor(GetSeparation(mean_Qtrunc[i],	std_Qtrunc[i],		mean_Qtrunc[j],	std_Qtrunc[j])*100)/100);
		h2_separation		->Fill(methods[1],	particles[k], std::floor(GetSeparation(mean_WFsum[i],	std_WFsum[i],	mean_WFsum[j],	std_WFsum[j])*100)/100);
		h2_separation		->Fill(methods[2],	particles[k], std::floor(GetSeparation(mean_GPv3[i],	std_GPv3[i],	mean_GPv3[j],	std_GPv3[j])*100)/100);
		h2_separation		->Fill(methods[3],	particles[k], std::floor(GetSeparation(mean_GPv6[i],	std_GPv6[i],	mean_GPv6[j],	std_GPv6[j])*100)/100);
		h2_separation		->Fill(methods[4],	particles[k], std::floor(GetSeparation(mean_XP[i],	std_XP[i],	mean_XP[j],	std_XP[j])*100)/100);
		k++;
	}
	}
	
	// histogram of errors
	int dk = 0;
	for(int i = 0; i < nParticles; i++){
	for(int j = i+1; j < nParticles; j++){
		h2_err_separation	->Fill(methods[0],	particles[dk], std::floor(GetSeparationError(mean_Qtrunc[i],	std_Qtrunc[i],	dmean_Qtrunc[i],	dstd_Qtrunc[i],	mean_Qtrunc[j],	std_Qtrunc[j],	dmean_Qtrunc[j],	dstd_Qtrunc[j])	*100)/100);
		h2_err_separation	->Fill(methods[1],	particles[dk], std::floor(GetSeparationError(mean_WFsum[i], std_WFsum[i], dmean_WFsum[i], dstd_WFsum[i],	mean_WFsum[j],	std_WFsum[j], dmean_WFsum[j],	dstd_WFsum[j]) *100)/100);
		h2_err_separation	->Fill(methods[2],	particles[dk], std::floor(GetSeparationError(mean_GPv3[i], std_GPv3[i], dmean_GPv3[i], dstd_GPv3[i],	mean_GPv3[j],	std_GPv3[j], dmean_GPv3[j],	dstd_GPv3[j]) *100)/100);
		h2_err_separation	->Fill(methods[3],	particles[dk], std::floor(GetSeparationError(mean_GPv6[i], std_GPv6[i], dmean_GPv6[i], dstd_GPv6[i],	mean_GPv6[j],	std_GPv6[j], dmean_GPv6[j],	dstd_GPv6[j]) *100)/100);
		h2_err_separation	->Fill(methods[4],	particles[dk], std::floor(GetSeparationError(mean_XP[i], std_XP[i], dmean_XP[i], dstd_XP[i],	mean_XP[j],	std_XP[j], dmean_XP[j],	dstd_XP[j]) *100)/100);
		dk++;
	}
	}

	// Draw
	std::string OutputFile		= inputDir + "Separation_power" + Comment +".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	TLegend* leg				= new TLegend(0.64,0.15,0.84,0.29); 
	pTCanvas->					cd();
	gStyle->						SetOptStat(0);

	// Separation power
	h2_separation->			SetMarkerSize(2.4);
	h2_err_separation->		SetMarkerSize(1.4);
	h2_separation->			GetXaxis()->SetLabelOffset(0.01);
	h2_separation->			SetLabelSize(0.06, "X");
	h2_separation->			SetLabelSize(0.06, "Y");
	h2_separation->			Draw("colz");
	h2_separation->			SetBarOffset(0.05);
	h2_separation->			Draw("TEXT SAME");
	h2_err_separation->		SetBarOffset(-0.25);
	h2_err_separation->		Draw("text same");
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());
	pTCanvas->					Clear();

	// Comparing particles dE/dx method by method
	gStyle->						SetOptStat(11);
	gStyle->						SetOptFit(111);
	// v_h1f_Qtrunc[0]->			SetName("positrons e^{+} 1GeV");
	// v_h1f_Qtrunc[0]->			Scale(1/v_h1f_Qtrunc[0]->GetEntries());
	// v_tf1_Qtrunc[0]->			SetRange(0, 1500);
	// v_tf1_Qtrunc[0]->			SetParameter(0, v_tf1_Qtrunc[0]->Integral(0, 1500)/v_h1f_Qtrunc[0]->GetEntries());
	// v_h1f_Qtrunc[0]->			SetLineColor(kBlue+3);
	// v_h1f_Qtrunc[0]->			SetLineWidth(2);
	// v_tf1_Qtrunc[0]->			SetLineColor(kBlue-6);
	// v_tf1_Qtrunc[0]->			SetLineWidth(2);
	// v_h1f_Qtrunc[2]->			SetName("muons #mu^{+} 1GeV");
	// v_h1f_Qtrunc[2]->			Scale(1/v_h1f_Qtrunc[2]->GetEntries());
	// v_tf1_Qtrunc[2]->			SetRange(0, 1500);
	// v_tf1_Qtrunc[2]->			SetParameter(0, v_tf1_Qtrunc[2]->Integral(0, 1500)/v_h1f_Qtrunc[2]->GetEntries());
	// v_h1f_Qtrunc[2]->			SetLineColor(kBlue+3);
	// v_h1f_Qtrunc[2]->			SetLineWidth(2);
	// v_h1f_Qtrunc[2]->			SetLineStyle(2);
	// v_tf1_Qtrunc[2]->			SetLineColor(kBlue-6);
	// v_tf1_Qtrunc[2]->			SetLineWidth(2);
	// if(v_h1f_Qtrunc[0]->GetMaximum() < v_h1f_Qtrunc[2]->GetMaximum()) v_h1f_Qtrunc[0]->SetAxisRange(0, 1.1*v_h1f_Qtrunc[2]->GetMaximum(),	"Y");
	// v_h1f_Qtrunc[0]->			Draw("hist");
	// v_tf1_Qtrunc[0]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_Qtrunc[0], 0.64, 0.89, 0.34,	0.32);	
	// v_h1f_Qtrunc[2]->			Draw("hist sames");
	// v_tf1_Qtrunc[2]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_Qtrunc[2], 0.64, 0.89, 0.64,	0.89);	
	// pTCanvas->					Update();
	// leg->						AddEntry(v_h1f_Qtrunc[2], "#mu^{+} 1GeV ", "l");	
	// leg->						AddEntry(v_h1f_Qtrunc[0], "e^{+} 1GeV ", "l");	
	// leg->						DrawClone();
	// pTCanvas->					SaveAs(OutputFile.c_str());
	// pTCanvas->					Clear();
	// leg->						Clear();

	v_h1f_WFsum[0]->			SetName("positrons e^{+} 1GeV");
	v_h1f_WFsum[0]->			Scale(1/v_h1f_WFsum[0]->GetEntries());
	v_tf1_WFsum[0]->			SetRange(0, 1500);
	v_tf1_WFsum[0]->			SetParameter(0, v_tf1_WFsum[0]->Integral(0, 1500)/v_h1f_WFsum[0]->GetEntries());
	v_h1f_WFsum[0]->			SetLineColor(kCyan+2);
	v_h1f_WFsum[0]->			SetLineWidth(2);
	v_tf1_WFsum[0]->			SetLineColor(kCyan+1);
	v_tf1_WFsum[0]->			SetLineWidth(2);
	v_h1f_WFsum[2]->			SetName("muons #mu^{+} 1GeV");
	v_h1f_WFsum[2]->			Scale(1/v_h1f_WFsum[2]->GetEntries());
	v_tf1_WFsum[2]->			SetRange(0, 1500);
	v_tf1_WFsum[2]->			SetParameter(0, v_tf1_WFsum[2]->Integral(0, 1500)/v_h1f_WFsum[2]->GetEntries());
	v_h1f_WFsum[2]->			SetLineColor(kCyan+2);
	v_h1f_WFsum[2]->			SetLineWidth(2);
	v_h1f_WFsum[2]->			SetLineStyle(2);
	v_tf1_WFsum[2]->			SetLineColor(kCyan+1);
	v_tf1_WFsum[2]->			SetLineWidth(2);
	if(v_h1f_WFsum[0]->GetMaximum() < v_h1f_WFsum[2]->GetMaximum()) v_h1f_WFsum[0]->SetAxisRange(0, 1.1*v_h1f_WFsum[2]->GetMaximum(),	"Y");
	v_h1f_WFsum[0]->			Draw("hist");
	v_tf1_WFsum[0]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_h1f_WFsum[0], 0.64, 0.89, 0.64,	0.89);	
	v_h1f_WFsum[2]->			Draw("hist sames");
	v_tf1_WFsum[2]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_h1f_WFsum[2], 0.64, 0.89, 0.39,	0.64);	
	pTCanvas->					Update();
	leg->						AddEntry(v_h1f_WFsum[2], "#mu^{+} 1GeV ", "l");	
	leg->						AddEntry(v_h1f_WFsum[0], "e^{+} 1GeV ", "l");	
	leg->						DrawClone();
	TLatex* ptText		= new TLatex;
	ptText->SetTextSize(0.05);
	ptText->SetTextFont(42);
	ptText->SetTextAlign(31);
	float sepa			= GetSeparation(mean_WFsum[0],	std_WFsum[0],		mean_WFsum[2],	std_WFsum[2]);
	float dsepa			= GetSeparationError(mean_WFsum[0], std_WFsum[0], dmean_WFsum[0], dstd_WFsum[0], mean_WFsum[2], std_WFsum[2], dmean_WFsum[2], dstd_WFsum[2]);
	ptText->SetText(1200, 0.19, Form("S(e^{+} | #mu^{+}) = %.2f #pm %.2f #sigma", sepa, dsepa));
	ptText->SetTextColor(kCyan+2);
	ptText->DrawClone();
	delete ptText;
	pTCanvas->					SaveAs(OutputFile.c_str());
	pTCanvas->					Clear();
	leg->						Clear();

	// v_h1f_GPv3[0]->			SetName("positrons e^{+} 1GeV");
	// v_h1f_GPv3[0]->			Scale(1/v_h1f_GPv3[0]->GetEntries());
	// v_tf1_GPv3[0]->			SetRange(0, 1500);
	// v_tf1_GPv3[0]->			SetParameter(0, v_tf1_GPv3[0]->Integral(0, 1500)/v_h1f_GPv3[0]->GetEntries());
	// v_h1f_GPv3[0]->			SetLineColor(kMagenta+3);
	// v_h1f_GPv3[0]->			SetLineWidth(2);
	// v_tf1_GPv3[0]->			SetLineColor(kMagenta-6);
	// v_tf1_GPv3[0]->			SetLineWidth(2);
	// v_h1f_GPv3[2]->			SetName("muons #mu^{+} 1GeV");
	// v_h1f_GPv3[2]->			Scale(1/v_h1f_GPv3[2]->GetEntries());
	// v_tf1_GPv3[2]->			SetRange(0, 1500);
	// v_tf1_GPv3[2]->			SetParameter(0, v_tf1_GPv3[2]->Integral(0, 1500)/v_h1f_GPv3[2]->GetEntries());
	// v_h1f_GPv3[2]->			SetLineColor(kMagenta+3);
	// v_h1f_GPv3[2]->			SetLineWidth(2);
	// v_h1f_GPv3[2]->			SetLineStyle(2);
	// v_tf1_GPv3[2]->			SetLineColor(kMagenta-6);
	// v_tf1_GPv3[2]->			SetLineWidth(2);
	// if(v_h1f_GPv3[0]->GetMaximum() < v_h1f_GPv3[2]->GetMaximum()) v_h1f_GPv3[0]->SetAxisRange(0, 1.1*v_h1f_GPv3[2]->GetMaximum(),	"Y");
	// v_h1f_GPv3[0]->			Draw("hist");
	// v_tf1_GPv3[0]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_GPv3[0], 0.64, 0.89, 0.34,	0.32);	
	// v_h1f_GPv3[2]->			Draw("hist sames");
	// v_tf1_GPv3[2]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_GPv3[2], 0.64, 0.89, 0.64,	0.89);	
	// pTCanvas->					Update();
	// leg->						AddEntry(v_h1f_GPv3[2], "#mu^{+} 1GeV ", "l");	
	// leg->						AddEntry(v_h1f_GPv3[0], "e^{+} 1GeV ", "l");	
	// leg->						DrawClone();
	// pTCanvas->					SaveAs(OutputFile.c_str());
	// pTCanvas->					Clear();
	// leg->						Clear();

	// v_h1f_GPv6[0]->			SetName("positrons e^{+} 1GeV");
	// v_h1f_GPv6[0]->			Scale(1/v_h1f_GPv6[0]->GetEntries());
	// v_tf1_GPv6[0]->			SetRange(0, 1500);
	// v_tf1_GPv6[0]->			SetParameter(0, v_tf1_GPv6[0]->Integral(0, 1500)/v_h1f_GPv6[0]->GetEntries());
	// v_h1f_GPv6[0]->			SetLineColor(kGreen-2);
	// v_h1f_GPv6[0]->			SetLineWidth(2);
	// v_tf1_GPv6[0]->			SetLineColor(kGreen+2);
	// v_tf1_GPv6[0]->			SetLineWidth(2);
	// v_h1f_GPv6[2]->			SetName("muons #mu^{+} 1GeV");
	// v_h1f_GPv6[2]->			Scale(1/v_h1f_GPv6[2]->GetEntries());
	// v_tf1_GPv6[2]->			SetRange(0, 1500);
	// v_tf1_GPv6[2]->			SetParameter(0, v_tf1_GPv6[2]->Integral(0, 1500)/v_h1f_GPv6[2]->GetEntries());
	// v_h1f_GPv6[2]->			SetLineColor(kGreen-2);
	// v_h1f_GPv6[2]->			SetLineWidth(2);
	// v_h1f_GPv6[2]->			SetLineStyle(2);
	// v_tf1_GPv6[2]->			SetLineColor(kGreen+2);
	// v_tf1_GPv6[2]->			SetLineWidth(2);
	// if(v_h1f_GPv6[0]->GetMaximum() < v_h1f_GPv6[2]->GetMaximum()) v_h1f_GPv6[0]->SetAxisRange(0, 1.1*v_h1f_GPv6[2]->GetMaximum(),	"Y");
	// v_h1f_GPv6[0]->			Draw("hist");
	// v_tf1_GPv6[0]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_GPv6[0], 0.64, 0.89, 0.34,	0.32);	
	// v_h1f_GPv6[2]->			Draw("hist sames");
	// v_tf1_GPv6[2]->			Draw("same");
	// gPad->						Update();
	// SetStatBoxPosition(v_h1f_GPv6[2], 0.64, 0.89, 0.64,	0.89);	
	// pTCanvas->					Update();
	// leg->						AddEntry(v_h1f_GPv6[2], "#mu^{+} 1GeV ", "l");	
	// leg->						AddEntry(v_h1f_GPv6[0], "e^{+} 1GeV ", "l");	
	// leg->						DrawClone();
	// pTCanvas->					SaveAs(OutputFile.c_str());
	// pTCanvas->					Clear();
	// leg->						Clear();

	v_h1f_XP[0]->			SetName("positrons e^{+} 1GeV");
	v_h1f_XP[0]->			Scale(1/v_h1f_XP[0]->GetEntries());
	v_tf1_XP[0]->			SetRange(0, 1500);
	v_tf1_XP[0]->			SetParameter(0, v_tf1_XP[0]->Integral(0, 1500)/v_h1f_XP[0]->GetEntries());
	v_h1f_XP[0]->			SetLineColor(kMagenta+2);
	v_h1f_XP[0]->			SetLineWidth(2);
	v_tf1_XP[0]->			SetLineColor(kMagenta+1);
	v_tf1_XP[0]->			SetLineWidth(2);
	v_h1f_XP[2]->			SetName("muons #mu^{+} 1GeV");
	v_h1f_XP[2]->			Scale(1/v_h1f_XP[2]->GetEntries());
	v_tf1_XP[2]->			SetRange(0, 1500);
	v_tf1_XP[2]->			SetParameter(0, v_tf1_XP[2]->Integral(0, 1500)/v_h1f_XP[2]->GetEntries());
	v_h1f_XP[2]->			SetLineColor(kMagenta+2);
	v_h1f_XP[2]->			SetLineWidth(2);
	v_h1f_XP[2]->			SetLineStyle(2);
	v_tf1_XP[2]->			SetLineColor(kMagenta+1);
	v_tf1_XP[2]->			SetLineWidth(2);
	if(v_h1f_XP[0]->GetMaximum() < v_h1f_XP[2]->GetMaximum()) v_h1f_XP[0]->SetAxisRange(0, 1.1*v_h1f_XP[2]->GetMaximum(),	"Y");
	v_h1f_XP[0]->			Draw("hist");
	v_tf1_XP[0]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_h1f_XP[0], 0.64, 0.89, 0.64,	0.89);	
	v_h1f_XP[2]->			Draw("hist sames");
	v_tf1_XP[2]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_h1f_XP[2], 0.64, 0.89, 0.39,	0.64);	
	pTCanvas->					Update();
	leg->						AddEntry(v_h1f_XP[2], "#mu^{+} 1GeV ", "l");	
	leg->						AddEntry(v_h1f_XP[0], "e^{+} 1GeV ", "l");	
	leg->						DrawClone();
	ptText->SetTextSize(0.05);
	ptText->SetTextFont(42);
	ptText->SetTextAlign(31);
	sepa			= GetSeparation(mean_XP[0],	std_XP[0],		mean_XP[2],	std_XP[2]);
	dsepa			= GetSeparationError(mean_XP[0], std_XP[0], dmean_XP[0], dstd_XP[0], mean_XP[2], std_XP[2], dmean_XP[2], dstd_XP[2]);
	ptText->SetText(1200, 0.19, Form("S(e^{+} | #mu^{+}) = %.2f #pm %.2f #sigma", sepa, dsepa));
	ptText->SetTextColor(kMagenta+2);
	ptText->DrawClone();
	delete ptText;
	pTCanvas->					SaveAs(OutputFile_End.c_str());


	// // Delete
	// delete						h2_separation;
	// delete						pTCanvas;
	// delete leg;
	
	// v_h1f_Qtrunc.clear();
	// v_h1f_WFsum.clear();
	// v_h1f_GPv3.clear();
	// v_h1f_GPv6.clear();
	// v_h1f_XP.clear();
	// v_tf1_Qtrunc.clear();
	// v_tf1_WFsum.clear();
	// v_tf1_GPv3.clear();
	// v_tf1_GPv6.clear();
	// v_tf1_XP.clear();
}








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void DrawOut_Separation_Reduced(const std::string& inputDir, const std::string& Comment, std::string Energy)
{
	int nParticles				= 4;

	// Get input files
	std::vector<TFile*>		v_pTFile_dEdx;
	// v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_e+_1GeV/3_CERN22_ERAM18_e+_1GeV_vD"	+ "_dEdx" + Comment + ".root")));
	// v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_p_1GeV/3_CERN22_ERAM18_p_1GeV_vD"		+ "_dEdx" + Comment + ".root")));
	// v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_mu+_1GeV/3_CERN22_ERAM18_mu+_1GeV_vD"	+ "_dEdx" + Comment + ".root")));
	// v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_ERAM18_pi+_0p5GeV/3_CERN22_ERAM18_pi+_0p5GeV_vD"	+ "_dEdx" + Comment + ".root")));
	v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_Escan_e+_" + Energy + "GeV/3_CERN22_Escan_e+_" + Energy + "GeV_dEdx" + Comment + ".root")));
	v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_Escan_p+_" + Energy + "GeV/3_CERN22_Escan_p+_" + Energy + "GeV_dEdx" + Comment + ".root")));
	v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_Escan_mu_" + Energy + "GeV/3_CERN22_Escan_mu_" + Energy + "GeV_dEdx" + Comment + ".root")));
	v_pTFile_dEdx.				push_back(TFile::Open(TString(inputDir + "CERN22_Escan_pi_" + Energy + "GeV/3_CERN22_Escan_pi_" + Energy + "GeV_dEdx" + Comment + ".root")));


	// Get functions
	std::vector<TF1*>		v_tf1_WFsum;
	std::vector<TF1*>		v_tf1_XP;
	std::vector<TH1F*>		v_th1_WFsum;
	std::vector<TH1F*>		v_th1_XP;
	for(int iPar = 0; iPar < nParticles; iPar++){
	v_th1_WFsum.			push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_WFsum"));
	v_th1_XP.				push_back(v_pTFile_dEdx[iPar]->Get<TH1F>("h1f_XP"));
	v_tf1_WFsum.			push_back(v_pTFile_dEdx[iPar]->Get<TF1>("tf1_WFsum"));
	v_tf1_XP.				push_back(v_pTFile_dEdx[iPar]->Get<TF1>("tf1_XP"));
	}
	int p;
	if((p = Energy.find("p"))	!= (int)std::string::npos) Energy.replace(p, 1, ".");

	// Get mean & std
	float mean_WFsum[4];
	float mean_XP[4];
	float dmean_WFsum[4];
	float dmean_XP[4];
	float std_WFsum[4];
	float std_XP[4];
	float dstd_WFsum[4];
	float dstd_XP[4];
	for(int iPar = 0; iPar < nParticles; iPar++){
	mean_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParameter(1);
	mean_XP[iPar]			= v_tf1_XP[iPar]->	GetParameter(1);
	dmean_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParError(1);
	dmean_XP[iPar]			= v_tf1_XP[iPar]->	GetParError(1);
	std_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParameter(2);
	std_XP[iPar]				= v_tf1_XP[iPar]->	GetParameter(2);
	dstd_WFsum[iPar]		= v_tf1_WFsum[iPar]->	GetParError(2);
	dstd_XP[iPar]			= v_tf1_XP[iPar]->	GetParError(2);
	}

	// Do the table
	TH2F* h2_separation		= new TH2F("h2_separation",	"Separation power",		6, -0.5, 5.5, 6, -0.5, 5.5);
	TH2F* h2_err_separation	= new TH2F("h2_err_separation", "Error on separation power", 6, -0.5, 5.5, 6, -0.5, 5.5);
	const char* particles[6]	= {"e^{+}-p","e^{+}-#mu^{+}","e^{+}-#pi^{+}", "p-#mu^{+}", "p-#pi^{+}", "#mu^{+}-#pi^{+}"};
	const char* methods[2]		= {"WF", "XP"};
	
	// histogram of values
	int k = 0;
	for(int i = 0; i < nParticles; i++){
	for(int j = i+1; j < nParticles; j++){
		h2_separation		->Fill(methods[0],	particles[k], std::floor(GetSeparation(mean_WFsum[i],	std_WFsum[i],	mean_WFsum[j],	std_WFsum[j])*100)/100);
		h2_separation		->Fill(methods[1],	particles[k], std::floor(GetSeparation(mean_XP[i],	std_XP[i],	mean_XP[j],	std_XP[j])*100)/100);
		k++;
	}
	}
	
	// histogram of errors
	int dk = 0;
	for(int i = 0; i < nParticles; i++){
	for(int j = i+1; j < nParticles; j++){
		h2_err_separation	->Fill(methods[0],	particles[dk], std::floor(GetSeparationError(mean_WFsum[i], std_WFsum[i], dmean_WFsum[i], dstd_WFsum[i],	mean_WFsum[j],	std_WFsum[j], dmean_WFsum[j],	dstd_WFsum[j]) *100)/100);
		h2_err_separation	->Fill(methods[1],	particles[dk], std::floor(GetSeparationError(mean_XP[i], std_XP[i], dmean_XP[i], dstd_XP[i],	mean_XP[j],	std_XP[j], dmean_XP[j],	dstd_XP[j]) *100)/100);
		dk++;
	}
	}

	// Draw
	std::string OutputFile		= inputDir + "Separation_power_reduced" + Comment +".png";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 650, 1200);
	pTCanvas->					cd();
	pTCanvas->SetLeftMargin(0.15);
	pTCanvas->SetRightMargin(0.11);
	gStyle->						SetOptStat(0);
	gStyle->SetTitleSize(0.1, "t");

	// Separation power
	h2_separation->			SetMarkerSize(4);
	h2_err_separation->		SetMarkerSize(2);
	h2_separation->			GetXaxis()->SetLabelOffset(0.01);
	h2_separation->			SetLabelSize(0.1, "X");
	h2_separation->			SetLabelSize(0.1, "Y");
	h2_separation->			Draw("colz");
	h2_separation->			SetBarOffset(0.05);
	h2_separation->			Draw("TEXT SAME");
	h2_err_separation->		SetBarOffset(-0.25);
	h2_err_separation->		Draw("text same");
	pTCanvas->					SaveAs(OutputFile.c_str());




	std::string OutputFile2		= inputDir + "Separation_power_emu" + Comment +".pdf";
	std::string OutputFile2_Beg	= OutputFile2 + "(";
	std::string OutputFile2_End	= OutputFile2 + ")";
	TCanvas* pTCanvas2			= new TCanvas("TCanvas2", "TCanvas2", 1800, 1200);
	TLegend* leg				= new TLegend(0.64,0.22,0.89,0.39); 
	pTCanvas2->					cd();

	// Comparing particles dE/dx method by method
	gStyle->						SetTitleSize(0.07, "t");
	gStyle->						SetOptStat(11);
	gStyle->						SetOptFit(111);


	v_th1_WFsum[0]->			SetName(TString("positrons e^{+} " + Energy + "GeV"));
	v_th1_WFsum[0]->			SetTitle(TString("dE/dx " + Energy + "GeV with WF"));
	v_th1_WFsum[0]->			Scale(1/v_th1_WFsum[0]->GetEntries());
	v_th1_WFsum[2]->			Scale(1/v_th1_WFsum[2]->GetEntries());
	v_th1_WFsum[0]->			SetMaximum(std::max({v_th1_WFsum[0]->GetMaximum(), v_th1_WFsum[2]->GetMaximum()})*1.2);
	v_tf1_WFsum[0]->			SetRange(0, 1800);
	v_tf1_WFsum[0]->			SetParameter(0, v_tf1_WFsum[0]->Integral(0, 1500)/v_th1_WFsum[0]->GetEntries());
	v_th1_WFsum[0]->			SetLineColor(kCyan+2);
	v_th1_WFsum[0]->			SetLineWidth(2);
	v_tf1_WFsum[0]->			SetLineColor(kCyan);
	v_tf1_WFsum[0]->			SetLineWidth(2);
	v_th1_WFsum[2]->			SetName(TString("muons #mu^{+} " + Energy + "GeV"));
	v_tf1_WFsum[2]->			SetRange(0, 1800);
	v_tf1_WFsum[2]->			SetParameter(0, v_tf1_WFsum[2]->Integral(0, 1500)/v_th1_WFsum[2]->GetEntries());
	v_th1_WFsum[2]->			SetLineColor(kCyan+2);
	v_th1_WFsum[2]->			SetLineWidth(2);
	v_th1_WFsum[2]->			SetLineStyle(2);
	v_tf1_WFsum[2]->			SetLineColor(kCyan);
	v_tf1_WFsum[2]->			SetLineWidth(2);
	if(v_th1_WFsum[0]->GetMaximum() < v_th1_WFsum[2]->GetMaximum()) v_th1_WFsum[0]->SetAxisRange(0, 1.1*v_th1_WFsum[2]->GetMaximum(),	"Y");
	v_th1_WFsum[0]->			Draw("hist");
	v_tf1_WFsum[0]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_th1_WFsum[0], 0.64, 0.89, 0.64,	0.89);	
	v_th1_WFsum[2]->			Draw("hist sames");
	v_tf1_WFsum[2]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_th1_WFsum[2], 0.64, 0.89, 0.39,	0.64);	
	pTCanvas2->					Update();
	leg->						AddEntry(v_th1_WFsum[2], TString("#mu^{+} " + Energy + "GeV "), "l");	
	leg->						AddEntry(v_th1_WFsum[0], TString("e^{+} " + Energy + "GeV "), "l");	
	leg->						DrawClone();
	TLatex* ptText		= new TLatex;
	ptText->SetTextSize(0.05);
	ptText->SetTextFont(42);
	ptText->SetTextAlign(31);
	float sepa			= GetSeparation(mean_WFsum[0],	std_WFsum[0],		mean_WFsum[2],	std_WFsum[2]);
	float dsepa			= GetSeparationError(mean_WFsum[0], std_WFsum[0], dmean_WFsum[0], dstd_WFsum[0], mean_WFsum[2], std_WFsum[2], dmean_WFsum[2], dstd_WFsum[2]);
	ptText->SetText(1200, gPad->GetUymin()+(gPad->GetUymax()-gPad->GetUymin())*0.9, Form("S(e^{+} | #mu^{+}) = %.2f #pm %.2f #sigma", sepa, dsepa));
	ptText->SetTextColor(kCyan+2);
	ptText->DrawClone();
	delete ptText;
	pTCanvas2->					SaveAs(OutputFile2_Beg.c_str());
	pTCanvas2->					Clear();
	leg->						Clear();

	v_th1_XP[0]->			SetName(TString("positrons e^{+} " + Energy + "GeV"));
	v_th1_XP[0]->			SetTitle(TString("dE/dx " + Energy + "GeV with XP"));
	v_th1_XP[0]->			Scale(1/v_th1_XP[0]->GetEntries());
	v_th1_XP[2]->			Scale(1/v_th1_XP[2]->GetEntries());
	v_th1_XP[0]->			SetMaximum(std::max({v_th1_XP[0]->GetMaximum(), v_th1_XP[2]->GetMaximum()})*1.2);
	v_tf1_XP[0]->			SetRange(0, 1800);
	v_tf1_XP[0]->			SetParameter(0, v_tf1_XP[0]->Integral(0, 1500)/v_th1_XP[0]->GetEntries());
	v_th1_XP[0]->			SetLineColor(kMagenta+2);
	v_th1_XP[0]->			SetLineWidth(2);
	v_tf1_XP[0]->			SetLineColor(kMagenta);
	v_tf1_XP[0]->			SetLineWidth(2);
	v_th1_XP[2]->			SetName(TString("muons #mu^{+} " + Energy + "GeV"));
	v_tf1_XP[2]->			SetRange(0, 1800);
	v_tf1_XP[2]->			SetParameter(0, v_tf1_XP[2]->Integral(0, 1500)/v_th1_XP[2]->GetEntries());
	v_th1_XP[2]->			SetLineColor(kMagenta+2);
	v_th1_XP[2]->			SetLineWidth(2);
	v_th1_XP[2]->			SetLineStyle(2);
	v_tf1_XP[2]->			SetLineColor(kMagenta);
	v_tf1_XP[2]->			SetLineWidth(2);
	if(v_th1_XP[0]->GetMaximum() < v_th1_XP[2]->GetMaximum()) v_th1_XP[0]->SetAxisRange(0, 1.1*v_th1_XP[2]->GetMaximum(),	"Y");
	v_th1_XP[0]->			Draw("hist");
	v_tf1_XP[0]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_th1_XP[0], 0.64, 0.89, 0.64,	0.89);	
	v_th1_XP[2]->			Draw("hist sames");
	v_tf1_XP[2]->			Draw("same");
	gPad->						Update();
	SetStatBoxPosition(v_th1_XP[2], 0.64, 0.89, 0.39,	0.64);	
	pTCanvas2->					Update();
	leg->						AddEntry(v_th1_XP[2], TString("#mu^{+} " + Energy + "GeV "), "l");	
	leg->						AddEntry(v_th1_XP[0], TString("e^{+} " + Energy + "GeV "), "l");	
	leg->						DrawClone();
	TLatex* ptText2		= new TLatex;
	ptText2->SetTextSize(0.05);
	ptText2->SetTextFont(42);
	ptText2->SetTextAlign(31);
	sepa			= GetSeparation(mean_XP[0],	std_XP[0],		mean_XP[2],	std_XP[2]);
	dsepa			= GetSeparationError(mean_XP[0], std_XP[0], dmean_XP[0], dstd_XP[0], mean_XP[2], std_XP[2], dmean_XP[2], dstd_XP[2]);
	ptText2->SetText(1200, gPad->GetUymin()+(gPad->GetUymax()-gPad->GetUymin())*0.9, Form("S(e^{+} | #mu^{+}) = %.2f #pm %.2f #sigma", sepa, dsepa));
	ptText2->SetTextColor(kMagenta+2);
	ptText2->DrawClone();
	delete ptText2;
	pTCanvas2->					SaveAs(OutputFile2_End.c_str());



	// Delete
	delete						h2_separation;
	delete						h2_err_separation;
	delete						pTCanvas;
	delete						pTCanvas2;
	v_tf1_WFsum.clear();
	v_tf1_XP.clear();
}














////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////














// Draw all scans together
void DrawOut_Scans(const std::string& inputDir, const std::string& Comment, const std::string& WFversion)
{
	// std::string Comment2 = "_zcalc_PRF_4IP_Gain_WFhatrecon";
	std::string OutputFile		= inputDir + "/Scans" + Comment + WFversion + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	gStyle->						SetErrorX(0);
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 2400, 1600);
	pTCanvas->SetMargin(0.1, 0.05, 0.27, 0.1);

	TLegend* leg				= new TLegend(0.87,0.78,0.98,0.98);

	int nz					= 10;
	int ny					= 11;
	int nphi				= 8;
	int ntheta				= 3;
	int npoint				= 0;

	TFile*					pTFile;
	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;
	std::vector<std::string>	Xlabel;
	std::vector<TLine*>		v_TLine;
	std::vector<TLatex*> v_Text;
	v_Text.push_back(new TLatex(5,	10, "#splitline{Z scan}{200 ns}"));
	v_Text.push_back(new TLatex(15, 10, "#splitline{Z scan}{412 ns}"));
	v_Text.push_back(new TLatex(26, 10, "Y scan"));
	v_Text.push_back(new TLatex(35, 10, "#splitline{#varphi scan}{	5 cm}"));
	v_Text.push_back(new TLatex(43, 10, "#splitline{#varphi scan}{ 55 cm}"));
	v_Text.push_back(new TLatex(51, 10, "#splitline{#varphi scan}{ 95 cm}"));
	v_Text.push_back(new TLatex(56, 10, "#theta scan"));
	for(int i = 0; i < 7; i++){
	v_Text[i]->SetTextSize(0.035);
	v_Text[i]->SetTextFont(132);
	v_Text[i]->SetTextAlign(22);
	}
	v_Text[6]->SetTextAngle(90);
	
	// Z scans
	int PT_arr[]	= {200, 412};
	for (int PT : PT_arr){
	for(int iz = 0; iz < nz; iz++){
		if(iz == 0) pTFile	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT" + PT + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + ".root"));
		else		pTFile	= TFile::Open(TString(inputDir + "/DESY21_zscan/DESY21_zscan_PT" + PT + "/DESY21_z" + std::to_string(iz-1) + "60_PT" + PT + "/3_DESY21_z" + std::to_string(iz-1) + "60_PT" + PT + "_dEdx" + Comment + ".root"));
		v_tf1_WF.				push_back(pTFile->Get<TF1>("tf1_WFsum"));
		v_tf1_XP.				push_back(pTFile->Get<TF1>("tf1_XP"));
		pTFile->					Clear();
		Xlabel.					push_back("PT" + std::to_string(PT) + "_Z" + std::to_string(iz*10+5) + "_Y8_#varphi0_#theta0");
		npoint++;
	}
	v_TLine.					push_back(new TLine(npoint, 0, npoint, 1));
	}

	// Y scan
	std::string Y_arr[]		= {"80", "60", "40", "20", "0", "m40", "m60", "m80", "m100", "m120", "m140"};
	int		Row_arr[]		= {4, 6, 8, 10, 12, 18, 20, 22, 24, 26, 27};
	for(int iy = 0; iy < ny; iy++){
	pTFile					= TFile::Open(TString(inputDir + "/DESY21_yscan/DESY21_y" + Y_arr[iy] + "/3_DESY21_y" + Y_arr[iy] + "_dEdx" + Comment + ".root"));
	v_tf1_WF.					push_back(pTFile->Get<TF1>("tf1_WFsum"));
	v_tf1_XP.					push_back(pTFile->Get<TF1>("tf1_XP"));
	pTFile->					Clear();
	Xlabel.					push_back("PT412_Z9_Y" + std::to_string(Row_arr[iy]) + "_#varphi0_#theta0");
	npoint++;
	}
	v_TLine.						push_back(new TLine(npoint, 0, npoint, 1));

	// Phi scans
	std::string Z_arr[]		= {"m40", "460", "860"};
	int Ztrue_arr[]			= {50, 550, 950};
	int phi_arr[] = {0, 5, 10, 20, 30, 30, 40 ,45};
	for (int iz = 0; iz < 3; iz++){
	for(int iphi = 0; iphi < nphi; iphi++){
		if(iphi < 5) pTFile	= TFile::Open(TString(inputDir + "/DESY21_phi/DESY21_phi_z" + Z_arr[iz] + "/DESY21_phi" + phi_arr[iphi] + "_z" + Z_arr[iz] + "/3_DESY21_phi" + phi_arr[iphi] + "_z" + Z_arr[iz] + "_dEdx" + Comment + ".root"));
		else		pTFile	= TFile::Open(TString(inputDir + "/DESY21_phi/DESY21_phi_z" + Z_arr[iz] + "/DESY21_phi" + phi_arr[iphi] + "_diag_z" + Z_arr[iz] + "/3_DESY21_phi" + phi_arr[iphi] + "_diag_z" + Z_arr[iz] + "_dEdx" + Comment + WFversion + ".root"));
		v_tf1_WF.				push_back(pTFile->Get<TF1>("tf1_WFsum"));
		v_tf1_XP.				push_back(pTFile->Get<TF1>("tf1_XP"));
		pTFile->					Clear();
		if(iphi < 5) Xlabel.		push_back("PT200_Z" + std::to_string(Ztrue_arr[iz]/10) + "_Y4_#varphi" + std::to_string(phi_arr[iphi]) + "_#theta0");
		else Xlabel.				push_back("PT200_Z" + std::to_string(Ztrue_arr[iz]/10) + "_Y4_#varphi" + std::to_string(phi_arr[iphi]) + "d_#theta0");
		npoint++;
	}
	v_TLine.					push_back(new TLine(npoint, 0, npoint, 1));
	}

	// Theta scan
	int theta_arr[] = {-45, -20, 20};
	std::string theta_index[] = {"m45", "m20", "20"};
	for(int itheta = 0; itheta < ntheta; itheta++){
	pTFile					= TFile::Open(TString(inputDir + "/DESY21_theta/DESY21_theta" + theta_index[itheta] + "/3_DESY21_theta" + theta_index[itheta] + "_dEdx" + Comment + ".root"));
	v_tf1_WF.					push_back(pTFile->Get<TF1>("tf1_WFsum"));
	v_tf1_XP.					push_back(pTFile->Get<TF1>("tf1_XP"));
	pTFile->					Clear();
	Xlabel.					push_back("PT200_Z45_Y8_#varphi0_#theta" + std::to_string(theta_arr[itheta]));
	npoint++;
	}

	TH1F* ph1_reso_WF	= new TH1F("ph1_reso_WF", "ph1_reso_WF", npoint, 0, npoint);
	TH1F* ph1_reso_XP	= new TH1F("ph1_reso_XP", "ph1_reso_XP", npoint, 0, npoint);
	TH1F* ph1_mean_WF	= new TH1F("ph1_mean_WF", "ph1_mean_WF", npoint, 0, npoint);
	TH1F* ph1_mean_XP	= new TH1F("ph1_mean_XP", "ph1_mean_XP", npoint, 0, npoint);
	TH1F* ph1_std_WF	= new TH1F("ph1_std_WF",	"ph1_std_WF",	npoint, 0, npoint);
	TH1F* ph1_std_XP	= new TH1F("ph1_std_XP",	"ph1_std_XP",	npoint, 0, npoint);

	// Get mean & std
	float mean_WF[npoint];
	float mean_XP[npoint];
	float dmean_WF[npoint];
	float dmean_XP[npoint];
	float std_WF[npoint];
	float std_XP[npoint];
	float dstd_WF[npoint];
	float dstd_XP[npoint];

	for(int ipoint = 0; ipoint < npoint; ipoint++){
	mean_WF[ipoint]				= v_tf1_WF[ipoint]->	GetParameter(1);
	mean_XP[ipoint]				= v_tf1_XP[ipoint]->	GetParameter(1);
	std_WF[ipoint]					= v_tf1_WF[ipoint]->	GetParameter(2);
	std_XP[ipoint]					= v_tf1_XP[ipoint]->	GetParameter(2);
	dmean_WF[ipoint]				= v_tf1_WF[ipoint]->	GetParError(1);
	dmean_XP[ipoint]				= v_tf1_XP[ipoint]->	GetParError(1);
	dstd_WF[ipoint]				= v_tf1_WF[ipoint]->	GetParError(2);
	dstd_XP[ipoint]				= v_tf1_XP[ipoint]->	GetParError(2);

	ph1_reso_WF->	Fill			(Xlabel[ipoint].c_str(), std_WF[ipoint]/mean_WF[ipoint]*100);
	ph1_reso_XP->	Fill			(Xlabel[ipoint].c_str(), std_XP[ipoint]/mean_XP[ipoint]*100);
	ph1_reso_WF->	SetBinError	(ipoint, GetResoError(v_tf1_WF[ipoint]));
	ph1_reso_XP->	SetBinError	(ipoint, GetResoError(v_tf1_XP[ipoint]));

	ph1_mean_WF->	Fill			(Xlabel[ipoint].c_str(), mean_WF[ipoint]);
	ph1_mean_XP->	Fill			(Xlabel[ipoint].c_str(), mean_XP[ipoint]);
	ph1_mean_WF->	SetBinError	(ipoint, dmean_WF[ipoint]);
	ph1_mean_XP->	SetBinError	(ipoint, dmean_XP[ipoint]);

	ph1_std_WF->	Fill			(Xlabel[ipoint].c_str(), std_WF[ipoint]);
	ph1_std_XP->	Fill			(Xlabel[ipoint].c_str(), std_XP[ipoint]);
	ph1_std_WF->	SetBinError	(ipoint, dstd_WF[ipoint]);
	ph1_std_XP->	SetBinError	(ipoint, dstd_XP[ipoint]);
	}
	std::cout << ph1_reso_XP->GetBinError(npoint-1) << std::endl;


	// Draw
	pTCanvas->					cd();

	// Resolution
	ph1_reso_XP->				SetMinimum(4);
	ph1_reso_XP->				SetMaximum(12);
	ph1_reso_XP->				SetNameTitle("ph1_reso_XP", "dE/dx: Resolution;;resolution (%)");
	Graphic_setup(ph1_reso_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(ph1_reso_XP,	3, 21, kMagenta+2, 1, kBlack);
	ph1_reso_XP->				Draw();
	ph1_reso_WF->				Draw("same");
	pTCanvas->					Update();
	for(int i = 0; i < (int)v_TLine.size(); i++){
	v_TLine[i]->SetY1(pTCanvas->GetUymin()); v_TLine[i]->SetY2(pTCanvas->GetUymax());
	v_TLine[i]->Draw();
	}
	leg->						AddEntry(ph1_reso_WF, "WF", "ep");	
	leg->						AddEntry(ph1_reso_XP, "XP" , "ep");	
	leg->						Draw();
	for(int i = 0; i < (int)v_Text.size(); i++){
	v_Text[i]->SetY(pTCanvas->GetUymin()+(pTCanvas->GetUymax()-pTCanvas->GetUymin())*7/10);
	v_Text[i]->Draw();
	}
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	ph1_mean_XP->				SetMinimum(600);
	ph1_mean_XP->				SetMaximum(1200);
	ph1_mean_XP->				SetNameTitle("ph1_mean_XP", "dE/dx: Mean;;Mean (ADC count)");
	Graphic_setup(ph1_mean_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(ph1_mean_XP,	3, 21, kMagenta+2, 1, kBlack);
	ph1_mean_XP->				Draw();
	ph1_mean_WF->				Draw("same");
	pTCanvas->					Update();
	for(int i = 0; i < (int)v_TLine.size(); i++){
	v_TLine[i]->SetY1(pTCanvas->GetUymin()); v_TLine[i]->SetY2(pTCanvas->GetUymax());
	v_TLine[i]->Draw();
	} 
	leg->						Draw();
	for(int i = 0; i < (int)v_Text.size(); i++){
	v_Text[i]->SetY(pTCanvas->GetUymin()+(pTCanvas->GetUymax()-pTCanvas->GetUymin())*7/10);
	v_Text[i]->Draw();
	}	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	ph1_std_XP->				SetMinimum(40);
	ph1_std_XP->				SetMaximum(100);
	ph1_std_XP->				SetNameTitle("ph1_std_XP", "dE/dx: Standard deviation;;std (ADC count)");
	Graphic_setup(ph1_std_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(ph1_std_XP,	3, 21, kMagenta+2, 1, kBlack);
	ph1_std_XP->				Draw();
	ph1_std_WF->				Draw("same");
	pTCanvas->					Update();
	for(int i = 0; i < (int)v_TLine.size(); i++){
	v_TLine[i]->SetY1(pTCanvas->GetUymin()); v_TLine[i]->SetY2(pTCanvas->GetUymax());
	v_TLine[i]->Draw();
	} 
	leg->						Draw();
	for(int i = 0; i < (int)v_Text.size(); i++){
	v_Text[i]->SetY(pTCanvas->GetUymin()+(pTCanvas->GetUymax()-pTCanvas->GetUymin())*7/10);
	v_Text[i]->Draw();
	}	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;
	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
	v_TLine.						clear();
	// Xlabel.						clear();
}










// Draw resolution as function of Z scan
void DrawOut_Zscan(const std::string& inputDir, const std::string& Comment, const int& PT)
{
	int ipoint				= 0;
	int nz					= 10;

	// Vectors of TFiles & TH1Fs & TF1s & TGEs
	TFile* pTFile;
	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;

	int		z_val[]	= {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
	std::string z_arr[]	= {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};

	for(int iz = 0; iz < int(std::size(z_arr)); iz++){
	pTFile					= TFile::Open(TString(inputDir + "/DESY21_z" + z_arr[iz] + "_PT" + PT + "/3_DESY21_z" + z_arr[iz] + "_PT" + PT + "_dEdx" + Comment + ".root"));
	v_tf1_WF.				push_back(pTFile->	Get<TF1>("tf1_WFsum"));
	v_tf1_XP.				push_back(pTFile->	Get<TF1>("tf1_XP"));
	pTFile->					Clear();
	}

	TGraphErrors* pTGE_reso_WF	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP	= new TGraphErrors();

	// Get mean & std
	float mean_WF[nz];
	float mean_XP[nz];
	float dmean_WF[nz];
	float dmean_XP[nz];
	float std_WF[nz];
	float std_XP[nz];
	float dstd_WF[nz];
	float dstd_XP[nz];
	for(int iz = 0; iz < nz; iz++){
	mean_WF[iz]				= v_tf1_WF[iz]->	GetParameter(1);
	mean_XP[iz]				= v_tf1_XP[iz]->	GetParameter(1);
	std_WF[iz]					= v_tf1_WF[iz]->	GetParameter(2);
	std_XP[iz]					= v_tf1_XP[iz]->	GetParameter(2);
	dmean_WF[iz]				= v_tf1_WF[iz]->	GetParError(1);
	dmean_XP[iz]				= v_tf1_XP[iz]->	GetParError(1);
	dstd_WF[iz]				= v_tf1_WF[iz]->	GetParError(2);
	dstd_XP[iz]				= v_tf1_XP[iz]->	GetParError(2);

	pTGE_reso_WF->	SetPoint		(ipoint, z_val[iz], std_WF[iz]/mean_WF[iz]*100);
	pTGE_reso_XP->	SetPoint		(ipoint, z_val[iz], std_XP[iz]/mean_XP[iz]*100);
	pTGE_reso_WF->	SetPointError (ipoint, 0,		GetResoError(v_tf1_WF[iz]));
	pTGE_reso_XP->	SetPointError (ipoint, 0,		GetResoError(v_tf1_XP[iz]));

	pTGE_mean_WF->	SetPoint		(ipoint, z_val[iz], mean_WF[iz]);
	pTGE_mean_XP->	SetPoint		(ipoint, z_val[iz], mean_XP[iz]);
	pTGE_mean_WF->	SetPointError (ipoint, 0,		dmean_WF[iz]);
	pTGE_mean_XP->	SetPointError (ipoint, 0,		dmean_XP[iz]);

	pTGE_std_WF->	SetPoint		(ipoint, z_val[iz], std_WF[iz]);
	pTGE_std_XP->	SetPoint		(ipoint, z_val[iz], std_XP[iz]);
	pTGE_std_WF->	SetPointError	(ipoint, 0,		dstd_WF[iz]);
	pTGE_std_XP->	SetPointError	(ipoint, 0,		dstd_XP[iz]);

	ipoint++;
	}


	// Draw
	std::string OutputFile		= inputDir + "/Zscan_Resolution" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.8,0.68,0.98,0.98);


	// Resolution
	pTGE_reso_WF->				GetXaxis()->SetLimits(0, 1000);
	pTGE_reso_WF->				SetMinimum(4);
	pTGE_reso_WF->				SetMaximum(12);
	pTGE_reso_WF->				SetNameTitle("pTGE_reso_WF", "Resolution vs drift distance;drift distance (mm);resolution (%)");
	Graphic_setup(pTGE_reso_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_reso_WF->				Draw("ap");
	pTGE_reso_XP->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF, "WF ", "ep"); 
	leg->						AddEntry(pTGE_reso_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	pTGE_mean_WF->				SetMinimum(600);
	pTGE_mean_WF->				SetMaximum(1200);
	pTGE_mean_WF->				SetNameTitle("pTGE_mean_WF", "Mean vs drift distance;drift distance (mm);Mean (ADC count)");
	Graphic_setup(pTGE_mean_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_mean_WF->				Draw("ap");
	pTGE_mean_XP->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	pTGE_std_WF->				SetMinimum(40);
	pTGE_std_WF->				SetMaximum(100);
	pTGE_std_WF->				SetNameTitle("pTGE_std_WF", "std vs drift distance;drift distance (mm);std (ADC count)");
	Graphic_setup(pTGE_std_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_std_WF->				Draw("ap");
	pTGE_std_XP->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;
	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Draw resolution as function of Y scan
void DrawOut_Yscan(const std::string& inputDir, const std::string& Comment)
{
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	int ipoint				= 0;
	int ny					= 11;
	std::string Y_arr[]	= {"m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"};
	int		Row_arr[] = {27, 26, 24, 22, 20, 18, 12, 10, 8, 6, 4};

	// Vectors of TFiles & TH1Fs & TF1s & TGEs
	std::vector<TFile*>		v_pTFile;
	std::vector<TH1F*>		v_h1f_WF;
	std::vector<TH1F*>		v_h1f_XP;
	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;

	for(int iy = 0; iy < ny; iy++){
	v_pTFile.				push_back(TFile::Open(TString(inputDir + "/DESY21_y" + Y_arr[iy] + "/3_DESY21_y" + Y_arr[iy] + "_dEdx" + Comment + ".root")));
	v_h1f_WF.				push_back(v_pTFile[iy]->	Get<TH1F>("h1f_WFsum"));
	v_h1f_XP.				push_back(v_pTFile[iy]->	Get<TH1F>("h1f_XP"));
	v_tf1_WF.				push_back(v_h1f_WF[iy]->	GetFunction("tf1_WFsum"));
	v_tf1_XP.				push_back(v_h1f_XP[iy]->	GetFunction("tf1_XP"));
	}

	TGraphErrors* pTGE_reso_WF	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP	= new TGraphErrors();

	// Get mean & std
	float mean_WF[ny];
	float mean_XP[ny];
	float dmean_WF[ny];
	float dmean_XP[ny];
	float std_WF[ny];
	float std_XP[ny];
	float dstd_WF[ny];
	float dstd_XP[ny];
	for(int iy = 0; iy < ny; iy++){
	mean_WF[iy]				= v_tf1_WF[iy]->	GetParameter(1);
	mean_XP[iy]				= v_tf1_XP[iy]->	GetParameter(1);
	std_WF[iy]					= v_tf1_WF[iy]->	GetParameter(2);
	std_XP[iy]					= v_tf1_XP[iy]->	GetParameter(2);
	dmean_WF[iy]				= v_tf1_WF[iy]->	GetParError(1);
	dmean_XP[iy]				= v_tf1_XP[iy]->	GetParError(1);
	dstd_WF[iy]				= v_tf1_WF[iy]->	GetParError(2);
	dstd_XP[iy]				= v_tf1_XP[iy]->	GetParError(2);

	// int y;
	// if(iy < 7)	y = -std::stoi(Y_arr[iy].erase(0,1)); // transform m into minus sign
	// else		y = std::stoi(Y_arr[iy]);

	pTGE_reso_WF->	SetPoint		(ipoint, Row_arr[iy], std_WF[iy]/mean_WF[iy]*100);
	pTGE_reso_XP->	SetPoint		(ipoint, Row_arr[iy], std_XP[iy]/mean_XP[iy]*100);
	pTGE_reso_WF->	SetPointError (ipoint, 0,			GetResoError(v_tf1_WF[iy]));
	pTGE_reso_XP->	SetPointError (ipoint, 0,			GetResoError(v_tf1_XP[iy]));

	pTGE_mean_WF->	SetPoint		(ipoint, Row_arr[iy], mean_WF[iy]);
	pTGE_mean_XP->	SetPoint		(ipoint, Row_arr[iy], mean_XP[iy]);
	pTGE_mean_WF->	SetPointError (ipoint, 0,			dmean_WF[iy]);
	pTGE_mean_XP->	SetPointError (ipoint, 0,			dmean_XP[iy]);

	pTGE_std_WF->	SetPoint		(ipoint, Row_arr[iy], std_WF[iy]);
	pTGE_std_XP->	SetPoint		(ipoint, Row_arr[iy], std_XP[iy]);
	pTGE_std_WF->	SetPointError	(ipoint, 0,			dstd_WF[iy]);
	pTGE_std_XP->	SetPointError	(ipoint, 0,			dstd_XP[iy]);

	ipoint++;
	}


	// Draw
	std::string OutputFile		= inputDir + "/Yscan_Resolution" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.8,0.75,0.98,0.95);


	// Resolution
	pTGE_reso_XP->				GetXaxis()->SetLimits(-0.5, 31.5);
	pTGE_reso_XP->				SetMinimum(4);
	pTGE_reso_XP->				SetMaximum(12);
	pTGE_reso_XP->				SetNameTitle("pTGE_reso_XP", "Resolution vs Y position;Pad row;resolution (%)");
	Graphic_setup(pTGE_reso_WF, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_reso_XP->				Draw("ap");
	pTGE_reso_WF->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_reso_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	pTGE_mean_XP->				GetXaxis()->SetLimits(-0.5, 31.5);
	pTGE_mean_XP->				SetMinimum(600);
	pTGE_mean_XP->				SetMaximum(1200);
	pTGE_mean_XP->				SetNameTitle("pTGE_mean_XP", "Mean vs Y position;Pad row;Mean (ADC count)");
	Graphic_setup(pTGE_mean_WF, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_mean_XP->				Draw("ap");
	pTGE_mean_WF->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	pTGE_std_XP->				GetXaxis()->SetLimits(-0.5, 31.5);
	pTGE_std_XP->				SetMinimum(40);
	pTGE_std_XP->				SetMaximum(121);
	pTGE_std_XP->				SetNameTitle("pTGE_std_XP", "std vs Y position;Pad row;std (ADC count)");
	Graphic_setup(pTGE_std_WF, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_std_XP->				Draw("ap");
	pTGE_std_WF->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;

	v_pTFile.					clear();
	v_h1f_WF.					clear();
	v_h1f_XP.					clear();
	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Draw resolution as function of phi scan
void DrawOut_Phiscan(const std::string& inputDir, const std::string& Comment, const std::string& WFversion, const std::string& zdrift)
{
	int ipoint				= 0;
	int nphi				= 8;
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);


	// Vectors of TFiles & TH1Fs & TF1s & TGEs
	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;
	TFile* pFile;

	int phi_arr[] = {0, 5, 10, 20, 30, 30, 40 ,45};
	for(int iphi = 0; iphi < nphi; iphi++){
	if(iphi == 0)	pFile = TFile::Open(TString("OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/DESY21_z" + zdrift + "_PT200/3_DESY21_z" + zdrift + "_PT200_dEdx" + Comment + ".root"));
	else if(iphi < 5) pFile = TFile::Open(TString(inputDir + "/DESY21_phi" + phi_arr[iphi] + "_z" + zdrift + "/3_DESY21_phi" + phi_arr[iphi] + "_z" + zdrift + "_dEdx" + Comment + ".root"));
	else				pFile = TFile::Open(TString(inputDir + "/DESY21_phi" + phi_arr[iphi] + "_diag_z" + zdrift + "/3_DESY21_phi" + phi_arr[iphi] + "_diag_z" + zdrift + "_dEdx" + Comment + WFversion + ".root"));
	v_tf1_WF.				push_back(pFile->	Get<TF1>("tf1_WFsum"));
	v_tf1_XP.				push_back(pFile->	Get<TF1>("tf1_XP"));
	}
	delete pFile;

	TGraphErrors* pTGE_reso_WF	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP	= new TGraphErrors();

	// Get mean & std
	float mean_WF[nphi];
	float mean_XP[nphi];
	float dmean_WF[nphi];
	float dmean_XP[nphi];
	float std_WF[nphi];
	float std_XP[nphi];
	float dstd_WF[nphi];
	float dstd_XP[nphi];

	int phi_arr_shift[] = {0, 5, 10, 20, 29, 31, 40, 45};
	for(int iphi = 0; iphi < nphi; iphi++){
	mean_WF[iphi]				= v_tf1_WF[iphi]->	GetParameter(1);
	mean_XP[iphi]				= v_tf1_XP[iphi]->	GetParameter(1);
	std_WF[iphi]					= v_tf1_WF[iphi]->	GetParameter(2);
	std_XP[iphi]					= v_tf1_XP[iphi]->	GetParameter(2);
	dmean_WF[iphi]				= v_tf1_WF[iphi]->	GetParError(1);
	dmean_XP[iphi]				= v_tf1_XP[iphi]->	GetParError(1);
	dstd_WF[iphi]				= v_tf1_WF[iphi]->	GetParError(2);
	dstd_XP[iphi]				= v_tf1_XP[iphi]->	GetParError(2);

	pTGE_reso_WF->	SetPoint		(ipoint, phi_arr_shift[iphi], std_WF[iphi]/mean_WF[iphi]*100);
	pTGE_reso_XP->	SetPoint		(ipoint, phi_arr_shift[iphi], std_XP[iphi]/mean_XP[iphi]*100);
	pTGE_reso_WF->	SetPointError (ipoint, 0,		GetResoError(v_tf1_WF[iphi]));
	pTGE_reso_XP->	SetPointError (ipoint, 0,		GetResoError(v_tf1_XP[iphi]));

	pTGE_mean_WF->	SetPoint		(ipoint, phi_arr_shift[iphi], mean_WF[iphi]);
	pTGE_mean_XP->	SetPoint		(ipoint, phi_arr_shift[iphi], mean_XP[iphi]);
	pTGE_mean_WF->	SetPointError (ipoint, 0,		dmean_WF[iphi]);
	pTGE_mean_XP->	SetPointError (ipoint, 0,		dmean_XP[iphi]);

	pTGE_std_WF->	SetPoint		(ipoint, phi_arr_shift[iphi], std_WF[iphi]);
	pTGE_std_XP->	SetPoint		(ipoint, phi_arr_shift[iphi], std_XP[iphi]);
	pTGE_std_WF->	SetPointError	(ipoint, 0,		dstd_WF[iphi]);
	pTGE_std_XP->	SetPointError	(ipoint, 0,		dstd_XP[iphi]);

	ipoint++;
	}	


	// Draw
	std::string OutputFile		= inputDir + "/Phiscan_Resolution" + Comment + WFversion + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.8,0.7,0.98,0.98);


	// Resolution
	pTGE_reso_WF->				GetXaxis()->SetLimits(-3, 48);
	pTGE_reso_WF->				SetMinimum(4);
	pTGE_reso_WF->				SetMaximum(12);
	pTGE_reso_WF->				SetNameTitle("pTGE_reso_WF", "Resolution vs #varphi angle;#varphi angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_reso_WF->				Draw("ap");
	pTGE_reso_XP->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_reso_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_mean_WF->				GetXaxis()->SetLimits(-3, 48);
	pTGE_mean_WF->				SetMinimum(600);
	pTGE_mean_WF->				SetMaximum(1200);
	pTGE_mean_WF->				SetNameTitle("pTGE_mean_WF", "Mean vs #varphi angle;#varphi angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_mean_WF->				Draw("ap");
	pTGE_mean_XP->				Draw("p same");
	leg->						AddEntry(pTGE_mean_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_mean_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_std_WF->				GetXaxis()->SetLimits(-3, 48);
	pTGE_std_WF->				SetMinimum(40);
	pTGE_std_WF->				SetMaximum(100);
	pTGE_std_WF->				SetNameTitle("pTGE_std_WF", "Standard deviation vs #varphi angle;#varphi angle (#circ);standard deviation (ADC count)");
	Graphic_setup(pTGE_std_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_std_WF->				Draw("ap");
	pTGE_std_XP->				Draw("p same");
	leg->						AddEntry(pTGE_std_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_std_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;

	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Draw resolution as function of theta scan
void DrawOut_Thetascan(const std::string& inputDir, const std::string& Comment)
{
	int ipoint				= 0;
	int ntheta				= 3;

	// Vectors of TFiles & TH1Fs & TF1s & TGEs
	std::vector<TFile*>		v_pTFile;
	std::vector<TH1F*>		v_h1f_WF;
	std::vector<TH1F*>		v_h1f_XP;
	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;

	int theta_arr[] = {-45, -20, 20};
	std::string theta_index[] = {"m45", "m20", "20"};
	for(int itheta = 0; itheta < ntheta; itheta++){
	v_pTFile.				push_back(TFile::Open(TString(inputDir + "/DESY21_theta" + theta_index[itheta] + "/3_DESY21_theta" + theta_index[itheta] + "_dEdx" + Comment + ".root")));
	v_h1f_WF.				push_back(v_pTFile[itheta]->	Get<TH1F>("h1f_WFsum"));
	v_h1f_XP.				push_back(v_pTFile[itheta]->	Get<TH1F>("h1f_XP"));
	v_tf1_WF.				push_back(v_h1f_WF[itheta]->	GetFunction("tf1_WFsum"));
	v_tf1_XP.				push_back(v_h1f_XP[itheta]->	GetFunction("tf1_XP"));
	}

	TGraphErrors* pTGE_reso_WF	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP	= new TGraphErrors();

	// Get mean & std
	float mean_WF[ntheta];
	float mean_XP[ntheta];
	float dmean_WF[ntheta];
	float dmean_XP[ntheta];
	float std_WF[ntheta];
	float std_XP[ntheta];
	float dstd_WF[ntheta];
	float dstd_XP[ntheta];

	for(int itheta = 0; itheta < ntheta; itheta++){
	mean_WF[itheta]				= v_tf1_WF[itheta]->	GetParameter(1);
	mean_XP[itheta]				= v_tf1_XP[itheta]->	GetParameter(1);
	std_WF[itheta]					= v_tf1_WF[itheta]->	GetParameter(2);
	std_XP[itheta]					= v_tf1_XP[itheta]->	GetParameter(2);
	dmean_WF[itheta]				= v_tf1_WF[itheta]->	GetParError(1);
	dmean_XP[itheta]				= v_tf1_XP[itheta]->	GetParError(1);
	dstd_WF[itheta]				= v_tf1_WF[itheta]->	GetParError(2);
	dstd_XP[itheta]				= v_tf1_XP[itheta]->	GetParError(2);

	pTGE_reso_WF->	SetPoint		(ipoint, theta_arr[itheta], std_WF[itheta]/mean_WF[itheta]*100);
	pTGE_reso_XP->	SetPoint		(ipoint, theta_arr[itheta], std_XP[itheta]/mean_XP[itheta]*100);
	pTGE_reso_WF->	SetPointError (ipoint, 0,		GetResoError(v_tf1_WF[itheta]));
	pTGE_reso_XP->	SetPointError (ipoint, 0,		GetResoError(v_tf1_XP[itheta]));

	pTGE_mean_WF->	SetPoint		(ipoint, theta_arr[itheta], mean_WF[itheta]);
	pTGE_mean_XP->	SetPoint		(ipoint, theta_arr[itheta], mean_XP[itheta]);
	pTGE_mean_WF->	SetPointError (ipoint, 0,		dmean_WF[itheta]);
	pTGE_mean_XP->	SetPointError (ipoint, 0,		dmean_XP[itheta]);

	pTGE_std_WF->	SetPoint		(ipoint, theta_arr[itheta], std_WF[itheta]);
	pTGE_std_XP->	SetPoint		(ipoint, theta_arr[itheta], std_XP[itheta]);
	pTGE_std_WF->	SetPointError	(ipoint, 0,		dstd_WF[itheta]);
	pTGE_std_XP->	SetPointError	(ipoint, 0,		dstd_XP[itheta]);

	ipoint++;
	}	


	// Draw
	std::string OutputFile		= inputDir + "/thetascan_Resolution" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.8,0.68,0.98,0.98);


	// Resolution
	pTGE_reso_WF->				GetXaxis()->SetLimits(-48, 23);
	pTGE_reso_WF->				SetMinimum(4);
	pTGE_reso_WF->				SetMaximum(12);
	pTGE_reso_WF->				SetNameTitle("pTGE_reso_WF", "Resolution vs #theta angle;#theta angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_reso_WF->				Draw("ap");
	pTGE_reso_XP->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_reso_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_mean_WF->				GetXaxis()->SetLimits(-48, 23);
	pTGE_mean_WF->				SetMinimum(600);
	pTGE_mean_WF->				SetMaximum(1200);
	pTGE_mean_WF->				SetNameTitle("pTGE_mean_WF", "Mean vs #theta angle;#theta angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_mean_WF->				Draw("ap");
	pTGE_mean_XP->				Draw("p same");
	leg->						AddEntry(pTGE_mean_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_mean_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_std_WF->				GetXaxis()->SetLimits(-48, 23);
	pTGE_std_WF->				SetMinimum(40);
	pTGE_std_WF->				SetMaximum(100);
	pTGE_std_WF->				SetNameTitle("pTGE_std_WF", "Standard deviation vs #theta angle;#theta angle (#circ);standard deviation (ADC count)");
	Graphic_setup(pTGE_std_WF,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_XP,	3, 21, kMagenta+2, 1, kBlack);
	pTGE_std_WF->				Draw("ap");
	pTGE_std_XP->				Draw("p same");
	leg->						AddEntry(pTGE_std_WF, "WF ", "ep");	
	leg->						AddEntry(pTGE_std_XP, "XP ", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;

	// for(int itheta = 0; itheta < ntheta; itheta++){
	//	delete						v_pTFile[itheta];	v_pTFile[itheta]	= 0;
	//	delete						v_h1f_WF[itheta];	v_h1f_WF[itheta]	= 0;
	//	delete						v_h1f_XP[itheta];	v_h1f_XP[itheta]	= 0;
	//	delete						v_tf1_WF[itheta];	v_tf1_WF[itheta]	= 0;
	//	delete						v_tf1_XP[itheta];	v_tf1_XP[itheta]	= 0;
	// }

	v_pTFile.					clear();
	v_h1f_WF.					clear();
	v_h1f_XP.					clear();
	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
}








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////












// Transverse diffusion coefficient effect on Z scan
void DrawOut_Zscan_PT(const std::string& inputDir, const std::string& Comment)
{
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	int ipoint				= 0;
	int nz					= 9;

	// Vectors of TFiles & TH1Fs & TF1s & TGEs
	std::vector<TFile*>		v_pTFile_200;
	std::vector<TFile*>		v_pTFile_412;
	std::vector<TH1F*>		v_h1f_WF_200;
	std::vector<TH1F*>		v_h1f_XP_200;
	std::vector<TH1F*>		v_h1f_XP_412;
	std::vector<TH1F*>		v_h1f_WF_412;
	std::vector<TF1*>		v_tf1_WF_200;
	std::vector<TF1*>		v_tf1_XP_200;
	std::vector<TF1*>		v_tf1_XP_412;
	std::vector<TF1*>		v_tf1_WF_412;

	int		z_val[]	= {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
	std::string z_arr[]	= {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};

	for(int iz = 0; iz < (int)std::size(z_arr); iz++){
	v_pTFile_200.		push_back(TFile::Open(TString(inputDir + "/DESY21_zscan_PT200/DESY21_z" + z_arr[iz] + "_PT200/3_DESY21_z" + z_arr[iz] + "_PT200_dEdx" + Comment + ".root")));
	v_pTFile_412.		push_back(TFile::Open(TString(inputDir + "/DESY21_zscan_PT412/DESY21_z" + z_arr[iz] + "_PT412/3_DESY21_z" + z_arr[iz] + "_PT412_dEdx" + Comment + ".root")));
	v_h1f_WF_200.			push_back(v_pTFile_200[iz]->	Get<TH1F>("h1f_WFsum"));
	v_h1f_WF_412.			push_back(v_pTFile_412[iz]->	Get<TH1F>("h1f_WFsum"));
	v_h1f_XP_200.			push_back(v_pTFile_200[iz]->	Get<TH1F>("h1f_XP"));
	v_h1f_XP_412.			push_back(v_pTFile_412[iz]->	Get<TH1F>("h1f_XP"));
	v_tf1_WF_200.			push_back(v_h1f_WF_200[iz]->	GetFunction("tf1_WFsum"));
	v_tf1_WF_412.			push_back(v_h1f_WF_412[iz]->	GetFunction("tf1_WFsum"));
	v_tf1_XP_200.			push_back(v_h1f_XP_200[iz]->	GetFunction("tf1_XP"));
	v_tf1_XP_412.			push_back(v_h1f_XP_412[iz]->	GetFunction("tf1_XP"));
	}

	TGraphErrors* pTGE_reso_WF_200	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP_200	= new TGraphErrors();
	TGraphErrors* pTGE_reso_WF_412	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP_412	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF_200	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP_200	= new TGraphErrors();
	TGraphErrors* pTGE_mean_WF_412	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP_412	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF_200	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP_200	= new TGraphErrors();
	TGraphErrors* pTGE_std_WF_412	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP_412	= new TGraphErrors();

	// Get mean & std
	float mean_WF_200[nz];
	float mean_XP_200[nz];
	float mean_WF_412[nz];
	float mean_XP_412[nz];
	float dmean_WF_200[nz];
	float dmean_XP_200[nz];
	float dmean_WF_412[nz];
	float dmean_XP_412[nz];
	float std_WF_200[nz];
	float std_XP_200[nz];
	float std_WF_412[nz];
	float std_XP_412[nz];
	float dstd_WF_200[nz];
	float dstd_XP_200[nz];
	float dstd_WF_412[nz];
	float dstd_XP_412[nz];
	for(int iz = 0; iz <= nz; iz++){
	mean_WF_200[iz]				= v_tf1_WF_200[iz]->	GetParameter(1);
	mean_XP_200[iz]				= v_tf1_XP_200[iz]->	GetParameter(1);
	mean_WF_412[iz]				= v_tf1_WF_412[iz]->	GetParameter(1);
	mean_XP_412[iz]				= v_tf1_XP_412[iz]->	GetParameter(1);
	std_WF_200[iz]					= v_tf1_WF_200[iz]->	GetParameter(2);
	std_XP_200[iz]				= v_tf1_XP_200[iz]->	GetParameter(2);
	std_WF_412[iz]				= v_tf1_WF_412[iz]->	GetParameter(2);
	std_XP_412[iz]					= v_tf1_XP_412[iz]->	GetParameter(2);
	dmean_WF_200[iz]				= v_tf1_WF_200[iz]->	GetParError(1);
	dmean_XP_200[iz]				= v_tf1_XP_200[iz]->	GetParError(1);
	dmean_WF_412[iz]				= v_tf1_WF_412[iz]->	GetParError(1);
	dmean_XP_412[iz]				= v_tf1_XP_412[iz]->	GetParError(1);
	dstd_WF_200[iz]				= v_tf1_WF_200[iz]->	GetParError(2);
	dstd_XP_200[iz]				= v_tf1_XP_200[iz]->	GetParError(2);
	dstd_WF_412[iz]				= v_tf1_WF_412[iz]->	GetParError(2);
	dstd_XP_412[iz]				= v_tf1_XP_412[iz]->	GetParError(2);

	pTGE_reso_WF_200->	SetPoint		(ipoint, z_val[iz], std_WF_200[iz]/mean_WF_200[iz]*100);
	pTGE_reso_XP_200->SetPoint		(ipoint, z_val[iz], std_XP_200[iz]/mean_XP_200[iz]*100);
	pTGE_reso_WF_412->SetPoint		(ipoint, z_val[iz], std_WF_412[iz]/mean_WF_412[iz]*100);
	pTGE_reso_XP_412->	SetPoint		(ipoint, z_val[iz], std_XP_412[iz]/mean_XP_412[iz]*100);
	pTGE_reso_WF_200->	SetPointError (ipoint, 0,		GetResoError(v_tf1_WF_200[iz]));
	pTGE_reso_XP_200->SetPointError (ipoint, 0,		GetResoError(v_tf1_XP_200[iz]));
	pTGE_reso_WF_412->SetPointError (ipoint, 0,		GetResoError(v_tf1_WF_412[iz]));
	pTGE_reso_XP_412->	SetPointError (ipoint, 0,		GetResoError(v_tf1_XP_412[iz]));

	pTGE_mean_WF_200->	SetPoint		(ipoint, z_val[iz], mean_WF_200[iz]);
	pTGE_mean_XP_200->SetPoint		(ipoint, z_val[iz], mean_XP_200[iz]);
	pTGE_mean_WF_412->SetPoint		(ipoint, z_val[iz], mean_WF_412[iz]);
	pTGE_mean_XP_412->	SetPoint		(ipoint, z_val[iz], mean_XP_412[iz]);
	pTGE_mean_WF_200->	SetPointError (ipoint, 0,		dmean_WF_200[iz]);
	pTGE_mean_XP_200->SetPointError (ipoint, 0,		dmean_XP_200[iz]);
	pTGE_mean_WF_412->SetPointError (ipoint, 0,		dmean_WF_412[iz]);
	pTGE_mean_XP_412->	SetPointError (ipoint, 0,		dmean_XP_412[iz]);

	pTGE_std_WF_200->	SetPoint		(ipoint, z_val[iz], std_WF_200[iz]);
	pTGE_std_XP_200->SetPoint		(ipoint, z_val[iz], std_XP_200[iz]);
	pTGE_std_WF_412->SetPoint		(ipoint, z_val[iz], std_WF_412[iz]);
	pTGE_std_XP_412->	SetPoint		(ipoint, z_val[iz], std_XP_412[iz]);
	pTGE_std_WF_200->	SetPointError	(ipoint, 0,		dstd_WF_200[iz]);
	pTGE_std_XP_200->SetPointError	(ipoint, 0,		dstd_XP_200[iz]);
	pTGE_std_WF_412->SetPointError	(ipoint, 0,		dstd_WF_412[iz]);
	pTGE_std_XP_412->	SetPointError	(ipoint, 0,		dstd_XP_412[iz]);

	ipoint++;
	}


	// Draw
	std::string OutputFile		= inputDir + "/Zscan_Resolution_PT" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	// gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.75,0.65,0.98,0.98);


	// Resolution
	pTGE_reso_XP_200->				GetXaxis()->SetLimits(0, 1000);
	pTGE_reso_XP_200->				SetMinimum(4);
	pTGE_reso_XP_200->				SetMaximum(10);
	pTGE_reso_XP_200->				SetNameTitle("pTGE_reso_XP", "Resolution vs drift distance;drift distance (mm);resolution (%)");
	Graphic_setup(pTGE_reso_WF_200, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_XP_200, 3, 21, kGreen+3,	1, kBlack);
	Graphic_setup(pTGE_reso_WF_412, 3, 20, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_reso_XP_412, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_reso_XP_200->				Draw("ap");
	pTGE_reso_WF_200->				Draw("p same");
	pTGE_reso_XP_412->				Draw("p same");
	pTGE_reso_WF_412->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF_200, "WF 200 ns", "ep");	
	leg->						AddEntry(pTGE_reso_WF_412, "WF 412 ns", "ep"); 
	leg->						AddEntry(pTGE_reso_XP_200, "XP 200 ns", "ep");	
	leg->						AddEntry(pTGE_reso_XP_412, "XP 412 ns", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	pTGE_mean_XP_200->				GetXaxis()->SetLimits(0, 1000);
	pTGE_mean_XP_200->				SetMinimum(600);
	pTGE_mean_XP_200->				SetMaximum(1200);
	pTGE_mean_XP_200->				SetNameTitle("pTGE_mean_XP", "Mean vs drift distance;drift distance (mm);Mean (ADC count)");
	Graphic_setup(pTGE_mean_WF_200, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_XP_200, 3, 21, kGreen+3,	1, kBlack);
	Graphic_setup(pTGE_mean_WF_412, 3, 20, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_mean_XP_412, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_mean_XP_200->				Draw("ap");
	pTGE_mean_WF_200->				Draw("p same");
	pTGE_mean_XP_412->				Draw("p same");
	pTGE_mean_WF_412->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	pTGE_std_XP_200->				GetXaxis()->SetLimits(0, 1000);
	pTGE_std_XP_200->				SetMinimum(40);
	pTGE_std_XP_200->				SetMaximum(100);
	pTGE_std_XP_200->				SetNameTitle("pTGE_std_XP", "std vs drift distance;drift distance (mm);std (ADC count)");
	Graphic_setup(pTGE_std_WF_200, 3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_XP_200, 3, 21, kGreen+3,	1, kBlack);
	Graphic_setup(pTGE_std_WF_412, 3, 20, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_std_XP_412, 3, 21, kMagenta+2, 1, kBlack);
	pTGE_std_XP_200->				Draw("ap");
	pTGE_std_WF_200->				Draw("p same");
	pTGE_std_XP_412->				Draw("p same");
	pTGE_std_WF_412->				Draw("p same");
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;
	v_pTFile_200.					clear();
	v_pTFile_412.					clear();
	v_h1f_WF_200.					clear();
	v_h1f_XP_200.					clear();
	v_h1f_WF_412.					clear();
	v_h1f_XP_412.					clear();
	v_tf1_WF_200.					clear();
	v_tf1_XP_200.					clear();
	v_tf1_WF_412.					clear();
	v_tf1_XP_412.					clear();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Draw resolution as function of phi scan for different drift distances
void DrawOut_Phiscan_Z(const std::string& inputDir, const std::string& Comment, const std::string& WFversion)
{
	int ipoint				= 0;
	int nphi				= 8;
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);

	// Vectors of TFiles & TH1Fs & TF1s & 
	TFile*					pTFile_5;
	TFile*					pTFile_55;
	TFile*					pTFile_95;
	std::vector<TH1F*>		v_h1f_WF_5;
	std::vector<TH1F*>		v_h1f_WF_55;
	std::vector<TH1F*>		v_h1f_WF_95;
	std::vector<TH1F*>		v_h1f_XP_5;
	std::vector<TH1F*>		v_h1f_XP_55;
	std::vector<TH1F*>		v_h1f_XP_95;
	std::vector<TF1*>		v_tf1_WF_5;
	std::vector<TF1*>		v_tf1_WF_55;
	std::vector<TF1*>		v_tf1_WF_95;
	std::vector<TF1*>		v_tf1_XP_5;
	std::vector<TF1*>		v_tf1_XP_55;
	std::vector<TF1*>		v_tf1_XP_95;

	int phi_arr[] = {0, 5, 10, 20, 30, 30, 40, 45};
	for(int iphi = 0; iphi < nphi; iphi++){
	if(iphi == 0){
		pTFile_5 = TFile::Open(TString("OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/DESY21_zm40_PT200/3_DESY21_zm40_PT200_dEdx" + Comment + ".root"));
		pTFile_55 = TFile::Open(TString("OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + ".root"));
		pTFile_95 = TFile::Open(TString("OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/DESY21_z860_PT200/3_DESY21_z860_PT200_dEdx" + Comment + ".root"));
		// v_pTFile_5.			push_back(TFile::Open(TString(inputDir + "/DESY21_phi_zm40/DESY21_phi" + phi_arr[iphi] + "_zm40/3_DESY21_phi" + phi_arr[iphi] + "_zm40_dEdx" + Comment + ".root")));
		// v_pTFile_55.			push_back(TFile::Open(TString(inputDir + "/DESY21_phi_z460/DESY21_phi" + phi_arr[iphi] + "_z460/3_DESY21_phi" + phi_arr[iphi] + "_z460_dEdx" + Comment + ".root")));
		// v_pTFile_95.			push_back(TFile::Open(TString(inputDir + "/DESY21_phi_z860/DESY21_phi" + phi_arr[iphi] + "_z860/3_DESY21_phi" + phi_arr[iphi] + "_z860_dEdx" + Comment + ".root")));
	}
	else if(iphi < 5){
		pTFile_5 = TFile::Open(TString(inputDir + "/DESY21_phi_zm40/DESY21_phi" + phi_arr[iphi] + "_zm40/3_DESY21_phi" + phi_arr[iphi] + "_zm40_dEdx" + Comment + ".root"));
		pTFile_55 = TFile::Open(TString(inputDir + "/DESY21_phi_z460/DESY21_phi" + phi_arr[iphi] + "_z460/3_DESY21_phi" + phi_arr[iphi] + "_z460_dEdx" + Comment + ".root"));
		pTFile_95 = TFile::Open(TString(inputDir + "/DESY21_phi_z860/DESY21_phi" + phi_arr[iphi] + "_z860/3_DESY21_phi" + phi_arr[iphi] + "_z860_dEdx" + Comment + ".root"));
	}
	else{
		pTFile_5 = TFile::Open(TString(inputDir + "/DESY21_phi_zm40/DESY21_phi" + phi_arr[iphi] + "_diag_zm40/3_DESY21_phi" + phi_arr[iphi] + "_diag_zm40_dEdx" + Comment + WFversion + ".root"));
		pTFile_55 = TFile::Open(TString(inputDir + "/DESY21_phi_z460/DESY21_phi" + phi_arr[iphi] + "_diag_z460/3_DESY21_phi" + phi_arr[iphi] + "_diag_z460_dEdx" + Comment + WFversion + ".root"));
		pTFile_95 = TFile::Open(TString(inputDir + "/DESY21_phi_z860/DESY21_phi" + phi_arr[iphi] + "_diag_z860/3_DESY21_phi" + phi_arr[iphi] + "_diag_z860_dEdx" + Comment + WFversion + ".root"));
	}
	v_h1f_WF_5.			push_back(pTFile_5->			Get<TH1F>("h1f_WFsum"));
	v_h1f_WF_55.			push_back(pTFile_55->			Get<TH1F>("h1f_WFsum"));
	v_h1f_WF_95.			push_back(pTFile_95->			Get<TH1F>("h1f_WFsum"));
	v_h1f_XP_5.			push_back(pTFile_5->			Get<TH1F>("h1f_XP"));
	v_h1f_XP_55.			push_back(pTFile_55->			Get<TH1F>("h1f_XP"));
	v_h1f_XP_95.			push_back(pTFile_95->			Get<TH1F>("h1f_XP"));
	v_tf1_WF_5.			push_back(v_h1f_WF_5[iphi]->	GetFunction("tf1_WFsum"));
	v_tf1_WF_55.			push_back(v_h1f_WF_55[iphi]->	GetFunction("tf1_WFsum"));
	v_tf1_WF_95.			push_back(v_h1f_WF_95[iphi]->	GetFunction("tf1_WFsum"));
	v_tf1_XP_5.			push_back(v_h1f_XP_5[iphi]->	GetFunction("tf1_XP"));
	v_tf1_XP_55.			push_back(v_h1f_XP_55[iphi]->	GetFunction("tf1_XP"));
	v_tf1_XP_95.			push_back(v_h1f_XP_95[iphi]->	GetFunction("tf1_XP"));
	}

	TGraphErrors* pTGE_reso_WF_5	= new TGraphErrors();
	TGraphErrors* pTGE_reso_WF_55	= new TGraphErrors();
	TGraphErrors* pTGE_reso_WF_95	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP_5	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP_55	= new TGraphErrors();
	TGraphErrors* pTGE_reso_XP_95	= new TGraphErrors();

	TGraphErrors* pTGE_mean_WF_5	= new TGraphErrors();
	TGraphErrors* pTGE_mean_WF_55	= new TGraphErrors();
	TGraphErrors* pTGE_mean_WF_95	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP_5	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP_55	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP_95	= new TGraphErrors();

	TGraphErrors* pTGE_std_WF_5	= new TGraphErrors();
	TGraphErrors* pTGE_std_WF_55	= new TGraphErrors();
	TGraphErrors* pTGE_std_WF_95	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP_5	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP_55	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP_95	= new TGraphErrors();

	// Get mean & std
	float mean_WF_5[nphi];
	float mean_WF_55[nphi];
	float mean_WF_95[nphi];
	float dmean_WF_5[nphi];
	float dmean_WF_55[nphi];
	float dmean_WF_95[nphi];
	float std_WF_5[nphi];
	float std_WF_55[nphi];
	float std_WF_95[nphi];
	float dstd_WF_5[nphi];
	float dstd_WF_55[nphi];
	float dstd_WF_95[nphi];
	float mean_XP_5[nphi];
	float mean_XP_55[nphi];
	float mean_XP_95[nphi];
	float dmean_XP_5[nphi];
	float dmean_XP_55[nphi];
	float dmean_XP_95[nphi];
	float std_XP_5[nphi];
	float std_XP_55[nphi];
	float std_XP_95[nphi];
	float dstd_XP_5[nphi];
	float dstd_XP_55[nphi];
	float dstd_XP_95[nphi];

	int phi_arr_shift[] = {0, 5, 10, 20, 29, 31, 40, 45};
	for(int iphi = 0; iphi < nphi; iphi++){
	mean_WF_5[iphi]				= v_tf1_WF_5[iphi]->	GetParameter(1);
	mean_WF_55[iphi]				= v_tf1_WF_55[iphi]->	GetParameter(1);
	mean_WF_95[iphi]				= v_tf1_WF_95[iphi]->	GetParameter(1);
	std_WF_5[iphi]					= v_tf1_WF_5[iphi]->	GetParameter(2);
	std_WF_55[iphi]				= v_tf1_WF_55[iphi]->	GetParameter(2);
	std_WF_95[iphi]				= v_tf1_WF_95[iphi]->	GetParameter(2);
	dmean_WF_5[iphi]				= v_tf1_WF_5[iphi]->	GetParError(1);
	dmean_WF_55[iphi]				= v_tf1_WF_55[iphi]->	GetParError(1);
	dmean_WF_95[iphi]				= v_tf1_WF_95[iphi]->	GetParError(1);
	dstd_WF_5[iphi]				= v_tf1_WF_5[iphi]->	GetParError(2);
	dstd_WF_55[iphi]				= v_tf1_WF_55[iphi]->	GetParError(2);
	dstd_WF_95[iphi]				= v_tf1_WF_95[iphi]->	GetParError(2);
	mean_XP_5[iphi]				= v_tf1_XP_5[iphi]->	GetParameter(1);
	mean_XP_55[iphi]				= v_tf1_XP_55[iphi]->	GetParameter(1);
	mean_XP_95[iphi]				= v_tf1_XP_95[iphi]->	GetParameter(1);
	std_XP_5[iphi]					= v_tf1_XP_5[iphi]->	GetParameter(2);
	std_XP_55[iphi]				= v_tf1_XP_55[iphi]->	GetParameter(2);
	std_XP_95[iphi]				= v_tf1_XP_95[iphi]->	GetParameter(2);
	dmean_XP_5[iphi]				= v_tf1_XP_5[iphi]->	GetParError(1);
	dmean_XP_55[iphi]				= v_tf1_XP_55[iphi]->	GetParError(1);
	dmean_XP_95[iphi]				= v_tf1_XP_95[iphi]->	GetParError(1);
	dstd_XP_5[iphi]				= v_tf1_XP_5[iphi]->	GetParError(2);
	dstd_XP_55[iphi]				= v_tf1_XP_55[iphi]->	GetParError(2);
	dstd_XP_95[iphi]				= v_tf1_XP_95[iphi]->	GetParError(2);

	pTGE_reso_WF_5->	SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_5[iphi]/mean_WF_5[iphi]*100);
	pTGE_reso_WF_55->SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_55[iphi]/mean_WF_55[iphi]*100);
	pTGE_reso_WF_95->SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_95[iphi]/mean_WF_95[iphi]*100);
	pTGE_reso_WF_5->	SetPointError (ipoint, 0,		GetResoError(v_tf1_WF_5[iphi]));
	pTGE_reso_WF_55->SetPointError (ipoint, 0,		GetResoError(v_tf1_WF_55[iphi]));
	pTGE_reso_WF_95->SetPointError (ipoint, 0,		GetResoError(v_tf1_WF_95[iphi]));

	pTGE_mean_WF_5->	SetPoint		(ipoint, phi_arr_shift[iphi], mean_WF_5[iphi]);
	pTGE_mean_WF_55->SetPoint		(ipoint, phi_arr_shift[iphi], mean_WF_55[iphi]);
	pTGE_mean_WF_95->SetPoint		(ipoint, phi_arr_shift[iphi], mean_WF_95[iphi]);
	pTGE_mean_WF_5->	SetPointError (ipoint, 0,		dmean_WF_5[iphi]);
	pTGE_mean_WF_55->SetPointError (ipoint, 0,		dmean_WF_55[iphi]);
	pTGE_mean_WF_95->SetPointError (ipoint, 0,		dmean_WF_95[iphi]);

	pTGE_std_WF_5->	SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_5[iphi]);
	pTGE_std_WF_55->SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_55[iphi]);
	pTGE_std_WF_95->SetPoint		(ipoint, phi_arr_shift[iphi], std_WF_95[iphi]);
	pTGE_std_WF_5->	SetPointError	(ipoint, 0,		dstd_WF_5[iphi]);
	pTGE_std_WF_55->SetPointError	(ipoint, 0,		dstd_WF_55[iphi]);
	pTGE_std_WF_95->SetPointError	(ipoint, 0,		dstd_WF_95[iphi]);

	pTGE_reso_XP_5->	SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_5[iphi]/mean_XP_5[iphi]*100);
	pTGE_reso_XP_55->SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_55[iphi]/mean_XP_55[iphi]*100);
	pTGE_reso_XP_95->SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_95[iphi]/mean_XP_95[iphi]*100);
	pTGE_reso_XP_5->	SetPointError (ipoint, 0,		GetResoError(v_tf1_XP_5[iphi]));
	pTGE_reso_XP_55->SetPointError (ipoint, 0,		GetResoError(v_tf1_XP_55[iphi]));
	pTGE_reso_XP_95->SetPointError (ipoint, 0,		GetResoError(v_tf1_XP_95[iphi]));

	pTGE_mean_XP_5->	SetPoint		(ipoint, phi_arr_shift[iphi], mean_XP_5[iphi]);
	pTGE_mean_XP_55->SetPoint		(ipoint, phi_arr_shift[iphi], mean_XP_55[iphi]);
	pTGE_mean_XP_95->SetPoint		(ipoint, phi_arr_shift[iphi], mean_XP_95[iphi]);
	pTGE_mean_XP_5->	SetPointError (ipoint, 0,		dmean_XP_5[iphi]);
	pTGE_mean_XP_55->SetPointError (ipoint, 0,		dmean_XP_55[iphi]);
	pTGE_mean_XP_95->SetPointError (ipoint, 0,		dmean_XP_95[iphi]);

	pTGE_std_XP_5->	SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_5[iphi]);
	pTGE_std_XP_55->SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_55[iphi]);
	pTGE_std_XP_95->SetPoint		(ipoint, phi_arr_shift[iphi], std_XP_95[iphi]);
	pTGE_std_XP_5->	SetPointError	(ipoint, 0,		dstd_XP_5[iphi]);
	pTGE_std_XP_55->SetPointError	(ipoint, 0,		dstd_XP_55[iphi]);
	pTGE_std_XP_95->SetPointError	(ipoint, 0,		dstd_XP_95[iphi]);
	ipoint++;
	}	


	// Draw
	std::string OutputFile_WF		= inputDir + "/Phiscan_Resolution_Z" + Comment + WFversion + ".pdf";
	std::string OutputFile_WF_Beg	= OutputFile_WF + "(";
	std::string OutputFile_WF_End	= OutputFile_WF + ")";
	std::string OutputFile_XP		= inputDir + "/Phiscan_Resolution_Z" + Comment + "_XP.pdf";
	std::string OutputFile_XP_Beg	= OutputFile_XP + "(";
	std::string OutputFile_XP_End	= OutputFile_XP + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	TLegend* leg				= new TLegend(0.7,0.7,0.98,0.95);


	// Resolution
	pTGE_reso_WF_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_reso_WF_55->				SetMinimum(4);
	pTGE_reso_WF_55->				SetMaximum(12);
	pTGE_reso_WF_55->				SetNameTitle("pTGE_reso_WF_55", "Resolution vs #varphi angle;#varphi angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_WF_5,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_reso_WF_55,	3, 21, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_reso_WF_95,	3, 22, kBlack,	1, kBlack);
	Graphic_setup(pTGE_reso_XP_5,	3, 20, kGreen+1,	1, kBlack);
	Graphic_setup(pTGE_reso_XP_55,	3, 21, kMagenta+2, 1, kBlack);
	Graphic_setup(pTGE_reso_XP_95,	3, 22, kMagenta+4, 1, kBlack);
	pTGE_reso_WF_55->				Draw("ap");
	pTGE_reso_WF_5->				Draw("p same");
	pTGE_reso_WF_95->				Draw("p same");
	leg->						AddEntry(pTGE_reso_WF_95, "WF Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_reso_WF_55, "WF Z = 55 cm", "ep");	
	leg->						AddEntry(pTGE_reso_WF_5, "WF Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_WF_Beg.c_str());
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_reso_XP_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_reso_XP_55->				SetMinimum(4);
	pTGE_reso_XP_55->				SetMaximum(12);
	pTGE_reso_XP_55->				SetNameTitle("pTGE_reso_XP_5", "Resolution vs #varphi angle;#varphi angle (#circ);resolution (%)");
	pTGE_reso_XP_55->				Draw("ap");
	pTGE_reso_XP_5->				Draw("p same");
	pTGE_reso_XP_95->				Draw("p same");
	leg->						AddEntry(pTGE_reso_XP_95, "XP Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_reso_XP_55, "XP Z = 55 cm", "ep"); 
	leg->						AddEntry(pTGE_reso_XP_5, "XP Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_XP_Beg.c_str());

	// Mean
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_mean_WF_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_mean_WF_55->				SetMinimum(600);
	pTGE_mean_WF_55->				SetMaximum(1200);
	pTGE_mean_WF_55->				SetNameTitle("pTGE_mean_WF_55", "Mean vs #varphi angle;#varphi angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_WF_5,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_mean_WF_55,	3, 21, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_mean_WF_95,	3, 22, kBlack,	1, kBlack);
	Graphic_setup(pTGE_mean_XP_5,	3, 20, kGreen+1,	1, kBlack);
	Graphic_setup(pTGE_mean_XP_55,	3, 21, kMagenta+2, 1, kBlack);
	Graphic_setup(pTGE_mean_XP_95,	3, 22, kMagenta+4, 1, kBlack);
	pTGE_mean_WF_55->				Draw("ap");
	pTGE_mean_WF_5->				Draw("p same");
	pTGE_mean_WF_95->				Draw("p same");
	leg->						AddEntry(pTGE_mean_WF_95, "WF Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_mean_WF_55, "WF Z = 55 cm", "ep");	
	leg->						AddEntry(pTGE_mean_WF_5, "WF Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_WF.c_str());
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_mean_XP_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_mean_XP_55->				SetMinimum(600);
	pTGE_mean_XP_55->				SetMaximum(1200);
	pTGE_mean_XP_55->				SetNameTitle("pTGE_mean_XP_5", "Mean vs #varphi angle;#varphi angle (#circ);mean (ADC count)");
	pTGE_mean_XP_55->				Draw("ap");
	pTGE_mean_XP_5->				Draw("p same");
	pTGE_mean_XP_95->				Draw("p same");
	leg->						AddEntry(pTGE_mean_XP_95, "XP Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_mean_XP_55, "XP Z = 55 cm", "ep"); 
	leg->						AddEntry(pTGE_mean_XP_5, "XP Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_XP.c_str());

	// Standard deviation
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_std_WF_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_std_WF_55->				SetMinimum(40);
	pTGE_std_WF_55->				SetMaximum(100);
	pTGE_std_WF_55->				SetNameTitle("pTGE_std_WF_55", "Std vs #varphi angle;#varphi angle (#circ);std (ADC count)");
	Graphic_setup(pTGE_std_WF_5,	3, 20, kCyan+2,	1, kBlack);
	Graphic_setup(pTGE_std_WF_55,	3, 21, kOrange-3,	1, kBlack);
	Graphic_setup(pTGE_std_WF_95,	3, 22, kBlack,	1, kBlack);
	Graphic_setup(pTGE_std_XP_5,	3, 20, kGreen+1,	1, kBlack);
	Graphic_setup(pTGE_std_XP_55,	3, 21, kMagenta+2, 1, kBlack);
	Graphic_setup(pTGE_std_XP_95,	3, 22, kMagenta+4, 1, kBlack);
	pTGE_std_WF_55->				Draw("ap");
	pTGE_std_WF_5->				Draw("p same");
	pTGE_std_WF_95->				Draw("p same");
	leg->						AddEntry(pTGE_std_WF_95, "WF Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_std_WF_55, "WF Z = 55 cm", "ep");	
	leg->						AddEntry(pTGE_std_WF_5, "WF Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_WF_End.c_str());
	pTCanvas->					Clear();
	leg->						Clear();
	pTGE_std_XP_55->				GetXaxis()->SetLimits(-3, 48);
	pTGE_std_XP_55->				SetMinimum(40);
	pTGE_std_XP_55->				SetMaximum(100);
	pTGE_std_XP_55->				SetNameTitle("pTGE_std_XP_5", "Std vs #varphi angle;#varphi angle (#circ);std (ADC count)");
	pTGE_std_XP_55->				Draw("ap");
	pTGE_std_XP_5->				Draw("p same");
	pTGE_std_XP_95->				Draw("p same");
	leg->						AddEntry(pTGE_std_XP_95, "XP Z = 95 cm", "ep");	
	leg->						AddEntry(pTGE_std_XP_55, "XP Z = 55 cm", "ep"); 
	leg->						AddEntry(pTGE_std_XP_5, "XP Z = 5 cm", "ep");	
	leg->						Draw();
	pTCanvas->					SaveAs(OutputFile_XP_End.c_str());

	// Delete
	delete						pTCanvas;
	delete						leg;
	delete pTFile_5;
	delete pTFile_55;
	delete pTFile_95;

	v_h1f_WF_5.					clear();
	v_h1f_WF_55.					clear();
	v_h1f_WF_95.					clear();
	v_h1f_XP_5.					clear();
	v_tf1_WF_5.					clear();
	v_tf1_WF_55.					clear();
	v_tf1_WF_95.					clear();
	v_tf1_XP_5.					clear();
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Draw Energy scan
void DrawOut_Escan(const std::string& inputDir, const std::string& Comment)
{
	gStyle->			SetPadTickX(1);
	gStyle->			SetPadTickY(1);
	int npoint			= 14;
	std::string Tag[]	= { "CERN22_Escan_e+_0p5GeV",	"CERN22_Escan_e+_0p75GeV",	"CERN22_Escan_e+_1GeV",	"CERN22_Escan_e+_1p25GeV", "CERN22_Escan_e+_1p5GeV",
							"CERN22_Escan_mu_0p75GeV",	"CERN22_Escan_mu_1GeV",	"CERN22_Escan_mu_1p5GeV",
							"CERN22_Escan_pi_0p75GeV",	"CERN22_Escan_pi_1p25GeV",	"CERN22_Escan_pi_1p5GeV", 
							"CERN22_Escan_p+_1GeV",	"CERN22_Escan_p+_1p25GeV",	"CERN22_Escan_p+_1p5GeV"};

	float E_arr[]		= { 0.5,	0.75, 1,	1.25, 1.5,
							0.75, 1,	1.5,
							0.75, 1.25, 1.5,
							1,	1.25, 1.5};

	int n[]			= { 0,1,2,3,4,
							0,1,2,
							0,1,2,
							0,1,2,};

	std::vector<TF1*>		v_tf1_WF;
	std::vector<TF1*>		v_tf1_XP;

	for(int iE = 0; iE < npoint; iE++){
	TFile* ptfile			= new TFile((inputDir + Tag[iE] + "/3_" + Tag[iE] + "_dEdx" + Comment + ".root").c_str());
	v_tf1_WF.				push_back(ptfile->Get<TF1>("tf1_WFsum"));
	v_tf1_XP.				push_back(ptfile->Get<TF1>("tf1_XP"));
	delete ptfile;
	}

	std::string particles[] = {" e^{+}", " #mu^{+}", " #pi^{+}", " p"};
	std::vector<TGraphErrors*> v_pTGE_reso_WF;
	std::vector<TGraphErrors*> v_pTGE_reso_XP;
	std::vector<TGraphErrors*> v_pTGE_mean_WF;
	std::vector<TGraphErrors*> v_pTGE_mean_XP;
	std::vector<TGraphErrors*> v_pTGE_std_WF;
	std::vector<TGraphErrors*> v_pTGE_std_XP;

	std::vector<double>		v_mass{0.511e-3, 105.658e-3, 139.570e-3, 938.272e-3}; // GeV
	std::vector<TF1*>			v_tf1_BB;
	std::vector<TF1*>			v_tf1_BB_Claudio;
	std::vector<TF1*>			v_tf1_BB_THATReconstruction;


	for(int i = 0; i < 4; i++){
	v_pTGE_reso_WF.		push_back(new TGraphErrors());
	v_pTGE_reso_XP.		push_back(new TGraphErrors());
	v_pTGE_mean_WF.		push_back(new TGraphErrors());
	v_pTGE_mean_XP.		push_back(new TGraphErrors());
	v_pTGE_std_WF.			push_back(new TGraphErrors());
	v_pTGE_std_XP.			push_back(new TGraphErrors());
	v_tf1_BB.				push_back(BetheBlochExp(0.25, 20, v_mass[i], particles[i])); // m & P range in
	v_tf1_BB_Claudio.		push_back(BetheBlochExp(0.25, 20, v_mass[i], particles[i])); // m & P range in
	v_tf1_BB_THATReconstruction.		push_back(BetheBlochExp(0.25, 20, v_mass[i], particles[i])); // m & P range in
	// if(i==0) v_tf1_BB.		push_back(BetheBlochExp(0.25, 2, v_mass[i], particles[i])); // m & P range in GeV GeV
	// else	v_tf1_BB.		push_back(BetheBloch(0.25, 2, v_mass[i], particles[i])); // m & P range in GeV
	}
	// v_tf1_BB[0]->SetParameters(511e-6, -0.3, -0.1, 7.5, 2, 1);

	// Get mean & std
	float mean_WF[npoint];
	float mean_XP[npoint];
	float dmean_WF[npoint];
	float dmean_XP[npoint];
	float std_WF[npoint];
	float std_XP[npoint];
	float dstd_WF[npoint];
	float dstd_XP[npoint];

	float keV					= 5.9/(224*1703.74/183); // 5.9 Fe peak energy | 1703 mean MockUp gain | 224 e- created with 5.9keV | 183 e- for 1 ADC | 2.23 scaling correction (to be understood)

	int index;
	for(int iE = 0; iE < npoint; iE++){
	if(iE < 5)		index = 0;
	else if(iE < 8)	index = 1;
	else if(iE < 11)	index = 2;
	else if(iE < 14)	index = 3;

	mean_WF[iE]				= v_tf1_WF[iE]->	GetParameter(1);
	mean_XP[iE]				= v_tf1_XP[iE]->	GetParameter(1);
	std_WF[iE]					= v_tf1_WF[iE]->	GetParameter(2);
	std_XP[iE]					= v_tf1_XP[iE]->	GetParameter(2);
	dmean_WF[iE]				= v_tf1_WF[iE]->	GetParError(1);
	dmean_XP[iE]				= v_tf1_XP[iE]->	GetParError(1);
	dstd_WF[iE]				= v_tf1_WF[iE]->	GetParError(2);
	dstd_XP[iE]				= v_tf1_XP[iE]->	GetParError(2);

	v_pTGE_reso_WF[index]-> SetPoint		(n[iE], E_arr[iE], std_WF[iE]/mean_WF[iE]*100);
	v_pTGE_reso_XP[index]-> SetPoint		(n[iE], E_arr[iE], std_XP[iE]/mean_XP[iE]*100);
	v_pTGE_reso_WF[index]-> SetPointError (n[iE], 0,		GetResoError(v_tf1_WF[iE]));
	v_pTGE_reso_XP[index]-> SetPointError (n[iE], 0,		GetResoError(v_tf1_XP[iE]));

	v_pTGE_mean_WF[index]-> SetPoint		(n[iE], E_arr[iE], mean_WF[iE]*keV);
	v_pTGE_mean_XP[index]-> SetPoint		(n[iE], E_arr[iE], mean_XP[iE]*keV);
	v_pTGE_mean_WF[index]-> SetPointError (n[iE], 0,		dmean_WF[iE]*keV);
	v_pTGE_mean_XP[index]-> SetPointError (n[iE], 0,		dmean_XP[iE]*keV);

	v_pTGE_std_WF[index]->	SetPoint		(n[iE], E_arr[iE], std_WF[iE]*keV);
	v_pTGE_std_XP[index]->	SetPoint		(n[iE], E_arr[iE], std_XP[iE]*keV);
	v_pTGE_std_WF[index]->	SetPointError (n[iE], 0,		dstd_WF[iE]*keV);
	v_pTGE_std_XP[index]->	SetPointError (n[iE], 0,		dstd_XP[iE]*keV);
	}


	combinedFit(v_pTGE_mean_XP, v_tf1_BB);
	
	//	double paramTHATReconstruction[5] = {0.301580, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01}; // THATReconstruction raw normalized
	double paramTHATReconstruction[5] = {0.186543, 5.382656,	0.004234,	2.028548,	-0.994807};	// THATReconstruction fit
	//	double paramClaudio[5] = {0.148800, 6.047,		0.00064,	2.308,		-1.359};		// Claudio raw normalized
	double paramClaudio[5] = {0.186524, 5.382954,	0.004232,	2.028659,	-0.994959};	// Claudio fit
	for(int ipart = 0; ipart < (int)v_tf1_BB_Claudio.size(); ipart++){
	for(int iparam = 1; iparam < (int)v_tf1_BB_Claudio[0]->GetNpar(); iparam++){
		v_tf1_BB_Claudio[ipart]->SetParameter(iparam, paramClaudio[iparam-1]);
		v_tf1_BB_THATReconstruction[ipart]->SetParameter(iparam, paramTHATReconstruction[iparam-1]);
	}
	}

	// Draw
	std::string OutputFile		= inputDir + "CERN22_Escan" + Comment + "_aprBB4.pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	gStyle->						SetGridStyle(1);
	pTCanvas->					cd();
	pTCanvas->					SetRightMargin(0.05);
	float x_leg				= 0.77;
	float y_leg				= 0.73;
	float dx_leg				= 0.09;
	float dy_leg				= 0.145;
	TLegend* leg1				= new TLegend(x_leg, y_leg, x_leg+dx_leg, y_leg+dy_leg);
	leg1->						SetBorderSize(0);
	leg1->						SetFillStyle(0);
	TLegend* leg2				= new TLegend(x_leg+dx_leg, y_leg, x_leg+2*dx_leg, y_leg+dy_leg);
	leg2->						SetBorderSize(0);
	leg2->						SetFillStyle(0);

	Color_t colors[]	= {kMagenta+1, kBlue, kGreen+1, kRed};
	int	markers[] = {20, 21, 24, 25}; 
	// Resolution
	v_pTGE_reso_WF[0]->				GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_reso_WF[0]->				SetMinimum(4);
	v_pTGE_reso_WF[0]->				SetMaximum(10);
	v_pTGE_reso_WF[0]->				SetNameTitle("pTGE_reso_WF", "Resolution vs energy with WF method;Energy (GeV);resolution (%)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_reso_WF[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_reso_WF[i]->	Draw("ap");
	else v_pTGE_reso_WF[i]->			Draw("p same");
	if(i<2) leg1->					AddEntry(v_pTGE_reso_WF[i], particles[i].c_str(), "ep");	
	else	leg2->					AddEntry(v_pTGE_reso_WF[i], particles[i].c_str(), "ep");	
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile_Beg.c_str());

	pTCanvas->							Clear();
	v_pTGE_reso_XP[0]->				GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_reso_XP[0]->				SetMinimum(4);
	v_pTGE_reso_XP[0]->				SetMaximum(10);
	v_pTGE_reso_XP[0]->				SetNameTitle("pTGE_reso_XP", "Resolution vs energy with XP method;Energy (GeV);resolution (%)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_reso_XP[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_reso_XP[i]->	Draw("ap");
	else v_pTGE_reso_XP[i]->			Draw("p same");
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile.c_str());


	// Mean
	pTCanvas->							Clear();
	v_pTGE_mean_WF[0]->				GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_mean_WF[0]->				SetMinimum(0.5);
	v_pTGE_mean_WF[0]->				SetMaximum(3);
	v_pTGE_mean_WF[0]->				SetNameTitle("pTGE_mean_WF", "Mean vs energy with WF method;Energy (GeV);mean (keV/cm)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_mean_WF[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_mean_WF[i]->	Draw("ap");
	else v_pTGE_mean_WF[i]->			Draw("p same");
	v_tf1_BB[i]->				SetLineColor(colors[i]);
	v_tf1_BB_Claudio[i]->				SetLineColor(colors[i]);
	v_tf1_BB_Claudio[i]->				SetLineStyle(2);
	v_tf1_BB_THATReconstruction[i]->				SetLineColor(colors[i]);
	v_tf1_BB_THATReconstruction[i]->				SetLineStyle(3);
	v_tf1_BB[i]->				Draw("same");
	v_tf1_BB_Claudio[i]->				Draw("same");
	v_tf1_BB_THATReconstruction[i]->				Draw("same");
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile_Beg.c_str());

	pTCanvas->							Clear();
	v_pTGE_mean_XP[0]->				GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_mean_XP[0]->				SetMinimum(0.5);
	v_pTGE_mean_XP[0]->				SetMaximum(3);
	v_pTGE_mean_XP[0]->				SetNameTitle("pTGE_mean_XP", "Mean vs energy with XP method;Energy (GeV);mean (keV/cm)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_mean_XP[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_mean_XP[i]->	Draw("ap");
	else v_pTGE_mean_XP[i]->			Draw("p same");
	v_tf1_BB[i]->				Draw("same");
	v_tf1_BB_Claudio[i]->				Draw("same");
	v_tf1_BB_THATReconstruction[i]->				Draw("same");
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile.c_str());



	// Bethe-Bloch
	pTCanvas->							Clear();
	gPad->							SetLogx();
	gPad->							SetLogy();
	v_tf1_BB[0]->					SetMinimum(0.7);
	v_tf1_BB[0]->					SetMaximum(5);
	v_tf1_BB[0]->					SetTitle("Bethe-Bloch for different particles;Energy (GeV);mean (keV/cm)");
	v_tf1_BB[0]->					Draw();
	for(int i = 1; i < 4; i++)v_tf1_BB[i]-> Draw("same");
	for(int i = 0; i < 4; i++){
	v_tf1_BB_Claudio[i]-> Draw("same"); 
	v_tf1_BB_THATReconstruction[i]-> Draw("same"); 
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile.c_str());



	// Std
	pTCanvas->							Clear();
	gPad->							SetLogx(0);
	gPad->							SetLogy(0);
	v_pTGE_std_WF[0]->					GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_std_WF[0]->					SetMinimum(0.05);
	v_pTGE_std_WF[0]->					SetMaximum(0.15);
	v_pTGE_std_WF[0]->					SetNameTitle("pTGE_std_WF", "Std vs energy with WF method;Energy (GeV);std (keV/cm)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_std_WF[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_std_WF[i]->	Draw("ap");
	else v_pTGE_std_WF[i]->			Draw("p same");
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile.c_str());

	pTCanvas->							Clear();
	v_pTGE_std_XP[0]->					GetXaxis()->SetLimits(0.45, 1.55);
	v_pTGE_std_XP[0]->					SetMinimum(0.05);
	v_pTGE_std_XP[0]->					SetMaximum(0.15);
	v_pTGE_std_XP[0]->					SetNameTitle("pTGE_std_XP", "Std vs energy with XP method;Energy (GeV);std (keV/cm)");
	for(int i = 0; i < 4; i++){
	Graphic_setup(v_pTGE_std_XP[i], 3, markers[i], colors[i], 1, colors[i]);
	if(i == 0) v_pTGE_std_XP[i]->	Draw("ap");
	else v_pTGE_std_XP[i]->			Draw("p same");
	}
	leg1->								Draw();
	leg2->								Draw();
	pTCanvas->							SaveAs(OutputFile_End.c_str());


	// Delete
	delete						pTCanvas;
	delete						leg1;
	delete						leg2;

	for(int i = 0; i < (int)v_pTGE_mean_WF.size(); i++){
	delete v_pTGE_reso_WF[i]; v_pTGE_reso_WF[i] = 0;
	delete v_pTGE_reso_XP[i]; v_pTGE_reso_XP[i] = 0;
	delete v_pTGE_mean_WF[i]; v_pTGE_mean_WF[i] = 0;
	delete v_pTGE_mean_XP[i]; v_pTGE_mean_XP[i] = 0;
	delete v_pTGE_std_WF[i]; v_pTGE_std_WF[i] = 0;
	delete v_pTGE_std_XP[i]; v_pTGE_std_XP[i] = 0;
	delete v_tf1_BB[i]; v_tf1_BB[i] = 0;
	delete v_tf1_BB_Claudio[i]; v_tf1_BB_Claudio[i] = 0;
	delete v_tf1_BB_THATReconstruction[i]; v_tf1_BB_THATReconstruction[i] = 0;
	}
	v_pTGE_reso_WF.				clear();
	v_pTGE_reso_XP.				clear();
	v_pTGE_mean_WF.				clear();
	v_pTGE_mean_XP.				clear();
	v_pTGE_std_WF.				clear();
	v_pTGE_std_XP.				clear();
	v_tf1_BB.					clear();
	v_tf1_BB_Claudio.					clear();
	v_tf1_BB_THATReconstruction.					clear();
	v_tf1_WF.					clear();
	v_tf1_XP.					clear();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







// TGraphError of WFsum vs L plot for 30 to 45#circ
void DrawOut_TGE_WFsum_L(const std::string& inputDir, const std::string& Comment){
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);

	std::vector<TGraphErrors*>	v_TGE_z50;
	std::vector<TGraphErrors*>	v_TGE_z550;
	std::vector<TGraphErrors*>	v_TGE_z950;

	int phi_arr[] = {30, 40 ,45};
	for(int iphi = 0; iphi < (int)std::size(phi_arr); iphi++){
	TFile* TFile50			= TFile::Open(TString(inputDir + "DESY21_phi_zm40/DESY21_phi" + phi_arr[iphi] + "_diag_zm40/2_DESY21_phi" + phi_arr[iphi] + "_diag_zm40_Checks" + Comment + ".root"));
	TH2F* h2f_z50				= TFile50->Get<TH2F>("h2f_WFvsLength");
	TGraphErrors* TGE_z50	= Convert_TH2_TGE(h2f_z50);
	v_TGE_z50.					push_back(TGE_z50);
	TFile* TFile550			= TFile::Open(TString(inputDir + "DESY21_phi_z460/DESY21_phi" + phi_arr[iphi] + "_diag_z460/2_DESY21_phi" + phi_arr[iphi] + "_diag_z460_Checks" + Comment + ".root"));
	TH2F* h2f_550			= TFile550->Get<TH2F>("h2f_WFvsLength");
	v_TGE_z550.				push_back(Convert_TH2_TGE(h2f_550));
	TFile* TFile950			= TFile::Open(TString(inputDir + "DESY21_phi_z860/DESY21_phi" + phi_arr[iphi] + "_diag_z860/2_DESY21_phi" + phi_arr[iphi] + "_diag_z860_Checks" + Comment + ".root"));
	TH2F* h2f_950			= TFile950->Get<TH2F>("h2f_WFvsLength");
	v_TGE_z950.				push_back(Convert_TH2_TGE(h2f_950));
	delete h2f_z50;
	delete h2f_550;
	delete h2f_950;
	TFile50->Close();
	TFile550->Close();
	TFile950->Close();
	} 

	std::string OutputFile			= inputDir + "Phiscan_Fig16" + Comment + ".pdf";
	std::string OutputFile_Beg		= OutputFile + "(";
	std::string OutputFile_End		= OutputFile + ")";
	TCanvas* pTCanvas				= new TCanvas("pTCanvas", "pTCanvas", 1800, 1200);

	TLegend* leg					= new TLegend(0.83,0.75,0.97,0.94);
	pTCanvas->cd();
	// pTGE_reso_PRF->				GetXaxis()->SetLimits(0, 1000);
	// pTGE_reso_PRF->				SetMinimum(4);

	v_TGE_z50[0]->				SetMaximum(1800);
	v_TGE_z50[0]->					SetNameTitle("TGE_z50", "max of WF vs L_{cluster} for Z_{drift} = 50 mm;length in cluster L (mm);A_{max} (ADC counts)");
	Graphic_setup(v_TGE_z50[0], 3, 20, kBlack, 1, kBlack);
	Graphic_setup(v_TGE_z50[1], 3, 21, kBlue, 1, kBlack);
	Graphic_setup(v_TGE_z50[2], 3, 22, kRed, 1, kBlack);
	v_TGE_z50[0]->					Draw("ap");
	v_TGE_z50[1]->					Draw("p same");
	v_TGE_z50[2]->					Draw("p same");
	leg->							AddEntry(v_TGE_z50[0], "30#circ ", "ep");	
	leg->							AddEntry(v_TGE_z50[1], "40#circ ", "ep");	
	leg->							AddEntry(v_TGE_z50[2], "45#circ ", "ep");	
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile_Beg.c_str());
	leg->							Clear();

	v_TGE_z550[0]->				SetMaximum(1800);
	v_TGE_z550[0]->					SetNameTitle("TGE_z550", "max of WF vs L_{cluster} for Z_{drift} = 550 mm;length in cluster L (mm);A_{max} (ADC counts)");
	Graphic_setup(v_TGE_z550[0], 3, 20, kBlack, 1, kBlack);
	Graphic_setup(v_TGE_z550[1], 3, 21, kBlue, 1, kBlack);
	Graphic_setup(v_TGE_z550[2], 3, 22, kRed, 1, kBlack);
	v_TGE_z550[0]->					Draw("ap");
	v_TGE_z550[1]->					Draw("p same");
	v_TGE_z550[2]->					Draw("p same");
	leg->							AddEntry(v_TGE_z550[0], "30#circ ", "ep");	
	leg->							AddEntry(v_TGE_z550[1], "40#circ ", "ep");	
	leg->							AddEntry(v_TGE_z550[2], "45#circ ", "ep");	
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	leg->							Clear();

	v_TGE_z950[0]->				SetMaximum(1800);
	v_TGE_z950[0]->					SetNameTitle("TGE_z950", "max of WF vs L_{cluster} for Z_{drift} = 950 mm;length in cluster L (mm);A_{max} (ADC counts)");
	Graphic_setup(v_TGE_z950[0], 3, 20, kBlack, 1, kBlack);
	Graphic_setup(v_TGE_z950[1], 3, 21, kBlue, 1, kBlack);
	Graphic_setup(v_TGE_z950[2], 3, 22, kRed, 1, kBlack);
	v_TGE_z950[0]->					Draw("ap");
	v_TGE_z950[1]->					Draw("p same");
	v_TGE_z950[2]->					Draw("p same");
	leg->							AddEntry(v_TGE_z950[0], "30#circ ", "ep");	
	leg->							AddEntry(v_TGE_z950[1], "40#circ ", "ep");	
	leg->							AddEntry(v_TGE_z950[2], "45#circ ", "ep");	
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile_End.c_str());
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void DrawOut_verif(const std::string& OutDir, const std::string& Comment){

	gStyle->						SetOptStat(111111);
	gStyle->						SetStatX(0.33);
	gStyle->						SetStatY(0.89);

	// Get histograms
	std::vector<TFile*> v_TFile;
	std::vector<TH2F*>	v_TH2F;
	std::vector<TF1*>	v_tf1;
	std::string z[]		= {"m40", "460", "860"};
	for(int i = 0; i < 3; i++){
	v_TFile.push_back(TFile::Open(TString(OutDir + "z" + z[i] + "/DESY21_phi30_z" + z[i] + "/2_DESY21_phi30_z" + z[i] + "_Checks"+ Comment + ".root")));
	v_TFile.push_back(TFile::Open(TString(OutDir + "z" + z[i] + "/DESY21_phi30_diag_z" + z[i] + "/2_DESY21_phi30_diag_z" + z[i] + "_Checks"+ Comment + ".root")));
	}
	for(int i = 0; i < (int)v_TFile.size(); i++){
	v_TH2F.						push_back(v_TFile[i]->Get<TH2F>("h2f_WFvsLength"));
	// if(i%2 == 0) v_tf1.			push_back(0);
	// else v_tf1.					push_back(v_TFile[i]->Get<TF1>("A_corr"));
	}

	// Draw out
	std::string OutputFile		= "OUT_Reconstruction/DESY21_phi/Verif_vertVSdiag" + Comment + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";

	TCanvas* pTCanvas			= new TCanvas("pTCanvas", "pTCanvas", 2700, 1800);
	TLegend* leg					= new TLegend(0.78,0.8,0.89,0.89);
	pTCanvas->						cd();

	std::string ztrue[]			= {"50", "550", "950"};
	for(int i = 0; i < (int)v_TFile.size(); i++){
	pTCanvas->						Clear();
	if(i%2 == 0) v_TH2F[i]->		SetTitle(("30#circ vertical " + ztrue[i/2] + "mm: WF VS length in cluster;Length in cluster (mm);WF (ADC count)").c_str());
	else v_TH2F[i]->				SetTitle(("30#circ diagonal " + ztrue[i/2] + "mm: WF VS length in cluster;Length in cluster (mm);WF (ADC count)").c_str());
	v_TH2F[i]->					Draw("colz");
	// if(i%2 == 1) v_tf1[i]->		SetLineWidth(4);
	// if(i%2 == 1) v_tf1[i]->		Draw("same");
	v_TH2F[i]->					GetYaxis()->SetRangeUser(0, 2000);
	gPad->							Update();
	if(i == 0)		pTCanvas->		SaveAs(OutputFile_Beg.c_str());
	else if(i == 5) pTCanvas->		SaveAs(OutputFile_End.c_str());
	else			pTCanvas->		SaveAs(OutputFile.c_str());
	}

	
	gStyle->						SetStatX(0.99);
	gStyle->						SetStatY(0.99);
	gROOT->ForceStyle();
	pTCanvas->UseCurrentStyle();
	for(int i = 0; i < 3; i++){
	OutputFile					= "OUT_Reconstruction/DESY21_phi/Verif_vertVSdiag_z" + ztrue[i] + Comment + ".jpg";
	pTCanvas->						Clear();
	Graphic_setup					(v_TH2F[2*i],	1, 1, kBlue);
	Graphic_setup					(v_TH2F[2*i+1], 1, 1, kRed);
	v_TH2F[2*i+1]->				SetTitle(("30#circ " + ztrue[i] + "mm: WF VS length in cluster;Length in cluster (mm);WF (ADC count)").c_str());
	v_TH2F[2*i+1]->				Draw();
	v_TH2F[2*i+1]->				GetYaxis()->SetRangeUser(0, 2000);
	v_TH2F[2*i]->					Draw("same");
	TLegendEntry* L1 = leg->		AddEntry(v_TH2F[2*i], "vertical ", "p");	
	L1->							SetTextColor(kBlue);
	TLegendEntry* L2 = leg->		AddEntry(v_TH2F[2*i+1], "diagonal ", "p"); 
	L2->							SetTextColor(kRed); 
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	leg->							Clear();
	}

	delete pTCanvas;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void DrawOut_corrections(){

	gStyle->						SetOptStat(0);

	// Get histograms
	std::vector<TF1*>		v_tf1;
	std::vector<std::string>	v_filename;
	v_filename.push_back("../Data_DESY21/Phi_scan_zm40/phi_200_30_zm40_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z460/phi_200_30_z460_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z860/phi_200_30_z860_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_zm40/phi_200_40_zm40_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z460/phi_200_40_z460_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z860/phi_200_40_z860_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_zm40/phi_200_45_zm40_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z460/phi_200_45_z460_ym60_diag_iter0_WFmax_correction.root");
	v_filename.push_back("../Data_DESY21/Phi_scan_z860/phi_200_45_z860_ym60_diag_iter0_WFmax_correction.root");

	TF1* F_HATRecon				= new TF1("F_Vlada", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
	v_tf1.push_back(F_HATRecon);
	for(int i = 0; i < (int)v_filename.size(); i++){
	TFile* pfile = new TFile(v_filename[i].c_str(), "READ");
	v_tf1.push_back(pfile->Get<TF1>("A_corr"));
	pfile->Close();
	}

	// Draw out
	std::string OutputFile		= "OUT_Reconstruction/DESY21_phi/Correction_functions_WFmax_correction.pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";

	int z[]						= {5, 55, 95};
	TCanvas* pTCanvas			= new TCanvas("pTCanvas", "pTCanvas", 2700, 1800);
	TLegend* leg					= new TLegend(0.85,0.8,0.99,0.99);
	pTCanvas->						cd();
	v_tf1[0]->						SetLineColor(kGray);	
	v_tf1[0]->						SetLineWidth(4); 
	v_tf1[0]->						GetYaxis()->SetRangeUser(0, 1500);
	
	v_tf1[0]->						SetTitle("Correction functions 30#circ;L_{cluster} (mm);WF (ADC count)");
	leg->							AddEntry(v_tf1[0], "HATRecon" , "l");
	for (int i = 1; i < 4; i++){
	v_tf1[0]->					Draw();
	v_tf1[i]->					SetLineColor(kGreen+2*(i-1));
	v_tf1[i]->					SetLineWidth(4);
	v_tf1[i]->					SetLineStyle((i-1)%3+1);
	leg->						AddEntry(v_tf1[i], Form("%i cm", z[i-1]), "l");
	}
	for (int i = 1; i < 4; i++)v_tf1[i]->Draw("same");
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile_Beg.c_str());
	leg->							Clear();
	pTCanvas->						Clear();
	
	v_tf1[0]->						SetTitle("Correction functions 40#circ;L_{cluster} (mm);WF (ADC count)");
	leg->							AddEntry(v_tf1[0], "HATRecon" , "l");
	for(int i = 4; i < 7; i++){ 
	v_tf1[0]->					Draw();
	v_tf1[i]->					SetLineColor(kBlue+2*(i-4));
	v_tf1[i]->					SetLineWidth(4);
	v_tf1[i]->					SetLineStyle((i-1)%3+1);
	leg->						AddEntry(v_tf1[i], Form("%i cm", z[i-4]), "l");
	}
	for (int i = 4; i < 7; i++)v_tf1[i]->Draw("same");
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	leg->							Clear();
	pTCanvas->						Clear();
	
	v_tf1[0]->						SetTitle("Correction functions 45#circ;L_{cluster} (mm);WF (ADC count)");
	leg->							AddEntry(v_tf1[0], "HATRecon" , "l");
	for(int i = 7; i < 10; i++){ 
	v_tf1[0]->					Draw();
	v_tf1[i]->					SetLineColor(kRed+2*(i-7));
	v_tf1[i]->					SetLineWidth(4);
	v_tf1[i]->					SetLineStyle((i-1)%3+1);
	leg->						AddEntry(v_tf1[i], Form("%i cm", z[i-7]), "l");
	}
	for (int i = 7; i < 10; i++)v_tf1[i]->Draw("same");
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	leg->							Clear();
	pTCanvas->						Clear();
	

	TLegend* legAll				= new TLegend(0.86,0.65,0.99,0.99);
	int angle[]					= {30, 40, 45};
	v_tf1[1]->						SetTitle("Correction functions;L_{cluster} (mm);WF (ADC count)");
	v_tf1[1]->						GetYaxis()->SetRangeUser(0, 1500);
	v_tf1[1]->						GetXaxis()->SetRangeUser(0, 17);
	// legAll->						AddEntry(v_tf1[0], "HATRecon" , "l");
	for(int i = 1; i < 10; i++){ 
	v_tf1[1]->					Draw();
	// if(i>1)v_tf1[i]->			Draw("same");
	legAll->						AddEntry(v_tf1[i], Form("%i#circ %i cm", angle[(i-1)/3], z[(i-1)%3]), "l");
	}
	for (int i = 1; i < 10; i++)v_tf1[i]->Draw("same");
	legAll->						Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	pTCanvas->						Clear();
	

	float ref					=	v_tf1[4]->Eval(11.28);
	for(int i = 1; i < 10; i++){ 
	TGraph* tg					= new TGraph();
	tg->SetMaximum(1500);
	tg->SetMinimum(0);
	tg->SetTitle("F(L_{cluster})*F_{ref}(11.28)/F_{ref}(L_{cluster});L_{cluster} (mm);F(L_{cluster})*F_{ref}(11.28)/F_{ref}(L_{cluster}) (ADC count)");
	for(int j = 0; j < 100; j++) tg->SetPoint(j, j/100.*17., v_tf1[i]->Eval(j/100.*17.)*ref/v_tf1[4]->Eval(j/100.*17.));
	if(i>=1 && i<4) tg->		SetLineColor(kGreen+2*(i-1));
	if(i>=4 && i<7) tg->		SetLineColor(kBlue +2*(i-4));
	if(i>=7 && i<10)tg->		SetLineColor(kRed	+2*(i-7));
	tg->						SetLineWidth(4);
	tg->						SetLineStyle((i-1)%3+1);
	tg->GetXaxis()->SetLimits(0, 17);
	if(i==1)tg->				DrawClone("AL");
	else	tg->				DrawClone("L same");
	}
	legAll->						Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	pTCanvas->						Clear();
	legAll->						Clear();
	

	TLegend* legRatio			= new TLegend(0.75,0.6,0.99,0.94);
	Color_t col[]				= {kRed-9, kRed-7, kRed, kRed+2, kRed+4};
	v_tf1[4]->						SetParameter(0, v_tf1[4]->GetParameter(0)-200);
	for(int i = 0; i < 5; i++){ 
	TGraph* tg					= new TGraph();
	tg->							SetMaximum(4);
	tg->							SetMinimum(0);
	tg->							SetTitle("F_{ref}(11.28)/F_{ref}(L_{cluster}) with F_{ref} shifted;L_{cluster} (mm);ratio");
	v_tf1[4]->					SetParameter(0, v_tf1[4]->GetParameter(0)+i*100);
	ref						= v_tf1[4]->Eval(11.28);
	for(int j = 0; j < 100; j++) tg->SetPoint(j, j/100.*17., ref/v_tf1[4]->Eval(j/100.*17.));
	tg->							SetLineColor(col[i]);
	tg->							SetLineWidth(4);
	tg->GetXaxis()->				SetLimits(0, 17);
	if(i==0)tg->					DrawClone("AL");
	else	tg->					DrawClone("L same");
	legRatio->					AddEntry(tg, Form("%i ADC counts", i*100-200), "l");
	}
	legRatio->						Draw();
	pTCanvas->						SaveAs(OutputFile_End.c_str());
	delete pTCanvas;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// DrawOut dE/dx systematics with Z scan
void DrawOut_Systematics(const std::string& inputDir, const std::string& Comment, const std::string& scan)
{
	int sys_RC				= 1;
	int sys_Z				= 1;
	int sys_d				= 1;

	// Draw
	std::string OutputFile		= inputDir + "Systematics_" + scan + "scan" + Comment;
	if(sys_RC == 1) OutputFile	= OutputFile + "_RC";
	if(sys_Z	== 1) OutputFile	= OutputFile + "_Z";
	if(sys_d == 1) OutputFile	= OutputFile + "_d";
	OutputFile					= OutputFile + ".pdf";
	std::string OutputFile_Beg	= OutputFile + "(";
	std::string OutputFile_End	= OutputFile + ")";
	TCanvas* pTCanvas			= new TCanvas("TCanvas", "TCanvas", 1800, 1200);
	gStyle->						SetPadTickX(1);
	gStyle->						SetPadTickY(1);
	gStyle->						SetOptStat(0);
	// gStyle->						SetGridStyle(1);
	pTCanvas->						cd();
	TLegend* leg					= new TLegend(0.67,0.65,0.90,0.89);
	leg->							SetBorderSize(0);
	leg->							SetFillStyle(0);


	// Reference
	TFile* pTFile_XP;
	TGraphErrors* pTGE_reso_XP	= new TGraphErrors();
	TGraphErrors* pTGE_mean_XP	= new TGraphErrors();
	TGraphErrors* pTGE_std_XP	= new TGraphErrors();

	std::vector<int>			val;
	std::vector<std::string>	arr;
	int						xmin, xmax;
	if(scan == "Z"){
	val = {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
	arr = {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};
	xmin = 0;
	xmax = 1000;
	}
	if(scan == "phi"){
	val = {0, 5, 10, 20, 29, 31, 40, 45};
	arr = {"0", "5", "10", "20", "30", "30_diag", "40_diag", "45_diag"};
	xmin = -3;
	xmax = 48;
	}

	std::vector<std::string>	file;
	for(std::string tag: arr){
	if(scan == "Z")	file.push_back((inputDir + "DESY21_z"	+ tag + "_PT412/3_DESY21_z" + tag + "_PT412_dEdx").c_str());
	if(scan == "phi") file.push_back((inputDir + "DESY21_phi" + tag + "_z460/3_DESY21_phi" + tag + "_z460_dEdx").c_str());
	}
	if(scan=="phi") file[0] = "OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx";

	for(int i = 0; i < (int)file.size(); i++){
	pTFile_XP					= TFile::Open(TString(file[i] + Comment + ".root"));
	TF1* tf1_XP				= pTFile_XP->Get<TF1>("tf1_XP");
	pTGE_reso_XP->				SetPoint(	i,val[i], tf1_XP->GetParameter(2)/tf1_XP->GetParameter(1)*100);
	pTGE_reso_XP->				SetPointError(i,0,		GetResoError(tf1_XP));
	pTGE_mean_XP->				SetPoint(	i,val[i], tf1_XP->GetParameter(1));
	pTGE_mean_XP->				SetPointError(i,0,		tf1_XP->GetParError(1));
	pTGE_std_XP->				SetPoint(	i,val[i], tf1_XP->GetParameter(2));
	pTGE_std_XP->				SetPointError(i,0,		tf1_XP->GetParError(2));
	}



	// RC
	if(sys_RC){
	TFile* pTFile_RCm;
	std::vector<TF1*> v_tf1_RCm;
	TGraphErrors* pTGE_reso_RCm	= new TGraphErrors();
	TGraphErrors* pTGE_mean_RCm	= new TGraphErrors();
	TGraphErrors* pTGE_std_RCm	= new TGraphErrors();

	TFile* pTFile_RCp;
	std::vector<TF1*> v_tf1_RCp;
	TGraphErrors* pTGE_reso_RCp	= new TGraphErrors();
	TGraphErrors* pTGE_mean_RCp	= new TGraphErrors();
	TGraphErrors* pTGE_std_RCp	= new TGraphErrors();

	// TFile* pTFile_RCpm;
	// std::vector<TF1*> v_tf1_RCpm;
	// TGraphErrors* pTGE_reso_RCpm	= new TGraphErrors();
	// TGraphErrors* pTGE_mean_RCpm	= new TGraphErrors();
	// TGraphErrors* pTGE_std_RCpm	= new TGraphErrors();

	for(int i = 0; i < (int)file.size(); i++){
		pTFile_RCm					= TFile::Open(TString(file[i] + Comment + "_RCm2%.root"));
		TF1* tf1_RCm				= pTFile_RCm->Get<TF1>("tf1_XP");
		pTGE_reso_RCm->				SetPoint(	i,val[i], tf1_RCm->GetParameter(2)/tf1_RCm->GetParameter(1)*100);
		pTGE_reso_RCm->				SetPointError(i,0,		GetResoError(tf1_RCm));
		pTGE_mean_RCm->				SetPoint(	i,val[i], tf1_RCm->GetParameter(1));
		pTGE_mean_RCm->				SetPointError(i,0,		tf1_RCm->GetParError(1));
		pTGE_std_RCm->				SetPoint(	i,val[i], tf1_RCm->GetParameter(2));
		pTGE_std_RCm->				SetPointError(i,0,		tf1_RCm->GetParError(2));
		
		pTFile_RCp					= TFile::Open(TString(file[i] + Comment + "_RCp2%.root"));
		TF1* tf1_RCp				= pTFile_RCp->Get<TF1>("tf1_XP");
		pTGE_reso_RCp->				SetPoint(	i,val[i], tf1_RCp->GetParameter(2)/tf1_RCp->GetParameter(1)*100);
		pTGE_reso_RCp->				SetPointError(i,0,		GetResoError(tf1_RCp));
		pTGE_mean_RCp->				SetPoint(	i,val[i], tf1_RCp->GetParameter(1));
		pTGE_mean_RCp->				SetPointError(i,0,		tf1_RCp->GetParError(1));
		pTGE_std_RCp->				SetPoint(	i,val[i], tf1_RCp->GetParameter(2));
		pTGE_std_RCp->				SetPointError(i,0,		tf1_RCp->GetParError(2));
		
		// pTFile_RCpm					= TFile::Open(TString(file[i] + Comment + "_RCpm%.root"));
		// TF1* tf1_RCpm				= pTFile_RCpm->Get<TF1>("tf1_XP");
		// pTGE_reso_RCpm->				SetPoint(	i,val[i], tf1_RCpm->GetParameter(2)/tf1_RCpm->GetParameter(1)*100);
		// pTGE_reso_RCpm->				SetPointError(i,0,		GetResoError(tf1_RCpm));
		// pTGE_mean_RCpm->				SetPoint(	i,val[i], tf1_RCpm->GetParameter(1));
		// pTGE_mean_RCpm->				SetPointError(i,0,		tf1_RCpm->GetParError(1));
		// pTGE_std_RCpm->				SetPoint(	i,val[i], tf1_RCpm->GetParameter(2));
		// pTGE_std_RCpm->				SetPointError(i,0,		tf1_RCpm->GetParError(2));
	}
	// Resolution
	pTGE_reso_XP->					GetXaxis()->SetLimits(xmin, xmax);
	pTGE_reso_XP->					SetMinimum(4);
	pTGE_reso_XP->					SetMaximum(12);
	if(scan == "Z") pTGE_reso_XP->	SetNameTitle("pTGE_reso_XP", "Z scan systematics | RC | Resolution;drift distance (mm);resolution (%)");
	if(scan == "phi") pTGE_reso_XP-> SetNameTitle("pTGE_reso_XP", "#phi scan systematics | RC | Resolution;#phi angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_reso_RCm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_reso_RCp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_reso_RCpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_reso_XP->						Draw("ap");
	pTGE_reso_RCm->					Draw("p same");
	pTGE_reso_RCp->					Draw("p same");
	// pTGE_reso_RCpm->					Draw("p same");
	leg->								AddEntry(pTGE_reso_RCp, "RC + 2%", "ep");	
	leg->								AddEntry(pTGE_reso_XP, "RC true", "ep");	
	leg->								AddEntry(pTGE_reso_RCm, "RC - 2%", "ep");	
	// leg->								AddEntry(pTGE_reso_RCpm, "RC #pm 25%", "ep");	
	leg->								Draw();
	pTCanvas->							SaveAs(OutputFile_Beg.c_str());
	// Mean
	pTCanvas->						Clear();
	pTGE_mean_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_mean_XP->				SetMinimum(600);
	pTGE_mean_XP->				SetMaximum(1200);
	if(scan == "Z") pTGE_mean_XP-> SetNameTitle("pTGE_mean_XP", "Z scan systematics | RC | Mean;drift distance (mm);mean (ADC count)");
	if(scan == "phi") pTGE_mean_XP-> SetNameTitle("pTGE_mean_XP", "#phi scan systematics | RC | Mean;#phi angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_RCm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_mean_RCp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_mean_RCpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_mean_XP->				Draw("ap");
	pTGE_mean_RCm->					Draw("p same");
	pTGE_mean_RCp->				Draw("p same"); 
	// pTGE_mean_RCpm->				Draw("p same"); 
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	// Std
	pTCanvas->						Clear();
	pTGE_std_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_std_XP->				SetMinimum(40);
	pTGE_std_XP->				SetMaximum(100);
	if(scan == "Z") pTGE_std_XP->				SetNameTitle("pTGE_std_XP", "Z scan systematics | RC | Std;drift distance (mm);standard deviation (ADC count)");
	if(scan == "phi") pTGE_std_XP->				SetNameTitle("pTGE_std_XP", "#phi scan systematics | RC | Std;#phi angle (#circ);standard deviation (ADC count)");
	Graphic_setup(pTGE_std_RCm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_std_RCp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_std_RCpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_std_XP->				Draw("ap");
	pTGE_std_RCm->					Draw("p same");
	pTGE_std_RCp->				Draw("p same"); 
	// pTGE_std_RCpm->				Draw("p same"); 
	leg->							Draw();
	if(!sys_Z and !sys_d) pTCanvas->SaveAs(OutputFile_End.c_str());
	else					pTCanvas->SaveAs(OutputFile.c_str());
	}



	// Z drift
	if(sys_Z){
	TFile* pTFile_Zm;
	std::vector<TF1*> v_tf1_Zm;
	TGraphErrors* pTGE_reso_Zm	= new TGraphErrors();
	TGraphErrors* pTGE_mean_Zm	= new TGraphErrors();
	TGraphErrors* pTGE_std_Zm	= new TGraphErrors();

	TFile* pTFile_Zp;
	std::vector<TF1*> v_tf1_Zp;
	TGraphErrors* pTGE_reso_Zp	= new TGraphErrors();
	TGraphErrors* pTGE_mean_Zp	= new TGraphErrors();
	TGraphErrors* pTGE_std_Zp	= new TGraphErrors();

	// TFile* pTFile_Zpm;
	// std::vector<TF1*> v_tf1_Zpm;
	// TGraphErrors* pTGE_reso_Zpm	= new TGraphErrors();
	// TGraphErrors* pTGE_mean_Zpm	= new TGraphErrors();
	// TGraphErrors* pTGE_std_Zpm	= new TGraphErrors();


	for(int i = 0; i < (int)file.size(); i++){
	pTFile_Zm				= TFile::Open(TString(file[i] + Comment + "_Zm15mm.root"));
	TF1* tf1_Zm				= pTFile_Zm->Get<TF1>("tf1_XP");
	pTGE_reso_Zm->				SetPoint(	i,val[i], tf1_Zm->GetParameter(2)/tf1_Zm->GetParameter(1)*100);
	pTGE_reso_Zm->				SetPointError(i,0,		GetResoError(tf1_Zm));
	pTGE_mean_Zm->				SetPoint(	i,val[i], tf1_Zm->GetParameter(1));
	pTGE_mean_Zm->				SetPointError(i,0,		tf1_Zm->GetParError(1));
	pTGE_std_Zm->				SetPoint(	i,val[i], tf1_Zm->GetParameter(2));
	pTGE_std_Zm->				SetPointError(i,0,		tf1_Zm->GetParError(2));

	pTFile_Zp				= TFile::Open(TString(file[i] + Comment + "_Zp15mm.root"));
	TF1* tf1_Zp				= pTFile_Zp->Get<TF1>("tf1_XP");
	pTGE_reso_Zp->				SetPoint(	i,val[i], tf1_Zp->GetParameter(2)/tf1_Zp->GetParameter(1)*100);
	pTGE_reso_Zp->				SetPointError(i,0,		GetResoError(tf1_Zp));
	pTGE_mean_Zp->				SetPoint(	i,val[i], tf1_Zp->GetParameter(1));
	pTGE_mean_Zp->				SetPointError(i,0,		tf1_Zp->GetParError(1));
	pTGE_std_Zp->				SetPoint(	i,val[i], tf1_Zp->GetParameter(2));
	pTGE_std_Zp->				SetPointError(i,0,		tf1_Zp->GetParError(2));

	// pTFile_Zpm				= TFile::Open(TString(file[i] + Comment + "_Zpmcm.root"));
	// TF1* tf1_Zpm				= pTFile_Zpm->Get<TF1>("tf1_XP");
	// pTGE_reso_Zpm->				SetPoint(	i,val[i], tf1_Zpm->GetParameter(2)/tf1_Zpm->GetParameter(1)*100);
	// pTGE_reso_Zpm->				SetPointError(i,0,		GetResoError(tf1_Zpm));
	// pTGE_mean_Zpm->				SetPoint(	i,val[i], tf1_Zpm->GetParameter(1));
	// pTGE_mean_Zpm->				SetPointError(i,0,		tf1_Zpm->GetParError(1));
	// pTGE_std_Zpm->				SetPoint(	i,val[i], tf1_Zpm->GetParameter(2));
	// pTGE_std_Zpm->				SetPointError(i,0,		tf1_Zpm->GetParError(2));
	}
	pTCanvas->						Clear();
	leg->							Clear();
	// Resolution
	pTGE_reso_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_reso_XP->				SetMinimum(4);
	pTGE_reso_XP->				SetMaximum(12);
	if(scan == "Z") pTGE_reso_XP->				SetNameTitle("pTGE_reso_XP", "Z scan systematics | Z_{drift} | Resolution;drift distance (mm);resolution (%)");
	if(scan == "phi") pTGE_reso_XP->				SetNameTitle("pTGE_reso_XP", "#phi scan systematics | Z_{drift} | Resolution;#phi angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_Zm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_reso_Zp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_reso_Zpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_reso_XP->				Draw("ap");
	pTGE_reso_Zm->					Draw("p same");
	pTGE_reso_Zp->				Draw("p same");
	// pTGE_reso_Zpm->				Draw("p same");
	leg->							AddEntry(pTGE_reso_Zp, "Z + 15 mm", "ep");	
	leg->							AddEntry(pTGE_reso_XP, "Z true", "ep");	
	leg->							AddEntry(pTGE_reso_Zm, "Z - 15 mm", "ep");	
	// leg->							AddEntry(pTGE_reso_Zpm, "Z #pm 10 cm", "ep");	
	leg->							Draw();
	if(!sys_RC) pTCanvas->			SaveAs(OutputFile_Beg.c_str());
	else		pTCanvas->			SaveAs(OutputFile.c_str());
	// Mean
	pTCanvas->						Clear();
	pTGE_mean_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_mean_XP->				SetMinimum(600);
	pTGE_mean_XP->				SetMaximum(1200);
	if(scan == "Z") pTGE_mean_XP->				SetNameTitle("pTGE_mean_XP", "Z scan systematics | Z_{drift} | Mean;drift distance (mm);mean (ADC count)");
	if(scan == "phi") pTGE_mean_XP->				SetNameTitle("pTGE_mean_XP", "#phi scan systematics | Z_{drift} | Mean;#phi angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_Zm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_mean_Zp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_mean_Zpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_mean_XP->				Draw("ap");
	pTGE_mean_Zm->					Draw("p same");
	pTGE_mean_Zp->				Draw("p same"); 
	// pTGE_mean_Zpm->				Draw("p same"); 
	leg->							Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	// Std
	pTCanvas->						Clear();
	pTGE_std_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_std_XP->				SetMinimum(40);
	pTGE_std_XP->				SetMaximum(100);
	if(scan == "Z") pTGE_std_XP->				SetNameTitle("pTGE_std_XP", "Z scan systematics | Z_{drift} | Std;drift distance (mm);standard deviation (ADC count)");
	if(scan == "phi") pTGE_std_XP->				SetNameTitle("pTGE_std_XP", "#phi scan systematics | Z_{drift} | Std;#phi angle (#circ);standard deviation (ADC count)");
	Graphic_setup(pTGE_std_Zm, 3, 23, kBlue+1, 1, kBlack);
	Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_std_Zp, 3, 22, kRed+1, 1, kBlack);
	// Graphic_setup(pTGE_std_Zpm, 3, 20, kGreen+1, 1, kBlack);
	pTGE_std_XP->				Draw("ap");
	pTGE_std_Zm->					Draw("p same");
	pTGE_std_Zp->				Draw("p same"); 
	// pTGE_std_Zpm->				Draw("p same"); 
	leg->							Draw();
	if(!sys_d)	pTCanvas->			SaveAs(OutputFile_End.c_str());
	else		pTCanvas->			SaveAs(OutputFile.c_str());
	}



	// Impact parameter
	if(sys_d){
	TFile* pTFile_dpdd;
	TGraphErrors* pTGE_reso_dpdd	= new TGraphErrors();
	TGraphErrors* pTGE_mean_dpdd	= new TGraphErrors();
	TGraphErrors* pTGE_std_dpdd	= new TGraphErrors();

	TFile* pTFile_dmdd;
	TGraphErrors* pTGE_reso_dmdd	= new TGraphErrors();
	TGraphErrors* pTGE_mean_dmdd	= new TGraphErrors();
	TGraphErrors* pTGE_std_dmdd	= new TGraphErrors();


	for(int i = 0; i < (int)file.size(); i++){
		pTFile_dpdd					= TFile::Open(TString(file[i] + Comment + "_dpdd.root"));
		TF1* tf1_dpdd				= pTFile_dpdd->Get<TF1>("tf1_XP");
		pTGE_reso_dpdd->				SetPoint(	i,val[i], tf1_dpdd->GetParameter(2)/tf1_dpdd->GetParameter(1)*100);
		pTGE_reso_dpdd->				SetPointError(i,0,		GetResoError(tf1_dpdd));
		pTGE_mean_dpdd->				SetPoint(	i,val[i], tf1_dpdd->GetParameter(1));
		pTGE_mean_dpdd->				SetPointError(i,0,		tf1_dpdd->GetParError(1));
		pTGE_std_dpdd->				SetPoint(	i,val[i], tf1_dpdd->GetParameter(2));
		pTGE_std_dpdd->				SetPointError(i,0,		tf1_dpdd->GetParError(2));

		pTFile_dmdd					= TFile::Open(TString(file[i] + Comment + "_dmdd.root"));
		TF1* tf1_dmdd				= pTFile_dmdd->Get<TF1>("tf1_XP");
		pTGE_reso_dmdd->				SetPoint(	i,val[i], tf1_dmdd->GetParameter(2)/tf1_dmdd->GetParameter(1)*100);
		pTGE_reso_dmdd->				SetPointError(i,0,		GetResoError(tf1_dmdd));
		pTGE_mean_dmdd->				SetPoint(	i,val[i], tf1_dmdd->GetParameter(1));
		pTGE_mean_dmdd->				SetPointError(i,0,		tf1_dmdd->GetParError(1));
		pTGE_std_dmdd->				SetPoint(	i,val[i], tf1_dmdd->GetParameter(2));
		pTGE_std_dmdd->				SetPointError(i,0,		tf1_dmdd->GetParError(2));
	}
	
	leg->							Clear();
	pTCanvas->						Clear();
	// Resolution
	pTGE_reso_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_reso_XP->				SetMinimum(4);
	pTGE_reso_XP->				SetMaximum(12);
	if(scan == "Z") pTGE_reso_XP->				SetNameTitle("pTGE_reso_XP", "Z scan systematics | impact parameter d | Resolution;drift distance (mm);resolution (%)");
	if(scan == "phi") pTGE_reso_XP->				SetNameTitle("pTGE_reso_XP", "#phi scan systematics | impact parameter d | Resolution;#phi angle (#circ);resolution (%)");
	Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_reso_dpdd, 3, 22, kRed+1, 1, kBlack);
	Graphic_setup(pTGE_reso_dmdd, 3, 23, kBlue+1, 1, kBlack);
	pTGE_reso_XP->				Draw("ap");
	pTGE_reso_dpdd->					Draw("p same");
	pTGE_reso_dmdd->					Draw("p same");
	leg->						AddEntry(pTGE_reso_dpdd, "d + #Deltad", "ep");	
	leg->						AddEntry(pTGE_reso_XP, "d true", "ep"); 
	leg->						AddEntry(pTGE_reso_dmdd, "d - #Deltad", "ep");	
	leg->						Draw();
	if(!sys_RC and !sys_Z) pTCanvas->SaveAs(OutputFile_Beg.c_str());
	else					pTCanvas->SaveAs(OutputFile.c_str());
	// Mean
	pTCanvas->						Clear();
	pTGE_mean_XP->				GetXaxis()->SetLimits(xmin, xmax);
	pTGE_mean_XP->				SetMinimum(600);
	pTGE_mean_XP->				SetMaximum(1200);
	if(scan == "Z") pTGE_mean_XP->				SetNameTitle("pTGE_mean_XP", "Z scan systematics | impact parameter d | Mean;drift distance (mm);mean (ADC count)");
	if(scan == "phi") pTGE_mean_XP->				SetNameTitle("pTGE_mean_XP", "#phi scan systematics | impact parameter d | Mean;#phi angle (#circ);mean (ADC count)");
	Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_mean_dpdd, 3, 22, kRed+1, 1, kBlack);
	Graphic_setup(pTGE_mean_dmdd, 3, 23, kBlue+1, 1, kBlack);
	pTGE_mean_XP->				Draw("ap");
	pTGE_mean_dpdd->					Draw("p same");
	pTGE_mean_dmdd->					Draw("p same");
	leg->						Draw();
	pTCanvas->						SaveAs(OutputFile.c_str());
	// Std
	pTCanvas->						Clear();
	pTGE_std_XP->					GetXaxis()->SetLimits(xmin, xmax);
	pTGE_std_XP->					SetMinimum(40);
	pTGE_std_XP->					SetMaximum(100);
	if(scan == "Z") pTGE_std_XP->					SetNameTitle("pTGE_std_XP", "Z scan systematics | impact parameter d | Std;drift distance (mm);standard deviation (ADC count)");
	if(scan == "phi") pTGE_std_XP->					SetNameTitle("pTGE_std_XP", "#phi scan systematics | impact parameter d | Std;#phi angle (#circ);standard deviation (ADC count)");
	Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+1, 1, kBlack);
	Graphic_setup(pTGE_std_dpdd, 3, 22, kRed+1, 1, kBlack);
	Graphic_setup(pTGE_std_dmdd, 3, 23, kBlue+1, 1, kBlack);
	pTGE_std_XP->					Draw("ap");
	pTGE_std_dpdd->					Draw("p same");
	pTGE_std_dmdd->					Draw("p same");
	leg->						Draw();
	pTCanvas->						SaveAs(OutputFile_End.c_str());
	}

	delete						pTCanvas;
	delete						leg;
}