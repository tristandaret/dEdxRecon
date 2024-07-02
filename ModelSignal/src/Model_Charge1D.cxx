#include "Model_Charge1D.h"

Model_Charge1D::Model_Charge1D()
:Model_ChargeI()
{
//	std::cout << "Model_Charge1D::Model_Charge1D " << std::endl;
	
	double qe	= 1.602176565E-19;
	double NberPrimary_cm = 100.;
	m_Lambda = NberPrimary_cm * 100. * qe;
	
	SetSecondaries();

//	std::cout << "->m_RC	" << m_RC	<< std::endl;
//	std::cout << "->m_Width	" << m_Width	<< std::endl;
//	std::cout << "->m_Gain	" << m_Gain	<< std::endl;
//	std::cout << "->m_Lambda " << m_Lambda << std::endl;
}

Model_Charge1D::~Model_Charge1D()
{
 int VerboseDelete = 0;
 if (VerboseDelete==1)
 std::cout 
		<< " Model_Charge1D Destructor "
		<< std::endl;
}

double Model_Charge1D::Get_Lambda () const { return m_Lambda; }
void Model_Charge1D::Set_Lambda (const double& Lambda ) { m_Lambda = Lambda; SetSecondaries(); }

void Model_Charge1D::WriteOut() const 
{
	std::cout << "StartModel_Charge1D" << std::endl;
	std::cout << " m_RC; " << std::setw(13) << std::setprecision(6)	<< m_RC	<< std::endl;
	std::cout << " m_Width; " << std::setw(13) << std::setprecision(6)	<< m_Width	<< std::endl;
	std::cout << " m_Lambda; " << std::setw(30) << std::setprecision(25) << m_Lambda << std::endl;
	std::cout << " m_Gain; " << std::setw(30) << std::setprecision(6)	<< m_Gain	<< std::endl;
	std::cout << "EndModel_Charge1D" << std::endl;
	
}

void Model_Charge1D::SetSecondaries()
{
	m_NormQPad =	m_Lambda * m_Gain / 2.;
	m_2RCinv	= 2./m_RC; 
	m_Width2	= m_Width*m_Width;
}

double Model_Charge1D::Get_Qpad(
					const double& Time ,
					const double& Time0,
					const double& XTrue,
					const double& YTrue,
					const double& XL,
					const double& XH,
					const double& YL,
					const double& YH
) const {
	double ToBeReturned = 0.;

	double LocalTime = Time - Time0;
	if (LocalTime<=0) return ToBeReturned;
	
	double Sigma = std::sqrt ( LocalTime*m_2RCinv + m_Width2 );
	double Inv_SQRT2_Sigma = 1./(std::sqrt(2.)*Sigma );

	double ARG=(YH-YTrue) * Inv_SQRT2_Sigma; double RESYH = std::erf(ARG);
		ARG=(YL-YTrue) * Inv_SQRT2_Sigma; double RESYL = std::erf(ARG);
	
	ToBeReturned =	( RESYH - RESYL ) *	m_NormQPad * (XH-XL);
	
	return ToBeReturned;
}
