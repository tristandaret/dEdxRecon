#include "Studies/Spread_03_WF.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
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
#include "TRandom3.h"
#include "TGaxis.h"
#include "TBox.h"

void Spread_03_WF()
{
//  
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF     = "OUT_Studies/Spread_03_WF.pdf" ; 
  std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
  std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;

//Page 1
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_Beg.c_str());

// 
  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*         pModel_Charge1D        = new Model_Charge1D        ;

  pModel_Charge1D->Set_Width(0.);
  pModel_Charge1D->Set_Gain(1.E3) ;
  pModel_Charge1D->Set_RC(50. * 1E-9 / ( 1E-3 * 1E-3)) ;

  
  double TheQValue = pModel_Electronics->Get_QValue() ;
  double TheWS     = pModel_Electronics->Get_ws()     ;
  pModel_Electronics->Set_TimeShape(TheQValue,TheWS);
  
  int    Nx       = pModel_ReadOutGeometry->Get_Nx      () ; 
  int    Ny       = pModel_ReadOutGeometry->Get_Ny      () ; 

  double Ly = pModel_ReadOutGeometry->Get_LY      () ; 

//  
  ROBoard aROBoard(
                   pModel_ReadOutGeometry  ,
                   pModel_Electronics      ,
                   pModel_Charge1D
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
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge1D , PadName , EventNber,EntryNber,ModuleNber   ,iX ,iY ); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//
  double Time0 = 0. ;

  int    iTem_Max  =  10    ;
  double SY_Max    =  0.98 * Ly/2. ;
  double SY_Min    = -0.98 * Ly/2. ;
  for (int iTem = 0 ; iTem< iTem_Max; iTem++){
    int iYTrue = Ny/2 ;
    double YTrue = pModel_ReadOutGeometry->Get_YcPad(0,iYTrue,ModuleNber);

    double SY = SY_Min + double(iTem)*(SY_Max-SY_Min)/double(iTem_Max-1) ;
    YTrue += SY ;
 
    int NberOfPads = aROBoard.Get_NberOfPads() ;
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,0., YTrue ); 
    }

    Pad* pPad_Center = aROBoard.Get_Pad(0,iYTrue  ) ;
    Pad* pPad_NextU  = aROBoard.Get_Pad(0,iYTrue+1) ;
    Pad* pPad_NextD  = aROBoard.Get_Pad(0,iYTrue-1) ;
    
//  
    TMultiGraph* pTMultiGraph_ADC = new TMultiGraph ;
    
    TLegend* pTLegend = new TLegend(0.3,0.65 ,0.4,0.55  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    std::vector < double > V_AMAX ;
    std::vector < double > V_TMAX ;
    for (int iTurn = 0 ; iTurn< 3 ; iTurn++){
      Pad* pPad_Cur = 0 ;
      if (iTurn==0) pPad_Cur = pPad_Center ;
      if (iTurn==1) pPad_Cur = pPad_NextU  ;
      if (iTurn==2) pPad_Cur = pPad_NextD  ;

      TGraph* pTGraph_ADC = new TGraph ;
      if (iTurn==0) pTGraph_ADC->SetLineColor( 2 ) ;
      if (iTurn==1) pTGraph_ADC->SetLineColor( 4 ) ;
      if (iTurn==2) pTGraph_ADC->SetLineColor( 8 ) ;

      pTMultiGraph_ADC->Add(pTGraph_ADC) ;
      
      if (iTurn==0) pTLegend->AddEntry( pTGraph_ADC  , " Leading " , "l");
      if (iTurn==1) pTLegend->AddEntry( pTGraph_ADC  , " Right "   , "l");
      if (iTurn==2) pTLegend->AddEntry( pTGraph_ADC  , " Left "    , "l");

      double Tmin =     0.        ;
      double Tmax =  4000. * 1E-9 ; 
      int    Npoints = 1000 ;
      double AMAX = 0. ;
      double TMAX = 0. ;
      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
        double ADC = pPad_Cur->Get_APad(TimeCur) ;
        pTGraph_ADC->SetPoint( pTGraph_ADC->GetN(), TimeCur /40.E-9 , ADC  ) ;
        if (iPoint==0){
          AMAX = 0. ;
          TMAX = 0. ;
        } 
        if (AMAX<ADC){
          AMAX = ADC ;
          TMAX = TimeCur /40.E-9 ;
        }
      }
      V_AMAX.push_back(AMAX);
      V_TMAX.push_back(TMAX);
      
    }

