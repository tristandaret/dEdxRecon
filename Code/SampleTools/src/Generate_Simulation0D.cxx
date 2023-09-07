#include "SampleTools/Generate_Simulation0D.h"
#include "Misc/Util.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_Charge0D.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Module.h"
#include "EvtModel/Sample.h"

#include "TRandom3.h"
#include "TFile.h"
#include "TTree.h"
 
void Generate_Simulation0D( 
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
    << " " << "Main_Generate_Simulation0D"                  
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
  std::cout << "Generate_Simulation0D: output file " << OutPutFile << std::endl ;

//
  TFile* pTFile = TFile::Open(OutPutFile.c_str(),"RECREATE");
  if (!pTFile) { 
    std::cout 
      << " Generate_Simulation0D "
      << " pTFile==0 "  
      << std::endl ;
    abort() ;
  }  
  
//  
  TTree* pTTree = new TTree("outtree","Tree of events");
  
  Int_t    ev_Nber  = 0 ;
  
  pTTree->Branch("ev_Nber" ,&ev_Nber ,"ev_Nber/I" ) ;

  double xTrue = 0. ;
  double yTrue = 0. ;
  
  pTTree->Branch("xTrue" ,&xTrue ,"xTrue/D" ) ;
  pTTree->Branch("yTrue" ,&yTrue ,"yTrue/D" ) ;

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
    

//Signal Models
  Model_ReadOutGeometry* pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*     pModel_Electronics     = new Model_Electronics     ;
  Model_Charge0D*        pModel_Charge0D        = new Model_Charge0D        ;
      
      
//Set Signal Diffusion Parameters
  double LocalRC = RC_ns_mmM2 * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge0D->Set_RC(LocalRC) ;
  
  pModel_Charge0D->Set_WidthFromDriftDistance(DD_mm/10.) ;
  
  double Qprim = 200.*1.602176565E-19  ;
  pModel_Charge0D->Set_Qprim(Qprim) ;


  pModel_Charge0D->Set_Gain(Gain) ;


//Random Generators
  TRandom3 aTRandom3 ;
  TRandom3 aTRandom3_ForSmearing  ;
  
  
//Geometry
  double LX       = pModel_ReadOutGeometry->Get_LX      () ;
  double LY       = pModel_ReadOutGeometry->Get_LY      () ;
  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ;
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ;


//Simulation

//Mid position
  int iX_Shoot = Nx/2 ;
  int iY_Shoot = Ny/2 ;

//   std::cout 
//     << "******START******"
//     << " iX_Shoot " << std::setw(3) << std::setprecision(0) << iX_Shoot 
//     << " iY_Shoot " << std::setw(3) << std::setprecision(0) << iY_Shoot 
//     << std::endl ;
  
  double XC = pModel_ReadOutGeometry->Get_XcPad(iX_Shoot,iY_Shoot,0) ;
  double YC = pModel_ReadOutGeometry->Get_YcPad(iX_Shoot,iY_Shoot,0) ;

//
  int iT_Max = NberOfEvents ;
  for (int iT = 0 ; iT< iT_Max; iT++){
    double Time0 = 0. ;
    double XTrue   = aTRandom3.Uniform(-0.5 * LX,0.5 * LX) + XC ;
    double YTrue   = aTRandom3.Uniform(-0.5 * LY,0.5 * LY) + YC ;
    
    int ev             = iT ;
    int iEvent         = iT ;                         

//  Create Event
    Event*  pEvent = new Event( 
                               ev     ,
                               iEvent ,
                               pModel_ReadOutGeometry  ,
                               pModel_Electronics      ,
                               pModel_Charge0D            
                              ); 
                              
//  Create Module: only one
    int ModuleNber_Cur = 0  ;  

    Module* pModule_Single = new Module( 
                             ev     ,
                             iEvent ,
                             ModuleNber_Cur   ,
                             pModel_ReadOutGeometry  ,
                             pModel_Electronics      ,
                             pModel_Charge0D            
                            ); 
                            
//  Add current Module to the current Event
    pEvent->Add_Module(pModule_Single) ;
                            
//  Create pad
    
    std::cout 
      << "******EVENT******"
      << " XTrue " << std::setw(10) << std::setprecision(6) << XTrue 
      << " YTrue " << std::setw(10) << std::setprecision(6) << YTrue 
      << std::endl ;
      
    for (int iXl = -1 ; iXl<= 1; iXl++){
      for (int iYl = -1 ; iYl<= 1; iYl++){

//         std::cout 
//           << "----PAD-----"
//           << std::endl ;

        int iX = iX_Shoot + iXl ;
        int iY = iY_Shoot + iYl ;
        
//         std::cout 
//           << "GENE " 
//           << " iX " << std::setw(3)  << std::setprecision(0) << iX
//           << " iY " << std::setw(3)  << std::setprecision(0) << iY
//           << " X "  << std::setw(10) << std::setprecision(6) << pModel_ReadOutGeometry->Get_XcPad(iX,iY,0)
//           << " Y "  << std::setw(10) << std::setprecision(6) << pModel_ReadOutGeometry->Get_XcPad(iX,iY,0)
//           << std::endl ;

        Pad* pPad = new Pad (
                             pModel_ReadOutGeometry, 
                             pModel_Electronics, 
                             pModel_Charge0D, 
                             "Noname"     ,
                             ev             ,
                             iEvent         ,
                             ModuleNber_Cur ,
                             iX, iY ); 
        pPad->SetSignalModel( Time0 ,XTrue ,YTrue ); 
        pModule_Single->Add_Pad(pPad ) ;
        
//      Generate unsmeared WF from t=0 to iTimeBin_Max time bins
        std::vector < double > v_WF_ADC  ;
        std::vector < int    > v_WF_TimeBin ;

        int iTimeBin_Max = 100 ;
        for (int iTimeBin = 1 ; iTimeBin <= iTimeBin_Max ; iTimeBin++){
          double TimeCur = double(iTimeBin)*40E-9 ;
         
          double A_Pad = pPad->Get_APad(TimeCur) ;
          v_WF_ADC    .push_back( A_Pad    ) ;
          v_WF_TimeBin.push_back( iTimeBin ) ;
          
//           std::cout 
//             << "GENE " 
//             << " " << std::setw(4) << std::setprecision(0) << iTimeBin
//             << " " << std::setw(7) << std::setprecision(1) << A_Pad   
//             << " " << std::setw(8) << std::setprecision(0) << pPad->Get_Qpad(TimeCur)/1.602176565E-19  
//             << std::endl ;
            
        }
        int Nber_WF_ADC = v_WF_ADC.size() ;

//      Smear the WF        
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
          
//           std::cout 
//             << "SMEA " 
//             << " " << std::setw(4) << std::setprecision(0) << iTimeBinProper
//             << " " << std::setw(7) << std::setprecision(1) << ADCProper   
//             << std::endl ;

          if ( iTimeBinProper < 510 ) {
            pPad->Set_ADC(iTimeBinProper,ADCProper ) ;
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
        
        pPad->Set_AMax( ADCProper_max     );
        pPad->Set_TMax( TimeBinProper_max );
        
        pPad->Wf_DoClosure() ;
        
        pPad->Set_Data_2Use(0) ;
      }
    }
      
//  Create Cluster: one at most per column
    for (int iX = 0 ; iX< Nx; iX++){
      Cluster* pCluster = new Cluster(
                                      ev     ,
                                      iEvent ,
                                      ModuleNber_Cur 
                                     ) ;
      int NPads = pModule_Single->Get_NberOfPads();
      for (int iP = 0 ; iP< NPads; iP++){
        Pad* pPad = pModule_Single->Get_Pad(iP);
        int Pad_iX = pPad->Get_iX() ;
        if (Pad_iX==iX) pCluster->Add_Pad(pPad) ;
      }
      if (pCluster->Get_NberOfPads()==0){
        delete pCluster ;
      }else{
        pModule_Single->Add_Cluster(pCluster);
      }
    }

//  Set Clusters
    int iCLuster_Max = pModule_Single->Get_NberOfCluster() ;
    for (int iCLuster = 0 ; iCLuster< iCLuster_Max; iCLuster++){
      Cluster* pCluster = pModule_Single->Get_Cluster(iCLuster);
      pCluster->DoClosure() ;
    }
    
//----------------------------------------------------//
//  Fill event Data
    ev_Nber  = pEvent->Get_EventNber() ;
    xTrue = (XTrue - XC)*10. ;
    yTrue = (YTrue - YC)*10. ;

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
  std::cout <<"Generate_Simulation0D: Done" << std::endl ;

}
