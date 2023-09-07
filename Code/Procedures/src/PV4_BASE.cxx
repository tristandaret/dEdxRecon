#include "Procedures/PV4_BASE.h"
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

#include "SignalShape/SC_VS_x_rc_dd.h"
#include "SignalShape/SC_VS_x_rc_dd_4TF1_AlphaPRF.h"

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
#include "TVectorD.h"

void PV4_BASE(
  const int&         iOptQuiet          ,
  const std::string& Flavor             ,
  const std::string& OutDir             ,
  const std::string& EventFile          ,
  const std::string& FILE_SC_VS_x_rc_dd ,
  const double&      DD_Sample          ,
  const int&         Uploader           ,
  const int&         StartFrom          ,
  const int&         iOpt_TrackType     ,
  const double&      SamplingFrequency  ,
  const int&         ModuleNber         ,
  const std::string& SelectionSet       ,
  const int&         Data_to_Use
  
){ 
//
   if (ModuleNber<0 || ModuleNber>7){
     std::cout << " PV4_BASE " 
               << " ModuleNber = " << ModuleNber
               << " out of range "
               << std::endl ;
     abort() ;
   }
//
  std::string AnalyseName = "PV4_" + Flavor ;
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
      << " " << "Main_AOBt_PV4_BASE"                  
      << " " << Flavor                  
      << " " << OutDir              
      << " ${EVENTDIR}/" << EventFile.substr(EventFile.find_last_of("//") + 1)     
      << " " << FILE_SC_VS_x_rc_dd 
      << " " << DD_Sample          
      << " " << Uploader                
      << " " << StartFrom               
      << " " << iOpt_TrackType 
      << " " << SamplingFrequency 
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
        std::cout << " PV4_BASE: "<< std::endl ;
        std::cout << "    Flavor             : " << Flavor             << std::endl ;
        std::cout << "    OutDir             : " << OutDir             << std::endl ;
        std::cout << "    EventFile          : " << EventFile          << std::endl ;
        std::cout << "    FILE_SC_VS_x_rc_dd : " << FILE_SC_VS_x_rc_dd << std::endl ;
        std::cout << "    DD_Sample          : " << DD_Sample          << std::endl ;
        std::cout << "    Uploader           : " << Uploader           << std::endl ;
        std::cout << "    StartFrom          : " << StartFrom          << std::endl ;
        std::cout << "    iOpt_TrackType     : " << iOpt_TrackType     << std::endl ;
        std::cout << "    SamplingFrequency  : " << SamplingFrequency  << std::endl ;
        std::cout << "    ModuleNber         : " << ModuleNber         << std::endl ;
        std::cout << "    SelectionSet       : " << SelectionSet       << std::endl ;
        std::cout << "    Data_to_Use        : " << Data_to_Use        << std::endl ;
        std::cout << std::endl ;
  
    std::cout 
      << " " << "Main_AOBt_PV4_BASE"                  
      << " " << Flavor                  
      << " " << OutDir              
      << " ${EVENTDIR}/" << EventFile.substr(EventFile.find_last_of("//") + 1)     
      << " " << FILE_SC_VS_x_rc_dd 
      << " " << DD_Sample          
      << " " << Uploader                
      << " " << StartFrom               
      << " " << iOpt_TrackType 
      << " " << SamplingFrequency 
      << " " << ModuleNber 
      << " " << SelectionSet 
      << " " << Data_to_Use 
      << std::endl ;

//-----------------------------------------------------------//
//Settings
  int iTerationMax = 10 ;
   
  double RC_Start = 100. ;
         RC_Start = 60. ;
  double DD_Start = DD_Sample ;
  
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

  int NberOfBins_TH2D_rhoLeading_VS_YTrackYPad = 100 ;


//Settings -------------------------------------------------------Starting YT 
  std::cout << std::endl ;
  std::cout << "*Option-Set starting YT as " << std::endl ;
  if ( StartFrom == 0 ) std::cout << "   weighthed  " << std::endl ;
  if ( StartFrom == 1 ) std::cout << "   Leading pad" << std::endl ;
  

//Settings -------------------------------------------------------Determine the hotest pad position along the Y-axis
  std::cout << std::endl ;
  std::cout << "*Determine the hotest pad position along the Y-axis " << std::endl ;
  TH1I* pTH1I_Tempo  = GiveMe_pTH1I_LeadingiY(aSample, ModuleNber ,"Tempo") ;   
  int iPad_Beam = pTH1I_Tempo->GetMaximumBin() - 1 ;
  delete pTH1I_Tempo ;
  std::cout << "   iPad_Beam = "   << iPad_Beam << std::endl ;


//-----------------------------------------------------------Hey Ho! Let's Go
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


//Settings -------------------------------------------------------Set the 3D LUT  from file
  std::cout << std::endl ;
  std::cout << "*Set the 3D LUT  from file : " << FILE_SC_VS_x_rc_dd << std::endl ;
  SC_VS_x_rc_dd aSC_VS_x_rc_dd(FILE_SC_VS_x_rc_dd,SamplingFrequency) ;


//--------------------------------------------------------------------
//Set Initial values of PRF function
  std::cout << std::endl ;
  std::cout << "*Set Initial values of PRF function used to fit rho VS (YT-YPad) " << std::endl ;

  SC_VS_x_rc_dd_4TF1_AlphaPRF  aSC_VS_x_rc_dd_4TF1_AlphaPRF(&aSC_VS_x_rc_dd) ;
  TF1 * pTF1_RhoLeading_VS_YTrackYPad = 0 ;
  pTF1_RhoLeading_VS_YTrackYPad = new TF1("pTF1_RhoLeading_VS_YTrackYPad" ,aSC_VS_x_rc_dd_4TF1_AlphaPRF ,vY_min_DiffPRF,vY_max_DiffPRF,2);
  
  pTF1_RhoLeading_VS_YTrackYPad->SetParameter(0,RC_Start);
  pTF1_RhoLeading_VS_YTrackYPad->SetParameter(1,DD_Start);
  
  pTF1_RhoLeading_VS_YTrackYPad->SetParName(0,"RC");
  pTF1_RhoLeading_VS_YTrackYPad->SetParName(1,"DD");

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

//--------------------------------------------------------------------
//  Do cluster position fit
    std::cout << std::endl ;
    std::cout << "  *Fit YT"<< std::endl ;
    std::cout << "   ->Start "<< std::endl ;
 
    Do_ClusterFit_PV4( aSample, ModuleNber  ,pTF1_RhoLeading_VS_YTrackYPad ) ;
    
    std::cout << "   ->End "<< std::endl ;

//--------------------------------------------------------------------
//Grand Manege
  TH1F* Input_pTH1F_AlphaNotLeading  = GiveMe_pTH1F_AlphaNotLeading(aSample, ModuleNber ,"DATA") ;
  TH1F* Input_pTH1F_TauNotLeading    = GiveMe_pTH1F_TauNotLeading  (aSample, ModuleNber ,"DATA",106,  -5.5 , 100.) ;

  TH2F* Input_pTH2F_Alpha2_VS_Alpha3onAlpha2 = GiveMe_pTH2F_Alpha2_VS_Alpha3onAlpha2 (aSample, ModuleNber , "DATA") ;
  TH2F* Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM    = GiveMe_pTH2F_Tau2_VS_Tau3mTau2        (aSample, ModuleNber , "DATA", 104,-3.5,100.5, 34,-3.5,30.5 ) ;

  TH2F* Input_pTH2F_Alpha2_VS_Tau2               = GiveMe_pTH2F_Alpha2_VS_Tau2               (aSample, ModuleNber , "DATA" ) ;  
  TH2F* Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2  = GiveMe_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2  (aSample, ModuleNber , "DATA" ) ;  

  int RCDDfit_Kounter_Fit     = 0 ; 
  int RCDDfit_Kounter_Failure = 0 ; 
  
  double RC_Last = RC_Start ;
  double DD_Last = DD_Start ;

  for (int iTeration = 0 ; iTeration< iTerationMax; iTeration++){

//
    std::cout << std::endl ;
    std::cout << "->START iTeration " << iTeration << std::endl ;

//
    aSample.Set_RC(RC_Last) ;
    aSample.Set_DD(DD_Last) ;
    std::cout << "  with RC = " << RC_Last << std::endl ;
    std::cout << "  and  DD = " << DD_Last << std::endl ;

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

    TH1F*     Input_pTH1F_YTrackYPad        = GiveMe_pTH1F_YTrackYPad        (aSample, ModuleNber ,TAGIN,1200, -3*LY *1.E2 , 3*LY *1.E2) ;

    TH1F*     Input_pTH1F_YTrackYPadLeading = GiveMe_pTH1F_YTrackYPadLeading (aSample, ModuleNber ,TAGIN,400, -LY *1.E2 , LY *1.E2) ;

    TH1F*     Input_pTH1F_YTrack            = GiveMe_pTH1F_YTrack            (aSample, ModuleNber ,TAGIN,iPad_Beam);
   
    TH2F*     Input_pTH2F_Alpha_VS_YTYpad  = GiveMe_pTH2F_Alpha_VS_YTYpad(
                                                                     aSample, ModuleNber ,TAGIN, 
                                                                     NberOfBins_TH2D_rhoLeading_VS_YTrackYPad, 
                                                                     vY_min_DiffPRF , 
                                                                     vY_max_DiffPRF, 
                                                                     102, 
                                                                     0. , 
                                                                     1.02
                                                                     ) ;
    TH2F*     Input_pTH2F_Tau_VS_YTYpad = GiveMe_pTH2F_Tau_VS_YTYpad(
                                                                     aSample, ModuleNber ,TAGIN, 
                                                                     NberOfBins_TH2D_rhoLeading_VS_YTrackYPad, 
                                                                     vY_min_DiffPRF , 
                                                                     vY_max_DiffPRF, 
                                                                     106, 
                                                                      -5.5 , 
                                                                     100.5
                                                                     ) ;

//--------------------------------------------------------------------
//  Graphics Page 1
gStyle->SetOptStat(111111) ;
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//--------------------------------------------------------------------
//  Graphics Page 2
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      Input_pTH1F_AlphaNotLeading->Draw();
      pTCanvas->Update();

 
      pTCanvas->cd( 4 );
      Input_pTH1F_TauNotLeading->Draw();
      pTCanvas->Update();

 
    pTCanvas->cd(2);
    Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->Draw("colz");

      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2} =A3/A2");
      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitle("#alpha_{2} = A2/A1");
      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(5);
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->Draw("colz") ;

      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(3);
      Input_pTH2F_Alpha2_VS_Tau2->Draw("colz") ;

      Input_pTH2F_Alpha2_VS_Tau2->GetXaxis()->SetTitle("#tau_{2}=T2-T1");
      Input_pTH2F_Alpha2_VS_Tau2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      Input_pTH2F_Alpha2_VS_Tau2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(6);
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->Draw("colz") ;

      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetYaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());

 //--------------------------------------------------------------------
