#include "Procedures/EVT_01_BASE.h"
#include "Misc/Util.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "EvtModelTools/JFL_Selector.h"

#include "EvtModelTools/EvtModelTools_Histos.h"

void EVT_01_BASE(
                        const int&         iOptQuiet          ,
                        std::string const& Tag ,
                        const std::string& OutDir ,
                        std::string const& EventFile ,
                        std::string const& SelectionSet ,
                        int         const& intUploader ,
                        int         const& ModuleNber  ,
                        int         const& Data_to_Use ,
                        double      const& Cut_Stage120_TLow ,
                        double      const& Cut_Stage120_THigh 

){ 
//Set output dir name
  std::string AnalyseName = Tag ;
  std::string base_EventFile = EventFile.substr(EventFile.find_last_of("//") + 1) ;

  std::ostringstream aostringstream_OUTDirName ;
  aostringstream_OUTDirName << std::setiosflags(std::ios::fixed) ;
  aostringstream_OUTDirName << OutDir  ;
  aostringstream_OUTDirName << "/Dir_"  ;
  aostringstream_OUTDirName << base_EventFile   ;
  aostringstream_OUTDirName << "_"   ;
  aostringstream_OUTDirName << AnalyseName   ;
  aostringstream_OUTDirName << "_Module_Nber_"   ;
  aostringstream_OUTDirName << ModuleNber   ;
  aostringstream_OUTDirName << "/"   ;

  std::string OUTDirName = aostringstream_OUTDirName.str() ;  ;

//
    std::cout 
      << " " << "Main_AOBt_EVT_01_BASE"                  
      << " " << Tag                  
      << " " << OutDir              
      << " ${EVENTDIR}/" << EventFile.substr(EventFile.find_last_of("//") + 1)     
      << " " << SelectionSet 
      << " " << intUploader                
      << " " << ModuleNber 
      << " " << Data_to_Use 
      << " " << Cut_Stage120_TLow 
      << " " << Cut_Stage120_THigh
      << std::endl ;

  if (iOptQuiet==1) return ;

//
  MyMakeDir(OUTDirName) ;
  
//Redirect Output
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
  std::cout << " EVT_01_BASE " << std::endl ;
  std::cout << "      Tag                : " << Tag                << std::endl ;
  std::cout << "      EventFile          : " << EventFile          << std::endl ;
  std::cout << "      SelectionSet       : " << SelectionSet       << std::endl ;
  std::cout << "      intUploader        : " << intUploader        << std::endl ;
  std::cout << "      ModuleNber         : " << ModuleNber         << std::endl ;
  std::cout << "      Data_to_Use        : " << Data_to_Use        << std::endl ;
  std::cout << "      Cut_Stage120_TLow  : " << Cut_Stage120_TLow  << std::endl ;
  std::cout << "      Cut_Stage120_THigh : " << Cut_Stage120_THigh << std::endl ;
  std::cout << std::endl ;
  std::cout << "      OUTDirName   : " << OUTDirName  << std::endl ;
  std::cout << std::endl ;


//Events Uploader  
  Uploader* pUploader = GiveMe_Uploader(intUploader,EventFile) ;


//Selector
  JFL_Selector aJFL_Selector(SelectionSet) ;
  
//Modify Selection
  aJFL_Selector.Set_Cut_Stage120_TLow ( Cut_Stage120_TLow  ) ;
  aJFL_Selector.Set_Cut_Stage120_THigh( Cut_Stage120_THigh ) ;
  
  std::cout << std::endl ;
  std::cout << "-----------------------------------Selection effectively applied" << std::endl ;
  aJFL_Selector.Tell_Selection() ;
  
  
//Module Multiplicity Histos
  std::ostringstream aostringstreamNAMEBEFORE ;
  aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
  aostringstreamNAMEBEFORE << "ModuleMultiplicityBEFORE"   ;
  std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
 
  std::ostringstream aostringstreamNAMEAFTER ;
  aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
  aostringstreamNAMEAFTER << "ModuleMultiplicityAFTER"   ;
  std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
 
  std::ostringstream aostringstreamTITLEBEFORE ;
  aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
  aostringstreamTITLEBEFORE << "ModuleMultiplicity Before: Mod " ;
  std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

  std::ostringstream aostringstreamTITLEAFTER ;
  aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
  aostringstreamTITLEAFTER << "ModuleMultiplicity After: Mod " ;
  std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

  TH1F* pTH1_ModuleMultiplicity_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),10,  -0.5 , 9.5 );
  TH1F* pTH1_ModuleMultiplicity_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),10,  -0.5 , 9.5 );
 
