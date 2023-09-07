#include "Procedures/PV1_BASE.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"
#include "EvtModel/Sample.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Selections.h"

#include "EvtModelTools/EvtModelTools_JFL_Histos.h"
#include "EvtModelTools/JFL_Selector.h"

#include "SampleTools/SampleUploader_Data.h"

#include "EvtModelTools/JFL_RecTools.h"
#include "EvtModelTools/JFL_Do_ClusterFit.h"
#include "EvtModelTools/JFL_Do_SampleFit.h"

#include "AnalysisTools/AnalyseReconstruction.h"
#include "AnalysisTools/DoTracksReconstruction.h"

#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/PRF_param.h"

#include "SignalShape/SC_VS_x_FIX_rc_dd.h"
#include "SignalShape/SC_VS_x_FIX_rc_dd_4TF1_RhoPRF.h"

#include "SignalShape/SC_VS_x_rc_FIX_dd.h"
#include "SignalShape/SC_VS_x_rc_FIX_dd_4TF1_RhoPRF.h"

#include "EvtModelTools/JeanFrancois_DrawOut_Status.h"


#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TFile.h"

void PV1_BASE(
  const int&         iOptQuiet           ,
  const std::string& Flavor              ,
  const std::string& OutDir              ,
  const std::string& EventFile           ,
  const std::string& PRFfile_Fixed_RC_DD ,
  const std::string& PRFfile_Fixed_DD    ,
  const int&         Uploader            ,
  const int&         StartFrom           ,
  const int&         iOpt_Initial_YT_fit ,
  const int&         iOpt_TrackType      ,
  const int&         ModuleNber          ,
  const std::string& SelectionSet        ,
  const int&         Data_to_Use
){ 
//
   if (ModuleNber<0 || ModuleNber>7){
     std::cout << " PV1_BASE " 
               << " ModuleNber = " << ModuleNber
               << " out of range "
               << std::endl ;
     abort() ;
   }
//
  std::string AnalyseName = "PV1_" + Flavor ;
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
      << " " << "Main_AOBt_PV1_BASE"                  
      << " " << Flavor                
      << " " << OutDir              
      << " ${EVENTDIR}/" << EventFile.substr(EventFile.find_last_of("//") + 1)               
      << " " << PRFfile_Fixed_RC_DD             
      << " " << PRFfile_Fixed_DD 
      << " " << Uploader              
      << " " << StartFrom             
      << " " << iOpt_Initial_YT_fit 
      << " " << iOpt_TrackType 
      << " " << ModuleNber 
      << " " << SelectionSet 
      << " " << Data_to_Use 
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
      std::cout << " PV1_BASE:" << std::endl ;
      std::cout << "   Flavor              : " << Flavor              << std::endl ;
      std::cout << "   OutDir              : " << OutDir              << std::endl ;
      std::cout << "   EventFile           : " << EventFile           << std::endl ;
      std::cout << "   PRFfile_Fixed_RC_DD : " << PRFfile_Fixed_RC_DD << std::endl ;
      std::cout << "   PRFfile_Fixed_DD    : " << PRFfile_Fixed_DD    << std::endl ;
      std::cout << "   Uploader            : " << Uploader            << std::endl ;
      std::cout << "   StartFrom           : " << StartFrom           << std::endl ;
      std::cout << "   iOpt_Initial_YT_fit : " << iOpt_Initial_YT_fit << std::endl ;
      std::cout << "   iOpt_TrackType      : " << iOpt_TrackType      << std::endl ;
      std::cout << "   ModuleNber          : " << ModuleNber          << std::endl ;
      std::cout << "   SelectionSet        : " << SelectionSet        << std::endl ;
      std::cout << "   Data_to_Use         : " << Data_to_Use         << std::endl ;
      std::cout << std::endl ;
  
    std::cout 
      << " " << "Main_AOBt_PV1_BASE"                  
      << " " << Flavor                
      << " " << OutDir              
      << " ${EVENTDIR}/" << EventFile.substr(EventFile.find_last_of("//") + 1)               
      << " " << PRFfile_Fixed_RC_DD             
      << " " << PRFfile_Fixed_DD 
      << " " << Uploader              
      << " " << StartFrom             
      << " " << iOpt_Initial_YT_fit 
      << " " << iOpt_TrackType 
      << " " << ModuleNber 
      << " " << SelectionSet 
      << " " << Data_to_Use 
      << std::endl ;

//-----------------------------------------------------------//
//Settings
  int NberOfBinsForProfile = 100 ;
  int iTerationMax = 10 ;
  

//-----------------------------------------------------------//
//Load Sample
  std::cout << std::endl ;
  std::cout << "*Load Sample " << EventFile << std::endl ;
  
  Sample aSample ;
  SampleUploader_Data(Uploader,aSample, ModuleNber , EventFile , Data_to_Use);
  

//Settings -------------------------------------------------------Set Geometry stuff
  const  Model_ReadOutGeometry* pModel_ReadOutGeometry =  aSample.Get_Model_ReadOutGeometry() ;
  double LY       = pModel_ReadOutGeometry->Get_LY()       ;
  
  double vY_min_DiffPRF = -1.5 * LY *1.E2 ;
  double vY_max_DiffPRF =  1.5 * LY *1.E2 ;
  int Npoints_DiffPRF = 1000 ;

  int NberOfBins_TH2D_rho_VS_YTrackYPad = 100 ;


//Settings -------------------------------------------------------Set the PRF from file if it exits
  std::cout << std::endl ;
  std::cout << "*Set the PRF from TFile if it exits " << std::endl ;

  PRF_param  aPRF_param_rho_VS_YTrackYPad_FromTFile  ;

  TF1 * pTF1_FromTFile = 0;

  int    L_Status = 0 ;
  double L_Norm   = 0. ;
  double L_a2     = 0. ;
  double L_a4     = 0. ;
  double L_b2     = 0. ;
  double L_b4     = 0. ;
  aSample.GetFilePRF(L_Status,L_Norm,L_a2,L_a4,L_b2,L_b4) ;
  if (L_Status==1){
    std::cout << "   It does exist! " << std::endl ;
    pTF1_FromTFile = new TF1("pTF1_FromTFile",aPRF_param_rho_VS_YTrackYPad_FromTFile ,vY_min_DiffPRF , vY_max_DiffPRF,5);
    pTF1_FromTFile->SetParameter(0, L_Norm );
    pTF1_FromTFile->SetParameter(1, L_a2   );
    pTF1_FromTFile->SetParameter(2, L_a4   );
    pTF1_FromTFile->SetParameter(3, L_b2   );
    pTF1_FromTFile->SetParameter(4, L_b4   );
    std::cout << "   Set PRF from TFile with  "  <<  std::endl;
    std::cout << "     L_Norm  "  << std::setw(20) << std::setprecision(6) <<  L_Norm    << std::endl;
    std::cout << "     L_a2    "  << std::setw(20) << std::setprecision(6) <<  L_a2      << std::endl;
    std::cout << "     L_a4    "  << std::setw(20) << std::setprecision(6) <<  L_a4      << std::endl;
    std::cout << "     L_b2    "  << std::setw(20) << std::setprecision(6) <<  L_b2      << std::endl;
    std::cout << "     L_b4    "  << std::setw(20) << std::setprecision(6) <<  L_b4      << std::endl;
  }else{
    std::cout << " It does NOT exist! " << std::endl ;
  }


//Settings -------------------------------------------------------Set the PRF from LUT at fixed RC and DD
  std::cout << std::endl ;
  std::cout << "*Set the PRF from LUT at fixed RC and DD  from file " << std::endl ;
  std::cout << "   " << PRFfile_Fixed_RC_DD << std::endl ;
  
  SC_VS_x_FIX_rc_dd aSC_VS_x_FIX_rc_dd(PRFfile_Fixed_RC_DD) ;
  SC_VS_x_FIX_rc_dd_4TF1_RhoPRF aSC_VS_x_FIX_rc_dd_4TF1_RhoPRF (&aSC_VS_x_FIX_rc_dd) ;
  TF1* pTF1_PRFfile_Fixed_RC_DD = new TF1("pTF1_PRFfile_Fixed_RC_DD",aSC_VS_x_FIX_rc_dd_4TF1_RhoPRF,vY_min_DiffPRF , vY_max_DiffPRF,0);
  

//Settings -------------------------------------------------------Starting YT 
  std::cout << std::endl ;
  std::cout << "*Option-Set starting YT as " << std::endl ;
  if ( StartFrom == 0 ) std::cout << "   weighthed  " << std::endl ;
  if ( StartFrom == 1 ) std::cout << "   Leading pad" << std::endl ;


//Settings -------------------------------------------------------Do an initial fit on YT values 
  std::cout << std::endl ;
  std::cout << "*Option-Do an initial fit on YT values using " << std::endl ;
  if ( pTF1_FromTFile==0 && iOpt_Initial_YT_fit==2 ){
    std::cout << std::endl ;
    std::cout 
      << " pTF1_FromTFile = 0 "
      << " and  iOpt_Initial_YT_fit = " << iOpt_Initial_YT_fit
      << std::endl ;
    std::cout << std::endl ;
    abort() ;
  } 
  if ( iOpt_Initial_YT_fit == 0 ) std::cout << "   no initial fit                     " << std::endl ;
  if ( iOpt_Initial_YT_fit == 1 ) std::cout << "   the PRF from LUT at fixed RC and DD" << std::endl ;
  if ( iOpt_Initial_YT_fit == 2 ) std::cout << "   the PRF from TFile                 " << std::endl ;


//-----------------------------------------------------------Hey Ho! Let's Go
//Determine the hotest pad position along the Y-axis
  std::cout << std::endl ;
  std::cout << "*Determine the hotest pad position along the Y-axis " << std::endl ;
  TH1I* pTH1I_Tempo  = GiveMe_pTH1I_LeadingiY(aSample, ModuleNber , "Tempo") ;   
  int iPad_Beam = pTH1I_Tempo->GetMaximumBin() - 1 ;
  delete pTH1I_Tempo ;
  std::cout << "   iPad_Beam = "   << iPad_Beam << std::endl ;


//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "BIDON" ; 
  std::string Str_Out_PDF_Beg = "BIDON" ; 
  std::string Str_Out_PDF_End = "BIDON" ; 

//
  double Vmin = 0. ;
  double Vmax = 0. ;


//--------------------------------------------------------------------
//Set Initial values of YTrack
  std::cout << std::endl ;
  std::cout << "*Set starting YT  " << std::endl ;
  if ( 
       StartFrom != 0
  &&   StartFrom != 1
  ){
    std::cout << "ARGGH! StartFrom = " << StartFrom << std::endl ;
    abort();
  }
  Set_Starting_YT_Sample ( aSample,ModuleNber , StartFrom) ;


//--------------------------------------------------------------------
//Set Initial values of PRF function
  std::cout << std::endl ;
  std::cout << "*Set Initial values of PRF function used to fit rho VS (YT-YPad) " << std::endl ;

  SC_VS_x_rc_FIX_dd           aSC_VS_x_rc_FIX_dd(PRFfile_Fixed_DD) ;
  SC_VS_x_rc_FIX_dd_4TF1_RhoPRF  aSC_VS_x_rc_FIX_dd_4TF1_RhoPRF(&aSC_VS_x_rc_FIX_dd) ;

  TF1 * pTF1_rho_VS_YTrackYPad = 0;
  pTF1_rho_VS_YTrackYPad = new TF1("pTF1_rho_VS_YTrackYPad" ,aSC_VS_x_rc_FIX_dd_4TF1_RhoPRF ,vY_min_DiffPRF,vY_max_DiffPRF,1);
  
//pTF1_rho_VS_YTrackYPad->SetParameter(0,40.);
  pTF1_rho_VS_YTrackYPad->SetParameter(0,60.);

  pTF1_rho_VS_YTrackYPad->SetParName(0,"RC");

//--------------------------------------------------------------------
//Draw out Initial Status
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"InitialState");


