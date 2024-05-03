#include "EvtModelTools/JFL_Selections.h"
#include "Misc/Util.h"

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

// All selections
void Selection_JFL_A(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Selection A: Start "   << std::endl ;

// Stage 1: Remove clusters in first and last columns 
  Selection_JFL_Stage1(aSample, ModuleNber ) ;

// Stage 2: Remove clusters with less than 2 pads 
  Selection_JFL_Stage2(aSample, ModuleNber ) ;

// Stage 3: Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis
  Selection_JFL_Stage3(aSample, ModuleNber ) ;

// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
  Selection_JFL_Stage4(aSample, ModuleNber ) ;

// Stage 5: Reject Events with less than 20 clusters
  Selection_JFL_Stage5(aSample, ModuleNber ) ;

  std::cout << "Selection A: End "   << std::endl ;
  
}

void Selection_JFL_B(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Selection B: Start "   << std::endl ;

// Stage 1: Remove clusters in first and last columns 
  Selection_JFL_Stage1(aSample, ModuleNber ) ;

// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad
  Selection_JFL_Stage6(aSample, ModuleNber ) ;
  
// Stage 5: Reject Events with less than 20 clusters
  Selection_JFL_Stage5(aSample, ModuleNber ) ;

  std::cout << "Selection B: End "   << std::endl ;
  
}

void Selection_JFL_BN(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Selection BN: Start "   << std::endl ;

// Stage 1: Remove clusters in first and last columns 
  Selection_JFL_Stage1(aSample, ModuleNber ) ;

// Stage 14: Remove Clusters with a too high APad 
  Selection_JFL_Stage14(aSample, ModuleNber );

// Stage 12: Remove clusters out of time (Run Based)
  Selection_JFL_Stage12(aSample, ModuleNber ) ;
  
// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
  Selection_JFL_Stage7(aSample, ModuleNber ) ;
  
// Stage 8: Remove Clusters with too high A3/A1
  Selection_JFL_Stage8(aSample, ModuleNber ) ;
  
// Stage 13: Remove Clusters with too small Tpad-TLeading 
  Selection_JFL_Stage13(aSample, ModuleNber ) ;
  
// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad
  Selection_JFL_Stage6(aSample, ModuleNber ) ;
  
// Stage 9: Remove clusters with less than 1pads 
  Selection_JFL_Stage9(aSample, ModuleNber ) ;
  
// Stage 10: Remove clusters with more than 6 pad 
  Selection_JFL_Stage10(aSample, ModuleNber ) ;
  
// Stage 11: Remove clusters out of time (Event Based)
  Selection_JFL_Stage11(aSample, ModuleNber ) ;
  
// Stage 5: Reject Events with less than 20 clusters
  Selection_JFL_Stage5(aSample, ModuleNber ) ;

  std::cout << "Selection BN: End "   << std::endl ;
  
}

void Selection_JFL_AN(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Selection AN: Start "   << std::endl ;

// Stage 1: Remove clusters in first and last columns 
  Selection_JFL_Stage1(aSample, ModuleNber ) ;

// Stage 14: Remove Clusters with a too high APad 
  Selection_JFL_Stage14(aSample, ModuleNber );

// Stage 12: Remove clusters out of time (Run Based)
  Selection_JFL_Stage12(aSample, ModuleNber ) ;
  
// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
  Selection_JFL_Stage7(aSample, ModuleNber ) ;
  
// Stage 8: Remove Clusters with too high A3/A1
  Selection_JFL_Stage8(aSample, ModuleNber ) ;
  
// Stage 13: Remove Clusters with too small Tpad-TLeading 
  Selection_JFL_Stage13(aSample, ModuleNber ) ;
  
// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad
  Selection_JFL_Stage6(aSample, ModuleNber ) ;
  
// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
  Selection_JFL_Stage4(aSample, ModuleNber ) ;

// Stage 9: Remove clusters with less than 1pads 
  Selection_JFL_Stage9(aSample, ModuleNber ) ;
  
// Stage 10: Remove clusters with more than 6 pad 
  Selection_JFL_Stage10(aSample, ModuleNber ) ;
  
// Stage 11: Remove clusters out of time (Event Based)
  Selection_JFL_Stage11(aSample, ModuleNber ) ;
  
// Stage 5: Reject Events with less than 20 clusters
  Selection_JFL_Stage5(aSample, ModuleNber ) ;

  std::cout << "Selection AN: End "   << std::endl ;
  
}

