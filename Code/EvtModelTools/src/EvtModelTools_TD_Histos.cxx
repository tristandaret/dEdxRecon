#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "Misc/Util.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "Tristan_DESY21/DESY21_Fits.h"

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

#include <vector>
using namespace std ;


///////////////////////////////////////////////// TH1I /////////////////////////////////////////////////////////////////////////////

// Beam vertical position
TH1I* GiveMe_LeadingiY (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title              = TAG + ": Row iY of the leading pad;Row;Count" ;
  TH1I* pth1i_iYlead        = new TH1I(TString("pth1i_iYlead" + TAG),Title.c_str(),32, -0.5 , 31.5 ) ; // 34 rows - 2 borders = 32 pads

  int NberOfEvents          = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){          
    Event* pEvent           =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;            // Events are invalidated if they have less than 20 clusters
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Lead  = pCluster->Get_LeadingPad() ;
      int iY_Leading        = pPad_Lead->Get_iY() ;
      pth1i_iYlead->Fill( iY_Leading ) ;
    }
  }
  return pth1i_iYlead ;
}


// Number of pads per cluster
TH1I* GiveMe_PadPerCluster (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title          = TAG + ": Number of pads per cluster;Number of pads;Count" ;
  TH1I* pth1i_NPads     = new TH1I("pth1i_NPads",Title.c_str(),12, -1.5 , 10.5 ) ;

  int NberOfEvents      = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent       =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads = pCluster->Get_NberOfPads() ;
      pth1i_NPads->Fill(NPads) ;
      
    }
  }
  return pth1i_NPads ;
}



// Cluster Verticality
TH1I* GiveMe_Cluster_Verticality (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title                = TAG + ": Column(Leading) - Column(Neighbour);Column difference ;Count" ;
  TH1I* pth1i_ClusVerticality = new TH1I("pth1i_ClusVerticality",Title.c_str(), 21, -10 , 10 ) ;

  int NberOfEvents            = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent             = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pLeadPad     = pCluster->Get_LeadingPad() ;

      int NPads = pCluster->Get_NberOfPads() ;
      int ix_leading          = pLeadPad->Get_iX() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad       = pCluster->Get_Pad(iP) ;
        int ix_pad            = pPad->Get_iX() ;
        if (pLeadPad != pPad){
          int deviation       = ix_leading - ix_pad ;
          pth1i_ClusVerticality->Fill(deviation) ;
        }
      }            
    }
  }
  return pth1i_ClusVerticality ;
}



// Number of clusters per pad column (should be 1, or 0 for border columns)
TH1I* GiveMe_NClusPerCol (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title = TAG + ": Cluster density;Number of clusters per pad column;Count" ;
  TH1I* pth1i_ClusperCol = new TH1I("pth1i_ClusperCol", Title.c_str(), 4, -0.5, 3.5) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    int Ncols = 36 ;
    vector<int> counter(Ncols,0) ; // counter of number of clusters per columns
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pLeadPad  = pCluster->Get_LeadingPad() ;
      int ix_leading    = pLeadPad->Get_iX() ;
      counter[ix_leading]++ ;
    }

    // number of clusters per column
    for (int iCol = 0 ; iCol < Ncols ; iCol++){
      pth1i_ClusperCol->Fill(counter[iCol]) ;
    }
  }
  return pth1i_ClusperCol ;
}


// Difference of A_max with Pad* or with ADC Waveform
TH1I* GiveMe_Amax_AmaxWF (Sample& aSample, const int& ModuleNber, const string& TAG) 
{
  string Title = TAG + ": #DeltaA_{max} = A_{max}(Pad) - A_{max}(ADC Waveform);ADC difference;Count" ;
  TH1I* pth1i_AmaxDiff = new TH1I("pth1i_AmaxDiff", Title.c_str(), 21, -10 , 10 ) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;
        
        pth1i_AmaxDiff->Fill( pPad->Get_AMax() - pPad->Get_AMax_WF() ) ;
      }
    }
  }
  return pth1i_AmaxDiff ;
}


// Time difference for A_max obtained directly with Pad* of reconstructed using ADC waveform
TH1I* GiveMe_Tmax_TmaxWF (Sample& aSample, const int& ModuleNber, const string& TAG) 
{
  string Title = TAG + ": #DeltaT_{max} = T_{max}(Pad) -T_{max}(ADC Waveform);#DeltaT_{max} (/40 ns);Count" ;
  TH1I* pth1i_TimeDiff = new TH1I("pth1i_TimeDiff",Title.c_str(), 21, -10 , 10 ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
            
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP) ;
        pth1i_TimeDiff->Fill( pPad->Get_TMax() - pPad->Get_TMax_WF() ) ;
      }
    }
  }
  return pth1i_TimeDiff ;
}



// Number of Clusters in an Event
TH1I* GiveMe_ClusterPerEvent (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title = TAG + ": Clusters in an Event ;Number of clusters ;Count" ;
  TH1I* pth1i_EventDensity = new TH1I("pth1i_EventDensity", Title.c_str(), 50, -0.5, 49.5) ;  

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    pth1i_EventDensity->Fill(NClusters) ;
  }
  return pth1i_EventDensity ;
}



