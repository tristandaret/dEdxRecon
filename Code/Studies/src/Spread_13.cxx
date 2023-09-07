#include "Studies/Spread_13.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_Charge1D.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ReadOutGeometry.h"

#include "SignalShape/SC_0D_VS_x_y_rc_dd.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd.h"

#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_A.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_ResX.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_ResY.h"

#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tbin.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_ResX.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_ResY.h"

#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX.h"
#include "SignalShape/SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResY.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TRandom3.h"

void Spread_13()
{
//  
  TCanvas* pTCanvas =  0 ;

// 
  gStyle->SetOptStat(0);
  
//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  double LX = pModel_ReadOutGeometry->Get_LX      () ; 
  double LY = pModel_ReadOutGeometry->Get_LY      () ; 
  
  double XMINcm = 0.        ;
  double XMAXcm = LX*1E2/2. ;
  double EPSX   = (LX*1E2/2.)/100. ;

  double YMINcm = 0.        ;
  double YMAXcm = LY*1E2/2. ;
  double EPSY   = (LY*1E2/2.)/100. ;

//
  double Conv_MicroSec_2_TimeBin = 1000./40. ;

  std::string FILE_SC_0D_VS_x_y_rc_dd = "Data_SignalTime_0D/Load_0D_400ns_VS_x_y_rc_dd.csv" ;
  SC_0D_VS_x_y_rc_dd aSC_0D_VS_x_y_rc_dd(FILE_SC_0D_VS_x_y_rc_dd,Conv_MicroSec_2_TimeBin) ;
    
  std::vector < std::string > V_Pad ;
  V_Pad.push_back("NN");
  V_Pad.push_back("NE");
  V_Pad.push_back("EE");
  
//  
  for (int iRC = 0 ; iRC< 4 ; iRC++){
//for (int iRC = 0 ; iRC< 1 ; iRC++){
    double RC = 50. ;
    if (iRC==0)  RC =  50. ;
    if (iRC==1)  RC = 100. ;
    if (iRC==2)  RC = 150. ;
    if (iRC==3)  RC = 200. ;
    double DD = 0.  ;
    SC_0D_VS_x_y_rc_dd_SET_rc_dd aSC_0D_VS_x_y_rc_dd_SET_rc_dd(&aSC_0D_VS_x_y_rc_dd,RC,DD);
    
//
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_A a_A_LE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"LE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_A a_A_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_A a_A_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_A a_A_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE");

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha a_Alpha_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha a_Alpha_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha a_Alpha_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE");

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadX_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE",XMINcm,XMAXcm,EPSX,0);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadX_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN",XMINcm,XMAXcm,EPSX,0);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadX_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE",XMINcm,XMAXcm,EPSX,0);

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadY_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE",YMINcm,YMAXcm,EPSY,1);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadY_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN",YMINcm,YMAXcm,EPSY,1);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dAlpha a_dAlphadY_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE",YMINcm,YMAXcm,EPSY,1);

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_ResX a_Alpha_ResX(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Alpha_ResY a_Alpha_ResY(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);

//
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tbin a_Tbin_LE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"LE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tbin a_Tbin_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tbin a_Tbin_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tbin a_Tbin_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE");

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau a_Tau_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau a_Tau_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN");
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau a_Tau_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE");

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudX_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE",XMINcm,XMAXcm,EPSX,0);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudX_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN",XMINcm,XMAXcm,EPSX,0);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudX_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE",XMINcm,XMAXcm,EPSX,0);

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudY_EE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"EE",YMINcm,YMAXcm,EPSY,1);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudY_NN(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NN",YMINcm,YMAXcm,EPSY,1);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_dTau a_dTaudY_NE(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,"NE",YMINcm,YMAXcm,EPSY,1);

    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_ResX a_Tau_ResX(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_Tau_ResY a_Tau_ResY(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);

//
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResX a_ResX(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);
    SC_0D_VS_x_y_rc_dd_SET_rc_dd_4TF2_ResY a_ResY(&aSC_0D_VS_x_y_rc_dd_SET_rc_dd,V_Pad,XMINcm,XMAXcm,EPSX,YMINcm,YMAXcm,EPSY);

//
    double YP = 0. ;
    
    double XP_Min = 0. ;
    double XP_Max = LX/2. ;
    double iXP_Max = 10 ;
    
    for (int iXP = 0 ; iXP< iXP_Max ; iXP++){
      double XP = XP_Min + double(iXP)*(XP_Max-XP_Min)/double(iXP_Max-1) ;

      double A_LE = a_A_LE.Eval(XP*1E2,YP*1E2) ;
      double A_EE = a_A_EE.Eval(XP*1E2,YP*1E2) ;
      double A_NN = a_A_NN.Eval(XP*1E2,YP*1E2) ;
      double A_NE = a_A_NE.Eval(XP*1E2,YP*1E2) ;
      
      double T_LE = a_Tbin_LE.Eval(XP*1E2,YP*1E2) ;
      double T_EE = a_Tbin_EE.Eval(XP*1E2,YP*1E2) ;
      double T_NN = a_Tbin_NN.Eval(XP*1E2,YP*1E2) ;
      double T_NE = a_Tbin_NE.Eval(XP*1E2,YP*1E2) ;
      
      std::cout 
        << std::setw(5) <<  " XP   "   << std::setw(7) << std::setprecision(4) << XP*1E2
        << std::setw(5) <<  " A_LE "   << std::setw(7) << std::setprecision(1) << A_LE 
        << std::setw(5) <<  " A_EE "   << std::setw(7) << std::setprecision(1) << A_EE 
        << std::setw(5) <<  " A_NN "   << std::setw(7) << std::setprecision(1) << A_NN 
        << std::setw(5) <<  " A_NE "   << std::setw(7) << std::setprecision(1) << A_NE 
        << std::setw(5) <<  " A-RESX " << std::setw(7) << std::setprecision(1) << a_Alpha_ResX.Eval(XP*1E2,YP*1E2) 
        << std::setw(5) <<  " A-RESY " << std::setw(7) << std::setprecision(1) << a_Alpha_ResY.Eval(XP*1E2,YP*1E2) 
        << std::setw(5) <<  " T-RESX " << std::setw(7) << std::setprecision(1) << a_Tau_ResX.Eval(XP*1E2,YP*1E2) 
        << std::setw(5) <<  " T-RESY " << std::setw(7) << std::setprecision(1) << a_Tau_ResY.Eval(XP*1E2,YP*1E2) 
        << std::setw(5) <<  " RESX "   << std::setw(7) << std::setprecision(1) << a_ResX.Eval(XP*1E2,YP*1E2) 
        << std::setw(5) <<  " RESY "   << std::setw(7) << std::setprecision(1) << a_ResY.Eval(XP*1E2,YP*1E2) 
        << std::endl ;
      std::cout 
        << std::setw(5) <<  "      " << std::setw(7) << std::setprecision(4) << " "
        << std::setw(5) <<  " T_LE " << std::setw(7) << std::setprecision(1) << T_LE 
        << std::setw(5) <<  " T_EE " << std::setw(7) << std::setprecision(1) << T_EE 
        << std::setw(5) <<  " T_NN " << std::setw(7) << std::setprecision(1) << T_NN 
        << std::setw(5) <<  " T_NE " << std::setw(7) << std::setprecision(1) << T_NE 
        << std::endl ;
    }

//
    std::ostringstream aostringstream_OUT ;
    aostringstream_OUT << std::setiosflags(std::ios::fixed) ;
    aostringstream_OUT << "OUT_Studies/Spread_13_" ;
    aostringstream_OUT << "RC_" <<std::setprecision(0) << RC;
    aostringstream_OUT << ".pdf" ;
    std::string Str_Out_PDF = aostringstream_OUT.str() ; 

    std::string Str_Out_PDF_Beg = Str_Out_PDF + "(" ;
    std::string Str_Out_PDF_End = Str_Out_PDF + ")" ;
    
//Page 1
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Print(Str_Out_PDF_Beg.c_str());


//Page 2
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_A_NN = new TF2("A_{NN};x (cm); y (cm); A (ADC)",&a_A_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_A_NE = new TF2("A_{NE};x (cm); y (cm); A (ADC)",&a_A_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_NE->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(3);
        pTCanvas->cd(3)->SetPhi(-30.);
        TF2* pTF2_A_LE = new TF2("A_{LE};x (cm); y (cm); A (ADC)",&a_A_LE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_LE->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(4);
        TF2* pTF2_A_EE = new TF2("A_{EE};x (cm); y (cm); A (ADC)",&a_A_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 3
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_A_NN->Draw("CONT1Z");
        pTCanvas->Update();

        TF2* pTF2_A_NN_Twin = new TF2("A_{NN};x (cm); y (cm); A (ADC)",&a_A_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        double level_MinAmplitude[1] ; level_MinAmplitude[0] = 70. ;
        pTF2_A_NN_Twin->SetContour(1,level_MinAmplitude);
        pTF2_A_NN_Twin->SetLineColor(2);
        pTF2_A_NN_Twin->SetLineStyle(2);
        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_A_NE->Draw("CONT1Z");
        pTCanvas->Update();

        TF2* pTF2_A_NE_Twin = new TF2("A_{NE};x (cm); y (cm); A (ADC)",&a_A_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_NE_Twin->SetContour(1,level_MinAmplitude);
        pTF2_A_NE_Twin->SetLineColor(2);
        pTF2_A_NE_Twin->SetLineStyle(2);
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(3);
        pTCanvas->cd(3)->SetPhi(-30.);
        pTF2_A_LE->Draw("CONT1Z");
        pTCanvas->Update();

        TF2* pTF2_A_LE_Twin = new TF2("A_{LE};x (cm); y (cm); A (ADC)",&a_A_LE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_LE_Twin->SetContour(1,level_MinAmplitude);
        pTF2_A_LE_Twin->SetLineColor(2);
        pTF2_A_LE_Twin->SetLineStyle(2);
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(4);
        pTF2_A_EE->Draw("CONT1Z");
        pTCanvas->Update();

        TF2* pTF2_A_EE_Twin = new TF2("A_{EE};x (cm); y (cm); A (ADC)",&a_A_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_A_EE_Twin->SetContour(1,level_MinAmplitude);
        pTF2_A_EE_Twin->SetLineColor(2);
        pTF2_A_EE_Twin->SetLineStyle(2);
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 4
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_Alpha_NN = new TF2("#alpha_{NN};x (cm); y (cm); #alpha",&a_Alpha_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Alpha_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_Alpha_NE = new TF2("#alpha_{NE};x (cm); y (cm); #alpha",&a_Alpha_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Alpha_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_Alpha_EE = new TF2("#alpha_{EE};x (cm); y (cm); #alpha",&a_Alpha_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Alpha_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 5
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_Alpha_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_Alpha_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_Alpha_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 6
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_dAlphadX_NN = new TF2("d#alpha_{NN}/dx;x (cm); y (cm); ",&a_dAlphadX_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadX_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_dAlphadX_NE = new TF2("d#alpha_{NE}/dx;x (cm); y (cm); ",&a_dAlphadX_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadX_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_dAlphadX_EE = new TF2("d#alpha_{EE}/dx;x (cm); y (cm); ",&a_dAlphadX_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadX_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 7
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_dAlphadX_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTF2_dAlphadX_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_dAlphadX_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 8
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_dAlphadY_NN = new TF2("d#alpha_{NN}/dy;x (cm); y (cm); ",&a_dAlphadY_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadY_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_dAlphadY_NE = new TF2("d#alpha_{NE}/dy;x (cm); y (cm); ",&a_dAlphadY_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadY_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_dAlphadY_EE = new TF2("d#alpha_{EE}/dy;x (cm); y (cm); ",&a_dAlphadY_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dAlphadY_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 9
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_dAlphadY_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_dAlphadY_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_dAlphadY_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 10
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_Tbin_NN = new TF2("T_{NN};x (cm); y (cm); T (Time Bin)",&a_Tbin_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tbin_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_Tbin_NE = new TF2("T_{NE};x (cm); y (cm); T (Time Bin)",&a_Tbin_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tbin_NE->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(3);
        pTCanvas->cd(3)->SetPhi(-30.);
        TF2* pTF2_Tbin_LE = new TF2("T_{LE};x (cm); y (cm); T (Time Bin)",&a_Tbin_LE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tbin_LE->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(4);
        TF2* pTF2_Tbin_EE = new TF2("T_{EE};x (cm); y (cm); T (Time Bin)",&a_Tbin_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tbin_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 11
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_Tbin_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_Tbin_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(3);
        pTF2_Tbin_LE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(4);
        pTF2_Tbin_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 12
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_Tau_NN = new TF2("#tau_{NN};x (cm); y (cm); #tau (Time Bin)",&a_Tau_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tau_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_Tau_NE = new TF2("#tau_{NE};x (cm); y (cm); #tau (Time Bin)",&a_Tau_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tau_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_Tau_EE = new TF2("#tau_{EE};x (cm); y (cm); #tau (Time Bin)",&a_Tau_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tau_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 13
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_Tau_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_Tau_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_Tau_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 14
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_dTaudX_NN = new TF2("d#tau_{NN}/dx;x (cm); y (cm); ",&a_dTaudX_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudX_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_dTaudX_NE = new TF2("d#tau_{NE}/dx;x (cm); y (cm); ",&a_dTaudX_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudX_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_dTaudX_EE = new TF2("d#tau_{EE}/dx;x (cm); y (cm); ",&a_dTaudX_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudX_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 15
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_dTaudX_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_dTaudX_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_dTaudX_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 16
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_dTaudY_NN = new TF2("d#tau_{NN}/dy;x (cm); y (cm); ",&a_dTaudY_NN,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudY_NN->Draw("surf1");
        pTCanvas->Update();


      pTCanvas->cd(2);
        TF2* pTF2_dTaudY_NE = new TF2("d#tau_{NE}/dy;x (cm); y (cm); ",&a_dTaudY_NE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudY_NE->Draw("surf1");
        pTCanvas->Update();


     pTCanvas->cd(4);
        TF2* pTF2_dTaudY_EE = new TF2("d#tau_{EE}/dy;x (cm); y (cm); ",&a_dTaudY_EE,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_dTaudY_EE->Draw("surf1");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 17
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        pTF2_dTaudY_NN->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTCanvas->Update();


      pTCanvas->cd(2);
        pTF2_dTaudY_NE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


     pTCanvas->cd(4);
        pTF2_dTaudY_EE->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();


    pTCanvas->Print(Str_Out_PDF.c_str());
    
//Page 18
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      std::ostringstream aostringstream_page_18_a ;
      aostringstream_page_18_a << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_18_a << "#sigma_{X}^{#alpha}" ;
      aostringstream_page_18_a << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_18_a << ";x (cm); y (cm); " ;
      std::string Tit_page_18_a = aostringstream_page_18_a.str() ; 

      std::ostringstream aostringstream_page_18_b ;
      aostringstream_page_18_b << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_18_b << "#sigma_{Y}^{#alpha}" ;
      aostringstream_page_18_b << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_18_b << ";x (cm); y (cm); " ;
      std::string Tit_page_18_b = aostringstream_page_18_b.str() ; 

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_Alpha_ResX = new TF2(Tit_page_18_a.c_str(),&a_Alpha_ResX,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Alpha_ResX->Draw("surf1");
        pTF2_Alpha_ResX->SetMaximum(120.);
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTF2_Alpha_ResX->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

      pTCanvas->cd(3);
        TF2* pTF2_Alpha_ResY = new TF2(Tit_page_18_b.c_str(),&a_Alpha_ResY,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Alpha_ResY->Draw("surf1");
        pTF2_Alpha_ResY->SetMaximum(120.);
        pTCanvas->Update();

      pTCanvas->cd(4);
        pTF2_Alpha_ResY->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 19
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      std::ostringstream aostringstream_page_19_a ;
      aostringstream_page_19_a << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_19_a << "#sigma_{X}^{#tau}" ;
      aostringstream_page_19_a << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_19_a << ";x (cm); y (cm); " ;
      std::string Tit_page_19_a = aostringstream_page_19_a.str() ; 

      std::ostringstream aostringstream_page_19_b ;
      aostringstream_page_19_b << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_19_b << "#sigma_{Y}^{#tau}" ;
      aostringstream_page_19_b << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_19_b << ";x (cm); y (cm); " ;
      std::string Tit_page_19_b = aostringstream_page_19_b.str() ; 

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_Tau_ResX = new TF2(Tit_page_19_a.c_str(),&a_Tau_ResX,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tau_ResX->Draw("surf1");
        pTF2_Tau_ResX->SetMaximum(1000.);
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTF2_Tau_ResX->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

      pTCanvas->cd(3);
        TF2* pTF2_Tau_ResY = new TF2(Tit_page_19_b.c_str(),&a_Tau_ResY,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_Tau_ResY->Draw("surf1");
        pTF2_Tau_ResY->SetMaximum(1000.);
        pTCanvas->Update();

      pTCanvas->cd(4);
        pTF2_Tau_ResY->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());

//Page 20
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

      std::ostringstream aostringstream_page_20_a ;
      aostringstream_page_20_a << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_20_a << "#sigma_{X}^{#alpha+#tau}" ;
      aostringstream_page_20_a << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_20_a << ";x (cm); y (cm); " ;
      std::string Tit_page_20_a = aostringstream_page_20_a.str() ; 

      std::ostringstream aostringstream_page_20_b ;
      aostringstream_page_20_b << std::setiosflags(std::ios::fixed) ;
      aostringstream_page_20_b << "#sigma_{Y}^{#alpha+#tau}" ;
      aostringstream_page_20_b << ", RC =" << std::setw(5) << std::setprecision(0) << RC;
      aostringstream_page_20_b << ";x (cm); y (cm); " ;
      std::string Tit_page_20_b = aostringstream_page_20_b.str() ; 

      pTCanvas->Divide(2,2);
      
      pTCanvas->cd(1);
        TF2* pTF2_ResX = new TF2(Tit_page_20_a.c_str(),&a_ResX,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_ResX->Draw("surf1");
        pTF2_ResX->SetMaximum(120.);
        pTCanvas->Update();

      pTCanvas->cd(2);
        pTF2_ResX->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

      pTCanvas->cd(3);
        TF2* pTF2_ResY = new TF2(Tit_page_20_b.c_str(),&a_ResY,XMINcm,XMAXcm,YMINcm,YMAXcm,4);
        pTF2_ResY->Draw("surf1");
        pTF2_ResY->SetMaximum(120.);
        pTCanvas->Update();

      pTCanvas->cd(4);
        pTF2_ResY->Draw("CONT1Z");
        pTCanvas->Update();

        pTF2_A_NN_Twin->Draw("Same CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();

    pTCanvas->Print(Str_Out_PDF.c_str());


//Page 20
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;

        pTF2_A_NN_Twin->Draw("CONT2");
        pTF2_A_NE_Twin->Draw("Same CONT2");
        pTF2_A_LE_Twin->Draw("Same CONT2");
        pTF2_A_EE_Twin->Draw("Same CONT2");
        pTCanvas->Update();
    
    pTCanvas->Print(Str_Out_PDF.c_str());

//Page LAST
    pTCanvas =  new TCanvas("Bla", "Bla", 180, 10, 1200, 1200) ;
    pTCanvas->Print(Str_Out_PDF_End.c_str());
  
  }
  
}
