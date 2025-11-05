#/***************************************************************************
 * File: ReconTools.h
 * Project: dEdxRecon
 *
 * Brief: Miscellaneous reconstruction helper functions and small tools.
 *        Contains utility routines used across reconstruction, plotting and
 *        fitting code.
 *
 * Contents: small helper functions and thin adapters for common tasks.
 *
 * Notes: Implementation in ReconTools.cxx.
 ***************************************************************************/

#ifndef Reconstruction_TOOLS_H
#define Reconstruction_TOOLS_H

#include <vector>

#include "TF1.h"
#include "TH1F.h"

#include "Misc.h"
#include "Util.h"

#include "Module.h"
#include "Pad.h"
#include "Track.h"

#include "GiveMe_Uploader.h"
#include "Selector.h"
#include "Uploader.h"

/* Python's numpy equivalent of linspace
 * -----------------------------------------------
 */

bool FourModulesInLine(const std::vector<int> &vec);

namespace Reconstruction {
void WFCorrection(const std::string &OutCorr);
}

/* Call the correction function of WF method
 * ----------------------------------------------*/
TF1 *corr_func(const std::string &EventFile, const std::string &Tag,
               const int &correctWF);

/* Initialize selection stages
 * -----------------------------------------------------------*/
void Init_selection(const std::string &SelectionSet, Selector &aSelector,
                    const std::string &Tag, Uploader *pUploader,
                    const int &NbrOfMod, const int &Data_to_Use);

/* FILE HANDLING */
// Read CSV file
std::vector<std::vector<float>> readCSV(std::string filename);

// Determine the time cut
std::vector<int> ComputeCutStage3_Cut(Uploader *pUploader, const int &NbrOfMod,
                                      const int &Data_to_Use);

// Function to search for a word in a CSV file
bool GetStage3Cut_CSV(const std::string &filename,
                      const std::string &targetWord, int &value1, int &value2);

// Function to update a CSV file
void SetStage3Cut_CSV(const std::string &filename,
                      const std::string &targetWord, int value1, int value2);

/* GENERAL PHYSICS */
// Bethe-Bloch function
TF1 *BetheBloch(const float &Emin, const float &Emax, const double &m,
                const std::string &particle);

// Bethe-Bloch relativistic for positrons with Bhabha scattering
TF1 *BetheBlochBhabha(const float &Pmin, const float &Pmax, const double &m,
                      const std::string &particle);

// Bethe-Bloch with experimental parametrisation
TF1 *BetheBlochExp(const float &Pmin, const float &Pmax, const double &M,
                   const std::string &particle);

/* GENERAL MATH*/

// Fit with 2 gaussians
TF1 *Fit2Gauss(TH1F *h1F);
TF1 *Fit2Gauss(TH1F *h1F, const float &x1min, const float &x1max,
               const float &x2min, const float &x2max);

/* SPECIFIC MATH*/
// impact parameter d (in m) & track angle phi (in degrees) computed locally at
// the level of the pad
void local_params(Pad *pPad, const Track *pTrack, float &d, float &dd,
                  float &phi, float &trk_len_pad);

// Track length
float trk_len(Module *pModule, const Track *pTrack);

// From TH1 get TGraph
TGraph *hist_to_graph(TH1 *h1);

// From TH1 get swapped TGraph (transpose x and y)
TGraph *Swapped_graph(TH1 *h1);

/* ROOT */

// Draw TH2
void DrawTH2(const std::string &OutDir, TH2 *h2);
#endif