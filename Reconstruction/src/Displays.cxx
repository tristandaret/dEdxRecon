#include "Displays.h"
#include "SignalTools.h"
#include "Util.h"
#include "ParabolaFunction.h"
#include "ParabolaFunctionNG.h"
#include "SetStyle.h"

#include <typeinfo>

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TLegendEntry.h"
#include "TFile.h"
#include "TROOT.h"

// Output event display of an event	with tagging string TAG, placed in OUTDIR dir
void DrawOut_EventDisplay(Event *pEvent, const int &ModuleNber, const std::string &OUTDIR,
                          const std::string &TAG)
{
   return DrawOut_EventDisplay(pEvent->Get_ThisModule(ModuleNber), OUTDIR, TAG,
                               "amplitude", 999, 999, 999);
}
void DrawOut_EventDisplay(Module *pModule, const std::string &OUTDIR,
                          const std::string &TAG, const std::string &type,
                          const double &parabola, const double &slope,
                          const double &intercept)
{

   int OPTSTAT_IN = gStyle->GetOptStat();
   gStyle->SetOptStat(0);
   std::string OutputFile;
   TH2D *pTH2D = new TH2D;
   if (type == "amplitude") {
      pTH2D = GiveMe_EvtDisplay(pModule, TAG);
      pTH2D->SetMaximum(4096);
      OutputFile = OUTDIR + "Event_Display_Entry_" +
                   std::to_string(pModule->Get_EntryNber()) + "_Evt_" +
                   std::to_string(pModule->Get_EventNber()) + "_Mod_" +
                   std::to_string(pModule->Get_ModuleNber()) + ".png";
   }
   if (type == "time") {
      pTH2D = GiveMe_TimeDisplay(pModule, TAG);
      OutputFile = OUTDIR + "Time_Display_Entry_" +
                   std::to_string(pModule->Get_EntryNber()) + "_Evt_" +
                   std::to_string(pModule->Get_EventNber()) + "_Mod_" +
                   std::to_string(pModule->Get_ModuleNber()) + ".png";
      // pTH2D->SetMaximum(510);
      pTH2D->SetMinimum(1.1 * pTH2D->GetMinimum(0) - 0.1 * pTH2D->GetMaximum());
      gStyle->SetPalette(62);
      TColor::InvertPalette();
   }
   TF1 *pTF1_Track = new TF1(Form("pTF1_Track_%i", pModule->Get_EntryNber()),
                             "[0] + [1]*x + [2]*x*x", -1, 37);

   TCanvas *pTCanvas_Evt = new TCanvas("pTCanGP", "pTCanGP", 1800, 1200);
   pTCanvas_Evt->cd();
   pTH2D->Draw("colz");
   if (slope != 999 && intercept != 999) {
      pTF1_Track->SetParameters((intercept * 100 - 0.5) / 1.019, slope * 1.128 / 1.019,
                                (parabola / 100) * 1.128 / 1.019);
      pTF1_Track->SetLineColor(kRed);
      pTF1_Track->SetLineWidth(5);
      pTF1_Track->Draw("same");
   }
   pTCanvas_Evt->SaveAs(OutputFile.c_str());
   delete pTH2D;
   delete pTF1_Track;
   delete pTCanvas_Evt;
   if (type == "time")
      TColor::InvertPalette(); // reset invertion

   gStyle->SetOptStat(OPTSTAT_IN);
}

// Get event display histo of an event with tagging string TAG
TH2D *GiveMe_EvtDisplay(Event *pEvent, const int &ModuleNber, const std::string &TAG)
{
   return GiveMe_EvtDisplay(pEvent->Get_ThisModule(ModuleNber), TAG);
}
TH2D *GiveMe_EvtDisplay(Module *pModule, const std::string &TAG)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << TAG;
   aostringstream << " Entry " << pModule->Get_EntryNber();
   aostringstream << " Event " << pModule->Get_EventNber();
   aostringstream << " Display ";
   std::string Title = aostringstream.str();

   TH2D *ToBeReturned =
      new TH2D(Title.c_str(), Title.c_str(), 36, -0.5, 35.5, 32, -0.5, 31.5);

   std::vector<Cluster *> ClusterSet = pModule->GiveMe_Clusters_ForThisModule();
   int NClusters = ClusterSet.size();
   for (int iC = 0; iC < NClusters; iC++) {
      Cluster *pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0; iP < NPads; iP++) {
         Pad *pPad = pCluster->Get_Pad(iP);
         int iX = pPad->Get_iX();
         int iY = pPad->Get_iY();
         int Qv = pPad->Get_AMax();

         ToBeReturned->Fill(iX, iY, Qv);
      }
   }

   return ToBeReturned;
}

// Get Timing Event display
TH2D *GiveMe_TimeDisplay(Module *pModule, const std::string &TAG)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << TAG;
   aostringstream << " Entry " << pModule->Get_EntryNber();
   aostringstream << " Event " << pModule->Get_EventNber();
   aostringstream << " Time Display ";
   std::string Title = aostringstream.str();

   TH2D *ToBeReturned =
      new TH2D(Title.c_str(), Title.c_str(), 36, -0.5, 35.5, 32, -0.5, 31.5);

   std::vector<Cluster *> ClusterSet = pModule->GiveMe_Clusters_ForThisModule();
   int NClusters = ClusterSet.size();
   for (int iC = 0; iC < NClusters; iC++) {
      Cluster *pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0; iP < NPads; iP++) {
         Pad *pPad = pCluster->Get_Pad(iP);
         int iX = pPad->Get_iX();
         int iY = pPad->Get_iY();
         int Tmax = pPad->Get_TMax();

         ToBeReturned->Fill(iX, iY, Tmax);
      }
   }

   return ToBeReturned;
}

