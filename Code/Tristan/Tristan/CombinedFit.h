#include <Fit/Fitter.h>
#include <Fit/BinData.h>
#include <Fit/Chi2FCN.h>
#include <TH1.h>
#include <Math/WrappedMultiTF1.h>
#include <HFitInterface.h>
#include <TCanvas.h>
#include <TStyle.h>

// struct GlobalChi2 {
//     GlobalChi2(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2);
    
//     double operator()(const double *par) const;
    
//     const ROOT::Math::IMultiGenFunction *fChi2_1;
//     const ROOT::Math::IMultiGenFunction *fChi2_2;
// };

void combinedFit(std::vector<TGraphErrors*> v_tge);