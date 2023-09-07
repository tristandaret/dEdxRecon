// Draw resolution as function of MC NoOpt Z scan
void DrawOut_Zscan_MC(const std::string& inputDir, const std::string& Comment)
{
  gStyle->                      SetPadTickX(1);
  gStyle->                      SetPadTickY(1);
  int ipoint              = 0 ;
  int nz                  = 9 ;

  // Vectors of TF1s
  std::vector<TF1*>         v_tf1_WF ;
  std::vector<TF1*>         v_tf1_XP ;

  for(int iz = 0 ; iz < nz ; iz++){
    TFile* pTFile           = TFile::Open(TString(inputDir + "/CERN23_MC_z" + (iz+1) + "00/3_CERN23_MC_z" + (iz+1) + "00_dEdx" + Comment + ".root")) ;
    v_tf1_WF.                 push_back(pTFile->Get<TF1>("tf1_WFtrunc_0")) ;
    v_tf1_XP.                 push_back(pTFile->Get<TF1>("tf1_XP_0")) ;
  }

  TGraphErrors* pTGE_reso_WF    = new TGraphErrors() ;
  TGraphErrors* pTGE_reso_XP    = new TGraphErrors() ;

  TGraphErrors* pTGE_mean_WF    = new TGraphErrors() ;
  TGraphErrors* pTGE_mean_XP    = new TGraphErrors() ;

  TGraphErrors* pTGE_std_WF     = new TGraphErrors() ;
  TGraphErrors* pTGE_std_XP     = new TGraphErrors() ;

  // Get mean & std
  float mean_WF[nz] ;
  float mean_XP[nz] ;
  float dmean_WF[nz] ;
  float dmean_XP[nz] ;
  float std_WF[nz] ;
  float std_XP[nz] ;
  float dstd_WF[nz] ;
  float dstd_XP[nz] ;
  for(int iz = 0 ; iz < nz ; iz++){
    mean_WF[iz]                 = v_tf1_WF[iz]->    GetParameter(1) ;
    mean_XP[iz]                 = v_tf1_XP[iz]->    GetParameter(1) ;
    std_WF[iz]                  = v_tf1_WF[iz]->    GetParameter(2) ;
    std_XP[iz]                  = v_tf1_XP[iz]->    GetParameter(2) ;
    dmean_WF[iz]                = v_tf1_WF[iz]->    GetParError(1) ;
    dmean_XP[iz]                = v_tf1_XP[iz]->    GetParError(1) ;
    dstd_WF[iz]                 = v_tf1_WF[iz]->    GetParError(2) ;
    dstd_XP[iz]                 = v_tf1_XP[iz]->    GetParError(2) ;

    pTGE_reso_WF->  SetPoint      (ipoint, (iz+1)*100, std_WF[iz]/mean_WF[iz]*100) ;
    pTGE_reso_XP->  SetPoint      (ipoint, (iz+1)*100, std_XP[iz]/mean_XP[iz]*100) ;
    pTGE_reso_WF->  SetPointError (ipoint, 0,         GetResoError(v_tf1_WF[iz])) ;
    pTGE_reso_XP->  SetPointError (ipoint, 0,         GetResoError(v_tf1_XP[iz])) ;

    pTGE_mean_WF->  SetPoint      (ipoint, (iz+1)*100, mean_WF[iz]) ;
    pTGE_mean_XP->  SetPoint      (ipoint, (iz+1)*100, mean_XP[iz]) ;
    pTGE_mean_WF->  SetPointError (ipoint, 0,         dmean_WF[iz]) ;
    pTGE_mean_XP->  SetPointError (ipoint, 0,         dmean_XP[iz]) ;

    pTGE_std_WF->  SetPoint       (ipoint, (iz+1)*100, std_WF[iz]) ;
    pTGE_std_XP->  SetPoint       (ipoint, (iz+1)*100, std_XP[iz]) ;
    pTGE_std_WF->  SetPointError  (ipoint, 0,         dstd_WF[iz]) ;
    pTGE_std_XP->  SetPointError  (ipoint, 0,         dstd_XP[iz]) ;

    ipoint++ ;
  }


  // Draw
  std::string OutputFile      = inputDir + "/MC_Zscan_Resolution" + Comment + ".pdf" ;
  std::string OutputFile_Beg  = OutputFile + "(" ;
  std::string OutputFile_End  = OutputFile + ")" ;
  TCanvas* pTCanvas           = new TCanvas("TCanvas", "TCanvas", 1800, 1200) ;
  gStyle->                      SetOptStat(0) ;
  pTCanvas->                    cd() ;
  TLegend* leg                = new TLegend(0.8,0.68,0.98,0.98) ;


  // Resolution
  pTGE_reso_WF->                GetXaxis()->SetLimits(0, 1000) ;
  pTGE_reso_WF->                SetMinimum(6) ;
  pTGE_reso_WF->                SetMaximum(12) ;
  pTGE_reso_WF->                SetNameTitle("pTGE_reso_WF", "MC: Resolution vs drift distance;drift distance z (mm);resolution (%)") ;
  Graphic_setup(pTGE_reso_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_reso_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_reso_WF->                Draw("ap") ;
  pTGE_reso_XP->                Draw("p same") ;
  leg->                         AddEntry(pTGE_reso_WF, "WF ", "ep") ;  
  leg->                         AddEntry(pTGE_reso_XP, "XP ", "ep") ;  
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile_Beg.c_str()) ;

  // Mean
  pTCanvas->                    Clear() ;
  pTGE_mean_WF->                SetMinimum(700) ;
  pTGE_mean_WF->                SetMaximum(1210) ;
  pTGE_mean_WF->                SetNameTitle("pTGE_mean_WF", "MC: Mean vs drift distance;drift distance z (mm);Mean (ADC count)") ;
  Graphic_setup(pTGE_mean_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_mean_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_mean_WF->                Draw("ap") ;
  pTGE_mean_XP->                Draw("p same") ;
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile.c_str()) ;

  // Standard deviation
  pTCanvas->                    Clear() ;
  pTGE_std_WF->                 SetMinimum(50) ;
  pTGE_std_WF->                 SetMaximum(121) ;
  pTGE_std_WF->                 SetNameTitle("pTGE_std_WF", "MC: Std vs drift distance;drift distance z (mm);std (ADC count)") ;
  Graphic_setup(pTGE_std_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_std_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_std_WF->                 Draw("ap") ;
  pTGE_std_XP->                 Draw("p same") ;
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile_End.c_str()) ;

  // Delete
  delete                        pTCanvas   ;
  delete                        leg ;

  v_tf1_WF.                     clear() ;
  v_tf1_XP.                     clear() ;
}












