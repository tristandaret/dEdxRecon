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
  Beg_PV2() ;
  Beg_PV3() ;
  Beg_PV31() ;
  Beg_PV4() ;

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

//
  Beg_PV2() ;
  Beg_PV3() ;
  Beg_PV31() ;
  Beg_PV4() ;

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

//
  End_PV2() ;
  End_PV3() ;
  End_PV31() ;
  End_PV4() ;

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
    
//---------------------PV2-----------------------------//
void   Sample::Beg_PV2()
{
  p_FitOutput_PV2 = new FitOutput ;
  p_FitOutput_PV2->Set(1) ;
  m_StatusFit_PV2 = 0 ;
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;
}
void   Sample::End_PV2()
{
  delete p_FitOutput_PV2 ; p_FitOutput_PV2 = 0 ;
}
int Sample::StatusFit_PV2(){return m_StatusFit_PV2 ;} 

void Sample::SetEval_PV2(TF1* pTF1, const int& ModuleNber)
{ 
  m_RC = pTF1->GetParameter(0) ;
  p_TF1_PV2 = pTF1 ; 
  m_ModuleNber_PV2 = ModuleNber ;
}

double Sample::Eval_PV2(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  p_TF1_PV2->SetParameter(0,m_RC);
  if (Xin!=Xin){
    std::cout << " Sample::Eval_PV2 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_TF1_PV2->Eval(Xin*1.E2) ) ;
  }
}

int Sample::SetParameter_PV2(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

//
  int ier = 0 ; 
  
  ier = pTVirtualFitter->SetParameter(0, "RC" , m_RC , 1. , 10. , 150.  ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_RC_BeforeMinimisation =  m_RC ;
    
  return ier ;
}

void Sample::SetResults_PV2(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV2 = 0 ;
  m_StatusFit = 0 ;

  p_FitOutput_PV2->SetResults(pTVirtualFitter) ;
  
  m_RC  = p_FitOutput_PV2->p_par[0]      ;
  m_eRC = (p_FitOutput_PV2->p_eparplus[0]-p_FitOutput_PV2->p_eparminus[0])/2. ;
  
  m_Chi2Min = Chi2_PV2(p_FitOutput_PV2->p_par) ;

//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV2) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV2) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV2(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;

        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;

        V_FitRes_PV2_Pad     .push_back(pPad    );
        V_FitRes_PV2_Residual.push_back(Residual);
        V_FitRes_PV2_Pull    .push_back(Pull    );
      
      }
    }
  }
  m_NberOf_V_FitRes_PV2_Pad = V_FitRes_PV2_Pad.size() ;

}

void Sample::SetResults_FailledFit_PV2(const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Sample::SetResults_FailledFit_PV2  " << std::endl ;
  
  m_StatusFit_PV2 = 1 ;
  m_StatusFit = 1 ;

  int iPt_Max = 100 ;
  double DeltaRC = 10. ;
  
  double Chi2_cur_min    = 0. ;
  double Chi2_cur_min_RC = 0. ;
  int    Chi2_cur_min_First = 1 ;
  
  double RC_Start = m_RC_BeforeMinimisation      ;
  double RC_End   = m_RC_BeforeMinimisation + DeltaRC ;
  
  for (int iPt = 1 ; iPt< iPt_Max ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV2(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "+RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }

  RC_End   = m_RC_BeforeMinimisation           ;
  RC_Start = m_RC_BeforeMinimisation - DeltaRC ;
  
  for (int iPt = 0 ; iPt< iPt_Max-1 ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV2(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "-RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }
  
  m_RC  = Chi2_cur_min_RC ;
  m_eRC = DeltaRC ;

  if (Verbose==1) {
    std::cout << " Sample::SetResults_FailledFit_PV2  After scanning" << std::endl ;
    std::cout << "   m_RC " << m_RC  
              << " +/-  "
              << " m_eRC "  << m_eRC 
              << std::endl ;
    std::cout << std::endl ;
  }
  
  m_Chi2Min = Chi2_cur_min ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("RC"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_RC  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eRC ) ;
  
  p_FitOutput_PV2->SetResults(V_PARname,V_PAR,V_ePAR) ;

  p_TF1_PV2->SetParameter(0,m_RC ) ;
  p_TF1_PV2->SetParError (0,m_eRC) ;

//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV2) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV2) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV2(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;

        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;
        
        V_FitRes_PV2_Pad     .push_back(pPad    );
        V_FitRes_PV2_Residual.push_back(Residual);
        V_FitRes_PV2_Pull    .push_back(Pull    );
      
      }
    }
  }
  m_NberOf_V_FitRes_PV2_Pad = V_FitRes_PV2_Pad.size() ;

}

double Sample::Chi2_PV2(double par[])
{
  SetParameters_Internal_PV2(par) ;
  
  double ToBeReturned = 0. ;
  
  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV2) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV2) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV2(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;

        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;

        ToBeReturned += Pull * Pull ;
      
      }
    }
  }
  
