#include "Pad.h"
#include "ParabolaFunction.h"
#include "ParabolaFunctionNG.h"

#include "TH1F.h"
#include "TF1.h"

Pad::Pad(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI,
	std::string	PadName,
	const int&	EventNber,
	const int&	EntryNber,
	const int&	ModuleNber, 
	const int&	iX,
	const int&	iY									
){
//Initialize Models
	Ini_Models(
			pModel_ReadOutGeometry,
			pModel_Electronics,
			pModel_ChargeI 
			);

//	
	m_PadName	= PadName;
	m_EventNber	= EventNber;
	m_EntryNber	= EntryNber;
	m_ModuleNber = ModuleNber;
	m_iX		= iX;
	m_iY		= iY;

	double Xpad_H = 0.;
	double Xpad_L = 0.;
	double Ypad_H = 0.;
	double Ypad_L = 0.;
	p_Model_ReadOutGeometry->GetPadEdges(m_iX, m_iY, ModuleNber, Xpad_L, Xpad_H , Ypad_L , Ypad_H);
	
	SetEdges( Xpad_L, Xpad_H , Ypad_L , Ypad_H);
	
	m_Time0	= 0;
	m_XTrue	= 0.;
	m_YTrue	= 0.;
	
	v_ADC	.clear();
	v_iTimeb.clear();

	m_AMax_FromSet = -1;
	m_TMax_FromSet = -1;

	m_AMax_WF = -1;
	m_TMax_WF = -1;

	m_AMax_WF_01 = -1;
	m_TMax_WF_01 = -1;

	m_AMax_FIT = -1;
	m_TMax_FIT = -1;
	
	m_AMax_True = -1;
	m_TMax_True = -1;

}

Pad::Pad(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry ,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI,
	std::string	PadName,
	const int&	EventNber,
	const int&	EntryNber,
	const int&	ModuleNber, 
	const int&	iX,
	const int&	iY,
	const double& XL,
	const double& XH,
	const double& YL,
	const double& YH
){
//Initialize Models
	Ini_Models(
			pModel_ReadOutGeometry,
			pModel_Electronics,
			pModel_ChargeI 
			);

//
	m_PadName	= PadName;
	m_EventNber	= EventNber;
	m_EntryNber	= EntryNber;
	m_ModuleNber = ModuleNber;
	m_iX		= iX;
	m_iY		= iY;

	SetEdges( XL, XH , YL , YH);
	
	v_ADC	.clear();
	v_iTimeb.clear();


	m_AMax_WF = -1;
	m_TMax_WF = -1;

	m_AMax_WF_01 = -1;
	m_TMax_WF_01 = -1;

	m_AMax_FIT = -1;
	m_TMax_FIT = -1;

}

Pad::~Pad()
{
	int VerboseDelete = 0;
	if (VerboseDelete==1)
	std::cout 
		<< " Pad Destructor "
		<< " m_iX " << m_iX
		<< " m_iY " << m_iY
		<< std::endl;
	
	v_ADC	.clear();
	v_iTimeb.clear();
 
//
	v_ADC.clear(); std::vector<short int>().swap( v_ADC	);
	v_iTimeb.clear(); std::vector<short int>().swap( v_iTimeb );
//
}

const Model_ReadOutGeometry*	Pad::Get_Model_ReadOutGeometry() const { return p_Model_ReadOutGeometry;}
const Model_Electronics	*	Pad::Get_Model_Electronics	() const { return p_Model_Electronics;}
const Model_ChargeI		*	Pad::Get_Model_ChargeI		() const { return p_Model_ChargeI	;}

std::string Pad::Get_PadName	() const { return m_PadName; }
int		Pad::Get_iX		() const { return m_iX	; }
int		Pad::Get_iY		() const { return m_iY	; }
int		Pad::Get_ModuleNber () const { return m_ModuleNber; }

