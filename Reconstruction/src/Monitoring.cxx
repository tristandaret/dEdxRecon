#include "Util.h"
#include "Misc.h"

#include "Monitoring.h"
#include "ReconTools.h"
#include "Variables.h"

#include "Control.h"

#include "Selector.h"

namespace Reconstruction {
Reconstruction::dEdx *p_dEdx;
Reconstruction::DrawOuts *p_DrawOuts;
Reconstruction::LUT *p_lut;
Uploader *p_uploader;

// Files to use
int prototype = 0;
int CERN_Escan = 1;
int CERN_drift = 0;

int DESY_drift = 0;
int DESY_row = 0;
int DESY_phi = 0;
int DESY_theta = 0;

// Computations
int correction_wf = 0;
int dedx = 1;

// DrawOuts
int Draw_Control = 0;
int Draw_dEdx = 0;
int Draw_Comparison = 0;
int Draw_Corrections = 0;

int Draw_DESY21SingleScan = 0;
int Draw_DESY21MultScan = 0;
int Draw_CERN22Scan = 0;

int DtwithBhere = 286;
int DtwithoutBhere = 323;

} // namespace Reconstruction

void Reconstruction::Monitoring()
{
   drawWhichMethods = 1; // 0: both methods | 1: only WF | 2: only XP
   comment = "";
   gErrorIgnoreLevel = kInfo;

   Correction(1, 1, 1, 1, 1);

   // Energy scan using the prototype (ERAM 18)
   if (prototype) {
      Settings("CERN22", "", "Prototype", "", "Energy (GeV)", 2, -1, 412, 350, 415, 40);

      int part_arr[] = {1, 2, 5, 6};
      for (int iFile : part_arr) {
         if (iFile == 0) {
            v_datafiles.push_back(dataScanPath +
                                  "ERAM18_350V_412ns_e+_0p5GeV_iter0.root");
            vTags.push_back("e+_0p5GeV");
         }
         if (iFile == 1) {
            v_datafiles.push_back(dataScanPath + "ERAM18_350V_412ns_e+_1GeV_iter0.root");
            vTags.push_back("e+_1GeV");
         }
         if (iFile == 2) {
            v_datafiles.push_back(dataScanPath + "ERAM18_350V_412ns_p_1GeV_iter0.root");
            vTags.push_back("p_1GeV");
         }
         if (iFile == 3) {
            v_datafiles.push_back(dataScanPath + "ERAM18_350V_412ns_mu-_1GeV_iter0.root");
            vTags.push_back("mu-_1GeV");
         }
         if (iFile == 4) {
            v_datafiles.push_back(dataScanPath +
                                  "ERAM18_350V_412ns_mu+_0p75GeV_iter0.root");
            vTags.push_back("mu+_0p75GeV");
         }
         if (iFile == 5) {
            v_datafiles.push_back(dataScanPath + "ERAM18_350V_412ns_mu+_1GeV_iter0.root");
            vTags.push_back("mu+_1GeV");
         }
         if (iFile == 6) {
            v_datafiles.push_back(dataScanPath +
                                  "ERAM18_350V_412ns_pi+_0p5GeV_iter0.root");
            vTags.push_back("pi+_0p5GeV");
         }
      }
   }

   // Energy scan using the mockup
   if (CERN_Escan) {
      Dt = DtwithoutBhere;
      Settings("CERN22", "", "Energy", "", "Energy (GeV)", 4, -1, 412, Dt, 415, 40);
      vScanVals.insert(vScanVals.end(), {0.5, 0.75, 1, 1.25, 1.5, 0.75, 1, 1.5, 0.75,
                                         1.25, 1.5, 1, 1.25, 1.5});
      vScanLabels.insert(vScanLabels.end(),
                         {"e+_0p5GeV", "e+_0p75GeV", "e+_1GeV", "e+_1p25GeV", "e+_1p5GeV",
                          "mu+_0p75GeV", "mu+_1GeV", "mu+_1p5GeV", "pi+_0p75GeV",
                          "pi+_1p25GeV", "pi+_1p5GeV", "p_1GeV", "p_1p25GeV",
                          "p_1p5GeV"});

      std::vector<std::string> v_suffix = {
         "_25V_z415_y2pad_iter0.root",   "_25V_z415_y2pad_iter0.root",
         "_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_1_iter0.root",
         "_25V_z415p1_y2pad_iter0.root", "_25V_z415_y2pad_iter0.root",
         "_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_iter0.root",
         "_25V_z415_y2pad_iter0.root",   "_25V_z415p1_y2pad_2_iter0.root",
         "_25V_z415p1_y2pad_iter0.root", "_25V_z415p1_y2pad_iter0.root",
         "_25V_z415p1_y2pad_iter0.root", "_25V_z415_y2pad_2_iter0.root"};

      for (int iEnergy = 0; iEnergy < (int)std::size(vScanLabels); iEnergy++) {
      // for (int iEnergy = 2; iEnergy < 3; iEnergy++) {
         v_datafiles.push_back(dataScanPath + "All_ERAMS_350V_412ns_" +
                               vScanLabels[iEnergy] + v_suffix[iEnergy]);
         vTags.push_back(Form("%s_%s_%s_Dt%d", testbeam.c_str(), scanName.c_str(),
                              vScanLabels[iEnergy].c_str(), Dt));

         DefaultAnalysis();
      }
      if (Draw_CERN22Scan)
         DrawCERN22Scan();
      ClearVectors();
   }

   // Drift distance scan scan using the mockup
   if (CERN_drift) {
      Settings("CERN22", "", "Drift", "", "Drift distance (mm)", 3, -1, 412, 350, 415,
               40);

      vScanVals.insert(vScanVals.end(), {60, 218, 415, 925});
      vScanLabels.insert(vScanLabels.end(), {"60", "218p5", "415", "925"});

      int NFiles = 4;
      for (int iz = 0; iz < NFiles; iz++) {
         const char *z = vScanLabels[iz].c_str();
         driftDist = vScanVals[iz];
         v_datafiles.push_back(dataScanPath + "All_ERAMS_350V_412ns_e+_0p5GeV_25V_z" + z +
                               "_iter4.root");
         vTags.push_back(Form("Drift%s", z));

         DefaultAnalysis();
      }
   }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // z scan scan with DESY21
   if (DESY_drift) {
      Dt = DtwithBhere;
      int PT_arr[] = {200, 412}; // 200, 412
      // int PT_arr[] = {412}; // 200, 412
      vScanVals.insert(vScanVals.end(),
                       {50, 150, 250, 350, 450, 550, 650, 750, 850, 950});
      vScanLabels.insert(vScanLabels.end(), {"m40", "060", "160", "260", "360", "460",
                                             "560", "660", "760", "860"});
      for (int iPT : PT_arr) {
         PT = iPT; // PT is a shared variable in the namespace so it's important to update
                   // it here
         Settings("DESY21", "Drift", Form("Drift_PT%i", PT), Form("%i ns", PT),
                  "Drift distance (mm)", 1, 0, PT, Dt, 450, 50);

         for (int iz = 0; iz < (int)std::size(vScanLabels); iz++) {
         // for (int iz = 0; iz < 1; iz++) {
            const char *z = vScanLabels[iz].c_str();
            driftDist = vScanVals[iz];
            v_datafiles.push_back(
               dataScanPath +
               Form("Drift_scan_PT%i/z_360_275_%i_02T_26_%s_iter9.root", iPT, iPT, z));
            vTags.push_back(
               Form("%s_%s_Z%s_Dt%d", testbeam.c_str(), scanName.c_str(), z, Dt));

            DefaultAnalysis();
         }
         if (Draw_DESY21SingleScan)
            DrawSingleScan(drawWhichMethods);
         scanIndex++;
      }
      if (Draw_DESY21MultScan)
         DrawMultipleScan(drawWhichMethods);
      ClearVectors();
   }

   // y scan scan with DESY21
   if (DESY_row) {
      Dt = DtwithBhere;
      Settings("DESY21", "", "Row", "", "Vertical beam position (mm)", 1, 0, 412, Dt, 90,
               50);
      vScanVals.insert(vScanVals.end(),
                       {-140, -120, -100, -80, -60, -40, 0, 20, 40, 60, 80});
      vScanLabels.insert(vScanLabels.end(), {"m140", "m120", "m100", "m80", "m60", "m40",
                                             "0", "20", "40", "60", "80"});

      for (int y = 0; y < (int)std::size(vScanLabels); y++) {
         v_datafiles.push_back(
            dataScanPath + Form("Row_scan/Y%s_Z0_iter9.root", vScanLabels[y].c_str()));
         vTags.push_back(Form("%s_%s_Y%s_Dt%d", testbeam.c_str(), scanName.c_str(),
                              vScanLabels[y].c_str(), Dt));
         DefaultAnalysis();
      }
      if (Draw_DESY21SingleScan)
         DrawSingleScan(drawWhichMethods);
      ClearVectors();
   }

   // Phi scan with DESY21
   if (DESY_phi) {
      std::vector<int> v_valdrift = {50, 550, 950};
      std::vector<std::string> v_strdrift = {"m40", "460", "860"};
      vScanVals.insert(vScanVals.end(), {0, 10, 20, 29, 31, 40, 45});
      std::vector<int> vScanValsreal = {0, 10, 20, 30, 30, 40, 45};

      for (int iz = 0; iz < (int)v_valdrift.size(); iz++) {
         Dt = DtwithBhere;
         const char *z = v_strdrift[iz].c_str();
         Settings("DESY21", "Phi", Form("Phi_Z%s", z),
                  Form("%2i cm", v_valdrift[iz] / 10), "Track angle #varphi (#circ)", 1,
                  0, 200, Dt, v_valdrift[iz], 40);

         for (int ifile = 0; ifile < (int)std::size(vScanVals); ifile++) {
         // for (int ifile = 4; ifile < (int)std::size(vScanVals); ifile++) {
            int phi = vScanVals[ifile];
            int phireal = vScanValsreal[ifile];
            if (ifile == 0) {
               v_datafiles.push_back(
                  dataScanPath +
                  Form("Drift_scan_PT200/z_360_275_200_02T_26_%s_iter9.root", z));
               vTags.push_back(
                  Form("%s_%s_Phi%i_Dt%d", testbeam.c_str(), scanName.c_str(), phi, Dt));
            } else if (ifile < 4) {
               v_datafiles.push_back(
                  dataScanPath +
                  Form("Phi_scan_z%s/phi_200_%i_z%s_ym60_iter9.root", z, phireal, z));
               vTags.push_back(Form("%s_%s_Phi%i_Dt%d", testbeam.c_str(),
                                    scanName.c_str(), phireal, Dt));
            } else {
               v_datafiles.push_back(
                  dataScanPath + Form("Phi_scan_z%s/phi_200_%i_z%s_ym60_diag_iter9.root",
                                      z, phireal, z));
               vTags.push_back(Form("%s_%s_Phi%i_diag_Dt%d", testbeam.c_str(),
                                    scanName.c_str(), phireal, Dt));
            }

            DefaultAnalysis();
         }
         if (Draw_DESY21SingleScan)
            DrawSingleScan(drawWhichMethods);
         scanIndex++;
      }
      if (Draw_DESY21MultScan)
         DrawMultipleScan(drawWhichMethods);
      ClearVectors();
   }

   // Theta scan with DESY21
   if (DESY_theta) {
      Dt = DtwithBhere;
      Settings("DESY21", "", "Theta", "", "#theta angle (#circ)", 1, 0, 200, Dt, 350, 40);
      selectionSet = "Sel_DESY21_theta";
      vScanVals.insert(vScanVals.end(), {-45, -20, 20});
      vScanLabels.insert(vScanLabels.end(), {"m45", "m20", "20"});

      for (int ifile = 0; ifile < (int)std::size(vScanVals); ifile++) {
         const char *theta = vScanLabels[ifile].c_str();
         v_datafiles.push_back(
            dataScanPath + Form("Theta_scan/theta_%s_02T_z460_ym60_iter9.root", theta));
         vTags.push_back(
            Form("%s_%s_%s_Dt%d", testbeam.c_str(), scanName.c_str(), theta, Dt));

         DefaultAnalysis();
      }
      if (Draw_DESY21SingleScan)
         DrawSingleScan(drawWhichMethods);
      ClearVectors();
   }

   //////////////////////////////////////////////////////////////////////////////////////
   // Drawing
   if (Draw_Comparison) {
      vTags.push_back("");
      std::string tag = "CERN22_Escan_mu_1p5GeV";
      drawoutScanPath = drawoutPath + "CERN22_Energy/" + tag + "/";

      std::string comment1 = "";
      std::string comment2 = "_RCMod_GainMod";

      v_comments.clear();
      v_comments.push_back(comment1);
      v_comments.push_back(comment2);

      v_datafiles.clear();
      v_datafiles.push_back(drawoutScanPath + "dEdx_" + tag + comment1 + ".root");
      v_datafiles.push_back(drawoutScanPath + "dEdx_" + tag + comment2 + ".root");

      p_DrawOuts = new DrawOuts(v_datafiles);
      p_DrawOuts->FileComparison();
   }

   if (Draw_Corrections) {
      p_DrawOuts = new DrawOuts();
      p_DrawOuts->AmplitudeVSLength();
   }
}