//   std::cout 
//     << " m_RC " << m_RC 
//     << " ToBeReturned " << ToBeReturned 
//     << std::endl; 
  
  return ToBeReturned ;
} 

void Sample::SetParameters_Internal_PV2(double par[])
{
  m_RC  = par[0] ;
}

int        Sample::FitRes_PV2_Get_NberOfTermsInChi2()            const  { return m_NberOf_V_FitRes_PV2_Pad      ; }
const Pad* Sample::FitRes_PV2_Get_Pad      (int Index1D) const  { return V_FitRes_PV2_Pad     [Index1D] ; }
double     Sample::FitRes_PV2_Get_Residual (int Index1D) const  { return V_FitRes_PV2_Residual[Index1D] ; }
double     Sample::FitRes_PV2_Get_Pull     (int Index1D) const  { return V_FitRes_PV2_Pull    [Index1D] ; }

//---------------------PV3-----------------------------//
void   Sample::Beg_PV3()
{
  p_FitOutput_PV3 = new FitOutput ;
  p_FitOutput_PV3->Set(1) ;
  m_StatusFit_PV3 = 0 ;
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;
}
void   Sample::End_PV3()
{
  delete p_FitOutput_PV3 ; p_FitOutput_PV3 = 0 ;
}
int Sample::StatusFit_PV3(){return m_StatusFit_PV3 ;} 

void Sample::SetEval_PV3(SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd, const int& ModuleNber)
{ 
  p_PV3_SC_VS_x_rc_dd_SET_dd = pSC_VS_x_rc_dd_SET_dd ; 
  m_ModuleNber_PV3 = ModuleNber ;
}

double Sample::Eval_Alpha_PV3(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Sample::Eval_Alpha_PV3 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV3_SC_VS_x_rc_dd_SET_dd->Eval_AlphaPRF(Xin*1.E2,m_RC) ) ;
  }
}

double Sample::Eval_Tau_PV3(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Sample::Eval_Tau_PV3 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV3_SC_VS_x_rc_dd_SET_dd->Eval_TauBinPRF(Xin*1.E2,m_RC) ) ;
//     double Pred_Analog = p_PV3_SC_VS_x_rc_dd_SET_dd->Eval_TauBinPRF(Xin*1.E2,m_RC) ;
//     int    Pred_Digital = std::floor (Pred_Analog) ;
//     return ( Pred_Digital ) ;
  }
}

