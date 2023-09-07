#include "AnalysisTools/AnalyseReconstruction.h"
#include "Misc/Util.h"

#include "EvtModel/Track.h"
#include "EvtModel/SetOfTracks.h"

#include "EvtModelTools/EvtModelTools_Histos.h"
#include "EvtModelTools/EvtModelTools_JFL_Histos.h"
#include "EvtModelTools/EvtModelTools_TD_Histos.h"

#include "SignalModel/Model_ReadOutGeometry.h"

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

void  AnalyseReconstruction(
                            const std::string& OUTDirName  ,
                            const std::string& AnalyseName ,
                            const std::string& TAG         ,
                            Sample& aSample, const int& ModuleNber
){
//--------------------------------------------------------------------
  int MinimalNberOfEntries = 100 ; // DESY 21
  int NberofPointAlongY    =  10 ; // DESY 21
 
  MinimalNberOfEntries = 10 ; // CERN 22
  NberofPointAlongY    =  5 ; // CERN 22

//--------------------------------------------------------------------
  TH1I* pTH1I_Tempo  = GiveMe_pTH1I_LeadingiY(aSample,ModuleNber, "Tempo") ;   
  int iY_Beam = pTH1I_Tempo->GetMaximumBin() - 1 ;
  delete pTH1I_Tempo ;

//--------------------------------------------------------------------
  std::string OutputFilesName = "AnalyseReconstruction_" + TAG ;
  std::string TAG_Analysis = "AnalyseReconstruction_" + AnalyseName + TAG ;

  SetOfTracks* pSetOfTracks = aSample.Get_SetOfTracks_ForThisModule(ModuleNber) ;

  pSetOfTracks->Set_MinimalNberOfEntries(MinimalNberOfEntries) ; 
   
//Drawout few strange tracks  
  int iT_Max = pSetOfTracks->Get_NberOfTrack();
  int Kounter_TrackDrawOut     = 0 ;
  int Kounter_TrackDrawOut_Max = 10 ; 
  for (int iT = 0 ; iT< iT_Max; iT++){
    const Track* pTrack = pSetOfTracks->Get_Track (iT);
    double Ch2Min = pTrack->Get_Chi2Min() ;
    if (Ch2Min>2000. && Kounter_TrackDrawOut <= Kounter_TrackDrawOut_Max ) {
      Kounter_TrackDrawOut += 1 ;
      pTrack->DrawOut(OUTDirName);
    }
  }

//
  std::ostringstream aostringstream_TFileName ;
  aostringstream_TFileName << std::setiosflags(std::ios::fixed) ;
  aostringstream_TFileName << OUTDirName << OutputFilesName ;
  aostringstream_TFileName << ".root" ;
  std::string TFileName = aostringstream_TFileName.str() ; 

  TFile*  pTFile = new TFile(TFileName.c_str(),"RECREATE") ;

//Redirect Output
  std::ostringstream aostringstream_OutPut_Analysis ;
  aostringstream_OutPut_Analysis << std::setiosflags(std::ios::fixed) ;
  aostringstream_OutPut_Analysis << OUTDirName << "DumpOfTracks.txt" ;
  
  std::string OutPut_Analysis = aostringstream_OutPut_Analysis.str() ; 
  
  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( OutPut_Analysis.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file

    pSetOfTracks->DumpRec() ;
    
  std::cout.rdbuf(coutbuf); // Reset to standard output again

//
  const  Model_ReadOutGeometry* pModel_ReadOutGeometry =  aSample.Get_Model_ReadOutGeometry() ;
  int    Nx       = pModel_ReadOutGeometry->Get_Nx()       ;
  int    Ny       = pModel_ReadOutGeometry->Get_Ny()       ;
  double LY       = pModel_ReadOutGeometry->Get_LY()       ;

  double Vert_Ymin = 0. ;
  double Vert_Ymax = 0. ;

//
  TCanvas* pTCanvas =  0 ;
  gStyle->SetOptFit(1011);

  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  aostringstream << OUTDirName <<OutputFilesName ;
  aostringstream << ".pdf"  ;
  std::string Str_Out_PDF     = aostringstream.str() ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

// 
  double Ymin = 1.E2* pModel_ReadOutGeometry->Get_YLPad(0,   0,ModuleNber);
  double Ymax = 1.E2* pModel_ReadOutGeometry->Get_YHPad(0,Ny-1,ModuleNber);
  int Nbins = Ny*15;

  std::vector< TH1F*  > V_pTH1F_YTrackInTracks      ;
  std::vector< TH1F*  > V_pTH1F_YTrackInTracksLARGE ;

  std::vector< TH1F*  > V_pTH1F_YFitCol ;
  std::vector< TH1F*  > V_pTH1F_YFitColLARGE ;

  std::vector< TH1F*  > V_pTH1F_YTrackYPadInTracks ;
  
  std::vector< TH1F*  > V_pTH1F_YTrackYPadLeadingInTracks ;
  
  std::vector< TH1F*  > V_pTH1F_TrackDeltaT  ;
  
  for (int iCol = 0 ; iCol< Nx; iCol++){
    TH1F* pTH1F_YFitCol      = pSetOfTracks->GiveMe_pTH1F_YFitCol ( TAG_Analysis,iY_Beam        ,iCol);
    TH1F* pTH1F_YFitColLARGE = pSetOfTracks->GiveMe_pTH1F_YFitCol ( TAG_Analysis,Nbins,Ymin,Ymax,iCol);
    V_pTH1F_YFitCol     .push_back( pTH1F_YFitCol      ) ;
    V_pTH1F_YFitColLARGE.push_back( pTH1F_YFitColLARGE ) ;
    
    TH1F* pTH1F_TrackDeltaT = pSetOfTracks->GiveMe_pTH1F_TrackDeltaT ( TAG_Analysis, iCol );
    V_pTH1F_TrackDeltaT .push_back( pTH1F_TrackDeltaT       ) ;
    
    TH1F* pTH1F_YTrackInTracks      = pSetOfTracks->GiveMe_pTH1F_YTrackInTracks ( TAG_Analysis,iY_Beam        ,iCol);
    TH1F* pTH1F_YTrackInTracksLARGE = pSetOfTracks->GiveMe_pTH1F_YTrackInTracks ( TAG_Analysis,Nbins,Ymin,Ymax,iCol);
    V_pTH1F_YTrackInTracks     .push_back( pTH1F_YTrackInTracks      ) ;
    V_pTH1F_YTrackInTracksLARGE.push_back( pTH1F_YTrackInTracksLARGE ) ;
    
    TH1F* pTH1F_YTrackYPadInTracks      = pSetOfTracks->GiveMe_pTH1F_YTrackYPadInTracks ( TAG_Analysis,iCol,1200, -3*LY *1.E2 , 3*LY *1.E2) ;
    V_pTH1F_YTrackYPadInTracks.push_back( pTH1F_YTrackYPadInTracks      ) ;

    TH1F* pTH1F_YTrackYPadLeadingInTracks      = pSetOfTracks->GiveMe_pTH1F_YTrackYPadLeadingInTracks ( TAG_Analysis,iCol,400, -LY *1.E2 , LY *1.E2) ;
    V_pTH1F_YTrackYPadLeadingInTracks.push_back( pTH1F_YTrackYPadLeadingInTracks      ) ;
  }

  TH1F* Final_pTH1F_YFitCol_AllCol        = pSetOfTracks->GiveMe_pTH1F_YFitCol( TAG_Analysis,iY_Beam         , -1 );
  TH1F* Final_pTH1F_YFitCol_AllColLARGE   = pSetOfTracks->GiveMe_pTH1F_YFitCol( TAG_Analysis,Nbins,Ymin,Ymax , -1 );

  TH1F* Final_pTH1F_TrackDeltaTAllCol  = pSetOfTracks->GiveMe_pTH1F_TrackDeltaT ( TAG_Analysis, -1 );

  TH1F* Final_pTH1F_YTrackInTracks_AllCol       = pSetOfTracks->GiveMe_pTH1F_YTrackInTracks ( TAG_Analysis,iY_Beam         , -1 );
  TH1F* Final_pTH1F_YTrackInTracks_AllColLARGE  = pSetOfTracks->GiveMe_pTH1F_YTrackInTracks ( TAG_Analysis,Nbins,Ymin,Ymax , -1 );

  TH1F* Final_TH1F_YTrackYPadInTracksAllCol  = pSetOfTracks->GiveMe_pTH1F_YTrackYPadInTracks ( TAG_Analysis , -1,1200, -3*LY *1.E2 , 3*LY *1.E2) ;
  TH1F* Final_TH1F_YTrackYPadLeadingInTracksAllCol  = pSetOfTracks->GiveMe_pTH1F_YTrackYPadLeadingInTracks ( TAG_Analysis , -1,400, -LY *1.E2 , LY *1.E2) ;

//
  TH1F* Final_pTH1F_Ch2MinPerNODF_5  = pSetOfTracks->GiveMe_pTH1F_Ch2MinPerNODF ( TAG_Analysis, 5.);
  TH1F* Final_pTH1F_Ch2MinPerNODF_10 = pSetOfTracks->GiveMe_pTH1F_Ch2MinPerNODF ( TAG_Analysis,10.);
  TH1F* Final_pTH1F_Ch2MinPerNODF_20 = pSetOfTracks->GiveMe_pTH1F_Ch2MinPerNODF ( TAG_Analysis,20.);
  
  
  TH1F* Final_pTH1F_Ch2Min_200   = pSetOfTracks->GiveMe_pTH1F_Ch2Min (
                                                                TAG_Analysis,
                                                                200.
                                                               );
  TH1F* Final_pTH1F_Ch2Min_400  = pSetOfTracks->GiveMe_pTH1F_Ch2Min (
                                                                TAG_Analysis,
                                                                400.
                                                               );
  TH1F* Final_pTH1F_Ch2Min_800 = pSetOfTracks->GiveMe_pTH1F_Ch2Min (
                                                                TAG_Analysis,
                                                                800.
                                                               );

//  
  TH1F* Final_pTH1F_Pull_ToStore = pSetOfTracks->GiveMe_pTH1F_Pull (
                                                                  TAG_Analysis   
                                                                  );
  double All_Pull_Mean   = 0. ;
  double All_Pull_eMean  = 0. ;
  double All_Pull_Sigma  = 0. ;
  double All_Pull_eSigma = 0. ;
  TH1F* Final_pTH1F_Pull        = pSetOfTracks->GiveMe_pTH1F_Pull (
                                                                  TAG_Analysis        ,
                                                                  All_Pull_Mean   ,
                                                                  All_Pull_eMean  ,
                                                                  All_Pull_Sigma  ,
                                                                  All_Pull_eSigma  
                                                                 );
  double Final_pTH1F_Pull_XminH = -5.* All_Pull_Sigma ;
  double Final_pTH1F_Pull_XmaxH =  5.* All_Pull_Sigma ;
  delete Final_pTH1F_Pull ;
       Final_pTH1F_Pull        = pSetOfTracks->GiveMe_pTH1F_Pull (
                                                                  TAG_Analysis        ,
                                                                  All_Pull_Mean   ,
                                                                  All_Pull_eMean  ,
                                                                  All_Pull_Sigma  ,
                                                                  All_Pull_eSigma ,
                                                                  Final_pTH1F_Pull_XminH ,
                                                                  Final_pTH1F_Pull_XmaxH 
                                                                 );
  
  double All_Mean   = 0. ;
  double All_eMean  = 0. ;
  double All_Sigma  = 0. ;
  double All_eSigma = 0. ;
  TH1F* Final_pTH1F_Residual    = pSetOfTracks->GiveMe_pTH1F_Residual (
                                                                      TAG_Analysis   ,
                                                                      All_Mean   ,
                                                                      All_eMean  ,
                                                                      All_Sigma  ,
                                                                      All_eSigma  
                                                                     );
  
  TGraphErrors* pTGraphErrors_Residual_Mean_VS_Col  = new TGraphErrors ;
  TGraphErrors* pTGraphErrors_Residual_Sigma_VS_Col = new TGraphErrors ;
  pTGraphErrors_Residual_Mean_VS_Col ->SetMarkerStyle(8) ;
  pTGraphErrors_Residual_Sigma_VS_Col->SetMarkerStyle(8) ;
  std::vector < TH1F*  > V_Col_pTH1F  ;  
  
  TGraphErrors* pTGraphErrors_Residual_Pull_Mean_VS_Col  = new TGraphErrors ;
  TGraphErrors* pTGraphErrors_Residual_Pull_Sigma_VS_Col = new TGraphErrors ;
  pTGraphErrors_Residual_Pull_Mean_VS_Col ->SetMarkerStyle(8) ;
  pTGraphErrors_Residual_Pull_Sigma_VS_Col->SetMarkerStyle(8) ;
  std::vector < TH1F*  > V_Col_Pull_pTH1F  ;  
  
  for (int iCol = 0 ; iCol< Nx; iCol++){
    int  TheCol = iCol ;
    double Col_Mean   = 0. ;
    double Col_eMean  = 0. ;
    double Col_Sigma  = 0. ;
    double Col_eSigma = 0. ;
    TH1F* pTH1 = pSetOfTracks->GiveMe_pTH1F_Residual (
                                                     TAG_Analysis   ,
                                                     TheCol     ,
                                                     Col_Mean   ,
                                                     Col_eMean  ,
                                                     Col_Sigma  ,
                                                     Col_eSigma  
                                                    );
    V_Col_pTH1F.push_back(pTH1) ;
    if (pTH1) {
      int IptCur_Mean  = pTGraphErrors_Residual_Mean_VS_Col ->GetN() ;
      pTGraphErrors_Residual_Mean_VS_Col ->SetPoint     ( IptCur_Mean , double( iCol ) , Col_Mean    *1.E3) ;
      pTGraphErrors_Residual_Mean_VS_Col ->SetPointError( IptCur_Mean , 0.             , Col_eMean   *1.E3) ;

      int IptCur_Sigma = pTGraphErrors_Residual_Sigma_VS_Col->GetN() ;
      pTGraphErrors_Residual_Sigma_VS_Col->SetPoint     ( IptCur_Sigma, double( iCol ) , Col_Sigma   *1.E3) ;
      pTGraphErrors_Residual_Sigma_VS_Col->SetPointError( IptCur_Sigma, 0.             , Col_eSigma  *1.E3) ;

    }
    
    double Col_Pull_Mean   = 0. ;
    double Col_Pull_eMean  = 0. ;
    double Col_Pull_Sigma  = 0. ;
    double Col_Pull_eSigma = 0. ;
    TH1F* pTH1_Pull = pSetOfTracks->GiveMe_pTH1F_Pull (
                                                 TAG_Analysis        ,
                                                 TheCol     ,
                                                 Col_Pull_Mean   ,
                                                 Col_Pull_eMean  ,
                                                 Col_Pull_Sigma  ,
                                                 Col_Pull_eSigma ,
                                                 Final_pTH1F_Pull_XminH ,
                                                 Final_pTH1F_Pull_XmaxH 
                                                );
    V_Col_Pull_pTH1F.push_back(pTH1_Pull) ;
    if (pTH1_Pull) {
      int IptCur_Pull_Mean  = pTGraphErrors_Residual_Pull_Mean_VS_Col ->GetN() ;
      pTGraphErrors_Residual_Pull_Mean_VS_Col ->SetPoint      ( IptCur_Pull_Mean , double( iCol ) , Col_Pull_Mean   ) ;
      pTGraphErrors_Residual_Pull_Mean_VS_Col ->SetPointError ( IptCur_Pull_Mean , 0.             , Col_Pull_eMean  ) ;

      int IptCur_Pull_Sigma = pTGraphErrors_Residual_Pull_Sigma_VS_Col->GetN() ;
      pTGraphErrors_Residual_Pull_Sigma_VS_Col->SetPoint      ( IptCur_Pull_Sigma, double( iCol ) , Col_Pull_Sigma  ) ;
      pTGraphErrors_Residual_Pull_Sigma_VS_Col->SetPointError ( IptCur_Pull_Sigma, 0.             , Col_Pull_eSigma ) ;

    } 
  }
    
//--------------------------------------------------------------------
//  Graphics Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      pTCanvas->cd(1)->SetLogy() ;
      Final_pTH1F_Ch2Min_200->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 2 );
      pTCanvas->cd(2)->SetLogy() ;
      Final_pTH1F_Ch2Min_400->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 3 );
      pTCanvas->cd(3)->SetLogy() ;
      Final_pTH1F_Ch2Min_800->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 4 );
      pTCanvas->cd(4)->SetLogy() ;
      Final_pTH1F_Ch2MinPerNODF_5->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 5 );
      pTCanvas->cd(5)->SetLogy() ;
      Final_pTH1F_Ch2MinPerNODF_10->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 6 );
      pTCanvas->cd(6)->SetLogy() ;
      Final_pTH1F_Ch2MinPerNODF_20->Draw();
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF_Beg.c_str());


    
//--------------------------------------------------------------------
//  Graphics Page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      Final_pTH1F_YFitCol_AllCol->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 4 );
      Final_pTH1F_YFitCol_AllColLARGE->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 2 );
      V_pTH1F_YFitCol[1]->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 5 );
      V_pTH1F_YFitColLARGE[1]->Draw();
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//--------------------------------------------------------------------
//  Graphics Page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      Final_pTH1F_YTrackInTracks_AllCol->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 4 );
      Final_pTH1F_YTrackInTracks_AllColLARGE->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 2 );
      V_pTH1F_YTrackInTracks[1]->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 5 );
      V_pTH1F_YTrackInTracksLARGE[1]->Draw();
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//--------------------------------------------------------------------
//  Graphics Page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      Final_pTH1F_TrackDeltaTAllCol->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 2 );
      V_pTH1F_TrackDeltaT[1]->Draw();
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//--------------------------------------------------------------------
//  Graphics Page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 1 );
      Final_TH1F_YTrackYPadInTracksAllCol->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 2 );
      V_pTH1F_YTrackYPadInTracks[1]->Draw();
      pTCanvas->Update();

       pTCanvas->cd( 4 );
      Final_TH1F_YTrackYPadLeadingInTracksAllCol->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 5 );
      V_pTH1F_YTrackYPadLeadingInTracks[1]->Draw();
      pTCanvas->Update();

 pTCanvas->Print(Str_Out_PDF.c_str());


