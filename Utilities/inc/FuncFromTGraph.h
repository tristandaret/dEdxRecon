#ifndef FuncFromTGraph_H
#define FuncFromTGraph_H

#/***************************************************************************
# * File: FuncFromTGraph.h
# * Project: dEdxRecon
# *
# * Brief: Utility to create analytic function wrappers from ROOT TGraph/TGraphErrors
# *        objects. Useful for interpolation and building TF1-like callables from
# *        discrete points.
# *
# * Contents: class/function declarations for converting TGraph to callable forms.
# *
# * Notes: Implementation in FuncFromTGraph.cxx.
# ***************************************************************************/
#ifndef FuncFromTGraph_H
#define FuncFromTGraph_H
#include "Misc.h"

#include "TGraph.h"

//
/////////////////////////////////////////////////////////////
class FuncFromTGraph {
public:
  /** Constructor */
  FuncFromTGraph(TGraph *pTGraph);
  virtual ~FuncFromTGraph();

  double Get_Y(const double &vX);

  void Dump();

  double operator()(double *x, double *par);

private:
  TGraph *p_TGraph;
  double m_StepX;
  double m_X_Lowest;
  double m_X_Highest;
};

#endif