double Pad::Get_XPad()		const { return ( m_XH + m_XL )/2.; }
double Pad::Get_YPad()		const { return ( m_YH + m_YL )/2.; }
double Pad::Get_LX()			const { return (m_XH - m_XL); }
double Pad::Get_LY()			const { return (m_YH - m_YL); }

double Pad::Get_XL()			const { return m_XL	; }
double Pad::Get_XH()			const { return m_XH	; }
double Pad::Get_YL()			const { return m_YL	; }
double Pad::Get_YH()			const { return m_YH	; }

int	Pad::Get_EventNber () const { return m_EventNber; }

int	Pad::Get_EntryNber () const { return m_EntryNber; }

int Pad::IsValid() const { return m_IsValid; }
void Pad::Validate() { m_IsValid = 1;}
void Pad::Invalidate() { m_IsValid = 0;}

void	Pad::Set_Data_2Use(const int& iOpt)
{ 
	if (iOpt==0){
	m_AMax = m_AMax_FromSet;
	m_TMax = m_TMax_FromSet;
	}
	if (iOpt==1){
	m_AMax = m_AMax_WF;
	m_TMax = m_TMax_WF;
	}
	if (iOpt==2){
	m_AMax = m_AMax_WF_01;
	m_TMax = m_TMax_WF_01;
	}
	if (iOpt==3){
	m_AMax = m_AMax_FIT;
	m_TMax = m_TMax_FIT;
	}
	if ( iOpt != 0 && iOpt != 1 && iOpt != 2	&& iOpt != 3){
	std::cout << " Pad::Set_Data_2Use: Unknown iOpt = " << iOpt << std::endl;
	abort();
	}
}

double Pad::Get_AMax() const { return	m_AMax; }
double Pad::Get_TMax() const { return	m_TMax; }
void	Pad::Set_AMax(const double& AMax){ m_AMax = AMax; m_AMax_FromSet = AMax;}
void	Pad::Set_TMax(const double& TMax){ m_TMax = TMax; m_TMax_FromSet = TMax;}

void Pad::SetEdges(
	const double& XL ,
	const double& XH ,
	const double& YL ,
	const double& YH
){

	m_XL = XL;
	m_XH = XH;
	m_YL = YL;
	m_YH = YH;
	
	double m_XPad = ( m_XH + m_XL )/2.;
	double m_YPad = ( m_YH + m_YL )/2.;
	double m_LX = m_XH - m_XL;
	double m_LY = m_YH - m_YL;

	if ( m_LX <= 0. ||	m_LY <= 0.){
	std::cout << std::endl;
	std::cout << "Pad::SetEdges" << std::endl;
	std::cout << "	m_LX <= 0. ||	m_LY <= 0. " << std::endl;
	std::cout << "	m_XPad " << std::setw(13) << std::setprecision(6) << m_XPad << std::endl;
	std::cout << "	m_YPad " << std::setw(13) << std::setprecision(6) << m_YPad << std::endl;
	std::cout << "	m_LX " << std::setw(13) << std::setprecision(6) << m_LX << std::endl;
	std::cout << "	m_LY " << std::setw(13) << std::setprecision(6) << m_LY << std::endl;
	std::cout << "	m_XL " << std::setw(13) << std::setprecision(6) << m_XL << std::endl;
	std::cout << "	m_XH " << std::setw(13) << std::setprecision(6) << m_XH << std::endl;
	std::cout << "	m_YL " << std::setw(13) << std::setprecision(6) << m_YL << std::endl;
	std::cout << "	m_YH " << std::setw(13) << std::setprecision(6) << m_YH << std::endl;
	std::cout << " abort "<< std::endl;
	std::cout << std::endl;
	abort();
	}
	
}


