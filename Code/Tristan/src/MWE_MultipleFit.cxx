#include <cmath>
#include <numeric>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TLegendEntry.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TAxis.h"




// Draw Energy scan
void DrawOut_Escan(const std::string& inputDir, const std::string& Comment)
{
  gStyle->            SetPadTickX(1);
  gStyle->            SetPadTickY(1);
  int npoint          = 14 ;
  std::string Tag[]   = { "CERN22_Escan_e+_0p5GeV",   "CERN22_Escan_e+_0p75GeV",  "CERN22_Escan_e+_1GeV",   "CERN22_Escan_e+_1p25GeV", "CERN22_Escan_e+_1p5GeV",
                          "CERN22_Escan_mu_0p75GeV",  "CERN22_Escan_mu_1GeV",     "CERN22_Escan_mu_1p5GeV",
                          "CERN22_Escan_pi_0p75GeV",  "CERN22_Escan_pi_1p25GeV",  "CERN22_Escan_pi_1p5GeV", 
                          "CERN22_Escan_p+_1GeV",     "CERN22_Escan_p+_1p25GeV",  "CERN22_Escan_p+_1p5GeV"} ;

  float E_arr[]       = { 0.5,  0.75, 1,  1.25, 1.5,
                          0.75, 1,    1.5,
                          0.75, 1.25, 1.5,
                          1,    1.25, 1.5} ;

  int n[]             = { 0,1,2,3,4,
                          0,1,2,
                          0,1,2,
                          0,1,2,} ;

  std::vector<TF1*>         v_tf1_WF ;
  std::vector<TF1*>         v_tf1_XP ;

  for(int iE = 0 ; iE < npoint ; iE++){
    TFile* ptfile           = new TFile((inputDir + Tag[iE] + "/3_" + Tag[iE] + "_dEdx" + Comment + ".root").c_str()) ;
    v_tf1_WF.                 push_back(ptfile->Get<TF1>("tf1_WFsum")) ;
    v_tf1_XP.                 push_back(ptfile->Get<TF1>("tf1_XP")) ;
    delete ptfile;
  }

  std::string particles[] = {" e^{+}", " #mu^{+}", " #pi^{+}", " p"};
  std::vector<TGraphErrors*> v_pTGE_reso_WF;
  std::vector<TGraphErrors*> v_pTGE_reso_XP;
  std::vector<TGraphErrors*> v_pTGE_mean_WF;
  std::vector<TGraphErrors*> v_pTGE_mean_XP;
  std::vector<TGraphErrors*> v_pTGE_std_WF;
  std::vector<TGraphErrors*> v_pTGE_std_XP;

  std::vector<double>        v_mass{0.511e-3, 105.658e-3, 139.570e-3, 938.272e-3}; // GeV
  std::vector<TF1*>          v_bethebloch;


  for(int i = 0 ; i < 4 ; i++){
    v_pTGE_reso_WF.         push_back(new TGraphErrors());
    v_pTGE_reso_XP.         push_back(new TGraphErrors());
    v_pTGE_mean_WF.         push_back(new TGraphErrors());
    v_pTGE_mean_XP.         push_back(new TGraphErrors());
    v_pTGE_std_WF.          push_back(new TGraphErrors());
    v_pTGE_std_XP.          push_back(new TGraphErrors());
    v_bethebloch.          push_back(BetheBlochExp(0.45, 1.55, v_mass[i], particles[i])); // m & P range in GeV
    // v_bethebloch.          push_back(BetheBloch(0, 2, v_mass[i], particles[i])); // m & P range in GeV
    // if(i==0) v_bethebloch.  push_back(BetheBlochBhabha(0, 2, v_mass[i], particles[i]));
    // else v_bethebloch.      push_back(BetheBloch(0, 2, v_mass[i], particles[i]));
  }

  // Get mean & std
  float mean_WF[npoint] ;
  float mean_XP[npoint] ;
  float dmean_WF[npoint] ;
  float dmean_XP[npoint] ;
  float std_WF[npoint] ;
  float std_XP[npoint] ;
  float dstd_WF[npoint] ;
  float dstd_XP[npoint] ;

  float keV                     = 5.9/(224*1703.74/183); // 5.9 Fe peak energy | 1703 mean MockUp gain | 224 e- created with 5.9keV | 183 e- for 1 ADC | 2.23 scaling correction (to be understood)

  int index;
  for(int iE = 0 ; iE < npoint ; iE++){
    if(iE < 5)        index = 0;
    else if(iE < 8)   index = 1;
    else if(iE < 11)  index = 2;
    else if(iE < 14)  index = 3;

    mean_WF[iE]                 = v_tf1_WF[iE]->    GetParameter(1) ;
    mean_XP[iE]                 = v_tf1_XP[iE]->    GetParameter(1) ;
    std_WF[iE]                  = v_tf1_WF[iE]->    GetParameter(2) ;
    std_XP[iE]                  = v_tf1_XP[iE]->    GetParameter(2) ;
    dmean_WF[iE]                = v_tf1_WF[iE]->    GetParError(1) ;
    dmean_XP[iE]                = v_tf1_XP[iE]->    GetParError(1) ;
    dstd_WF[iE]                 = v_tf1_WF[iE]->    GetParError(2) ;
    dstd_XP[iE]                 = v_tf1_XP[iE]->    GetParError(2) ;

    v_pTGE_reso_WF[index]-> SetPoint      (n[iE], E_arr[iE], std_WF[iE]/mean_WF[iE]*100) ;
    v_pTGE_reso_XP[index]-> SetPoint      (n[iE], E_arr[iE], std_XP[iE]/mean_XP[iE]*100) ;
    v_pTGE_reso_WF[index]-> SetPointError (n[iE], 0,         GetResoError(v_tf1_WF[iE])) ;
    v_pTGE_reso_XP[index]-> SetPointError (n[iE], 0,         GetResoError(v_tf1_XP[iE])) ;

    v_pTGE_mean_WF[index]-> SetPoint      (n[iE], E_arr[iE], mean_WF[iE]*keV) ;
    v_pTGE_mean_XP[index]-> SetPoint      (n[iE], E_arr[iE], mean_XP[iE]*keV) ;
    v_pTGE_mean_WF[index]-> SetPointError (n[iE], 0,         dmean_WF[iE]*keV) ;
    v_pTGE_mean_XP[index]-> SetPointError (n[iE], 0,         dmean_XP[iE]*keV) ;

    v_pTGE_std_WF[index]->  SetPoint      (n[iE], E_arr[iE], std_WF[iE]*keV) ;
    v_pTGE_std_XP[index]->  SetPoint      (n[iE], E_arr[iE], std_XP[iE]*keV) ;
    v_pTGE_std_WF[index]->  SetPointError (n[iE], 0,         dstd_WF[iE]*keV) ;
    v_pTGE_std_XP[index]->  SetPointError (n[iE], 0,         dstd_XP[iE]*keV) ;
  }

  for(int i = 0 ; i < 4 ; i++) v_pTGE_mean_XP[i]->Fit(v_bethebloch[i], "R", "", 0.45, 1.55);


  // Draw
  std::string OutputFile        = inputDir + "CERN22_Escan" + Comment + ".pdf" ;
  std::string OutputFile_Beg    = OutputFile + "(" ;
  std::string OutputFile_End    = OutputFile + ")" ;
  TCanvas* pTCanvas           = new TCanvas("TCanvas", "TCanvas", 1800, 1200) ;
  gStyle->                      SetPadTickX(1);
  gStyle->                      SetPadTickY(1);
  gStyle->                      SetOptStat(0) ;
  gStyle->                      SetGridStyle(1) ;
  pTCanvas->                    cd() ;
  pTCanvas->                    SetRightMargin(0.05);
  float x_leg                 = 0.77;
  float y_leg                 = 0.73;
  float dx_leg                = 0.09;
  float dy_leg                = 0.145;
  TLegend* leg1               = new TLegend(x_leg, y_leg, x_leg+dx_leg, y_leg+dy_leg) ;
  leg1->                        SetBorderSize(0);
  leg1->                        SetFillStyle(0);
  TLegend* leg2               = new TLegend(x_leg+dx_leg, y_leg, x_leg+2*dx_leg, y_leg+dy_leg) ;
  leg2->                        SetBorderSize(0);
  leg2->                        SetFillStyle(0);

  Color_t colors[]  = {kMagenta+1, kBlue, kGreen+1, kRed};
  int     markers[] = {20, 21, 24, 25}; 
  // Resolution
  v_pTGE_reso_WF[0]->                GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_reso_WF[0]->                SetMinimum(4) ;
  v_pTGE_reso_WF[0]->                SetMaximum(10) ;
  v_pTGE_reso_WF[0]->                SetNameTitle("pTGE_reso_WF", "Resolution vs energy with WF method;Energy (GeV);resolution (%)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_reso_WF[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_reso_WF[i]->    Draw("ap") ;
    else v_pTGE_reso_WF[i]->          Draw("p same") ;
    if(i<2) leg1->                    AddEntry(v_pTGE_reso_WF[i], particles[i].c_str(), "ep") ;  
    else    leg2->                    AddEntry(v_pTGE_reso_WF[i], particles[i].c_str(), "ep") ;  
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile_Beg.c_str()) ;

  pTCanvas->                          Clear();
  v_pTGE_reso_XP[0]->                 GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_reso_XP[0]->                 SetMinimum(4) ;
  v_pTGE_reso_XP[0]->                 SetMaximum(10) ;
  v_pTGE_reso_XP[0]->                 SetNameTitle("pTGE_reso_XP", "Resolution vs energy with XP method;Energy (GeV);resolution (%)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_reso_XP[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_reso_XP[i]->    Draw("ap") ;
    else v_pTGE_reso_XP[i]->          Draw("p same") ;
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile.c_str()) ;


  // Mean
  pTCanvas->                          Clear();
  v_pTGE_mean_WF[0]->                 GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_mean_WF[0]->                 SetMinimum(0.5) ;
  v_pTGE_mean_WF[0]->                 SetMaximum(5) ;
  v_pTGE_mean_WF[0]->                 SetNameTitle("pTGE_mean_WF", "Mean vs energy with WF method;Energy (GeV);mean (keV/cm)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_mean_WF[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_mean_WF[i]->    Draw("ap") ;
    else v_pTGE_mean_WF[i]->          Draw("p same") ;
    v_bethebloch[i]->                 SetLineColor(colors[i]);
    v_bethebloch[i]->                 Draw("same");
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile_Beg.c_str()) ;

  pTCanvas->                          Clear();
  v_pTGE_mean_XP[0]->                 GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_mean_XP[0]->                 SetMinimum(0.5) ;
  v_pTGE_mean_XP[0]->                 SetMaximum(5) ;
  v_pTGE_mean_XP[0]->                SetNameTitle("pTGE_mean_XP", "Mean vs energy with XP method;Energy (GeV);mean (keV/cm)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_mean_XP[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_mean_XP[i]->    Draw("ap") ;
    else v_pTGE_mean_XP[i]->          Draw("p same") ;
    v_bethebloch[i]->                 Draw("same");
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile.c_str()) ;



  // Bethe-Bloch
  pTCanvas->                          Clear();
  v_bethebloch[0]->                   SetMinimum(0.5) ;
  v_bethebloch[0]->                   SetMaximum(5) ;
  v_bethebloch[0]->                   SetTitle("Bethe-Bloch for different particles;Energy (GeV);mean (keV/cm)") ;
  v_bethebloch[0]->                   Draw() ;
  for(int i = 1 ; i < 4 ; i++)v_bethebloch[i]-> Draw("same") ; 
  leg1->                              Draw() ;
  leg2->                              Draw() ;
  pTCanvas->                          SaveAs(OutputFile.c_str()) ;



  // Std
  pTCanvas->                          Clear();
  v_pTGE_std_WF[0]->                  GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_std_WF[0]->                  SetMinimum(0.05) ;
  v_pTGE_std_WF[0]->                  SetMaximum(0.15) ;
  v_pTGE_std_WF[0]->                  SetNameTitle("pTGE_std_WF", "Std vs energy with WF method;Energy (GeV);std (keV/cm)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_std_WF[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_std_WF[i]->    Draw("ap") ;
    else v_pTGE_std_WF[i]->          Draw("p same") ;
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile.c_str()) ;

  pTCanvas->                          Clear();
  v_pTGE_std_XP[0]->                  GetXaxis()->SetLimits(0.45, 1.55) ;
  v_pTGE_std_XP[0]->                  SetMinimum(0.05) ;
  v_pTGE_std_XP[0]->                  SetMaximum(0.15) ;
  v_pTGE_std_XP[0]->                  SetNameTitle("pTGE_std_XP", "Std vs energy with XP method;Energy (GeV);std (keV/cm)") ;
  for(int i = 0 ; i < 4 ; i++){
    Graphic_setup(v_pTGE_std_XP[i], 3, markers[i], colors[i], 1, colors[i]) ;
    if(i == 0) v_pTGE_std_XP[i]->     Draw("ap") ;
    else v_pTGE_std_XP[i]->           Draw("p same") ;
  }
  leg1->                               Draw() ;
  leg2->                               Draw() ;
  pTCanvas->                          SaveAs(OutputFile_End.c_str()) ;


  // Delete
  delete                        pTCanvas   ;
  delete                        leg1 ;
  delete                        leg2 ;

  for(int i = 0 ; i < (int)v_pTGE_mean_WF.size() ; i++){
    delete v_pTGE_reso_WF[i]; v_pTGE_reso_WF[i] = 0 ;
    delete v_pTGE_reso_XP[i]; v_pTGE_reso_XP[i] = 0 ;
    delete v_pTGE_mean_WF[i]; v_pTGE_mean_WF[i] = 0 ;
    delete v_pTGE_mean_XP[i]; v_pTGE_mean_XP[i] = 0 ;
    delete v_pTGE_std_WF[i]; v_pTGE_std_WF[i] = 0 ;
    delete v_pTGE_std_XP[i]; v_pTGE_std_XP[i] = 0 ;
    delete v_bethebloch[i] ; v_bethebloch[i] = 0 ;
  }
  v_pTGE_reso_WF.               clear();
  v_pTGE_reso_XP.               clear();
  v_pTGE_mean_WF.               clear();
  v_pTGE_mean_XP.               clear();
  v_pTGE_std_WF.                clear();
  v_pTGE_std_XP.                clear();
  v_bethebloch.                 clear();

  v_tf1_WF.                     clear() ;
  v_tf1_XP.                     clear() ;
}



