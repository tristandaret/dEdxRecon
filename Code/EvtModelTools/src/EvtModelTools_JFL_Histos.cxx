#include "EvtModelTools/EvtModelTools_JFL_Histos.h"
#include "Misc/Util.h"
#include "Misc/ParabolaFunction.h"
#include "Misc/ParabolaFunctionNG.h"

#include "EvtModelTools/EvtModelTools_Histos.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"

//Output waveform of a pad  with tagging string TAG, placed in OUTDIR dir
void DrawOut_JF_WaveFormDisplay(const Pad* pPad,const std::string& OUTDIR,const std::string& TAG,const int& Opt)
{
  MakeMyDir(OUTDIR) ;

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << OUTDIR ;
  aostringstream << TAG ;
  aostringstream << "_WaveForm" ;
  aostringstream << "_Ent_" << pPad->Get_EntryNber() ;
  aostringstream << "_EVt_" << pPad->Get_EventNber() ;
  aostringstream << "_Mod_" << pPad->Get_ModuleNber() ;
  aostringstream << "_iX_"  << pPad->Get_iX();
  aostringstream << "_iY_"  << pPad->Get_iY();
  aostringstream << ".png" ;
  std::string OutputFile =  aostringstream.str() ;

  double AMax_WF = pPad->Get_AMax_WF() ;
  double TMax_WF = pPad->Get_TMax_WF() ;
  double WIDTH = 100.5 ;
  if (Opt==1) WIDTH = 20.5 ;
  if (Opt==2) WIDTH = 75.5 ;
  TH1F* pTH1F = GiveMe_JF_WaveFormDisplay(pPad,TAG,TMax_WF-WIDTH,TMax_WF+WIDTH) ;

  TCanvas* pTCanvas = new TCanvas;
  pTCanvas->cd();
  pTH1F->Draw();
  pTCanvas->Update()    ;
  
  double Xmin = pTCanvas->GetUxmin() ;
  double Xmax = pTCanvas->GetUxmax() ;
  double Ymin = pTCanvas->GetUymin() ;
  double Ymax = pTCanvas->GetUymax() ;
  
//  
  TLine* pTLineV_WF = new TLine( TMax_WF  , Ymin , TMax_WF , Ymax ) ;
  pTLineV_WF->SetLineStyle(10); 
  pTLineV_WF->SetLineWidth(1); 
  pTLineV_WF->SetLineColor(4) ; 
  pTLineV_WF->Draw();  
  pTCanvas->Update()    ;
  
  TLine* pTLineH_WF = new TLine( Xmin , AMax_WF  , Xmax , AMax_WF  ) ;
  pTLineH_WF->SetLineStyle(10); 
  pTLineH_WF->SetLineWidth(1); 
  pTLineH_WF->SetLineColor(4) ; 
  pTLineH_WF->Draw();  
  pTCanvas->Update()    ;
  
  TGraph* pTGraph_WF = new TGraph ;
  pTGraph_WF->SetPoint( pTGraph_WF->GetN(), TMax_WF , AMax_WF  ) ;
  pTGraph_WF->SetMarkerStyle(25) ;
  pTGraph_WF->SetMarkerColor(4) ;
  pTGraph_WF->Draw("P");  
  pTCanvas->Update()    ;
  
  double T_FitMin = pPad->Get_FIT_Xmin() ;
  double T_FitMax = pPad->Get_FIT_Xmax() ;
  
  ParabolaFunctionNG aParabolaFunctionNG ;
  TF1* pTF1 = new TF1("ParabolaFunctionNG",aParabolaFunctionNG ,T_FitMin ,T_FitMax,4);

  pTF1->SetParameter(0,pPad->Get_FIT_A0P());
  pTF1->SetParameter(1,pPad->Get_FIT_A0M());
  pTF1->SetParameter(2,pPad->Get_FIT_X0 ());
  pTF1->SetParameter(3,pPad->Get_FIT_Y0 ());
  pTF1->Draw("Same");
  pTCanvas->Update()    ;
        
  double X_MaxFit = pPad->Get_FIT_X0() ;
  double Y_MaxFit = pPad->Get_FIT_Y0() ;
  TGraph* pTGraph_MaxFit = new TGraph ;
  pTGraph_MaxFit->SetPoint( pTGraph_MaxFit->GetN(), X_MaxFit , Y_MaxFit  ) ;
  pTGraph_MaxFit->SetMarkerStyle(24) ;
  pTGraph_MaxFit->SetMarkerColor(2) ;
  pTGraph_MaxFit->Draw("P");  
  pTCanvas->Update()    ;
 
  TLegend* pTLegend_A = new TLegend(0.40,0.15 ,0.60,0.23  ); 
  pTLegend_A->SetFillStyle(1001);
  pTLegend_A->SetBorderSize(1);

  std::ostringstream aostringstream2 ;
  aostringstream2 << std::setiosflags(std::ios::fixed) ;
  aostringstream2 << " #Delta A=" << std::setw(6) << std::setprecision(0) <<  pPad->Get_AMax_WF() - pPad->Get_AMax_FIT();
  aostringstream2 << " #Delta T=" << std::setw(5) << std::setprecision(0) <<  pPad->Get_TMax_WF() - pPad->Get_TMax_FIT();
  std::string Comment =  aostringstream2.str() ;

  pTLegend_A->AddEntry( pTF1 , Comment.c_str() , "p");

  pTLegend_A->Draw();       
  pTCanvas->Update();

  pTCanvas->SaveAs(OutputFile.c_str());
  delete pTH1F ;
  delete pTF1 ;
  delete pTCanvas ;
}

//Get waveform histo of pad with tagging string TAG
TH1F* GiveMe_JF_WaveFormDisplay(const Pad* pPad,const std::string& TAG, const double& Tmin, const double& Tmax)
{
  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << TAG ;
  aostringstream << " WaveForm "  ;
  aostringstream << " Entry " << pPad->Get_EntryNber() ;
  aostringstream << " Event " << pPad->Get_EventNber() ;
  aostringstream << " Mod "   << pPad->Get_ModuleNber() ;
  aostringstream << " iX "    << pPad->Get_iX();
  aostringstream << " iY "    << pPad->Get_iY();
  aostringstream << ";Time (/40 ns);ADC"    ;
  std::string Title =  aostringstream.str() ;

  std::ostringstream namestring ;
  namestring << std::setiosflags(std::ios::fixed) ;
  namestring << " pTH1_WF_Evt_"  ;
  namestring << pPad->Get_EventNber() ;
  namestring << "_iX" << pPad->Get_iX();
  namestring << "_iY" << pPad->Get_iY();
  namestring << "_Mod" << pPad->Get_ModuleNber();
  std::string Name =  namestring.str() ;

  int Nbins = Tmax-Tmin ;
  TH1F* ToBeReturned = new TH1F(Name.c_str(), Title.c_str(),Nbins,Tmin,Tmax);
  
  std::vector < int > The_v_ADC = pPad->Get_vADC() ;
  int NADC = The_v_ADC.size() ;
  for (Int_t iTimeBin = 0; iTimeBin < NADC; iTimeBin++){
    int ADC_value = The_v_ADC[iTimeBin] ;
    if(ADC_value <= -250) continue;
    int iTimeBinLoc = iTimeBin - (Tmin+0.5)  + 1 ;
    if ( iTimeBinLoc >= 1 && iTimeBinLoc <= Nbins )
    ToBeReturned->SetBinContent( iTimeBinLoc ,ADC_value);         
  } 
  
  return ToBeReturned ;
}

TH1F*     GiveMe_TH1F_MeanClusterMultiplicity ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_TH1F_MeanClusterMultiplicity" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": Mean Cluster multiplicity" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, -0.5 , 5.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    double MeanMul = 0. ;
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      MeanMul += double(NPads) ;
    }
    MeanMul = MeanMul/double(NClusters);
    ToBeReturned->Fill( MeanMul ) ;
  }
  return ToBeReturned ;
}


