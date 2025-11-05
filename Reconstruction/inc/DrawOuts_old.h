#/***************************************************************************
 * File: DrawOuts_old.h
 * Project: dEdxRecon
 *
 * Brief: Legacy/older drawout interfaces kept for backward compatibility.
 *        Declares older drawing utilities that may be used by legacy
 *        workflows or for regression comparisons.
 *
 * Contents: legacy drawing class/function declarations.
 *
 * Notes: Prefer the newer DrawOuts API; this header is retained for
 *        compatibility.
 ***************************************************************************/

#ifndef DRAWOUTSOLD_H
#define DRAWOUTSOLD_H

#include "Misc.h"
#include "SetStyle.h"
#include "dEdx.h"

// TH2 comparisons of the different methods
void DrawOut_Methods(const std::string &OutDir, const std::string &Tag,
                     const std::string &Comment, const int &nMod);

// Draw resolution of 2 different procedures
void DrawOut_Versions(const std::string &inputDir, const std::string &Method,
                      const std::string &Comment1, const std::string &Comment2);

// Draw separation power histograms for all methods and particles
void DrawOut_Separation(const std::string &inputDir,
                        const std::string &Comment);

// Draw separation power histograms for all methods and particles only for WF &
// XP
void DrawOut_Separation_Reduced(const std::string &inputDir,
                                const std::string &Comment, std::string Energy);

// Draw all scans together
void DrawOut_Scans(const std::string &inputDir, const std::string &Comment,
                   const std::string &WFversion);

// Draw resolution as function of Z scan
void DrawOut_Zscan(const std::string &inputDir, const std::string &Comment,
                   const int &PT);

// Draw resolution as function of Y scan
void DrawOut_Yscan(const std::string &inputDir, const std::string &Comment);

// Draw resolution as function of phi scan
void DrawOut_Phiscan(const std::string &inputDir, const std::string &Comment,
                     const std::string &WFversion, const std::string &zdrift);

// Draw resolution as function of theta scan
void DrawOut_Thetascan(const std::string &inputDir, const std::string &Comment);

// Transverse diffusion coefficient effect on Z scan
void DrawOut_Zscan_PT(const std::string &inputDir, const std::string &Comment);

// Draw resolution as function of phi scan for different drift distances
void DrawOut_Phiscan_Z(const std::string &inputDir, const std::string &Comment,
                       const std::string &WFversion);

// Draw Energy scan
void DrawOut_Escan(const std::string &inputDir, const std::string &Comment);

// DrawOut dE/dx systematics with Z scan
void DrawOut_Systematics(const std::string &inputDir,
                         const std::string &Comment, const std::string &scan);

#endif