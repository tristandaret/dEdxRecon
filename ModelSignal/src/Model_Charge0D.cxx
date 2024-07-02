#include "Model_Charge0D.h"

Model_Charge0D::Model_Charge0D()
:Model_ChargeI()
{
	double qe	= 1.602176565E-19;
	m_Qprim = 1. * qe;
	
	SetSecondaries();

	m_DoIons	= 0	;
	m_Tion		= 400.E-9;
	m_RTrue	= 50. * 1E-9 / ( 1E-3 * 1E-3);
	m_RFactor	= 1.	;

}

Model_Charge0D::~Model_Charge0D()
{
 int VerboseDelete = 0;
 if (VerboseDelete==1)
 std::cout 
		<< " Model_Charge0D Destructor "
		<< std::endl;
}

double Model_Charge0D::Get_Qprim () const { return m_Qprim; }
void Model_Charge0D::Set_Qprim (const double& Qprim ) { m_Qprim = Qprim; SetSecondaries(); }

void Model_Charge0D::Set_Ion(
			const int&	DoIons,
			const double& Tion,
			const double& RTrue,
			const double& RFactor	
){
	m_DoIons	= DoIons;
	m_Tion	= Tion;
	m_RTrue	= RTrue;
	m_RFactor = RFactor;
}

void Model_Charge0D::WriteOut() const 
{
	std::cout << "StartModel_Charge0D" << std::endl;
	std::cout << " m_RC; " << std::setw(13) << std::setprecision(6)	<< m_RC	<< std::endl;
	std::cout << " m_Width; " << std::setw(13) << std::setprecision(6)	<< m_Width	<< std::endl;
	std::cout << " m_Qprim; " << std::setw(30) << std::setprecision(25) << m_Qprim << std::endl;
	std::cout << " m_Gain; " << std::setw(30) << std::setprecision(25) << m_Gain	<< std::endl;
	std::cout << "EndModel_Charge0D" << std::endl;
	
}

void Model_Charge0D::SetSecondaries()
{
	m_NormQPad =	m_Qprim * m_Gain / 4.;
	m_2RCinv	= 2./m_RC; 
	m_Width2	= m_Width*m_Width;

}

double Model_Charge0D::Get_Qpad(
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
	
	if (m_DoIons==1){
	double Sigma2 = 0.;
	if (LocalTime <	m_Tion ) Sigma2 =	LocalTime		* (2./(m_RTrue*m_RFactor))									+ m_Width2;
	if (LocalTime >= m_Tion ) Sigma2 = (LocalTime-m_Tion) * (2./ m_RTrue			) + m_Tion * (2./(m_RTrue*m_RFactor)) + m_Width2;
	Sigma = std::sqrt(Sigma2);
	}
	
	
	double Inv_SQRT2_Sigma = 1./(std::sqrt(2.)*Sigma );

	double ARG=(YH-YTrue) * Inv_SQRT2_Sigma; double RESYH = std::erf(ARG);
		ARG=(YL-YTrue) * Inv_SQRT2_Sigma; double RESYL = std::erf(ARG);
	
		ARG=(XH-XTrue) * Inv_SQRT2_Sigma; double RESXH = std::erf(ARG);
		ARG=(XL-XTrue) * Inv_SQRT2_Sigma; double RESXL = std::erf(ARG);
	
	ToBeReturned =	( RESXH - RESXL ) * ( RESYH - RESYL ) * m_NormQPad;
	
	return ToBeReturned;
}
