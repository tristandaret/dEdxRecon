#ifndef Event_H
#define Event_H

#include "Misc/Misc.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Track.h"
#include "EvtModel/Module.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class Event {
public:
   /** Constructor */
   Event(
     const int& EventNber ,
     const int& EntryNber ,
     Model_ReadOutGeometry* pModel_ReadOutGeometry  ,
     Model_Electronics*     pModel_Electronics      ,
     Model_ChargeI*         pModel_ChargeI
   );
   virtual ~Event();


//------------------------------IDs-----------------------//
   int Get_EventNber () const ; // Get Event nber
   int Get_EntryNber () const ; // Get Entry nber


//------------------------------Print-----------------------//
   void SmallDump() const ;// Small dump
   void WriteOut() const  ;// Big dump


//------------------------------Models-----------------------//
   const Model_ReadOutGeometry*  Get_Model_ReadOutGeometry() ;
   const Model_Electronics    *  Get_Model_Electronics    () ;
   const Model_ChargeI        *  Get_Model_ChargeI        () ;
   
   
//------------------------------Event Validity-----------------------//
   int  IsValid()    const ; // Get validation status of the Event
   void Validate()         ; // Validate the Event
   void Invalidate()       ; // Invalidate the Event


//------------------------------Module Addition-----------------------//
   void Clear_Modules()            ; // Clear existing Module set
   void Add_Module(Module* pModule); // Add a Module 


//------------------------------Module Access-----------------------//
   Module*       Get_ThisModule      (const int& ModuleNber)       ; // Get Module by the Module Nber if exists
   const Module* Get_ThisModuleConst (const int& ModuleNber) const ;
   
   int     Get_NberOfModule() const                ; // Size of Module set
   Module* Get_Module_InArray (const int& Index1D) ; // Get Module


//------------------------------Module Validity-----------------------//
   int  Validity_ForThisModule (const int& ModuleNber) const ; // Get validation status of the Modle
   void Validate_ThisModule    (const int& ModuleNber)       ; // Validate the Module
   void Invalidate_ThisModule  (const int& ModuleNber)       ; // Invalidate theModule


//------------------------------Clusters Access-----------------------//
   std::vector < Cluster* > GiveMe_Clusters_ForThisModule(const int& ModuleNber) ;

   double GiveMe_CutSlopeXY_ForThisModule    (const int& ModuleNber) ;
   
   double GiveMe_CutSlopeXZ_ForThisModule    (const int& ModuleNber) ;
   double GiveMe_CutInterCeptXZ_ForThisModule(const int& ModuleNber) ;

   std::vector <double> GiveMe_CutSlopeXYZ_ForThisModule(const int& ModuleNber) ;
  
   void Replace_Clusters_ForThisModule(
                                       std::vector < Cluster* >& VCluster,
                                       const int& ModuleNber
                                      );  // Replace existing clusters set
   
   Cluster* Get_Cluster_Copy (Cluster* pCluster) ; // Get copy of the input cluster 


//------------------------------Track Access-----------------------//
  Track* GiveMe_AnUnfittedTrack_ForThisModule(const int& ModuleNber)  ; // Ownership is passed to the user
  void   Set_Track_ForThisModule(Track* pTrack,const int& ModuleNber) ; // Ownership is passed to the Module
  const  Track* GiveMe_Track_ForThisModule(const int& ModuleNber)     ; 
   
   
//------------------------------Data Members-----------------------//
private:
  short int m_IsValid ;
  
  int m_EventNber ;
  int m_EntryNber ;
  
  short int m_NberOfModules ;
  std::vector < Module* > V_Module  ;

  void Ini_Models(
    Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
    Model_Electronics*      pModel_Electronics      ,
    Model_ChargeI*          pModel_ChargeI   
  ) ;

  Model_ReadOutGeometry*  p_Model_ReadOutGeometry  ;
  Model_Electronics*      p_Model_Electronics      ;
  Model_ChargeI*          p_Model_ChargeI          ;

};



#endif