// Output waveform of a pad	with tagging string TAG, placed in OUTDIR dir
void DrawOut_WaveFormDisplay(Pad *pPad, const std::string &OUTDIR, const std::string &TAG)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << OUTDIR;
   aostringstream << TAG;
   aostringstream << "_WaveForm";
   aostringstream << "_Ent_" << pPad->Get_EntryNber();
   aostringstream << "_EVt_" << pPad->Get_EventNber();
   aostringstream << "_Mod_" << pPad->Get_ModuleNber();
   aostringstream << "_iX_" << pPad->Get_iX();
   aostringstream << "_iY_" << pPad->Get_iY();
   aostringstream << ".png";
   std::string OutputFile = aostringstream.str();

   TH1F *pTH1F = GiveMe_WaveFormDisplay(pPad, TAG);

   int AMax_WF = pPad->Get_AMax_WF();
   int TMax_WF = pPad->Get_TMax_WF();

   TCanvas *pTCanvas = new TCanvas;
   pTCanvas->cd();
   pTH1F->Draw();
   pTCanvas->Update();

   double Xmin = pTCanvas->GetUxmin();
   double Xmax = pTCanvas->GetUxmax();
   double Ymin = pTCanvas->GetUymin();
   double Ymax = pTCanvas->GetUymax();

   //
   TLine *pTLineV_WF = new TLine(TMax_WF, Ymin, TMax_WF, Ymax);
   pTLineV_WF->SetLineStyle(10);
   pTLineV_WF->SetLineWidth(1);
   pTLineV_WF->SetLineColor(4);
   pTLineV_WF->Draw();
   pTCanvas->Update();

   TLine *pTLineH_WF = new TLine(Xmin, AMax_WF, Xmax, AMax_WF);
   pTLineH_WF->SetLineStyle(10);
   pTLineH_WF->SetLineWidth(1);
   pTLineH_WF->SetLineColor(4);
   pTLineH_WF->Draw();
   pTCanvas->Update();

   TGraph *pTGraph_WF = new TGraph;
   pTGraph_WF->SetPoint(pTGraph_WF->GetN(), TMax_WF, AMax_WF);
   pTGraph_WF->SetMarkerStyle(25);
   pTGraph_WF->SetMarkerColor(4);
   pTGraph_WF->Draw("P");
   pTCanvas->Update();

   double T_FitMin = TMax_WF - 5;
   double T_FitMax = TMax_WF + 5;

   ParabolaFunctionNG aParabolaFunctionNG;
   TF1 *pTF1 = new TF1("ParabolaFunctionNG", aParabolaFunctionNG, T_FitMin, T_FitMax, 4);

   pTF1->SetParameter(0, pPad->Get_FIT_A0P());
   pTF1->SetParameter(1, pPad->Get_FIT_A0M());
   pTF1->SetParameter(2, pPad->Get_FIT_X0());
   pTF1->SetParameter(3, pPad->Get_FIT_Y0());
   pTF1->Draw("Same");
   pTCanvas->Update();

   pTCanvas->SaveAs(OutputFile.c_str());
   delete pTH1F;
   delete pTF1;
   delete pTCanvas;
}

// Get waveform histo of pad with tagging string TAG
TH1F *GiveMe_WaveFormDisplay(Pad *pPad, const std::string &TAG)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << TAG;
   aostringstream << " WaveForm ";
   aostringstream << " Entry " << pPad->Get_EntryNber();
   aostringstream << " Event " << pPad->Get_EventNber();
   aostringstream << " Mod " << pPad->Get_ModuleNber();
   aostringstream << " iX " << pPad->Get_iX();
   aostringstream << " iY " << pPad->Get_iY();
   aostringstream << ";Time (/40 ns);ADC";
   std::string Title = aostringstream.str();

   std::ostringstream namestring;
   namestring << std::setiosflags(std::ios::fixed);
   namestring << " pTH1_WF";
   namestring << "_Entry" << pPad->Get_EntryNber();
   namestring << "_Evt" << pPad->Get_EventNber();
   namestring << "_Mod" << pPad->Get_ModuleNber();
   namestring << "_iX" << pPad->Get_iX();
   namestring << "_iY" << pPad->Get_iY();
   namestring << "_AMax" << pPad->Get_AMax();
   namestring << TAG;
   std::string Name = namestring.str();

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 510, -0.5, 509.5);

   std::vector<int> The_v_ADC = pPad->Get_vADC();
   int NADC = The_v_ADC.size();
   for (Int_t iTimeBin = 0; iTimeBin < NADC; iTimeBin++) {
      int ADC_value = The_v_ADC[iTimeBin];
      if (ADC_value <= -250)
         continue;
      int iTimeBinLoc = iTimeBin + 1; // NB: the 1st Data (iTimeBin=0) goes in the 1st bin
                                      // (iTimeBinLoc=1) [-0.5,+0.5]
      if (iTimeBinLoc >= 1 && iTimeBinLoc <= 510)
         ToBeReturned->SetBinContent(iTimeBinLoc, ADC_value);
   }

   return ToBeReturned;
}

// TD: Draw Waveforms of all Pads in a Cluster
void DrawOut_ClusterWFDisplay(Cluster *pCluster, const std::string &OUTDIR,
                              const std::string &TAG)
{
   return DrawOut_ClusterWFDisplay(pCluster, OUTDIR, TAG, 1, 412, 40);
}

