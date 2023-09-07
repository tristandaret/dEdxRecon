#include "EvtModel/Cluster.h"
#include "Misc/Util.h"
#include "EvtModel/TimeError.h"
#include "EvtModel/AmplitudeError.h"

Cluster::Cluster(
           const int& EventNber ,
           const int& EntryNber ,
           const int& ModuleNber 
){
  m_EventNber  = EventNber ;
  m_EntryNber  = EntryNber ;
  m_ModuleNber = ModuleNber;

//
  Pad_Leading         = 0 ;
  Pad_NextLeading     = 0 ;
  Pad_NextNextLeading = 0 ;
  
  m_NberOfPads=0 ; 

  m_StatusFit = 0 ;

//
  Beg_PV0_PV1() ;
  Beg_PV2() ;
  Beg_PV3() ;
  Beg_PV31() ;
  Beg_PV4() ;
  Beg_PV0_Diag() ;
}

Cluster::~Cluster()
{
  V_Pad.clear();
//
  V_Pad.clear()     ; std::vector< const Pad* >().swap( V_Pad    );
//
//
  End_PV0_PV1() ;
  End_PV2() ;
  End_PV3() ;
  End_PV31() ;
  End_PV4() ;
  End_PV0_Diag() ;
}

int  Cluster::Get_EventNber () const { return m_EventNber ; }

int  Cluster::Get_EntryNber () const { return m_EntryNber ; }

int  Cluster::Get_ModuleNber () const { return m_ModuleNber ; }

void Cluster::Add_Pad(const Pad* pPad)
{
  V_Pad.push_back( pPad ) ;
  m_NberOfPads += 1 ;
}

void Cluster::DoClosure()
{
  m_Acluster = 0. ;
  m_XWeight = 0. ;
  m_YWeight = 0. ;
  int NberOfPads = Get_NberOfPads() ;

  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    const Pad*   pPad = Get_Pad (iPad) ;
    double SMax = pPad->Get_AMax() ;
    m_Acluster += SMax ;
    double XPad = pPad->Get_XPad() ;
    double YPad = pPad->Get_YPad() ;
    m_XWeight += XPad * SMax ;
    m_YWeight += YPad * SMax ;
  }
  m_XWeight = m_XWeight/m_Acluster ;
  m_YWeight = m_YWeight/m_Acluster ;
  m_XTrack = m_XWeight ;
  m_eXTrack = 0.001 ;
  m_YTrack = m_YWeight ;
  m_eYTrack = 0.001 ;

  std::vector < RankedValue > V_RankedValue ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    const Pad* pPad = Get_Pad (iPad) ;
    double SMax = pPad->Get_AMax() ;
    RankedValue aRankedValue ; 
    aRankedValue.Value = SMax ; 
    aRankedValue.Rank  = iPad       ; 
    V_RankedValue.push_back( aRankedValue );
  }
  std::sort(V_RankedValue.begin(), V_RankedValue.end());
  std::vector < const Pad* > L_Pad ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    const Pad*   pPad = Get_Pad (V_RankedValue[iPad].Rank) ;
    L_Pad.push_back(pPad); 
  }
  V_Pad.clear() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    V_Pad.push_back(L_Pad[iPad]); 
  }
  
  Pad_Leading = V_Pad[NberOfPads-1] ;
  
  double     Amax_Next     = 0. ;
  const Pad* pPad_Next     = 0  ;
  double     Amax_NextNext = 0. ;
  const Pad* pPad_NextNext = 0  ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    const Pad*  pPad = V_Pad[iPad] ;
    if ( std::fabs( pPad->Get_iY() - Pad_Leading->Get_iY() ) == 1 ){
      double AMax = pPad->Get_AMax() ;
      if (pPad_Next==0){
        pPad_Next = pPad ;
        Amax_Next = AMax ;
      }
      if (Amax_Next<AMax){
        pPad_Next = pPad ;
        Amax_Next = AMax ;
      }
      if (pPad_NextNext==0){
        pPad_NextNext = pPad ;
        Amax_NextNext = AMax ;
      }
      if (Amax_NextNext>AMax){
        pPad_NextNext = pPad ;
        Amax_NextNext = AMax ;
      }
    }
  }
  if (pPad_Next) Pad_NextLeading = pPad_Next ;
  if (pPad_NextNext) Pad_NextNextLeading = pPad_NextNext ;
  if (Pad_NextNextLeading){
    if ( Pad_NextNextLeading->Get_iY()==Pad_NextLeading->Get_iY()) Pad_NextNextLeading = 0 ;
  }
  

  m_AT  = Get_AMaxLeading() ;
  m_eAT = 0.1*m_AT ;
  m_TT  = Get_TMaxLeading() ;
  m_eTT = 0.1*m_TT ;

}

int  Cluster::Get_NberOfPads     ()      const  { return m_NberOfPads   ; }
const Pad* Cluster::Get_Pad(const int& Index1D) const  { return V_Pad[Index1D] ; }

double Cluster::Get_Acluster () const { return m_Acluster ; }
double Cluster::Get_XWeight  () const { return m_XWeight  ; }
double Cluster::Get_YWeight  () const { return m_YWeight  ; }

const Pad* Cluster::Get_LeadingPad() const { return Pad_Leading ; }
double Cluster::Get_YLeading     ()  const { return Pad_Leading->Get_YPad() ; }
double Cluster::Get_TMaxLeading  ()  const { return Pad_Leading->Get_TMax() ; }

const Pad* Cluster::Get_NextLeadingPad()     const { return Pad_NextLeading     ; }
const Pad* Cluster::Get_NextNextLeadingPad() const { return Pad_NextNextLeading ; }

double Cluster::Get_AMaxLeading  ()  const { return Pad_Leading->Get_AMax() ; }