//Cluster Multiplicity Histos
  std::vector < TH1F* > V_pTH1F_ClusterMultiplicity_Before ;
  std::vector < TH1F* > V_pTH1F_ClusterMultiplicity_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "ClusterMultiplicityBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "ClusterMultiplicityAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "ClusterMultiplicity Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "ClusterMultiplicity After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),37,  -0.5 , 36.5 );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),37,  -0.5 , 36.5 );
  
    V_pTH1F_ClusterMultiplicity_Before.push_back(pTH1_Before);
    V_pTH1F_ClusterMultiplicity_After .push_back(pTH1_After );
  
  }
  
//Pad Clusters Multiplicity Histos
  std::vector < TH1F* > V_pTH1F_PadMultiplicity_Before ;
  std::vector < TH1F* > V_pTH1F_PadMultiplicity_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "PadMultiplicityBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "PadMultiplicityAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "PadMultiplicity Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "PadMultiplicity After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),11,  -0.5 , 10.5 );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),11,  -0.5 , 10.5 );
  
    V_pTH1F_PadMultiplicity_Before.push_back(pTH1_Before);
    V_pTH1F_PadMultiplicity_After .push_back(pTH1_After );
  
  }
  
//T Leading Histos
  std::vector < TH1F* > V_pTH1F_TLeading_Before ;
  std::vector < TH1F* > V_pTH1F_TLeading_After  ;
  std::vector < TH1F* > V_pTH1F_TLeading_Before_Cut ;
  std::vector < TH1F* > V_pTH1F_TLeading_After_Cut ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "TLeadingBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "TLeadingAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "TLeading Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "TLeading After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before     = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),511,  -0.5 , 510.5 );
    TH1F* pTH1_After      = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),511,  -0.5 , 510.5 );

    V_pTH1F_TLeading_Before.push_back(pTH1_Before);
    V_pTH1F_TLeading_After .push_back(pTH1_After );
  
    std::ostringstream aostringstreamNAMEBEFORECut ;
    aostringstreamNAMEBEFORECut << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORECut << "TLeadingBEFORECut"  << iMod;
    std::string NameBEFORECut =  aostringstreamNAMEBEFORECut.str() ;
    
    TH1F* pTH1_Before_Cut = new TH1F(NameBEFORECut.c_str(),TitleBEFORE.c_str(),511,  -0.5 , 510.5 );

    V_pTH1F_TLeading_Before_Cut .push_back(pTH1_Before_Cut );
    
    std::ostringstream aostringstreamNAMEAFTERCut ;
    aostringstreamNAMEAFTERCut << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTERCut << "TLeadingAFTERCut"  << iMod;
    std::string NameAFTERCut =  aostringstreamNAMEAFTERCut.str() ;
    
    TH1F* pTH1_After_Cut = new TH1F(NameAFTERCut.c_str(),TitleAFTER.c_str(),511,  -0.5 , 510.5 );

    V_pTH1F_TLeading_After_Cut .push_back(pTH1_After_Cut );
    
  }
  
//A Leading Histos
  std::vector < TH1F* > V_pTH1F_ALeading_Before ;
  std::vector < TH1F* > V_pTH1F_ALeading_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "ALeadingBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "ALeadingAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "ALeading Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "ALeading After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),4097,  -0.5 , 4096.5 );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),4097,  -0.5 , 4096.5 );
  
    V_pTH1F_ALeading_Before.push_back(pTH1_Before);
    V_pTH1F_ALeading_After .push_back(pTH1_After );
  
  }