// DeltaT between neighbour pads and leading pad (using Pad*)
TH1I* GiveMe_DeltaT (Sample& aSample, const int& ModuleNber, const string& TAG, const int& NeighbourNumber, std::string Stage)
{
  return GiveMe_DeltaT(aSample, ModuleNber , TAG, Stage, NeighbourNumber, 450, -150, 300);
}
TH1I* GiveMe_DeltaT (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber, int Nbin, int DTmin, int DTmax)
{
  string Title          = TAG + ": #DeltaT_{max} = T_{max}(Leading) - T_{max}(Neighbour#"+std::to_string(NeighbourNumber)+");#DeltaT_{max} (/40 ns) ;Count" ;
  TH1I* pth1i_DeltaT    = new TH1I(TString("pth1i_DeltaT" + std::to_string(NeighbourNumber) + TAG),Title.c_str(), Nbin, DTmin, DTmax ) ;

  int NberOfEvents      = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent       =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading      = pCluster->Get_TMaxLeading() ;
      int NPads         = pCluster->Get_NberOfPads() ;
      if(NPads >= NeighbourNumber){
        const Pad* pPadNeigh       = pCluster->Get_Pad(NPads - NeighbourNumber) ;          
        int Tneigh      = pPadNeigh->Get_TMax() ;
        pth1i_DeltaT->Fill(Tneigh - TLeading) ;
      }
    }
  }
  return pth1i_DeltaT ;
}



// T_max of Leading Pad
TH1I* GiveMe_LeadTmax (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title          = TAG + ": T_{max} of leading pad ; #DeltaT_{max} (/40ns) ; Count" ;
  TH1I* pth1i_Tmax = new TH1I("pth1i_Tmax",Title.c_str(), 510, 0, 510 ) ;

  int NberOfEvents      = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent       =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading      = pCluster->Get_TMaxLeading() ;
      pth1i_Tmax->Fill(TLeading) ;
    }
  }
  return pth1i_Tmax ;
}

// Time difference between each T_{leading} the mean of all T_{leading}'s
TH1I* GiveMe_TLeading_TEvent (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_TLeading_TEvent(aSample, ModuleNber ,TAG,600,-150,450) ;
}
TH1I* GiveMe_TLeading_TEvent (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Nbins, const int& Diffmin, const int& Diffmax)
{
  std::string Title     = TAG + ": #DeltaT_{max} between T_{leading} and the mean of T_{leading};#DeltaT_{max};Count" ;
  TH1I* pth1i_TLeadTEvent    = new TH1I("pth1i_TLeadTEvent",Title.c_str(), Nbins, Diffmin, Diffmax ) ;

  int NberOfEvents      = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent       =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    double Tmean        = 0 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      Tmean += pCluster->Get_TMaxLeading() ;
    }
    Tmean = Tmean / double(NClusters) ;

    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      pth1i_TLeadTEvent->Fill(pCluster->Get_TMaxLeading() - Tmean) ;
    }
  }
  return pth1i_TLeadTEvent ;
}

// Amplitude du leading pad
TH1I* GiveMe_AmaxLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_AmaxLeading(aSample, ModuleNber , TAG, 4096, 0, 4096) ;
}
TH1I* GiveMe_AmaxLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& nbins, const int& ADCmin, const int& ADCmax) 
{
  std::string Title = TAG + ": Maximum amplitude of the leading pads;ADCs;Count" ;
  TH1I* pth1i_Amax_Lead = new TH1I("pth1i_Amax_Lead",Title.c_str(), nbins,  ADCmin , ADCmax ) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int ALeading      = pCluster->Get_AMaxLeading() ;
      pth1i_Amax_Lead->Fill(ALeading) ;
    }
  }
  return pth1i_Amax_Lead ;
}


// Amplitude du subleading pad
TH1I* GiveMe_AmaxSubLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_AmaxSubLeading(aSample, ModuleNber , TAG, 4096, 0, 4096) ;
}
TH1I* GiveMe_AmaxSubLeading (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& nbins, const int& ADCmin, const int& ADCmax) 
{
  std::string Title = TAG + ": Maximum amplitude of the Subleading pads;ADCs;Count" ;
  TH1I* pth1i_ASubleading = new TH1I("pth1i_ASubleading",Title.c_str(), nbins,  ADCmin , ADCmax ) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads               = pCluster->Get_NberOfPads() ;
      if(NPads > 1){
        const Pad* pSubLeading  = pCluster->Get_Pad(NPads-2) ;
        int ASubLeading         = pSubLeading->Get_AMax() ;
        pth1i_ASubleading->Fill(ASubLeading) ;
      }
    }
  }
  return pth1i_ASubleading ;
}


