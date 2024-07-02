#ifndef ParabolaFunctionNG_H
#define ParabolaFunctionNG_H

#include "Misc.h"

// 
//Fit parabola -std::fabs(A0) * std::pow(X0-vX,2)+ Y0 );

/////////////////////////////////////////////////////////////
class ParabolaFunctionNG{
public:
	/** Constructor */
	ParabolaFunctionNG();
	virtual ~ParabolaFunctionNG();
	
	double Get_A0P();
	double Get_A0M();
	double Get_X0 ();
	double Get_Y0 ();
	void	Set_A0P(const double& A0P);
	void	Set_A0M(const double& A0M);
	void	Set_X0 (const double& X0 );
	void	Set_Y0 (const double& Y0 );
	
	double operator() (double *x, double *par);

private:

	double m_A0P;
	double m_A0M;
	double m_X0;
	double m_Y0;
	
};



#endif