//--------------------------------------------------------------------
//Selection
  std::cout << std::endl ;
  std::cout << "*Do Selection " << std::endl ;
  
  JFL_Selector aJFL_Selector(SelectionSet) ;
  
  aJFL_Selector.ApplySelection(aSample, ModuleNber) ;
  
  aJFL_Selector.PrintStat();

//--------------------------------------------------------------------
//Draw out Status After Selection
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"AfterSelection");


//Do an initial fit for YT values 
  std::cout << std::endl ;
  std::cout << "*Initial fit for YT values " << std::endl ;
  std::cout << "   ->Start " << std::endl ;

  if (iOpt_Initial_YT_fit==1) Do_ClusterFit_PV0_PV1( aSample, ModuleNber  ,pTF1_PRFfile_Fixed_RC_DD ) ;
  if (iOpt_Initial_YT_fit==2) Do_ClusterFit_PV0_PV1( aSample, ModuleNber  ,pTF1_FromTFile   ) ;

  std::cout << "   ->End " << std::endl ;


//--------------------------------------------------------------------
//Draw out Status After Selection
  JeanFrancois_DrawOut_Status(aSample, ModuleNber ,OUTDirName,"AfterInitialFit");


//--------------------------------------------------------------------
//Grand Manege
  TH1F* Input_pTH1F_Rho = GiveMe_pTH1F_Rho(aSample, ModuleNber ) ;
 
  int RCfit_Kounter_Fit     = 0 ; 
  int RCfit_Kounter_Failure = 0 ; 

  for (int iTeration = 0 ; iTeration< iTerationMax; iTeration++){

//
    std::cout << std::endl ;
    std::cout << "->START iTeration " << iTeration << std::endl ;

//
    std::ostringstream aostringstream_TFileName ;
    aostringstream_TFileName << std::setiosflags(std::ios::fixed) ;
    aostringstream_TFileName << OUTDirName <<"Turn_" << iTeration ;
    aostringstream_TFileName << ".root" ;
    std::string TFileName = aostringstream_TFileName.str() ; 

    TFile*  pTFile = new TFile(TFileName.c_str(),"RECREATE") ;

//
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << OUTDirName <<"Turn_" << iTeration ;
    aostringstream << ".pdf"  ;
    Str_Out_PDF = aostringstream.str() ; 
    Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
    Str_Out_PDF_End = Str_Out_PDF + ")" ;


//--------------------------------------------------------------------
//  Characterize INPUT
  
    std::ostringstream aostringstream_TAGIN ;
    aostringstream_TAGIN << std::setiosflags(std::ios::fixed) ;
    aostringstream_TAGIN << "Input_Turn_" << iTeration ;
    std::string TAGIN = aostringstream_TAGIN.str() ; 

    TH1F*     Input_pTH1F_YTrackYPad               = GiveMe_pTH1F_YTrackYPad               (aSample, ModuleNber ,TAGIN) ;
    TProfile* Input_pTProfile_rho_VS_YTrackYPad    = GiveMe_pTProfile_rho_VS_YTrackYPad    (aSample, ModuleNber ,TAGIN,NberOfBinsForProfile, vY_min_DiffPRF , vY_max_DiffPRF);
    TH2D*     Input_pTH2D_rho_VS_YTrackYPad        = GiveMe_pTH2D_rho_VS_YTrackYPad        (aSample, ModuleNber ,TAGIN,NberOfBins_TH2D_rho_VS_YTrackYPad, vY_min_DiffPRF , vY_max_DiffPRF);

    TH1F*     Input_pTH1F_YTrack                   = GiveMe_pTH1F_YTrack                   (aSample, ModuleNber ,TAGIN,iPad_Beam);
   
//--------------------------------------------------------------------
//  Fit Scatter plot #rho VS YTrack-YPad
    std::cout << std::endl ;
    std::cout << "  *Fit PRF plot "<< std::endl ;
    std::cout << "   ->Start "<< std::endl ;
    
    std::cout << "      Initial parameters"<< std::endl ;
    int NberOfParameters_pTF1_rho_VS_YTrackYPad = pTF1_rho_VS_YTrackYPad->GetNpar ();
    for (int iPar = 0 ; iPar< NberOfParameters_pTF1_rho_VS_YTrackYPad; iPar++){
      std::cout << "         " << std::setw(20)                         << pTF1_rho_VS_YTrackYPad->GetParName(iPar)
                << " "         << std::setw(20) << std::setprecision(6) << pTF1_rho_VS_YTrackYPad->GetParameter(iPar) << std::endl ;
    }
    
//  Fit PRF plot on a TProfile  
//  int fitStatus = -1 ;
//  fitStatus = Input_pTProfile_rho_VS_YTrackYPad->Fit("pTF1_rho_VS_YTrackYPad");

//  Fit PRF plot on a TH2D; Samira's code
    int fitStatus = -1 ;
    TGraphErrors* pTGraphErrors_FitTH2D = FitRhoVsYTYP(
                                                       fitStatus ,
                                                       Input_pTH2D_rho_VS_YTrackYPad,
                                                       "pTF1_rho_VS_YTrackYPad" ,
                                                       vY_min_DiffPRF,
                                                       vY_max_DiffPRF
                                                      ); 

    RCfit_Kounter_Fit += 1 ;
    if (fitStatus!=0) RCfit_Kounter_Failure += 1 ;
    if (fitStatus!=0) 
    std::cout 
      << " PV1_BASE WARNING: " 
      << " fitStatus = " << fitStatus
      << std::endl ;

    std::cout << "      Final parameters"<< std::endl ;
    for (int iPar = 0 ; iPar< NberOfParameters_pTF1_rho_VS_YTrackYPad; iPar++){
      std::cout << "         " << std::setw(20)                         << pTF1_rho_VS_YTrackYPad->GetParName(iPar)
                << " "         << std::setw(20) << std::setprecision(6) << pTF1_rho_VS_YTrackYPad->GetParameter(iPar) 
                << " +/- "     << std::setw(20) << std::setprecision(6) << pTF1_rho_VS_YTrackYPad->GetParError(iPar) 
                << std::endl ;
    }
    std::cout << "   ->End "<< std::endl ;

    if (pTF1_rho_VS_YTrackYPad->GetParameter(0) <= 20.) pTF1_rho_VS_YTrackYPad->SetParameter(0,60.);

//--------------------------------------------------------------------
    TGraph* pTGraph_PRF_rho_VS_YTrackYPad_Diff  = 0 ;
    TGraph* pTGraph_PRF_rho_VS_YTrackYPad_Ratio = 0 ;
    
    if (pTF1_FromTFile){
      pTGraph_PRF_rho_VS_YTrackYPad_Diff  = new TGraph ;
      pTGraph_PRF_rho_VS_YTrackYPad_Ratio = new TGraph ;

      for (int iPoint_DiffPRF = 0 ; iPoint_DiffPRF< Npoints_DiffPRF ; iPoint_DiffPRF++){
        double vY_cur = vY_min_DiffPRF + double (iPoint_DiffPRF)*(vY_max_DiffPRF-vY_min_DiffPRF)/double(Npoints_DiffPRF-1) ;

        double DiffPRF_rho_VS_YTrackYPad = pTF1_rho_VS_YTrackYPad->Eval(vY_cur)-pTF1_FromTFile->Eval(vY_cur) ;
        pTGraph_PRF_rho_VS_YTrackYPad_Diff->SetPoint( pTGraph_PRF_rho_VS_YTrackYPad_Diff->GetN(), vY_cur , DiffPRF_rho_VS_YTrackYPad  ) ;

        double RatioPRF_rho_VS_YTrackYPad = pTF1_rho_VS_YTrackYPad->Eval(vY_cur)/pTF1_FromTFile->Eval(vY_cur) ;
        pTGraph_PRF_rho_VS_YTrackYPad_Ratio->SetPoint( pTGraph_PRF_rho_VS_YTrackYPad_Ratio->GetN(), vY_cur , RatioPRF_rho_VS_YTrackYPad  ) ;
      }
    }
    
    
//--------------------------------------------------------------------
//  Graphics Page 1
gStyle->SetOptStat(111111) ;
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//--------------------------------------------------------------------
//  Graphics Page 2
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 1 );
      Input_pTH1F_YTrack  ->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 1 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 1 ))->GetUymax() ;
      for (int iV = -4 ; iV< 6; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
        
    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 3
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 1 );
      Input_pTH1F_Rho->Draw();
      pTCanvas->Update();
  
      pTCanvas->cd( 3 );
      Input_pTH1F_YTrackYPad->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 3 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 3 ))->GetUymax() ;
      for (int iV = -2 ; iV< 4; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 4
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 
 
      pTCanvas->cd( 1 );
      Input_pTProfile_rho_VS_YTrackYPad->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 1 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 1 ))->GetUymax() ;
      for (int iV = -1 ; iV< 3; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      Vmin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = -1 ; iV< 3; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
 
      pTCanvas->cd( 3 );
      Input_pTH2D_rho_VS_YTrackYPad->Draw("colz");
      pTF1_rho_VS_YTrackYPad->Draw("SAME");
      pTGraphErrors_FitTH2D->Draw("SAME");
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 3 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 3 ))->GetUymax() ;
      for (int iV = -1 ; iV< 3; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 5
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 
 
      if (pTGraph_PRF_rho_VS_YTrackYPad_Diff){
        pTCanvas->cd( 3 );
        pTCanvas->cd( 3 )->SetGrid();
        pTGraph_PRF_rho_VS_YTrackYPad_Diff->SetTitle("PRF_{Param}(x)- PRF_{Ref}(x) VS x (cm)");  
        pTGraph_PRF_rho_VS_YTrackYPad_Diff->Draw();
        pTCanvas->Update();
 
        Vmin = (pTCanvas->cd( 3 ))->GetUymin() ;
        Vmax = (pTCanvas->cd( 3 ))->GetUymax() ;
        for (int iV = -1 ; iV< 3; iV++){
          double YCur =   LY/2. + double (iV-1) * LY ;
          TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
          pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
          pTCanvas->Update()    ;
        }
      }
      
      if (pTGraph_PRF_rho_VS_YTrackYPad_Ratio){
        pTCanvas->cd( 4 );
        pTCanvas->cd( 4 )->SetGrid();
        pTGraph_PRF_rho_VS_YTrackYPad_Ratio->SetTitle("PRF_{Param}(x)/PRF_{Ref}(x) VS x (cm)");  
        pTGraph_PRF_rho_VS_YTrackYPad_Ratio->Draw();
        pTCanvas->Update();
 
        Vmin = (pTCanvas->cd( 4 ))->GetUymin() ;
        Vmax = (pTCanvas->cd( 4 ))->GetUymax() ;
        for (int iV = -1 ; iV< 3; iV++){
          double YCur =   LY/2. + double (iV-1) * LY ;
          TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
          pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
          pTCanvas->Update()    ;
        }
      }
 
    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Do cluster position fit
    std::cout << std::endl ;
    std::cout << "  *Fit YT"<< std::endl ;
    std::cout << "   ->Start "<< std::endl ;

    Do_ClusterFit_PV0_PV1( aSample, ModuleNber  ,pTF1_rho_VS_YTrackYPad ) ;
    
    std::cout << "   ->End "<< std::endl ;


//--------------------------------------------------------------------
//  Characterize OUTPUT
    
    std::ostringstream aostringstream_TAGOUT ;
    aostringstream_TAGOUT << std::setiosflags(std::ios::fixed) ;
    aostringstream_TAGOUT << "Output_Turn_" << iTeration ;
    std::string TAGOUT = aostringstream_TAGOUT.str() ; 

    TH1F*     Output_pTH1F_YTrackYPad                 = GiveMe_pTH1F_YTrackYPad                  (aSample, ModuleNber ,TAGOUT) ;

    TH1F*     Output_pTH1F_YTrack                     = GiveMe_pTH1F_YTrack                      (aSample, ModuleNber ,TAGOUT,iPad_Beam);

    TProfile* Output_pTProfile_DeltaYTrack_VS_YTrack  = GiveMe_pTProfile_DeltaYTrack_VS_YTrack   (aSample, ModuleNber ,TAGOUT,iPad_Beam);
    TProfile* Output_pTProfile_ErrorOnYTrack_VS_Track = GiveMe_pTProfile_ErrorOnYTrack_VS_YTrack (aSample, ModuleNber ,TAGOUT,iPad_Beam);

    TH1F*     Output_pTH1F_PullFitCluster             = GiveMe_pTH1F_PullFitCluster_PV0_PV1      (aSample, ModuleNber ,TAGOUT) ;

//--------------------------------------------------------------------
//  Graphics Page 6
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 3 );
      Output_pTH1F_PullFitCluster     ->Draw();
      pTCanvas->Update();
      
      pTCanvas->cd( 4 );
      pTCanvas->cd( 4 )->SetLogy(1);
      Output_pTH1F_PullFitCluster     ->Draw();
      pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 7
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 1 );
      Output_pTH1F_YTrack  ->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 1 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 1 ))->GetUymax() ;
      for (int iV = -4 ; iV< 6; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
    

    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 8
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 2 );
      Output_pTH1F_YTrackYPad->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = -2 ; iV< 4; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
    pTCanvas->Print(Str_Out_PDF.c_str());