void Cluster::WriteOut() const 
{
  std::cout << "StartCluster" << std::endl ;
  std::cout << " m_EventNber ; " << std::setw(16) << m_EventNber << std::endl ;
  std::cout << " m_EntryNber ; " << std::setw(16) << m_EntryNber << std::endl ;
//std::cout << " m_ClNber ; " << std::setw(16) << m_ClNber << std::endl ;
  for (int iPad = 0 ; iPad< Get_NberOfPads(); iPad++){
    const Pad*   pPad = V_Pad[iPad] ;
    int iX = pPad->Get_iX();
    int iY = pPad->Get_iY();
    std::cout << " iXiY       ; " << std::setw(16)                         << iX          
              <<           "  ; " << std::setw(16)                         << iY         << std::endl ;
  }
  std::cout << "EndCluster" << std::endl ;
}


//--------------------------------------------------------------------------//   
void Cluster::Set_XTrack(const double& XTrack){ m_XTrack = XTrack; }
void Cluster::Set_YTrack(const double& YTrack){ m_YTrack = YTrack; }
void Cluster::Set_AT    (const double& AT    ){ m_AT     = AT    ; }
void Cluster::Set_TT    (const double& TT    ){ m_TT     = TT    ; }

void Cluster::Set_eXTrack(const double& eXTrack){ m_eXTrack = eXTrack; }
void Cluster::Set_eYTrack(const double& eYTrack){ m_eYTrack = eYTrack; }

int Cluster::StatusFit(){ return m_StatusFit ; }

double Cluster::Get_XTrack  () const { return m_XTrack    ; }
double Cluster::Get_eXTrack () const { return m_eXTrack   ; }
double Cluster::Get_YTrack  () const { return m_YTrack    ; }
double Cluster::Get_eYTrack () const { return m_eYTrack   ; }
double Cluster::Get_AT      () const { return m_AT        ; }
double Cluster::Get_eAT     () const { return m_eAT       ; }
double Cluster::Get_TT      () const { return m_TT        ; }
double Cluster::Get_eTT     () const { return m_eTT       ; }

double Cluster::Get_Chi2Min () const { return m_Chi2Min   ; }
    
double Cluster::Get_XTrack_BeforeMinimisation () const { return m_XTrack_BeforeMinimisation ; }
double Cluster::Get_YTrack_BeforeMinimisation () const { return m_YTrack_BeforeMinimisation ; }
double Cluster::Get_AT_BeforeMinimisation     () const { return m_AT_BeforeMinimisation     ; }
double Cluster::Get_TT_BeforeMinimisation     () const { return m_TT_BeforeMinimisation     ; }

//---------------------PV0_PV1-----------------------------//
void   Cluster::Beg_PV0_PV1()
{
  p_FitOutput_PV0_PV1 = new FitOutput ;
  p_FitOutput_PV0_PV1->Set(1) ;
  m_StatusFit_PV0_PV1 = 0 ;
  m_NberOf_V_FitRes_PV0_PV1_Pad = 0 ;
  V_FitRes_PV0_PV1_Pad     .clear() ;
  V_FitRes_PV0_PV1_Residual.clear() ;
  V_FitRes_PV0_PV1_Pull    .clear() ;
}
void   Cluster::End_PV0_PV1()
{
  delete p_FitOutput_PV0_PV1 ; p_FitOutput_PV0_PV1 = 0 ;
//
  V_FitRes_PV0_PV1_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV0_PV1_Pad      );
  V_FitRes_PV0_PV1_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV0_PV1_Residual );
  V_FitRes_PV0_PV1_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV0_PV1_Pull     );
//
}
int Cluster::StatusFit_PV0_PV1(){ return m_StatusFit_PV0_PV1 ; }

void Cluster::SetEval_PV0_PV1(TF1* pTF1)
{ 
  p_TF1_PV0_PV1 = pTF1 ; 
}

double Cluster::Eval_PV0_PV1(const double& Xin)
{
//The function Evaluate has its argument in cm
//std::cout <<  " p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) << std::endl ;
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_PV0_PV1 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_TF1_PV0_PV1->Eval(Xin*1.E2) ) ;
  }
}

int Cluster::SetParameter_PV0_PV1(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV0_PV1_Pad = 0 ;
  V_FitRes_PV0_PV1_Pad     .clear() ;
  V_FitRes_PV0_PV1_Residual.clear() ;
  V_FitRes_PV0_PV1_Pull    .clear() ;

//
  int ier = 0 ; 
  
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , m_YTrack-0.10, m_YTrack+0.10 ) ;
  ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , 0. , 0. ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_YTrack_BeforeMinimisation =  m_YTrack ;
  
  return ier ;
}

