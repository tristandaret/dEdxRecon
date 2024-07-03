#include "Selector.h"

#include "Event.h"
#include "Pad.h"

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


Selector::Selector(const std::string DefSelection
){
	ListOfSelectionName.clear();
	Reset_StatCounters();
	
	Set_Cuts();
	
	if (DefSelection=="Sel_DESY21"	) { Set_DESY21_Event(); }
	if (DefSelection=="Sel_DESY21_theta"	) { Set_DESY21theta_Event(); }
	if (DefSelection=="Sel_CERN22"	) { Set_T2_CERN22_Event(); }
	if (DefSelection=="TMC_CERN22_Event"	) { Set_TMC_CERN22_Event(); }

//
	if (
		DefSelection!="Sel_DESY21"	
	&&DefSelection!="Sel_DESY21_theta"	
	&&DefSelection!="Sel_CERN22" 
	&&DefSelection!="TMC_CERN22_Event"
	) { 
	std::cout 
		<< "Selector::Selector "
		<< " unknown predefined selection " << DefSelection
		<< std::endl;
	
	abort();
	}

}	

Selector::Selector()
{
	ListOfSelectionName.clear();
	Reset_StatCounters();

//
	Set_Cuts();
	
}	

Selector::~Selector()
{
	ListOfSelectionName.clear();
	ListOfNberOfEvents_Before	.clear();
	ListOfNberOfEvents_After	.clear();	
	ListOfNberOfModules_Before .clear();	
	ListOfNberOfModules_After	.clear();	
	ListOfNberOfClusters_Before.clear();	
	ListOfNberOfClusters_After .clear();	
	ListOfNberOfPads_Before	.clear();	
	ListOfNberOfPads_After	.clear();	
}

void Selector::Set_Cuts()
{
	m_Cut_StageFinal_NCluster_Low	= 36;

	m_Cut_Stage5_Npads_Hig	=		5;
	
	m_Cut_Stage2_EventBased	=		3;

	m_Cut_Stage3_TLow	=				160.;
	m_Cut_Stage3_THigh	=			220.;

	m_Cut_Stage6_Amax_Low	= 		0.;
	m_Cut_Stage6_Amax_Hig	= 		3700.;

	m_Cut_Stage4_APM_Low	= 			2;
	m_Cut_Stage4_APM_High	= 		4;

}

//---------------------------------------------------------------------//


void Selector::Set_DESY21_Event()
{
	Reset_Selection();

	std::cout << std::endl;
	std::cout << "-------------->Predefined selection Sel_DESY21" << std::endl;
	ListOfSelectionName.clear();
//	Add_Selection( "Stage1"	);	// border clusters
	Add_Selection( "Stage2"	);	// time sync event based
	Add_Selection( "Stage3" );	// time sync run based
	Add_Selection( "Stage4"	);	// average pad multiplicity
	Add_Selection( "Stage5"	);	// maximum number of pads
	Add_Selection( "Stage6"	);	// min & max amplitude
	Add_Selection( "StageFinal"	);	// number of clusters
}


void Selector::Set_DESY21theta_Event()
{
	Reset_Selection();

	std::cout << std::endl;
	std::cout << "-------------->Predefined selection Sel_DESY21_theta" << std::endl;
	ListOfSelectionName.clear();
//	Add_Selection( "Stage1"	);	// border clusters
	Add_Selection( "Stage4"	);	// average pad multiplicity
	Add_Selection( "Stage5"	);	// maximum number of pads
	Add_Selection( "Stage6"	);	// min & max amplitude
	Add_Selection( "StageFinal"	);	// number of clusters
}

void Selector::Set_T2_CERN22_Event()
{
	Reset_Selection();

	std::cout << std::endl;
	std::cout << "-------------->Predefined selection Sel_CERN22" << std::endl;
	ListOfSelectionName.clear();
//	Add_Selection( "Stage1"	);	// border clusters
	Add_Selection( "Stage2"	);	// time sync event based
	Add_Selection( "Stage3" );	// time sync run based
	Add_Selection( "Stage4"	);	// average pad multiplicity in cluster
	Add_Selection( "StageFinal"	);	// number of clusters
}

