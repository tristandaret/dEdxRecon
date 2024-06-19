#ifndef SETSTYLE_H
#define SETSTYLE_H

#include "TStyle.h"
#include "TH1.h"
#include "TPad.h"
// --- T2K style ---

TStyle* SetMyStyle() {
  TStyle *myStyle= new TStyle("tristanstyle", "Tristan's style");

  Int_t FontStyle = 42;
  Float_t FontSizeLabel = 0.045;
  Float_t FontSizeTitle = 0.055;

  // use plain black on white colors
  myStyle->SetFrameBorderMode(0);
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetCanvasBorderSize(0);
  myStyle->SetFrameBorderSize(0);
  myStyle->SetDrawBorder(0);
  myStyle->SetTitleBorderSize(0);

  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(0);
  myStyle->SetStatColor(0);
  myStyle->SetFillColor(0);

  myStyle->SetEndErrorSize(4);
  myStyle->SetStripDecimals(kFALSE);

  myStyle->SetLegendBorderSize(0);
  // myStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  // myStyle->SetPaperSize(20, 26);
  myStyle->SetPadTopMargin(0.08);
  myStyle->SetPadBottomMargin(0.15);
  myStyle->SetPadRightMargin(0.05);
  myStyle->SetPadLeftMargin(0.1);
  
  // Fonts, sizes, offsets
  myStyle->SetTextFont(FontStyle);
  myStyle->SetTextSize(0.04);

  myStyle->SetLabelFont(FontStyle, "x");
  myStyle->SetLabelFont(FontStyle, "y");
  myStyle->SetLabelFont(FontStyle, "z");
  myStyle->SetLabelFont(FontStyle, "t");
  myStyle->SetLabelSize(FontSizeLabel, "x");
  myStyle->SetLabelSize(FontSizeLabel, "y");
  myStyle->SetLabelSize(FontSizeLabel, "z");
  myStyle->SetLabelOffset(0.01, "x");
  myStyle->SetLabelOffset(0.01, "y");
  myStyle->SetLabelOffset(0.01, "z");

  myStyle->SetTitleFont(FontStyle, "x");
  myStyle->SetTitleFont(FontStyle, "y");
  myStyle->SetTitleFont(FontStyle, "z");
  myStyle->SetTitleFont(FontStyle, "t");
  myStyle->SetTitleSize(FontSizeTitle, "y");
  myStyle->SetTitleSize(FontSizeTitle, "x");
  myStyle->SetTitleSize(FontSizeTitle, "z");
  myStyle->SetTitleOffset(1, "x");
  myStyle->SetTitleOffset(1, "y");
  myStyle->SetTitleOffset(1, "z");

  myStyle->SetTitleStyle(0);
  myStyle->SetTitleFontSize(0.06);
  myStyle->SetTitleFont(FontStyle, "pad");
  myStyle->SetTitleBorderSize(0);
  myStyle->SetTitleX(0.1f);
  myStyle->SetTitleW(0.8f);

  // myStyle->SetAxisMaxDigits(3);

  // use bold lines and markers
  myStyle->SetMarkerStyle(20);
  myStyle->SetHistLineWidth( Width_t(2.5) );
  myStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  myStyle->SetErrorX(0.001);
  
  // Standard histogram decorations
  myStyle->SetOptTitle(1);
  // myStyle->SetOptStat(0);
  // myStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);
  
  // -- color --
  myStyle->SetFuncColor(600-4); // blue
  // myStyle->SetFillColor(1); // make color fillings (not white)
  myStyle->SetPalette(kViridis);
  myStyle->SetNumberContours(250);

  // -- axis --
  myStyle->SetStripDecimals(kFALSE); // removes decimals in labels
  myStyle->SetHistMinimumZero(kTRUE); // forces 0 to apeear on y-axis

 return(myStyle);
}