// Difference Amplitude of Cluster with Get_Amplitude vs summing amplitudes of all pads
TH1I* GiveMe_AClus_ASumPads (Sample& aSample, const int& ModuleNber, const std::string& TAG) 
{
  return GiveMe_AClus_ASumPads(aSample, ModuleNber , TAG, 21, -10 , 10 ) ;
}
TH1I* GiveMe_AClus_ASumPads (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& nbins, const int& ADCmin, const int& ADCmax)
{ 
  std::string Title = TAG + ": A_{Cluster} - Sum(A_{pads});ADCs difference;Count" ;
  TH1I* pth1i_AClus_ASumPads = new TH1I("pth1i_AClus_ASumPads", Title.c_str(), nbins, ADCmin , ADCmax ) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int ACluster        = pCluster->Get_Acluster() ;
      int ASumPads        = 0 ;
            
      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads; iP++){
        const Pad* pPad   = pCluster->Get_Pad(iP) ;
        ASumPads         += pPad->Get_AMax() ;
      }
      pth1i_AClus_ASumPads->Fill( ACluster - ASumPads ) ;
    }
  }
  return pth1i_AClus_ASumPads ;
}



///////////////////////////////////////////////// TH1D /////////////////////////////////////////////////////////////////////////////



// Rho weight = A(pad) / A(Cluster)
TH1D* GiveMe_Rho (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  return GiveMe_Rho(aSample, ModuleNber , TAG, -1000, 1000) ;
}
TH1D* GiveMe_Rho (Sample& aSample, const int& ModuleNber, const string& TAG, const int& Npadmin, const int& Npadmax)
{
  string Title = TAG + ": Weight #rho of leading pads ;#rho ;Normalized occurences" ;
  TH1D* pth1d_rho = new TH1D(TString("pth1d_rho" + TAG),Title.c_str(), 120, -0.1 , 1.1 ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pLeadPad  = pCluster->Get_LeadingPad() ;
      int NPads = pCluster->Get_NberOfPads() ;
      int AsubLeading = pCluster->Get_Pad(0)->Get_AMax() ; // only for clusters with 2 pads so subleading is #NPads-2 = 0
      if(NPads <= Npadmax and NPads >= Npadmin){
        if(NPads != 2)                      pth1d_rho->Fill(pLeadPad->Get_AMax() / pCluster->Get_Acluster()) ;
        if(NPads ==2 and AsubLeading > 110) pth1d_rho->Fill(pLeadPad->Get_AMax() / pCluster->Get_Acluster()) ;
      }
    }
  }
  int Nhrho = pth1d_rho->GetEntries();
  if(Nhrho == 0){
    delete pth1d_rho ;
    pth1d_rho = 0 ;
  }
  return pth1d_rho ;
}




// y_T weighted position
TH1D* GiveMe_YT (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  TH1I* hiY    = GiveMe_LeadingiY(aSample, ModuleNber , "Tempo") ; // to be able to center the plot on iY
  int iYmean = hiY->GetMaximumBin() ;
  delete hiY ;
  Model_ReadOutGeometry* pROG = aSample.Get_Model_ReadOutGeometry() ;
  double Ymean = pROG->Get_YcPad(0,iYmean-1,ModuleNber) ;
  double Ymin  = Ymean - 2.5*pROG->Get_LY() ;
  double Ymax  = Ymean + 2.5*pROG->Get_LY() ;         // Sets limits of Y (on X-axis): 5 pads in total

  string Title = TAG + ": Track position y_{T} of cluster ;y_{T} (cm);Count" ;
  TH1D* pth1d_YT = new TH1D("pth1d_YT",Title.c_str(), 100, Ymin*100 , Ymax*100) ; // *100 to work with cm

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      pth1d_YT->Fill(pCluster->Get_YTrack()*100) ;
    }
  }
  return pth1d_YT ;
}



// Relative Y position: y_T - y_{pad} for each pad
TH1D* GiveMe_YTYpad (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  return GiveMe_YTYpad(aSample, ModuleNber , TAG, 100, -3, 3) ;
}
TH1D* GiveMe_YTYpad (Sample& aSample, const int& ModuleNber, const string& TAG, const int& Ybin, const int& Ymin, const int& Ymax)
{
  string Title          = TAG + ": Track position y_{T}-y_{pad} in the cluster;y_{T} - y_{pad} (cm);Count" ;
  TH1D* pth1d_YYTYpad         = new TH1D("pth1d_YYTYpad",Title.c_str(), Ybin, Ymin , Ymax) ;                               // at most +- 1.5 * pad size (1.009cm) since we keep 3 pads/Cluster

  int NberOfEvents      = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent       =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT         = pCluster->Get_YTrack()*100 ;

      int NPads         = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad       = pCluster->Get_Pad(iP) ;          
        double YTYpad   = YT - pPad->Get_YPad()*100 ;
        pth1d_YYTYpad->Fill(YTYpad) ;
      }  
    }
  }
  return pth1d_YYTYpad ;
}


// Difference YT and YT_before_miniminsation
TH1D* GiveMe_DeltaYT (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax)  
{ 
  std::string Title = TAG + ": y_{T} - y_{T}(before minimization);#DeltaY_{T} (mm);Count" ;
  TH1D* pth1d_Y1_diffDeltaYT = new TH1D("pth1d_Y1_diffDeltaYT", Title.c_str(), Ybins, Ymin , Ymax) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack();
      double YT_before    = pCluster->Get_YTrack_BeforeMinimisation() ;
      pth1d_Y1_diffDeltaYT->Fill( (YT - YT_before)*1000 ) ;
    }
  }
  return pth1d_Y1_diffDeltaYT ;
}


