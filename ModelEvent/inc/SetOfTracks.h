#ifndef SetOfTracks_H
#define SetOfTracks_H

#include "Misc.h"
#include "Track.h"

#include "TH1F.h"

/////////////////////////////////////////////////////////////
class SetOfTracks {
public:
	SetOfTracks();
	virtual ~SetOfTracks();


//------------------------------Track Addition-----------------------//
	void Add_Track(const Track* pTrack);


//------------------------------Track Acess-----------------------//
	int			Get_NberOfTrack() const;
	const Track* Get_Track (int Index1D) const;


//------------------------------Prints-----------------------//
	void DumpRec() const;


//------------------------------Plots-----------------------//
	int	Get_MinimalNberOfEntries();
	void Set_MinimalNberOfEntries(const int& MinimalNberOfEntries);	
	
	TH1F*	GiveMe_pTH1F_YTrackInTracks (	
									const std::string& TAG,
									const int& iYBeam
								);
	TH1F*	GiveMe_pTH1F_YTrackInTracks (	
									const std::string& TAG,
									const int&	Nbins ,
									const double& Ymin ,
									const double& Ymax
								);
	TH1F*	GiveMe_pTH1F_YTrackInTracks (	
									const std::string& TAG,
									const int& iYBeam,
									const int& iCol
								);
	TH1F*	GiveMe_pTH1F_YTrackInTracks (	
									const std::string& TAG,
									const int&	Nbins ,
									const double& Ymin ,
									const double& Ymax ,
									const int& iCol
								);

//
	TH1F*	GiveMe_pTH1F_YFitCol (	
									const std::string& TAG,
									const int& iYBeam
										);
	TH1F*	GiveMe_pTH1F_YFitCol (	
									const std::string& TAG,
									const int&	Nbins ,
									const double& Ymin ,
									const double& Ymax
										);
	TH1F*	GiveMe_pTH1F_YFitCol (	
									const std::string& TAG,
									const int& iYBeam,
									const int& iCol
										);
	TH1F*	GiveMe_pTH1F_YFitCol (	
									const std::string& TAG,
									const int&	Nbins ,
									const double& Ymin ,
									const double& Ymax ,
									const int& iCol
										);
//
	TH1F*	GiveMe_pTH1F_YTrackYPadInTracks(	
											const std::string& TAG,
											const int& iCol
										);
	TH1F*	GiveMe_pTH1F_YTrackYPadInTracks(	
											const std::string& TAG,
											const int& iCol,
											const int& Nbins, 
											const double& Xmin, 
											const double& Xmax
										);
	TH1F*	GiveMe_pTH1F_YTrackYPadLeadingInTracks(	
											const std::string& TAG,
											const int& iCol
										);
	TH1F*	GiveMe_pTH1F_YTrackYPadLeadingInTracks(	
											const std::string& TAG,
											const int& iCol,
											const int& Nbins, 
											const double& Xmin, 
											const double& Xmax
										);
//
	TH1F*	GiveMe_pTH1F_TrackDeltaT(	
											const std::string& TAG,
											const int& iCol
										);
//
	TH1F*	GiveMe_pTH1F_Ch2MinPerNODF(	
										const std::string& TAGFinal ,
										const double& Xmax
										);
	TH1F*	GiveMe_pTH1F_Ch2Min	(	
									const std::string& TAGFinal ,
									const double& Xmax
									);
	TH1F*	GiveMe_pTH1F_Residual (	
									const std::string& TAGFinal ,
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma 
									);
	TH1F*	GiveMe_pTH1F_Residual (	
									const std::string& TAGFinal ,
									const int& iX, 
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma	
									);
									
	TH1F*	GiveMe_pTH1F_Residual (	
									const std::string& TAGFinal ,
									const int& iX, 
									double YL, 
									double YH, 
									double YL_Rescaled , 
									double YH_Rescaled , 
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma 
									);

	
	TH1F*	GiveMe_pTH1F_Pull	(
									const std::string& TAGFinal 
									);
	TH1F*	GiveMe_pTH1F_Pull	(
									const std::string& TAGFinal ,
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma 
									);
	TH1F*	GiveMe_pTH1F_Pull	(
									const std::string& TAGFinal ,
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma ,
									const double& XminH,
									const double& XmaxH 
									);
	TH1F*	GiveMe_pTH1F_Pull	(
									const std::string& TAGFinal ,
									const int& iX, 
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma
									);
	TH1F*	GiveMe_pTH1F_Pull	(
									const std::string& TAGFinal ,
									const int& iX, 
									double& Mean, 
									double& eMean, 
									double& Sigma, 
									double& eSigma ,
									const double& XminH,
									const double& XmaxH 
									);
	

private:
///////////////////////////////////
	int m_MinimalNberOfEntries;

	int m_NberOfTracks;
	std::vector < const Track* > V_Track;


};



#endif