int Sample::SetParameter_PV3(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

//
  int ier = 0 ; 
  
  ier = pTVirtualFitter->SetParameter(0, "RC" , m_RC , 1. , 10. , 150.  ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_RC_BeforeMinimisation =  m_RC ;
    
  return ier ;
}

void Sample::SetResults_PV3(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV3 = 0 ;
  m_StatusFit = 0 ;

  p_FitOutput_PV3->SetResults(pTVirtualFitter) ;
  
  m_RC  = p_FitOutput_PV3->p_par[0]      ;
  m_eRC = (p_FitOutput_PV3->p_eparplus[0]-p_FitOutput_PV3->p_eparminus[0])/2. ;
  
  m_Chi2Min = Chi2_PV3(p_FitOutput_PV3->p_par) ;

//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV3) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV3) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      double TT_cur = pCluster->Get_TT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
        double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//      double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//      double TheError_Amplitude = 6. ;  // Const Amplitude Error
        double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);
    
//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;

        V_FitRes_PV3_Pad     .push_back(pPad              );
        V_FitRes_PV3_Residual.push_back(Residual_Amplitude);
        V_FitRes_PV3_Pull    .push_back(Pull_Amplitude    );
        
        int DoIt = 0 ;
        if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) DoIt = 1 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        DoIt = 0 ;
        if (DoIt==1) {
          V_FitRes_PV3_Pad     .push_back(pPad         );
          V_FitRes_PV3_Residual.push_back(Residual_Time);
          V_FitRes_PV3_Pull    .push_back(Pull_Time    );
        }
      
      }
    }
  }
  m_NberOf_V_FitRes_PV3_Pad = V_FitRes_PV3_Pad.size() ;

}

void Sample::SetResults_FailledFit_PV3(const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Sample::SetResults_FailledFit_PV3  " << std::endl ;
  
  m_StatusFit_PV3 = 1 ;
  m_StatusFit = 1 ;

  int iPt_Max = 100 ;
  double DeltaRC = 10. ;
  
  double Chi2_cur_min    = 0. ;
  double Chi2_cur_min_RC = 0. ;
  int    Chi2_cur_min_First = 1 ;
  
  double RC_Start = m_RC_BeforeMinimisation      ;
  double RC_End   = m_RC_BeforeMinimisation + DeltaRC ;
  
  for (int iPt = 1 ; iPt< iPt_Max ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV3(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "+RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }

  RC_End   = m_RC_BeforeMinimisation           ;
  RC_Start = m_RC_BeforeMinimisation - DeltaRC ;
  
  for (int iPt = 0 ; iPt< iPt_Max-1 ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV3(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "-RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }
  
  m_RC  = Chi2_cur_min_RC ;
  m_eRC = DeltaRC ;

  if (Verbose==1) {
    std::cout << " Sample::SetResults_FailledFit_PV3  After scanning" << std::endl ;
    std::cout << "   m_RC " << m_RC  
              << " +/-  "
              << " m_eRC "  << m_eRC 
              << std::endl ;
    std::cout << std::endl ;
  }
  
  m_Chi2Min = Chi2_cur_min ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("RC"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_RC  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eRC ) ;
  
  p_FitOutput_PV3->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV3) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV3) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      double TT_cur = pCluster->Get_TT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
        double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//      double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//      double TheError_Amplitude = 6. ;  // Const Amplitude Error
        double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;
        
        V_FitRes_PV3_Pad     .push_back(pPad              );
        V_FitRes_PV3_Residual.push_back(Residual_Amplitude);
        V_FitRes_PV3_Pull    .push_back(Pull_Amplitude    );
        
        int DoIt = 0 ;
        if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) DoIt = 1 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        DoIt = 0 ;
        if (DoIt==1) {
          V_FitRes_PV3_Pad     .push_back(pPad         );
          V_FitRes_PV3_Residual.push_back(Residual_Time);
          V_FitRes_PV3_Pull    .push_back(Pull_Time    );
        }
      
      }
    }
  }
  m_NberOf_V_FitRes_PV3_Pad = V_FitRes_PV3_Pad.size() ;

}

