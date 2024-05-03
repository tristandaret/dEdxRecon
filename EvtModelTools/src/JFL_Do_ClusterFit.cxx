#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "Misc/Util.h"


//-------------------------------------Cluster Fits-------------------------------/

//-------------------------------------PV0_PV1-------------------------------/
void Do_ClusterFit_PV0_PV1(Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed)
{
//Set the fitter
  TheFitterCluster_PV0_PV1 aTheFitterCluster_PV0_PV1("Minuit") ;

//Do the fits
  int Kounter_Fit     = 0 ; 
  int Kounter_Failure = 0 ; 
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    Do_ClusterFit_PV0_PV1_Event(pEvent,ModuleNber,pTF1_ToBeUsed, Kounter_Fit, Kounter_Failure, aTheFitterCluster_PV0_PV1);
  }
  std::cout << std::endl ;
  std::cout << " Do_ClusterFit_PV0_PV1 " << std::endl ;
  std::cout << "   Nber of Fits        " << std::setw(20) << Kounter_Fit     << std::endl ;
  std::cout << "   Nber of Failed Fits " << std::setw(20) << Kounter_Failure 
     << " ( " 
     << std::setw(10) << std::setprecision(4) 
     << 100.* double (Kounter_Failure)/double (Kounter_Fit) 
     << " % ) "
     << std::endl ;

}

void Do_ClusterFit_PV0_PV1_Event(Event* pEvent, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1)
{
  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    
    Do_ClusterFit_PV0_PV1_Cluster(pCluster,ModuleNber,pTF1_ToBeUsed,Kounter_Fit,Kounter_Failure,aTheFitterCluster_PV0_PV1);
  }
}

void Do_ClusterFit_PV0_PV1_Cluster(Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure,TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1)
{
//Tell to the cluster which shape function it should use
  pCluster->SetEval_PV0_PV1( pTF1_ToBeUsed ) ;
  
//Tell to the fitter to fit the cluster position
  aTheFitterCluster_PV0_PV1.Set_Cluster(pCluster) ;
  int FitResult = aTheFitterCluster_PV0_PV1.DoMinimisation() ;
  Kounter_Fit += 1 ;
  if (FitResult!=0) Kounter_Failure += 1 ;
}

//-------------------------------------PV0_Diag-------------------------------/
void Do_ClusterFit_PV0_Diag(const double& AngleRot, Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed)
{
//Set the fitter
  TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit") ;

//Do the fits
  int Kounter_Fit     = 0 ; 
  int Kounter_Failure = 0 ; 
  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;  
     
    Do_ClusterFit_PV0_Diag_Event(AngleRot, pEvent,ModuleNber,pTF1_ToBeUsed, Kounter_Fit, Kounter_Failure, aTheFitterCluster_PV0_Diag);
  }
  std::cout << std::endl ;
  std::cout << " Do_ClusterFit_PV0_Diag " << std::endl ;
  std::cout << "   Nber of Fits        " << std::setw(20) << Kounter_Fit     << std::endl ;
  std::cout << "   Nber of Failed Fits " << std::setw(20) << Kounter_Failure 
     << " ( " 
     << std::setw(10) << std::setprecision(4) 
     << 100.* double (Kounter_Failure)/double (Kounter_Fit) 
     << " % ) "
     << std::endl ;

}

void Do_ClusterFit_PV0_Diag_Event(const double& AngleRot, Event* pEvent, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag)
{
  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    
    Do_ClusterFit_PV0_Diag_Cluster(AngleRot, pCluster,ModuleNber,pTF1_ToBeUsed,Kounter_Fit,Kounter_Failure,aTheFitterCluster_PV0_Diag);
  }
}

void Do_ClusterFit_PV0_Diag_Cluster(const double& AngleRot, Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure,TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag)
{
//Tell to the cluster which shape function it should use
  pCluster->SetEval_PV0_Diag( pTF1_ToBeUsed ) ;

//Tell to the cluster which angle should be used
  pCluster->m_AngleRot = AngleRot  ;
  
//Tell to the fitter to fit the cluster position
  aTheFitterCluster_PV0_Diag.Set_Cluster(pCluster) ;
  int FitResult = aTheFitterCluster_PV0_Diag.DoMinimisation() ;
  Kounter_Fit += 1 ;
  if (FitResult!=0) Kounter_Failure += 1 ;
}