//----------------------------------------------------------------//
// Stage 1: Remove clusters in first and last columns 
void Selection_JFL_Stage1(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  1 -> Remove clusters in first and last columns " << std::endl ;
  
  Model_ReadOutGeometry* pModel_ReadOutGeometry = aSample.Get_Model_ReadOutGeometry() ;
  int iXFirst = 0 ;
  int iXLast  = pModel_ReadOutGeometry->Get_Nx() - 1 ;
  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iXCluster = (pCluster->Get_LeadingPad())->Get_iX() ;
      
      if (iXCluster == iXFirst || iXCluster == iXLast) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

  }

}


// Stage 2: Remove clusters with less than 2 pads 
void Selection_JFL_Stage2(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  2 -> Remove clusters with less than 2 pads " << std::endl ;
  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      if(NPads<=2) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}


// Stage 3: Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis
void Selection_JFL_Stage3(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  3 -> Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis" << std::endl ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iYCluster = (pCluster->Get_LeadingPad())->Get_iY() ;
      
      int HasP = 0 ;
      int HasM = 0 ;
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP <NPads ; iP++){ 
        const Pad* pPad = pCluster->Get_Pad(iP);  
        int iYPad = pPad->Get_iY() ;
        if ( (iYPad -iYCluster) == 1 ) HasP = 1 ;
        if ( (iYPad -iYCluster) ==-1 ) HasM = 1 ;
      }
      if ( HasP==0 || HasM==0 ) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
void Selection_JFL_Stage4(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  4 -> Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis" << std::endl ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iYCluster = (pCluster->Get_LeadingPad())->Get_iY() ;

      Cluster* pCluster_new = new Cluster(
                                          pCluster->Get_EventNber () ,
                                          pCluster->Get_EntryNber () ,
                                          pCluster->Get_ModuleNber() 
                                         ) ;
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP <NPads ; iP++){ 
        const Pad* pPad = pCluster->Get_Pad(iP);  
        int iYPad = pPad->Get_iY() ;
        if ( std::fabs(iYPad -iYCluster) <= 1 ) pCluster_new->Add_Pad(pPad);
      }

      pCluster_new->DoClosure();
      V_pCluster.push_back(pCluster_new) ;

    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

  }

}

// Stage 5: Reject Events with less than 20 clusters
void Selection_JFL_Stage5(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  5 -> Reject Events with less than 20 clusters" << std::endl ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    if(NClusters < 20)pEvent->Invalidate_ThisModule(ModuleNber) ; 

  }

}

// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad
void Selection_JFL_Stage6(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  6 -> Redo cluster keeping only the pads properly ordered wrt Leading pad" << std::endl ;

  double Cut_Stage6_ARelat_Low    =   0.02 ;
  std::cout << "                   -> Cut_Stage6_ARelat_Low = " << Cut_Stage6_ARelat_Low << std::endl ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iYCluster = (pCluster->Get_LeadingPad())->Get_iY() ;
      
      std::vector < RankedValue > V_RankedValue ;      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad <NPads ; iPad++){ 
        const Pad* pPad = pCluster->Get_Pad(iPad);  
 
        int iYPad = pPad->Get_iY() ;
        
        RankedValue aRankedValue ; 
        aRankedValue.Value = double(iYPad) ; 
        aRankedValue.Rank  = iPad            ; 
        V_RankedValue.push_back( aRankedValue );
      
      }
      
      std::sort(V_RankedValue.begin(), V_RankedValue.end());

      int iPad_Leading = 0 ;
      std::vector < const Pad* > V_SetOfPads_A ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad*   pPad = pCluster->Get_Pad( (V_RankedValue[iPad].Rank) ) ;
        V_SetOfPads_A.push_back(pPad); 
        if ( (V_RankedValue[iPad].Value) == double (iYCluster) ) iPad_Leading = iPad ;
      }

      std::vector < const Pad* > V_SetOfPads_B ;
      V_SetOfPads_B.push_back( pCluster->Get_LeadingPad() );
      
      int iPad_Previous = iPad_Leading ;
      for (int iPad = iPad_Leading+1 ; iPad< NPads; iPad++){
        const Pad* pPad          = V_SetOfPads_A[iPad]          ;
        const Pad* pPad_Previous = V_SetOfPads_A[iPad_Previous] ;
        
        double Amax            = pPad         ->Get_AMax() ;
        double Amax_Previous   = pPad_Previous->Get_AMax() ;
        
        int    iYPad           = pPad         ->Get_iY()   ;
        int    iYPad_Previous  = pPad_Previous->Get_iY()   ;
        
        if ( iYPad == (iYPad_Previous+1) && Amax < Amax_Previous ){
          V_SetOfPads_B.push_back(pPad) ;
          iPad_Previous = iPad ;
        }else{
          break ;
        }
        
      }  
      
      iPad_Previous = iPad_Leading ;
      for (int iPad = iPad_Leading - 1; iPad >= 0; iPad--){  
        const Pad* pPad          = V_SetOfPads_A[iPad]          ;
        const Pad* pPad_Previous = V_SetOfPads_A[iPad_Previous] ;
        
        double Amax            = pPad         ->Get_AMax() ;
        double Amax_Previous   = pPad_Previous->Get_AMax() ;
        
        int    iYPad           = pPad         ->Get_iY()   ;
        int    iYPad_Previous  = pPad_Previous->Get_iY()   ;
        
        if ( iYPad == (iYPad_Previous-1) && Amax < Amax_Previous ){
          V_SetOfPads_B.push_back(pPad) ;
          iPad_Previous = iPad ;
        }else{
          break ;
        }
        
      }    

      Cluster* pCluster_new = new Cluster(                                          
                                          pCluster->Get_EventNber () ,
                                          pCluster->Get_EntryNber () ,
                                          pCluster->Get_ModuleNber() 
                                         ) ;
      
      int NPads_V_new = V_SetOfPads_B.size() ;
      for (int iPad = 0 ; iPad< NPads_V_new; iPad++){
        const Pad* pPad = V_SetOfPads_B[iPad]; 
        pCluster_new->Add_Pad(pPad); 
      }
      
      pCluster_new->DoClosure();
      V_pCluster.push_back(pCluster_new) ;

    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

  }

