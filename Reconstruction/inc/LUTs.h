#ifndef LUT_H
#define LUT_H

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class EramInfo : public TObject
{
public:
	int Id;
	int Position;
	int XX;
	int YY;
	float RC;
	float Gain;
	float Resolution;

	int Endplate;
	bool InbTPC;
	bool IntTPC;

	EramInfo() {}
	virtual ~EramInfo() {}

	ClassDef(EramInfo, 2)
};

namespace Reconstruction{
	
	/* ERAM MAPS READING --------------------------------------------------------------------------------------------------------------------------- */
	class ERAMMaps {
	public:
		/* Constructor */
		ERAMMaps(const std::string& file	 = "$HOME/Documents/Code/CPP_projects/Read_ERAM_MAP/ERAMinfo.root");
		virtual ~ERAMMaps();
		
		int	ID(const int& position);
		float RC(const int& position, const int& iX, const int& iY);
		float Gain(const int& position, const int& iX, const int& iY);
		float Resolution(const int& position, const int& iX, const int& iY);
		float MeanGain(const int& position);
		float MeanRC(const int& position);

	private:
		bool verbose	 = false;
		std::string fFile;
		std::vector<int> fID;
		std::vector<float> v_sides;
		
		float fGain		[34][36][32]; // [position][iX][iY]
		float fRC			[34][36][32];
		float fResolution	[34][36][32]; 
		std::vector<float> fmean_gain;
		std::vector<float> fmean_RC;

		void Load();
		void setGain		(const int& position, const int& iX, const int& iY, const float& gain) ;
		void setRC		(const int& position, const int& iX, const int& iY, const float& RC) ;
		void setResolution (const int& position, const int& iX, const int& iY, const float& resolution) ;
		void setMeanGain	(const int& position, const float& meanGain) ;
		void setMeanRC	(const int& position, const float& meanRC) ;
		void FillHoles();

		std::vector<int> channel2iD	 = {24, 30, 28, 19, 21, 13,	9,	2, 26, 17, 23, 29,	1, 10, 11,	3,	/*bottom HATPC*/ 
									47, 16, 14, 15, 42, 45, 37, 36, 20, 38,	7, 44, 43, 39, 41, 46,	/*top	HATPC*/
									12, 18}; // CERN22 MockUp and prototype


	

		//------------------------------Data Members-----------------------//
		TFile	*pFile ;
		TTree	*pTree ;
		TBranch	*pBranch;

		// Leaves
		int			fid;
		int			fpos;
		int			fx;
		int			fy;
		float		frc;
		float		fgain;
		float		fres;
	};



	/* Look Up Tables for XP method ---------------------------------------------------------------------------------------------------------------- */
	class LUT {
	public:
		/* Constructor */
		LUT(const int& transDiffCoeff, const int& peakingTime);
		LUT(const std::string& file);

		virtual ~LUT();
		
		float getRatio(const float& phi, const float& d, const float& RC, const float& z);

	private:
	//	std::string fhatReconRoot;
	//	std::string fhatReconConfig;
		std::string fFile_LUT;
	//	int fTransDiffCoeff	= ND::TOARuntimeParams::Get().GetParameterI("hatRecon.TransDiffCoeff");	
		int fTransDiffCoeff	= 350;	

		static constexpr float PAD_DIAG	= 15.20; // sqrt(pow(11.28,2) + pow(10.19, 2))

		static const int sN_PHI		= 150;
		static const int sN_D		= 150;
		static const int sN_RC		= 41;
		static const int sN_Z		= 21;
		
		float fValue[sN_PHI][sN_D][sN_RC][sN_Z];

		static constexpr float sSTEP_PHI	= (90-2e-6)/(sN_PHI-1);
		static constexpr float sSTEP_D	= PAD_DIAG/2/(sN_D-1);
		static constexpr float sSTEP_RC	= 200./(sN_RC-1);
		static constexpr float sSTEP_Z	= 1000./(sN_Z-1);

		void Load();

	
		//------------------------------Data Members-----------------------//

		TFile *pFile_LUT ;
		TTree *pTree_LUT ;

		// Leaves
		float		fweight;
		float		fd;
		float		fphi;
		float		fRC;
		float		fz;
	};

}

#endif