//--------------------------------------------------------------------
//  Graphics Page 6
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    pTCanvas->Divide(3,2); 

      pTCanvas->cd( 2 );
      Final_pTH1F_Residual->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 3 );
      Final_pTH1F_Pull->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 5 );
      pTCanvas->cd(5)->SetLogy() ;
      Final_pTH1F_Residual->Draw();
      pTCanvas->Update();

      pTCanvas->cd( 6 );
      pTCanvas->cd( 6 )->SetLogy() ;
      Final_pTH1F_Pull->Draw();
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//Plots VS Y 
  int iOpt_DoDBG_Plots_VS_X = 1 ;
  if (iOpt_DoDBG_Plots_VS_X==1){
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      pTCanvas->Divide(4,9); 

      for (int iCol = 0 ; iCol< Nx; iCol++){
        pTCanvas->cd( iCol + 1 );
        if (V_Col_pTH1F[iCol]) V_Col_pTH1F[iCol]->Draw();
      }

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
  
//
//--------------------------------------------------------------------
//  Graphics Page 7
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,2); 
      
      pTCanvas->cd( 1 );
      
      TMultiGraph* pTMultiGraph_Residual_Sigma_VS_Col = new TMultiGraph();      
      pTMultiGraph_Residual_Sigma_VS_Col->Add( pTGraphErrors_Residual_Sigma_VS_Col , "p" );
      pTMultiGraph_Residual_Sigma_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Residual_Sigma_VS_Col->GetYaxis()->SetTitle("Sigma Residual (#mu m)");
      pTMultiGraph_Residual_Sigma_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Residual_Sigma_VS_Col->GetYaxis()->SetTitleOffset(1.5);
