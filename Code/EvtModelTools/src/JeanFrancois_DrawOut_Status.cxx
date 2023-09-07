#include "EvtModelTools/JeanFrancois_DrawOut_Status.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "EvtModelTools/EvtModelTools_SH_Histos.h"
#include "EvtModelTools/EvtModelTools_JFL_Histos.h"
#include "EvtModel/TimeError.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"

void JeanFrancois_DrawOut_Status(Sample& aSample, const int& ModuleNber ,const std::string& OUTDIR,const std::string& TAG)
{
  std::string OutputFile     = OUTDIR + TAG + "_JeanFrancois_DrawOut_Status.pdf" ;
  std::string OutputFile_Beg = OutputFile + "(" ;
  std::string OutputFile_End = OutputFile + ")" ;

//Selection Description
  double Cut_Stage8_A3onA1_Hig      =   0.3  ;
  double Cut_Stage8_A3onA1_Low      =   0.03 ;
  double Cut_Stage8_A2onA1_Low      =   0.06 ;
  double Cut_Stage8_A3onA2_Low      =   0.03 ;
  
  double Cut_Stage6_ARelat_Low  =   0.03 ;
  double Cut_Stage6_A_Low       =   30. ;
  
  double Cut_Stage7_T2T1_Hig    =   30.  ;
  double Cut_Stage7_T2T1_Low    =   -3.  ;
  double Cut_Stage7_T3T1_Hig    =  100.  ;
  double Cut_Stage7_T3T1_Low    =   -3.  ;
  double Cut_Stage7_T3T2_Hig    =  100.  ;
  double Cut_Stage7_T3T2_Low    =   -3.  ;
  
  double Cut_Stage12_RunBased   =   40.  ;
  
  double Cut_Stage11_EventBased =    3.  ;
  
  double Cut_Stage13_DT_Low     =   -3.  ;
  
  double Cut_Stage14_Amax_Hig   = 3600.  ;

  double Cut_Stage15_CutSlopeXY_Hig  =  3.6  ;
  double Cut_Stage15_CutSlopeXY_Low  = -3.6  ;

  double Cut_Stage16_CutSlopeXZ_Hig  =  13.  ;
  double Cut_Stage16_CutSlopeXZ_Low  = -13.  ;

  double Cut_Stage17_CutInterCeptXZ_Hig  = 290.  ;
  double Cut_Stage17_CutInterCeptXZ_Low  = 240.  ;

  // Samira
  // 40 ns/bin   7 cm /us  -->   0.007 cm/ns ---> 0.28 cm / bin
  double Cut_Stage18_v_drift_est    = 0.28 ;
  double Cut_Stage18_CutSlopeXYZ0   = 0.1  ;
  double Cut_Stage18_CutSlopeXYZ1   = 0.1  ;
  
// 
  Cut_Stage12_RunBased   = 150. ;
  Cut_Stage11_EventBased = 150. ;
  

//From EvtModelTools_TD_Histos
  TH1I*     pTH1I_PadPerCluster         = GiveMe_PadPerCluster       (aSample, ModuleNber , TAG) ;
  TH1I*     pTH1I_ClusterPerEvent       = GiveMe_ClusterPerEvent     (aSample, ModuleNber , TAG) ;



//From EvtModelTools_JFL_Histos
  TH1F* pTH1F_AlphaNotLeading = GiveMe_pTH1F_AlphaNotLeading(aSample, ModuleNber , TAG,100, 0. , 0.2) ;

  TH1F* pTH1F_AMax_WFmAMax_FIT              = GiveMe_pTH1F_AMax_WFmAMax_FIT(aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_TMax_WFmTMax_FIT              = GiveMe_pTH1F_TMax_WFmTMax_FIT(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF   = GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF   = GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF_Z = GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF(aSample, ModuleNber , TAG, 501, -0.5 , 500.5, 81, -40.5 , 40.5) ;
  TH2F* pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF_Z = GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF(aSample, ModuleNber , TAG, 501, -0.5 , 500.5, 81, -40.5 , 40.5) ;

  TH1F* pTH1F_AMaxmAMax_FIT         = GiveMe_pTH1F_AMaxmAMax_FIT(aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_TMaxmTMax_FIT         = GiveMe_pTH1F_TMaxmTMax_FIT(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMaxmAMax_FIT_VS_AMax = GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMaxmTMax_FIT_VS_AMax = GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax(aSample, ModuleNber , TAG) ;


//
  TH1F* pTH1F_AMax_FITmAMax_MC              = GiveMe_pTH1F_AMax_FITmAMax_MC(aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_TMax_FITmTMax_MC              = GiveMe_pTH1F_TMax_FITmTMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FITmAMax_MC_VS_AMax_MC   = GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMax_FITmTMax_MC_VS_AMax_MC   = GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FITmAMax_MC_VS_AMax_MC_Z = GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG, 501, -0.5 , 500.5, 201,  -100.5 , 100.5) ;
  TH2F* pTH2F_TMax_FITmTMax_MC_VS_AMax_MC_Z = GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG, 201, -0.5 , 200.5,  61,   -30.5 ,  30.5) ;

  TH1F* pTH1F_AMax_FromSetmAMax_MC              = GiveMe_pTH1F_AMax_FromSetmAMax_MC(aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_TMax_FromSetmTMax_MC              = GiveMe_pTH1F_TMax_FromSetmTMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC   = GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC   = GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC_Z = GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG, 501, -0.5 , 500.5, 201,  -100.5 , 100.5) ;
  TH2F* pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC_Z = GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC(aSample, ModuleNber , TAG, 201, -0.5 , 200.5,  61,   -30.5 ,  30.5) ;

  TH2F* pTH2F_AMax_FITmAMax_MC_VS_TMax_MC   = GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMax_FITmTMax_MC_VS_TMax_MC   = GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FITmAMax_MC_VS_TMax_MC_Z = GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG, 31, -0.5 , 30.5, 201,  -100.5 , 100.5) ;
  TH2F* pTH2F_TMax_FITmTMax_MC_VS_TMax_MC_Z = GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG, 20, 30.5 , 55.5,  61,   -30.5 ,  30.5) ;

  TH2F* pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC   = GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC   = GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC_Z = GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG, 31, -0.5 , 30.5, 201,  -100.5 , 100.5) ;
  TH2F* pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC_Z = GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC(aSample, ModuleNber , TAG, 20, 30.5 , 55.5,  61,   -30.5 ,  30.5) ;

  TProfile* pTProfile_TMax_FITmTMax_MC_VS_TMax_MC        = GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC  (aSample, ModuleNber , TAG ) ;

//
  TH1F* pTH1F_MeanClusterMultiplicity = GiveMe_TH1F_MeanClusterMultiplicity(aSample, ModuleNber , TAG) ;

  TH1F* pTH1F_Rho                   = GiveMe_pTH1F_Rho       (aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_Rho2                  = GiveMe_pTH1F_Rho       (aSample, ModuleNber , TAG + "_2", 2, 2) ;
  TH1F* pTH1F_Rho3                  = GiveMe_pTH1F_Rho       (aSample, ModuleNber , TAG + "_3", 3, 3) ;
  TH1F* pTH1F_Rho4                  = GiveMe_pTH1F_Rho       (aSample, ModuleNber , TAG + "_4", 4, 4) ;
  TH1F* pTH1F_Yw                    = GiveMe_pTH1F_Yw        (aSample, ModuleNber , TAG) ;
//TH1F* pTH1F_Yw                    = GiveMe_pTH1F_Yw        (aSample, ModuleNber , TAG,100,0.,38.) ;
  TH1I* pTH1I_LeadingiY             = GiveMe_pTH1I_LeadingiY (aSample, ModuleNber , TAG) ;

  TH1F*     pTH1F_YwYpad                = GiveMe_pTH1F_YwYpad            (aSample, ModuleNber , TAG, -3.,3. ) ;
  TProfile* pTProfile_DeltaT_YwYpad     = GiveMe_pTProfile_DeltaT_YwYpad (aSample, ModuleNber , TAG, -3.,3. ) ;

  TProfile* pTProfile_Rho_DeltaT        = GiveMe_pTProfile_Rho_DeltaT (aSample, ModuleNber , TAG) ;
  TH2F*     pTH2F_Rho_YwYpad            = GiveMe_pTH2F_Rho_YwYpad     (aSample, ModuleNber , TAG, 250, -3., 3., 120, -0.1, 1.1) ; 
  TH2F*     pTH2F_DeltaT_YwYpad         = GiveMe_pTH2F_DeltaT_YwYpad  (aSample, ModuleNber , TAG,-3.,3.) ;

  TProfile* pTProfile_Rho_YwYpad        = GiveMe_pTProfile_Rho_YwYpad  (aSample, ModuleNber , TAG, 250, -3., 3., -0.1, 1.1) ;

  TH1F* pTH1F_Tmax                    = GiveMe_pTH1F_Tmax                    (aSample, ModuleNber , TAG) ;
  
//----------------------  Alpha2_VS_Alpha3 ----------------------//
  TGraph* pTGraph_Alpha2_VS_Alpha3   =  GiveMe_pTGraph_Alpha2_VS_Alpha3 (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Alpha2_VS_Alpha3     =  GiveMe_pTH2F_Alpha2_VS_Alpha3   (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Alpha2_VS_Alpha3ZOOM =  GiveMe_pTH2F_Alpha2_VS_Alpha3   (aSample, ModuleNber , TAG, 100,0.,0.3, 100,0.,1. ) ;

//----------------------  Tau2_VS_Tau3  ----------------------//
  TGraph* pTGraph_Tau2_VS_Tau3   =  GiveMe_pTGraph_Tau2_VS_Tau3 (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Tau2_VS_Tau3     =  GimeMe_pTH2F_Tau2_VS_Tau3   (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Tau2_VS_Tau3ZOOM =  GimeMe_pTH2F_Tau2_VS_Tau3   (aSample, ModuleNber , TAG, 104,-3.5,100.5, 34,-3.5,30.5 ) ;

//----------------------  Alpha2_VS_Alpha3onAlpha2  ----------------------//
  TGraph* pTGraph_Alpha2_VS_Alpha3onAlpha2 =  GiveMe_pTGraph_Alpha2_VS_Alpha3onAlpha2 (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Alpha2_VS_Alpha3onAlpha2   =  GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2   (aSample, ModuleNber , TAG) ;
  
//----------------------  Tau2_VS_Tau3mTau2  ----------------------//
  TGraph* pTGraph_Tau2_VS_Tau3mTau2   =  GiveMe_pTGraph_Tau2_VS_Tau3mTau2 (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Tau2_VS_Tau3mTau2     =  GiveMe_pTH2F_Tau2_VS_Tau3mTau2   (aSample, ModuleNber , TAG) ;
  TH2F*   pTH2F_Tau2_VS_Tau3mTau2ZOOM =  GiveMe_pTH2F_Tau2_VS_Tau3mTau2   (aSample, ModuleNber , TAG, 104,-3.5,100.5, 34,-3.5,30.5 ) ;

  TH1F*  pTH1F_Tleading          = GiveMe_pTH1F_LeadTmax (aSample, ModuleNber , TAG) ;
  int    Imax_pTH1F_Tleading     = pTH1F_Tleading->GetMaximumBin()-1 ;
  double Xmin_pTH1F_TleadingZOOM = Imax_pTH1F_Tleading - 30. - 0.5 ;
  double Xmax_pTH1F_TleadingZOOM = Imax_pTH1F_Tleading + 30. + 0.5 ;
  int    Nbin_pTH1F_TleadingZOOM = int ( Xmax_pTH1F_TleadingZOOM - Xmin_pTH1F_TleadingZOOM ) ;
  TH1F*  pTH1F_TleadingZOOM      = GiveMe_pTH1F_LeadTmax (aSample, ModuleNber , TAG,Nbin_pTH1F_TleadingZOOM,Xmin_pTH1F_TleadingZOOM,Xmax_pTH1F_TleadingZOOM) ;
  
  TH1F* pTH1F_DTLeading     = GiveMe_pTH1F_DTLeading (aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_DTLeadingZOOM = GiveMe_pTH1F_DTLeading (aSample, ModuleNber , TAG,100,-10.,10.) ;
                             
  TH1F* pTH1F_TauNotLeading     = GiveMe_pTH1F_TauNotLeading(aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_TauNotLeadingZOOM = GiveMe_pTH1F_TauNotLeading(aSample, ModuleNber , TAG,111,-10.5,100.5) ;

  TH2F* pTH2F_Alpha_VS_Tau_NotLeading     = GiveMe_pTH2F_Alpha_VS_Tau_NotLeading  (aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_Alpha_VS_Tau_NotLeadingZOOM = GiveMe_pTH2F_Alpha_VS_Tau_NotLeading  (aSample, ModuleNber , TAG, 311, -10.5,300.5, 120, -0.1, 1.1) ;  

  TH2F* pTH2F_TauNotLeading_VS_TmaxLeading     = GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_TauNotLeading_VS_TmaxLeadingZOOM = GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (aSample, ModuleNber , TAG, Nbin_pTH1F_TleadingZOOM,Xmin_pTH1F_TleadingZOOM,Xmax_pTH1F_TleadingZOOM, 311, -10.5,300.) ;  

  TH1F* pTH1F_Amax      = GiveMe_pTH1F_Amax (aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_AmaxZOOM  = GiveMe_pTH1F_Amax (aSample, ModuleNber , TAG,597, 3500. - 0.5 , 4096.5 );
  TH1F* pTH1F_AmaxZOOM2 = GiveMe_pTH1F_Amax (aSample, ModuleNber , TAG+"Z2",101, - 0.5 , 100.5 );

  TH1F* pTH1F_PadMultiplicityPerEvent = GiveMe_pTH1F_PadMultiplicityPerEvent (aSample, ModuleNber , TAG ,101, - 0.5 , 100.5 );

  TH1F* pTH1F_Amax_Leading      = GiveMe_pTH1F_Amax_Leading (aSample, ModuleNber , TAG) ;
  
  TH1F* pTH1F_SlopeXY = GiveMe_pTH1F_SlopeXY (aSample, ModuleNber , TAG) ;
  TH1F* pTH1F_SlopeXZ = GiveMe_pTH1F_SlopeXZ (aSample, ModuleNber , TAG) ;

  TH1F* pTH1F_InterCeptXZ = GiveMe_pTH1F_InterCeptXZ (aSample, ModuleNber , TAG) ;
  //Samira

  TH1F* pTH1F_SlopeXYZ0 = GiveMe_pTH1F_SlopeXYZ0     (aSample,ModuleNber , TAG, Cut_Stage18_v_drift_est) ;
  TH1F* pTH1F_SlopeXYZ1 = GiveMe_pTH1F_SlopeXYZ1     (aSample,ModuleNber , TAG) ;

  TH2F* pTH2F_XZ_Display     = GiveMe_pTH2F_XZ_display (aSample, ModuleNber , TAG) ;
  TH2F* pTH2F_XY_Display     = GiveMe_pTH2F_XY_display (aSample, ModuleNber , TAG) ;
//From EvtModelTools_JFL_Histos  

//
  int OPTSTAT_IN = gStyle->GetOptStat();
  gStyle->SetOptStat(111111);
//gStyle->SetOptStat(0);

//Page 1
  TCanvas* pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(OutputFile_Beg.c_str());
  
//Page 2
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(4);
    pTCanvas->cd(4)->SetLogy() ;
    pTH1F_PadMultiplicityPerEvent->Draw();
    pTCanvas->Update();
  
    pTCanvas->cd(1);
    pTCanvas->cd(1)->SetLogy() ;
    pTH1I_PadPerCluster->Draw();
    pTCanvas->Update();
  
    pTCanvas->cd(2);
    pTH1I_LeadingiY->Draw();
    pTCanvas->Update();
  

    pTCanvas->cd(3);
    pTCanvas->cd(3)->SetLogy() ;
    pTH1F_MeanClusterMultiplicity->Draw();
    pTCanvas->Update();
  

    pTCanvas->cd(5);
    pTCanvas->cd(5)->SetLogy() ;
    pTH1I_ClusterPerEvent->Draw();
    pTCanvas->Update();
  
    pTCanvas->cd(6) ;
    gPad->SetLogy(0) ;
    
    if(pTH1F_Rho3 != 0) { pTH1F_Rho3->Scale(1./double(pTH1F_Rho3->GetEntries() ) ) ; pTH1F_Rho3->SetStats(0) ; }
                          pTH1F_Rho ->Scale(1./double(pTH1F_Rho ->GetEntries() ) ) ; pTH1F_Rho ->SetStats(0) ;
    if(pTH1F_Rho2 != 0) { pTH1F_Rho2->Scale(1./double(pTH1F_Rho2->GetEntries() ) ) ; pTH1F_Rho2->SetStats(0) ; }
    if(pTH1F_Rho4 != 0) { pTH1F_Rho4->Scale(1./double(pTH1F_Rho4->GetEntries() ) ) ; pTH1F_Rho4->SetStats(0) ; }
     
    if(pTH1F_Rho3 != 0) { pTH1F_Rho3->SetLineColor(3) ; }
                          pTH1F_Rho ->SetLineColor(1) ;
    if(pTH1F_Rho2 != 0) { pTH1F_Rho2->SetLineColor(2) ; }
    if(pTH1F_Rho4 != 0) { pTH1F_Rho4->SetLineColor(4) ; }
    
    if(pTH1F_Rho3 != 0) {
                          pTH1F_Rho3->Draw("HIST") ;
                          pTH1F_Rho ->Draw("SAME, HIST") ; 
                        }else{
                          pTH1F_Rho ->Draw("HIST") ; 
                        }
    if(pTH1F_Rho2 != 0) { pTH1F_Rho2->Draw("SAME, HIST") ; }
    if(pTH1F_Rho4 != 0) { pTH1F_Rho4->Draw("SAME, HIST") ; }
    
    TLegend* pTLegend = new TLegend(0.15,0.55,0.6,0.85) ; 
                          pTLegend->AddEntry(pTH1F_Rho  ,"All clusters"       ,"l") ;
    if(pTH1F_Rho2 != 0) { pTLegend->AddEntry(pTH1F_Rho2,"Clusters with 2 pads","l") ; }
    if(pTH1F_Rho3 != 0) { pTLegend->AddEntry(pTH1F_Rho3,"Clusters with 3 pads","l") ; }
    if(pTH1F_Rho4 != 0) { pTLegend->AddEntry(pTH1F_Rho4,"Clusters with 4 pads","l") ; }
    pTLegend->Draw("SAME") ;
  
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 3
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
    pTH1F_Yw->Draw();
    pTCanvas->Update();
  
    pTCanvas->cd(2);
    pTH1F_YwYpad->Draw();
    pTCanvas->Update();
  
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 4
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
    pTGraph_Alpha2_VS_Alpha3->Draw("AP");

      pTGraph_Alpha2_VS_Alpha3->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}=A3/A1");
      pTGraph_Alpha2_VS_Alpha3->GetXaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTGraph_Alpha2_VS_Alpha3->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTGraph_Alpha2_VS_Alpha3->GetYaxis()->SetTitleOffset(1.5);
      double Xmax_pTGraph_Alpha2_VS_Alpha3 = std::min (1. ,pTGraph_Alpha2_VS_Alpha3->GetXaxis()->GetXmax() ) ;
      pTGraph_Alpha2_VS_Alpha3->GetXaxis()->SetLimits(0. , Xmax_pTGraph_Alpha2_VS_Alpha3);
      pTGraph_Alpha2_VS_Alpha3->SetMaximum( 1. );
      pTGraph_Alpha2_VS_Alpha3->SetMinimum( 0. );

      pTCanvas->Update();

      double pTGraph_Alpha2_VS_Alpha3_Xmin = pTCanvas->cd(1)->GetUxmin() ;
      double pTGraph_Alpha2_VS_Alpha3_Xmax = pTCanvas->cd(1)->GetUxmax() ;
      double pTGraph_Alpha2_VS_Alpha3_Ymin = pTCanvas->cd(1)->GetUymin() ;
      double pTGraph_Alpha2_VS_Alpha3_Ymax = pTCanvas->cd(1)->GetUymax() ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A1= new TLine( Cut_Stage8_A3onA1_Hig   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Hig  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A2= new TLine( pTGraph_Alpha2_VS_Alpha3_Xmin  , Cut_Stage8_A2onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Xmax , Cut_Stage8_A2onA1_Low      ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A3= new TLine( Cut_Stage8_A3onA1_Low   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;

      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A1->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A2->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A3->SetLineColor( 2 ) ;
      
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A1->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A2->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A3->Draw();  
      pTCanvas->Update();
 
      TF1* pTF1_pTGraph_Alpha2_VS_Alpha3_A = new TF1("pTF1_pTGraph_Alpha2_VS_Alpha3_A","x/[0]",0,10);
      pTF1_pTGraph_Alpha2_VS_Alpha3_A->SetParameter(0,Cut_Stage8_A3onA2_Low);
      pTF1_pTGraph_Alpha2_VS_Alpha3_A->SetLineColor(2) ;
      pTF1_pTGraph_Alpha2_VS_Alpha3_A->SetLineStyle(4) ;

      pTF1_pTGraph_Alpha2_VS_Alpha3_A->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(2);
    pTH2F_Alpha2_VS_Alpha3->Draw("colz") ;

      pTH2F_Alpha2_VS_Alpha3->GetXaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTH2F_Alpha2_VS_Alpha3->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTH2F_Alpha2_VS_Alpha3->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      pTGraph_Alpha2_VS_Alpha3_Xmin = pTCanvas->cd(2)->GetUxmin() ;
      pTGraph_Alpha2_VS_Alpha3_Xmax = pTCanvas->cd(2)->GetUxmax() ;
      pTGraph_Alpha2_VS_Alpha3_Ymin = pTCanvas->cd(2)->GetUymin() ;
      pTGraph_Alpha2_VS_Alpha3_Ymax = pTCanvas->cd(2)->GetUymax() ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B1= new TLine( Cut_Stage8_A3onA1_Hig   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Hig  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B2= new TLine( pTGraph_Alpha2_VS_Alpha3_Xmin  , Cut_Stage8_A2onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Xmax , Cut_Stage8_A2onA1_Low      ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B3= new TLine( Cut_Stage8_A3onA1_Low   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;

      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B1->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B2->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B3->SetLineColor( 2 ) ;
      
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B1->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B2->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B3->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Alpha2_VS_Alpha3_A->Draw("Same");
      pTCanvas->Update();
 
    pTCanvas->cd(3);
    pTH2F_Alpha2_VS_Alpha3ZOOM->Draw("colz") ;

      pTH2F_Alpha2_VS_Alpha3ZOOM->GetXaxis()->SetTitle("#alpha_{3}=A3/A1");
      pTH2F_Alpha2_VS_Alpha3ZOOM->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTH2F_Alpha2_VS_Alpha3ZOOM->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      pTGraph_Alpha2_VS_Alpha3_Xmin = pTCanvas->cd(3)->GetUxmin() ;
      pTGraph_Alpha2_VS_Alpha3_Xmax = pTCanvas->cd(3)->GetUxmax() ;
      pTGraph_Alpha2_VS_Alpha3_Ymin = pTCanvas->cd(3)->GetUymin() ;
      pTGraph_Alpha2_VS_Alpha3_Ymax = pTCanvas->cd(3)->GetUymax() ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C1= new TLine( Cut_Stage8_A3onA1_Hig   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Hig  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C2= new TLine( pTGraph_Alpha2_VS_Alpha3_Xmin  , Cut_Stage8_A2onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Xmax , Cut_Stage8_A2onA1_Low      ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C3= new TLine( Cut_Stage8_A3onA1_Low   , pTGraph_Alpha2_VS_Alpha3_Ymin , Cut_Stage8_A3onA1_Low  , pTGraph_Alpha2_VS_Alpha3_Ymax ) ;

      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C1->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C2->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C3->SetLineColor( 2 ) ;
      
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C1->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C2->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C3->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Alpha2_VS_Alpha3_A->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(4);
    pTGraph_Alpha2_VS_Alpha3onAlpha2->Draw("AP");

      pTGraph_Alpha2_VS_Alpha3onAlpha2->SetTitle("#alpha_{2}=A2/A1 VS #alpha_{3}/#alpha_{2}=A3/A2");
      pTGraph_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      pTGraph_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTGraph_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitleOffset(1.5);
      double Xmax_pTGraph_Alpha2_VS_Alpha3onAlpha2 = std::min (1. ,pTGraph_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->GetXmax() ) ;
      pTGraph_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->SetLimits(0. , Xmax_pTGraph_Alpha2_VS_Alpha3onAlpha2);
      pTGraph_Alpha2_VS_Alpha3onAlpha2->SetMaximum( 1. );
      pTGraph_Alpha2_VS_Alpha3onAlpha2->SetMinimum( 0. );
      pTCanvas->Update();

      double pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmin = pTCanvas->cd(4)->GetUxmin() ;
      double pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmax = pTCanvas->cd(4)->GetUxmax() ;
      double pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymin = pTCanvas->cd(4)->GetUxmin() ;
      double pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymax = pTCanvas->cd(4)->GetUxmax() ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A2= new TLine( pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmin  , Cut_Stage8_A2onA1_Low      , pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmax , Cut_Stage8_A2onA1_Low      ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A1= new TLine( Cut_Stage8_A3onA2_Low    , pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymin    , Cut_Stage8_A3onA2_Low   , pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymax    ) ;

      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A2->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A1->SetLineColor( 2 ) ;
      
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A2->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A1->Draw();  
      pTCanvas->Update();
      
      TF1* pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A = new TF1("pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A","[0]/x",0,10);
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A->SetParameter(0,Cut_Stage8_A3onA1_Hig);
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A->SetLineColor(2) ;
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A->SetLineStyle(4) ;

      TF1* pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B = new TF1("pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B","[0]/x",0,10);
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B->SetParameter(0,Cut_Stage8_A3onA1_Low);
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B->SetLineColor(2) ;
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B->SetLineStyle(4) ;

      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A->Draw("Same");
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(5);
    pTH2F_Alpha2_VS_Alpha3onAlpha2->Draw("colz");

      pTH2F_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->SetTitle("#alpha{3}/#alpha_{2}=A3/A2");
      pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmin = pTCanvas->cd(5)->GetUxmin() ;
      pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmax = pTCanvas->cd(5)->GetUxmax() ;
      pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymin = pTCanvas->cd(5)->GetUxmin() ;
      pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymax = pTCanvas->cd(5)->GetUxmax() ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B2= new TLine( pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmin  , Cut_Stage8_A2onA1_Low      , pTGraph_Alpha2_VS_Alpha3onAlpha2_Xmax , Cut_Stage8_A2onA1_Low      ) ;
      TLine* pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B1= new TLine( Cut_Stage8_A3onA2_Low    , pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymin    , Cut_Stage8_A3onA2_Low   , pTGraph_Alpha2_VS_Alpha3onAlpha2_Ymax    ) ;

      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B2->SetLineColor( 2 ) ;
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B1->SetLineColor( 2 ) ;
      
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B2->Draw();  
      pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B1->Draw();  
      pTCanvas->Update();
     
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A->Draw("Same");
      pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B->Draw("Same");
      pTCanvas->Update();


  pTCanvas->SaveAs(OutputFile.c_str());

//Page 5
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
      pTGraph_Tau2_VS_Tau3->Draw("AP");
      pTCanvas->Update();

      pTGraph_Tau2_VS_Tau3->SetTitle("#tau_{2}=T2-T1 VS #tau_{3}=T3-T1");
      pTGraph_Tau2_VS_Tau3->GetXaxis()->SetTitle("#tau_{3}=T3-T1");
      pTGraph_Tau2_VS_Tau3->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      pTGraph_Tau2_VS_Tau3->GetYaxis()->SetTitleOffset(1.5);

      double pTH2F_Tau2_VS_Tau3_Xmin = pTCanvas->cd(1)->GetUxmin() ;
      double pTH2F_Tau2_VS_Tau3_Xmax = pTCanvas->cd(1)->GetUxmax() ;
      double pTH2F_Tau2_VS_Tau3_Ymin = pTCanvas->cd(1)->GetUymin() ;
      double pTH2F_Tau2_VS_Tau3_Ymax = pTCanvas->cd(1)->GetUymax() ;
      TLine* ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A1= new TLine( Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A2= new TLine( Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A3= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A4= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A1->SetLineColor( 2 ) ;
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A2->SetLineColor( 2 ) ;
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A3->SetLineColor( 2 ) ;
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A4->SetLineColor( 2 ) ;
      
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A1->Draw();  
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A2->Draw();  
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A3->Draw();  
      ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A4->Draw();  
      pTCanvas->Update();
 
      TF1* pTF1_pTGraph_Tau2_VS_Tau3_A = new TF1("pTF1_pTGraph_Tau2_VS_Tau3_A","x-[0]",-1000,1000);
      pTF1_pTGraph_Tau2_VS_Tau3_A->SetParameter(0,Cut_Stage7_T3T2_Low);
      pTF1_pTGraph_Tau2_VS_Tau3_A->SetLineColor(2) ;
      pTF1_pTGraph_Tau2_VS_Tau3_A->SetLineStyle(4) ;

      TF1* pTF1_pTGraph_Tau2_VS_Tau3_B = new TF1("pTF1_pTGraph_Tau2_VS_Tau3_B","x-[0]",-1000,1000);
      pTF1_pTGraph_Tau2_VS_Tau3_B->SetParameter(0,Cut_Stage7_T3T2_Hig);
      pTF1_pTGraph_Tau2_VS_Tau3_B->SetLineColor(2) ;
      pTF1_pTGraph_Tau2_VS_Tau3_B->SetLineStyle(4) ;

      pTF1_pTGraph_Tau2_VS_Tau3_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_Tau2_VS_Tau3->Draw("colz") ;

      pTH2F_Tau2_VS_Tau3->GetXaxis()->SetTitle("#tau_{3}=T3-T1");
      pTH2F_Tau2_VS_Tau3->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      pTH2F_Tau2_VS_Tau3->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      pTH2F_Tau2_VS_Tau3_Xmin = pTCanvas->cd(2)->GetUxmin() ;
      pTH2F_Tau2_VS_Tau3_Xmax = pTCanvas->cd(2)->GetUxmax() ;
      pTH2F_Tau2_VS_Tau3_Ymin = pTCanvas->cd(2)->GetUymin() ;
      pTH2F_Tau2_VS_Tau3_Ymax = pTCanvas->cd(2)->GetUymax() ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B1= new TLine( Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B2= new TLine( Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B3= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B4= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B1->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B2->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B3->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B4->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B1->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B2->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B3->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B4->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Tau2_VS_Tau3_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(3);
      pTH2F_Tau2_VS_Tau3ZOOM->Draw("colz") ;

      pTH2F_Tau2_VS_Tau3ZOOM->GetXaxis()->SetTitle("#tau_{3}=T3-T1");
      pTH2F_Tau2_VS_Tau3ZOOM->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      pTH2F_Tau2_VS_Tau3ZOOM->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      
      pTH2F_Tau2_VS_Tau3_Xmin = pTCanvas->cd(3)->GetUxmin() ;
      pTH2F_Tau2_VS_Tau3_Xmax = pTCanvas->cd(3)->GetUxmax() ;
      pTH2F_Tau2_VS_Tau3_Ymin = pTCanvas->cd(3)->GetUymin() ;
      pTH2F_Tau2_VS_Tau3_Ymax = pTCanvas->cd(3)->GetUymax() ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C1= new TLine( Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Low  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C2= new TLine( Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymin , Cut_Stage7_T3T1_Hig  , pTH2F_Tau2_VS_Tau3_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C3= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C4= new TLine( pTH2F_Tau2_VS_Tau3_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C1->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C2->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C3->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C4->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C1->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C2->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C3->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C4->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Tau2_VS_Tau3_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(4);
      pTGraph_Tau2_VS_Tau3mTau2->Draw("AP");
      pTCanvas->Update();

      pTGraph_Tau2_VS_Tau3mTau2->SetTitle("#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2");
      pTGraph_Tau2_VS_Tau3mTau2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      pTGraph_Tau2_VS_Tau3mTau2->GetYaxis()->SetTitle("#tau_{2}=T2-T2");
      pTGraph_Tau2_VS_Tau3mTau2->GetYaxis()->SetTitleOffset(1.5);

      double pTH2F_Tau2_VS_Tau3mTau2_Xmin = pTCanvas->cd(4)->GetUxmin() ;
      double pTH2F_Tau2_VS_Tau3mTau2_Xmax = pTCanvas->cd(4)->GetUxmax() ;
      double pTH2F_Tau2_VS_Tau3mTau2_Ymin = pTCanvas->cd(4)->GetUymin() ;
      double pTH2F_Tau2_VS_Tau3mTau2_Ymax = pTCanvas->cd(4)->GetUymax() ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A1= new TLine( Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A2= new TLine( Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A3= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A4= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A1->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A2->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A3->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A4->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A1->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A2->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A3->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A4->Draw();  
      pTCanvas->Update();
 
      TF1* pTF1_pTGraph_Tau2_VS_Tau3mTau2_A = new TF1("pTF1_pTGraph_Tau2_VS_Tau3mTau2_A","[0]-x",-1000,1000);
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->SetParameter(0,Cut_Stage7_T3T1_Low);
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->SetLineColor(2) ;
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->SetLineStyle(4) ;

      TF1* pTF1_pTGraph_Tau2_VS_Tau3mTau2_B = new TF1("pTF1_pTGraph_Tau2_VS_Tau3mTau2_B","[0]-x",-1000,1000);
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->SetParameter(0,Cut_Stage7_T3T1_Hig);
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->SetLineColor(2) ;
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->SetLineStyle(4) ;

      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_Tau2_VS_Tau3mTau2->Draw("colz") ;

      pTH2F_Tau2_VS_Tau3mTau2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      pTH2F_Tau2_VS_Tau3mTau2->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      pTH2F_Tau2_VS_Tau3mTau2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      pTH2F_Tau2_VS_Tau3mTau2_Xmin = pTCanvas->cd(5)->GetUxmin() ;
      pTH2F_Tau2_VS_Tau3mTau2_Xmax = pTCanvas->cd(5)->GetUxmax() ;
      pTH2F_Tau2_VS_Tau3mTau2_Ymin = pTCanvas->cd(5)->GetUymin() ;
      pTH2F_Tau2_VS_Tau3mTau2_Ymax = pTCanvas->cd(5)->GetUymax() ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B1= new TLine( Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B2= new TLine( Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B3= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B4= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B1->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B2->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B3->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B4->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B1->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B2->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B3->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B4->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->Draw("Same");
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_Tau2_VS_Tau3mTau2ZOOM->Draw("colz") ;

      pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
      
      
      pTH2F_Tau2_VS_Tau3mTau2_Xmin = pTCanvas->cd(6)->GetUxmin() ;
      pTH2F_Tau2_VS_Tau3mTau2_Xmax = pTCanvas->cd(6)->GetUxmax() ;
      pTH2F_Tau2_VS_Tau3mTau2_Ymin = pTCanvas->cd(6)->GetUymin() ;
      pTH2F_Tau2_VS_Tau3mTau2_Ymax = pTCanvas->cd(6)->GetUymax() ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C1= new TLine( Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Low  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C2= new TLine( Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymin , Cut_Stage7_T3T2_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Ymax ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C3= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Low  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Low  ) ;
      TLine* pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C4= new TLine( pTH2F_Tau2_VS_Tau3mTau2_Xmin , Cut_Stage7_T2T1_Hig  , pTH2F_Tau2_VS_Tau3mTau2_Xmax , Cut_Stage7_T2T1_Hig  ) ;

      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C1->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C2->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C3->SetLineColor( 2 ) ;
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C4->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C1->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C2->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C3->Draw();  
      pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C4->Draw();  
      pTCanvas->Update();

      pTF1_pTGraph_Tau2_VS_Tau3mTau2_A->Draw("Same");
      pTF1_pTGraph_Tau2_VS_Tau3mTau2_B->Draw("Same");
      pTCanvas->Update();

  pTCanvas->SaveAs(OutputFile.c_str());

//Page 6
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
      pTCanvas->cd(1)->SetLogy() ;
      pTH1F_Tleading->Draw();
      pTCanvas->Update();
    
      int    pTH1F_Tleading_Max = pTH1F_Tleading->GetMaximumBin()-1 ;
      double pTH1F_Tleading_Low = pTH1F_Tleading_Max - Cut_Stage12_RunBased ;
      double pTH1F_Tleading_Hig = pTH1F_Tleading_Max + Cut_Stage12_RunBased ;
      
      TGraph* pTGraph_For_pTH1F_Tleading_1 = new TGraph ;
      pTGraph_For_pTH1F_Tleading_1->SetPoint( pTGraph_For_pTH1F_Tleading_1->GetN(), pTH1F_Tleading_Low , 1.E4*pTH1F_Tleading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_Tleading_1->SetPoint( pTGraph_For_pTH1F_Tleading_1->GetN(), pTH1F_Tleading_Low , 0.1  ) ;
      pTGraph_For_pTH1F_Tleading_1->SetLineColor(2) ;
  
      TGraph* pTGraph_For_pTH1F_Tleading_2 = new TGraph ;
      pTGraph_For_pTH1F_Tleading_2->SetPoint( pTGraph_For_pTH1F_Tleading_2->GetN(), pTH1F_Tleading_Hig , 1.E4*pTH1F_Tleading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_Tleading_2->SetPoint( pTGraph_For_pTH1F_Tleading_2->GetN(), pTH1F_Tleading_Hig , 0.1  ) ;
      pTGraph_For_pTH1F_Tleading_2->SetLineColor(2) ;
  
      TGraph* pTGraph_For_pTH1F_Tleading_3 = new TGraph ;
      pTGraph_For_pTH1F_Tleading_3->SetPoint( pTGraph_For_pTH1F_Tleading_3->GetN(), pTH1F_Tleading_Max , 1.E4*pTH1F_Tleading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_Tleading_3->SetPoint( pTGraph_For_pTH1F_Tleading_3->GetN(), pTH1F_Tleading_Max , 0.1  ) ;
      pTGraph_For_pTH1F_Tleading_3->SetLineColor(2) ;
      pTGraph_For_pTH1F_Tleading_3->SetLineStyle(4) ;
    
      pTGraph_For_pTH1F_Tleading_1->Draw("l");  
      pTGraph_For_pTH1F_Tleading_2->Draw("l");  
      pTGraph_For_pTH1F_Tleading_3->Draw("l");  
      pTCanvas->Update();
    
    pTCanvas->cd(2);
    pTCanvas->cd(2)->SetLogy() ;
      pTH1F_TleadingZOOM->Draw();
      pTCanvas->Update();
    
      pTGraph_For_pTH1F_Tleading_1->Draw("l");  
      pTGraph_For_pTH1F_Tleading_2->Draw("l");  
      pTGraph_For_pTH1F_Tleading_3->Draw("l");  
      pTCanvas->Update();

    pTCanvas->cd(3);
      pTH1F_TleadingZOOM->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_Tleading_1->Draw("l");  
      pTGraph_For_pTH1F_Tleading_2->Draw("l");  
      pTGraph_For_pTH1F_Tleading_3->Draw("l");  
      pTCanvas->Update();
  
    pTCanvas->cd(4);
    pTCanvas->cd(4)->SetLogy(1);
      pTH1F_DTLeading->Draw();
      pTCanvas->Update();
    
      double pTH1F_DTLeading_Low  = -Cut_Stage11_EventBased ;
      double pTH1F_DTLeading_Hig  =  Cut_Stage11_EventBased ;

      TGraph* pTGraph_For_pTH1F_DTLeading_1 = new TGraph ;
      pTGraph_For_pTH1F_DTLeading_1->SetPoint( pTGraph_For_pTH1F_DTLeading_1->GetN(), pTH1F_DTLeading_Low , 1.E4*pTH1F_DTLeading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_DTLeading_1->SetPoint( pTGraph_For_pTH1F_DTLeading_1->GetN(), pTH1F_DTLeading_Low , 0.1  ) ;
      pTGraph_For_pTH1F_DTLeading_1->SetLineColor(2) ;
  
      TGraph* pTGraph_For_pTH1F_DTLeading_2 = new TGraph ;
      pTGraph_For_pTH1F_DTLeading_2->SetPoint( pTGraph_For_pTH1F_DTLeading_2->GetN(), pTH1F_DTLeading_Hig , 1.E4*pTH1F_DTLeading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_DTLeading_2->SetPoint( pTGraph_For_pTH1F_DTLeading_2->GetN(), pTH1F_DTLeading_Hig , 0.1  ) ;
      pTGraph_For_pTH1F_DTLeading_2->SetLineColor(2) ;
  
      pTGraph_For_pTH1F_DTLeading_1->Draw("l");  
      pTGraph_For_pTH1F_DTLeading_2->Draw("l");  
      pTCanvas->Update();
      pTCanvas->Update();

    pTCanvas->cd(5);
    pTCanvas->cd(5)->SetLogy(1);
      pTH1F_DTLeadingZOOM->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_DTLeading_1->Draw("l");  
      pTGraph_For_pTH1F_DTLeading_2->Draw("l");  
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH1F_DTLeadingZOOM->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_DTLeading_1->Draw("l");  
      pTGraph_For_pTH1F_DTLeading_2->Draw("l");  
      pTCanvas->Update();
  
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 7
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
    pTCanvas->cd(1)->SetLogy() ;
      pTH1F_TauNotLeading->Draw();
      pTCanvas->Update();
  
      TGraph* pTGraph_For_pTH1F_TauNotLeading_1 = new TGraph ;
      pTGraph_For_pTH1F_TauNotLeading_1->SetPoint( pTGraph_For_pTH1F_TauNotLeading_1->GetN(), Cut_Stage13_DT_Low , 1.E4*pTH1F_TauNotLeading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_TauNotLeading_1->SetPoint( pTGraph_For_pTH1F_TauNotLeading_1->GetN(), Cut_Stage13_DT_Low , 0.1  ) ;
      pTGraph_For_pTH1F_TauNotLeading_1->SetLineColor(2) ;

      pTGraph_For_pTH1F_TauNotLeading_1->Draw("l");  
      pTCanvas->Update();

    pTCanvas->cd(2);
    pTCanvas->cd(2)->SetLogy() ;
      pTH1F_TauNotLeadingZOOM->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_TauNotLeading_1->Draw("l");  
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH1F_TauNotLeadingZOOM->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_TauNotLeading_1->Draw("l");  
      pTCanvas->Update();
  
    pTCanvas->cd(4);
      pTH2F_Alpha_VS_Tau_NotLeading->Draw("colz") ;
      pTCanvas->Update();
    
      double pTH2F_Alpha_VS_Tau_NotLeading_Ymin = pTCanvas->cd(4)->GetUymin() ;
      double pTH2F_Alpha_VS_Tau_NotLeading_Ymax = pTCanvas->cd(4)->GetUymax() ;
      TLine* pTLine_pTH2F_Alpha_VS_Tau_NotLeading_1= new TLine( Cut_Stage13_DT_Low  , pTH2F_Alpha_VS_Tau_NotLeading_Ymin , Cut_Stage13_DT_Low , pTH2F_Alpha_VS_Tau_NotLeading_Ymax ) ;

      pTLine_pTH2F_Alpha_VS_Tau_NotLeading_1->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_Alpha_VS_Tau_NotLeading_1->Draw();  
      pTCanvas->Update();
  
    pTCanvas->cd(5);
      pTH2F_Alpha_VS_Tau_NotLeadingZOOM->Draw("colz") ;
      pTCanvas->Update();
    
      pTLine_pTH2F_Alpha_VS_Tau_NotLeading_1->Draw();  
      pTCanvas->Update();
  
  pTCanvas->SaveAs(OutputFile.c_str());


//Page 8
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
      pTH2F_TauNotLeading_VS_TmaxLeading->Draw("colz") ;
      pTCanvas->Update();     
    
      double pTH2F_TauNotLeading_VS_TmaxLeading_Xmin = pTCanvas->cd(1)->GetUxmin() ;
      double pTH2F_TauNotLeading_VS_TmaxLeading_Xmax = pTCanvas->cd(1)->GetUxmax() ;
      double pTH2F_TauNotLeading_VS_TmaxLeading_Ymin = pTCanvas->cd(1)->GetUymin() ;
      double pTH2F_TauNotLeading_VS_TmaxLeading_Ymax = pTCanvas->cd(1)->GetUymax() ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A1= new TLine( pTH2F_TauNotLeading_VS_TmaxLeading_Xmin , Cut_Stage13_DT_Low  , pTH2F_TauNotLeading_VS_TmaxLeading_Xmax , Cut_Stage13_DT_Low ) ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A2= new TLine( pTH1F_Tleading_Low  , pTH2F_TauNotLeading_VS_TmaxLeading_Ymin , pTH1F_Tleading_Low , pTH2F_TauNotLeading_VS_TmaxLeading_Ymax ) ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A3= new TLine( pTH1F_Tleading_Hig  , pTH2F_TauNotLeading_VS_TmaxLeading_Ymin , pTH1F_Tleading_Hig , pTH2F_TauNotLeading_VS_TmaxLeading_Ymax ) ;

      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A1->SetLineColor( 2 ) ;
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A2->SetLineColor( 2 ) ;
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A3->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A1->Draw();  
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A2->Draw();  
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A3->Draw();  
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_TauNotLeading_VS_TmaxLeadingZOOM->Draw("colz") ;
      pTCanvas->Update();     

      pTH2F_TauNotLeading_VS_TmaxLeading_Xmin = pTCanvas->cd(2)->GetUxmin() ;
      pTH2F_TauNotLeading_VS_TmaxLeading_Xmax = pTCanvas->cd(2)->GetUxmax() ;
      pTH2F_TauNotLeading_VS_TmaxLeading_Ymin = pTCanvas->cd(2)->GetUymin() ;
      pTH2F_TauNotLeading_VS_TmaxLeading_Ymax = pTCanvas->cd(2)->GetUymax() ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B1= new TLine( pTH2F_TauNotLeading_VS_TmaxLeading_Xmin , Cut_Stage13_DT_Low  , pTH2F_TauNotLeading_VS_TmaxLeading_Xmax , Cut_Stage13_DT_Low ) ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B2= new TLine( pTH1F_Tleading_Low  , pTH2F_TauNotLeading_VS_TmaxLeading_Ymin , pTH1F_Tleading_Low , pTH2F_TauNotLeading_VS_TmaxLeading_Ymax ) ;
      TLine* pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B3= new TLine( pTH1F_Tleading_Hig  , pTH2F_TauNotLeading_VS_TmaxLeading_Ymin , pTH1F_Tleading_Hig , pTH2F_TauNotLeading_VS_TmaxLeading_Ymax ) ;

      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B1->SetLineColor( 2 ) ;
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B2->SetLineColor( 2 ) ;
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B3->SetLineColor( 2 ) ;
      
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B1->Draw();  
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B2->Draw();  
      pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B3->Draw();  
      pTCanvas->Update();

  pTCanvas->SaveAs(OutputFile.c_str());

//Page 9
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
    pTCanvas->cd(1)->SetLogy(1);
      pTH1F_Amax->Draw();
      pTCanvas->Update();
    
      TGraph* pTGraph_For_pTH1F_Amax_1 = new TGraph ;
      pTGraph_For_pTH1F_Amax_1->SetPoint( pTGraph_For_pTH1F_Amax_1->GetN(), Cut_Stage14_Amax_Hig , 1.E4*pTH1F_Amax->GetMaximum()  ) ;
      pTGraph_For_pTH1F_Amax_1->SetPoint( pTGraph_For_pTH1F_Amax_1->GetN(), Cut_Stage14_Amax_Hig , 0.1  ) ;
      pTGraph_For_pTH1F_Amax_1->SetLineColor(2) ;

      TGraph* pTGraph_For_pTH1F_Amax_2 = new TGraph ;
      pTGraph_For_pTH1F_Amax_2->SetPoint( pTGraph_For_pTH1F_Amax_2->GetN(), Cut_Stage6_A_Low , 1.E4*pTH1F_Amax->GetMaximum()  ) ;
      pTGraph_For_pTH1F_Amax_2->SetPoint( pTGraph_For_pTH1F_Amax_2->GetN(), Cut_Stage6_A_Low , 0.1  ) ;
      pTGraph_For_pTH1F_Amax_2->SetLineColor(2) ;

      pTGraph_For_pTH1F_Amax_1->Draw("l");  
      pTGraph_For_pTH1F_Amax_2->Draw("l");  
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH1F_AmaxZOOM->Draw();
      pTCanvas->Update();

//       double pTH1F_Amax_Ymin = pTCanvas->cd(2)->GetUymin() ;
//       double pTH1F_Amax_Ymax = pTCanvas->cd(2)->GetUymax() ;
//       TLine* pTLine_pTH1F_Amax_A1= new TLine(  Cut_Stage14_Amax_Hig , pTH1F_Amax_Ymin , Cut_Stage14_Amax_Hig  , pTH1F_Amax_Ymax) ;
// 
//       pTLine_pTH1F_Amax_A1->SetLineColor( 2 ) ;
//       
//       pTLine_pTH1F_Amax_A1->Draw();  
//       pTCanvas->Update();

      pTGraph_For_pTH1F_Amax_1->Draw("l");  
      pTGraph_For_pTH1F_Amax_2->Draw("l");  
      pTCanvas->Update();
    
    pTCanvas->cd(4);
    pTCanvas->cd(4)->SetLogy(1);
      pTH1F_AmaxZOOM2->Draw();
      pTCanvas->Update();

      pTGraph_For_pTH1F_Amax_1->Draw("l");  
      pTGraph_For_pTH1F_Amax_2->Draw("l");  
      pTCanvas->Update();
    
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 10
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
    pTCanvas->cd(1)->SetLogy(1);
      pTH1F_AlphaNotLeading->Draw();
      pTCanvas->Update();
     
      TGraph* pTGraph_For_pTH1F_AlphaNotLeading = new TGraph ;
      pTGraph_For_pTH1F_AlphaNotLeading->SetPoint( pTGraph_For_pTH1F_AlphaNotLeading->GetN(), Cut_Stage6_ARelat_Low , 1.E4*pTH1F_AlphaNotLeading->GetMaximum()  ) ;
      pTGraph_For_pTH1F_AlphaNotLeading->SetPoint( pTGraph_For_pTH1F_AlphaNotLeading->GetN(), Cut_Stage6_ARelat_Low , 0.1  ) ;
      pTGraph_For_pTH1F_AlphaNotLeading->SetLineColor(2) ;
      pTGraph_For_pTH1F_AlphaNotLeading->Draw("l");  
      pTCanvas->Update();
    
    pTCanvas->cd(2);
    pTCanvas->cd(2)->SetLogy(1);
      pTH1F_Amax_Leading->Draw();
      pTCanvas->Update();
     
        
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 11
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
    pTCanvas->cd(1)->SetLogy(1);
      pTH1F_Tmax->Draw();
      pTCanvas->Update();

  pTCanvas->SaveAs(OutputFile.c_str());

//Page 12
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMax_WFmAMax_FIT->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMax_WFmTMax_FIT->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF->Draw("colz") ;
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF_Z->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF_Z->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());


//Page 13
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMaxmAMax_FIT->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMaxmTMax_FIT->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMaxmAMax_FIT_VS_AMax->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMaxmTMax_FIT_VS_AMax->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());

//Page 14
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMax_FITmAMax_MC->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMax_FITmTMax_MC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMax_FITmAMax_MC_VS_AMax_MC->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMax_FITmTMax_MC_VS_AMax_MC->Draw("colz") ;
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH2F_AMax_FITmAMax_MC_VS_AMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_TMax_FITmTMax_MC_VS_AMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());


//Page 15
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMax_FITmAMax_MC->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMax_FITmTMax_MC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMax_FITmAMax_MC_VS_TMax_MC->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMax_FITmTMax_MC_VS_TMax_MC->Draw("colz") ;
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH2F_AMax_FITmAMax_MC_VS_TMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_TMax_FITmTMax_MC_VS_TMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());


//Page 16
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(2,1); 
  
    pTCanvas->cd(1);
      pTProfile_TMax_FITmTMax_MC_VS_TMax_MC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
     TMultiGraph* pTMultiGraph_TimEr = new TMultiGraph ;
      TGraph* pTGraph_TimEr_Limit_TimeError = new TGraph ;
      TGraph* pTGraph_TimEr_Limit_Max       = new TGraph ;
      TGraph* pTGraph_TimEr_Limit           = new TGraph ;
      TGraph* pTGraph_TimEr_Mean            = new TGraph ;
      TGraph* pTGraph_TimEr_Sigm            = new TGraph ;
      pTGraph_TimEr_Limit_TimeError ->SetLineColor(3) ;  
      pTGraph_TimEr_Limit_Max       ->SetLineColor(2) ;  
      pTGraph_TimEr_Limit           ->SetLineColor(2) ;  
      pTGraph_TimEr_Mean            ->SetLineColor(1) ; 
      pTGraph_TimEr_Sigm            ->SetLineColor(4) ; 
      pTMultiGraph_TimEr->Add( pTGraph_TimEr_Limit_TimeError );
      pTMultiGraph_TimEr->Add( pTGraph_TimEr_Limit_Max       );
      pTMultiGraph_TimEr->Add( pTGraph_TimEr_Limit           );
      pTMultiGraph_TimEr->Add( pTGraph_TimEr_Mean            );
      pTMultiGraph_TimEr->Add( pTGraph_TimEr_Sigm            );
      double TimEr_Limit_Max = 0. ;
      double TimEr_NberOfEntrie = pTProfile_TMax_FITmTMax_MC_VS_TMax_MC->GetXaxis()->GetNbins() ;
      for (int iTem = 1 ; iTem <= TimEr_NberOfEntrie ; iTem++){
        double Time = pTProfile_TMax_FITmTMax_MC_VS_TMax_MC->GetXaxis()->GetBinCenter(iTem) ;
        double Mean = pTProfile_TMax_FITmTMax_MC_VS_TMax_MC->GetBinContent(iTem) ;
        double Sigm = pTProfile_TMax_FITmTMax_MC_VS_TMax_MC->GetBinError(iTem) ;
        double BigSigma  = std::fabs( Mean ) + Sigm ;
        double BigMean   = 5.*std::fabs( Mean ) ;
        double Limit = std::max(BigMean,2.*BigSigma) ;
               Limit = std::max(2.,Limit) ;
        if (TimEr_Limit_Max < Limit ) TimEr_Limit_Max = Limit ;
//         std::cout 
//           << " iTem "      << std::setw(3) << std::setprecision(0) << iTem
//           << " Time "      << std::setw(4) << std::setprecision(0) << Time
//           << " mean  "     << std::setw(4) << std::setprecision(1) << Mean
//           << " sigma "     << std::setw(4) << std::setprecision(1) << Sigm
//           << " Limit "     << std::setw(3) << std::setprecision(0) << Limit
//           << " Limit_Max " << std::setw(3) << std::setprecision(0) << TimEr_Limit_Max
//           << std::endl ;
        pTGraph_TimEr_Limit_TimeError->SetPoint( pTGraph_TimEr_Limit_TimeError->GetN(), Time , TimeError(Time) ) ;
        pTGraph_TimEr_Limit_Max      ->SetPoint( pTGraph_TimEr_Limit_Max      ->GetN(), Time , TimEr_Limit_Max ) ;
        pTGraph_TimEr_Limit          ->SetPoint( pTGraph_TimEr_Limit          ->GetN(), Time , Limit ) ;
        pTGraph_TimEr_Mean           ->SetPoint( pTGraph_TimEr_Mean           ->GetN(), Time , std::fabs( Mean ) ) ;
        pTGraph_TimEr_Sigm           ->SetPoint( pTGraph_TimEr_Sigm           ->GetN(), Time , Sigm ) ;
      }
      pTMultiGraph_TimEr->SetMaximum( 30. );
      pTMultiGraph_TimEr->SetMinimum(  0. );
      pTMultiGraph_TimEr->Draw("al");

      
  pTCanvas->SaveAs(OutputFile.c_str());


//Page 17
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMax_FromSetmAMax_MC->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMax_FromSetmTMax_MC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC->Draw("colz") ;
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());


//Page 18
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 

    pTCanvas->cd(1);
      pTH1F_AMax_FromSetmAMax_MC->Draw();
      pTCanvas->Update();
    
    pTCanvas->cd(4);
      pTH1F_TMax_FromSetmTMax_MC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2);
      pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(5);
      pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC->Draw("colz") ;
      pTCanvas->Update();
    
    pTCanvas->cd(3);
      pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();

    pTCanvas->cd(6);
      pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC_Z->Draw("colz") ;
      pTCanvas->Update();
    

  pTCanvas->SaveAs(OutputFile.c_str());


//Page 19
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(4);
    pTProfile_Rho_DeltaT->Draw();
    pTCanvas->Update();
    
    pTCanvas->cd(2);
    pTH2F_Rho_YwYpad->Draw("colz") ;
    pTCanvas->Update();
    
    pTCanvas->cd(5);
    pTProfile_Rho_YwYpad->Draw();
    pTCanvas->Update();
    
    pTCanvas->cd(3);
    pTH2F_DeltaT_YwYpad->Draw("colz") ;
    pTCanvas->Update();
    
    pTCanvas->cd(6);
    pTProfile_DeltaT_YwYpad->Draw();
    pTCanvas->Update();
    
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 20
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(3,2); 
  
    pTCanvas->cd(1);
    pTH1F_SlopeXY->Draw();
    pTCanvas->Update();
   
      TGraph* pCut_Stage15_1 = new TGraph ;
      pCut_Stage15_1->SetPoint( pCut_Stage15_1->GetN(), Cut_Stage15_CutSlopeXY_Hig , 1000.  ) ;
      pCut_Stage15_1->SetPoint( pCut_Stage15_1->GetN(), Cut_Stage15_CutSlopeXY_Hig , 0.1    ) ;
      pCut_Stage15_1->SetLineColor(2) ;
      pCut_Stage15_1->Draw("l");  
      pTCanvas->Update();
  
      TGraph* pCut_Stage15_2 = new TGraph ;
      pCut_Stage15_2->SetPoint( pCut_Stage15_2->GetN(), Cut_Stage15_CutSlopeXY_Low , 1000.  ) ;
      pCut_Stage15_2->SetPoint( pCut_Stage15_2->GetN(), Cut_Stage15_CutSlopeXY_Low , 0.1    ) ;
      pCut_Stage15_2->SetLineColor(2) ;
      pCut_Stage15_2->Draw("l");  
      pTCanvas->Update();
  
    pTCanvas->cd(2);
    pTH1F_SlopeXZ->Draw();
    pTCanvas->Update();

      TGraph* pCut_Stage16_1 = new TGraph ;
      pCut_Stage16_1->SetPoint( pCut_Stage16_1->GetN(), Cut_Stage16_CutSlopeXZ_Hig , 1000.  ) ;
      pCut_Stage16_1->SetPoint( pCut_Stage16_1->GetN(), Cut_Stage16_CutSlopeXZ_Hig , 0.1    ) ;
      pCut_Stage16_1->SetLineColor(2) ;
      pCut_Stage16_1->Draw("l");  
      pTCanvas->Update();
  
      TGraph* pCut_Stage16_2 = new TGraph ;
      pCut_Stage16_2->SetPoint( pCut_Stage16_2->GetN(), Cut_Stage16_CutSlopeXZ_Low , 1000.  ) ;
      pCut_Stage16_2->SetPoint( pCut_Stage16_2->GetN(), Cut_Stage16_CutSlopeXZ_Low , 0.1    ) ;
      pCut_Stage16_2->SetLineColor(2) ;
      pCut_Stage16_2->Draw("l");  
      pTCanvas->Update();
   
    pTCanvas->cd(3);
    pTH1F_InterCeptXZ->Draw();
    pTCanvas->Update();

      TGraph* pCut_Stage17_1 = new TGraph ;
      pCut_Stage17_1->SetPoint( pCut_Stage17_1->GetN(), Cut_Stage17_CutInterCeptXZ_Hig , 1000.  ) ;
      pCut_Stage17_1->SetPoint( pCut_Stage17_1->GetN(), Cut_Stage17_CutInterCeptXZ_Hig , 0.1    ) ;
      pCut_Stage17_1->SetLineColor(2) ;
      pCut_Stage17_1->Draw("l");  
      pTCanvas->Update();
  
      TGraph* pCut_Stage17_2 = new TGraph ;
      pCut_Stage17_2->SetPoint( pCut_Stage17_2->GetN(), Cut_Stage17_CutInterCeptXZ_Low , 1000.  ) ;
      pCut_Stage17_2->SetPoint( pCut_Stage17_2->GetN(), Cut_Stage17_CutInterCeptXZ_Low , 0.1    ) ;
      pCut_Stage17_2->SetLineColor(2) ;
      pCut_Stage17_2->Draw("l");  
      pTCanvas->Update();
  
    pTCanvas->cd(4);
    pTH1F_SlopeXYZ1->Draw();
    pTCanvas->Update();

      TGraph* pCut_Stage18_SlopeXYZ1_1 = new TGraph ;
      pCut_Stage18_SlopeXYZ1_1->SetPoint( pCut_Stage18_SlopeXYZ1_1->GetN(), Cut_Stage18_CutSlopeXYZ1 , 1000.  ) ;
      pCut_Stage18_SlopeXYZ1_1->SetPoint( pCut_Stage18_SlopeXYZ1_1->GetN(), Cut_Stage18_CutSlopeXYZ1 , 0.1    ) ;
      pCut_Stage18_SlopeXYZ1_1->SetLineColor(2) ;
      pCut_Stage18_SlopeXYZ1_1->Draw("l");  
      pTCanvas->Update();
  
      TGraph* pCut_Stage18_SlopeXYZ1_2 = new TGraph ;
      pCut_Stage18_SlopeXYZ1_2->SetPoint( pCut_Stage18_SlopeXYZ1_2->GetN(), -Cut_Stage18_CutSlopeXYZ1 , 1000.  ) ;
      pCut_Stage18_SlopeXYZ1_2->SetPoint( pCut_Stage18_SlopeXYZ1_2->GetN(), -Cut_Stage18_CutSlopeXYZ1 , 0.1    ) ;
      pCut_Stage18_SlopeXYZ1_2->SetLineColor(2) ;
      pCut_Stage18_SlopeXYZ1_2->Draw("l");  
      pTCanvas->Update();
  
    pTCanvas->cd(5);
    pTH1F_SlopeXYZ0->Draw();
    pTCanvas->Update();
   
      TGraph* pCut_Stage18_SlopeXYZ0_1 = new TGraph ;
      pCut_Stage18_SlopeXYZ0_1->SetPoint( pCut_Stage18_SlopeXYZ0_1->GetN(), Cut_Stage18_CutSlopeXYZ0 , 1000.  ) ;
      pCut_Stage18_SlopeXYZ0_1->SetPoint( pCut_Stage18_SlopeXYZ0_1->GetN(), Cut_Stage18_CutSlopeXYZ0 , 0.1    ) ;
      pCut_Stage18_SlopeXYZ0_1->SetLineColor(2) ;
      pCut_Stage18_SlopeXYZ0_1->Draw("l");  
      pTCanvas->Update();
  
      TGraph* pCut_Stage18_SlopeXYZ0_2 = new TGraph ;
      pCut_Stage18_SlopeXYZ0_2->SetPoint( pCut_Stage18_SlopeXYZ0_2->GetN(), -Cut_Stage18_CutSlopeXYZ0 , 1000.  ) ;
      pCut_Stage18_SlopeXYZ0_2->SetPoint( pCut_Stage18_SlopeXYZ0_2->GetN(), -Cut_Stage18_CutSlopeXYZ0 , 0.1    ) ;
      pCut_Stage18_SlopeXYZ0_2->SetLineColor(2) ;
      pCut_Stage18_SlopeXYZ0_2->Draw("l");  
      pTCanvas->Update();
   
   
  pTCanvas->SaveAs(OutputFile.c_str());

  // Samira Page 21
  //                                                                                                                        
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Divide(1,2);

    pTCanvas->cd(1);
    pTH2F_XZ_Display->Draw("colz");
    pTCanvas->Update();

    pTCanvas->cd(2);
    pTH2F_XY_Display->Draw("colz");
    pTCanvas->Update();
   

  pTCanvas->SaveAs(OutputFile.c_str());
//Page 21
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(OutputFile_End.c_str());

//  
  delete pTLegend ;

  delete pTH1F_SlopeXY ;
  delete pTH1F_SlopeXZ ;
  delete pTH1F_InterCeptXZ ;

  delete pTH1F_MeanClusterMultiplicity ;

  delete pTH1I_PadPerCluster          ;
  delete pTH1I_ClusterPerEvent        ;
  delete pTProfile_Rho_YwYpad         ;

  delete pTH1F_Tleading               ;
  delete pTH1F_TleadingZOOM           ;
  delete pTH1I_LeadingiY              ;
  delete pTH1F_Yw                     ;
  delete pTH1F_Rho                    ;
  delete pTH1F_Rho2                   ;
  delete pTH1F_Rho3                   ;
  delete pTH1F_Rho4                   ;

  delete pTH1F_YwYpad                 ;
  delete pTProfile_DeltaT_YwYpad      ;

  delete pTProfile_Rho_DeltaT         ;
  delete pTH2F_Rho_YwYpad             ;
  delete pTH2F_DeltaT_YwYpad          ;
  
  delete pTH1F_Tmax                    ;
    
  delete pTGraph_Alpha2_VS_Alpha3 ;
  delete pTH2F_Alpha2_VS_Alpha3 ;
  delete pTH2F_Alpha2_VS_Alpha3ZOOM ;

  delete pTGraph_Alpha2_VS_Alpha3onAlpha2 ;
  delete pTH2F_Alpha2_VS_Alpha3onAlpha2 ;

  delete pTGraph_Tau2_VS_Tau3 ;
  delete pTH2F_Tau2_VS_Tau3 ;
  delete pTH2F_Tau2_VS_Tau3ZOOM ;
  
  delete pTGraph_Tau2_VS_Tau3mTau2 ;
  delete pTH2F_Tau2_VS_Tau3mTau2 ;
  delete pTH2F_Tau2_VS_Tau3mTau2ZOOM ;
  
  delete pTH1F_DTLeading              ;
  delete pTH1F_DTLeadingZOOM          ;
  
  delete pTH1F_TauNotLeading                 ;
  delete pTH1F_TauNotLeadingZOOM             ;

  delete pTH2F_Alpha_VS_Tau_NotLeading             ;
  delete pTH2F_Alpha_VS_Tau_NotLeadingZOOM         ;
  
  delete pTH2F_TauNotLeading_VS_TmaxLeading           ;
  delete pTH2F_TauNotLeading_VS_TmaxLeadingZOOM       ;

  delete pTH1F_Amax                    ;
  delete pTH1F_AmaxZOOM                ;
  delete pTH1F_AmaxZOOM2               ;

  delete pTH1F_PadMultiplicityPerEvent               ;

  delete pTH1F_AMax_WFmAMax_FIT            ;
  delete pTH1F_TMax_WFmTMax_FIT            ;
  delete pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF ;
  delete pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF ;
                                      
  delete pTH1F_AMaxmAMax_FIT         ;
  delete pTH1F_TMaxmTMax_FIT         ;
  delete pTH2F_AMaxmAMax_FIT_VS_AMax ;
  delete pTH2F_TMaxmTMax_FIT_VS_AMax ;

  delete pTH1F_AMax_FITmAMax_MC              ;
  delete pTH1F_TMax_FITmTMax_MC              ;
  delete pTH2F_AMax_FITmAMax_MC_VS_AMax_MC   ;
  delete pTH2F_TMax_FITmTMax_MC_VS_AMax_MC   ;
  delete pTH2F_AMax_FITmAMax_MC_VS_AMax_MC_Z ;
  delete pTH2F_TMax_FITmTMax_MC_VS_AMax_MC_Z ;
                                      
  delete pTH1F_AMax_FromSetmAMax_MC              ;
  delete pTH1F_TMax_FromSetmTMax_MC              ;
  delete pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC   ;
  delete pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC   ;
  delete pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC_Z ;
  delete pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC_Z ;
                                      
  delete pTH2F_AMax_FITmAMax_MC_VS_TMax_MC   ;
  delete pTH2F_TMax_FITmTMax_MC_VS_TMax_MC   ;
  delete pTH2F_AMax_FITmAMax_MC_VS_TMax_MC_Z ;
  delete pTH2F_TMax_FITmTMax_MC_VS_TMax_MC_Z ;
                                      
  delete pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC   ;
  delete pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC   ;
  delete pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC_Z ;
  delete pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC_Z ;

  delete pTProfile_TMax_FITmTMax_MC_VS_TMax_MC ;

  delete pTMultiGraph_TimEr ;

  delete pTGraph_For_pTH1F_Tleading_1  ;
  delete pTGraph_For_pTH1F_Tleading_2  ;
  delete pTGraph_For_pTH1F_Tleading_3  ;
  delete pTGraph_For_pTH1F_DTLeading_1  ;
  delete pTGraph_For_pTH1F_DTLeading_2  ;
  delete pTGraph_For_pTH1F_TauNotLeading_1  ;
  delete pTGraph_For_pTH1F_Amax_1  ;
  delete pTGraph_For_pTH1F_Amax_2  ;
  delete pTGraph_For_pTH1F_AlphaNotLeading  ;
  
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A1          ;       
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A2          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_A3          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B1          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B2          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_B3          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C1          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C2          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3_Tleading_C3          ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A2  ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_A1  ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B2  ;      
  delete pTLine_pTGraph_Alpha2_VS_Alpha3onAlpha2_Tleading_B1  ;      
  delete ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A1                ;      
  delete ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A2                ;      
  delete ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A3                ;      
  delete ppTLine_TH2F_Tau2_VS_Tau3_Tleading_A4                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B1                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B2                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B3                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_B4                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C1                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C2                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C3                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3_Tleading_C4                ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A1           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A2           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A3           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_A4           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B1           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B2           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B3           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_B4           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C1           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C2           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C3           ;      
  delete pTLine_pTH2F_Tau2_VS_Tau3mTau2_Tleading_C4           ;      
  delete pTLine_pTH2F_Alpha_VS_Tau_NotLeading_1        ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A1  ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A2  ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_A3  ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B1  ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B2  ;      
  delete pTLine_pTH2F_TauNotLeading_VS_TmaxLeading_B3  ;      

  delete pTF1_pTGraph_Alpha2_VS_Alpha3_A         ; 
  delete pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_A ;
  delete pTF1_pTGraph_Alpha2_VS_Alpha3onAlpha2_B ;
  delete pTF1_pTGraph_Tau2_VS_Tau3_A             ;
  delete pTF1_pTGraph_Tau2_VS_Tau3_B             ;
  delete pTF1_pTGraph_Tau2_VS_Tau3mTau2_A        ;
  delete pTF1_pTGraph_Tau2_VS_Tau3mTau2_B        ;

  //Samira
 delete pTH1F_SlopeXYZ0 ;
 delete pTH1F_SlopeXYZ1 ;
 delete pTH2F_XZ_Display ;
 delete pTH2F_XY_Display ;
//
  gStyle->SetOptStat(OPTSTAT_IN);

}