// Ampllitude of a cluster divide by its number of pads
TH1D* GiveMe_AclusNPads (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage) 
{
  return GiveMe_AclusNPads(aSample, ModuleNber , TAG, Stage, 2001, 0 , 2000 ) ;
}
TH1D* GiveMe_AclusNPads (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& nbins, const int& Xmin, const int& Xmax)
{ 
  std::string Title = TAG + ": Amplitude in a cluster divide by its number of pads;ADCs/N_{pads};Count" ;
  TH1D* pth1_AclusNPads = new TH1D(TString(TAG + "_pth1_AclusNPads_" + Stage), Title.c_str(), nbins, Xmin , Xmax ) ;
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double ACluster        = pCluster->Get_Acluster() ;  
      double NPads           = pCluster->Get_NberOfPads() ;
      pth1_AclusNPads->Fill( ACluster/NPads ) ;
    }
  }
  return pth1_AclusNPads ;
}



// Discard clusters misaligned with the tendency of the track
TH1D* GiveMe_TrackAlignResiduals (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage, const int& iIter)
{
  std::string Title         = TAG + ": Residuals of track alignment selection;Residuals (#mum);Count" ;
  TH1D* pth1_trackalignresi = new TH1D (TString(TAG + "pth1_trackalignresi" + Stage),Title.c_str(), 100, -7000, 7000) ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NEvents; iE++){
    Event* pEvent           = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    TH2D* pTH2D_Evt_display = GiveMe_EvtDisplay(pEvent, ModuleNber ,std::string(TAG + std::to_string(iIter))) ;
    TGraphErrors* ptge_Evt  = TH2_to_TGE(pTH2D_Evt_display); // gets Yw out of the EvtDisp
    TF1* ptf1_Evt_fit       = GiveMe_FitYwforSelection(ptge_Evt) ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iX                = pCluster->Get_LeadingPad()->Get_iX() ;
      double fresidual      = ptge_Evt->GetPointY(iC) - ptf1_Evt_fit->Eval(iX) ;
      pth1_trackalignresi->Fill(fresidual*10000) ;
    }
    delete pTH2D_Evt_display ;
    delete ptge_Evt ;
    delete ptf1_Evt_fit ;
  }
  return pth1_trackalignresi ;
}




///////////////////////////////////////////////// TH2I /////////////////////////////////////////////////////////////////////////////

// 2D DeltaT VS Tmax Leading  (Y vs X)
TH2I* GiveMe_Tmax_PadvLead (Sample& aSample, const int& ModuleNber, const string& TAG) // 2D
{
  string Title = TAG + ": #DeltaT_{max}(Neighbour-Lead) vs T_{max}(Leading); T_{max}(Leading) (/40ns) ; #DeltaT_{max} (/40ns)" ;
  TH2I* pth2i_YTmaxPadvLead     = new TH2I("pth2i_YTmaxPadvLead",Title.c_str(), 510, 0, 510, 450,-150,300) ;

  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pLeadPad    = pCluster->Get_LeadingPad() ;
      int TLeading        = pCluster->Get_TMaxLeading() ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        int DeltaT        = pPad->Get_TMax() - pCluster->Get_TMaxLeading() ;
        if (pLeadPad  != pPad){
          pth2i_YTmaxPadvLead->Fill(TLeading, DeltaT) ;
        }
      }  
    }
  }
  return pth2i_YTmaxPadvLead ;
}



// (T2max - T1max) VS (T3max - T1max)
TH2I* GiveMe_DeltaT21vs31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage)
{
  return GiveMe_DeltaT21vs31(aSample, ModuleNber , TAG, Stage, 351, -150, 200, 351, -150, 200);
}
TH2I* GiveMe_DeltaT21vs31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, int Nbin21, int DTmin21, int DTmax21, int Nbin31, int DTmin31, int DTmax31)
{
  string Title              = TAG + ": #DeltaT_{max}(2 - Lead) VS #DeltaT_{max}(3 - Lead);#DeltaT_{max}(3 - Lead) (/40 ns);#DeltaT_{max}(2 - Lead) (/40 ns)" ;
  TH2I* pth2i_YDeltaT21vs31 = new TH2I(TString(TAG + "hDeltaT21vs31_Stage" + Stage),Title.c_str(), Nbin21, DTmin21, DTmax21, Nbin31, DTmin31, DTmax31) ;

  int NberOfEvents          = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent           =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading          = pCluster->Get_TMaxLeading() ;
      int NPads             = pCluster->Get_NberOfPads() ;
      if(NPads >= 3){
        int iY              = pCluster->Get_LeadingPad()->Get_iY() ;
        const Pad* pPad2    = pCluster->Get_Pad(NPads - 2) ;          
        const Pad* pPad3    = pCluster->Get_Pad(NPads - 3) ;
        int iY2             = pPad2->Get_iY() ;
        int iY3             = pPad3->Get_iY() ;
        if((iY3 == iY-1 or iY3 == iY+1) and (iY2 == iY-1 or iY2 == iY+1)){
          int T2            = pPad2->Get_TMax() ;
          int T3            = pPad3->Get_TMax() ;
          pth2i_YDeltaT21vs31->Fill(T3 - TLeading, T2 - TLeading) ;
        }
      }
    }
  }
  return pth2i_YDeltaT21vs31 ;
}


