#include "EvtModel/Track.h"
#include "Misc/Util.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TFile.h"

Track::Track(
         const int& EventNber ,
         const int& EntryNber ,
         const int& ModuleNber
){
  m_EventNber  = EventNber  ; 
  m_EntryNber  = EntryNber  ; 
  m_ModuleNber = ModuleNber ; 
  
  p_FitOutput=0 ;
  SetNberOfParameters(2) ;
  
}

Track::~Track()
{
 int VerboseDelete = 0 ;
 if (VerboseDelete==1)
 std::cout 
       << " Track Destructor "
       << std::endl ;
  delete p_FitOutput ;
  p_FitOutput = 0 ;
  V_Cluster  .clear() ;
  V_Residual .clear() ;
  V_Pull     .clear() ;
}

int Track::Get_EventNber  () const { return m_EventNber  ; }
int Track::Get_EntryNber  () const { return m_EntryNber  ; }
int Track::Get_ModuleNber () const { return m_ModuleNber ; }

void Track::SetNberOfParameters(const int& NberOfParam ) 
{ 
  m_NberOfParam = NberOfParam ;
  if ( m_NberOfParam <= 0 ){
    std::cout 
      << " Track::SetNberOfParameters : "
      << " m_NberOfParam <= 0 "
      << " m_NberOfParam = " << m_NberOfParam
      << std::endl ;
      abort() ;
  }

  delete p_FitOutput ;
  p_FitOutput = new FitOutput ;
  p_FitOutput->Set(m_NberOfParam) ;

  m_ParameterName               .clear() ;
  m_Parameter                   .clear() ;
  m_eParameter                  .clear() ;
  m_Parameter_BeforeMinimisation.clear() ;
  
  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "p" << iPar ;
    std::string ParameterName = aostringstream.str() ;
    m_ParameterName.push_back( ParameterName ) ;
    
    m_Parameter.push_back( 0.) ;
    if (iPar==0){
      m_eParameter.push_back( 0.001) ;
    }else{
      m_eParameter.push_back( 1) ;
    }
  
    m_Parameter_BeforeMinimisation.push_back( 0.) ;

  }

//   int iParMax = m_NberOfParam ;
//   std::cout << " iParMax " << iParMax << std::endl ;
//   for (int iPar = 0 ; iPar< iParMax; iPar++){
//    std::cout 
//      << " "     << m_ParameterName[iPar]
//      << " = "   << m_Parameter[iPar]
//      << " +/- " << m_eParameter[iPar]
//      << std::endl ;
//   }


}
int Track::GetNberOfParameters() const { return m_NberOfParam ; } 

std::string Track::Get_ParameterName (const int& iPar) const { return m_ParameterName[iPar] ; }
double      Track::Get_ParameterValue(const int& iPar) const { return m_Parameter    [iPar] ; }
double      Track::Get_ParameterError(const int& iPar) const { return m_eParameter   [iPar] ; }

double      Track::Get_ParameterValue_BeforeMinimisation(const int& iPar) const { return m_Parameter_BeforeMinimisation[iPar] ; }

double Track::Y_Position( const double& X ) const
{
  double ToBeReturned = 0. ;

  double XN = 1. ;
  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
    ToBeReturned += m_Parameter[iPar] * XN ;
    XN = XN * X ;
  }
  return ToBeReturned  ;
}

void Track::Add_Cluster(Cluster* pCluster)
{ 
  V_Cluster.push_back(pCluster);  
}

int            Track::Get_NberOfCluster()              const { return V_Cluster.size()    ; }
const Cluster* Track::Get_Cluster        (int Index1D) const { return V_Cluster[Index1D]  ; }
double         Track::Get_Residual       (int Index1D) const { return V_Residual[Index1D] ; }
double         Track::Get_Pull           (int Index1D) const { return V_Pull[Index1D]     ; }