//
//--------------------------------------------------------------------
//  Graphics Page 9
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 1 );
      pTCanvas->cd( 1 )->SetGrid();

      Output_pTProfile_DeltaYTrack_VS_YTrack->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 1 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 1 ))->GetUymax() ;
      for (int iV = 0 ; iV< 2; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      pTCanvas->cd( 2 );
      pTCanvas->cd( 2 )->SetGrid();

      Output_pTProfile_ErrorOnYTrack_VS_Track->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = 0 ; iV< 2; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

    pTCanvas->Print(Str_Out_PDF_End.c_str());


//--------------------------------------------------------------------
//  Store histos stuff in output root file
    Input_pTH2D_rho_VS_YTrackYPad ->Write("Input_pTH2D_rho_VS_YTrackYPad" ) ;

    pTGraphErrors_FitTH2D         ->Write("pTGraphErrors_FitTH2D"         ) ;

    pTF1_rho_VS_YTrackYPad        ->Write("pTF1_rho_VS_YTrackYPad"        ) ;

//
    Input_pTProfile_rho_VS_YTrackYPad    ->Write("Input_pTProfile_rho_VS_YTrackYPad"        ) ;

    Input_pTH1F_YTrackYPad               ->Write("Input_pTH1F_YTrackYPad"                   ) ;

    Input_pTH1F_YTrack                   ->Write("Input_pTH1F_YTrack"                       ) ;

//
    Output_pTH1F_YTrackYPad                 ->Write("Output_pTH1F_YTrackYPad"                  ) ;

    Output_pTH1F_YTrack                     ->Write("Output_pTH1F_YTrack"                      ) ;

    Output_pTProfile_DeltaYTrack_VS_YTrack  ->Write("Output_pTProfile_DeltaYTrack_VS_YTrack"   ) ;

    Output_pTProfile_ErrorOnYTrack_VS_Track ->Write("Output_pTProfile_ErrorOnYTrack_VS_Track"  ) ;
    
    if (pTGraph_PRF_rho_VS_YTrackYPad_Diff ) pTGraph_PRF_rho_VS_YTrackYPad_Diff  ->Write("pTGraph_PRF_rho_VS_YTrackYPad_Diff"  ) ;
    if (pTGraph_PRF_rho_VS_YTrackYPad_Ratio) pTGraph_PRF_rho_VS_YTrackYPad_Ratio ->Write("pTGraph_PRF_rho_VS_YTrackYPad_Ratio"  ) ;

    Output_pTH1F_PullFitCluster         ->Write("Output_pTH1F_PullFitCluster"        ) ;

    pTFile->Close() ;
    delete pTFile ;

//
//  delete Input_pTH1F_YTrackYPad               ;
//  delete Input_pTProfile_rho_VS_YTrackYPad    ;
//  delete Input_pTH2D_rho_VS_YTrackYPad        ;
//  delete Input_pTH1F_YTrack                   ;
//  
//  delete Output_pTH1F_YTrackYPad                 ; 
//  delete Output_pTH1F_YTrack                     ;
//  delete Output_pTProfile_DeltaYTrack_VS_YTrack  ;
//  delete Output_pTProfile_ErrorOnYTrack_VS_Track ;
//  
//  delete pTGraphErrors_FitTH2D                      ;

//
    std::cout << std::endl ;
    std::cout << "->END iTeration " << iTeration << std::endl ;

  }
  std::cout << std::endl ;
  std::cout << " PV1_BASE " << std::endl ;
  std::cout << "   Nber of RC Fits        " << std::setw(20) << RCfit_Kounter_Fit     << std::endl ;
  std::cout << "   Nber of Failed RC Fits " << std::setw(20) << RCfit_Kounter_Failure 
     << " ( " 
     << std::setw(10) << std::setprecision(4) 
     << 100.* double (RCfit_Kounter_Failure)/double (RCfit_Kounter_Fit) 
     << " % ) "
     << std::endl ;

  delete Input_pTH1F_Rho  ;
  
  delete pTF1_FromTFile         ;
  delete pTF1_PRFfile_Fixed_RC_DD       ;
  delete pTF1_rho_VS_YTrackYPad ;

//--------------------------------------------------------------------  
//Reconstruct Tracks 
  std::cout << std::endl ;
  std::cout << "*Tracks reconstruction "<< std::endl ;
  std::cout << "  ->Start "<< std::endl ;
  DoTracksReconstruction(aSample, ModuleNber ,iOpt_TrackType) ;
  std::cout << "  ->End "<< std::endl ;

//--------------------------------------------------------------------  
//Analyse Tracks 
  std::cout << std::endl ;
  std::cout << "*Tracks Analysis "<< std::endl ;
  std::cout << "  ->Start "<< std::endl ;
  AnalyseReconstruction(OUTDirName,AnalyseName,"FINAL",aSample,ModuleNber) ;
  std::cout << "  ->End "<< std::endl ;
  
//
  std::cout.rdbuf(coutbuf); // Reset to standard output again

}