///////////////////////////////////////////////// TH2F /////////////////////////////////////////////////////////////////////////////

// TH2 rho vs Delta_T
TH2F* GiveMe_2D_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  return GiveMe_2D_Rho_DeltaT(aSample, ModuleNber , TAG, std::string("B"), 2, 450, -150, 300, 120, -0.1, 0.6) ; // rhomax = 0.6 since its the rho of the subleading so it can't excess 0.5
}
TH2F* GiveMe_2D_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const string& TAG, std::string Stage, const int& NeighbourNumber)
{
  return GiveMe_2D_Rho_DeltaT(aSample, ModuleNber , TAG, Stage, NeighbourNumber, 450, -150, 300, 120, -0.1, 0.6) ;
}
TH2F* GiveMe_2D_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string& TAG, std::string Stage, const int& NeighbourNumber, int DTbins, int DTmin, int DTmax, int rhobin, float rhomin, float rhomax) 
{
  string Title                = TAG + ": #rho VS #DeltaT_{max}(Leading-Neighbour#"+std::to_string(NeighbourNumber)+");#DeltaT_{max} (/40 ns);#rho" ;
  TH2F* pth2f_YRho_DeltaT     = new TH2F(TString("pth2f_YRho_DeltaT" + std::to_string(NeighbourNumber) + TAG),Title.c_str(), DTbins, DTmin, DTmax, rhobin, rhomin, rhomax) ;

  int NberOfEvents            = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent             =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading            = pCluster->Get_TMaxLeading() ;
      int NPads               = pCluster->Get_NberOfPads() ;
      if(NPads >= NeighbourNumber){
        const Pad* pPadNeigh  = pCluster->Get_Pad(NPads - NeighbourNumber) ;
        int Tneigh            = pPadNeigh->Get_TMax() ;        
        int DeltaT            = Tneigh - TLeading ;
        double rho            = pPadNeigh->Get_AMax() / pCluster->Get_Acluster() ;
        pth2f_YRho_DeltaT->Fill(DeltaT,rho) ;
      }
    }
  }
  return pth2f_YRho_DeltaT ;
}

///////////////////////////////////////////////// TH2D /////////////////////////////////////////////////////////////////////////////

// 2D: rho vs y_{T}
TH2D* GiveMe_2D_Rho_YT (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_2D_Rho_YT(aSample, ModuleNber , TAG, 1700, 0, 17, 120, -0.1, 1.1) ;
}
TH2D* GiveMe_2D_Rho_YT(Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rho VS y_{T};y_{T} (cm);#rho" ;
  TH2D* pth2d_Rho_YT  = new TH2D("pth2d_Rho_YT",Title.c_str(), NYbins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        double rho        = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        pth2d_Rho_YT->Fill(YT,rho) ;
      }  
    }
  }
  return pth2d_Rho_YT ;
}



// 2D: rho* = Ai/A(leading) vs y_{T} for PV2 procedure
TH2D* GiveMe_2D_RhoPV2_YT (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_2D_RhoPV2_YT(aSample, ModuleNber , TAG, 1700, 0, 17, 120, -0.1, 1.1) ;
}
TH2D* GiveMe_2D_RhoPV2_YT(Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rho_{PV2} = A_{i} / A_{leading} VS y_{T};y_{T} (cm);#rho" ;
  TH2D* pth2d_RhoPV2_YT  = new TH2D("pth2d_RhoPV2_YT",Title.c_str(), NYbins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        double rhoPV2        = pPad->Get_AMax() / pCluster->Get_LeadingPad()->Get_AMax() ;
        pth2d_RhoPV2_YT->Fill(YT,rhoPV2) ;
      }  
    }
  }
  return pth2d_RhoPV2_YT ;
}



// 2D: rho vs y_{T} - y_{pad}
TH2D* GiveMe_2D_Rho_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int PV)
{
  if(PV == 0) return GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , TAG, 100, -2.5*1.019, 2.5*1.019, 120, -0.1, 1.1) ;
  if(PV == 1) return GiveMe_2D_Rho_YTYpad(aSample, ModuleNber , TAG, 100, -1.5*1.019, 1.5*1.019, 120, -0.1, 1.1) ;
  return 0 ;
}
TH2D* GiveMe_2D_Rho_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::string Title       = TAG + ": #rho VS y_{T} - y_{pad};y_{T} - y_{pad} (cm);#rho" ;
  TH2D* pth2d_Rho_YTYpad  = new TH2D(TString("pth2d_Rho_YTYpad" + TAG),Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad   = pCluster->Get_Pad(iP) ;          
        double YTYpad     = YT - pPad->Get_YPad()*100 ;
        double rho        = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        pth2d_Rho_YTYpad->Fill(YTYpad,rho) ;
      }  
    }
  }
  return pth2d_Rho_YTYpad ;
}



