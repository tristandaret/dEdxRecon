#include "ROBoard.h"

ROBoard::ROBoard(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI
)
{
//Initialize Models
	Ini_Models(
			pModel_ReadOutGeometry,
			pModel_Electronics,
			pModel_ChargeI 
			);

}

ROBoard::~ROBoard()
{
 int VerboseDelete = 0;
 if (VerboseDelete==1)
 std::cout 
		<< " ROBoard Destructor "
		<< std::endl;
	int NPads = Get_NberOfPads();
	for (int iP = 0; iP< NPads; iP++){
	delete V_Pad[iP];
	V_Pad[iP] = 0;
	}
	V_Pad.clear();
	V_ExisFlag2D.clear();
//
	V_Pad.clear(); std::vector< Pad* >().swap( V_Pad	);
	V_ExisFlag2D.clear(); std::vector<int>().swap( V_ExisFlag2D );
//
}

const Model_ReadOutGeometry*	ROBoard::Get_Model_ReadOutGeometry(){ return p_Model_ReadOutGeometry;}
const Model_Electronics	*	ROBoard::Get_Model_Electronics	(){ return p_Model_Electronics;}
const Model_ChargeI		*	ROBoard::Get_Model_ChargeI		(){ return p_Model_ChargeI	;}

//------------------------------Pads-----------------------//
void ROBoard::Add_Pad(Pad* pPad)
{ 
	V_Pad.push_back(pPad);	
	int iX = pPad->Get_iX();
	int iY = pPad->Get_iY();
	V_ExisFlag2D[ GetLinearIndex(iX,iY) ] = V_Pad.size(); 
}

int	ROBoard::Get_NberOfPads() const { return V_Pad.size(); }
Pad* ROBoard::Get_Pad(const int& Index1D)	{ return V_Pad[Index1D];}


int ROBoard::GetLinearIndex(const int& iX, const int& iY) const { return (iX+ iY*m_Nx);}

int ROBoard::Get_IsThisPadExisting (const int& iX, const int& iY) const 
{
 if (iX < 0 || iX >= m_Nx || iY < 0 || iY >= m_Ny ) return 0;
 if ( V_ExisFlag2D[ GetLinearIndex(iX,iY) ] == 0 ) return 0;
 return 1;
}

Pad* ROBoard::Get_Pad(const int& iX, const int& iY)
{
 if (Get_IsThisPadExisting(iX,iY)==0) return 0;
 return V_Pad[ V_ExisFlag2D[ GetLinearIndex(iX,iY) ] - 1 ];
}


void ROBoard::Ini_Models(
	Model_ReadOutGeometry*	pModel_ReadOutGeometry,
	Model_Electronics*		pModel_Electronics,
	Model_ChargeI*			pModel_ChargeI	
){
//
	p_Model_ReadOutGeometry	= pModel_ReadOutGeometry;
	p_Model_Electronics		= pModel_Electronics	;
	p_Model_ChargeI			= pModel_ChargeI		;
	
//	
	m_Nx		= p_Model_ReadOutGeometry->Get_Nx		();
	m_Ny		= p_Model_ReadOutGeometry->Get_Ny		();

//
	V_ExisFlag2D	.clear(); V_ExisFlag2D	.resize( m_Nx*m_Ny , 0 );

}

Pad*	ROBoard::Get_Pad(const Pad* pPad)
{
	int iX = pPad->Get_iX();
	int iY = pPad->Get_iY();
	return Get_Pad(iX,iY);
}
