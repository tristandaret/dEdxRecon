#include "SampleTools/Generate_Simulation1D.h"
#include "Misc/Util.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_Charge1D.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Module.h"
#include "EvtModel/Sample.h"

#include "TRandom3.h"
#include "TFile.h"
#include "TTree.h"
 
void Generate_Simulation1D( 
                         const int&    iOptQuiet            ,
                         const std::string& Output_RootFile ,
                         const int&    iOptSmear            ,
                         const double& RC_ns_mmM2           ,
                         const double& Gain                 ,
                         const double& DD_mm                ,
                         const int&    NberOfEvents
){
//
  std::cout 
    << " " << "Main_Generate_Simulation1D"                  
    << " " << Output_RootFile                  
    << " " << iOptSmear                  
    << " " << RC_ns_mmM2                  
    << " " << Gain                  
    << " " << DD_mm                  
    << " " << NberOfEvents              
    << std::endl ;

  if (iOptQuiet==1) return ;

//
  std::string OutPutFile = "OUT_SampleGenerator/" + Output_RootFile + ".root" ; 

  std::cout << std::endl ;
  std::cout << "Generate_Simulation1D: output file " << OutPutFile << std::endl ;

//
  TFile* pTFile = TFile::Open(OutPutFile.c_str(),"RECREATE");
  if (!pTFile) { 
    std::cout 
      << " Generate_Simulation1D "
      << " pTFile==0 "  
      << std::endl ;
    abort() ;
  }  
  
//  
  TTree* pTTree = new TTree("outtree","Tree of events");
  
  Int_t    ev_Nber  = 0 ;
  
  pTTree->Branch("ev_Nber" ,&ev_Nber ,"ev_Nber/I" ) ;

  std::vector< int > Cluster_NPads      ;
  std::vector< int > Cluster_ModuleNber ;
  std::vector< int > Cluster_iX         ;
  std::vector< int > Cluster_iY         ;
  
  pTTree->Branch("Cluster_NPads"      ,&Cluster_NPads      ) ;
  pTTree->Branch("Cluster_ModuleNber" ,&Cluster_ModuleNber ) ;
  pTTree->Branch("Cluster_iX"         ,&Cluster_iX         ) ;
  pTTree->Branch("Cluster_iY"         ,&Cluster_iY         ) ;

  std::vector< int > Pad_iX         ;
  std::vector< int > Pad_iY         ;
  std::vector< int > Pad_ModuleNber ;
  std::vector< int > Pad_charge     ;
  std::vector< int > Pad_time       ;
  std::vector< int > Pad_Nwf        ;
  std::vector< int > Pad_wf_q       ;
  
  pTTree->Branch("Pad_iX"         ,&Pad_iX         ) ;
  pTTree->Branch("Pad_iY"         ,&Pad_iY         ) ;
  pTTree->Branch("Pad_ModuleNber" ,&Pad_ModuleNber ) ;
  pTTree->Branch("Pad_charge"     ,&Pad_charge     ) ;
  pTTree->Branch("Pad_time"       ,&Pad_time       ) ;
  pTTree->Branch("Pad_Nwf"        ,&Pad_Nwf        ) ; 
  pTTree->Branch("Pad_wf_q"       ,&Pad_wf_q       ) ; 
    
  std::vector< int > Pad_charge_True     ;
  std::vector< int > Pad_time_True       ;
 
  pTTree->Branch("Pad_charge_True"     ,&Pad_charge_True     ) ;
  pTTree->Branch("Pad_time_True"       ,&Pad_time_True       ) ;

//Signal Models
  Model_ReadOutGeometry* pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*     pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*        pModel_Charge1D        = new Model_Charge1D        ;
      
      
//Set Signal Diffusion Parameters
  double LocalRC = RC_ns_mmM2 * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge1D->Set_RC(LocalRC) ;
  
  pModel_Charge1D->Set_WidthFromDriftDistance(DD_mm/10.) ;
  
  double Lambda = pModel_Charge1D->Get_Lambda() ;
  pModel_Charge1D->Set_Lambda(Lambda*2.) ;


  pModel_Charge1D->Set_Gain(Gain) ;


//Random Generators
  TRandom3 aTRandom3 ;
  TRandom3 aTRandom3_ForSmearing  ;
  TRandom3 aTRandom3_ForSmearing2 ;
  
  
//Geometry
  double LY       = pModel_ReadOutGeometry->Get_LY      () ;
  double Ypad_min = pModel_ReadOutGeometry->Get_Ypad_min() ;
  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ;
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ;


//Simulation

//Mid position
  double Ymid = Ypad_min + 0.5 * LY + double (Ny/2) * LY ;

//
  int iT_Max = NberOfEvents ;
  for (int iT = 0 ; iT< iT_Max; iT++){
    double Time0 = 0. ;
    double YTrue   = aTRandom3.Gaus(0.,1E-2) + Ymid ;
    
    int ev             = iT ;
    int iEvent         = iT ;                         

//  Create Event
    Event*  pEvent = new Event( 
                               ev     ,
                               iEvent ,
                               pModel_ReadOutGeometry  ,
                               pModel_Electronics      ,
                               pModel_Charge1D            
                              ); 
                              
//  Create Module: only one
    int ModuleNber_Cur = 0  ;  

    Module* pModule_Single = new Module( 
                             ev     ,
                             iEvent ,
                             ModuleNber_Cur   ,
                             pModel_ReadOutGeometry  ,
                             pModel_Electronics      ,
                             pModel_Charge1D            
                            ); 
                            
//  Add current Module to the current Event
    pEvent->Add_Module(pModule_Single) ;
                            
//  Create Cluster: one in each column
    for (int iX = 0 ; iX< Nx; iX++){
      Cluster* pCluster = new Cluster(
                                      ev     ,
                                      iEvent ,
                                      ModuleNber_Cur 
                                     ) ;

      double XTrack_at_C = 0. ;
      double YTrack_at_C = YTrue ;


//    Create Pads and store them in the current cluster: only the leading crossed by the track and the 2 next pads
      for (int iY = 0 ; iY< Ny; iY++){
        double Ypad_H = pModel_ReadOutGeometry->Get_YHPad(iX,iY,ModuleNber_Cur );
        double Ypad_L = pModel_ReadOutGeometry->Get_YLPad(iX,iY,ModuleNber_Cur );         ;
        
        if ( Ypad_L <= YTrack_at_C && Ypad_H >= YTrack_at_C ){

//        Create Up pad
          if ((iY+1) <  Ny){
                             Pad* pPad = new Pad (
                                                  pModel_ReadOutGeometry, 
                                                  pModel_Electronics, 
                                                  pModel_Charge1D, 
                                                  "Up"     ,
                                                  ev             ,
                                                  iEvent         ,
                                                  ModuleNber_Cur ,
                                                  iX, iY+1 ); 
                             pPad->SetSignalModel( Time0 ,XTrack_at_C ,YTrack_at_C ); 
                             pModule_Single->Add_Pad(pPad ) ;
                             pCluster->Add_Pad(pPad) ;
                           }
//        Create Leading pad
                             Pad* pPad = new Pad ( 
                                                  pModel_ReadOutGeometry, 
                                                  pModel_Electronics, 
                                                  pModel_Charge1D, 
                                                  "Central",
                                                  ev             ,
                                                  iEvent         ,
                                                  ModuleNber_Cur ,
                                                  iX,iY  ); 
                             pPad->SetSignalModel( Time0 ,XTrack_at_C ,YTrack_at_C ); 
                             pModule_Single->Add_Pad(pPad ) ;
                             pCluster->Add_Pad(pPad) ; 
//        Create Down pad
          if ((iY-1) >=  0){ 
                             Pad* pPad = new Pad (
                                                  pModel_ReadOutGeometry, 
                                                  pModel_Electronics, 
                                                  pModel_Charge1D, 
                                                  "Down"   ,
                                                  ev             ,
                                                  iEvent         ,
                                                  ModuleNber_Cur ,
                                                  iX,iY-1); 
                             pPad->SetSignalModel( Time0 ,XTrack_at_C ,YTrack_at_C ); 
                             pModule_Single->Add_Pad(pPad ) ;
                             pCluster->Add_Pad(pPad) ; 
                           }
        }
      }
//    Add the current cluster to the current Module
      pModule_Single->Add_Cluster(pCluster);
    }


//  Generate Signal in first column pads and smear it in all pads of the same row in all columns
    int iX = 0 ;
    for (int iY = 0 ; iY< Ny; iY++){
      if ( pModule_Single->Get_IsThisPadExisting(iX,iY) == 1 ){
        Pad*   pPad = pModule_Single->Get_Pad (iX,iY) ;

//      Generate unsmeared WF from t=0 to iTimeBin_Max time bins
        std::vector < double > v_WF_ADC  ;
        std::vector < int    > v_WF_TimeBin ;

        int ADC_max     = 0 ;
        int TimeBin_max = 0 ;
        int iTimeBin_Max = 100 ;
        for (int iTimeBin = 1 ; iTimeBin <= iTimeBin_Max ; iTimeBin++){
          double TimeCur = double(iTimeBin)*40E-9 ;
         
          double A_Pad = pPad->Get_APad(TimeCur) ;
          v_WF_ADC    .push_back( A_Pad    ) ;
          v_WF_TimeBin.push_back( iTimeBin ) ;
          if (iTimeBin==1){
            ADC_max     = A_Pad    ;
            TimeBin_max = iTimeBin ;
          }
          if (A_Pad>ADC_max){
            ADC_max     = A_Pad    ;
            TimeBin_max = iTimeBin ;
          }
        }
        int Nber_WF_ADC = v_WF_ADC.size() ;
        
//      Generate smeared WF in in all pads of the same row in all columns
        for (int iXl = 0 ; iXl< Nx; iXl++){
          if ( pModule_Single->Get_IsThisPadExisting(iXl,iY) == 1 ){
            Pad*   pPadl = pModule_Single->Get_Pad (iXl,iY) ;
            
            int ADCProper_max     = 0 ;
            int TimeBinProper_max = 0 ;
            int F_max             = 1 ;
            for (int IndexADC = 0 ; IndexADC < Nber_WF_ADC ; IndexADC++){
              int    iTimeBin = v_WF_TimeBin[IndexADC] ;
              double ADC      = v_WF_ADC    [IndexADC] ;
              
              int iTimeBinProper = iTimeBin ;
              int ADCProper      = std::floor( ADC ) ;
              
              int ADCsmear = ADCProper  ;
              if ( iOptSmear == 1 ) ADCsmear = std::floor( ADC + aTRandom3_ForSmearing.Gaus(0.,6.) ) ;
              if ( iOptSmear == 2 ) ADCsmear = std::floor( ADC + aTRandom3_ForSmearing.Gaus(0.,std::sqrt(std::fabs(ADC))) ) ;
              ADCProper = ADCsmear ;
              
              if ( iTimeBinProper < 510 ) {
                pPadl->Set_ADC(iTimeBinProper,ADCProper ) ;
                if (F_max==1){
                  F_max             = 0              ;
                  ADCProper_max     = ADCProper      ;
                  TimeBinProper_max = iTimeBinProper ;
                }
                if (ADCProper_max<ADCProper){
                  ADCProper_max     = ADCProper      ;
                  TimeBinProper_max = iTimeBinProper ;
                }
              }
            }
            
            pPadl->Set_AMax( ADCProper_max     );
            pPadl->Set_TMax( TimeBinProper_max );
            
            pPadl->Set_AMax_True( ADC_max     );
            pPadl->Set_TMax_True( TimeBin_max );
            
            pPadl->Wf_DoClosure() ;
            
            pPadl->Set_Data_2Use(0) ;
          }
        }
        
        
      }
    }

//Set Clusters
    int iCLuster_Max = pModule_Single->Get_NberOfCluster() ;
    for (int iCLuster = 0 ; iCLuster< iCLuster_Max; iCLuster++){
      Cluster* pCluster = pModule_Single->Get_Cluster(iCLuster);
      pCluster->DoClosure() ;
    }
    
//----------------------------------------------------//
//  Fill event Data
    ev_Nber  = pEvent->Get_EventNber() ;

    int NberOfModule = pEvent->Get_NberOfModule() ;
    
//  Fill Cluster Data
    Cluster_NPads     .clear()  ;
    Cluster_ModuleNber.clear()  ;
    Cluster_iX        .clear()  ;
    Cluster_iY        .clear()  ;
    
    for (int iModule = 0; iModule < NberOfModule; iModule++) {
      Module* pModule = pEvent->Get_Module_InArray(iModule) ;
      
      int NClusters = pModule->Get_NberOfCluster();
      for (int iC = 0 ; iC< NClusters; iC++){
        Cluster* pCluster = pModule->Get_Cluster(iC);
        
        Cluster_NPads     .push_back( pCluster->Get_NberOfPads() ) ;
        Cluster_ModuleNber.push_back( pCluster->Get_ModuleNber() ) ;
       
        int NPads = pCluster->Get_NberOfPads();
        for (int iP = 0 ; iP< NPads; iP++){
          const Pad* pPad = pCluster->Get_Pad(iP);

          int iX = pPad->Get_iX();
          int iY = pPad->Get_iY();
          Cluster_iX.push_back(  iX ) ;
          Cluster_iY.push_back(  iY ) ;
          
        }
        
      }  
    }

//  Fill Pad Data
    Pad_iX         .clear()  ;
    Pad_iY         .clear()  ;
    Pad_ModuleNber .clear()  ;
    Pad_charge     .clear()  ;
    Pad_time       .clear()  ;
    Pad_Nwf        .clear()  ;
    Pad_wf_q       .clear()  ;
    
    Pad_charge_True     .clear()  ;
    Pad_time_True       .clear()  ;
    
    for (int iModule = 0; iModule < NberOfModule; iModule++) {
      Module* pModule = pEvent->Get_Module_InArray(iModule) ;
      
      int NPads = pModule->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
        Pad* pPad = pModule->Get_Pad(iP);
        
        Pad_iX         .push_back( pPad->Get_iX()         ) ;
        Pad_iY         .push_back( pPad->Get_iY()         ) ;
        Pad_ModuleNber .push_back( pPad->Get_ModuleNber() ) ;
        Pad_charge     .push_back( pPad->Get_AMax()       ) ;
        Pad_time       .push_back( pPad->Get_TMax()       ) ;
        
        Pad_charge_True     .push_back( pPad->Get_AMax_True()       ) ;
        Pad_time_True       .push_back( pPad->Get_TMax_True()       ) ;
        
        std::vector < int > The_v_ADC = pPad->Get_vADC() ;
        int iTem_Max = The_v_ADC.size() ;

        Pad_Nwf.push_back( iTem_Max ) ;    
        
        for (Int_t iTem = 0; iTem < iTem_Max; iTem++){
          Pad_wf_q.push_back( The_v_ADC[iTem] ) ;    
        } 
      }
    }
    
    pTTree->Fill();

//----------------------------------------------------//
    
    delete pEvent ;
  }

//
  pTFile->Write();
  
  delete pTFile ;

//
  std::cout <<"Generate_Simulation1D: Done" << std::endl ;

}
