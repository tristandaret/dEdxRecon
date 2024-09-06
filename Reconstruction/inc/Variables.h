#ifndef Reconstruction_VARIABLES_H
#define Reconstruction_VARIABLES_H

#include "Misc.h"
#include "Uploader.h"
#include "LUTs.h"

namespace Reconstruction{
	
	extern Uploader					*p_uploader;
	extern Reconstruction::LUT		*p_lut;

	// Output file variables
	extern const std::string 		dedx_folder;
	extern const std::string		data_folder;
	extern std::string				data_scanfolder;
	extern const std::string		drawout_folder;
	extern std::string				drawout_metascanfolder;
	extern std::string				drawout_scanfolder;
	extern std::string 				drawout_runfolder;
	extern std::string				drawout_file;
	extern std::string				rootout_file;
	extern std::string				log_file;
	extern std::string				comment;
	extern std::string				selectionSet;
	extern std::string				inputDir;

	// Run variables
	extern std::vector<int>			v_valint;
	extern std::vector<std::string>	v_valstr;
	extern std::vector<std::string>	v_scanspec;
	extern std::vector<std::string> v_comments;
	extern std::vector<std::string> v_tags;
	extern std::vector<std::string> v_datafiles;
	extern std::vector<std::string> v_rootout_files;
	extern std::string				tag;
	extern std::string				testbeam;
	extern std::string				metascan;
	extern std::string				scan;
	extern int 						scanindex;
	extern int						intUploader;
	extern int						moduleCase; // DESY21:0 | CERN22:-1
	extern int						PT;		// Peaking time (ns)
	extern int						Dt;		// Transverse diffusion coefficient (µm/sqrt(cm))
	extern int						driftDist;	// drift distance (mm)
	extern int						TB;		// timebin length (ns)
	extern int						fcorrectGain; // 0: no correction | 1: correct pad per pad | 2: correct ERAM per ERAM
	extern int						fcorrectRC;
	extern int						fcorrectWF;
	extern int						fcorrectDrift;
	extern int 						fsaveSelectOnly;
}

#endif