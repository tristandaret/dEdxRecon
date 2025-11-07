#/**
 * @file Monitoring.h
 * @brief Monitoring utilities and entry points for analysis workflows.
 *
 * Declares functions used to run monitoring, produce diagnostic summaries
 * (histograms, performance numbers) and control analysis settings.
 *
 * @details
 * A small executable `Apps/Analysis.cxx` exists which calls
 * `Reconstruction::Monitoring()` as a top-level entry point to run
 * analysis workflows; the implementation of Monitoring lives in
 * `Reconstruction/src/Monitoring.cxx`.
 *
 * @details
 * The source file contains routines that assemble diagnostic histograms,
 * run monitoring workflows and produce summaries for inspection. It is the
 * primary entry point used by the `Analysis` executable to run automated
 * monitoring and analysis sequences; configuration and selection setup
 * live in the header-level declarations and other helper modules.
 */
#ifndef MONITORING_H
#define MONITORING_H

#include "GiveMe_Uploader.h"
#include "Uploader.h"

#include "DrawOuts.h"
#include "dEdx.h"

namespace Reconstruction {

extern Reconstruction::dEdx *p_dEdx;
extern Reconstruction::DrawOuts *p_DrawOuts;

void Monitoring();
void Correction(const int &corrRC = 1, const int &corrGain = 1,
                const int &corrWF = 1, const int &corrDrift = 1,
                const int &saveSelectOnly = 1);
void Settings(const std::string &testbeam, const std::string &multiScanName,
              const std::string &scanName, const std::string &scanspec,
              const std::string &runvarstr_name, const int &uploader,
              const int &modules, const int &peaking_time, const int &diffusion,
              const int &drift_dist, const int &timbin);
void ClearVectors();
void DefaultAnalysis();
void DrawSingleScan(const int &methods = 0);
void DrawMultipleScan(const int &methods = 0);
void DrawCERN22Scan();

// Run selection
// Files to use
extern int prototype;
extern int CERN_Escan;
extern int CERN_drift;

extern int DESY_zscan;
extern int DESY_yscan;
extern int DESY_phi;
extern int DESY_theta;

// Computations
extern int dedx;

// DrawOuts
extern int DO_Control;
extern int DO_dEdx;
extern int DO_Comparison;
} // namespace Reconstruction

#endif