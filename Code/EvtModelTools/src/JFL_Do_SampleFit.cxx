#include "EvtModelTools/JFL_Do_SampleFit.h"
#include "Misc/Util.h"

#include "Fitters_Sample/TheFitterSample_PV2.h"
#include "Fitters_Sample/TheFitterSample_PV3.h"
#include "Fitters_Sample/TheFitterSample_PV31.h"
#include "Fitters_Sample/TheFitterSample_PV4.h"

//-------------------------------------PRF like Fits-------------------------------/

//-------------------------------------PV0_PV1-------------------------------/
TGraphErrors* FitRhoVsYTYP (
                            int& fitStatus ,
                            TH2D* pTH2D, 
                            const std::string& FitName, 
                            const double& vY_min_DiffPRF, 
                            const double& vY_max_DiffPRF
                           )
{
  TGraphErrors* ToBeReturned = new TGraphErrors(); 
  for (int i = 1; i < pTH2D->GetXaxis()->GetNbins(); ++i) {

    TH1D* pTH1D = pTH2D-> ProjectionY(Form("projections_bin_%i", i), i, i);

    double x = pTH2D->GetXaxis()->GetBinCenter(i);
    double y = pTH1D->GetBinCenter(pTH1D->GetMaximumBin());

    float start = -1.;
    float end   = -1.;
    float max = pTH1D->GetMaximum();

    for (Int_t bin = 0; bin < pTH1D->GetXaxis()->GetNbins(); ++bin) {
      if (start == -1. &&                 pTH1D->GetBinContent(bin) >= max / 2.) start = pTH1D->GetBinCenter(bin);
      if (end   == -1. && start != -1. && pTH1D->GetBinContent(bin) <= max / 2.) end   = pTH1D->GetBinCenter(bin);
    }

    double e = end - start;

    ToBeReturned->SetPoint(ToBeReturned->GetN(), x, y);
    ToBeReturned->SetPointError(ToBeReturned->GetN()-1, 0, e/2.);
  }  
  
  fitStatus = ToBeReturned->Fit(FitName.c_str(),"Q","R",vY_min_DiffPRF , vY_max_DiffPRF); 
  return ToBeReturned ;
}

//-------------------------------------PV2-------------------------------/
int Do_SampleFit_PV2(Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed)
{
//Set the fitter
  TheFitterSample_PV2 aTheFitterSample_PV2("Minuit") ;

//Tell to the sample which shape function it should use
  aSample.SetEval_PV2(pTF1_ToBeUsed , ModuleNber ) ;
      
//Tell to the fitter to fit the sample
  aTheFitterSample_PV2.Set_Sample(&aSample) ;
  return aTheFitterSample_PV2.DoMinimisation() ;
  
}

//-------------------------------------PV3-------------------------------/
int Do_SampleFit_PV3(Sample& aSample, const int& ModuleNber, SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd)
{
//Set the fitter
  TheFitterSample_PV3 aTheFitterSample_PV3("Minuit") ;

//Tell to the sample which shape function it should use
  aSample.SetEval_PV3(pSC_VS_x_rc_dd_SET_dd , ModuleNber ) ;
      
//Tell to the fitter to fit the sample
  aTheFitterSample_PV3.Set_Sample(&aSample) ;
  return aTheFitterSample_PV3.DoMinimisation() ;
  
}

//-------------------------------------PV4-------------------------------/
int Do_SampleFit_PV4(Sample& aSample, const int& ModuleNber, TF1* pTF1_ToBeUsed)
{
//Set the fitter
  TheFitterSample_PV4 aTheFitterSample_PV4("Minuit") ;

//Tell to the sample which shape function it should use
  aSample.SetEval_PV4(pTF1_ToBeUsed , ModuleNber ) ;
      
//Tell to the fitter to fit the sample
  aTheFitterSample_PV4.Set_Sample(&aSample) ;
  return aTheFitterSample_PV4.DoMinimisation() ;
  
}

//-------------------------------------PV31-------------------------------/
int Do_SampleFit_PV31(Sample& aSample, const int& ModuleNber, SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd)
{
//Set the fitter
  TheFitterSample_PV31 aTheFitterSample_PV31("Minuit") ;

//Tell to the sample which shape function it should use
  aSample.SetEval_PV31(pSC_VS_x_rc_dd_SET_dd , ModuleNber ) ;
      
//Tell to the fitter to fit the sample
  aTheFitterSample_PV31.Set_Sample(&aSample) ;
  return aTheFitterSample_PV31.DoMinimisation() ;
  
}

