#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "Misc/Util.h"

#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"

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


JFL_Selector::JFL_Selector(const std::string DefSelection
){
  ListOfSelectionName.clear();
  Reset_StatCounters();
  
//
  Set_Cuts() ;

//
  if (DefSelection=="A"  ) { Set_A () ; }
  if (DefSelection=="B"  ) { Set_B () ; }
  if (DefSelection=="BN_DESY21_Sample" ) { Set_BN_DESY21_Sample() ; }
  if (DefSelection=="AN_DESY21_Sample" ) { Set_AN_DESY21_Sample() ; }
  
  if (DefSelection=="BN_DESY21_Event"  ) { Set_BN_DESY21_Event()  ; }
  if (DefSelection=="AN_DESY21_Event"  ) { Set_AN_DESY21_Event()  ; }
  
  if (DefSelection=="BN_CERN22_Sample" ) { Set_BN_CERN22_Sample() ; }
  if (DefSelection=="AN_CERN22_Sample" ) { Set_AN_CERN22_Sample() ; }
  
  if (DefSelection=="BN_CERN22_Event"  ) { Set_BN_CERN22_Event()  ; }
  if (DefSelection=="AN_CERN22_Event"  ) { Set_AN_CERN22_Event()  ; }
  
  if (DefSelection=="BN_CERN22_Sample_Sim" ) { Set_BN_CERN22_Sample_Sim() ; }
  if (DefSelection=="AN_CERN22_Sample_Sim" ) { Set_AN_CERN22_Sample_Sim() ; }
  
  if (DefSelection=="AN_CERN22_SimHomeMade" ) { Set_AN_CERN22_SimHomeMade() ; }
  
  if (DefSelection=="BN_DESY21_200_Sample" ) { Set_BN_DESY21_200_Sample() ; }
  if (DefSelection=="AN_DESY21_200_Sample" ) { Set_AN_DESY21_200_Sample() ; }
  
  if (DefSelection=="BN_COSMICS23_Sample" ) { Set_BN_COSMICS23_Sample() ; }
  
  if (DefSelection=="T_DESY21_Event"  ) { Set_T_DESY21_Event()  ; }
  if (DefSelection=="T2_CERN22_Event"  ) { Set_T2_CERN22_Event()  ; }
  if (DefSelection=="TMC_CERN22_Event"  ) { Set_TMC_CERN22_Event()  ; }

//
  if (
      DefSelection!="A"   
    &&DefSelection!="B"   
    &&DefSelection!="BN_DESY21_Sample"  
    &&DefSelection!="AN_DESY21_Sample" 
    &&DefSelection!="BN_DESY21_Event"  
    &&DefSelection!="AN_DESY21_Event" 
    &&DefSelection!="BN_CERN22_Sample"  
    &&DefSelection!="AN_CERN22_Sample"  
    &&DefSelection!="BN_CERN22_Event"  
    &&DefSelection!="AN_CERN22_Event"  
    &&DefSelection!="BN_CERN22_Sample_Sim"  
    &&DefSelection!="AN_CERN22_Sample_Sim"  
    &&DefSelection!="AN_CERN22_SimHomeMade"  
    &&DefSelection!="BN_DESY21_200_Sample"  
    &&DefSelection!="AN_DESY21_200_Sample" 
    &&DefSelection!="BN_COSMICS23_Sample"  
    &&DefSelection!="T_DESY21_Event"  
    &&DefSelection!="T2_CERN22_Event" 
    &&DefSelection!="TMC_CERN22_Event"
  ) { 
    std::cout 
      << "JFL_Selector::JFL_Selector "
      << " unknown predefined selection " << DefSelection
      << std::endl ;
    
    abort();
  }

}  

JFL_Selector::JFL_Selector()
{
  ListOfSelectionName.clear();
  Reset_StatCounters();

//
  Set_Cuts() ;
  
}  

JFL_Selector::~JFL_Selector()
{
  ListOfSelectionName.clear();
  ListOfNberOfEvents_Before  .clear() ;
  ListOfNberOfEvents_After   .clear() ;  
  ListOfNberOfModules_Before .clear() ;  
  ListOfNberOfModules_After  .clear() ;  
  ListOfNberOfClusters_Before.clear() ;  
  ListOfNberOfClusters_After .clear() ;  
  ListOfNberOfPads_Before    .clear() ;  
  ListOfNberOfPads_After     .clear() ;  
}

void JFL_Selector::Set_Cuts()
{
  m_Cut_Stage2_Npads_Low    =   2 ;

  m_Cut_Stage5_NCluster_Low =   34 ;

  m_Cut_Stage6_ARelat_Low   =   0.03 ;
  m_Cut_Stage6_A_Low        =   30. ;

  m_Cut_Stage7_T2T1_Low     =   -3. ;
  m_Cut_Stage7_T2T1_Hig     =   30. ;
  m_Cut_Stage7_T3T1_Low     =   -3. ;
  m_Cut_Stage7_T3T1_Hig     =  100. ;
  m_Cut_Stage7_T3T2_Low     =   -3. ;
  m_Cut_Stage7_T3T2_Hig     =  100. ;

  m_Cut_Stage8_A3onA1_Hig   =   0.3  ;
  m_Cut_Stage8_A3onA1_Low   =   0.03 ;
  m_Cut_Stage8_A2onA1_Low   =   0.06 ;
  m_Cut_Stage8_A3onA2_Low   =   0.03 ;

  m_Cut_Stage9_Npads_Low    =   1 ;

  m_Cut_Stage10_Npads_Hig   =   5 ;
  
  m_Cut_Stage11_EventBased  =    3. ;

  m_Cut_Stage12_RunBased    =   10. ;

  m_Cut_Stage120_TLow       =   160. ;
  m_Cut_Stage120_THigh      =   220. ;

  m_Cut_Stage13_DT_Low      =   -3. ;

  m_Cut_Stage14_Amax_Low    = 0. ;
  m_Cut_Stage14_Amax_Hig    = 3700. ;

//
  m_Cut_Stage15_CutSlopeXY_Hig  = 3+10.  ;
  m_Cut_Stage15_CutSlopeXY_Low  = 3-10.  ;

  m_Cut_Stage15_CutSlopeXY_Hig  =  3.6  ;
  m_Cut_Stage15_CutSlopeXY_Low  = -3.6  ;

  m_Cut_Stage15_CutSlopeXY_Hig  =  100000000.  ;
  m_Cut_Stage15_CutSlopeXY_Low  = -100000000.  ;

//
  m_Cut_Stage16_CutSlopeXZ_Hig  = 50.  ;
  m_Cut_Stage16_CutSlopeXZ_Low  =-50.  ;

  m_Cut_Stage16_CutSlopeXZ_Hig  =  13.  ;
  m_Cut_Stage16_CutSlopeXZ_Low  = -13.  ;
 
  m_Cut_Stage16_CutSlopeXZ_Hig  = 100000000.  ;
  m_Cut_Stage16_CutSlopeXZ_Low  =-100000000.  ;

//
  m_Cut_Stage17_CutInterCeptXZ_Hig  = 100000000.  ;
  m_Cut_Stage17_CutInterCeptXZ_Low  =-100000000.  ;

  m_Cut_Stage17_CutInterCeptXZ_Hig  = 350.  ;
  m_Cut_Stage17_CutInterCeptXZ_Low  = 300.  ;


  // Samira
  // 40 ns/bin   7 cm /us  -->   0.007 cm/ns ---> 0.28 cm / bin
  m_Cut_Stage18_v_drift_est    = 0.28 ;
  m_Cut_Stage18_CutSlopeXYZ0   = 0.1  ;
  m_Cut_Stage18_CutSlopeXYZ1   = 0.1  ;

  m_Cut_StageT15_APM_Low      = 2  ;
  m_Cut_StageT15_APM_High     = 4 ;

  m_Cut_StageT16_PadIntegral = 0.      ;

  m_Cut_StageT17_PadIntegral = 0.      ;

  m_Cut_StageT19_BinDerivative = 240.   ;

  m_Cut_StageT20_Amax_dRay     = 900 ;
  m_Cut_StageT20_deltaY_dRay   = 0.5 ;

  m_Cut_StageT21_Rho3vsRho2     = 0.005 ;

}

//---------------------------------------------------------------------//


void JFL_Selector::Set_T_DESY21_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection T_DESY21_Event" << std::endl ;
  ListOfSelectionName.clear();
// 1+11+15+5
  Add_Selection( "Stage1"   );  // border clusters
  Add_Selection( "Stage11"  );  // time sync event based
  Add_Selection( "Stage120" );  // time sync run based
  Add_Selection( "StageT15"  );  // average pad multiplicity
  Add_Selection( "Stage10"  );  // maximum number of pads
  Add_Selection( "Stage14"  );  // min & max amplitude
  Add_Selection( "Stage5"   );  // number of clusters
}

void JFL_Selector::Set_T2_CERN22_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection T2_CERN22_Event" << std::endl ;
  ListOfSelectionName.clear();
// 1+11+120+15+5
  Add_Selection( "Stage1"   );  // border clusters
  Add_Selection( "Stage11"  );  // time sync event based
  Add_Selection( "Stage120" );  // time sync run based
  Add_Selection( "StageT15"  );  // average pad multiplicity in cluster
  Add_Selection( "Stage5"   );  // number of clusters
}

void JFL_Selector::Set_TMC_CERN22_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection TMC_CERN22_Event" << std::endl ;
  ListOfSelectionName.clear();
  //
  Add_Selection( "Stage1"   );  // border clusters
  // Add_Selection( "Stage11"  );  // time sync event based
  Add_Selection( "Stage5"   );  // number of clusters
}


void JFL_Selector::Set_A()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection A " << std::endl ;
// 1+2+3+4+5
  Add_Selection( "Stage1" );
  Add_Selection( "Stage2" );
  Add_Selection( "Stage3" );
  Add_Selection( "Stage4" );
  Add_Selection( "Stage5" );
  
  Tell_Selection();
}

void JFL_Selector::Set_B()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection B " << std::endl ;
// 1+6+5
  Add_Selection("Stage1" );
  Add_Selection("Stage6" );
  Add_Selection("Stage5" );
  
  Tell_Selection();
}

void JFL_Selector::Set_BN_DESY21_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_DESY21_Sample" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+12+11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_DESY21_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_DESY21_Sample" << std::endl ;
// 1+14+12+11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage4"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );

  Tell_Selection();
}