// 2D: rho* = Ai/A(leading) vs y_{T} - y_{pad} for PV2 procedure
TH2D* GiveMe_2D_RhoPV2_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_2D_RhoPV2_YTYpad(aSample, ModuleNber , TAG, 100, -1.5*1.019, 1.5*1.019, 120, -0.1, 1.1) ;
}
TH2D* GiveMe_2D_RhoPV2_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Ybins, const double& Ymin, const double& Ymax, const int& rhobin, const double& rhomin, const double& rhomax)
{
  std::string Title       = TAG + ": #rho = A_{i}/A_{leading} VS y_{T} - y_{pad};y_{T} - y_{pad} (cm);#rho" ;
  TH2D* pth2d_RhoPV2_YTYpad = new TH2D("pth2d_RhoPV2_YTYpad",Title.c_str(), Ybins, Ymin, Ymax, rhobin, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad   = pCluster->Get_Pad(iP) ;          
        double YTYpad     = YT - pPad->Get_YPad()*100 ;
        double rho        = pPad->Get_AMax() / pCluster->Get_LeadingPad()->Get_AMax() ;
        pth2d_RhoPV2_YTYpad->Fill(YTYpad,rho) ;
      }  
    }
  }
  return pth2d_RhoPV2_YTYpad ;
}



// 2D: DeltaYT VS Y_T - Y_pad
TH2D* GiveMe_2D_DeltaY_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& ytypadbin, const double& ytypadmin, const double& ytypadmax, const int& DYbins, const double& DYmin, const double& DYmax)
{
  std::string Title       = TAG + ": #DeltaY_{T} VS y_{T} - y_{pad};y_{T} - y_{pad} (mm);#DeltaYT (cm)" ;
  TH2D* ph2_DeltaY_YTYpad = new TH2D(TString("ph2_DeltaY_YTYpad" + TAG),Title.c_str(), ytypadbin, ytypadmin, ytypadmax, DYbins, DYmin, DYmax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack() ;
      double YT_before    = pCluster->Get_YTrack_BeforeMinimisation() ;
      double DeltaYT      = (YT-YT_before)*1000 ; // Get it in mm

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad   = pCluster->Get_Pad(iP) ; 
        double Ypad       = pPad->Get_YPad() ;
        double YTYpad     = (YT - Ypad)*100 ; // Get it in cm
        ph2_DeltaY_YTYpad->Fill(YTYpad, DeltaYT) ;
      }
    }
  }
  return ph2_DeltaY_YTYpad ;
}


// 2D: A_leading VS iX
TH2D* GiveMe_2D_ALeading_iX (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_2D_ALeading_iX (aSample, ModuleNber , TAG, 100, 0, 4000, 36, 0, 35) ;
}
TH2D* GiveMe_2D_ALeading_iX (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Abin, const double& Amin, const double& Amax, const int& iXbin, const double& iXmin, const double& iXmax)
{
  std::string Title       = TAG + ": Max amplitude of leading pad VS iX position ;iX;A_{max}(leading) (ADCs)" ;
  TH2D* ph2_Alead_iX      = new TH2D("ph2_Alead_iX",Title.c_str(), iXbin, iXmin, iXmax, Abin, Amin, Amax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iX              = pCluster->Get_LeadingPad()->Get_iX() ;
      int Amax            = pCluster->Get_LeadingPad()->Get_AMax() ;
      ph2_Alead_iX->Fill(iX, Amax) ;
    }
  }
  return ph2_Alead_iX ;
}


// 2D: A_subleading / A_leading VS iX
TH2D* GiveMe_2D_AsubRelative_iX (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_2D_AsubRelative_iX (aSample, ModuleNber , TAG, 100, 0, 0.3, 36, 0, 35) ;
}
TH2D* GiveMe_2D_AsubRelative_iX (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& Abin, const double& Amin, const double& Amax, const int& iXbin, const double& iXmin, const double& iXmax)
{
  std::string Title       = TAG + ": A_{max}(subleading)/A_{max}(leading) VS iX position ;iX;A_{max}(subleading)/A_{max}(leading)" ;
  TH2D* ph2_AsubRelative_iX = new TH2D("ph2_AsubRelative_iX",Title.c_str(), iXbin, iXmin, iXmax, Abin, Amin, Amax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int NPads           = pCluster->Get_NberOfPads() ;
      int iX              = pCluster->Get_LeadingPad()->Get_iX() ;
      double Amax_lead       = pCluster->Get_LeadingPad()->Get_AMax() ;
      if(NPads >=2){
        double Amax_sub = pCluster->Get_Pad(NPads - 2)->Get_AMax() ;
        ph2_AsubRelative_iX->Fill(iX, Amax_sub/Amax_lead) ;
      }
    }
  }
  return ph2_AsubRelative_iX ;
}