void Cluster::SetResults_PV0_PV1(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV0_PV1 = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV0_PV1->SetResults(pTVirtualFitter) ;
    
  m_YTrack  = p_FitOutput_PV0_PV1->p_par[0]      ;
  m_eYTrack = (p_FitOutput_PV0_PV1->p_eparplus[0]-p_FitOutput_PV0_PV1->p_eparminus[0])/2. ;

  m_Chi2Min = Chi2_PV0_PV1(p_FitOutput_PV0_PV1->p_par) ;

//
  m_NberOf_V_FitRes_PV0_PV1_Pad = 0 ;
  V_FitRes_PV0_PV1_Pad     .clear() ;
  V_FitRes_PV0_PV1_Residual.clear() ;
  V_FitRes_PV0_PV1_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = Eval_PV0_PV1(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    V_FitRes_PV0_PV1_Pad     .push_back(pPad    );
    V_FitRes_PV0_PV1_Residual.push_back(Residual);
    V_FitRes_PV0_PV1_Pull    .push_back(Pull    );
        
  }
  m_NberOf_V_FitRes_PV0_PV1_Pad = V_FitRes_PV0_PV1_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV0_PV1 (const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV0_PV1  " << std::endl ;
  
  m_StatusFit_PV0_PV1 = 1 ;
  m_StatusFit    = 1 ;
  
  m_YTrack  = m_YTrack_BeforeMinimisation ;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  
  
  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ;
  
  p_FitOutput_PV0_PV1->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV0_PV1_Pad = 0 ;
  V_FitRes_PV0_PV1_Pad     .clear() ;
  V_FitRes_PV0_PV1_Residual.clear() ;
  V_FitRes_PV0_PV1_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = Eval_PV0_PV1(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    V_FitRes_PV0_PV1_Pad     .push_back(pPad    );
    V_FitRes_PV0_PV1_Residual.push_back(Residual);
    V_FitRes_PV0_PV1_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV0_PV1_Pad = V_FitRes_PV0_PV1_Pad.size() ;
  
}

double Cluster::Chi2_PV0_PV1(double par[])
{
  SetParameters_Internal_PV0_PV1(par) ;
  
  double ToBeReturned = 0. ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = Eval_PV0_PV1(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    ToBeReturned += Pull * Pull ;
    
  }
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV0_PV1(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV0_PV1 called with a NAN !" << std::endl ;
  }else{
    m_YTrack  = par[0] ;
  }
}

int        Cluster::FitRes_PV0_PV1_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV0_PV1_Pad      ; }
const Pad* Cluster::FitRes_PV0_PV1_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV0_PV1_Pad     [Index1D] ; }
double     Cluster::FitRes_PV0_PV1_Get_Residual (const int& Index1D) const  { return V_FitRes_PV0_PV1_Residual[Index1D] ; }
double     Cluster::FitRes_PV0_PV1_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV0_PV1_Pull    [Index1D] ; }

//---------------------PV2-----------------------------//
void   Cluster::Beg_PV2()
{
  p_FitOutput_PV2 = new FitOutput ;
  p_FitOutput_PV2->Set(2) ;
  m_StatusFit_PV2 = 0 ;
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;
}
void   Cluster::End_PV2()
{
  delete p_FitOutput_PV2 ; p_FitOutput_PV2 = 0 ;
//
  V_FitRes_PV2_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV2_Pad      );
  V_FitRes_PV2_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV2_Residual );
  V_FitRes_PV2_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV2_Pull     );
//
}
int Cluster::StatusFit_PV2(){ return m_StatusFit_PV2 ; }

void Cluster::SetEval_PV2(SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd)
{ 
  p_PV2_SC_VS_x_rc_dd_SET_rc_dd = pSC_VS_x_rc_dd_SET_rc_dd ; 
}

double Cluster::Eval_PV2(const double& Xin)
{
//The function Evaluate has its argument in cm
//std::cout <<  " p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) << std::endl ;
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_PV2 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV2_SC_VS_x_rc_dd_SET_rc_dd->Eval_AlphaPRF(Xin*1.E2) ) ;
  }
}

int Cluster::SetParameter_PV2(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

//
  int ier = 0 ; 
  
  double Start_YTrack = m_YTrack ;
  double Start_AT     = m_AT ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Start_YTrack-0.10, Start_YTrack+0.10 ) ;
  ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 0 " << std::endl;

///ier = pTVirtualFitter->SetParameter(1, "QT" , Start_AT , 6 ,0, 0 ) ;
  ier = pTVirtualFitter->SetParameter(1, "QT" , Start_AT , 6 ,0, 0 ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 1 " << std::endl;
  
  m_YTrack_BeforeMinimisation =  m_YTrack ;
  m_AT_BeforeMinimisation     =  m_AT     ;
  
  return ier ;
}

void Cluster::SetResults_PV2(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV2 = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV2->SetResults(pTVirtualFitter) ;
    
  m_YTrack  = p_FitOutput_PV2->p_par[0]      ;
  m_eYTrack = (p_FitOutput_PV2->p_eparplus[0]-p_FitOutput_PV2->p_eparminus[0])/2. ;

  m_AT      = p_FitOutput_PV2->p_par[1]      ;
  m_eAT     = (p_FitOutput_PV2->p_eparplus[1]-p_FitOutput_PV2->p_eparminus[1])/2. ;
  
  m_Chi2Min = Chi2_PV2(p_FitOutput_PV2->p_par) ;

//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV2(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    V_FitRes_PV2_Pad     .push_back(pPad    );
    V_FitRes_PV2_Residual.push_back(Residual);
    V_FitRes_PV2_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV2_Pad = V_FitRes_PV2_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV2 (const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV2  " << std::endl ;
  
  m_StatusFit_PV2 = 1 ;
  m_StatusFit    = 1 ;
  
  m_YTrack  = m_YTrack_BeforeMinimisation ;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  
  
  m_AT      = m_AT_BeforeMinimisation ;
  m_eAT     = m_AT  ;

  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ; V_PARname .push_back("QT"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ; V_PAR     .push_back(m_AT  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ; V_ePAR    .push_back(m_eAT ) ;
  
  p_FitOutput_PV2->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV2_Pad = 0 ;
  V_FitRes_PV2_Pad     .clear() ;
  V_FitRes_PV2_Residual.clear() ;
  V_FitRes_PV2_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV2(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    V_FitRes_PV2_Pad     .push_back(pPad    );
    V_FitRes_PV2_Residual.push_back(Residual);
    V_FitRes_PV2_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV2_Pad = V_FitRes_PV2_Pad.size() ;

}

double Cluster::Chi2_PV2(double par[])
{
  
  SetParameters_Internal_PV2(par) ;
  
  double ToBeReturned = 0. ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV2(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    ToBeReturned += Pull * Pull ;

  }
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV2(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV2 called with a NAN ! for par[0] " << std::endl ;
  }else{
    m_YTrack  = par[0] ;
  }
  
  if (par[1]!=par[1]){
//     std::cout << " Cluster::SetParameters_Internal_PV2 called with a NAN ! for par[1] " << std::endl ;
  }else{
    m_AT     = par[1];
  }
}

int        Cluster::FitRes_PV2_Get_NberOfTermsInChi2()           const  { return m_NberOf_V_FitRes_PV2_Pad      ; }
const Pad* Cluster::FitRes_PV2_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV2_Pad     [Index1D] ; }
double     Cluster::FitRes_PV2_Get_Residual (const int& Index1D) const  { return V_FitRes_PV2_Residual[Index1D] ; }
double     Cluster::FitRes_PV2_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV2_Pull    [Index1D] ; }

//---------------------PV3-----------------------------//
void   Cluster::Beg_PV3()
{
  p_FitOutput_PV3 = new FitOutput ;
  p_FitOutput_PV3->Set(3) ;
  m_StatusFit_PV3 = 0 ;
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;
}
void   Cluster::End_PV3()
{
  delete p_FitOutput_PV3 ; p_FitOutput_PV3 = 0 ;
//
  V_FitRes_PV3_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV3_Pad      );
  V_FitRes_PV3_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV3_Residual );
  V_FitRes_PV3_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV3_Pull     );
