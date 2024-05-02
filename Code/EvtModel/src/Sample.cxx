#include "EvtModel/Sample.h"
#include "Misc/Util.h"
#include "EvtModel/TimeError.h"
#include "EvtModel/AmplitudeError.h"

Sample::Sample()
{
//
  p_Model_ReadOutGeometry = 0 ;
  p_Model_Electronics     = 0 ;
  p_Model_ChargeI         = 0 ;
  
//
  m_FilePRF_Exist = 0  ;
  m_FilePRF_Norm  = 0. ;
  m_FilePRF_a2    = 0. ;
  m_FilePRF_a4    = 0. ;
  m_FilePRF_b2    = 0. ;
  m_FilePRF_b4    = 0. ;

//

}

Sample::Sample(
          Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
          Model_Electronics*      pModel_Electronics      ,
          Model_ChargeI*          pModel_ChargeI  
)
{
//
  p_Model_ReadOutGeometry = pModel_ReadOutGeometry ;
  p_Model_Electronics     = pModel_Electronics     ;
  p_Model_ChargeI         = pModel_ChargeI         ;
  
//
  m_FilePRF_Exist = 0  ;
  m_FilePRF_Norm  = 0. ;
  m_FilePRF_a2    = 0. ;
  m_FilePRF_a4    = 0. ;
  m_FilePRF_b2    = 0. ;
  m_FilePRF_b4    = 0. ;


}

Sample::~Sample()
{
 int VerboseDelete = 0 ;
 if (VerboseDelete==1)
 std::cout 
       << " Sample Destructor "
       << std::endl ;
  int NEvents = Get_NberOfEvents();
  for (int iE = 0 ; iE< NEvents; iE++){
    delete V_Event[iE] ;
    V_Event[iE] = 0 ;
  }
  V_Event.clear() ;
  
  delete p_Model_ReadOutGeometry  ; p_Model_ReadOutGeometry  = 0 ;
  delete p_Model_Electronics      ; p_Model_Electronics      = 0 ;
  delete p_Model_ChargeI          ; p_Model_ChargeI          = 0 ;


}

void Sample::Add_Event(Event* pEvent)
{ 
  V_Event.push_back(pEvent); 
  m_NberOfEvents = V_Event.size() ;
}

int    Sample::Get_NberOfEvents() const { return m_NberOfEvents   ; }
Event* Sample::Get_Event(int Index1D)   { return V_Event[Index1D] ; }

//-------------------------Tracks-------------------------//    
SetOfTracks* Sample::Get_SetOfTracks_ForThisModule(const int& ModuleNber){ 
  SetOfTracks* pSetOfTracks = new SetOfTracks ; 
  
  int NberOfEvents = Get_NberOfEvents();
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent = Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    pSetOfTracks->Add_Track( pEvent->GiveMe_Track_ForThisModule(ModuleNber) );   
  }
  return pSetOfTracks ; 
}

//-------------------------Prints-------------------------//    
void Sample::WriteOut() const 
{
  std::cout << "StartSample" << std::endl ;
  
  p_Model_ReadOutGeometry  ->WriteOut() ;
  p_Model_Electronics      ->WriteOut() ;
  p_Model_ChargeI          ->WriteOut() ;
  
  for (int iE = 0 ; iE< m_NberOfEvents; iE++){
    V_Event[iE]->WriteOut() ;
  }
  
  std::cout << "EndSample" << std::endl ;
}

void Sample::Set_Model_ReadOutGeometry(Model_ReadOutGeometry* pModel_ReadOutGeometry) { p_Model_ReadOutGeometry = pModel_ReadOutGeometry ; }
void Sample::Set_Model_Electronics    (Model_Electronics    * pModel_Electronics    ) { p_Model_Electronics     = pModel_Electronics     ; }
void Sample::Set_Model_Charge         (Model_ChargeI        * pModel_ChargeI        ) { p_Model_ChargeI         = pModel_ChargeI         ; }

Model_ReadOutGeometry*  Sample::Get_Model_ReadOutGeometry(){ return p_Model_ReadOutGeometry ;}
Model_Electronics    *  Sample::Get_Model_Electronics    (){ return p_Model_Electronics     ;}
Model_ChargeI        *  Sample::Get_Model_ChargeI        (){ return p_Model_ChargeI         ;}

//--------------------------------------------------------------------------//   
int Sample::StatusFit(){ return m_StatusFit ; }

void Sample::Set_RC(const double& RC){ m_RC = RC; }
void Sample::Set_DD(const double& DD){ m_DD = DD; }

double Sample::Get_RC      () const { return m_RC      ; }
double Sample::Get_eRC     () const { return m_eRC     ; }

double Sample::Get_DD      () const { return m_DD      ; }
double Sample::Get_eDD     () const { return m_eDD     ; }

double Sample::Get_Chi2Min () const { return m_Chi2Min ; }
    
double Sample::Get_RC_BeforeMinimisation () const { return m_RC_BeforeMinimisation ; }
double Sample::Get_DD_BeforeMinimisation () const { return m_DD_BeforeMinimisation ; }

//---------------------------------------
void Sample::SetFilePRF(
                const double& Norm ,
                const double& a2   ,
                const double& a4   ,
                const double& b2   ,
                const double& b4   
){
  m_FilePRF_Exist = 1 ;
  m_FilePRF_Norm  = Norm ;
  m_FilePRF_a2    = a2   ;
  m_FilePRF_a4    = a4   ;
  m_FilePRF_b2    = b2   ;
  m_FilePRF_b4    = b4   ;
}

void Sample::GetFilePRF(
                  int&    FilePRF_Exist ,
                  double& Norm          ,
                  double& a2            ,
                  double& a4            ,
                  double& b2            ,    
                  double& b4   
){
  FilePRF_Exist  = m_FilePRF_Exist ;
  Norm           = m_FilePRF_Norm ;
  a2             = m_FilePRF_a2   ;
  a4             = m_FilePRF_a4   ;
  b2             = m_FilePRF_b2   ;
  b4             = m_FilePRF_b4   ;
}

// Small dump
void Sample::SmallDump() const 
{
  std::cout << std::endl ;
  std::cout << "Sample::SmallDump" << std::endl ;
  std::cout << "->Get_NberOfEvents " << Get_NberOfEvents() << std::endl ;
  int NEvents = Get_NberOfEvents();
  for (int iE = 0 ; iE< NEvents; iE++){
    V_Event[iE]->SmallDump() ;
  }

}