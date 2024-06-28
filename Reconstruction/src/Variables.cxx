#include "Variables.h"

namespace Reconstruction{

	// Output file variables
	std::string outDir	 = "../OUT_Reconstruction/";
	std::string outFile_dEdx;
	std::string comment;
	std::string selectionSet;
	std::string	inputDir;

	// Run variables
	std::vector<std::string> v_comments;
	std::vector<std::string> v_tags;
	std::vector<std::string> v_dataFiles;
	std::vector<std::string> v_prtcles;
	Uploader*	pUploader;
	int		intUploader;
	int		moduleCase; // DESY21:0 | CERN22:-1
	int		PT;		// Peaking time (ns)
	int		Dt;		// Transverse diffusion coefficient (µm/sqrt(cm))
	int		driftDist;	// drift distance (mm)
	int		TB;		// timebin length (ns)
	int		fcorrectGain;
	int		fcorrectRC;
	int		fcorrectWF;
	int		fcorrectDrift;
	

}