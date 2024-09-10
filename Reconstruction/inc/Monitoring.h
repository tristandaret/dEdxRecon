#ifndef MONITORING_H
#define MONITORING_H

#include "Uploader.h"
#include "GiveMe_Uploader.h"

#include "dEdx.h"
#include "DrawOuts.h"

namespace Reconstruction{

	extern Reconstruction::dEdx		*p_dEdx;
	extern Reconstruction::DrawOuts	*p_DrawOuts;

	void Monitoring();
	void Correction(const int& corrRC	= 1, const int& corrGain	= 1, const int& corrWF	= 1, const int& corrDrift	= 1, const int& saveSelectOnly = 1);
	void Settings(const std::string &testbeam, const std::string &metascan, const std::string &scan, const std::string &scanspec, const std::string &runvarstr_name, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin);
	void ClearVectors();
	void DefaultAnalysis();
	void DrawSingleScan();
	void DrawMultipleScan();

	// Run selection
	// Files to use
	extern int prototype;
	extern int CERN_Escan;
	extern int CERN_drift;

	extern int DESY_zscan;
	extern int DESY_yscan;
	extern int DESY_phi;
	extern int DESY_theta;

	// Computations	
	extern int dedx;

	// DrawOuts	
	extern int DO_Control;
	extern int DO_dEdx;
	extern int DO_Comparison;
}

#endif