TStyle* SetT2KStyle(Int_t WhichStyle = 1, TString styleName = "T2K") {
  TStyle *t2kStyle= new TStyle(styleName, "T2K approved plots style");
  
  // -- WhichStyle --
  // 1 = presentation large fonts
  // 2 = presentation small fonts
  // 3 = publication/paper

  Int_t FontStyle = 22;
  Float_t FontSizeLabel = 0.035;
  Float_t FontSizeTitle = 0.05;
  Float_t YOffsetTitle = 1.3;
 
  switch(WhichStyle) {
  case 1:
    FontStyle = 42;
    FontSizeLabel = 0.05;
    FontSizeTitle = 0.065;
    YOffsetTitle = 1.19;
    break;
  case 2:
    FontStyle = 42;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  case 3:
    FontStyle = 132;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  }

  // use plain black on white colors
  t2kStyle->SetFrameBorderMode(0);
  t2kStyle->SetCanvasBorderMode(0);
  t2kStyle->SetPadBorderMode(0);
  t2kStyle->SetCanvasBorderSize(0);
  t2kStyle->SetFrameBorderSize(0);
  t2kStyle->SetDrawBorder(0);
  t2kStyle->SetTitleBorderSize(0);

  t2kStyle->SetPadColor(0);
  t2kStyle->SetCanvasColor(0);
  t2kStyle->SetStatColor(0);
  t2kStyle->SetFillColor(0);

  t2kStyle->SetEndErrorSize(4);
  t2kStyle->SetStripDecimals(kFALSE);

  t2kStyle->SetLegendBorderSize(0);
  t2kStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  t2kStyle->SetPaperSize(20, 26);
  t2kStyle->SetPadTopMargin(0.1);
  t2kStyle->SetPadBottomMargin(0.15);
  t2kStyle->SetPadRightMargin(0.13); // 0.075 -> 0.13 for colz option
  t2kStyle->SetPadLeftMargin(0.16);//to include both large/small font options
  
  // Fonts, sizes, offsets
  t2kStyle->SetTextFont(FontStyle);
  t2kStyle->SetTextSize(0.08);

  t2kStyle->SetLabelFont(FontStyle, "x");
  t2kStyle->SetLabelFont(FontStyle, "y");
  t2kStyle->SetLabelFont(FontStyle, "z");
  t2kStyle->SetLabelFont(FontStyle, "t");
  t2kStyle->SetLabelSize(FontSizeLabel, "x");
  t2kStyle->SetLabelSize(FontSizeLabel, "y");
  t2kStyle->SetLabelSize(FontSizeLabel, "z");
  t2kStyle->SetLabelOffset(0.015, "x");
  t2kStyle->SetLabelOffset(0.015, "y");
  t2kStyle->SetLabelOffset(0.015, "z");

  t2kStyle->SetTitleFont(FontStyle, "x");
  t2kStyle->SetTitleFont(FontStyle, "y");
  t2kStyle->SetTitleFont(FontStyle, "z");
  t2kStyle->SetTitleFont(FontStyle, "t");
  t2kStyle->SetTitleSize(FontSizeTitle, "y");
  t2kStyle->SetTitleSize(FontSizeTitle, "x");
  t2kStyle->SetTitleSize(FontSizeTitle, "z");
  t2kStyle->SetTitleOffset(1.14, "x");
  t2kStyle->SetTitleOffset(YOffsetTitle, "y");
  t2kStyle->SetTitleOffset(1.2, "z");

  t2kStyle->SetTitleStyle(0);
  t2kStyle->SetTitleFontSize(0.06);
  t2kStyle->SetTitleFont(FontStyle, "pad");
  t2kStyle->SetTitleBorderSize(0);
  t2kStyle->SetTitleX(0.1f);
  t2kStyle->SetTitleW(0.8f);

  // use bold lines and markers
  t2kStyle->SetMarkerStyle(20);
  t2kStyle->SetHistLineWidth( Width_t(2.5) );
  t2kStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  t2kStyle->SetErrorX(0.001);
  
  // do not display any of the standard histogram decorations
  t2kStyle->SetOptTitle(0);
  // t2kStyle->SetOptStat(0);
  // t2kStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  t2kStyle->SetPadTickX(1);
  t2kStyle->SetPadTickY(1);
  
  // -- color --
  // functions blue
  t2kStyle->SetFuncColor(600-4);

  t2kStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
  Double_t red[]   = { 0.00, 0.00, 0.00 };
  Double_t green[] = { 1.00, 0.00, 0.00 };
  Double_t blue[]  = { 1.00, 1.00, 0.25 };
  // - "warm" red-ish colors -
  //  Double_t red[]   = {1.00, 1.00, 0.25 };
  //  Double_t green[] = {1.00, 0.00, 0.00 };
  //  Double_t blue[]  = {0.00, 0.00, 0.00 };

  Double_t stops[] = { 0.25, 0.75, 1.00 };
  const Int_t NRGBs = 3;
  const Int_t NCont = 500;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  t2kStyle->SetNumberContours(NCont);

  // -- axis --
  t2kStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  t2kStyle->SetHistMinimumZero(kTRUE);


 return(t2kStyle);
}


void CenterHistoTitles(TH1 *thisHisto){
  thisHisto->GetXaxis()->CenterTitle();
  thisHisto->GetYaxis()->CenterTitle();
  thisHisto->GetZaxis()->CenterTitle();
}


int AddGridLinesToPad(TPad *thisPad) {
  thisPad->SetGridx();
  thisPad->SetGridy();
  return(0);
}

#endif