TH1F* GiveMe_pTH1F_Rho (Sample& aSample, const int& ModuleNber)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Rho" + NameMODULE2 ; 
  std::string Title = NameMODULE1 + ":#rho = A_{Max}/A^{Cluster}" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100,  0. , 1. );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      double A_cluster = pCluster->Get_Acluster()  ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double rho = pPad->Get_AMax()/A_cluster;
        
        ToBeReturned->Fill( rho ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_YTrackYPad (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
){
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_YTrackYPad(aSample, ModuleNber , SubTAG, 100, -3. , 3. ) ;
}
TH1F* GiveMe_pTH1F_YTrackYPad ( 
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& Nbins, 
                                                    const double& Xmin, 
                                                    const double& Xmax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_YTrackYPad" + TAG + NameMODULE2  ; 
  std::string Title = TAG + NameMODULE1 + ": Y_{T}-Y_{Pad} (cm)" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),Nbins, Xmin , Xmax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack = pCluster->Get_YTrack()        ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double YTrackYPad = YTrack-pPad->Get_YPad() ;
        ToBeReturned->Fill( YTrackYPad*1.E2 ) ;
        
      }
    }
  }
  
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_YTrackYPadLeading (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
){
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_YTrackYPadLeading(aSample, ModuleNber , SubTAG, 100, -3. , 3. ) ;
}
TH1F* GiveMe_pTH1F_YTrackYPadLeading (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& Nbins, 
                                                    const double& Xmin, 
                                                    const double& Xmax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_YTrackYPadLeading" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": Y_{T}-Y_{Pad}^{Leading} (cm)" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),Nbins, Xmin , Xmax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack            = pCluster->Get_YTrack()    ;
      double YLeading          = pCluster->Get_YLeading()  ;
      double YTrackYPadLeading = YTrack-YLeading ;
      
      ToBeReturned->Fill( YTrackYPadLeading*1.E2 ) ;
       
    }
  }
  
  return ToBeReturned ;
}

TProfile* GiveMe_pTProfile_rho_VS_YTrackYPad (
                                                 Sample& aSample, const int& ModuleNber ,
                                                 const std::string& TAG,
                                                 const double& NberOfBinsForProfile, 
                                                 const double& vY_min_DiffPRF , 
                                                 const double& vY_max_DiffPRF
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_rho_VS_YTrackYPad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #rho VS #left[ Y^{T}-Y_{Pad} #right] (cm)" ;
  
  TProfile* ToBeReturned = new TProfile (Name.c_str(), Title.c_str(),NberOfBinsForProfile, vY_min_DiffPRF , vY_max_DiffPRF, 0. , 1.) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack = pCluster->Get_YTrack()        ;

      double A_cluster = pCluster->Get_Acluster()  ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double rho        = pPad->Get_AMax()/A_cluster;
        double YTrackYPad = YTrack-pPad->Get_YPad() ;
        
        ToBeReturned->Fill( YTrackYPad*1.E2 , rho ) ;
        
      }
    }
  }
  return ToBeReturned ;
}

TH2D* GiveMe_pTH2D_rho_VS_YTrackYPad (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG
){
  const  Model_ReadOutGeometry* pModel_ReadOutGeometry =  aSample.Get_Model_ReadOutGeometry() ;
  double LY       = pModel_ReadOutGeometry->Get_LY()       ;
  double vY_min = -1.5 * LY *1.E2 ;
  double vY_max =  1.5 * LY *1.E2 ;
  return GiveMe_pTH2D_rho_VS_YTrackYPad(aSample, ModuleNber ,TAG,120,vY_min,vY_max);
}
TH2D* GiveMe_pTH2D_rho_VS_YTrackYPad (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG ,
                                                    const int& NberOfBins_TH2D_rho_VS_YTrackYPad, 
                                                    const double& vY_min , 
                                                    const double& vY_max  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2D_rho_VS_YTrackYPad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #rho = A_{Max}/A^{Cluster} VS  Y_{T}-Y_{Pad} " ;
  
  TH2D* ToBeReturned = new TH2D (Name.c_str(), Title.c_str(),NberOfBins_TH2D_rho_VS_YTrackYPad, vY_min , vY_max, 102, 0. , 1.02) ;
//PRFTRUC-HISTO-2D
//TH2D* ToBeReturned = new TH2D (Name.c_str(), Title.c_str(),NberOfBins_TH2D_rho_VS_YTrackYPad, vY_min , vY_max, 150, 0. , 1.50) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack = pCluster->Get_YTrack()        ;

      double A_cluster  = pCluster->Get_Acluster()  ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double rho        = pPad->Get_AMax()/A_cluster ;
        double YTrackYPad = YTrack-pPad->Get_YPad()    ;
        
        ToBeReturned->Fill( YTrackYPad*1.E2 , rho ) ;
        
      }
    }
  }
  return ToBeReturned ;
}

TProfile* GiveMe_pTProfile_RhoLeading_VS_YTrackYPad (Sample& aSample, const int& ModuleNber ,
                                                 const std::string& TAG,
                                                 const double& NberOfBinsForProfile, 
                                                 const double& vY_min_DiffPRF , 
                                                 const double& vY_max_DiffPRF
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_RhoLeading_VS_YTrackYPad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #rho Leading VS #left[ Y^{T}-Y_{Pad} #right] (cm)" ;
  
  TProfile* ToBeReturned = new TProfile (Name.c_str(), Title.c_str(),NberOfBinsForProfile, vY_min_DiffPRF , vY_max_DiffPRF, 0. , 1.) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack = pCluster->Get_YTrack()        ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double rhoLeading  = pPad->Get_AMax()/pCluster->Get_AMaxLeading();
        double YTrackYPad = YTrack-pPad->Get_YPad() ;
        
        ToBeReturned->Fill( YTrackYPad*1.E2 , rhoLeading ) ;
        
      }
    }
  }
  return ToBeReturned ;
}

TProfile* GiveMe_pTProfile_DT_VS_YTrackYPad (Sample& aSample, const int& ModuleNber ,
                                                 const std::string& TAG,
                                                 const double& NberOfBinsForProfile, 
                                                 const double& vY_min_DiffPRF , 
                                                 const double& vY_max_DiffPRF
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_DT_VS_YTrackYPad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #Delta t (bin) VS #left[ Y_{T}-Y_{Pad} #right] (cm)" ;
    
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  std::vector < double > V_DT ;
  std::vector < double > V_DY ;
  double DTmax = 0. ; 
  double DTmin = 0. ; 
  int    FirstDT = 1 ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack       = pCluster->Get_YTrack()        ;
      double Tmax_Leading = pCluster->Get_TMaxLeading()  ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double DeltaT = pPad->Get_TMax()-Tmax_Leading;
        V_DT.push_back(DeltaT) ;
        
        double YTrackYPad = YTrack-pPad->Get_YPad() ;
        V_DY.push_back(YTrackYPad*1.E2) ;
        
        if (FirstDT==1){
          FirstDT = 0 ;
          DTmax = DeltaT ; 
          DTmin = DeltaT ; 
        }
        if (DTmax < DeltaT ) DTmax = DeltaT ;
        if (DTmin > DeltaT ) DTmin = DeltaT ;

      }
    }
  }
  
  double Diff = DTmax - DTmin ;
  DTmax = DTmax + 0.1 * Diff ;
  DTmin = DTmin - 0.1 * Diff ;
  TProfile* ToBeReturned = new TProfile (Name.c_str(), Title.c_str(),NberOfBinsForProfile, vY_min_DiffPRF , vY_max_DiffPRF, DTmin , DTmax) ;

  int iTem_Max = V_DT.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    double DeltaT = V_DT[iTem] ;
    double DeltaY = V_DY[iTem] ;
    ToBeReturned->Fill( DeltaY , DeltaT ) ;
  }

  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_YTrack(
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
){
  const Model_ReadOutGeometry* pModel_ReadOutGeometry = aSample.Get_Model_ReadOutGeometry();
  double Ycen = 1.E2* pModel_ReadOutGeometry->Get_YcPad(0,iYBeam,ModuleNber);
  double Ymin = Ycen-5. ;
  double Ymax = Ycen+5. ;
    
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_YTrack" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": Y_{T} (cm)" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100,  Ymin , Ymax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YTrack = pCluster->Get_YTrack() ;
      
      ToBeReturned->Fill( YTrack*1.E2 ) ;
    }
  }
  return ToBeReturned ;
}


TProfile* GiveMe_pTProfile_DeltaYTrack_VS_YTrack (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
){

  const Model_ReadOutGeometry* pModel_ReadOutGeometry = aSample.Get_Model_ReadOutGeometry();
  double Ycen = 1.E2* pModel_ReadOutGeometry->Get_YcPad(0,iYBeam,ModuleNber);
  double Ymin = Ycen-0.8 ;
  double Ymax = Ycen+0.8 ;

  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_DeltaYTrack_VS_YTrack" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #DeltaY^{T} (mm) VS  Y_{T} (cm)" ;

  TProfile* ToBeReturned  = new TProfile (Name.c_str(), Title.c_str(),500, Ymin, Ymax , -17. , 17.) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
              
      double DeltaYTrack = pCluster->Get_YTrack() - pCluster->Get_YTrack_BeforeMinimisation() ;
      
      ToBeReturned->Fill(  pCluster->Get_YTrack() *1.E2 , DeltaYTrack*1.E3 ) ;
    
    }
  }

  return ToBeReturned ;
}