void Selector::Set_TMC_CERN22_Event()
{
	Reset_Selection();

	std::cout << std::endl;
	std::cout << "-------------->Predefined selection TMC_CERN22_Event" << std::endl;
	ListOfSelectionName.clear();
	//
//	Add_Selection( "Stage1"	);	// border clusters
	Add_Selection( "StageFinal"	);	// number of clusters
}

//---------------------------------------------------------------------//
void Selector::Tell_Selection()
{
	std::cout << std::endl;
	std::cout << "-------------->List of selections" << std::endl;
	int iTem_Max	= NberOfSelections();
	for (int iTem	= 0; iTem< iTem_Max; iTem++){
	if ( ListOfSelectionName[iTem]=="Stage1"	) { Stage1_Def(); std::cout << std::endl; } 
	if ( ListOfSelectionName[iTem]=="StageFinal"	) { StageFinal_Def(); std::cout << std::endl; }	
	if ( ListOfSelectionName[iTem]=="Stage5"	) { Stage5_Def(); std::cout << std::endl; }	
	if ( ListOfSelectionName[iTem]=="Stage2"	) { Stage2_Def(); std::cout << std::endl; }	
	if ( ListOfSelectionName[iTem]=="Stage3" ) { Stage3_Def(); std::cout << std::endl; }	
	if ( ListOfSelectionName[iTem]=="Stage6"	) { Stage6_Def(); std::cout << std::endl; }	
	if ( ListOfSelectionName[iTem]=="Stage4"	) { Stage4_Def(); std::cout << std::endl; }
	}
	std::cout << std::endl;

}

//---------------------------------------------------------------------//
void Selector::Reset_StatCounters()
{
	ListOfNberOfEvents_Before	.clear();
	ListOfNberOfEvents_After	.clear();	
	ListOfNberOfModules_Before .clear();	
	ListOfNberOfModules_After	.clear();	
	ListOfNberOfClusters_Before.clear();	
	ListOfNberOfClusters_After .clear();	
	ListOfNberOfPads_Before	.clear();	
	ListOfNberOfPads_After	.clear();	
	int iTem_Max	= NberOfSelections();
	for (int iTem	= 0; iTem< iTem_Max; iTem++){
	ListOfNberOfEvents_Before	.push_back( 0);
	ListOfNberOfEvents_After	.push_back( 0);	
	ListOfNberOfModules_Before .push_back( 0);	
	ListOfNberOfModules_After	.push_back( 0);	
	ListOfNberOfClusters_Before.push_back( 0);	
	ListOfNberOfClusters_After .push_back( 0);	
	ListOfNberOfPads_Before	.push_back( 0);	
	ListOfNberOfPads_After	.push_back( 0);	
	}
}

void Selector::Reset_Selection()
{
	ListOfSelectionName.clear();
	Reset_StatCounters();
}


int Selector::NberOfSelections()
{
	return ListOfSelectionName.size();
}

std::string Selector::Get_SelectionName(const int& iTem)
{
	return ListOfSelectionName[iTem];
}

void Selector::Add_Selection(const std::string& SelectionName)
{
	int IsOK	= 0;
	if ( SelectionName=="Stage1"	) { IsOK	= 1; } 
	if ( SelectionName=="Stage2"	) { IsOK	= 1; } 
	if ( SelectionName=="Stage3"	) { IsOK	= 1; } 
	if ( SelectionName=="Stage4"	) { IsOK	= 1; } 
	if ( SelectionName=="Stage5"	) { IsOK	= 1; } 
	if ( SelectionName=="Stage6"	) { IsOK	= 1; } 
	if ( SelectionName=="StageFinal"){ IsOK	= 1; } 
	
	if (IsOK==0){
	std::cout 
		<< "Selector::Add_Selection "
		<< " unknown selection " << SelectionName
		<< std::endl;
	abort();
	}

	ListOfSelectionName.push_back(SelectionName);
}

