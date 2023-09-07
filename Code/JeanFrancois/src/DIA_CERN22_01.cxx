#include "JeanFrancois/DIA_CERN22_01.h"
#include "Misc/Util.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"

void DIA_CERN22_01()
{ 
//
  std::string Tag = "DIA_CERN22_01" ;

  int intUploader         =  3 ; // CERN22 Data 
  int ModuleNber          = -1 ; // Work on all modules nbers
  int Data_to_Use         = 0  ; // Use ntuple Maxs


  std::string EventFile = "BIDON" ;
  int iFile_Min = 0 ;
  int iFile_Max = 1 ;
  for (int iFile = iFile_Min ; iFile< iFile_Max; iFile++){
    if (iFile==0)  EventFile = "../Real_Data/CERN22_V6/All_ERAMS_350V_412ns_e+_0p5GeV_1_iter2.root" ;
    if (iFile==1)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_1GeV_iter0.root"     ;
    if (iFile==2)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_0p75GeV_iter0.root" ;
    if (iFile==3)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu-_1GeV_iter0.root"    ;
    if (iFile==4)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_mu+_1GeV_iter0.root"    ;
    if (iFile==5)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_e+_0p5GeV_iter0.root"   ;
    if (iFile==6)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_p_1GeV_iter0.root"      ;
    if (iFile==7)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_p_0p75GeV_iter0.root"   ;
    if (iFile==8)  EventFile = "../Real_Data/CERN22_V6/ERAM18_HS/ERAM18_350V_412ns_pi+_0p5GeV_iter0.root"  ;


//  Set output dir name
    std::string AnalyseName = Tag ;
    std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;

    std::string OUTDirName = "OUT_JeanFrancois/Dir_" + base_EventFile + "_" + AnalyseName + "/";

    MyMakeDir(OUTDirName) ;
    std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
    std::cout << std::endl ;


//  Redirect Output
    std::cout << std::endl ;
    std::ostringstream aostringstream_OutPut_Analysis ;
    aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
    aostringstream_OutPut_Analysis << OUTDirName <<AnalyseName<< ".txt" ;
    
    std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
    
    std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
    std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
    std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file


//
    std::cout << std::endl ;
    std::cout << " DIA_CERN22_01 " << std::endl ;
    std::cout << "      Tag          : " << Tag          << std::endl ;
    std::cout << "      EventFile    : " << EventFile    << std::endl ;
    std::cout << "      intUploader  : " << intUploader  << std::endl ;
    std::cout << "      ModuleNber   : " << ModuleNber   << std::endl ;
    std::cout << "      Data_to_Use  : " << Data_to_Use   << std::endl ;
    std::cout << std::endl ;
    std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
    std::cout << std::endl ;


//  Events Uploader  
    Uploader* pUploader = GiveMe_Uploader(intUploader,EventFile) ;


//  Module Stat Histos
    std::string Name  = "pTH1_Module"; 
    std::string Title = "% of entries containing such Module Nber"; 

    TH1F* pTH1_Module = new TH1F(Name.c_str(),Title.c_str(),8, -0.5 , 7.5 );

    Name  = "pTH1_Logic0123"; 
    Title = "% of entries with Nber of modules = 0, 1, 2, 3 or  4 of the 0/1/2/3 Modules"; 

    TH1F* pTH1_Logic0123 = new TH1F(Name.c_str(),Title.c_str(),5, -0.5 , 4.5 );

    Name  = "pTH1_Logic0123_Inc"; 
    Title = "% of entries with Nber of modules >= 1, 2, 3 or  4 of the 0/1/2/3 Modules"; 

    TH1F* pTH1_Logic0123_Inc = new TH1F(Name.c_str(),Title.c_str(),4,  0.5 , 4.5 );

    Name  = "pTH1_Logic4567"; 
    Title = "% of entries containing Nber of modules = 0, 1, 2, 3 or  4 of the 4/5/6/7 Modules"; 

    TH1F* pTH1_Logic4567 = new TH1F(Name.c_str(),Title.c_str(),5, -0.5 , 4.5 );

    Name  = "pTH1_Logic4567_Inc"; 
    Title = "% of entries containing Nber of modules >= 1, 2, 3 or  4 of the 4/5/6/7 Modules"; 

    TH1F* pTH1_Logic4567_Inc = new TH1F(Name.c_str(),Title.c_str(),4,  0.5 , 4.5 );


//  Cluster Multiplicity Histos
    std::vector < TH1F* > V_pTH1F_ClusterMultiplicity ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::ostringstream aostringstreamNAME ;
      aostringstreamNAME << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME << "ClusterMultiplicity"  << iMod;
      std::string Name =  aostringstreamNAME.str() ;
            
      std::ostringstream aostringstreamTITLE ;
      aostringstreamTITLE << std::setiosflags(std::ios::fixed) ;
      aostringstreamTITLE << "ClusterMultiplicity : Mod "  << iMod;
      std::string Title =  aostringstreamTITLE.str() ;

      TH1F* pTH1 = new TH1F(Name.c_str(),Title.c_str(),37,  -0.5 , 36.5 );
    
      V_pTH1F_ClusterMultiplicity.push_back(pTH1);
    
    }
  
//  Pad Clusters Multiplicity Histos
    std::vector < TH1F* > V_pTH1F_PadMultiplicity ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::ostringstream aostringstreamNAME ;
      aostringstreamNAME << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME << "PadMultiplicity"  << iMod;
      std::string Name =  aostringstreamNAME.str() ;
      
      std::ostringstream aostringstreamTITLE ;
      aostringstreamTITLE << std::setiosflags(std::ios::fixed) ;
      aostringstreamTITLE << "PadMultiplicity : Mod "  << iMod;
      std::string Title =  aostringstreamTITLE.str() ;

      TH1F* pTH1 = new TH1F(Name.c_str(),Title.c_str(),11,  -0.5 , 10.5 );
    
      V_pTH1F_PadMultiplicity.push_back(pTH1);
    
    }
    