//
}
int Cluster::StatusFit_PV3(){ return m_StatusFit_PV3 ; }

void Cluster::SetEval_PV3(SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd)
{ 
  p_PV3_SC_VS_x_rc_dd_SET_rc_dd = pSC_VS_x_rc_dd_SET_rc_dd ; 
}

double Cluster::Eval_Alpha_PV3(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_Alpha_PV3 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV3_SC_VS_x_rc_dd_SET_rc_dd->Eval_AlphaPRF(Xin*1.E2) ) ;
  }
}

double Cluster::Eval_Tau_PV3(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_Tau_PV3 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV3_SC_VS_x_rc_dd_SET_rc_dd->Eval_TauBinPRF(Xin*1.E2) ) ;
//     double Pred_Analog = p_PV3_SC_VS_x_rc_dd_SET_rc_dd->Eval_TauBinPRF(Xin*1.E2) ;
//     int    Pred_Digital = std::floor (Pred_Analog) ;
//     return ( Pred_Digital ) ;
  }
}

int Cluster::SetParameter_PV3(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

//
  int ier = 0 ; 
  
  double Start_YTrack = m_YTrack ;
  double Start_AT     = m_AT ;
  double Start_TT     = m_TT ;
  
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Start_YTrack-0.10, Start_YTrack+0.10 ) ;
  ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 0 " << std::endl;

///ier = pTVirtualFitter->SetParameter(1, "QT" , Start_AT , 6 ,0, 0 ) ;
  ier = pTVirtualFitter->SetParameter(1, "QT" , Start_AT , 6 ,0, 0 ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 1 " << std::endl;
 
  ier = pTVirtualFitter->SetParameter(2, "TT" , Start_TT , 0.1 ,0, 0 ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 2 " << std::endl;
  
//pTVirtualFitter->FixParameter(2);
  
  m_YTrack_BeforeMinimisation =  m_YTrack ;
  m_AT_BeforeMinimisation     =  m_AT     ;
  m_TT_BeforeMinimisation     =  m_TT     ;
  
  return ier ;
}

void Cluster::SetResults_PV3(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV3 = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV3->SetResults(pTVirtualFitter) ;
    
  m_YTrack  = p_FitOutput_PV3->p_par[0]      ;
  m_eYTrack = (p_FitOutput_PV3->p_eparplus[0]-p_FitOutput_PV3->p_eparminus[0])/2. ;

  m_AT      = p_FitOutput_PV3->p_par[1]      ;
  m_eAT     = (p_FitOutput_PV3->p_eparplus[1]-p_FitOutput_PV3->p_eparminus[1])/2. ;

  m_TT      = p_FitOutput_PV3->p_par[2]      ;
  m_eTT     = (p_FitOutput_PV3->p_eparplus[2]-p_FitOutput_PV3->p_eparminus[2])/2. ;
  
  m_Chi2Min = Chi2_PV3(p_FitOutput_PV3->p_par) ;

//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  double AT_cur = Get_AT()  ;
  double TT_cur = Get_TT()  ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;

    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
    double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() ;

    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//  double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//  double TheError_Amplitude = 6. ;  // Const Amplitude Error
    double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
//      DoIt = 0 ;
    if (DoIt==1) {
      V_FitRes_PV3_Pad     .push_back(pPad         );
      V_FitRes_PV3_Residual.push_back(Residual_Time);
      V_FitRes_PV3_Pull    .push_back(Pull_Time    );
   }

  }
  m_NberOf_V_FitRes_PV3_Pad = V_FitRes_PV3_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV3 (const int& Verbose,TVirtualFitter* pTVirtualFitter )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV3  " << std::endl ;
  
  m_StatusFit_PV3 = 1 ;
  m_StatusFit    = 1 ;
  
  p_FitOutput_PV3->SetResults(pTVirtualFitter) ;

  m_YTrack  = m_YTrack_BeforeMinimisation ;
//m_YTrack  = p_FitOutput_PV3->p_par[0]      ;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  
  
  m_AT      = m_AT_BeforeMinimisation ;
//m_AT      = p_FitOutput_PV3->p_par[1]      ;
  m_eAT     = m_AT  ;

  m_TT      = m_TT_BeforeMinimisation ;
//m_TT      = p_FitOutput_PV3->p_par[2]      ;
  m_eTT     = m_TT  ;

  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ; V_PARname .push_back("QT"  )  ; V_PARname .push_back("TT"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ; V_PAR     .push_back(m_AT  )  ; V_PAR     .push_back(m_TT  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ; V_ePAR    .push_back(m_eAT )  ; V_ePAR    .push_back(m_eTT ) ;
    
  p_FitOutput_PV3->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV3_Pad = 0 ;
  V_FitRes_PV3_Pad     .clear() ;
  V_FitRes_PV3_Residual.clear() ;
  V_FitRes_PV3_Pull    .clear() ;

  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  double AT_cur = Get_AT()  ;
  double TT_cur = Get_TT()  ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
    double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() ;
    
    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//  double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//  double TheError_Amplitude = 6. ;  // Const Amplitude Error
    double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
//      DoIt = 0 ;
    if (DoIt==1) {
      V_FitRes_PV3_Pad     .push_back(pPad         );
      V_FitRes_PV3_Residual.push_back(Residual_Time);
      V_FitRes_PV3_Pull    .push_back(Pull_Time    );
    }

  }
  m_NberOf_V_FitRes_PV3_Pad = V_FitRes_PV3_Pad.size() ;

}

double Cluster::Chi2_PV3(double par[])
{
  
  SetParameters_Internal_PV3(par) ;
  
  double ToBeReturned = 0. ;
  
  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  double AT_cur = Get_AT()  ;
  double TT_cur = Get_TT()  ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV3(YTrackYPad) * AT_cur ;
    double ThePrediction_Time      = Eval_Tau_PV3  (YTrackYPad) + TT_cur ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() ;
    
    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

//  double TheError_Amplitude = std::sqrt(TheObserved_Amplitude) ;
//  double TheError_Amplitude = 6. ;  // Const Amplitude Error
    double TheError_Amplitude = AmplitudeError( pPad ,pPad_Leading,pPad_NextLeading,pPad_NextNextLeading);
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
//      DoIt = 0 ;
    if (DoIt==1) ToBeReturned += Pull_Time * Pull_Time ;

  }
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV3(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV3 called with a NAN ! for par[0] " << std::endl ;
  }else{
    m_YTrack  = par[0] ;
  }
  
  if (par[1]!=par[1]){
//     std::cout << " Cluster::SetParameters_Internal_PV3 called with a NAN ! for par[1] " << std::endl ;
  }else{
    m_AT     = par[1];
  }
  
  if (par[2]!=par[2]){
//     std::cout << " Cluster::SetParameters_Internal_PV3 called with a NAN ! for par[2] " << std::endl ;
  }else{
    m_TT     = par[2];
  }
}