TProfile* GiveMe_pTProfile_ErrorOnYTrack_VS_YTrack  (
                                                    Sample& aSample, const int& ModuleNber ,
                                                    const std::string& TAG,
                                                    const int& iYBeam
){

  const Model_ReadOutGeometry* pModel_ReadOutGeometry = aSample.Get_Model_ReadOutGeometry();
  double Ycen = 1.E2* pModel_ReadOutGeometry->Get_YcPad(0,iYBeam,ModuleNber);
  double Ymin = Ycen-0.8 ;
  double Ymax = Ycen+0.8 ;

  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_ErrorOnYTrack_VS_YTrack" + TAG + NameMODULE2  ; 
  std::string Title = TAG + NameMODULE1 + ": Error on Y_{T} (mm) VS  Y_{T}  (cm)" ;

  TProfile* ToBeReturned  = new TProfile (Name.c_str(), Title.c_str(),500, Ymin, Ymax , -17. , 17.) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
              
      double eYTrack = pCluster->Get_eYTrack()   ;
      
      ToBeReturned->Fill(  pCluster->Get_YTrack()*1.E2 , eYTrack*1.E3 ) ;
    
    }
  }
  
  return ToBeReturned ;
}

//-----------------------------------------------------------------------//

TH1F* GiveMe_pTH1F_DT (Sample& aSample, const int& ModuleNber )
{
  return ( GiveMe_DT (aSample, ModuleNber ,"NoTag") ) ;
}

TH1F* GiveMe_pTH1F_Yw(
                               Sample& aSample, const int& ModuleNber ,
                               const std::string& TAG,
                               const int&    Nbin ,
                               const double& Ymin ,
                               const double& Ymax  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Yw" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": Y_{W} (cm)" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),Nbin,  Ymin , Ymax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber)==0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      ToBeReturned->Fill( pCluster->Get_YWeight() *1.E2 ) ;
    }
  }
  return ToBeReturned ;
}


TH1F* GiveMe_pTH1F_YwYpad(
                               Sample& aSample, const int& ModuleNber ,
                               const std::string& TAG,
                               const int&    Nbin ,
                               const double& Ymin ,
                               const double& Ymax  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_YwYpad" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": Y_{w}-Y_{Pad} (cm)" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),Nbin,  Ymin , Ymax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber)==0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw = pCluster->Get_YWeight() ;
      
      int NPads = pCluster->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        ToBeReturned->Fill( (Yw-pPad->Get_YPad() ) *1.E2 ) ;
      }
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_DT (Sample& aSample, const int& ModuleNber,const std::string& TAG) 
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_DT" + TAG + NameMODULE2 ;
  std::string Title = TAG + NameMODULE1 + ": #Delta T=T_{pad}-T_{Leading} (bin)" ;
  
  std::vector < double > V_DT ;
  double DTmax = 0. ; 
  double DTmin = 0. ; 
  int    FirstDT = 1 ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber)==0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      double Tmax_Leading  = pCluster->Get_TMaxLeading()  ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads-1; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double DeltaT  = pPad->Get_TMax()-Tmax_Leading;
        V_DT.push_back(DeltaT) ;
        if (FirstDT==1){
          FirstDT = 0 ;
          DTmax = DeltaT ; 
          DTmin = DeltaT ; 
        }
        if (DTmax < DeltaT ) DTmax = DeltaT ;
        if (DTmin > DeltaT ) DTmin = DeltaT ;
                  
      }
    }
  }
  
  double Diff = DTmax - DTmin ;
  DTmax = DTmax + 0.1 * Diff ;
  DTmin = DTmin - 0.1 * Diff ;
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str() ,100,  DTmin , DTmax );
  
  int iTem_Max = V_DT.size() ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    double DeltaT = V_DT[iTem] ;
    ToBeReturned->Fill( DeltaT ) ;
  }

  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_Tmax(Sample& aSample, const int& ModuleNber , const std::string& TAG) 
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Tmax" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": T_{max}" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),510, -0.5 , 509.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
 
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        ToBeReturned->Fill( pPad->Get_TMax() ) ;
      }            
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_Amax_Leading(Sample& aSample, const int& ModuleNber , const std::string& TAG) 
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Amax_Leading" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": A_{max} Leading Pad" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),2001, -0.5 , 2000.5  );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPadLeading = pCluster->Get_LeadingPad() ;
      
      ToBeReturned->Fill( pPadLeading->Get_AMax() ) ;
    }
  }
  return ToBeReturned ;
}


TProfile* GiveMe_pTProfile_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string & TAG)
{
  return GiveMe_pTProfile_Rho_DeltaT (aSample, ModuleNber , TAG, 450,-150,300, -0.1, 1.1 ) ;
}
TProfile* GiveMe_pTProfile_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string & TAG, const int& DTbin, const int& DTmin, const int& DTmax, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_Rho_DeltaT" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": #rho VS #DeltaT_{max};#DeltaT_{max} (/40 ns);#rho" ;
  
  TProfile* ToBeReturned = new TProfile(Name.c_str(),Title.c_str(), DTbin, DTmin, DTmax, rhomin, rhomax) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double Tmax     = pPad->Get_TMax() ;
        double DeltaT   = Tmax - TLeading  ;
        double rho      = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        
        ToBeReturned->Fill(DeltaT,rho) ;
      }

    }

  }

  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_pTH2F_Rho_YwYpad(aSample, ModuleNber , TAG, 250, -2.5, 2.5, 120, -0.1, 1.1) ;
}
TH2F* GiveMe_pTH2F_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Rho_YwYpad" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": #rho VS Y_{W} - Y_{pad}; Y_{W} - Y_{pad} (cm);#rho" ;

  TH2F* ToBeReturned  = new TH2F(Name.c_str(),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw = pCluster->Get_YWeight()*100 ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;  
                
        double YwYpad = Yw - pPad->Get_YPad()*100 ;
        double rho    = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        
        ToBeReturned->Fill(YwYpad,rho) ;
      }  
    }
  }
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_DeltaT_YwYpad (
                                  Sample& aSample, const int& ModuleNber, 
                                  const std::string& TAG
){
  return GiveMe_pTH2F_DeltaT_YwYpad (aSample, ModuleNber ,TAG,-1.8,1.8) ;
}
TH2F* GiveMe_pTH2F_DeltaT_YwYpad (
                                  Sample& aSample, const int& ModuleNber, 
                                  const std::string& TAG,
                                  const double& ywypad_min ,
                                  const double& ywypad_max
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_DeltaT_YwYpad" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": #DeltaT_{max}(Neighbour-Lead) VS Y_{W} - Y_{pad};Y_{W} - Y_{pad} (cm);#DeltaT_{max}" ;

  TH2F* ToBeReturned  = new TH2F(Name.c_str(),Title.c_str(), 100, ywypad_min, ywypad_max, 100, -510, 510) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw       = pCluster->Get_YWeight()*100 ;
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad = pCluster->Get_Pad(iP) ;
        
        double YwYpad = Yw - pPad->Get_YPad()*100 ;
        double Tneigh = pPad->Get_TMax() ;        
        double DeltaT = Tneigh - TLeading ;
        
        ToBeReturned->Fill(YwYpad, DeltaT) ;
      }  
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_YwYpad (
                          Sample& aSample, const int& ModuleNber, 
                          const std::string& TAG
)
{
  return GiveMe_pTH1F_YwYpad(aSample, ModuleNber ,TAG,-1.8,1.8) ;
}
TH1F* GiveMe_pTH1F_YwYpad (
                          Sample& aSample, const int& ModuleNber, 
                          const std::string& TAG ,
                          const double ywpad_min ,
                          const double ywpad_max  
                          )
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_YwYpad" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": Y_{W} - Y_{pad}  ; Y_{W} - Y_{pad} (cm) ; " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), 100, ywpad_min ,ywpad_max  ) ;               

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw = pCluster->Get_YWeight()*100 ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad       = pCluster->Get_Pad(iP) ;  
                
        double YwYpad   = Yw - pPad->Get_YPad()*100 ;
        
        ToBeReturned->Fill(YwYpad) ;
      }  
    }
  }
  return ToBeReturned ;
}