//  Graphics Page 3
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

     pTCanvas->cd( 4 );
      Input_pTH1F_YTrack  ->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 4 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 4 ))->GetUymax() ;
      for (int iV = -4 ; iV< 6; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
        
      pTCanvas->cd( 5 );
      Input_pTH1F_YTrackYPad->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 5 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 5 ))->GetUymax() ;
      for (int iV = -2 ; iV< 4; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
      pTCanvas->cd( 6 );
      Input_pTH1F_YTrackYPadLeading->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 6 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 6 ))->GetUymax() ;
      for (int iV = 0 ; iV< 2; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      
      pTCanvas->cd( 2 );
      Input_pTH2F_Alpha_VS_YTYpad->Draw("colz");
      pTF1_RhoLeading_VS_YTrackYPad->Draw("SAME");
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = -1 ; iV< 3; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      pTCanvas->cd( 3 );
      Input_pTH2F_Tau_VS_YTYpad->Draw("colz");
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

//-----------------------------------------------------------------------------------//
//-------------------------------Fit RC and DD on the sample-------------------------//

    std::cout << "  *Fit RC and DD on the sample "<< std::endl ;
    std::cout << "   ->Start "<< std::endl ;
    std::cout << "  with RC = " << aSample.Get_RC() << std::endl ;
    std::cout << "  and  DD = " << aSample.Get_DD() << std::endl ;
    
//-------------------------------Before Fit-------------------------//

    pTF1_RhoLeading_VS_YTrackYPad->SetParameter(0, RC_Last);
    pTF1_RhoLeading_VS_YTrackYPad->SetParameter(1, DD_Last);
    std::cout << "      Initial parameters"<< std::endl ;
    int NberOfParameters_pTF1_RhoLeading_VS_YTrackYPad = pTF1_RhoLeading_VS_YTrackYPad->GetNpar ();
    for (int iPar = 0 ; iPar< NberOfParameters_pTF1_RhoLeading_VS_YTrackYPad; iPar++){
      std::cout << "         " << std::setw(20)                         << pTF1_RhoLeading_VS_YTrackYPad->GetParName(iPar)
                << " "         << std::setw(20) << std::setprecision(6) << pTF1_RhoLeading_VS_YTrackYPad->GetParameter(iPar) << std::endl ;
    }
    
//-------------------------------Fit--------------------------------//

    int FitResult = Do_SampleFit_PV4(aSample, ModuleNber ,pTF1_RhoLeading_VS_YTrackYPad) ;
    RCDDfit_Kounter_Fit += 1 ;
    if (FitResult!=0) RCDDfit_Kounter_Failure += 1 ;

//-------------------------------After Fit--------------------------//

    double RC_New = aSample.Get_RC() ;
    double DD_New = aSample.Get_DD() ;
    if (RC_New<= 20.) { RC_New = 60.;    aSample.Set_RC(RC_New) ; }

    pTF1_RhoLeading_VS_YTrackYPad->SetParameter(0,aSample.Get_RC());
    pTF1_RhoLeading_VS_YTrackYPad->SetParameter(1,aSample.Get_DD());

//
    pTF1_RhoLeading_VS_YTrackYPad->SetParError(0,aSample.Get_eRC()) ;
    pTF1_RhoLeading_VS_YTrackYPad->SetParError(1,aSample.Get_eDD()) ;

    std::cout << "      Final parameters"<< std::endl ;
    for (int iPar = 0 ; iPar< NberOfParameters_pTF1_RhoLeading_VS_YTrackYPad; iPar++){
      std::cout << "         " << std::setw(20)                         << pTF1_RhoLeading_VS_YTrackYPad->GetParName(iPar)
                << " "         << std::setw(20) << std::setprecision(6) << pTF1_RhoLeading_VS_YTrackYPad->GetParameter(iPar) 
                << " +/- "     << std::setw(20) << std::setprecision(6) << pTF1_RhoLeading_VS_YTrackYPad->GetParError(iPar) 
                << std::endl ;
    }
    
//
    std::cout << std::endl ;
    std::cout << "After Fit " << std::endl ;
    std::cout 
      << " RC = " << std::setw(7) << std::setprecision(3) << aSample.Get_RC()
      << " +/-  " << std::setw(7) << std::setprecision(3) << aSample.Get_eRC()
      << std::endl ;
    std::cout 
      << " DD = " << std::setw(7) << std::setprecision(3) << aSample.Get_DD()
      << " +/-  " << std::setw(7) << std::setprecision(3) << aSample.Get_eDD()
      << std::endl ;

    double    Chi2Min_RCfit = aSample.Get_Chi2Min();
    int       NODF_RCfit    = aSample.FitRes_PV4_Get_NberOfTermsInChi2() - 1 ;
    std::cout << std::endl ;
    std::cout << " Chi2Min_RCfit "            << std::setw(20) << std::setprecision(3) << Chi2Min_RCfit            << std::endl ;
    std::cout << " NODF_RCfit    "            << std::setw(20) << std::setprecision(3) << NODF_RCfit               << std::endl ;
    std::cout << " Chi2Min_RCfit/NODF_RCfit " << std::setw(20) << std::setprecision(3) << Chi2Min_RCfit/NODF_RCfit << std::endl ;
    
    std::cout << "   ->End "<< std::endl ;


//-----------------------------------------------------------------------------------//
//-------------------------------Do cluster position fit-----------------------------//

    std::cout << std::endl ;
    std::cout << "  *Fit YT"<< std::endl ;
    std::cout << "   ->Start "<< std::endl ;

    Do_ClusterFit_PV4( aSample, ModuleNber  ,pTF1_RhoLeading_VS_YTrackYPad ) ;
    
    std::cout << "   ->End "<< std::endl ;


//--------------------------------------------------------------------
//  Characterize OUTPUT
    
    std::ostringstream aostringstream_TAGOUT ;
    aostringstream_TAGOUT << std::setiosflags(std::ios::fixed) ;
    aostringstream_TAGOUT << "Output_Turn_" << iTeration ;
    std::string TAGOUT = aostringstream_TAGOUT.str() ; 

    TH1F*     Output_pTH1F_YTrackYPad        = GiveMe_pTH1F_YTrackYPad        (aSample, ModuleNber ,TAGOUT,1200, -3*LY *1.E2 , 3*LY *1.E2) ;
    
    TH1F*     Output_pTH1F_YTrackYPadLeading = GiveMe_pTH1F_YTrackYPadLeading (aSample, ModuleNber ,TAGOUT,400, -LY *1.E2 , LY *1.E2) ;

    TH1F*     Output_pTH1F_YTrack            = GiveMe_pTH1F_YTrack            (aSample, ModuleNber ,TAGOUT,iPad_Beam);

    TH2F*     Output_pTH2F_Alpha_VS_YTYpad  = GiveMe_pTH2F_Alpha_VS_YTYpad(
                                                                   aSample, ModuleNber ,TAGOUT, 
                                                                   NberOfBins_TH2D_rhoLeading_VS_YTrackYPad, 
                                                                   vY_min_DiffPRF , 
                                                                   vY_max_DiffPRF, 
                                                                   102, 
                                                                   0. , 
                                                                   1.02
                                                                   ) ;
    TH2F*     Output_pTH2F_Tau_VS_YTYpad = GiveMe_pTH2F_Tau_VS_YTYpad(
                                                                     aSample, ModuleNber ,TAGOUT, 
                                                                     NberOfBins_TH2D_rhoLeading_VS_YTrackYPad, 
                                                                     vY_min_DiffPRF , 
                                                                     vY_max_DiffPRF, 
                                                                     106, 
                                                                      -5.5 , 
                                                                     100.5
                                                                     ) ;

    TProfile* Output_pTProfile_DeltaYTrack_VS_YTrack  = GiveMe_pTProfile_DeltaYTrack_VS_YTrack   (aSample, ModuleNber ,TAGOUT,iPad_Beam);
    TProfile* Output_pTProfile_ErrorOnYTrack_VS_Track = GiveMe_pTProfile_ErrorOnYTrack_VS_YTrack (aSample, ModuleNber ,TAGOUT,iPad_Beam);

    TH1F*     Output_pTH1F_PullFitRC                  = GiveMe_pTH1F_PullFitRC_PV4               (aSample, ModuleNber ,TAGOUT) ;

    TH1F*     Output_pTH1F_PullFitCluster             = GiveMe_pTH1F_PullFitCluster_PV2          (aSample, ModuleNber ,TAGOUT) ;

//--------------------------------------------------------------------
//  Graphics Page 4
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 4 );
      Output_pTH1F_YTrack  ->Draw();
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 4 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 4 ))->GetUymax() ;
      for (int iV = -4 ; iV< 6; iV++){
        double YCur =  pModel_ReadOutGeometry->Get_YHPad(0,iPad_Beam + iV-1,ModuleNber) ;        
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
  
      pTCanvas->cd( 5 );
      Output_pTH1F_YTrackYPad->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 5 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 5 ))->GetUymax() ;
      for (int iV = -2 ; iV< 4; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }
      pTCanvas->cd( 6 );
      Output_pTH1F_YTrackYPadLeading->Draw();
      pTCanvas->Update();
  
      Vmin = (pTCanvas->cd( 6 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 6 ))->GetUymax() ;
      for (int iV = 0 ; iV< 2; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      pTCanvas->cd( 2 );
      Output_pTH2F_Alpha_VS_YTYpad->Draw("colz");
      pTF1_RhoLeading_VS_YTrackYPad->Draw("SAME");
      pTCanvas->Update();

      Vmin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vmax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = -1 ; iV< 3; iV++){
        double YCur =   LY/2. + double (iV-1) * LY ;
        TLine* pTLineV = new TLine( YCur*1E2  , Vmin , YCur*1E2 , Vmax ) ;
        pTLineV->SetLineStyle(10); pTLineV->SetLineWidth(1); pTLineV->SetLineColor(2) ; pTLineV->Draw();  
        pTCanvas->Update()    ;
      }

      pTCanvas->cd( 3 );
      Output_pTH2F_Tau_VS_YTYpad->Draw("colz");
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
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

    pTCanvas->cd(2);
    Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->Draw("colz");

      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetXaxis()->SetTitle("#alpha_{3}/#alpha_{2} =A3/A2");
      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitle("#alpha_{2} = A2/A1");
      Input_pTH2F_Alpha2_VS_Alpha3onAlpha2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(5);
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->Draw("colz") ;

      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitle("#tau_{2}=T2-T1");
      Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(3);
      Input_pTH2F_Alpha2_VS_Tau2->Draw("colz") ;

      Input_pTH2F_Alpha2_VS_Tau2->GetXaxis()->SetTitle("#tau_{2}=T2-T1");
      Input_pTH2F_Alpha2_VS_Tau2->GetYaxis()->SetTitle("#alpha_{2}=A2/A1");
      Input_pTH2F_Alpha2_VS_Tau2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->cd(6);
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->Draw("colz") ;

      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetXaxis()->SetTitle("#tau_{3}-#tau_{2}=T3-T2");
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetYaxis()->SetTitle("#alpha_{3}/#alpha_{2}=A3/A2");
      Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 6
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(2,2); 

      pTCanvas->cd( 1 );
      Output_pTH1F_PullFitRC  ->Draw();
      pTCanvas->Update();
      
      pTCanvas->cd( 2 );
      pTCanvas->cd( 2 )->SetLogy(1);
      Output_pTH1F_PullFitRC  ->Draw();
      pTCanvas->Update();


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
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
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

    pTCanvas->Print(Str_Out_PDF.c_str());

