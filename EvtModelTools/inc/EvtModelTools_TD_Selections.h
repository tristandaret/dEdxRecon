#include "Misc.h"

#include "Uploader.h"
#include "Sample.h"
#include "Event.h"
#include "Pad.h"

#include "TH1F.h"
#include "TH2D.h"

// CERN22; Get cut values for the time selection (run based)
std::vector<int> Comp120_Cut(Uploader* pUploader, const int& NbrOfMod, const int& Data_to_Use, const int& CloseWF) ;