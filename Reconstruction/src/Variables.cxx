#include "Variables.h"

namespace Reconstruction {

// Output file variables
const std::string dEdxPath = "../";

const std::string dataPath = dEdxPath + "Data/";
std::string dataScanPath = "";

const std::string drawoutPath = dEdxPath + "OUT_Reconstruction/";
std::string drawoutMultiScanPath = "";
std::string drawoutScanPath = "";
std::string drawoutRunPath = "";
std::string drawout_file = "";

std::string rootout_file = "";
std::string log_file = "";
std::string comment;
std::string selectionSet;
std::string inputDir;     // To be removed when the whole drawout code is updated
int drawWhichMethods = 0; // 0: both methods | 1: only WF | 2: only XP
std::string corrFuncPath = ""; // Path to the correction functions
std::vector<std::string> vcorrFuncPaths; // Paths to the correction functions

// Run variables
std::vector<float> vScanVals;
std::vector<std::string> vScanLabels;
std::vector<std::string> v_scanspec;
std::vector<std::string> v_comments;
std::vector<std::string> vTags;
std::vector<std::string> v_datafiles;
std::vector<std::string> v_rootout_files;
std::string tag;
std::string testbeam;
std::string multiScanName;
std::string scanName;
std::string runvarstr;
int scanIndex = 0;
Uploader *pUploader;
int intUploader;
int moduleCase; // DESY21:0 | CERN22:-1
int PT;         // Peaking time (ns)
int Dt;         // Transverse diffusion coefficient (µm/sqrt(cm))
int driftDist;  // drift distance (mm)
int TB;         // timebin length (ns)
int fcorrectGain;
int fcorrectRC;
int fcorrectWF;
int fcorrectDrift;
int fsaveSelectOnly;
} // namespace Reconstruction