#ifndef SETSTYLE_H
#define SETSTYLE_H

#include "TStyle.h"
#include "TH1.h"
#include "TPad.h"

// --- T2K style ---

TStyle* SetMyStyle();

TStyle* SetT2KStyle(Int_t WhichStyle = 1, TString styleName = "T2K");

void CenterHistoTitles(TH1 *thisHisto);

void AddGridLinesToPad(TPad *thisPad);

#endif