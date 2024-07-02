#include "Model_Electronics.h"

Model_Electronics::Model_Electronics()
{
	m_QValue	= 0.6368	;
	m_ws		= 0.19518/40.E-9;
	m_Amplitude = 1.		;
	
	m_AcurMax = 1.;
	
	SetSecondaries();
	GetExtremum();
	SetSecondaries();

//	std::cout 
//	<< " m_AcurMax " << std::setw(16) << std::setprecision(9) << m_AcurMax 
//	<< " m_TcurMax " << std::setw(16) << std::setprecision(3) << m_TcurMax*1.E9 
//	<< std::endl;
	
}

Model_Electronics::~Model_Electronics()
{
 int VerboseDelete = 0;
 if (VerboseDelete==1)
 std::cout 
		<< " Model_Electronics Destructor "
		<< std::endl;
}

double Model_Electronics::Get_QValue	() const { return m_QValue; }
double Model_Electronics::Get_ws		() const { return m_ws	; }
double Model_Electronics::Get_Amplitude() const { return m_Amplitude; }

void Model_Electronics::Set_TimeShape	(const double& QValue ,const double& ws ) 
{ 
	m_QValue	= QValue; 
	m_ws		= ws	;
	SetSecondaries(); 
	GetExtremum();
	SetSecondaries(); 
}

void Model_Electronics::Set_Amplitude(const double& Amplitude ) { m_Amplitude = Amplitude; SetSecondaries(); }

void Model_Electronics::WriteOut() const 
{
	std::cout << "StartModel_Electronics" << std::endl;
	std::cout << " m_QValue; " << std::setw(13) << std::setprecision(6) << m_QValue	<< std::endl;
	std::cout << " m_ws	; " << std::setw(16) << std::setprecision(3) << m_ws		<< std::endl;
	std::cout << " m_Amplitude; " << std::setw(13) << std::setprecision(6) << m_Amplitude << std::endl;
	std::cout << "EndModel_Electronics" << std::endl;
	
}

void Model_Electronics::SetSecondaries()
{
	m_Qfactor1	= std::sqrt( (2.*m_QValue-1.)/(2.*m_QValue+1.) );
	m_Qfactor2	= std::sqrt( 4. - 1./(m_QValue*m_QValue) );

	double ChargeRange = 120.E-15;
	m_Prefactor = m_Amplitude * (4096./ChargeRange) * 1./(m_AcurMax);

}

double Model_Electronics::Response_Base(const double& Time) const 
{
	double ToBeReturned = 0.;

	if (Time<=0) return ToBeReturned;
	
	double argexp	= -m_ws*Time/(2.*m_QValue);
	double argtrigo =	m_ws*Time*0.5*m_Qfactor2;
	
	double TheSin = std::sin(argtrigo);
	double TheCos = std::cos(argtrigo);

	ToBeReturned = 
	std::exp(-m_ws*Time) 
	+
	std::exp(argexp) *( m_Qfactor1 * TheSin - TheCos );
	
	return ToBeReturned;
}

double Model_Electronics::Derive_Response_Base(const double& Time) const 
{
	double ToBeReturned = 0.;

	if (Time<=0) return ToBeReturned;
	
	double argexp	= -(m_ws/(2.*m_QValue)) *Time;
	double argtrigo =	m_Qfactor2*m_ws*0.5	*Time;
	
	double TheSin = std::sin(argtrigo);
	double TheCos = std::cos(argtrigo);
	
	ToBeReturned = 
	-m_ws * std::exp(-m_ws*Time) 
	+ 
	std::exp(argexp)	
	*(
		(-m_ws/(2.*m_QValue)) * ( m_Qfactor1 * TheSin - TheCos ) 
	+
		m_Qfactor2*m_ws*0.5	* ( m_Qfactor1 * TheCos + TheSin ) 
	);

	ToBeReturned *= m_Prefactor;
	
	return ToBeReturned;
}

void Model_Electronics::GetExtremum()
{
	double Tmin = 0.;
	int iTem_Max = 1001;
	double Tmax = double(iTem_Max-1)*1.E-9;
	
	m_AcurMax = 0.;
	m_TcurMax = 0.;
	for (int iTem = 0; iTem< iTem_Max; iTem++){
	double Tcur = Tmin + double(iTem)*(Tmax-Tmin)/double(iTem_Max-1);
	double Acur = Response_Base(Tcur);
	if (iTem==0){
		m_AcurMax = Acur;
		m_TcurMax = Tcur;
	}
	if (m_AcurMax<Acur){
		m_AcurMax = Acur;
		m_TcurMax = Tcur;
	}
	}
 
}
