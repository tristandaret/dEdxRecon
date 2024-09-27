#ifndef DRAWOUTS_H
#define DRAWOUTS_H

#include "Misc.h"
#include "dEdx.h"
#include "SetStyle.h"
#include "TLegend.h"
namespace Reconstruction{

	class DrawOuts{
	public:
		// Constructors
		DrawOuts(const std::string &inputFile);
		DrawOuts(const std::vector<std::string> &v_inputFiles);
		// Destructor
		virtual ~DrawOuts();

		// Methods
		void CleanUp();
		void SetStyle();
		
		void DESY21ScanFill();
		void DESY21ScanDraw();
		
		void CERN22ScanFill();
		void CERN22ScanDraw();

		void Control();
		void EnergyLoss();
		void FileComparison();

		void DESY21Scan(const std::string &drawtype);
		void CERN22Scan();

	private:
		// Files
		std::string finputFile;
		std::string foutputDir;
		std::string foutputFile;
		std::string foutputFileWF;
		std::string foutputFileXP;
		
		// Data file
		std::vector<TFile*> 	v_fFiles;
		std::vector<TTree*> 	v_fTrees;
		std::vector<RecoEvent*>	v_fEvents;
		std::vector<int> 		v_fnentries;

		// Reconstruction classes
		Reconstruction::RecoEvent 	*p_recoevent;
		Reconstruction::RecoModule 	*p_recomodule;
		Reconstruction::RecoCluster *p_recocluster;
		Reconstruction::RecoPad 	*p_recopad;

		// Settings
		std::string fparticleType;
		TCanvas *fpCanvas;
		TLegend *fpLeg;
		TLegend *fpLegWF;
		TLegend *fpLegXP;
		std::string ftype = "single";

		// Shared variables
		int NMod = 				0;
		int NClusters =			0;
		int NPads = 			0;
		int ix = 				0;
		int iy = 				0;
		int position = 			0;

		constexpr static float YRESOMAX =	10;
		constexpr static float YRESOMIN =	6.5;
		constexpr static float YMEANMAX =	1150;
		constexpr static float YMEANMIN =	500;
		constexpr static float YSTDMAX =	85;
		constexpr static float YSTDMIN =	62;

		constexpr static float YRESOMAXCERN =	7;
		constexpr static float YRESOMINCERN =	3;
		constexpr static float YMEANMAXCERN =	3;
		constexpr static float YMEANMINCERN =	0.5;
		constexpr static float YSTDMAXCERN =	0.15;
		constexpr static float YSTDMINCERN =	0;
		float keV = 						5.9/(224*1703.74/183); // 5.9 Fe peak energy | 1703 mean MockUp gain | 224 e- created with 5.9keV | 183 e- for 1 ADC

		// Single scan pointers
		TF1* fptf1_WF;
		TF1* fptf1_XP;
		TGraphErrors* fpTGE_reso_WF;
		TGraphErrors* fpTGE_reso_XP;
		TGraphErrors* fpTGE_mean_WF;
		TGraphErrors* fpTGE_mean_XP;
		TGraphErrors* fpTGE_std_WF;
		TGraphErrors* fpTGE_std_XP;

		// Multiple scan pointers
		int nscans =				0;
		int nruns =					0;
		std::vector<int> markers = {22, 34, 23, 47, 33, 43};
		std::vector<int> colors = {kCyan-6, kMagenta-6, kCyan+2, kMagenta+2, kCyan+3, kMagenta+3};
		std::vector<int> markersCERN = {20, 47, 34, 21};
		std::vector<int> colorsCERN = {kOrange+7, kAzure-6, kSpring-6, kRed+1};
		std::vector<TF1*> v_fptf1_WF;
		std::vector<TF1*> v_fptf1_XP;
		std::vector<TGraphErrors*> v_fpTGE_mean_WF;
		std::vector<TGraphErrors*> v_fpTGE_mean_XP;
		std::vector<TGraphErrors*> v_fpTGE_std_WF;
		std::vector<TGraphErrors*> v_fpTGE_std_XP;
		std::vector<TGraphErrors*> v_fpTGE_reso_WF;
		std::vector<TGraphErrors*> v_fpTGE_reso_XP;

		// Bethe-Bloch fitting
		std::vector<TF1*> v_fptf1_BB;
		
	};
}

#endif