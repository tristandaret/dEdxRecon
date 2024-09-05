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

		void SingleScanFill();
		void SingleScanDraw();

		void Control();
		void EnergyLoss();
		void FileComparison();

		void DESY21SingleScan();

	private:
		// Files
		std::string finputFile;
		std::string foutputDir;
		std::string foutputFile;
		
		// Data file
		std::vector<TFile*> finputFiles;
		std::vector<TTree*> finputTrees;
		TFile *fpFile;
		TTree *fpTree;
		std::vector<RecoEvent*> finputEvents;
		std::vector<int> finputnEntries;
		int fnentries;

		// Reconstruction classes
		Reconstruction::RecoEvent 	*p_recoevent;
		Reconstruction::RecoModule 	*p_recomodule;
		Reconstruction::RecoCluster *p_recocluster;
		Reconstruction::RecoPad 	*p_recopad;

		// Settings
		std::string fparticleType;
		TCanvas *fpCanvas;
		TLegend *fpLeg;

		// Shared variables
		int NMod = 				0;
		int NClusters =			0;
		int NPads = 			0;
		int ix = 				0;
		int iy = 				0;
		int position = 			0;

		constexpr static float YRESOMIN =	6;
		constexpr static float YRESOMAX =	10;
		constexpr static float YMEANMIN =	750;
		constexpr static float YMEANMAX =	1050;
		constexpr static float YSTDMIN =	55;
		constexpr static float YSTDMAX =	85;

		// Single scan pointers
		TF1* fptf1_WF;
		TF1* fptf1_XP;
		TGraphErrors* fpTGE_reso_WF;
		TGraphErrors* fpTGE_reso_XP;
		TGraphErrors* fpTGE_mean_WF;
		TGraphErrors* fpTGE_mean_XP;
		TGraphErrors* fpTGE_std_WF;
		TGraphErrors* fpTGE_std_XP;
		
	};
}

#endif