void Selector::ApplySelection(Sample& aSample, const int& ModuleNber)
{
	std::cout << std::endl;
	std::cout << "-------------->Apply selections " << std::endl;
	
	Reset_StatCounters();
	
	int iTem_Max	= NberOfSelections();
	for (int iTem	= 0; iTem< iTem_Max; iTem++){	
	Apply_ASelection(aSample,ModuleNber, iTem);
	}
	std::cout << std::endl;
	
}

void Selector::Apply_ASelection(Sample& aSample, const int& ModuleNber,const int& iTem)
{
	std::string SelectionName	= ListOfSelectionName[iTem];

	SetStat_Before(aSample, ModuleNber ,iTem);
	
	if ( SelectionName=="Stage1"	) { Stage1	(aSample, ModuleNber); } 
	if ( SelectionName=="StageFinal"	) { StageFinal	(aSample, ModuleNber); } 
	if ( SelectionName=="Stage5"	) { Stage5 (aSample, ModuleNber); } 
	if ( SelectionName=="Stage2"	) { Stage2 (aSample, ModuleNber); } 
	if ( SelectionName=="Stage3" ) { Stage3(aSample, ModuleNber); } 
	if ( SelectionName=="Stage6"	) { Stage6 (aSample, ModuleNber); } 
	if ( SelectionName=="Stage4"	) { Stage4 (aSample, ModuleNber); }

	SetStat_After(aSample, ModuleNber ,iTem);
}

void Selector::SetStat_Before(Sample& aSample, const int& ModuleNber,const int& iTem)
{
	int NberOfEvents	= aSample.Get_NberOfEvents();
	int NberOfValidEvents	= 0;
	int NberOfClusters	= 0;
	int NberOfPads		= 0;
	for (int iE	= 0; iE< NberOfEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) continue; 
	NberOfValidEvents += 1;
	
	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	
	NberOfClusters += NClusters;
	
	for (int iC	= 0; iC< NClusters; iC++){
		Cluster* pCluster	= ClusterSet[iC];
		
		NberOfPads += pCluster->Get_NberOfPads();
	}
	}
	
	ListOfNberOfEvents_Before	[iTem]	= NberOfValidEvents;
	ListOfNberOfModules_Before [iTem]	= NberOfValidEvents;
	ListOfNberOfClusters_Before[iTem]	= NberOfClusters;
	ListOfNberOfPads_Before	[iTem]	= NberOfPads	;
}

void Selector::SetStat_After(Sample& aSample, const int& ModuleNber,const int& iTem)
{
	int NberOfEvents	= aSample.Get_NberOfEvents();
	int NberOfValidEvents	= 0;
	int NberOfClusters	= 0;
	int NberOfPads		= 0;
	for (int iE	= 0; iE< NberOfEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	NberOfValidEvents += 1;
	
	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	
	NberOfClusters += NClusters;
	
	for (int iC	= 0; iC< NClusters; iC++){
		Cluster* pCluster	= ClusterSet[iC];
		
		NberOfPads += pCluster->Get_NberOfPads();
	}
	}
	
	ListOfNberOfEvents_After	[iTem]	= NberOfValidEvents;
	ListOfNberOfModules_After [iTem]	= NberOfValidEvents;
	ListOfNberOfClusters_After[iTem]	= NberOfClusters;
	ListOfNberOfPads_After	[iTem]	= NberOfPads	;
}

void Selector::ApplySelection(Event*	pEvent)
{
	int NberOfModule	= pEvent->Get_NberOfModule();

	int iTem_Max	= NberOfSelections();
	for (int iTem	= 0; iTem< iTem_Max; iTem++){	
	
	SetStat_Before(pEvent , iTem);

	for (int iModule	= 0; iModule< NberOfModule; iModule++){	
		Module* pModule	= pEvent->Get_Module_InArray(iModule);
		int ModuleNber	= pModule->Get_ModuleNber();
		// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) continue;
		Apply_ASelection(pEvent,ModuleNber,iTem);
	}
	
	SetStat_After(pEvent , iTem);

	}

}