//
  for (int iE = 0 ; iE < NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      const Pad* pPad_Leading = pCluster->Get_LeadingPad();
      double ALeading = pCluster->Get_AMaxLeading() ;

      Cluster* pCluster_new = new Cluster(                                          
                                          pCluster->Get_EventNber () ,
                                          pCluster->Get_EntryNber () ,
                                          pCluster->Get_ModuleNber() 
                                         ) ;

      pCluster_new->Add_Pad(pPad_Leading); 
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP< NPads; iP++){
        const Pad* pPad = pCluster->Get_Pad(iP);
        
        if (pPad!=pPad_Leading){
          double Amax   = pPad->Get_AMax() ;
          double Ratio = Amax/ALeading  ;
          if (Ratio >= Cut_Stage6_ARelat_Low) pCluster_new->Add_Pad(pPad); 
        }
      }
      pCluster_new->DoClosure();
      V_pCluster.push_back(pCluster_new) ;

    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
  }

}

// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
void Selection_JFL_Stage7(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  7 -> Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2" << std::endl ;

  double Cut_Stage7_T2T1_Low    =   -3. ;
  double Cut_Stage7_T2T1_Hig    =   30. ;
  double Cut_Stage7_T3T1_Low    =   -3. ;
  double Cut_Stage7_T3T1_Hig    =  100. ;
  double Cut_Stage7_T3T2_Low    =   -3. ;
  double Cut_Stage7_T3T2_Hig    =  100. ;
  std::cout << "                   -> Cut_Stage7_T2T1_Low = " << Cut_Stage7_T2T1_Low << std::endl ;
  std::cout << "                   -> Cut_Stage7_T2T1_Hig = " << Cut_Stage7_T2T1_Hig << std::endl ;
  std::cout << "                   -> Cut_Stage7_T3T1_Low = " << Cut_Stage7_T3T1_Low << std::endl ;
  std::cout << "                   -> Cut_Stage7_T3T1_Hig = " << Cut_Stage7_T3T1_Hig << std::endl ;
  std::cout << "                   -> Cut_Stage7_T3T2_Low = " << Cut_Stage7_T3T2_Low << std::endl ;
  std::cout << "                   -> Cut_Stage7_T3T2_Hig = " << Cut_Stage7_T3T2_Hig << std::endl ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
    std::vector<Cluster*> V_pCluster ;
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;
      double T1 = pPad_Leading->Get_TMax() ;

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
      
      int RemoveCluster = 0 ;
      if (AU_Found==1 && AD_Found==1 ){
        double T2 = TU ;
        double T3 = TD ;
        if (AU < AD ){
          T2 = TD ;
          T3 = TU ;
        }
        if (  T2     >  Cut_Stage7_T2T1_Hig ||  T2     < Cut_Stage7_T2T1_Low ) RemoveCluster = 1 ;
        if (  T3     >  Cut_Stage7_T3T1_Hig ||  T3     < Cut_Stage7_T3T1_Low ) RemoveCluster = 1 ;
        if ( (T3-T2) >  Cut_Stage7_T3T2_Hig || (T3-T2) < Cut_Stage7_T3T2_Low ) RemoveCluster = 1 ;
      }else{
        if (AU_Found==1 || AD_Found==1 ){
          double T2 = TU ;
          if (AD_Found==1) T2 = TD ;
          if ( T2 >  30 || T2 < -3 ) RemoveCluster = 1 ;
        }
      }
      if (RemoveCluster==1) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
              
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 8: Remove Clusters with too high A3/A1
void Selection_JFL_Stage8(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  8 -> Remove Clusters with too high A3/A1" << std::endl ;

  double Cut_Stage8_A3onA1      =   0.3 ;
  std::cout << "                   -> Cut_Stage8_A3onA1 = " << Cut_Stage8_A3onA1 << std::endl ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
    std::vector<Cluster*> V_pCluster ;
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
      int iY_Leading = pPad_Leading->Get_iY();
      double A1 = pPad_Leading->Get_AMax() ;

      double AU = 0. ; int AU_Found = 0 ;
      double AD = 0. ; int AD_Found = 0 ;
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);
       
        int iY = pPad->Get_iY();
        if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
        if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
        if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
        if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
        
      }
      
      int RemoveCluster = 0 ;
      if (AU_Found==1 && AD_Found==1 ){
        double A3 = std::min(AU, AD) ;
        if ( A3 >  Cut_Stage8_A3onA1 ) RemoveCluster = 1 ;
      }
      if (RemoveCluster==1) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
              
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 9: Remove clusters with less than 1pads 
void Selection_JFL_Stage9(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection  9 -> Remove clusters with less than 1 pads " << std::endl ;
  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      if(NPads<=1) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 10: Remove clusters with more than 6 pad 
void Selection_JFL_Stage10(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection 10 -> Remove clusters with more than 6 pad " << std::endl ;
  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int NPads = pCluster->Get_NberOfPads() ;
      if(NPads>=6) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 11: Remove clusters out of time (Event Based)
void Selection_JFL_Stage11(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection 11 -> Remove clusters out of time (Event Based) " << std::endl ;
  
  double Cut_Stage11_EventBased =    3. ;
  std::cout << "                   -> Cut_Stage11_EventBased = " << Cut_Stage11_EventBased << std::endl ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber)==0) continue ;
    std::vector<Cluster*> V_pCluster ;

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
      if(std::fabs(DeltaT) > Cut_Stage11_EventBased) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
  }

}

