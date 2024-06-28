#ifndef MONITORING_H
#define MONITORING_H

namespace Reconstruction{
	void Correction(const int& corrRC	 = 1, const int& corrGain	 = 1, const int& corrWF	 = 1, const int& corrDrift	 = 1);
	void Settings(const std::string &Selection, const std::string &outputDir, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin);
	void DefaultAnalysis();
	void Monitoring();

	// Run selection
	// Files to use
	extern int prototype;
	extern int CERN_Escan;

	extern int DESY_zscan;
	extern int DESY_yscan;
	extern int DESY_phi;
	extern int DESY_theta;

	// Computations	
	extern int control;
	extern int dedx;

	// DrawOuts	
	extern int DO_control;
	extern int DO_Checks;
	extern int DO_dEdx;
	extern int DO_Resolution;
	extern int DO_Global;
	extern int DO_Scans;

	extern int DO_Separation;
	extern int DO_Systematics;
}

#endif