void Selector::Apply_ASelection(Event*	pEvent, const int& ModuleNber,const int& iTem)
{
	std::string SelectionName	= ListOfSelectionName[iTem];

	if ( SelectionName=="Stage1"	) { Stage1	(pEvent, ModuleNber); }
	if ( SelectionName=="StageFinal"	) { StageFinal	(pEvent, ModuleNber); } 
	if ( SelectionName=="Stage5"	) { Stage5 (pEvent, ModuleNber); } 
	if ( SelectionName=="Stage2"	) { Stage2 (pEvent, ModuleNber); } 
	if ( SelectionName=="Stage3" ) { Stage3(pEvent, ModuleNber); } 
	if ( SelectionName=="Stage6"	) { Stage6 (pEvent, ModuleNber); } 
	if ( SelectionName=="Stage4"	) { Stage4 (pEvent, ModuleNber); }
}

void Selector::SetStat_Before(Event*	pEvent , const int& iTem)
{
	if (pEvent->IsValid()	== 0) return;
	
	int NberOfModule	= pEvent->Get_NberOfModule();

	int NberOfModules	= 0;
	int NberOfClusters	= 0;
	int NberOfPads		= 0;
	for (int iModule	= 0; iModule< NberOfModule; iModule++){	
	Module* pModule	= pEvent->Get_Module_InArray(iModule);
	int ModuleNber	= pModule->Get_ModuleNber();
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) continue;
	
	NberOfModules +=1;
	
	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	
	NberOfClusters += NClusters;
	
	for (int iC	= 0; iC< NClusters; iC++){
		Cluster* pCluster	= ClusterSet[iC];
		
		NberOfPads += pCluster->Get_NberOfPads();
	}
	}

	ListOfNberOfEvents_Before	[iTem] += 1			;
	ListOfNberOfModules_Before [iTem] += NberOfModules;
	ListOfNberOfClusters_Before[iTem] += NberOfClusters;
	ListOfNberOfPads_Before	[iTem] += NberOfPads	;
}

void Selector::SetStat_After(Event*	pEvent , const int& iTem)
{
	if (pEvent->IsValid()	== 0) return;
	int NberOfModule	= pEvent->Get_NberOfModule();

	int NberOfModules	= 0;
	int NberOfClusters	= 0;
	int NberOfPads		= 0;
	for (int iModule	= 0; iModule< NberOfModule; iModule++){	
	Module* pModule	= pEvent->Get_Module_InArray(iModule);
	int ModuleNber	= pModule->Get_ModuleNber();
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) continue;
	
	NberOfModules +=1;

	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	
	NberOfClusters += NClusters;
	
	for (int iC	= 0; iC< NClusters; iC++){
		Cluster* pCluster	= ClusterSet[iC];
		
		NberOfPads += pCluster->Get_NberOfPads();
	}
	}

	ListOfNberOfEvents_After	[iTem] += 1			;
	ListOfNberOfModules_After [iTem] += NberOfModules;
	ListOfNberOfClusters_After[iTem] += NberOfClusters;
	ListOfNberOfPads_After	[iTem] += NberOfPads	;
}