void JFL_Selector::Set_BN_DESY21_200_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_DESY21_200_Sample" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+12+11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  
  m_Cut_Stage6_ARelat_Low   =   0.01 ;

  m_Cut_Stage8_A3onA1_Hig   =   0.2  ;
  m_Cut_Stage8_A3onA1_Low   =   0.01;
  m_Cut_Stage8_A2onA1_Low   =   0.02 ;
  m_Cut_Stage8_A3onA2_Low   =   0.01 ;
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_DESY21_200_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_DESY21_200_Sample" << std::endl ;
// 1+14+12+11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage4"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  
  m_Cut_Stage6_ARelat_Low   =   0.01 ;

  m_Cut_Stage8_A3onA1_Hig   =   0.2  ;
  m_Cut_Stage8_A3onA1_Low   =   0.01;
  m_Cut_Stage8_A2onA1_Low   =   0.02 ;
  m_Cut_Stage8_A3onA2_Low   =   0.01 ;
  
  Tell_Selection();
}

void JFL_Selector::Set_BN_CERN22_Sample_Sim()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_CERN22_Sample_Sim" << std::endl ;
  ListOfSelectionName.clear();
// 1+14   +11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_CERN22_SimHomeMade()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_CERN22_SimHomeMade" << std::endl ;
// 1+14   +11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage4"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );

  m_Cut_Stage6_A_Low = 0. ;
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_CERN22_Sample_Sim()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_CERN22_Sample_Sim" << std::endl ;
// 1+14   +11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage4"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );

  Tell_Selection();
}

void JFL_Selector::Set_BN_CERN22_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_CERN22_Sample" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+12+11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  
  m_Cut_Stage12_RunBased = 40. ;

  Tell_Selection();
}

void JFL_Selector::Set_AN_CERN22_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_CERN22_Sample" << std::endl ;
// 1+14+12+11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage4"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );

  m_Cut_Stage12_RunBased = 40. ;

  Tell_Selection();
}


void JFL_Selector::Set_BN_CERN22_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_CERN22_Event" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+120+11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"   );
  Add_Selection( "Stage14"  );
  Add_Selection( "Stage120" );
  Add_Selection( "Stage11"  );
  Add_Selection( "Stage6"   );
  Add_Selection( "Stage13"  );
  Add_Selection( "Stage7"   );
  Add_Selection( "Stage8"   );
  Add_Selection( "Stage9"   );
  Add_Selection( "Stage10"  );
  Add_Selection( "Stage5"   );
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_CERN22_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_CERN22_Event" << std::endl ;
// 1+14+120+11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"   );
  Add_Selection( "Stage14"  );
  Add_Selection( "Stage120" );
  Add_Selection( "Stage11"  );
  Add_Selection( "Stage6"   );
  Add_Selection( "Stage13"  );
  Add_Selection( "Stage7"   );
  Add_Selection( "Stage8"   );
  Add_Selection( "Stage4"   );
  Add_Selection( "Stage9"   );
  Add_Selection( "Stage10"  );
  Add_Selection( "Stage5"   );

  Tell_Selection();
}
void JFL_Selector::Set_BN_DESY21_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_DESY21_Event" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+120+11+6+13+7+8  +9+10+5
  Add_Selection( "Stage1"   );
  Add_Selection( "Stage14"  );
  Add_Selection( "Stage120" );
  Add_Selection( "Stage11"  );
  Add_Selection( "Stage6"   );
  Add_Selection( "Stage13"  );
  Add_Selection( "Stage7"   );
  Add_Selection( "Stage8"   );
  Add_Selection( "Stage9"   );
  Add_Selection( "Stage10"  );
  Add_Selection( "Stage5"   );
  
  Tell_Selection();
}

void JFL_Selector::Set_AN_DESY21_Event()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection AN_DESY21_Event" << std::endl ;
// 1+14+120+11+6+13+7+8+4+9+10+5  
  Add_Selection( "Stage1"   );
  Add_Selection( "Stage14"  );
  Add_Selection( "Stage120" );
  Add_Selection( "Stage11"  );
  Add_Selection( "Stage6"   );
  Add_Selection( "Stage13"  );
  Add_Selection( "Stage7"   );
  Add_Selection( "Stage8"   );
  Add_Selection( "Stage4"   );
  Add_Selection( "Stage9"   );
  Add_Selection( "Stage10"  );
  Add_Selection( "Stage5"   );

  Tell_Selection();
}
//---------------------------------------------------------------------//
void JFL_Selector::Tell_Selection()
{
  std::cout << std::endl ;
  std::cout << "-------------->List of selections" << std::endl ;
  int iTem_Max = NberOfSelections() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    if ( ListOfSelectionName[iTem]=="Stage1"   ) { Stage1_Def()    ; std::cout << std::endl ; } 
    if ( ListOfSelectionName[iTem]=="Stage2"   ) { Stage2_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage3"   ) { Stage3_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage4"   ) { Stage4_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage5"   ) { Stage5_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage6"   ) { Stage6_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage7"   ) { Stage7_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage8"   ) { Stage8_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage9"   ) { Stage9_Def()    ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage10"  ) { Stage10_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage11"  ) { Stage11_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage12"  ) { Stage12_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage120" ) { Stage120_Def()  ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage13"  ) { Stage13_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage14"  ) { Stage14_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage15"  ) { Stage15_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage16"  ) { Stage16_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage17"  ) { Stage17_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="Stage18"  ) { Stage18_Def()   ; std::cout << std::endl ; }  
    if ( ListOfSelectionName[iTem]=="StageT15"  ) { StageT15_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT16"  ) { StageT16_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT17"  ) { StageT17_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT18"  ) { StageT18_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT19"  ) { StageT19_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT20"  ) { StageT20_Def()   ; std::cout << std::endl ; }
    if ( ListOfSelectionName[iTem]=="StageT21"  ) { StageT21_Def()   ; std::cout << std::endl ; }
  }
  std::cout << std::endl ;

}

//---------------------------------------------------------------------//
void JFL_Selector::Reset_StatCounters()
{
  ListOfNberOfEvents_Before  .clear() ;
  ListOfNberOfEvents_After   .clear() ;  
  ListOfNberOfModules_Before .clear() ;  
  ListOfNberOfModules_After  .clear() ;  
  ListOfNberOfClusters_Before.clear() ;  
  ListOfNberOfClusters_After .clear() ;  
  ListOfNberOfPads_Before    .clear() ;  
  ListOfNberOfPads_After     .clear() ;  
  int iTem_Max = NberOfSelections() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
    ListOfNberOfEvents_Before  .push_back( 0) ;
    ListOfNberOfEvents_After   .push_back( 0) ;  
    ListOfNberOfModules_Before .push_back( 0) ;  
    ListOfNberOfModules_After  .push_back( 0) ;  
    ListOfNberOfClusters_Before.push_back( 0) ;  
    ListOfNberOfClusters_After .push_back( 0) ;  
    ListOfNberOfPads_Before    .push_back( 0) ;  
    ListOfNberOfPads_After     .push_back( 0) ;  
  }
}

void JFL_Selector::Reset_Selection()
{
  ListOfSelectionName.clear() ;
  Reset_StatCounters() ;
}


int JFL_Selector::NberOfSelections()
{
  return ListOfSelectionName.size() ;
}

std::string JFL_Selector::Get_SelectionName(const int& iTem)
{
  return ListOfSelectionName[iTem] ;
}

void JFL_Selector::Add_Selection(const std::string& SelectionName)
{
  int IsOK = 0 ;
  if ( SelectionName=="Stage1"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage2"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage3"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage4"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage5"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage6"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage7"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage8"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage9"   ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage10"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage11"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage12"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage120" ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage13"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage14"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage15"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage16"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage17"  ) { IsOK = 1 ; } 
  if ( SelectionName=="Stage18"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT15"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT16"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT17"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT18"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT19"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT20"  ) { IsOK = 1 ; } 
  if ( SelectionName=="StageT21"  ) { IsOK = 1 ; } 
  
  if (IsOK==0){
    std::cout 
      << "JFL_Selector::Add_Selection "
      << " unknown selection" << SelectionName
      << std::endl ;
    abort() ;
  }

  ListOfSelectionName.push_back(SelectionName) ;
}

void JFL_Selector::ApplySelection(Sample& aSample, const int& ModuleNber)
{
  std::cout << std::endl ;
  std::cout << "-------------->Apply selections " << std::endl ;
  
  Reset_StatCounters();
  
  int iTem_Max = NberOfSelections() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){   
    Apply_ASelection(aSample,ModuleNber, iTem) ;
  }
  std::cout << std::endl ;
  
}

void JFL_Selector::Apply_ASelection(Sample& aSample, const int& ModuleNber,const int& iTem)
{
  std::string SelectionName = ListOfSelectionName[iTem] ;

  SetStat_Before(aSample, ModuleNber ,iTem) ;
  
  if ( SelectionName=="Stage1"   ) { Stage1  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage2"   ) { Stage2  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage3"   ) { Stage3  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage4"   ) { Stage4  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage5"   ) { Stage5  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage6"   ) { Stage6  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage7"   ) { Stage7  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage8"   ) { Stage8  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage9"   ) { Stage9  (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage10"  ) { Stage10 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage11"  ) { Stage11 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage12"  ) { Stage12 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage120" ) { Stage120(aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage13"  ) { Stage13 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage14"  ) { Stage14 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage15"  ) { Stage15 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage16"  ) { Stage16 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage17"  ) { Stage17 (aSample, ModuleNber) ; } 
  if ( SelectionName=="Stage18"  ) { Stage18 (aSample, ModuleNber) ; } 
  if ( SelectionName=="StageT15"  ) { StageT15 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT16"  ) { StageT16 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT17"  ) { StageT17 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT18"  ) { StageT18 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT19"  ) { StageT19 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT20"  ) { StageT20 (aSample, ModuleNber) ; }
  if ( SelectionName=="StageT21"  ) { StageT21 (aSample, ModuleNber) ; }

  SetStat_After(aSample, ModuleNber ,iTem) ;
}

void JFL_Selector::SetStat_Before(Sample& aSample, const int& ModuleNber,const int& iTem)
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  int NberOfValidEvents = 0 ;
  int NberOfClusters    = 0 ;
  int NberOfPads        = 0 ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; 
    NberOfValidEvents += 1 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    
    NberOfClusters += NClusters ;
    
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      NberOfPads += pCluster->Get_NberOfPads() ;
    }
  }
  
  ListOfNberOfEvents_Before  [iTem] = NberOfValidEvents ;
  ListOfNberOfModules_Before [iTem] = NberOfValidEvents ;
  ListOfNberOfClusters_Before[iTem] = NberOfClusters    ;
  ListOfNberOfPads_Before    [iTem] = NberOfPads        ;
}

void JFL_Selector::SetStat_After(Sample& aSample, const int& ModuleNber,const int& iTem)
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  int NberOfValidEvents = 0 ;
  int NberOfClusters    = 0 ;
  int NberOfPads        = 0 ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; 
    NberOfValidEvents += 1 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    
    NberOfClusters += NClusters ;
    
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      NberOfPads += pCluster->Get_NberOfPads() ;
    }
  }
  
  ListOfNberOfEvents_After  [iTem] = NberOfValidEvents ;
  ListOfNberOfModules_After [iTem] = NberOfValidEvents ;
  ListOfNberOfClusters_After[iTem] = NberOfClusters    ;
  ListOfNberOfPads_After    [iTem] = NberOfPads        ;
}