/////////////////////////////////////////////////////////////////////////////////////////

// Configuration functions

void Reconstruction::Correction(const int &corrRC, const int &corrGain, const int &corrWF,
                                const int &corrDrift, const int &saveSelectOnly)
{
   fcorrectRC = corrRC;
   if (corrRC == 0)
      comment = comment + "_RCoff";
   if (corrRC == 2)
      comment = comment + "_RCMod";

   fcorrectGain = corrGain;
   if (corrGain == 0)
      comment = comment + "_Gainoff";
   if (corrGain == 2)
      comment = comment + "_GainMod";

   fcorrectWF = corrWF; // Default 1: use one corr func for each
   if (corrWF == 0)     // Use my (40, m40) correction function for all
      comment = comment + "_WF0";
   if (corrWF == 2) // Use hatRecon's correction function
      comment = comment + "_WF2HAT";
   if (corrWF == 3) // Do not use a correction function
      comment = comment + "_WFoff";
   if (corrWF == 4) // Use my (40, m40) corr func shifted down by 100 ADCs for all
      comment = comment + "_WFcorrm100";

   fcorrectDrift = corrDrift;
   if (corrDrift == 0)
      comment = comment + "_DriftFile";

   fsaveSelectOnly = saveSelectOnly;
   if (saveSelectOnly == 0)
      comment = comment + "_wDiscard";
}