//       pTMultiGraph_Residual_Sigma_VS_Col->SetMinimum(  0.);
//       pTMultiGraph_Residual_Sigma_VS_Col->SetMaximum(220.);

      pTMultiGraph_Residual_Sigma_VS_Col->Draw("A");
      pTCanvas->Update();
    
      pTCanvas->cd( 2 );
      
      TMultiGraph* pTMultiGraph_Residual_Mean_VS_Col = new TMultiGraph();      
      pTMultiGraph_Residual_Mean_VS_Col->Add( pTGraphErrors_Residual_Mean_VS_Col , "p" );
      pTMultiGraph_Residual_Mean_VS_Col->GetXaxis()->SetTitle("Column number");
      pTMultiGraph_Residual_Mean_VS_Col->GetYaxis()->SetTitle("Mean Residual (#mu m)");
      pTMultiGraph_Residual_Mean_VS_Col->GetXaxis()->SetLimits(-1.,37.);
      pTMultiGraph_Residual_Mean_VS_Col->GetYaxis()->SetTitleOffset(1.5);
//       pTMultiGraph_Residual_Mean_VS_Col->SetMinimum(-90.);
//       pTMultiGraph_Residual_Mean_VS_Col->SetMaximum( 90.);

      pTMultiGraph_Residual_Mean_VS_Col->Draw("A");
      pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//
