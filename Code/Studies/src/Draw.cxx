#include "Studies/Draw.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_Charge1D.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLine.h"
#include "TF1.h"

void Draw_01()
{
//
  Model_ReadOutGeometry aModel_ReadOutGeometry ;
  Model_Electronics     aModel_Electronics     ;
  Model_Charge1D        aModel_Charge1D        ;

  double LX= aModel_ReadOutGeometry.Get_LX() ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double Time0  = 0. ;

  double qe     = 1.602176565E-19 ;

//
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  Pad aPad_UpUp     (&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "UpUp    " , EventNber,EntryNber,ModuleNber , 0 , 18 , 0. , LX ,  1.5*LY ,  2.5*LY );
  Pad aPad_Up       (&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "Up      " , EventNber,EntryNber,ModuleNber , 0 , 17 , 0. , LX ,  0.5*LY ,  1.5*LY );
  Pad aPad_Central  (&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "Central " , EventNber,EntryNber,ModuleNber , 0 , 16 , 0. , LX , -0.5*LY ,  0.5*LY );
  Pad aPad_Down     (&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "Down    " , EventNber,EntryNber,ModuleNber , 0 , 15 , 0. , LX , -1.5*LY , -0.5*LY );
  Pad aPad_DownDown (&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "DownDown" , EventNber,EntryNber,ModuleNber , 0 , 13 , 0. , LX , -2.5*LY , -1.5*LY );

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF = "BIDON" ; 

//
  double Tmin =  0. * 1E-6 ;
  double Tmax = 10. * 1E-6 ; 
  int    Npoints = 1000 ;
  
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Divide(2,2); 

  for (int iTurn = 0 ; iTurn< 4; iTurn++){
    double XTrue = 0. ;
    double YTrue = 0. ;
    if (iTurn==1) YTrue = 0.25*LY ;
    if (iTurn==2) YTrue =-0.25*LY ;
    if (iTurn==3) YTrue = 0.75*LY ;
    
    aPad_UpUp     .SetSignalModel( Time0 , XTrue , YTrue );
    aPad_Up       .SetSignalModel( Time0 , XTrue , YTrue );
    aPad_Central  .SetSignalModel( Time0 , XTrue , YTrue );
    aPad_Down     .SetSignalModel( Time0 , XTrue , YTrue );
    aPad_DownDown .SetSignalModel( Time0 , XTrue , YTrue );
    
    TMultiGraph* pTMultiGraph = new TMultiGraph();

    TGraph* pTGraph_Pad_UpUp     = new TGraph  ; 
    TGraph* pTGraph_Pad_Up       = new TGraph  ;
    TGraph* pTGraph_Pad_Central  = new TGraph  ;
    TGraph* pTGraph_Pad_Down     = new TGraph  ;
    TGraph* pTGraph_Pad_DownDown = new TGraph  ;

    pTGraph_Pad_UpUp     ->SetLineStyle( 1) ; pTGraph_Pad_UpUp     ->SetLineColor(2) ;
    pTGraph_Pad_Up       ->SetLineStyle( 3) ; pTGraph_Pad_Up       ->SetLineColor(2) ;
    pTGraph_Pad_Central  ->SetLineStyle( 1) ; pTGraph_Pad_Central  ->SetLineColor(1) ;
    pTGraph_Pad_Down     ->SetLineStyle( 3) ; pTGraph_Pad_Down     ->SetLineColor(4) ;
    pTGraph_Pad_DownDown ->SetLineStyle( 1) ; pTGraph_Pad_DownDown ->SetLineColor(4) ;
    
    for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
      double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
      
      double Q_Pad_UpUp     = aPad_UpUp     .Get_Qpad(TimeCur) / qe;
      double Q_Pad_Up       = aPad_Up       .Get_Qpad(TimeCur) / qe;
      double Q_Pad_Central  = aPad_Central  .Get_Qpad(TimeCur) / qe;
      double Q_Pad_Down     = aPad_Down     .Get_Qpad(TimeCur) / qe;
      double Q_Pad_DownDown = aPad_DownDown .Get_Qpad(TimeCur) / qe;
      
      pTGraph_Pad_UpUp     ->SetPoint( pTGraph_Pad_UpUp    ->GetN() , TimeCur*1.E6 , Q_Pad_UpUp     ) ;
      pTGraph_Pad_Up       ->SetPoint( pTGraph_Pad_Up      ->GetN() , TimeCur*1.E6 , Q_Pad_Up       ) ;
      pTGraph_Pad_Central  ->SetPoint( pTGraph_Pad_Central ->GetN() , TimeCur*1.E6 , Q_Pad_Central  ) ;
      pTGraph_Pad_Down     ->SetPoint( pTGraph_Pad_Down    ->GetN() , TimeCur*1.E6 , Q_Pad_Down     ) ;
      pTGraph_Pad_DownDown ->SetPoint( pTGraph_Pad_DownDown->GetN() , TimeCur*1.E6 , Q_Pad_DownDown ) ; 

    }

    pTMultiGraph->Add( pTGraph_Pad_Central  ,"l");
    pTMultiGraph->Add( pTGraph_Pad_UpUp     ,"l");
    pTMultiGraph->Add( pTGraph_Pad_Up       ,"l");
    pTMultiGraph->Add( pTGraph_Pad_Down     ,"l");
    pTMultiGraph->Add( pTGraph_Pad_DownDown ,"l");

    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "Q_{Pad} Vs Time for YTrue/LY = " ;
    aostringstream << std::setw(7) << std::setprecision(3) << YTrue/LY ;
    pTMultiGraph->SetTitle(aostringstream.str().c_str());  

    pTMultiGraph->GetXaxis()->SetTitle("Time (#mu s)");
    pTMultiGraph->GetYaxis()->SetTitle("Q_{Pad} (e) ");
    pTMultiGraph->GetXaxis()->SetLimits(Tmin*1.E6,Tmax*1.E6);
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    TLegend* pTLegend = new TLegend(0.35,0.55 ,0.65,0.79  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    pTLegend->AddEntry( pTGraph_Pad_UpUp     ,"Pad_UpUp     "  , "l");
    pTLegend->AddEntry( pTGraph_Pad_Up       ,"Pad_Up       "  , "l");
    pTLegend->AddEntry( pTGraph_Pad_Central  ,"Pad_Central  "  , "l");
    pTLegend->AddEntry( pTGraph_Pad_Down     ,"Pad_Down     "  , "l");
    pTLegend->AddEntry( pTGraph_Pad_DownDown ,"Pad_DownDown "  , "l");
    
    pTCanvas->cd( iTurn + 1 );
    pTCanvas->cd( iTurn + 1 )->SetGrid();
    
    pTMultiGraph->Draw("A");
    pTCanvas->Update();

    pTLegend->Draw();       
    pTCanvas->Update();

  }
  
//
  Str_Out_PDF = "OUT_Studies/Draw_01_01.pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

}