void Reconstruction::Settings(const std::string &testbeam_name,
                              const std::string &multiscan_name,
                              const std::string &scan_name, const std::string &scanspec,
                              const std::string &runvarstr_name, const int &uploader,
                              const int &modules, const int &peaking_time,
                              const int &diffusion, const int &drift_dist,
                              const int &timbin)
{

   testbeam = testbeam_name;
   multiScanName = multiscan_name;
   scanName = scan_name;
   v_scanspec.push_back(scanspec);
   runvarstr = runvarstr_name;
   selectionSet = "Sel_" + testbeam;
   dataScanPath = dataPath + "Data_" + testbeam + "/";
   drawoutMultiScanPath = drawoutPath + testbeam + "_" + multiScanName + "/";
   if (multiScanName != "")
      drawoutScanPath = drawoutMultiScanPath + testbeam + "_" + scanName + "/";
   else
      drawoutScanPath = drawoutPath + testbeam + "_" + scanName + "/";
   MakeMyDir(drawoutScanPath);

   intUploader = uploader;
   moduleCase = modules;
   PT = peaking_time;
   if (diffusion == 286) {
      DtB = diffusion;
      DtnoB = 323;
   } else if (diffusion == 323) {
      DtnoB = diffusion;
      DtB = 286;
   } else if (diffusion == 310) {
      DtB = diffusion;
      DtnoB = 350;
   } else if (diffusion == 350) {
      DtnoB = diffusion;
      DtB = 310;
   }
   driftDist = drift_dist;
   TB = timbin;

   if (dedx)
      p_lut = new LUT(DtB, DtnoB, PT);
}

