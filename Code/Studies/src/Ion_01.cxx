#include "Studies/Ion_01.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge0D.h"
#include "SignalModel/Model_Electronics.h"
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

void Ion_01()
{
  
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Ion_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge0D*         pModel_Charge0D        = new Model_Charge0D        ;

  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ;  
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

//
  double RC_True =  60. * 1E-9 / ( 1E-3 * 1E-3) ;
  double RC_Eff  = 120. * 1E-9 / ( 1E-3 * 1E-3) ;
  RC_True = RC_Eff/1.7 ;
  double RC_Test  = 100. * 1E-9 / ( 1E-3 * 1E-3) ;
  double T_ion    = 400.E-9 ; 
    
//
  pModel_Charge0D->Set_Width(0.);
  
  double Qprim = 200.*1.602176565E-19  ;
  pModel_Charge0D->Set_Qprim(Qprim) ;
  pModel_Charge0D->Set_Gain( 1500. ) ;

  pModel_Charge0D->Set_RC( RC_Eff ) ;

//  
  ROBoard aROBoard(
                   pModel_ReadOutGeometry  ,
                   pModel_Electronics      ,
                   pModel_Charge0D
                  );
                  
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  for (int iX = 0 ; iX< Nx; iX++){
    for (int iY = 0 ; iY< Ny; iY++){
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << "( " << iX << " , " << iY << " )" ;
       std::string PadName = aostringstream.str() ; 
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge0D , PadName , EventNber,EntryNber,ModuleNber ,iX ,iY); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//
  int iXc =  Nx/2 ;
  int iYc =  Ny/2 ;
  Pad* pPad_Center = aROBoard.Get_Pad(iXc,iYc) ;
  
  double Time0 = 0. ;
  
  double Xc = pPad_Center->Get_XPad() ;
  double Yc = pPad_Center->Get_YPad() ;
  double XH = pPad_Center->Get_XH() ;
  double YH = pPad_Center->Get_YH() ;
  
  
  std::cout << " iXc " << std::setw(3) << iXc << std::endl ;
  std::cout << " iYc " << std::setw(3) << iYc << std::endl ;
  
  std::cout << "  Xc (mm)  " << std::setw(10) << std::setprecision(3) << Xc   *1.E3 << std::endl ;
  std::cout << "  Yc (mm)  " << std::setw(10) << std::setprecision(3) << Yc   *1.E3 << std::endl ;

//
  pModel_Charge0D->Set_RC( RC_Eff ) ;
  
//
  std::vector< std::vector< TGraph* >*  > V_V_pTGraph_Q ;
  std::vector< std::vector< TGraph* >*  > V_V_pTGraph_A ;
  
  

  for (int iOpt = 0 ; iOpt< 3 ; iOpt++){
  
    double XTrue = Xc + 2.E-3 ;
    double YTrue = Yc + 2.E-3 ;

    if (iOpt==1){
      XTrue = Xc + 3.1E-3 ;
      YTrue = Yc + 2.9E-3 ;
      RC_Test  = 200. * 1E-9 / ( 1E-3 * 1E-3) ;
    }
    
    int NberOfPads = aROBoard.Get_NberOfPads() ;
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,XTrue,YTrue ); 
    }

    std::vector< TGraph* >* p_V_pTGraph_Q = new std::vector< TGraph* >  ;
    std::vector< TGraph* >* p_V_pTGraph_A = new std::vector< TGraph* >  ;
    
    V_V_pTGraph_Q.push_back(p_V_pTGraph_Q);
    V_V_pTGraph_A.push_back(p_V_pTGraph_A);

    if (iOpt==0){
     pModel_Charge0D->Set_RC( RC_Eff ) ;
    }
    
    if (iOpt==1){
     pModel_Charge0D->Set_RC( RC_Test ) ;
    }
    
    if (iOpt==2){
      pModel_Charge0D->Set_Ion(
                               1  ,
                               T_ion ,
                               RC_True   ,
                               RC_Eff/RC_True  
                              ) ;
    }
    
    for (int iDY = 0 ; iDY< 3 ; iDY++){
    for (int iDX = 0 ; iDX< 3 ; iDX++){
      int iDY_Local = 2 - iDY ;
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY_Local ;
      
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      
      TGraph* pTGraph_Q_Pad = new TGraph ;
      TGraph* pTGraph_A_Pad = new TGraph ;
      
      p_V_pTGraph_Q->push_back( pTGraph_Q_Pad );
      p_V_pTGraph_A->push_back( pTGraph_A_Pad );

      double Tmin =     0.        ;
      double Tmax =  2000. * 1E-9 ; 
      int    Npoints = 1000 ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
        double Q_Pad = pPad_Cur->Get_Qpad(TimeCur) ;
        double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
        pTGraph_Q_Pad ->SetPoint( pTGraph_Q_Pad ->GetN(), TimeCur*1.E6 , Q_Pad /1.602176565E-19  ) ;
        pTGraph_A_Pad ->SetPoint( pTGraph_Q_Pad ->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      }
    }
    }
    
  
  }
  