void Track::DoClosure()
{
  m_NberOfClusters = V_Cluster.size() ;
  if (m_NberOfClusters==0){
    std::cout 
      << " Track::DoClosure: "
      << " m_NberOfClusters==0 "
      << std::endl ;
    abort() ;
  }
  Cluster* pCluster_First = V_Cluster[0] ;
  Cluster* pCluster_Last  = V_Cluster[m_NberOfClusters-1] ;
  
  double X_First = pCluster_First->Get_XTrack() ;
  double X_Last  = pCluster_Last ->Get_XTrack() ;
  double Y_First = pCluster_First->Get_YTrack() ;
  double Y_Last  = pCluster_Last ->Get_YTrack() ;
  
  m_Parameter[0] = ( Y_First * X_Last - Y_Last * X_First )/( X_Last - X_First ) ;

  if (m_NberOfParam >= 2)
  m_Parameter[1] = ( Y_Last - Y_First )/( X_Last - X_First ) ;

}

void Track::DumpRec() const
{
  std::cout 
    << " EventNber " << std::setw(16) << m_EventNber
    << " EntryNber " << std::setw(16) << m_EntryNber
    << std::endl ;

  std::cout 
    << " Chi2Min " << std::setw(16) << std::setprecision(3) << Get_Chi2Min()
    << std::endl ;

  std::cout 
    << " NberOfParam " << std::setw(16) << m_NberOfParam
    << std::endl ;

  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
   std::cout 
     << "Parameter: "     << m_ParameterName[iPar]
     << " = "   << std::setw(16) << std::setprecision(3) << m_Parameter[iPar]  * 1.E3
     << " +/- " << std::setw(16) << std::setprecision(3) << m_eParameter[iPar] * 1.E3
     << std::endl ;
  }

  std::cout 
    << " NberOfCluster " << std::setw(16) << Get_NberOfCluster()
    << std::endl ;
  int NClusters = Get_NberOfCluster();
  for (int iC = 0 ; iC< NClusters; iC++){
    const Cluster* pCluster = Get_Cluster(iC);
    double ThePosition_X = pCluster ->Get_XTrack() ;
    double ThePrediction = Y_Position(ThePosition_X)  ;
    std::cout
     << " Y "   << std::setw(16) << std::setprecision(3) <<  pCluster->Get_YTrack()  * 1.E3
     << " Y "   << std::setw(16) << std::setprecision(3) <<  ThePrediction           * 1.E3
     << " Res " << std::setw(16) << std::setprecision(3) <<  Get_Residual(iC)        * 1.E3
     << " +/- " << std::setw(16) << std::setprecision(3) <<  pCluster->Get_eYTrack() * 1.E3
     << std::endl ;
  }
    
    
}


//--------------------------------------------------------------------------//   
double Track::Get_Chi2Min     () const{ return m_Chi2Min   ; }
TMatrixD Track::Get_CovMatrix () const{ return m_covmatrix ; }

//---------------------------------------
int Track::SetParameter(TVirtualFitter* pTVirtualFitter)
{
  int ier = 0 ; 
  
  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
//  ier = pTVirtualFitter->SetParameter(iPar, m_ParameterName[iPar].c_str() , m_Parameter[iPar] , m_eParameter[iPar] , -1. , 1. ) ;
    ier = pTVirtualFitter->SetParameter(iPar, m_ParameterName[iPar].c_str() , m_Parameter[iPar] , m_eParameter[iPar] , 0. , 0. ) ;
    if (ier != 0 ) std::cout << " ier " << ier << " SetParameter " << m_ParameterName[iPar] << std::endl;
    m_Parameter_BeforeMinimisation[iPar] = m_Parameter[iPar] ;
  }

  return ier ;
}

