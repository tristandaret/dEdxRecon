#include "Variables.h"

namespace Reconstruction{

	// Output file variables
	const std::string 			dedx_folder = 			"../";

	const std::string 			data_folder = 			dedx_folder + "Data/";
	std::string 				data_scanfolder = 		"";

	const std::string 			drawout_folder =		dedx_folder + "OUT_Reconstruction/";
	std::string 				drawout_scanfolder =	"";
	std::string					drawout_runfolder =		"";
	std::string 				drawout_file = 			"";

	std::string 				rootout_file = 			"";
	std::string 				log_file = 				"";
	std::string 				comment;
	std::string 				selectionSet;
	std::string					inputDir; // To be removed when the whole drawout code is updated

	// Run variables
	std::vector<std::string> 	v_comments;
	std::vector<std::string> 	v_tags;
	std::vector<std::string> 	v_data_files;
	std::vector<std::string> 	v_rootout_files;
	std::string 				tag;
	std::string					testbeam;
	std::string					scan;
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