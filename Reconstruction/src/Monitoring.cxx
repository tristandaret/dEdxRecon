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
	int prototype =			0;
	int CERN_Escan =		0;
	int CERN_drift = 		1;

	int DESY_drift =		0;
	int DESY_yscan =		0;
	int DESY_phi =			0;
	int DESY_theta =		0;

	// Computations	
	int dedx =				1;

	// DrawOuts	
	int DO_dEdx =			0;
	int DO_Comparison =		0;
}



void Reconstruction::Monitoring()
{
	comment = "_test";
	gErrorIgnoreLevel = kInfo;

	Correction(1,1,1,1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Energy scan using the prototype (ERAM 18)
	if(prototype){
		Settings("CERN22", "Prototype", 2, -1, 412, 350, 415, 40);

		int part_arr[] = {1,2,5,6};
		for (int iFile : part_arr) {
			if (iFile == 0) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_e+_0p5GeV_iter0.root");							v_tags.push_back("e+_0p5GeV");	}
			if (iFile == 1) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_e+_1GeV_iter0.root");							v_tags.push_back("e+_1GeV");	}
			if (iFile == 2) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_p_1GeV_iter0.root");							v_tags.push_back("p_1GeV");		}
			if (iFile == 3) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_mu-_1GeV_iter0.root");							v_tags.push_back("mu-_1GeV");	}
			if (iFile == 4) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_mu+_0p75GeV_iter0.root");						v_tags.push_back("mu+_0p75GeV");}
			if (iFile == 5) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_mu+_1GeV_iter0.root");							v_tags.push_back("mu+_1GeV");	}
			if (iFile == 6) { v_dataFiles.push_back(data_subfolder + "ERAM18_350V_412ns_pi+_0p5GeV_iter0.root");						v_tags.push_back("pi+_0p5GeV");	}
		}
	}

	// Energy scan using the mockup
	if(CERN_Escan){
		Settings("CERN22", "Energy_Scan", 4, -1, 412, 350, 415, 40);

		// int NFiles = 14;
		// for (int iFile = 0; iFile < NFiles; iFile++){
		int part_arr[] = {7};
		for (int iFile : part_arr) {
			if (iFile == 0) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("e+_0p5GeV");	}
			if (iFile == 1) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("e+_0p75GeV");	}
			if (iFile == 2) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("e+_1GeV");	}
			if (iFile == 3) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root");	v_tags.push_back("e+_1p25GeV");	}
			if (iFile == 4) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("e+_1p5GeV");	}
			if (iFile == 5) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("mu_0p75GeV");	}
			if (iFile == 6) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("mu_1GeV");	}
			if (iFile == 7) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("mu_1p5GeV");	}
			if (iFile == 8) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("pi_0p75GeV");	}
			if (iFile == 9) {v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root");	v_tags.push_back("pi_1p25GeV");	}
			if (iFile == 10){v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("pi_1p5GeV");	}
			if (iFile == 11){v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root");			v_tags.push_back("p+_1GeV");	}
			if (iFile == 12){v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("p+_1p25GeV");	}
			if (iFile == 13){v_dataFiles.push_back(data_subfolder + "All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root");		v_tags.push_back("p+_1p5GeV");	}

			DefaultAnalysis();
		}
	}


	// Drift distance scan scan using the mockup
	if(CERN_drift){
		Settings("CERN22", "Drift_Scan", 3, -1, 412, 350, 415, 40);

		int			z_val[] = 	{60, 218, 415, 925};
		std::string z_arr[] = 	{"60", "218p5", "415", "925"};

		int NFiles = 4;
		for (int iz = 0; iz < NFiles; iz++){
			const char* z = 	z_arr[iz].c_str();
			driftDist = 		z_val[iz];
			v_dataFiles.		push_back(data_subfolder + "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z" + z + "_iter4.root");	
			v_tags.				push_back(Form("Drift%s", z));

			DefaultAnalysis();
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// z scan scan with DESY21
	if(DESY_drift){
		int PT_arr[] = {412}; // 200, 412
		for (int iPT : PT_arr){
			PT = iPT;
			Settings("DESY21", Form("Drift_Scan_PT%i", PT), 1, 0, PT, 310, 450, 50);

			// int		z_val[] = {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
			// std::string z_arr[] = {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};
			int		z_val[] = {550};
			std::string z_arr[] = {"460"};

			for (int iz = 0; iz < (int)std::size(z_arr); iz++){
				const char* z = z_arr[iz].c_str();
				driftDist = z_val[iz];
				v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i/z_360_275_%i_02T_26_%s_iter9.root", iPT, iPT, z)); v_tags.push_back(Form("DESY21_z%s_PT%i", z, iPT));

				DefaultAnalysis();
			}
		}
	}
	


	// y scan scan with DESY21
	if(DESY_yscan){
		Settings("DESY21", "Row_Scan", 1, 0, 412, 310, 90, 50);

		std::string Y_arr[] = {"m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"};
		for (int y = 0; y < (int)std::size(Y_arr); y++){
			v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/yscan/Y%s_Z0_iter9.root", Y_arr[y].c_str())); v_tags.push_back(Form("DESY21_y%s", Y_arr[y].c_str())); 
			DefaultAnalysis();
		}
	}
	


	// Phi scan with DESY21
	if(DESY_phi){
		int			z_val[] =	{50, 550, 950};
		std::string z_arr[] =	{"m40", "460", "860"};
		int			phi_val[] =	{0, 5, 10, 20, 30, 30, 40, 45};
		
		for (int iz = 0; iz < (int)std::size(z_arr); iz++){
			const char* z = z_arr[iz].c_str();
			Settings("DESY21", Form("DESY21_phi_z%s", z), 1, 0, 200, 310, z_val[iz], 40);
			
			std::string comment_phi = comment;
			for (int ifile = 0; ifile < (int)std::size(phi_val); ifile++){
				int phi = phi_val[ifile];
				if(ifile == 0){		drawout_scanfolder = drawout_folder + "/DESY21_Drift_Scan/DESY21_Drift_Scan_PT200/"; 
									v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT200/z_360_275_200_02T_26_%s_iter9.root", z));				v_tags.push_back(Form("DESY21_z%s_PT200", z));}
				if(ifile < 5)		{v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_iter9.root", z, phi, z));		v_tags.push_back(Form("DESY21_phi%i_z%s", phi, z));}
				else				{v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter9.root", z, phi, z));	v_tags.push_back(Form("DESY21_phi%i_diag_z%s", phi, z));
									comment_phi = comment + "_WF" + fcorrectWF;}

				DefaultAnalysis();
				
				if(ifile ==0)drawout_scanfolder = drawout_folder + Form("/DESY21_phi/DESY21_phi_z%s/", z); 
			}
		}
	}
	


	// Theta scan with DESY21
	if(DESY_theta){
		Settings("DESY21", "Theta_scan", 1, 0, 200, 310, 350, 40);
		selectionSet = "Sel_DESY21_theta";
		std::string theta_arr[] = {"m45", "m20", "20"};
		for (std::string theta : theta_arr){
			v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Theta_scan/theta_%s_02T_z460_ym60_iter9.root", theta.c_str())); v_tags.push_back(Form("DESY21_theta%s", theta.c_str()));
			
			DefaultAnalysis();
		}
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Drawing
	if(DO_Comparison){
		v_tags.push_back("");
		std::string tag =		"CERN22_Escan_mu_1p5GeV";
		drawout_scanfolder =		drawout_folder + "CERN22_Energy_Scan/" + tag + "/";

		std::string comment1 = 	"";
		std::string comment2 = 	"_RCMod_GainMod";

		v_comments.				clear();
		v_comments.				push_back(comment1);
		v_comments.				push_back(comment2);

		v_dataFiles.			clear();
		v_dataFiles.			push_back(drawout_scanfolder + "dEdx_" + tag + comment1 + ".root");
		v_dataFiles.			push_back(drawout_scanfolder + "dEdx_" + tag + comment2 + ".root");

		p_DrawOuts =			new DrawOuts(v_dataFiles);
		p_DrawOuts->				FileComparison();
	}


}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Configuration functions

void Reconstruction::Correction(const int& corrRC, const int& corrGain, const int& corrWF, const int& corrDrift){
	fcorrectRC =				corrRC;
	if(corrRC == 0)	comment =	comment + "_RCoff";
	if(corrRC == 2)	comment =	comment + "_RCMod";

	fcorrectGain =				corrGain;
	if(corrGain == 0) comment =	comment + "_Gainoff";
	if(corrGain == 2) comment =	comment + "_GainMod";

	fcorrectWF =				corrWF;
	if(corrWF == 0)	comment =	comment + "_WFoff";

	fcorrectDrift =				corrDrift;
	if(corrDrift == 0)comment =	comment + "_DriftFile";
}

void Reconstruction::Settings(const std::string &testbeam_name, const std::string &scan_name, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin){
	v_dataFiles.clear();
	v_tags.clear();

	testbeam = 					testbeam_name;
	scan = 						scan_name;
	selectionSet =				"Sel_" + testbeam;
	data_subfolder =				data_folder + "Data_" + testbeam + "/";
	drawout_scanfolder =		drawout_folder + testbeam + "_" + scan + "/";
	intUploader =				uploader;
	moduleCase =				modules;
	PT =						peaking_time;
	Dt =						diffusion;
	driftDist =					drift_dist;
	TB =						timbin;

	if(dedx) p_lut =			new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
}


void Reconstruction::DefaultAnalysis(){
	tag = 						v_tags.back();
	drawout_runfolder =			drawout_scanfolder + testbeam + "_" + scan + "_" + tag + "/";
	MakeMyDir(drawout_runfolder);
	rootout_file =				drawout_runfolder + "dEdx_" + testbeam + "_" + scan + "_" + tag + comment + ".root";
	drawout_file =				drawout_runfolder + "dEdx_" + testbeam + "_" + scan + "_" + tag + comment + ".pdf";
	log_file = 					drawout_runfolder + "dEdx_" + testbeam + "_" + scan + "_" + tag + ".log";
	std::cout << "logs:		" << log_file		<< std::endl;

	if(dedx){
		p_uploader =			GiveMe_Uploader (intUploader, v_dataFiles.back());
		p_dEdx =				new dEdx();
		p_dEdx->				Reconstruction();
		delete					p_uploader;
		delete					p_dEdx;
	}

	if(DO_dEdx){
		p_DrawOuts =			new DrawOuts(rootout_file);
		p_DrawOuts->			EnergyLoss();
		delete  				p_DrawOuts;
		std::cout << "delete done" << std::endl;
	}
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
//	drawout_scanfolder = drawout_folder + "/DESY21_ExB/";	
//	MakeMyDir(drawout_scanfolder); 
//	intUploader = 1;
//	moduleCase =	0;
//	PT = 200; Dt = 310;; TB = 40;
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	std::string B_arr[] = {"0", "04", "06"};
//	std::string Z_arr[] = {"60", "460", "860"};
//	int z_val[] = {150, 550, 950};
//	for(int j = 0; j < 3; j++){
//		const char* B = B_arr[j].c_str();
//		for(int i = 0; i < 3; i++){
//		const char* z = Z_arr[i].c_str();
//		driftDist = z_val[i];
//		// v_dataFiles.push_back(Form			("../Data_DESY21_dev_v9/ExB_scan/ExB_360_412ns_B02_ym10_z%s_iter9.root", z)); v_tags.push_back(Form("DESY21_ExB02_Z%s", z)); v_prtcles.push_back(Form("electron_ExB02_Z%s", z);
//		v_dataFiles.push_back(Form			("../Data_DESY21_dev_v9/ExB_scan/ExB_360_phim3_200ns_B%s_ym60_z%s_iter0.root", B, z)); v_tags.push_back(Form("DESY21_ExB%s_Z%s", B, z)); v_prtcles.push_back(Form("electron_ExB%s_Z%s", B, z));
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)				Control (drawout_scanfolder, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)			DrawOut_Control (drawout_scanfolder, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)				p_dEdx->Reconstruction();
//		if (DO_dEdx)			DrawOut_Methods (drawout_scanfolder, v_tags.back(), comment, 1, v_prtcles.back());
//		if (DO_Checks)				DrawOut_Checks (drawout_scanfolder, v_dataFiles.back(), v_tags.back(), comment);
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
//	drawout_scanfolder = drawout_folder + "/DESY19_phi/";
//	MakeMyDir(drawout_scanfolder); 
//	for (int ifile = 3; ifile < NFiles; ifile++){
//		int phi = phi_val[ifile];
//		if(ifile < 3) { v_dataFiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi));		v_tags.push_back(Form("DESY19_phi%i", phi));		v_prtcles.push_back(Form("electron_phi%i", phi));}
//		else			{ v_dataFiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi));	v_tags.push_back(Form("DESY19_phi%i_diag", phi)); v_prtcles.push_back(Form("electron_phi%i_diag", phi));}
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(drawout_scanfolder, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanfolder, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(drawout_scanfolder, v_dataFiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanfolder, v_tags.back(), comment, 1, v_prtcles.back());
//		delete p_uploader;
//	}
//	}




//	// z scan with MC
//	if(MC_zscan){
//	selectionSet = "TMC_CERN22_Event";
//	drawout_scanfolder = drawout_folder + "/CERN23_MC/MC_zscan/";	
//	MakeMyDir(drawout_scanfolder); 
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; Dt = 286; TB = 40; // PT = 400 actually
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	int zmax = 9;
//	for (int zDrift = 0; zDrift < zmax; zDrift++){
//		// v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v2_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm", zDrift+1);		
//		v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm NoOpt", zDrift+1));		
//		// v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00_old", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm old", zDrift+1);		
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(drawout_scanfolder, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanfolder, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(drawout_scanfolder, v_dataFiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanfolder, v_tags.back(), comment, 1, v_prtcles.back());
//	}
//	}
	



//	// z scan scan using the mockup
//	if(MockUp_zdist){
//	selectionSet = "Sel_CERN22";
//	drawout_scanfolder = drawout_folder + "/CERN22_Mockup_zscan/";	
//	MakeMyDir(drawout_scanfolder); 
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; Dt = 350; driftDist = 415; TB = 40;

//	int NFiles = 10;
//	for (int iFile = 0; iFile < NFiles; iFile++){
//		if (iFile == 0) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z060");}
//		if (iFile == 1) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_14V_z218");}
//		if (iFile == 2) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z415");}
//		if (iFile == 3) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z572");}
//		if (iFile == 5) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z060");}
//		if (iFile == 6) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_25V_z218");}
//		if (iFile == 7) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z415");}
//		if (iFile == 8) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z572");}
//		if (iFile == 9) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z925");}
//		if (DO_control)	DrawOut_Control			(drawout_scanfolder, v_tags.back(), comment, selectionSet, 8);
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
//		drawout_scanfolder = drawout_folder + Form("/DESY21_Drift_Scan/DESY21_Drift_Scan_PT%i_139V/", PT); 
//		MakeMyDir(drawout_scanfolder); 
//	if (control or dedx) p_lut = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//		int NFiles = 9;
//		for (int zDrift = -1; zDrift < NFiles; zDrift++){
//		if(zDrift == -1) {v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT));			v_tags.push_back(Form("DESY21_zm40_PT%i", PT));			}
//		else {v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift)); v_tags.push_back(Form("DESY21_z%i60_PT%i", zDrift, PT)); v_prtcles.push_back(Form("electron_z%i60", zDrift));}
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(drawout_scanfolder, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(drawout_scanfolder, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_dEdx)	DrawOut_Methods		(drawout_scanfolder, v_tags.back(), comment, 1, v_prtcles.back());
//		}
//		if(DO_Resolution)	DrawOut_Zscan	(drawout_folder + Form("/DESY21_Drift_Scan/DESY21_Drift_Scan_PT%i_139V", PT), comment, PT);
//	}
//	}