//TPad-T Leading Histos
  std::vector < TH1F* > V_pTH1F_TPadMTLeading_Before ;
  std::vector < TH1F* > V_pTH1F_TPadMTLeading_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "TPadMTLeadingBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "TPadMTLeadingAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "TPad-TLeading Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "TPad-TLeading After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),151,  -149.5 , 300.5 );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),151,  -149.5 , 300.5 );
  
    V_pTH1F_TPadMTLeading_Before.push_back(pTH1_Before);
    V_pTH1F_TPadMTLeading_After .push_back(pTH1_After );
  
  }
  
//
  std::vector < TH2F* > V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before ;
  std::vector < TH2F* > V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "pTH2F_Alpha2_VS_Alpha3onAlpha2BEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "pTH2F_Alpha2_VS_Alpha3onAlpha2AFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "#alpha_{2} = A2/A1 VS #alpha_{3}/#alpha_{2} =A3/A2 Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "#alpha_{2} = A2/A1 VS #alpha_{3}/#alpha_{2} =A3/A2 After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;
  
    TH2F* pTH2F_Before = new TH2F(NameBEFORE.c_str(),TitleBEFORE.c_str() ,100,0.,1., 100,0.,1.);
    TH2F* pTH2F_After  = new TH2F(NameAFTER .c_str(),TitleAFTER .c_str() ,100,0.,1., 100,0.,1.);

    V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before.push_back(pTH2F_Before);
    V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After .push_back(pTH2F_After );

  }
  
//
  std::vector < TH2F* > V_pTH2F_Tau2_VS_Tau3mTau2_Before ;
  std::vector < TH2F* > V_pTH2F_Tau2_VS_Tau3mTau2_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "pTH2F_Tau2_VS_Tau3mTau2BEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "pTH2F_Tau2_VS_Tau3mTau2AFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2 Before: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "#tau_{2}=T2-T1 VS #tau_{3}-#tau_{2}=T3-T2 After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;
  
    TH2F* pTH2F_Before = new TH2F(NameBEFORE.c_str(),TitleBEFORE.c_str() ,201,-50.5,150.5, 71,-20.5,50.5);
    TH2F* pTH2F_After  = new TH2F(NameAFTER .c_str(),TitleAFTER .c_str() ,201,-50.5,150.5, 71,-20.5,50.5);

    V_pTH2F_Tau2_VS_Tau3mTau2_Before.push_back(pTH2F_Before);
    V_pTH2F_Tau2_VS_Tau3mTau2_After .push_back(pTH2F_After );

  }

//TPad-TMC  
  std::vector < TH1F* > V_pTH1F_TPadTMC_Before ;
  std::vector < TH1F* > V_pTH1F_TPadTMC_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "TPadTMCBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "TPadTMCAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "TPad-TMC: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "TPad-TMC After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),200,  -50 , 50.  );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),200,  -50 , 50.  );
  
    V_pTH1F_TPadTMC_Before.push_back(pTH1_Before);
    V_pTH1F_TPadTMC_After .push_back(pTH1_After );
  
  }
  
//APad-AMC  
  std::vector < TH1F* > V_pTH1F_APadAMC_Before ;
  std::vector < TH1F* > V_pTH1F_APadAMC_After  ;
  for (int iMod = 0; iMod < 8; iMod++) {
    std::ostringstream aostringstreamNAMEBEFORE ;
    aostringstreamNAMEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEBEFORE << "APadAMCBEFORE"  << iMod;
    std::string NameBEFORE =  aostringstreamNAMEBEFORE.str() ;
    
    std::ostringstream aostringstreamNAMEAFTER ;
    aostringstreamNAMEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamNAMEAFTER << "APadAMCAFTER"  << iMod;
    std::string NameAFTER =  aostringstreamNAMEAFTER.str() ;
    
    std::ostringstream aostringstreamTITLEBEFORE ;
    aostringstreamTITLEBEFORE << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEBEFORE << "APad-AMC: Mod "  << iMod;
    std::string TitleBEFORE =  aostringstreamTITLEBEFORE.str() ;

    std::ostringstream aostringstreamTITLEAFTER ;
    aostringstreamTITLEAFTER << std::setiosflags(std::ios::fixed) ;
    aostringstreamTITLEAFTER << "APad-AMC After: Mod "  << iMod;
    std::string TitleAFTER =  aostringstreamTITLEAFTER.str() ;

    TH1F* pTH1_Before = new TH1F(NameBEFORE.c_str(),TitleBEFORE.c_str(),200,  -50 , 50.  );
    TH1F* pTH1_After  = new TH1F(NameAFTER .c_str(),TitleAFTER .c_str(),200,  -50 , 50.  );
  
    V_pTH1F_APadAMC_Before.push_back(pTH1_Before);
    V_pTH1F_APadAMC_After .push_back(pTH1_After );
  
  }
  