//  T Leading Histos
    std::vector < TH1F* > V_pTH1F_TLeading_1 ;
    std::vector < TH1F* > V_pTH1F_TLeading_2 ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::ostringstream aostringstreamNAME_A ;
      aostringstreamNAME_A << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME_A << "TLeading_A"  << iMod;
      std::string NAME_A = aostringstreamNAME_A.str() ;
      
      std::ostringstream aostringstreamNAME_B ;
      aostringstreamNAME_B << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME_B << "TLeading_B"  << iMod;
      std::string NAME_B = aostringstreamNAME_B.str() ;
      
      std::ostringstream aostringstreamTITLE ;
      aostringstreamTITLE << std::setiosflags(std::ios::fixed) ;
      aostringstreamTITLE << "TLeading: Mod "  << iMod;
      std::string  Title =  aostringstreamTITLE.str() ;

      TH1F* pTH1_1 = new TH1F(NAME_A.c_str(),Title.c_str(),11,  -0.5 , 10.5 );
      TH1F* pTH1_2 = new TH1F(NAME_B.c_str(),Title.c_str(),511,  -0.5 , 510.5 );
    
      V_pTH1F_TLeading_1.push_back(pTH1_1);
      V_pTH1F_TLeading_2.push_back(pTH1_2);
    
    }
  
//  A Leading Histos
    std::vector < TH1F* > V_pTH1F_ALeading ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::ostringstream aostringstreamNAME ;
      aostringstreamNAME << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME << "ALeading"  << iMod;
      std::string Name =  aostringstreamNAME.str() ;
      
      std::ostringstream aostringstreamTITLE ;
      aostringstreamTITLE << std::setiosflags(std::ios::fixed) ;
      aostringstreamTITLE << "ALeading : Mod "  << iMod;
      std::string Title =  aostringstreamTITLE.str() ;

      TH1F* pTH1 = new TH1F(Name.c_str(),Title.c_str(),4097,  -0.5 , 4096.5 );
    
      V_pTH1F_ALeading.push_back(pTH1);
    
    }

//  TPad-T Leading Histos
    std::vector < TH1F* > V_pTH1F_TPadMTLeading ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::ostringstream aostringstreamNAME ;
      aostringstreamNAME << std::setiosflags(std::ios::fixed) ;
      aostringstreamNAME << "TPadMTLeading"  << iMod;
      std::string Name =  aostringstreamNAME.str() ;
      
      std::ostringstream aostringstreamTITLE ;
      aostringstreamTITLE << std::setiosflags(std::ios::fixed) ;
      aostringstreamTITLE << "TPad-TLeading : Mod "  << iMod;
      std::string Title =  aostringstreamTITLE.str() ;

      TH1F* pTH1 = new TH1F(Name.c_str(),Title.c_str(),151,  -149.5 , 300.5 );
    
      V_pTH1F_TPadMTLeading.push_back(pTH1);
    
    }
    
//
    int NberOfEvent = pUploader->Get_NberOfEvent() ;
    double TheWeight = 100./double (NberOfEvent) ;

    std::vector < int > v_Kounter_Module(8,0) ;
    
    int KountNoModule = 0 ;

