/***************************************************************************
 * File: Displays.h
 * Project: dEdxRecon
 *
 * Brief: High-level display utilities and wrappers around plotting code.
 *        Declares helpers used by the GUI/plotting components of the
 *        reconstruction pipeline.
 *
 * Contents: class/function declarations to manage drawing and visual
 *           presentation of results.
 *
 * Notes: Implementation in Displays.cxx.
 ***************************************************************************/

#ifndef DISPLAYS_H
#define DISPLAYS_H
#include "Misc.h"

#include "Event.h"
#include "Module.h"
#include "Pad.h"
#include "Sample.h"

#include "TH1F.h"
#include "TH2D.h"

//------------------------Event Display
// Output event display of an event	with tagging string TAG, placed in
// OUTDIR dir
void DrawOut_EventDisplay(Event *pEvent, const int &ModuleNber,
                          const std::string &OUTDIR, const std::string &TAG);
void DrawOut_EventDisplay(Module *pModule, const std::string &OUTDIR,
                          const std::string &TAG, const std::string &type,
                          const double &parabola, const double &slope,
                          const double &intercept);

// Get event display histo of an event with tagging string TAG
TH2D *GiveMe_EvtDisplay(Event *pEvent, const int &ModuleNber,
                        const std::string &TAG);
TH2D *GiveMe_EvtDisplay(Module *pModule, const std::string &TAG);

// Get Timing Event display
TH2D *GiveMe_TimeDisplay(Module *pModule, const std::string &TAG);

//------------------------WF pad Display
// Output waveform of a pad	with tagging string TAG, placed in OUTDIR dir
void DrawOut_WaveFormDisplay(Pad *pPad, const std::string &OUTDIR,
                             const std::string &TAG);

// Get waveform histo of pad with tagging string TAG
TH1F *GiveMe_WaveFormDisplay(Pad *pPad, const std::string &TAG);

//------------------------WFs cluster Display
// TD: Draw Waveforms of all Pads in a Cluster
void DrawOut_ClusterWFDisplay(Cluster *pCluster, const std::string &OUTDIR,
                              const std::string &TAG);
void DrawOut_ClusterWFDisplay(Cluster *pCluster, const std::string &OUTDIR,
                              const std::string &TAG, const int &Option,
                              const int &PT, const int &TB);

// Draw Event waveform
void DrawOut_GWF(Event *pEvent, const int &ModuleNber,
                 const std::string &OUTDIR, const std::string &TAG,
                 const int &PT, const int &TB, const float &phi_rad);

// Redo Cluster display
void NewClusterDisplay(Event *pEvent, const std::string &OUTDIR,
                       const std::string &TAG, const int &PT, const int &TB);

void NewClusterDisplayMinimal(Event *pEvent, const std::string &OUTDIR,
                              const std::string &TAG);