void Pad::WriteOut() const
{
	std::cout << "StartPad" << std::endl;
	std::cout << " m_EventNber; " << std::setw(16) << m_EventNber << std::endl;
	std::cout << " m_EntryNber; " << std::setw(16) << m_EntryNber << std::endl;
	std::cout << " m_iX	; " << std::setw(16)						<< m_iX		<< std::endl;
	std::cout << " m_iY	; " << std::setw(16)						<< m_iY		<< std::endl;
	std::cout << " m_Time0; " << std::setw(16) << std::setprecision(9) << m_Time0 << std::endl;
	std::cout << " m_XTrue; " << std::setw(16) << std::setprecision(9) << m_XTrue << std::endl;
	std::cout << " m_YTrue; " << std::setw(16) << std::setprecision(9) << m_YTrue << std::endl;

	std::cout << " m_AMax_WF; " << std::setw(16) << std::setprecision(9) << m_AMax_WF << std::endl;
	std::cout << " m_TMax_WF; " << std::setw(16) << std::setprecision(9) << m_TMax_WF << std::endl;
	std::vector < int > The_v_ADC = Get_vADC();
	int iTem_Max = The_v_ADC.size();
	std::cout << " v_ADC; " << std::setw(5)	<< iTem_Max;
	for (int iTem = 0; iTem< iTem_Max; iTem++){
	std::cout << "; "	<< std::setw(5)	<< The_v_ADC[iTem];
	}
	std::cout << std::endl;

	std::cout << "EndPad" << std::endl;
}


void Pad::Ini_Models(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI	
){
//
	p_Model_ReadOutGeometry	= pModel_ReadOutGeometry;
	p_Model_Electronics		= pModel_Electronics	;
	p_Model_ChargeI			= pModel_ChargeI		;
	
//
	int	TimeBinMin = -1;
	int	TimeBinMax = 10000;
	m_TimeConvoMin			=	double(TimeBinMin)*1.E-9;
	m_TimeConvoMax			=	double(TimeBinMax)*1.E-9;
	m_NberOfTimeConvoPoints = (TimeBinMax-TimeBinMin)*1 + 1;
	
	m_TimeConvoStep = (m_TimeConvoMax-m_TimeConvoMin)/double(m_NberOfTimeConvoPoints-1);

}

void	Pad::Clear_ADC() 
{ 
	v_ADC	.clear(); 
	v_iTimeb.clear(); 
}
void	Pad::Set_ADC(const int&	iTimeb,const int&	ADC)			
{ 
	if ( ADC == 0 ) return;
	
	v_ADC	.push_back(ADC	); 
	v_iTimeb.push_back(iTimeb); 
}
void	Pad::Wf_DoClosure()
{
//		std::cout << " Pad::Wf_DoClosure "
//				<< "	"
//				<< "	Before Get_A_T_Max_WF"
//				<< std::endl; 
	m_AMax_WF = 0;
	m_TMax_WF = 0;
	Get_A_T_Max_WF(m_AMax_WF,m_TMax_WF);
	
//		std::cout << " Pad::Wf_DoClosure "
//				<< "	"
//				<< "	Before Get_A_T_Max_WF_01"
//				<< std::endl; 
	m_AMax_WF_01 = 0;
	m_TMax_WF_01 = 0;
	Get_A_T_Max_WF_01(m_AMax_WF_01,m_TMax_WF_01);
	
//		std::cout << " Pad::Wf_DoClosure "
//				<< "	"
//				<< "	Before Get_A_T_Max_FIT"
//				<< std::endl; 
	m_AMax_FIT = 0;
	m_TMax_FIT = 0;
	Get_A_T_Max_FIT(m_AMax_FIT,m_TMax_FIT);
	
//		std::cout << " Pad::Wf_DoClosure "
//				<< "	"
//				<< "	After Get_A_T_Max_FIT"
//				<< std::endl; 
//	v_ADC.clear(); std::vector<short int>().swap( v_ADC	);
//	v_iTimeb.clear(); std::vector<short int>().swap( v_iTimeb );
		
}

std::vector < int > Pad::Get_vADC() const
{
	std::vector< int > ToBeReturned;
	ToBeReturned	.clear(); 
	ToBeReturned	.resize( 510 , 0. );
	int iTimeBin_Max = v_ADC.size();
	for (int iTimeBin = 0; iTimeBin < iTimeBin_Max; iTimeBin++){
	int TheTimeBin = v_iTimeb[iTimeBin];
	int TheADC	=	v_ADC	[iTimeBin];
	if (TheTimeBin<510) ToBeReturned[ TheTimeBin ] = TheADC;
	}
	return ToBeReturned;
}