TProfile* GiveMe_pTProfile_DeltaT_YwYpad (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG
)
{
  return GiveMe_pTProfile_DeltaT_YwYpad(aSample, ModuleNber ,TAG,-1.8,1.8);
}
TProfile* GiveMe_pTProfile_DeltaT_YwYpad (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG,
                                          const double& ywypad_min ,
                                          const double& ywypad_max
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_DeltaT_YwYpad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": #DeltaT_{max} VS Y_{W} - Y_{pad}; Y_{W} - Y_{pad} (cm);#DeltaT_{max}" ;

  TProfile* ToBeReturned = new TProfile(Name.c_str(),Title.c_str(), 100, ywypad_min, ywypad_max, -510, 510) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw       = pCluster->Get_YWeight()*100 ;
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad       = pCluster->Get_Pad(iP) ;
        
        double YwYpad = Yw - pPad->Get_YPad()*100 ;
        double Tneigh      = pPad->Get_TMax() ;        
        double DeltaT      = Tneigh - TLeading ;
        
        ToBeReturned->Fill(YwYpad, DeltaT) ;
      }  
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_Rho (Sample& aSample, const int& ModuleNber, const std::string& TAG){
  return GiveMe_pTH1F_Rho(aSample, ModuleNber , TAG, -1000, 1000) ;
}
TH1F* GiveMe_pTH1F_Rho (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Npadmin, const int& Npadmax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Rho" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": Weight #rho of leading pads ;#rho ;Normalized occurences" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), 120, -0.1 , 1.1 ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pLeadingPad  = pCluster->Get_LeadingPad() ;
      int NPads = pCluster->Get_NberOfPads() ;
      
      if(NPads <= Npadmax and NPads >= Npadmin)
      ToBeReturned->Fill(pLeadingPad->Get_AMax() / pCluster->Get_Acluster()) ;
    }
  }
  
  int NEntries = ToBeReturned->GetEntries();
  if(NEntries == 0){
    delete ToBeReturned ;
    ToBeReturned = 0 ;
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_Yw (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  TH1I* pTHI    = GiveMe_pTH1I_LeadingiY(aSample, ModuleNber , "Tempo") ;                            
  int iYmean = pTHI->GetMaximumBin() ;
  delete pTHI ;

  Model_ReadOutGeometry* pROG = aSample.Get_Model_ReadOutGeometry() ;
  double Ymean = pROG->Get_YcPad(0,iYmean-1,ModuleNber) ;
  double Ymin  = Ymean - 2.5*pROG->Get_LY() ;
  double Ymax  = Ymean + 2.5*pROG->Get_LY() ;                                

  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name = "GiveMe_pTH1F_Yw" + TAG + NameMODULE2;
  std::string Title = TAG + NameMODULE1 + ": Weighted position Y_{W} of cluster ; Y_{W} (cm);Occurences" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), 100, Ymin*100 , Ymax*100) ;         

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      ToBeReturned->Fill(pCluster->Get_YWeight()*100) ;
    }
  }
  return ToBeReturned ;
}

TH1I* GiveMe_pTH1I_LeadingiY (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1I_LeadingiY" + TAG + NameMODULE2 ;
  std::string Title = TAG + NameMODULE1 + ": Row iY of the leading pad;Row;Occurences" ;

  TH1I* ToBeReturned = new TH1I(Name.c_str(),Title.c_str(),32, -0.5 , 31.5 ) ; 

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){          
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;            
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY() ;
      
      ToBeReturned->Fill( iY_Leading ) ;
    }
  }
  return ToBeReturned ;
}



TProfile* GiveMe_pTProfile_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_pTProfile_Rho_YwYpad(aSample, ModuleNber , TAG, 250, -2.5, 2.5, -0.1, 1.1) ;
}
TProfile* GiveMe_pTProfile_Rho_YwYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_Rho_YwYpad" + TAG + NameMODULE2 ;
  std::string Title = TAG + NameMODULE1 + ": #rho VS Y_{W} - Y_{Pad}; Y_{W} - Y_{Pad} (cm);#rho" ;
  
  TProfile* ToBeReturned  = new TProfile(Name.c_str(),Title.c_str(), NYbins, Ymin, Ymax, rhomin, rhomax) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Yw = pCluster->Get_YWeight()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;  
                
        double YwYpad = Yw - pPad->Get_YPad()*100 ;
        double rho    = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        
        ToBeReturned->Fill(YwYpad,rho) ;
      }  
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_PullFitRC_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitRC_PV2 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitRC_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitRC_PV2" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit RC" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int iPad_Max = aSample.FitRes_PV2_Get_NberOfTermsInChi2();
  for (int iPad = 0 ; iPad < iPad_Max ; iPad++){
    ToBeReturned->Fill( aSample.FitRes_PV2_Get_Pull(iPad) ) ;
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitRC_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitRC_PV3 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitRC_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitRC_PV3" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit RC" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int iPad_Max = aSample.FitRes_PV3_Get_NberOfTermsInChi2();
  for (int iPad = 0 ; iPad < iPad_Max ; iPad++){
    ToBeReturned->Fill( aSample.FitRes_PV3_Get_Pull(iPad) ) ;
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitRC_PV4 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitRC_PV4 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitRC_PV4 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitRC_PV4" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit RC" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int iPad_Max = aSample.FitRes_PV4_Get_NberOfTermsInChi2();
  for (int iPad = 0 ; iPad < iPad_Max ; iPad++){
    ToBeReturned->Fill( aSample.FitRes_PV4_Get_Pull(iPad) ) ;
  }

  return ToBeReturned ;

}


TH1F* GiveMe_pTH1F_PullFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitCluster_PV0_PV1 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitCluster_PV0_PV1" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit Cluster 1D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int iPad_Max = pCluster->FitRes_PV0_PV1_Get_NberOfTermsInChi2() ;
      for (int iPad = 0 ; iPad< iPad_Max; iPad++){
        ToBeReturned->Fill( pCluster->FitRes_PV0_PV1_Get_Pull(iPad) ) ;
      }
      
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitCluster_PV2 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitCluster_PV2" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int iPad_Max = pCluster->FitRes_PV2_Get_NberOfTermsInChi2() ;
      for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      
        ToBeReturned->Fill( pCluster->FitRes_PV2_Get_Pull(iPad) ) ;
      }
      
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitCluster_PV3 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitCluster_PV3" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int iPad_Max = pCluster->FitRes_PV3_Get_NberOfTermsInChi2() ;
      for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      
        ToBeReturned->Fill( pCluster->FitRes_PV3_Get_Pull(iPad) ) ;
      }
      
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinFitCluster_PV2 (aSample, ModuleNber ,TAG,100 , 0., 50.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinMin, const double& Ch2MinMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinFitCluster_PV2" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2Min fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinMin , Ch2MinMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min() ) ;
              
    }
    
  }

  return ToBeReturned ;

}


TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinFitCluster_PV3 (aSample, ModuleNber ,TAG,100 , 0., 50.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinMin, const double& Ch2MinMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinFitCluster_PV3" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2Min fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinMin , Ch2MinMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min() ) ;
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2 (aSample, ModuleNber ,TAG,100 , 0., 5.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinPerDOFMin, const double& Ch2MinPerDOFMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV2" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2MinPerDOF fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinPerDOFMin , Ch2MinPerDOFMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min()/double( pCluster->FitRes_PV2_Get_NberOfTermsInChi2() - 2 ) )   ;
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3 (aSample, ModuleNber ,TAG,100 , 0., 5.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinPerDOFMin, const double& Ch2MinPerDOFMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV3" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2MinPerDOF fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinPerDOFMin , Ch2MinPerDOFMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min()/double( pCluster->FitRes_PV3_Get_NberOfTermsInChi2() - 2 ) )   ;
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitPRF (TGraphErrors* pTGraphErrors, TF1* pTF1, double& Chi2Min_PRFfit, int& NODF_PRFfit , const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitPRF (pTGraphErrors,pTF1,Chi2Min_PRFfit,NODF_PRFfit,TAG,50 , -3., 3.) ;
}
TH1F* GiveMe_pTH1F_PullFitPRF (TGraphErrors* pTGraphErrors, TF1* pTF1, double& Chi2Min_PRFfit, int& NODF_PRFfit , const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax) 
{
  std::string Name  = "GiveMe_pTH1F_PullFitPRF" + TAG  ;
  
  std::string Title = TAG + ": Pull fit PRF" ;
//std::cout << " Title " << Name << std::endl;
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int NberOfParameters_pTF1 = pTF1->GetNpar ();
 
  int iTem_Max = pTGraphErrors->GetN();
  double* TheX_A  = pTGraphErrors->GetX ()  ;                                          
  double* TheY_A  = pTGraphErrors->GetY ()  ;                                          
  double* TheEY_A = pTGraphErrors->GetEY()  ;   
  NODF_PRFfit    = pTGraphErrors->GetN()- NberOfParameters_pTF1 ;
  Chi2Min_PRFfit = 0. ;                                        
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    double TheX    = TheX_A [iTem] ;
    double TheY    = TheY_A [iTem] ;
    double TheEY   = TheEY_A[iTem] ;
    double TheY_F  = pTF1->Eval( TheX ) ;
    double ThePull = ( TheY - TheY_F )/TheEY ;
    ToBeReturned->Fill( ThePull ) ;
    Chi2Min_PRFfit += ThePull*ThePull ;
//     std::cout 
//       << " iTem "     << std::setw(8) << std::setprecision(3) << iTem
//       << " TheX "     << std::setw(8) << std::setprecision(3) << TheX
//       << " TheY "     << std::setw(8) << std::setprecision(3) << TheY
//       << " TheEY "    << std::setw(8) << std::setprecision(3) << TheEY
//       << " TheY_F "   << std::setw(8) << std::setprecision(3) << TheY_F
//       << " ThePull "  << std::setw(8) << std::setprecision(3) << ThePull
//       << std::endl ;
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1 (aSample, ModuleNber ,TAG,100 , 0., 50.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinMin, const double& Ch2MinMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinFitCluster_PV0_PV1" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2Min fit Cluster 1D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinMin , Ch2MinMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min() ) ;
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1 (aSample, ModuleNber ,TAG,100 , 0., 5.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinPerDOFMin, const double& Ch2MinPerDOFMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV0_PV1" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2MinPerDOF fit Cluster 1D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinPerDOFMin , Ch2MinPerDOFMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min()/double( pCluster->FitRes_PV0_PV1_Get_NberOfTermsInChi2() - 1 ) )   ;
              
    }
    
  }

  return ToBeReturned ;

}

TGraph* GiveMe_pTGraph_Alpha2_VS_Alpha3 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  TGraph* ToBeReturned = new TGraph ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
        double A3 = std::min(AU, AD) ;
        ToBeReturned->SetPoint( ToBeReturned->GetN(), A3 , A2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TGraph* GiveMe_pTGraph_Alpha2_VS_Alpha3onAlpha2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  TGraph* ToBeReturned = new TGraph ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
        double A3 = std::min(AU, AD) ;
        
        ToBeReturned->SetPoint( ToBeReturned->GetN(), A3/A2 , A2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TGraph* GiveMe_pTGraph_Tau2_VS_Tau3 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  TGraph* ToBeReturned = new TGraph ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        
        ToBeReturned->SetPoint( ToBeReturned->GetN(), T3 , T2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}


TGraph* GiveMe_pTGraph_Tau2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  TGraph* ToBeReturned = new TGraph ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        
        ToBeReturned->SetPoint( ToBeReturned->GetN(), T3-T2 , T2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}


TH2F* GiveMe_pTH2F_Alpha2_VS_Alpha3 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_Alpha2_VS_Alpha3( aSample, ModuleNber , SubTAG, 100,0.,1., 100,0.,1. ) ;
}
TH2F* GiveMe_pTH2F_Alpha2_VS_Alpha3 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Alpha2_VS_Alpha3" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":#alpha_{2}=A2/A1 VS #alpha_{3}=A3/A1" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
        double A3 = std::min(AU, AD) ;
        
        ToBeReturned->Fill( A3 , A2 ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH2F* GimeMe_pTH2F_Tau2_VS_Tau3 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GimeMe_pTH2F_Tau2_VS_Tau3( aSample, ModuleNber , SubTAG,  171,-20.5,150.5, 91,-20.5,70.5) ;
}
TH2F* GimeMe_pTH2F_Tau2_VS_Tau3 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GimeMe_pTH2F_Tau2_VS_Tau3" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":#tau_{2}=T2-T1 VS #tau_{3}=T3-T1" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        
        ToBeReturned->Fill( T3 , T2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_LeadTmax (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_LeadTmax(aSample, ModuleNber ,SubTAG,511, -0.5 , 510.5 );
}

TH1F* GiveMe_pTH1F_LeadTmax (Sample& aSample, const int& ModuleNber, const std::string& TAG,const int& NbinX,const double& Xmin, const double& Xmax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_LeadTmax" + TAG + NameMODULE2; 
  std::string Title = NameMODULE1 + ":T_{max}^{Leading}" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),NbinX,Xmin,Xmax );


  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double TLeading = pCluster->Get_TMaxLeading() ;
      
      if (TLeading>10.) ToBeReturned->Fill(TLeading) ;
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_DTLeading (Sample& aSample, const int& ModuleNber,const std::string& TAG) 
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_DTLeading(aSample, ModuleNber ,SubTAG,100,  -150. , 200. );
}
TH1F* GiveMe_pTH1F_DTLeading (Sample& aSample, const int& ModuleNber,const std::string& TAG,const int& NbinX,const double& Xmin, const double& Xmax) 
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_DTLeading" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":T_{max}^{Leading}-T_{max}^{Leading, Mean over Evt} " ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str() ,NbinX,Xmin,Xmax );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber)==0) continue ;

    double Tmean         = 0. ;
    int    Tmean_Kounter = 0 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      Tmean += pCluster->Get_TMaxLeading() ;
      Tmean_Kounter += 1 ;
      
    }
    Tmean = Tmean/double(NClusters); 

    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      double DeltaT  = pCluster->Get_TMaxLeading() - Tmean ;
      
      ToBeReturned->Fill( DeltaT ) ;
    }
  }
  
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_TauNotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_TauNotLeading(aSample, ModuleNber ,SubTAG,451,-150.5,300.5);
}
TH1F* GiveMe_pTH1F_TauNotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Nbin,const double& DTmin, const double& DTmax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_TauNotLeading" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":#tau^{NotLeading} = T_{max}^{NotLeading} - T_{max}^{Leading}  " ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), Nbin, DTmin, DTmax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading = pCluster->Get_LeadingPad();
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        if (pPad==pPad_Leading) continue ;
        double Tau = pPad->Get_TMax()- TLeading  ;
        
        ToBeReturned->Fill( Tau ) ;
      }

    }

  }
  
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_Alpha_VS_Tau_NotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH2F_Alpha_VS_Tau_NotLeading(aSample, ModuleNber , SubTAG, 451,-150.5,300.5, 120, -0.1, 1.1) ;  
}
TH2F* GiveMe_pTH2F_Alpha_VS_Tau_NotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, int DTbins, int DTmin, int DTmax, int rhobin, float rhomin, float rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name = "GiveMe_pTH2F_Alpha_VS_Tau_NotLeading" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":#alpha^{NotLeading} = A_{max}^{NotLeading}/A_{max}^{Leading} VS #tau^{NotLeading} = T_{max}^{NotLeading} - T_{max}^{Leading} " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str(), DTbins, DTmin, DTmax, rhobin, rhomin, rhomax) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad();
      double TLeading = pCluster->Get_TMaxLeading() ;
      double ALeading = pPad_Leading->Get_AMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        if (pPad!=pPad_Leading){
          double Tmax     = pPad->Get_TMax() ;
          double DeltaT   = Tmax - TLeading  ;
          double rho      = pPad->Get_AMax() / ALeading ;
          
          ToBeReturned->Fill(DeltaT,rho) ;
        }
      }

    }

  }

  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading(aSample, ModuleNber , SubTAG, 511, -0.5, 510.5, 451,-150.5,300.5) ;  
}
TH2F* GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name = "GiveMe_pTH2F_TauNotLeading_VS_TmaxLeading" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":#tau^{NotLeading} = T_{max}^{NotLeading} - T_{max}^{Leading} VS T_{max}^{Leading}" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str(), NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading = pCluster->Get_LeadingPad();
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad = pCluster->Get_Pad(iP) ;          
        
        double DeltaT        = pPad->Get_TMax() - TLeading ;
        if (pPad!=pPad_Leading){
        
          ToBeReturned->Fill(TLeading, DeltaT) ;
        }
      }  
    }
  }
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_Amax(Sample& aSample, const int& ModuleNber , const std::string& TAG) 
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH1F_Amax(aSample, ModuleNber ,SubTAG,4097, -0.5 , 4096.5 );
}
TH1F* GiveMe_pTH1F_Amax(
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Amax" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":A_{max}" ;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),NbinX, Xmin, Xmax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
 
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        ToBeReturned->Fill( pPad->Get_AMax() ) ;
      }            
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_Tau2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_Tau2_VS_Tau3mTau2( aSample, ModuleNber , SubTAG,  171,-20.5,150.5, 91,-20.5,70.5) ;
}
TH2F* GiveMe_pTH2F_Tau2_VS_Tau3mTau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Tau2_VS_Tau3mTau2" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        
        ToBeReturned->Fill( T3-T2 , T2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_AMaxmAMax_FIT ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_AMaxmAMax_FIT" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + "AMax - AMax_FIT" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),41, -20.5 , 20.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double AMax     = pPad->Get_AMax()     ;
        double AMax_FIT = pPad->Get_AMax_FIT() ;
        double DIFF  = AMax - AMax_FIT ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax( aSample, ModuleNber , SubTAG, 4097, -0.5 , 4096.5, 81, -40.5 , 40.5) ;
}
TH2F* GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMaxmAMax_FIT_VS_AMax" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":AMax - AMax_FIT VS AMax" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax     = pPad->Get_AMax()     ;
        double AMax_FIT = pPad->Get_AMax_FIT() ;
        double DIFF  = AMax - AMax_FIT ;
        
        ToBeReturned->Fill( AMax , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_TMaxmTMax_FIT ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_TMaxmTMax_FIT" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + "TMax - TMax_FIT" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),41, -20.5 , 20.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double TMax     = pPad->Get_TMax()     ;
        double TMax_FIT = pPad->Get_TMax_FIT() ;
        double DIFF  = TMax - TMax_FIT ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax( aSample, ModuleNber , SubTAG, 4097, -0.5 , 4096.5, 81, -40.5 , 40.5) ;
}
TH2F* GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMaxmTMax_FIT_VS_AMax" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":TMax - TMax_FIT VS AMax" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax     = pPad->Get_AMax()     ;
        double TMax     = pPad->Get_TMax()     ;
        double TMax_FIT = pPad->Get_TMax_FIT() ;
        double DIFF  = TMax - TMax_FIT ;
        
        ToBeReturned->Fill( AMax , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH2F* GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2( aSample, ModuleNber , SubTAG, 100,0.,1., 100,0.,1. ) ;
}
TH2F* GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":#alpha_{2} = A2/A1 VS #alpha_{3}/#alpha_{2} =A3/A2" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
        double A3 = std::min(AU, AD) ;
        
        ToBeReturned->Fill( A3/A2 , A2 ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_AMax_WFmAMax_FIT ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_AMax_WFmAMax_FIT" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + ":AMax_WF - AMax_FIT" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),41, -20.5 , 20.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double AMax_WF  = pPad->Get_AMax_WF_01 () ;
        double AMax_FIT = pPad->Get_AMax_FIT() ;
        double DIFF  = AMax_WF - AMax_FIT ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF( aSample, ModuleNber , SubTAG, 4097, -0.5 , 4096.5, 81, -40.5 , 40.5) ;
}
TH2F* GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMax_WFmAMax_FIT_VS_AMax_WF" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":AMax_WF - AMax_FIT VS AMax_WF" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_WF  = pPad->Get_AMax_WF_01 () ;
        double AMax_FIT = pPad->Get_AMax_FIT() ;
        double DIFF  = AMax_WF - AMax_FIT ;
        
        ToBeReturned->Fill( AMax_WF , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_TMax_WFmTMax_FIT ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_TMax_WFmTMax_FIT" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + ":TMax_WF - TMax_FIT" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),41, -20.5 , 20.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double TMax_WF  = pPad->Get_TMax_WF_01 () ;
        double TMax_FIT = pPad->Get_TMax_FIT() ;
        double DIFF  = TMax_WF - TMax_FIT ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF( aSample, ModuleNber , SubTAG, 4097, -0.5 , 4096.5, 81, -40.5 , 40.5) ;
}
TH2F* GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMax_WFmTMax_FIT_VS_AMax_WF" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":TMax_WF - TMax_FIT VS TMax_WF" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_WF  = pPad->Get_AMax_WF_01 () ;
        double TMax_WF  = pPad->Get_TMax_WF_01 () ;
        double TMax_FIT = pPad->Get_TMax_FIT() ;
        double DIFF  = TMax_WF - TMax_FIT ;
        
        ToBeReturned->Fill( AMax_WF , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_AlphaNotLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH1F_AlphaNotLeading( aSample, ModuleNber , SubTAG, 100,  0. , 1.) ;
}
TH1F* GiveMe_pTH1F_AlphaNotLeading(
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_AlphaNotLeading" + TAG + NameMODULE2; 
  std::string Title = NameMODULE1 + ":#alpha^{NotLeading}=A_{Max}^{NotLeading}/A_{max}^{Leading}" ;


  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax);
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading = pCluster->Get_LeadingPad();
      double ALeading = pCluster->Get_AMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        if (pPad==pPad_Leading) continue ;
        double rhoLeading  = pPad->Get_AMax()/ALeading;
        
        ToBeReturned->Fill( rhoLeading ) ;
      }

    }

  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_Alpha_VS_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH2F_Alpha_VS_YTYpad(aSample, ModuleNber , SubTAG, 100, -1.5*1.019, 1.5*1.019, 120, -0.1, 1.1) ;
}
TH2F* GiveMe_pTH2F_Alpha_VS_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Alpha_VS_YTYpad" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": #alpha = A_{Max}/A_{Max}^{Leading} VS Y_{T} - Y_{Pad}" ;

  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
   
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT       = pCluster->Get_YTrack()*100. ;
      double ALeading = pCluster->Get_AMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;   
               
        double YTYpad = YT - pPad->Get_YPad()*100. ;
        double rho    = pPad->Get_AMax() / ALeading ;
        
        ToBeReturned->Fill(YTYpad,rho) ;
      }  
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_Tau_VS_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return GiveMe_pTH2F_Tau_VS_YTYpad(aSample, ModuleNber , SubTAG, 100, -1.5*1.019, 1.5*1.019, 156, -5.5, 150.5) ;
}
TH2F* GiveMe_pTH2F_Tau_VS_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Tau_VS_YTYpad" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE2  + ": #tau = T_{Max}-T_{Max}^{Leading} VS Y_{T} - Y_{Pad}" ;

  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
   
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT       = pCluster->Get_YTrack()*100. ;
      double TLeading = pCluster->Get_TMaxLeading() ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;   
               
        double YTYpad = YT - pPad->Get_YPad()*100. ;
        double Tau    = pPad->Get_TMax()- TLeading  ;
        
        ToBeReturned->Fill(YTYpad,Tau) ;
      }  
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_Alpha2_VS_Tau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_Alpha2_VS_Tau2( aSample, ModuleNber , SubTAG, 34,-3.5,30.5, 100,0.,1.) ;
}
TH2F* GiveMe_pTH2F_Alpha2_VS_Tau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Alpha2_VS_Tau2" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":#alpha_{2}=A2/A1 VS #tau_{2}=T2-T1 " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
//      double A3 = std::min(AU, AD) ;

        double T2 = TU ;