//
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
    
      std::ostringstream aostringstream ;
      aostringstream << std::setiosflags(std::ios::fixed) ;
      aostringstream << "ADC vs Time: X_{T} = " << std::setw(6) << std::setprecision(3) << SY * 1.E3 << " mm " ;
      std::string Title = aostringstream.str() ; 
      
      pTMultiGraph_ADC->SetTitle(Title.c_str());
      pTMultiGraph_ADC->GetXaxis()->SetTitle("Time (time bin)");
      pTMultiGraph_ADC->GetYaxis()->SetTitle("ADC");
      pTMultiGraph_ADC->GetYaxis()->SetTitleOffset(1.5);
      pTMultiGraph_ADC->SetMaximum( 600. );
      pTMultiGraph_ADC->SetMinimum(-150.);

      pTMultiGraph_ADC->Draw("A") ;

      pTCanvas->Update();
      
      pTLegend->Draw(); 
            
      pTCanvas->Update();
      
      double Xmin = pTCanvas->GetUxmin() ;
      double Xmax = pTCanvas->GetUxmax() ;
      TLine* pTLineH = new TLine( Xmin  , 0. , Xmax , 0. ) ;
      pTLineH ->SetLineStyle(1); 
      pTLineH ->SetLineWidth(1); 
      pTLineH ->SetLineColor( 1 ) ;
      pTLineH->Draw();  
      pTCanvas->Update()    ;
      
      double Ymin = pTCanvas->GetUymin() ;
      double Ymax = pTCanvas->GetUymax() ;
      TLine* pTLineV = new TLine( 0.  , Ymin , 0. , Ymax) ;
      pTLineV ->SetLineStyle(1); 
      pTLineV ->SetLineWidth(1); 
      pTLineV ->SetLineColor( 1 ) ;
      pTLineV->Draw();  
      pTCanvas->Update()    ;
      
      for (int iTurn = 0 ; iTurn< 3; iTurn++){
        double XCur =  V_TMAX[iTurn] ;        
        double YCur =  V_AMAX[iTurn] ;        
        TLine* pTLineV = new TLine( XCur  , 0. , XCur , YCur ) ;
        pTLineV->SetLineStyle(2); 
        pTLineV->SetLineWidth(1); 
        if (iTurn==0) pTLineV->SetLineColor( 2 ) ;
        if (iTurn==1) pTLineV->SetLineColor( 4 ) ;
        if (iTurn==2) pTLineV->SetLineColor( 8 ) ;
        pTLineV->Draw();  
        TLine* pTLineH = new TLine( 0.    , YCur , XCur , YCur ) ;
        pTLineH->SetLineStyle(2); 
        pTLineH->SetLineWidth(1); 
        if (iTurn==0) pTLineH->SetLineColor( 2 ) ;
        if (iTurn==1) pTLineH->SetLineColor( 4 ) ;
        if (iTurn==2) pTLineH->SetLineColor( 8 ) ;
        pTLineH->Draw();  
        pTCanvas->Update()    ;

      }
      
       
      double Axis_Xmin =  pTCanvas->GetUxmin() + 0.40 * (pTCanvas->GetUxmax()-pTCanvas->GetUxmin() ) ;
      double Axis_Xmax =  pTCanvas->GetUxmin() + 0.80 * (pTCanvas->GetUxmax()-pTCanvas->GetUxmin() ) ;
      double Axis_Ymax =  pTCanvas->GetUymin() + 0.80 * (pTCanvas->GetUymax()-pTCanvas->GetUymin() ) ;
      double Axis_Xmin_new = -Ly*1.E3*3./2. ;
      double Axis_Xmax_new =  Ly*1.E3*3./2. ;
      
      std::cout << "*Axis_Xmin " << Axis_Xmin << std::endl ;
      std::cout << " Axis_Xmax " << Axis_Xmax << std::endl ;
      
      double X_SY_new = SY*1.E3 ;
      
      double Y_SY = Axis_Ymax ;
      double X_SY = Axis_Xmin +  (Axis_Xmin_new-X_SY_new)  * ( Axis_Xmax - Axis_Xmin) / (Axis_Xmin_new-Axis_Xmax_new) ;
      
      double TrackL = 0.10 * (pTCanvas->GetUymax()-pTCanvas->GetUymin() ) ;
      
      TGraph* pTGraph_Axis_Point = new TGraph ;
      pTGraph_Axis_Point->SetPoint( pTGraph_Axis_Point->GetN(), X_SY , Y_SY - TrackL*0.3          ) ;
      pTGraph_Axis_Point->SetPoint( pTGraph_Axis_Point->GetN(), X_SY , Y_SY + TrackL*0.3 + TrackL ) ;
      pTGraph_Axis_Point->SetMarkerStyle(20) ;
      pTGraph_Axis_Point->SetMarkerColor(2) ;
      pTGraph_Axis_Point->SetMarkerSize(1.8) ;
      pTGraph_Axis_Point->SetLineWidth(2) ;
      pTGraph_Axis_Point->SetLineStyle(9) ;
      pTGraph_Axis_Point->SetLineColor(1) ;

      TGaxis* pTGaxis = new TGaxis(
        Axis_Xmin,Axis_Ymax,Axis_Xmax,Axis_Ymax,
        Axis_Xmin_new,Axis_Xmax_new,511,"");
      pTGaxis->SetName("pTGaxis");
      pTGaxis->SetLabelSize(0.02);
      pTGaxis->SetTextFont(72);
      pTGaxis->SetLabelOffset(0.02);
      pTGaxis->Draw();
      pTCanvas->Update()    ;
      
      for (int iPad = 0 ; iPad< 3; iPad++){
        double X_SY_1_new = -Ly*1.E3*3./2 + double(iPad)*Ly*1.E3;
        double X_SY_2_new = X_SY_1_new + Ly*1.E3 ;
        
        std::cout << "*X_SY_1_new " << X_SY_1_new << std::endl ;
        std::cout << " X_SY_2_new " << X_SY_2_new << std::endl ;
        
        double X_SY_1 = Axis_Xmin +  (Axis_Xmin_new-X_SY_1_new)  * ( Axis_Xmax - Axis_Xmin) / (Axis_Xmin_new-Axis_Xmax_new) ;
        double Y_SY_1 = Axis_Ymax ;

        double X_SY_2 = Axis_Xmin +  (Axis_Xmin_new-X_SY_2_new)  * ( Axis_Xmax - Axis_Xmin) / (Axis_Xmin_new-Axis_Xmax_new) ;
        double Y_SY_2 = Y_SY_1    + TrackL ;
        
        std::cout << "*X_SY_1 " << X_SY_1 << std::endl ;
        std::cout << " Y_SY_1 " << Y_SY_1 << std::endl ;
        std::cout << " X_SY_2 " << X_SY_2 << std::endl ;
        std::cout << " Y_SY_2 " << Y_SY_2 << std::endl ;
        
        TBox* pTBox = new TBox(X_SY_1,Y_SY_1,X_SY_2,Y_SY_2) ;
        pTBox->SetFillColor(0) ;
        pTBox->SetFillStyle(0) ;
        pTBox->SetLineStyle(2) ;
        pTBox->SetLineColor(2) ;
        if (iPad==0) pTBox->SetLineColor( 8 ) ;
        if (iPad==1) pTBox->SetLineColor( 2 ) ;
        if (iPad==2) pTBox->SetLineColor( 4 ) ;
        pTBox->Draw() ;
        pTCanvas->Update()    ;

      }
      
      pTGraph_Axis_Point->Draw("l") ;
      pTCanvas->Update()    ;

      
    pTCanvas->Print(Str_Out_PDF.c_str());

     std::ostringstream aostringstream_PNG ;
     aostringstream_PNG << std::setiosflags(std::ios::fixed) ;
     aostringstream_PNG << "OUT_Studies/Spread_03_WF_" << iTem << ".svg";
     std::string Str_Out_PNG     = aostringstream_PNG.str() ; 
     pTCanvas->Print(Str_Out_PNG.c_str());
    
  }

//Page Last
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1800, 1200) ;
  pTCanvas->Print(Str_Out_PDF_End.c_str());

}
