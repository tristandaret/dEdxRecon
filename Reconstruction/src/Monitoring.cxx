#include "Util.h"
#include "Misc.h"

#include "Monitoring.h"
#include "ReconTools.h"
#include "Variables.h"
#include "LUTs.h"
#include "dEdx.h"

#include "Control.h"
#include "DrawOuts.h"

#include "Selector.h"

#include "Uploader.h"
#include "GiveMe_Uploader.h"

namespace Reconstruction{
	Reconstruction::dEdx		*p_dEdx;
	Reconstruction::DrawOuts	*p_DrawOuts;
	Reconstruction::LUT			*p_lut;
	Uploader					*p_uploader;
	
	// Files to use
	int prototype	 =			0;
	int CERN_Escan	 =		1;

	int DESY_zscan	 =		0;
	int DESY_yscan	 =		0;
	int DESY_phi	 =			0;
	int DESY_theta	 =		0;

	// Computations	
	int control	 =			0;
	int dedx	 =				1;

	// DrawOuts	
	int DO_control	 =		0;
	int DO_Checks	 =			0;
	int DO_dEdx	 =			1;
	int DO_Resolution	 =		0;
	int DO_Global	 =			0;
	int DO_Scans	 =			0;

	int DO_Separation	 =		0;
	int DO_Systematics	 =	0;
}



void Reconstruction::Correction(const int& corrRC, const int& corrGain, const int& corrWF, const int& corrDrift){
	fcorrectRC	 =				corrRC;
	if(corrRC	 == 0)	comment	 =	comment + "_RCoff";
	if(corrRC	 == 2)	comment	 =	comment + "_RCMod";

	fcorrectGain	 =				corrGain;
	if(corrGain	 == 0) comment	 =	comment + "_Gainoff";
	if(corrGain	 == 2) comment	 =	comment + "_GainMod";

	fcorrectWF	 =				corrWF;
	if(corrWF	 == 0)	comment	 =	comment + "_WFoff";

	fcorrectDrift	 =			corrDrift;
	if(corrDrift	 == 0)comment	 =	comment + "_DriftFile";
}

void Reconstruction::Settings(const std::string &Selection, const std::string &outputDir, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin){
	v_dataFiles.clear();
	v_tags.clear();
	v_prtcles.clear();

	selectionSet	 =				Selection;
	outDir	 =					outputDir;
	MakeMyDir(outDir);
	intUploader	 =				uploader;
	moduleCase	 =				modules;
	PT	 =						peaking_time;
	Dt	 =						diffusion;
	driftDist	 =					drift_dist;
	TB	 =						timbin;

	if(dedx) p_lut	 =			new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
}


void Reconstruction::DefaultAnalysis(){
	outFile_dEdx	 =				(outDir + v_tags.back() +"/" + "dEdx_" + v_tags.back() + comment + ".root").c_str();
	if(dedx){
		p_uploader	 =			GiveMe_Uploader (intUploader, v_dataFiles.back());
		p_dEdx->				Reconstruction();
		delete					p_uploader;
	}
	if(DO_dEdx){
		p_DrawOuts	 =			new DrawOuts(outFile_dEdx);
		p_DrawOuts->			EnergyLoss();
	}
}



