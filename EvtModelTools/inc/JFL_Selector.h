#ifndef JFL_Selector_H
#define JFL_Selector_H

#include "Misc.h"

#include "Sample.h"

/////////////////////////////////////////////////////////////
class JFL_Selector {
public:
   /** Constructor */
   JFL_Selector(const std::string DefSelection);
   JFL_Selector();
   virtual ~JFL_Selector();

//
   void Reset_Selection() ;
   
   void Add_Selection(const std::string& SelectionName) ;
   
   int  NberOfSelections() ;
   
   std::string Get_SelectionName(const int& iTem) ;
   
   void Tell_Selection();
   
//
   void ApplySelection(Sample& aSample, const int& ModuleNber) ;
   void Apply_ASelection(Sample& aSample, const int& ModuleNber,const int& iTem) ;

//
   void ApplySelection(Event*  pEvent) ;
   void Apply_ASelection(Event*  pEvent, const int& ModuleNber,const int& iTem) ;

//
   void Reset_StatCounters() ;
   void PrintStat();
   
//
  double Get_Cut_Stage2_Npads_Low    () ;
  void   Set_Cut_Stage2_Npads_Low    (double Cut_Stage2_Npads_Low    ) ; 

  double Get_Cut_Stage5_NCluster_Low () ;
  void   Set_Cut_Stage5_NCluster_Low (double Cut_Stage5_NCluster_Low ) ; 

  double Get_Cut_Stage6_ARelat_Low   () ;
  double Get_Cut_Stage6_A_Low        () ;
  void   Set_Cut_Stage6_ARelat_Low   (double Cut_Stage6_ARelat_Low   ) ;
  void   Set_Cut_Stage6_A_Low        (double Cut_Stage6_A_Low        ) ;

  double Get_Cut_Stage7_T2T1_Low     () ;
  double Get_Cut_Stage7_T2T1_Hig     () ;
  double Get_Cut_Stage7_T3T1_Low     () ;
  double Get_Cut_Stage7_T3T1_Hig     () ;
  double Get_Cut_Stage7_T3T2_Low     () ;
  double Get_Cut_Stage7_T3T2_Hig     () ;
  void   Set_Cut_Stage7_T2T1_Low     (double Cut_Stage7_T2T1_Low     ) ;
  void   Set_Cut_Stage7_T2T1_Hig     (double Cut_Stage7_T2T1_Hig     ) ;
  void   Set_Cut_Stage7_T3T1_Low     (double Cut_Stage7_T3T1_Low     ) ;
  void   Set_Cut_Stage7_T3T1_Hig     (double Cut_Stage7_T3T1_Hig     ) ;
  void   Set_Cut_Stage7_T3T2_Low     (double Cut_Stage7_T3T2_Low     ) ;
  void   Set_Cut_Stage7_T3T2_Hig     (double Cut_Stage7_T3T2_Hig     ) ;

  double Get_Cut_Stage8_A3onA1_Hig   () ;
  double Get_Cut_Stage8_A3onA1_Low   () ;
  double Get_Cut_Stage8_A2onA1_Low   () ;
  double Get_Cut_Stage8_A3onA2_Low   () ;
  void   Set_Cut_Stage8_A3onA1_Hig   (double Cut_Stage8_A3onA1_Hig   ) ;
  void   Set_Cut_Stage8_A3onA1_Low   (double Cut_Stage8_A3onA1_Low   ) ;
  void   Set_Cut_Stage8_A2onA1_Low   (double Cut_Stage8_A2onA1_Low   ) ;
  void   Set_Cut_Stage8_A3onA2_Low   (double Cut_Stage8_A3onA2_Low   ) ;

  double Get_Cut_Stage9_Npads_Low    () ;
  void   Set_Cut_Stage9_Npads_Low    (double Cut_Stage9_Npads_Low    ) ;

  double Get_Cut_Stage10_Npads_Hig   () ;
  void   Set_Cut_Stage10_Npads_Hig   (double Cut_Stage10_Npads_Hig   ) ;
  