// 2D: A2/A_leading VS A3/A_leading
TH2D* GiveMe_2D_A21vsA31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage)
{
  return GiveMe_2D_A21vsA31 (aSample, ModuleNber , TAG, Stage, 100, 0, 0.5, 100, 0, 1) ;
}
TH2D* GiveMe_2D_A21vsA31 (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string Stage, const int& A31bin, const double& A31min, const double& A31max, const int& A21bin, const double& A21min, const double& A21max)
{
  std::string Title       = TAG + ": A_{max}(2)/A_{max}(leading) VS A_{max}(3)/A_{max}(leading);A_{max}(3)/A_{max}(leading);A_{max}(2)/A_{max}(leading)" ;
  TH2D* pth2_A21vsA31     = new TH2D(TString( TAG + "_ph2_A21vsA31_" + Stage),Title.c_str(), A31bin, A31min, A31max, A21bin, A21min, A21max) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double Amax_lead    = pCluster->Get_LeadingPad()->Get_AMax() ;
      int NPads           = pCluster->Get_NberOfPads() ;
      if(NPads >=3){
        double Amax2      = pCluster->Get_Pad(NPads - 2)->Get_AMax() ;
        double Amax3      = pCluster->Get_Pad(NPads - 3)->Get_AMax() ;
        pth2_A21vsA31->Fill(Amax3/Amax_lead, Amax2/Amax_lead) ;
      }
    }
  }
  return pth2_A21vsA31 ;
}

///////////////////////////////////////////////// TPROFILE /////////////////////////////////////////////////////////////////////////

// TPROFILE rho vs y_{T}
TProfile* GiveMe_Rho_YT (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_Rho_YT(aSample, ModuleNber , TAG, 1700, 0, 17, -0.1, 1.1) ;
}
TProfile* GiveMe_Rho_YT(Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rho VS y_{T};y_{T} (cm);#rho" ;
  TProfile* tpRho_YT  = new TProfile(TString("tpRho_YT" + TAG),Title.c_str(), NYbins, Ymin, Ymax, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad   = pCluster->Get_Pad(iP) ;          
        double rho        = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        tpRho_YT->Fill(YT,rho) ;
      }  
    }
  }
  return tpRho_YT ;
}



// TPROFILE rhoPV2 vs y_{T}
TProfile* GiveMe_RhoPV2_YT (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_RhoPV2_YT(aSample, ModuleNber , TAG, 1700, 0, 17, -0.1, 1.1) ;
}
TProfile* GiveMe_RhoPV2_YT(Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rhoPV2 VS y_{T};y_{T} (cm);#rho" ;
  TProfile* pth2d_RhoPV2_YT  = new TProfile("pth2d_RhoPV2_YT",Title.c_str(), NYbins, Ymin, Ymax, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        double rhoPV2        = pPad->Get_AMax() / pCluster->Get_LeadingPad()->Get_AMax() ;
        pth2d_RhoPV2_YT->Fill(YT,rhoPV2) ;
      }  
    }
  }
  return pth2d_RhoPV2_YT ;
}



// TPROFILE rho vs y_{T} - y_{pad}
TProfile* GiveMe_Rho_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_Rho_YTYpad(aSample, ModuleNber , TAG, 250, -1.5*1.019, 1.5*1.019, -0.1, 1.1) ;
}
TProfile* GiveMe_Rho_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rho VS y_{T} - y_{pad};y_{T} - y_{pad} (cm);#rho" ;
  TProfile* tpRho_YTYpad  = new TProfile("tpRho_YT-Ypad",Title.c_str(), NYbins, Ymin, Ymax, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        double YTYpad     = YT - pPad->Get_YPad()*100 ;
        double rho        = pPad->Get_AMax() / pCluster->Get_Acluster() ;
        tpRho_YTYpad->Fill(YTYpad,rho) ;
      }  
    }
  }
  return tpRho_YTYpad ;
}



// TPROFILE rhoPV2 vs y_{T} - y_{pad}
TProfile* GiveMe_RhoPV2_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG)
{
  return GiveMe_RhoPV2_YTYpad(aSample, ModuleNber , TAG, 250, -1.5*1.019, 1.5*1.019, -0.1, 1.1) ;
}
TProfile* GiveMe_RhoPV2_YTYpad (Sample& aSample, const int& ModuleNber, const std::string& TAG, const int& NYbins, const double& Ymin, const double& Ymax, const double& rhomin, const double& rhomax)
{
  string Title            = TAG + ": #rho = A_{i}/A_{leading} VS y_{T} - y_{pad};y_{T} - y_{pad} (cm);#rho" ;
  TProfile* tpRhoPV2_YTYpad  = new TProfile("pth2d_RhoPV2_YT-Ypad",Title.c_str(), NYbins, Ymin, Ymax, rhomin, rhomax) ;
  
  int NberOfEvents        = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent         =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT           = pCluster->Get_YTrack()*100 ;

      int NPads           = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){
        const Pad* pPad         = pCluster->Get_Pad(iP) ;          
        double YTYpad     = YT - pPad->Get_YPad()*100 ;
        double rhoPV2        = pPad->Get_AMax() / pCluster->Get_LeadingPad()->Get_AMax() ;
        tpRhoPV2_YTYpad->Fill(YTYpad,rhoPV2) ;
      }  
    }
  }
  return tpRhoPV2_YTYpad ;
}



