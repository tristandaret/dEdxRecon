#/***************************************************************************
 * File: Model_Electronics.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of an electronics response model. Provides methods to
 *        configure impulse response parameters and evaluate the electronics
 *        time-shape and its derivative. Used to model shaping and readout
 *        chain effects applied to detector signals.
 *
 * Contents: class Model_Electronics with response evaluation methods.
 *
 * Notes: Implementation in Model_Electronics.cxx.
 ***************************************************************************/

#ifndef Model_Electronics_H
#define Model_Electronics_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class Model_Electronics {
public:
  /** Constructor */
  Model_Electronics();
  virtual ~Model_Electronics();

  // Get electronics parameters
  double Get_QValue() const;
  double Get_ws() const;
  double Get_Amplitude() const;

  // Set electronics parameters
  void Set_TimeShape(const double &QValue, const double &ws);
  void Set_Amplitude(const double &Amplitude);

  // Impulse response
  double Response_Base(const double &Time) const;

  // Derivative of Impulse response
  double Derive_Response_Base(const double &Time) const;

  // Big Dump
  void WriteOut() const;

private:
private:
  void SetSecondaries();
  void GetExtremum();

  double m_QValue;
  double m_ws;
  double m_Amplitude;

  double m_Qfactor1;
  double m_Qfactor2;
  double m_Prefactor;

  double m_AcurMax;
  double m_TcurMax;
};

#endif