//
  int Do_Display          =   2 ;
  int Kounter_Display     =   0 ;
  int Kounter_Display_Max = 500 ;
  std::string OUTDIR_Display_Cluster = OUTDirName + "/Display_Cluster/" ;
  MyMakeDir(OUTDIR_Display_Cluster) ;
  
//Loop On Events
  aJFL_Selector.Reset_StatCounters();
  
  int NberOfEvent = pUploader->Get_NberOfEvent() ;
  NberOfEvent = 100 ;
  for (int iEvent = 0; iEvent < NberOfEvent; iEvent++) {
    
    Event*  pEvent = pUploader->GiveMe_Event( iEvent, ModuleNber, Data_to_Use );

    if (!pEvent) continue ;

//  Before Selection
    if ( Do_Display == 1 || Do_Display ==-1 ){
      if ( Kounter_Display <= Kounter_Display_Max ){
        if (pEvent->IsValid() == 1) { // Valid events only
//        Loop On Modules
          int NberOfModule = pEvent->Get_NberOfModule() ;
          for (int iModule = 0; iModule < NberOfModule; iModule++) {
            Module* pModule = pEvent->Get_Module_InArray(iModule) ;

            int ModuleNber = pModule->Get_ModuleNber() ;
            if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; // Valid modules only

//          Loop On Clusters
            int NClusters = pModule->Get_NberOfCluster() ;
            for (int iCluster = 0; iCluster < NClusters; iCluster++) {
              Cluster* pCluster = pModule->Get_Cluster(iCluster) ;
              int DoIt = 1 ;
              if ( Do_Display < 0 ){
//              Loop On Pads
                DoIt = 0 ;
                int NberOfPads = pCluster->Get_NberOfPads() ;
                for (int iPad = 0; iPad < NberOfPads; iPad++) {
                  const Pad* pPad = pCluster->Get_Pad(iPad) ;
                  if (pPad->Get_FIT_Status() != 0 ) DoIt = 1 ;
                }
              }
              if (DoIt==0) continue ;
              Kounter_Display += 1 ;
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_Display_Cluster,"DumpClusterDisplay", 2, 412, 40);
            }
          }
        }
      }
    }
    