double Sample::Chi2_PV3(double par[])
{
  SetParameters_Internal_PV3(par) ;
  
  double ToBeReturned = 0. ;
  
  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV3) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV3) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      double TT_cur = pCluster->Get_TT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
        double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//      double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//      double TheError_Amplitude = 6. ;  // Const Amplitude Error
        double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;

        ToBeReturned += Pull_Amplitude * Pull_Amplitude ;
        
        int DoIt = 0 ;
        if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) DoIt = 1 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) DoIt = 1 ;
        }
        DoIt = 0 ;
        if (DoIt==1) ToBeReturned += Pull_Time * Pull_Time ;

      
      }
    }
  }
  
//   std::cout 
//     << " m_RC " << m_RC 
//     << " ToBeReturned " << ToBeReturned 
//     << std::endl; 
  
  return ToBeReturned ;
} 

void Sample::SetParameters_Internal_PV3(double par[])
{
  m_RC  = par[0] ;
}

int        Sample::FitRes_PV3_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV3_Pad      ; }
const Pad* Sample::FitRes_PV3_Get_Pad      (int Index1D) const  { return V_FitRes_PV3_Pad     [Index1D] ; }
double     Sample::FitRes_PV3_Get_Residual (int Index1D) const  { return V_FitRes_PV3_Residual[Index1D] ; }
double     Sample::FitRes_PV3_Get_Pull     (int Index1D) const  { return V_FitRes_PV3_Pull    [Index1D] ; }


//---------------------PV4-----------------------------//
void   Sample::Beg_PV4()
{
  p_FitOutput_PV4 = new FitOutput ;
  p_FitOutput_PV4->Set(2) ;
  m_StatusFit_PV4 = 0 ;
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;
}
void   Sample::End_PV4()
{
  delete p_FitOutput_PV4 ; p_FitOutput_PV4 = 0 ;
}
int Sample::StatusFit_PV4(){return m_StatusFit_PV4 ;} 

void Sample::SetEval_PV4(TF1* pTF1, const int& ModuleNber)
{ 
  m_RC = pTF1->GetParameter(0) ;
  m_DD = pTF1->GetParameter(1) ;
  p_TF1_PV4 = pTF1 ; 
  m_ModuleNber_PV4 = ModuleNber ;
}

double Sample::Eval_PV4(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  p_TF1_PV4->SetParameter(0,m_RC);
  p_TF1_PV4->SetParameter(1,m_DD);
  if (Xin!=Xin){
    std::cout << " Sample::Eval_PV4 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_TF1_PV4->Eval(Xin*1.E2) ) ;
  }
}

int Sample::SetParameter_PV4(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

//
  int ier = 0 ; 
  
  ier = pTVirtualFitter->SetParameter(0, "RC" , m_RC , 1. , 10. , 150.  ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  ier = pTVirtualFitter->SetParameter(1, "DD" , m_DD , 1. , -2000. , 2000.  ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_RC_BeforeMinimisation =  m_RC ;
  m_DD_BeforeMinimisation =  m_DD ;
    
  return ier ;
}

void Sample::SetResults_PV4(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV4 = 0 ;
  m_StatusFit = 0 ;

  p_FitOutput_PV4->SetResults(pTVirtualFitter) ;
  
  m_RC  = p_FitOutput_PV4->p_par[0]      ;
  m_eRC = (p_FitOutput_PV4->p_eparplus[0]-p_FitOutput_PV4->p_eparminus[0])/2. ;
  
  m_DD  = p_FitOutput_PV4->p_par[1]      ;
  m_eDD = (p_FitOutput_PV4->p_eparplus[1]-p_FitOutput_PV4->p_eparminus[1])/2. ;
  
  m_Chi2Min = Chi2_PV4(p_FitOutput_PV4->p_par) ;

//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV4) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV4) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV4(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;

        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;
        
        V_FitRes_PV4_Pad     .push_back(pPad    );
        V_FitRes_PV4_Residual.push_back(Residual);
        V_FitRes_PV4_Pull    .push_back(Pull    );
      
      }
    }
  }
  m_NberOf_V_FitRes_PV4_Pad = V_FitRes_PV4_Pad.size() ;

}

