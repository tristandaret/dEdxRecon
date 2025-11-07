#/**
 * @file Model_Charge1D.h
 * @brief 1D charge model declaration (inherits from Model_ChargeI).
 *
 * Declares Model_Charge1D which models charge spreading along a single axis
 * (e.g. time/drift). Concrete Get_Qpad and related helpers are provided in
 * Model_Charge1D.cxx.
 *
 * @details
 * The implementation computes pad-integrated charge for longitudinal
 * spreading-dominated scenarios and exposes helpers to configure lambda and
 * normalization used in simulations and fits.
 */
#ifndef Model_Charge1D_H
#define Model_Charge1D_H

#include "Misc.h"
#include "Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class Model_Charge1D : public Model_ChargeI {
public:
  /** Constructor */
  Model_Charge1D();
  virtual ~Model_Charge1D();

  // Get charge spreading parameters
  double Get_Lambda() const;

  // Set charge spreading parameters
  void Set_Lambda(const double &Lambda);

  // Get Charge
  double Get_Qpad(const double &Time, const double &Time0, const double &XTrue,
                  const double &YTrue, const double &XL, const double &XH,
                  const double &YL, const double &YH) const;

  // Big Dump
  void WriteOut() const;

protected:
  void SetSecondaries();

  double m_Lambda;
};

#endif
