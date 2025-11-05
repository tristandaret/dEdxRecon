#/**
 * @file FitOutput.h
 * @brief Containers to hold fit results from cluster/track fitters.
 *
 * Declares FitOutput which stores parameter values, uncertainties and
 * auxiliary quality flags produced by fitting routines.
 */
#ifndef FitOutput_H
#define FitOutput_H

#include "Misc.h"

#include "TCanvas.h"
#include "TMatrixD.h"
#include "TVirtualFitter.h"

/////////////////////////////////////////////////////////////
class FitOutput {
public:
  FitOutput();
  virtual ~FitOutput();

  FitOutput(const FitOutput &ToBeCopied);
  FitOutput &operator=(const FitOutput &ToBeCopied);

public:
  ///////////////////////////////////

  void Reset();
  void Set(const int &NberOfModelParameters);

  void SetResults(TVirtualFitter *pTVirtualFitter);
  void SetResults(const std::vector<std::string> &V_PARname,
                  const std::vector<double> &V_PAR,
                  const std::vector<double> &V_ePAR);

  void PrintFitOutput();
  void PrintFitOutputInCanvas(double Xstart, double Ystart, double Step);

public:
  ///////////////////////////////////

  std::string m_NameOfTheModel;

  int m_NberOfModelParameters;

  int m_NberOfDataPoints;

  double *p_par;          //!< Parameters
  std::string *p_parName; //!< Name
  double *p_eparplus;     //!< Errors
  double *p_eparminus;    //!< Errors
  double *p_eparparab;    //!< Errors
  double *p_CovMatrix;    //!< Cov matrix
  double m_MinnLL;        //!< Likelihood

  std::string m_Description;
};

#endif