void Selector::PrintStat()
{
 std::cout << std::endl;
 std::cout << "Selection Statistics " << std::endl;
	std::cout
	<< std::setw(5) << std::setprecision(0) << "#Cut"
	<< std::setw(8) << std::setprecision(0) << "SelName"
	<< std::setw(9) << std::setprecision(0) << "EvBef"	
	<< std::setw(9) << std::setprecision(0) << "EvAfter" << " (" << std::setw(8) <<	"wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"		<< ") "
	<< std::setw(9) << std::setprecision(0) << "MdBef"	
	<< std::setw(9) << std::setprecision(0) << "MdAfter" << " (" << std::setw(8) <<	"wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"		<< ") "
	<< std::setw(9) << std::setprecision(0) << "ClBef"	
	<< std::setw(9) << std::setprecision(0) << "ClAfter" << " (" << std::setw(8) <<	"wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"		<< ") "
	<< std::setw(9) << std::setprecision(0) << "PdBef"	
	<< std::setw(9) << std::setprecision(0) << "PdAfter" << " (" << std::setw(8) <<	"wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"		<< ") "
	<< std::endl;
 int iTem_Max	= NberOfSelections();
 for (int iTem	= 0; iTem< iTem_Max; iTem++){
	std::cout
	<< std::setw(5) << std::setprecision(0) << iTem
	<< std::setw(8) << std::setprecision(0) << ListOfSelectionName[iTem]
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_Before[iTem]	
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_After[iTem]	<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfEvents_After[iTem]-ListOfNberOfEvents_Before[iTem])/ListOfNberOfEvents_Before[iTem]		<< ") "<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfEvents_After[iTem]-ListOfNberOfEvents_Before[0])/ListOfNberOfEvents_Before[0]		<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfModules_Before[iTem] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfModules_After[iTem]	<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfModules_After[iTem]-ListOfNberOfModules_Before[iTem])/ListOfNberOfModules_Before[iTem]	<< ") "<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfModules_After[iTem]-ListOfNberOfModules_Before[0])/ListOfNberOfModules_Before[0]	<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_Before[iTem] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_After[iTem]	<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfClusters_After[iTem]-ListOfNberOfClusters_Before[iTem])/ListOfNberOfClusters_Before[iTem]<< ") "<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfClusters_After[iTem]-ListOfNberOfClusters_Before[0])/ListOfNberOfClusters_Before[0]<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfPads_Before[iTem] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfPads_After[iTem]		<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfPads_After[iTem]-ListOfNberOfPads_Before[iTem])/ListOfNberOfPads_Before[iTem]			<< ") "<< " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfPads_After[iTem]-ListOfNberOfPads_Before[0])/ListOfNberOfPads_Before[0]			<< ") "
	<< std::endl;
 }
 std::cout << std::setw(13) << std::setprecision(0) << "Summary Sel"	
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_Before[0]	
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_After[iTem_Max-1]	<< std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfEvents_After[iTem_Max-1]-ListOfNberOfEvents_Before[0])/ListOfNberOfEvents_Before[0]		<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfModules_Before[0] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfModules_After[iTem_Max-1]	<< std::setw(12)	<< " " << " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfModules_After[iTem_Max-1]-ListOfNberOfModules_Before[0])/ListOfNberOfModules_Before[0]	<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_Before[0] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_After[iTem_Max-1]	<< std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfClusters_After[iTem_Max-1]-ListOfNberOfClusters_Before[0])/ListOfNberOfClusters_Before[0]<< ") "
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfPads_Before[0] 
	<< std::setw(9) << std::setprecision(0) << ListOfNberOfPads_After[iTem_Max-1]		<< std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<	100.*(ListOfNberOfPads_After[iTem_Max-1]-ListOfNberOfPads_Before[0])/ListOfNberOfPads_Before[0]			<< ") "
	<< std::endl;
}

//----------------------------------------------------------------//
// Stage 1: Remove clusters in first and last columns 
void Selector::Stage1_Def()
{
	std::cout << "Apply selection	1 -> Remove clusters in first and last columns " << std::endl;
}
void Selector::Stage1(Sample& aSample, const int& ModuleNber)
{
	int NEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE< NEvents; iE++){
	Event* pEvent	= aSample.Get_Event(iE);
	Stage1(pEvent,ModuleNber);
	}

}
void Selector::Stage1(Event* pEvent, const int& ModuleNber)
{
	const Model_ReadOutGeometry* pModel_ReadOutGeometry	= pEvent->Get_Model_ReadOutGeometry();
	int iXFirst	= 0;
	int iXLast	= pModel_ReadOutGeometry->Get_Nx() - 1;

	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;
	std::vector<Cluster*> V_pCluster;

	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	for (int iC	= 0; iC< NClusters; iC++){
		Cluster* pCluster	= ClusterSet[iC];
		
		int iXCluster	= (pCluster->Get_LeadingPad())->Get_iX();
		
		if (iXCluster	== iXFirst || iXCluster	== iXLast) continue;
		
		Cluster* pCluster_new	= pEvent->Get_Cluster_Copy(pCluster);
		V_pCluster.push_back(pCluster_new);
		
	}
	pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber);
}