//--------------------------------------------------------------------
//  Graphics Page 8
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Print(Str_Out_PDF_End.c_str());


//--------------------------------------------------------------------
//  Store histos stuff in output root file
//
      TVectorD aTVectorD_Chi2Min_RCfit(1) ; 
      aTVectorD_Chi2Min_RCfit[0] = Chi2Min_RCfit ;  
      aTVectorD_Chi2Min_RCfit.Write("aTVectorD_Chi2Min_RCfit_PV4");

      TVectorD aTVectorD_NODF_RCfit(1) ; 
      aTVectorD_NODF_RCfit[0] = NODF_RCfit ;  
      aTVectorD_NODF_RCfit.Write("aTVectorD_NODF_RCfit_PV4");

//
      pTF1_RhoLeading_VS_YTrackYPad        ->Write("pTF1_RhoLeading_VS_YTrackYPad"        ) ;

//
      Input_pTH1F_YTrackYPad               ->Write("Input_pTH1F_YTrackYPad"                   ) ;

      Input_pTH1F_YTrackYPadLeading        ->Write("Input_pTH1F_YTrackYPadLeading"            ) ;

      Input_pTH1F_YTrack                   ->Write("Input_pTH1F_YTrack"                       ) ;

      Input_pTH2F_Alpha_VS_YTYpad    ->Write("Input_pTH2F_Alpha_VS_YTYpad"        ) ;

      Input_pTH2F_Tau_VS_YTYpad    ->Write("Input_pTH2F_Tau_VS_YTYpad"        ) ;

