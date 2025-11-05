#/***************************************************************************
 * File: Control.h
 * Project: dEdxRecon
 *
 * Brief: Declaration for Control(), the high-level orchestration function
 *        that sets up runs, parsing arguments and invoking appropriate
 *        reconstruction/analysis steps.
 *
 * Contents: Control(...) function declaration used by top-level apps.
 *
 * Notes: Implementation in Control.cxx.
 ***************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include "Misc.h"

#include "GiveMe_Uploader.h"
#include "Uploader.h"

void Control(const std::string &OutDir, std::string const &Tag,
             std::string const &Comment, std::string const &EventFile,
             std::string const &SelectionSet, Uploader *pUploader,
             int const &NbrOfMod, int const &Data_to_Use, int const &PT,
             int const &TB, const std::string &particle);

#endif // CONTROL_H