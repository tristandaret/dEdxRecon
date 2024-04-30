#include "PID/Displayer.h"
#include "PID/Misc_Functions.h"
#include "Misc/Util.h"

#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "Fitters_Track/TheFitterTrack.h"
#include "AnalysisTools/DoTracksReconstruction.h"

void Displayer(
                        const std::string& OutDir ,
                        std::string const& Tag ,
                        std::string const& SelectionSet ,
                        Uploader*          pUploader ,
                        int         const& Moduleslist  ,
                        int         const& Data_to_Use,
                        std::string const& particle)
{
  std::string OUTDirName = OutDir + "/" + Tag + "/" ;
  MakeMyDir(OUTDirName) ;  
  // std::string OUTDIR_Evt_Display = OUTDirName + "/Evt_Display/" ;
  // MakeMyDir(OUTDIR_Evt_Display) ;
  // std::string OUTDIR_Time_Display = OUTDirName + "/Time_Display/" ;
  // MakeMyDir(OUTDIR_Time_Display) ;
  std::string OUTDIR_WF_Display = OUTDirName + "/WF_Display/" ;
  MakeMyDir(OUTDIR_WF_Display) ;
  std::string OUTDIR_CWF_Display = OUTDirName + "/CWF_Display/" ;
  MakeMyDir(OUTDIR_CWF_Display) ;
  std::string OUTDIR_GWF_Display = OUTDirName + "/GWF_Display/" ;
  MakeMyDir(OUTDIR_GWF_Display) ;

  // int NEvent                    = pUploader->Get_NberOfEvent() ;
  // int TLow                      = 0 ;
  // int THigh                     = 0 ;
  // if(particle == "e^{+} 1GeV")    {TLow = 160 ; THigh = 201 ; }
  // else if(particle == "#mu^{+} 1GeV")  {TLow = 158 ; THigh = 194 ; }
  // else if(particle == "protons 1GeV")  {TLow = 152 ; THigh = 207 ; }
  // else if(particle == "#pi^{-} 0.5GeV"){TLow = 153 ; THigh = 195 ; }
  // else{
  //   std::vector<int> v_TCut             = Comp120_Cut(pUploader, 0, Data_to_Use, 0) ;
  //   TLow                                = v_TCut[0] ;
  //   THigh                               = v_TCut[1] ;
  // }
  
  // /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // // Selection
  // JFL_Selector aJFL_Selector(SelectionSet) ;
  // aJFL_Selector.                  Reset_StatCounters() ;
  // aJFL_Selector.                  Set_Cut_Stage120_TLow (TLow) ;
  // aJFL_Selector.                  Set_Cut_Stage120_THigh(THigh) ;

  // for (int iEvent = 0 ; iEvent < NEvent ; iEvent++){
  //   Event*  pEvent              = pUploader->GiveMe_Event(iEvent, Moduleslist, Data_to_Use, 0) ;
  //   if (!pEvent)                  continue ;
  //   aJFL_Selector.                ApplySelection(pEvent) ;
  //   if (pEvent->IsValid() == 0)   continue ;
  //   int nMod                    = pEvent->Get_NberOfModule() ;

  //   for (int iMod = 0 ; iMod < nMod ; iMod++){
  //     Module* pModule           = pEvent->Get_Module_InArray(iMod) ;
  //     int ModuleNber            = pModule->Get_ModuleNber() ;
  //     if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      Event*  pEvent              = pUploader->GiveMe_Event(1528, Moduleslist, Data_to_Use, 0) ;
      Module* pModule           = pEvent->Get_Module_InArray(0) ;
      int NClusters             = pModule->Get_NberOfCluster() ; 
      for(int iC = 0 ; iC < NClusters ; iC++){
        Cluster* pCluster       = pModule->Get_Cluster(iC) ;
        // const Pad* pLead        = pCluster->Get_LeadingPad() ;
        DrawOut_ClusterWFDisplay(pCluster, OUTDIR_CWF_Display, Tag) ;
      }
    // }
  //   delete                        pEvent ;
  // }
}




TH1F* Displayer_DPR(const std::string OUTDirName, const float& tmin, float const& tmax, const float& t_0, const int& nbins)
{
  TH1F* h1f_DPR                 = DPR("h1f_DPR", -0.5, 509.5, tmax-9.55, 510, 999, 412, 40) ;
  
  // Drawing
  gStyle->SetOptStat(111111) ;
  std::string OutputFile          = std::string(OUTDirName + "/DPR_" + int(tmin) + "_" + int(tmax) + "_" + int(t_0) + ".pdf") ;
  TCanvas* pTCanvas               = new TCanvas("pTCanvas", "pTCanvas", 1800, 1200) ;
  pTCanvas->                        cd() ;
  h1f_DPR->                       Draw("hist") ;
  pTCanvas->                        SaveAs(OutputFile.c_str()) ;
  delete                            h1f_DPR ;

  return h1f_DPR ;
}