int        Cluster::FitRes_PV3_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV3_Pad      ; }
const Pad* Cluster::FitRes_PV3_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV3_Pad     [Index1D] ; }
double     Cluster::FitRes_PV3_Get_Residual (const int& Index1D) const  { return V_FitRes_PV3_Residual[Index1D] ; }
double     Cluster::FitRes_PV3_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV3_Pull    [Index1D] ; }


//---------------------PV4-----------------------------//
void   Cluster::Beg_PV4()
{
  p_FitOutput_PV4 = new FitOutput ;
  p_FitOutput_PV4->Set(2) ;
  m_StatusFit_PV4 = 0 ;
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;
}
void   Cluster::End_PV4()
{
  delete p_FitOutput_PV4 ; p_FitOutput_PV4 = 0 ;
//
  V_FitRes_PV4_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV4_Pad      );
  V_FitRes_PV4_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV4_Residual );
  V_FitRes_PV4_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV4_Pull     );
//
}
int Cluster::StatusFit_PV4(){ return m_StatusFit_PV4 ; }

void Cluster::SetEval_PV4(TF1* pTF1)
{ 
  p_TF1_PV4 = pTF1 ; 
}

double Cluster::Eval_PV4(const double& Xin)
{
//The function Evaluate has its argument in cm
//std::cout <<  " p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) << std::endl ;
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_PV4 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_TF1_PV4->Eval(Xin*1.E2) ) ;
  }
}

int Cluster::SetParameter_PV4(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

//
  int ier = 0 ; 
  
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , m_YTrack-0.10, m_YTrack+0.10 ) ;
  ier = pTVirtualFitter->SetParameter(0, "YTrack" , m_YTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;

///ier = pTVirtualFitter->SetParameter(1, "QT" , m_AT , 6 ,0, 0 ) ;
  ier = pTVirtualFitter->SetParameter(1, "QT" , m_AT , 6 ,0, 0 ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_YTrack_BeforeMinimisation =  m_YTrack ;
  m_AT_BeforeMinimisation     =  m_AT     ;
  
  return ier ;
}

void Cluster::SetResults_PV4(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV4 = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV4->SetResults(pTVirtualFitter) ;
    
  m_YTrack  = p_FitOutput_PV4->p_par[0]      ;
  m_eYTrack = (p_FitOutput_PV4->p_eparplus[0]-p_FitOutput_PV4->p_eparminus[0])/2. ;

  m_AT      = p_FitOutput_PV4->p_par[1]      ;
  m_eAT     = (p_FitOutput_PV4->p_eparplus[1]-p_FitOutput_PV4->p_eparminus[1])/2. ;
  
  m_Chi2Min = Chi2_PV4(p_FitOutput_PV4->p_par) ;

//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV4(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    V_FitRes_PV4_Pad     .push_back(pPad    );
    V_FitRes_PV4_Residual.push_back(Residual);
    V_FitRes_PV4_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV4_Pad = V_FitRes_PV4_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV4 (const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV4  " << std::endl ;
  
  m_StatusFit_PV4 = 1 ;
  m_StatusFit    = 1 ;
  
  m_YTrack  = m_YTrack_BeforeMinimisation ;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  
  
  m_AT      = m_AT_BeforeMinimisation ;
  m_eAT     = m_AT  ;

  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ; V_PARname .push_back("QT"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ; V_PAR     .push_back(m_AT  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ; V_ePAR    .push_back(m_eAT ) ;
  
  p_FitOutput_PV4->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV4_Pad = 0 ;
  V_FitRes_PV4_Pad     .clear() ;
  V_FitRes_PV4_Residual.clear() ;
  V_FitRes_PV4_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV4(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    V_FitRes_PV4_Pad     .push_back(pPad    );
    V_FitRes_PV4_Residual.push_back(Residual);
    V_FitRes_PV4_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV4_Pad = V_FitRes_PV4_Pad.size() ;

}

double Cluster::Chi2_PV4(double par[])
{
  
  SetParameters_Internal_PV4(par) ;
  
  double ToBeReturned = 0. ;
  
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction = m_AT*Eval_PV4(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax() ;
    
    double Residual = TheObserved - ThePrediction  ;

    double TheError = std::sqrt(TheObserved)  ;
//  double TheError = 6.  ;  // Const Amplitude Error

    double Pull = Residual / TheError ;

    ToBeReturned += Pull * Pull ;

  }
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV4(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV4 called with a NAN ! for par[0] " << std::endl ;
  }else{
    m_YTrack  = par[0] ;
  }
  
  if (par[1]!=par[1]){
//     std::cout << " Cluster::SetParameters_Internal_PV4 called with a NAN ! for par[1] " << std::endl ;
  }else{
    m_AT     = par[1];
  }
}

int        Cluster::FitRes_PV4_Get_NberOfTermsInChi2()            const  { return m_NberOf_V_FitRes_PV4_Pad      ; }
const Pad* Cluster::FitRes_PV4_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV4_Pad     [Index1D] ; }
double     Cluster::FitRes_PV4_Get_Residual (const int& Index1D) const  { return V_FitRes_PV4_Residual[Index1D] ; }
double     Cluster::FitRes_PV4_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV4_Pull    [Index1D] ; }

//---------------------PV31-----------------------------//
void   Cluster::Beg_PV31()
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
void   Cluster::End_PV31()
{
  delete p_FitOutput_PV31 ; p_FitOutput_PV31 = 0 ;
//
  V_FitRes_PV31_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV31_Pad      );
  V_FitRes_PV31_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV31_Residual );
  V_FitRes_PV31_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV31_Pull     );
//
}
int Cluster::StatusFit_PV31(){ return m_StatusFit_PV31 ; }

