#ifndef Model_Charge1D_H
#define Model_Charge1D_H

#include "Misc.h"
#include "Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class Model_Charge1D : public Model_ChargeI {
public:
	/** Constructor */
	Model_Charge1D();
	virtual ~Model_Charge1D();
	
	
//Get charge spreading parameters	
	double Get_Lambda	() const;


//Set charge spreading parameters	
	void Set_Lambda(const double& Lambda );


//Get Charge
	double Get_Qpad(
					const double& Time ,
					const double& Time0,
					const double& XTrue,
					const double& YTrue,
					const double& XL,
					const double& XH,
					const double& YL,
					const double& YH
					) const;

					
//Big Dump	
	void WriteOut() const;
	
protected:
	void SetSecondaries();
	
	double m_Lambda;

};



#endif

