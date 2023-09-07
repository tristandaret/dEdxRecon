#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "Fitters_Cluster/TheFitterCluster_PV0_PV1.h"
#include "Fitters_Cluster/TheFitterCluster_PV2.h"
#include "Fitters_Cluster/TheFitterCluster_PV3.h"
#include "Fitters_Cluster/TheFitterCluster_PV31.h"
#include "Fitters_Cluster/TheFitterCluster_PV4.h"
#include "Fitters_Cluster/TheFitterCluster_PV0_Diag.h"

 
#include "TF1.h"
#include "SignalShape/SC_VS_x_rc_dd_SET_rc_dd.h"
 
//-------------------------------------Cluster Fits-------------------------------/
//Cluster Fits
void Do_ClusterFit_PV0_PV1        (Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);
void Do_ClusterFit_PV0_PV1_Event  (Event*   pEvent  , const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1) ;
void Do_ClusterFit_PV0_PV1_Cluster(Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_PV1& aTheFitterCluster_PV0_PV1) ;

//OBSOLETE: JFL 10/12/22
void Do_ClusterFit_PV2         (Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);

void Do_ClusterFit_PV2         (Sample&  aSample , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd);
void Do_ClusterFit_PV2_Event   (Event*   pEvent  , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV2& aTheFitterCluster_PV2) ;
void Do_ClusterFit_PV2_Cluster (Cluster* pCluster, const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV2& aTheFitterCluster_PV2) ;

void Do_ClusterFit_PV3         (Sample&  aSample , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd);
void Do_ClusterFit_PV3_Event   (Event*   pEvent  , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV3& aTheFitterCluster_PV3) ;
void Do_ClusterFit_PV3_Cluster (Cluster* pCluster, const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV3& aTheFitterCluster_PV3) ;

void Do_ClusterFit_PV31         (Sample&  aSample , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd);
void Do_ClusterFit_PV31_Event   (Event*   pEvent  , const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV31& aTheFitterCluster_PV31) ;
void Do_ClusterFit_PV31_Cluster (Cluster* pCluster, const int& ModuleNber, SC_VS_x_rc_dd_SET_rc_dd* pSC_VS_x_rc_dd_SET_rc_dd, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV31& aTheFitterCluster_PV31) ;

void Do_ClusterFit_PV4         (Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);
void Do_ClusterFit_PV4_Event   (Event*   pEvent  , const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV4& aTheFitterCluster_PV4);
void Do_ClusterFit_PV4_Cluster (Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV4& aTheFitterCluster_PV4);

void Do_ClusterFit_PV0_Diag        (const double& AngleRot, Sample&  aSample , const int& ModuleNber, TF1* pTF1_ToBeUsed);
void Do_ClusterFit_PV0_Diag_Event  (const double& AngleRot, Event*   pEvent  , const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag) ;
void Do_ClusterFit_PV0_Diag_Cluster(const double& AngleRot, Cluster* pCluster, const int& ModuleNber, TF1* pTF1_ToBeUsed, int& Kounter_Fit, int& Kounter_Failure, TheFitterCluster_PV0_Diag& aTheFitterCluster_PV0_Diag) ;