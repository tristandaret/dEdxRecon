#include "Uploader_MockUp_V2.h"

Uploader_MockUp_V2::Uploader_MockUp_V2(
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
  m_Nx = pModel_ReadOutGeometry->Get_Nx();
  m_Ny = pModel_ReadOutGeometry->Get_Ny();
  
  Init();

  m_BeforeMerging = 0 ;
  m_Swap_XY = 0 ;
}


Uploader_MockUp_V2::~Uploader_MockUp_V2(){}

Event* Uploader_MockUp_V2::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use
){
  return GiveMe_Event(iEvent, ModuleNber_Input, Data_to_Use, 1) ;
}

Event* Uploader_MockUp_V2::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use,
                               const int& CloseWF
){
  if (iEvent >= Get_NberOfEvent() ) return 0 ;
 
  if (!p_TTree) return 0;
  fCurrent = iEvent;
  
//Set off all branches but beforeMerging  
  p_TTree->SetBranchStatus("*",0);
  p_TTree->SetBranchStatus("beforeMerging",1);
  p_TTree->GetEntry(iEvent);
 
//Keep only the entries corresponding to before or after merging 

  if (beforeMerging!=m_BeforeMerging) return 0 ;
  
//The Event is OK, Set on all branches 
  p_TTree->SetBranchStatus("*",1); 
  p_TTree->GetEntry(iEvent);

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
  
  int iCluster_Max = module->size() ;
  for (Int_t iCluster = 0; iCluster < iCluster_Max; iCluster++){
  
//  SetModule
    int ModuleNber_Cur = module->at(iCluster);
    if ( ModuleNber_Input!=-1 && ModuleNber_Cur != ModuleNber_Input ) continue ;

    Module* pModule = V_pModule[ModuleNber_Cur] ;
     
    if(charge->at(iCluster) < 0) continue;
    Cluster* pCluster = new Cluster(
                                    ev     ,
                                    iEvent ,
                                    ModuleNber_Cur 
                                   ) ;
    
    // It seems that there are clusters (cluster charge >=0 but with no pad (charge <0 for all pads)
    // check and store the cluster only if there are pads in it
    int NberOfPads = 0 ; 
    int iPad_Max= pad_charge->at(iCluster).size() ;
    for (Int_t iPad = 0; iPad < iPad_Max; iPad++){
      if(pad_charge->at(iCluster).at(iPad)<0)continue;
      NberOfPads += 1 ;
      
      int iX_Get         = pad_x->at(iCluster).at(iPad); ;
      int iY_Get         = pad_y->at(iCluster).at(iPad); ;
                      
      int iX_Set         = iX_Get ;
      int iY_Set         = iY_Get ;
      
//    Swap X and Y axes
      if (m_Swap_XY==1){                      
        iX_Set = (m_Ny-1) - iY_Get ;
        iY_Set = iX_Get             ;
      }

      if (iX_Set < 0 || iX_Set >= m_Nx || iY_Set < 0 || iY_Set >= m_Ny ){
        std::cout << " Uploader_MockUp_V2::GiveMe_Event "
                  << " Wrong  "  
                  << " iCluster= " << iCluster
                  << " iX_Set= " << iX_Set
                  << " iY_Set= " << iY_Set
                  << std::endl; 
        abort(); 
      }
      
                            
      Pad* pPad = new Pad (p_Model_ReadOutGeometry , 
                           p_Model_Electronics     , 
                           p_Model_Charge1D        ,
                           "noName"                ,
                           ev     ,
                           iEvent ,
                           ModuleNber_Cur ,
                           iX_Set ,iY_Set ); 
                                           
      pPad->Set_AMax(pad_charge->at(iCluster).at(iPad));
      pPad->Set_TMax(pad_time  ->at(iCluster).at(iPad));

      pPad->Clear_ADC() ;
      int iTimeBin_Min = 0 ;
      int iTimeBin_Max = pad_wf_q->at(iCluster).at(iPad).size() ;
      for (Int_t iTimeBin = iTimeBin_Min; iTimeBin < iTimeBin_Max; iTimeBin++){
        int iTimeBinProper = iTimeBin ;
        int ADCProper      = pad_wf_q->at(iCluster)[iPad][iTimeBin] ;
        if ( ADCProper < -250 ) {
          ADCProper = 0 ;
        }
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
      delete pModule ; pModule = 0 ;
    }
  }
  int NberOFModule = pEvent->Get_NberOfModule() ;
  if (NberOFModule==0){
    delete pEvent ;
    pEvent = 0 ;
    
  }

  return pEvent ;
}

void Uploader_MockUp_V2::SetBeforeMerging(const int& i_SetBeforeMerging)
{
  m_BeforeMerging = i_SetBeforeMerging;
}

void Uploader_MockUp_V2::Setwap_XY(const int& i_Swap_XY)
{
  m_Swap_XY = i_Swap_XY;
}

void Uploader_MockUp_V2::Init()
{
   multiplicity = 0;
   charge = 0;
   residual = 0;
   residual_corr = 0;
   dx = 0;
   qfrac = 0;
   time = 0;
   clust_pos = 0;
   clust_pos_err = 0;
   track_pos = 0;
   module = 0;
   pad_charge = 0;
   pad_time = 0;
   wf_width = 0;
   wf_fwhm = 0;
   pad_x = 0;
   pad_y = 0;
   pad_wf_q = 0;
   // Set branch addresses and branch pointers

   fCurrent = -1;
   p_TTree->SetMakeClass(1);

   p_TTree->SetBranchAddress("ev", &ev, &b_ev);
   p_TTree->SetBranchAddress("track", &track, &b_track);
   p_TTree->SetBranchAddress("date", &date, &b_date);
   p_TTree->SetBranchAddress("beforeMerging", &beforeMerging, &b_beforeMerging);
   p_TTree->SetBranchAddress("dEdx", &dEdx, &b_dEdx);
   p_TTree->SetBranchAddress("angle_yz", &angle_yz, &b_angle_yz);
   p_TTree->SetBranchAddress("angle_xy", &angle_xy, &b_angle_xy);
   p_TTree->SetBranchAddress("rob_clusters", &rob_clusters, &b_rob_clusters);
   p_TTree->SetBranchAddress("quality", &quality, &b_quality);
   p_TTree->SetBranchAddress("mom", &mom, &b_mom);
   p_TTree->SetBranchAddress("sina", &sina, &b_sina);
   p_TTree->SetBranchAddress("offset", &offset, &b_offset);
   p_TTree->SetBranchAddress("max_mult", &max_mult, &b_max_mult);
   p_TTree->SetBranchAddress("mean_mult", &mean_mult, &b_mean_mult);
   p_TTree->SetBranchAddress("multiplicity", &multiplicity, &b_multiplicity);
   p_TTree->SetBranchAddress("charge", &charge, &b_charge);
   p_TTree->SetBranchAddress("residual", &residual, &b_residual);
   p_TTree->SetBranchAddress("residual_corr", &residual_corr, &b_residual_corr);
   p_TTree->SetBranchAddress("dx", &dx, &b_dx);
   p_TTree->SetBranchAddress("qfrac", &qfrac, &b_qfrac);
   p_TTree->SetBranchAddress("time", &time, &b_time);
   p_TTree->SetBranchAddress("clust_pos", &clust_pos, &b_clust_pos);
   p_TTree->SetBranchAddress("clust_pos_err", &clust_pos_err, &b_clust_pos_err);
   p_TTree->SetBranchAddress("track_pos", &track_pos, &b_track_pos);
   p_TTree->SetBranchAddress("module", &module, &b_module);
   p_TTree->SetBranchAddress("pad_charge", &pad_charge, &b_pad_charge);
   p_TTree->SetBranchAddress("pad_time", &pad_time, &b_pad_time);
   p_TTree->SetBranchAddress("wf_width", &wf_width, &b_wf_width);
   p_TTree->SetBranchAddress("wf_fwhm", &wf_fwhm, &b_wf_fwhm);
   p_TTree->SetBranchAddress("pad_x", &pad_x, &b_pad_x);
   p_TTree->SetBranchAddress("pad_y", &pad_y, &b_pad_y);
   p_TTree->SetBranchAddress("pad_wf_q", &pad_wf_q, &b_pad_wf_q);
}
