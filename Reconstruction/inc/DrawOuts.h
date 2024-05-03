#ifndef DRAWOUTS_H
#define DRAWOUTS_H

#include "Misc.h"

// Draw Control plots
void DrawOut_Control    (const std::string& inputDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod);

// Draw Check plots
void DrawOut_Checks(const std::string& OutDir, const std::string EvtFile, const std::string& Tag, const std::string& Comment);

// Draw dE/dx for the different methods
void DrawOut_dEdx       (const std::string& OutDir, const std::string& Tag, const std::string& Comment, const std::string SelectionSet, const int& nMod);

// TH2 comparisons of the different methods
void DrawOut_Methods (const std::string& OutDir, const std::string& Tag, const std::string& Comment, const int& nMod, const std::string& prtcle);

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