//  Before Selection
    if (pEvent->IsValid() == 1) { // Valid events only
//  Loop On Modules
      int NberOfModule = pEvent->Get_NberOfModule() ;
      pTH1_ModuleMultiplicity_Before->Fill( NberOfModule );
      for (int iModule = 0; iModule < NberOfModule; iModule++) {
        Module* pModule = pEvent->Get_Module_InArray(iModule) ;

        int ModuleNber = pModule->Get_ModuleNber() ;
        if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; // Valid modules only

//      Loop On Clusters
        int NClusters = pModule->Get_NberOfCluster() ;
        V_pTH1F_ClusterMultiplicity_Before[ModuleNber]->Fill( NClusters ) ;
        for (int iCluster = 0; iCluster < NClusters; iCluster++) {
          Cluster* pCluster = pModule->Get_Cluster(iCluster) ;

          const Pad* pPad_Leading = pCluster->Get_LeadingPad();
          int iY_Leading = pPad_Leading->Get_iY();
          
          double TLeading = pCluster->Get_TMaxLeading() ;
          V_pTH1F_TLeading_Before[ModuleNber]->Fill( TLeading ) ;

          if (TLeading>5.) V_pTH1F_TLeading_Before_Cut[ModuleNber]->Fill( TLeading ) ;

          double ALeading = pCluster->Get_AMaxLeading() ;
          V_pTH1F_ALeading_Before[ModuleNber]->Fill( ALeading ) ;

//        Loop On Pads
          int NberOfPads = pCluster->Get_NberOfPads() ;
          V_pTH1F_PadMultiplicity_Before[ModuleNber]->Fill( NberOfPads ) ;
          for (int iPad = 0; iPad < NberOfPads; iPad++) {
            const Pad* pPad = pCluster->Get_Pad(iPad) ;

            if (pPad!=pPad_Leading){
              double DeltaT = pPad->Get_TMax() -TLeading ;
              V_pTH1F_TPadMTLeading_Before[ModuleNber]->Fill( DeltaT ) ;

            }

             double DeltaTPadTMC = pPad->Get_TMax() - pPad->Get_TMax_True() ;
             V_pTH1F_TPadTMC_Before[ModuleNber]->Fill( DeltaTPadTMC ) ;

             double DeltaAPadAMC = pPad->Get_AMax() - pPad->Get_AMax_True() ;
             V_pTH1F_APadAMC_Before[ModuleNber]->Fill( DeltaAPadAMC ) ;

          }

//          
          double A1 = pPad_Leading->Get_AMax() ;
          double T1 = pPad_Leading->Get_TMax() ;

          if (NberOfPads>=3){
            double AU = 0. ; int AU_Found = 0 ;
            double AD = 0. ; int AD_Found = 0 ;
            double TU = 0. ;  
            double TD = 0. ; 
            for (int iPad = 0 ; iPad< NberOfPads; iPad++){
              const Pad* pPad = pCluster->Get_Pad(iPad);
             
              int iY = pPad->Get_iY();
              if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
              if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
              if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
              if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
              if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
              if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
              
            }
            
            if (AU_Found==1 && AD_Found==1 ){
              double T2 = TU ;
              double T3 = TD ;
              if (AU < AD ){
                T2 = TD ;
                T3 = TU ;
              }
              double A2 = std::max(AU, AD) ;
              double A3 = std::min(AU, AD) ;
              
              
              V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before[ModuleNber]->Fill( A3/A2 , A2 ) ;
              V_pTH2F_Tau2_VS_Tau3mTau2_Before       [ModuleNber]->Fill( T3-T2 , T2  ) ;
             
            }
          }

        }

      }

    }
        
        
//  Apply Selections
    aJFL_Selector.ApplySelection(pEvent ) ;
    
    
//  After Selection
    if ( Do_Display == 2 || Do_Display ==-2 ){
      if ( Kounter_Display <= Kounter_Display_Max ){
        if (pEvent->IsValid() == 1) { // Valid events only
//        Loop On Modules
          int NberOfModule = pEvent->Get_NberOfModule() ;
          for (int iModule = 0; iModule < NberOfModule; iModule++) {
            Module* pModule = pEvent->Get_Module_InArray(iModule) ;

            int ModuleNber = pModule->Get_ModuleNber() ;
            if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; // Valid modules only

//          Loop On Clusters
            int NClusters = pModule->Get_NberOfCluster() ;
            for (int iCluster = 0; iCluster < NClusters; iCluster++) {
              Cluster* pCluster = pModule->Get_Cluster(iCluster) ;
              int DoIt = 1 ;
              if ( Do_Display < 0 ){
//              Loop On Pads
                DoIt = 0 ;
                int NberOfPads = pCluster->Get_NberOfPads() ;
                for (int iPad = 0; iPad < NberOfPads; iPad++) {
                  const Pad* pPad = pCluster->Get_Pad(iPad) ;
                  if (pPad->Get_FIT_Status() != 0 ) DoIt = 1 ;
                }
              }
              if (DoIt==0) continue ;
              Kounter_Display += 1 ;
              DrawOut_ClusterWFDisplay  (pCluster,OUTDIR_Display_Cluster,"DumpClusterDisplay", 2, 412, 40);
            }
          }
        }
      }
    }
    