void JFL_Selector::ApplySelection(Event*  pEvent)
{
  int NberOfModule = pEvent->Get_NberOfModule() ;

  int iTem_Max = NberOfSelections() ;
  for (int iTem = 0 ; iTem< iTem_Max ; iTem++){   
  
    SetStat_Before(pEvent , iTem) ;

    for (int iModule = 0 ; iModule< NberOfModule ; iModule++){   
      Module* pModule = pEvent->Get_Module_InArray(iModule) ;
      int ModuleNber = pModule->Get_ModuleNber() ;
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      Apply_ASelection(pEvent,ModuleNber,iTem) ;
    }
  
    SetStat_After(pEvent , iTem) ;

  }

}

void JFL_Selector::Apply_ASelection(Event*  pEvent, const int& ModuleNber,const int& iTem)
{
  std::string SelectionName = ListOfSelectionName[iTem] ;

  if ( SelectionName=="Stage1"   ) { Stage1  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage2"   ) { Stage2  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage3"   ) { Stage3  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage4"   ) { Stage4  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage5"   ) { Stage5  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage6"   ) { Stage6  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage7"   ) { Stage7  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage8"   ) { Stage8  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage9"   ) { Stage9  (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage10"  ) { Stage10 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage11"  ) { Stage11 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage120" ) { Stage120(pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage13"  ) { Stage13 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage14"  ) { Stage14 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage15"  ) { Stage15 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage16"  ) { Stage16 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage17"  ) { Stage17 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="Stage18"  ) { Stage18 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="StageT15"  ) { StageT15 (pEvent, ModuleNber) ; } 
  if ( SelectionName=="StageT16"  ) { StageT16 (pEvent, ModuleNber) ; }
  if ( SelectionName=="StageT17"  ) { StageT17 (pEvent, ModuleNber) ; }
  if ( SelectionName=="StageT18"  ) { StageT18 (pEvent, ModuleNber) ; }
  if ( SelectionName=="StageT19"  ) { StageT19 (pEvent, ModuleNber) ; }
  if ( SelectionName=="StageT20"  ) { StageT20 (pEvent, ModuleNber) ; }
  if ( SelectionName=="StageT21"  ) { StageT21 (pEvent, ModuleNber) ; }
}

void JFL_Selector::SetStat_Before(Event*  pEvent , const int& iTem)
{
  if (pEvent->IsValid() == 0) return ;
  
  int NberOfModule = pEvent->Get_NberOfModule() ;

  int NberOfModules     = 0 ;
  int NberOfClusters    = 0 ;
  int NberOfPads        = 0 ;
  for (int iModule = 0 ; iModule< NberOfModule ; iModule++){   
    Module* pModule = pEvent->Get_Module_InArray(iModule) ;
    int ModuleNber = pModule->Get_ModuleNber() ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    NberOfModules +=1 ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    
    NberOfClusters += NClusters ;
    
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      NberOfPads += pCluster->Get_NberOfPads() ;
    }
  }

  ListOfNberOfEvents_Before  [iTem] += 1                 ;
  ListOfNberOfModules_Before [iTem] += NberOfModules     ;
  ListOfNberOfClusters_Before[iTem] += NberOfClusters    ;
  ListOfNberOfPads_Before    [iTem] += NberOfPads        ;
}

void JFL_Selector::SetStat_After(Event*  pEvent , const int& iTem)
{
  if (pEvent->IsValid() == 0) return ;
  int NberOfModule = pEvent->Get_NberOfModule() ;

  int NberOfModules     = 0 ;
  int NberOfClusters    = 0 ;
  int NberOfPads        = 0 ;
  for (int iModule = 0 ; iModule< NberOfModule ; iModule++){   
    Module* pModule = pEvent->Get_Module_InArray(iModule) ;
    int ModuleNber = pModule->Get_ModuleNber() ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    
    NberOfModules +=1 ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    
    NberOfClusters += NClusters ;
    
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      NberOfPads += pCluster->Get_NberOfPads() ;
    }
  }

  ListOfNberOfEvents_After  [iTem] += 1                 ;
  ListOfNberOfModules_After [iTem] += NberOfModules     ;
  ListOfNberOfClusters_After[iTem] += NberOfClusters    ;
  ListOfNberOfPads_After    [iTem] += NberOfPads        ;
}

void JFL_Selector::PrintStat()
{
 std::cout << std::endl ;
 std::cout << "Selection Statistics " << std::endl ;
   std::cout
     << std::setw(5) << std::setprecision(0) << "#Cut"
     << std::setw(8) << std::setprecision(0) << "SelName"
     << std::setw(9) << std::setprecision(0) << "EvBef"   
     << std::setw(9) << std::setprecision(0) << "EvAfter" << " (" << std::setw(8) <<  "wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"      << ") "
     << std::setw(9) << std::setprecision(0) << "MdBef"   
     << std::setw(9) << std::setprecision(0) << "MdAfter" << " (" << std::setw(8) <<  "wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"      << ") "
     << std::setw(9) << std::setprecision(0) << "ClBef"   
     << std::setw(9) << std::setprecision(0) << "ClAfter" << " (" << std::setw(8) <<  "wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"      << ") "
     << std::setw(9) << std::setprecision(0) << "PdBef"   
     << std::setw(9) << std::setprecision(0) << "PdAfter" << " (" << std::setw(8) <<  "wrtPrev" << ") "<< " (" << std::setw(8) << "wrtStart"      << ") "
     << std::endl ;
 int iTem_Max = NberOfSelections() ;
 for (int iTem = 0 ; iTem< iTem_Max ; iTem++){
   std::cout
     << std::setw(5) << std::setprecision(0) << iTem
     << std::setw(8) << std::setprecision(0) << ListOfSelectionName[iTem]
     << std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_Before[iTem]   
     << std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_After[iTem]    << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfEvents_After[iTem]-ListOfNberOfEvents_Before[iTem])/ListOfNberOfEvents_Before[iTem]      << ") "<< " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfEvents_After[iTem]-ListOfNberOfEvents_Before[0])/ListOfNberOfEvents_Before[0]      << ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfModules_Before[iTem] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfModules_After[iTem]  << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfModules_After[iTem]-ListOfNberOfModules_Before[iTem])/ListOfNberOfModules_Before[iTem]    << ") "<< " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfModules_After[iTem]-ListOfNberOfModules_Before[0])/ListOfNberOfModules_Before[0]   << ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_Before[iTem] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_After[iTem]  << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfClusters_After[iTem]-ListOfNberOfClusters_Before[iTem])/ListOfNberOfClusters_Before[iTem]<< ") "<< " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfClusters_After[iTem]-ListOfNberOfClusters_Before[0])/ListOfNberOfClusters_Before[0]<< ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfPads_Before[iTem] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfPads_After[iTem]      << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfPads_After[iTem]-ListOfNberOfPads_Before[iTem])/ListOfNberOfPads_Before[iTem]            << ") "<< " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfPads_After[iTem]-ListOfNberOfPads_Before[0])/ListOfNberOfPads_Before[0]            << ") "
     << std::endl ;
 }
 std::cout << std::setw(13) << std::setprecision(0) << "Summary Sel"  
     << std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_Before[0]   
     << std::setw(9) << std::setprecision(0) << ListOfNberOfEvents_After[iTem_Max-1]    << std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfEvents_After[iTem_Max-1]-ListOfNberOfEvents_Before[0])/ListOfNberOfEvents_Before[0]      << ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfModules_Before[0] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfModules_After[iTem_Max-1]  << std::setw(12)  << " " << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfModules_After[iTem_Max-1]-ListOfNberOfModules_Before[0])/ListOfNberOfModules_Before[0]   << ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_Before[0] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfClusters_After[iTem_Max-1]  << std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfClusters_After[iTem_Max-1]-ListOfNberOfClusters_Before[0])/ListOfNberOfClusters_Before[0]<< ") "
     << std::setw(9) << std::setprecision(0) << ListOfNberOfPads_Before[0] 
     << std::setw(9) << std::setprecision(0) << ListOfNberOfPads_After[iTem_Max-1]      << std::setw(12) << " " << " (" << std::setw(8) << std::setprecision(2) <<  100.*(ListOfNberOfPads_After[iTem_Max-1]-ListOfNberOfPads_Before[0])/ListOfNberOfPads_Before[0]            << ") "
     << std::endl ;
}

//----------------------------------------------------------------//
// Stage 1: Remove clusters in first and last columns 
void JFL_Selector::Stage1_Def()
{
  std::cout << "Apply selection  1 -> Remove clusters in first and last columns " << std::endl ;
}
void JFL_Selector::Stage1(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    Stage1(pEvent,ModuleNber) ;
  }

}
void JFL_Selector::Stage1(Event* pEvent, const int& ModuleNber)
{
  const Model_ReadOutGeometry* pModel_ReadOutGeometry = pEvent->Get_Model_ReadOutGeometry() ;
  int iXFirst = 0 ;
  int iXLast  = pModel_ReadOutGeometry->Get_Nx() - 1 ;

    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
    std::vector<Cluster*> V_pCluster ;

    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int iXCluster = (pCluster->Get_LeadingPad())->Get_iX() ;
      
      if (iXCluster == iXFirst || iXCluster == iXLast) continue ;
      
      Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluster_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}


