#ifndef MISC_SIGNALTOOLS_H
#define MISC_SIGNALTOOLS_H

#include "TH1F.h"

/* GENERAL MATH*/
// Heaviside
float Heaviside(const float &t, const float &t_0);

/* SIGNAL MODELISATION*/
// Electronics pulse response function
TH1F *DPR(std::string name, const float &tmin, float const &tmax, const float &t_0,
          const int &nbins, const int &iC, const float &PT, const float &TB);

#endif