//  After Selection
    if (pEvent->IsValid() == 1) { // Valid events only
//  Loop On Modules
      int NberOfModule = pEvent->Get_NberOfModule() ;
      pTH1_ModuleMultiplicity_After->Fill( NberOfModule );
      for (int iModule = 0; iModule < NberOfModule; iModule++) {
        Module* pModule = pEvent->Get_Module_InArray(iModule) ;

        int ModuleNber = pModule->Get_ModuleNber() ;
        if (pEvent->Validity_ForThisModule(ModuleNber) == 0) continue ; // Valid modules only

//      Loop On Clusters
        int NClusters = pModule->Get_NberOfCluster() ;
        V_pTH1F_ClusterMultiplicity_After[ModuleNber]->Fill( NClusters ) ;
        for (int iCluster = 0; iCluster < NClusters; iCluster++) {
          Cluster* pCluster = pModule->Get_Cluster(iCluster) ;

          const Pad* pPad_Leading = pCluster->Get_LeadingPad();
          int iY_Leading = pPad_Leading->Get_iY();
          
          double TLeading = pCluster->Get_TMaxLeading() ;
          V_pTH1F_TLeading_After[ModuleNber]->Fill( TLeading ) ;

          if (TLeading>5.) V_pTH1F_TLeading_After_Cut[ModuleNber]->Fill( TLeading ) ;

          double ALeading = pCluster->Get_AMaxLeading() ;
          V_pTH1F_ALeading_After[ModuleNber]->Fill( ALeading ) ;

//        Loop On Pads
          int NberOfPads = pCluster->Get_NberOfPads() ;
          V_pTH1F_PadMultiplicity_After[ModuleNber]->Fill( NberOfPads ) ;
          for (int iPad = 0; iPad < NberOfPads; iPad++) {
            const Pad* pPad = pCluster->Get_Pad(iPad) ;

            if (pPad!=pPad_Leading){
              double DeltaT = pPad->Get_TMax() -TLeading ;
              V_pTH1F_TPadMTLeading_After[ModuleNber]->Fill( DeltaT ) ;

            }

             double DeltaTPadTMC = pPad->Get_TMax() - pPad->Get_TMax_True() ;
             V_pTH1F_TPadTMC_After[ModuleNber]->Fill( DeltaTPadTMC ) ;

             double DeltaAPadAMC = pPad->Get_AMax() - pPad->Get_AMax_True() ;
             V_pTH1F_APadAMC_After[ModuleNber]->Fill( DeltaAPadAMC ) ;

          }

//          
          double A1 = pPad_Leading->Get_AMax() ;
          double T1 = pPad_Leading->Get_TMax() ;

          if (NberOfPads>=3){
            double AU = 0. ; int AU_Found = 0 ;
            double AD = 0. ; int AD_Found = 0 ;
            double TU = 0. ;  
            double TD = 0. ; 
            for (int iPad = 0 ; iPad< NberOfPads; iPad++){
              const Pad* pPad = pCluster->Get_Pad(iPad);
             
              int iY = pPad->Get_iY();
              if ( iY == (iY_Leading+1) ) AU_Found = 1 ;
              if ( iY == (iY_Leading+1) ) AU       = pPad->Get_AMax()/A1;
              if ( iY == (iY_Leading+1) ) TU       = pPad->Get_TMax()-T1;
              if ( iY == (iY_Leading-1) ) AD_Found = 1 ;
              if ( iY == (iY_Leading-1) ) AD       = pPad->Get_AMax()/A1;
              if ( iY == (iY_Leading-1) ) TD       = pPad->Get_TMax()-T1;
              
            }
            
            if (AU_Found==1 && AD_Found==1 ){
              double T2 = TU ;
              double T3 = TD ;
              if (AU < AD ){
                T2 = TD ;
                T3 = TU ;
              }
              double A2 = std::max(AU, AD) ;
              double A3 = std::min(AU, AD) ;
              
              V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After[ModuleNber]->Fill( A3/A2 , A2 ) ;
              V_pTH2F_Tau2_VS_Tau3mTau2_After       [ModuleNber]->Fill( T3-T2 , T2  ) ;

            }
            
          }

        }

      }

    }

//
    delete pEvent ;

  }
  
  std::cout << std::endl ;
  std::cout << " Nber Of Entries :" << NberOfEvent << std::endl ;