void Track::SetResults(TVirtualFitter* pTVirtualFitter)
{
  p_FitOutput->SetResults(pTVirtualFitter) ;
  
//p_FitOutput->PrintFitOutput();
  
  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
    m_Parameter[iPar]  = p_FitOutput->p_par[iPar]      ;
    m_eParameter[iPar] = (p_FitOutput->p_eparplus[iPar]-p_FitOutput->p_eparminus[iPar])/2. ;      ;
  }

  m_Chi2Min = Chi2(p_FitOutput->p_par) ;
  m_covmatrix.ResizeTo(m_NberOfParam, m_NberOfParam);
  for (int irow = 0 ; irow<m_NberOfParam ; irow++) {
    for (int icol = 0 ; icol<m_NberOfParam ; icol++) {
      m_covmatrix(irow,icol) = p_FitOutput->p_CovMatrix[irow + icol*m_NberOfParam] ;
    }
  }

//
  V_Residual.clear() ;
  V_Pull    .clear() ;
  int NClusters = Get_NberOfCluster();
  for (int iC = 0 ; iC< NClusters; iC++){
    const Cluster* pCluster = Get_Cluster(iC);

    double TheObservation    = pCluster->Get_YTrack  ()   ;
    
    double ThePosition_X = pCluster ->Get_XTrack() ;
    double ThePrediction = Y_Position(ThePosition_X)  ;

    double Residual = TheObservation-ThePrediction ; 
    V_Residual.push_back(Residual);

    double Pull = Residual/pCluster->Get_eYTrack  () ; 
    V_Pull.push_back(Pull);

  }

}

double Track::Chi2(double par[])
{
  SetParameters_Internal(par) ;
  
  double ToBeReturned = 0. ;
  
  int NClusters = Get_NberOfCluster();
  for (int iC = 0 ; iC<NClusters ; iC++) {
    const Cluster* pCluster = Get_Cluster(iC);
    
    double TheObservation    = pCluster->Get_YTrack  ()   ;
    double eTheObservation   = pCluster->Get_eYTrack ()   ;
    
    double ThePosition_X = pCluster ->Get_XTrack() ;
    double ThePrediction = Y_Position(ThePosition_X)  ;

    double Diff = (TheObservation-ThePrediction)/eTheObservation; 
    
    ToBeReturned += Diff * Diff ;

  }
 
  return ToBeReturned ;
} 

void Track::SetParameters_Internal(double par[])
{
  for (int iPar = 0 ; iPar< m_NberOfParam; iPar++){
    m_Parameter[iPar] = par[iPar] ;
  }
}

