#include "Studies/Gain_02.h"

#include "Misc/SetT2KStyle.h"

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
#include "TROOT.h"

void Gain_02()
{
  TString localStyleName = "T2K";
  // -- WhichStyle --
  // 1 = presentation large fonts
  // 2 = presentation small fonts
  // 3 = publication/paper
  Int_t localWhichStyle = 1;
  TStyle* t2kstyle = SetT2KStyle(localWhichStyle, localStyleName);
  gROOT->SetStyle(t2kstyle->GetName());
  
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Gain_02.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge0D*         pModel_Charge0D        = new Model_Charge0D        ;

  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ;  
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

//
  pModel_Charge0D->Set_Width(0.);
  
  double Qprim = 100.*1.602176565E-19  ;
  pModel_Charge0D->Set_Qprim(Qprim) ;
  pModel_Charge0D->Set_Gain( 1000. ) ;

  pModel_Charge0D->Set_RC( 50. * 1E-9 / ( 1E-3 * 1E-3) ) ;

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
  pModel_Charge0D->Set_RC( 50. * 1E-9 / ( 1E-3 * 1E-3) ) ;
  
  double XTrue = Xc + 1.E-3 ;
  double YTrue = Yc + 1.E-3 ;
//   XTrue = Xc   ;
//   YTrue = Yc   ;
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,XTrue,YTrue ); 
  }