//----------------------------------------------------------------//
// Stage 2: Remove clusters out of time (Event Based)
double Selector::Get_Cut_Stage2_EventBased	() { return m_Cut_Stage2_EventBased; }
void	Selector::Set_Cut_Stage2_EventBased	(double Cut_Stage2_EventBased	) { m_Cut_Stage2_EventBased	= Cut_Stage2_EventBased; }
void Selector::Stage2_Def()
{
	std::cout << "Apply selection 2 -> Remove clusters out of time (Event Based) " << std::endl;
	
	std::cout << "						|TLeading -TLeadingMeanOverEvent| <= m_Cut_Stage2_EventBased (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage2_EventBased << ")" << std::endl;
}
void Selector::Stage2(Sample& aSample, const int& ModuleNber)
{	
	int NberOfEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE< NberOfEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	Stage2(pEvent,ModuleNber);
	}

}
void Selector::Stage2(Event*	pEvent, const int& ModuleNber)
{
	if (pEvent->Validity_ForThisModule(ModuleNber)==0) return;
	std::vector<Cluster*> V_pCluster;

	double Tmean		= 0.;
	int	Tmean_Kounter	= 0;

	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	for (int iC	= 0; iC< NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];
	
	Tmean += pCluster->Get_TMaxLeading();
	Tmean_Kounter += 1;
	
	}
	Tmean	= Tmean/double(NClusters); 

	for (int iC	= 0; iC< NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];

	double DeltaT	= pCluster->Get_TMaxLeading() - Tmean;
	if(std::fabs(DeltaT) > m_Cut_Stage2_EventBased) continue;
	
	Cluster* pCluster_new	= pEvent->Get_Cluster_Copy(pCluster);
	V_pCluster.push_back(pCluster_new);
	
	}
	pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber);
}

//----------------------------------------------------------------//
// Stage 3: Remove clusters out of time	
double Selector::Get_Cut_Stage3_TLow		() { return m_Cut_Stage3_TLow	; }
double Selector::Get_Cut_Stage3_THigh		() { return m_Cut_Stage3_THigh	; }
void	Selector::Set_Cut_Stage3_TLow		(double Cut_Stage3_TLow		) { m_Cut_Stage3_TLow		= Cut_Stage3_TLow	; }
void	Selector::Set_Cut_Stage3_THigh		(double Cut_Stage3_THigh		) { m_Cut_Stage3_THigh		= Cut_Stage3_THigh	; }
void Selector::Stage3_Def()
{
	std::cout << "Apply selection 3 -> Remove clusters out of time	" << std::endl;
	
	std::cout << "						m_Cut_Stage3_TLow <= TLeading <= m_Cut_Stage3_THigh " << std::endl;
	std::cout << "						m_Cut_Stage3_TLow	= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage3_TLow	<< std::endl;
	std::cout << "						m_Cut_Stage3_THigh	= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage3_THigh << std::endl;
}
void Selector::Stage3(Sample& aSample, const int& ModuleNber)
{	
	int NberOfEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE < NberOfEvents; iE++){
	Event* pEvent	= aSample.Get_Event(iE);
	Stage3(pEvent,ModuleNber);
	}

}
void Selector::Stage3(Event*	pEvent , const int& ModuleNber)
{	
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;
	std::vector<Cluster*> V_pCluster;
	
	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	for (int iC	= 0; iC< NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];
	
	int TLeading	= pCluster->Get_TMaxLeading();
	if(TLeading > m_Cut_Stage3_THigh) continue;
	if(TLeading < m_Cut_Stage3_TLow ) continue;
	
	Cluster* pCluster_new	= pEvent->Get_Cluster_Copy(pCluster);
	V_pCluster.push_back(pCluster_new);
	
	}
	pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber);
}