double Pad::Get_AMax_FromSet() const { return	m_AMax_FromSet; }
double Pad::Get_TMax_FromSet() const { return	m_TMax_FromSet; }

double Pad::Get_AMax_WF() const { return	m_AMax_WF; }
double Pad::Get_TMax_WF() const { return	m_TMax_WF; }

double Pad::Get_AMax_WF_01() const { return	m_AMax_WF_01; }
double Pad::Get_TMax_WF_01() const { return	m_TMax_WF_01; }

double Pad::Get_AMax_FIT() const { return	m_AMax_FIT; }
double Pad::Get_TMax_FIT() const { return	m_TMax_FIT; }
int	Pad::Get_FIT_Status()	const { return	m_FIT_Status; }
double Pad::Get_FIT_A0P	()	const { return	m_FIT_A0P; }
double Pad::Get_FIT_A0M	()	const { return	m_FIT_A0M; }
double Pad::Get_FIT_X0	()	const { return	m_FIT_X0; }
double Pad::Get_FIT_Y0	()	const { return	m_FIT_Y0; }

double Pad::Get_FIT_Xmin	()	const { return	m_FIT_Xmin; }
double Pad::Get_FIT_Xmax	()	const { return	m_FIT_Xmax; }

double Pad::Get_AMax_True	()			const { return	m_AMax_True; }
double Pad::Get_TMax_True	()			const { return	m_TMax_True; }
void	Pad::Set_AMax_True	(const double& AMax){ m_AMax_True = AMax; }
void	Pad::Set_TMax_True	(const double& TMax){ m_TMax_True = TMax; }


void	Pad::Get_A_T_Max_WF(double& Amax, double& Tmax)
{
	Amax = 0;
	Tmax = 0;
	std::vector < int > The_v_ADC = Get_vADC();
	int First = 1;
	int NADC = The_v_ADC.size();
//	std::cout << "Pad::Get_A_T_Max_WF " 
//			<< "NADC " << NADC
//			<< std::endl;
	for (int iTimeBin = 0; iTimeBin < NADC; iTimeBin++){
	int ADC_value = The_v_ADC[iTimeBin];
	if (First==1){
		First = 0;
		Amax	= ADC_value;
		Tmax	= iTimeBin;
	}
	if (Amax<ADC_value){
		Amax = ADC_value;
		Tmax = iTimeBin;
	}
	}
	
}

void	Pad::Get_A_T_Max_WF_01(double& Amax, double& Tmax)
{
	Amax = 0;
	Tmax = 0;
	std::vector < int > The_v_ADC = Get_vADC();
	int First = 1;
	int NADC = The_v_ADC.size();
	for (int iTimeBin = 0; iTimeBin < NADC; iTimeBin++){
	int ADC_value = The_v_ADC[iTimeBin];
	if (First==1){
		First = 0;
		Amax	= ADC_value;
		Tmax	= iTimeBin;
	}
	if (Amax<ADC_value){
		double Sum		= 0.;
		int	Sum_Kount = 0;
		if ( (iTimeBin-1) > 0	) { Sum_Kount += 1; Sum+= The_v_ADC[iTimeBin-1]; }
		if ( (iTimeBin+1) < NADC) { Sum_Kount += 1; Sum+= The_v_ADC[iTimeBin+1]; }
		Sum = Sum/double(Sum_Kount);
		if ( std::fabs( (ADC_value - Sum)/Sum ) < 0.3 ){
		Amax = ADC_value;
		Tmax = iTimeBin;
		}
	}
	}
	if (Tmax==0) Get_A_T_Max_WF(Amax,Tmax); // Prevent absurd records
	
}