//Pull VS X
  int iOpt_DoDBG_Pulls_VS_X = 1 ;
  if (iOpt_DoDBG_Pulls_VS_X==1){

    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      pTCanvas->Divide(4,9); 

      for (int iCol = 0 ; iCol< Nx; iCol++){
        pTCanvas->cd( iCol + 1 );
        if (V_Col_Pull_pTH1F[iCol]) V_Col_Pull_pTH1F[iCol]->Draw();
      }

    pTCanvas->Print(Str_Out_PDF.c_str());

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
      pTCanvas->Divide(1,2); 
        
        pTCanvas->cd( 1 );
        
        TMultiGraph* pTMultiGraph_Residual_Pull_Sigma_VS_Col = new TMultiGraph();      
        pTMultiGraph_Residual_Pull_Sigma_VS_Col->Add( pTGraphErrors_Residual_Pull_Sigma_VS_Col , "p" );
        pTMultiGraph_Residual_Pull_Sigma_VS_Col->GetXaxis()->SetTitle("Column number");
        pTMultiGraph_Residual_Pull_Sigma_VS_Col->GetYaxis()->SetTitle("Pull Sigma");
        pTMultiGraph_Residual_Pull_Sigma_VS_Col->GetXaxis()->SetLimits(-1.,37.);
        pTMultiGraph_Residual_Pull_Sigma_VS_Col->GetYaxis()->SetTitleOffset(1.5);

        pTMultiGraph_Residual_Pull_Sigma_VS_Col->Draw("A");
        pTCanvas->Update();
      
        pTCanvas->cd( 2 );
        
        TMultiGraph* pTMultiGraph_Residual_Pull_Mean_VS_Col = new TMultiGraph();      
        pTMultiGraph_Residual_Pull_Mean_VS_Col->Add( pTGraphErrors_Residual_Pull_Mean_VS_Col , "p" );
        pTMultiGraph_Residual_Pull_Mean_VS_Col->GetXaxis()->SetTitle("Column number");
        pTMultiGraph_Residual_Pull_Mean_VS_Col->GetYaxis()->SetTitle("Pull Mean");
        pTMultiGraph_Residual_Pull_Mean_VS_Col->GetXaxis()->SetLimits(-1.,37.);
        pTMultiGraph_Residual_Pull_Mean_VS_Col->GetYaxis()->SetTitleOffset(1.5);

        pTMultiGraph_Residual_Pull_Mean_VS_Col->Draw("A");
        pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());
  }
  