//
  std::cout << std::endl ;
  std::cout << " Before MPTL " ;
  for (int iMod = 0; iMod < 8; iMod++) {
    int TLeadingMOPOverRun = V_pTH1F_TLeading_Before_Cut[iMod]->GetMaximumBin()-1 ;
    std::cout << "; " << TLeadingMOPOverRun ;
  }
  std::cout << std::endl ;
  std::cout << std::endl ;

//
  std::cout << std::endl ;
  std::cout << " After MPTL " ;
  for (int iMod = 0; iMod < 8; iMod++) {
    int TLeadingMOPOverRun = V_pTH1F_TLeading_After_Cut[iMod]->GetMaximumBin()-1 ;
    std::cout << "; " << TLeadingMOPOverRun ;
  }
  std::cout << std::endl ;
  std::cout << std::endl ;

//Print Out Selection Stat
  aJFL_Selector.PrintStat();
  
//
  std::string OutputFile     = OUTDirName + Tag + ".pdf" ;
  std::string OutputFile_Beg = OutputFile + "(" ;
  std::string OutputFile_End = OutputFile + ")" ;

//Page 1
  TCanvas* pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(OutputFile_Beg.c_str());
  
//Page 2
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,1); 

      pTCanvas->cd(1);
        pTH1_ModuleMultiplicity_Before->Draw("HIST");
       
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTH1_ModuleMultiplicity_After->Draw("HIST");
       
        pTCanvas->Update();
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 3
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_ClusterMultiplicity_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 4
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_ClusterMultiplicity_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 5
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TLeading_Before[iMod]->Draw("HIST");
        
        
        TGraph* pTGraph_1 = new TGraph ;
        pTGraph_1->SetPoint( pTGraph_1->GetN(), Cut_Stage120_TLow  , 1.E4*V_pTH1F_TLeading_Before_Cut[iMod]->GetMaximum() ) ;
        pTGraph_1->SetPoint( pTGraph_1->GetN(), Cut_Stage120_TLow  , 0.1  ) ;
        pTGraph_1->SetLineColor(2) ;
  
        TGraph* pTGraph_2 = new TGraph ;
        pTGraph_2->SetPoint( pTGraph_2->GetN(), Cut_Stage120_THigh , 1.E4*V_pTH1F_TLeading_Before_Cut[iMod]->GetMaximum() ) ;
        pTGraph_2->SetPoint( pTGraph_2->GetN(), Cut_Stage120_THigh , 0.1  ) ;
        pTGraph_2->SetLineColor(2) ;

        pTGraph_1->SetLineColor( 2 ) ;
        pTGraph_2->SetLineColor( 2 ) ;
        pTGraph_1->Draw("l");  
        pTGraph_2->Draw("l");  
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 6
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TLeading_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 7
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_ALeading_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 8
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_ALeading_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 9
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_PadMultiplicity_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 10
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_PadMultiplicity_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 11
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TPadMTLeading_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 12
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TPadMTLeading_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 13
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
        V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before[iMod]->Draw("colz");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 14
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
        V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After[iMod]->Draw("colz");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 15
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
        V_pTH2F_Tau2_VS_Tau3mTau2_Before[iMod]->Draw("colz");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 16
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
        V_pTH2F_Tau2_VS_Tau3mTau2_After[iMod]->Draw("colz");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 17
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TPadTMC_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 18
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_TPadTMC_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());


