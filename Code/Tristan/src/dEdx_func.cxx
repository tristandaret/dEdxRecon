#include "Tristan/dEdx_func.h"
#include "Tristan/Misc_Functions.h"

#include "Misc/Util.h"
#include "Misc/Misc.h"

#include "EvtModelTools/EvtModelTools_TD_Selections.h"
#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"



void LoadMaps(const std::string &EventFile, std::vector<ReadRCmap*>& RCmaps, std::vector<ReadGainmap*>& Gainmaps, const std::vector<std::string>& eram_id){
  ReadRCmap   *RCmap0,   *RCmap1,   *RCmap2,   *RCmap3,   *RCmap4,   *RCmap5,   *RCmap6,    *RCmap7;
  ReadGainmap *Gainmap0, *Gainmap1, *Gainmap2, *Gainmap3, *Gainmap4, *Gainmap5, *Gainmap6, *Gainmap7;
  RCmaps.push_back(RCmap0);
  Gainmaps.push_back(Gainmap0);
  int n_maps = 1;
  if(EventFile.find("All_ERAMS") != std::string::npos){
    n_maps = 8;
    RCmaps.push_back(RCmap1); RCmaps.push_back(RCmap2); RCmaps.push_back(RCmap3); RCmaps.push_back(RCmap4); RCmaps.push_back(RCmap5); RCmaps.push_back(RCmap6); RCmaps.push_back(RCmap7); 
    Gainmaps.push_back(Gainmap1); Gainmaps.push_back(Gainmap2); Gainmaps.push_back(Gainmap3); Gainmaps.push_back(Gainmap4); Gainmaps.push_back(Gainmap5); Gainmaps.push_back(Gainmap6);  Gainmaps.push_back(Gainmap7);
  }
  for(int i = 0 ; i < n_maps ; i++){
    RCmaps[i] = new ReadRCmap(eram_id[i]);
    Gainmaps[i] = new ReadGainmap(eram_id[i]);
  }
  std::cout << "Gain & RC maps: loaded" << std::endl ;
}



float avg_Gain(const std::string &EventFile, const std::vector<std::string>& eram_id, std::vector<ReadGainmap*>& Gainmaps){
  std::vector<float> v_avg_G;
  int lowest, status;
  float avg_G, n_pads;
  for(int i = 0; i < (int)eram_id.size(); i++){
    status        = 0 ;
    n_pads        = 0 ;
    avg_G         = 0 ;
    lowest        = 9999 ;
    for(int iX = 0 ; iX < 36 ; iX++){
      for(int iY = 0 ; iY < 32 ; iY++){
        if(Gainmaps[i]->GetData(iX, iY, status) != 0){
          avg_G        += Gainmaps[i]->GetData(iX, iY, status) ;
          n_pads++ ;
        } 
        if(Gainmaps[i]->GetData(iX, iY, status) < lowest and Gainmaps[i]->GetData(iX, iY, status) > 0) lowest = Gainmaps[i]->GetData(iX, iY, status) ;
      }
    }
    // DrawOut_GainMap(eram_id[i], Gainmaps[i]);
    avg_G /= n_pads ;
    v_avg_G.push_back(avg_G);
    std::cout << "Average Gain in " << eram_id[i] << " = " << avg_G << std::endl ;
  }
  avg_G = mean(v_avg_G);
  if(EventFile.find("All_ERAMS") != std::string::npos) std::cout << "Average gain in Mockup = " << avg_G << std::endl ;

  return avg_G;
}



void Fill_Maps(std::vector<ReadRCmap*>& RCmaps, std::vector<ReadGainmap*>& Gainmaps, const std::vector<std::string>& eram_id){
  int status = 0;
  for(int i = 0; i < (int)eram_id.size(); i++){
    for(int iX = 0 ; iX < 36 ; iX++){
      for(int iY = 0 ; iY < 32 ; iY++){
        double RC_pad                   = RCmaps[i]->GetData(iX,iY, status) ;
        double G_pad                    = Gainmaps[i]->GetData(iX,iY, status) ;
        if(RC_pad == 0){
          std::cout << eram_id[i] << ": RC   hole in " <<  " iX = " << iX << " | iY = " << iY ; 
          float RC_left                 = RCmaps[i]->GetData(iX-1,iY,   status) ;
          float RC_right                = RCmaps[i]->GetData(iX+1,iY,   status) ;
          float RC_low                  = RCmaps[i]->GetData(iX,  iY-1, status) ;
          float RC_top                  = RCmaps[i]->GetData(iX,  iY+1, status) ;
          RC_pad                        = (RC_left + RC_right + RC_low + RC_top)/4 ;
          RCmaps[i]->                          SetData(iX,iY, RC_pad) ;
          std::cout << " | value reset at " << RC_pad << std::endl ;
        }
        if(G_pad == 0){
          std::cout << eram_id[i] << ": Gain hole in " << " iX = " << iX << " | iY = " << iY ; 
          float G_left                  = Gainmaps[i]->GetData(iX-1,iY,   status) ;
          float G_right                 = Gainmaps[i]->GetData(iX+1,iY,   status) ;
          float G_low                   = Gainmaps[i]->GetData(iX,  iY-1, status) ;
          float G_top                   = Gainmaps[i]->GetData(iX,  iY+1, status) ;
          G_pad                         = (G_left + G_right + G_low + G_top)/4 ;
          Gainmaps[i]->                        SetData(iX,iY, G_pad) ;
          std::cout << " | value reset at " << G_pad << std::endl ;
        }
      }
    }
  }
}