//Plots VS Y 
  std::cout << std::endl ;
  double Step_Y = LY/double(NberofPointAlongY);
  double Ylow_Beam = pModel_ReadOutGeometry->Get_YLPad(0,iY_Beam+1,ModuleNber)               ;
  double Ylow      = pModel_ReadOutGeometry->Get_YLPad(0,iY_Beam-1,ModuleNber) - 5. * Step_Y ;
  double YHigh     = pModel_ReadOutGeometry->Get_YLPad(0,iY_Beam+2,ModuleNber) + 5. * Step_Y ;
  std::cout << " iY_Beam  =          " << std::setw(20)                          << iY_Beam           << std::endl ;
  std::cout << " YHigh =               " << std::setw(20) << std::setprecision(10) << YHigh               << std::endl ;
  std::cout << " Ylow  =               " << std::setw(20) << std::setprecision(10) << Ylow                << std::endl ;
  std::cout << " YHigh-Ylow_Beam =     " << std::setw(20) << std::setprecision(10) << YHigh-Ylow_Beam     << std::endl ;
  std::cout << " Ylow -Ylow_Beam =     " << std::setw(20) << std::setprecision(10) << Ylow -Ylow_Beam     << std::endl ;
  std::cout << " YHigh-Ylow =          " << std::setw(20) << std::setprecision(10) << YHigh-Ylow          << std::endl ;
  std::cout << " (YHigh-Ylow)/Step_Y = " << std::setw(20) << std::setprecision(10) << (YHigh-Ylow)/Step_Y << std::endl ;
  std::cout << std::endl ;

  int iBinY_Max = std::round( (YHigh-Ylow)/Step_Y ) ;
  std::cout << " iBinY_Max= " << std::setw(20) << std::setprecision(10) << iBinY_Max << std::endl ;
  
  std::vector< TGraphErrors* > V_pTGraphErrors_Mean  ;
  std::vector< TGraphErrors* > V_pTGraphErrors_Sigma ;
  std::vector< std::vector < TH1F* > > V_V_pTH1F ;
  for (int iCol = 0 ; iCol< Nx; iCol++){
    int  TheCol = iCol ;
    TGraphErrors* pTGraphErrors_Mean  = new TGraphErrors ;
    TGraphErrors* pTGraphErrors_Sigma = new TGraphErrors ;
    pTGraphErrors_Mean ->SetMarkerStyle(8) ;
    pTGraphErrors_Sigma->SetMarkerStyle(8) ;
    std::vector < TH1F* > V_pTH1F ;
    for (int iBinY = 0 ; iBinY< iBinY_Max; iBinY++){
      double YL = Ylow +  double(iBinY     ) * Step_Y ;
      double YH = Ylow +  double(iBinY + 1 ) * Step_Y ;
      double Col_Mean   = 0. ;
      double Col_eMean  = 0. ;
      double Col_Sigma  = 0. ;
      double Col_eSigma = 0. ;
      TH1F* pTH1 = pSetOfTracks->GiveMe_pTH1F_Residual (
                                                     TAG_Analysis   ,
                                                     TheCol     ,
                                                     YL         ,
                                                     YH         ,
                                                     (YL -Ylow_Beam )*1E2 ,
                                                     (YH -Ylow_Beam )*1E2 ,
                                                     Col_Mean   ,
                                                     Col_eMean  ,
                                                     Col_Sigma  ,
                                                     Col_eSigma  
                                                    );
      V_pTH1F.push_back( pTH1 ) ;
      if (pTH1){
        int IptCur_Mean  = pTGraphErrors_Mean ->GetN() ;
        pTGraphErrors_Mean ->SetPoint        ( IptCur_Mean , ( (YL+YH)/2. -Ylow_Beam )*1E2 , Col_Mean    *1.E3) ;
        pTGraphErrors_Mean ->SetPointError   ( IptCur_Mean , 0.                            , Col_eMean   *1.E3) ;

        int IptCur_Sigma = pTGraphErrors_Sigma->GetN() ;
        pTGraphErrors_Sigma->SetPoint        ( IptCur_Sigma, ( (YL+YH)/2. -Ylow_Beam )*1E2 , Col_Sigma   *1.E3) ;
        pTGraphErrors_Sigma->SetPointError   ( IptCur_Sigma, 0.                            , Col_eSigma  *1.E3) ;

      }
      
      
//       if (iCol==7){
//         std::cout 
//           << "iCol "  << std::setw(6) << iCol
//           << "iBinY " << std::setw(6) << iBinY
//           << " "     << std::setw(8) << std::setprecision(3) << Col_Mean   *1.E3
//           << " +/- " << std::setw(8) << std::setprecision(3) << Col_eMean  *1.E3
//           << " "     << std::setw(8) << std::setprecision(3) << Col_Sigma  *1.E3
//           << " +/- " << std::setw(8) << std::setprecision(3) << Col_eSigma *1.E3
//           << std::endl ;
//       }
      
    }
    V_V_pTH1F.push_back( V_pTH1F ) ;
    
    V_pTGraphErrors_Mean .push_back( pTGraphErrors_Mean  ) ;
    V_pTGraphErrors_Sigma.push_back( pTGraphErrors_Sigma ) ;
  }
  
//
  int iOpt_DoDBG_Plots_VS_Y = 1 ;
  if (iOpt_DoDBG_Plots_VS_Y==1){
    for (int iColDBG = 0 ; iColDBG< Nx; iColDBG++){
//for (int iColDBG = 0 ; iColDBG< 1; iColDBG++){
//
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
        pTCanvas->Divide(5,8); 

        for (int iBinY = 0 ; iBinY< iBinY_Max; iBinY++){
          pTCanvas->cd( iBinY + 1 );
          if ((V_V_pTH1F[iColDBG])[iBinY]) (V_V_pTH1F[iColDBG])[iBinY]->Draw();
        }

      pTCanvas->Print(Str_Out_PDF.c_str());
//
      pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
        pTCanvas->Divide(1,2); 
        
          pTCanvas->cd( 1 );
          
            TMultiGraph* pTMultiGraph_DBG_V_pTGraphErrors_Sigma = new TMultiGraph(); 
            
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->Add( V_pTGraphErrors_Sigma[iColDBG] , "p" );

            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->GetXaxis()->SetTitle("Y (cm)");
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->GetYaxis()->SetTitle("Sigma Residual (#mu m)");
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->GetYaxis()->SetTitleOffset(1.5);
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->SetMinimum(  0.);
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->SetMaximum(500.);
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->GetXaxis()->SetLimits((Ylow -Ylow_Beam)*1.E2,(YHigh -Ylow_Beam)*1.E2);
         
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->Draw("A");

            std::ostringstream aostringstream_V_pTGraphErrors_Sigma ;
            aostringstream_V_pTGraphErrors_Sigma << std::setiosflags(std::ios::fixed) ;
            aostringstream_V_pTGraphErrors_Sigma << " iCol = " << iColDBG ;
            pTMultiGraph_DBG_V_pTGraphErrors_Sigma->GetHistogram()->SetTitle( (aostringstream_V_pTGraphErrors_Sigma.str()).c_str() );

            pTCanvas->Update();
        
            Vert_Ymin = (pTCanvas->cd( 1 ))->GetUymin() ;
            Vert_Ymax = (pTCanvas->cd( 1 ))->GetUymax() ;
            for (int iV = iY_Beam-1 ; iV< iY_Beam+3; iV++){
              double YCur =  pModel_ReadOutGeometry->Get_YLPad(0,iV,ModuleNber) ;
              YCur = YCur - Ylow_Beam ;
              TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
              pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); 
              pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
              pTCanvas->Update()    ;
            }
            
          pTCanvas->cd( 2 );
          
          TMultiGraph* pTMultiGraph_DBG_V_pTGraphErrors_Mean = new TMultiGraph(); 
          
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->Add( V_pTGraphErrors_Mean[iColDBG] , "p" );

            pTMultiGraph_DBG_V_pTGraphErrors_Mean->GetXaxis()->SetTitle("Y (m)");
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->GetYaxis()->SetTitle("Mean Residual (#mu m)");
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->GetYaxis()->SetTitleOffset(1.5);
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->SetMinimum(-800.);
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->SetMaximum( 800.);
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->GetXaxis()->SetLimits((Ylow -Ylow_Beam)*1.E2,(YHigh -Ylow_Beam)*1.E2);
         
            pTMultiGraph_DBG_V_pTGraphErrors_Mean->Draw("A");
            pTCanvas->Update();

            Vert_Ymin = (pTCanvas->cd( 2 ))->GetUymin() ;
            Vert_Ymax = (pTCanvas->cd( 2 ))->GetUymax() ;
            for (int iV = iY_Beam-1 ; iV< iY_Beam+3; iV++){
              double YCur =   pModel_ReadOutGeometry->Get_YLPad(0,iV,ModuleNber) ;
              YCur = YCur - Ylow_Beam ;
              TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
              pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); 
              pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
              pTCanvas->Update()    ;
            }
          
      pTCanvas->Print(Str_Out_PDF.c_str());
    }
  }
  
