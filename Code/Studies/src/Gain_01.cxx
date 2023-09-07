#include "Studies/Gain_01.h"

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

void Gain_01()
{
  
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Gain_01.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge0D*         pModel_Charge0D        = new Model_Charge0D        ;

  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ;  
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

  pModel_Charge0D->Set_Width(0.);
  
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
//double XL = pPad_Center->Get_XL() ;
  double XH = pPad_Center->Get_XH() ;
//double YL = pPad_Center->Get_YL() ;
  double YH = pPad_Center->Get_YH() ;
  
  
  std::cout << " iXc " << std::setw(3) << iXc << std::endl ;
  std::cout << " iYc " << std::setw(3) << iYc << std::endl ;
  
  std::cout << "  Xc (mm)  " << std::setw(10) << std::setprecision(3) << Xc   *1.E3 << std::endl ;
  std::cout << "  Yc (mm)  " << std::setw(10) << std::setprecision(3) << Yc   *1.E3 << std::endl ;

//
  int NberOfPads = aROBoard.Get_NberOfPads() ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,Xc,Yc ); 
  }

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

//
  TMultiGraph* pTMultiGraph_AllSingle = new TMultiGraph();
  
  std::vector < TGraph* > V_pTGraph_ADC_Pad ;
  std::vector < TGraph* > V_pTGraph_ADC2Charge_Pad ;
  std::vector < TGraph* > V_pTGraph_Q_Pad ;
  
  for (int iDY = 0 ; iDY< 3 ; iDY++){
  for (int iDX = 0 ; iDX< 3 ; iDX++){
    int iDY_Local = 2 - iDY ;
    int iX = iXc - 1 + iDX ;
    int iY = iYc - 1 + iDY_Local ;
    Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
    
    TGraph* pTGraph_ADC_Pad = new TGraph ;
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << " iX=" << iX << ", iY=" << iY ;
    pTGraph_ADC_Pad->SetTitle( (aostringstream.str()).c_str() );  
    
    pTMultiGraph_AllSingle->Add( pTGraph_ADC_Pad  ,"l");
    V_pTGraph_ADC_Pad.push_back( pTGraph_ADC_Pad ) ;
              
    TGraph* pTGraph_ADC2Charge_Pad = new TGraph ;
    pTGraph_ADC2Charge_Pad->SetTitle( (aostringstream.str()).c_str() );  
    V_pTGraph_ADC2Charge_Pad.push_back( pTGraph_ADC2Charge_Pad ) ;

    TGraph* pTGraph_Q_Pad = new TGraph ;
    pTGraph_Q_Pad->SetTitle( (aostringstream.str()).c_str() );  
    V_pTGraph_Q_Pad.push_back( pTGraph_Q_Pad ) ;

    double Tmin =     0.        ;
    double Tmax =  2000. * 1E-9 ; 
    int    Npoints = 1000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
      double Q_Pad = pPad_Cur->Get_Qpad(TimeCur) ;
      pTGraph_ADC_Pad       ->SetPoint( pTGraph_ADC_Pad       ->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      pTGraph_ADC2Charge_Pad->SetPoint( pTGraph_ADC2Charge_Pad->GetN(), TimeCur*1.E6 , A_Pad*(120.E-15/4096.)/1.602176565E-19  ) ;
      pTGraph_Q_Pad         ->SetPoint( pTGraph_Q_Pad         ->GetN(), TimeCur*1.E6 , Q_Pad                 /1.602176565E-19  ) ;
    }

    
  }
  }

  TGraph* pTGraph_Sum_AllSingle     = new TGraph ; pTGraph_Sum_AllSingle     ->SetLineColor( 2 ) ;
  TGraph* pTGraph_Central_AllSingle = new TGraph ; pTGraph_Central_AllSingle ->SetLineColor( 4 ) ;
  
  double Tmin =     0.        ;
  double Tmax =  2000. * 1E-9 ; 
  int    Npoints = 1000 ;
  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double TheSum = 0. ;
    double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
    for (int iDX = 0 ; iDX< 3 ; iDX++){
    for (int iDY = 0 ; iDY< 3 ; iDY++){
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY ;
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
      TheSum += A_Pad ;
      if (iDX==1&&iDY==1)  pTGraph_Central_AllSingle->SetPoint( pTGraph_Central_AllSingle->GetN(), TimeCur*1.E6 , A_Pad  ) ;
    }
    }
    pTGraph_Sum_AllSingle->SetPoint( pTGraph_Sum_AllSingle->GetN(), TimeCur*1.E6 , TheSum  ) ;
  }
  pTMultiGraph_AllSingle->Add( pTGraph_Central_AllSingle  ,"l");
  pTMultiGraph_AllSingle->Add( pTGraph_Sum_AllSingle      ,"l");
  
  TLegend* pTLegend_AllSingle = new TLegend(0.55,0.65 ,0.85,0.79  ); 
  pTLegend_AllSingle->SetFillStyle(1001);
  pTLegend_AllSingle->SetBorderSize(1);

  pTLegend_AllSingle->AddEntry( pTGraph_Sum_AllSingle     ,"Sum"     , "l");
  pTLegend_AllSingle->AddEntry( pTGraph_Central_AllSingle ,"Leading" , "l");
  pTLegend_AllSingle->AddEntry( V_pTGraph_ADC_Pad[0]      ,"Other pads");

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(3,3) ;
  
    int iTGraph_Max = V_pTGraph_ADC_Pad.size() ;
    for (int iTGraph = 0 ; iTGraph< iTGraph_Max ; iTGraph++){
      TGraph* pTGraph = V_pTGraph_ADC_Pad[iTGraph] ;
      pTGraph->SetLineColor( 2 ) ;

      TMultiGraph* pTMultiGraph_Local = new TMultiGraph();
      pTMultiGraph_Local->Add( pTGraph  ,"l");

      pTMultiGraph_Local->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Local->GetYaxis()->SetTitle("ADC");
      pTMultiGraph_Local->GetYaxis()->SetTitleOffset(1.5);
      
      pTCanvas->cd(iTGraph+1) ;
      pTMultiGraph_Local->Draw("A");
      pTCanvas->Update();
    }
  
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(3,3) ;
  
    iTGraph_Max = V_pTGraph_ADC2Charge_Pad.size() ;
    for (int iTGraph = 0 ; iTGraph< iTGraph_Max ; iTGraph++){
      TGraph* pTGraph_ADC2Charge = V_pTGraph_ADC2Charge_Pad[iTGraph] ;
      TGraph* pTGraph_Q_Pad      = V_pTGraph_Q_Pad[iTGraph] ;

      pTGraph_ADC2Charge->SetLineColor( 2 ) ;
      pTGraph_Q_Pad     ->SetLineColor( 4 ) ;
      
      TMultiGraph* pTMultiGraph_Local = new TMultiGraph();
      pTMultiGraph_Local->Add( pTGraph_ADC2Charge  ,"l");
      pTMultiGraph_Local->Add( pTGraph_Q_Pad  ,"l");

      pTMultiGraph_Local->SetTitle(pTGraph_ADC2Charge->GetTitle());
      pTMultiGraph_Local->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Local->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Local->GetYaxis()->SetTitleOffset(1.5);
      
      TLegend* pTLegend_Local = new TLegend(0.55,0.65 ,0.85,0.79  ); 
      pTLegend_Local->SetFillStyle(1001);
      pTLegend_Local->SetBorderSize(1);
      
      pTLegend_Local->AddEntry( pTGraph_ADC2Charge ,"ADC->Q"     , "l");
      pTLegend_Local->AddEntry( pTGraph_Q_Pad      ,"Charge"  , "l");
      
      pTCanvas->cd(iTGraph+1) ;
      pTMultiGraph_Local->Draw("A");
      pTCanvas->Update();
      
      pTLegend_Local->Draw();
      pTCanvas->Update();
    }
  
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
    iTGraph_Max = V_pTGraph_ADC_Pad.size() ;
    for (int iTGraph = 0 ; iTGraph< iTGraph_Max ; iTGraph++){
      TGraph* pTGraph = V_pTGraph_ADC_Pad[iTGraph] ;
      pTGraph->SetLineColor( 1 ) ;
    }
    
    pTMultiGraph_AllSingle->SetTitle("ADC");  
    pTMultiGraph_AllSingle->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph_AllSingle->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_AllSingle->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_AllSingle->SetMinimum(-2.);
    pTMultiGraph_AllSingle->SetMaximum( 6.);
    
    pTMultiGraph_AllSingle->Draw("A");
    pTCanvas->Update();

    pTLegend_AllSingle->Draw();
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  TMultiGraph* pTMultiGraph_LeadingSum_XYHIT = new TMultiGraph();

  TLegend* pTLegend_LeadingSum_XYHIT = new TLegend(0.55,0.65 ,0.85,0.79  ); 
  pTLegend_LeadingSum_XYHIT->SetFillStyle(1001);
  pTLegend_LeadingSum_XYHIT->SetBorderSize(1);
  
  Tmin =     0.        ;
  Tmax =  2000. * 1E-9 ; 
  Npoints = 1000 ;
  int iPos_Max = 5 ;
  double Xhit_min = Xc ;
  double Xhit_max = XH - (XH-Xc)/10.  ;
  double Yhit_min = Yc ;
  double Yhit_max = YH - (YH-Yc)/10.  ;
  for (int iPos = 0 ; iPos< iPos_Max ; iPos++){
    double Xhit = Xhit_min + double (iPos)*(Xhit_max-Xhit_min)/double(iPos_Max-1) ;
    double Yhit = Yhit_min + double (iPos)*(Yhit_max-Yhit_min)/double(iPos_Max-1) ;
    
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,Xhit,Yhit ); 
    }
  
    TGraph* pTGraph_SUM     = new TGraph ; pTGraph_SUM     ->SetLineColor( 2 ) ;
    TGraph* pTGraph_Central = new TGraph ; pTGraph_Central ->SetLineColor( 4 ) ;
    pTGraph_SUM     ->SetLineStyle( 1 ) ;
    pTGraph_Central ->SetLineStyle( 1 ) ;
    if (iPos!=0) pTGraph_SUM     ->SetLineStyle( 3 ) ;
    if (iPos!=0) pTGraph_Central ->SetLineStyle( 3 ) ;

    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TheSum = 0. ;
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      for (int iDX = 0 ; iDX< 3 ; iDX++){
      for (int iDY = 0 ; iDY< 3 ; iDY++){
        int iX = iXc - 1 + iDX ;
        int iY = iYc - 1 + iDY ;
        Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
        double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
        TheSum += A_Pad ;
        if (iDX==1&&iDY==1)  pTGraph_Central->SetPoint( pTGraph_Central->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      }
      }
      pTGraph_SUM->SetPoint( pTGraph_SUM->GetN(), TimeCur*1.E6 , TheSum  ) ;
    }
    
    pTMultiGraph_LeadingSum_XYHIT->Add( pTGraph_SUM      ,"l");
    pTMultiGraph_LeadingSum_XYHIT->Add( pTGraph_Central  ,"l");
    if (iPos==0){
      pTLegend_LeadingSum_XYHIT->AddEntry( pTGraph_SUM     ,"Sum"     , "l");
      pTLegend_LeadingSum_XYHIT->AddEntry( pTGraph_Central ,"Leading" , "l");
    }
    
  }

  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
    pTMultiGraph_LeadingSum_XYHIT->SetTitle("Effect of changing Hit position");  
    pTMultiGraph_LeadingSum_XYHIT->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph_LeadingSum_XYHIT->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_LeadingSum_XYHIT->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_LeadingSum_XYHIT->SetMinimum(-2.);
    pTMultiGraph_LeadingSum_XYHIT->SetMaximum( 6.);

    pTMultiGraph_LeadingSum_XYHIT->Draw("A");
    pTCanvas->Update();

    pTLegend_LeadingSum_XYHIT->Draw();
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());