void DrawOut_ClusterWFDisplay(Cluster *pCluster, const std::string &OUTDIR,
                              const std::string &TAG, const int &Option, const int &PT,
                              const int &TB)
{

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << OUTDIR << "/";
   aostringstream << TAG;
   aostringstream << "_WF";
   aostringstream << "_Entry_" << pCluster->Get_EntryNber();
   aostringstream << "_Evt_" << pCluster->Get_EventNber();
   aostringstream << "_Mod_" << pCluster->Get_ModuleNber();
   aostringstream << "_Clus_" << (pCluster->Get_LeadingPad())->Get_iX();
   aostringstream << ".png";
   std::string OutputFile = aostringstream.str();
   std::string OutCWF = OUTDIR + TAG + "_CWF_Event_" +
                        std::to_string(pCluster->Get_EventNber()) + "_Mod_" +
                        std::to_string(pCluster->Get_ModuleNber()) + "_Cluster_" +
                        std::to_string(pCluster->Get_LeadingPad()->Get_iX()) + ".png";
   gStyle->SetStatX(0.9);
   gStyle->SetStatY(0.9);

   std::vector<TH1F *> v_histo;
   std::vector<Pad *> v_Pads;
   std::vector<RankedValue> vRank;
   int NPads = pCluster->Get_NberOfPads();
   for (int iP = 0; iP < NPads; iP++) { // Pads
      Pad *pPad = pCluster->Get_Pad(NPads - iP -
                                    1); // Start by end of list to get most energetic pads
      TH1F *pTH1F = GiveMe_WaveFormDisplay(pPad, TAG);
      pTH1F->SetLineWidth(4);
      v_histo.push_back(pTH1F);
      v_Pads.push_back(pPad);
      double iY = pPad->Get_iY();
      RankedValue rankiY;
      rankiY.Value = iY;
      rankiY.Rank = iP;
      vRank.push_back(rankiY);
   }
   std::sort(vRank.rbegin(), vRank.rend()); // Sorting the pads wrt iY

   // Sum the pads WF
   TH1F *pTH1F_Sum = (TH1F *)v_histo[0]->Clone("Sum");
   pTH1F_Sum->SetLineWidth(4);
   for (int iP = 1; iP < NPads; iP++) {
      pTH1F_Sum->Add(v_histo[iP]);
   }
   std::ostringstream aostringstream_pTH1F_Sum;
   aostringstream_pTH1F_Sum << std::setiosflags(std::ios::fixed);
   aostringstream_pTH1F_Sum << "Sum of the pads of cluster: ";
   aostringstream_pTH1F_Sum << " Entry " << pCluster->Get_EntryNber();
   aostringstream_pTH1F_Sum << " Event " << pCluster->Get_EventNber();
   aostringstream_pTH1F_Sum << " Module " << pCluster->Get_ModuleNber();
   aostringstream_pTH1F_Sum << " Cluster " << (pCluster->Get_LeadingPad())->Get_iX();
   std::string Title_pTH1F_Sum = aostringstream_pTH1F_Sum.str();
   pTH1F_Sum->SetTitle(Title_pTH1F_Sum.c_str());

   double ADCminWF = 0; // Search in all pads the min and max value reached for ADCs
   double ADCmaxWF = 0;
   for (int iP = 0; iP < NPads; iP++) {
      double ADCminPad = v_histo[iP]->GetMinimum();
      double ADCmaxPad = v_histo[iP]->GetMaximum();
      if (iP == 0) {
         ADCminWF = ADCminPad;
         ADCmaxWF = ADCmaxPad;
      }
      if (ADCminPad < ADCminWF)
         ADCminWF = ADCminPad;
      if (ADCmaxPad > ADCmaxWF)
         ADCmaxWF = ADCmaxPad;
   }

   double Diff = ADCmaxWF - ADCminWF;
   ADCminWF = ADCminWF - 0.1 * Diff;
   ADCmaxWF = ADCmaxWF + 0.1 * Diff;

   TCanvas *pTCanWF = new TCanvas("WF", "WF", 180, 10, 1000, 500 * NPads);
   TCanvas *pTCanCWF = new TCanvas("pTCanCWF", "pTCanCWF", 180, 10, 1200, 900);
   if (Option == 0) {
      pTCanWF->Divide(1, NPads);
   } else {
      pTCanWF->Divide(1, NPads + 1);
   }
   for (int iP = 0; iP < NPads; iP++) {
      int iPL = vRank[iP].Rank;
      pTCanWF->cd(iP + 1);
      v_histo[iPL]->SetAxisRange(pTH1F_Sum->GetMaximumBin() - 20,
                                 pTH1F_Sum->GetMaximumBin() + 75, "X");
      v_histo[iPL]->Draw();
      pTCanWF->Update();

      // Apply Y range to all WF but if summit fits are requested
      if (Option >= 2) {
         ADCmaxWF = v_histo[iPL]->GetMaximum();
         ADCminWF = v_histo[iPL]->GetMinimum();
         Diff = ADCmaxWF - ADCminWF;
         ADCminWF = ADCminWF - 0.1 * Diff;
         ADCmaxWF = ADCmaxWF + 0.1 * Diff;
         v_histo[iPL]->SetMinimum(ADCminWF);
         v_histo[iPL]->SetMaximum(ADCmaxWF);
      } else {
         v_histo[iPL]->SetMinimum(ADCminWF);
         v_histo[iPL]->SetMaximum(ADCmaxWF);
      }

      // Add line of TMax of each pad, on each WF
      TLine *pTLineV_IN =
         new TLine(v_Pads[iPL]->Get_TMax(), ADCminWF, v_Pads[iPL]->Get_TMax(), ADCmaxWF);
      pTLineV_IN->SetLineStyle(2);
      pTLineV_IN->SetLineWidth(4);
      pTLineV_IN->SetLineColor(4);
      pTLineV_IN->Draw();
      pTCanWF->Update();

      // Add line of TMax of Leading pad, on each WF
      TLine *pTLineV_Lead =
         new TLine(v_Pads[0]->Get_TMax(), ADCminWF, v_Pads[0]->Get_TMax(), ADCmaxWF);
      pTLineV_Lead->SetLineStyle(9);
      pTLineV_Lead->SetLineWidth(4);
      pTLineV_Lead->SetLineColor(2);
      pTLineV_Lead->Draw();
      pTCanWF->Update();

      if (Option >= 2) {
         // Add summit fit
         TLegend *pTLegend_A = new TLegend(0.40, 0.75, 0.60, 0.83);
         pTLegend_A->SetBorderSize(0);

         std::ostringstream aostringstream_pTLegEntr_A;
         aostringstream_pTLegEntr_A << std::setiosflags(std::ios::fixed);
         aostringstream_pTLegEntr_A << "Pad Rank " << iPL;
         TLegendEntry *pTLegendEntry_A = pTLegend_A->AddEntry(
            (TObject *)0, (aostringstream_pTLegEntr_A.str()).c_str(), "");
         pTLegendEntry_A->SetTextColor(2);

         pTLegend_A->Draw();
         pTCanWF->Update();

         if (v_Pads[iPL]->Get_FIT_Status() == 0) {
            double T_FitMin = v_Pads[iPL]->Get_FIT_Xmin();
            double T_FitMax = v_Pads[iPL]->Get_FIT_Xmax();

            ParabolaFunctionNG aParabolaFunctionNG;
            TF1 *pTF1 =
               new TF1("ParabolaFunctionNG", aParabolaFunctionNG, T_FitMin, T_FitMax, 4);

            pTF1->SetParameter(0, v_Pads[iPL]->Get_FIT_A0P());
            pTF1->SetParameter(1, v_Pads[iPL]->Get_FIT_A0M());
            pTF1->SetParameter(2, v_Pads[iPL]->Get_FIT_X0());
            pTF1->SetParameter(3, v_Pads[iPL]->Get_FIT_Y0());
            pTF1->Draw("Same");
            pTCanWF->Update();

            double X_MaxFit = v_Pads[iPL]->Get_FIT_X0();
            double Y_MaxFit = v_Pads[iPL]->Get_FIT_Y0();
            TGraph *pTGraph_MaxFit = new TGraph;
            pTGraph_MaxFit->SetPoint(pTGraph_MaxFit->GetN(), X_MaxFit, Y_MaxFit);
            pTGraph_MaxFit->SetMarkerStyle(24);
            pTGraph_MaxFit->SetMarkerColor(2);
            pTGraph_MaxFit->Draw("P");
            pTCanWF->Update();
         } else {
            TLegend *pTLegend_B = new TLegend(0.40, 0.67, 0.60, 0.75);
            pTLegend_B->SetBorderSize(0);

            TLegendEntry *pTLegendEntry_B =
               pTLegend_B->AddEntry((TObject *)0, "Failed fit", "");
            pTLegendEntry_B->SetTextColor(2);

            pTLegend_B->Draw();
            pTCanWF->Update();
         }
      }
   }

   // Draw sum
   if (Option >= 1) {
      pTCanWF->cd(NPads + 1);

      ADCmaxWF = pTH1F_Sum->GetMaximum();
      ADCminWF = pTH1F_Sum->GetMinimum();

      float Tsum = pTH1F_Sum->GetMaximumBin();
      TH1F *pTH1F_ETF = ETF("pTH1F_ETF_" + pCluster->Get_EntryNber() +
                               pCluster->Get_LeadingPad()->Get_iX(),
                            -0.5, 509.5, Tsum - PT / TB, 510, 999, PT, TB);
      pTH1F_ETF->SetAxisRange(pTH1F_Sum->GetMaximumBin() - 20,
                              pTH1F_Sum->GetMaximumBin() + 75, "X");
      pTH1F_ETF->Scale(ADCmaxWF);
      pTH1F_ETF->SetLineColor(kRed);
      pTH1F_ETF->SetLineWidth(4);

      Diff = ADCmaxWF - ADCminWF;
      ADCminWF = ADCminWF - 0.1 * Diff;
      ADCmaxWF = ADCmaxWF + 0.1 * Diff;
      pTH1F_Sum->SetMinimum(ADCminWF);
      pTH1F_Sum->SetMaximum(ADCmaxWF);

      pTH1F_Sum->SetAxisRange(pTH1F_Sum->GetMaximumBin() - 20,
                              pTH1F_Sum->GetMaximumBin() + 75, "X");
      pTH1F_Sum->Draw();
      pTH1F_ETF->DrawClone("hist same");
      TLegend *leg = new TLegend(0.5, 0.65, 0.9, 0.9);
      leg->AddEntry(pTH1F_Sum, "Cluster's waveform ", "l");
      leg->AddEntry(pTH1F_ETF, "Dirac Pulse Response (ETF)", "l");
      leg->DrawClone();

      pTCanWF->Update();
      pTCanCWF->cd();
      pTH1F_Sum->Draw();
      pTH1F_ETF->DrawClone("hist same");
      leg->DrawClone();
      pTCanCWF->Update();

      delete pTH1F_ETF;
      delete leg;
   }

   //
   pTCanWF->SaveAs(OutputFile.c_str());
   // pTCanCWF->SaveAs(OutCWF.c_str());
   delete pTCanWF;
   delete pTCanCWF;

   //
   delete pTH1F_Sum;
   for (int iP = 0; iP < NPads; iP++) {
      delete v_histo[iP];
      v_histo[iP] = 0;
   }
   v_histo.clear();
}