//--------------------------------------------------------------------
//  Graphics Page 8
  TLegend* pTLegend_V_pTGraphErrors_Sigma = new TLegend(0.35,0.55 ,0.65,0.79  ); 
  pTLegend_V_pTGraphErrors_Sigma->SetFillStyle(1001);
  pTLegend_V_pTGraphErrors_Sigma->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_V_pTGraphErrors_Sigma = new TMultiGraph(); 
 
  TLegend* pTLegend_V_pTGraphErrors_Mean = new TLegend(0.35,0.55 ,0.65,0.79  ); 
  pTLegend_V_pTGraphErrors_Mean->SetFillStyle(1001);
  pTLegend_V_pTGraphErrors_Mean->SetBorderSize(1);

  TMultiGraph* pTMultiGraph_V_pTGraphErrors_Mean = new TMultiGraph(); 
 
  int Kount_iCol = 0. ;
  for (int iCol = 0 ; iCol< Nx; iCol++){
    if (iCol !=  5
    &&  iCol != 10
    &&  iCol != 15
    &&  iCol != 20
    &&  iCol != 25
    ) continue ;
    
    Kount_iCol += 1 ;
    if (Kount_iCol==1) V_pTGraphErrors_Sigma[iCol]->SetMarkerColor(2) ;
    if (Kount_iCol==2) V_pTGraphErrors_Sigma[iCol]->SetMarkerColor(4) ;
    if (Kount_iCol==3) V_pTGraphErrors_Sigma[iCol]->SetMarkerColor(2) ;
    if (Kount_iCol==4) V_pTGraphErrors_Sigma[iCol]->SetMarkerColor(4) ;
    if (Kount_iCol==5) V_pTGraphErrors_Sigma[iCol]->SetMarkerColor(2) ;
    
    if (Kount_iCol==1) V_pTGraphErrors_Sigma[iCol]->SetMarkerStyle(20) ;
    if (Kount_iCol==2) V_pTGraphErrors_Sigma[iCol]->SetMarkerStyle(20) ;
    if (Kount_iCol==3) V_pTGraphErrors_Sigma[iCol]->SetMarkerStyle(21) ;
    if (Kount_iCol==4) V_pTGraphErrors_Sigma[iCol]->SetMarkerStyle(21) ;
    if (Kount_iCol==5) V_pTGraphErrors_Sigma[iCol]->SetMarkerStyle(22) ;
    
    if (Kount_iCol==1) V_pTGraphErrors_Mean[iCol]->SetMarkerColor(2) ;
    if (Kount_iCol==2) V_pTGraphErrors_Mean[iCol]->SetMarkerColor(4) ;
    if (Kount_iCol==3) V_pTGraphErrors_Mean[iCol]->SetMarkerColor(2) ;
    if (Kount_iCol==4) V_pTGraphErrors_Mean[iCol]->SetMarkerColor(4) ;
    if (Kount_iCol==5) V_pTGraphErrors_Mean[iCol]->SetMarkerColor(2) ;
    
    if (Kount_iCol==1) V_pTGraphErrors_Mean[iCol]->SetMarkerStyle(20) ;
    if (Kount_iCol==2) V_pTGraphErrors_Mean[iCol]->SetMarkerStyle(20) ;
    if (Kount_iCol==3) V_pTGraphErrors_Mean[iCol]->SetMarkerStyle(21) ;
    if (Kount_iCol==4) V_pTGraphErrors_Mean[iCol]->SetMarkerStyle(21) ;
    if (Kount_iCol==5) V_pTGraphErrors_Mean[iCol]->SetMarkerStyle(22) ;
    
    pTMultiGraph_V_pTGraphErrors_Sigma->Add( V_pTGraphErrors_Sigma[iCol] , "p" );
    std::ostringstream aostringstream_V_pTGraphErrors_Sigma ;
    aostringstream_V_pTGraphErrors_Sigma << std::setiosflags(std::ios::fixed) ;
    aostringstream_V_pTGraphErrors_Sigma << " iCol = " << iCol ;
    pTLegend_V_pTGraphErrors_Sigma->AddEntry(V_pTGraphErrors_Sigma[iCol],(aostringstream_V_pTGraphErrors_Sigma.str()).c_str() ) ;

    pTMultiGraph_V_pTGraphErrors_Mean->Add( V_pTGraphErrors_Mean[iCol] , "p" );
    std::ostringstream aostringstream_V_pTGraphErrors_Mean ;
    aostringstream_V_pTGraphErrors_Mean << std::setiosflags(std::ios::fixed) ;
    aostringstream_V_pTGraphErrors_Mean << " iCol = " << iCol ;
    pTLegend_V_pTGraphErrors_Mean->AddEntry(V_pTGraphErrors_Mean[iCol],(aostringstream_V_pTGraphErrors_Mean.str()).c_str() ) ;
        
  }

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,2); 
    
      pTCanvas->cd( 1 );
      
      pTMultiGraph_V_pTGraphErrors_Sigma->GetXaxis()->SetTitle("Y (cm)");
      pTMultiGraph_V_pTGraphErrors_Sigma->GetYaxis()->SetTitle("Sigma Residual (#mu m)");
      pTMultiGraph_V_pTGraphErrors_Sigma->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_V_pTGraphErrors_Sigma->SetMinimum(  0.);
      pTMultiGraph_V_pTGraphErrors_Sigma->SetMaximum(500.);
      pTMultiGraph_V_pTGraphErrors_Sigma->GetXaxis()->SetLimits((Ylow -Ylow_Beam)*1.E2,(YHigh -Ylow_Beam)*1.E2);

      pTMultiGraph_V_pTGraphErrors_Sigma->Draw("A");
      pTCanvas->Update();
    
      Vert_Ymin = (pTCanvas->cd( 1 ))->GetUymin() ;
      Vert_Ymax = (pTCanvas->cd( 1 ))->GetUymax() ;
      for (int iV = iY_Beam-1 ; iV< iY_Beam+3; iV++){
        double YCur =   pModel_ReadOutGeometry->Get_YLPad(0,iV,ModuleNber) ;
        YCur = YCur - Ylow_Beam ;
        TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
        pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); 
        pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
        pTCanvas->Update()    ;
      }
      
