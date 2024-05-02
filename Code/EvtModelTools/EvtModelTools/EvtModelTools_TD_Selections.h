#include "Misc/Misc.h"

#include "SampleTools/Uploader.h"
#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"

#include "TH1F.h"
#include "TH2D.h"

// CERN22; Get cut values for the time selection (run based)
std::vector<int> Comp120_Cut(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF) ;