// DrawOut dE/dx distribution for systematics
void DrawOut_systematics(const std::string& OutDir, const std::string& Tag, const std::string& Comment){
  int statXin                   = gStyle->GetStatX() ;
  int statYin                   = gStyle->GetStatY() ;

  // Reference
  TFile* pTFile_XP             = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + ".root")) ;
  TH1F* h1f_XP                  = pTFile_XP->Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_XP                  = pTFile_XP->Get<TF1>("tf1_XP_0") ;
  h1f_XP->                        SetLineColor(kMagenta+2) ;
  tf1_XP->                        SetLineColor(kMagenta-3) ;
  h1f_XP->                        SetLineWidth(3) ;
  tf1_XP->                        SetLineWidth(2) ;
  TPaveStats*                     pStat_XP ;
  int XPMax                     = h1f_XP->GetMaximum() ;


  // RC
  TFile* pTFile_RC87            = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_RC87.root")) ;
  TH1F* h1f_RC87                = pTFile_RC87-> Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_RC87                = pTFile_RC87-> Get<TF1>("tf1_XP_0") ;
  h1f_RC87->                      SetTitle("Mean dE/dx: RC dependence;mean dE/dx;Counts") ;
  h1f_RC87->                      SetLineColor(kBlue+2) ;
  h1f_RC87->                      SetLineWidth(3) ;
  tf1_RC87->                      SetLineColor(kBlue-3) ;
  tf1_RC87->                      SetLineWidth(2) ;
  TPaveStats*                     pStat_RC87 ;
  int RC87Max                   = h1f_RC87->GetMaximum() ;

  TFile* pTFile_RC147           = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_RC147.root")) ;
  TH1F* h1f_RC147               = pTFile_RC147->Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_RC147               = pTFile_RC147->Get<TF1>("tf1_XP_0") ;
  h1f_RC147->                     SetLineColor(kRed+2) ;
  h1f_RC147->                     SetLineWidth(3) ;
  tf1_RC147->                     SetLineColor(kRed-3) ;
  tf1_RC147->                     SetLineWidth(2) ;
  TPaveStats*                     pStat_RC147 ;
  int RC147Max                  = h1f_RC147->GetMaximum() ;


  // Dt
  TFile* pTFile_Dt286           = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_Dt286.root")) ;
  TH1F* h1f_Dt286               = pTFile_Dt286-> Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_Dt286               = pTFile_Dt286-> Get<TF1>("tf1_XP_0") ;
  h1f_Dt286->                     SetTitle("Mean dE/dx: Dt dependence;mean dE/dx;Counts") ;
  h1f_Dt286->                     SetLineColor(kBlue+2) ;
  h1f_Dt286->                     SetLineWidth(3) ;
  tf1_Dt286->                     SetLineColor(kBlue-3) ;
  tf1_Dt286->                     SetLineWidth(2) ;
  TPaveStats*                     pStat_Dt286 ;
  int Dt286Max                  = h1f_Dt286->GetMaximum() ;

  TFile* pTFile_Dt350           = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_Dt350.root")) ;
  TH1F* h1f_Dt350               = pTFile_Dt350-> Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_Dt350               = pTFile_Dt350-> Get<TF1>("tf1_XP_0") ;
  h1f_Dt350->                     SetLineColor(kRed+2) ;
  h1f_Dt350->                     SetLineWidth(3) ;
  tf1_Dt350->                     SetLineColor(kRed-3) ;
  tf1_Dt350->                     SetLineWidth(2) ;
  TPaveStats*                     pStat_Dt350 ;
  int Dt350Max                  = h1f_Dt350->GetMaximum() ;


  // Z drift distance
  TFile* pTFile_Zm10 ;
  TH1F* h1f_Zm10 ;
  TF1*  tf1_Zm10 ;
  TPaveStats* pStat_Zm10 ;
  int Zm10Max = 0;
  if(Tag.find("m40") == std::string::npos){
    pTFile_Zm10                   = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_Zm10.root")) ;
    h1f_Zm10                      = pTFile_Zm10-> Get<TH1F>("h1f_XP_0") ;
    tf1_Zm10                      = pTFile_Zm10-> Get<TF1>("tf1_XP_0") ;
    h1f_Zm10->                      SetTitle("Mean dE/dx: Z_{drift} dependence;mean dE/dx;Counts") ;
    h1f_Zm10->                      SetLineColor(kBlue+2) ;
    h1f_Zm10->                      SetLineWidth(3) ;
    tf1_Zm10->                      SetLineColor(kBlue-3) ;
    tf1_Zm10->                      SetLineWidth(2) ;
    Zm10Max                       = h1f_Zm10->GetMaximum() ;
  }

  TFile* pTFile_Zp10 ;
  TH1F* h1f_Zp10 ;
  TF1*  tf1_Zp10 ;
  TPaveStats* pStat_Zp10 ;
  int Zp10Max = 0;
  if(Tag.find("860") == std::string::npos){
    pTFile_Zp10                   = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx" + Comment + "_Zp10.root")) ;
    h1f_Zp10                      = pTFile_Zp10-> Get<TH1F>("h1f_XP_0") ;
    tf1_Zp10                      = pTFile_Zp10-> Get<TF1>("tf1_XP_0") ;
    h1f_Zp10->                      SetTitle("Mean dE/dx: Z_{drift} dependence;mean dE/dx;Counts") ;
    h1f_Zp10->                      SetLineColor(kRed+2) ;
    h1f_Zp10->                      SetLineWidth(3) ;
    tf1_Zp10->                      SetLineColor(kRed-3) ;
    tf1_Zp10->                      SetLineWidth(2) ;
    Zp10Max                       = h1f_Zp10->GetMaximum() ;
  }


  // PRF or Yw
  TFile* pTFile_PRF             = TFile::Open(TString(OutDir + Tag + "/3_" + Tag + "_dEdx_XP_normsel_cut2mm_zcalc_PRF.root")) ;
  TH1F* h1f_PRF                 = pTFile_PRF-> Get<TH1F>("h1f_XP_0") ;
  TF1*  tf1_PRF                 = pTFile_PRF-> Get<TF1>("tf1_XP_0") ;
  h1f_PRF->                       SetTitle("Mean dE/dx: track fitter dependence;mean dE/dx;Counts") ;
  h1f_PRF->                       SetLineColor(kBlue+2) ;
  h1f_PRF->                       SetLineWidth(3) ;
  tf1_PRF->                       SetLineColor(kBlue-3) ;
  tf1_PRF->                       SetLineWidth(2) ;
  TPaveStats*                     pStat_PRF ;
  int PRFMax                    = h1f_PRF->GetMaximum() ;




  // Make canvas
  std::string OutputFile        = OutDir + Tag + "/" + Tag + "_systematics" + Comment + ".pdf" ;
  std::string OutputFile_Beg    = OutputFile + "(" ;
  std::string OutputFile_End    = OutputFile + ")" ;
  TCanvas* pTCanvas             = new TCanvas("pTCanvas", "pTCanvas", 1800, 1200) ;

  //  Draw
  float inv                     = 0 ;
  if(h1f_XP->GetMean() > 800) inv = 0.38 ;
  pTCanvas->cd() ;
  gStyle->                        SetOptStat(11) ;
  gStyle->                        SetOptFit(111) ; 



  // RC
  h1f_RC87->                      SetAxisRange(0, 1.1 * std::max({RC87Max, XPMax, RC147Max}),  "Y") ;

  h1f_RC87->                      Draw("HIST ") ;
  tf1_RC87->                      Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_RC87, 0.76, 0.99, 0.32, 0.52) ;
  pStat_RC87                    = (TPaveStats*)h1f_RC87->FindObject("stats") ;
  pStat_RC87->                    SetTextColor(kBlue+2) ;

  h1f_XP->                        Draw("HIST sames") ;
  tf1_XP->                        Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_XP, 0.76, 0.99, 0.52, 0.72) ;
  pStat_XP                      = (TPaveStats*)h1f_XP->FindObject("stats") ;
  pStat_XP->                      SetTextColor(kMagenta+2) ;

  h1f_RC147->                     Draw("HIST sames") ;
  tf1_RC147->                     Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_RC147, 0.76, 0.99, 0.72, 0.92) ;
  pStat_RC147                   = (TPaveStats*)h1f_RC147->FindObject("stats") ;
  pStat_RC147->                   SetTextColor(kRed+2) ;

  PrintResolution(tf1_RC147,  pTCanvas, 0.8-inv, 0.95, kRed+2, "RC = 147") ;
  PrintResolution(tf1_XP,     pTCanvas, 0.8-inv, 0.85, kMagenta+2, "RC = 115") ;
  PrintResolution(tf1_RC87,   pTCanvas, 0.8-inv, 0.75, kBlue+2, "RC = 87") ;

  pTCanvas->                      SaveAs(OutputFile_Beg.c_str()) ;
  pTCanvas->                      Clear() ;



  // Dt
  h1f_Dt286->                     SetAxisRange(0, 1.1 * std::max({Dt286Max, XPMax, Dt350Max}),  "Y") ;

  h1f_Dt286->                     Draw("HIST ") ;
  tf1_Dt286->                     Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_Dt286, 0.76, 0.99, 0.32, 0.52) ;
  pStat_Dt286                    = (TPaveStats*)h1f_Dt286->FindObject("stats") ;
  pStat_Dt286->                    SetTextColor(kBlue+2) ;

  h1f_XP->                        Draw("HIST sames") ;
  tf1_XP->                        Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_XP, 0.76, 0.99, 0.52, 0.72) ;
  pStat_XP                      = (TPaveStats*)h1f_XP->FindObject("stats") ;
  pStat_XP->                      SetTextColor(kMagenta+2) ;

  h1f_Dt350->                     Draw("HIST sames") ;
  tf1_Dt350->                     Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_Dt350, 0.76, 0.99, 0.72, 0.92) ;
  pStat_Dt350                   = (TPaveStats*)h1f_Dt350->FindObject("stats") ;
  pStat_Dt350->                   SetTextColor(kRed+2) ;

  PrintResolution(tf1_Dt350,  pTCanvas, 0.8-inv, 0.95, kRed+2, "D_{t} = 350") ;
  PrintResolution(tf1_XP,     pTCanvas, 0.8-inv, 0.85, kMagenta+2, "D_{t} = 310") ;
  PrintResolution(tf1_Dt286,   pTCanvas, 0.8-inv, 0.75, kBlue+2, "D_{t}= 286") ;

  pTCanvas->                      SaveAs(OutputFile.c_str()) ;
  pTCanvas->                      Clear() ;


  // Drift distance shift
  if(Tag.find("m40") == std::string::npos and Tag.find("860") == std::string::npos){
    h1f_Zm10->                      SetAxisRange(0, 1.1 * std::max({Zm10Max, XPMax, Zp10Max}),  "Y") ;

    h1f_Zm10->                      Draw("HIST ") ;
    tf1_Zm10->                      Draw("same") ;
    gPad->                          Update() ;
    SetStatBoxPosition(h1f_Zm10, 0.76, 0.99, 0.32, 0.52) ;
    pStat_Zm10                    = (TPaveStats*)h1f_Zm10->FindObject("stats") ;
    pStat_Zm10->                    SetTextColor(kBlue+2) ;
    PrintResolution(tf1_Zm10, pTCanvas, 0.8-inv, 0.75, kBlue+2, "Z-10cm") ;

    h1f_XP->                        Draw("HIST sames") ;
    tf1_XP->                        Draw("same") ;
    gPad->                          Update() ;
    SetStatBoxPosition(h1f_XP, 0.76, 0.99, 0.52, 0.72) ;
    pStat_XP                      = (TPaveStats*)h1f_XP->FindObject("stats") ;
    pStat_XP->                      SetTextColor(kMagenta+2) ;    
    PrintResolution(tf1_XP,   pTCanvas, 0.8-inv, 0.85, kMagenta+2, "true Z") ;

    h1f_Zp10->                      Draw("HIST sames") ;
    tf1_Zp10->                      Draw("same") ;
    gPad->                          Update() ;
    SetStatBoxPosition(h1f_Zp10, 0.76, 0.99, 0.72, 0.92) ;
    pStat_Zp10                    = (TPaveStats*)h1f_Zp10->FindObject("stats") ;
    pStat_Zp10->                    SetTextColor(kRed+2) ;
    PrintResolution(tf1_Zp10, pTCanvas, 0.8-inv, 0.95, kRed+2, "Z+10cm") ;



    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    pTCanvas->                      Clear() ;
  }



  // impact parameter shift shift
  h1f_PRF->                       SetAxisRange(0, 1.1 * std::max({PRFMax, XPMax, PRFMax}),  "Y") ;

  h1f_PRF->                       Draw("HIST") ;
  tf1_PRF->                       Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_PRF, 0.76, 0.99, 0.72, 0.92) ;
  pStat_PRF                     = (TPaveStats*)h1f_PRF->FindObject("stats") ;
  pStat_PRF->                     SetTextColor(kBlue+2) ;

  h1f_XP->                        Draw("HIST sames") ;
  tf1_XP->                        Draw("same") ;
  gPad->                          Update() ;
  SetStatBoxPosition(h1f_XP, 0.76, 0.99, 0.52, 0.72) ;
  pStat_XP                      = (TPaveStats*)h1f_XP->FindObject("stats") ;
  pStat_XP->                      SetTextColor(kMagenta+2) ;

  PrintResolution(tf1_PRF, pTCanvas, 0.8-inv, 0.95, kBlue+2, "PRF fit") ;
  PrintResolution(tf1_XP,  pTCanvas, 0.8-inv, 0.85, kMagenta+2, "y_{w} fit") ;

  pTCanvas->                      SaveAs(OutputFile_End.c_str()) ;

  delete                            pTCanvas ;
  delete                            pTFile_XP ;
  delete                            pTFile_RC87 ;
  delete                            pTFile_RC147 ;
  delete                            pTFile_Dt286 ;
  delete                            pTFile_Dt350 ;
  delete                            pTFile_PRF ;

  gStyle->                          SetStatX(statXin) ;
  gStyle->                          SetStatY(statYin) ;
}
