//    pTLegend_V_pTGraphErrors_Sigma->Draw("A");
      pTCanvas->Update();
    
      pTCanvas->cd( 2 );
      
      pTMultiGraph_V_pTGraphErrors_Mean->GetXaxis()->SetTitle("Y (m)");
      pTMultiGraph_V_pTGraphErrors_Mean->GetYaxis()->SetTitle("Mean Residual (#mu m)");
      pTMultiGraph_V_pTGraphErrors_Mean->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_V_pTGraphErrors_Mean->SetMinimum(-800.);
      pTMultiGraph_V_pTGraphErrors_Mean->SetMaximum( 800.);
      pTMultiGraph_V_pTGraphErrors_Mean->GetXaxis()->SetLimits((Ylow -Ylow_Beam)*1.E2,(YHigh -Ylow_Beam)*1.E2);
 
      pTMultiGraph_V_pTGraphErrors_Mean->Draw("A");
      pTCanvas->Update();

      Vert_Ymin = (pTCanvas->cd( 2 ))->GetUymin() ;
      Vert_Ymax = (pTCanvas->cd( 2 ))->GetUymax() ;
      for (int iV = iY_Beam-1 ; iV< iY_Beam+3; iV++){
        double YCur =   pModel_ReadOutGeometry->Get_YLPad(0,iV,ModuleNber) ;
        YCur = YCur - Ylow_Beam ;
        TLine* pTLineVertical_Max = new TLine( YCur*1E2  , Vert_Ymin , YCur*1E2 , Vert_Ymax ) ;
        pTLineVertical_Max->SetLineStyle(10); pTLineVertical_Max->SetLineWidth(1); 
        pTLineVertical_Max->SetLineColor(2) ; pTLineVertical_Max->Draw();  
        pTCanvas->Update()    ;
      }
      
//    pTLegend_V_pTGraphErrors_Mean->Draw("A");
      pTCanvas->Update();
      
  pTCanvas->Print(Str_Out_PDF.c_str());

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

//
  Final_pTH1F_YTrackInTracks_AllCol       ->Write("Final_pTH1F_YTrackInTracks_AllCol"       ) ;
  Final_pTH1F_YTrackInTracks_AllColLARGE  ->Write("Final_pTH1F_YTrackInTracks_AllColLARGE"  ) ;
 
  Final_TH1F_YTrackYPadInTracksAllCol  ->Write("Final_TH1F_YTrackYPadInTracksAllCol"  ) ;
  Final_TH1F_YTrackYPadLeadingInTracksAllCol  ->Write("Final_TH1F_YTrackYPadLeadingInTracksAllCol"  ) ;
 
  Final_pTH1F_TrackDeltaTAllCol  ->Write("Final_pTH1F_TrackDeltaTAllCol"  ) ;
 
  Final_pTH1F_YFitCol_AllCol       ->Write("Final_pTH1F_YFitCol_AllCol"       ) ;
  Final_pTH1F_YFitCol_AllColLARGE  ->Write("Final_pTH1F_YFitCol_AllColLARGE"  ) ;

// 
  Final_pTH1F_Ch2MinPerNODF_5  ->Write("Final_pTH1F_Ch2MinPerNODF_5"  ) ;
 
  Final_pTH1F_Ch2MinPerNODF_5  ->Write("Final_pTH1F_Ch2MinPerNODF_5"  ) ;
  Final_pTH1F_Ch2MinPerNODF_10 ->Write("Final_pTH1F_Ch2MinPerNODF_10" ) ;
  Final_pTH1F_Ch2MinPerNODF_20 ->Write("Final_pTH1F_Ch2MinPerNODF_20" ) ;

  Final_pTH1F_Ch2Min_200               ->Write("Final_pTH1F_Ch2Min_200" ) ;
  Final_pTH1F_Ch2Min_400               ->Write("Final_pTH1F_Ch2Min_400" ) ;
  Final_pTH1F_Ch2Min_800               ->Write("Final_pTH1F_Ch2Min_800" ) ;
 