//Page 19
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_APadAMC_Before[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 20
  pTCanvas = new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(4,2); 

    for (int iMod = 0; iMod < 8; iMod++) {
      pTCanvas->cd(iMod+1);
      pTCanvas->cd(iMod+1)->SetLogy(1);
        V_pTH1F_APadAMC_After[iMod]->Draw("HIST");
       
        pTCanvas->Update();
    }
     
  pTCanvas->SaveAs(OutputFile.c_str());

//Page 21
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->SaveAs(OutputFile_End.c_str());

//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

//
  delete pUploader ;
  delete pTH1_ModuleMultiplicity_Before ;
  delete pTH1_ModuleMultiplicity_After ;
  for (int iMod = 0; iMod < 8; iMod++) {
    delete V_pTH1F_ClusterMultiplicity_Before[iMod]      ; V_pTH1F_ClusterMultiplicity_Before[iMod]      = 0 ;
    delete V_pTH1F_ClusterMultiplicity_After [iMod]      ; V_pTH1F_ClusterMultiplicity_After [iMod]      = 0 ;
    delete V_pTH1F_PadMultiplicity_Before[iMod]          ; V_pTH1F_PadMultiplicity_Before[iMod]          = 0 ;
    delete V_pTH1F_PadMultiplicity_After [iMod]          ; V_pTH1F_PadMultiplicity_After [iMod]          = 0 ;
    delete V_pTH1F_TLeading_Before[iMod]                 ; V_pTH1F_TLeading_Before[iMod]                 = 0 ;
    delete V_pTH1F_TLeading_After [iMod]                 ; V_pTH1F_TLeading_After [iMod]                 = 0 ;
    delete V_pTH1F_TLeading_Before_Cut[iMod]             ; V_pTH1F_TLeading_Before_Cut[iMod]             = 0 ;
    delete V_pTH1F_ALeading_Before[iMod]                 ; V_pTH1F_ALeading_Before[iMod]                 = 0 ;
    delete V_pTH1F_ALeading_After [iMod]                 ; V_pTH1F_ALeading_After [iMod]                 = 0 ;
    delete V_pTH1F_TPadMTLeading_Before[iMod]            ; V_pTH1F_TPadMTLeading_Before[iMod]            = 0 ;
    delete V_pTH1F_TPadMTLeading_After [iMod]            ; V_pTH1F_TPadMTLeading_After [iMod]            = 0 ;
    delete V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before[iMod] ; V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before[iMod] = 0 ;
    delete V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After [iMod] ; V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After [iMod] = 0 ;
    delete V_pTH2F_Tau2_VS_Tau3mTau2_Before[iMod]        ; V_pTH2F_Tau2_VS_Tau3mTau2_Before[iMod]        = 0 ;
    delete V_pTH2F_Tau2_VS_Tau3mTau2_After [iMod]        ; V_pTH2F_Tau2_VS_Tau3mTau2_After [iMod]        = 0 ;
 
    delete V_pTH1F_TPadTMC_Before[iMod]            ; V_pTH1F_TPadTMC_Before[iMod]            = 0 ;
    delete V_pTH1F_TPadTMC_After [iMod]            ; V_pTH1F_TPadTMC_After [iMod]            = 0 ;
 
    delete V_pTH1F_APadAMC_Before[iMod]            ; V_pTH1F_APadAMC_Before[iMod]            = 0 ;
    delete V_pTH1F_APadAMC_After [iMod]            ; V_pTH1F_APadAMC_After [iMod]            = 0 ;
  }
  V_pTH1F_ClusterMultiplicity_Before      .clear() ;
  V_pTH1F_ClusterMultiplicity_After       .clear() ;
  V_pTH1F_PadMultiplicity_Before          .clear() ;
  V_pTH1F_PadMultiplicity_After           .clear() ;
  V_pTH1F_TLeading_Before                 .clear() ;
  V_pTH1F_TLeading_After                  .clear() ;
  V_pTH1F_TLeading_Before_Cut             .clear() ;
  V_pTH1F_ALeading_Before                 .clear() ;
  V_pTH1F_ALeading_After                  .clear() ;
  V_pTH1F_TPadMTLeading_Before            .clear() ;
  V_pTH1F_TPadMTLeading_After             .clear() ;
  V_pTH2F_Alpha2_VS_Alpha3onAlpha2_Before .clear() ;
  V_pTH2F_Alpha2_VS_Alpha3onAlpha2_After  .clear() ;
  V_pTH2F_Tau2_VS_Tau3mTau2_Before        .clear() ;
  V_pTH2F_Tau2_VS_Tau3mTau2_After         .clear() ;

  V_pTH1F_TPadTMC_Before  .clear() ;
  V_pTH1F_TPadTMC_After   .clear() ;

  V_pTH1F_APadAMC_Before  .clear() ;
  V_pTH1F_APadAMC_After   .clear() ;

}
