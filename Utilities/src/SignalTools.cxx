#include "SignalTools.h"
#include "Misc_Functions.h"

/* SIGNAL MODELISATION*/

// Dirac Pulse Response function
TH1F *ETF(std::string name, const float &tmin, float const &tmax, const float &t_0,
          const int &nbins, const int &iC, const float &PT, const float &TB)
{
   float Q = 2. / 3.;
   float ws = 2 * TB / PT;
   // if(PT == 200){ws = 0.4172; Q = 0.665; }
   // if(PT == 412){ws = 0.19518; Q = 0.6368; }
   name = name + "_" + std::to_string(iC);
   TH1F *h1f_ETF = new TH1F(
      name.c_str(), "Electronics pulse response function;time bin;signal (normalized)",
      nbins, tmin, tmax);
   float step = (tmax - tmin) / nbins;
   float ETFmax = -999;
   for (int t = tmin; t < tmax; t += step) {
      float arg = ws * (t - t_0) / 2 * sqrt(4 - 1 / pow(Q, 2));
      float ETF =
         Heaviside(t, t_0) * (exp(-ws * (t - t_0)) +
                              exp(-(ws * (t - t_0) / (2 * Q))) *
                                 (sqrt((2 * Q - 1) / (2 * Q + 1)) * sin(arg) - cos(arg)));
      h1f_ETF->SetBinContent((t - tmin) / step, ETF);
      if (ETF > ETFmax)
         ETFmax = ETF;
   }
   h1f_ETF->Scale(1 / ETFmax);
   return h1f_ETF;
}