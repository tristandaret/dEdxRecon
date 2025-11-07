#include "Model_ChargeI.h"

Model_ChargeI::Model_ChargeI() {
  //	std::cout << "Model_ChargeI::Model_ChargeI " << std::endl;

  m_RC = 60. * 1E-9 / (1E-3 * 1E-3);

  double DriftDistance_cm = 7.5;
  WidthFromDriftDistance(DriftDistance_cm);

  m_Gain = 1.E3;

  //	std::cout << "->m_RC	" << m_RC	<< std::endl;
  //	std::cout << "->m_Width	" << m_Width	<< std::endl;
  //	std::cout << "->m_Gain	" << m_Gain	<< std::endl;
}

Model_ChargeI::~Model_ChargeI() {
  int VerboseDelete = 0;
  if (VerboseDelete == 1)
    std::cout << " Model_ChargeI Destructor " << std::endl;
}

double Model_ChargeI::Get_RC() const { return m_RC; }
double Model_ChargeI::Get_Width() const { return m_Width; }
double Model_ChargeI::Get_Gain() const { return m_Gain; }

void Model_ChargeI::Set_RC(const double &RC) {
  m_RC = RC;
  SetSecondaries();
}
void Model_ChargeI::Set_Width(const double &Width) {
  m_Width = Width;
  SetSecondaries();
}
void Model_ChargeI::Set_Gain(const double &Gain) {
  m_Gain = Gain;
  SetSecondaries();
}

void Model_ChargeI::Set_WidthFromDriftDistance(const double &DriftDistance_cm) {
  WidthFromDriftDistance(DriftDistance_cm);
  SetSecondaries();
}

void Model_ChargeI::WidthFromDriftDistance(const double &DriftDistance_cm) {
  double D_um_sqrt_cm = 237.;
  m_Width = D_um_sqrt_cm * std::sqrt(DriftDistance_cm) * 1.E-6;
}