//----------------------------------------------------------------//
// Stage 2: Remove clusters with too few pads 
double JFL_Selector::Get_Cut_Stage2_Npads_Low    () { return m_Cut_Stage2_Npads_Low    ; }
void   JFL_Selector::Set_Cut_Stage2_Npads_Low    (double Cut_Stage2_Npads_Low    ) { m_Cut_Stage2_Npads_Low    = Cut_Stage2_Npads_Low    ; }
void JFL_Selector::Stage2_Def()
{
  std::cout << "Apply selection  2 -> Remove clusters with too few pads " << std::endl ;
    
  std::cout << "                      Npads >= m_Cut_Stage2_Npads_Low (= " << std::setw(8) << std::setprecision(0) << m_Cut_Stage2_Npads_Low << ")" << std::endl ;
}
void JFL_Selector::Stage2(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage2(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage2(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster ;

  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];

    int NPads = pCluster->Get_NberOfPads() ;
    if(NPads<m_Cut_Stage2_Npads_Low) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
    
  
}


//----------------------------------------------------------------//
// Stage 3: Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis
void JFL_Selector::Stage3_Def()
{
  std::cout << "Apply selection  3 -> Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis" << std::endl ;
}
void JFL_Selector::Stage3(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage3(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::Stage3(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
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
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
void JFL_Selector::Stage4_Def()
{
  std::cout << "Apply selection  4 -> Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis" << std::endl ;
}
void JFL_Selector::Stage4(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage4(pEvent,ModuleNber);
  }

}
void JFL_Selector::Stage4(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
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

//----------------------------------------------------------------//
// Stage 5: Reject Events with too few clusters
double JFL_Selector::Get_Cut_Stage5_NCluster_Low () { return m_Cut_Stage5_NCluster_Low ; }
void   JFL_Selector::Set_Cut_Stage5_NCluster_Low (double Cut_Stage5_NCluster_Low ) { m_Cut_Stage5_NCluster_Low = Cut_Stage5_NCluster_Low ; }
void JFL_Selector::Stage5_Def()
{
  std::cout << "Apply selection  5 -> Reject Events with too few clusters" << std::endl ;
  
  std::cout << "                      NClusters >= m_Cut_Stage5_NCluster_Low (=" << std::setw(5) << std::setprecision(0) << m_Cut_Stage5_NCluster_Low << ")" << std::endl ;
}
void JFL_Selector::Stage5(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    Stage5(pEvent,ModuleNber) ;
  }

}
void JFL_Selector::Stage5(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;

  std::vector < Cluster* > ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  if(NClusters < m_Cut_Stage5_NCluster_Low) pEvent->Invalidate_ThisModule(ModuleNber) ; 
}

//----------------------------------------------------------------//
// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad keeping hot enough pads
double JFL_Selector::Get_Cut_Stage6_ARelat_Low   () { return m_Cut_Stage6_ARelat_Low   ; }
double JFL_Selector::Get_Cut_Stage6_A_Low        () { return m_Cut_Stage6_A_Low        ; }
void   JFL_Selector::Set_Cut_Stage6_ARelat_Low   (double Cut_Stage6_ARelat_Low   ) { m_Cut_Stage6_ARelat_Low   = Cut_Stage6_ARelat_Low   ; }
void   JFL_Selector::Set_Cut_Stage6_A_Low        (double Cut_Stage6_A_Low        ) { m_Cut_Stage6_A_Low        = Cut_Stage6_A_Low        ; }
void JFL_Selector::Stage6_Def()
{
  std::cout << "Apply selection  6 -> Redo cluster keeping only the pads properly ordered wrt Leading pad keeping hot enough pads" << std::endl ;

  std::cout << "                      A/ALeading >= m_Cut_Stage6_ARelat_Low (= " << std::setw(6) << std::setprecision(2) << m_Cut_Stage6_ARelat_Low << ")" << std::endl ;
  std::cout << "                      A          >= m_Cut_Stage6_A_Low      (= " << std::setw(6) << std::setprecision(0) << m_Cut_Stage6_A_Low      << ")" << std::endl ;
}
void JFL_Selector::Stage6(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage6(pEvent,ModuleNber) ;
  }

}
void JFL_Selector::Stage6(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
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

//
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster2 ;
  
  std::vector < Cluster* >ClusterSet2 = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters2 = ClusterSet2.size() ;
  for (int iC = 0 ; iC< NClusters2; iC++){
    Cluster* pCluster = ClusterSet2[iC];
    
    double ALeading = pCluster->Get_AMaxLeading() ;

    Cluster* pCluster_new = new Cluster(                                          
                                        pCluster->Get_EventNber () ,
                                        pCluster->Get_EntryNber () ,
                                        pCluster->Get_ModuleNber() 
                                       ) ;
          
    int OneIsSaved = 0 ;
    int NPads = pCluster->Get_NberOfPads() ;
    for (int iP = 0 ; iP< NPads; iP++){
      const Pad* pPad = pCluster->Get_Pad(iP);

      double Amax   = pPad->Get_AMax() ;
      double Ratio = Amax/ALeading  ;
      
      if (Ratio < m_Cut_Stage6_ARelat_Low) continue ; 
      
      if (Amax  < m_Cut_Stage6_A_Low     ) continue ; 
      
      OneIsSaved = 1 ;
      pCluster_new->Add_Pad(pPad); 
    }
    if (OneIsSaved!=0){
      pCluster_new->DoClosure();
      V_pCluster2.push_back(pCluster_new) ;
    }else{
      delete pCluster_new ;
    }

  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster2, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
double JFL_Selector::Get_Cut_Stage7_T2T1_Low     () { return m_Cut_Stage7_T2T1_Low     ; }
double JFL_Selector::Get_Cut_Stage7_T2T1_Hig     () { return m_Cut_Stage7_T2T1_Hig     ; }
double JFL_Selector::Get_Cut_Stage7_T3T1_Low     () { return m_Cut_Stage7_T3T1_Low     ; }
double JFL_Selector::Get_Cut_Stage7_T3T1_Hig     () { return m_Cut_Stage7_T3T1_Hig     ; }
double JFL_Selector::Get_Cut_Stage7_T3T2_Low     () { return m_Cut_Stage7_T3T2_Low     ; }
double JFL_Selector::Get_Cut_Stage7_T3T2_Hig     () { return m_Cut_Stage7_T3T2_Hig     ; }
void   JFL_Selector::Set_Cut_Stage7_T2T1_Low     (double Cut_Stage7_T2T1_Low     ) { m_Cut_Stage7_T2T1_Low     = Cut_Stage7_T2T1_Low     ; }
void   JFL_Selector::Set_Cut_Stage7_T2T1_Hig     (double Cut_Stage7_T2T1_Hig     ) { m_Cut_Stage7_T2T1_Hig     = Cut_Stage7_T2T1_Hig     ; }
void   JFL_Selector::Set_Cut_Stage7_T3T1_Low     (double Cut_Stage7_T3T1_Low     ) { m_Cut_Stage7_T3T1_Low     = Cut_Stage7_T3T1_Low     ; }
void   JFL_Selector::Set_Cut_Stage7_T3T1_Hig     (double Cut_Stage7_T3T1_Hig     ) { m_Cut_Stage7_T3T1_Hig     = Cut_Stage7_T3T1_Hig     ; }
void   JFL_Selector::Set_Cut_Stage7_T3T2_Low     (double Cut_Stage7_T3T2_Low     ) { m_Cut_Stage7_T3T2_Low     = Cut_Stage7_T3T2_Low     ; }
void   JFL_Selector::Set_Cut_Stage7_T3T2_Hig     (double Cut_Stage7_T3T2_Hig     ) { m_Cut_Stage7_T3T2_Hig     = Cut_Stage7_T3T2_Hig     ; }
void JFL_Selector::Stage7_Def()
{
  std::cout << "Apply selection  7 -> Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2" << std::endl ;

  std::cout << "                      T2-T1 <= m_Cut_Stage7_T2T1_Hig (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T2T1_Hig << ")" << std::endl ;
  std::cout << "                      T2-T1 >= m_Cut_Stage7_T2T1_Low (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T2T1_Low << ")" << std::endl ;
  std::cout << "                      T3-T1 <= m_Cut_Stage7_T3T1_Hig (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T3T1_Hig << ")" << std::endl ;
  std::cout << "                      T3-T1 >= m_Cut_Stage7_T3T1_Low (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T3T1_Low << ")" << std::endl ;
  std::cout << "                      T3-T2 <= m_Cut_Stage7_T3T2_Hig (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T3T2_Hig << ")" << std::endl ;
  std::cout << "                      T3-T2 >= m_Cut_Stage7_T3T2_Low (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage7_T3T2_Low << ")" << std::endl ;
}
void JFL_Selector::Stage7(Sample& aSample, const int& ModuleNber)
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage7(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::Stage7(Event*  pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;  
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
      if (  T2     >  m_Cut_Stage7_T2T1_Hig ||  T2     < m_Cut_Stage7_T2T1_Low ) RemoveCluster = 1 ;
      if (  T3     >  m_Cut_Stage7_T3T1_Hig ||  T3     < m_Cut_Stage7_T3T1_Low ) RemoveCluster = 1 ;
      if ( (T3-T2) >  m_Cut_Stage7_T3T2_Hig || (T3-T2) < m_Cut_Stage7_T3T2_Low ) RemoveCluster = 1 ;
    }else{
      if (AU_Found==1 || AD_Found==1 ){
        double T2 = TU ;
        if (AD_Found==1) T2 = TD ;
        if ( T2 >  m_Cut_Stage7_T2T1_Hig || T2 < m_Cut_Stage7_T2T1_Low ) RemoveCluster = 1 ;
      }
    }
    if (RemoveCluster==1) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
            
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

}

