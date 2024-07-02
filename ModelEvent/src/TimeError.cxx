#include "TimeError.h"

//
double TimeError_Leading()		; 
double TimeError_NextLeading()	;
double TimeError_NextNextLeading();

double TimeError_BASE(const double& DeltaT_in);


//
double TimeError(
		const Pad* pPad,
		const Pad* pPad_Leading,
		const Pad* pPad_NextLeading,
		const Pad* pPad_NextNextLeading	
){
	double ToBeReturned = TimeError( pPad->Get_TMax() - pPad_Leading->Get_TMax());
	
	if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) ToBeReturned = TimeError_Leading();
	
	if ( pPad_NextLeading ){
	if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) ToBeReturned = TimeError_NextLeading();
	}
	
	if ( pPad_NextNextLeading ){
	if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) ToBeReturned = TimeError_NextNextLeading();
	}
	
	return ToBeReturned;
}


double TimeError(const double& DeltaT_in)
{
 return 3.;
 return (TimeError_BASE(DeltaT_in))/5.;
}

double TimeError_BASE(const double& DeltaT_in)
{
 double DeltaT = std::fabs(DeltaT_in);
 
 double X1 = 25.; double Y1 =	4.;
 if (DeltaT <= X1 ) return Y1;
 
 double X2 = 40.; double Y2 = 10.;
 if (DeltaT_in <= X2) return ( (DeltaT_in-X1)*(Y2-Y1)/(X2-X1) + Y1 );

 double X3 = 50.; double Y3 = 25.;
 return ( (DeltaT_in-X2)*(Y3-Y2)/(X3-X2) + Y2 );
 
}

double TimeError_Leading()		{	return	2. * 10000000.; }
double TimeError_NextLeading()	{	return	2. * 10000000.; }
double TimeError_NextNextLeading() {	return	2. * 10000000.; }
double TimeError_BASE(const double& DeltaT_in);