// Stage 12: Remove clusters out of time (Run Based)
void Selection_JFL_Stage12(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection 12 -> Remove clusters out of time (Run Based)" << std::endl ;
  
  double Cut_Stage12_RunBased   =   10. ;
  std::cout << "                   -> Cut_Stage12_RunBased = " << Cut_Stage12_RunBased << std::endl ;
 
  TH1I* pTH1I = new TH1I("BIDON","BIDON", 510, -0.5, 509.5 ) ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int TLeading = pCluster->Get_TMaxLeading() ;
      pTH1I->Fill(TLeading) ;
    }
  }

  int TLeading_Run = pTH1I->GetMaximumBin() ;
  delete pTH1I ;
  
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int Delta_TLeading = pCluster->Get_TMaxLeading() - TLeading_Run ;
      if(std::fabs(Delta_TLeading) > Cut_Stage12_RunBased) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
  }

}

// Stage 13: Remove Clusters with too small Tpad-TLeading 
void Selection_JFL_Stage13(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection 13 -> Remove Clusters with too small Tpad-TLeading " << std::endl ;

  double Cut_Stage13_DT_Low     =   -3. ;
  std::cout << "                   -> Cut_Stage13_DT_Low = " << Cut_Stage13_DT_Low << std::endl ;

  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
    std::vector<Cluster*> V_pCluster ;
     
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      double TLeading = pCluster->Get_TMaxLeading() ;
      
      int RemoveCluster = 0 ;
      
      int NPads = pCluster->Get_NberOfPads() ;
      for (int iPad = 0 ; iPad< NPads; iPad++){
        const Pad* pPad = pCluster->Get_Pad(iPad);

        double DeltaT = pPad->Get_TMax()-TLeading ;
        if (DeltaT<Cut_Stage13_DT_Low) RemoveCluster = 1 ;
        
        if (RemoveCluster==1) break ;
        
      }
      
      if (RemoveCluster==1) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
              
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

// Stage 14: Remove Clusters with a too high APad 
void Selection_JFL_Stage14(Sample& aSample, const int& ModuleNber)
{
  std::cout << "Apply selection 14 -> Remove Clusters with a too high APad " << std::endl ;
  
  double Cut_Stage14_Amax_Hig   = 3700. ;
  std::cout << "                   -> Cut_Stage14_Amax_Hig = " << Cut_Stage14_Amax_Hig << std::endl ;

  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];

      int RemoveCluster = 0 ;

      int NPads = pCluster->Get_NberOfPads() ;
      for (int iP = 0 ; iP <NPads ; iP++){ 
        const Pad* pPad = pCluster->Get_Pad(iP);  

        double Amax = pPad ->Get_AMax() ;
        if (Amax > Cut_Stage14_Amax_Hig) RemoveCluster = 1 ;
        if (RemoveCluster==1) break ;
      
      }
      if (RemoveCluster==1) continue ;
      
      Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluser_new) ;
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  }
  
}