//
      Output_pTH1F_YTrackYPad                 ->Write("Output_pTH1F_YTrackYPad"           ) ;

      Output_pTH1F_YTrackYPadLeading          ->Write("Output_pTH1F_YTrackYPadLeading"           ) ;

      Output_pTH1F_YTrack                     ->Write("Output_pTH1F_YTrack"                      ) ;

      Output_pTH2F_Alpha_VS_YTYpad   ->Write("Output_pTH2F_Alpha_VS_YTYpad"       ) ;

      Output_pTH2F_Tau_VS_YTYpad   ->Write("Output_pTH2F_Tau_VS_YTYpad"       ) ;

      Output_pTProfile_DeltaYTrack_VS_YTrack  ->Write("Output_pTProfile_DeltaYTrack_VS_YTrack"   ) ;

      Output_pTProfile_ErrorOnYTrack_VS_Track ->Write("Output_pTProfile_ErrorOnYTrack_VS_Track"  ) ;
      
      Output_pTH1F_PullFitRC         ->Write("Output_pTH1F_PullFitRC"         ) ;
      
      Output_pTH1F_PullFitCluster    ->Write("Output_pTH1F_PullFitCluster" ) ;

    pTFile->Close() ;
    delete pTFile ;

//  Prepare next iteration
    RC_Last = RC_New ;
    DD_Last = DD_New ;
    
