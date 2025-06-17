#include "DrawOuts.h"
#include "ReconTools.h"
#include "SignalTools.h"
#include "CombinedFit.h"
#include "dEdx.h"
#include "Misc_Functions.h"

#include "Util.h"
#include <cmath>
#include <numeric>

#include "TFile.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TLegendEntry.h"
#include "TROOT.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLatex.h"

// Default constructor
Reconstruction::DrawOuts::DrawOuts()
   : v_fFiles(),
     v_fTrees(),
     v_fEvents(),
     v_fnentries(),
     p_recoevent(nullptr),
     p_recomodule(nullptr),
     p_recocluster(nullptr),
     p_recopad(nullptr),
     fpCanvas(nullptr),
     drawMultiScans(0)
{
   SetStyle();
}

// Constructor delegation
Reconstruction::DrawOuts::DrawOuts(const std::string &inputFile)
   : DrawOuts(std::vector<std::string>{inputFile})
{
}

// Main constructor
Reconstruction::DrawOuts::DrawOuts(const std::vector<std::string> &v_inputFiles)
{
   // Number of runs in a scan
   nRuns = vScanVals.size();
   // Number of scans: number of tags divided by number runs
   nScans = vTags.size() / vScanVals.size();

   for (int ifile = 0; ifile < (int)v_inputFiles.size(); ifile++) {
      v_fEvents.push_back(new RecoEvent());
      v_fFiles.push_back(TFile::Open(v_inputFiles[ifile].c_str()));
      v_fTrees.push_back((TTree *)v_fFiles[ifile]->Get("dEdx_tree"));
      v_fTrees[ifile]->SetBranchAddress("event_branch", &v_fEvents[ifile]);
      v_fnentries.push_back(v_fTrees[ifile]->GetEntries());
   }

   SetStyle();
}

