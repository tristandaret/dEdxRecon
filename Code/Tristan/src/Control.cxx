#include "Tristan/Control.h"
#include "Tristan/dEdx_func.h"
#include "Tristan/Displayer.h"
#include "Tristan/Misc_Functions.h"

#include "Misc/Util.h"

#include "EvtModelTools/JFL_Selector.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "EvtModelTools/EvtModelTools_Histos.h"

void Control(
                        const std::string& OutDir,
                        std::string const& Tag,
                        std::string const& Comment,
                        std::string const& EventFile,
                        std::string const& SelectionSet,
                        Uploader*          pUploader,
                        int         const& NbrOfMod,
                        int         const& Data_to_Use,
                        int         const& PT,
                        int         const& TB,
                        const std::string& particle)
{ 
  std::string OUTDirName = OutDir + Tag + "/" ;
  MakeMyDir(OUTDirName) ;  
  std::string OUTDIR_Evt_Display = OUTDirName + "/Evt_Display/" ;
  MakeMyDir(OUTDIR_Evt_Display) ;
  std::string OUTDIR_WF_Display = OUTDirName + "/WF_Display/" ;
  MakeMyDir(OUTDIR_WF_Display) ;

  // Redirect Output
  std::string OutPut_Analysis = OUTDirName + "1_" + Tag + "_Control_XP.txt" ;
  std::cout <<    OutPut_Analysis       << std::endl ;
  std::streambuf *coutbuf = std::cout.rdbuf() ;           // Save old buf
  std::ofstream OUT_DataFile(OutPut_Analysis.c_str()) ; // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf()) ;                 // Redirect std::cout to output file

  std::cout << "Control" << Comment << std::endl ;
  std::cout << "Tag           : " << Tag                << std::endl ;
  std::cout << "EventFile     : " << EventFile          << std::endl ;
  std::cout << "SelectionSet  : " << SelectionSet       << std::endl ;
  std::cout << "iMod          : " << NbrOfMod        << std::endl ;
  std::cout << "Data_to_Use   : " << Data_to_Use        << std::endl ;
  std::cout << "OUTDirName    : " << OUTDirName         << std::endl ;
  std::cout <<                                             std::endl ;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // T_Leading
  std::vector<TH1F*>          v_h1f_TL_Raw ;
  std::vector<TH1F*>          v_h1f_TL_Sel ;
  // T_Leading
  std::vector<TH2F*>          v_h2f_LeadPos_Raw ;
  std::vector<TH2F*>          v_h2f_LeadPos_Sel ;
  // Theta angle
  std::vector<TH2F*>          v_h2f_Theta_Raw ;
  std::vector<TH2F*>          v_h2f_Theta_Sel ;
  // Pad Multiplicity
  std::vector<TH1F*>          v_h1f_PM_Raw ;
  std::vector<TH1F*>          v_h1f_PM_Sel ;
  // Average Pad Multiplicity
  std::vector<TH1F*>          v_h1f_APM_Raw ;
  std::vector<TH1F*>          v_h1f_APM_Sel ;
  // Average pad integral
  std::vector<TH1F*>          v_h1f_API_Raw ;
  std::vector<TH1F*>          v_h1f_API_Sel ;
  // Q_pad
  std::vector<TH1F*>          v_h1f_Qpad_Raw ;
  std::vector<TH1F*>          v_h1f_Qpad_Sel ;
  // Q_lead
  std::vector<TH1F*>          v_h1f_Qlead_Raw ;
  std::vector<TH1F*>          v_h1f_Qlead_Sel ;
  // Q_lead
  std::vector<TH1F*>          v_h1f_Qneigh_Raw ;
  std::vector<TH1F*>          v_h1f_Qneigh_Sel ;
  // Q_cluster
  std::vector<TH1F*>          v_h1f_Qclus_Raw ;
  std::vector<TH1F*>          v_h1f_Qclus_Sel ;
  // Number of clusters per event
  std::vector<TH1F*>          v_h1f_CM_Raw ;
  std::vector<TH1F*>          v_h1f_CM_Sel ;
  // Efficiency
  std::vector<TH1F*>          v_h1f_Eff_Raw ;
  std::vector<TH1F*>          v_h1f_Eff_Sel ;
  // Theta angle
  std::vector<TH2F*>          v_h2f_rho_DT_Raw ;
  std::vector<TH2F*>          v_h2f_rho_DT_Sel ;

  for(int iMod = 0 ; iMod < 8 ; iMod++){
    TH1F* h1f_TL_Raw                  = new TH1F(Form("h1f_TL_Raw_%i", iMod),  Form("T_{Leading} Raw (Mod %i);Time bin;Count", iMod), 511, -0.5, 510.5) ;
    TH1F* h1f_TL_Sel                  = new TH1F(Form("h1f_TL_Sel_%i", iMod),   Form("T_{Leading} Cut (Mod %i);Time bin;Count", iMod), 511, -0.5, 510.5) ;
    TH1F* h1f_PM_Raw                  = new TH1F(Form("h1f_PM_Raw_%i", iMod),  Form("Pad Multiplicity Raw (Mod %i);Number of pads;Count", iMod), 11, -0.5, 10.5) ;
    TH1F* h1f_PM_Sel                  = new TH1F(Form("h1f_PM_Sel_%i", iMod),   Form("Pad Multiplicity Cut (Mod %i);Number of pads;Count", iMod), 11, -0.5, 10.5) ;
    TH1F* h1f_APM_Raw                 = new TH1F(Form("h1f_APM_Raw_%i", iMod), Form("Average Pad Multiplicity Raw (Mod %i);average pad integral (ADC counts);Count", iMod), 44, -0.5, 10.5) ;
    TH1F* h1f_APM_Sel                 = new TH1F(Form("h1f_APM_Sel_%i", iMod),  Form("Average Pad Multiplicity Cut (Mod %i);average pad integral (ADC counts);Count", iMod), 44, -0.5, 10.5) ;
    TH1F* h1f_API_Raw                 = new TH1F(Form("h1f_API_Raw_%i", iMod), Form("Average of the pad integral Raw (Mod %i);Mean pad integral (ADC counts);Count", iMod), 400, -20, 255) ;
    TH1F* h1f_API_Sel                 = new TH1F(Form("h1f_API_Sel_%i", iMod),  Form("Average of the pad integral Cut (Mod %i);Mean pad integral (ADC counts);Count", iMod), 400, -20, 255) ;
    TH1F* h1f_Qpad_Raw                = new TH1F(Form("h1f_Qpad_Raw_%i", iMod), Form("Q_{pad} Raw (Mod %i);Amplitude (ADC counts);Count", iMod), 400, -100, 3900) ;
    TH1F* h1f_Qpad_Sel                = new TH1F(Form("h1f_Qpad_Sel_%i", iMod),  Form("Q_{pad} Cut (Mod %i);Amplitude (ADC counts);Count", iMod), 400, -100, 3900) ;
    TH1F* h1f_Qlead_Raw               = new TH1F(Form("h1f_Qlead_Raw_%i", iMod),Form("Q_{lead} Raw (Mod %i);Amplitude (ADC counts);Count", iMod), 200, -100, 3900) ;
    TH1F* h1f_Qlead_Sel               = new TH1F(Form("h1f_Qlead_Sel_%i", iMod), Form("Q_{lead} Cut (Mod %i);Amplitude (ADC counts);Count", iMod), 200, -100, 3900) ;
    TH1F* h1f_Qneigh_Raw              = new TH1F(Form("h1f_Qneigh_Raw_%i", iMod),Form("Q_{neighbours} Raw (Mod %i);Amplitude (ADC counts);Count", iMod), 200, -100, 3900) ;
    TH1F* h1f_Qneigh_Sel              = new TH1F(Form("h1f_Qneigh_Sel_%i", iMod), Form("Q_{neighbours} Cut (Mod %i);Amplitude (ADC counts);Count", iMod), 200, -100, 3900) ;
    TH1F* h1f_Qclus_Raw               = new TH1F(Form("h1f_Qclus_Raw_%i", iMod),Form("Q_{cluster} Raw (Mod %i);Amplitude (ADC counts);Count", iMod), 120, -100, 3900) ;
    TH1F* h1f_Qclus_Sel               = new TH1F(Form("h1f_Qclus_Sel_%i", iMod), Form("Q_{cluster} Cut (Mod %i);Amplitude (ADC counts);Count", iMod), 120, -100, 3900) ;
    TH1F* h1f_CM_Raw                  = new TH1F(Form("h1f_CM_Raw_%i", iMod),  Form("Number of clusters per module Raw (Mod %i);Number of clusters;Count", iMod), 37, -0.5, 36.5) ;
    TH1F* h1f_CM_Sel                  = new TH1F(Form("h1f_CM_Sel_%i", iMod),   Form("Number of clusters per module Cut (Mod %i);Number of clusters;Count", iMod), 37, -0.5, 36.5) ;
    TH1F* h1f_Eff_Raw                 = new TH1F(Form("h1f_Eff_Raw_%i", iMod), Form("Efficiency : final fraction of events Raw (Mod %i);N_{cluster} minimum;Fraction of events kept (%%)", iMod), 37, -0.5, 36.5) ;
    TH1F* h1f_Eff_Sel                 = new TH1F(Form("h1f_Eff_Sel_%i", iMod),  Form("Efficiency : final fraction of events Cut (Mod %i);N_{cluster} minimum;Fraction of events kept (%%)", iMod), 37, -0.5, 36.5) ;
    TH2F* h2f_LeadPos_Raw             = new TH2F(Form("h2f_LeadPos_Raw_%i", iMod), Form("Position of leading pads in ERAM (Mod %i);iX;iY", iMod), 36, -0.5, 35.5, 32, -0.5, 31.5) ;
    TH2F* h2f_LeadPos_Sel             = new TH2F(Form("h2f_LeadPos_Sel_%i", iMod), Form("Position of leading pads in ERAM (Mod %i);iX;iY", iMod), 36, -0.5, 35.5, 32, -0.5, 31.5) ;
    TH2F* h2f_Theta_Raw               = new TH2F(Form("h2f_Theta_Raw_%i", iMod), Form("Track inclination along #theta angle(Mod %i);Pad row (iX);Drift distance (mm)", iMod), 36, -0.5, 35.5, 80, 0, 1010) ;
    TH2F* h2f_Theta_Sel               = new TH2F(Form("h2f_Theta_Sel_%i", iMod), Form("Track inclination along #theta angle(Mod %i);Pad row (iX);Drift distance (mm)", iMod), 36, -0.5, 35.5, 80, 0, 1010) ;
    TH2F* h2f_rho_DT_Raw            = new TH2F(Form("h2f_rho_DT_Raw_%i", iMod), Form("A_{1st neighbour} / A_{leading} vs T_{1st neighbour} - T_{leading} (Mod %i);T_{1st neighbour} - T_{leading};A_{1st neighbour} / A_{leading}", iMod), 101, -20, 80, 200, 0, 1) ;
    TH2F* h2f_rho_DT_Sel            = new TH2F(Form("h2f_rho_DT_Sel_%i", iMod), Form("A_{1st neighbour} / A_{leading} vs T_{1st neighbour} - T_{leading} (Mod %i);T_{1st neighbour} - T_{leading};A_{1st neighbour} / A_{leading}", iMod), 101, -20, 80, 200, 0, 1) ;
    v_h1f_TL_Raw.                       push_back(h1f_TL_Raw) ;
    v_h1f_TL_Sel.                       push_back(h1f_TL_Sel) ;
    v_h1f_PM_Raw.                       push_back(h1f_PM_Raw) ;
    v_h1f_PM_Sel.                       push_back(h1f_PM_Sel) ;
    v_h1f_APM_Raw.                      push_back(h1f_APM_Raw) ;
    v_h1f_APM_Sel.                      push_back(h1f_APM_Sel) ;
    v_h1f_API_Raw.                      push_back(h1f_API_Raw) ;
    v_h1f_API_Sel.                      push_back(h1f_API_Sel) ;
    v_h1f_Qpad_Raw.                     push_back(h1f_Qpad_Raw) ;
    v_h1f_Qpad_Sel.                     push_back(h1f_Qpad_Sel) ;
    v_h1f_Qlead_Raw.                    push_back(h1f_Qlead_Raw) ;
    v_h1f_Qlead_Sel.                    push_back(h1f_Qlead_Sel) ;
    v_h1f_Qneigh_Raw.                   push_back(h1f_Qneigh_Raw) ;
    v_h1f_Qneigh_Sel.                   push_back(h1f_Qneigh_Sel) ;
    v_h1f_Qclus_Raw.                    push_back(h1f_Qclus_Raw) ;
    v_h1f_Qclus_Sel.                    push_back(h1f_Qclus_Sel) ;
    v_h1f_CM_Raw.                       push_back(h1f_CM_Raw) ;
    v_h1f_CM_Sel.                       push_back(h1f_CM_Sel) ;
    v_h1f_Eff_Raw.                      push_back(h1f_Eff_Raw) ;
    v_h1f_Eff_Sel.                      push_back(h1f_Eff_Sel) ;
    v_h2f_LeadPos_Raw.                  push_back(h2f_LeadPos_Raw) ;
    v_h2f_LeadPos_Sel.                  push_back(h2f_LeadPos_Sel) ;
    v_h2f_Theta_Raw.                    push_back(h2f_Theta_Raw) ;
    v_h2f_Theta_Sel.                    push_back(h2f_Theta_Sel) ;
    v_h2f_rho_DT_Raw.                   push_back(h2f_rho_DT_Raw) ;
    v_h2f_rho_DT_Sel.                   push_back(h2f_rho_DT_Sel) ;
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Selection stage
  JFL_Selector aJFL_Selector(SelectionSet) ;
  int NEvent = pUploader->Get_NberOfEvent() ;
  std::cout << "Number of entries :" << NEvent << std::endl ;

  Init_selection(SelectionSet, aJFL_Selector, Tag, pUploader, NbrOfMod, Data_to_Use);
    
  aJFL_Selector.Tell_Selection() ;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Loop On Events
  int nEvt_raw4mod = 0;
  int nEvt_sel4mod = 0;
  aJFL_Selector.Reset_StatCounters() ;
  std::cout << "Processing events:" << std::endl ;
  for(int iEvent = 0 ; iEvent < NEvent ; iEvent++){
    if(iEvent % 1000 == 0 or iEvent == NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl ;
    Event*  pEvent                      = pUploader->GiveMe_Event(iEvent, NbrOfMod, Data_to_Use, 0) ;
    if (!pEvent or pEvent->IsValid() == 0){
      delete pEvent;
      continue ;
    }
    //  Loop On Modules
    int nMod                            = pEvent->Get_NberOfModule() ;
    // if(nMod < 4){
    //   delete pEvent;
    //   continue ;
    // }
    nEvt_raw4mod++;

    for(int iMod = 0 ; iMod < nMod ; iMod++){
      Module* pModule                   = pEvent->Get_Module_InArray(iMod) ;
      int ModuleNber                    = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      float avrg_padmutl                = 0 ;
      int NClusters                     = pModule->Get_NberOfCluster() ;
      v_h1f_CM_Raw[ModuleNber]->                Fill(NClusters) ;

      for(int iC = 0 ; iC < NClusters ; iC++){
        Cluster* pCluster               = pModule->Get_Cluster(iC) ;
        const Pad* pLead                = pCluster->Get_LeadingPad() ;
        v_h2f_LeadPos_Raw[ModuleNber]->         Fill(pLead->Get_iX(), pLead->Get_iY()) ;
        int NPads                       = pCluster->Get_NberOfPads() ;
        float TL                        = pCluster->Get_TMaxLeading() ;
        avrg_padmutl                   += (float)NPads ;
        if(TL>5 and TL < 509) v_h1f_TL_Raw[ModuleNber]->     Fill(TL) ;
        v_h1f_PM_Raw[ModuleNber]->              Fill(NPads) ;
        v_h1f_Qclus_Raw[ModuleNber]->           Fill(pCluster->Get_Acluster()) ;
        TH1F* h1f_clus                  = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ;

        if(NPads > 1) v_h2f_rho_DT_Raw[ModuleNber]->Fill(pCluster->Get_Pad(NPads-2)->Get_TMax()-TL, pCluster->Get_Pad(NPads-2)->Get_AMax()/pCluster->Get_AMaxLeading());

        float z_calc = 0;
        float T0 ;
        if      (PT == 412 and TB == 50){ T0 = 45 ; z_calc = 3.90*(TL-T0) ; } // 45 = 37(time shift) +  8(PT) from David
        else if (PT == 412 and TB == 40){ T0 = 56 ; z_calc = 3.12*(TL-T0) ; } // 56 = 46(time shift) + 10(PT)
        else if (PT == 200 and TB == 50){ T0 = 39 ; z_calc = 3.90*(TL-T0) ; } // 39 = 35(time shift) +  4(PT) own computation
        else if (PT == 200 and TB == 40){ T0 = 48 ; z_calc = 3.12*(TL-T0) ; } // 48 = 44(time shift) +  5(PT)
        if(z_calc < 0) z_calc           = 0 ;
        v_h2f_Theta_Raw[ModuleNber]->Fill(pLead->Get_iX(), z_calc) ;

        for(int iP = 0 ; iP < NPads ; iP++){
          const Pad* pPad               = pCluster->Get_Pad(iP) ;
          TH1F* h1f_WF                  = GiveMe_WaveFormDisplay(pPad, "dummy") ;
          h1f_clus->                      Add(h1f_WF) ;
          float AP                      = pPad->Get_AMax() ;
          float pad_integral            = h1f_WF->Integral() ;
          if(iP == NPads-1) v_h1f_Qlead_Raw[ModuleNber]-> Fill(AP) ;
          else v_h1f_Qneigh_Raw[ModuleNber]->   Fill(AP) ;
          v_h1f_Qpad_Raw[ModuleNber]->          Fill(AP) ;
          float avg_pad_int            = pad_integral/510 ;
          v_h1f_API_Raw[ModuleNber]->           Fill(avg_pad_int) ;
          delete                          h1f_WF ;
        }
        delete h1f_clus ;
      }
      avrg_padmutl                     /= (float)NClusters ;
      v_h1f_APM_Raw[ModuleNber]->               Fill(avrg_padmutl) ;
    }

    //  Apply Selection
    aJFL_Selector.ApplySelection(pEvent) ;
    if (pEvent->IsValid() == 0){
      delete pEvent;
      continue ;
    }

    nEvt_sel4mod++;

    //  Loop On Modules
    for(int iMod = 0 ; iMod < nMod ; iMod++){
      Module* pModule                   = pEvent->Get_Module_InArray(iMod) ;
      int ModuleNber                    = pModule->Get_ModuleNber();
      if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ;
      float avrg_padmutl                = 0 ;
      int NClusters                     = pModule->Get_NberOfCluster() ;
      v_h1f_CM_Sel[ModuleNber]->              Fill(NClusters) ;

      for(int iC = 0 ; iC < NClusters ; iC++){
        Cluster* pCluster               = pModule->Get_Cluster(iC) ;
        const Pad* pLead                = pCluster->Get_LeadingPad() ;
        v_h2f_LeadPos_Sel[ModuleNber]->         Fill(pLead->Get_iX(), pLead->Get_iY()) ;
        int NPads                       = pCluster->Get_NberOfPads() ;
        float TL                        = pCluster->Get_TMaxLeading() ;
        avrg_padmutl                   += (float)NPads ;
        v_h1f_TL_Sel[ModuleNber]->              Fill(TL) ;
        v_h1f_PM_Sel[ModuleNber]->              Fill(NPads) ;
        v_h1f_Qclus_Sel[ModuleNber]->           Fill(pCluster->Get_Acluster()) ;
        TH1F* h1f_clus                  = new TH1F("h1f_WF_cluster", "h1f_WF_cluster", 510, -0.5, 509.5) ;

        if(NPads > 1) v_h2f_rho_DT_Sel[ModuleNber]->Fill(pCluster->Get_Pad(NPads-2)->Get_TMax()-TL, pCluster->Get_Pad(NPads-2)->Get_AMax()/pCluster->Get_AMaxLeading());

        float z_calc = 0;
        float T0 ;
        if      (PT == 412 and TB == 50){ T0 = 45 ; z_calc = 3.90*(TL-T0) ; } // 45 = 37(time shift) +  8(PT) from David
        else if (PT == 412 and TB == 40){ T0 = 56 ; z_calc = 3.12*(TL-T0) ; } // 56 = 46(time shift) + 10(PT)
        else if (PT == 200 and TB == 50){ T0 = 39 ; z_calc = 3.90*(TL-T0) ; } // 39 = 35(time shift) +  4(PT) own computation
        else if (PT == 200 and TB == 40){ T0 = 48 ; z_calc = 3.12*(TL-T0) ; } // 48 = 44(time shift) +  5(PT)
        if(z_calc < 0) z_calc           = 0 ;
        v_h2f_Theta_Sel[ModuleNber]->Fill(pLead->Get_iX(), z_calc) ;

        for(int iP = 0 ; iP < NPads ; iP++){
          const Pad* pPad               = pCluster->Get_Pad(iP) ;
          TH1F* h1f_WF                  = GiveMe_WaveFormDisplay(pPad, "dummy") ;
          h1f_clus->                      Add(h1f_WF) ;
          float AP                      = pPad->Get_AMax() ;
          float pad_integral            = h1f_WF->Integral() ;
          if(iP == NPads-1) v_h1f_Qlead_Sel[ModuleNber]-> Fill(AP) ;
          else v_h1f_Qneigh_Sel[ModuleNber]->   Fill(AP) ;
          v_h1f_Qpad_Sel[ModuleNber]->          Fill(AP) ;
          float avg_pad_int            = pad_integral/510 ;
          v_h1f_API_Sel[ModuleNber]->           Fill(avg_pad_int) ;
          delete                          h1f_WF ;
        }
        delete h1f_clus ;
      }
      avrg_padmutl                     /= (float)NClusters ;
      v_h1f_APM_Sel[ModuleNber]->               Fill(avrg_padmutl) ;
    }
    delete pEvent ;
  }

  for(int iMod = 0 ; iMod < 8 ; iMod++){
    int nBin                = v_h1f_CM_Sel[iMod]->GetNbinsX() ;
    float norm              = v_h1f_CM_Raw[iMod]->GetEntries() ;
    float eff               = 0 ;
    for(int iBin            = nBin ; iBin > 0 ; iBin--){
      eff                   += v_h1f_CM_Raw[iMod]->GetBinContent(iBin) ;
      float norm_eff        = eff/norm * 100 ;
      v_h1f_Eff_Raw[iMod]->Fill(iBin-1, norm_eff) ;
    }

    nBin                    = v_h1f_CM_Sel[iMod]->GetNbinsX() ;
    norm                    = v_h1f_CM_Sel[iMod]->GetEntries() ;
    eff                     = 0 ;
    for(int iBin            = nBin ; iBin > 0 ; iBin--){
      eff                   += v_h1f_CM_Sel[iMod]->GetBinContent(iBin) ;
      float norm_eff        = eff/norm * 100 ;
      v_h1f_Eff_Sel[iMod]->Fill(iBin-1, norm_eff) ;
    }
  }
  
  std::cout << "Nber Of Entries :" << NEvent << std::endl ;
  if(EventFile.find("All_ERAMS") != std::string::npos){
    std::cout << "Raw      events with at least 4 modules: " << nEvt_raw4mod << std::endl;
    std::cout << "Selected events with at least 4 modules: " << nEvt_sel4mod << std::endl;
  }
  aJFL_Selector.PrintStat() ;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Saving //
  int nMod;
  if(EventFile.find("All_ERAMS") != std::string::npos) nMod = 8;
  else nMod = 1;

  TFile* pfileROOT = new TFile(TString(OUTDirName + "1_" + Tag + "_Control" + Comment + ".root"), "RECREATE") ;
  for(int iMod = 0 ; iMod < nMod ; iMod++){
    v_h1f_TL_Raw[iMod]->   Write() ;
    v_h1f_TL_Sel[iMod]->    Write() ;  
    v_h1f_PM_Raw[iMod]->   Write() ;  
    v_h1f_PM_Sel[iMod]->    Write() ;
    v_h1f_APM_Raw[iMod]->  Write() ;
    v_h1f_APM_Sel[iMod]->   Write() ;
    v_h1f_API_Raw[iMod]->  Write() ;
    v_h1f_API_Sel[iMod]->   Write() ;
    v_h1f_Qpad_Raw[iMod]->  Write() ;
    v_h1f_Qpad_Sel[iMod]->   Write() ;
    v_h1f_Qlead_Raw[iMod]->  Write() ;
    v_h1f_Qlead_Sel[iMod]->   Write() ;
    v_h1f_Qneigh_Raw[iMod]->  Write() ;
    v_h1f_Qneigh_Sel[iMod]->   Write() ;
    v_h1f_Qclus_Raw[iMod]->  Write() ;
    v_h1f_Qclus_Sel[iMod]->   Write() ;
    v_h1f_CM_Raw[iMod]->   Write() ;
    v_h1f_CM_Sel[iMod]->    Write() ; 
    v_h1f_Eff_Raw[iMod]->  Write() ; 
    v_h1f_Eff_Sel[iMod]->   Write() ;
    v_h2f_LeadPos_Raw[iMod]->  Write() ; 
    v_h2f_LeadPos_Sel[iMod]->   Write() ; 
    v_h2f_Theta_Raw[iMod]->  Write() ; 
    v_h2f_Theta_Sel[iMod]->   Write() ; 
    v_h2f_rho_DT_Raw[iMod]->  Write() ; 
    v_h2f_rho_DT_Sel[iMod]->   Write() ; 
  }
  pfileROOT->Close() ;

  // Deleting //
  delete pfileROOT ;
  for(int iMod = 0 ; iMod < 8 ; iMod++){
    delete v_h1f_PM_Raw[iMod] ;      v_h1f_PM_Raw[iMod]   = 0 ;
    delete v_h1f_PM_Sel[iMod] ;       v_h1f_PM_Sel[iMod]    = 0 ;
    delete v_h1f_TL_Raw[iMod] ;      v_h1f_TL_Raw[iMod]   = 0 ;
    delete v_h1f_TL_Sel[iMod] ;       v_h1f_TL_Sel[iMod]    = 0 ;
    delete v_h1f_APM_Raw[iMod] ;     v_h1f_APM_Raw[iMod]  = 0 ;
    delete v_h1f_APM_Sel[iMod] ;      v_h1f_APM_Sel[iMod]   = 0 ;
    delete v_h1f_API_Raw[iMod] ;     v_h1f_API_Raw[iMod]  = 0 ;
    delete v_h1f_API_Sel[iMod] ;      v_h1f_API_Sel[iMod]   = 0 ;
    delete v_h1f_Qpad_Raw[iMod] ;     v_h1f_Qpad_Raw[iMod]  = 0 ;
    delete v_h1f_Qpad_Sel[iMod] ;      v_h1f_Qpad_Sel[iMod]   = 0 ;
    delete v_h1f_Qlead_Raw[iMod] ;     v_h1f_Qlead_Raw[iMod]  = 0 ;
    delete v_h1f_Qlead_Sel[iMod] ;      v_h1f_Qlead_Sel[iMod]   = 0 ;
    delete v_h1f_Qneigh_Raw[iMod] ;     v_h1f_Qneigh_Raw[iMod]  = 0 ;
    delete v_h1f_Qneigh_Sel[iMod] ;      v_h1f_Qneigh_Sel[iMod]   = 0 ;
    delete v_h1f_Qclus_Raw[iMod] ;     v_h1f_Qclus_Raw[iMod]  = 0 ;
    delete v_h1f_Qclus_Sel[iMod] ;      v_h1f_Qclus_Sel[iMod]   = 0 ;
    delete v_h1f_CM_Raw[iMod] ;      v_h1f_CM_Raw[iMod]   = 0 ;
    delete v_h1f_CM_Sel[iMod] ;       v_h1f_CM_Sel[iMod]    = 0 ;
    delete v_h1f_Eff_Raw[iMod] ;     v_h1f_Eff_Raw[iMod]  = 0 ;
    delete v_h1f_Eff_Sel[iMod] ;      v_h1f_Eff_Sel[iMod]   = 0 ;
    delete v_h2f_LeadPos_Raw[iMod] ;     v_h2f_LeadPos_Raw[iMod]  = 0 ;
    delete v_h2f_LeadPos_Sel[iMod] ;      v_h2f_LeadPos_Sel[iMod]   = 0 ;
    delete v_h2f_Theta_Raw[iMod] ;     v_h2f_Theta_Raw[iMod]  = 0 ;
    delete v_h2f_Theta_Sel[iMod] ;      v_h2f_Theta_Sel[iMod]   = 0 ;
    delete v_h2f_rho_DT_Raw[iMod] ;     v_h2f_rho_DT_Raw[iMod]  = 0 ;
    delete v_h2f_rho_DT_Sel[iMod] ;      v_h2f_rho_DT_Sel[iMod]   = 0 ;
  }
  v_h1f_TL_Raw     .clear() ;
  v_h1f_TL_Sel      .clear() ;
  v_h1f_PM_Raw     .clear() ;
  v_h1f_PM_Sel      .clear() ;
  v_h1f_APM_Raw    .clear() ;
  v_h1f_APM_Sel     .clear() ;
  v_h1f_API_Raw    .clear() ;
  v_h1f_API_Sel     .clear() ;
  v_h1f_Qpad_Raw    .clear() ;
  v_h1f_Qpad_Sel     .clear() ;
  v_h1f_Qlead_Raw    .clear() ;
  v_h1f_Qlead_Sel     .clear() ;
  v_h1f_Qneigh_Raw    .clear() ;
  v_h1f_Qneigh_Sel     .clear() ;
  v_h1f_Qclus_Raw    .clear() ;
  v_h1f_Qclus_Sel     .clear() ;
  v_h1f_CM_Raw     .clear() ;
  v_h1f_CM_Sel      .clear() ;
  v_h1f_Eff_Raw    .clear() ;
  v_h1f_Eff_Sel     .clear() ;
  v_h2f_LeadPos_Raw    .clear() ;
  v_h2f_LeadPos_Sel     .clear() ;
  v_h2f_Theta_Raw    .clear() ;
  v_h2f_Theta_Sel     .clear() ;
  v_h2f_rho_DT_Raw    .clear() ;
  v_h2f_rho_DT_Sel     .clear() ;

  std::cout.rdbuf(coutbuf) ; // Reset to standard output again
}
