#include "SampleTools/Uploader_Simulation.h"

Uploader_Simulation::Uploader_Simulation(
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
  std::cout << "  Uploader_Simulation::Uploader_Simulation "<< std::endl ;
}


Uploader_Simulation::~Uploader_Simulation()
{
//std::cout << " In Uploader_Simulation::~Uploader_Simulation() " << std::endl ;
}

Event* Uploader_Simulation::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use
){
  return GiveMe_Event(iEvent, ModuleNber_Input, Data_to_Use, 1) ;
}

Event* Uploader_Simulation::GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use,
                               const int& CloseWF
){
  if (iEvent >= Get_NberOfEvent() ) return 0 ;
 
  Int_t    ev_Nber  = 0 ;

  std::vector< int      >* Cluster_NPads      = 0 ;
  std::vector< int      >* Cluster_ModuleNber = 0 ;
  std::vector< int      >* Cluster_iX         = 0 ;
  std::vector< int      >* Cluster_iY         = 0 ;
  
  std::vector< int      >* Pad_iX         = 0 ;
  std::vector< int      >* Pad_iY         = 0 ;
  std::vector< int      >* Pad_ModuleNber = 0 ;
  std::vector< int      >* Pad_charge     = 0 ;
  std::vector< int      >* Pad_time       = 0 ;
  std::vector< int      >* Pad_Nwf        = 0 ;
  std::vector< int      >* Pad_wf_q       = 0 ;
  
  std::vector< int      >* Pad_charge_True     = 0 ;
  std::vector< int      >* Pad_time_True       = 0 ;

  p_TTree->SetBranchAddress("ev_Nber"       ,&ev_Nber        ) ;

  p_TTree->SetBranchAddress("Cluster_NPads"      ,&Cluster_NPads      ) ;
  p_TTree->SetBranchAddress("Cluster_ModuleNber" ,&Cluster_ModuleNber ) ;
  p_TTree->SetBranchAddress("Cluster_iX"         ,&Cluster_iX         ) ;
  p_TTree->SetBranchAddress("Cluster_iY"         ,&Cluster_iY         ) ;

  p_TTree->SetBranchAddress("Pad_iX"         ,&Pad_iX         ) ;
  p_TTree->SetBranchAddress("Pad_iY"         ,&Pad_iY         ) ;
  p_TTree->SetBranchAddress("Pad_ModuleNber" ,&Pad_ModuleNber ) ;
  p_TTree->SetBranchAddress("Pad_charge"     ,&Pad_charge     ) ;
  p_TTree->SetBranchAddress("Pad_time"       ,&Pad_time       ) ;
  p_TTree->SetBranchAddress("Pad_Nwf"        ,&Pad_Nwf        ) ; 
  p_TTree->SetBranchAddress("Pad_wf_q"       ,&Pad_wf_q       ) ; 

  p_TTree->SetBranchAddress("Pad_charge_True"     ,&Pad_charge_True     ) ;
  p_TTree->SetBranchAddress("Pad_time_True"       ,&Pad_time_True       ) ;


// 
  int TimeBin_Max = 510 ;
  int TimeBin_Min =   0 ;

//
  p_TTree->GetEntry(iEvent);
  
//  
  
  Event*  pEvent = new Event( 
                             ev_Nber ,
                             iEvent  ,
                             p_Model_ReadOutGeometry  ,
                             p_Model_Electronics      ,
                             p_Model_Charge1D            
                            ); 
                                
  std::vector < Module* > V_pModule ;
  int iMod_Max = 8 ;
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = new Module( 
                             ev_Nber ,
                             iEvent  ,
                             iMod    ,
                             p_Model_ReadOutGeometry  ,
                             p_Model_Electronics      ,
                             p_Model_Charge1D            
                            ); 
    V_pModule.push_back( pModule ) ;
  }
     