// Destructor
Reconstruction::DrawOuts::~DrawOuts()
{

   for (int i = 0; i < (int)v_fEvents.size(); i++)
      delete v_fEvents[i];
   for (int i = 0; i < (int)v_fTrees.size(); i++)
      delete v_fTrees[i];
   for (int i = 0; i < (int)v_fFiles.size(); i++)
      delete v_fFiles[i];
   v_fEvents.clear();
   v_fTrees.clear();
   v_fFiles.clear();
   v_fnentries.clear();

   delete fpCanvas;
}
// Configuration functions
void Reconstruction::DrawOuts::SetStyle()
{

   fpCanvas = new TCanvas("pCanvas", "pCanvas", 1800, 1350);
   TStyle *ptstyle = SetMyStyle();
   gROOT->SetStyle(ptstyle->GetName());
   gPad->UseCurrentStyle();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Scan functions
void Reconstruction::DrawOuts::DESY21ScanFill()
{

   // If only one scan must be drawn, but belongs to a multi-scan, set the index to the
   // first scan index that corresponds to this scan
   int iStartScan = 0;
   if (drawMultiScans == 0)
      iStartScan = scanIndex;

   // Start iterating on the scans
   for (int iscan = 0; iscan < nScans; iscan++) {
      v_fpTGE_mean_WF.push_back(new TGraphErrors());
      v_fpTGE_mean_XP.push_back(new TGraphErrors());
      v_fpTGE_std_WF.push_back(new TGraphErrors());
      v_fpTGE_std_XP.push_back(new TGraphErrors());
      v_fpTGE_reso_WF.push_back(new TGraphErrors());
      v_fpTGE_reso_XP.push_back(new TGraphErrors());

      // Skip to the correct scan index if only one scan is drawn
      if (iscan < iStartScan)
         continue;

      // Iterate on the runs of the scan
      for (int irun = 0; irun < nRuns; irun++) {
         // Keep track of index in the case of multi-scans
         int index;
         if (drawMultiScans == 0)
            index = irun;
         // For multi-scans, shift the index to take into account the number of scans
         // iterated before this one
         else
            index = iscan * nRuns + irun;

         v_fptf1_WF.push_back(
            (TF1 *)v_fFiles[iscan * nRuns + irun]->Get<TH1F>("ph1f_WF")->GetFunction(
               "gausn"));
         v_fptf1_XP.push_back(
            (TF1 *)v_fFiles[iscan * nRuns + irun]->Get<TH1F>("ph1f_XP")->GetFunction(
               "gausn"));

         // Compute the mean, std, and resolution
         float mean_WF = v_fptf1_WF[index]->GetParameter(1);
         float mean_XP = v_fptf1_XP[index]->GetParameter(1);
         float dmean_WF = v_fptf1_WF[index]->GetParError(1);
         float dmean_XP = v_fptf1_XP[index]->GetParError(1);

         float std_WF = v_fptf1_WF[index]->GetParameter(2);
         float std_XP = v_fptf1_XP[index]->GetParameter(2);
         float dstd_WF = v_fptf1_WF[index]->GetParError(2);
         float dstd_XP = v_fptf1_XP[index]->GetParError(2);

         float reso_WF = std_WF / mean_WF * 100;
         float reso_XP = std_XP / mean_XP * 100;
         float dreso_WF = GetResoError(v_fptf1_WF[index]);
         float dreso_XP = GetResoError(v_fptf1_XP[index]);

         v_fpTGE_mean_WF[iscan]->SetPoint(irun, vScanVals[irun], mean_WF);
         v_fpTGE_mean_XP[iscan]->SetPoint(irun, vScanVals[irun], mean_XP);
         v_fpTGE_mean_WF[iscan]->SetPointError(irun, 0, dmean_WF);
         v_fpTGE_mean_XP[iscan]->SetPointError(irun, 0, dmean_XP);

         v_fpTGE_std_WF[iscan]->SetPoint(irun, vScanVals[irun], std_WF);
         v_fpTGE_std_XP[iscan]->SetPoint(irun, vScanVals[irun], std_XP);
         v_fpTGE_std_WF[iscan]->SetPointError(irun, 0, dstd_WF);
         v_fpTGE_std_XP[iscan]->SetPointError(irun, 0, dstd_XP);

         v_fpTGE_reso_WF[iscan]->SetPoint(irun, vScanVals[irun], reso_WF);
         v_fpTGE_reso_XP[iscan]->SetPoint(irun, vScanVals[irun], reso_XP);
         v_fpTGE_reso_WF[iscan]->SetPointError(irun, 0, dreso_WF);
         v_fpTGE_reso_XP[iscan]->SetPointError(irun, 0, dreso_XP);
      }
   }
}

void Reconstruction::DrawOuts::DESY21ScanDraw()
{
   // Number of algorithms to draw
   fnMethods = 2;                                // Draw both by default
   if (fwhichMethods == 1 or fwhichMethods == 2) // Only draw WF or XP
      fnMethods = 1;
   // shortcut to access the chosen method's tge
   TGraphErrors *tge = nullptr;

   // Output naming
   std::string methodsString;
   if (fwhichMethods == 1)
      methodsString = "_WF";
   else if (fwhichMethods == 2)
      methodsString = "_XP";

   // Variable to get the index of the first scan to draw
   int firstScan = 0;
   int nLegEntries = 2 * nScans; // WF and XP for each scan

   // Shift index if we don't draw all scans
   if (drawMultiScans == 0) {
      firstScan = scanIndex;
   }

   // Drawing settings
   if (drawMultiScans == 1)
      foutputFile = drawoutMultiScanPath + testbeam + "_" + multiScanName + comment +
                    methodsString + "_Dt" + std::to_string(Dt) + ".pdf";
   else
      foutputFile = drawoutScanPath + scanName + comment + methodsString + "_Dt" +
                    std::to_string(Dt) + ".pdf";

   // Styling
   gPad->SetMargin(0.16, 0.03, 0.15, 0.04);
   gStyle->SetTitleSize(0.07, "xy");
   gStyle->SetLabelSize(0.07, "xy");
   gStyle->SetTitleOffset(1.2, "y");
   int marksize = 9;
   int linesize = 3;
   gStyle->SetMarkerSize(marksize);
   fpCanvas->Clear();
   fpCanvas->cd();

   // Legend for single scan
   TLegend legSingle(0.75, 0.76, 0.94, 0.92);
   Graphic_setup(&legSingle, 0.07, kBlue - 1, 1001, kWhite, 1, 1);

   legSingle.AddEntry(v_fpTGE_reso_WF[firstScan], " WF", "p");
   legSingle.AddEntry(v_fpTGE_reso_XP[firstScan], " XP", "p");

   // Legend for multi-scans
   TLegend legMulti(0.94 - 0.21 * fnMethods, 0.84 - 0.04 * nLegEntries, 0.94, 0.92);
   if (fnMethods == 2)
      legMulti.SetHeader("    WF              XP");
   Graphic_setup(&legMulti, 0.07, kBlue - 1, 1001, kWhite, 1, fnMethods);
   for (int i = 0; i < nScans; i++) {
      if (fwhichMethods == 0 or fwhichMethods == 1) // Only WF or both methods
         legMulti.AddEntry(v_fpTGE_reso_WF[i], Form("%s", v_scanspec[i].c_str()), "p");
      if (fwhichMethods == 0 or fwhichMethods == 2) // Only XP or both methods
         legMulti.AddEntry(v_fpTGE_reso_XP[i], Form("%s", v_scanspec[i].c_str()), "p");
   }

   // Resolution
   if (fwhichMethods == 2) // Only XP
      tge = v_fpTGE_reso_XP[firstScan];
   else
      tge = v_fpTGE_reso_WF[firstScan];
   tge->SetMinimum(YRESOMIN);
   tge->SetMaximum(YRESOMAX);
   tge->SetNameTitle("fpTGE_reso_WF", Form(";%s;Resolution [%%]", runvarstr.c_str()));
   if (multiScanName == "Phi")
      tge->GetXaxis()->SetLimits(-3, tge->GetXaxis()->GetXmax());
   // Draw
   for (int i = firstScan; i < (int)v_fpTGE_reso_WF.size(); i++) {
      if (drawMultiScans) {
         Graphic_setup(v_fpTGE_reso_WF[i], marksize, markers[2 * i], colors[2 * i],
                       linesize, colors[2 * i]);
         Graphic_setup(v_fpTGE_reso_XP[i], marksize, markers[2 * i + 1],
                       colors[2 * i + 1], linesize, colors[2 * i + 1]);
      } else {
         Graphic_setup(v_fpTGE_reso_WF[i], marksize, markers[4], colors[2], linesize,
                       colors[2]);
         Graphic_setup(v_fpTGE_reso_XP[i], marksize, markers[3], colors[3], linesize,
                       colors[3]);
      }
      if (fwhichMethods == 1) { // Only draw WF
         if (i == firstScan)
            v_fpTGE_reso_WF[i]->DrawClone("AP");
         else
            v_fpTGE_reso_WF[i]->DrawClone("P same");
      } else if (fwhichMethods == 2) { // Only draw XP
         if (i == firstScan)
            v_fpTGE_reso_XP[i]->DrawClone("AP");
         else
            v_fpTGE_reso_XP[i]->DrawClone("P same");
      } else // Both methods
      {
         if (i == firstScan)
            v_fpTGE_reso_WF[i]->DrawClone("AP");
         else
            v_fpTGE_reso_WF[i]->DrawClone("P same");
         v_fpTGE_reso_XP[i]->DrawClone("P same");
      }
   }
   if (drawMultiScans == 0 and fwhichMethods == 0) // Single scan and both methods
      legSingle.Draw();
   if (drawMultiScans)
      legMulti.Draw();
   fpCanvas->SaveAs((foutputFile + "(").c_str());

   // Mean
   if (fwhichMethods == 2) // Only XP
      tge = v_fpTGE_mean_XP[firstScan];
   else
      tge = v_fpTGE_mean_WF[firstScan];
   tge->SetMinimum(YMEANMIN);
   tge->SetMaximum(YMEANMAX);
   tge->SetNameTitle("fpTGE_mean_WF",
                     Form(";%s;Mean dE/dx [ADC counts/cm]", runvarstr.c_str()));
   if (multiScanName == "Phi")
      tge->GetXaxis()->SetLimits(-3, tge->GetXaxis()->GetXmax());

   // Change legend position for mean
   legSingle.SetX1NDC(0.2);
   legSingle.SetX2NDC(0.39);
   legSingle.SetY1NDC(0.2);
   legSingle.SetY2NDC(0.36);

   legMulti.SetX1NDC(0.17);
   legMulti.SetX2NDC(0.17 + 0.21 * fnMethods);
   legMulti.SetY1NDC(0.45 - 0.04 * nLegEntries);
   legMulti.SetY2NDC(0.55);
   // Draw
   for (int i = firstScan; i < (int)v_fpTGE_mean_WF.size(); i++) {
      if (drawMultiScans) {
         Graphic_setup(v_fpTGE_mean_WF[i], marksize, markers[2 * i], colors[2 * i],
                       linesize, colors[2 * i]);
         Graphic_setup(v_fpTGE_mean_XP[i], marksize, markers[2 * i + 1],
                       colors[2 * i + 1], linesize, colors[2 * i + 1]);
      } else {
         Graphic_setup(v_fpTGE_mean_WF[i], marksize, markers[4], colors[2], linesize,
                       colors[2]);
         Graphic_setup(v_fpTGE_mean_XP[i], marksize, markers[3], colors[3], linesize,
                       colors[3]);
      }
      if (fwhichMethods == 1) { // Only draw WF
         if (i == firstScan)
            v_fpTGE_mean_WF[i]->Draw("AP");
         else
            v_fpTGE_mean_WF[i]->Draw("P same");
      } else if (fwhichMethods == 2) { // Only draw XP
         if (i == firstScan)
            v_fpTGE_mean_XP[i]->Draw("AP");
         else
            v_fpTGE_mean_XP[i]->Draw("P same");
      } else // Both methods
      {
         if (i == firstScan)
            v_fpTGE_mean_WF[i]->Draw("AP");
         else
            v_fpTGE_mean_WF[i]->Draw("P same");
         v_fpTGE_mean_XP[i]->Draw("P same");
      }
   }
   if (drawMultiScans == 0 and fwhichMethods == 0)
      legSingle.Draw();
   if (drawMultiScans)
      legMulti.Draw();
   fpCanvas->SaveAs(foutputFile.c_str());

   // Standard deviation
   if (fwhichMethods == 2) // Only XP
      tge = v_fpTGE_std_XP[firstScan];
   else
      tge = v_fpTGE_std_WF[firstScan];
   tge->SetMinimum(YSTDMIN);
   tge->SetMaximum(YSTDMAX);
   tge->SetNameTitle("fpTGE_std_WF",
                     Form(";%s;Standard deviation [ADC counts/cm]", runvarstr.c_str()));
   if (multiScanName == "Phi")
      tge->GetXaxis()->SetLimits(-3, tge->GetXaxis()->GetXmax());

   // Reset legend positions
   legSingle.SetX1NDC(0.75);
   legSingle.SetX2NDC(0.94);
   legSingle.SetY1NDC(0.76);
   legSingle.SetY2NDC(0.92);

   legMulti.SetX1NDC(0.94 - 0.21 * fnMethods);
   legMulti.SetX2NDC(0.94);
   legMulti.SetY1NDC(0.84 - 0.04 * nLegEntries);
   legMulti.SetY2NDC(0.92);

   // Draw
   for (int i = firstScan; i < (int)v_fpTGE_std_WF.size(); i++) {
      if (drawMultiScans) {
         Graphic_setup(v_fpTGE_std_WF[i], marksize, markers[2 * i], colors[2 * i],
                       linesize, colors[2 * i]);
         Graphic_setup(v_fpTGE_std_XP[i], marksize, markers[2 * i + 1], colors[2 * i + 1],
                       linesize, colors[2 * i + 1]);
      } else {
         Graphic_setup(v_fpTGE_std_WF[i], marksize, markers[4], colors[2], linesize,
                       colors[2]);
         Graphic_setup(v_fpTGE_std_XP[i], marksize, markers[3], colors[3], linesize,
                       colors[3]);
      }
      if (fwhichMethods == 1) { // Only draw WF
         if (i == firstScan)
            v_fpTGE_std_WF[i]->Draw("AP");
         else
            v_fpTGE_std_WF[i]->Draw("P same");
      } else if (fwhichMethods == 2) { // Only draw XP
         if (i == firstScan)
            v_fpTGE_std_XP[i]->Draw("AP");
         else
            v_fpTGE_std_XP[i]->Draw("P same");
      } else // Both methods
      {
         if (i == firstScan)
            v_fpTGE_std_WF[i]->Draw("AP");
         else
            v_fpTGE_std_WF[i]->Draw("P same");
         v_fpTGE_std_XP[i]->Draw("P same");
      }
   }
   if (drawMultiScans == 0 and fwhichMethods == 0)
      legSingle.Draw();
   if (drawMultiScans)
      legMulti.Draw();
   fpCanvas->SaveAs((foutputFile + ")").c_str());

   delete tge;
}

void Reconstruction::DrawOuts::CERN22ScanFill()
{

   // Bethel-Bloch fitting
   std::string particles[] = {" e^{+}", " #mu^{+}", " #pi^{+}", " p"};
   std::vector<double> v_mass{0.511e-3, 105.658e-3, 139.570e-3, 938.272e-3}; // GeV

   int itotalrun = 0;
   std::vector<int> v_runs = {5, 3, 3, 3};
   for (int iscan = 0; iscan < 4; iscan++) { // 4 types of particles

      v_fpTGE_mean_WF.push_back(new TGraphErrors());
      v_fpTGE_mean_XP.push_back(new TGraphErrors());
      v_fpTGE_std_WF.push_back(new TGraphErrors());
      v_fpTGE_std_XP.push_back(new TGraphErrors());
      v_fpTGE_reso_WF.push_back(new TGraphErrors());
      v_fpTGE_reso_XP.push_back(new TGraphErrors());
      v_fptf1_BB.push_back(BetheBlochExp(0.25, 20, v_mass[iscan], particles[iscan]));

      for (int irun = 0; irun < v_runs[iscan]; irun++) {

         fptf1_WF =
            (TF1 *)v_fFiles[itotalrun]->Get<TH1F>("ph1f_WF")->GetFunction("gausn");
         fptf1_XP =
            (TF1 *)v_fFiles[itotalrun]->Get<TH1F>("ph1f_XP")->GetFunction("gausn");

         float mean_WF = fptf1_WF->GetParameter(1);
         float mean_XP = fptf1_XP->GetParameter(1);
         float dmean_WF = fptf1_WF->GetParError(1);
         float dmean_XP = fptf1_XP->GetParError(1);

         float std_WF = fptf1_WF->GetParameter(2);
         float std_XP = fptf1_XP->GetParameter(2);
         float dstd_WF = fptf1_WF->GetParError(2);
         float dstd_XP = fptf1_XP->GetParError(2);

         float reso_WF = std_WF / mean_WF * 100;
         float reso_XP = std_XP / mean_XP * 100;
         float dreso_WF = GetResoError(fptf1_WF);
         float dreso_XP = GetResoError(fptf1_XP);

         v_fpTGE_mean_WF[iscan]->SetPoint(irun, vScanVals[itotalrun], mean_WF * keV);
         v_fpTGE_mean_XP[iscan]->SetPoint(irun, vScanVals[itotalrun], mean_XP * keV);
         v_fpTGE_mean_WF[iscan]->SetPointError(irun, 0, dmean_WF * keV);
         v_fpTGE_mean_XP[iscan]->SetPointError(irun, 0, dmean_XP * keV);

         v_fpTGE_std_WF[iscan]->SetPoint(irun, vScanVals[itotalrun], std_WF * keV);
         v_fpTGE_std_XP[iscan]->SetPoint(irun, vScanVals[itotalrun], std_XP * keV);
         v_fpTGE_std_WF[iscan]->SetPointError(irun, 0, dstd_WF * keV);
         v_fpTGE_std_XP[iscan]->SetPointError(irun, 0, dstd_XP * keV);

         v_fpTGE_reso_WF[iscan]->SetPoint(irun, vScanVals[itotalrun], reso_WF);
         v_fpTGE_reso_XP[iscan]->SetPoint(irun, vScanVals[itotalrun], reso_XP);
         v_fpTGE_reso_WF[iscan]->SetPointError(irun, 0, dreso_WF);
         v_fpTGE_reso_XP[iscan]->SetPointError(irun, 0, dreso_XP);

         delete fptf1_WF;
         delete fptf1_XP;

         itotalrun++;
      }
      // v_fptf1_BB[iscan]->			SetParameters(v_mass[iscan]
      // ,1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, 7.14413e-01);
   }
   combinedFit(v_fpTGE_mean_XP, v_fptf1_BB);

   for (int iparticle = 0; iparticle < 4; iparticle++) {
      std::cout << particles[iparticle] << ": ";
      for (int ipar = 0; ipar < 6; ipar++) {
         std::cout << v_fptf1_BB[iparticle]->GetParameter(ipar) << " ";
      }
      std::cout << std::endl;
   }
}

void Reconstruction::DrawOuts::CERN22ScanDraw()
{

   // Drawing settings
   foutputFileWF =
      drawoutScanPath + scanName + comment + "_WF_Dt" + std::to_string(Dt) + ".pdf";
   foutputFileXP =
      drawoutScanPath + scanName + comment + "_XP_Dt" + std::to_string(Dt) + ".pdf";
   gPad->SetRightMargin(0.04);
   gPad->SetTopMargin(0.04);
   fpCanvas->Clear();
   fpCanvas->cd();
   // Legend
   TLegend *fpLeg = new TLegend(0.75, 0.78, 0.95, 0.93);
   fpLeg->SetTextSize(0.05);
   fpLeg->SetFillStyle(0);
   fpLeg->SetTextColor(kBlack);
   fpLeg->SetNColumns(2);
   fpLeg->AddEntry(v_fpTGE_reso_WF[0], " e^{+}", "p");
   fpLeg->AddEntry(v_fpTGE_reso_WF[1], " #mu^{+}", "p");
   fpLeg->AddEntry(v_fpTGE_reso_WF[3], " p", "p");
   fpLeg->AddEntry(v_fpTGE_reso_WF[2], " #pi^{+}", "p");

   // WF
   // Resolution
   v_fpTGE_reso_WF[0]->SetMaximum(YRESOMAXCERN);
   v_fpTGE_reso_WF[0]->SetMinimum(YRESOMINCERN);
   v_fpTGE_reso_WF[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_reso_WF[0]->SetNameTitle("fpTGE_reso_WF",
                                    Form(";%s;Resolution [%%]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_reso_WF[i], 4, markersCERN[i], colorsCERN[i], 1,
                    colorsCERN[i]);
      v_fpTGE_reso_WF[i]->Draw(i == 0 ? "AP" : "P same");
      v_fpTGE_reso_WF[i]->SetMarkerSize(5);
   }
   fpLeg->Draw();
   fpCanvas->SaveAs((foutputFileWF + "(").c_str());

   // Mean
   fpCanvas->Clear();
   v_fpTGE_mean_WF[0]->SetMaximum(YMEANMAXCERN);
   v_fpTGE_mean_WF[0]->SetMinimum(YMEANMINCERN);
   v_fpTGE_mean_WF[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_mean_WF[0]->SetNameTitle(
      "fpTGE_mean_WF", Form(";%s;Mean dE/dx [ADC counts/cm]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_mean_WF[i], 4, markersCERN[i], colorsCERN[i], 2,
                    colorsCERN[i]);
      v_fpTGE_mean_WF[i]->Draw(i == 0 ? "AP" : "P same");
   }
   fpLeg->Draw();
   fpCanvas->SaveAs(foutputFileWF.c_str());

   // Standard deviation
   fpCanvas->Clear();
   v_fpTGE_std_WF[0]->SetMaximum(YSTDMAXCERN);
   v_fpTGE_std_WF[0]->SetMinimum(YSTDMINCERN);
   v_fpTGE_std_WF[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_std_WF[0]->SetNameTitle(
      "fpTGE_std_WF", Form(";%s;Standard deviation [ADC counts/cm]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_std_WF[i], 4, markersCERN[i], colorsCERN[i], 2,
                    colorsCERN[i]);
      v_fpTGE_std_WF[i]->Draw(i == 0 ? "AP" : "P same");
   }
   fpLeg->Draw();
   fpCanvas->SaveAs((foutputFileWF + ")").c_str());

   // XP
   // Resolution
   fpCanvas->Clear();
   v_fpTGE_reso_XP[0]->SetMaximum(YRESOMAXCERN);
   v_fpTGE_reso_XP[0]->SetMinimum(YRESOMINCERN);
   v_fpTGE_reso_XP[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_reso_XP[0]->SetNameTitle("fpTGE_reso_XP",
                                    Form(";%s;Resolution [%%]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_reso_XP[i], 4, markersCERN[i], colorsCERN[i], 2,
                    colorsCERN[i]);
      v_fpTGE_reso_XP[i]->Draw(i == 0 ? "AP" : "P same");
   }
   fpLeg->Draw();
   fpCanvas->SaveAs((foutputFileXP + "(").c_str());

   // Mean
   fpCanvas->Clear();
   v_fpTGE_mean_XP[0]->SetMaximum(YMEANMAXCERN);
   v_fpTGE_mean_XP[0]->SetMinimum(YMEANMINCERN);
   v_fpTGE_mean_XP[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_mean_XP[0]->SetNameTitle(
      "fpTGE_mean_XP", Form(";%s;Mean dE/dx [ADC counts/cm]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_mean_XP[i], 4, markersCERN[i], colorsCERN[i], 2,
                    colorsCERN[i]);
      v_fpTGE_mean_XP[i]->Draw(i == 0 ? "AP" : "P same");
      v_fptf1_BB[i]->SetNpx(1000);
      v_fptf1_BB[i]->SetLineColor(colorsCERN[i]);
      v_fptf1_BB[i]->Draw("same");
   }
   fpLeg->Draw();
   fpCanvas->SaveAs(foutputFileXP.c_str());

   // Bethe-Bloch extended
   fpCanvas->Clear();
   gPad->SetLogx();
   gPad->SetLogy();
   v_fptf1_BB[0]->SetMinimum(0.7);
   v_fptf1_BB[0]->SetMaximum(5);
   v_fptf1_BB[0]->Draw();
   for (int i = 1; i < 4; i++)
      v_fptf1_BB[i]->Draw("same");
   fpLeg->Draw();
   fpCanvas->SaveAs(foutputFileXP.c_str());

   gPad->SetLogx(0);
   gPad->SetLogy(0);
   // Standard deviation
   fpCanvas->Clear();
   v_fpTGE_std_XP[0]->SetMaximum(YSTDMAXCERN);
   v_fpTGE_std_XP[0]->SetMinimum(YSTDMINCERN);
   v_fpTGE_std_XP[0]->GetXaxis()->SetLimits(0.25, 1.75);
   v_fpTGE_std_XP[0]->SetNameTitle(
      "fpTGE_std_XP", Form(";%s;Standard deviation [ADC counts/cm]", runvarstr.c_str()));
   for (int i = 0; i < 4; i++) {
      Graphic_setup(v_fpTGE_std_XP[i], 4, markersCERN[i], colorsCERN[i], 2,
                    colorsCERN[i]);
      v_fpTGE_std_XP[i]->Draw(i == 0 ? "AP" : "P same");
   }
   fpLeg->Draw();
   fpCanvas->SaveAs((foutputFileXP + ")").c_str());

   delete fpLeg;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Single run functions
void Reconstruction::DrawOuts::Control()
{

   // Prepare histograms
   TH1I *ph1i_nmodules =
      new TH1I("ph1i_nmodules", "Number of modules;N_{modules};Counts", 8, 0, 8);
   TH1I *ph1i_nmodulesSel = new TH1I(
      "ph1i_nmodulesSel", "Number of modules (after cut);N_{modules};Counts", 8, 0, 8);
   TH1I *ph1i_nclusters =
      new TH1I("ph1i_nclusters", "Number of clusters;N_{clusters};Counts", 55, 0, 55);
   TH1I *ph1i_nclustersSel =
      new TH1I("ph1i_nclustersSel", "Number of clusters (after cut);N_{clusters};Counts",
               55, 0, 55);
   TH1I *ph1i_npads = new TH1I("ph1i_npads", "Number of pads;N_{pads};Counts", 10, 0, 10);
   TH1I *ph1i_npadsSel =
      new TH1I("ph1i_npadsSel", "Number of pads (after cut);N_{pads};Counts", 10, 0, 10);

   TH2I *ph2i_heatmap =
      new TH2I("ph2i_heatmap", "Events heatmap;iX;iY;", 144, 0, 143, 64, 0, 63);
   TH1I *ph1i_TLead =
      new TH1I("ph1i_TLead", "T_{max} of leading pad;T_{max};Counts", 511, 0, 510);
   TH1I *ph1i_TLeadSel = new TH1I(
      "ph1i_TLeadSel", "T_{max} of leading pad (after cut);T_{max};Counts", 511, 0, 510);
   TH1I *ph1i_PadMult =
      new TH1I("ph1i_PadMult", "Pad multiplicity;N_{pads};Counts", 11, 0, 10);
   TH1I *ph1i_PadMultSel = new TH1I(
      "ph1i_PadMultSel", "Pad multiplicity (after cut);N_{pads};Counts", 11, 0, 10);
   TH1F *ph1f_AvgPadMult =
      new TH1F("ph1f_AvgPadMult", "Average pad multiplicity;N_{pads};Counts", 100, 0, 10);
   TH1F *ph1f_AvgPadMultSel =
      new TH1F("ph1f_AvgPadMultSel",
               "Average pad multiplicity (after cut);N_{pads};Counts", 100, 0, 10);
   TH1F *ph1f_ADCmax =
      new TH1F("ph1f_ADCmax", "ADC max;ADC counts/cm;Counts", 400, 0, 4000);
   TH1F *ph1f_ADCmaxSel = new TH1F(
      "ph1f_ADCmaxSel", "ADC max (after cut);ADC counts/cm;Counts", 400, 0, 4000);
   TH1F *ph1f_ALead =
      new TH1F("ph1f_ALead", "Leading pad amplitude;ADC counts/cm;Counts", 400, 0, 4000);
   TH1F *ph1f_ALeadSel =
      new TH1F("ph1f_ALeadSel", "Leading pad amplitude (after cut);ADC counts/cm;Counts",
               400, 0, 4000);
   TH1F *ph1f_ANeighbour = new TH1F(
      "ph1f_ANeighbour", "Neighbouring pad amplitude;ADC counts/cm;Counts", 400, 0, 4000);
   TH1F *ph1f_ANeighbourSel = new TH1F(
      "ph1f_ANeighbourSel", "Neighbouring pad amplitude (after cut);ADC counts/cm;Counts",
      400, 0, 4000);
   TH1F *ph1f_yCluster = new TH1F(
      "ph1f_yCluster",
      "Vertical cluster position;vertical position (cm);Normalized counts", 100, 4, 14);
   TH1F *ph1f_yWeight = new TH1F("ph1f_yWeight",
                                 "Vertical cluster position;vertical "
                                 "position (cm);Normalized counts",
                                 100, 4, 14);
   TH1F *ph1f_dyClus = new TH1F(
      "ph1f_dyClus", "\\Deltay position};\\Deltay (cm);Normalized counts", 100, -5, 5);
   TH1F *ph1f_dyWeight = new TH1F(
      "ph1f_dyWeight", "\\Deltay position;\\Deltay (cm);Normalized counts", 100, -5, 5);
   std::vector<TH1F *> v_AvgPadMult;
   std::vector<TH1F *> v_AvgPadMultSel;
   std::vector<TH1I *> v_PadMult;
   std::vector<TH1I *> v_PadMultSel;
   std::vector<TH1I *> v_TLead;
   std::vector<TH1I *> v_TLeadSel;
   std::vector<TH1F *> v_ADCmax;
   std::vector<TH1F *> v_ADCmaxSel;
   std::vector<TH1F *> v_ALead;
   std::vector<TH1F *> v_ALeadSel;
   std::vector<TH1F *> v_ANeighbour;
   std::vector<TH1F *> v_ANeighbourSel;
   std::vector<TH1F *> v_yCluster;
   std::vector<TH1F *> v_yClusterSel;
   std::vector<TH1F *> v_yWeight;
   std::vector<TH1F *> v_dyClus;
   std::vector<TH1F *> v_dyWeight;

   for (int i = 0; i < 8; i++) {
      v_TLead.push_back(new TH1I(
         Form("ph1i_TLead_%i", i),
         Form("T_{max} of leading pad (module %i);T_{max};Counts", i), 511, 0, 510));
      v_TLeadSel.push_back(new TH1I(
         Form("ph1i_TLeadSel_%i", i),
         Form("T_{max} of leading pad (module %i) (after cut);T_{max};Counts", i), 511, 0,
         510));
      v_PadMult.push_back(
         new TH1I(Form("ph1i_PadMult_%i", i),
                  Form("Pad multiplicity (module %i);N_{pads};Counts", i), 100, 0, 100));
      v_PadMultSel.push_back(
         new TH1I(Form("ph1i_PadMultSel_%i", i),
                  Form("Pad multiplicity (module %i) (after cut);N_{pads};Counts", i),
                  100, 0, 100));
      v_AvgPadMult.push_back(new TH1F(
         Form("ph1f_AvgPadMult_%i", i),
         Form("Average pad multiplicity (module %i);N_{pads};Counts", i), 100, 0, 100));
      v_AvgPadMultSel.push_back(new TH1F(
         Form("ph1f_AvgPadMultSel_%i", i),
         Form("Average pad multiplicity (module %i) (after cut);N_{pads};Counts", i), 100,
         0, 100));
      v_ADCmax.push_back(new TH1F(Form("ph1f_ADCmax_%i", i),
                                  Form("ADC max (module %i);ADC counts/cm;Counts", i),
                                  400, 0, 4000));
      v_ADCmaxSel.push_back(new TH1F(
         Form("ph1f_ADCmaxSel_%i", i),
         Form("ADC max (module %i) (after cut);ADC counts/cm;Counts", i), 400, 0, 4000));
      v_ALead.push_back(
         new TH1F(Form("ph1f_ALead_%i", i),
                  Form("Leading pad amplitude (module %i);ADC counts/cm;Counts", i), 400,
                  0, 4000));
      v_ALeadSel.push_back(new TH1F(
         Form("ph1f_ALeadSel_%i", i),
         Form("Leading pad amplitude (module %i) (after cut);ADC counts/cm;Counts", i),
         400, 0, 4000));
      v_ANeighbour.push_back(
         new TH1F(Form("ph1f_ANeighbour_%i", i),
                  Form("Neighbouring pad amplitude (module %i);ADC counts/cm;Counts", i),
                  400, 0, 4000));
      v_ANeighbourSel.push_back(new TH1F(
         Form("ph1f_ANeighbourSel_%i", i),
         Form("Neighbouring pad amplitude (module %i) (after cut);ADC counts/cm;Counts",
              i),
         400, 0, 4000));
   }

   for (int i = 0; i < 36; i++) {
      v_yCluster.push_back(new TH1F(Form("ph1f_yCluster_%i", i),
                                    Form("Vertical cluster position (column %i);vertical "
                                         "position (cm);Normalized counts",
                                         i),
                                    100, 4, 14));
      v_yWeight.push_back(new TH1F(Form("ph1f_yWeight_%i", i),
                                   Form("Vertical cluster position (column %i);vertical "
                                        "position (cm);Normalized counts",
                                        i),
                                   100, 4, 14));
      v_dyClus.push_back(new TH1F(Form("ph1f_dyClus_%i", i),
                                  Form("#Deltay (column %i);#Deltay (cm);Counts", i), 100,
                                  -5, 5));
      v_dyWeight.push_back(new TH1F(Form("ph1f_dyWeight_%i", i),
                                    Form("#Deltay (column %i);#Deltay (cm);Counts", i),
                                    100, -5, 5));
   }

   // Get entries and fill histograms
   for (int i = 0; i < v_fnentries.back(); i++) {
      v_fTrees.back()->GetEntry(i);
      p_recoevent = v_fEvents.back();
      NMod = p_recoevent->v_modules.size();
      int NSelMod = 0;

      for (int iMod = 0; iMod < NMod; iMod++) {
         p_recomodule = p_recoevent->v_modules[iMod];
         position = p_recomodule->position;
         int modCol = position % 4;
         NClusters = p_recomodule->NClusters;
         int NSelClus = 0;

         for (int iC = 0; iC < NClusters; iC++) {
            p_recocluster = p_recomodule->v_clusters[iC];
            int clusCol = 36 * modCol + p_recocluster->v_pads[0]->ix;
            NPads = p_recocluster->NPads;
            int NSelPads = 0;

            for (int iP = 0; iP < NPads; iP++) {
               // Heatmap
               ix = p_recocluster->v_pads[iP]->ix;
               iy = p_recocluster->v_pads[iP]->iy;
               if (position < 4)
                  iy += 32;
               ix += 36 * (position % 4);
               ph2i_heatmap->Fill(ix, iy);

               p_recopad = p_recocluster->v_pads[iP];
               ph1f_ADCmax->Fill(p_recopad->ADCmax_base);
               v_ADCmax[position]->Fill(p_recopad->ADCmax_base);
               if (!p_recopad->leading) {
                  ph1f_ANeighbour->Fill(p_recopad->ADCmax_base);
                  v_ANeighbour[position]->Fill(p_recopad->ADCmax_base);
               }
               int TPad = p_recopad->TMax;

               ph1f_dyClus->Fill(p_recopad->dy);
               v_dyClus[clusCol]->Fill(p_recopad->dy);
               ph1f_dyWeight->Fill(p_recocluster->yWeight - p_recopad->yPad);
               v_dyWeight[clusCol]->Fill(p_recocluster->yWeight - p_recopad->yPad);

               if (!p_recomodule->selected)
                  continue;
               NSelPads++;
               ph1f_ADCmaxSel->Fill(p_recopad->ADCmax_base);
               v_ADCmaxSel[position]->Fill(p_recopad->ADCmax_base);
               if (!p_recopad->leading) {
                  ph1f_ANeighbourSel->Fill(p_recopad->ADCmax_base);
                  v_ANeighbourSel[position]->Fill(p_recopad->ADCmax_base);
               }
            } // Pads
            ph1i_npads->Fill(NPads);
            ph1i_npadsSel->Fill(NSelPads);

            ph1i_TLead->Fill(p_recocluster->TLead);
            ph1i_PadMult->Fill(NPads);
            ph1f_ALead->Fill(p_recocluster->ALead_base);
            ph1f_yCluster->Fill(p_recocluster->yCluster);
            ph1f_yWeight->Fill(p_recocluster->yWeight);

            v_TLead[position]->Fill(p_recocluster->TLead);
            v_PadMult[position]->Fill(NPads);
            v_ALead[position]->Fill(p_recocluster->ALead_base);
            // Not readyClus for CERN22 and multiple modules
            v_yCluster[clusCol]->Fill(p_recocluster->yCluster);
            v_yWeight[clusCol]->Fill(p_recocluster->yWeight);

            if (!p_recomodule->selected)
               continue;
            NSelClus++;
            ph1i_TLeadSel->Fill(p_recocluster->TLead);
            ph1i_PadMultSel->Fill(NPads);
            ph1f_ALeadSel->Fill(p_recocluster->ALead_base);
            v_TLeadSel[position]->Fill(p_recocluster->TLead);
            v_PadMultSel[position]->Fill(NPads);
            v_ALeadSel[position]->Fill(p_recocluster->ALead_base);
         } // Clusters
         ph1i_nclusters->Fill(NClusters);
         ph1i_nclustersSel->Fill(NSelClus);

         ph1f_AvgPadMult->Fill(p_recomodule->avg_pad_mult);
         v_AvgPadMult[position]->Fill(p_recomodule->avg_pad_mult);

         if (!p_recomodule->selected)
            continue;
         NSelMod++;
         ph1f_AvgPadMultSel->Fill(p_recomodule->avg_pad_mult);
         v_AvgPadMultSel[position]->Fill(p_recomodule->avg_pad_mult);
      } // Modules
      ph1i_nmodules->Fill(NMod);
      ph1i_nmodulesSel->Fill(NSelMod);
   }
   //  Renormalize Y histograms
   ph1f_yCluster->Scale(1. / ph1f_yCluster->Integral());
   ph1f_yWeight->Scale(1. / ph1f_yWeight->Integral());
   ph1f_dyClus->Scale(1. / ph1f_dyClus->Integral());
   ph1f_dyWeight->Scale(1. / ph1f_dyWeight->Integral());
   for (int i = 0; i < 36; i++) {
      v_yCluster[i]->Scale(1. / v_yCluster[i]->Integral());
      v_yWeight[i]->Scale(1. / v_yWeight[i]->Integral());
      v_dyClus[i]->Scale(1. / v_dyClus[i]->Integral());
      v_dyWeight[i]->Scale(1. / v_dyWeight[i]->Integral());
   }

   // Draw
   gStyle->SetOptStat("nmerou");
   gStyle->SetOptFit(0);

   // Get maximum of module plots

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->cd();
   fpCanvas->Clear();
   ph1i_TLead->SetLineWidth(3);
   ph1i_TLead->Draw("HIST");
   ph1i_TLeadSel->SetLineWidth(2);
   ph1i_TLeadSel->SetLineColor(kRed);
   ph1i_TLeadSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   fpCanvas->Divide(4, 2);
   for (int i = 0; i < 8; i++) {
      fpCanvas->cd(i + 1);
      v_TLeadSel[i]->SetLineColor(kRed);
      v_TLead[i]->Draw("HIST");
      v_TLeadSel[i]->Draw("HIST sames");
   }
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->cd();
   gPad->SetLogy();
   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   ph1f_ADCmax->SetLineWidth(3);
   ph1f_ADCmax->Draw("HIST");
   ph1f_ADCmaxSel->SetLineWidth(2);
   ph1f_ADCmaxSel->SetLineColor(kRed);
   ph1f_ADCmaxSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   ph1f_ALead->SetLineWidth(3);
   ph1f_ALead->Draw("HIST");
   ph1f_ALeadSel->SetLineWidth(2);
   ph1f_ALeadSel->SetLineColor(kRed);
   ph1f_ALeadSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   ph1f_ANeighbour->SetLineWidth(3);
   ph1f_ANeighbour->Draw("HIST");
   ph1f_ANeighbourSel->SetLineWidth(2);
   ph1f_ANeighbourSel->SetLineColor(kRed);
   ph1f_ANeighbourSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   gPad->SetLogy(0);
   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   ph1i_PadMult->SetLineWidth(3);
   ph1i_PadMult->Draw("HIST");
   ph1i_PadMultSel->SetLineWidth(2);
   ph1i_PadMultSel->SetLineColor(kRed);
   ph1i_PadMultSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   ph1f_AvgPadMult->SetLineWidth(3);
   ph1f_AvgPadMult->Draw("HIST");
   ph1f_AvgPadMultSel->SetLineWidth(2);
   ph1f_AvgPadMultSel->SetLineColor(kRed);
   ph1f_AvgPadMultSel->Draw("HIST sames");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   gPad->SetRightMargin(0.12);
   gPad->SetLeftMargin(0.12);
   ph2i_heatmap->GetYaxis()->SetTitleOffset(1.1);
   gStyle->SetTitleX((1. - gPad->GetRightMargin() + gPad->GetLeftMargin()) / 2);
   ph2i_heatmap->Draw("COLZ");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   // Y position in clusters for each column

   // Precompute vertical lines (same x positions for all clusters)
   std::vector<TLine> verticalLines;
   verticalLines.reserve(36); // j = 0 to 35
   for (int j = 4; j <= 13; ++j) {
      double x = 1.019 / 2 + j * 1.019;
      // Dummy y range, will be updated before drawing
      verticalLines.push_back(TLine(x, 0, x, 1));
      verticalLines.back().SetLineColor(kBlack);
      verticalLines.back().SetLineWidth(1);
      verticalLines.back().SetLineStyle(2); // Dashed line
   }

   //  Legend
   TLegend legy(0.73, 0.65, 0.95, 0.88);
   legy.SetTextSize(0.05);
   legy.SetFillStyle(1001);
   legy.SetTextColor(kBlue - 1);
   legy.SetBorderSize(1);
   legy.SetLineWidth(1);

   fpCanvas->Clear();
   gPad->SetGrid(0, 0);
   gPad->SetRightMargin(0.03);
   gPad->SetTopMargin(0.1);
   gStyle->SetOptStat(0);
   for (int i = 1; i < 35; ++i) {
      float maxY =
         1.1 * std::max(v_yCluster[i]->GetMaximum(), v_yWeight[i]->GetMaximum());
      v_yWeight[i]->SetAxisRange(0, maxY, "Y");
      Graphic_setup(v_yWeight[i], 0.5, 1, kOrange - 3, 3, kOrange - 3, kOrange - 3, 0.2);
      v_yWeight[i]->Draw("HIST");
      Graphic_setup(v_yCluster[i], 0.5, 1, kBlue + 2, 3, kBlue + 2, kBlue + 2, 0.2);
      v_yCluster[i]->Draw("HIST SAME");

      gPad->Update();
      // Draw vertical lines with updated y-range
      for (auto &line : verticalLines) {
         line.SetY1(0);
         line.SetY2(gPad->GetUymax());
         line.Draw("same");
      }

      if (i == 1) {
         legy.AddEntry(v_yCluster[i], "PRF", "l");
         legy.AddEntry(v_yWeight[i], "CoA", "l");
         legy.AddEntry(&verticalLines[0], "Pad centers", "l");
      }
      legy.Draw();
      fpCanvas->SaveAs(drawout_file.c_str());
   }

   //////////////////////////////////////////////////////////////////////////////////////////
   // Y position in clusters
   fpCanvas->Clear();
   float maxY = 1.1 * std::max(ph1f_yCluster->GetMaximum(), ph1f_yWeight->GetMaximum());
   Graphic_setup(ph1f_yWeight, 0.5, 1, kOrange - 3, 3, kOrange - 3, kOrange - 3, 0.2);
   ph1f_yWeight->Draw("HIST");
   Graphic_setup(ph1f_yCluster, 0.5, 1, kBlue + 2, 3, kBlue + 2, kBlue + 2, 0.2);
   ph1f_yCluster->Draw("HIST SAME");

   gPad->Update();
   for (auto &line : verticalLines) {
      line.SetY1(0);
      line.SetY2(gPad->GetUymax());
      line.Draw("same");
   }
   legy.Draw();
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   // Delta y
   std::vector<TLine> dyClusLines;
   dyClusLines.reserve(36); // j = 0 to 35
   for (int j = 0; j <= 4; ++j) {
      double xminus = -j * 1.019;
      double xplus = j * 1.019;
      // Dummy y range, will be updated before drawing
      dyClusLines.push_back(TLine(xminus, 0, xminus, 1));
      dyClusLines.push_back(TLine(xplus, 0, xplus, 1));
   }
   for (auto &line : dyClusLines) {
      line.SetLineColor(kBlack);
      line.SetLineWidth(1);
      line.SetLineStyle(2);
   }
   fpCanvas->Clear();
   for (int i = 1; i < 35; ++i) {
      float maxY = 1.1 * std::max(v_dyClus[i]->GetMaximum(), v_dyWeight[i]->GetMaximum());
      Graphic_setup(v_dyWeight[i], 0.5, 1, kOrange - 3, 3, kOrange - 3, kOrange - 3, 0.2);
      v_dyWeight[i]->GetYaxis()->SetTitleOffset(1.1);
      v_dyWeight[i]->SetAxisRange(0, maxY, "Y");
      v_dyWeight[i]->Draw("HIST");
      Graphic_setup(v_dyClus[i], 0.5, 1, kBlue + 2, 3, kBlue + 2, kBlue + 2, 0.2);
      v_dyClus[i]->GetYaxis()->SetTitleOffset(1.1);
      v_dyClus[i]->Draw("HIST SAME");
      gPad->Update();
      // Draw vertical lines with updated y-range
      for (auto &line : dyClusLines) {
         line.SetY1(0);
         line.SetY2(gPad->GetUymax());
         line.Draw("same");
      }
      legy.Draw();
      fpCanvas->SaveAs(drawout_file.c_str());
   }
   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->Clear();
   maxY = 1.1 * std::max(ph1f_dyClus->GetMaximum(), ph1f_dyWeight->GetMaximum());
   Graphic_setup(ph1f_dyWeight, 0.5, 1, kOrange - 3, 3, kOrange - 3, kOrange - 3, 0.2);
   ph1f_dyWeight->SetAxisRange(0, maxY, "Y");
   ph1f_dyWeight->GetYaxis()->SetTitleOffset(1.1);
   ph1f_dyWeight->Draw("HIST");
   Graphic_setup(ph1f_dyClus, 0.5, 1, kBlue + 2, 3, kBlue + 2, kBlue + 2, 0.2);
   ph1f_dyClus->GetYaxis()->SetTitleOffset(1.1);
   ph1f_dyClus->Draw("HIST SAME");
   gPad->Update();
   // Draw vertical lines with updated y-range
   for (auto &line : dyClusLines) {
      line.SetY1(0);
      line.SetY2(gPad->GetUymax());
      line.Draw("same");
   }
   legy.Draw();
   fpCanvas->SaveAs((drawout_file + ")").c_str());
}

void Reconstruction::DrawOuts::EnergyLoss(const int &methods)
{
   // Graphic style
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);
   gPad->SetMargin(0.14, 0.05, 0.15, 0.02);
   gStyle->SetTitleSize(0.07, "xyz");
   gStyle->SetLabelSize(0.07, "xyz");

   // Define which algorithms must be drawn
   fnMethods = 2; // Draw both by default
   fwhichMethods = methods;
   if (fwhichMethods == 1 or fwhichMethods == 2) // Only draw WF or XP
      fnMethods = 1;

   // Shift x axis for theta scan
   float xDefaultNDC = 0.68;
   float xmax = 1600;
   if (tag.find("Theta") != std::string::npos)
      xmax = 2000;

   // Prepare histograms
   TH1F *ph1f_GP1 = new TH1F("ph1f_GP1", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GP2 = new TH1F("ph1f_GP2", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GP3 = new TH1F("ph1f_GP3", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GP4 = new TH1F("ph1f_GP4", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GP5 = new TH1F("ph1f_GP5", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GPC = new TH1F("ph1f_GPC", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_GPL = new TH1F("ph1f_GPL", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_rhoC =
      new TH1F("ph1f_rhoC", ";LUT ratio for crossed pads;Counts", 100, 0, 2);
   TH1F *ph1f_rhoL =
      new TH1F("ph1f_rhoL", ";LUT ratio for leading pad;Counts", 100, 0, 2);
   TH1F *ph1f_TmaxCrossed =
      new TH1F("ph1f_TmaxCrossed", ";T_{max} of crossed pads;Counts", 80, 20, 100);
   TH1F *ph1f_TmaxLeading =
      new TH1F("ph1f_TmaxLeading", ";T_{max} of leading pad;Counts", 80, 20, 100);

   TH1F *ph1f_XP = new TH1F("ph1f_XP", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_WF = new TH1F("ph1f_WF", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_XPnoTrunc =
      new TH1F("ph1f_XPnoTrunc", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH1F *ph1f_WFnoTrunc =
      new TH1F("ph1f_WFnoTrunc", ";dE/dx [ADC counts/cm];Counts", 100, 0, xmax);
   TH2F *ph2f_XPWF =
      new TH2F("ph2f_XPWF", ";dE/dx (WF);dE/dx (XP)", 100, 0, xmax, 100, 0, xmax);
   std::vector<TH1F *> v_h1f_XP_mod;
   std::vector<TH1F *> v_h1f_WF_mod;
   std::vector<TH1F *> v_h1f_XP_modnoTrunc;
   std::vector<TH1F *> v_h1f_WF_modnoTrunc;

   // Prepare vector of dEdx histograms, one for each module
   for (int i = 0; i < 8; i++) {
      v_h1f_XP_mod.push_back(new TH1F(Form("ph1f_XP_%i", i),
                                      Form("Module %i;dE/dx [ADC counts/cm];Counts", i),
                                      100, 0, xmax));
      v_h1f_WF_mod.push_back(new TH1F(Form("ph1f_WF_%i", i),
                                      Form("Module %i;dE/dx [ADC counts/cm];Counts", i),
                                      100, 0, xmax));
      v_h1f_XP_modnoTrunc.push_back(
         new TH1F(Form("ph1f_XPnoTrunc_%i", i),
                  Form("Module %i (no truncation);dE/dx [ADC counts/cm];Counts", i), 100,
                  0, xmax));
      v_h1f_WF_modnoTrunc.push_back(
         new TH1F(Form("ph1f_WFnoTrunc_%i", i),
                  Form("Module %i (no truncation);dE/dx [ADC counts/cm];Counts", i), 100,
                  0, xmax));
   }

   // Prepare vector of giga waveforms
   std::vector<TH1F *> v_ph1f_GWF;
   std::vector<TH1F *> v_ph1f_GWFtruncGP1;

   // Get entries and fill histograms
   std::vector<int> v_ModulesOfEvent;
   for (int i = 0; i < v_fnentries.back(); i++) {
      v_fTrees.back()->GetEntry(i);
      p_recoevent = v_fEvents.back();
      NMod = p_recoevent->v_modules.size();
      if (!p_recoevent->selected)
         continue;

      for (int iMod = 0; iMod < NMod; iMod++) {
         p_recomodule = p_recoevent->v_modules[iMod];
         if (!p_recomodule->selected)
            continue;
         v_ModulesOfEvent.push_back(p_recomodule->position);
         position = p_recomodule->position;
         v_h1f_XP_mod[position]->Fill(p_recomodule->dEdxXP);
         v_h1f_WF_mod[position]->Fill(p_recomodule->dEdxWF);
         v_h1f_XP_modnoTrunc[position]->Fill(p_recomodule->dEdxXPnoTrunc);
         v_h1f_WF_modnoTrunc[position]->Fill(p_recomodule->dEdxWFnoTrunc);

         NClusters = p_recomodule->NClusters;
         for (int iC = 0; iC < NClusters; iC++) {
            p_recocluster = p_recomodule->v_clusters[iC];
            ph1f_rhoL->Fill(p_recocluster->LUTrhoLead);
            ph1f_TmaxLeading->Fill(p_recocluster->TLead);
            NPads = p_recocluster->NPads;
            for (int iP = 0; iP < NPads; iP++) {
               p_recopad = p_recocluster->v_pads[iP];
               if (p_recopad->ratioDrift != 0) {
                  ph1f_rhoC->Fill(p_recopad->ratioDrift);
                  ph1f_TmaxCrossed->Fill(p_recopad->TMax);
               }
            }
         }
      }
      if (testbeam.find("CERN") != std::string::npos) {
         if (NMod < 4)
            continue;
         if (!FourModulesInLine(v_ModulesOfEvent))
            continue;
      }
      ph1f_WF->Fill(p_recoevent->dEdxWF);
      ph1f_XP->Fill(p_recoevent->dEdxXP);
      ph1f_GP1->Fill(p_recoevent->dEdxGP1);
      ph1f_GP2->Fill(p_recoevent->dEdxGP2);
      ph1f_GP3->Fill(p_recoevent->dEdxGP3);
      ph1f_GP4->Fill(p_recoevent->dEdxGP4);
      ph1f_GP5->Fill(p_recoevent->dEdxGP5);
      ph1f_GPC->Fill(p_recoevent->dEdxGPC);
      ph1f_GPL->Fill(p_recoevent->dEdxGPL);
      ph1f_WFnoTrunc->Fill(p_recoevent->dEdxWFnoTrunc);
      ph1f_XPnoTrunc->Fill(p_recoevent->dEdxXPnoTrunc);
      ph2f_XPWF->Fill(p_recoevent->dEdxWF, p_recoevent->dEdxXP);

      // Giga waveforms
      if (i < 100) {
         TH1F *ph1f_GWF = new TH1F(
            Form("ph1f_GWF_%i", i),
            Form(";time bin [%d ns];ADC counts", p_recoevent->timeBinSize), 510, 0, 510);
         TH1F *ph1f_GWFtruncatedGP1 = new TH1F(Form("ph1f_GWFtruncGP1_%i", i),
                                               Form(";time bin [%d ns];ADC "
                                                    "counts",
                                                    p_recoevent->timeBinSize),
                                               510, 0, 510);
         TH1F *ph1F_GWFclone =
            dynamic_cast<TH1F *>(p_recoevent->GWF->Clone(Form("ph1f_GWF_%i", i)));
         TH1F *ph1F_GWFtruncGP1clone = dynamic_cast<TH1F *>(
            p_recoevent->GWFtruncatedGP1->Clone(Form("ph1f_GWFtruncGP1_%i", i)));
         for (int j = 1; j <= ph1F_GWFclone->GetNbinsX(); j++) {
            ph1f_GWF->SetBinContent(j, ph1F_GWFclone->GetBinContent(j));
            ph1f_GWFtruncatedGP1->SetBinContent(j,
                                                ph1F_GWFtruncGP1clone->GetBinContent(j));
         }
         v_ph1f_GWF.push_back(ph1f_GWF);
         v_ph1f_GWFtruncGP1.push_back(ph1f_GWFtruncatedGP1);
         delete ph1F_GWFclone;
         delete ph1F_GWFtruncGP1clone;
      }
   }

   // GP3 parametrisation plots
   TH2F *pth2fGP3param_tmp = dynamic_cast<TH2F *>(v_fFiles.back()->Get("pth2f_paramGP3"));
   TH2F *pth2fGP3param =
      new TH2F("pth2f_paramGP3", ";y_{track}-y_{leading};A_{cluster}/A_{leading}", 100,
               -1, 1, 100, 1, 2.5);
   for (int j = 1; j <= pth2fGP3param_tmp->GetNbinsX(); j++) {
      for (int k = 1; k <= pth2fGP3param_tmp->GetNbinsY(); k++) {
         pth2fGP3param->SetBinContent(j, k, pth2fGP3param_tmp->GetBinContent(j, k));
      }
   }
   TF1 *ptf1GP3param = dynamic_cast<TF1 *>(v_fFiles.back()->Get("ptf1_paramGP3"));

   // Fitting
   Fit1Gauss(ph1f_WF, 2);
   Fit1Gauss(ph1f_XP, 2);
   Fit1Gauss(ph1f_GP1, 2);
   Fit1Gauss(ph1f_GP2, 2);
   Fit1Gauss(ph1f_GP3, 2);
   Fit1Gauss(ph1f_GP4, 2);
   Fit1Gauss(ph1f_GP5, 2);
   Fit1Gauss(ph1f_GPC, 2);
   Fit1Gauss(ph1f_GPL, 2);
   Fit1Gauss(ph1f_WFnoTrunc, 2);
   Fit1Gauss(ph1f_XPnoTrunc, 2);

   // Display settings
   Graphic_setup(ph1f_WF, 0.5, 1, kCyan + 1, 2, kCyan - 2, kCyan, 0.2);
   Graphic_setup(ph1f_XP, 0.5, 1, kMagenta + 2, 2, kMagenta - 2, kMagenta, 0.2);
   Graphic_setup(ph1f_GP1, 0.5, 1, kOrange + 7, 2, kOrange + 2, kOrange, 0.2);
   Graphic_setup(ph1f_GP2, 0.5, 1, kOrange + 7, 2, kOrange + 2, kOrange, 0.2);
   Graphic_setup(ph1f_GP3, 0.5, 1, kOrange + 7, 2, kOrange + 2, kOrange, 0.2);
   Graphic_setup(ph1f_GP4, 0.5, 1, kOrange + 7, 2, kOrange + 2, kOrange, 0.2);
   Graphic_setup(ph1f_GP5, 0.5, 1, kOrange + 7, 2, kOrange + 2, kOrange, 0.2);
   Graphic_setup(ph1f_GPC, 0.5, 1, kYellow + 7, 2, kYellow + 2, kYellow, 0.2);
   Graphic_setup(ph1f_GPL, 0.5, 1, kYellow + 7, 2, kYellow + 2, kYellow, 0.2);
   Graphic_setup(ph1f_WFnoTrunc, 0.5, 1, kBlue + 1, 2, kBlue - 2, kBlue, 0.2);
   Graphic_setup(ph1f_XPnoTrunc, 0.5, 1, kRed + 2, 2, kRed - 2, kRed, 0.2);
   Graphic_setup(ph1f_rhoC, 0.5, 1, kBlue + 2, 2, kBlue - 2, kBlue, 0.2);
   Graphic_setup(ph1f_rhoL, 0.5, 1, kRed + 2, 2, kRed - 2, kRed, 0.2);

   for (int i = 0; i < 8; i++) {
      Graphic_setup(v_h1f_WF_mod[i], 0.5, 1, kCyan + 1, 1, kCyan - 2, kCyan, 0.2);
      Graphic_setup(v_h1f_XP_mod[i], 0.5, 1, kMagenta + 2, 1, kMagenta - 2, kMagenta,
                    0.2);
      Graphic_setup(v_h1f_WF_modnoTrunc[i], 0.5, 1, kBlue + 1, 1, kBlue - 2, kBlue, 0.2);
      Graphic_setup(v_h1f_XP_modnoTrunc[i], 0.5, 1, kRed + 2, 1, kRed - 2, kRed, 0.2);
      if (v_h1f_WF_mod[i]->GetEntries() < 100)
         continue;
      Fit1Gauss(v_h1f_WF_mod[i], 2);
      Fit1Gauss(v_h1f_XP_mod[i], 2);
      Fit1Gauss(v_h1f_WF_modnoTrunc[i], 2);
      Fit1Gauss(v_h1f_XP_modnoTrunc[i], 2);
   }

   // Get maximum of event plots
   int WFMax = ph1f_WF->GetMaximum();
   int XPMax = ph1f_XP->GetMaximum();
   int GP1Max = ph1f_GP1->GetMaximum();
   int GP2Max = ph1f_GP2->GetMaximum();
   int GP3Max = ph1f_GP3->GetMaximum();
   int GP4Max = ph1f_GP4->GetMaximum();
   int GP5Max = ph1f_GP5->GetMaximum();

   // Get maximum of module plots
   int overallMax = 0, overallMaxMod = 0, maxVal_WF = 0, maxVal_XP = 0;
   int maxVal_GP1 = ph1f_GP1->GetMaximum();
   int maxVal_GP2 = ph1f_GP2->GetMaximum();
   int maxVal_GP3 = ph1f_GP3->GetMaximum();
   int maxVal_GP4 = ph1f_GP4->GetMaximum();
   int maxVal_GP5 = ph1f_GP5->GetMaximum();
   overallMax = std::max({WFMax, XPMax, GP1Max, GP2Max, GP3Max, GP4Max, GP5Max});
   for (int i = 0; i < 8; i++) {
      maxVal_WF = v_h1f_WF_mod[i]->GetMaximum();
      maxVal_XP = v_h1f_XP_mod[i]->GetMaximum();
      overallMaxMod = std::max({overallMaxMod, maxVal_WF, maxVal_XP});
   }

   // Invert X position of PrintResolution object if distribution is to the right
   float invX, invY;
   ph1f_WF->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_XP->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_GP4->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_GP5->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_GPC->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_GPL->SetAxisRange(0, 1.1 * overallMax, "Y");
   invX = 0, invY = 0;
   if (ph1f_WF->GetMean() > xmax / 2)
      invX = 0.5;

   // Force divisions on X axis
   ph1f_WF->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_XP->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GP1->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GP2->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GP3->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GP4->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GP5->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GPC->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_GPL->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_WFnoTrunc->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_XPnoTrunc->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_rhoC->GetXaxis()->SetNdivisions(404, kFALSE);
   ph1f_rhoL->GetXaxis()->SetNdivisions(404, kFALSE);

   // Drawing
   fpCanvas->cd();

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw both methods
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_XP->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_XP, fpCanvas, xDefaultNDC - invX, 0.65, kMagenta + 2, "XP");
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GP1
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GP1->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GP1, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP1");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GP2
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GP2->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GP2, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP2");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GP3
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GP3->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GP3, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP3");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GP4
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GP4->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GP4, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP4");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GPL
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GPL->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GPL, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GPL");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw GP4 & GPL
   fpCanvas->Clear();
   ph1f_GP4->Draw("HIST");
   ph1f_GPL->Draw("HIST sames");
   PrintResolution(ph1f_GP4, fpCanvas, xDefaultNDC - invX, 0.3, kOrange + 2, "GP4");
   PrintResolution(ph1f_GPL, fpCanvas, xDefaultNDC - invX, 0.65, kYellow + 2, "GPL");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GP5
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GP5->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GP5, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP5");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF & GPC
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_GPC->Draw("HIST sames");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2, "WF");
   PrintResolution(ph1f_GPC, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GPC");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw GP5 & GPC
   fpCanvas->Clear();
   ph1f_GP5->Draw("HIST");
   ph1f_GPC->Draw("HIST sames");
   PrintResolution(ph1f_GP5, fpCanvas, xDefaultNDC - invX, 0.3, kOrange + 2, "GP5");
   PrintResolution(ph1f_GPC, fpCanvas, xDefaultNDC - invX, 0.65, kYellow + 2, "GPC");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw XP & GP5
   fpCanvas->Clear();
   ph1f_XP->Draw("HIST");
   ph1f_GP5->Draw("HIST sames");
   PrintResolution(ph1f_XP, fpCanvas, xDefaultNDC - invX, 0.3, kMagenta + 2, "XP");
   PrintResolution(ph1f_GP5, fpCanvas, xDefaultNDC - invX, 0.65, kOrange + 2, "GP5");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw rhoC & rhoL
   fpCanvas->Clear();
   TLegend leg_rho(0.6, 0.7, 0.9, 0.9);
   Graphic_setup(&leg_rho, 0.06, kBlue - 1, 1001, kWhite, 1, 1);
   leg_rho.AddEntry(ph1f_rhoC, "Crossed pads", "l");
   leg_rho.AddEntry(ph1f_rhoL, "Leading pads", "l");
   ph1f_rhoC->GetXaxis()->SetTitle("LUT ratio");
   ph1f_rhoC->Draw("HIST");
   ph1f_rhoL->Draw("HIST sames");
   leg_rho.Draw();
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw Tmax of crossed and leading pads
   fpCanvas->Clear();
   TLegend leg_Tmax(0.6, 0.7, 0.9, 0.9);
   Graphic_setup(&leg_Tmax, 0.06, kBlue - 1, 1001, kWhite, 1, 1);
   leg_Tmax.AddEntry(ph1f_TmaxCrossed, "Crossed pads", "l");
   leg_Tmax.AddEntry(ph1f_TmaxLeading, "Leading pads", "l");
   Graphic_setup(ph1f_TmaxCrossed, 0.5, 1, kBlue + 2, 2, kBlue - 2, kBlue, 0.2);
   Graphic_setup(ph1f_TmaxLeading, 0.5, 1, kRed + 2, 2, kRed - 2, kRed, 0.2);
   ph1f_TmaxCrossed->Draw("HIST");
   ph1f_TmaxLeading->Draw("HIST sames");
   leg_Tmax.Draw();
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF alone
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.65, kCyan + 2, "WF");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw XP alone
   fpCanvas->Clear();
   ph1f_XP->Draw("HIST");
   PrintResolution(ph1f_XP, fpCanvas, xDefaultNDC - invX, 0.65, kMagenta + 2, "XP");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw both methods for each module
   fpCanvas->Clear();
   fpCanvas->Divide(4, 2);
   for (int i = 0; i < 8; i++) {
      v_h1f_WF_mod[i]->SetAxisRange(0, 1.1 * overallMaxMod, "Y");
      fpCanvas->cd(i + 1);
      invX = 0;
      if (v_h1f_WF_mod[i]->GetMean() > xmax / 2)
         invX = 0.5;
      gPad->SetRightMargin(0);
      v_h1f_WF_mod[i]->Draw("HIST");
      v_h1f_XP_mod[i]->Draw("HIST sames");
      if (v_h1f_WF_mod[i]->GetEntries() < 100)
         continue;
      PrintResolution(v_h1f_WF_mod[i], fpCanvas, xDefaultNDC - invX, 0.3, kCyan + 2,
                      "WF");
      PrintResolution(v_h1f_XP_mod[i], fpCanvas, xDefaultNDC - invX, 0.65, kMagenta + 2,
                      "XP");
   }
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Not truncated distributions
   ph1f_WFnoTrunc->SetAxisRange(0, 1.1 * overallMax, "Y");
   ph1f_XPnoTrunc->SetAxisRange(0, 1.1 * overallMax, "Y");
   invX = 0, invY = 0;
   if (ph1f_WFnoTrunc->GetMean(1) > xmax / 2)
      invX = 0.5;
   //////////////////////////////////////////////////////////////////////////////////////
   // Draw both non-truncated methods
   fpCanvas->Clear();
   ph1f_WFnoTrunc->Draw("HIST");
   ph1f_XPnoTrunc->Draw("HIST sames");
   PrintResolution(ph1f_WFnoTrunc, fpCanvas, xDefaultNDC - invX, 0.3, kBlue + 2,
                   "Not truncated WF");
   PrintResolution(ph1f_XPnoTrunc, fpCanvas, xDefaultNDC - invX, 0.65, kRed + 2,
                   "Not truncated XP");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw WF truncated and not truncated
   if (ph1f_WF->GetMean(1) > xmax / 2)
      invX = 0.5;
   else
      invX = 0;
   fpCanvas->Clear();
   ph1f_WF->Draw("HIST");
   ph1f_WFnoTrunc->Draw("HIST SAMES");
   PrintResolution(ph1f_WF, fpCanvas, xDefaultNDC - invX, 0.65, kCyan + 2,
                   "Truncated WF");
   PrintResolution(ph1f_WFnoTrunc, fpCanvas, xDefaultNDC - invX, 0.3, kBlue + 2,
                   "Not truncated WF");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw XP truncated and not truncated
   if (ph1f_XP->GetMean(1) > xmax / 2)
      invX = 0.5;
   else
      invX = 0;
   fpCanvas->Clear();
   ph1f_XP->Draw("HIST");
   ph1f_XPnoTrunc->Draw("HIST SAMES");
   PrintResolution(ph1f_XP, fpCanvas, xDefaultNDC - invX, 0.65, kMagenta + 2,
                   "Truncated XP");
   PrintResolution(ph1f_XPnoTrunc, fpCanvas, xDefaultNDC - invX, 0.3, kRed + 2,
                   "Not truncated XP");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////
   // Draw both non-truncated methods for each module
   fpCanvas->Clear();
   fpCanvas->Divide(4, 2);

   for (int i = 0; i < 8; i++) {
      v_h1f_WF_modnoTrunc[i]->SetAxisRange(0, 1.1 * overallMaxMod, "Y");
      fpCanvas->cd(i + 1);
      invX = 0;
      if (v_h1f_WF_modnoTrunc[i]->GetMean() > xmax / 2)
         invX = 0.5;
      gPad->SetRightMargin(0);
      v_h1f_WF_modnoTrunc[i]->Draw("HIST");
      v_h1f_XP_modnoTrunc[i]->Draw("HIST sames");
      if (v_h1f_WF_mod[i]->GetEntries() < 100)
         continue;
      PrintResolution(v_h1f_XP_modnoTrunc[i], fpCanvas, xDefaultNDC - invX, 0.65,
                      kRed + 2, "XP");
      PrintResolution(v_h1f_WF_modnoTrunc[i], fpCanvas, xDefaultNDC - invX, 0.3,
                      kBlue + 2, "WF");
   }
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   // Draw XP vs WF
   fpCanvas->cd();
   fpCanvas->Clear();
   fpCanvas->SetGrid(0, 0);
   ph2f_XPWF->GetXaxis()->SetNdivisions(505, kTRUE);
   ph2f_XPWF->GetYaxis()->SetNdivisions(505, kTRUE);
   ph2f_XPWF->GetZaxis()->SetNdivisions(505, kTRUE);
   invX, invY = 0;
   if (ph2f_XPWF->GetMean(1) > xmax / 2)
      invX = 0.5;
   if (ph2f_XPWF->GetMean(2) > xmax / 2)
      invY = 0.5;
   gStyle->SetOptStat("merou");
   gStyle->SetStatX(0.36);
   gStyle->SetStatY(0.88);
   gPad->SetRightMargin(0.14);
   gPad->SetLeftMargin(0.16);
   ph2f_XPWF->GetYaxis()->SetTitleOffset(1.2);
   ph2f_XPWF->Draw("colz");
   fpCanvas->SaveAs(drawout_file.c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   // Draw parametrisation of GP3
   fpCanvas->cd();
   fpCanvas->Clear();
   fpCanvas->SetGrid(1, 1);
   gStyle->SetOptStat(0);
   pth2fGP3param->GetXaxis()->SetNdivisions(505, kTRUE);
   pth2fGP3param->GetYaxis()->SetNdivisions(505, kTRUE);
   pth2fGP3param->GetZaxis()->SetNdivisions(505, kTRUE);
   pth2fGP3param->Draw("colz");
   ptf1GP3param->SetLineColor(kRed);
   ptf1GP3param->SetLineWidth(2);
   ptf1GP3param->Draw("sames");
   fpCanvas->SaveAs((drawout_file + ")").c_str());

   //////////////////////////////////////////////////////////////////////////////////
   bool drawGigaWF = false;
   if (drawGigaWF) {
      // Draw Giga waveforms
      std::vector<TH1F *> v_ph1f_ETF;
      std::vector<TH1F *> v_ph1f_ETFtruncated;
      std::string drawoutFileGWF = drawout_file.substr(0, drawout_file.size() - 4);
      fpCanvas->Clear();
      gPad->SetGrid(1, 1);
      gPad->SetTopMargin(0.07);
      gPad->SetRightMargin(0.04);
      gPad->SetLeftMargin(0.12);
      gStyle->SetTitleOffset(0.8, "y");
      gStyle->SetOptStat(0);
      TLegend legGWF(0.6, 0.55, 0.92, 0.9);
      Graphic_setup(&legGWF, 0.05, kBlue - 1, 1001, kWhite, 1, 1);
      for (int i = 0; i < (int)v_ph1f_GWF.size(); i++) {
         Graphic_setup(v_ph1f_GWF[i], 1, 1, kRed + 2, 2, kRed + 2, kRed, 0.2);
         Graphic_setup(v_ph1f_GWFtruncGP1[i], 1, 1, kBlue + 2, 2, kBlue + 2, kBlue, 0.2);
         // ETF
         int maxBin = v_ph1f_GWF[i]->GetMaximumBin();
         int maxGWF = v_ph1f_GWF[i]->GetMaximum();
         TH1F *pTH1F_ETF = ETF("pTH1F_ETF_" + std::to_string(i), 0, 510, maxBin - PT / TB,
                               510, 999, PT, TB);
         pTH1F_ETF->Scale(maxGWF);
         Graphic_setup(pTH1F_ETF, 1, 1, kRed + 3, 2, kRed + 3, kRed, 0.2);
         v_ph1f_ETF.push_back(pTH1F_ETF);
         /// Truncated ETF
         int maxBinTrunc = v_ph1f_GWFtruncGP1[i]->GetMaximumBin();
         int maxGWFtrunc = v_ph1f_GWFtruncGP1[i]->GetMaximum();
         TH1F *pTH1F_ETFtrunc = ETF("pTH1F_ETFtrunc_" + std::to_string(i), 0, 510,
                                    maxBinTrunc - PT / TB, 510, 999, PT, TB);
         pTH1F_ETFtrunc->Scale(maxGWFtrunc);
         Graphic_setup(pTH1F_ETFtrunc, 1, 1, kBlue + 3, 2, kBlue + 3, kBlue, 0.2);
         v_ph1f_ETFtruncated.push_back(pTH1F_ETFtrunc);
         // Set X range
         float maxX = v_ph1f_GWF[i]->GetXaxis()->GetBinCenter(maxBin);
         v_ph1f_GWF[i]->GetXaxis()->SetRangeUser(maxX - 20, maxX + 120);
         // Set Y range
         float ymin =
            std::min(v_ph1f_GWF[i]->GetMinimum(), v_ph1f_GWFtruncGP1[i]->GetMinimum());
         float ymax =
            std::max(v_ph1f_GWF[i]->GetMaximum(), v_ph1f_GWFtruncGP1[i]->GetMaximum());
         float margin = (ymax - ymin) * 0.1; // 10% margin
         v_ph1f_GWF[i]->GetYaxis()->SetRangeUser(ymin - margin, ymax + margin);
         v_ph1f_GWF[i]->GetYaxis()->SetTitleOffset(0.8);
      }
      legGWF.AddEntry(v_ph1f_GWF[0], "GWF", "f");
      legGWF.AddEntry(v_ph1f_GWFtruncGP1[0], "Truncated GWF", "f");
      legGWF.AddEntry(v_ph1f_ETF[0], "ETF", "l");
      legGWF.AddEntry(v_ph1f_ETFtruncated[0], "Truncated ETF", "l");
      for (int i = 0; i < (int)v_ph1f_GWF.size(); i++) {
         fpCanvas->Clear();
         v_ph1f_GWF[i]->Draw("HIST");
         v_ph1f_ETF[i]->Draw("HIST sames");
         v_ph1f_GWFtruncGP1[i]->Draw("HIST sames");
         v_ph1f_ETFtruncated[i]->Draw("HIST sames");
         legGWF.Draw();
         if (i == 0)
            fpCanvas->SaveAs((drawoutFileGWF + "_GWF.pdf(").c_str());
         else if (i == (int)v_ph1f_GWF.size() - 1)
            fpCanvas->SaveAs((drawoutFileGWF + "_GWF.pdf)").c_str());
         else
            fpCanvas->SaveAs((drawoutFileGWF + "_GWF.pdf").c_str());
         delete v_ph1f_ETF[i];
         delete v_ph1f_ETFtruncated[i];
      }
      v_ph1f_ETF.clear();
      v_ph1f_ETFtruncated.clear();
   }

   // Delete
   delete ph1f_GP1;
   delete ph1f_GP2;
   delete ph1f_GP3;
   delete ph1f_GP4;
   delete ph1f_GP5;
   delete ph1f_XP;
   delete ph1f_WF;
   delete ph1f_XPnoTrunc;
   delete ph1f_WFnoTrunc;
   for (int i = 0; i < 8; i++) {
      delete v_h1f_XP_mod[i];
      delete v_h1f_WF_mod[i];
      delete v_h1f_XP_modnoTrunc[i];
      delete v_h1f_WF_modnoTrunc[i];
   }
   for (int i = 0; i < (int)v_ph1f_GWF.size(); i++) {
      delete v_ph1f_GWF[i];
      delete v_ph1f_GWFtruncGP1[i];
   }
   v_ph1f_GWF.clear();
   v_ph1f_GWFtruncGP1.clear();
   delete ph2f_XPWF;
}

void Reconstruction::DrawOuts::FileComparison()
{
   float xmax = 1600;
   // Set output
   std::string comment_list;
   for (int i = 0; i < (int)v_comments.size(); i++) {
      if (v_comments[i] != "")
         comment_list += v_comments[i] + "_";
      else
         comment_list += "Default_";
   }
   drawout_file = foutputDir + "Comparison_" + comment_list + ".pdf";

   // Prepare histograms
   std::vector<TH1F *> v_h1f_XP;
   std::vector<TH1F *> v_h1f_WF;
   std::vector<std::vector<TH1F *>> v_h1f_XP_mod;
   std::vector<std::vector<TH1F *>> v_h1f_WF_mod;
   for (int i = 0; i < (int)v_comments.size(); i++) {
      v_h1f_XP.push_back(new TH1F(
         Form("ph1f_XP_%i", i),
         Form("Energy loss (%s);dE/dx [ADC counts/cm];Counts", v_comments[i].c_str()),
         100, 0, 1300));
      v_h1f_WF.push_back(new TH1F(
         Form("ph1f_WF_%i", i),
         Form("Energy loss (%s);dE/dx [ADC counts/cm];Counts", v_comments[i].c_str()),
         100, 0, 1300));
      v_h1f_XP_mod.push_back(std::vector<TH1F *>());
      v_h1f_WF_mod.push_back(std::vector<TH1F *>());
      for (int j = 0; j < 8; j++) {
         v_h1f_XP_mod[i].push_back(
            new TH1F(Form("ph1f_XP_%i_%i", i, j),
                     Form("Energy loss (%s, module %i);dE/dx [ADC counts/cm];Counts",
                          v_comments[i].c_str(), j),
                     100, 0, 1300));
         v_h1f_WF_mod[i].push_back(
            new TH1F(Form("ph1f_WF_%i_%i", i, j),
                     Form("Energy loss (%s, module %i);dE/dx [ADC counts/cm];Counts",
                          v_comments[i].c_str(), j),
                     100, 0, 1300));
      }
   }

   int NMod = 0, position = 0;
   // Get entries and fill histograms
   for (int ifile = 0; ifile < (int)v_comments.size(); ifile++) {
      for (int i = 0; i < v_fnentries[ifile]; i++) {
         v_fTrees[ifile]->GetEntry(i);
         NMod = v_fEvents[ifile]->v_modules.size();

         v_h1f_WF[i]->Fill(v_fEvents[ifile]->dEdxWF);
         v_h1f_XP[i]->Fill(v_fEvents[ifile]->dEdxXP);
         for (int iMod = 0; iMod < NMod; iMod++) {
            position = v_fEvents[ifile]->v_modules[iMod]->position;
            v_h1f_WF_mod[i][position]->Fill(v_fEvents[ifile]->v_modules[iMod]->dEdxWF);
            v_h1f_XP_mod[i][position]->Fill(v_fEvents[ifile]->v_modules[iMod]->dEdxXP);
         }
      }
   }

   for (int i = 0; i < (int)v_comments.size(); i++) {
      Fit1Gauss(v_h1f_WF[i], 2);
      Fit1Gauss(v_h1f_XP[i], 2);
      Graphic_setup(v_h1f_WF[i], 0.5, 1, 2 + i, 2, 2 + i, 2 + i, 0.2);
      Graphic_setup(v_h1f_XP[i], 0.5, 1, 2 + i, 2, 2 + i, 2 + i, 0.2);
      for (int j = 0; j < 8; j++) {
         Fit1Gauss(v_h1f_WF_mod[i][j], 2);
         Fit1Gauss(v_h1f_XP_mod[i][j], 2);
         Graphic_setup(v_h1f_WF_mod[i][j], 0.5, 1, 2 + i, 1, 2 + i, 2 + i, 0.2);
         Graphic_setup(v_h1f_XP_mod[i][j], 0.5, 1, 2 + i, 1, 2 + i, 2 + i, 0.2);
      }
   }

   // Draw
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   // Get maximum of event plots
   int overallMax = 0, overallMaxMod = 0, maxVal_WF = 0, maxVal_XP = 0;
   for (int i = 0; i < (int)v_comments.size(); i++) {
      maxVal_WF = v_h1f_WF[i]->GetMaximum();
      maxVal_XP = v_h1f_XP[i]->GetMaximum();
      overallMax = std::max({overallMax, maxVal_WF, maxVal_XP});
      for (int j = 0; j < 8; j++) {
         maxVal_WF = v_h1f_WF_mod[i][j]->GetMaximum();
         maxVal_XP = v_h1f_XP_mod[i][j]->GetMaximum();
         overallMaxMod = std::max({overallMaxMod, maxVal_WF, maxVal_XP});
      }
   }

   float invX;
   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->cd();
   fpCanvas->Clear();

   v_h1f_WF[0]->SetAxisRange(0, 1.1 * overallMax, "Y");
   invX = 0;
   if (v_h1f_WF[0]->GetMean() > xmax / 2)
      invX = 0.5;
   v_h1f_WF[0]->Draw("HIST");
   for (int i = 0; i < (int)v_comments.size(); i++) {
      if (i != 0)
         v_h1f_WF[i]->Draw("HIST sames");
      PrintResolution(v_h1f_WF[i], fpCanvas, 0.65 - invX, 0.58 - 0.33, 2 + i,
                      v_comments[i].c_str());
   }
   fpCanvas->SaveAs((drawout_file + "(").c_str());

   //////////////////////////////////////////////////////////////////////////////////////////
   fpCanvas->cd();
   fpCanvas->Clear();

   v_h1f_XP[0]->SetAxisRange(0, 1.1 * overallMax, "Y");
   invX = 0;
   if (v_h1f_XP[0]->GetMean() > xmax / 2)
      invX = 0.5;
   v_h1f_XP[0]->Draw("HIST");
   for (int i = 0; i < (int)v_comments.size(); i++) {
      if (i != 0)
         v_h1f_XP[i]->Draw("HIST sames");
      PrintResolution(v_h1f_XP[i], fpCanvas, 0.65 - invX, 0.58 - 0.33, 2 + i,
                      v_comments[i].c_str());
   }
   fpCanvas->SaveAs((drawout_file + ")").c_str());

   // Delete
   for (int i = 0; i < (int)v_comments.size(); i++) {
      delete v_h1f_XP[i];
      delete v_h1f_WF[i];
      for (int j = 0; j < 8; j++) {
         delete v_h1f_XP_mod[i][j];
         delete v_h1f_WF_mod[i][j];
      }
   }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Scan calling functions
void Reconstruction::DrawOuts::DESY21SingleScan(const int &methods)
{
   drawMultiScans = 0;
   fwhichMethods = methods;
   DESY21ScanFill();
   DESY21ScanDraw();
}

void Reconstruction::DrawOuts::DESY21MultiScan(const int &methods)
{
   drawMultiScans = 1;
   fwhichMethods = methods;
   DESY21ScanFill();
   DESY21ScanDraw();
}

void Reconstruction::DrawOuts::CERN22Scan()
{
   CERN22ScanFill();
   CERN22ScanDraw();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Correction function drawers

// WFsum as a function of cluster length in TH2 and TGE and TF1
void Reconstruction::DrawOuts::AmplitudeVSLength()
{
   // Drawing settings
   gStyle->SetOptStat(0);
   std::string OutputFile = drawoutPath + "DESY21_Phi/AmplitudeVSLength.pdf";
   fpCanvas->Clear();
   fpCanvas->cd();
   fpCanvas->SetGrid(0, 0);
   Color_t colors[] = {kRed, kBlue, kGreen + 2};
   Int_t linestyles[] = {1, 7, 2};
   fpCanvas->SaveAs((OutputFile + "(").c_str());

   // Three angles: 30, 40, 45 degrees
   int phi_arr[] = {30, 40, 45};
   // Three drift distances: 50, 550, 950 mm
   int drift_arr[] = {50, 550, 950};

   // Default correction function
   TF1 *ptf1CorrVlada =
      new TF1("ptf1CorrVlada",
              "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0,
              17); // values provided by Vlada (2022/10/11)
   Graphic_setup(ptf1CorrVlada, 3, kGray + 1, 1);
   ptf1CorrVlada->SetTitle(";Track length in cluster [mm];Amplitude [ADC counts/cm]");

   // Legend for each drift distance
   TLegend leg(0.15, 0.55, 0.5, 0.9);
   leg.SetTextSize(0.05);
   leg.SetBorderSize(0);
   leg.SetFillColor(kWhite);
   leg.SetFillStyle(1001);
   leg.SetTextColor(kBlue - 1);

   // Legend for all correction functions
   TLegend legAll(0.15, 0.7, 0.65, 0.9);
   legAll.SetTextSize(0.05);
   legAll.SetBorderSize(0);
   // legAll.SetFillColor(kWhite);
   legAll.SetFillStyle(0);
   legAll.SetTextColor(kBlue - 1);
   legAll.SetNColumns(3);
   std::vector<TGraph *> vptglegAll;
   for (int i = 0; i < 3; i++) {
      TGraph *tmp = new TGraph();
      tmp->SetLineStyle(linestyles[i]);
      tmp->SetLineWidth(3);
      legAll.AddEntry(tmp, Form("%d mm", drift_arr[i]), "l");
      vptglegAll.push_back(tmp);
   }
   for (int i = 0; i < 3; i++) {
      TGraph *tmp = new TGraph();
      tmp->SetLineColor(colors[i]);
      tmp->SetLineWidth(3);
      legAll.AddEntry(tmp, Form("%d#circ", phi_arr[i]), "l");
      vptglegAll.push_back(tmp);
   }
   legAll.AddEntry(ptf1CorrVlada, "Default", "l");

   // Storage for all correction functions
   std::vector<TF1 *> v_ptf1CorrFunc;

   // Correction function to use as a reference
   TF1 *ptf1CorrFuncRef = TFile::Open(vcorrFuncPaths[1].c_str())->Get<TF1>("A_corr");
   float refval = ptf1CorrFuncRef->Eval(10.19); // reference value at 10.19 mm
   float Lmax = 16.5;

   // Storage of TGraphs made from TF1 / TF1ref
   std::vector<TGraph *> vptgCorrFunc;

   // Run
   for (int i = 0; i < 3; i++) {
      // Set indexes for the three distances
      int i30 = i * 3;
      int i40 = i * 3 + 1;
      int i45 = i * 3 + 2;

      // Print name of files
      std::cout << "i: " << i << " | i30: " << i30 << " | i40: " << i40
                << " | i45: " << i45 << std::endl;
      std::cout << "Opening files for " << drift_arr[i] << " mm drift:" << std::endl;
      std::cout << " - " << vcorrFuncPaths[i30] << std::endl;
      std::cout << " - " << vcorrFuncPaths[i40] << std::endl;
      std::cout << " - " << vcorrFuncPaths[i45] << std::endl;

      // Open files for each angle
      TFile *TFileCorrFunc30 = TFile::Open(vcorrFuncPaths[i30].c_str());
      TFile *TFileCorrFunc40 = TFile::Open(vcorrFuncPaths[i40].c_str());
      TFile *TFileCorrFunc45 = TFile::Open(vcorrFuncPaths[i45].c_str());

      // Get histograms
      TH2F *th2f50 = TFileCorrFunc30->Get<TH2F>("h2f_WFvsLength");
      TH2F *th2f550 = TFileCorrFunc40->Get<TH2F>("h2f_WFvsLength");
      TH2F *th2f950 = TFileCorrFunc45->Get<TH2F>("h2f_WFvsLength");

      // Rename histograms
      th2f50->SetTitle(Form("#varphi = %d#circ | %d mm drift;Track length in cluster "
                            "[mm]; Amplitude [ADC counts/cm]",
                            phi_arr[0], drift_arr[i]));
      th2f550->SetTitle(Form("#varphi = %d#circ | %d mm drift;Track length in cluster "
                             "[mm]; Amplitude [ADC counts/cm]",
                             phi_arr[1], drift_arr[i]));
      th2f950->SetTitle(Form("#varphi = %d#circ | %d mm drift;Track length in cluster "
                             "[mm]; Amplitude [ADC counts/cm]",
                             phi_arr[2], drift_arr[i]));

      // Get the TF1
      TF1 *ptf1CorrFunc30 = TFileCorrFunc30->Get<TF1>("A_corr");
      TF1 *ptf1CorrFunc40 = TFileCorrFunc40->Get<TF1>("A_corr");
      TF1 *ptf1CorrFunc45 = TFileCorrFunc45->Get<TF1>("A_corr");

      // Print all parameters of each TF1
      std::cout << "TF1 parameters for drift " << drift_arr[i] << " mm:" << std::endl;
      std::cout << " - #varphi = " << phi_arr[0] << " degrees: ";
      ptf1CorrFunc30->Print();
      std::cout << " - #varphi = " << phi_arr[1] << " degrees: ";
      ptf1CorrFunc40->Print();
      std::cout << " - #varphi = " << phi_arr[2] << " degrees: ";
      ptf1CorrFunc45->Print();
      std::cout << " - Default: ";
      ptf1CorrVlada->Print();
      std::cout << std::endl;

      // Set graphic style of TF1
      Graphic_setup(ptf1CorrFunc30, 3, kRed, 1);
      Graphic_setup(ptf1CorrFunc40, 3, kRed, 1);
      Graphic_setup(ptf1CorrFunc45, 3, kRed, 1);
      ptf1CorrVlada->SetLineStyle(1);

      // Draw the TH2
      gPad->SetRightMargin(0.13);
      gPad->SetTopMargin(0.08);
      th2f50->Draw("colz");
      ptf1CorrFunc30->Draw("same");
      ptf1CorrVlada->Draw("same");
      fpCanvas->SaveAs(OutputFile.c_str());
      th2f550->Draw("colz");
      ptf1CorrFunc40->Draw("same");
      ptf1CorrVlada->Draw("same");
      fpCanvas->SaveAs(OutputFile.c_str());
      th2f950->Draw("colz");
      ptf1CorrFunc45->Draw("same");
      ptf1CorrVlada->Draw("same");
      fpCanvas->SaveAs(OutputFile.c_str());

      // Change the colors, linestyles and titles of the TF1s
      ptf1CorrFunc30->SetLineColor(colors[0]);
      ptf1CorrFunc40->SetLineColor(colors[1]);
      ptf1CorrFunc45->SetLineColor(colors[2]);
      ptf1CorrFunc30->SetLineStyle(linestyles[i]);
      ptf1CorrFunc40->SetLineStyle(linestyles[i]);
      ptf1CorrFunc45->SetLineStyle(linestyles[i]);
      ptf1CorrVlada->SetLineStyle(3);
      ptf1CorrFunc30->SetTitle(";Track length in cluster [mm];Amplitude [ADC counts/cm]");
      ptf1CorrFunc40->SetTitle(";Track length in cluster [mm];Amplitude [ADC counts/cm]");
      ptf1CorrFunc45->SetTitle(";Track length in cluster [mm];Amplitude [ADC counts/cm]");

      // Draw the TF1s on the same canvas
      fpCanvas->Clear();
      gPad->SetRightMargin(0.03);
      gPad->SetTopMargin(0.03);
      ptf1CorrFunc30->GetXaxis()->SetRangeUser(0, Lmax);
      ptf1CorrFunc30->GetYaxis()->SetRangeUser(0, 1600);
      ptf1CorrFunc30->Draw();
      ptf1CorrFunc40->Draw("same");
      ptf1CorrFunc45->Draw("same");
      ptf1CorrVlada->Draw("same");

      // Store TF1s in vector
      v_ptf1CorrFunc.push_back(ptf1CorrFunc30);
      v_ptf1CorrFunc.push_back(ptf1CorrFunc40);
      v_ptf1CorrFunc.push_back(ptf1CorrFunc45);

      // Add legend
      leg.Clear();
      leg.SetHeader(Form("Drift: %d mm", drift_arr[i]));
      leg.AddEntry(ptf1CorrFunc30, Form("#varphi = %d#circ", phi_arr[0]), "l");
      leg.AddEntry(ptf1CorrFunc40, Form("#varphi = %d#circ", phi_arr[1]), "l");
      leg.AddEntry(ptf1CorrFunc45, Form("#varphi = %d#circ", phi_arr[2]), "l");
      leg.AddEntry(ptf1CorrVlada, "Default", "l");
      leg.Draw();
      fpCanvas->SaveAs(OutputFile.c_str());

      // legAll.AddEntry("drift", Form("%d mm", drift_arr[i]), "");
      // legAll.AddEntry(ptf1CorrFunc30, Form("%d#circ", phi_arr[0]), "l");
      // legAll.AddEntry(ptf1CorrFunc40, Form("%d#circ", phi_arr[1]), "l");
      // legAll.AddEntry(ptf1CorrFunc45, Form("%d#circ", phi_arr[2]), "l");

      // Create TGraph from TF1 and TF1ref
      int npoint = 200;
      std::vector<TF1 *> vptf1CorrFuncTmp = {ptf1CorrFunc30, ptf1CorrFunc40,
                                             ptf1CorrFunc45};
      for (TF1 *ptf1 : vptf1CorrFuncTmp) {
         TGraph *ptgCorrFunc = new TGraph();
         for (int j = 0; j < npoint; j++) {
            ptgCorrFunc->SetPoint(j, j / (float)npoint * Lmax,
                                  ptf1->Eval(j / (float)npoint * Lmax) * refval /
                                     ptf1CorrFuncRef->Eval(j / (float)npoint * Lmax));
         }
         ptgCorrFunc->SetLineColor(ptf1->GetLineColor());
         ptgCorrFunc->SetLineStyle(ptf1->GetLineStyle());
         ptgCorrFunc->SetLineWidth(ptf1->GetLineWidth());
         ptgCorrFunc->SetTitle(ptf1->GetTitle());
         ptgCorrFunc->GetXaxis()->SetTitle("Track length in cluster [mm]");
         ptgCorrFunc->GetYaxis()->SetTitle("Normalized amplitude [ADC counts/cm]");
         ptgCorrFunc->GetXaxis()->SetRangeUser(0, Lmax);
         ptgCorrFunc->GetYaxis()->SetRangeUser(0, 1600);
         vptgCorrFunc.push_back(ptgCorrFunc);
      }

      // Close the TFiles
      TFileCorrFunc30->Close();
      TFileCorrFunc40->Close();
      TFileCorrFunc45->Close();
   }

   // Draw all TF1s on the same canvas
   // legAll.AddEntry(ptf1CorrVlada, "Default", "l");
   fpCanvas->Clear();
   ptf1CorrVlada->GetXaxis()->SetRangeUser(0, Lmax);
   ptf1CorrVlada->GetYaxis()->SetRangeUser(0, 1600);
   ptf1CorrVlada->Draw();
   for (auto &ptf1 : v_ptf1CorrFunc) {
      ptf1->SetLineWidth(3);
      ptf1->Draw("same");
   }
   legAll.Draw();
   fpCanvas->SaveAs(OutputFile.c_str());

   // Draw TGraphs
   fpCanvas->Clear();
   for (int i = 0; i < (int)vptgCorrFunc.size(); i++) {
      vptgCorrFunc[i]->SetLineWidth(2);
      if (i == 0)
         vptgCorrFunc[i]->Draw("AL");
      else
         vptgCorrFunc[i]->Draw("L same");
   }
   // Remove "default" entry of the legend
   legAll.GetListOfPrimitives()->Remove(legAll.GetListOfPrimitives()->Last());
   legAll.SetY2NDC(0.4);
   legAll.SetY1NDC(0.2);
   legAll.Draw();
   fpCanvas->SaveAs(OutputFile.c_str());

   // Level arm of the correction function
   TLegend legRatio(0.5, 0.6, 0.95, 0.94);
   legRatio.SetTextSize(0.05);
   legRatio.SetTextColor(kBlue - 1);
   Color_t col[] = {kBlue - 9, kBlue - 7, kBlue, kBlue + 2, kBlue + 4};
   ptf1CorrFuncRef->SetParameter(0, ptf1CorrFuncRef->GetParameter(0) - 200);
   for (int i = 0; i < 5; i++) {
      TGraph *tg = new TGraph();
      tg->SetMaximum(4);
      tg->SetMinimum(0);
      tg->SetTitle(";Track length in cluster [mm];F_{ref}(10.19)/F_{ref}(L_{cluster})");
      ptf1CorrFuncRef->SetParameter(0, ptf1CorrFuncRef->GetParameter(0) + i * 100);
      refval = ptf1CorrFuncRef->Eval(10.19);
      for (int j = 0; j < 100; j++)
         tg->SetPoint(j, j / 100. * Lmax,
                      refval / ptf1CorrFuncRef->Eval(j / 100. * Lmax));
      tg->SetLineColor(col[i]);
      tg->SetLineWidth(4);
      tg->GetXaxis()->SetLimits(0, Lmax);
      if (i == 0)
         tg->DrawClone("AL");
      else
         tg->DrawClone("L same");
      legRatio.AddEntry(tg, Form("%i ADC counts/cm", i * 100 - 200), "l");
   }
   legRatio.Draw();
   fpCanvas->SaveAs((OutputFile + ")").c_str());
}

void DrawOut_corrections()
{

   gStyle->SetOptStat(0);

   // Get histograms
   std::vector<TF1 *> v_tf1;
   std::vector<std::string> v_filename;
   v_filename.push_back("../Data_DESY21/Phi_scan_zm40/"
                        "phi_200_30_zm40_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z460/"
                        "phi_200_30_z460_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z860/"
                        "phi_200_30_z860_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_zm40/"
                        "phi_200_40_zm40_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z460/"
                        "phi_200_40_z460_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z860/"
                        "phi_200_40_z860_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_zm40/"
                        "phi_200_45_zm40_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z460/"
                        "phi_200_45_z460_ym60_diag_iter0_WFmax_correction.root");
   v_filename.push_back("../Data_DESY21/Phi_scan_z860/"
                        "phi_200_45_z860_ym60_diag_iter0_WFmax_correction.root");

   TF1 *F_HATRecon = new TF1(
      "F_Vlada", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0,
      17); // values provided by Vlada (2022/10/11)
   v_tf1.push_back(F_HATRecon);
   for (int i = 0; i < (int)v_filename.size(); i++) {
      TFile *pfile = new TFile(v_filename[i].c_str(), "READ");
      v_tf1.push_back(pfile->Get<TF1>("A_corr"));
      pfile->Close();
   }

   // Draw out
   std::string OutputFile =
      "OUT_Reconstruction/DESY21_phi/Correction_functions_WFmax_correction.pdf";
   std::string OutputFile_Beg = OutputFile + "(";
   std::string OutputFile_End = OutputFile + ")";

   int z[] = {5, 55, 95};
   TCanvas *pTCanvas = new TCanvas("pTCanvas", "pTCanvas", 2700, 1800);
   TLegend *leg = new TLegend(0.85, 0.8, 0.99, 0.99);
   pTCanvas->cd();
   v_tf1[0]->SetLineColor(kGray);
   v_tf1[0]->SetLineWidth(4);
   v_tf1[0]->GetYaxis()->SetRangeUser(0, 1500);

   v_tf1[0]->SetTitle("Correction functions 30#circ;L_{cluster} (mm);WF (ADC count)");
   leg->AddEntry(v_tf1[0], "HATRecon", "l");
   for (int i = 1; i < 4; i++) {
      v_tf1[0]->Draw();
      v_tf1[i]->SetLineColor(kGreen + 2 * (i - 1));
      v_tf1[i]->SetLineWidth(4);
      v_tf1[i]->SetLineStyle((i - 1) % 3 + 1);
      leg->AddEntry(v_tf1[i], Form("%i cm", z[i - 1]), "l");
   }
   for (int i = 1; i < 4; i++)
      v_tf1[i]->Draw("same");
   leg->Draw();
   pTCanvas->SaveAs(OutputFile_Beg.c_str());
   leg->Clear();
   pTCanvas->Clear();

   v_tf1[0]->SetTitle("Correction functions 40#circ;L_{cluster} (mm);WF (ADC count)");
   leg->AddEntry(v_tf1[0], "HATRecon", "l");
   for (int i = 4; i < 7; i++) {
      v_tf1[0]->Draw();
      v_tf1[i]->SetLineColor(kBlue + 2 * (i - 4));
      v_tf1[i]->SetLineWidth(4);
      v_tf1[i]->SetLineStyle((i - 1) % 3 + 1);
      leg->AddEntry(v_tf1[i], Form("%i cm", z[i - 4]), "l");
   }
   for (int i = 4; i < 7; i++)
      v_tf1[i]->Draw("same");
   leg->Draw();
   pTCanvas->SaveAs(OutputFile.c_str());
   leg->Clear();
   pTCanvas->Clear();

   v_tf1[0]->SetTitle("Correction functions 45#circ;L_{cluster} (mm);WF (ADC count)");
   leg->AddEntry(v_tf1[0], "HATRecon", "l");
   for (int i = 7; i < 10; i++) {
      v_tf1[0]->Draw();
      v_tf1[i]->SetLineColor(kRed + 2 * (i - 7));
      v_tf1[i]->SetLineWidth(4);
      v_tf1[i]->SetLineStyle((i - 1) % 3 + 1);
      leg->AddEntry(v_tf1[i], Form("%i cm", z[i - 7]), "l");
   }
   for (int i = 7; i < 10; i++)
      v_tf1[i]->Draw("same");
   leg->Draw();
   pTCanvas->SaveAs(OutputFile.c_str());
   leg->Clear();
   pTCanvas->Clear();

   TLegend *legAll = new TLegend(0.86, 0.65, 0.99, 0.99);
   int angle[] = {30, 40, 45};
   v_tf1[1]->SetTitle("Correction functions;L_{cluster} (mm);WF (ADC count)");
   v_tf1[1]->GetYaxis()->SetRangeUser(0, 1500);
   v_tf1[1]->GetXaxis()->SetRangeUser(0, 17);
   // legAll->						AddEntry(v_tf1[0], "HATRecon" , "l");
   for (int i = 1; i < 10; i++) {
      v_tf1[1]->Draw();
      // if(i>1)v_tf1[i]->			Draw("same");
      legAll->AddEntry(v_tf1[i],
                       Form("%i#circ %i cm", angle[(i - 1) / 3], z[(i - 1) % 3]), "l");
   }
   for (int i = 1; i < 10; i++)
      v_tf1[i]->Draw("same");
   legAll->Draw();
   pTCanvas->SaveAs(OutputFile.c_str());
   pTCanvas->Clear();

   float ref = v_tf1[4]->Eval(11.28);
   for (int i = 1; i < 10; i++) {
      TGraph *tg = new TGraph();
      tg->SetMaximum(1500);
      tg->SetMinimum(0);
      tg->SetTitle("F(L_{cluster})*F_{ref}(11.28)/F_{ref}(L_{cluster});L_{cluster} "
                   "(mm);F(L_{cluster})*F_{ref}(11.28)/F_{ref}(L_{cluster}) (ADC count)");
      for (int j = 0; j < 100; j++)
         tg->SetPoint(j, j / 100. * 17,
                      v_tf1[i]->Eval(j / 100. * 17.) * ref /
                         v_tf1[4]->Eval(j / 100. * 17.));
      if (i >= 1 && i < 4)
         tg->SetLineColor(kGreen + 2 * (i - 1));
      if (i >= 4 && i < 7)
         tg->SetLineColor(kBlue + 2 * (i - 4));
      if (i >= 7 && i < 10)
         tg->SetLineColor(kRed + 2 * (i - 7));
      tg->SetLineWidth(4);
      tg->SetLineStyle((i - 1) % 3 + 1);
      tg->GetXaxis()->SetLimits(0, 17);
      if (i == 1)
         tg->DrawClone("AL");
      else
         tg->DrawClone("L same");
   }
   legAll->Draw();
   pTCanvas->SaveAs(OutputFile.c_str());
   pTCanvas->Clear();
   legAll->Clear();

   TLegend *legRatio = new TLegend(0.75, 0.6, 0.99, 0.94);
   Color_t col[] = {kRed - 9, kRed - 7, kRed, kRed + 2, kRed + 4};
   v_tf1[4]->SetParameter(0, v_tf1[4]->GetParameter(0) - 200);
   for (int i = 0; i < 5; i++) {
      TGraph *tg = new TGraph();
      tg->SetMaximum(4);
      tg->SetMinimum(0);
      tg->SetTitle("F_{ref}(11.28)/F_{ref}(L_{cluster}) with F_{ref} shifted;L_{cluster} "
                   "(mm);ratio");
      v_tf1[4]->SetParameter(0, v_tf1[4]->GetParameter(0) + i * 100);
      ref = v_tf1[4]->Eval(11.28);
      for (int j = 0; j < 100; j++)
         tg->SetPoint(j, j / 100. * 17., ref / v_tf1[4]->Eval(j / 100. * 17.));
      tg->SetLineColor(col[i]);
      tg->SetLineWidth(4);
      tg->GetXaxis()->SetLimits(0, 17);
      if (i == 0)
         tg->DrawClone("AL");
      else
         tg->DrawClone("L same");
      legRatio->AddEntry(tg, Form("%i ADC counts", i * 100 - 200), "l");
   }
   legRatio->Draw();
   pTCanvas->SaveAs(OutputFile_End.c_str());
   delete pTCanvas;
}