//
  TMultiGraph* pTMultiGraph_Q_OffCenter     = new TMultiGraph();
  TMultiGraph* pTMultiGraph_A_OffCenter     = new TMultiGraph();
  
  std::vector< TGraph* > V_pTGraph_Q_OffCenter ;
  std::vector< TGraph* > V_pTGraph_A_OffCenter ;

  for (int iDY = 0 ; iDY< 3 ; iDY++){
  for (int iDX = 0 ; iDX< 3 ; iDX++){
    int iDY_Local = 2 - iDY ;
    int iX = iXc - 1 + iDX ;
    int iY = iYc - 1 + iDY_Local ;
    
    Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
    
    TGraph* pTGraph_Q_Pad = new TGraph ;
    TGraph* pTGraph_A_Pad = new TGraph ;
    
    pTMultiGraph_Q_OffCenter ->Add( pTGraph_Q_Pad     ,"l");
    pTMultiGraph_A_OffCenter ->Add( pTGraph_A_Pad     ,"l");
    
    V_pTGraph_Q_OffCenter.push_back( pTGraph_Q_Pad );
    V_pTGraph_A_OffCenter.push_back( pTGraph_A_Pad );

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
  
  TGraph* pTGraph_SUM_Q_OffCenter = new TGraph ;
  TGraph* pTGraph_SUM_A_OffCenter = new TGraph ;

  pTGraph_SUM_Q_OffCenter->SetLineColor( 1 ) ;
  pTGraph_SUM_A_OffCenter->SetLineColor( 1 ) ;

  pTGraph_SUM_Q_OffCenter->SetLineStyle( 2 ) ;
  pTGraph_SUM_A_OffCenter->SetLineStyle( 2 ) ;

  pTMultiGraph_Q_OffCenter ->Add( pTGraph_SUM_Q_OffCenter ,"l");
  pTMultiGraph_A_OffCenter ->Add( pTGraph_SUM_A_OffCenter ,"l");
  
  int iTgraph_Max = V_pTGraph_Q_OffCenter.size();
  int iPoints_Max = V_pTGraph_Q_OffCenter[0] ->GetN();
  for (int iPoints = 0 ; iPoints< iPoints_Max ; iPoints++){
    double TimeCur = (V_pTGraph_Q_OffCenter[0]->GetX())[iPoints];
    double SUM_Q = 0. ;
    double SUM_A = 0. ;
    for (int iTgraph = 0 ; iTgraph< iTgraph_Max ; iTgraph++){
      SUM_Q += (V_pTGraph_Q_OffCenter[iTgraph]->GetY())[iPoints] ;
      SUM_A += (V_pTGraph_A_OffCenter[iTgraph]->GetY())[iPoints] ;
    }
    pTGraph_SUM_Q_OffCenter ->SetPoint( pTGraph_SUM_Q_OffCenter ->GetN() , TimeCur  , SUM_Q ) ;
    pTGraph_SUM_A_OffCenter ->SetPoint( pTGraph_SUM_A_OffCenter ->GetN() , TimeCur  , SUM_A ) ;
  }
  
  TLegend* pTLegend_Q_OffCenter = new TLegend(0.55,0.55 ,0.85,0.70  ); 
  pTLegend_Q_OffCenter->SetFillStyle(1001);
  pTLegend_Q_OffCenter->SetBorderSize(1);

  pTLegend_Q_OffCenter->AddEntry( pTGraph_SUM_Q_OffCenter  ,"Charges Sum"  , "l");
  pTLegend_Q_OffCenter->AddEntry( V_pTGraph_Q_OffCenter[0] ,"Pad Charges" , "l");
  
  TLegend* pTLegend_A_OffCenter = new TLegend(0.55,0.52 ,0.80,0.70  ); 
  pTLegend_A_OffCenter->SetFillStyle(1001);
  pTLegend_A_OffCenter->SetBorderSize(1);

  pTLegend_A_OffCenter->AddEntry( pTGraph_SUM_A_OffCenter  ,"ADC Sum"  , "l");
  pTLegend_A_OffCenter->AddEntry( V_pTGraph_A_OffCenter[0] ,"Pad ADCs" , "l");

//
  TMultiGraph* pTMultiGraph_Q_VarRC     = new TMultiGraph();
  TMultiGraph* pTMultiGraph_A_VarRC     = new TMultiGraph();

  TLegend* pTLegend_Q_VarRC = new TLegend(0.55,0.52 ,0.80,0.70  ); 
  pTLegend_Q_VarRC->SetFillStyle(1001);
  pTLegend_Q_VarRC->SetBorderSize(1);
  
  TLegend* pTLegend_A_VarRC = new TLegend(0.53,0.38 ,0.85,0.70  ); 
  pTLegend_A_VarRC->SetFillStyle(1001);
  pTLegend_A_VarRC->SetBorderSize(1);

  for (int iRC = 0 ; iRC< 4 ; iRC++){
    double      RCcur = 0. ;
    std::string RCcur_Str = "BIDON" ;
    if (iRC==0) { RCcur =  50. ; RCcur_Str = "RC =  50 ns mm^{-2} " ; }
    if (iRC==1) { RCcur =  75. ; RCcur_Str = "RC =  75 ns mm^{-2} " ; }
    if (iRC==2) { RCcur = 100. ; RCcur_Str = "RC = 100 ns mm^{-2} " ; }
    if (iRC==3) { RCcur = 200. ; RCcur_Str = "RC = 200 ns mm^{-2} " ; }
    pModel_Charge0D->Set_RC( RCcur * 1E-9 / ( 1E-3 * 1E-3) ) ;
    
    double XTrue = Xc + 1.E-3 ;
    double YTrue = Yc + 1.E-3 ;
    int NberOfPads = aROBoard.Get_NberOfPads() ;
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,XTrue,YTrue ); 
    }
    
    std::vector< TGraph* > V_pTGraph_Q_VarRC ;
    std::vector< TGraph* > V_pTGraph_A_VarRC ;

    for (int iDY = 0 ; iDY< 3 ; iDY++){
    for (int iDX = 0 ; iDX< 3 ; iDX++){
      int iDY_Local = 2 - iDY ;
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY_Local ;
      
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      
      TGraph* pTGraph_Q_Pad = new TGraph ;
      TGraph* pTGraph_A_Pad = new TGraph ;
      
      V_pTGraph_Q_VarRC.push_back( pTGraph_Q_Pad );
      V_pTGraph_A_VarRC.push_back( pTGraph_A_Pad );

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
    
    TGraph* pTGraph_SUM_Q_VarRC = new TGraph ;
    TGraph* pTGraph_SUM_A_VarRC = new TGraph ;

    if (iRC==0) pTGraph_SUM_Q_VarRC->SetLineColor( 2 ) ;
    if (iRC==1) pTGraph_SUM_Q_VarRC->SetLineColor( 3 ) ;
    if (iRC==2) pTGraph_SUM_Q_VarRC->SetLineColor( 4 ) ;
    if (iRC==3) pTGraph_SUM_Q_VarRC->SetLineColor( 6 ) ;
    
    if (iRC==0) pTGraph_SUM_A_VarRC->SetLineColor( 2 ) ;
    if (iRC==1) pTGraph_SUM_A_VarRC->SetLineColor( 3 ) ;
    if (iRC==2) pTGraph_SUM_A_VarRC->SetLineColor( 4 ) ;
    if (iRC==3) pTGraph_SUM_A_VarRC->SetLineColor( 6 ) ;
    
    pTMultiGraph_Q_VarRC ->Add( pTGraph_SUM_Q_VarRC ,"l");
    pTMultiGraph_A_VarRC ->Add( pTGraph_SUM_A_VarRC ,"l");
    
    pTLegend_Q_VarRC->AddEntry( pTGraph_SUM_Q_VarRC  ,RCcur_Str.c_str()  , "l");
    pTLegend_A_VarRC->AddEntry( pTGraph_SUM_A_VarRC  ,RCcur_Str.c_str() , "l");

    int iTgraph_Max = V_pTGraph_Q_VarRC.size();
    int iPoints_Max = V_pTGraph_Q_VarRC[0] ->GetN();
    for (int iPoints = 0 ; iPoints< iPoints_Max ; iPoints++){
      double TimeCur = (V_pTGraph_Q_VarRC[0]->GetX())[iPoints];
      double SUM_Q = 0. ;
      double SUM_A = 0. ;
      for (int iTgraph = 0 ; iTgraph< iTgraph_Max ; iTgraph++){
        SUM_Q += (V_pTGraph_Q_VarRC[iTgraph]->GetY())[iPoints] ;
        SUM_A += (V_pTGraph_A_VarRC[iTgraph]->GetY())[iPoints] ;
      }
      pTGraph_SUM_Q_VarRC ->SetPoint( pTGraph_SUM_Q_VarRC ->GetN() , TimeCur  , SUM_Q ) ;
      pTGraph_SUM_A_VarRC ->SetPoint( pTGraph_SUM_A_VarRC ->GetN() , TimeCur  , SUM_A ) ;
    }

  }

//
  TMultiGraph* pTMultiGraph_Q_VarTrue     = new TMultiGraph();
  TMultiGraph* pTMultiGraph_A_VarTrue     = new TMultiGraph();

  TLegend* pTLegend_Q_VarTrue = new TLegend(0.55,0.15 ,0.80,0.65  ); 
  pTLegend_Q_VarTrue->SetFillStyle(1001);
  pTLegend_Q_VarTrue->SetBorderSize(1);
  pTLegend_Q_VarTrue->SetMargin(0.4) ;
  
  TLegend* pTLegend_A_VarTrue = new TLegend(0.61,0.27 ,0.84,0.88  ); 
  pTLegend_A_VarTrue->SetFillStyle(1001);
  pTLegend_A_VarTrue->SetBorderSize(1);
  pTLegend_A_VarTrue->SetMargin(0.35) ;

  for (int iTrue = 0 ; iTrue< 6 ; iTrue++){
    pModel_Charge0D->Set_RC( 50. * 1E-9 / ( 1E-3 * 1E-3) ) ;
  
    double XTrue = 0. ;
    double YTrue = 0. ;
    std::string Pos_Str = "BIDON" ;
    if (iTrue==0) { XTrue = Xc + 0.    ; YTrue = Yc + 0.     ; Pos_Str = "#Delta X = #Delta Y = 0   " ; }
    if (iTrue==1) { XTrue = Xc + 1.E-3 ; YTrue = Yc + 1.E-3  ; Pos_Str = "#Delta X = #Delta Y = 1 mm" ; }
    if (iTrue==2) { XTrue = Xc + 2.E-3 ; YTrue = Yc + 2.E-3  ; Pos_Str = "#Delta X = #Delta Y = 2 mm" ; }
    if (iTrue==3) { XTrue = Xc + 3.E-3 ; YTrue = Yc + 3.E-3  ; Pos_Str = "#Delta X = #Delta Y = 3 mm" ; }
    if (iTrue==4) { XTrue = Xc + 4.E-3 ; YTrue = Yc + 4.E-3  ; Pos_Str = "#Delta X = #Delta Y = 4 mm" ; }
    if (iTrue==5) { XTrue = Xc + 5.E-3 ; YTrue = Yc + 5.E-3  ; Pos_Str = "#Delta X = #Delta Y = 5 mm" ; }
    int NberOfPads = aROBoard.Get_NberOfPads() ;
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,XTrue,YTrue ); 
    }
    
    std::vector< TGraph* > V_pTGraph_Q_VarTrue ;
    std::vector< TGraph* > V_pTGraph_A_VarTrue ;

    for (int iDY = 0 ; iDY< 3 ; iDY++){
    for (int iDX = 0 ; iDX< 3 ; iDX++){
      int iDY_Local = 2 - iDY ;
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY_Local ;
      
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      
      TGraph* pTGraph_Q_Pad = new TGraph ;
      TGraph* pTGraph_A_Pad = new TGraph ;
      
      V_pTGraph_Q_VarTrue.push_back( pTGraph_Q_Pad );
      V_pTGraph_A_VarTrue.push_back( pTGraph_A_Pad );

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
    
    TGraph* pTGraph_SUM_Q_VarTrue = new TGraph ;
    TGraph* pTGraph_SUM_A_VarTrue = new TGraph ;

    if (iTrue==0) { pTGraph_SUM_Q_VarTrue->SetLineColor( 2 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==1) { pTGraph_SUM_Q_VarTrue->SetLineColor( 3 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==2) { pTGraph_SUM_Q_VarTrue->SetLineColor( 4 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==3) { pTGraph_SUM_Q_VarTrue->SetLineColor( 2 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 9 ) ;}
    if (iTrue==4) { pTGraph_SUM_Q_VarTrue->SetLineColor( 3 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 9 ) ;}
    if (iTrue==5) { pTGraph_SUM_Q_VarTrue->SetLineColor( 4 ) ;  pTGraph_SUM_Q_VarTrue->SetLineStyle( 9 ) ;}
    
    if (iTrue==0) { pTGraph_SUM_A_VarTrue->SetLineColor( 2 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==1) { pTGraph_SUM_A_VarTrue->SetLineColor( 3 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==2) { pTGraph_SUM_A_VarTrue->SetLineColor( 4 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 1 ) ;}
    if (iTrue==3) { pTGraph_SUM_A_VarTrue->SetLineColor( 2 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 9 ) ;}
    if (iTrue==4) { pTGraph_SUM_A_VarTrue->SetLineColor( 3 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 9 ) ;}
    if (iTrue==5) { pTGraph_SUM_A_VarTrue->SetLineColor( 4 ) ;  pTGraph_SUM_A_VarTrue->SetLineStyle( 9 ) ;}

    pTMultiGraph_Q_VarTrue ->Add( pTGraph_SUM_Q_VarTrue ,"l");
    pTMultiGraph_A_VarTrue ->Add( pTGraph_SUM_A_VarTrue ,"l");
    
    pTLegend_Q_VarTrue->AddEntry( pTGraph_SUM_Q_VarTrue  ,Pos_Str.c_str()  , "l");
    pTLegend_A_VarTrue->AddEntry( pTGraph_SUM_A_VarTrue  ,Pos_Str.c_str() , "l");

    int iTgraph_Max = V_pTGraph_Q_VarTrue.size();
    int iPoints_Max = V_pTGraph_Q_VarTrue[0] ->GetN();
    for (int iPoints = 0 ; iPoints< iPoints_Max ; iPoints++){
      double TimeCur = (V_pTGraph_Q_VarTrue[0]->GetX())[iPoints];
      double SUM_Q = 0. ;
      double SUM_A = 0. ;
      for (int iTgraph = 0 ; iTgraph< iTgraph_Max ; iTgraph++){
        SUM_Q += (V_pTGraph_Q_VarTrue[iTgraph]->GetY())[iPoints] ;
        SUM_A += (V_pTGraph_A_VarTrue[iTgraph]->GetY())[iPoints] ;
      }
      pTGraph_SUM_Q_VarTrue ->SetPoint( pTGraph_SUM_Q_VarTrue ->GetN() , TimeCur  , SUM_Q ) ;
      pTGraph_SUM_A_VarTrue ->SetPoint( pTGraph_SUM_A_VarTrue ->GetN() , TimeCur  , SUM_A ) ;
    }

  }

//page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//page 2
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,2) ;

    pTCanvas->cd(1) ;
      pTMultiGraph_Q_OffCenter->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_Q_OffCenter->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_Q_OffCenter->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_A_OffCenter->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_OffCenter->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_OffCenter->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_OffCenter->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_A_OffCenter->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//page 2 Q
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 600) ;
  
      pTMultiGraph_Q_OffCenter->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_Q_OffCenter->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetTitleOffset(1.);
      pTMultiGraph_Q_OffCenter->GetXaxis()->SetLabelSize(0.05);
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetLabelSize(0.05);
      pTMultiGraph_Q_OffCenter->GetXaxis()->SetTitleSize(0.05);
      pTMultiGraph_Q_OffCenter->GetYaxis()->SetTitleSize(0.05);
      pTCanvas->Update();
 
      pTLegend_Q_OffCenter->Draw();
      pTCanvas->Update();

  pTCanvas->Print("OUT_Studies/Gain_02_Q_OffCenter.pdf");
  pTCanvas->SaveAs("OUT_Studies/Gain_02_Q_OffCenter.C");

//page 2 A
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 600) ;
  
      pTMultiGraph_A_OffCenter->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_OffCenter->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_OffCenter->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_OffCenter->GetYaxis()->SetTitleOffset(1.);
      pTMultiGraph_A_OffCenter->GetXaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_OffCenter->GetYaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_OffCenter->GetXaxis()->SetTitleSize(0.05);
      pTMultiGraph_A_OffCenter->GetYaxis()->SetTitleSize(0.05);
      pTCanvas->Update();
 
      pTLegend_A_OffCenter->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print("OUT_Studies/Gain_02_A_OffCenter.pdf");
  pTCanvas->SaveAs("OUT_Studies/Gain_02_A_OffCenter.C");

//page 3
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,2) ;

    pTCanvas->cd(1) ;
      pTMultiGraph_Q_VarRC->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_Q_VarRC->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Q_VarRC->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Q_VarRC->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_Q_VarRC->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_A_VarRC->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_VarRC->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_VarRC->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_VarRC->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_A_VarRC->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print(Str_Out_PDF.c_str());

//page 3 A
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 600) ;
  
      pTMultiGraph_A_VarRC->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_VarRC->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_VarRC->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_VarRC->GetYaxis()->SetTitleOffset(1.);
      pTMultiGraph_A_VarRC->GetXaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_VarRC->GetYaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_VarRC->GetXaxis()->SetTitleSize(0.05);
      pTMultiGraph_A_VarRC->GetYaxis()->SetTitleSize(0.05);
      pTCanvas->Update();
 
      pTLegend_A_VarRC->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print("OUT_Studies/Gain_02_A_VarRC.pdf");
  pTCanvas->SaveAs("OUT_Studies/Gain_02_A_VarRC.C");
  
//page 4
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(1,2) ;

    pTCanvas->cd(1) ;
      pTMultiGraph_Q_VarTrue->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_Q_VarTrue->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Q_VarTrue->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Q_VarTrue->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_Q_VarTrue->Draw();
      pTCanvas->Update();

    pTCanvas->cd(2) ;
      pTMultiGraph_A_VarTrue->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_VarTrue->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_VarTrue->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_VarTrue->GetYaxis()->SetTitleOffset(1.5);
      pTCanvas->Update();
 
      pTLegend_A_VarTrue->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//page 3 A
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 600) ;
  
      pTMultiGraph_A_VarTrue->Draw("A");
      pTCanvas->Update();
  
      pTMultiGraph_A_VarTrue->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_A_VarTrue->GetYaxis()->SetTitle("ADC counts");
      pTMultiGraph_A_VarTrue->GetYaxis()->SetTitleOffset(1.);
      pTMultiGraph_A_VarTrue->GetXaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_VarTrue->GetYaxis()->SetLabelSize(0.05);
      pTMultiGraph_A_VarTrue->GetXaxis()->SetTitleSize(0.05);
      pTMultiGraph_A_VarTrue->GetYaxis()->SetTitleSize(0.05);
      pTCanvas->Update();
 
      pTLegend_A_VarTrue->Draw();
      pTCanvas->Update();
      
  pTCanvas->Print("OUT_Studies/Gain_02_A_VarTrue.pdf");
  pTCanvas->SaveAs("OUT_Studies/Gain_02_A_VarTrue.C");
  
//page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