  double Get_Cut_Stage11_EventBased  () ;
  void   Set_Cut_Stage11_EventBased  (double Cut_Stage11_EventBased  ) ;

  double Get_Cut_Stage12_RunBased    () ;
  void   Set_Cut_Stage12_RunBased    (double Cut_Stage12_RunBased    ) ;

  double Get_Cut_Stage120_TLow       () ;
  double Get_Cut_Stage120_THigh      () ;
  void   Set_Cut_Stage120_TLow       (double Cut_Stage120_TLow       ) ;
  void   Set_Cut_Stage120_THigh      (double Cut_Stage120_THigh      ) ;

  double Get_Cut_Stage13_DT_Low      () ;
  void   Set_Cut_Stage13_DT_Low      (double Cut_Stage13_DT_Low      ) ;

  double Get_Cut_Stage14_Amax_Low    () ;
  void   Set_Cut_Stage14_Amax_Low    (double Cut_Stage14_Amax_Low    ) ;
  double Get_Cut_Stage14_Amax_Hig    () ;
  void   Set_Cut_Stage14_Amax_Hig    (double Cut_Stage14_Amax_Hig    ) ;

  double Get_Cut_Stage15_CutSlopeXY_Hig    () ;
  double Get_Cut_Stage15_CutSlopeXY_Low    () ;
  void   Set_Cut_Stage15_CutSlopeXY_Hig    (double Cut_Stage15_CutSlopeXY_Hig );
  void   Set_Cut_Stage15_CutSlopeXY_Low    (double Cut_Stage15_CutSlopeXY_Low );

  double Get_Cut_Stage16_CutSlopeXZ_Hig    () ;
  double Get_Cut_Stage16_CutSlopeXZ_Low    () ;
  void   Set_Cut_Stage16_CutSlopeXZ_Hig    (double Cut_Stage16_CutSlopeXZ_Hig );
  void   Set_Cut_Stage16_CutSlopeXZ_Low    (double Cut_Stage16_CutSlopeXZ_Low );

  double Get_Cut_Stage17_CutInterCeptXZ_Hig    () ;
  double Get_Cut_Stage17_CutInterCeptXZ_Low    () ;
  void   Set_Cut_Stage17_CutInterCeptXZ_Hig    (double Cut_Stage17_CutInterCeptXZ_Hig );
  void   Set_Cut_Stage17_CutInterCeptXZ_Low    (double Cut_Stage17_CutInterCeptXZ_Low );

  double Get_Cut_Stage18_v_drift_est    () ;
  double Get_Cut_Stage18_CutSlopeXYZ0   () ;
  double Get_Cut_Stage18_CutSlopeXYZ1   () ;
  void   Set_Cut_Stage18_v_drift_est    (double Cut_Stage18_v_drift_est  );
  void   Set_Cut_Stage18_CutSlopeXYZ0   (double Cut_Stage18_CutSlopeXYZ0 );
  void   Set_Cut_Stage18_CutSlopeXYZ1   (double Cut_Stage18_CutSlopeXYZ1 );

  double Get_Cut_StageT15_APM_Low      () ;
  void   Set_Cut_StageT15_APM_Low      (double Cut_StageT15_APM_Low    ) ;
  double Get_Cut_StageT15_APM_High     () ;
  void   Set_Cut_StageT15_APM_High     (double Cut_StageT15_APM_High   ) ;

  double Get_Cut_StageT16_PadIntegral () ;
  void   Set_Cut_StageT16_PadIntegral (double Cut_StageT16_PadIntegral      ) ;

  double Get_Cut_StageT17_PadIntegral () ;
  void   Set_Cut_StageT17_PadIntegral (double Cut_StageT17_PadIntegral      ) ;

  double Get_Cut_StageT19_BinDerivative () ;
  void   Set_Cut_StageT19_BinDerivative (double Cut_StageT19_BinDerivative) ;