double GetResoError(TF1* tf1){
  return GetResoError(tf1, 1, 2) ;
}
double GetResoError(TF1* tf1, const int& mu, const int& sigma){
    double mean     = tf1->GetParameter(mu) ;
    double std      = tf1->GetParameter(sigma) ;
    double dmean    = tf1->GetParError(mu) ;
    double dstd     = tf1->GetParError(sigma) ;
    return (std*dmean/(mean*mean) + dstd/mean) * 100 ;
}



// Set graphic settings of a TGraphErrors
void Graphic_setup(TGraphErrors* ptge, Size_t markersize, Style_t markerstyle, Color_t markercolor, Width_t linewidth, Color_t linecolor){
  ptge->SetMarkerSize(markersize) ;
  ptge->SetMarkerStyle(markerstyle) ;
  ptge->SetMarkerColor(markercolor) ;
  ptge->SetLineWidth(linewidth) ;
  ptge->SetLineColor(linecolor) ;
}



TF1* BetheBlochExp(const float& Pmin, const float& Pmax, const double& M, const std::string& particle){
  /*  Input: Pmin & Pmax GeV | m GeV 
      Output: keV/cm */
  double M2                       = pow(M,2); // GeV^2
  double par[] = {1.65179e+02, 3.62857e+00, 3.18209e-02, 2.07081e+00, -7.14413e-01};


  const char* formula             = "[1]/pow(x/sqrt(x*x+[0]),[4]) * ( [2] - pow(x/sqrt(x*x+[0]),[4]) - log([3]+ pow(x*x/[0], [5])) )" ;
  TF1* dEdx                       = new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->                            SetParameters(M2, par[0], par[1], par[2], par[3], par[4]);
  dEdx->                            FixParameter(0, M2);

  dEdx->                            SetTitle(Form("Experimental Bethe-Bloch for%s;Energy (GeV);mean (keV/cm)", particle.c_str()));
  return dEdx ; // keV/cm
}