// Draw Event waveform
void DrawOut_GWF(Event *pEvent, const int &ModuleNber, const std::string &OUTDIR,
                 const std::string &TAG, const int &PT, const int &TB,
                 const float &phi_rad)
{
   std::string OutputFile = OUTDIR + TAG + "_GigaWF_Entry_" +
                            std::to_string(pEvent->Get_EntryNber()) + "_Event_" +
                            std::to_string(pEvent->Get_EventNber()) + "_Mod_" +
                            std::to_string(ModuleNber) + ".png";
   TH1F *pTH1F_GWF = new TH1F("pTH1F_GWF", "pTH1F_GWF", 510, -0.5, 509.5);
   pTH1F_GWF->SetTitle(
      std::string(
         TAG + " Entry " + std::to_string(pEvent->Get_EntryNber()) + " Event " +
         std::to_string(pEvent->Get_EventNber()) + " Module " +
         std::to_string(pEvent->Get_Module_InArray(ModuleNber)->Get_ModuleNber()) +
         " Giga WaveForm (GWF);Time (/40 ns);ADC count")
         .c_str());
   pTH1F_GWF->SetTitleSize(0.1, "t");
   Module *pModule = pEvent->Get_ThisModule(ModuleNber);
   int NClusters = pModule->Get_NberOfCluster();
   int NClus_trunc = int(floor(pModule->Get_NberOfCluster() * 0.7));
   std::vector<TH1F *> v_pTH1F_WF_ETFcluster;
   std::vector<RankedValue> v_GWF;

   // Loop On Clusters
   for (int iC = 0; iC < NClusters; iC++) {
      TH1F *pTH1F_cluster = new TH1F("pTH1F_cluster", "pTH1F_cluster", 510, -0.5,
                                     509.5); // Store WF of cluster's pads
      Cluster *pCluster = pModule->Get_Cluster(iC);

      // Loop On Pads
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0; iP < NPads; iP++) {
         Pad *pPad = pCluster->Get_Pad(iP);
         TH1F *pTH1F_pad = GiveMe_WaveFormDisplay(pPad, "main");
         pTH1F_cluster->Add(pTH1F_pad);
         pTH1F_GWF->Add(pTH1F_pad);
         if (pPad == pCluster->Get_LeadingPad()) {
            RankedValue rank_iC;
            rank_iC.Rank = iC;
            rank_iC.Value = pPad->Get_AMax();
            v_GWF.push_back(rank_iC);
         }
         delete pTH1F_pad;
      }
      TH1F *pTH1F_WF_ETFcluster =
         ETF("pTH1F_WF_ETFcluster", -0.5, 509.5, pTH1F_cluster->GetMaximumBin() - PT / TB,
             510, iC, PT, TB);
      pTH1F_WF_ETFcluster->Scale(pTH1F_cluster->GetMaximum());
      v_pTH1F_WF_ETFcluster.push_back(pTH1F_WF_ETFcluster);
      delete pTH1F_cluster;
   }

   // GPv3
   std::sort(v_GWF.begin(), v_GWF.end());
   TH1F *pTH1F_GWFsum = new TH1F("pTH1F_GWFsum", "pTH1F_GWFsum", 510, -0.5, 509.5);
   pTH1F_GWFsum->Add(pTH1F_GWF);
   for (int iC = (int)NClus_trunc; iC < NClusters; iC++) {
      pTH1F_GWFsum->Add(v_pTH1F_WF_ETFcluster[v_GWF[iC].Rank], -1);
   }

   int TmaxGWF = pTH1F_GWF->GetMaximumBin();
   int TmaxGWFsum = pTH1F_GWFsum->GetMaximumBin();
   float maxGP = pTH1F_GWF->GetMaximum();
   float maxGPtrunc = pTH1F_GWFsum->GetMaximum();

   TH1F *pTH1F_ETF =
      ETF("pTH1F_ETF", -0.5, 509.5, TmaxGWFsum - PT / TB, 510, 999, PT, TB);
   pTH1F_ETF->Scale(maxGPtrunc);

   // Drawing the GWF
   // ------------------------------------------------------------------------------------------------------------

   TCanvas *pTCanGP = new TCanvas("pTCanGP", "pTCanGP", 1600, 1200);
   pTCanGP->SetMargin(0.12, 0.05, 0.1, 0.1);
   gStyle->SetOptStat(0);
   pTCanGP->cd();
   pTH1F_GWF->SetLineWidth(7);
   pTH1F_GWF->SetLineColor(kGreen + 2);
   pTH1F_GWF->Draw();
   pTH1F_GWF->SetAxisRange(pTH1F_GWF->GetMaximumBin() - 20,
                           pTH1F_GWF->GetMaximumBin() + 75, "X");
   pTH1F_GWFsum->SetLineWidth(7);
   pTH1F_GWFsum->SetLineColor(kMagenta + 3);
   pTH1F_GWFsum->Draw("same hist");
   pTH1F_ETF->SetLineWidth(4);
   pTH1F_ETF->SetLineColor(kOrange + 7);
   pTH1F_ETF->Draw("same hist");
   pTCanGP->Update();

   double tmin = pTCanGP->GetUxmin();
   double tmax = pTCanGP->GetUxmax();
   double Ymin = pTCanGP->GetUymin();
   double Ymax = pTCanGP->GetUymax();

   TLine *pTLine_TmaxGWF = new TLine(TmaxGWF - 1, Ymin, TmaxGWF - 1, Ymax);
   pTLine_TmaxGWF->SetLineStyle(1);
   pTLine_TmaxGWF->SetLineWidth(4);
   pTLine_TmaxGWF->SetLineColor(kGreen + 2);
   pTLine_TmaxGWF->Draw();

   TLine *pTLine_TmaxGWFsum = new TLine(TmaxGWFsum - 1, Ymin, TmaxGWFsum - 1, Ymax);
   pTLine_TmaxGWFsum->SetLineStyle(1);
   pTLine_TmaxGWFsum->SetLineWidth(4);
   pTLine_TmaxGWFsum->SetLineColor(kMagenta + 3);
   pTLine_TmaxGWFsum->Draw();

   TLine *pTLine_maxGP = new TLine(tmin, maxGP, tmax, maxGP);
   pTLine_maxGP->SetLineStyle(1);
   pTLine_maxGP->SetLineWidth(4);
   pTLine_maxGP->SetLineColor(kGreen + 2);
   pTLine_maxGP->Draw();

   TLine *pTLine_maxGPtrunc = new TLine(tmin, maxGPtrunc, tmax, maxGPtrunc);
   pTLine_maxGPtrunc->SetLineStyle(1);
   pTLine_maxGPtrunc->SetLineWidth(5);
   pTLine_maxGPtrunc->SetLineColor(kMagenta + 3);
   pTLine_maxGPtrunc->Draw();

   TLegend *leg = new TLegend(0.55, 0.7, 0.95, 0.9);
   leg->AddEntry(pTH1F_GWF, "GWF_{total} ", "l");
   leg->AddEntry(pTH1F_GWFsum, "GWF_{truncated} ", "l");
   leg->AddEntry(pTH1F_ETF, "Dirac Pulse Response (ETF) ", "l");
   leg->Draw();
   pTCanGP->Update();

   maxGP /= (NClusters / TMath::Cos(phi_rad));
   maxGPtrunc /= (NClus_trunc / TMath::Cos(phi_rad));

   TLatex *ptText = new TLatex;
   ptText->SetTextSize(0.045);
   ptText->SetTextAlign(32);
   ptText->SetTextFont(42);
   ptText->SetText(tmax, 0.4 * Ymax, Form("#bar{dE/dx_{total}}	= %.0f ADCs ", maxGP));
   ptText->SetTextColor(kGreen + 2);
   ptText->DrawClone();
   ptText->SetText(tmax, 0.3 * Ymax,
                   Form("#bar{dE/dx_{truncated}}	= %.0f ADCs ", maxGPtrunc));
   ptText->SetTextColor(kMagenta + 3);
   ptText->DrawClone();

   pTCanGP->SaveAs(OutputFile.c_str());

   delete pTH1F_GWF;
   delete pTH1F_GWFsum;
   delete pTH1F_ETF;
   delete pTCanGP;
   delete leg;
   for (int iv = 0; iv < (int)v_pTH1F_WF_ETFcluster.size(); iv++) {
      delete v_pTH1F_WF_ETFcluster[iv];
      v_pTH1F_WF_ETFcluster[iv] = 0;
   }
}

