#include "Uploader_Prototype.h"

Uploader_Prototype::Uploader_Prototype(
            const std::string& SampleFile                 ,
            Model_ReadOutGeometry* pModel_ReadOutGeometry ,
            Model_Electronics*     pModel_Electronics     ,
            Model_Charge1D*        pModel_Charge1D        
):Uploader(
                                   SampleFile             ,
                                   pModel_ReadOutGeometry ,
                                   pModel_Electronics     ,
                                   pModel_Charge1D        
)
{  
  std::cout << std::endl ;
  std::cout << "  Uploader_Prototype::Uploader_Prototype "<< std::endl ;
}


Uploader_Prototype::~Uploader_Prototype()
{
//std::cout << " In Uploader_Prototype::~Uploader_Prototype() " << std::endl ;
}

Event* Uploader_Prototype::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use
){
  return GiveMe_Event(iEvent, ModuleNber_Input, Data_to_Use, 1) ;
}

Event* Uploader_Prototype::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use,
                               const int& CloseWF
){
  if (iEvent >= Get_NberOfEvent() ) return 0 ;
 
//Declaration of leaf types
  Int_t           ev ;
  Double_t        dEdx ;
  Double_t        angle_yz ;
  Double_t        angle_xy ;
  Int_t           rob_clusters ;
  Double_t        quality ;
  Double_t        mom  ;
  Double_t        sina ;
  Double_t        offset ;
  Int_t           max_mult ;
  Double_t        mean_mult ;
  Int_t           multiplicity[200];
  Int_t           charge[200];
  Double_t        residual[200];
  Double_t        residual_corr[200];
  Double_t        dx[200][10];
  Double_t        qfrac[200][10];
  Int_t           time[200][10];
  Double_t        clust_pos[200];
  Double_t        clust_pos_err[200];
  Double_t        track_pos[200];
  Int_t           module[200];
  Int_t           pad_charge[200][10];
  Int_t           pad_time[200][10];
  Int_t           wf_width[200][10];
  Int_t           wf_fwhm[200][10];
  Int_t           pad_x[200][10];
  Int_t           pad_y[200][10];
  Int_t           pad_wf_q[200][10][520];


//Set branch addresses and branch pointers
  p_TTree->SetBranchAddress("ev", &ev);
  p_TTree->SetBranchAddress("dEdx", &dEdx);
  p_TTree->SetBranchAddress("angle_yz", &angle_yz);
  p_TTree->SetBranchAddress("angle_xy", &angle_xy);
  p_TTree->SetBranchAddress("rob_clusters", &rob_clusters);
  p_TTree->SetBranchAddress("quality", &quality);
  p_TTree->SetBranchAddress("mom", &mom);
  p_TTree->SetBranchAddress("sina", &sina);
  p_TTree->SetBranchAddress("offset", &offset);
  p_TTree->SetBranchAddress("max_mult", &max_mult);
  p_TTree->SetBranchAddress("mean_mult", &mean_mult);
  p_TTree->SetBranchAddress("multiplicity", &multiplicity);
  p_TTree->SetBranchAddress("charge", &charge);
  p_TTree->SetBranchAddress("residual", &residual);
  p_TTree->SetBranchAddress("residual_corr", &residual_corr);
  p_TTree->SetBranchAddress("dx", &dx);
  p_TTree->SetBranchAddress("qfrac", &qfrac);
  p_TTree->SetBranchAddress("time", &time);
  p_TTree->SetBranchAddress("clust_pos", &clust_pos);
  p_TTree->SetBranchAddress("clust_pos_err", &clust_pos_err);
  p_TTree->SetBranchAddress("track_pos", &track_pos);
  p_TTree->SetBranchAddress("module", &module);
  p_TTree->SetBranchAddress("pad_charge", &pad_charge);
  p_TTree->SetBranchAddress("pad_time", &pad_time);
  p_TTree->SetBranchAddress("wf_width", &wf_width);
  p_TTree->SetBranchAddress("wf_fwhm", &wf_fwhm);
  p_TTree->SetBranchAddress("pad_x", &pad_x);
  p_TTree->SetBranchAddress("pad_y", &pad_y);
  p_TTree->SetBranchAddress("pad_wf_q", &pad_wf_q);                                   

// 
  int iPad_Max     =  10 ; 
  int iCluster_Max = 200 ; 
  int iTimeBin_Max = 510 ;
  int iTimeBin_Min =   0 ;

//
  p_TTree->GetEntry(iEvent);
  
//  
  Event*  pEvent = new Event( 
                             ev     ,
                             iEvent ,
                             p_Model_ReadOutGeometry  ,
                             p_Model_Electronics      ,
                             p_Model_Charge1D            
                            ); 
                                
  std::vector < Module* > V_pModule ;
  int iMod_Max = 8 ;
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = new Module( 
                             ev     ,
                             iEvent ,
                             iMod   ,
                             p_Model_ReadOutGeometry  ,
                             p_Model_Electronics      ,
                             p_Model_Charge1D            
                            ); 
    V_pModule.push_back( pModule ) ;
  }
     
  for (Int_t iCluster = 0; iCluster < iCluster_Max; iCluster++){
  
//  SetModule
    int ModuleNber_Cur = module[iCluster] ;  
    if ( ModuleNber_Input!=-1 && ModuleNber_Cur != ModuleNber_Input ) continue ;

    Module* pModule = V_pModule[ModuleNber_Cur] ;
     
    if(charge[iCluster] < 0) continue;
    Cluster* pCluster = new Cluster(
                                    ev     ,
                                    iEvent ,
                                    ModuleNber_Cur 
                                   ) ;
    
    // It seems that there are clusters (cluster charge >=0 but with no pad (charge <0 for all pads)
    // check and store the cluster only if there are pads in it
    int NberOfPads = 0 ; 
    for (Int_t iPad = 0; iPad < iPad_Max; iPad++){
      if(pad_charge[iCluster][iPad]<0)continue;
      NberOfPads += 1 ;
      
      int iX         = pad_x[iCluster][iPad] ;
      int iY         = pad_y[iCluster][iPad] ;
                      
      Pad* pPad = new Pad (p_Model_ReadOutGeometry , 
                           p_Model_Electronics     , 
                           p_Model_Charge1D        ,
                           "noName"                ,
                           ev     ,
                           iEvent ,
                           ModuleNber_Cur ,
                           iX ,iY ); 
                                           
      pPad->Set_AMax(pad_charge[iCluster][iPad]);
      pPad->Set_TMax(pad_time  [iCluster][iPad]);

      pPad->Clear_ADC() ;
      for (Int_t iTimeBin = iTimeBin_Min; iTimeBin < iTimeBin_Max; iTimeBin++){
        int iTimeBinProper = iTimeBin ;
        int ADCProper      = pad_wf_q[iCluster][iPad][iTimeBin] ;
        if ( ADCProper < -250 ) ADCProper = 0 ;
        if ( iTimeBinProper < iTimeBin_Max ) pPad->Set_ADC(iTimeBinProper,ADCProper ) ;
      }
      if(CloseWF == 1) pPad->Wf_DoClosure() ;
      
      pPad->Set_Data_2Use(Data_to_Use) ;
              
      pModule->Add_Pad(pPad ) ;
      
      pCluster->Add_Pad(pPad) ;

    }
    if (NberOfPads!=0){
      pCluster->DoClosure() ;
      pModule->Add_Cluster(pCluster);
    }else{
      delete pCluster ;
    }
  }
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = V_pModule[iMod] ;
    int NClusters = pModule->Get_NberOfCluster() ;
    if (NClusters!=0){
      pEvent->Add_Module(pModule) ;
    }else{
      delete pModule ;
    }
  }
  int NberOFModule = pEvent->Get_NberOfModule() ;
  if (NberOFModule==0){
    delete pEvent ;
    pEvent = 0 ;
    
  }

  return pEvent ;
}