void Cluster::SetEval_PV31(SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd)
{ 
  p_PV31_SC_VS_x_rc_dd_SET_rc_dd = pSC_VS_x_rc_dd_SET_rc_dd ; 
}

double Cluster::Eval_Alpha_PV31(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_Alpha_PV31 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV31_SC_VS_x_rc_dd_SET_rc_dd->Eval_AlphaPRF(Xin*1.E2) ) ;
  }
}

double Cluster::Eval_Tau_PV31(
                     const double& Xin
){
//The function Evaluate has its argument in cm
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_Tau_PV31 called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_PV31_SC_VS_x_rc_dd_SET_rc_dd->Eval_TauBinPRF(Xin*1.E2) ) ;
//     double Pred_Analog = p_PV31_SC_VS_x_rc_dd_SET_rc_dd->Eval_TauBinPRF(Xin*1.E2) ;
//     int    Pred_Digital = std::floor (Pred_Analog) ;
//     return ( Pred_Digital ) ;
  }
}

int Cluster::SetParameter_PV31(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

//
  int ier = 0 ; 
  
  double Start_YTrack = m_YTrack ;
  
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Start_YTrack-0.10, Start_YTrack+0.10 ) ;
  ier = pTVirtualFitter->SetParameter(0, "YTrack" , Start_YTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter 0 " << std::endl;

  m_YTrack_BeforeMinimisation =  m_YTrack ;
  
  return ier ;
}

void Cluster::SetResults_PV31(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV31 = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV31->SetResults(pTVirtualFitter) ;
    
  m_YTrack  = p_FitOutput_PV31->p_par[0]      ;
  m_eYTrack = (p_FitOutput_PV31->p_eparplus[0]-p_FitOutput_PV31->p_eparminus[0])/2. ;

  m_Chi2Min = Chi2_PV31(p_FitOutput_PV31->p_par) ;

//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
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

    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
    double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;

    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

    double Nume = pPad        ->Get_AMax() ;
    double Deno = pPad_Leading->Get_AMax()       ;
    double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
  m_NberOf_V_FitRes_PV31_Pad = V_FitRes_PV31_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV31 (const int& Verbose,TVirtualFitter* pTVirtualFitter )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV31  " << std::endl ;
  
  m_StatusFit_PV31 = 1 ;
  m_StatusFit    = 1 ;
  
  p_FitOutput_PV31->SetResults(pTVirtualFitter) ;

  m_YTrack  = m_YTrack_BeforeMinimisation ;
//m_YTrack  = p_FitOutput_PV31->p_par[0]      ;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  

  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ; 
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ;
    
  p_FitOutput_PV31->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV31_Pad = 0 ;
  V_FitRes_PV31_Pad     .clear() ;
  V_FitRes_PV31_Residual.clear() ;
  V_FitRes_PV31_Pull    .clear() ;

  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
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

    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
    double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;
    
    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

    double Nume = pPad        ->Get_AMax() ;
    double Deno = pPad_Leading->Get_AMax()       ;
    double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
  m_NberOf_V_FitRes_PV31_Pad = V_FitRes_PV31_Pad.size() ;

}

double Cluster::Chi2_PV31(double par[])
{
  
  SetParameters_Internal_PV31(par) ;
  
  double ToBeReturned = 0. ;
  
  const Pad* pPad_Leading         = Get_LeadingPad()         ;
  const Pad* pPad_NextLeading     = Get_NextLeadingPad()     ;
  const Pad* pPad_NextNextLeading = Get_NextNextLeadingPad() ;
            
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
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

    double YTrackYPad  = m_YTrack - pPad->Get_YPad() ;
    double ThePrediction_Amplitude = Eval_Alpha_PV31(YTrackYPad) ;
    double ThePrediction_Time      = Eval_Tau_PV31  (YTrackYPad) ;
    
    double TheObserved_Amplitude = pPad->Get_AMax() / pPad_Leading->Get_AMax() ;
    double TheObserved_Time      = pPad->Get_TMax() - pPad_Leading->Get_TMax() ;
    
    double Residual_Amplitude = TheObserved_Amplitude - ThePrediction_Amplitude  ;
    double Residual_Time      = TheObserved_Time      - ThePrediction_Time       ;

    double Nume = pPad        ->Get_AMax() ;
    double Deno = pPad_Leading->Get_AMax()       ;
    double TheError_Amplitude = TheObserved_Amplitude * std::sqrt( 1./Nume +  1./Deno )  ;
    
//  double TheError_Time = TimeError( TheObserved_Time - pPad_Leading->Get_TMax());
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
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV31(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV31 called with a NAN ! for par[0] " << std::endl ;
  }else{
    m_YTrack  = par[0] ;
  }
}