//
  TMultiGraph* pTMultiGraph_LeadingSum_RC = new TMultiGraph();
  
  TLegend* pTLegend_LeadingSum_RC = new TLegend(0.55,0.65 ,0.85,0.79  ); 
  pTLegend_LeadingSum_RC->SetFillStyle(1001);
  pTLegend_LeadingSum_RC->SetBorderSize(1);
  
  Tmin =     0.        ;
  Tmax =  2000. * 1E-9 ; 
  Npoints = 1000 ;
  
  double Xhit = XH - (XH-Xc)/10. ;
  double Yhit = YH - (YH-Yc)/10. ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,Xhit,Yhit ); 
  }
  
  double RC_min =  40. * 1E-9 / ( 1E-3 * 1E-3) ;
  double RC_max = 100. * 1E-9 / ( 1E-3 * 1E-3) ;
  int iRC_Max = 5 ;
  for (int iRC = 0 ; iRC< iRC_Max ; iRC++){
    double RCcur = RC_min + double (iRC)*(RC_max-RC_min)/double(iRC_Max-1) ;
    pModel_Charge0D->Set_RC( RCcur ) ;
    TGraph* pTGraph_SUM     = new TGraph ; pTGraph_SUM     ->SetLineColor( 2 ) ;
    TGraph* pTGraph_Central = new TGraph ; pTGraph_Central ->SetLineColor( 4 ) ;
    pTGraph_SUM     ->SetLineStyle( 1 ) ;
    pTGraph_Central ->SetLineStyle( 1 ) ;
    if (iRC!=0) pTGraph_SUM     ->SetLineStyle( 3 ) ;
    if (iRC!=0) pTGraph_Central ->SetLineStyle( 3 ) ;

    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TheSum = 0. ;
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      for (int iDX = 0 ; iDX< 3 ; iDX++){
      for (int iDY = 0 ; iDY< 3 ; iDY++){
        int iX = iXc - 1 + iDX ;
        int iY = iYc - 1 + iDY ;
        Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
        double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
        TheSum += A_Pad ;
        if (iDX==1&&iDY==1)  pTGraph_Central->SetPoint( pTGraph_Central->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      }
      }
      pTGraph_SUM->SetPoint( pTGraph_SUM->GetN(), TimeCur*1.E6 , TheSum  ) ;
    }
    
    pTMultiGraph_LeadingSum_RC->Add( pTGraph_SUM      ,"l");
    pTMultiGraph_LeadingSum_RC->Add( pTGraph_Central  ,"l");
    if (iRC==0){
      pTLegend_LeadingSum_RC->AddEntry( pTGraph_SUM     ,"Sum"     , "l");
      pTLegend_LeadingSum_RC->AddEntry( pTGraph_Central ,"Leading" , "l");
    }
    
  }

  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  
    pTMultiGraph_LeadingSum_RC->SetTitle("Effect of changing RC");  
    pTMultiGraph_LeadingSum_RC->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph_LeadingSum_RC->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_LeadingSum_RC->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_LeadingSum_RC->SetMinimum(-2.);
    pTMultiGraph_LeadingSum_RC->SetMaximum( 6.);

    pTMultiGraph_LeadingSum_RC->Draw("A");
    pTCanvas->Update();

    pTLegend_LeadingSum_RC->Draw();
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  TMultiGraph* pTMultiGraph_LeadingSum_A_QHistory = new TMultiGraph();
  TMultiGraph* pTMultiGraph_LeadingSum_Q_QHistory = new TMultiGraph();
  
  TLegend* pTLegend_LeadingSum_A_QHistory = new TLegend(0.65,0.65 ,0.85,0.79  ); 
  pTLegend_LeadingSum_A_QHistory->SetFillStyle(1001);
  pTLegend_LeadingSum_A_QHistory->SetBorderSize(1);
  
  TLegend* pTLegend_LeadingSum_Q_QHistory = new TLegend(0.35,0.65 ,0.55,0.79  ); 
  pTLegend_LeadingSum_Q_QHistory->SetFillStyle(1001);
  pTLegend_LeadingSum_Q_QHistory->SetBorderSize(1);
  
  Tmin =     0.        ;
  Tmax =  1000. * 1E-9 ; 
  Npoints = 1000 ;
  
  Xhit = XH - (XH-Xc)/10. ;
  Yhit = YH - (YH-Yc)/10.  ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,Xhit,Yhit ); 
  }
  
  TGraph* pTGraph_A_SUM     = new TGraph ; pTGraph_A_SUM     ->SetLineColor( 2 ) ;
  TGraph* pTGraph_A_Central = new TGraph ; pTGraph_A_Central ->SetLineColor( 4 ) ;

  TGraph* pTGraph_Q_SUM     = new TGraph ; pTGraph_Q_SUM     ->SetLineColor( 2 ) ;
  TGraph* pTGraph_Q_Central = new TGraph ; pTGraph_Q_Central ->SetLineColor( 4 ) ;

  for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
    double TheSum_A = 0. ;
    double TheSum_Q = 0. ;
    double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
    for (int iDX = 0 ; iDX< 3 ; iDX++){
    for (int iDY = 0 ; iDY< 3 ; iDY++){
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY ;
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
      double Q_Pad = pPad_Cur->Get_Qpad(TimeCur) ;
      TheSum_A += A_Pad ;
      TheSum_Q += Q_Pad/1.602176565E-19 ;
      if (iDX==1&&iDY==1)  pTGraph_A_Central->SetPoint( pTGraph_A_Central->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      if (iDX==1&&iDY==1)  pTGraph_Q_Central->SetPoint( pTGraph_Q_Central->GetN(), TimeCur*1.E6 , Q_Pad/1.602176565E-19  ) ;
    }
    }
    pTGraph_A_SUM->SetPoint( pTGraph_A_SUM->GetN(), TimeCur*1.E6 , TheSum_A  ) ;
    pTGraph_Q_SUM->SetPoint( pTGraph_Q_SUM->GetN(), TimeCur*1.E6 , TheSum_Q  ) ;
  }
  
  pTMultiGraph_LeadingSum_A_QHistory->Add( pTGraph_A_SUM      ,"l");
  pTMultiGraph_LeadingSum_A_QHistory->Add( pTGraph_A_Central  ,"l");
    
  pTMultiGraph_LeadingSum_Q_QHistory->Add( pTGraph_Q_SUM      ,"l");
  pTMultiGraph_LeadingSum_Q_QHistory->Add( pTGraph_Q_Central  ,"l");
    
  pTLegend_LeadingSum_A_QHistory->AddEntry( pTGraph_A_SUM     ,"Sum"     , "l");
  pTLegend_LeadingSum_A_QHistory->AddEntry( pTGraph_A_Central ,"Leading" , "l");
  pTLegend_LeadingSum_Q_QHistory->AddEntry( pTGraph_Q_SUM     ,"Sum"     , "l");
  pTLegend_LeadingSum_Q_QHistory->AddEntry( pTGraph_Q_Central ,"Leading" , "l");
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
   pTCanvas->Divide(1,2) ;
    
    pTCanvas->cd(1);
    pTMultiGraph_LeadingSum_A_QHistory->SetTitle("ADC");  
    pTMultiGraph_LeadingSum_A_QHistory->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph_LeadingSum_A_QHistory->GetYaxis()->SetTitle("ADC");
    pTMultiGraph_LeadingSum_A_QHistory->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_LeadingSum_A_QHistory->Draw("A");
    pTCanvas->Update();

    pTLegend_LeadingSum_A_QHistory->Draw();
    pTCanvas->Update();

    pTCanvas->cd(2);
    pTCanvas->cd( 2 )->SetGrid();
    pTMultiGraph_LeadingSum_Q_QHistory->SetTitle("Charge");  
    pTMultiGraph_LeadingSum_Q_QHistory->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph_LeadingSum_Q_QHistory->GetYaxis()->SetTitle("Q (electrons)");
    pTMultiGraph_LeadingSum_Q_QHistory->GetYaxis()->SetTitleOffset(1.5);
  
    pTMultiGraph_LeadingSum_Q_QHistory->Draw("A");
    pTCanvas->Update();

    pTLegend_LeadingSum_Q_QHistory->Draw();
    pTCanvas->Update();

  pTCanvas->Print(Str_Out_PDF.c_str());

