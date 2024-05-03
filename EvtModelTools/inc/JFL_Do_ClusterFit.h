#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "Fitters/TheFitterCluster_PV0_PV1.h"
#include "Fitters/TheFitterCluster_PV0_Diag.h"

 
#include "TF1.h"
 
//-------------------------------------Cluster Fits-------------------------------/
//Cluster Fits
void Do_ClusterFit_PV0_PV1        (Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);
void Do_ClusterFit_PV0_PV1_Event  (Event*   pEvent  , const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1) ;
void Do_ClusterFit_PV0_PV1_Cluster(Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1) ;


void Do_ClusterFit_PV0_Diag        (const double& AngleRot, Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);
void Do_ClusterFit_PV0_Diag_Event  (const double& AngleRot, Event*   pEvent  , const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag) ;
void Do_ClusterFit_PV0_Diag_Cluster(const double& AngleRot, Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag) ;