//  Loop On Events
    for (int iEvent = 0; iEvent < NberOfEvent; iEvent++) {
      
      Event*  pEvent = pUploader->GiveMe_Event( iEvent, ModuleNber, Data_to_Use );

      if (!pEvent) continue ;
      
      int NberOfModule = pEvent->Get_NberOfModule() ;
      
//    Module stat
      std::vector < int > v_IOK_Module(8,0) ;

//    Loop On Modules
      for (int iModule = 0; iModule < NberOfModule; iModule++) {
        Module* pModule = pEvent->Get_Module_InArray(iModule) ;
        int ModuleNber = pModule->Get_ModuleNber() ;

        v_IOK_Module[ModuleNber] = 1 ;

      }

//    Stat of Modules in events    
      int SumOf_IOK = 0 ;
      for (int iMod = 0; iMod < 8; iMod++) {

        if (v_IOK_Module[iMod] == 1 ) {
          v_Kounter_Module[iMod] += 1 ;
          pTH1_Module->Fill( double(iMod) , TheWeight ) ;
        }
        SumOf_IOK += v_IOK_Module[iMod] ;

      }
      
      if (SumOf_IOK==0) KountNoModule += 1 ;

      double Logic0123 = v_IOK_Module[0] + v_IOK_Module[1] + v_IOK_Module[3] + v_IOK_Module[3] ;
      pTH1_Logic0123->Fill( double(Logic0123) , TheWeight ) ;
      
      if (Logic0123>=1) pTH1_Logic0123_Inc->Fill( 1. , TheWeight ) ;
      if (Logic0123>=2) pTH1_Logic0123_Inc->Fill( 2. , TheWeight ) ;
      if (Logic0123>=3) pTH1_Logic0123_Inc->Fill( 3. , TheWeight ) ;
      if (Logic0123>=4) pTH1_Logic0123_Inc->Fill( 4. , TheWeight ) ;

      double Logic4567 = v_IOK_Module[4] + v_IOK_Module[5] + v_IOK_Module[6] + v_IOK_Module[7] ;
      pTH1_Logic4567->Fill( double(Logic4567) , TheWeight ) ;

      if (Logic4567>=1) pTH1_Logic4567_Inc->Fill( 1. , TheWeight ) ;
      if (Logic4567>=2) pTH1_Logic4567_Inc->Fill( 2. , TheWeight ) ;
      if (Logic4567>=3) pTH1_Logic4567_Inc->Fill( 3. , TheWeight ) ;
      if (Logic4567>=4) pTH1_Logic4567_Inc->Fill( 4. , TheWeight ) ;

//    T Leading
//    Loop On Modules
      for (int iModule = 0; iModule < NberOfModule; iModule++) {
        Module* pModule = pEvent->Get_Module_InArray(iModule) ;
        
        int ModuleNber = pModule->Get_ModuleNber() ;
        
//      Loop On Clusters
        int NClusters = pModule->Get_NberOfCluster() ;
        V_pTH1F_ClusterMultiplicity[ModuleNber]->Fill( NClusters ) ;
        for (int iCluster = 0; iCluster < NClusters; iCluster++) {
          Cluster* pCluster = pModule->Get_Cluster(iCluster) ;
        
          double TLeading = pCluster->Get_TMaxLeading() ;
          V_pTH1F_TLeading_1[ModuleNber]->Fill( TLeading ) ;
          V_pTH1F_TLeading_2[ModuleNber]->Fill( TLeading ) ;
    
          double ALeading = pCluster->Get_AMaxLeading() ;
          V_pTH1F_ALeading [ModuleNber]->Fill( ALeading ) ;

//        Loop On Pads
          int NberOfPads = pCluster->Get_NberOfPads() ;
          V_pTH1F_PadMultiplicity[ModuleNber]->Fill( NberOfPads ) ;
          const Pad* TPadLeading = pCluster->Get_LeadingPad();
          for (int iPad = 0; iPad < NberOfPads; iPad++) {
            const Pad* pPad = pCluster->Get_Pad(iPad) ;
            
            if (pPad!=TPadLeading){
              double DeltaT = pPad->Get_TMax() -TLeading ;
              V_pTH1F_TPadMTLeading[ModuleNber]->Fill( DeltaT ) ;
            }
            
          }
        }
        
      }
      
//
      delete pEvent ;

    }
    
    std::cout << std::endl ;
    std::cout << " Nber Of Entries :" << NberOfEvent << std::endl ;
    std::cout << " Nber Of Enrties with no module :" << KountNoModule << std::endl ;
    for (int iMod = 0; iMod < 8; iMod++) {
      std::cout << " Nber Of Enrties with module " << iMod << ": " << v_Kounter_Module[iMod] << std::endl ;
    }
    std::cout << std::endl ;

