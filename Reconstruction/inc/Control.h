#ifndef CONTROL_H
#define CONTROL_H

#include "Misc.h"

#include "Uploader.h"
#include "GiveMe_Uploader.h"

void Control(const std::string &OutDir, std::string const &Tag, std::string const &Comment,
             std::string const &EventFile, std::string const &SelectionSet, Uploader *pUploader, int const &NbrOfMod,
             int const &Data_to_Use, int const &PT, int const &TB, const std::string &particle);

#endif // CONTROL_H