// TPROFILE rho vs Delta_T
TProfile* GiveMe_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Stage)
{
  return GiveMe_Rho_DeltaT (aSample, ModuleNber , TAG, Stage, 2, 450, -150, 300, -0.1, 0.6) ; // rhomax = 0.6 since its the rho of the subleading so it can't excess 0.5
}
TProfile* GiveMe_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Stage, const int& NeighbourNumber)
{
  return GiveMe_Rho_DeltaT (aSample, ModuleNber , TAG, Stage, NeighbourNumber, 450, -150, 300, -0.1, 0.6) ;
}
TProfile* GiveMe_Rho_DeltaT (Sample& aSample, const int& ModuleNber, const std::string& TAG, const std::string& Stage, const int& NeighbourNumber, const int& DTbin, const int& DTmin, const int& DTmax, const double& rhomin, const double& rhomax)
{
  string Title                = TAG + ": TProfile #rho VS #DeltaT_{max}(Leading-Neighbour#"+std::to_string(NeighbourNumber)+");#DeltaT_{max} (/40 ns);#rho" ;
  TProfile* tpRho_DeltaT      = new TProfile(TString("tpRho_DeltaT" + std::to_string(NeighbourNumber) + TAG),Title.c_str(), DTbin, DTmin, DTmax, rhomin, rhomax) ;

  int NberOfEvents            = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent             =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading            = pCluster->Get_TMaxLeading() ;

      int NPads               = pCluster->Get_NberOfPads() ;
      if(NPads >= NeighbourNumber){
        const Pad* pPadNeigh  = pCluster->Get_Pad(NPads - NeighbourNumber) ;
        int Tneigh            = pPadNeigh->Get_TMax() ;        
        int DeltaT            = Tneigh - TLeading ;
        double rho            = pPadNeigh->Get_AMax() / pCluster->Get_Acluster() ;
        tpRho_DeltaT->Fill(DeltaT,rho) ;
      }  
    }
  }
  return tpRho_DeltaT ;
}




// TPROFILE Delta_T vs Y_relat
TProfile* GiveMe_DeltaT_YTYpad (Sample& aSample, const int& ModuleNber, const string& TAG)
{
  string Title = TAG + ": #DeltaT_{max}(Neighbour-Lead) VS y_T - y_{pad};y_T - y_{pad} (cm);#DeltaT_{max}" ;
  TProfile* tpDeltaT_YTYpad = new TProfile("tpDeltaT_YTYpad",Title.c_str(), 100, -1.8, 1.8, -510, 510) ;

  int NberOfEvents       = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent        =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      double YT         = pCluster->Get_YTrack()*100 ;
      int TLeading      = pCluster->Get_TMaxLeading() ;

      int NPads         = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP < NPads ; iP++){                
        const Pad* pPad       = pCluster->Get_Pad(iP) ;
        double YTYpad = YT - pPad->Get_YPad()*100 ;
        int Tneigh      = pPad->Get_TMax() ;        
        int DeltaT      = Tneigh - TLeading ;
        tpDeltaT_YTYpad->Fill(YTYpad, DeltaT) ;
      }  
    }
  }
  return tpDeltaT_YTYpad ;
}



///////////////////////////////////////////////// TGraphErrors /////////////////////////////////////////////////////////////////////////



TGraphErrors* TH2_to_TGE(const TH2D* pTH2)
{
  TGraphErrors* ptge  = new TGraphErrors() ;
  for (int ix = 0 ; ix < pTH2->GetXaxis()->GetNbins(); ix++){
      double Amp_ix     = 0 ;
      double ywtemp     = 0 ;

    std::vector<int> v_Amp_iy ;
    for(int iy = 0 ; iy < pTH2->GetYaxis()->GetNbins(); iy++){
      ywtemp          += pTH2->GetBinContent(ix, iy)*pTH2->GetYaxis()->GetBinCenter(iy) ;
      Amp_ix          += pTH2->GetBinContent(ix, iy) ; 
      v_Amp_iy.push_back(pTH2->GetBinContent(ix, iy)) ;
    }
    double yw         = ywtemp/Amp_ix ;
    double x          = pTH2->GetXaxis()->GetBinCenter(ix);
    if(yw == 0 or std::isnan(yw)) continue ; // these values mean that the cluster is empty, notably in column 0

    // Error computation part
    float start       = -1.;
    float end         = -1.;
    float max         = *max_element(v_Amp_iy.begin(), v_Amp_iy.end()) ;

    for (int iy = 0 ; iy < pTH2->GetXaxis()->GetNbins() ; iy++)
    {
      if (start == -1. && pTH2->GetBinContent(ix, iy) >= max / 2.)
        start = pTH2->GetYaxis()->GetBinCenter(iy);

      if (end == -1. && start != -1. && pTH2->GetBinContent(ix, iy) <= max / 2.)
        end = pTH2->GetYaxis()->GetBinCenter(iy);
    }
    float e = end - start;

    ptge->SetPoint(ptge->GetN(), x, yw);
    ptge->SetPointError(ptge->GetN()-1, 0, e/2.); // N-1 because we just added a new point at the previous line
  } 
  return ptge ;
}