void Draw_02()
{
//
  Model_ReadOutGeometry aModel_ReadOutGeometry ;
  Model_Electronics     aModel_Electronics     ;
  Model_Charge1D        aModel_Charge1D        ;

  double LX= aModel_ReadOutGeometry.Get_LX() ;
  double LY= aModel_ReadOutGeometry.Get_LY() ;

  double Time0  = 0. ;

//
  double Tmin =    0. * 1E-9 ;
  double Tmax = 3000. * 1E-9 ; 
  int    Npoints = 1000 ;

//
  std::vector < Pad* > V_Pad ;
  std::vector < int  > V_LineStyle ;
  std::vector < int  > V_LineColor ;
  int iPadUp_Max   = 1 ;
  int iPadDown_Max = 1 ;
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  
  for (int iPadUp = 0 ; iPadUp< iPadUp_Max; iPadUp++){
    int iPadUpLocal = iPadUp_Max-1-iPadUp ;
    std::string PadNameUp ="" ;
    for (int iTem = 0 ; iTem< (iPadUpLocal+1); iTem++){
      PadNameUp +="Up" ;
    }
    Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , PadNameUp , EventNber,EntryNber,ModuleNber , 0 , 16 + iPadUpLocal + 1 , 0. , LX , (double(iPadUpLocal)+0.5)*LY , (double(iPadUpLocal)+1.5)*LY );
    V_Pad.push_back(pPad) ;
    if ( iPadUpLocal % 2 ) {
      V_LineStyle.push_back(1) ;
    }else{
      V_LineStyle.push_back(3) ;
    }
    V_LineColor.push_back(2) ;
  }
  
  Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , "Central " , EventNber,EntryNber,ModuleNber, 0 , 16   , 0. , LX , -0.5*LY ,  0.5*LY );
  V_Pad.push_back(pPad) ;
  V_LineStyle.push_back(1) ;
  V_LineColor.push_back(1) ;
  
  for (int iPadDown = 0 ; iPadDown< iPadDown_Max; iPadDown++){
    int iPadDownLocal = iPadDown ;
    std::string PadNameDown ="" ;
    for (int iTem = 0 ; iTem< (iPadDownLocal+1); iTem++){
      PadNameDown +="Down" ;
    }
    Pad* pPad = new Pad(&aModel_ReadOutGeometry , &aModel_Electronics , &aModel_Charge1D , PadNameDown , EventNber,EntryNber,ModuleNber , 0 , 16 - iPadDown - 1  , 0. , LX , (-double(iPadDownLocal)-1.5)*LY , (-double(iPadDownLocal)-0.5)*LY );
    V_Pad.push_back(pPad) ;
    if ( iPadDownLocal % 2 ) {
      V_LineStyle.push_back(1) ;
    }else{
      V_LineStyle.push_back(3) ;
    }
    V_LineColor.push_back(4) ;
  }

  int iPad_Max = V_Pad.size() ;

