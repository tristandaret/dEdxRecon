#ifndef ROBoard_H
#define ROBoard_H

#include "Misc/Misc.h"

#include "EvtModel/Pad.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class ROBoard {
public:
   /** Constructor */
   ROBoard(
     Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
     Model_Electronics*      pModel_Electronics      ,
     Model_ChargeI*          pModel_ChargeI
   );
   virtual ~ROBoard();
   
   
//------------------------------Models-----------------------//
   const Model_ReadOutGeometry*  Get_Model_ReadOutGeometry() ;
   const Model_Electronics    *  Get_Model_Electronics    () ;
   const Model_ChargeI        *  Get_Model_ChargeI        () ;
   
   
//------------------------------Pads addition-----------------------//
   virtual void Add_Pad(Pad* pPad); // Add a pad 
   

//------------------------------Pads Access-----------------------//
   int  Get_NberOfPads() const ; // Size of Pad set
   Pad* Get_Pad (const int& Index1D)  ; // Get Pad
   
   int  Get_IsThisPadExisting(const int& iX, const int& iY) const ; // return Pad (iX,iY) existence state
   Pad* Get_Pad              (const int& iX, const int& iY)       ; // return Pad (iX,iY) if it exists
      
   Pad* Get_Pad (const Pad* pPad)       ; // return Pad (iX,iY) if it exists
      
   
//------------------------------Data Members-----------------------//
protected:

   int    GetLinearIndex(const int& iX, const int& iY) const ;
   
   void Ini_Models(
     Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
     Model_Electronics*      pModel_Electronics      ,
     Model_ChargeI*          pModel_ChargeI   
   ) ;

protected:
  short int m_Nx ;
  short int m_Ny ;

  std::vector < int > V_ExisFlag2D ;
  
  std::vector < Pad* > V_Pad  ;
  
  Model_ReadOutGeometry*  p_Model_ReadOutGeometry  ;
  Model_Electronics*      p_Model_Electronics      ;
  Model_ChargeI*          p_Model_ChargeI          ;
  
};



#endif