void Reconstruction::ClearVectors()
{
   vScanVals.clear();
   vScanLabels.clear();
   v_scanspec.clear();
   v_comments.clear();
   vTags.clear();
   v_datafiles.clear();
   v_rootout_files.clear();
   scanIndex = 0;
}

void Reconstruction::DefaultAnalysis()
{
   tag = vTags.back();
   drawoutRunPath = drawoutScanPath + tag + "/";
   MakeMyDir(drawoutRunPath);
   rootout_file = drawoutRunPath + tag + comment + ".root";
   if (tag.find("diag") == std::string::npos) {
      std::cout << "comment:	" << comment << std::endl;
      std::cout << "tag:		" << tag << std::endl;
      std::cout << "rootout:	" << rootout_file << std::endl;
      if (comment.find("WF0") != std::string::npos)
         rootout_file.erase(rootout_file.find("_WF0"), 4);
      if (comment.find("WF2HAT") != std::string::npos)
         rootout_file.erase(rootout_file.find("_WF2HAT"), 7);
      if (comment.find("WFoff") != std::string::npos)
         rootout_file.erase(rootout_file.find("_WFoff"), 6);
      if (comment.find("corrv2") != std::string::npos)
         rootout_file.erase(rootout_file.find("_corrv2"), 7);
      if (comment.find("corrm100") != std::string::npos)
         rootout_file.erase(rootout_file.find("_WFcorrm100"), 11);
   }
   std::cout << "rootout:	" << rootout_file << std::endl;
   v_rootout_files.push_back(rootout_file);

   // Correction functions
   if (tag.find("diag") != std::string::npos) {
      corrFuncPath = v_datafiles.back().substr(0, v_datafiles.back().length() - 5) +
                     "_WFCorrFunc.root";
      vcorrFuncPaths.push_back(corrFuncPath);
      if (correction_wf) {
         p_uploader = GiveMe_Uploader(intUploader, v_datafiles.back());
         Reconstruction::WFCorrection(v_datafiles.back());
         delete p_uploader;
      }
   }

   if (dedx) {
      log_file = drawoutRunPath + tag + ".log";
      std::cout << "logs:		" << log_file << std::endl;
      p_uploader = GiveMe_Uploader(intUploader, v_datafiles.back());
      p_dEdx = new dEdx();
      p_dEdx->Reconstruction();
      delete p_uploader;
      delete p_dEdx;
   }

   if (Draw_Control) {
      drawout_file = drawoutRunPath + "Control_" + tag + comment + ".pdf";
      std::cout << "rootout:	" << rootout_file << std::endl;
      std::cout << "drawout:	" << drawout_file << std::endl;
      p_DrawOuts = new DrawOuts(rootout_file);
      p_DrawOuts->Control();
      delete p_DrawOuts;
   }

   if (Draw_dEdx) {
      drawout_file = drawoutRunPath + "dEdx_" + tag + comment + ".pdf";
      p_DrawOuts = new DrawOuts(rootout_file);
      p_DrawOuts->EnergyLoss(drawWhichMethods);
      delete p_DrawOuts;
   }
}