//---------------------------------------------------//
// Eliminate cluster with NAN YT
void Cleaning_JFL_01(Sample& aSample, const int& ModuleNber)
{
  int ElminatedKounter = 0 ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   
    
    int ThereAreProblematicClusters = 0 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
     
      double YT = pCluster->Get_YTrack() ;
      if (YT!=YT) ThereAreProblematicClusters = 1 ;
//       if (YT!=YT){
//         std::cout 
//           << " Cluster "
//           << " iE = " << iE 
//           << " iC = " << iC 
//           << " iY = " << (pCluster->Get_LeadingPad())->Get_iY()  
//           << " : YT is a NAN "
//           << std::endl ;
//       }
      if (ThereAreProblematicClusters==1) break ;
      
    }
    
    if (ThereAreProblematicClusters==1){
      std::vector<Cluster*> V_pCluster ;

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];
        
        double YT = pCluster->Get_YTrack() ;
        if (YT!=YT){
          ElminatedKounter += 1 ;
        }
//         if (YT!=YT){
//           std::cout 
//             << " Cluster "
//             << " iE = " << iE 
//             << " iC = " << iC 
//             << " iY = " << (pCluster->Get_LeadingPad())->Get_iY()  
//             << " : ELIMINATED "
//             << std::endl ;
//         }
        if (YT!=YT) continue ;
        
        Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
        V_pCluster.push_back(pCluser_new) ;
        
      }
      pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
      
    }
    
  }
  
  if (ElminatedKounter!=0){
    std::cout 
      << " Cleaning_JFL_01: nber of eleminated Clusters: "
      << ElminatedKounter
      << std::endl ;
  }

}

// Eliminate cluster failling fit
void Cleaning_JFL_02(Sample& aSample, const int& ModuleNber)
{
  int ElminatedKounter = 0 ;
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;   
    
    int ThereAreProblematicClusters = 0 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
     
      int TheStatus = pCluster->StatusFit() ;
      if (TheStatus!=0) ThereAreProblematicClusters = 1 ;
//       if (TheStatus!=0){
//         std::cout 
//           << " Cluster "
//           << " iE = " << iE 
//           << " iC = " << iC 
//           << " iY = " << (pCluster->Get_LeadingPad())->Get_iY()  
//           << " : this cluster failled "
//           << std::endl ;
//       }
      if (ThereAreProblematicClusters==1) break ;

    }
    
    if (ThereAreProblematicClusters==1){
      std::vector<Cluster*> V_pCluster ;

      std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
      int NClusters = ClusterSet.size() ;
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = ClusterSet[iC];
        
        int TheStatus = pCluster->StatusFit() ;
        if (TheStatus!=0){
          ElminatedKounter += 1 ;
        }
//        if (TheStatus!=0){
//           std::cout 
//             << " Cluster "
//             << " iE = " << iE 
//             << " iC = " << iC 
//             << " iY = " << (pCluster->Get_LeadingPad())->Get_iY()  
//             << " : ELIMINATED "
//             << std::endl ;
//        }
        if (TheStatus!=0) continue ;
        
        Cluster* pCluser_new = pEvent->Get_Cluster_Copy(pCluster) ;
        V_pCluster.push_back(pCluser_new) ;
        
      }
      if (V_pCluster.size()==0){
        std::cout 
          << " Cleaning_JFL_02 "
          << " V_pCluster.size()==0 "
          << std::endl ;
      }
      pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
      
    }
    
  }
  
  if (ElminatedKounter!=0){
    std::cout 
      << " Cleaning_JFL_02: nber of eleminated Clusters: "
      << ElminatedKounter
      << std::endl ;
  }

}