  double Get_Cut_StageT20_Amax_dRay    ();
  void   Set_Cut_StageT20_Amax_dRay    (double Cut_StageT20_Amax_dRay);
  double Get_Cut_StageT20_deltaY_dRay  ();
  void   Set_Cut_StageT20_deltaY_dRay  (double Cut_StageT20_deltaY_dRay) ;

  double Get_Cut_StageT21_GhostPads () ;
  void   Set_Cut_StageT21_GhostPads (double Cut_StageT21_GhostPads) ;

private:
// Stage 1: Remove clusters in first and last columns 
   void Stage1_Def();
   void Stage1(Sample& aSample, const int& ModuleNber);
   void Stage1(Event*  pEvent , const int& ModuleNber);


// Stage 2: Remove clusters with too few pads 
   void Stage2_Def();
   void Stage2(Sample& aSample, const int& ModuleNber);
   void Stage2(Event*  pEvent , const int& ModuleNber);

// Stage 3: Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis
   void Stage3_Def();
   void Stage3(Sample& aSample, const int& ModuleNber);
   void Stage3(Event*  pEvent , const int& ModuleNber);


// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
   void Stage4_Def();
   void Stage4(Sample& aSample, const int& ModuleNber);
   void Stage4(Event*  pEvent , const int& ModuleNber);


// Stage 5: Reject Events with too few clusters
   void Stage5_Def();
   void Stage5(Sample& aSample, const int& ModuleNber);
   void Stage5(Event*  pEvent , const int& ModuleNber);


// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad keeping hot enough pads
   void Stage6_Def();
   void Stage6(Sample& aSample, const int& ModuleNber);
   void Stage6(Event*  pEvent , const int& ModuleNber);


// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
   void Stage7_Def();
   void Stage7(Sample& aSample, const int& ModuleNber);
   void Stage7(Event*  pEvent , const int& ModuleNber);


// Stage 8: Remove Clusters with too high or too low A3/A1 or too low A2/A1 or too low A3/A2
   void Stage8_Def();
   void Stage8(Sample& aSample, const int& ModuleNber);
   void Stage8(Event*  pEvent , const int& ModuleNber);


// Stage 9: Remove clusters with too few pads 
   void Stage9_Def();
   void Stage9(Sample& aSample, const int& ModuleNber);
   void Stage9(Event*  pEvent , const int& ModuleNber);


// Stage 10: Remove clusters with too many pads 
   void Stage10_Def();
   void Stage10(Sample& aSample, const int& ModuleNber);
   void Stage10(Event*  pEvent , const int& ModuleNber);


// Stage 11: Remove clusters out of time (Event Based)
   void Stage11_Def();
   void Stage11(Sample& aSample, const int& ModuleNber);
   void Stage11(Event*  pEvent , const int& ModuleNber);


// Stage 12: Remove clusters out of time (Run Based)
   void Stage12_Def();
   void Stage12(Sample& aSample, const int& ModuleNber);


// Stage 120: Remove clusters out of time 
   void Stage120_Def();
   void Stage120(Sample& aSample, const int& ModuleNber);
   void Stage120(Event*  pEvent , const int& ModuleNber);


// Stage 13: Remove Clusters with too small Tpad-TLeading 
   void Stage13_Def();
   void Stage13(Sample& aSample, const int& ModuleNber);
   void Stage13(Event*  pEvent , const int& ModuleNber);


// Stage 14: Remove Clusters with a too high APad 
   void Stage14_Def();
   void Stage14(Sample& aSample, const int& ModuleNber);
   void Stage14(Event*  pEvent , const int& ModuleNber);

// Stage 15: Remove events with a too high or too low CutSlopeXY 
   void Stage15_Def();
   void Stage15(Sample& aSample, const int& ModuleNber);
   void Stage15(Event*  pEvent , const int& ModuleNber);
   
   
// Stage 16: Remove events with a too high or too low CutSlopeXZ 
   void Stage16_Def();
   void Stage16(Sample& aSample, const int& ModuleNber);
   void Stage16(Event*  pEvent , const int& ModuleNber);
   
   
// Stage 17: Remove events with a too high or too low CutSlopeXZ 
   void Stage17_Def();
   void Stage17(Sample& aSample, const int& ModuleNber);
   void Stage17(Event*  pEvent , const int& ModuleNber);
   
   
// Stage 18: Remove events with unconvenient CutSlopeXYZ 
   void Stage18_Def();
   void Stage18(Sample& aSample, const int& ModuleNber);
   void Stage18(Event*  pEvent , const int& ModuleNber);