//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(3,3) ;

    TLegend* pTLegend_Q = new TLegend(0.35,0.73 ,0.885,0.88  ); 
    pTLegend_Q->SetFillStyle(1001);
    pTLegend_Q->SetBorderSize(1);

    for (int iC = 0 ; iC< 9 ; iC++){
      pTCanvas->cd(iC+1 ) ;
      TMultiGraph* pTMultiGraph = new TMultiGraph();
      
      std::vector< TGraph* >* p_V_pTGraph_Q_0 = V_V_pTGraph_Q[0] ;
      std::vector< TGraph* >* p_V_pTGraph_Q_1 = V_V_pTGraph_Q[1] ;
      std::vector< TGraph* >* p_V_pTGraph_Q_2 = V_V_pTGraph_Q[2] ;
      
      TGraph* pTGraph_Q_0 =  (*p_V_pTGraph_Q_0)[iC] ;
      TGraph* pTGraph_Q_1 =  (*p_V_pTGraph_Q_1)[iC] ;
      TGraph* pTGraph_Q_2 =  (*p_V_pTGraph_Q_2)[iC] ;

      pTGraph_Q_0 ->SetLineColor( 1 ) ;
      pTGraph_Q_1 ->SetLineColor( 4 ) ;
      pTGraph_Q_2 ->SetLineColor( 2 ) ;
      
      pTGraph_Q_1 ->SetLineStyle( 2 ) ;
      
      pTMultiGraph ->Add( pTGraph_Q_0     ,"l");
      pTMultiGraph ->Add( pTGraph_Q_1     ,"l");
      pTMultiGraph ->Add( pTGraph_Q_2     ,"l");
      
      
      pTMultiGraph->Draw("A");
      pTCanvas->Update();
      
      if (iC==4){
        std::ostringstream aostringstream_0 ;
        aostringstream_0 << std::setiosflags(std::ios::fixed) ;
        aostringstream_0 << "1 RC: RC=R_{eff}" << std::setw(5) << std::setprecision(0) << RC_Eff/(1E-9 / ( 1E-3 * 1E-3));

        std::ostringstream aostringstream_1 ;
        aostringstream_1 << std::setiosflags(std::ios::fixed) ;
        aostringstream_1 << "1 RC: RC=" << std::setw(5) << std::setprecision(0) << RC_Test/(1E-9 / ( 1E-3 * 1E-3));

        std::ostringstream aostringstream_2 ;
        aostringstream_2 << std::setiosflags(std::ios::fixed) ;
        aostringstream_2 << "2 RCs:" ;
        aostringstream_2 << " RC_{eff}/R_{True}=" << std::setw(4) << std::setprecision(1) << RC_Eff/RC_True;
        aostringstream_2 << ", T=" << std::setw(5) << std::setprecision(0) << T_ion/(1E-9);

        pTLegend_Q->AddEntry( pTGraph_Q_0  ,(aostringstream_0.str()).c_str()  , "l");
        pTLegend_Q->AddEntry( pTGraph_Q_1  ,(aostringstream_1.str()).c_str()  , "l"); 
        pTLegend_Q->AddEntry( pTGraph_Q_2  ,(aostringstream_2.str()).c_str()  , "l"); 
        pTLegend_Q->Draw();
        pTCanvas->Update();
      }
      
    }

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(3,3) ;

    TLegend* pTLegend_A = new TLegend(0.35,0.73 ,0.885,0.88  ); 
    pTLegend_A->SetFillStyle(1001);
    pTLegend_A->SetBorderSize(1);

    for (int iC = 0 ; iC< 9 ; iC++){
      pTCanvas->cd(iC+1 ) ;
      TMultiGraph* pTMultiGraph = new TMultiGraph();
      
      std::vector< TGraph* >* p_V_pTGraph_A_0 = V_V_pTGraph_A[0] ;
      std::vector< TGraph* >* p_V_pTGraph_A_1 = V_V_pTGraph_A[1] ;
      std::vector< TGraph* >* p_V_pTGraph_A_2 = V_V_pTGraph_A[2] ;
      
      TGraph* pTGraph_A_0 =  (*p_V_pTGraph_A_0)[iC] ;
      TGraph* pTGraph_A_1 =  (*p_V_pTGraph_A_1)[iC] ;
      TGraph* pTGraph_A_2 =  (*p_V_pTGraph_A_2)[iC] ;
      
      pTGraph_A_0 ->SetLineColor( 1 ) ;
      pTGraph_A_1 ->SetLineColor( 4 ) ;
      pTGraph_A_2 ->SetLineColor( 2 ) ;
      
      pTGraph_A_1 ->SetLineStyle( 2 ) ;

      pTMultiGraph ->Add( pTGraph_A_0     ,"l");
      pTMultiGraph ->Add( pTGraph_A_1     ,"l");
      pTMultiGraph ->Add( pTGraph_A_2     ,"l");
      
      pTMultiGraph->Draw("A");
      pTCanvas->Update();
      
//    pTMultiGraph->SetMaximum( 1700 );
      pTCanvas->Update();

      if (iC==4){
        std::ostringstream aostringstream_0 ;
        aostringstream_0 << std::setiosflags(std::ios::fixed) ;
        aostringstream_0 << "1 RC: RC=R_{eff}" << std::setw(5) << std::setprecision(0) << RC_Eff/(1E-9 / ( 1E-3 * 1E-3));

        std::ostringstream aostringstream_1 ;
        aostringstream_1 << std::setiosflags(std::ios::fixed) ;
        aostringstream_1 << "1 RC: RC=" << std::setw(5) << std::setprecision(0) << RC_Test/(1E-9 / ( 1E-3 * 1E-3));

        std::ostringstream aostringstream_2 ;
        aostringstream_2 << std::setiosflags(std::ios::fixed) ;
        aostringstream_2 << "2 RCs:" ;
        aostringstream_2 << " RC_{eff}/R_{True}=" << std::setw(4) << std::setprecision(1) << RC_Eff/RC_True;
        aostringstream_2 << ", T=" << std::setw(5) << std::setprecision(0) << T_ion/(1E-9);

        pTLegend_A->AddEntry( pTGraph_A_0  ,(aostringstream_0.str()).c_str()  , "l");
        pTLegend_A->AddEntry( pTGraph_A_1  ,(aostringstream_1.str()).c_str()  , "l"); 
        pTLegend_A->AddEntry( pTGraph_A_2  ,(aostringstream_2.str()).c_str()  , "l"); 
        pTLegend_A->Draw();
        pTCanvas->Update();
      }
      
    }

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2) ;

    int iC_eff = 0 ;
    for (int iC = 0 ; iC< 9 ; iC++){
      if ( (iC+1) == 1
      ||   (iC+1) == 4
      ||   (iC+1) == 7
      ||   (iC+1) == 8
      ||   (iC+1) == 9
      ) continue ;
      iC_eff += 1 ;
      pTCanvas->cd(iC_eff) ;
      TMultiGraph* pTMultiGraph = new TMultiGraph();
      
      std::vector< TGraph* >* p_V_pTGraph_Q_0 = V_V_pTGraph_Q[0] ;
      std::vector< TGraph* >* p_V_pTGraph_Q_1 = V_V_pTGraph_Q[1] ;
      std::vector< TGraph* >* p_V_pTGraph_Q_2 = V_V_pTGraph_Q[2] ;
      
      TGraph* pTGraph_Q_0 =  (*p_V_pTGraph_Q_0)[iC] ;
      TGraph* pTGraph_Q_1 =  (*p_V_pTGraph_Q_1)[iC] ;
      TGraph* pTGraph_Q_2 =  (*p_V_pTGraph_Q_2)[iC] ;

      pTGraph_Q_0 ->SetLineColor( 1 ) ;
      pTGraph_Q_1 ->SetLineColor( 4 ) ;
      pTGraph_Q_2 ->SetLineColor( 2 ) ;
      
      pTGraph_Q_1 ->SetLineStyle( 2 ) ;
      
      pTMultiGraph ->Add( pTGraph_Q_0     ,"l");
      pTMultiGraph ->Add( pTGraph_Q_1     ,"l");
      pTMultiGraph ->Add( pTGraph_Q_2     ,"l");
      
      
      pTMultiGraph->Draw("A");
      pTCanvas->Update();
      
      if (iC==4){
        pTLegend_Q->Draw();
        pTCanvas->Update();
      }
      
    }

  pTCanvas->Print(Str_Out_PDF.c_str());
    
