#include "Misc/Util.h"
#include "TF1.h"
#include "TH1F.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Track.h"
#include "EvtModel/Module.h"



// Read CSV file
std::vector<std::vector<float>> readCSV(std::string filename) ;

// Function to search for a word in a CSV file
bool Get120_CSV(const std::string& filename, const std::string& targetWord, int& value1, int& value2) ;

// Function to update a CSV file
void Set120_CSV(const std::string& filename, const std::string& targetWord, int value1, int value2) ;

//Heaviside
float Heaviside(const float& t, const float& t_0) ;

// impact parameter d (in m) & track angle phi (in degrees) computed locally at the level of the pad
std::vector<float> local_params(const Pad* pPad, const Track* pTrack) ;

// Track length
float trk_len(Module* pModule, const Track* pTrack) ;

// Electronics pulse response function
TH1F* DPR(std::string name, const float& tmin, float const& tmax, const float& t_0, const int& nbins, const int& iC, const float& PT, const float& TB) ;

// Formula to get resolution error using a TF1
double GetResoError(TF1* tf1) ;
double GetResoError(TF1* tf1, const int& mu, const int& sigma) ;



// Get separation power between 2 particles
double GetSeparation(const float& mean1, const float& std1, const float& mean2, const float& std2) ;

// Get separation power error 
double GetSeparationError(const float& mean1, const float& std1, const float& dmean1, const float& dstd1, const float& mean2, const float& std2, const float& dmean2, const float& dstd2) ;

// Fit a gaussian but within +/- 2sigma to fit only the peak correctly
TF1*  Fit1Gauss(TH1F* h1F) ;
TF1* Fit1Gauss(TH1F* h1F, float range) ;

// Fit with 2 gaussians
TF1*  Fit2Gauss(TH1F* h1F) ;
TF1*  Fit2Gauss(TH1F* h1F, const float& x1min, const float& x1max, const float& x2min, const float& x2max) ;

// Write resolution value for a 1-gaussian fit
void PrintResolution(TF1* tf1, TCanvas* pCanvas) ;
void PrintResolution(TF1* tf1, TCanvas* pCanvas, float NDCx, float NDCy, Color_t color, const std::string& title) ;

// From TH1 get TGraph
TGraph* hist_to_graph(TH1 *h1) ;

// From TH1 get swapped TGraph (transpose x and y)
TGraph* Swapped_graph(TH1 *h1) ;