void Sample::SetResults_FailledFit_PV4(const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Sample::SetResults_FailledFit_PV4  " << std::endl ;
  
  m_StatusFit_PV4 = 1 ;
  m_StatusFit = 1 ;

  int iPt_Max = 100 ;
  double DeltaRC = 10. ;
  
  double Chi2_cur_min    = 0. ;
  double Chi2_cur_min_RC = 0. ;
  int    Chi2_cur_min_First = 1 ;
  
  m_DD  = m_DD_BeforeMinimisation ;
  m_eDD = 500. ;

  double RC_Start = m_RC_BeforeMinimisation      ;
  double RC_End   = m_RC_BeforeMinimisation + DeltaRC ;
  
  for (int iPt = 1 ; iPt< iPt_Max ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[2]  ;
    par[0] = RC_cur ;
    par[1] = m_DD ;
    
    double Chi2_cur = Chi2_PV4(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "+RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }

  RC_End   = m_RC_BeforeMinimisation           ;
  RC_Start = m_RC_BeforeMinimisation - DeltaRC ;
  
  for (int iPt = 0 ; iPt< iPt_Max-1 ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[2]  ;
    par[0] = RC_cur ;
    par[1] = m_DD ;
    
    double Chi2_cur = Chi2_PV4(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "-RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }
  
  m_RC  = Chi2_cur_min_RC ;
  m_eRC = DeltaRC ;

  if (Verbose==1) {
    std::cout << " Sample::SetResults_FailledFit_PV4  After scanning" << std::endl ;
    std::cout << "   m_RC " << m_RC  
              << " +/-  "
              << " m_eRC "  << m_eRC 
              << std::endl ;
    std::cout << "   m_DD " << m_DD  
              << " +/-  "
              << " m_eDD "  << m_eDD 
              << std::endl ;
    std::cout << std::endl ;
  }
  
  m_Chi2Min = Chi2_cur_min ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("RC"  ) ; V_PARname .push_back("DD"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_RC  ) ; V_PAR     .push_back(m_DD  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eRC ) ; V_ePAR    .push_back(m_eDD ) ;
  
  p_FitOutput_PV4->SetResults(V_PARname,V_PAR,V_ePAR) ;

  p_TF1_PV4->SetParameter(0,m_RC ) ;
  p_TF1_PV4->SetParError (0,m_eRC) ;
  p_TF1_PV4->SetParameter(1,m_DD ) ;
  p_TF1_PV4->SetParError (1,m_eDD) ;

//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV4) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV4) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV4(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;

        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;
        
        V_FitRes_PV4_Pad     .push_back(pPad    );
        V_FitRes_PV4_Residual.push_back(Residual);
        V_FitRes_PV4_Pull    .push_back(Pull    );
      
      }
    }
  }
  m_NberOf_V_FitRes_PV4_Pad = V_FitRes_PV4_Pad.size() ;

}

double Sample::Chi2_PV4(double par[])
{
  SetParameters_Internal_PV4(par) ;
  
  double ToBeReturned = 0. ;
  
  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV4) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV4) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      double YTrack = pCluster->Get_YTrack()   ;
      
      double AT_cur = pCluster->Get_AT()  ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
       const Pad* pPad = pCluster->Get_Pad(iP);
      
        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction = AT_cur * Eval_PV4(YTrackYPad) ;
        
        double TheObserved = pPad->Get_AMax() ;

        double Residual = TheObserved - ThePrediction  ;
        
        double TheError = std::sqrt(TheObserved)  ;
//      double TheError = 6.  ;  // Const Amplitude Error

        double Pull = Residual / TheError ;

        ToBeReturned += Pull * Pull ;
      
      }
    }
  }

  return ToBeReturned ;
} 

void Sample::SetParameters_Internal_PV4(double par[])
{
  m_RC  = par[0] ;
  m_DD  = par[1] ;
}