  // Stage 15: Remove Events with an average pad multiplicity too high or too low
  void StageT15_Def();
  void StageT15(Sample& aSample, const int& ModuleNber) ;
  void StageT15(Event*  pEvent , const int& ModuleNber) ;
   
  // Stage 16: Remove Clusters with an pad integral value too low for the Leading Pad
  void StageT16_Def() ;
  void StageT16(Sample& aSample, const int& ModuleNber) ;
  void StageT16(Event*  pEvent , const int& ModuleNber) ;
   
  // Stage 17: Remove pads with an pad integral value too low
  void StageT17_Def() ;
  void StageT17(Sample& aSample, const int& ModuleNber) ;
  void StageT17(Event*  pEvent , const int& ModuleNber) ;
   
  // Stage 18: Remove holes in clusters
  void StageT18_Def() ;
  void StageT18(Sample& aSample, const int& ModuleNber) ;
  void StageT18(Event*  pEvent , const int& ModuleNber) ;
   
  // Stage 19: Remove pads with a too big positive derivative between 2 bins with the first bin < 0
  void StageT19_Def() ;
  void StageT19(Sample& aSample, const int& ModuleNber) ;
  void StageT19(Event*  pEvent , const int& ModuleNber) ;

// Stage 20: Remove delta rays off track
   void StageT20_Def() ;
   void StageT20(Sample& aSample, const int& ModuleNber) ;
   void StageT20(Event*  pEvent , const int& ModuleNber) ;
   
  // Stage 21: Remove ghost pads
  void StageT21_Def() ;
  void StageT21(Sample& aSample, const int& ModuleNber) ;
  void StageT21(Event*  pEvent , const int& ModuleNber) ;
   
   
//Defaults
   void Set_A ()       ; // 1+2+3+4+5
   void Set_B ()       ; // 1+6+5
   void Set_BN_DESY21_Sample            () ; // 1+14+12 +11+6+13+7+8  +9+10+5
   void Set_AN_DESY21_Sample            () ; // 1+14+12 +11+6+13+7+8+4+9+10+5  
   void Set_BN_CERN22_Event             () ; // 1+14+120+11+6+13+7+8  +9+10+5
   void Set_AN_CERN22_Event             () ; // 1+14+120+11+6+13+7+8+4+9+10+5
     
   void Set_BN_CERN22_Sample            () ; // 1+14+12 +11+6+13+7+8  +9+10+5
   void Set_AN_CERN22_Sample            () ; // 1+14+12 +11+6+13+7+8+4+9+10+5  
   void Set_BN_DESY21_Event             () ; // 1+14+120+11+6+13+7+8  +9+10+5
   void Set_AN_DESY21_Event             () ; // 1+14+120+11+6+13+7+8+4+9+10+5
   
   void Set_BN_CERN22_Sample_Sim        () ; // 1+14    +11+6+13+7+8  +9+10+5
   void Set_AN_CERN22_Sample_Sim        () ; // 1+14    +11+6+13+7+8+4+9+10+5  
   
   void Set_AN_CERN22_SimHomeMade() ; // 1+14    +11+6+13+7+8+4+9+10+5  
                                        
   void Set_BN_DESY21_200_Sample        () ; // 1+14+12 +11+6+13+7+8  +9+10+5
   void Set_AN_DESY21_200_Sample        () ; // 1+14+12 +11+6+13+7+8+4+9+10+5  
   