//
  TCanvas* pTCanvas =  0 ;
  std::string Str_Out_PDF = "BIDON" ; 

//
  pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
  pTCanvas->Divide(2,2); 

  for (int iTurn = 0 ; iTurn< 4; iTurn++){
    
    double XTrue = 0. ;
    double YTrue = 0. ;
    if (iTurn==1) YTrue = 0.125*LY ;
    if (iTurn==2) YTrue = 0.250*LY ;
    if (iTurn==3) YTrue = 0.375*LY ;

    TMultiGraph* pTMultiGraph = new TMultiGraph();

    TLegend* pTLegend = new TLegend(0.55,0.55 ,0.85,0.79  ); 
    pTLegend->SetFillStyle(1001);
    pTLegend->SetBorderSize(1);

    for (int iPad = 0 ; iPad< iPad_Max; iPad++){
      Pad* pPad = V_Pad[iPad] ;
      pPad->SetSignalModel( Time0 , XTrue , YTrue );
      
      TGraph* pTGraph      = new TGraph  ;  
      pTGraph ->SetLineStyle( V_LineStyle[iPad] ) ; 
      pTGraph ->SetLineColor( V_LineColor[iPad] ) ;

      for (int iPoint = 0 ; iPoint< Npoints ; iPoint++){
        double TimeCur = Tmin + double (iPoint)*(Tmax-Tmin)/double(Npoints-1) ;
        
        double A_Pad = pPad->Get_APad(TimeCur)  ;
        
        pTGraph ->SetPoint( pTGraph->GetN() , TimeCur*1.E9 , A_Pad       ) ;
      
      }
 
      pTMultiGraph->Add( pTGraph  ,"l");

      pTLegend->AddEntry( pTGraph       ,(pPad->Get_PadName()).c_str()  , "l");
      
    }
    
    std::ostringstream aostringstream ;
    aostringstream << std::setiosflags(std::ios::fixed) ;
    aostringstream << "A_{Pad} Vs Time for YTrue/LY = " ;
    aostringstream << std::setw(7) << std::setprecision(3) << YTrue/LY ;

    pTMultiGraph->SetTitle(aostringstream.str().c_str());  

    pTMultiGraph->GetXaxis()->SetTitle("Time (ns)");

    pTMultiGraph->GetYaxis()->SetTitle("A_{Pad}");  

    pTMultiGraph->GetXaxis()->SetLimits(Tmin*1.E9,Tmax*1.E9);
    pTMultiGraph->GetYaxis()->SetTitleOffset(1.5);

    pTCanvas->cd( iTurn + 1 );
    pTCanvas->cd( iTurn + 1 )->SetGrid();
    
    pTMultiGraph->Draw("A");
    pTCanvas->Update();

    pTLegend->Draw();       
    pTCanvas->Update();

  }
  
//
  Str_Out_PDF = "OUT_Studies/Draw_02_01.pdf" ; 
  pTCanvas->SaveAs(Str_Out_PDF.c_str());

}