void Track::DrawOut( const std::string& OUTDirName) const
{
  TGraphErrors* pTGraphErrors = new TGraphErrors ;
  pTGraphErrors->SetMarkerStyle(8) ;
  pTGraphErrors->SetMarkerColor(2) ;
  pTGraphErrors->SetLineColor(2) ;
  
  double  Ymax = 0. ;
  double  Ymin = 0. ;
  int    iYmax = 0  ;
  int    iYmin = 0  ;
  double  Xmax = 0. ;
  double  Xmin = 0. ;
  int    iXmax = 0  ;
  int    iXmin = 0  ;
  int First_Limits = 1 ;
  
  int NClusters = Get_NberOfCluster();
  for (int iC = 0 ; iC<NClusters ; iC++) {
    const Cluster* pCluster = Get_Cluster(iC);
    
    double  XTrack = pCluster->Get_XTrack() ;
    double  YTrack = pCluster->Get_YTrack()  ;
    double eYTrack = pCluster->Get_eYTrack() ;
    
    pTGraphErrors->SetPoint     ( pTGraphErrors->GetN()  , XTrack ,  YTrack) ;
    pTGraphErrors->SetPointError( pTGraphErrors->GetN()-1, 0.     , eYTrack) ;

    int    NPads      = pCluster->Get_NberOfPads();
    for (int iP = 0 ; iP< NPads; iP++){
      const Pad* pPad = pCluster->Get_Pad(iP);
    
      int    iY = pPad->Get_iY();
      double YL = pPad->Get_YL();
      double YH = pPad->Get_YH();
      
      int    iX = pPad->Get_iX();
      double XL = pPad->Get_XL();
      double XH = pPad->Get_XH();
      
      if (First_Limits==1){
        First_Limits = 0 ;
         Ymax = YH;
         Ymin = YL;
        iYmax = iY;
        iYmin = iY;
         Xmax = XH;
         Xmin = XL;
        iXmax = iX;
        iXmin = iX;
      }
      if ( Ymax<YH)  Ymax=YH;
      if ( Ymin>YL)  Ymin=YL;
      if (iYmax<iY) iYmax=iY;
      if (iYmin>iY) iYmin=iY;
      if ( Xmax<XH)  Xmax=XH;
      if ( Xmin>XL)  Xmin=XL;
      if (iXmax<iX) iXmax=iX;
      if (iXmin>iX) iXmin=iX;
    }
  }
  int NbinX = iXmax - iXmin + 1 ;
  int NbinY = iYmax - iYmin + 1 ;
  
  std::ostringstream aostringstream2  ;
  aostringstream2 << std::setiosflags(std::ios::fixed) ;
  aostringstream2 << "Event_" << m_EventNber ;
  aostringstream2 << "Entry_" << m_EntryNber ;
  std::string Name = "DrawOut"  + aostringstream2.str() ;

  std::ostringstream aostringstream3  ;
  aostringstream3 << std::setiosflags(std::ios::fixed) ;
  aostringstream3 << " EventNber : " << m_EventNber ;
  aostringstream3 << " EntryNber : " << m_EntryNber ;
  aostringstream3 << " Chi2min  : " << std::setw(16) << std::setprecision(3) << Get_Chi2Min() ;
  std::string Title = aostringstream3.str() ;

  TH2F* pTH2F = new TH2F(Title.c_str(),Title.c_str(),NbinX,Xmin,Xmax,NbinY,Ymin,Ymax);
  
  for (int iC = 0 ; iC< NClusters; iC++){
    const Cluster* pCluster = Get_Cluster(iC);
    
    int  NPads = pCluster->Get_NberOfPads();
    for (int iP = 0 ; iP< NPads; iP++){
      const Pad* pPad = pCluster->Get_Pad(iP);
      
      double Xpad = pPad->Get_XPad();
      double Ypad = pPad->Get_YPad();
      int Qv =  pPad->Get_AMax() ;
      
      pTH2F->Fill(Xpad,Ypad,Qv);
  
    }
  }

  TGraph* pTGraph = new TGraph ;
  pTGraph->SetLineColor(2) ; 
  
  int iPt_Max = 1000 ;
  for (int iPt = 0 ; iPt< iPt_Max; iPt++){
    double Xcur = Xmin + double(iPt)*(Xmax-Xmin)/double(iPt_Max-1);
    double Ycur = Y_Position(Xcur) ;
    pTGraph->SetPoint     ( pTGraph->GetN()  , Xcur ,  Ycur) ;
  }


//  
  std::string OUTDirNameLoc= OUTDirName + "TrackDisplays/" ;
  MakeMyDir(OUTDirNameLoc) ;
  
  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << OUTDirNameLoc  ;
  aostringstream << "Track_EventNber_" ;
  aostringstream << m_EventNber ;
  aostringstream << "Track_EntryNber_" ;
  aostringstream << m_EntryNber ;
  aostringstream << ".png"  ;
  std::string Str_Out_PDF     = aostringstream.str() ; 

  gStyle->SetOptStat(0);
  TCanvas* pTCanvas = new TCanvas("BID"," ",1200, 900) ;

    pTH2F->Draw("colz box");
    pTCanvas->Update();
  
    pTGraphErrors->Draw("p");
    pTCanvas->Update();
  
    pTGraph->Draw("l");
    pTCanvas->Update();
  
  pTCanvas->SaveAs(Str_Out_PDF.c_str());


}
