#ifndef Model_ChargeI_H
#define Model_ChargeI_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class Model_ChargeI {
public:
   /** Constructor */
   Model_ChargeI();
   virtual ~Model_ChargeI();

   // Get charge spreading parameters
   double Get_RC() const;
   double Get_Width() const;
   double Get_Gain() const;

   // Set charge spreading parameters
   void Set_RC(const double &RC);
   void Set_Width(const double &Width);
   void Set_Gain(const double &Gain);

   // Get width from Drift Distance
   void Set_WidthFromDriftDistance(const double &DriftDistance_cm);

   // Get Charge
   virtual double Get_Qpad(const double &Time, const double &Time0, const double &XTrue,
                           const double &YTrue, const double &XL, const double &XH,
                           const double &YL, const double &YH) const = 0;

   // Big Dump
   virtual void WriteOut() const = 0;

protected:
   void WidthFromDriftDistance(const double &DriftDistance_cm);
   virtual void SetSecondaries() = 0;

   double m_RC;
   double m_Width;
   double m_Gain;

   double m_2RCinv;
   double m_Width2;
   double m_NormQPad;
};

#endif
