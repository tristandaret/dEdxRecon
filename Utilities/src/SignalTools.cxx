#include "SignalTools.h"


/* GENERAL MATH*/

// Heaviside
float Heaviside(const float& t, const float& t_0){
	if(t_0 >	t)	return 0.0;
	if(t_0 == t)	return 0.5;
	if(t_0 <	t)	return 1.0;
	else			return -999.0;
}







/* SIGNAL MODELISATION*/

// Dirac Pulse Response function
TH1F* DPR(std::string name, const float& tmin, float const& tmax, const float& t_0, const int& nbins, const int& iC, const float& PT, const float& TB){
	float Q	= 2./3.; 
	float ws = 2*TB/PT;
	// if(PT == 200){ws = 0.4172; Q = 0.665; }
	// if(PT == 412){ws = 0.19518; Q = 0.6368; }
	name							= name + "_" + std::to_string(iC);
	TH1F* h1f_DPR					= new TH1F(name.c_str(), "Electronics pulse response function;time bin;signal (normalized)", nbins, tmin, tmax);
	float step						= (tmax-tmin)/nbins;
	float DPRmax					= -999;
	for (int t = tmin; t < tmax; t+=step){
	float arg					= ws*(t-t_0)/2 * sqrt(4-1/pow(Q,2));
	float DPR					= Heaviside(t, t_0)*(exp(-ws*(t-t_0)) + exp(-(ws*(t-t_0)/(2*Q))) * (sqrt((2*Q-1)/(2*Q+1)) * sin(arg) - cos(arg) ) );
	h1f_DPR->					SetBinContent((t-tmin)/step, DPR);
	if(DPR > DPRmax) DPRmax		= DPR;
	}
	h1f_DPR->						Scale(1/DPRmax);
	return h1f_DPR;
}