int        Cluster::FitRes_PV31_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV31_Pad      ; }
const Pad* Cluster::FitRes_PV31_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV31_Pad     [Index1D] ; }
double     Cluster::FitRes_PV31_Get_Residual (const int& Index1D) const  { return V_FitRes_PV31_Residual[Index1D] ; }
double     Cluster::FitRes_PV31_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV31_Pull    [Index1D] ; }


//---------------------PV0_Diag-----------------------------//
void   Cluster::Beg_PV0_Diag()
{
  m_AngleRot = M_PI/4. ;
  p_FitOutput_PV0_Diag = new FitOutput ;
  p_FitOutput_PV0_Diag->Set(1) ;
  m_StatusFit_PV0_Diag = 0 ;
  m_NberOf_V_FitRes_PV0_Diag_Pad = 0 ;
  V_FitRes_PV0_Diag_Pad     .clear() ;
  V_FitRes_PV0_Diag_Residual.clear() ;
  V_FitRes_PV0_Diag_Pull    .clear() ;
}
void   Cluster::End_PV0_Diag()
{
  delete p_FitOutput_PV0_Diag ; p_FitOutput_PV0_Diag = 0 ;
//
  V_FitRes_PV0_Diag_Pad     .clear() ; std::vector< const Pad* >().swap( V_FitRes_PV0_Diag_Pad      );
  V_FitRes_PV0_Diag_Residual.clear() ; std::vector< double     >().swap( V_FitRes_PV0_Diag_Residual );
  V_FitRes_PV0_Diag_Pull    .clear() ; std::vector< double     >().swap( V_FitRes_PV0_Diag_Pull     );
//
}
int Cluster::StatusFit_PV0_Diag(){ return m_StatusFit_PV0_Diag ; }

void Cluster::SetEval_PV0_Diag(TF1* pTF1)
{ 
  p_TF1_PV0_Diag = pTF1 ; 
}

double Cluster::Eval_PV0_Diag(const double& Xin)
{
//The function Evaluate has its argument in cm
//std::cout <<  " p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) << std::endl ;
  if (Xin!=Xin){
    std::cout << " Cluster::Eval_PV0_Diag called with a NAN !" << std::endl ;
    return 1.E99 ;
  }else{
    return ( p_TF1_PV0_Diag->Eval(Xin*1.E2) ) ;
  }
}

int Cluster::SetParameter_PV0_Diag(TVirtualFitter* pTVirtualFitter)
{
//
  m_NberOf_V_FitRes_PV0_Diag_Pad = 0 ;
  V_FitRes_PV0_Diag_Pad     .clear() ;
  V_FitRes_PV0_Diag_Residual.clear() ;
  V_FitRes_PV0_Diag_Pull    .clear() ;

//
  int ier = 0 ; 
  
  double CosRot = std::cos(m_AngleRot) ;
  double SinRot = std::sin(m_AngleRot) ;
  double Uref =  CosRot * m_XWeight +  SinRot * m_YWeight ;
  double Vref = -SinRot * m_XWeight +  CosRot * m_YWeight ;
  m_VTrack = Vref ;
  
//ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , Pad_Leading->Get_YL(), Pad_Leading->Get_YH() ) ;
//ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , m_VTrack-0.10, m_VTrack+0.10 ) ;
//ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , Pad_Leading->Get_YL() - (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() + (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) ) ;
  ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , Vref - 5.E-2, Vref + 5.E-2 ) ;
//ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , 0. , 0. ) ;
  if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << std::endl;
  
  m_XTrack_BeforeMinimisation =  CosRot * Uref - SinRot * m_VTrack ;
  m_YTrack_BeforeMinimisation =  SinRot * Uref + CosRot * m_VTrack ;
  
  return ier ;
}

void Cluster::SetResults_PV0_Diag(TVirtualFitter* pTVirtualFitter)
{
  m_StatusFit_PV0_Diag = 0 ;
  m_StatusFit    = 0 ;

  p_FitOutput_PV0_Diag->SetResults(pTVirtualFitter) ;
    
  m_VTrack  = p_FitOutput_PV0_Diag->p_par[0]      ;

  double CosRot = std::cos(m_AngleRot) ;
  double SinRot = std::sin(m_AngleRot) ;
  double Uref = CosRot * Get_LeadingPad()-> Get_XPad() +  SinRot * Get_LeadingPad()-> Get_YPad() ;
  m_XTrack =  CosRot * Uref -  SinRot * m_VTrack ;
  m_YTrack =  SinRot * Uref +  CosRot * m_VTrack ;

  m_eXTrack = (p_FitOutput_PV0_Diag->p_eparplus[0]-p_FitOutput_PV0_Diag->p_eparminus[0])/2. ;
  m_eYTrack = m_eXTrack ;
  
  m_Chi2Min = Chi2_PV0_Diag(p_FitOutput_PV0_Diag->p_par) ;

//
  m_NberOf_V_FitRes_PV0_Diag_Pad = 0 ;
  V_FitRes_PV0_Diag_Pad     .clear() ;
  V_FitRes_PV0_Diag_Residual.clear() ;
  V_FitRes_PV0_Diag_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;

//
    double XLocal = pPad->Get_XPad() ;
    double YLocal = pPad->Get_YPad() ;
    
//  double ULocal =  CosRot * XLocal + SinRot * YLocal ;
    double VLocal = -SinRot * XLocal + CosRot * YLocal ;
    
    double YTrackYPad = m_VTrack - VLocal ;
//    
    double ThePrediction = Eval_PV0_Diag(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    V_FitRes_PV0_Diag_Pad     .push_back(pPad    );
    V_FitRes_PV0_Diag_Residual.push_back(Residual);
    V_FitRes_PV0_Diag_Pull    .push_back(Pull    );
        
  }
  m_NberOf_V_FitRes_PV0_Diag_Pad = V_FitRes_PV0_Diag_Pad.size() ;

}