//
  V_pTGraph_ADC2Charge_Pad.clear() ;
  V_pTGraph_Q_Pad.clear() ;
  
  Tmin =     0.        ;
  Tmax =  2000. * 1E-9 ; 
  Npoints = 1000 ;
  
  Xhit = Xc ;
  Yhit = Yc ;
  for (int iPad = 0 ; iPad< NberOfPads; iPad++){
    Pad* pPad = aROBoard.Get_Pad(iPad) ;
    pPad->SetSignalModel( Time0 ,Xhit,Yhit ); 
  }
  
  double RCcur = 100000. * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge0D->Set_RC( RCcur ) ;

  for (int iDY = 0 ; iDY< 3 ; iDY++){
  for (int iDX = 0 ; iDX< 3 ; iDX++){
    int iDY_Local = 2 - iDY ;
    int iX = iXc - 1 + iDX ;
    int iY = iYc - 1 + iDY_Local ;
    Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
    
    TGraph* pTGraph_ADC_Pad = new TGraph ;
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << " iX=" << iX << ", iY=" << iY ;
    pTGraph_ADC_Pad->SetTitle( (aostringstream.str()).c_str() );  
    
    pTMultiGraph_AllSingle->Add( pTGraph_ADC_Pad  ,"l");
    V_pTGraph_ADC_Pad.push_back( pTGraph_ADC_Pad ) ;
              
    TGraph* pTGraph_ADC2Charge_Pad = new TGraph ;
    pTGraph_ADC2Charge_Pad->SetTitle( (aostringstream.str()).c_str() );  
    V_pTGraph_ADC2Charge_Pad.push_back( pTGraph_ADC2Charge_Pad ) ;

    TGraph* pTGraph_Q_Pad = new TGraph ;
    pTGraph_Q_Pad->SetTitle( (aostringstream.str()).c_str() );  
    V_pTGraph_Q_Pad.push_back( pTGraph_Q_Pad ) ;

    double Tmin =     0.        ;
    double Tmax =  2000. * 1E-9 ; 
    int    Npoints = 1000 ;
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
      double Q_Pad = pPad_Cur->Get_Qpad(TimeCur) ;
      pTGraph_ADC_Pad       ->SetPoint( pTGraph_ADC_Pad       ->GetN(), TimeCur*1.E6 , A_Pad  ) ;
      pTGraph_ADC2Charge_Pad->SetPoint( pTGraph_ADC2Charge_Pad->GetN(), TimeCur*1.E6 , A_Pad*(120.E-15/4096.)/1.602176565E-19  ) ;
      pTGraph_Q_Pad         ->SetPoint( pTGraph_Q_Pad         ->GetN(), TimeCur*1.E6 , Q_Pad                 /1.602176565E-19  ) ;
    }

    
  }
  }
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Divide(3,3) ;
  
    iTGraph_Max = V_pTGraph_ADC2Charge_Pad.size() ;
    for (int iTGraph = 0 ; iTGraph< iTGraph_Max ; iTGraph++){
      TGraph* pTGraph_ADC2Charge = V_pTGraph_ADC2Charge_Pad[iTGraph] ;
      TGraph* pTGraph_Q_Pad      = V_pTGraph_Q_Pad[iTGraph] ;

      pTGraph_ADC2Charge->SetLineColor( 2 ) ;
      pTGraph_Q_Pad     ->SetLineColor( 4 ) ;
      
      TMultiGraph* pTMultiGraph_Local = new TMultiGraph();
      pTMultiGraph_Local->Add( pTGraph_ADC2Charge  ,"l");
      pTMultiGraph_Local->Add( pTGraph_Q_Pad  ,"l");

      pTMultiGraph_Local->SetTitle(pTGraph_ADC2Charge->GetTitle());
      pTMultiGraph_Local->GetXaxis()->SetTitle("Time (#mu s)");
      pTMultiGraph_Local->GetYaxis()->SetTitle("Charge (e)");
      pTMultiGraph_Local->GetYaxis()->SetTitleOffset(1.5);
      
      TLegend* pTLegend_Local = new TLegend(0.55,0.65 ,0.85,0.79  ); 
      pTLegend_Local->SetFillStyle(1001);
      pTLegend_Local->SetBorderSize(1);
      
      pTLegend_Local->AddEntry( pTGraph_ADC2Charge ,"ADC->Q"     , "l");
      pTLegend_Local->AddEntry( pTGraph_Q_Pad      ,"Charge"  , "l");
      
      pTCanvas->cd(iTGraph+1) ;
      pTMultiGraph_Local->Draw("A");
      pTCanvas->Update();
      
      pTLegend_Local->Draw();
      pTCanvas->Update();
    }
  
  pTCanvas->Print(Str_Out_PDF.c_str());
  
//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
