#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "SampleTools/ReadRCmap.h"
#include "SampleTools/ReadGainmap.h"
#include "SampleTools/THATERAMMaps.h"
#include "Tristan/ReadLUT_vROOT.h"
#include "EvtModelTools/JFL_Selector.h"


// void LoadMaps(const std::string &EventFile, std::vector<ReadRCmap*> &RCmaps, std::vector<ReadGainmap*> &Gainmaps, const std::vector<std::string> &eram_id) ;


float avg_Gain(const std::vector<ERAM_map*>& Gainmaps);


// void Fill_Maps(std::vector<ReadRCmap*> &RCmaps, std::vector<ReadGainmap*> &Gainmaps, const std::vector<std::string> &eram_id);


void Init_selection(const std::string &SelectionSet, JFL_Selector &aJFL_Selector, const std::string &Tag, Uploader *pUploader, const int &NbrOfMod, const int &Data_to_Use);


TF1* corr_func(const std::string &EventFile, const std::string &Tag, const int& mode) ;