void Reconstruction::Monitoring()
{
	comment	 = "";
	gErrorIgnoreLevel	 = kInfo;

	Correction(0,0,1,1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Energy scan using the prototype (ERAM 18)
	if(prototype){
		Settings("T2_CERN22_Event", "../OUT_Reconstruction/CERN22_ERAM18/", 2, -1, 412, 350, 415, 40);

		int part_arr[]	 = {1,2,5,6};
		for (int iFile : part_arr) {
			if (iFile	 == 0) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_e+_0p5GeV_iter0.root");	v_tags.push_back("CERN22_ERAM18_e+_0p5GeV");	v_prtcles.push_back("e^{+} 0.5GeV");}
			if (iFile	 == 1) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_e+_1GeV_iter0.root");		v_tags.push_back("CERN22_ERAM18_e+_1GeV");		v_prtcles.push_back("e^{+} 1GeV");}
			if (iFile	 == 2) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_p_1GeV_iter0.root");		v_tags.push_back("CERN22_ERAM18_p_1GeV");		v_prtcles.push_back("protons 1GeV");} 
			if (iFile	 == 3) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_mu-_1GeV_iter0.root");		v_tags.push_back("CERN22_ERAM18_mu-_1GeV");		v_prtcles.push_back("#mu^{-} 1GeV");}
			if (iFile	 == 4) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root");	v_tags.push_back("CERN22_ERAM18_mu+_0p75GeV");	v_prtcles.push_back("#mu^{+} 0.75GeV");}
			if (iFile	 == 5) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_mu+_1GeV_iter0.root");		v_tags.push_back("CERN22_ERAM18_mu+_1GeV");		v_prtcles.push_back("#mu^{+} 1GeV");}
			if (iFile	 == 6) { v_dataFiles.push_back("../Data_CERN22_vD/ERAM18_350V_412ns_pi+_0p5GeV_iter0.root");	v_tags.push_back("CERN22_ERAM18_pi+_0p5GeV");	v_prtcles.push_back("#pi^{+} 0.5GeV");}
		}
	}

	// Energy scan using the mockup
	if(CERN_Escan){
		Settings("T2_CERN22_Event", "../OUT_Reconstruction/CERN22_Energy_Scan/", 3, -1, 412, 350, 415, 40);

		// int NFiles	 = 14;
		// for (int iFile	 = 0; iFile < NFiles; iFile++){
		int part_arr[]	 = {7};
		for (int iFile : part_arr) {
			if (iFile	 == 0)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("CERN22_Escan_e+_0p5GeV");		v_prtcles.push_back("e^{+} 0p5GeV (Mockup)");}
			if (iFile	 == 1)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_0p75GeV_25V_z415_y2pad_iter0.root");		v_tags.push_back("CERN22_Escan_e+_0p75GeV");	v_prtcles.push_back("e^{+} 0p75GeV (Mockup)");}
			if (iFile	 == 2)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("CERN22_Escan_e+_1GeV");		v_prtcles.push_back("e^{+} 1GeV (Mockup)");}
			if (iFile	 == 3)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p25GeV_25V_z415p1_y2pad_1_iter0.root");	v_tags.push_back("CERN22_Escan_e+_1p25GeV");	v_prtcles.push_back("e^{+} 1p25GeV (Mockup)");}
			if (iFile	 == 4)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_e+_1p5GeV_25V_z415p1_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_e+_1p5GeV");		v_prtcles.push_back("e^{+} 1p5GeV (Mockup)");}
			if (iFile	 == 5)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_0p75GeV_25V_z415_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_mu_0p75GeV");	v_prtcles.push_back("#mu^{+} 0p75GeV (Mockup)");}
			if (iFile	 == 6)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("CERN22_Escan_mu_1GeV");		v_prtcles.push_back("#mu^{+} 1GeV (Mockup)");}
			if (iFile	 == 7)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_mu+_1p5GeV_25V_z415p1_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_mu_1p5GeV");		v_prtcles.push_back("#mu^{+} 1p5GeV (Mockup)");}
			if (iFile	 == 8)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_0p75GeV_25V_z415_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_pi_0p75GeV");	v_prtcles.push_back("#pi^{+} 0p75GeV (Mockup)");}
			if (iFile	 == 9)	{ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p25GeV_25V_z415p1_y2pad_2_iter0.root");v_tags.push_back("CERN22_Escan_pi_1p25GeV");	v_prtcles.push_back("#pi^{+} 1p25GeV (Mockup)");}
			if (iFile	 == 10){ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_pi+_1p5GeV_25V_z415p1_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_pi_1p5GeV");		v_prtcles.push_back("#pi^{+} 1p5GeV (Mockup)");}
			if (iFile	 == 11){ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1GeV_25V_z415p1_y2pad_iter0.root");		v_tags.push_back("CERN22_Escan_p+_1GeV");		v_prtcles.push_back("protons 1GeV (Mockup)");}
			if (iFile	 == 12){ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p25GeV_25V_z415p1_y2pad_iter0.root");	v_tags.push_back("CERN22_Escan_p+_1p25GeV");	v_prtcles.push_back("protons 1p25GeV (Mockup)");}
			if (iFile	 == 13){ v_dataFiles.push_back("../Data_CERN22_vD/All_ERAMS_350V_412ns_p_1p5GeV_25V_z415_y2pad_2_iter0.root");		v_tags.push_back("CERN22_Escan_p+_1p5GeV");		v_prtcles.push_back("protons 1p5GeV (Mockup)");}

			DefaultAnalysis();
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// z scan scan with DESY21
	if(DESY_zscan){
	int PT_arr[]	 = {412}; // 200, 412
	for (int iPT : PT_arr){
		PT	 = iPT;
		Settings("T_DESY21_Event", Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i/", PT), 1, 0, PT, 310, 450, 50);

		// int		z_val[]	 = {50, 150, 250, 350, 450, 550, 650, 750, 850, 950};
		// std::string z_arr[]	 = {"m40", "060", "160", "260", "360", "460", "560", "660", "760", "860"};
		int		z_val[]	 = {550};
		std::string z_arr[]	 = {"460"};

		for (int iz	 = 0; iz < (int)std::size(z_arr); iz++){
		const char* z	 = z_arr[iz].c_str();
		driftDist	 = z_val[iz];
		v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i/z_360_275_%i_02T_26_%s_iter9.root", iPT, iPT, z)); v_tags.push_back(Form("DESY21_z%s_PT%i", z, iPT)); v_prtcles.push_back(Form("electron_z%s", z));

		DefaultAnalysis();
		}
		// if(DO_Systematics)	p_DrawOuts->Systematics();
		// if(DO_Resolution)	p_DrawOuts->ResolutionDriftScan();
	}
	// if(DO_Global)		p_DrawOuts->ResolutionDriftScanPT();
	}
	


	// y scan scan with DESY21
	if(DESY_yscan){
		Settings("T_DESY21_Event", "../OUT_Reconstruction/DESY21_yscan/", 1, 0, 412, 310, 90, 50);

		std::string Y_arr[]	 = {"m140", "m120", "m100", "m80", "m60", "m40", "0", "20", "40", "60", "80"};
		for (int y	 = 0; y < (int)std::size(Y_arr); y++){
			v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/yscan/Y%s_Z0_iter9.root", Y_arr[y].c_str())); v_tags.push_back(Form("DESY21_y%s", Y_arr[y].c_str())); v_prtcles.push_back(Form("electron_y%s", Y_arr[y].c_str())); 
			DefaultAnalysis();
		}
		if(DO_Resolution)	DrawOut_Yscan("../OUT_Reconstruction/DESY21_yscan", comment);
	}
	


	// Phi scan with DESY21
	if(DESY_phi){
		int			z_val[]	 =	{50, 550, 950};
		std::string z_arr[]	 =	{"m40", "460", "860"};
		int			phi_val[]	 =	{0, 5, 10, 20, 30, 30, 40, 45};
		
		for (int iz	 = 0; iz < (int)std::size(z_arr); iz++){
			const char* z	 = z_arr[iz].c_str();
			Settings("T_DESY21_Event", Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s/", z), 1, 0, 200, 310, z_val[iz], 40);
			
			std::string comment_phi	 = comment;
			for (int ifile	 = 0; ifile < (int)std::size(phi_val); ifile++){
				int phi	 = phi_val[ifile];
				if(ifile	 == 0){		outDir	 = "../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT200/"; 
									v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT200/z_360_275_200_02T_26_%s_iter9.root", z));			v_tags.push_back(Form("DESY21_z%s_PT200", z));				v_prtcles.push_back(Form("electron_z%s", z));}
				if(ifile < 5)		{v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_iter9.root", z, phi, z));		v_tags.push_back(Form("DESY21_phi%i_z%s", phi, z));		v_prtcles.push_back(Form("electron_phi%i_z%s", phi, z));}
				else				{v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter9.root", z, phi, z));	v_tags.push_back(Form("DESY21_phi%i_diag_z%s", phi, z));	v_prtcles.push_back(Form("electron_phi%i_diag_z%s", phi, z));
									comment_phi	 = comment + "_WF" + fcorrectWF;}

				DefaultAnalysis();
				
				if(ifile	 ==0)outDir	 = Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s/", z); 
			}
			if(DO_Resolution)	DrawOut_Phiscan (Form("../OUT_Reconstruction/DESY21_phi/DESY21_phi_z%s", z), comment, "_WF" + fcorrectWF, z);
			if(DO_Systematics)	DrawOut_Systematics(outDir, comment, "phi");
		}
		// DrawOut_TGE_WFsum_L("../OUT_Reconstruction/DESY21_phi/", comment);
		if(DO_Global) DrawOut_Phiscan_Z("../OUT_Reconstruction/DESY21_phi", comment, "_WF" + fcorrectWF);
	}
	


	// Theta scan with DESY21
	if(DESY_theta){
		Settings("T_DESY21theta_Event", "../OUT_Reconstruction/DESY21_theta/", 1, 0, 200, 310, 350, 40);
		std::string theta_arr[]	 = {"m45", "m20", "20"};
		for (std::string theta : theta_arr){
			v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/Theta_scan/theta_%s_02T_z460_ym60_iter9.root", theta.c_str())); v_tags.push_back(Form("DESY21_theta%s", theta.c_str())); v_prtcles.push_back(Form("electron_theta%s", theta.c_str()));
			
			DefaultAnalysis();
		}
		if(DO_Resolution)			DrawOut_Thetascan ("../OUT_Reconstruction/DESY21_theta",	comment);
	}



	if(DO_Scans) DrawOut_Scans("../OUT_Reconstruction", comment, "_WF" + fcorrectWF);




}









	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//	int DESY_ExB	 = 0;
//	int DESY_zscan_139V	 = 0; 
//	int MC_zscan	 = 0; 
//	int DESY19_phi	 = 0; 
//	int MockUp_zdist	 = 0; 



//	// ExB scan scan with DESY21
//	if(DESY_ExB){
//	selectionSet	 = "T_DESY21_Event";
//	outDir	 = "../OUT_Reconstruction/DESY21_ExB/";	
//	MakeMyDir(outDir); 
//	intUploader	 = 1;
//	moduleCase	 =	0;
//	PT	 = 200; Dt	 = 310;; TB	 = 40;
//	if (control or dedx) p_lut	 = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	std::string B_arr[]	 = {"0", "04", "06"};
//	std::string Z_arr[]	 = {"60", "460", "860"};
//	int z_val[]	 = {150, 550, 950};
//	for(int j	 = 0; j < 3; j++){
//		const char* B	 = B_arr[j].c_str();
//		for(int i	 = 0; i < 3; i++){
//		const char* z	 = Z_arr[i].c_str();
//		driftDist	 = z_val[i];
//		// v_dataFiles.push_back(Form			("../Data_DESY21_dev_v9/ExB_scan/ExB_360_412ns_B02_ym10_z%s_iter9.root", z)); v_tags.push_back(Form("DESY21_ExB02_Z%s", z)); v_prtcles.push_back(Form("electron_ExB02_Z%s", z);
//		v_dataFiles.push_back(Form			("../Data_DESY21_dev_v9/ExB_scan/ExB_360_phim3_200ns_B%s_ym60_z%s_iter0.root", B, z)); v_tags.push_back(Form("DESY21_ExB%s_Z%s", B, z)); v_prtcles.push_back(Form("electron_ExB%s_Z%s", B, z));
//		if(control or dedx) p_uploader	 = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)				Control (outDir, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)			DrawOut_Control (outDir, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)				p_dEdx->Reconstruction();
//		if (DO_dEdx)			DrawOut_Methods (outDir, v_tags.back(), comment, 1, v_prtcles.back());
//		if (DO_Checks)				DrawOut_Checks (outDir, v_dataFiles.back(), v_tags.back(), comment);
//		}
//	}
//	}





//	// Phi scan with DESY19
//	if(DESY19_phi){
//	selectionSet	 = "T_DESY21_Event";
//	intUploader	 = 1;
//	moduleCase	 =	0;
//	PT	 = 412; Dt	 = 310; TB	 = 40; driftDist	 = 430;
//	if (control or dedx) p_lut	 = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	int phi_val[]	 = {0, 10, 20, 30, 40, 45};
//	int NFiles	 = 6;
//	outDir	 = "../OUT_Reconstruction/DESY19_phi/";
//	MakeMyDir(outDir); 
//	for (int ifile	 = 3; ifile < NFiles; ifile++){
//		int phi	 = phi_val[ifile];
//		if(ifile < 3) { v_dataFiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi));		v_tags.push_back(Form("DESY19_phi%i", phi));		v_prtcles.push_back(Form("electron_phi%i", phi));}
//		else			{ v_dataFiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi));	v_tags.push_back(Form("DESY19_phi%i_diag", phi)); v_prtcles.push_back(Form("electron_phi%i_diag", phi));}
//		if(control or dedx) p_uploader	 = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(outDir, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(outDir, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(outDir, v_dataFiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(outDir, v_tags.back(), comment, 1, v_prtcles.back());
//		delete p_uploader;
//	}
//	}




//	// z scan with MC
//	if(MC_zscan){
//	selectionSet	 = "TMC_CERN22_Event";
//	outDir	 = "../OUT_Reconstruction/CERN23_MC/MC_zscan/";	
//	MakeMyDir(outDir); 
//	intUploader	 = 2;
//	moduleCase	 = -1;
//	PT	 = 412; Dt	 = 286; TB	 = 40; // PT	 = 400 actually
//	if (control or dedx) p_lut	 = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//	int zmax	 = 9;
//	for (int zDrift	 = 0; zDrift < zmax; zDrift++){
//		// v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v2_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm", zDrift+1);		
//		v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm NoOpt", zDrift+1));		
//		// v_dataFiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root", zDrift+1)); v_tags.push_back(Form("CERN23_MC_z%i00_old", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm old", zDrift+1);		
//		if(control or dedx) p_uploader	 = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(outDir, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(outDir, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_Checks)	DrawOut_Checks			(outDir, v_dataFiles.back(), v_tags.back(), comment);
//		if (DO_dEdx)	DrawOut_Methods		(outDir, v_tags.back(), comment, 1, v_prtcles.back());
//	}
//	}
	



//	// z scan scan using the mockup
//	if(MockUp_zdist){
//	selectionSet	 = "T2_CERN22_Event";
//	outDir	 = "../OUT_Reconstruction/CERN22_Mockup_zscan/";	
//	MakeMyDir(outDir); 
//	intUploader	 = 2;
//	moduleCase	 = -1;
//	PT	 = 412; Dt	 = 350; driftDist	 = 415; TB	 = 40;

//	int NFiles	 = 10;
//	for (int iFile	 = 0; iFile < NFiles; iFile++){
//		if (iFile	 == 0) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z060");}
//		if (iFile	 == 1) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_14V_z218");}
//		if (iFile	 == 2) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z415");}
//		if (iFile	 == 3) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_14V_z572");}
//		if (iFile	 == 5) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z060");}
//		if (iFile	 == 6) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root");	v_tags.push_back("CERN22_Mockup_e+_25V_z218");}
//		if (iFile	 == 7) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z415");}
//		if (iFile	 == 8) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z572");}
//		if (iFile	 == 9) { v_dataFiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root");		v_tags.push_back("CERN22_Mockup_e+_25V_z925");}
//		if (DO_control)	DrawOut_Control			(outDir, v_tags.back(), comment, selectionSet, 8);
//	}
//	}
	


//	// z scan scan with DESY21 at 139 V/cm
//	if(DESY_zscan_139V){
//	selectionSet	 = "T_DESY21_Event";
//	intUploader	 = 1;
//	moduleCase	 =	0; 
//	Dt	 = 310; TB	 = 50;
//	int PT_arr[]	 = {200};
//	for (int PT : PT_arr){
//		outDir	 = Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i_139V/", PT); 
//		MakeMyDir(outDir); 
//	if (control or dedx) p_lut	 = new LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", Dt, PT));
//		int NFiles	 = 9;
//		for (int zDrift	 = -1; zDrift < NFiles; zDrift++){
//		if(zDrift	 == -1) {v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT));			v_tags.push_back(Form("DESY21_zm40_PT%i", PT));			v_prtcles.push_back("electron_z-40");}
//		else {v_dataFiles.push_back(Form("../Data_DESY21_dev_v9/zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift)); v_tags.push_back(Form("DESY21_z%i60_PT%i", zDrift, PT)); v_prtcles.push_back(Form("electron_z%i60", zDrift));}
//		if(control or dedx) p_uploader	 = GiveMe_Uploader (intUploader, v_dataFiles.back());
//		if (control)		Control		(outDir, v_tags.back(), comment, v_dataFiles.back(), selectionSet, p_uploader, moduleCase, 0, PT, TB, v_prtcles.back());
//		if (DO_control)	DrawOut_Control			(outDir, v_tags.back(), comment, selectionSet, 1);
//		if (dedx)			p_dEdx->Reconstruction();
//		if (DO_dEdx)	DrawOut_Methods		(outDir, v_tags.back(), comment, 1, v_prtcles.back());
//		}
//		if(DO_Resolution)	DrawOut_Zscan	(Form("../OUT_Reconstruction/DESY21_zscan/DESY21_zscan_PT%i_139V", PT), comment, PT);
//	}
//	}