void Reconstruction::DrawSingleScan(const int &methods)
{

   p_DrawOuts = new DrawOuts(v_rootout_files);
   p_DrawOuts->DESY21SingleScan(methods);
   delete p_DrawOuts;
}

void Reconstruction::DrawMultipleScan(const int &methods)
{

   p_DrawOuts = new DrawOuts(v_rootout_files);
   p_DrawOuts->DESY21MultiScan(methods);
   delete p_DrawOuts;
}

void Reconstruction::DrawCERN22Scan()
{

   p_DrawOuts = new DrawOuts(v_rootout_files);
   p_DrawOuts->CERN22Scan();
   delete p_DrawOuts;
}

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	int DESY_ExB = 0;
//	int DESY_drift_139V = 0;
//	int MC_zscan = 0;
//	int DESY19_phi = 0;
//	int MockUp_zdist = 0;

//	// ExB scan scan with DESY21
//	if(DESY_ExB){
//	selectionSet = "Sel_DESY21";
//	drawoutScanPath = drawoutPath + "/DESY21_ExB/";
//	MakeMyDir(drawoutScanPath);
//	intUploader = 1;
//	moduleCase =	0;
//	PT = 200; DtB = 310;; TB = 40;
//	if (control or dedx) p_lut = new
// LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root",
// DtB, PT)); 	std::string B_arr[] = {"0", "04", "06"}; 	std::string Z_arr[] = {"60",
// "460", "860"}; 	int vScanVals[] = {150, 550, 950}; 	for(int j = 0; j < 3; j++){
// const char* B = B_arr[j].c_str(); 		for(int i = 0; i < 3; i++){ const char* z =
// Z_arr[i].c_str(); 		driftDist = vScanVals[i];
//		// v_datafiles.push_back(Form
//("../Data/Data_DESY21/ExB_scan/ExB_360_412ns_B02_ym10_z%s_iter9.root", z));
// vTags.push_back(Form("DESY21_ExB02_Z%s", z));
// v_prtcles.push_back(Form("electron_ExB02_Z%s", z);
//		v_datafiles.push_back(Form
//("../Data/Data_DESY21/ExB_scan/ExB_360_phim3_200ns_B%s_ym60_z%s_iter0.root", B,
// z)); vTags.push_back(Form("DESY21_ExB%s_Z%s", B, z));
// v_prtcles.push_back(Form("electron_ExB%s_Z%s", B, z)); if(control or dedx) p_uploader =
// GiveMe_Uploader (intUploader, v_datafiles.back()); 		if (control) Control
// (drawoutScanPath, vTags.back(), comment, v_datafiles.back(), selectionSet,
// p_uploader, moduleCase, 0, PT, TB, v_prtcles.back()); 		if (Draw_control)
// DrawOut_Control (drawoutScanPath, vTags.back(), comment, selectionSet, 1); 		if
// (dedx)				p_dEdx->Reconstruction(); 		if (Draw_dEdx)			DrawOut_Methods
//(drawoutScanPath, vTags.back(), comment, 1, v_prtcles.back()); 		if (Draw_Checks)
// DrawOut_Checks (drawoutScanPath, v_datafiles.back(), vTags.back(), comment);
//		}
//	}
//	}