void   Cluster::SetResults_FailledFit_PV0_Diag (const int& Verbose )
{
  if (Verbose==1) 
  std::cout << " Cluster::SetResults_FailledFit_PV0_Diag  " << std::endl ;
  
  m_StatusFit_PV0_Diag = 1 ;
  m_StatusFit    = 1 ;
  
  m_XTrack  = m_XTrack_BeforeMinimisation ;
  m_YTrack  = m_YTrack_BeforeMinimisation ;
  m_eXTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL() ;  
  m_eYTrack = m_eXTrack ;  
  
  double CosRot = std::cos(m_AngleRot) ;
  double SinRot = std::sin(m_AngleRot) ;
//double Uref =  CosRot * m_XTrack +  SinRot * m_YTrack ;
  double Vref = -SinRot * m_XTrack +  CosRot * m_YTrack ;
  m_VTrack = Vref ;
  m_Chi2Min = 1.E9 ;
  
  std::vector < std::string > V_PARname ; V_PARname .push_back("YTrack"  ) ;
  std::vector < double      > V_PAR     ; V_PAR     .push_back(m_YTrack  ) ;
  std::vector < double      > V_ePAR    ; V_ePAR    .push_back(m_eYTrack ) ;
  
  p_FitOutput_PV0_Diag->SetResults(V_PARname,V_PAR,V_ePAR) ;

//
  m_NberOf_V_FitRes_PV0_Diag_Pad = 0 ;
  V_FitRes_PV0_Diag_Pad     .clear() ;
  V_FitRes_PV0_Diag_Residual.clear() ;
  V_FitRes_PV0_Diag_Pull    .clear() ;

  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
//
    double XLocal = pPad->Get_XPad() ;
    double YLocal = pPad->Get_YPad() ;
    
//  double ULocal =  CosRot * XLocal + SinRot * YLocal ;
    double VLocal = -SinRot * XLocal + CosRot * YLocal ;
    
    double YTrackYPad = m_VTrack - VLocal ;
//    
    double ThePrediction = Eval_PV0_Diag(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    V_FitRes_PV0_Diag_Pad     .push_back(pPad    );
    V_FitRes_PV0_Diag_Residual.push_back(Residual);
    V_FitRes_PV0_Diag_Pull    .push_back(Pull    );

  }
  m_NberOf_V_FitRes_PV0_Diag_Pad = V_FitRes_PV0_Diag_Pad.size() ;
  
}

double Cluster::Chi2_PV0_Diag(double par[])
{
  SetParameters_Internal_PV0_Diag(par) ;
  
  double ToBeReturned = 0. ;
  
  double CosRot = std::cos(m_AngleRot) ;
  double SinRot = std::sin(m_AngleRot) ;
 
  int NberOfPad = V_Pad.size() ;
  for (int iPad = 0 ; iPad<NberOfPad ; iPad++) {
    const Pad* pPad = V_Pad[iPad] ;
    
//
    double XLocal = pPad->Get_XPad() ;
    double YLocal = pPad->Get_YPad() ;
    
//  double ULocal =  CosRot * XLocal + SinRot * YLocal ;
    double VLocal = -SinRot * XLocal + CosRot * YLocal ;
    
    double YTrackYPad = m_VTrack - VLocal ;
//    
    double ThePrediction = Eval_PV0_Diag(YTrackYPad) ;
    
    double TheObserved = pPad->Get_AMax()/m_Acluster ;
    
    double Residual = TheObserved - ThePrediction  ;
    
    double Nume = pPad->Get_AMax() ;
    double Deno = m_Acluster       ;
    
    double Error_Nume = std::sqrt(std::fabs(Nume))  ;
    double Error_Deno = std::sqrt(std::fabs(Deno))  ;
    
//  double Error_Nume = 6. ;  // Const Amplitude Error
//  double Error_Deno = 6. ;  // Const Amplitude Error
    
    double TheError = TheObserved *  std::sqrt( std::pow(Error_Nume/Nume, 2 ) + std::pow(Error_Deno/Deno, 2 ) ) ;
    
    double Pull = Residual / TheError ;
    
    ToBeReturned += Pull * Pull ;
    
  }
  
  return ToBeReturned ;
} 

void Cluster::SetParameters_Internal_PV0_Diag(double par[])
{
  if (par[0]!=par[0]){
//     std::cout << " Cluster::SetParameters_Internal_PV0_Diag called with a NAN !" << std::endl ;
  }else{
    m_VTrack  = par[0] ;
  }
}

int        Cluster::FitRes_PV0_Diag_Get_NberOfTermsInChi2()    const  { return m_NberOf_V_FitRes_PV0_Diag_Pad      ; }
const Pad* Cluster::FitRes_PV0_Diag_Get_Pad      (const int& Index1D) const  { return V_FitRes_PV0_Diag_Pad     [Index1D] ; }
double     Cluster::FitRes_PV0_Diag_Get_Residual (const int& Index1D) const  { return V_FitRes_PV0_Diag_Residual[Index1D] ; }
double     Cluster::FitRes_PV0_Diag_Get_Pull     (const int& Index1D) const  { return V_FitRes_PV0_Diag_Pull    [Index1D] ; }

