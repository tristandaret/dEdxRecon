#ifndef Pad_H
#define Pad_H

#include "Misc.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_ChargeI.h"


/////////////////////////////////////////////////////////////
class Pad {
public:
	/** Constructor */
	Pad(
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
	);
	Pad(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI,
	std::string	PadName,
	const int&	EventNber,
	const int&	EntryNber,
	const int&	ModuleNber, 
	const int&	iX,
	const int&	iY									
	);
	virtual ~Pad();


//------------------------------IDs-----------------------//
	int Get_EventNber	() const; // Get Event nber
	int Get_EntryNber	() const; // Get Entry nber
	int Get_ModuleNber () const; // Get Module nber
	int Get_iX		() const; // Get X index
	int Get_iY		() const; // Get Y index
	

//------------------------------Print-----------------------//
	void WriteOut() const; // Big dump


//------------------------------Pad Validity-----------------------//
	int	IsValid()	const; // Get validation status
	void Validate()	; // Validate
	void Invalidate()	; // Invalidate


//------------------------------Models-----------------------//
	const Model_ReadOutGeometry*	Get_Model_ReadOutGeometry() const;
	const Model_Electronics	*	Get_Model_Electronics	() const;
	const Model_ChargeI		*	Get_Model_ChargeI		() const;
	
	
//------------------------------Pad Data-----------------------//
	std::string Get_PadName	() const		; // Get Pad name
	
	double		Get_XPad()	const		; // Get X position of Pad center (m)
	double		Get_YPad()	const		; // Get Y position of Pad center (m)
	
	double		Get_XL()		const		; // Get X position of Left	Pad edge (m)
	double		Get_XH()		const		; // Get X position of Right	Pad edge (m)
	double		Get_YL()		const		; // Get Y position of Bottom Pad edge (m)
	double		Get_YH()		const		; // Get Y position of top	Pad edge (m)
	
	double		Get_LX()		const		; // Get X Pad size (m)
	double		Get_LY()		const		; // Get Y Pad size (m)
						
	double		Get_AMax()	const		; // Get max amplitude (ADC)
	double		Get_TMax()	const		; // Get Time at max	(Time bin)
	
	void		Set_AMax(const double& AMax); // Set max amplitude (ADC)
	void		Set_TMax(const double& TMax); // Set Time at max	(Time bin)

	void Set_Data_2Use(const int& iOpt); // Data: switch for data type to use
										// iOpt = 0; Amax and Tmax from input ntuple
										//		= 1; Amax and Tmax from 1st max of WF
										//		= 2; Amax and Tmax from 1st clean max of WF
										//		= 3; Amax and Tmax from fit of WF peak (Default)


//------------------------------WFs-----------------------//
//Waveform stuff
	void	Clear_ADC()					; // Reset ADC set
	void	Set_ADC(
					const int&	iTimeb,
					const int&	ADC
					)						; // Set ADC set
	void	Wf_DoClosure()					; // Closure: compute max amplitude and time	
	
	std::vector < int > Get_vADC() const	; // Get ADC as vector
	
//A and T max
//A and T from last call to Set functions
	double	Get_AMax_FromSet()	const		; // Get max amplitude (ADC)
	double	Get_TMax_FromSet()	const		; // Get Time at max	(Time bin)

//1st maximum in WF
	double	Get_AMax_WF()	const			; // Get max amplitude (ADC)
	double	Get_TMax_WF()	const			; // Get Time at max	(Time bin)

//Improved 1st maximum in WF
	double	Get_AMax_WF_01()	const			; // Get max amplitude (ADC)
	double	Get_TMax_WF_01()	const			; // Get Time at max	(Time bin)

//Fit of peak
	double	Get_AMax_FIT()	const			; // Get max amplitude (ADC)
	double	Get_TMax_FIT()	const			; // Get Time at max	(Time bin)
	int	Get_FIT_Status()	const		; // 1: fit failed; 0: Fit OK
	double	Get_FIT_A0P	()	const		; //
	double	Get_FIT_A0M	()	const		; //
	double	Get_FIT_X0	()	const		; //
	double	Get_FIT_Y0	()	const		; //
	double	Get_FIT_Xmin	()	const		; //
	double	Get_FIT_Xmax	()	const		; //

//A and T from thruth
	double	Get_AMax_True()	const		; // Get max amplitude (ADC)
	double	Get_TMax_True()	const		; // Get Time at max	(Time bin)
	void	Set_AMax_True(const double& AMax); // Set max amplitude (ADC)
	void	Set_TMax_True(const double& TMax); // Set Time at max	(Time bin)

private:
	void	Get_A_T_Max_WF (double& Amax, double& Tmax);
												// Get Amax and Tmax from WF in the time window [iTimeBin_min;iTimeBin_max]
	void	Get_A_T_Max_WF_01 (double& Amax, double& Tmax);
												// Get Amax and Tmax from WF in the time window [iTimeBin_min;iTimeBin_max]
	void	Get_A_T_Max_FIT(double& Amax, double& Tmax);
												// Get Amax and Tmax from WF Fit in the time window [iTimeBin_min;iTimeBin_max]

//------------------------------Signal Simulation-----------------------//
public:
//Set parameters of the simulation
	void SetSignalModel(
	const double& Time0,
	const double& XTrue,	
	const double& YTrue	
						);		

	double Get_Time0()	const; 
	double Get_XTrue()	const; 
	double Get_YTrue()	const; 
	
	double Get_Qpad(const double& Time); // charge
	double Get_APad(const double& Time); // amplitude


//------------------------------Data Members-----------------------//
private:

	short int m_IsValid = 1;

	void SetEdges(
		const double& XL ,
		const double& XH ,
		const double& YL ,
		const double& YH
	);

	void Ini_Models(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI	
	);
		
private:

	std::string m_PadName;
	
	int		m_EventNber;
	int		m_EntryNber;
	short int	m_ModuleNber;
	short int	m_iX	;
	short int	m_iY	;

//	double m_XPad;
//	double m_YPad;
//	double m_LX;
//	double m_LY;

	double m_XL;
	double m_XH;
	double m_YL;
	double m_YH;
	
	double m_Time0;
	double m_XTrue;
	double m_YTrue;
	
	double m_TimeConvoMin		;
	double m_TimeConvoMax		;
	int	m_NberOfTimeConvoPoints;
	double m_TimeConvoStep	;
	
	double m_AMax;
	double m_TMax;
	
	Model_ReadOutGeometry*	p_Model_ReadOutGeometry;
	Model_Electronics*		p_Model_Electronics	;
	Model_ChargeI*			p_Model_ChargeI		;

	std::vector < short int > v_ADC;
	std::vector < short int > v_iTimeb;
	
	double m_AMax_FromSet;
	double m_TMax_FromSet;
	
	double m_AMax_WF;
	double m_TMax_WF;
	
	double m_AMax_WF_01;
	double m_TMax_WF_01;
	
	double m_AMax_FIT;
	double m_TMax_FIT;
	bool	m_FIT_Status; // True: fit failed; False: Fit OK
	double m_FIT_A0P;
	double m_FIT_A0M;
	double m_FIT_X0;
	double m_FIT_Y0;
	double m_FIT_Xmin;
	double m_FIT_Xmax;

	double m_AMax_True;
	double m_TMax_True;
	
};



#endif