int        Sample::FitRes_PV4_Get_NberOfTermsInChi2()            const  { return m_NberOf_V_FitRes_PV4_Pad      ; }
const Pad* Sample::FitRes_PV4_Get_Pad      (int Index1D) const  { return V_FitRes_PV4_Pad     [Index1D] ; }
double     Sample::FitRes_PV4_Get_Residual (int Index1D) const  { return V_FitRes_PV4_Residual[Index1D] ; }
double     Sample::FitRes_PV4_Get_Pull     (int Index1D) const  { return V_FitRes_PV4_Pull    [Index1D] ; }


//---------------------PV31-----------------------------//
void   Sample::Beg_PV31()
{
  p_FitOutput_PV31 = new FitOutput ;
  p_FitOutput_PV31->Set(1) ;
  m_StatusFit_PV31 = 0 ;
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;
  
  m_PV31_AddTime_Leading         = 0 ;
  m_PV31_AddTime_NextLeading     = 0 ;
  m_PV31_AddTime_NextNextLeading = 0 ;

}
void   Sample::End_PV31()
{
  delete p_FitOutput_PV31 ; p_FitOutput_PV31 = 0 ;
}
int Sample::StatusFit_PV31(){return m_StatusFit_PV31 ;} 

void Sample::SetEval_PV31(SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd, const int& ModuleNber)
{ 
  p_PV31_SC_VS_x_rc_dd_SET_dd = pSC_VS_x_rc_dd_SET_dd ; 
  m_ModuleNber_PV31 = ModuleNber ;
}

double Sample::Eval_Alpha_PV31(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Sample::Eval_Alpha_PV31 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV31_SC_VS_x_rc_dd_SET_dd->Eval_AlphaPRF(Xin*1.E2,m_RC) ) ;
  }
}

double Sample::Eval_Tau_PV31(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Sample::Eval_Tau_PV31 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV31_SC_VS_x_rc_dd_SET_dd->Eval_TauBinPRF(Xin*1.E2,m_RC) ) ;
  }
}

int Sample::SetParameter_PV31(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

//
  int ier = 0 ; 
  
  ier = pTVirtualFitter->SetParameter(0, "RC" , m_RC , 1. , 10. , 150.  ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_RC_BeforeMinimisation =  m_RC ;
    
  return ier ;
}

void Sample::SetResults_PV31(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV31 = 0 ;
  m_StatusFit = 0 ;

  p_FitOutput_PV31->SetResults(pTVirtualFitter) ;
  
  m_RC  = p_FitOutput_PV31->p_par[0]      ;
  m_eRC = (p_FitOutput_PV31->p_eparplus[0]-p_FitOutput_PV31->p_eparminus[0])/2. ;
  
  m_Chi2Min = Chi2_PV31(p_FitOutput_PV31->p_par) ;

//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV31) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV31) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;

      double YTrack = pCluster->Get_YTrack()   ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
         const Pad* pPad = pCluster->Get_Pad(iP);
    
//
        int Is_NextLeading =  0 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) Is_NextLeading = 1 ;
        }
        
        int Is_NextNextLeading =  0 ;
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) Is_NextNextLeading = 1 ;
        }
        if (
           Is_NextLeading!=1 &&  Is_NextNextLeading!=1
        ) continue ;
//

        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
        double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

        double Nume = pPad        ->Get_AMax() ;
        double Deno = pPad_Leading->Get_AMax()       ;
        double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;
    
//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeErrorPV31( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;

        V_FitRes_PV31_Pad     .push_back(pPad              );
        V_FitRes_PV31_Residual.push_back(Residual_Amplitude);
        V_FitRes_PV31_Pull    .push_back(Pull_Amplitude    );

