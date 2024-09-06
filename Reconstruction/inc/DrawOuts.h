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

		void MultipleScanFill();
		void MultipleScanDraw();

		void Control();
		void EnergyLoss();
		void FileComparison();

		void DESY21SingleScan();
		void DESY21Scan(const std::string &drawtype);

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
		std::string ftype = "single";

		// Shared variables
		int NMod = 				0;
		int NClusters =			0;
		int NPads = 			0;
		int ix = 				0;
		int iy = 				0;
		int position = 			0;

		constexpr static float YRESOMIN =	6.5;
		constexpr static float YRESOMAX =	10;
		constexpr static float YMEANMIN =	800;
		constexpr static float YMEANMAX =	1050;
		constexpr static float YSTDMIN =	62;
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

		// Multiple scan pointers
		int nscans =				0;
		int nruns =					0;
		std::vector<int> markers = {22, 34, 23, 47, 33, 43};
		std::vector<TF1*> v_fptf1_WF;
		std::vector<TF1*> v_fptf1_XP;
		std::vector<TGraphErrors*> v_fpTGE_mean_WF;
		std::vector<TGraphErrors*> v_fpTGE_mean_XP;
		std::vector<TGraphErrors*> v_fpTGE_std_WF;
		std::vector<TGraphErrors*> v_fpTGE_std_XP;
		std::vector<TGraphErrors*> v_fpTGE_reso_WF;
		std::vector<TGraphErrors*> v_fpTGE_reso_XP;
		
	};
}

#endif