//----------------------------------------------------------------//
// Stage 8: Remove Clusters with too high or too low A3/A1 or too low A2/A1 or too low A3/A2
double JFL_Selector::Get_Cut_Stage8_A3onA1_Hig   () { return m_Cut_Stage8_A3onA1_Hig   ; }
double JFL_Selector::Get_Cut_Stage8_A3onA1_Low   () { return m_Cut_Stage8_A3onA1_Low   ; }
double JFL_Selector::Get_Cut_Stage8_A2onA1_Low   () { return m_Cut_Stage8_A2onA1_Low   ; }
double JFL_Selector::Get_Cut_Stage8_A3onA2_Low   () { return m_Cut_Stage8_A3onA2_Low   ; }
void   JFL_Selector::Set_Cut_Stage8_A3onA1_Hig   (double Cut_Stage8_A3onA1_Hig   ) { m_Cut_Stage8_A3onA1_Hig   = Cut_Stage8_A3onA1_Hig   ; }
void   JFL_Selector::Set_Cut_Stage8_A3onA1_Low   (double Cut_Stage8_A3onA1_Low   ) { m_Cut_Stage8_A3onA1_Low   = Cut_Stage8_A3onA1_Low   ; }
void   JFL_Selector::Set_Cut_Stage8_A2onA1_Low   (double Cut_Stage8_A2onA1_Low   ) { m_Cut_Stage8_A2onA1_Low   = Cut_Stage8_A2onA1_Low   ; }
void   JFL_Selector::Set_Cut_Stage8_A3onA2_Low   (double Cut_Stage8_A3onA2_Low   ) { m_Cut_Stage8_A3onA2_Low   = Cut_Stage8_A3onA2_Low   ; }
void JFL_Selector::Stage8_Def()
{
  std::cout << "Apply selection  8 -> Remove Clusters with too high or too low A3/A1 or too low A2/A1 or too low A3/A2" << std::endl ;

  std::cout << "                      A3/A1 <= m_Cut_Stage8_A3onA1_Hig (= " << std::setw(5) << std::setprecision(2) << m_Cut_Stage8_A3onA1_Hig << ")" << std::endl ;
  std::cout << "                      A3/A1 >= m_Cut_Stage8_A3onA1_Low (= " << std::setw(5) << std::setprecision(2) << m_Cut_Stage8_A3onA1_Low << ")" << std::endl ;
  std::cout << "                      A2/A1 >= m_Cut_Stage8_A2onA1_Low (= " << std::setw(5) << std::setprecision(2) << m_Cut_Stage8_A2onA1_Low << ")" << std::endl ;
  std::cout << "                      A3/A2 >= m_Cut_Stage8_A3onA2_Low (= " << std::setw(5) << std::setprecision(2) << m_Cut_Stage8_A3onA2_Low << ")" << std::endl ;
}
void JFL_Selector::Stage8(Sample& aSample, const int& ModuleNber)
{
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage8 ( pEvent, ModuleNber);
  }
  
}
void JFL_Selector::Stage8(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;  
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
      double A2 = std::max(AU, AD) ;
      if ( A3    <  m_Cut_Stage8_A3onA1_Low ) RemoveCluster = 1 ;
      if ( A3    >  m_Cut_Stage8_A3onA1_Hig ) RemoveCluster = 1 ;
      if ( A2    <  m_Cut_Stage8_A2onA1_Low ) RemoveCluster = 1 ;
      if ( A3/A2 <  m_Cut_Stage8_A3onA2_Low ) RemoveCluster = 1 ;
    }else{
      if (AU_Found==1 || AD_Found==1 ){
        double A2 = AU ;
        if (AD_Found==1) A2 = AD ;
        if ( A2 <  m_Cut_Stage8_A2onA1_Low ) RemoveCluster = 1 ;
      }
    }
    if (RemoveCluster==1) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
            
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

}

//----------------------------------------------------------------//
// Stage 9: Remove clusters with too few pads 
double JFL_Selector::Get_Cut_Stage9_Npads_Low    () { return m_Cut_Stage9_Npads_Low    ; }
void   JFL_Selector::Set_Cut_Stage9_Npads_Low    (double Cut_Stage9_Npads_Low    ) { m_Cut_Stage9_Npads_Low    = Cut_Stage9_Npads_Low    ; }
void JFL_Selector::Stage9_Def()
{
  std::cout << "Apply selection  9 -> Remove clusters with too few pads " << std::endl ;
  
  std::cout << "                      Npads > m_Cut_Stage9_Npads_Low (= " << std::setw(3) << std::setprecision(0) << m_Cut_Stage9_Npads_Low << ")" << std::endl ;
}
void JFL_Selector::Stage9(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage9(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage9(Event*  pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster ;

  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];

    int NPads = pCluster->Get_NberOfPads() ;
    if(NPads<=m_Cut_Stage9_Npads_Low) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;

}

//----------------------------------------------------------------//
// Stage 10: Remove clusters with too many pads 
double JFL_Selector::Get_Cut_Stage10_Npads_Hig   () { return m_Cut_Stage10_Npads_Hig   ; }
void   JFL_Selector::Set_Cut_Stage10_Npads_Hig   (double Cut_Stage10_Npads_Hig   ) { m_Cut_Stage10_Npads_Hig   = Cut_Stage10_Npads_Hig   ; }
void JFL_Selector::Stage10_Def()
{
  std::cout << "Apply selection 10 -> Remove clusters with too many pads " << std::endl ;
  std::cout << "                      Npads <= m_Cut_Stage10_Npads_Hig (= " << std::setw(3) << std::setprecision(0) << m_Cut_Stage10_Npads_Hig << ")" << std::endl ;
}
void JFL_Selector::Stage10(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage10(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::Stage10(Event*  pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster ;

  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];

    int NPads = pCluster->Get_NberOfPads() ;
    if(NPads>m_Cut_Stage10_Npads_Hig) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 11: Remove clusters out of time (Event Based)
double JFL_Selector::Get_Cut_Stage11_EventBased  () { return m_Cut_Stage11_EventBased  ; }
void   JFL_Selector::Set_Cut_Stage11_EventBased  (double Cut_Stage11_EventBased  ) { m_Cut_Stage11_EventBased  = Cut_Stage11_EventBased  ; }
void JFL_Selector::Stage11_Def()
{
  std::cout << "Apply selection 11 -> Remove clusters out of time (Event Based) " << std::endl ;
  
  std::cout << "                      |TLeading -TLeadingMeanOverEvent| <= m_Cut_Stage11_EventBased (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage11_EventBased << ")" << std::endl ;
}
void JFL_Selector::Stage11(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage11(pEvent,ModuleNber);
  }

}
void JFL_Selector::Stage11(Event*  pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber)==0) return ;
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
    if(std::fabs(DeltaT) > m_Cut_Stage11_EventBased) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 12: Remove clusters out of time (Run Based)
double JFL_Selector::Get_Cut_Stage12_RunBased    () { return m_Cut_Stage12_RunBased    ; }
void   JFL_Selector::Set_Cut_Stage12_RunBased    (double Cut_Stage12_RunBased    ) { m_Cut_Stage12_RunBased    = Cut_Stage12_RunBased    ; }
void JFL_Selector::Stage12_Def()
{
  std::cout << "Apply selection 12 -> Remove clusters out of time (Run Based)" << std::endl ;
  
  std::cout << "                      |TLeading -TLeadingMOPOverRun| <= m_Cut_Stage12_RunBased (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage12_RunBased << ")" << std::endl ;
}
void JFL_Selector::Stage12(Sample& aSample, const int& ModuleNber)
{  
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
      if (TLeading>10) pTH1I->Fill(TLeading) ;
    }
  }

  int TLeadingMOPOverRun = pTH1I->GetMaximumBin()-1 ;
  std::cout << "Apply selection 12 -> Additional information" << std::endl ;
  std::cout << "                                                            TLeadingMOPOverRun (= " << std::setw(4) << std::setprecision(0) << TLeadingMOPOverRun           << ")" << std::endl ;
  delete pTH1I ;
  
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
    std::vector<Cluster*> V_pCluster ;
    
    std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
    int NClusters = ClusterSet.size() ;
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = ClusterSet[iC];
      
      int Delta_TLeading = pCluster->Get_TMaxLeading() - TLeadingMOPOverRun ;
      if(std::fabs(Delta_TLeading) > m_Cut_Stage12_RunBased) continue ;
      
      Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluster_new) ;
      
    }
    pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
  }

}