//
        if (Is_NextLeading    !=1) m_PV31_AddTime_NextLeading     = 0 ;
        if (Is_NextNextLeading!=1) m_PV31_AddTime_NextNextLeading = 1 ;
        int PV31_AddTime = 0 ;
        if (
             m_PV31_AddTime_NextLeading    ==1 
          || m_PV31_AddTime_NextNextLeading==1 
        ) PV31_AddTime = 1 ;
//    
        if (PV31_AddTime==1){
          V_FitRes_PV31_Pad     .push_back(pPad         );
          V_FitRes_PV31_Residual.push_back(Residual_Time);
          V_FitRes_PV31_Pull    .push_back(Pull_Time    );
        }
      }
    }
  }
  m_NberOf_V_FitRes_PV31_Pad = V_FitRes_PV31_Pad.size() ;

}

void Sample::SetResults_FailledFit_PV31(const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Sample::SetResults_FailledFit_PV31  " << std::endl ;
  
  m_StatusFit_PV31 = 1 ;
  m_StatusFit = 1 ;

  int iPt_Max = 100 ;
  double DeltaRC = 10. ;
  
  double Chi2_cur_min    = 0. ;
  double Chi2_cur_min_RC = 0. ;
  int    Chi2_cur_min_First = 1 ;
  
  double RC_Start = m_RC_BeforeMinimisation      ;
  double RC_End   = m_RC_BeforeMinimisation + DeltaRC ;
  
  for (int iPt = 1 ; iPt< iPt_Max ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV31(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "+RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }

  RC_End   = m_RC_BeforeMinimisation           ;
  RC_Start = m_RC_BeforeMinimisation - DeltaRC ;
  
  for (int iPt = 0 ; iPt< iPt_Max-1 ; iPt++){
    double RC_cur = RC_Start + double(iPt)*(RC_End-RC_Start)/double(iPt_Max-1) ;
    
    double par[1]  ;
    par[0] = RC_cur ;
    
    double Chi2_cur = Chi2_PV31(par);
    
    if (Chi2_cur_min_First==1){
      Chi2_cur_min_First = 0 ;
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
    if (Chi2_cur_min>Chi2_cur){
      Chi2_cur_min    = Chi2_cur ;
      Chi2_cur_min_RC = RC_cur   ;
    }
//     std::cout 
//       << "-RC_cur "   << RC_cur 
//       << "Chi2_cur "  << Chi2_cur 
//       << std::endl ;
    
  }
  
  m_RC  = Chi2_cur_min_RC ;
  m_eRC = DeltaRC ;

  if (Verbose==1) {
    std::cout << " Sample::SetResults_FailledFit_PV31  After scanning" << std::endl ;
    std::cout << "   m_RC " << m_RC  
              << " +/-  "
              << " m_eRC "  << m_eRC 
              << std::endl ;
    std::cout << std::endl ;
  }
  
  m_Chi2Min = Chi2_cur_min ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("RC"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_RC  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eRC ) ;
  
  p_FitOutput_PV31->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV31) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV31) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;
            
      double YTrack = pCluster->Get_YTrack()   ;

      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
    
//
        int Is_NextLeading =  0 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) Is_NextLeading = 1 ;
        }
        
        int Is_NextNextLeading =  0 ;
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) Is_NextNextLeading = 1 ;
        }
        if (
           Is_NextLeading!=1 &&  Is_NextNextLeading!=1
        ) continue ;
//

        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
        double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

        double Nume = pPad        ->Get_AMax() ;
        double Deno = pPad_Leading->Get_AMax()       ;
        double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;

//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeErrorPV31( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;
        
        V_FitRes_PV31_Pad     .push_back(pPad              );
        V_FitRes_PV31_Residual.push_back(Residual_Amplitude);
        V_FitRes_PV31_Pull    .push_back(Pull_Amplitude    );

