#/***************************************************************************
 * File: Monitoring.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of monitoring utilities used to run analysis and
 *        produce diagnostic summaries (histograms, performance numbers,
 *        etc.). Contains the top-level entry used by the Analysis app.
 *
 * Contents: function declarations for monitoring workflows.
 *
 * Notes: Implementation in Monitoring.cxx.
 ***************************************************************************/

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