//Fill Pad Data
  int Kounter_wf_Pad     = 0 ;
  int Kounter_wf_Pad_MAX = (*Pad_wf_q).size() ;
  int NPads = (*Pad_charge).size() ;
  for (int iP = 0 ; iP< NPads; iP++){
    int iX             = (*Pad_iX)[iP] ;
    int iY             = (*Pad_iY)[iP] ;

    int ModuleNber = (*Pad_ModuleNber)[iP] ;
    Module* pModule = V_pModule[ModuleNber] ;

    Pad* pPad = new Pad (p_Model_ReadOutGeometry , 
                         p_Model_Electronics     , 
                         p_Model_Charge1D        ,
                         "noName"                ,
                         ev_Nber    ,
                         iEvent     ,
                         ModuleNber ,
                         iX ,iY ); 
    pPad->Set_AMax ((*Pad_charge)[iP]) ;
    pPad->Set_TMax ((*Pad_time  )[iP]) ;
    
    pPad->Set_AMax_True ((*Pad_charge_True)[iP]) ;
    pPad->Set_TMax_True ((*Pad_time_True  )[iP]) ;
    
    pPad->Clear_ADC() ;
    int iTimeBin_Max = (*Pad_Nwf)[iP] ;
    for (Int_t iTimeBin = 0; iTimeBin < iTimeBin_Max; iTimeBin++){
      if ( Kounter_wf_Pad == Kounter_wf_Pad_MAX ){
        std::cout 
          << "Uploader_Simulation::GiveMe_Event "
          << " Kounter_wf_Pad == Kounter_wf_Pad_MAX  "
          << " Kounter_wf_Pad = "     << Kounter_wf_Pad
          << " Kounter_wf_Pad_MAX = " << Kounter_wf_Pad_MAX
          << std::endl ;
        abort();
      }
      
      int iTimeBinProper = iTimeBin ;
      int ADCProper      = (*Pad_wf_q)[Kounter_wf_Pad] ;
      if ( ADCProper < -250 ) ADCProper = 0 ;
      if ( 
         iTimeBinProper >= TimeBin_Min 
      && iTimeBinProper <  TimeBin_Max 
      ) 
      pPad->Set_ADC(iTimeBinProper,ADCProper ) ;
      
      Kounter_wf_Pad += 1 ;
    }
    if(CloseWF == 1) pPad->Wf_DoClosure() ;
    
    pPad->Set_Data_2Use(Data_to_Use) ;

    pModule->Add_Pad(pPad ) ;
  
  }


//Fill Cluster Data
  int Kounter_ixiy_Cluster     = 0 ;
  int Kounter_ixiy_Cluster_MAX = (*Cluster_iX).size() ;
  int NClusters = (*Cluster_NPads).size() ;
  for (Int_t iCluster = 0; iCluster < NClusters; iCluster++){
    int ModuleNber = (*Cluster_ModuleNber)[iCluster] ; 
    Module* pModule = V_pModule[ModuleNber] ;

    Cluster* pCluster = new Cluster(
                                    ev_Nber        ,
                                    iEvent         ,
                                    ModuleNber  
                                   ) ;
    
    int NPads = (*Cluster_NPads)[iCluster] ; 
    for (Int_t iPad = 0; iPad < NPads; iPad++){
      
      if ( Kounter_ixiy_Cluster == Kounter_ixiy_Cluster_MAX ){
        std::cout 
          << "Uploader_Simulation::GiveMe_Event "
          << " Kounter_ixiy_Cluster == Kounter_ixiy_Cluster_MAX  "
          << " Kounter_ixiy_Cluster = "     << Kounter_ixiy_Cluster
          << " Kounter_ixiy_Cluster_MAX = " << Kounter_ixiy_Cluster_MAX
          << std::endl ;
        abort();
      }
      
      int iX = (*Cluster_iX)[Kounter_ixiy_Cluster] ;
      int iY = (*Cluster_iY)[Kounter_ixiy_Cluster] ;
      
      Pad* pPad_forCluster = pModule->Get_Pad(iX,iY) ;
      if (pPad_forCluster==0) {
        std::cout 
          << " pPad_forCluster==0 " 
          << " Kounter_ixiy_Cluster " << Kounter_ixiy_Cluster
          << " iX  " << iX 
          << " iY  " << iY 
          << std::endl ;
        abort();
      }
      pCluster->Add_Pad(pPad_forCluster);
      
      Kounter_ixiy_Cluster += 1 ;
    }
    if (NPads!=0){
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