//      double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
//        T3 = TU ;
        }
        
        ToBeReturned->Fill( T2 , A2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH2F* GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2 (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2( aSample, ModuleNber , SubTAG, 104, -3.5,100.5, 100,0.,1.) ;
}
TH2F* GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2 (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ": #alpha_{3}/#alpha_{2}=A3/A2 VS #tau_{3}-#tau_{2}=T3-T2 " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      if (NPads<3) continue ;
      
      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      double TU = 0. ;  
      double TD = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
        
      }
      
      if (AU_Found==1 && AD_Found==1 ){
        double A2 = std::max(AU, AD) ;
        double A3 = std::min(AU, AD) ;

        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        
        ToBeReturned->Fill( T3-T2 , A3/A2  ) ;
      }
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PadMultiplicityPerEvent (
                                            Sample& aSample, const int& ModuleNber, 
                                            const std::string& TAG, 
                                            const int& NbinX, const double& Xmin, const double& Xmax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PadMultiplicityPerEvent" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ": Pad Multiplicity per event" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    double NPadTot = 0. ;
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPad = pCluster->Get_NberOfPads() ;
      NPadTot += double(NPad) ;
    }
    ToBeReturned->Fill( NPadTot ) ;
  }
  return ToBeReturned ;
}

//--------------------------------------------//
TH1F*     GiveMe_pTH1F_AMax_FITmAMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_AMax_FITmAMax_MC" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + ":Amax_FIT - AMax_True" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),201, -100.5, 100.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double Amax_FIT  = pPad->Get_AMax_FIT ()   ;
        double AMax_True = pPad->Get_AMax_True()   ;
        double DIFF = Amax_FIT - AMax_True ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC( aSample, ModuleNber , SubTAG, 1501, -0.5 , 1500.5, 201, -100.5, 100.5) ;
}
TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMax_FITmAMax_MC_VS_AMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":Amax_FIT - AMax_True VS AMax_True ; AMax_True ; Amax_FIT - AMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double Amax_FIT  = pPad->Get_AMax_FIT () ;
        double AMax_True = pPad->Get_AMax_True() ;
        double DIFF = Amax_FIT - AMax_True ;
        
        ToBeReturned->Fill( AMax_True , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_TMax_FITmTMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_TMax_FITmTMax_MC" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + ":TMax_FIT - TMax_True" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),61, -30.5, 30.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double TMax_FIT  = pPad->Get_TMax_FIT () ;
        double TMax_True = pPad->Get_TMax_True()   ;
        double DIFF = TMax_FIT - TMax_True ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC( aSample, ModuleNber , SubTAG, 1501, -0.5 , 1500.5, 61, -30.5, 30.5) ;
}
TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMax_FITmTMax_MC_VS_AMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":TMax_FIT - TMax_True VS AMax_True ; AMax_True ; TMax_FIT - TMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_True = pPad->Get_AMax_True ()  ;
        double TMax_FIT  = pPad->Get_TMax_FIT ()   ;
        double TMax_True = pPad->Get_TMax_True()   ;
        double DIFF = TMax_FIT - TMax_True ;
        
        ToBeReturned->Fill( AMax_True , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_TMax_FromSetmTMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_TMax_FromSetmTMax_MC" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + "TMax_FromSet - TMax_True" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),61, -30.5, 30.5);

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double TMax_FromSet = pPad->Get_TMax_FromSet() ;
        double TMax_True    = pPad->Get_TMax_True()    ;
        double DIFF = TMax_FromSet - TMax_True ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC( aSample, ModuleNber , SubTAG, 1501, -0.5 , 1500.5 , 61, -30.5, 30.5) ;
}
TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_AMax_MC" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":TMax_FromSet - TMax_True VS AMax_True ; AMax_True ; TMax_FromSet - TMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_True    = pPad->Get_AMax_True()    ;
        double TMax_FromSet = pPad->Get_TMax_FromSet() ;
        double TMax_True    = pPad->Get_TMax_True()    ;
        double DIFF = TMax_FromSet - TMax_True ;
        
        ToBeReturned->Fill( AMax_True , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH1F*     GiveMe_pTH1F_AMax_FromSetmAMax_MC ( 
                                Sample& aSample, const int& ModuleNber, 
                                const std::string& TAG  
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_AMax_FromSetmAMax_MC" + TAG + NameMODULE2 ; 
  std::string Title = NameMODULE1 + "AMax_FromSet - AMax_True" ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),201, -100.5, 100.5 );

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        double AMax_FromSet = pPad->Get_AMax_FromSet() ;
        double AMax_True    = pPad->Get_AMax_True()    ;
        double DIFF = AMax_FromSet - AMax_True ;
        
        ToBeReturned->Fill( DIFF ) ;
                  
      }
    }
  }
  return ToBeReturned ;
}


TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC( aSample, ModuleNber , SubTAG, 1501, -0.5 , 1500.5 , 201, -100.5, 100.5) ;
}
TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_AMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":AMax_FromSet - AMax_True VS AMax_True ; AMax_True ; AMax_FromSet - AMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_FromSet = pPad->Get_AMax_FromSet() ;
        double AMax_True    = pPad->Get_AMax_True()    ;
        double DIFF = AMax_FromSet - AMax_True ;
        
        ToBeReturned->Fill( AMax_True , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

//--------------------------------------------//
TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC( aSample, ModuleNber , SubTAG,  61, -0.5 , 60.5, 201, -100.5, 100.5) ;
}
TH2F* GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMax_FITmAMax_MC_VS_TMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":Amax_FIT - AMax_True VS TMax_True ; TMax_True ; Amax_FIT - AMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      double TMax_True_Min = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
        double TMax_True = pPad->Get_TMax_True()   ;
        if (iPad==0) {
          TMax_True_Min = TMax_True ;
        }
        if (TMax_True_Min>TMax_True) TMax_True_Min = TMax_True ;
      }              
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double Amax_FIT  = pPad->Get_AMax_FIT () ;
        double AMax_True = pPad->Get_AMax_True() ;
        double TMax_True = pPad->Get_TMax_True()   ;
        double DIFF = Amax_FIT - AMax_True ;
        
        ToBeReturned->Fill( (TMax_True - TMax_True_Min) , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC( aSample, ModuleNber , SubTAG,  61, -0.5 , 60.5, 61, -30.5, 30.5) ;
}
TH2F* GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMax_FITmTMax_MC_VS_TMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":TMax_FIT - TMax_True VS TMax_True ; TMax_True ; TMax_FIT - TMax_True" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      double TMax_True_Min = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
        double TMax_True = pPad->Get_TMax_True()   ;
        if (iPad==0) {
          TMax_True_Min = TMax_True ;
        }
        if (TMax_True_Min>TMax_True) TMax_True_Min = TMax_True ;
      }              
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double TMax_FIT  = pPad->Get_TMax_FIT ()   ;
        double TMax_True = pPad->Get_TMax_True()   ;
        double DIFF = TMax_FIT - TMax_True ;
        
        ToBeReturned->Fill( (TMax_True - TMax_True_Min) , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC( aSample, ModuleNber , SubTAG,  61, -0.5 , 60.5 , 61, -30.5, 30.5) ;
}
TH2F* GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_TMax_FromSetmTMax_MC_VS_TMax_MC" + TAG + NameMODULE2;
  std::string Title = NameMODULE1 + ":TMax_FromSet - TMax_True VS TMax_True ; TMax_True ; TMax_FromSet - TMax_True" ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      double TMax_True_Min = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
        double TMax_True = pPad->Get_TMax_True()   ;
        if (iPad==0) {
          TMax_True_Min = TMax_True ;
        }
        if (TMax_True_Min>TMax_True) TMax_True_Min = TMax_True ;
      }              
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double TMax_FromSet = pPad->Get_TMax_FromSet() ;
        double TMax_True    = pPad->Get_TMax_True()    ;
        double DIFF = TMax_FromSet - TMax_True ;
        
        ToBeReturned->Fill( (TMax_True - TMax_True_Min) , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  std::string SubTAG = TAG + "Indirect" ;
  return  GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC( aSample, ModuleNber , SubTAG,  61, -0.5 , 60.5 , 201, -100.5, 100.5) ;
}
TH2F* GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC (
                              Sample& aSample, const int& ModuleNber, const std::string& TAG,
                              const int& NbinX, const double& Xmin, const double& Xmax,
                              const int& NbinY, const double& Ymin, const double& Ymax
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_AMax_FromSetmAMax_MC_VS_TMax_MC" + TAG + NameMODULE2 ;
  std::string Title = NameMODULE1 + ":AMax_FromSet - AMax_True VS TMax_True ; TMax_True ; AMax_FromSet - AMax_True " ;
  
  TH2F* ToBeReturned = new TH2F(Name.c_str(),Title.c_str() ,NbinX, Xmin, Xmax, NbinY, Ymin , Ymax);

  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      double TMax_True_Min = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
        double TMax_True = pPad->Get_TMax_True()   ;
        if (iPad==0) {
          TMax_True_Min = TMax_True ;
        }
        if (TMax_True_Min>TMax_True) TMax_True_Min = TMax_True ;
      }              
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double AMax_FromSet = pPad->Get_AMax_FromSet() ;
        double AMax_True    = pPad->Get_AMax_True()    ;
        double TMax_True    = pPad->Get_TMax_True()    ;
        double DIFF = AMax_FromSet - AMax_True ;
        
        ToBeReturned->Fill( (TMax_True - TMax_True_Min) , DIFF  ) ;
        
      }              
    }
  }

  return ToBeReturned ;

}