// DrawOut dE/dx distribution for different RC values on Z scan
void DrawOut_Systematics_Zscan(const std::string& inputDir, const std::string& Comment, const int& PT)
{
  int nz                  = 10 ;

  int sys_RC              = 1 ;
  int sys_Dt              = 1 ;
  int sys_Z               = 1 ;
  int sys_track           = 1 ;

  // Draw
  std::string OutputFile        = inputDir + "/Systematics" + Comment ;
  if(sys_RC == 1) OutputFile    = OutputFile + "_RC" ;
  if(sys_Dt == 1) OutputFile    = OutputFile + "_Dt" ;
  if(sys_Z  == 1) OutputFile    = OutputFile + "_Z" ;
  if(sys_track == 1) OutputFile = OutputFile + "_track" ;
  OutputFile                    = OutputFile + ".pdf" ;
  std::string OutputFile_Beg    = OutputFile + "(" ;
  std::string OutputFile_End    = OutputFile + ")" ;
  TCanvas* pTCanvas             = new TCanvas("TCanvas", "TCanvas", 1800, 1200) ;
  gStyle->                        SetPadTickX(1);
  gStyle->                        SetPadTickY(1);
  gStyle->                        SetOptStat(0) ;
  // gStyle->                        SetGridStyle(1) ;
  pTCanvas->                      cd() ;
  TLegend* leg                  = new TLegend(0.7,0.64,0.98,0.94) ;
  pTCanvas->                      SaveAs(OutputFile_Beg.c_str()) ;


  // Reference
  TFile* pTFile_XP ;
  TGraphErrors* pTGE_reso_XP  = new TGraphErrors();
  TGraphErrors* pTGE_mean_XP  = new TGraphErrors();
  TGraphErrors* pTGE_std_XP   = new TGraphErrors();

  for(int iz = -1 ; iz < nz-1 ; iz++){
    if(iz == -1) pTFile_XP      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + ".root")) ;
    else pTFile_XP              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + ".root")) ;
    TF1* tf1_XP                 = pTFile_XP->Get<TF1>("tf1_XP_0") ;
    pTGE_reso_XP->                SetPoint(     iz+1,iz*100+50, tf1_XP->GetParameter(2)/tf1_XP->GetParameter(1)*100) ;
    pTGE_reso_XP->                SetPointError(iz+1,0,         GetResoError(tf1_XP)) ;
    pTGE_mean_XP->                SetPoint(     iz+1,iz*100+50, tf1_XP->GetParameter(1)) ;
    pTGE_mean_XP->                SetPointError(iz+1,0,         tf1_XP->GetParError(1)) ;
    pTGE_std_XP->                 SetPoint(     iz+1,iz*100+50, tf1_XP->GetParameter(2)) ;
    pTGE_std_XP->                 SetPointError(iz+1,0,         tf1_XP->GetParError(2)) ;
  }



  // RC
  if(sys_RC){
    TFile* pTFile_RC87 ;
    std::vector<TF1*> v_tf1_RC87 ;
    TGraphErrors* pTGE_reso_RC87  = new TGraphErrors();
    TGraphErrors* pTGE_mean_RC87  = new TGraphErrors();
    TGraphErrors* pTGE_std_RC87   = new TGraphErrors();

    TFile* pTFile_RC147 ;
    std::vector<TF1*> v_tf1_RC147 ;
    TGraphErrors* pTGE_reso_RC147   = new TGraphErrors();
    TGraphErrors* pTGE_mean_RC147   = new TGraphErrors();
    TGraphErrors* pTGE_std_RC147  = new TGraphErrors();

    for(int iz = -1 ; iz < nz-1 ; iz++){
      if(iz == -1) pTFile_RC87      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + "_RC87.root")) ;
      else pTFile_RC87              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_RC87.root")) ;
      TF1* tf1_RC87                 = pTFile_RC87->Get<TF1>("tf1_XP_0") ;
      pTGE_reso_RC87->                SetPoint(     iz+1,iz*100+50, tf1_RC87->GetParameter(2)/tf1_RC87->GetParameter(1)*100) ;
      pTGE_reso_RC87->                SetPointError(iz+1,0,         GetResoError(tf1_RC87)) ;
      pTGE_mean_RC87->                SetPoint(     iz+1,iz*100+50, tf1_RC87->GetParameter(1)) ;
      pTGE_mean_RC87->                SetPointError(iz+1,0,         tf1_RC87->GetParError(1)) ;
      pTGE_std_RC87->                 SetPoint(     iz+1,iz*100+50, tf1_RC87->GetParameter(2)) ;
      pTGE_std_RC87->                 SetPointError(iz+1,0,         tf1_RC87->GetParError(2)) ;

      if(iz == -1) pTFile_RC147      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + "_RC147.root")) ;
      else pTFile_RC147              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_RC147.root")) ;
      TF1* tf1_RC147                 = pTFile_RC147->Get<TF1>("tf1_XP_0") ;
      pTGE_reso_RC147->                SetPoint(     iz+1,iz*100+50, tf1_RC147->GetParameter(2)/tf1_RC147->GetParameter(1)*100) ;
      pTGE_reso_RC147->                SetPointError(iz+1,0,         GetResoError(tf1_RC147)) ;
      pTGE_mean_RC147->                SetPoint(     iz+1,iz*100+50, tf1_RC147->GetParameter(1)) ;
      pTGE_mean_RC147->                SetPointError(iz+1,0,         tf1_RC147->GetParError(1)) ;
      pTGE_std_RC147->                 SetPoint(     iz+1,iz*100+50, tf1_RC147->GetParameter(2)) ;
      pTGE_std_RC147->                 SetPointError(iz+1,0,         tf1_RC147->GetParError(2)) ;
    }
    // Resolution
    pTGE_reso_RC87->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_reso_RC87->                SetMinimum(7) ;
    pTGE_reso_RC87->                SetMaximum(12) ;
    pTGE_reso_RC87->                SetNameTitle("pTGE_reso_RC87", "Resolution vs drift distance: RC systematics;drift distance z (mm);resolution (%)") ;
    Graphic_setup(pTGE_reso_RC87, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_RC147, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_reso_RC87->                Draw("ap") ;
    pTGE_reso_XP->                  Draw("p same") ;
    pTGE_reso_RC147->               Draw("p same") ;
    leg->                           AddEntry(pTGE_reso_RC147, "RC = 147 ns/mm^{2}", "ep") ;  
    leg->                           AddEntry(pTGE_reso_XP, "RC = 115 ns/mm^{2}", "ep") ;  
    leg->                           AddEntry(pTGE_reso_RC87, "RC = 87 ns/mm^{2}", "ep") ;  
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile_Beg.c_str()) ;
    // Mean
    pTCanvas->                      Clear() ;
    pTGE_mean_RC87->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_mean_RC87->                SetMinimum(700) ;
    pTGE_mean_RC87->                SetMaximum(1200) ;
    pTGE_mean_RC87->                SetNameTitle("pTGE_mean_RC87", "Mean vs drift distance: RC systematics;drift distance z (mm);mean (ADC count)") ;
    Graphic_setup(pTGE_mean_RC87, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_RC147, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_mean_RC87->                Draw("ap") ;
    pTGE_mean_XP->                  Draw("p same") ;
    pTGE_mean_RC147->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Std
    pTCanvas->                      Clear() ;
    pTGE_std_RC87->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_std_RC87->                SetMinimum(50) ;
    pTGE_std_RC87->                SetMaximum(120) ;
    pTGE_std_RC87->                SetNameTitle("pTGE_std_RC87", "Std vs drift distance: RC systematics;drift distance z (mm);standard deviation (ADC count)") ;
    Graphic_setup(pTGE_std_RC87, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_RC147, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_std_RC87->                Draw("ap") ;
    pTGE_std_XP->                  Draw("p same") ;
    pTGE_std_RC147->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
  }



  // Dt
  if(sys_Dt){
    TFile* pTFile_Dt286 ;
    std::vector<TF1*> v_tf1_Dt286 ;
    TGraphErrors* pTGE_reso_Dt286  = new TGraphErrors();
    TGraphErrors* pTGE_mean_Dt286  = new TGraphErrors();
    TGraphErrors* pTGE_std_Dt286   = new TGraphErrors();

    TFile* pTFile_Dt350 ;
    std::vector<TF1*> v_tf1_Dt350 ;
    TGraphErrors* pTGE_reso_Dt350   = new TGraphErrors();
    TGraphErrors* pTGE_mean_Dt350   = new TGraphErrors();
    TGraphErrors* pTGE_std_Dt350  = new TGraphErrors();

    for(int iz = -1 ; iz < nz-1 ; iz++){
      if(iz == -1) pTFile_Dt286      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + "_Dt286.root")) ;
      else pTFile_Dt286              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_Dt286.root")) ;
      TF1* tf1_Dt286                 = pTFile_Dt286->Get<TF1>("tf1_XP_0") ;
      pTGE_reso_Dt286->                SetPoint(     iz+1,iz*100+50, tf1_Dt286->GetParameter(2)/tf1_Dt286->GetParameter(1)*100) ;
      pTGE_reso_Dt286->                SetPointError(iz+1,0,         GetResoError(tf1_Dt286)) ;
      pTGE_mean_Dt286->                SetPoint(     iz+1,iz*100+50, tf1_Dt286->GetParameter(1)) ;
      pTGE_mean_Dt286->                SetPointError(iz+1,0,         tf1_Dt286->GetParError(1)) ;
      pTGE_std_Dt286->                 SetPoint(     iz+1,iz*100+50, tf1_Dt286->GetParameter(2)) ;
      pTGE_std_Dt286->                 SetPointError(iz+1,0,         tf1_Dt286->GetParError(2)) ;

      if(iz == -1) pTFile_Dt350      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + "_Dt350.root")) ;
      else pTFile_Dt350              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_Dt350.root")) ;
      TF1* tf1_Dt350                 = pTFile_Dt350->Get<TF1>("tf1_XP_0") ;
      pTGE_reso_Dt350->                SetPoint(     iz+1,iz*100+50, tf1_Dt350->GetParameter(2)/tf1_Dt350->GetParameter(1)*100) ;
      pTGE_reso_Dt350->                SetPointError(iz+1,0,         GetResoError(tf1_Dt350)) ;
      pTGE_mean_Dt350->                SetPoint(     iz+1,iz*100+50, tf1_Dt350->GetParameter(1)) ;
      pTGE_mean_Dt350->                SetPointError(iz+1,0,         tf1_Dt350->GetParError(1)) ;
      pTGE_std_Dt350->                 SetPoint(     iz+1,iz*100+50, tf1_Dt350->GetParameter(2)) ;
      pTGE_std_Dt350->                 SetPointError(iz+1,0,         tf1_Dt350->GetParError(2)) ;
    }
    pTCanvas->                      Clear() ;
    leg->                           Clear() ;
    // Resolution
    pTGE_reso_Dt286->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_reso_Dt286->                SetMinimum(7) ;
    pTGE_reso_Dt286->                SetMaximum(12) ;
    pTGE_reso_Dt286->                SetNameTitle("pTGE_reso_Dt286", "Resolution vs drift distance: D_{t}  systematics;drift distance z (mm);resolution (%)") ;
    Graphic_setup(pTGE_reso_Dt286, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_Dt350, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_reso_Dt286->                Draw("ap") ;
    pTGE_reso_XP->                  Draw("p same") ;
    pTGE_reso_Dt350->               Draw("p same") ;
    leg->                           AddEntry(pTGE_reso_Dt350, "D_{t} = 350 #mum/#sqrt{cm}", "ep") ;  
    leg->                           AddEntry(pTGE_reso_XP, "D_{t} = 310 #mum/#sqrt{cm}", "ep") ;  
    leg->                           AddEntry(pTGE_reso_Dt286, "D_{t} = 286 #mum/#sqrt{cm}", "ep") ;  
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Mean
    pTCanvas->                      Clear() ;
    pTGE_mean_Dt286->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_mean_Dt286->                SetMinimum(700) ;
    pTGE_mean_Dt286->                SetMaximum(1200) ;
    pTGE_mean_Dt286->                SetNameTitle("pTGE_mean_Dt286", "Mean vs drift distance: D_{t}  systematics;drift distance z (mm);mean (ADC count)") ;
    Graphic_setup(pTGE_mean_Dt286, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_Dt350, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_mean_Dt286->                Draw("ap") ;
    pTGE_mean_XP->                  Draw("p same") ;
    pTGE_mean_Dt350->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Std
    pTCanvas->                      Clear() ;
    pTGE_std_Dt286->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_std_Dt286->                SetMinimum(50) ;
    pTGE_std_Dt286->                SetMaximum(120) ;
    pTGE_std_Dt286->                SetNameTitle("pTGE_std_Dt286", "Std vs drift distance: D_{t}  systematics;drift distance z (mm);standard deviation (ADC count)") ;
    Graphic_setup(pTGE_std_Dt286, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_Dt350, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_std_Dt286->                Draw("ap") ;
    pTGE_std_XP->                  Draw("p same") ;
    pTGE_std_Dt350->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
  }


  // Z drift
  if(sys_Z){
    TFile* pTFile_Zm10 ;
    std::vector<TF1*> v_tf1_Zm10 ;
    TGraphErrors* pTGE_reso_Zm10    = new TGraphErrors();
    TGraphErrors* pTGE_mean_Zm10    = new TGraphErrors();
    TGraphErrors* pTGE_std_Zm10     = new TGraphErrors();

    TFile* pTFile_Zp10 ;
    std::vector<TF1*> v_tf1_Zp10 ;
    TGraphErrors* pTGE_reso_Zp10    = new TGraphErrors();
    TGraphErrors* pTGE_mean_Zp10    = new TGraphErrors();
    TGraphErrors* pTGE_std_Zp10     = new TGraphErrors();

    for(int iz = -1 ; iz < nz-1 ; iz++){
      if(iz >= 0){
        pTFile_Zm10                 = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_Zm10.root")) ;
        TF1* tf1_Zm10               = pTFile_Zm10->Get<TF1>("tf1_XP_0") ;
        pTGE_reso_Zm10->              SetPoint(     iz+1,iz*100+50, tf1_Zm10->GetParameter(2)/tf1_Zm10->GetParameter(1)*100) ;
        pTGE_reso_Zm10->              SetPointError(iz+1,0,         GetResoError(tf1_Zm10)) ;
        pTGE_mean_Zm10->              SetPoint(     iz+1,iz*100+50, tf1_Zm10->GetParameter(1)) ;
        pTGE_mean_Zm10->              SetPointError(iz+1,0,         tf1_Zm10->GetParError(1)) ;
        pTGE_std_Zm10->               SetPoint(     iz+1,iz*100+50, tf1_Zm10->GetParameter(2)) ;
        pTGE_std_Zm10->               SetPointError(iz+1,0,         tf1_Zm10->GetParError(2)) ;
      }
      else{
        pTFile_Zm10                 = 0 ;
        pTGE_reso_Zm10->              SetPoint(     iz+1,3000, 0) ;
        pTGE_reso_Zm10->              SetPointError(iz+1,0,    0) ;
        pTGE_mean_Zm10->              SetPoint(     iz+1,3000, 0) ;
        pTGE_mean_Zm10->              SetPointError(iz+1,0,    0) ;
        pTGE_std_Zm10->               SetPoint(     iz+1,3000, 0) ;
        pTGE_std_Zm10->               SetPointError(iz+1,0,    0) ;
      }

      if(iz < nz-2){
        pTFile_Zp10                 = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + "_Zp10.root")) ;
        TF1* tf1_Zp10               = pTFile_Zp10->Get<TF1>("tf1_XP_0") ;
        pTGE_reso_Zp10->              SetPoint(     iz+1,iz*100+50, tf1_Zp10->GetParameter(2)/tf1_Zp10->GetParameter(1)*100) ;
        pTGE_reso_Zp10->              SetPointError(iz+1,0,         GetResoError(tf1_Zp10)) ;
        pTGE_mean_Zp10->              SetPoint(     iz+1,iz*100+50, tf1_Zp10->GetParameter(1)) ;
        pTGE_mean_Zp10->              SetPointError(iz+1,0,         tf1_Zp10->GetParError(1)) ;
        pTGE_std_Zp10->               SetPoint(     iz+1,iz*100+50, tf1_Zp10->GetParameter(2)) ;
        pTGE_std_Zp10->               SetPointError(iz+1,0,         tf1_Zp10->GetParError(2)) ;
      }
      else{
        pTFile_Zp10                 = 0 ;
        pTGE_reso_Zp10->              SetPoint(     iz+1,3000, 0) ;
        pTGE_reso_Zp10->              SetPointError(iz+1,0,    0) ;
        pTGE_mean_Zp10->              SetPoint(     iz+1,3000, 0) ;
        pTGE_mean_Zp10->              SetPointError(iz+1,0,    0) ;
        pTGE_std_Zp10->               SetPoint(     iz+1,3000, 0) ;
        pTGE_std_Zp10->               SetPointError(iz+1,0,    0) ;
      }
    }
    pTCanvas->                      Clear() ;
    leg->                           Clear() ;
    // Resolution
    pTGE_reso_Zm10->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_reso_Zm10->                SetMinimum(7) ;
    pTGE_reso_Zm10->                SetMaximum(12) ;
    pTGE_reso_Zm10->                SetNameTitle("pTGE_reso_Zm10", "Resolution vs drift distance: Z_{drift} systematics;drift distance z (mm);resolution (%)") ;
    Graphic_setup(pTGE_reso_Zm10, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_Zp10, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_reso_Zm10->                Draw("ap") ;
    pTGE_reso_XP->                  Draw("p same") ;
    pTGE_reso_Zp10->                Draw("p same") ;
    leg->                           AddEntry(pTGE_reso_Zp10, "Z + 10 cm", "ep") ;  
    leg->                           AddEntry(pTGE_reso_XP, "Z true", "ep") ;  
    leg->                           AddEntry(pTGE_reso_Zm10, "Z - 10 cm", "ep") ;  
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Mean
    pTCanvas->                      Clear() ;
    pTGE_mean_Zm10->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_mean_Zm10->                SetMinimum(700) ;
    pTGE_mean_Zm10->                SetMaximum(1200) ;
    pTGE_mean_Zm10->                SetNameTitle("pTGE_mean_Zm10", "Mean vs drift distance: Z_{drift} systematics;drift distance z (mm);mean (ADC count)") ;
    Graphic_setup(pTGE_mean_Zm10, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_Zp10, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_mean_Zm10->                Draw("ap") ;
    pTGE_mean_XP->                  Draw("p same") ;
    pTGE_mean_Zp10->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Std
    pTCanvas->                      Clear() ;
    pTGE_std_Zm10->                GetXaxis()->SetLimits(0, 1000) ;
    pTGE_std_Zm10->                SetMinimum(50) ;
    pTGE_std_Zm10->                SetMaximum(120) ;
    pTGE_std_Zm10->                SetNameTitle("pTGE_std_Zm10", "Std vs drift distance: Z_{drift} systematics;drift distance z (mm);standard deviation (ADC count)") ;
    Graphic_setup(pTGE_std_Zm10, 3, 23, kBlue+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_Zp10, 3, 22, kRed+2, 1, kBlack) ;
    pTGE_std_Zm10->                Draw("ap") ;
    pTGE_std_XP->                  Draw("p same") ;
    pTGE_std_Zp10->               Draw("p same") ; 
    leg->                           Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
  }



  // track fitter
  if(sys_track){
    TFile* pTFile_PRF ;
    TGraphErrors* pTGE_reso_PRF  = new TGraphErrors();
    TGraphErrors* pTGE_mean_PRF  = new TGraphErrors();
    TGraphErrors* pTGE_std_PRF   = new TGraphErrors();

    for(int iz = -1 ; iz < nz-1 ; iz++){
      if(iz == -1) pTFile_PRF      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx_XP_normsel_cut2mm_zcalc_PRF.root")) ;
      else pTFile_PRF              = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx_XP_normsel_cut2mm_zcalc_PRF.root")) ;
      TF1* tf1_PRF                 = pTFile_PRF->Get<TF1>("tf1_XP_0") ;
      pTGE_reso_PRF->                SetPoint(     iz+1,iz*100+50, tf1_PRF->GetParameter(2)/tf1_PRF->GetParameter(1)*100) ;
      pTGE_reso_PRF->                SetPointError(iz+1,0,         GetResoError(tf1_PRF)) ;
      pTGE_mean_PRF->                SetPoint(     iz+1,iz*100+50, tf1_PRF->GetParameter(1)) ;
      pTGE_mean_PRF->                SetPointError(iz+1,0,         tf1_PRF->GetParError(1)) ;
      pTGE_std_PRF->                 SetPoint(     iz+1,iz*100+50, tf1_PRF->GetParameter(2)) ;
      pTGE_std_PRF->                 SetPointError(iz+1,0,         tf1_PRF->GetParError(2)) ;
    }
    
    leg->                           Clear() ;
    pTCanvas->                      Clear() ;
    // Resolution
    TLegend* legPRF               = new TLegend(0.8,0.75,0.97,0.94) ;
    pTGE_reso_PRF->                 GetXaxis()->SetLimits(0, 1000) ;
    pTGE_reso_PRF->                 SetMinimum(7) ;
    pTGE_reso_PRF->                 SetMaximum(12) ;
    pTGE_reso_PRF->                 SetNameTitle("pTGE_reso_PRF", "Resolution vs drift distance: track fitter systematics;drift distance z (mm);resolution (%)") ;
    Graphic_setup(pTGE_reso_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_reso_PRF, 3, 20, kBlue+2, 1, kBlack) ;
    pTGE_reso_PRF->                 Draw("ap") ;
    pTGE_reso_XP->                  Draw("p same") ;
    legPRF->                        AddEntry(pTGE_reso_PRF, "PRF fit ", "ep") ;  
    legPRF->                        AddEntry(pTGE_reso_XP, "y_{w} fit ", "ep") ;  
    legPRF->                        Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Mean
    pTCanvas->                      Clear() ;
    pTGE_mean_PRF->                 GetXaxis()->SetLimits(0, 1000) ;
    pTGE_mean_PRF->                 SetMinimum(700) ;
    pTGE_mean_PRF->                 SetMaximum(1200) ;
    pTGE_mean_PRF->                 SetNameTitle("pTGE_mean_PRF", "Mean vs drift distance: track fitter systematics;drift distance z (mm);mean (ADC count)") ;
    Graphic_setup(pTGE_mean_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_mean_PRF, 3, 20, kBlue+2, 1, kBlack) ;
    pTGE_mean_PRF->                 Draw("ap") ;
    pTGE_mean_XP->                  Draw("p same") ;
    legPRF->                        Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    // Std
    pTCanvas->                      Clear() ;
    pTGE_std_PRF->                  GetXaxis()->SetLimits(0, 1000) ;
    pTGE_std_PRF->                  SetMinimum(50) ;
    pTGE_std_PRF->                  SetMaximum(120) ;
    pTGE_std_PRF->                  SetNameTitle("pTGE_std_PRF", "Std vs drift distance: track fitter systematics;drift distance z (mm);standard deviation (ADC count)") ;
    Graphic_setup(pTGE_std_XP, 3, 21, kMagenta+2, 1, kBlack) ;
    Graphic_setup(pTGE_std_PRF, 3, 20, kBlue+2, 1, kBlack) ;
    pTGE_std_PRF->                  Draw("ap") ;
    pTGE_std_XP->                   Draw("p same") ;
    legPRF->                        Draw() ;
    pTCanvas->                      SaveAs(OutputFile.c_str()) ;
    delete                          legPRF ;
  }


  pTCanvas->                      SaveAs(OutputFile_End.c_str()) ;

  // Delete
  delete                        pTCanvas   ;
  delete                        leg ;
}













void DrawOut_Alpha(const std::string& inputDir, const std::string& Comment)
{
  std::vector<TFile*>           v_TFile ;
  int nalpha                  = 10 ;
  std::vector<TF1*>             v_pTF1_XP ;
  std::vector<TF1*>             v_pTF1_WF ;
  TGraphErrors* pTGE_reso_XP  = new TGraphErrors() ;
  TGraphErrors* pTGE_mean_XP  = new TGraphErrors() ;
  TGraphErrors* pTGE_std_XP   = new TGraphErrors() ;
  TGraphErrors* pTGE_reso_WF  = new TGraphErrors() ;
  TGraphErrors* pTGE_mean_WF  = new TGraphErrors() ;
  TGraphErrors* pTGE_std_WF   = new TGraphErrors() ;
  float mean_XP[nalpha] ;
  float dmean_XP[nalpha] ;
  float std_XP[nalpha] ;
  float dstd_XP[nalpha] ;
  float mean_WF[nalpha] ;
  float dmean_WF[nalpha] ;
  float std_WF[nalpha] ;
  float dstd_WF[nalpha] ;

  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha10.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha20.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha30.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha40.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha50.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha60.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + ".root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha80.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha90.root"))) ;
  v_TFile.push_back(TFile::Open(TString(inputDir + "/DESY21_z460_PT200/3_DESY21_z460_PT200_dEdx" + Comment + "_alpha100.root"))) ;

  for(int i = 0 ; i < nalpha ; i++){
    v_pTF1_XP.push_back(v_TFile[i]->Get<TF1>("tf1_XP_0")) ;
    v_pTF1_WF.push_back(v_TFile[i]->Get<TF1>("tf1_WFtrunc_0")) ;
  }

  for(int i = 0 ; i < nalpha ; i++){
    mean_XP[i]                = v_pTF1_XP[i]->GetParameter(1) ;
    dmean_XP[i]               = v_pTF1_XP[i]->GetParError(1) ;
    std_XP[i]                 = v_pTF1_XP[i]->GetParameter(2) ;
    dstd_XP[i]                = v_pTF1_XP[i]->GetParError(2) ;
    mean_WF[i]                = v_pTF1_WF[i]->GetParameter(1) ;
    dmean_WF[i]               = v_pTF1_WF[i]->GetParError(1) ;
    std_WF[i]                 = v_pTF1_WF[i]->GetParameter(2) ;
    dstd_WF[i]                = v_pTF1_WF[i]->GetParError(2) ;
  }

  for(int i = 0 ; i < nalpha ; i++){
    int alpha = (i+1)*10 ;
    pTGE_reso_XP->SetPoint      (i, alpha, std_XP[i]/mean_XP[i]*100) ;
    pTGE_reso_XP->SetPointError (i, 0,     GetResoError(v_pTF1_XP[i])) ;
    pTGE_mean_XP->SetPoint      (i, alpha, mean_XP[i]) ;
    pTGE_mean_XP->SetPointError (i, 0,     dmean_XP[i]) ;
    pTGE_std_XP->SetPoint       (i, alpha, std_XP[i]) ;
    pTGE_std_XP->SetPointError  (i, 0,     dstd_XP[i]) ;

    pTGE_reso_WF->SetPoint      (i, alpha, std_WF[i]/mean_WF[i]*100) ;
    pTGE_reso_WF->SetPointError (i, 0,     GetResoError(v_pTF1_WF[i])) ;
    pTGE_mean_WF->SetPoint      (i, alpha, mean_WF[i]) ;
    pTGE_mean_WF->SetPointError (i, 0,     dmean_WF[i]) ;
    pTGE_std_WF->SetPoint       (i, alpha, std_WF[i]) ;
    pTGE_std_WF->SetPointError  (i, 0,     dstd_WF[i]) ;
  }


  // Draw
  std::string OutputFile        = inputDir + "/Resolution_Alpha_z460" + Comment + ".pdf" ;
  std::string OutputFile_Beg    = OutputFile + "(" ;
  std::string OutputFile_End    = OutputFile + ")" ;
  TCanvas* pTCanvas           = new TCanvas("TCanvas", "TCanvas", 1800, 1200) ;
  gStyle->                      SetPadTickX(1);
  gStyle->                      SetPadTickY(1);
  gStyle->                      SetOptStat(0) ;
  gStyle->                      SetGridStyle(1) ;
  pTCanvas->                    cd() ;
  TLegend* leg                = new TLegend(0.83,0.75,0.95,0.98) ;


  // Resolution
  pTGE_reso_XP->                GetXaxis()->SetLimits(7, 103) ;
  pTGE_reso_XP->                SetMinimum(7) ;
  pTGE_reso_XP->                SetMaximum(13) ;
  pTGE_reso_XP->                SetNameTitle("pTGE_reso_XP", "Resolution vs #alpha cut for Z_{drift} = 550 mm;#alpha (%);resolution (%)") ;
  pTGE_mean_WF->                SetNameTitle("pTGE_mean_WF", "Mean vs #varphi angle;#varphi angle (#circ);mean (ADC count)") ;
  Graphic_setup(pTGE_reso_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_reso_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_reso_XP->                Draw("ap") ;
  pTGE_reso_WF->                Draw("p same") ;
  leg->                         AddEntry(pTGE_reso_XP, "XP", "ep") ;  
  leg->                         AddEntry(pTGE_reso_WF, "WF", "ep") ;  
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile_Beg.c_str()) ;
  pTCanvas->                    Clear() ;
  
  // Mean
  pTGE_mean_XP->                GetXaxis()->SetLimits(7, 103) ;
  pTGE_mean_XP->                SetMinimum(500) ;
  pTGE_mean_XP->                SetMaximum(1400) ;
  pTGE_mean_XP->                SetNameTitle("pTGE_mean_XP", "Mean vs #alpha cut for Z_{drift} = 550 mm;#alpha (%);mean (ADC count)") ;
  Graphic_setup(pTGE_mean_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_mean_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_mean_XP->                Draw("ap") ;
  pTGE_mean_WF->                Draw("p same") ;
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile.c_str()) ;
  pTCanvas->                    Clear() ;
  
  // Std
  pTGE_std_XP->                 GetXaxis()->SetLimits(7, 103) ;
  pTGE_std_XP->                 SetMinimum(50) ;
  pTGE_std_XP->                 SetMaximum(140) ;
  pTGE_std_XP->                 SetNameTitle("pTGE_std_XP", "Std vs #alpha cut for Z_{drift} = 550 mm;#alpha (%);std (ADC count)") ;
  Graphic_setup(pTGE_std_WF,   3, 20, kCyan+2,    1, kBlack) ;
  Graphic_setup(pTGE_std_XP,   3, 21, kMagenta+2, 1, kBlack) ;
  pTGE_std_XP->                 Draw("ap") ;
  pTGE_std_WF->                 Draw("p same") ;
  leg->                         Draw() ;
  pTCanvas->                    SaveAs(OutputFile_End.c_str()) ;

  // Delete
  delete                        pTCanvas   ;
  delete                        leg ;
  for(int i = 0 ; i < nalpha ; i++){
    delete                      v_TFile[i] ;    v_TFile[i] = 0 ;
    delete                      v_pTF1_XP[i] ;  v_pTF1_XP[i] = 0 ;
    delete                      v_pTF1_WF[i] ;  v_pTF1_WF[i] = 0 ;
  }
  v_TFile.                      clear() ;
  v_pTF1_XP.                    clear() ;
  v_pTF1_WF.                    clear() ;

}