//
        int PV31_AddTime = 0 ;
        if (Is_NextLeading    ==1 && m_PV31_AddTime_NextLeading     == 1 ) PV31_AddTime = 1 ;
        if (Is_NextNextLeading==1 && m_PV31_AddTime_NextNextLeading == 1 ) PV31_AddTime = 1 ;
        if (PV31_AddTime==1){
          V_FitRes_PV31_Pad     .push_back(pPad         );
          V_FitRes_PV31_Residual.push_back(Residual_Time);
          V_FitRes_PV31_Pull    .push_back(Pull_Time    );
        }
      
      }
    }
  }
  m_NberOf_V_FitRes_PV31_Pad = V_FitRes_PV31_Pad.size() ;

}

double Sample::Chi2_PV31(double par[])
{
  SetParameters_Internal_PV31(par) ;
  
  double ToBeReturned = 0. ;
  
  for (int iE = 0 ; iE<m_NberOfEvents ; iE++) {
    Event* pEvent =  Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(m_ModuleNber_PV31) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (m_ModuleNber_PV31) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      const Pad* pPad_Leading         = pCluster->Get_LeadingPad()         ;
      const Pad* pPad_NextLeading     = pCluster->Get_NextLeadingPad()     ;
      const Pad* pPad_NextNextLeading = pCluster->Get_NextNextLeadingPad() ;
            
      double YTrack = pCluster->Get_YTrack()   ;

      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
    
//
        int Is_Leading =  0 ;
        if ( std::fabs( pPad->Get_iY() - pPad_Leading->Get_iY() ) == 0 ) Is_Leading = 1 ;

        int Is_NextLeading =  0 ;
        if ( pPad_NextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextLeading->Get_iY() ) == 0 ) Is_NextLeading = 1 ;
        }
        
        int Is_NextNextLeading =  0 ;
        if ( pPad_NextNextLeading ){
          if ( std::fabs( pPad->Get_iY() - pPad_NextNextLeading->Get_iY() ) == 0 ) Is_NextNextLeading = 1 ;
        }
        if (
           Is_Leading!=1 && Is_NextLeading!=1 &&  Is_NextNextLeading!=1
        ) continue ;
//

        double YTrackYPad = YTrack - pPad->Get_YPad() ;
        double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
        double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
        
        double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
        double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;

        double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
        double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

        double Nume = pPad        ->Get_AMax() ;
        double Deno = pPad_Leading->Get_AMax()       ;
        double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;

//      double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
        double TheError_Time = TimeErrorPV31( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);

        double Pull_Amplitude = Residual_Amplitude / TheError_Amplitude ;
        double Pull_Time      = Residual_Time      / TheError_Time      ;

        ToBeReturned += Pull_Amplitude * Pull_Amplitude ;
        
//
        int PV31_AddTime = 0 ;
        if (Is_Leading        ==1 && m_PV31_AddTime_Leading         == 1 ) PV31_AddTime = 1 ;
        if (Is_NextLeading    ==1 && m_PV31_AddTime_NextLeading     == 1 ) PV31_AddTime = 1 ;
        if (Is_NextNextLeading==1 && m_PV31_AddTime_NextNextLeading == 1 ) PV31_AddTime = 1 ;
        if (PV31_AddTime==1){
          ToBeReturned += Pull_Time * Pull_Time ;
        }

      
      }
    }
  }
  
//   std::cout 
//     << " m_RC " << m_RC 
//     << " ToBeReturned " << ToBeReturned 
//     << std::endl; 
  
  return ToBeReturned ;
} 

void Sample::SetParameters_Internal_PV31(double par[])
{
  m_RC  = par[0] ;
}

int        Sample::FitRes_PV31_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV31_Pad      ; }
const Pad* Sample::FitRes_PV31_Get_Pad      (int Index1D) const  { return V_FitRes_PV31_Pad     [Index1D] ; }
double     Sample::FitRes_PV31_Get_Residual (int Index1D) const  { return V_FitRes_PV31_Residual[Index1D] ; }
double     Sample::FitRes_PV31_Get_Pull     (int Index1D) const  { return V_FitRes_PV31_Pull    [Index1D] ; }


