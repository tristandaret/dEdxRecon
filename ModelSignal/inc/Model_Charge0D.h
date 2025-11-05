#/***************************************************************************
 * File: Model_Charge0D.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of a 0-dimensional charge model used to estimate the
 *        integrated charge produced by an ionising particle in a pad or
 *        collection region. Intended for simplified or analytic studies.
 *
 * Contents: class Model_Charge0D : public Model_ChargeI
 *
 * Notes: Implementation in Model_Charge0D.cxx.
 ***************************************************************************/

#ifndef Model_Charge0D_H
#define Model_Charge0D_H

#include "Misc.h"
#include "Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class Model_Charge0D : public Model_ChargeI {
public:
  /** Constructor */
  Model_Charge0D();
  virtual ~Model_Charge0D();

  // Get charge spreading parameters
  double Get_Qprim() const;

  // Set charge spreading parameters
  void Set_Qprim(const double &Qprim);

  // Get Charge
  double Get_Qpad(const double &Time, const double &Time0, const double &XTrue,
                  const double &YTrue, const double &XL, const double &XH,
                  const double &YL, const double &YH) const;

  // Big Dump
  void WriteOut() const;

  // Ion Effect: Set charge spreading parameters
  void Set_Ion(const int &DoIons, const double &Tion, const double &RTrue,
               const double &RFactor);

protected:
  void SetSecondaries();

  double m_Qprim;

  int m_DoIons;
  double m_Tion;
  double m_RTrue;
  double m_RFactor;
};

#endif