// Get v_drift with Tmax & z_file
void DrawOut_v_drift(const std::string& inputDir, const std::string& Comment, const int& PT, const int& TB)
{
  int ipoint              = 0 ;
  int nz                  = 9 ;

  // TFiles & TH1Fs & TF1s & TGEs
  TFile* pTFile ;
  TH1F* h1f_Tcross ;
  TF1* tf1_Tcross ;
  TGraphErrors* pTGE_Tcross = new TGraphErrors() ;

  for(int iz = -1 ; iz < nz ; iz++){
    if(iz == -1)pTFile      = TFile::Open(TString(inputDir + "/DESY21_zm40_PT" + PT + "/3_DESY21_zm40_PT" + PT + "_dEdx" + Comment + ".root")) ;
    else        pTFile      = TFile::Open(TString(inputDir + "/DESY21_z" + iz + "60_PT" + PT + "/3_DESY21_z" + iz + "60_PT" + PT + "_dEdx" + Comment + ".root")) ;
    h1f_Tcross              = pTFile->Get<TH1F>("h1f_Tcross") ;
    tf1_Tcross              = Fit1Gauss(h1f_Tcross, 2) ;
    tf1_Tcross->              SetNameTitle  (Form("tf1_Tcross_%i", iz), Form("tf1_Tcross_%i", iz)) ;
    pTGE_Tcross->             SetPoint      (ipoint, iz*100+150, tf1_Tcross->GetParameter(1)) ;
    pTGE_Tcross->             SetPointError (ipoint, 0,          tf1_Tcross->GetParError(1)) ;
    h1f_Tcross->              Clear() ;
    tf1_Tcross->              Clear() ;
    ipoint++ ;
  }

  // Fitting
  TF1* tf1_vdrift = new TF1("tf1_vdrift" ,"[0] + x*[1]", 0,  1050);
  pTGE_Tcross->Fit(tf1_vdrift, "RQ") ;

  float v_d                   = 100/(tf1_vdrift->GetParameter(1)*TB) ;

  // Draw
  std::string OutputFile      = inputDir + "/v_drift" + Comment + ".png" ;
  TCanvas* pTCanvas           = new TCanvas("TCanvas", "TCanvas", 1800, 1200) ;
  gStyle->                      SetOptStat(11) ;
  gStyle->                      SetOptFit(111) ;
  pTCanvas->                    cd() ;

  TLatex* ptText              = new TLatex ;
  ptText->                      SetTextSize(0.06) ;
  ptText->                      SetTextFont(42) ;
  ptText->                      SetTextAlign(31) ;
  ptText->SetText(1000, 100, Form("v_{drift} = %.2f cm/#mus", v_d)) ;
  ptText->SetTextColor(kGreen+3) ;

  // Resolution
  pTGE_Tcross->                 SetNameTitle("pTGE_Tcross", "Average T_{max} of crossed pads wrt drift distance;drift distance z (mm);<T_{max}> (timebins)") ;
  Graphic_setup(pTGE_Tcross, 3, 20, kGreen+2, 1, kGreen-3) ;
  pTGE_Tcross->                 Draw("ap") ;
  ptText->                      Draw() ;
  pTCanvas->                    SaveAs(OutputFile.c_str()) ;

  // Delete
  delete                        pTCanvas   ;
  delete                        ptText ;
}