//	// Phi scan with DESY19
//	if(DESY19_phi){
//	selectionSet = "Sel_DESY21";
//	intUploader = 1;
//	moduleCase =	0;
//	PT = 412; DtB = 310; TB = 40; driftDist = 430;
//	if (control or dedx) p_lut = new
// LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root",
// DtB, PT)); 	int phi_val[] = {0, 10, 20, 30, 40, 45}; 	int NFiles = 6; drawoutScanPath
// = drawoutPath +
// "/DESY19_phi/"; 	MakeMyDir(drawoutScanPath); 	for (int ifile = 3; ifile < NFiles;
// ifile++){ 		int phi = phi_val[ifile];
//		if(ifile < 3) {
// v_datafiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_iter0.root", phi));
// vTags.push_back(Form("DESY19_phi%i", phi));
// v_prtcles.push_back(Form("electron_phi%i", phi));} 		else {
// v_datafiles.push_back(Form("../Data_DESY19/Phi_scan/phi_412_%i_diag_iter0.root", phi));
// vTags.push_back(Form("DESY19_phi%i_diag", phi));
// v_prtcles.push_back(Form("electron_phi%i_diag", phi));} if(control or dedx) p_uploader
// = GiveMe_Uploader (intUploader, v_datafiles.back()); 		if (control)		Control
//(drawoutScanPath, vTags.back(), comment, v_datafiles.back(), selectionSet, p_uploader,
// moduleCase, 0, PT, TB,
// v_prtcles.back()); 		if (Draw_control)	DrawOut_Control			(drawoutScanPath,
// vTags.back(), comment, selectionSet, 1); 		if (dedx)
// p_dEdx->Reconstruction(); 		if (Draw_Checks)	DrawOut_Checks (drawoutScanPath,
// v_datafiles.back(), vTags.back(), comment); 		if (Draw_dEdx)	DrawOut_Methods
// (drawoutScanPath, vTags.back(), comment, 1, v_prtcles.back()); 		delete p_uploader;
//	}
//	}

