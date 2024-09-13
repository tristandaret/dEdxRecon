#include "Variables.h"

namespace Reconstruction{

	// Output file variables
	const std::string 			dedx_path = 				"../";

	const std::string 			data_path = 				dedx_path + "Data/";
	std::string 				data_scanpath = 			"";

	const std::string 			drawout_path =			dedx_path + "OUT_Reconstruction/";
	std::string 				drawout_scanpath =		"";
	std::string 				drawout_metascanpath =	"";
	std::string					drawout_runpath =			"";
	std::string 				drawout_file = 				"";

	std::string 				rootout_file = 				"";
	std::string 				log_file = 					"";
	std::string 				comment;
	std::string 				selectionSet;
	std::string					inputDir; // To be removed when the whole drawout code is updated

	// Run variables
	std::vector<float>	 		v_valint;
	std::vector<std::string>	v_valstr;
	std::vector<std::string>	v_scanspec;
	std::vector<std::string> 	v_comments;
	std::vector<std::string> 	v_tags;
	std::vector<std::string> 	v_datafiles;
	std::vector<std::string> 	v_rootout_files;
	std::string 				tag;
	std::string					testbeam;
	std::string					metascan;
	std::string					scan;
	std::string					runvarstr;
	int 						scanindex = 0;
	Uploader*					pUploader;
	int							intUploader;
	int							moduleCase; // DESY21:0 | CERN22:-1
	int							PT;			// Peaking time (ns)
	int							Dt;			// Transverse diffusion coefficient (µm/sqrt(cm))
	int							driftDist;	// drift distance (mm)
	int							TB;			// timebin length (ns)
	int							fcorrectGain;
	int							fcorrectRC;
	int							fcorrectWF;
	int							fcorrectDrift;
	int 						fsaveSelectOnly;
}