//
  Final_pTH1F_Residual                ->Write("Final_pTH1F_Residual" ) ;
  Final_pTH1F_Pull_ToStore              ->Write("Final_pTH1F_Pull" ) ;
 
  pTGraphErrors_Residual_Sigma_VS_Col ->Write("pTGraphErrors_Residual_Sigma_VS_Col" ) ;
  pTGraphErrors_Residual_Mean_VS_Col  ->Write("pTGraphErrors_Residual_Mean_VS_Col" ) ;
  
  pTGraphErrors_Residual_Pull_Sigma_VS_Col ->Write("pTGraphErrors_Residual_Pull_Sigma_VS_Col" ) ;
  pTGraphErrors_Residual_Pull_Mean_VS_Col  ->Write("pTGraphErrors_Residual_Pull_Mean_VS_Col" ) ;

  int iTem_V_pTGraphErrors_Sigma_Max = V_pTGraphErrors_Sigma.size() ;
  for (int iTem_V_pTGraphErrors_Sigma = 0 ; iTem_V_pTGraphErrors_Sigma< iTem_V_pTGraphErrors_Sigma_Max; iTem_V_pTGraphErrors_Sigma++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTGraphErrors_Sigma_" << iTem_V_pTGraphErrors_Sigma ;
    V_pTGraphErrors_Sigma[iTem_V_pTGraphErrors_Sigma] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTGraphErrors_Mean_Max = V_pTGraphErrors_Mean.size() ;
  for (int iTem_V_pTGraphErrors_Mean = 0 ; iTem_V_pTGraphErrors_Mean< iTem_V_pTGraphErrors_Mean_Max; iTem_V_pTGraphErrors_Mean++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTGraphErrors_Mean_" << iTem_V_pTGraphErrors_Mean ;
    V_pTGraphErrors_Mean[iTem_V_pTGraphErrors_Mean] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YTrackInTracks_Max = V_pTH1F_YTrackInTracks.size() ;
  for (int iTem_V_pTH1F_YTrackInTracks = 0 ; iTem_V_pTH1F_YTrackInTracks< iTem_V_pTH1F_YTrackInTracks_Max; iTem_V_pTH1F_YTrackInTracks++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackInTracks_" << iTem_V_pTH1F_YTrackInTracks ;
    V_pTH1F_YTrackInTracks[iTem_V_pTH1F_YTrackInTracks] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YTrackInTracksLARGE_Max = V_pTH1F_YTrackInTracksLARGE.size() ;
  for (int iTem_V_pTH1F_YTrackInTracksLARGE = 0 ; iTem_V_pTH1F_YTrackInTracksLARGE< iTem_V_pTH1F_YTrackInTracksLARGE_Max; iTem_V_pTH1F_YTrackInTracksLARGE++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackInTracksLARGE_" << iTem_V_pTH1F_YTrackInTracksLARGE ;
    V_pTH1F_YTrackInTracksLARGE[iTem_V_pTH1F_YTrackInTracksLARGE] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YFitCol_Max = V_pTH1F_YFitCol.size() ;
  for (int iTem_V_pTH1F_YFitCol = 0 ; iTem_V_pTH1F_YFitCol< iTem_V_pTH1F_YFitCol_Max; iTem_V_pTH1F_YFitCol++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YFitCol_" << iTem_V_pTH1F_YFitCol ;
    V_pTH1F_YFitCol[iTem_V_pTH1F_YFitCol] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YFitColLARGE_Max = V_pTH1F_YFitColLARGE.size() ;
  for (int iTem_V_pTH1F_YFitColLARGE = 0 ; iTem_V_pTH1F_YFitColLARGE< iTem_V_pTH1F_YFitColLARGE_Max; iTem_V_pTH1F_YFitColLARGE++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YFitColLARGE_" << iTem_V_pTH1F_YFitColLARGE ;
    V_pTH1F_YFitColLARGE[iTem_V_pTH1F_YFitColLARGE] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YTrackYPadInTracks_Max = V_pTH1F_YTrackYPadInTracks.size() ;
  for (int iTem_V_pTH1F_YTrackYPadInTracks = 0 ; iTem_V_pTH1F_YTrackYPadInTracks< iTem_V_pTH1F_YTrackYPadInTracks_Max; iTem_V_pTH1F_YTrackYPadInTracks++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackYPadInTracks_" << iTem_V_pTH1F_YTrackYPadInTracks ;
    V_pTH1F_YTrackYPadInTracks[iTem_V_pTH1F_YTrackYPadInTracks] ->Write( (aostringstream.str()).c_str() ) ;
  }
  
  int iTem_V_pTH1F_YTrackYPadLeadingInTracks_Max = V_pTH1F_YTrackYPadLeadingInTracks.size() ;
  for (int iTem_V_pTH1F_YTrackYPadLeadingInTracks = 0 ; iTem_V_pTH1F_YTrackYPadLeadingInTracks< iTem_V_pTH1F_YTrackYPadLeadingInTracks_Max; iTem_V_pTH1F_YTrackYPadLeadingInTracks++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_YTrackYPadLeadingInTracks_" << iTem_V_pTH1F_YTrackYPadLeadingInTracks ;
    V_pTH1F_YTrackYPadLeadingInTracks[iTem_V_pTH1F_YTrackYPadLeadingInTracks] ->Write( (aostringstream.str()).c_str() ) ;
  }

  int iTem_V_pTH1F_TrackDeltaT_Max = V_pTH1F_TrackDeltaT.size() ;
  for (int iTem_V_pTH1F_TrackDeltaT = 0 ; iTem_V_pTH1F_TrackDeltaT< iTem_V_pTH1F_TrackDeltaT_Max; iTem_V_pTH1F_TrackDeltaT++){
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "V_pTH1F_TrackDeltaT_" << iTem_V_pTH1F_TrackDeltaT ;
    V_pTH1F_TrackDeltaT[iTem_V_pTH1F_TrackDeltaT] ->Write( (aostringstream.str()).c_str() ) ;
  }

  
  delete Final_pTH1F_Pull ;
  
  int iTem_V_Col_Pull_pTH1F_Max = V_Col_Pull_pTH1F.size() ;
  for (int iTem_V_Col_Pull_pTH1F = 0 ; iTem_V_Col_Pull_pTH1F< iTem_V_Col_Pull_pTH1F_Max; iTem_V_Col_Pull_pTH1F++){
    delete V_Col_Pull_pTH1F[iTem_V_Col_Pull_pTH1F] ;
    V_Col_Pull_pTH1F[iTem_V_Col_Pull_pTH1F] = 0 ;
  }

//  delete Final_pTH1F_Residual ;
  
  int iTem_V_Col_pTH1F_Max = V_Col_pTH1F.size() ;
  for (int iTem_V_Col_pTH1F = 0 ; iTem_V_Col_pTH1F< iTem_V_Col_pTH1F_Max; iTem_V_Col_pTH1F++){
    delete V_Col_pTH1F[iTem_V_Col_pTH1F] ;
    V_Col_pTH1F[iTem_V_Col_pTH1F] = 0 ;
  }
  
//delete pTGraphErrors_Residual_Mean_VS_Col  ;
//delete pTGraphErrors_Residual_Sigma_VS_Col ;
  
//   int iTem_V_pTGraphErrors_Sigma_Max = V_pTGraphErrors_Sigma.size() ;
//   for (int iTem_V_pTGraphErrors_Sigma = 0 ; iTem_V_pTGraphErrors_Sigma< iTem_V_pTGraphErrors_Sigma_Max; iTem_V_pTGraphErrors_Sigma++){
//     delete V_pTGraphErrors_Sigma[iTem_V_pTGraphErrors_Sigma] ;
//     V_pTGraphErrors_Sigma[iTem_V_pTGraphErrors_Sigma] = 0 ;
//   }
//   
//   int iTem_V_pTGraphErrors_Mean_Max = V_pTGraphErrors_Mean.size() ;
//   for (int iTem_V_pTGraphErrors_Mean = 0 ; iTem_V_pTGraphErrors_Mean< iTem_V_pTGraphErrors_Mean_Max; iTem_V_pTGraphErrors_Mean++){
//     delete V_pTGraphErrors_Mean[iTem_V_pTGraphErrors_Mean] ;
//     V_pTGraphErrors_Mean[iTem_V_pTGraphErrors_Mean] = 0 ;
//   }
  
  int iTem_V_V_pTH1F_Max = V_V_pTH1F.size() ;
  for (int iTem_V_V_pTH1F = 0 ; iTem_V_V_pTH1F< iTem_V_V_pTH1F_Max; iTem_V_V_pTH1F++){
    int iTem_V_pTH1F_Max = V_V_pTH1F[iTem_V_V_pTH1F].size() ;
    for (int iTem_V_pTH1F = 0 ; iTem_V_pTH1F< iTem_V_pTH1F_Max; iTem_V_pTH1F++){
      delete (V_V_pTH1F[iTem_V_V_pTH1F])[iTem_V_pTH1F] ;
      (V_V_pTH1F[iTem_V_V_pTH1F])[iTem_V_pTH1F] = 0 ;
    }
  }
  
//
  pTFile->Close() ;
  delete pTFile ;

}