   void Set_BN_COSMICS23_Sample         () ; // 1+14+12+11+6+13+7+8  +9+10+5+15+16

  void Set_T0_CERN22_Event              () ; // None
  void Set_T_DESY21_Event              () ; // 1+5+11
  void Set_T_DESY21theta_Event              () ;
  void Set_T2_CERN22_Event              () ; // 1+11+120+15+5+20
  void Set_TMC_CERN22_Event        () ; // 1+5

//
   void SetStat_Before(Sample& aSample, const int& ModuleNber,const int& iTem);
   void SetStat_After (Sample& aSample, const int& ModuleNber,const int& iTem);
   
   void SetStat_Before(Event*  pEvent , const int& iTem);
   void SetStat_After (Event*  pEvent , const int& iTem);
   
   std::vector < std::string > ListOfSelectionName ;  
   std::vector < int         > ListOfNberOfEvents_Before ;  
   std::vector < int         > ListOfNberOfEvents_After ;  
   std::vector < int         > ListOfNberOfModules_Before ;  
   std::vector < int         > ListOfNberOfModules_After ;  
   std::vector < int         > ListOfNberOfClusters_Before ;  
   std::vector < int         > ListOfNberOfClusters_After ;  
   std::vector < int         > ListOfNberOfPads_Before ;  
   std::vector < int         > ListOfNberOfPads_After ;  

//
  void Set_Cuts() ;
  
  double m_Cut_Stage2_Npads_Low    ;

  double m_Cut_Stage5_NCluster_Low ;

  double m_Cut_Stage6_ARelat_Low   ;
  double m_Cut_Stage6_A_Low        ;

  double m_Cut_Stage7_T2T1_Low     ;
  double m_Cut_Stage7_T2T1_Hig     ;
  double m_Cut_Stage7_T3T1_Low     ;
  double m_Cut_Stage7_T3T1_Hig     ;
  double m_Cut_Stage7_T3T2_Low     ;
  double m_Cut_Stage7_T3T2_Hig     ;

  double m_Cut_Stage8_A3onA1_Hig   ;
  double m_Cut_Stage8_A3onA1_Low   ;
  double m_Cut_Stage8_A2onA1_Low   ;
  double m_Cut_Stage8_A3onA2_Low   ;

  double m_Cut_Stage9_Npads_Low    ;

  double m_Cut_Stage10_Npads_Hig   ;
  
  double m_Cut_Stage11_EventBased  ;

  double m_Cut_Stage12_RunBased    ;

  double m_Cut_Stage120_TLow       ;
  double m_Cut_Stage120_THigh      ;

  double m_Cut_Stage13_DT_Low      ;

  double m_Cut_Stage14_Amax_Low    ;
  double m_Cut_Stage14_Amax_Hig    ;

  double m_Cut_Stage15_CutSlopeXY_Hig    ;
  double m_Cut_Stage15_CutSlopeXY_Low    ;

  double m_Cut_Stage16_CutSlopeXZ_Hig    ;
  double m_Cut_Stage16_CutSlopeXZ_Low    ;

  double m_Cut_Stage17_CutInterCeptXZ_Hig    ;
  double m_Cut_Stage17_CutInterCeptXZ_Low    ;

  double m_Cut_Stage18_v_drift_est    ;
  double m_Cut_Stage18_CutSlopeXYZ0   ;
  double m_Cut_Stage18_CutSlopeXYZ1   ;

  double m_Cut_StageT15_APM_Low       ;
  double m_Cut_StageT15_APM_High      ;

  double m_Cut_StageT16_PadIntegral  ;

  double m_Cut_StageT17_PadIntegral  ;

  double m_Cut_StageT19_BinDerivative ;

  double m_Cut_StageT20_Amax_dRay ;
  double m_Cut_StageT20_deltaY_dRay ;

  double m_Cut_StageT21_Rho3vsRho2 ;

};



#endif

