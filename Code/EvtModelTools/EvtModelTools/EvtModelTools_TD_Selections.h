#include "Misc/Misc.h"

#include "SampleTools/Uploader.h"
#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"

#include "TH1F.h"
#include "TH2D.h"

// CERN22; Get cut values for the time selection (run based)
std::vector<int> SetStage120Cuts(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF) ;

// Discard clusters misaligned with the tendency of the track
void Selection_TD_TrackAlignement   (Sample& aSample, const int& ModuleNber, const std::string& TAG, const float& cutoff, const int& zdrift) ;
