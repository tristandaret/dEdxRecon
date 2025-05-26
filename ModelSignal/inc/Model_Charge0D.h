#ifndef Model_Charge0D_H
#define Model_Charge0D_H

#include "Model_ChargeI.h"
#include "Misc.h"

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
