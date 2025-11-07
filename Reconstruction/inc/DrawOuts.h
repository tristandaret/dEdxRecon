/**
 * @file DrawOuts.h
 * @brief Declaration of the DrawOuts helper class for plotting and summaries.
 *
 * The DrawOuts class provides routines for creating plots and visual
 * summaries of reconstruction results (scan drawing, energy loss plots,
 * comparisons, etc.). Implementation and plotting logic live in
 * DrawOuts.cxx.
 *
 * @details
 * A small top-level executable `DrawOut` exists (Apps/DrawOut.cxx) which
 * calls into `Reconstruction::DrawOuts` as the runner for generating
 * visual outputs. The class itself provides fine-grained control functions
 * used by the executable and by any other consumer of plotting helpers.
 */

#ifndef DRAWOUTS_H
#define DRAWOUTS_H

#include "Misc.h"
#include "SetStyle.h"
#include "TLegend.h"
#include "dEdx.h"
namespace Reconstruction {

/**
 * @class DrawOuts
 * @brief Plotting helper for reconstruction results
 *
 * Provides methods to create scan drawings, energy loss plots, comparisons
 * and other visual summaries used by the analysis tools.
 */
class DrawOuts {
public:
  /** @brief Default constructor */
  DrawOuts();

  /**
   * @brief Construct from a single input file
   * @param inputFile path to the input file
   */
  DrawOuts(const std::string &inputFile);

  /**
   * @brief Construct from a list of input files
   * @param v_inputFiles vector with input file paths
   */
  DrawOuts(const std::vector<std::string> &v_inputFiles);

  /** @brief Virtual destructor */
  virtual ~DrawOuts();

  /** @brief Set plotting style (ROOT style settings) */
  void SetStyle();

  /** @brief Fill DESY21 single-run scan data */
  void DESY21ScanFill();

  /** @brief Draw DESY21 scan summaries */
  void DESY21ScanDraw();

  /** @brief Fill CERN22 single-run scan data */
  void CERN22ScanFill();

  /** @brief Draw CERN22 scan summaries */
  void CERN22ScanDraw();

  /** @brief General control/entry point for plotting */
  void Control();

  /**
   * @brief Produce energy-loss plots
   * @param methods selection of methods (0: both, 1: only WF, 2: only XP)
   */
  void EnergyLoss(const int &methods = 0);

  /** @brief Produce file comparison plots */
  void FileComparison();

  /**
   * @brief Draw DESY21 single scan
   * @param methods selection of methods (0: both, 1: only WF, 2: only XP)
   */
  void DESY21SingleScan(const int &methods = 0);

  /**
   * @brief Draw DESY21 multi-scan summaries
   * @param methods selection of methods (0: both, 1: only WF, 2: only XP)
   */
  void DESY21MultiScan(const int &methods = 0);

  /** @brief Draw CERN22 scan summaries (wrapper) */
  void CERN22Scan();

  /** @brief Plot amplitude versus cluster length */
  void AmplitudeVSLength();

private:
  // Files
  std::string finputFile;
  std::string foutputDir;
  std::string foutputFile;
  std::string foutputFileWF;
  std::string foutputFileXP;

  // Data file
  std::vector<TFile *> v_fFiles;
  std::vector<TTree *> v_fTrees;
  std::vector<RecoEvent *> v_fEvents;
  std::vector<int> v_fnentries;

  // Reconstruction classes
  Reconstruction::RecoEvent *p_recoevent;
  Reconstruction::RecoModule *p_recomodule;
  Reconstruction::RecoCluster *p_recocluster;
  Reconstruction::RecoPad *p_recopad;

  // Settings
  std::string fparticleType;
  TCanvas *fpCanvas;
  int drawMultiScans = 0; // 0: normal scan | 1: multi scan
  int fwhichMethods = 0;  // 0: both methods | 1: only WF | 2: only XP
  int fnMethods = 2;      // Number of methods: 2 (WF and XP) or 1 (WF or XP)

  // Shared variables
  int NMod = 0;
  int NClusters = 0;
  int NPads = 0;
  int ix = 0;
  int iy = 0;
  int position = 0;

  constexpr static float YRESOMAX = 11;
  constexpr static float YRESOMIN = 6.5;
  constexpr static float YMEANMAX = 1000;
  constexpr static float YMEANMIN = 550;
  constexpr static float YSTDMAX = 90;
  constexpr static float YSTDMIN = 40;

  constexpr static float YRESOMAXCERN = 7;
  constexpr static float YRESOMINCERN = 3;
  constexpr static float YMEANMAXCERN = 3;
  constexpr static float YMEANMINCERN = 0.5;
  constexpr static float YSTDMAXCERN = 0.15;
  constexpr static float YSTDMINCERN = 0;
  float keV = 5.9 / (224 * 1703.74 /
                     183); // 5.9 Fe peak energy | 1703 mean MockUp gain | 224
                           // e- created with 5.9keV | 183 e- for 1 ADC

  // Single scan pointers
  TF1 *fptf1_WF;
  TF1 *fptf1_XP;
  TGraphErrors *fpTGE_reso_WF;
  TGraphErrors *fpTGE_reso_XP;
  TGraphErrors *fpTGE_mean_WF;
  TGraphErrors *fpTGE_mean_XP;
  TGraphErrors *fpTGE_std_WF;
  TGraphErrors *fpTGE_std_XP;

  // Multiple scan pointers
  int nScans = 0;
  int nRuns = 0;
  std::vector<int> markers = {22, 34, 23, 47, 33, 43};
  std::vector<int> colors = {kCyan - 6,    kMagenta - 6, kCyan + 2,
                             kMagenta + 2, kCyan + 3,    kMagenta + 3};
  std::vector<int> markersCERN = {20, 47, 34, 21};
  std::vector<int> colorsCERN = {kOrange + 7, kAzure - 6, kSpring - 6,
                                 kRed + 1};
  std::vector<TF1 *> v_fptf1_WF;
  std::vector<TF1 *> v_fptf1_XP;
  std::vector<TGraphErrors *> v_fpTGE_mean_WF;
  std::vector<TGraphErrors *> v_fpTGE_mean_XP;
  std::vector<TGraphErrors *> v_fpTGE_std_WF;
  std::vector<TGraphErrors *> v_fpTGE_std_XP;
  std::vector<TGraphErrors *> v_fpTGE_reso_WF;
  std::vector<TGraphErrors *> v_fpTGE_reso_XP;

  // Bethe-Bloch fitting
  std::vector<TF1 *> v_fptf1_BB;
};
} // namespace Reconstruction

#endif