//	// z scan with MC
//	if(MC_zscan){
//	selectionSet = "TMC_CERN22_Event";
//	drawoutScanPath = drawoutPath + "/CERN23_MC/MC_zscan/";
//	MakeMyDir(drawoutScanPath);
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; DtB = 286; TB = 40; // PT = 400 actually
//	if (control or dedx) p_lut = new
// LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root",
// DtB, PT)); 	int zmax = 9; 	for (int zDrift = 0; zDrift < zmax; zDrift++){
//		//
// v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_MD_RC100_v3_iter4.root",
// zDrift+1));
// vTags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC
// %i0cm", zDrift+1);
//		v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_NoOpt_iter4.root",
// zDrift+1));
// vTags.push_back(Form("CERN23_MC_z%i00", zDrift+1)); v_prtcles.push_back(Form("MC %i0cm
// NoOpt", zDrift+1));
//		//
// v_datafiles.push_back(Form("../Data_MC/MC_zscan/z_400_nomDrift_%i0cm_RC100_iter4.root",
// zDrift+1));
// vTags.push_back(Form("CERN23_MC_z%i00_old", zDrift+1)); v_prtcles.push_back(Form("MC
// %i0cm old", zDrift+1);
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)		Control		(drawoutScanPath, vTags.back(), comment,
// v_datafiles.back(), selectionSet,
// p_uploader, moduleCase, 0, PT, TB, v_prtcles.back()); 		if (Draw_control)
// DrawOut_Control (drawoutScanPath, vTags.back(), comment, selectionSet, 1); 		if
// (dedx)			p_dEdx->Reconstruction(); 		if (Draw_Checks) DrawOut_Checks
// (drawoutScanPath, v_datafiles.back(), vTags.back(), comment); 		if (Draw_dEdx)
// DrawOut_Methods
//(drawoutScanPath, vTags.back(), comment, 1, v_prtcles.back());
//	}
//	}

//	// z scan scan using the mockup
//	if(MockUp_zdist){
//	selectionSet = "Sel_CERN22";
//	drawoutScanPath = drawoutPath + "/CERN22_Mockup_zscan/";
//	MakeMyDir(drawoutScanPath);
//	intUploader = 2;
//	moduleCase = -1;
//	PT = 412; DtB = 350; driftDist = 415; TB = 40;

//	int NFiles = 10;
//	for (int iFile = 0; iFile < NFiles; iFile++){
//		if (iFile == 0) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z60_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_14V_z060");} 		if (iFile == 1) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z218p5_1_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_14V_z218");} 		if (iFile == 2) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z415_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_14V_z415");} 		if (iFile == 3) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_14V_z572_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_14V_z572");} 		if (iFile == 5) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z60_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_25V_z060");} 		if (iFile == 6) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z218p5_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_25V_z218");} 		if (iFile == 7) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z415_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_25V_z415");} 		if (iFile == 8) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z572_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_25V_z572");} 		if (iFile == 9) {
// v_datafiles.push_back("../Data_CERN22_vD_iter0/All_ERAMS_350V_412ns_e+_0p5GeV_25V_z925_iter0.root");
// vTags.push_back("CERN22_Mockup_e+_25V_z925");}
//		if (Draw_control)	DrawOut_Control			(drawoutScanPath, vTags.back(), comment,
// selectionSet, 8);
//	}
//	}

//	// z scan scan with DESY21 at 139 V/cm
//	if(DESY_drift_139V){
//	selectionSet = "Sel_DESY21";
//	intUploader = 1;
//	moduleCase =	0;
//	DtB = 310; TB = 50;
//	int PT_arr[] = {200};
//	for (int PT : PT_arr){
//		drawoutScanPath = drawoutPath + Form("/DESY21_Drift/DESY21_Drift_PT%i_139V/", PT);
//		MakeMyDir(drawoutScanPath);
//	if (control or dedx) p_lut = new
// LUT(Form("~/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root",
// DtB, PT)); 		int NFiles = 9; 		for (int zDrift = -1; zDrift < NFiles; zDrift++){
// if(zDrift == -1)
//{v_datafiles.push_back(dataScanPath +
// Form("zscan_PT%i_139V/z_360_139_%i_02T_26_m40_iter0.root", PT, PT));
// vTags.push_back(Form("DESY21_zm40_PT%i", PT));			} 		else
// {v_datafiles.push_back(dataScanPath +
// Form("zscan_PT%i_139V/z_360_139_%i_02T_26_%i60_iter0.root", PT, PT, zDrift));
// vTags.push_back(Form("DESY21_z%i60_PT%i", zDrift, PT));
// v_prtcles.push_back(Form("electron_z%i60", zDrift));}
//		if(control or dedx) p_uploader = GiveMe_Uploader (intUploader, v_datafiles.back());
//		if (control)		Control		(drawoutScanPath, vTags.back(), comment,
// v_datafiles.back(), selectionSet,
// p_uploader, moduleCase, 0, PT, TB, v_prtcles.back()); 		if (Draw_control)
// DrawOut_Control (drawoutScanPath, vTags.back(), comment, selectionSet, 1); 		if
// (dedx)			p_dEdx->Reconstruction(); 		if (Draw_dEdx) DrawOut_Methods
// (drawoutScanPath, vTags.back(), comment, 1, v_prtcles.back());
//		}
//		if(Draw_Resolution)	DrawOut_Zscan	(drawoutPath +
// Form("/DESY21_Drift/DESY21_Drift_PT%i_139V", PT), comment,
// PT);
//	}
//	}