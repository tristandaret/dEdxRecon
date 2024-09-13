#include "Util.h"
#include "Misc.h"

#include "Monitoring.h"
#include "ReconTools.h"
#include "Variables.h"

#include "Control.h"

#include "Selector.h"


namespace Reconstruction{
	Reconstruction::dEdx		*p_dEdx;
	Reconstruction::DrawOuts	*p_DrawOuts;
	Reconstruction::LUT			*p_lut;
	Uploader					*p_uploader;
	
	// Files to use
	int prototype =				0;
	int CERN_Escan =			1;
	int CERN_drift = 			0;

	int DESY_drift =			0;
	int DESY_row =				0;
	int DESY_phi =				0;
	int DESY_theta =			0;

	// Computations
	int correction_wf =			0;
	int dedx =					0;

	// DrawOuts		
	int DO_Control =			1;
	int DO_dEdx =				0;
	int DO_Comparison =			0;

	int DO_DESY21SingleScan =	0;
	int DO_DESY21MultScan =		0;
}



void Reconstruction::Monitoring()
{
	comment = "_5k";
	gErrorIgnoreLevel = kInfo;

	Correction(1,1,1,1,0);


	// Energy scan using the prototype (ERAM 18)
	if(prototype){
		Settings("CERN22", "", "Prototype", "", "Energy (GeV)", 2, -1, 412, 350, 415, 40);

		int part_arr[] = {1,2,5,6};
		for (int iFile : part_arr) {
			if (iFile == 0) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_e+_0p5GeV_iter0.root");							v_tags.push_back("e+_0p5GeV");	}
			if (iFile == 1) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_e+_1GeV_iter0.root");							v_tags.push_back("e+_1GeV");	}
			if (iFile == 2) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_p_1GeV_iter0.root");							v_tags.push_back("p_1GeV");		}
			if (iFile == 3) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_mu-_1GeV_iter0.root");							v_tags.push_back("mu-_1GeV");	}
			if (iFile == 4) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_mu+_0p75GeV_iter0.root");						v_tags.push_back("mu+_0p75GeV");}
			if (iFile == 5) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_mu+_1GeV_iter0.root");							v_tags.push_back("mu+_1GeV");	}
			if (iFile == 6) { v_datafiles.push_back(data_scanpath + "ERAM18_350V_412ns_pi+_0p5GeV_iter0.root");						v_tags.push_back("pi+_0p5GeV");	}
		}
	}

	// Energy scan using the mockup
	if(CERN_Escan){
		Settings("CERN22", "", "Energy", "", "Energy (GeV)", 4, -1, 412, 350, 415, 40);
		v_valint.	insert(v_valint.end(), {0.5, 0.75, 1, 1.25, 1.5, 0.75, 1, 1.5, 0.75, 1.25, 1.5, 1, 1.25, 1.5});
		v_valstr.	insert(v_valstr.end(), {"e+_0p5GeV",	"e+_0p75GeV",	"e+_1GeV",		"e+_1p25GeV", "e+_1p5GeV",	"mu+_0p75GeV",	"mu+_1GeV", 
											"mu+_1p5GeV",	"pi+_0p75GeV",	"pi+_1p25GeV",	"pi+_1p5GeV", "p+_1GeV",	"p+_1p25GeV",	"p+_1p5GeV"});

		std::vector<std::string> v_suffix = {	"_25V_z415_y2pad_iter0.root",	"_25V_z415_y2pad_iter0.root",		"_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_1_iter0.root",
												"_25V_z415p1_y2pad_iter0.root", "_25V_z415_y2pad_iter0.root",		"_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_iter0.root", 
												"_25V_z415_y2pad_iter0.root",	"_25V_z415p1_y2pad_2_iter0.root",	"_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_iter0.root", 
												"_25V_z415p1_y2pad_iter0.root",	"_25V_z415_y2pad_2_iter0.root"};

		// for(int iEnergy = 0; iEnergy < (int)std::size(v_valstr); iEnergy++){
		for(int iEnergy = 0; iEnergy < 1; iEnergy++){
			v_datafiles.push_back(data_scanpath + "All_ERAMS_350V_412ns_" + v_valstr[iEnergy] + v_suffix[iEnergy]);
			v_tags.push_back(Form("%s_%s_%s", testbeam.c_str(), scan.c_str(), v_valstr[iEnergy].c_str()));

			DefaultAnalysis();
		}
		ClearVectors();
	}


	// Drift distance scan scan using the mockup
	if(CERN_drift){
		Settings("CERN22", "", "Drift", "", "Drift distance (mm)", 3, -1, 412, 350, 415, 40);

		v_valint.	insert(v_valint.end(), {60, 218, 415, 925});
		v_valstr.	insert(v_valstr.end(), {"60", "218p5", "415", "925"});

		int NFiles = 4;
		for (int iz = 0; iz < NFiles; iz++){
			const char* z = 	v_valstr[iz].c_str();
			driftDist = 		v_valint[iz];
			v_datafiles.		push_back(data_scanpath + "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z" + z + "_iter4.root");	
			v_tags.				push_back(Form("Drift%s", z));

			DefaultAnalysis();
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// z scan scan with DESY21
	if(DESY_drift){
		int PT_arr[] = 				{200, 412}; // 200, 412
		v_valint.					insert(v_valint.end(), {50, 150, 250, 350, 450, 550, 650, 750, 850, 950});
		v_valstr.					insert(v_valstr.end(), {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"});
		for (int iPT : PT_arr){
			PT = 					iPT; // PT is a shared variable in the namespace so it's important to update it here
			Settings("DESY21", "Drift", Form("Drift_PT%i", PT), Form("%i ns peak", PT), "Drift distance (mm)", 1, 0, PT, 310, 450, 50);

			for (int iz = 0; iz < (int)std::size(v_valstr); iz++){
				const char* z = 	v_valstr[iz].c_str();
				driftDist = 		v_valint[iz];
				v_datafiles.		push_back(data_scanpath + Form("Drift_scan_PT%i/z_360_275_%i_02T_26_%s_iter9.root", iPT, iPT, z)); v_tags.push_back(Form("%s_%s_Z%s", testbeam.c_str(), scan.c_str(), z));

				DefaultAnalysis();
			}
			if(DO_DESY21SingleScan) DrawSingleScan();
			scanindex++;
		}
		if(DO_DESY21MultScan) DrawMultipleScan();
		ClearVectors();
	}
	


	// y scan scan with DESY21
	if(DESY_row){
		Settings("DESY21", "", "Row", "", "Vertical beam position (mm)", 1, 0, 412, 310, 90, 50);
		v_valint.					insert(v_valint.end(), {-140, -120, -100, -80, -60, -40, 0, 20, 40, 60, 80});
		v_valstr.					insert(v_valstr.end(), {"m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"});

		for (int y = 0; y < (int)std::size(v_valstr); y++){
			v_datafiles.push_back(data_scanpath + Form("Row_scan/Y%s_Z0_iter9.root", v_valstr[y].c_str())); v_tags.push_back(Form("%s_%s_Y%s", testbeam.c_str(), scan.c_str(), v_valstr[y].c_str()));
			DefaultAnalysis();
		}
		if(DO_DESY21SingleScan) DrawSingleScan();
		ClearVectors();
	}
	


	// Phi scan with DESY21
	if(DESY_phi){
		std::vector<int> v_valdrift =			{50, 550, 950};
		std::vector<std::string> v_strdrift =	{"m40", "460", "860"};
		std::vector<int> v_valintreal =			{0, 5, 10, 20, 30, 30, 40, 45};
		v_valint.								insert(v_valint.end(), {0, 5, 10, 20, 29, 31, 40, 45});
		
		for (int iz = 0; iz < (int)v_valdrift.size(); iz++){
		// for (int iz = 0; iz < 1; iz++){
			const char* z = v_strdrift[iz].c_str();
			Settings("DESY21", "Phi", Form("Phi_Z%s", z), Form("%2i cm drift", v_valdrift[iz]/10), "Phi angle (#circ)", 1, 0, 200, 310, v_valdrift[iz], 40);
			
			for (int ifile = 0; ifile < (int)std::size(v_valintreal); ifile++){
			// for (int ifile = 5; ifile < 6; ifile++){
				int phi = v_valintreal[ifile];
				if(ifile == 0){		v_datafiles.push_back(data_scanpath + Form("Drift_scan_PT200/z_360_275_200_02T_26_%s_iter9.root", z));		v_tags.push_back(Form("%s_%s_Phi%i",		testbeam.c_str(), scan.c_str(), phi));}
				else if(ifile < 5){	v_datafiles.push_back(data_scanpath + Form("Phi_scan_z%s/phi_200_%i_z%s_ym60_iter9.root", z, phi, z));		v_tags.push_back(Form("%s_%s_Phi%i",		testbeam.c_str(), scan.c_str(), phi));}
				else{				v_datafiles.push_back(data_scanpath + Form("Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter9.root", z, phi, z));	v_tags.push_back(Form("%s_%s_Phi%i_diag",	testbeam.c_str(), scan.c_str(), phi));}

				DefaultAnalysis();
			}
			if(DO_DESY21SingleScan) DrawSingleScan();
			scanindex++;
		}
		if(DO_DESY21MultScan) DrawMultipleScan();
		ClearVectors();
	}
	


	// Theta scan with DESY21
	if(DESY_theta){
		Settings("DESY21", "", "Theta", "", "Theta angle (#circ)", 1, 0, 200, 310, 350, 40);
		selectionSet = "Sel_DESY21_theta";
		v_valint.					insert(v_valint.end(), {-45, -20, 20});
		v_valstr.					insert(v_valstr.end(), {"m45", "m20", "20"});

		for (int ifile = 0; ifile < (int)std::size(v_valint); ifile++){
			const char* theta = v_valstr[ifile].c_str();
			v_datafiles.push_back(data_scanpath + Form("Theta_scan/theta_%s_02T_z460_ym60_iter9.root", theta)); v_tags.push_back(Form("%s_%s_%s",	testbeam.c_str(), scan.c_str(), theta));
			
			DefaultAnalysis();
		}
		if(DO_DESY21SingleScan) DrawSingleScan();
		ClearVectors();
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Drawing
	if(DO_Comparison){
		v_tags.push_back("");
		std::string tag =		"CERN22_Escan_mu_1p5GeV";
		drawout_scanpath =		drawout_path + "CERN22_Energy/" + tag + "/";

		std::string comment1 = 	"";
		std::string comment2 = 	"_RCMod_GainMod";

		v_comments.				clear();
		v_comments.				push_back(comment1);
		v_comments.				push_back(comment2);

		v_datafiles.			clear();
		v_datafiles.			push_back(drawout_scanpath + "dEdx_" + tag + comment1 + ".root");
		v_datafiles.			push_back(drawout_scanpath + "dEdx_" + tag + comment2 + ".root");

		p_DrawOuts =			new DrawOuts(v_datafiles);
		p_DrawOuts->				FileComparison();
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Configuration functions

void Reconstruction::Correction(const int& corrRC, const int& corrGain, const int& corrWF, const int& corrDrift, const int& saveSelectOnly){
	fcorrectRC =				corrRC;
	if(corrRC == 0)	comment =	comment + "_RCoff";
	if(corrRC == 2)	comment =	comment + "_RCMod";

	fcorrectGain =				corrGain;
	if(corrGain == 0) comment =	comment + "_Gainoff";
	if(corrGain == 2) comment =	comment + "_GainMod";

	fcorrectWF =				corrWF;
	if(corrWF == 0)	comment =	comment + "_WF0";

	fcorrectDrift =				corrDrift;
	if(corrDrift == 0)comment =	comment + "_DriftFile";

	fsaveSelectOnly =			saveSelectOnly;
	if(saveSelectOnly == 0) comment =	comment + "_wDiscard";	
}

void Reconstruction::Settings(const std::string &testbeam_name, const std::string &metascan_name, const std::string &scan_name, const std::string &scanspec, const std::string &runvarstr_name, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin){

	testbeam = 								testbeam_name;
	metascan = 								metascan_name;
	scan = 									scan_name;
	v_scanspec.								push_back(scanspec);
	runvarstr =								runvarstr_name;
	selectionSet =							"Sel_" + testbeam;
	data_scanpath =							data_path + "Data_" + 	testbeam + "/";
	drawout_metascanpath =					drawout_path + 			testbeam + "_" + metascan + "/";
	if(metascan != "") drawout_scanpath =	drawout_metascanpath + 	testbeam + "_" + scan + "/";
	else drawout_scanpath =					drawout_path + 			testbeam + "_" + scan + "/";
	MakeMyDir(drawout_scanpath);

	intUploader =							uploader;
	moduleCase =							modules;
	PT =									peaking_time;
	Dt =									diffusion;
	driftDist =								drift_dist;
	TB =									timbin;

	if(dedx) p_lut =						new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
}

void Reconstruction::ClearVectors(){
	v_valint.					clear();
	v_valstr.					clear();
	v_scanspec.					clear();
	v_comments.					clear();
	v_tags.						clear();
	v_datafiles.				clear();
	v_rootout_files.			clear();
	scanindex =					0;
}

void Reconstruction::DefaultAnalysis(){
	tag = 						v_tags.back();
	drawout_runpath =			drawout_scanpath + tag + "/";
	MakeMyDir(drawout_runpath);
	rootout_file =				drawout_runpath + tag + comment + ".root";
	if(scan.find("Phi") != std::string::npos and tag.find("diag") == std::string::npos)
	{
		if(comment.find("WF0")		!= std::string::npos) rootout_file.erase(rootout_file.find("_WF0"), 4);
		if(comment.find("_corrv2")	!= std::string::npos) rootout_file.erase(rootout_file.find("_corrv2"), 7);
	}
	v_rootout_files.			push_back(rootout_file);

	if(correction_wf)
	{
		p_uploader =			GiveMe_Uploader (intUploader, v_datafiles.back());
		Reconstruction::WFCorrection(v_datafiles.back());
		delete 					p_uploader;
	}

	if(dedx)
	{
		log_file = 				drawout_runpath + tag + ".log";
		std::cout << "logs:		" << log_file << std::endl;
		p_uploader =			GiveMe_Uploader (intUploader, v_datafiles.back());
		p_dEdx =				new dEdx();
		p_dEdx->				Reconstruction();
		delete					p_uploader;
		delete					p_dEdx;
	}

	if(DO_Control)
	{
		drawout_file =			drawout_runpath + "Control_" + tag + comment + ".pdf";
		std::cout << "rootout:	" << rootout_file << std::endl;
		std::cout << "drawout:	" << drawout_file << std::endl;
		p_DrawOuts =			new DrawOuts(rootout_file);
		p_DrawOuts->			Control();
		delete 					p_DrawOuts;
	}

	if(DO_dEdx)
	{
		drawout_file =			drawout_runpath + "dEdx_" + tag + comment + ".pdf";
		p_DrawOuts =			new DrawOuts(rootout_file);
		p_DrawOuts->			EnergyLoss();
		delete 					p_DrawOuts;
	}
}

void Reconstruction::DrawSingleScan(){

	p_DrawOuts =				new DrawOuts(v_rootout_files);
	p_DrawOuts->				DESY21Scan("single");
	delete  					p_DrawOuts;
}

void Reconstruction::DrawMultipleScan(){

	p_DrawOuts =				new DrawOuts(v_rootout_files);
	p_DrawOuts->				DESY21Scan("multiple");
	delete  					p_DrawOuts;
}












////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//	int DESY_ExB = 0;
//	int DESY_drift_139V = 0; 
//	int MC_zscan = 0; 
//	int DESY19_phi = 0; 
//	int MockUp_zdist = 0; 



//	// ExB scan scan with DESY21
//	if(DESY_ExB){
//	selectionSet = "Sel_DESY21";
//	drawout_scanpath = drawout_path + "/DESY21_ExB/";	
//	MakeMyDir(drawout_scanpath); 
//	intUploader = 1;
//	moduleCase =	0;
//	PT = 200; Dt = 310;; TB = 40;
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	std::string B_arr[] = {"0", "04", "06"};
//	std::string Z_arr[] = {"60", "460", "860"};
//	int v_valint[] = {150, 550, 950};
//	for(int j = 0; j < 3; j++){
//		const char* B = B_arr[j].c_str();
//		for(int i = 0; i < 3; i++){
//		const char* z = Z_arr[i].c_str();
//		driftDist = v_valint[i];
//		// v_datafiles.push_back(Form			("../Data/Data_DESY21/ExB_scan/ExB_360_412ns_B02_ym10_z%s_iter9.root", z)); v_tags.push_back(Form("DESY21_ExB02_Z%s", z)); v_prtcles.push_back(Form("electron_ExB02_Z%s", z);
//		v_datafiles.push_back(Form			("../Data/Data_DESY21/ExB_scan/ExB_360_phim3_200ns_B%s_ym60_z%s_iter0.root", B, z)); v_tags.push_back(Form("DESY21_ExB%s_Z%s", B, z)); v_prtcles.push_back(Form("electron_ExB%s_Z%s", B, z));
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)				Control (drawout_scanpath, v_tags.back(), comment, v_datafiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)			DrawOut_Control (drawout_scanpath, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)				p_dEdx->Reconstruction();
//		if (DO_dEdx)			DrawOut_Methods (drawout_scanpath, v_tags.back(), comment, 1, v_prtcles.back());
//		if (DO_Checks)				DrawOut_Checks (drawout_scanpath, v_datafiles.back(), v_tags.back(), comment);
//		}
//	}
//	}





//	// Phi scan with DESY19
//	if(DESY19_phi){
//	selectionSet = "Sel_DESY21";
//	intUploader = 1;
//	moduleCase =	0;
//	PT = 412; Dt = 310; TB = 40; driftDist = 430;
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	int phi_val[] = {0, 10, 20, 30, 40, 45};
//	int NFiles = 6;
//	drawout_scanpath = drawout_path + "/DESY19_phi/";
//	MakeMyDir(drawout_scanpath); 
//	for (int ifile = 3; ifile < NFiles; ifile++){
//		int phi = phi_val[ifile];
//		if(ifile < 3) { v_datafiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi));		v_tags.push_back(Form("DESY19_phi%i", phi));		v_prtcles.push_back(Form("electron_phi%i", phi));}
//		else			{ v_datafiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi));	v_tags.push_back(Form("DESY19_phi%i_diag", phi)); v_prtcles.push_back(Form("electron_phi%i_diag", phi));}
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)		Control		(drawout_scanpath, v_tags.back(), comment, v_datafiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanpath, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(drawout_scanpath, v_datafiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanpath, v_tags.back(), comment, 1, v_prtcles.back());
//		delete p_uploader;
//	}
//	}




//	// z scan with MC
//	if(MC_zscan){
//	selectionSet = "TMC_CERN22_Event";
//	drawout_scanpath = drawout_path + "/CERN23_MC/MC_zscan/";	
//	MakeMyDir(drawout_scanpath); 
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; Dt = 286; TB = 40; // PT = 400 actually
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	int zmax = 9;
//	for (int zDrift = 0; zDrift < zmax; zDrift++){
//		// v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v2_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm", zDrift+1);		
//		v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm NoOpt", zDrift+1));		
//		// v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00_old", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm old", zDrift+1);		
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)		Control		(drawout_scanpath, v_tags.back(), comment, v_datafiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanpath, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(drawout_scanpath, v_datafiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanpath, v_tags.back(), comment, 1, v_prtcles.back());
//	}
//	}
	



//	// z scan scan using the mockup
//	if(MockUp_zdist){
//	selectionSet = "Sel_CERN22";
//	drawout_scanpath = drawout_path + "/CERN22_Mockup_zscan/";	
//	MakeMyDir(drawout_scanpath); 
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; Dt = 350; driftDist = 415; TB = 40;

//	int NFiles = 10;
//	for (int iFile = 0; iFile < NFiles; iFile++){
//		if (iFile == 0) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z060");}
//		if (iFile == 1) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_14V_z218");}
//		if (iFile == 2) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z415");}
//		if (iFile == 3) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z572");}
//		if (iFile == 5) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z060");}
//		if (iFile == 6) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_25V_z218");}
//		if (iFile == 7) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z415");}
//		if (iFile == 8) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z572");}
//		if (iFile == 9) { v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z925");}
//		if (DO_control)	DrawOut_Control			(drawout_scanpath, v_tags.back(), comment, selectionSet, 8);
//	}
//	}
	


//	// z scan scan with DESY21 at 139 V/cm
//	if(DESY_drift_139V){
//	selectionSet = "Sel_DESY21";
//	intUploader = 1;
//	moduleCase =	0; 
//	Dt = 310; TB = 50;
//	int PT_arr[] = {200};
//	for (int PT : PT_arr){
//		drawout_scanpath = drawout_path + Form("/DESY21_Drift/DESY21_Drift_PT%i_139V/", PT); 
//		MakeMyDir(drawout_scanpath); 
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//		int NFiles = 9;
//		for (int zDrift = -1; zDrift < NFiles; zDrift++){
//		if(zDrift == -1) {v_datafiles.push_back(data_scanpath + Form("zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT));			v_tags.push_back(Form("DESY21_zm40_PT%i", PT));			}
//		else {v_datafiles.push_back(data_scanpath + Form("zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift)); v_tags.push_back(Form("DESY21_z%i60_PT%i", zDrift, PT)); v_prtcles.push_back(Form("electron_z%i60", zDrift));}
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)		Control		(drawout_scanpath, v_tags.back(), comment, v_datafiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanpath, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanpath, v_tags.back(), comment, 1, v_prtcles.back());
//		}
//		if(DO_Resolution)	DrawOut_Zscan	(drawout_path + Form("/DESY21_Drift/DESY21_Drift_PT%i_139V", PT), comment, PT);
//	}
//	}