// Load a .csv into a TH2
  std::string LUT_name[nZ][nRC] ;
  TH2F* h2f_LUT[nZ][nRC] ;
  for(int iZ = 0 ; iZ < nZ ; iZ++){
    for(int iRC = 0 ; iRC < nRC ; iRC++){
      // LUT_name[iZ][iRC] = Form("/home/td263283/Documents/Python/Ratio_2D_LUT/Ratio_2D_nphi%i_nd%i_PT%i_Dt%i_theo/Ratio_2D_nphi%i_nd%i_RC%i_zscan_PT%i_Dt%i_theo/Ratio_2D_nphi%i_nd%i_RC%i_z%i_PT%i_Dt%i_theo.csv", size, size, PT, Dt, size, size, 50+iRC*5, PT, Dt, size, size, 50+iRC*5, iZ*50, PT, Dt) ;
      LUT_name[iZ][iRC] = (Form("/home/td263283/Documents/Python/LUT_XP/LUT_Dt%i_PT%i_nphi%i_nd%i/LUT_RC%i/LUT_z%i.csv", Dt, PT, size, size, 50+iRC*5, iZ*50)) ;
      std::vector<std::vector<float>> data  = readCSV(LUT_name[iZ][iRC]) ;
      h2f_LUT[iZ][iRC]                      = new TH2F(Form("h2f_LUT_z%i_RC%i", iZ*50, 50+iRC*5), Form("h2f_LUT_z%i_RC%i", iZ*50, 50+iRC*5), data.size(), 0, data.size(), data[0].size(), 0, data[0].size());
      // Fill the histogram with data from the data vector
      for (std::vector<std::vector<float>>::size_type i = 0; i < data[iZ].size(); i++) {
        for (std::vector<float>::size_type j = 0; j < data[i].size(); j++) {
          h2f_LUT[iZ][iRC]->SetBinContent(j, i, data[i][j]);
        }
      }
      data.clear() ;
    }
  }