void NewClusterDisplay(Event *pEvent, const std::string &OUTDIR, const std::string &TAG,
                       const int &PT, const int &TB)
{
   // Style setup
   TCanvas *pCanvas = new TCanvas("pCanvas", "pCanvas", 1800, 1350);
   TStyle *ptstyle = SetMyStyle();
   gROOT->SetStyle(ptstyle->GetName());
   gPad->UseCurrentStyle();
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   pCanvas->Divide(1, 2, 0, 0);

   TPad *pTopPad = new TPad("pTopPad", "Top Pad", 0, 0.3, 1, 1);
   TPad *pBottomPad = new TPad("pBottomPad", "Bottom Pad", 0, 0, 1, 0.3);
   pTopPad->SetMargin(0.14, 0.02, 0.0, 0.03);
   pBottomPad->SetMargin(0.14, 0.02, 0.4, 0.0);
   pTopPad->Draw();
   pBottomPad->Draw();
   pTopPad->SetGrid(1, 1);
   pBottomPad->SetGrid(1, 0);

   std::vector<int> colors = {kOrange + 7, kBlue, kRed};
   // Output file setup
   std::string outputFile = OUTDIR + "/" + TAG + "_ClusterWF_Evt" +
                            std::to_string(pEvent->Get_EventNber()) + ".pdf";

   // Start code

   int nModules = pEvent->Get_NberOfModule();
   // Module loop
   for (int iModule = 0; iModule < nModules; iModule++) {
      Module *pModule = pEvent->Get_Module_InArray(iModule);
      int nClusters = pModule->Get_NberOfCluster();

      // Cluster loop
      for (int iCluster = 0; iCluster < nClusters; iCluster++) {
         pTopPad->cd();
         // Initialize range for canvas
         int xmin = 510;
         int xmax = 0;
         double ymin = 1000;
         double ymax = -1000;

         Cluster *pCluster = pModule->Get_Cluster(iCluster);
         std::vector<TH1F *> vWF;
         TH1F *wfCluster =
            new TH1F(Form("wfCluster_%d_%d_%d_%d", pEvent->Get_EventNber(),
                          pModule->Get_ModuleNber(), pCluster->Get_LeadingPad()->Get_iX(),
                          pCluster->Get_LeadingPad()->Get_iY()),
                     "clusterWaveform", 510, -0.5, 509.5);
         int NPads = pCluster->Get_NberOfPads();

         // Legend setup
         TLegend leg(0.55, 0.55, 0.92, 0.9);
         leg.SetTextSize(0.06);
         leg.SetTextColor(kBlue - 1);
         leg.SetFillStyle(1001);
         leg.SetBorderSize(1);
         leg.SetLineWidth(1);

         // Pad loop: Get waveforms
         for (int iP = 0; iP < NPads; iP++) {
            // Get waveform of pad
            Pad *pPad = pCluster->Get_Pad(NPads - iP - 1);
            vWF.push_back(GiveMe_WaveFormDisplay(pPad, TAG));
            wfCluster->Add(vWF.back());

            // Find T0
            int x0 = 0;
            for (int ix = 1; ix < vWF.back()->GetNbinsX(); ix++) {
               int binContent = vWF.back()->GetBinContent(ix);
               if (binContent != 0) {
                  x0 = ix;
                  break; // Stop at first non-zero bin
               }
            }

            // Update axes range
            xmin = std::min(xmin, (int)vWF.back()->GetXaxis()->GetBinLowEdge(x0));
            xmax = xmin + 150;
            ymax = std::max(ymax, vWF.back()->GetMaximum());
            ymin = std::min(ymin, vWF.back()->GetMinimum());
         }
         // Update axes range with cluster waveform
         vWF[0]->SetAxisRange(xmin - 20, xmax, "X");
         ymin = std::min(ymin, wfCluster->GetMinimum());
         ymax = std::max(ymax, wfCluster->GetMaximum());
         vWF[0]->GetYaxis()->SetRangeUser(ymin - 0.1 * ymax, 1.1 * ymax);
         vWF[0]->SetTitle(";timebins;Signal [ADC count]");
         vWF[0]->GetYaxis()->SetNdivisions(505, kTRUE);
         vWF[0]->GetYaxis()->SetLabelSize(0.065);
         vWF[0]->GetYaxis()->SetTitleSize(0.065);

         // Pad loop: Draw waveforms
         vWF[0]->SetTitle(Form("Event %d, Module %d, Column %d", pEvent->Get_EventNber(),
                               pModule->Get_ModuleNber(),
                               pCluster->Get_LeadingPad()->Get_iX()));
         Graphic_setup(vWF[0], 1, 1, colors[0], 2, colors[0], 0, 0);
         vWF[0]->DrawClone("HIST");
         for (int iP = 1; iP < NPads; iP++) {
            Graphic_setup(vWF[iP], 1, 1, kGray + 1, 2, kGray + 1, 0, 0);
            vWF[iP]->DrawClone("HIST SAME");
         }

         // Draw sum of waveforms
         Graphic_setup(wfCluster, 1, 1, colors[1], 2, colors[1], 0, 0);
         wfCluster->DrawClone("HIST SAME");

         // Draw ETF
         TH1F *wfETF = ETF(
            Form("wfETF_%d_%d_%d_%d_%d", pEvent->Get_EventNber(), pEvent->Get_EntryNber(),
                 pModule->Get_ModuleNber(), pCluster->Get_LeadingPad()->Get_iX(),
                 pCluster->Get_LeadingPad()->Get_iY()),
            -0.5, 509.5, wfCluster->GetMaximumBin() - float(PT) / float(TB), 510, 999,
            float(PT), float(TB));
         wfETF->Scale(wfCluster->GetMaximum());
         Graphic_setup(wfETF, 1, 1, colors[2], 2, colors[2], 0, 0);
         wfETF->SetLineStyle(1);
         wfETF->DrawClone("HIST SAME");

         // Draw vertical lines for Amax
         TLine xAmax(wfCluster->GetMaximumBin() - 1, ymin - 0.1 * ymax,
                     wfCluster->GetMaximumBin() - 1, ymax + 0.1 * ymax);
         xAmax.SetLineStyle(7);
         xAmax.SetLineWidth(2);
         xAmax.SetLineColor(kViolet - 5);
         xAmax.DrawClone("SAME");

         // Legend display
         leg.AddEntry(wfETF, "Total signal (theory)", "l");
         leg.AddEntry(wfCluster, "Sum of signals", "l");
         leg.AddEntry(vWF[0], "Leading pad's signal", "l");
         if (NPads > 1)
            leg.AddEntry(vWF[1], "Neighbor pads' signals", "l");
         leg.DrawClone();

         // Difference between ETF and cluster waveform
         TH1F *wfDiff = (TH1F *)wfCluster->Clone("wfRelDiff");
         for (int i = 1; i <= wfDiff->GetNbinsX(); ++i) {
            double num = (wfCluster->GetBinContent(i) - wfETF->GetBinContent(i)) * 100;
            double denom = wfCluster->GetMaximum();
            wfDiff->SetBinContent(i, (denom != 0) ? num / fabs(denom) : 0.0);
         }
         Graphic_setup(wfDiff, 1, 1, kMagenta + 2, 2, kMagenta + 2, 0, 0);
         pBottomPad->cd();
         wfDiff->SetAxisRange(xmin - 20, xmax, "X");
         double deltay = 0.1 * (wfDiff->GetMaximum() - wfDiff->GetMinimum());
         wfDiff->SetAxisRange(wfDiff->GetMinimum() - deltay,
                              wfDiff->GetMaximum() + deltay, "Y");
         wfDiff->GetXaxis()->SetTitleSize(0.15);
         wfDiff->GetXaxis()->SetLabelSize(0.15);
         wfDiff->GetYaxis()->SetTitleSize(0.15);
         wfDiff->GetYaxis()->SetLabelSize(0.15);
         wfDiff->GetYaxis()->SetNdivisions(303, kTRUE);
         wfDiff->GetYaxis()->SetTitleOffset(0.4);
         wfDiff->SetTitle(";timebins;#frac{S_{sum} - S_{theo}}{S_{sum}^{max}} [%]");
         wfDiff->DrawClone("HIST");

         // Horizontal line at y=0
         TLine yzero(xmin - 20, 0, xmax, 0); // Draw horizontal line at y=0
         yzero.SetLineStyle(7);
         yzero.SetLineColor(kBlue - 1);
         yzero.Draw("SAME");

         // Vertical line at TMax of leading pad
         xAmax.SetY1(wfDiff->GetMinimum());
         xAmax.SetY2(wfDiff->GetMaximum() + deltay);
         xAmax.DrawClone("SAME");

         // Save
         if (iModule == 0 && iCluster == 0) {
            pCanvas->SaveAs((outputFile + "(").c_str());
         } else if (iModule == nModules - 1 && iCluster == nClusters - 1) {
            pCanvas->SaveAs((outputFile + ")").c_str());
         } else {
            pCanvas->SaveAs(outputFile.c_str());
         }
         for (auto &wf : vWF) {
            delete wf;
            wf = nullptr;
         }

         // Cleaning
         vWF.clear();
         delete wfCluster;
         delete wfETF;
         delete wfDiff;
      }
   }

   // Cleaning
   delete pCanvas;
   delete ptstyle;
}