void	Pad::Get_A_T_Max_FIT(double& Amax, double& Tmax)
{
	Amax = 0;
	Tmax = 0;
	
	double Amax_WF = 0.;
	double Tmax_WF = 0.;
	Get_A_T_Max_WF_01(Amax_WF,Tmax_WF);
	
	TH1F* pTH1F = new TH1F("BIDONGet_A_T_Max_FIT", "BIDONGet_A_T_Max_FIT",510,-0.5,509.5);
	
	std::vector < int > The_v_ADC = Get_vADC();
	int NADC = The_v_ADC.size();
	for (Int_t iTimeBin = 0; iTimeBin < NADC; iTimeBin++){
	int ADC_value = The_v_ADC[iTimeBin];
	if(ADC_value < 0) continue;
	int iTimeBinLoc = iTimeBin	+ 1; //NB: the 1st Data (iTimeBin=0) goes in the 1st bin (iTimeBinLoc=1) [-0.5,+0.5]
	if ( iTimeBinLoc >= 1 && iTimeBinLoc <= 510 )
	pTH1F->SetBinContent( iTimeBinLoc ,ADC_value);		
	} 

	double TheRange = 50.;
	double T_FitMin = Tmax_WF	- TheRange;
	double T_FitMax = Tmax_WF	+ TheRange;
	
	TAxis* pTAxis = pTH1F->GetXaxis();
	int	bmin = pTAxis->FindBin(T_FitMin); 
	int	bmax = pTAxis->FindBin(T_FitMax);
	
	bmin = pTAxis->FindBin(T_FitMin); 
	bmax = pTAxis->FindBin(T_FitMax);
	pTAxis->SetRange(bmin ,bmax);	
	
	double TheTSigm = pTH1F->GetRMS (); 
	
	T_FitMin = Tmax_WF - std::max(10.,TheTSigm	);
	T_FitMax = Tmax_WF + std::max( 5.,TheTSigm/2.);
	
	pTAxis->SetRange(); 

	if (T_FitMax<0){
	std::cout << " T_FitMax " << T_FitMax << " Tmax_WF " << std::endl;
	abort();
	}
	if (T_FitMin>510){
	std::cout << " T_FitMin " << T_FitMin << " Tmax_WF " << std::endl;
	abort();
	}
	
//
	m_FIT_Status = 1;
	m_FIT_X0 = Tmax_WF;
	m_FIT_Y0 = Amax_WF;
	m_FIT_A0P= -m_FIT_Y0/(2.*5*5);
	m_FIT_A0M= -m_FIT_Y0/(2.*5*5);
	Amax = m_FIT_Y0;
	Tmax = m_FIT_X0;
	m_FIT_Xmin = 0.;
	m_FIT_Xmax = 0.;
	
	ParabolaFunction aParabolaFunction;
	TF1* pTF1 = new TF1("ParabolaFunction",aParabolaFunction ,T_FitMin ,T_FitMax,3);	

	pTF1->SetParName(0,"A0P");
	pTF1->SetParName(1,"X0");
	pTF1->SetParName(2,"Y0");
	
	pTF1->SetParameter(0,m_FIT_A0P);
	pTF1->SetParameter(1,m_FIT_X0);
	pTF1->SetParameter(2,m_FIT_Y0);
	
	pTF1->SetParLimits (1, T_FitMin, T_FitMax);
	
	int Loc_FIT_Status_A = pTH1F->Fit("ParabolaFunction","Q","",T_FitMin ,T_FitMax);
	
	if ( Loc_FIT_Status_A == 0){
	m_FIT_Status = Loc_FIT_Status_A;
	m_FIT_A0P = pTF1->GetParameter(0);
	m_FIT_A0M = pTF1->GetParameter(0);
	m_FIT_X0	= pTF1->GetParameter(1);
	m_FIT_Y0	= pTF1->GetParameter(2);
	Amax = m_FIT_Y0;
	Tmax = m_FIT_X0;
	m_FIT_Xmin = T_FitMin;
	m_FIT_Xmax = T_FitMax;
	
	T_FitMin = pTF1->GetParameter(1) - std::max(10.,TheTSigm	);
	T_FitMax = pTF1->GetParameter(1) + std::max( 5.,TheTSigm/2.);

	pTF1->SetRange(T_FitMin ,T_FitMax);
	
	pTF1->SetParLimits (1, T_FitMin, T_FitMax);

	int Loc_FIT_Status_B = pTH1F->Fit("ParabolaFunction","Q","",T_FitMin ,T_FitMax);
	
	if ( Loc_FIT_Status_B == 0){
		m_FIT_Status = Loc_FIT_Status_B;
		m_FIT_A0P = pTF1->GetParameter(0);
		m_FIT_A0M = pTF1->GetParameter(0);
		m_FIT_X0	= pTF1->GetParameter(1);
		m_FIT_Y0	= pTF1->GetParameter(2);
		Amax = m_FIT_Y0;
		Tmax = m_FIT_X0;
		m_FIT_Xmin = T_FitMin;
		m_FIT_Xmax = T_FitMax;
		
		ParabolaFunctionNG aParabolaFunctionNG;
		TF1* pTF12 = new TF1("ParabolaFunctionNG",aParabolaFunctionNG ,T_FitMin ,T_FitMax,4);	

		pTF12->SetParName(0,"A0P");
		pTF12->SetParName(1,"A0M");
		pTF12->SetParName(2,"X0" );
		pTF12->SetParName(3,"Y0" );
		
		pTF12->SetParameter(0,m_FIT_A0P);
		pTF12->SetParameter(1,m_FIT_A0M);
		pTF12->SetParameter(2,m_FIT_X0 );
		pTF12->SetParameter(3,m_FIT_Y0 );
		
		pTF12->SetParLimits (2, T_FitMin, T_FitMax);
		
		int Loc_FIT_Status_C = pTH1F->Fit("ParabolaFunctionNG","Q","",T_FitMin ,T_FitMax);

		if ( Loc_FIT_Status_C == 0){
		m_FIT_Status = Loc_FIT_Status_C;
		m_FIT_A0P = pTF12->GetParameter(0);
		m_FIT_A0M = pTF12->GetParameter(1);
		m_FIT_X0	= pTF12->GetParameter(2);
		m_FIT_Y0	= pTF12->GetParameter(3);
		Amax = m_FIT_Y0;
		Tmax = m_FIT_X0;
		m_FIT_Xmin = T_FitMin;
		m_FIT_Xmax = T_FitMax;
		}
		
		delete pTF12;
	}

	}

	delete pTH1F;
	delete pTF1;
}