// Interpolation with vectors
  // Z file
  float IP_0_RC0                  = h2f_LUT[iz_file][iRC_low]-> GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_file][iRC_low]->GetBinContent(phi0, d1) - h2f_LUT[iz_file][iRC_low]->GetBinContent(phi0, d0)) ;
  float IP_1_RC0                  = h2f_LUT[iz_file][iRC_low]-> GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_file][iRC_low]->GetBinContent(phi1, d1) - h2f_LUT[iz_file][iRC_low]->GetBinContent(phi1, d0)) ;
  float IP_0_RC1                  = h2f_LUT[iz_file][iRC_high]->GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_file][iRC_high]->GetBinContent(phi0, d1) - h2f_LUT[iz_file][iRC_high]->GetBinContent(phi0, d0)) ;
  float IP_1_RC1                  = h2f_LUT[iz_file][iRC_high]->GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_file][iRC_high]->GetBinContent(phi1, d1) - h2f_LUT[iz_file][iRC_high]->GetBinContent(phi1, d0)) ;
  float ratio_RC0_file            = IP_0_RC0 + (phiconv-phi0)*(IP_1_RC0 - IP_0_RC0) ;
  float ratio_RC1_file            = IP_0_RC1 + (phiconv-phi0)*(IP_1_RC1 - IP_0_RC1) ;
  float ratio_file                = ratio_RC0_file   + (RCconv-iRC_low)   *(ratio_RC1_file   - ratio_RC0_file) ;

  // Z recomputed
  float IP_0_z0_RC0               = h2f_LUT[iz_low][iRC_low]->  GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_low] [iRC_low]->GetBinContent(phi0, d1)  - h2f_LUT[iz_low][iRC_low]->GetBinContent(phi0, d0)) ;
  float IP_1_z0_RC0               = h2f_LUT[iz_low][iRC_low]->  GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_low] [iRC_low]->GetBinContent(phi1, d1)  - h2f_LUT[iz_low][iRC_low]->GetBinContent(phi1, d0)) ;
  float IP_0_z1_RC0               = h2f_LUT[iz_high][iRC_low]-> GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_high][iRC_low]->GetBinContent(phi0, d1)  - h2f_LUT[iz_high][iRC_low]->GetBinContent(phi0, d0)) ;
  float IP_1_z1_RC0               = h2f_LUT[iz_high][iRC_low]-> GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_high][iRC_low]->GetBinContent(phi1, d1)  - h2f_LUT[iz_high][iRC_low]->GetBinContent(phi1, d0)) ;
  float IP_0_z0_RC1               = h2f_LUT[iz_low][iRC_high]-> GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_low] [iRC_high]->GetBinContent(phi0, d1) - h2f_LUT[iz_low][iRC_high]->GetBinContent(phi0, d0)) ;
  float IP_1_z0_RC1               = h2f_LUT[iz_low][iRC_high]-> GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_low] [iRC_high]->GetBinContent(phi1, d1) - h2f_LUT[iz_low][iRC_high]->GetBinContent(phi1, d0)) ;
  float IP_0_z1_RC1               = h2f_LUT[iz_high][iRC_high]->GetBinContent(phi0, d0) + (dconv-d0)*(h2f_LUT[iz_high][iRC_high]->GetBinContent(phi0, d1) - h2f_LUT[iz_high][iRC_high]->GetBinContent(phi0, d0)) ;
  float IP_1_z1_RC1               = h2f_LUT[iz_high][iRC_high]->GetBinContent(phi1, d0) + (dconv-d0)*(h2f_LUT[iz_high][iRC_high]->GetBinContent(phi1, d1) - h2f_LUT[iz_high][iRC_high]->GetBinContent(phi1, d0)) ;
  float IP_z0_RC0                 = IP_0_z0_RC0 + (phiconv-phi0)*(IP_1_z0_RC0 - IP_0_z0_RC0) ;
  float IP_z1_RC0                 = IP_0_z1_RC0 + (phiconv-phi0)*(IP_1_z1_RC0 - IP_0_z1_RC0) ;
  float IP_z0_RC1                 = IP_0_z0_RC1 + (phiconv-phi0)*(IP_1_z0_RC1 - IP_0_z0_RC1) ;
  float IP_z1_RC1                 = IP_0_z1_RC1 + (phiconv-phi0)*(IP_1_z1_RC1 - IP_0_z1_RC1) ;
  float ratio_RC0                 = IP_z0_RC0   + (zconv-iz_low)     *(IP_z1_RC0   - IP_z0_RC0) ;
  float ratio_RC1                 = IP_z0_RC1   + (zconv-iz_low)     *(IP_z1_RC1   - IP_z0_RC1) ;
  float ratio_calc                = ratio_RC0   + (RCconv-iRC_low)   *(ratio_RC1   - ratio_RC0) ;