//page 5
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(2,2) ;

    iC_eff = 0 ;
    for (int iC = 0 ; iC< 9 ; iC++){
      if ( (iC+1) == 1
      ||   (iC+1) == 4
      ||   (iC+1) == 7
      ||   (iC+1) == 8
      ||   (iC+1) == 9
      ) continue ;
      iC_eff += 1 ;
      pTCanvas->cd(iC_eff) ;
      TMultiGraph* pTMultiGraph = new TMultiGraph();
      
      std::vector< TGraph* >* p_V_pTGraph_A_0 = V_V_pTGraph_A[0] ;
      std::vector< TGraph* >* p_V_pTGraph_A_1 = V_V_pTGraph_A[1] ;
      std::vector< TGraph* >* p_V_pTGraph_A_2 = V_V_pTGraph_A[2] ;
      
      TGraph* pTGraph_A_0 =  (*p_V_pTGraph_A_0)[iC] ;
      TGraph* pTGraph_A_1 =  (*p_V_pTGraph_A_1)[iC] ;
      TGraph* pTGraph_A_2 =  (*p_V_pTGraph_A_2)[iC] ;
      
      pTGraph_A_0 ->SetLineColor( 1 ) ;
      pTGraph_A_1 ->SetLineColor( 4 ) ;
      pTGraph_A_2 ->SetLineColor( 2 ) ;
      
      pTGraph_A_1 ->SetLineStyle( 2 ) ;

      pTMultiGraph ->Add( pTGraph_A_0     ,"l");
      pTMultiGraph ->Add( pTGraph_A_1     ,"l");
      pTMultiGraph ->Add( pTGraph_A_2     ,"l");
      
      pTMultiGraph->Draw("A");
      pTCanvas->Update();
      
//    pTMultiGraph->SetMaximum( 1700 );
      pTCanvas->Update();

      if (iC==4){
        pTLegend_A->Draw();
        pTCanvas->Update();
      }
      
    }

  pTCanvas->Print(Str_Out_PDF.c_str());
    
    
//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
