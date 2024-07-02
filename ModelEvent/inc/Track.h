#ifndef Track_H
#define Track_H

#include "Misc.h"
#include "Cluster.h"

#include "FitOutput.h"


// Track equation :	Y = Sum_(i=0,Npar-1) par_i * X^i

/////////////////////////////////////////////////////////////
class Track {
public:
	Track(
		const int& EventNber ,
		const int& EntryNber ,
		const int& ModuleNber
		);
	virtual ~Track();
	
	
//------------------------------IDs-----------------------//
	int Get_EventNber	() const; // Get Event nber
	int Get_EntryNber	() const; // Get Entry nber
	int Get_ModuleNber () const; // Get Module nber


//------------------------------Track Data-----------------------//
	void SetNberOfParameters(const int& NberOfParam );
	int	GetNberOfParameters() const;
	
	std::string Get_ParameterName	(const int& iPar) const;
	double		Get_ParameterValue (const int& iPar) const;
	double		Get_ParameterError (const int& iPar) const;

	double		Get_ParameterValue_BeforeMinimisation(const int& iPar) const;
	
	
//------------------------------Cluster Addition-----------------------//
	void Add_Cluster(Cluster* pCluster);


//------------------------------Cluster Access-----------------------//
	int			Get_NberOfCluster()		const;
	const Cluster* Get_Cluster	(int Index1D) const;
	double		Get_Residual (int Index1D) const;
	double		Get_Pull	(int Index1D) const;

	void DoClosure();
	void DumpRec() const;


//------------------According to Fit Y position for given X-----------------------//
	double Y_Position( const double& X ) const;
		
		
//------------------------------Data Members-----------------------//
private:
	int m_EventNber;
	int m_EntryNber;
	short int m_ModuleNber;

	int m_NberOfParam;
	
	std::vector< double		> m_Parameter;
	std::vector< double		> m_eParameter;
	std::vector< std::string > m_ParameterName;
	
	std::vector< double		> m_Parameter_BeforeMinimisation;

public:
///////////////////////////////////

	int m_NberOfClusters;
	std::vector < Cluster* > V_Cluster;
	std::vector < double >	V_Residual;
	std::vector < double >	V_Pull;

//-----------------------------Track Fit Stuff----------------------//	
private:
	double	m_Chi2Min;
	TMatrixD m_covmatrix;

public:
	double Get_Chi2Min			() const;
	int	SetParameter			(TVirtualFitter* pTVirtualFitter);
	void	SetResults			(TVirtualFitter* pTVirtualFitter);
	double Chi2					(double par[])				;
	void	SetParameters_Internal (double par[])				;
	TMatrixD Get_CovMatrix		() const;
	
private:
	FitOutput* p_FitOutput;

};



#endif