// // impact parameter d (in m) & track angle phi (in degrees) computed locally at the level of the pad
std::vector<float> local_params(const Pad* pPad, const Track* pTrack){
  std::vector<float>          v_loc_par ;
  // Get geometry of pad
  float x                   = pPad->Get_XPad() ;              // in m
  float y                   = pPad->Get_YPad() ;              // in m
  double a                  = pTrack->Get_ParameterValue(0) ; // in m         (intercept)
  double b                  = pTrack->Get_ParameterValue(1) ; // no dimension (slope)
  // y = a + bx
  float impact_param        = (b*x - y + a) / sqrt(pow(b,2) + 1) ; // in m
  float angle               = TMath::ATan(b)/TMath::Pi()*180 ;

  if(pTrack->GetNberOfParameters() == 3){                     // Magnetic Field is On
    // y = a + bx + cx^2
    float x0                = pPad->Get_XL() ;                // in meters
    double c                = pTrack->Get_ParameterValue(2) ; // in 1/meters  (curvature)
    // tangent equation to the parabola in pad wrt left border is y = (a - c*x0^2) + (2*c*x0 + b)*x instead of y = a + b*x
    impact_param            = ((2*c*x0 + b)*x - y + (a - c*x0*x0)) / sqrt(pow((2*c*x0 + b),2) + 1) ; // in m
    angle                   = TMath::ATan((2*c*x0 + b))/TMath::Pi()*180 ;
  }
  v_loc_par.push_back(impact_param) ;
  v_loc_par.push_back(angle) ;
  return v_loc_par ; // (d, phi) in (meters, degrees)
}