//----------------------------------------------------------------//
// Stage 4: Remove Events with an average pad multiplicity too high or too low
double Selector::Get_Cut_Stage4_APM_Low		() { return m_Cut_Stage4_APM_Low	; }
void	Selector::Set_Cut_Stage4_APM_Low		(double Cut_Stage4_APM_Low		) { m_Cut_Stage4_APM_Low		= Cut_Stage4_APM_Low	; }
double Selector::Get_Cut_Stage4_APM_High		() { return m_Cut_Stage4_APM_High	; }
void	Selector::Set_Cut_Stage4_APM_High		(double Cut_Stage4_APM_High		) { m_Cut_Stage4_APM_High		= Cut_Stage4_APM_High	; }
void Selector::Stage4_Def()
{
	std::cout << "Apply selection 4 -> Remove Events with an average pad multiplicity too high or too low " << std::endl;

	std::cout << "						Pad Multiplicity >	m_Cut_Stage4_APM_Low	(= " << std::setw(4) << std::setprecision(1) << m_Cut_Stage4_APM_Low << ")" << std::endl;
	std::cout << "						Pad Multiplicity <= m_Cut_Stage4_APM_High (= " << std::setw(4) << std::setprecision(1) << m_Cut_Stage4_APM_High << ")" << std::endl;
}
void Selector::Stage4(Sample& aSample, const int& ModuleNber)
{	
	int NberOfEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE < NberOfEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	Stage4(pEvent,ModuleNber);
	}
	
}
void Selector::Stage4(Event*	pEvent , const int& ModuleNber)
{
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;	
	
	std::vector<Cluster*>ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	float NClusters	= ClusterSet.size();
	float NPads_in_Evt	= 0;
	for (int iC	= 0; iC < NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];
	NPads_in_Evt += pCluster->Get_NberOfPads();			
	}
	float PadMulti	= NPads_in_Evt/NClusters;
	if(PadMulti <= m_Cut_Stage4_APM_Low or PadMulti > m_Cut_Stage4_APM_High) pEvent->Invalidate_ThisModule(ModuleNber);
}

//----------------------------------------------------------------//
// Stage 5: Remove clusters with too many pads 
double Selector::Get_Cut_Stage5_Npads_Hig	() { return m_Cut_Stage5_Npads_Hig; }
void	Selector::Set_Cut_Stage5_Npads_Hig	(double Cut_Stage5_Npads_Hig	) { m_Cut_Stage5_Npads_Hig	= Cut_Stage5_Npads_Hig; }
void Selector::Stage5_Def()
{
	std::cout << "Apply selection 5 -> Remove clusters with too many pads " << std::endl;
	std::cout << "						Npads <= m_Cut_Stage5_Npads_Hig (= " << std::setw(3) << std::setprecision(0) << m_Cut_Stage5_Npads_Hig << ")" << std::endl;
}
void Selector::Stage5(Sample& aSample, const int& ModuleNber)
{	
	int NEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE< NEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	Stage5(pEvent,ModuleNber);
	}
	
}
void Selector::Stage5(Event*	pEvent, const int& ModuleNber)
{
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;
	std::vector<Cluster*> V_pCluster;

	std::vector < Cluster* >ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	for (int iC	= 0; iC< NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];

	int NPads	= pCluster->Get_NberOfPads();
	if(NPads>m_Cut_Stage5_Npads_Hig) continue;
	
	Cluster* pCluster_new	= pEvent->Get_Cluster_Copy(pCluster);
	V_pCluster.push_back(pCluster_new);
	
	}
	pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber);
}