//
    std::string OutputFile     = OUTDirName + Tag + ".pdf" ;
    std::string OutputFile_Beg = OutputFile + "(" ;
    std::string OutputFile_End = OutputFile + ")" ;

//  Page 1
    TCanvas* pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->SaveAs(OutputFile_Beg.c_str());
    
//  Page 2
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;

      pTH1_Module->Draw("HIST");
      pTH1_Module->SetStats(0) ;
      pTH1_Module->SetMinimum(0.) ;
      pTH1_Module->SetMaximum(100.) ;
      pTH1_Module->GetXaxis()->SetTitle("Module Number") ;
      pTCanvas->Update();
    
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 3
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(2,2); 

      pTCanvas->cd(1);
        pTH1_Logic0123->Draw("HIST");
        pTH1_Logic0123->SetStats(0) ;
        pTH1_Logic0123->SetMinimum(0.) ;
        pTH1_Logic0123->SetMaximum(100.) ;
        pTH1_Logic0123->GetXaxis()->SetTitle("Modules count") ;
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTH1_Logic4567->Draw("HIST");
        pTH1_Logic4567->SetStats(0) ;
        pTH1_Logic4567->SetMinimum(0.) ;
        pTH1_Logic4567->SetMaximum(100.) ;
        pTH1_Logic4567->GetXaxis()->SetTitle("Modules count") ;
        pTCanvas->Update();

      pTCanvas->cd(3);
        pTH1_Logic0123_Inc->Draw("HIST");
        pTH1_Logic0123_Inc->SetStats(0) ;
        pTH1_Logic0123_Inc->SetMinimum(0.) ;
        pTH1_Logic0123_Inc->SetMaximum(100.) ;
        pTH1_Logic0123_Inc->GetXaxis()->SetTitle("Modules count") ;
        pTCanvas->Update();

      pTCanvas->cd(4);
        pTH1_Logic4567_Inc->Draw("HIST");
        pTH1_Logic4567_Inc->SetStats(0) ;
        pTH1_Logic4567_Inc->SetMinimum(0.) ;
        pTH1_Logic4567_Inc->SetMaximum(100.) ;
        pTH1_Logic4567_Inc->GetXaxis()->SetTitle("Modules count") ;
        pTCanvas->Update();

    pTCanvas->SaveAs(OutputFile.c_str());

// Page 4
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_ClusterMultiplicity[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
     
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 5
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_TLeading_1[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
       
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 6
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_TLeading_2[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
       
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 7
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_ALeading[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
       
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 8
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_PadMultiplicity[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
       
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 9
    pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
      pTCanvas->Divide(4,2); 

      for (int iMod = 0; iMod < 8; iMod++) {
        pTCanvas->cd(iMod+1);
        pTCanvas->cd(iMod+1)->SetLogy(1);
          V_pTH1F_TPadMTLeading[iMod]->Draw("HIST");
         
          pTCanvas->Update();
      }
       
    pTCanvas->SaveAs(OutputFile.c_str());

//  Page 10
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->SaveAs(OutputFile_End.c_str());

//
    std::cout.rdbuf(coutbuf); // Reset to standard output again

//
    delete pUploader ;
    delete pTH1_Module ; 
    delete pTH1_Logic0123 ; 
    delete pTH1_Logic0123_Inc ; 
    delete pTH1_Logic4567 ; 
    delete pTH1_Logic4567_Inc ; 
    for (int iMod = 0; iMod < 8; iMod++) {
      delete V_pTH1F_ClusterMultiplicity[iMod] ; V_pTH1F_ClusterMultiplicity[iMod] = 0 ;
      delete V_pTH1F_PadMultiplicity[iMod]     ; V_pTH1F_PadMultiplicity[iMod]     = 0 ;
      delete V_pTH1F_TLeading_1[iMod]          ; V_pTH1F_TLeading_1[iMod]          = 0 ;
      delete V_pTH1F_TLeading_2[iMod]          ; V_pTH1F_TLeading_2[iMod]          = 0 ;
      delete V_pTH1F_TPadMTLeading[iMod]       ; V_pTH1F_TPadMTLeading[iMod]       = 0 ;
    }
    V_pTH1F_ClusterMultiplicity.clear() ;
    V_pTH1F_PadMultiplicity    .clear() ;
    V_pTH1F_TLeading_1         .clear() ;
    V_pTH1F_TLeading_2         .clear() ;
    V_pTH1F_TPadMTLeading      .clear() ;
    
    
  }

}