//------------------------------Signal Simulation-----------------------//
void Pad::SetSignalModel(
	const double& Time0,
	const double& XTrue,	
	const double& YTrue	
){
	m_Time0	= Time0;
	m_XTrue	= XTrue;
	m_YTrue	= YTrue;
}

double Pad::Get_Time0	()		const { return m_Time0; }
double Pad::Get_XTrue	()		const { return m_XTrue; }
double Pad::Get_YTrue	()		const { return m_YTrue; }


double Pad::Get_Qpad(const double& Time)
{
	return ( p_Model_ChargeI->Get_Qpad(Time,m_Time0,m_XTrue,m_YTrue,m_XL,m_XH,m_YL,m_YH) );
}

double Pad::Get_APad(const double& Time)
{
	double ToBeReturned = 0.;

	double LocalTime = Time - m_Time0;
	if (LocalTime<=0) return ToBeReturned;

	for (int iTem = 0; iTem< m_NberOfTimeConvoPoints; iTem++){
	double Taui = m_TimeConvoMin + double(iTem)*m_TimeConvoStep;
	ToBeReturned +=p_Model_ChargeI	->Get_Qpad(Taui,0.,m_XTrue,m_YTrue,m_XL,m_XH,m_YL,m_YH)
					*p_Model_Electronics->Derive_Response_Base(LocalTime-Taui)	
					*m_TimeConvoStep;
	}
	
	return ToBeReturned;
	
}
