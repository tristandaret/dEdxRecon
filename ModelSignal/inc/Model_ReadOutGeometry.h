#ifndef Model_ReadOutGeometry_H
#define Model_ReadOutGeometry_H

#include "Misc.h"

/////////////////////////////////////////////////////////////
class Model_ReadOutGeometry {
public:
	/** Constructor */
	Model_ReadOutGeometry();
	virtual ~Model_ReadOutGeometry();
	
//Get Pad board data	
	double Get_LX		() const; // Get X Pad size (m)
	double Get_LY		() const; // Get Y Pad size (m)	
	
	double Get_Xpad_min() const; // Get Lower left corner X position (m)
	double Get_Ypad_min() const; // Get Lower left corner Y position (m)
	
	int	Get_Nx		() const; // Get Nber of pads along X
	int	Get_Ny		() const; // Get Nber of pads along Y


//Swap Geometry	
	void SwapGeometry(); // Swap Nx/Ny and Lx/Ly

//Square Geometry	
	void SquareGeometry(); // Set Nx=Ny to the highest of the originals
					

//Set Pad board data	
	void Set_LX		(const double& LX		); // Set X Pad size (m)					
	void Set_LY		(const double& LY		); // Set Y Pad size (m)	
	void Set_Xpad_min(const double& Xpad_min ) 
											; // Set Lower left corner X position (m)
	void Set_Ypad_min(const double& Ypad_min ); // Set Lower left corner Y position (m)
	
	void Set_Nx		(const int&	Nx		); // Set Nber of pads along X
	void Set_Ny		(const int&	Ny		); // Set Nber of pads along Y


//Pad Parameters 
	double Get_XcPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the center
	double Get_YcPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the center
	
	double Get_XLPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the Left	edge
	double Get_YLPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the Bottom edge
	
	double Get_XHPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the Right edge
	double Get_YHPad(const int& iX,const int& iY, const int& ModuleNber) const; // Get Pad(iX,iY) X position (m) of the Top edge
	
	void GetPadEdges(
	const int& iX , 
	const int& iY ,
	const int& ModuleNber ,
	double& XL ,
	double& XH ,
	double& YL ,
	double& YH
	) const										;	// Get Pad(iX,iY) X sides position (m)
	
					
//Big Dump	
	void WriteOut() const;
	
private:


private:

	double m_LX	;
	double m_LY	;
	double m_Xpad_min;
	double m_Ypad_min;
	double m_Nx	;
	double m_Ny	;

};



#endif