//
    std::cout << std::endl ;
    std::cout << "->END iTeration " << iTeration << std::endl ;

  }
  std::cout << std::endl ;
  std::cout << " PV4_BASE " << std::endl ;
  std::cout << "   Nber of RC/DD Fits        " << std::setw(20) << RCDDfit_Kounter_Fit     << std::endl ;
  std::cout << "   Nber of Failed RC/DD Fits " << std::setw(20) << RCDDfit_Kounter_Failure 
     << " ( " 
     << std::setw(10) << std::setprecision(4) 
     << 100.* double (RCDDfit_Kounter_Failure)/double (RCDDfit_Kounter_Fit) 
     << " % ) "
     << std::endl ;

//
  std::ostringstream aostringstream_TFileName ;
  aostringstream_TFileName << std::setiosflags(std::ios::fixed) ;
  aostringstream_TFileName << OUTDirName <<"PureData" ;
  aostringstream_TFileName << ".root" ;
  std::string TFileName = aostringstream_TFileName.str() ; 

  TFile*  pTFile = new TFile(TFileName.c_str(),"RECREATE") ;

    Input_pTH1F_AlphaNotLeading        ->Write("Input_pTH1F_AlphaNotLeading" ) ;
    Input_pTH1F_TauNotLeading          ->Write("Input_pTH1F_TauNotLeading"   ) ;

    Input_pTH2F_Alpha2_VS_Alpha3onAlpha2    ->Write("Input_pTH2F_Alpha2_VS_Alpha3onAlpha2"    ) ;
    Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM       ->Write("Input_pTH2F_Tau2_VS_Tau3mTau2ZOOM"       ) ;
    Input_pTH2F_Alpha2_VS_Tau2              ->Write("Input_pTH2F_Alpha2_VS_Tau2"              ) ;
    Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2 ->Write("Input_pTH2F_Alpha3onAlpha2_VS_Tau3mTau2" ) ;

  pTFile->Close() ;
  delete pTFile ;

// 
  delete pTF1_RhoLeading_VS_YTrackYPad ;

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
