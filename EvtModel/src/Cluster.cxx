#include "Cluster.h"
#include "Util.h"
#include "TimeError.h"
#include "AmplitudeError.h"

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