void Init_selection(const std::string &SelectionSet, JFL_Selector &aJFL_Selector, const std::string &Tag, Uploader *pUploader, const int &NbrOfMod, const int &Data_to_Use){
 // Get the correct cut on TLeading
  if(SelectionSet == "T2_CERN22_Event" or SelectionSet == "T_DESY21_Event"){
    int TLow = 0, THigh = 0 ;
    if(Get120_CSV("../Stage120_Cuts.csv", Tag, TLow, THigh)) std::cout << "TLow = " << TLow << " | THigh = " << THigh << std::endl ;
    else{
      std::cout << "No Stage120 cuts found in CSV. Getting them now..." << std::endl ;
      std::vector<int> v_TCut           = Comp120_Cut(pUploader, NbrOfMod, Data_to_Use, 0) ;
      TLow                              = v_TCut[0] ;
      THigh                             = v_TCut[1] ;
      Set120_CSV("../Stage120_Cuts.csv", Tag, TLow, THigh) ;
      std::cout << "Stage120 cuts are " << TLow << " to " << THigh << ". Values added to CSV file." << std::endl ;
    }
    aJFL_Selector.Set_Cut_Stage120_TLow (TLow) ;
    aJFL_Selector.Set_Cut_Stage120_THigh(THigh) ;
  }

  // Selection for DESY21 phi diagonal clustering
  if(Tag.find("diag") != std::string::npos){
    aJFL_Selector.Set_Cut_Stage5_NCluster_Low(50) ;
    aJFL_Selector.Set_Cut_StageT15_APM_Low(1) ;
    aJFL_Selector.Set_Cut_StageT15_APM_High(3.5) ;
  }

  // Selection for DESY21 theta
  if(Tag.find("theta") != std::string::npos){
    aJFL_Selector.Set_Cut_Stage120_TLow(0) ;
    aJFL_Selector.Set_Cut_Stage120_THigh(510) ;
    aJFL_Selector.Set_Cut_Stage11_EventBased(200) ;
  }
}



TF1* corr_func(const std::string &EventFile, const std::string &Tag){
  TF1* corr_func                 = new TF1("corr_func", "291.012 + 9.4669*x - 4.04*x*x + 1.31624*x*x*x - 0.059534*x*x*x*x", 0, 17); // values provided by Vlada (2022/10/11)
  if(Tag.find("diag") != std::string::npos){
    std::string filename      = EventFile.substr(0, EventFile.length()-5) ;
    int angle ;
    if(    (angle = filename.find("30"))  != (int)std::string::npos or (angle = filename.find("45"))  != (int)std::string::npos) filename.replace(angle, 2, "40") ;
    while( (angle = filename.find("460")) != (int)std::string::npos or (angle = filename.find("860")) != (int)std::string::npos) filename.replace(angle, 3, "m40") ;
    TFile* pfile              = new TFile((filename + "_WFmax_correction.root").c_str(), "READ") ;
    std::cout << (filename + "_WFmax_correction.root").c_str() << std::endl ;
    corr_func                    = pfile->Get<TF1>("corr_func") ;
    pfile->                     Close() ;
    delete                      pfile;
    corr_func->                    SetParameter(0, corr_func->GetParameter(0)-100) ;
    std::cout << std::setprecision(2) << "WF correction parameters: " << corr_func->GetParameter(0) << " | " << corr_func->GetParameter(1) << " | " << corr_func->GetParameter(2) << " | " << corr_func->GetParameter(3) << " | " << corr_func->GetParameter(4) << std::endl ;
  }
  return corr_func;
}