void NewClusterDisplayMinimal(Event *pEvent, const std::string &OUTDIR,
                              const std::string &TAG)
{
   // Style setup
   TCanvas *pCanvas = new TCanvas("pCanvas", "pCanvas", 1800, 1350);
   TStyle *ptstyle = SetMyStyle();
   gROOT->SetStyle(ptstyle->GetName());
   gPad->UseCurrentStyle();
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   gPad->SetGrid(1, 1);
   gPad->SetLeftMargin(0.14);
   gPad->SetTopMargin(0.02);

   std::vector<int> colors = {kOrange + 7, kBlue + 1, kRed};
   // Output file setup
   std::string outputFile = OUTDIR + "/" + TAG + "_ClusterWF_Evt" +
                            std::to_string(pEvent->Get_EventNber()) + "_Minimal.pdf";

   // Start code

   int nModules = pEvent->Get_NberOfModule();
   // Module loop
   for (int iModule = 0; iModule < nModules; iModule++) {
      Module *pModule = pEvent->Get_Module_InArray(iModule);
      int nClusters = pModule->Get_NberOfCluster();

      // Cluster loop
      for (int iCluster = 0; iCluster < nClusters; iCluster++) {
         // Initialize range for canvas
         int xmin = 510;
         int xmax = 0;
         double ymin = 1000;
         double ymax = -1000;

         Cluster *pCluster = pModule->Get_Cluster(iCluster);
         std::vector<TH1F *> vWF;
         TH1F *wfCluster =
            new TH1F(Form("wfCluster_%d_%d_%d_%d_minimal", pEvent->Get_EventNber(),
                          pModule->Get_ModuleNber(), pCluster->Get_LeadingPad()->Get_iX(),
                          pCluster->Get_LeadingPad()->Get_iY()),
                     "clusterWaveform", 510, -0.5, 509.5);
         int NPads = pCluster->Get_NberOfPads();

         // Legend setup
         TLegend leg(0.5, 0.65, 0.95, 0.9);
         leg.SetTextColor(kBlue - 1);
         leg.SetFillStyle(1001);
         leg.SetBorderSize(1);
         leg.SetLineWidth(1);
         leg.SetTextSize(0.05);

         // Pad loop: Get waveforms
         for (int iP = 0; iP < NPads; iP++) {
            // Get waveform of pad
            Pad *pPad = pCluster->Get_Pad(NPads - iP - 1);
            vWF.push_back(GiveMe_WaveFormDisplay(pPad, TAG + "_minimal"));
            wfCluster->Add(vWF.back());

            // Find T0
            int x0 = 0;
            for (int ix = 1; ix < vWF.back()->GetNbinsX(); ix++) {
               int binContent = vWF.back()->GetBinContent(ix);
               if (binContent != 0) {
                  x0 = ix;
                  break; // Stop at first non-zero bin
               }
            }

            // Update axes range
            xmin = std::min(xmin, (int)vWF.back()->GetXaxis()->GetBinLowEdge(x0));
            xmax = xmin + 150;
            ymax = std::max(ymax, vWF.back()->GetMaximum());
            ymin = std::min(ymin, vWF.back()->GetMinimum());
         }
         // Update axes range with cluster waveform
         vWF[0]->SetAxisRange(xmin - 20, xmax, "X");
         ymin = std::min(ymin, wfCluster->GetMinimum());
         ymax = std::max(ymax, wfCluster->GetMaximum());
         vWF[0]->GetYaxis()->SetRangeUser(ymin - 0.1 * ymax, 1.1 * ymax);
         vWF[0]->SetTitle(";timebins;Signal [ADC count]");
         vWF[0]->GetYaxis()->SetNdivisions(505, kTRUE);
         vWF[0]->GetXaxis()->SetLabelSize(0.05);
         vWF[0]->GetXaxis()->SetTitleSize(0.05);
         vWF[0]->GetYaxis()->SetLabelSize(0.05);
         vWF[0]->GetYaxis()->SetTitleSize(0.05);
         vWF[0]->GetYaxis()->SetTitleOffset(1.3);

         // Pad loop: Draw waveforms
         vWF[0]->SetTitle(Form("Event %d, Module %d, Column %d", pEvent->Get_EventNber(),
                               pModule->Get_ModuleNber(),
                               pCluster->Get_LeadingPad()->Get_iX()));
         Graphic_setup(vWF[0], 1, 1, colors[0], 2, colors[0], 0, 0);
         vWF[0]->DrawClone("HIST");
         for (int iP = 1; iP < NPads; iP++) {
            Graphic_setup(vWF[iP], 1, 1, kGray + 1, 2, kGray + 1, 0, 0);
            vWF[iP]->DrawClone("HIST SAME");
         }

         // Draw sum of waveforms
         Graphic_setup(wfCluster, 1, 1, colors[1], 2, colors[1], 0, 0);
         wfCluster->DrawClone("HIST SAME");

         // Draw vertical lines for Amax
         TLine xAmax(wfCluster->GetMaximumBin() - 1, ymin - 0.1 * ymax,
                     wfCluster->GetMaximumBin() - 1, ymax + 0.1 * ymax);
         xAmax.SetLineStyle(7);
         xAmax.SetLineWidth(2);
         xAmax.SetLineColor(kBlue + 2);
         xAmax.DrawClone("SAME");

         // Legend display
         leg.AddEntry(wfCluster, "Sum of signals", "l");
         leg.AddEntry(vWF[0], "Leading pad's signal", "l");
         if (NPads > 1)
            leg.AddEntry(vWF[1], "Neighbor pads' signals", "l");
         leg.DrawClone();

         // Save
         if (iModule == 0 && iCluster == 0) {
            pCanvas->SaveAs((outputFile + "(").c_str());
         } else if (iModule == nModules - 1 && iCluster == nClusters - 1) {
            pCanvas->SaveAs((outputFile + ")").c_str());
         } else {
            pCanvas->SaveAs(outputFile.c_str());
         }
         for (auto &wf : vWF) {
            delete wf;
            wf = nullptr;
         }

         // Cleaning
         vWF.clear();
         delete wfCluster;
      }
}

   // Cleaning
   delete pCanvas;
   delete ptstyle;
}