//----------------------------------------------------------------//
// Stage 6: Remove Clusters with a too high APad 
double Selector::Get_Cut_Stage6_Amax_Low	() { return m_Cut_Stage6_Amax_Low; }
void	Selector::Set_Cut_Stage6_Amax_Low	(double Cut_Stage6_Amax_Low	) { m_Cut_Stage6_Amax_Low	= Cut_Stage6_Amax_Low; }
double Selector::Get_Cut_Stage6_Amax_Hig	() { return m_Cut_Stage6_Amax_Hig; }
void	Selector::Set_Cut_Stage6_Amax_Hig	(double Cut_Stage6_Amax_Hig	) { m_Cut_Stage6_Amax_Hig	= Cut_Stage6_Amax_Hig; }
void Selector::Stage6_Def()
{
	std::cout << "Apply selection 6 -> Remove Clusters with a too high APad " << std::endl;
	
	std::cout << "						APad >= m_Cut_Stage6_Amax_Low (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage6_Amax_Low << ")" << std::endl;
	std::cout << "						APad <= m_Cut_Stage6_Amax_Hig (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage6_Amax_Hig << ")" << std::endl;
}
void Selector::Stage6(Sample& aSample, const int& ModuleNber)
{	
	int NEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE < NEvents; iE++){
	Event* pEvent	=	aSample.Get_Event(iE);
	Stage6(pEvent,ModuleNber);
	}
	
}
void Selector::Stage6(Event*	pEvent , const int& ModuleNber)
{
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;
	
	std::vector<Cluster*> V_pCluster;

	std::vector<Cluster*>ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	for (int iC	= 0; iC < NClusters; iC++){
	Cluster* pCluster	= ClusterSet[iC];
	Cluster* pCluster_new	= new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber() );

	int RemoveCluster	= 0;

	int NPads	= pCluster->Get_NberOfPads();
	for (int iP	= 0; iP < NPads; iP++){ 
		const Pad* pPad	= pCluster->Get_Pad(iP);	

		double Amax	= pPad ->Get_AMax();
		if (Amax > m_Cut_Stage6_Amax_Low) pCluster_new->Add_Pad(pPad);
		if (Amax > m_Cut_Stage6_Amax_Hig) RemoveCluster	= 1;
		if (RemoveCluster==1) break;
	
	}
	if (RemoveCluster==1 or pCluster_new->Get_NberOfPads()	== 0) continue;
	pCluster_new->DoClosure();
	V_pCluster.push_back(pCluster_new);
	}
	pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber);
}

//----------------------------------------------------------------//
// Stage Final: Reject Events with too few clusters
double Selector::Get_Cut_StageFinal_NCluster_Low () { return m_Cut_StageFinal_NCluster_Low; }
void	Selector::Set_Cut_StageFinal_NCluster_Low (double Cut_StageFinal_NCluster_Low ) { m_Cut_StageFinal_NCluster_Low	= Cut_StageFinal_NCluster_Low; }
void Selector::StageFinal_Def()
{
	std::cout << "Apply selection	Final -> Reject Events with too few clusters" << std::endl;
	
	std::cout << "						NClusters >= m_Cut_StageFinal_NCluster_Low (=" << std::setw(5) << std::setprecision(0) << m_Cut_StageFinal_NCluster_Low << ")" << std::endl;
}
void Selector::StageFinal(Sample& aSample, const int& ModuleNber)
{
	int NEvents	= aSample.Get_NberOfEvents();
	for (int iE	= 0; iE< NEvents; iE++){
	Event* pEvent	= aSample.Get_Event(iE);
	StageFinal(pEvent,ModuleNber);
	}

}
void Selector::StageFinal(Event* pEvent, const int& ModuleNber)
{
	// if (pEvent->Validity_ForThisModule(ModuleNber)	== 0) return;

	std::vector < Cluster* > ClusterSet	= pEvent->GiveMe_Clusters_ForThisModule (ModuleNber);	
	int NClusters	= ClusterSet.size();
	if(NClusters < m_Cut_StageFinal_NCluster_Low) pEvent->Invalidate_ThisModule(ModuleNber); 
}