#ifndef Uploader__V2_H
#define Uploader__V2_H

#include "Misc.h"
#include "Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_MockUp_V2 : public Uploader	{
public:
	/** Constructor */
	Uploader_MockUp_V2(
			const std::string& SampleFile,
			Model_ReadOutGeometry* pModel_ReadOutGeometry ,
			Model_Electronics*	pModel_Electronics,
			Model_Charge1D*		pModel_Charge1D		
	);
	virtual ~Uploader_MockUp_V2();
			
	virtual Event* GiveMe_Event(
								const int& iEvent, 
								const int& ModuleNber_Input, 
								const int& Data_to_Use,
								const int& CloseWF
								);

	virtual Event* GiveMe_Event(
								const int& iEvent, 
								const int& ModuleNber_Input, 
								const int& Data_to_Use
								);
	
	void SetBeforeMerging(const int& i_SetBeforeMerging);
	void Setwap_XY(const int& i_Swap_XY);

//------------------------------Data Members-----------------------//
public:
	Int_t										fCurrent;

	Int_t										ev;
	Int_t										track;
	ULong64_t									date; 			// Added since code update contains the date of the events !
	Bool_t										beforeMerging; 	// Added since code update tells if this track has been saved before or after merging algorithm
	Double_t									dEdx;
	Double_t									angle_yz;
	Double_t									angle_xy;
	Int_t										rob_clusters;
	Double_t									quality;
	Double_t									mom;
	Double_t									sina;
	Double_t									offset;
	Int_t										max_mult;
	Double_t									mean_mult;
	std::vector<int>							*multiplicity;
	std::vector<int>							*charge;
	std::vector<double>							*residual;
	std::vector<double>							*residual_corr;
	std::vector<std::vector<double>> 			*dx;
	std::vector<std::vector<double>> 			*qfrac;
	std::vector<std::vector<int>>				*time;
	std::vector<double>							*clust_pos;
	std::vector<double>							*clust_pos_err;
	std::vector<double>							*track_pos;
	std::vector<int>							*module;
	std::vector<std::vector<int>>				*pad_charge;
	std::vector<std::vector<int>>				*pad_time;
	std::vector<std::vector<int>>				*wf_width;
	std::vector<std::vector<int>>				*wf_fwhm;
	std::vector<std::vector<int>>				*pad_x;
	std::vector<std::vector<int>>				*pad_y;
	std::vector<std::vector<std::vector<int>>>	*pad_wf_q;

	// List of branches
	TBranch		*b_ev;
	TBranch		*b_track;
	TBranch		*b_date;
	TBranch		*b_beforeMerging;
	TBranch		*b_dEdx;
	TBranch		*b_angle_yz;
	TBranch		*b_angle_xy;
	TBranch		*b_rob_clusters;
	TBranch		*b_quality;
	TBranch		*b_mom;
	TBranch		*b_sina;
	TBranch		*b_offset;
	TBranch		*b_max_mult;
	TBranch		*b_mean_mult;
	TBranch		*b_multiplicity;
	TBranch		*b_charge;
	TBranch		*b_residual;
	TBranch		*b_residual_corr;
	TBranch		*b_dx;
	TBranch		*b_qfrac;
	TBranch		*b_time;
	TBranch		*b_clust_pos;
	TBranch		*b_clust_pos_err;
	TBranch		*b_track_pos;
	TBranch		*b_module;
	TBranch		*b_pad_charge;
	TBranch		*b_pad_time;
	TBranch		*b_wf_width;
	TBranch		*b_wf_fwhm;
	TBranch		*b_pad_x;
	TBranch		*b_pad_y;
	TBranch		*b_pad_wf_q;

	void Init();

private:

	int 		m_BeforeMerging;
	int 		m_Swap_XY;
	int 		m_Nx;
	int 		m_Ny;

};

#endif