//----------------------------------------------------------------//
// Stage 120: Remove clusters out of time  
double JFL_Selector::Get_Cut_Stage120_TLow       () { return m_Cut_Stage120_TLow       ; }
double JFL_Selector::Get_Cut_Stage120_THigh      () { return m_Cut_Stage120_THigh      ; }
void   JFL_Selector::Set_Cut_Stage120_TLow       (double Cut_Stage120_TLow       ) { m_Cut_Stage120_TLow       = Cut_Stage120_TLow       ; }
void   JFL_Selector::Set_Cut_Stage120_THigh      (double Cut_Stage120_THigh      ) { m_Cut_Stage120_THigh      = Cut_Stage120_THigh      ; }
void JFL_Selector::Stage120_Def()
{
  std::cout << "Apply selection 120 -> Remove clusters out of time  " << std::endl ;
  
  std::cout << "                      m_Cut_Stage120_TLow <= TLeading <= m_Cut_Stage120_THigh " << std::endl ;
  std::cout << "                      m_Cut_Stage120_TLow  = " << std::setw(4) << std::setprecision(0) << m_Cut_Stage120_TLow  << std::endl ;
  std::cout << "                      m_Cut_Stage120_THigh = " << std::setw(4) << std::setprecision(0) << m_Cut_Stage120_THigh << std::endl ;
}
void JFL_Selector::Stage120(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents ; iE++){
    Event* pEvent = aSample.Get_Event(iE) ;
    Stage120(pEvent,ModuleNber) ;
  }

}
void JFL_Selector::Stage120(Event*  pEvent , const int& ModuleNber)
{  
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster ;
  
  std::vector < Cluster* >ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    
    int TLeading = pCluster->Get_TMaxLeading() ;
    if(TLeading > m_Cut_Stage120_THigh) continue ;
    if(TLeading < m_Cut_Stage120_TLow ) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
    
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 13: Remove Clusters with too small Tpad-TLeading 
double JFL_Selector::Get_Cut_Stage13_DT_Low      () { return m_Cut_Stage13_DT_Low      ; }
void   JFL_Selector::Set_Cut_Stage13_DT_Low      (double Cut_Stage13_DT_Low      ) { m_Cut_Stage13_DT_Low      = Cut_Stage13_DT_Low      ; }
void JFL_Selector::Stage13_Def()
{
  std::cout << "Apply selection 13 -> Remove Clusters with too small Tpad-TLeading " << std::endl ;

  std::cout << "                      |TPad -TLeading| <= m_Cut_Stage13_DT_Low (= " << std::setw(4) << std::setprecision(0) << m_Cut_Stage13_DT_Low << ")" << std::endl ;
}
void JFL_Selector::Stage13(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage13(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::Stage13(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;  
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
      if (DeltaT<m_Cut_Stage13_DT_Low) RemoveCluster = 1 ;
      
      if (RemoveCluster==1) break ;
      
    }
    
    if (RemoveCluster==1) continue ;
    
    Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
    V_pCluster.push_back(pCluster_new) ;
            
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 14: Remove Clusters with a too high APad 
double JFL_Selector::Get_Cut_Stage14_Amax_Low    () { return m_Cut_Stage14_Amax_Low    ; }
void   JFL_Selector::Set_Cut_Stage14_Amax_Low    (double Cut_Stage14_Amax_Low    ) { m_Cut_Stage14_Amax_Low    = Cut_Stage14_Amax_Low    ; }
double JFL_Selector::Get_Cut_Stage14_Amax_Hig    () { return m_Cut_Stage14_Amax_Hig    ; }
void   JFL_Selector::Set_Cut_Stage14_Amax_Hig    (double Cut_Stage14_Amax_Hig    ) { m_Cut_Stage14_Amax_Hig    = Cut_Stage14_Amax_Hig    ; }
void JFL_Selector::Stage14_Def()
{
  std::cout << "Apply selection 14 -> Remove Clusters with a too high APad " << std::endl ;
  
  std::cout << "                      APad >= m_Cut_Stage14_Amax_Low (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage14_Amax_Low << ")" << std::endl ;
  std::cout << "                      APad <= m_Cut_Stage14_Amax_Hig (= " << std::setw(5) << std::setprecision(0) << m_Cut_Stage14_Amax_Hig << ")" << std::endl ;
}
void JFL_Selector::Stage14(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage14(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage14(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector<Cluster*> V_pCluster ;

  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    Cluster* pCluster_new = new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber() ) ;

    int RemoveCluster = 0 ;

    int NPads = pCluster->Get_NberOfPads() ;
    for (int iP = 0 ; iP < NPads ; iP++){ 
      const Pad* pPad = pCluster->Get_Pad(iP);  

      double Amax = pPad ->Get_AMax() ;
      if (Amax > m_Cut_Stage14_Amax_Low) pCluster_new->Add_Pad(pPad) ;
      if (Amax > m_Cut_Stage14_Amax_Hig) RemoveCluster = 1 ;
      if (RemoveCluster==1) break ;
    
    }
    if (RemoveCluster==1 or pCluster_new->Get_NberOfPads() == 0) continue ;
    pCluster_new->DoClosure() ;
    V_pCluster.push_back(pCluster_new) ;
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 15: Remove Events with an average pad multiplicity too high or too low
double JFL_Selector::Get_Cut_StageT15_APM_Low       () { return m_Cut_StageT15_APM_Low      ; }
void   JFL_Selector::Set_Cut_StageT15_APM_Low       (double Cut_StageT15_APM_Low      ) { m_Cut_StageT15_APM_Low        = Cut_StageT15_APM_Low      ; }
double JFL_Selector::Get_Cut_StageT15_APM_High      () { return m_Cut_StageT15_APM_High      ; }
void   JFL_Selector::Set_Cut_StageT15_APM_High      (double Cut_StageT15_APM_High      ) { m_Cut_StageT15_APM_High      = Cut_StageT15_APM_High      ; }
void JFL_Selector::StageT15_Def()
{
  std::cout << "Apply selection 15 -> Remove Events with an average pad multiplicity too high or too low " << std::endl ;

  std::cout << "                      Pad Multiplicity >  m_Cut_StageT15_APM_Low  (= " << std::setw(4) << std::setprecision(1) << m_Cut_StageT15_APM_Low << ")" << std::endl ;
  std::cout << "                      Pad Multiplicity <= m_Cut_StageT15_APM_High (= " << std::setw(4) << std::setprecision(1) << m_Cut_StageT15_APM_High << ")" << std::endl ;
}
void JFL_Selector::StageT15(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT15(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::StageT15(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;  
   
  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  float NClusters = ClusterSet.size() ;
  float NPads_in_Evt = 0 ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    NPads_in_Evt += pCluster->Get_NberOfPads() ;           
  }
  float PadMulti = NPads_in_Evt/NClusters ;
  if(PadMulti <= m_Cut_StageT15_APM_Low or PadMulti > m_Cut_StageT15_APM_High) pEvent->Invalidate_ThisModule(ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 16: Remove Clusters with an pad integral value too low for the Leading Pad
double  JFL_Selector::Get_Cut_StageT16_PadIntegral  () { return m_Cut_StageT16_PadIntegral      ; }
void    JFL_Selector::Set_Cut_StageT16_PadIntegral  (double Cut_StageT16_PadIntegral) { m_Cut_StageT16_PadIntegral = Cut_StageT16_PadIntegral ; }
void    JFL_Selector::StageT16_Def()
{
  std::cout << "Apply selection 16 -> Remove Clusters with an pad integral value too low" << std::endl ;

  std::cout << "                      Pad Integral > m_Cut_StageT16_PadIntegral  (= " << std::setw(4) << std::setprecision(0) << m_Cut_StageT16_PadIntegral << ")" << std::endl ;
}
void JFL_Selector::StageT16(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT16(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::StageT16(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector<Cluster*> V_pCluster ;

  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters       = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    TH1F* pTH1F_WF  = GiveMe_WaveFormDisplay(pCluster->Get_LeadingPad(), "dummy") ;
    double pad_integral = pTH1F_WF->Integral() ;
    if (pad_integral >= m_Cut_StageT16_PadIntegral){
      Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluster_new) ;
    }
    delete pTH1F_WF ;
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 17: Remove pads with a pad integral value too low and discard cluster if integral of leading pad is below threshold
double  JFL_Selector::Get_Cut_StageT17_PadIntegral  () { return m_Cut_StageT17_PadIntegral      ; }
void    JFL_Selector::Set_Cut_StageT17_PadIntegral  (double Cut_StageT17_PadIntegral) { m_Cut_StageT17_PadIntegral = Cut_StageT17_PadIntegral ; }
void    JFL_Selector::StageT17_Def()
{
  std::cout << "Apply selection 17 -> Remove pads with an pad integral value too low" << std::endl ;

  std::cout << "                      Pad Integral > m_Cut_StageT17_PadIntegral  (= " << std::setw(4) << std::setprecision(0) << m_Cut_StageT17_PadIntegral << ")" << std::endl ;
}
void JFL_Selector::StageT17(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT17(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::StageT17(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector<Cluster*> V_pCluster ;

  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters           = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster     = ClusterSet[iC];
    Cluster* pCluster_new = new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber() ) ;

    int NPads             = pCluster->Get_NberOfPads() ;
    double leadpad_int = 0 ;
    for (int iP = 0 ; iP < NPads ; iP++){ 
      const Pad* pPad     = pCluster->Get_Pad(iP);
      
      TH1F* pTH1F_WF      = GiveMe_WaveFormDisplay(pPad, "dummy") ;
      double pad_integral = pTH1F_WF->Integral() ;
      if(pPad == pCluster->Get_LeadingPad()) leadpad_int = pTH1F_WF->Integral() ;
      if(pad_integral >= m_Cut_StageT17_PadIntegral) pCluster_new->Add_Pad(pPad) ;
      delete pTH1F_WF ;
    }
    if(leadpad_int >= 0){
      pCluster_new->DoClosure() ;
      V_pCluster.push_back(pCluster_new) ;
    }
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}


//----------------------------------------------------------------//
// Stage 18: Remove holes in clusters
void JFL_Selector::StageT18_Def()
{
  std::cout << "Apply selection  18 -> Remove holes in clusters" << std::endl ;
}
void JFL_Selector::StageT18(Sample& aSample, const int& ModuleNber)
{
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT18(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::StageT18(Event* pEvent, const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  std::vector<Cluster*> V_pCluster ;
  std::vector<Cluster*> ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;  

  int NClusters           = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster     = ClusterSet[iC];
    Cluster* pCluster_new = new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber()) ;
    int NPads             = pCluster->Get_NberOfPads() ;

    std::vector<int> v_iY ;
    std::vector<int> v_iP ;
    std::vector<int> v_isPad(36, 0) ;
    int iY_low            = 35 ;
    int iY_high           = 0 ;
    int iY_lead           = pCluster->Get_LeadingPad()->Get_iY() ;
    for (int iP = 0 ; iP < NPads ; iP++){
      const Pad* pPad     = pCluster->Get_Pad(iP) ;
      double iY           = pPad->Get_iY() ;

      v_isPad[iY]         = 1 ;
      if(iY < iY_low) iY_low    = iY ;
      if(iY > iY_high) iY_high  = iY ;

      v_iY.push_back(iY) ;
      v_iP.push_back(iP) ;
    }

    for (int iY = iY_lead-1 ; iY > iY_low-1 ; iY--){ // run through pads below Leading
      if(v_isPad[iY] != 1) break ; // stop if a hole is encountered
      else{
        std::vector<int>::iterator itr = std::find(v_iY.begin(), v_iY.end(), iY);
        int pad_index = std::distance(v_iY.begin(), itr) ;
        if(v_isPad[iY] == 1)  pCluster_new->Add_Pad(pCluster->Get_Pad(pad_index)) ; // add pad to new cluster
      }
    }

    for (int iY = iY_lead ; iY < iY_high+1 ; iY++){
      if(v_isPad[iY] != 1) break ; // stop if a hole is encountered
      else{
        std::vector<int>::iterator itr = std::find(v_iY.begin(), v_iY.end(), iY);
        int pad_index = std::distance(v_iY.begin(), itr) ;
        if(v_isPad[iY] == 1)  pCluster_new->Add_Pad(pCluster->Get_Pad(pad_index)) ;
      }
    }


    if(pCluster_new->Get_NberOfPads() != 0){
      pCluster_new->DoClosure() ;
      V_pCluster.push_back(pCluster_new) ;
    }
    for (int iP = 0 ; iP < NPads ; iP++){
      v_iY[iP] = 0    ; v_iP[iP] = 0 ;
      }
    v_iY.clear() ;
    v_iP.clear() ;
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 19: Remove pads with a too big positive derivative between 2 bins with the first bin < 0
double  JFL_Selector::Get_Cut_StageT19_BinDerivative  () { return m_Cut_StageT19_BinDerivative ; }
void    JFL_Selector::Set_Cut_StageT19_BinDerivative  (double Cut_StageT19_BinDerivative) { m_Cut_StageT19_BinDerivative = Cut_StageT19_BinDerivative ; }
void    JFL_Selector::StageT19_Def()
{
  std::cout << "Apply selection 19 -> Remove pads with a too big positive derivative between 2 bins with the first bin < 0" << std::endl ;

  std::cout << "                      Maximum bin derivative > m_Cut_StageT19_BinDerivative  (= " << std::setw(4) << std::setprecision(0) << m_Cut_StageT19_BinDerivative << ")" << std::endl ;
}
void JFL_Selector::StageT19(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT19(pEvent, ModuleNber);
  }
  
}
void JFL_Selector::StageT19(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector<Cluster*> V_pCluster ;

  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters           = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster     = ClusterSet[iC];
    Cluster* pCluster_new = new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber() ) ;

    int NPads             = pCluster->Get_NberOfPads() ;
    const Pad* pLeading   = pCluster->Get_LeadingPad() ;
    int discard_leading = 0 ;
    for (int iP = 0 ; iP < NPads ; iP++){ 
      const Pad* pPad     = pCluster->Get_Pad(iP);
      
      TH1F* pTH1F_WF      = GiveMe_WaveFormDisplay(pPad, "dummy") ;
      int nBin            = pTH1F_WF->GetNbinsX() ;
      int discard         = 0 ;
      for(int iB = 50 ; iB < nBin-50 ; iB++){
        float bin_val0    = pTH1F_WF->GetBinContent(iB) ;
        float bin_val1    = pTH1F_WF->GetBinContent(iB+1) ;
        if(bin_val0 < 0 and bin_val1 - bin_val0 > m_Cut_StageT19_BinDerivative) discard = 1 ;
      }
      if(discard == 1 and pPad == pLeading) discard_leading = 1 ;
      if(discard == 0) pCluster_new->Add_Pad(pPad) ;
      delete pTH1F_WF ;
    }
    if(pCluster_new->Get_NberOfPads() != 0 and discard_leading != 1){
      pCluster_new->DoClosure() ;
      V_pCluster.push_back(pCluster_new) ;
    }
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
// Stage 20: Remove delta rays off track
double  JFL_Selector::Get_Cut_StageT20_Amax_dRay  () { return m_Cut_StageT20_Amax_dRay      ; }
void    JFL_Selector::Set_Cut_StageT20_Amax_dRay  (double Cut_StageT20_Amax_dRay) { m_Cut_StageT20_Amax_dRay = Cut_StageT20_Amax_dRay ; }
double  JFL_Selector::Get_Cut_StageT20_deltaY_dRay  () { return m_Cut_StageT20_deltaY_dRay      ; }
void    JFL_Selector::Set_Cut_StageT20_deltaY_dRay  (double Cut_StageT20_deltaY_dRay) { m_Cut_StageT20_deltaY_dRay = Cut_StageT20_deltaY_dRay ; }
void    JFL_Selector::StageT20_Def()
{
  std::cout << "Apply selection 20 -> Remove delta rays off track" << std::endl ;
  std::cout << "                      |y_track - y_pad| > m_Cut_StageT20_Amax_dRay   (= " << std::setw(4) << std::setprecision(0) << m_Cut_StageT20_Amax_dRay << ")" << std::endl ;
  std::cout << "                      |y_track - y_pad| > m_Cut_StageT20_deltaY_dRay (= " << std::setw(4) << std::setprecision(0) << m_Cut_StageT20_deltaY_dRay << ")" << std::endl ;
}
void JFL_Selector::StageT20(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT20(pEvent,ModuleNber);
  }
  
}
void JFL_Selector::StageT20(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  Module* pModule                 = pEvent->Get_Module_InArray(ModuleNber) ;
  std::vector<Cluster*> V_pCluster ;
  std::vector<Cluster*> ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters                   = ClusterSet.size() ;

  TH2D* pTH2D_Evt                 = GiveMe_EvtDisplay(pModule, "Selection_StageT20") ;
  TGraphErrors* pTGE_Evt          = Convert_TH2_TGE_v2(pTH2D_Evt, 900) ;
  TF1* pTF1_Track                 = new TF1("track_selection_stage_20", "[0] + [1]*x", -0.5, 36.5) ;
  float x1 = 0, x2 = 0, y1 = 0, y2 = 0 ;
  for(int iC = 0                 ; iC < NClusters/2 ; iC++) {x1 += (pModule->Get_Cluster(iC)->Get_LeadingPad()->Get_iX()+0.5)*1.128 ; y1 += pModule->Get_Cluster(iC)->Get_LeadingPad()->Get_YPad()*100 ; }
  for(int iC = ceil(NClusters/2) ; iC < NClusters   ; iC++) {x2 += (pModule->Get_Cluster(iC)->Get_LeadingPad()->Get_iX()+0.5)*1.128 ; y2 += pModule->Get_Cluster(iC)->Get_LeadingPad()->Get_YPad()*100 ; }
  x1 /= floor(NClusters/2) ; y1 /= floor(NClusters/2) ; x2 /= ceil(NClusters/2) ; y2 /= ceil(NClusters/2) ; 
  pTF1_Track->                      SetParameters(x1, (y2-y1)/(x2-x1)) ;
  pTGE_Evt->                        Fit(pTF1_Track, "RQ", "") ;

  for (int iC = 0 ; iC < NClusters; iC++){
    int discard_cluster   = 0 ;
    Cluster* pCluster     = ClusterSet[iC];

    int NPads             = pCluster->Get_NberOfPads() ;
    for (int iP = 0 ; iP < NPads ; iP++){ 
      const Pad* pPad     = pCluster->Get_Pad(iP);
      float A_pad         = pPad->Get_AMax() ;
      float delta_y       = pTF1_Track->Eval(pPad->Get_iX())*1.019-pPad->Get_iY()*1.019 ;
      if(A_pad > m_Cut_StageT20_Amax_dRay and fabs(delta_y) > m_Cut_StageT20_deltaY_dRay){
        discard_cluster = 1 ;
        break ;
      }
    }
    if(discard_cluster == 0){
      Cluster* pCluster_new = pEvent->Get_Cluster_Copy(pCluster) ;
      V_pCluster.push_back(pCluster_new) ;
    }
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
  delete pTH2D_Evt ;
  delete pTGE_Evt ;
  delete pTF1_Track ;
}

//----------------------------------------------------------------//
// Stage 21: Remove ghost pads
double  JFL_Selector::Get_Cut_StageT21_GhostPads  () { return m_Cut_StageT21_Rho3vsRho2 ; }
void    JFL_Selector::Set_Cut_StageT21_GhostPads  (double Cut_StageT21_GhostPads) { m_Cut_StageT21_Rho3vsRho2 = Cut_StageT21_GhostPads ; }
void    JFL_Selector::StageT21_Def()
{
  std::cout << "Apply selection 21 -> Remove ghost pads" << std::endl ;

  std::cout << "                      A(1st non-zero bin) / Amax(pad) > m_Cut_StageT21_Rho3vsRho2  (= " << std::setw(6) << std::setprecision(3) << m_Cut_StageT21_Rho3vsRho2 << ")" << std::endl ;
}
void JFL_Selector::StageT21(Sample& aSample, const int& ModuleNber)
{  
  int NberOfEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE < NberOfEvents; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    StageT21(pEvent, ModuleNber);
  }
  
}
void JFL_Selector::StageT21(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector<Cluster*> V_pCluster ;

  std::vector<Cluster*>ClusterSet = pEvent->GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters           = ClusterSet.size() ;
  for (int iC = 0 ; iC < NClusters; iC++){
    Cluster* pCluster     = ClusterSet[iC];
    Cluster* pCluster_new = new Cluster(pCluster->Get_EventNber (), pCluster->Get_EntryNber (), pCluster->Get_ModuleNber() ) ;
    int NPads                       = pCluster->Get_NberOfPads() ;

    pCluster_new->                    Add_Pad(pCluster->Get_Pad(NPads-1)) ;
    if(NPads > 1) pCluster_new->      Add_Pad(pCluster->Get_Pad(NPads-2)) ;
    float rho2 = 0 ;

    TH1F* pTH1F_clus                = new TH1F("pTH1F_WF_cluster", "pTH1F_WF_cluster", 510, -0.5, 509.5) ;
    for(int iP = 0 ; iP < NPads ; iP++){
      const Pad* pPad               = pCluster->Get_Pad(iP) ;
      TH1F* pTH1F_WF                = GiveMe_WaveFormDisplay(pPad, "dummy") ;
      pTH1F_clus->                    Add(pTH1F_WF) ;
      delete                          pTH1F_WF ;
    }
    float max_WF_clus               = pTH1F_clus->GetMaximum() ;
    if(NPads > 1) rho2              = pCluster->Get_Pad(NPads-2)->Get_vADC()[pTH1F_clus->GetMaximumBin()]/max_WF_clus ;

    for(int iP = 0 ; iP < NPads-2 ; iP++){
      float rho3plus                = pCluster->Get_Pad(iP)->Get_vADC()[pTH1F_clus->GetMaximumBin()]/max_WF_clus ;
      if(rho3plus/rho2 < m_Cut_StageT21_Rho3vsRho2) continue ;
      pCluster_new->Add_Pad(pCluster->Get_Pad(iP)) ;
    }
    delete pTH1F_clus ;
    pCluster_new->DoClosure() ;
    V_pCluster.push_back(pCluster_new) ;
  }
  pEvent->Replace_Clusters_ForThisModule(V_pCluster, ModuleNber) ;
}

//----------------------------------------------------------------//
void JFL_Selector::Set_BN_COSMICS23_Sample()
{
  Reset_Selection() ;

  std::cout << std::endl ;
  std::cout << "-------------->Predefined selection BN_COSMICS23_Sample" << std::endl ;
  ListOfSelectionName.clear();
// 1+14+12+11+6+13+7+8  +9+10+5+15+16
  Add_Selection( "Stage1"  );
  Add_Selection( "Stage14" );
  Add_Selection( "Stage12" );
  Add_Selection( "Stage11" );
  Add_Selection( "Stage6"  );
  Add_Selection( "Stage13" );
  Add_Selection( "Stage7"  );
  Add_Selection( "Stage8"  );
  Add_Selection( "Stage9"  );
  Add_Selection( "Stage10" );
  Add_Selection( "Stage5"  );
  Add_Selection( "Stage15" );
  Add_Selection( "Stage16" );
  Add_Selection( "Stage17" );
//Add_Selection( "Stage18" );
  
  m_Cut_Stage12_RunBased   = 100000. ;
  m_Cut_Stage11_EventBased = 150. ;

  Tell_Selection();
}

//----------------------------------------------------------------//
// Stage 15: Remove events with a too high or too low CutSlopeXY 
double JFL_Selector::Get_Cut_Stage15_CutSlopeXY_Hig    () { return m_Cut_Stage15_CutSlopeXY_Hig    ; }
double JFL_Selector::Get_Cut_Stage15_CutSlopeXY_Low    () { return m_Cut_Stage15_CutSlopeXY_Low    ; }
void   JFL_Selector::Set_Cut_Stage15_CutSlopeXY_Hig    (double Cut_Stage15_CutSlopeXY_Hig ) { m_Cut_Stage15_CutSlopeXY_Hig = Cut_Stage15_CutSlopeXY_Hig ; }
void   JFL_Selector::Set_Cut_Stage15_CutSlopeXY_Low    (double Cut_Stage15_CutSlopeXY_Low ) { m_Cut_Stage15_CutSlopeXY_Low = Cut_Stage15_CutSlopeXY_Low ; }
void JFL_Selector::Stage15_Def()
{
  std::cout << "Apply selection 15 -> Remove events with a too high or too low CutSlopeXY " << std::endl ;
  
  std::cout << "                      m_Cut_Stage15_CutSlopeXY_Low <= CutSlopeXY <= m_Cut_Stage15_CutSlopeXY_Hig " << std::endl ;
  std::cout << "                      m_Cut_Stage15_CutSlopeXY_Hig = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage15_CutSlopeXY_Hig << std::endl ;
  std::cout << "                      m_Cut_Stage15_CutSlopeXY_Low = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage15_CutSlopeXY_Low << std::endl ;
}
void JFL_Selector::Stage15(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage15(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage15(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  double CutSlopeXY = pEvent->GiveMe_CutSlopeXY_ForThisModule(ModuleNber) ;  
  if(CutSlopeXY < m_Cut_Stage15_CutSlopeXY_Low) pEvent->Invalidate_ThisModule(ModuleNber) ; 
  if(CutSlopeXY > m_Cut_Stage15_CutSlopeXY_Hig) pEvent->Invalidate_ThisModule(ModuleNber) ; 
}

//----------------------------------------------------------------//
// Stage 16: Remove events with a too high or too low CutSlopeXZ 
double JFL_Selector::Get_Cut_Stage16_CutSlopeXZ_Hig    () { return m_Cut_Stage16_CutSlopeXZ_Hig    ; }
double JFL_Selector::Get_Cut_Stage16_CutSlopeXZ_Low    () { return m_Cut_Stage16_CutSlopeXZ_Low    ; }
void   JFL_Selector::Set_Cut_Stage16_CutSlopeXZ_Hig    (double Cut_Stage16_CutSlopeXZ_Hig ) { m_Cut_Stage16_CutSlopeXZ_Hig = Cut_Stage16_CutSlopeXZ_Hig ; }
void   JFL_Selector::Set_Cut_Stage16_CutSlopeXZ_Low    (double Cut_Stage16_CutSlopeXZ_Low ) { m_Cut_Stage16_CutSlopeXZ_Low = Cut_Stage16_CutSlopeXZ_Low ; }
void JFL_Selector::Stage16_Def()
{
  std::cout << "Apply selection 16 -> Remove events with a too high or too low CutSlopeXZ " << std::endl ;
  
  std::cout << "                      m_Cut_Stage16_CutSlopeXZ_Low <= CutSlopeXZ <= m_Cut_Stage16_CutSlopeXZ_Hig " << std::endl ;
  std::cout << "                      m_Cut_Stage16_CutSlopeXZ_Hig = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage16_CutSlopeXZ_Hig << std::endl ;
  std::cout << "                      m_Cut_Stage16_CutSlopeXZ_Low = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage16_CutSlopeXZ_Low << std::endl ;
}
void JFL_Selector::Stage16(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage16(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage16(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  double CutSlopeXZ = pEvent->GiveMe_CutSlopeXZ_ForThisModule(ModuleNber) ;  
  if(CutSlopeXZ < m_Cut_Stage16_CutSlopeXZ_Low) pEvent->Invalidate_ThisModule(ModuleNber) ; 
  if(CutSlopeXZ > m_Cut_Stage16_CutSlopeXZ_Hig) pEvent->Invalidate_ThisModule(ModuleNber) ; 
}

//----------------------------------------------------------------//
// Stage 17: Remove events with a too high or too low CutInterCeptXZ 
double JFL_Selector::Get_Cut_Stage17_CutInterCeptXZ_Hig    () { return m_Cut_Stage17_CutInterCeptXZ_Hig    ; }
double JFL_Selector::Get_Cut_Stage17_CutInterCeptXZ_Low    () { return m_Cut_Stage17_CutInterCeptXZ_Low    ; }
void   JFL_Selector::Set_Cut_Stage17_CutInterCeptXZ_Hig    (double Cut_Stage17_CutInterCeptXZ_Hig ) { m_Cut_Stage17_CutInterCeptXZ_Hig = Cut_Stage17_CutInterCeptXZ_Hig ; }
void   JFL_Selector::Set_Cut_Stage17_CutInterCeptXZ_Low    (double Cut_Stage17_CutInterCeptXZ_Low ) { m_Cut_Stage17_CutInterCeptXZ_Low = Cut_Stage17_CutInterCeptXZ_Low ; }
void JFL_Selector::Stage17_Def()
{
  std::cout << "Apply selection 17 -> Remove events with a too high or too low CutInterCeptXZ " << std::endl ;
  
  std::cout << "                      m_Cut_Stage17_CutInterCeptXZ_Low <= CutInterCeptXZ <= m_Cut_Stage17_CutInterCeptXZ_Hig " << std::endl ;
  std::cout << "                      m_Cut_Stage17_CutInterCeptXZ_Hig = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage17_CutInterCeptXZ_Hig << std::endl ;
  std::cout << "                      m_Cut_Stage17_CutInterCeptXZ_Low = " << std::setw(5) << std::setprecision(0) << m_Cut_Stage17_CutInterCeptXZ_Low << std::endl ;
}
void JFL_Selector::Stage17(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage17(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage17(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  double CutInterCeptXZ = pEvent->GiveMe_CutInterCeptXZ_ForThisModule(ModuleNber) ;  
  if(CutInterCeptXZ < m_Cut_Stage17_CutInterCeptXZ_Low) pEvent->Invalidate_ThisModule(ModuleNber) ; 
  if(CutInterCeptXZ > m_Cut_Stage17_CutInterCeptXZ_Hig) pEvent->Invalidate_ThisModule(ModuleNber) ; 
}

//----------------------------------------------------------------//
// Stage 18: Remove events with a too high or too low CutInterCeptXZ 
double JFL_Selector::Get_Cut_Stage18_v_drift_est    () { return m_Cut_Stage18_v_drift_est       ; }
double JFL_Selector::Get_Cut_Stage18_CutSlopeXYZ0   () { return m_Cut_Stage18_CutSlopeXYZ0      ; }
double JFL_Selector::Get_Cut_Stage18_CutSlopeXYZ1   () { return m_Cut_Stage18_CutSlopeXYZ1      ; }
void   JFL_Selector::Set_Cut_Stage18_v_drift_est    (double Cut_Stage18_v_drift_est    ) { m_Cut_Stage18_v_drift_est    = Cut_Stage18_v_drift_est    ; }
void   JFL_Selector::Set_Cut_Stage18_CutSlopeXYZ0   (double Cut_Stage18_CutSlopeXYZ0   ) { m_Cut_Stage18_CutSlopeXYZ0   = Cut_Stage18_CutSlopeXYZ0   ; }
void   JFL_Selector::Set_Cut_Stage18_CutSlopeXYZ1   (double Cut_Stage18_CutSlopeXYZ1   ) { m_Cut_Stage18_CutSlopeXYZ1   = Cut_Stage18_CutSlopeXYZ1   ; }
void JFL_Selector::Stage18_Def()
{
  std::cout << "Apply selection 18 -> Remove events with unconvenient CutSlopeXYZ  " << std::endl ;
  
  std::cout << "                      m_Cut_Stage18_v_drift_est    = " << std::setw(20) << std::setprecision(10) << m_Cut_Stage18_v_drift_est    << std::endl ;
  std::cout << "                      m_Cut_Stage18_CutSlopeXYZ0   = " << std::setw(20) << std::setprecision(10) << m_Cut_Stage18_CutSlopeXYZ0   << std::endl ;
  std::cout << "                      m_Cut_Stage18_CutSlopeXYZ1   = " << std::setw(20) << std::setprecision(10) << m_Cut_Stage18_CutSlopeXYZ1   << std::endl ;
}
void JFL_Selector::Stage18(Sample& aSample, const int& ModuleNber)
{  
  int NEvents = aSample.Get_NberOfEvents() ;
  for (int iE = 0 ; iE< NEvents ; iE++){
    Event* pEvent =  aSample.Get_Event(iE) ;
    Stage18(pEvent,ModuleNber) ;
  }
  
}
void JFL_Selector::Stage18(Event*  pEvent , const int& ModuleNber)
{
  if (pEvent->Validity_ForThisModule(ModuleNber) == 0) return ;
  
  std::vector <double> CutSlopeXYZ = pEvent->GiveMe_CutSlopeXYZ_ForThisModule(ModuleNber) ;

//   double theta =  TMath::ATan(CutSlopeXYZ[0]* m_Cut_Stage18_v_drift_est) / TMath::DegToRad () ;
//   double phi   =  TMath::ATan(CutSlopeXYZ[1])/ TMath::DegToRad () ;
//   std::cout <<"CutSlopeXYZ[1]  abs(CutSlopeXYZ[0])* m_Cut_Stage18_v_drift_est theta phi"<<std::setw(6) << std::setprecision(2) <<CutSlopeXYZ[0] <<" "<<abs(CutSlopeXYZ[0])* m_Cut_Stage18_v_drift_est<<" "<<CutSlopeXYZ[1]<<" "<<theta<<" "<<phi <<std::endl;


  if (CutSlopeXYZ[0] !=-999. &&  abs(CutSlopeXYZ[0])* m_Cut_Stage18_v_drift_est > m_Cut_Stage18_CutSlopeXYZ0 ) pEvent->Invalidate_ThisModule(ModuleNber) ;
  if (CutSlopeXYZ[1] !=-999. &&  abs(CutSlopeXYZ[1])                            > m_Cut_Stage18_CutSlopeXYZ1 ) pEvent->Invalidate_ThisModule(ModuleNber) ;

}

