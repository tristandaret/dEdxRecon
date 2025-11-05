/***************************************************************************
 * File: Selector.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of selection utilities used to filter events, modules,
 *        clusters or pads according to configurable selection sets. The
 *        Selector class encapsulates selection stages and criteria.
 *
 * Contents: Selector class and selection-stage related declarations.
 *
 * Notes: Implementation in Selector.cxx.
 ***************************************************************************/
#ifndef SELECTOR_H
#define SELECTOR_H
#ifndef Selector_H
#define Selector_H

#include "Misc.h"

#include "Sample.h"

/////////////////////////////////////////////////////////////
class Selector {
public:
  /** Constructor */
  Selector(const std::string DefSelection);
  Selector();
  virtual ~Selector();

  //
  void Reset_Selection();

  void Add_Selection(const std::string &SelectionName);

  int NberOfSelections();

  std::string Get_SelectionName(const int &iTem);

  void Tell_Selection();

  //
  void ApplySelection(Sample &aSample, const int &ModuleNber);
  void Apply_ASelection(Sample &aSample, const int &ModuleNber,
                        const int &iTem);

  //
  void ApplySelection(Event *pEvent);
  void Apply_ASelection(Event *pEvent, const int &ModuleNber, const int &iTem);

  //
  void Reset_StatCounters();
  void PrintStat();

  double Get_Cut_StageFinal_NCluster_Low();
  void Set_Cut_StageFinal_NCluster_Low(double Cut_StageFinal_NCluster_Low);

  double Get_Cut_Stage5_Npads_Hig();
  void Set_Cut_Stage5_Npads_Hig(double Cut_Stage5_Npads_Hig);

  double Get_Cut_Stage2_EventBased();
  void Set_Cut_Stage2_EventBased(double Cut_Stage2_EventBased);

  double Get_Cut_Stage3_TLow();
  double Get_Cut_Stage3_THigh();
  void Set_Cut_Stage3_TLow(double Cut_Stage3_TLow);
  void Set_Cut_Stage3_THigh(double Cut_Stage3_THigh);

  double Get_Cut_Stage6_Amax_Low();
  void Set_Cut_Stage6_Amax_Low(double Cut_Stage6_Amax_Low);
  double Get_Cut_Stage6_Amax_Hig();
  void Set_Cut_Stage6_Amax_Hig(double Cut_Stage6_Amax_Hig);

  double Get_Cut_Stage4_APM_Low();
  void Set_Cut_Stage4_APM_Low(double Cut_Stage4_APM_Low);
  double Get_Cut_Stage4_APM_High();
  void Set_Cut_Stage4_APM_High(double Cut_Stage4_APM_High);

private:
  // Stage 1: Remove clusters in first and last columns
  void Stage1_Def();
  void Stage1(Sample &aSample, const int &ModuleNber);
  void Stage1(Event *pEvent, const int &ModuleNber);

  // Stage 2: Remove clusters out of time (Event Based)
  void Stage2_Def();
  void Stage2(Sample &aSample, const int &ModuleNber);
  void Stage2(Event *pEvent, const int &ModuleNber);

  // Stage 3: Remove clusters out of time
  void Stage3_Def();
  void Stage3(Sample &aSample, const int &ModuleNber);
  void Stage3(Event *pEvent, const int &ModuleNber);

  // Stage 4: Remove Events with an average pad multiplicity too high or too low
  void Stage4_Def();
  void Stage4(Sample &aSample, const int &ModuleNber);
  void Stage4(Event *pEvent, const int &ModuleNber);

  // Stage 5: Remove clusters with too many pads
  void Stage5_Def();
  void Stage5(Sample &aSample, const int &ModuleNber);
  void Stage5(Event *pEvent, const int &ModuleNber);

  // Stage 6: Remove Clusters with a too high APad
  void Stage6_Def();
  void Stage6(Sample &aSample, const int &ModuleNber);
  void Stage6(Event *pEvent, const int &ModuleNber);

  // Stage Final: Reject Events with too few clusters
  void StageFinal_Def();
  void StageFinal(Sample &aSample, const int &ModuleNber);
  void StageFinal(Event *pEvent, const int &ModuleNber);

  // Defaults
  void Set_DESY21_Event(); // 1+5+11
  void Set_DESY21theta_Event();
  void Set_T2_CERN22_Event();  // 1+11+120+15+5+20
  void Set_TMC_CERN22_Event(); // 1+5

  //
  void SetStat_Before(Sample &aSample, const int &ModuleNber, const int &iTem);
  void SetStat_After(Sample &aSample, const int &ModuleNber, const int &iTem);

  void SetStat_Before(Event *pEvent, const int &iTem);
  void SetStat_After(Event *pEvent, const int &iTem);

  std::vector<std::string> ListOfSelectionName;
  std::vector<int> ListOfNberOfEvents_Before;
  std::vector<int> ListOfNberOfEvents_After;
  std::vector<int> ListOfNberOfModules_Before;
  std::vector<int> ListOfNberOfModules_After;
  std::vector<int> ListOfNberOfClusters_Before;
  std::vector<int> ListOfNberOfClusters_After;
  std::vector<int> ListOfNberOfPads_Before;
  std::vector<int> ListOfNberOfPads_After;

  //
  void Set_Cuts();

  double m_Cut_StageFinal_NCluster_Low;

  double m_Cut_Stage5_Npads_Hig;

  double m_Cut_Stage2_EventBased;

  double m_Cut_Stage3_TLow;
  double m_Cut_Stage3_THigh;

  double m_Cut_Stage6_Amax_Low;
  double m_Cut_Stage6_Amax_Hig;

  double m_Cut_Stage4_APM_Low;
  double m_Cut_Stage4_APM_High;
};

#endif