TProfile* GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG
)
{
  return GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC(aSample, ModuleNber ,TAG,61, -0.5 , 60.5);
}
TProfile* GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC (
                                          Sample& aSample, const int& ModuleNber, 
                                          const std::string& TAG,
                                          const int&    NbinX ,
                                          const double& X_min ,
                                          const double& X_max
){
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTProfile_TMax_FITmTMax_MC_VS_TMax_MC" + TAG + NameMODULE2 ; 
  std::string Title = TAG + NameMODULE1 + ":TMax_FIT - TMax_True VS TMax_True ; TMax_True ; TMax_FIT - TMax_True" ;

  TProfile* ToBeReturned = new TProfile(Name.c_str(),Title.c_str(), NbinX, X_min, X_max, -50., 50.,"s") ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      double TMax_True_Min = 0. ; 
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
        double TMax_True = pPad->Get_TMax_True()   ;
        if (iPad==0) {
          TMax_True_Min = TMax_True ;
        }
        if (TMax_True_Min>TMax_True) TMax_True_Min = TMax_True ;
      }              
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        double TMax_FIT  = pPad->Get_TMax_FIT ()   ;
        double TMax_True = pPad->Get_TMax_True()  ;
        double DIFF = TMax_FIT - TMax_True ;
        
        ToBeReturned->Fill( (TMax_True - TMax_True_Min) , DIFF  ) ;
        
      }              
    }
  }
  return ToBeReturned ;
}


TH1F* GiveMe_pTH1F_PullFitRC_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitRC_PV31 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitRC_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitRC_PV31" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit RC" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int iPad_Max = aSample.FitRes_PV31_Get_NberOfTermsInChi2();
  for (int iPad = 0 ; iPad < iPad_Max ; iPad++){
    ToBeReturned->Fill( aSample.FitRes_PV31_Get_Pull(iPad) ) ;
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_PullFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_PullFitCluster_PV31 (aSample, ModuleNber ,TAG,100 , -5., 5.) ;
}
TH1F* GiveMe_pTH1F_PullFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& PullMin, const double& PullMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_PullFitCluster_PV31" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Pull fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, PullMin , PullMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int iPad_Max = pCluster->FitRes_PV31_Get_NberOfTermsInChi2() ;
      for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      
        ToBeReturned->Fill( pCluster->FitRes_PV31_Get_Pull(iPad) ) ;
      }
      
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinFitCluster_PV31 (aSample, ModuleNber ,TAG,100 , 0., 50.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinMin, const double& Ch2MinMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinFitCluster_PV31" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2Min fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinMin , Ch2MinMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min() ) ;
              
    }
    
  }

  return ToBeReturned ;

}



TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31 (aSample, ModuleNber ,TAG,100 , 0., 5.) ;
}
TH1F* GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NBins, const double& Ch2MinPerDOFMin, const double& Ch2MinPerDOFMax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_Ch2MinPerDOFFitCluster_PV31" + TAG + NameMODULE2 ;
  
  std::string Title = TAG + NameMODULE1 + ": Ch2MinPerDOF fit Cluster 2D" ;
//std::cout << " Title " << Name << std::endl;
  
  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(), NBins, Ch2MinPerDOFMin , Ch2MinPerDOFMax ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      ToBeReturned->Fill( pCluster->Get_Chi2Min()/double( pCluster->FitRes_PV31_Get_NberOfTermsInChi2() - 2 ) )   ;
              
    }
    
  }

  return ToBeReturned ;

}

TH1F* GiveMe_pTH1F_SlopeXY (Sample& aSample, const int& ModuleNber , const std::string&  TAG)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_SlopeXY" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": CutSlopeXY " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, -36 , 36. );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    double CutSlopeXY = pEvent->GiveMe_CutSlopeXY_ForThisModule(ModuleNber) ;  
    
    ToBeReturned->Fill( CutSlopeXY ) ;
  }  
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_SlopeXZ (Sample& aSample, const int& ModuleNber , const std::string&  TAG)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_SlopeXZ" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": CutSlopeXZ " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, -200 , 200. );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    double CutSlopeXZ = pEvent->GiveMe_CutSlopeXZ_ForThisModule(ModuleNber) ;  
    
    ToBeReturned->Fill( CutSlopeXZ ) ;
  }  
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_InterCeptXZ (Sample& aSample, const int& ModuleNber , const std::string&  TAG)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_SlopeXZ" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": CutInterCeptXZ " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, 0 , 520. );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    double CutInterCeptXZ = pEvent->GiveMe_CutInterCeptXZ_ForThisModule(ModuleNber) ;  
    
    ToBeReturned->Fill( CutInterCeptXZ ) ;
  }  
  return ToBeReturned ;
}

//--------Samira My histos---------------------------------------------------------------------
TH1F* GiveMe_pTH1F_SlopeXYZ0 (Sample& aSample, const int& ModuleNber , const std::string&  TAG, const double& v_drift)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_SlopeXYZ0" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": CutSlopeXYZ0 " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, -200.*0.28/36. , 200.*0.28/36. );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector <double> CutSlopeXZ = pEvent->GiveMe_CutSlopeXYZ_ForThisModule(ModuleNber) ;
    double SlopeXYZ0 = CutSlopeXZ[0] * v_drift  ;  
    
    ToBeReturned->Fill( SlopeXYZ0 ) ;
  }  
  return ToBeReturned ;
}

TH1F* GiveMe_pTH1F_SlopeXYZ1 (Sample& aSample, const int& ModuleNber , const std::string&  TAG)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH1F_SlopeXYZ1" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + ": CutSlopeXYZ1 " ;

  TH1F* ToBeReturned = new TH1F(Name.c_str(),Title.c_str(),100, -1. , 1. );
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   

    std::vector <double> CutSlopeXZ = pEvent->GiveMe_CutSlopeXYZ_ForThisModule(ModuleNber) ;
    double SlopeXYZ1 = CutSlopeXZ[1] ;  
    
    ToBeReturned->Fill( SlopeXYZ1 ) ;
  }  
  return ToBeReturned ;
}

TH2F* GiveMe_pTH2F_XZ_display (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_pTH2F_XZ_display(aSample, ModuleNber , TAG, 36, 0., 36, 511, 0., 511.) ;
}
TH2F* GiveMe_pTH2F_XZ_display (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_XZ_display" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + "; X_{Leading Pad} (Unit); Tmax_{Leading Pad} (Time Bin)" ;

  TH2F* ToBeReturned  = new TH2F(Name.c_str(),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iX_Leading          = pPad_Leading->Get_iX()     ;
      double Tmax_Leading     = pPad_Leading->Get_TMax()   ;
  
      ToBeReturned->Fill(iX_Leading,Tmax_Leading) ;
      }  
    }
  
  return ToBeReturned ;
}
//--------------------------------------------------------------
TH2F* GiveMe_pTH2F_XY_display (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_pTH2F_XY_display(aSample, ModuleNber , TAG, 36, 0., 36, 36, 0., 36.) ;
}
TH2F* GiveMe_pTH2F_XY_display (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::ostringstream aostringstreamMODULE1 ;
  aostringstreamMODULE1 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE1 << " Mod "  << ModuleNber;
  std::string NameMODULE1 =  aostringstreamMODULE1.str() ;

  std::ostringstream aostringstreamMODULE2 ;
  aostringstreamMODULE2 << std::setiosflags(std::ios::fixed) ;
  aostringstreamMODULE2 << "_Mod_"  << ModuleNber;
  std::string NameMODULE2 =  aostringstreamMODULE2.str() ;

  std::string Name  = "GiveMe_pTH2F_XY_display" + TAG + NameMODULE2; 
  std::string Title = TAG + NameMODULE1 + "; X_{Leading Pad} (cm); Y_{Weight} (cm);" ;

  TH2F* ToBeReturned  = new TH2F(Name.c_str(),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iX_Leading          = pPad_Leading->Get_iX()     ;
      double iY_Leading       = pPad_Leading->Get_iY()   ;

      //double Yi = pCluster->Get_YWeight() *1.E2 ;
      //double Xi = (pCluster->Get_Pad(0))->Get_XPad()*1.E2 ;
      
      ToBeReturned->Fill(iX_Leading,iY_Leading) ;
      }  
    }
  
  return ToBeReturned ;
}
