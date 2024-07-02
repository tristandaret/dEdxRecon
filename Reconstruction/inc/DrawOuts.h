#ifndef DRAWOUTS_H
#define DRAWOUTS_H

#include "Misc.h"
#include "dEdx.h"
#include "SetStyle.h"
namespace Reconstruction{

	class DrawOuts{
	public:
		// Constructors
		DrawOuts(const std::string &inputFile);
		DrawOuts(const std::vector<std::string> &v_inputFiles);
		// Destructor
		virtual ~DrawOuts();

		// Methods
		void SetStyle();

		void Control();
		void EnergyLoss();
		void FileComparison();

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

		// Data
		RecoEvent *fpEvent;
		std::vector<RecoEvent*> finputEvents;
		std::vector<int> finputnEntries;
		int fnentries;

		// Settings
		std::string fparticleType;
		TCanvas *fpCanvas;

		// Control
		TH2I 				*ph2i_heatmap;
		TH1I 				*ph1i_TLead;
		std::vector<TH1I*> 	v_TLead;
		TH1I 				*ph1i_TLeadSel;
		std::vector<TH1I*> 	v_TLeadSel;
		TH1I 				*ph1i_PadMult;
		std::vector<TH1I*> 	v_PadMult;
		TH1I 				*ph1i_PadMultSel;
		std::vector<TH1I*> 	v_PadMultSel;
		TH1F 				*ph1f_AvgPadMult;
		std::vector<TH1F*> 	v_AvgPadMult;
		TH1F 				*ph1f_AvgPadMultSel;
		std::vector<TH1F*> 	v_AvgPadMultSel;
		TH1F 				*ph1f_ADCmax;
		std::vector<TH1F*> 	v_ADCmax;
		TH1F 				*ph1f_ADCmaxSel;
		std::vector<TH1F*> 	v_ADCmaxSel;
		TH1F 				*ph1f_ALead;
		std::vector<TH1F*> 	v_ALead;
		TH1F 				*ph1f_ALeadSel;
		std::vector<TH1F*> 	v_ALeadSel;
		TH1F 				*ph1f_ANeighbour;
		std::vector<TH1F*> 	v_ANeighbour;
		TH1F 				*ph1f_ANeighbourSel;
		std::vector<TH1F*> 	v_ANeighbourSel;
		

	};
}

// Draw Control plots
void DrawOut_Control	(const std::string& inputDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod);

// Draw Check plots
void DrawOut_Checks(const std::string& OutDir, const std::string EvtFile, const std::string& Tag, const std::string& Comment);

// Draw dE/dx for the different methods
void DrawOut_dEdx		(const std::string& OutDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod);

// TH2 comparisons of the different methods
void DrawOut_Methods (const std::string& OutDir, const std::string& Tag, const std::string& Comment, const int& nMod);

// Draw resolution of 2 different procedures
void DrawOut_Versions(const std::string& inputDir, const std::string& Method, const std::string& Comment1, const std::string& Comment2);


// Draw separation power histograms for all methods and particles
void DrawOut_Separation(const std::string& inputDir, const std::string& Comment);

// Draw separation power histograms for all methods and particles only for WF & XP
void DrawOut_Separation_Reduced(const std::string& inputDir, const std::string& Comment, std::string Energy);

// Draw all scans together
void DrawOut_Scans(const std::string& inputDir, const std::string& Comment, const std::string& WFversion);

// Draw resolution as function of Z scan
void DrawOut_Zscan(const std::string& inputDir, const std::string& Comment, const int& PT); 

// Draw resolution as function of Y scan
void DrawOut_Yscan(const std::string& inputDir, const std::string& Comment);

// Draw resolution as function of phi scan
void DrawOut_Phiscan(const std::string& inputDir, const std::string& Comment, const std::string& WFversion, const std::string& zdrift);

// Draw resolution as function of theta scan
void DrawOut_Thetascan(const std::string& inputDir, const std::string& Comment);


// Transverse diffusion coefficient effect on Z scan
void DrawOut_Zscan_PT(const std::string& inputDir, const std::string& Comment);

// Draw resolution as function of phi scan for different drift distances
void DrawOut_Phiscan_Z(const std::string& inputDir, const std::string& Comment, const std::string& WFversion);

// Draw Energy scan
void DrawOut_Escan(const std::string& inputDir, const std::string& Comment);


// TGraphError of WFsum vs L plot for 30 to 45°
void DrawOut_TGE_WFsum_L(const std::string& inputDir, const std::string& Comment);

void DrawOut_verif(const std::string& OutDir, const std::string& Comment);

void DrawOut_corrections();


// DrawOut dE/dx systematics with Z scan
void DrawOut_Systematics(const std::string& inputDir, const std::string& Comment, const std::string& scan);

#endif