//  Track length in pad (linear approximation within the pad) in m
float length_in_pad(const Pad* pPad, const Track* pTrack){
  float x0                  = pPad->Get_XL() ;                // in meters
  float x1                  = pPad->Get_XH() ;                // in meters
  float y0                  = pPad->Get_YL() ;                // in meters
  float y1                  = pPad->Get_YH() ;                // in meters

  double a                  = pTrack->Get_ParameterValue(0) ; // in meters    (intercept)
  double b                  = pTrack->Get_ParameterValue(1) ; // no dimension (slope)
  float x_y0                = (y0-a)/b ;                      // in meters
  float x_y1                = (y1-a)/b ;                      // in meters
  float y_x0                = b*x0+a ;                        // in meters
  float y_x1                = b*x1+a ;                        // in meters

  if(pTrack->GetNberOfParameters() == 3){ // Magnetic Field is On
    float x_pad             = pPad->Get_XPad() ;
    double c                = pTrack->Get_ParameterValue(2) ; // in 1/meters  (curvature)
    float xroot1_y0         = (-b-sqrt(b*b-4*c*(a-y0)))/(2*c) ;
    float xroot2_y0         = (-b+sqrt(b*b-4*c*(a-y0)))/(2*c) ;
    float xroot1_y1         = (-b-sqrt(b*b-4*c*(a-y1)))/(2*c) ;
    float xroot2_y1         = (-b+sqrt(b*b-4*c*(a-y1)))/(2*c) ;

    if(fabs(xroot1_y0-x_pad) < fabs((xroot2_y0-x_pad))) x_y0 = xroot1_y0 ;
    else                                                x_y0 = xroot2_y0 ;
    if(fabs(xroot1_y1-x_pad) < fabs((xroot2_y1-x_pad))) x_y1 = xroot1_y1 ;
    else                                                x_y1 = xroot2_y1 ;
    y_x0                    = a + b*x0 + c*x0*x0 ;
    y_x1                    = a + b*x1 + c*x1*x1 ;
  }

  int   wall                = 0 ; // count how many walls were crossed
  float x[3] = {0} ;
  float y[3] = {0} ;

  // Determine which walls were crossed
  if(y0 <= y_x0 && y_x0 <= y1){   // left
    x[wall]                 = x0 ;
    y[wall]                 = y_x0 ; 
    wall++ ;
  }
  if(y0 <= y_x1 && y_x1 <= y1){   // right
    x[wall]                 = x1 ;
    y[wall]                 = y_x1 ;
    wall++ ;
  }
  if(x0 <= x_y1 && x_y1 <= x1){   // top
    x[wall]                 = x_y1 ;
    y[wall]                 = y1 ;
    wall++ ;
  }
  if(x0 <= x_y0 && x_y0 <= x1){   // bottom
    x[wall]                 = x_y0 ;
    y[wall]                 = y0 ;
    wall++ ;
  }

  if(wall != 2 and wall > 0){
    std::cout << "ALERT: " << wall << " walls crossed for a pad in entry #" << pPad->Get_EntryNber() << std::endl ;
    std::cout << x0*100 << " " << x1*100 << " " << y0*100 << " " << y1*100 << std::endl ;
    std::cout << x_y0*100 << " " << x_y1*100 << " " << y_x0*100 << " " << y_x1*100 << std::endl ;
    std::cout << a*100 << " " << b << " " << pTrack->Get_ParameterValue(2)/100 << std::endl ;
  }

  float r                   = sqrt(pow(y[1]-y[0],2) + pow(x[1]-x[0],2)) ; // in meters (track length in the pad)
  return r ; // in meters
}