#include "THATdEdx.hxx"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>  // for std::find
#include <iterator>   // for std::begin, std::end


/* ERAM MAPS READING ------------------------------------------------------------------------------------------------------------------ */
// Default constructor
PID::ERAMMaps::ERAMMaps()
{
	// fHATPath = getenv("HATRECONROOT");
	// fFile = (fHATPath + "/parameters/Mapping/ERAM_maps.root").c_str();
    fFile = "$HOME/Docuements/Code/CPP_projects/Read_ERAM_MAP/ERAMinfo.root";

	std::cout << "ERAM maps: LOADING " << fFile << std::endl;
	Load();
	std::cout << "ERAM maps: LOADED" << std::endl;
}

PID::ERAMMaps::ERAMMaps(const std::string& file)
{
	fFile     = file ;

	std::cout << "ERAM maps: LOADING " << fFile << std::endl;
	Load();
	std::cout << "ERAM maps: LOADED" << std::endl;
}

// Destructor
PID::ERAMMaps::~ERAMMaps()
{
	pFile->Close();
	delete pFile;
	for(int i=0;i<(int)fID.size();i++) fID[i] = 0;
	fID.clear();
	for(int i=0;i<32;i++){
		for(int j=0;j<36;j++){
			for(int k=0;k<32;k++){
				fGain[i][j][k] = 0;
				fRC[i][j][k] = 0;
				fResolution[i][j][k] = 0;
			}
		}
	}
}

// Load the ERAM maps
void PID::ERAMMaps::Load()
{
	pFile        = TFile::Open(fFile.c_str(),"READ");
	pTree        = (TTree*) pFile->Get("Infotree");
	pBranch      = pTree->GetBranch("EramInfo");
	int nentries  = pTree->GetEntries();
	pTree->SetMakeClass(1); // Because ROOT version in ND280 is old
	pTree->SetBranchAddress("Id",         &fid,   &pBranch);
	pTree->SetBranchAddress("Position",   &fpos,  &pBranch);
	pTree->SetBranchAddress("XX",         &fx,    &pBranch);
	pTree->SetBranchAddress("YY",         &fy,    &pBranch);
	pTree->SetBranchAddress("RC",         &frc,   &pBranch);
	pTree->SetBranchAddress("Gain",       &fgain, &pBranch);
	pTree->SetBranchAddress("Resolution", &fres,  &pBranch);
	pTree->GetVal(0);

	// Initializing all values to 0
	for(int i=0;i<32;i++) for(int j=0;j<36;j++) for(int k=0;k<32;k++){
		setGain       (i,j,k,0);
		setRC         (i,j,k,0);
		setResolution (i,j,k,0);
	}

	// Filling the maps
	for (int i=0; i<nentries;i++) {
		pTree->GetEntry(i);
		if(fpos > 31) continue;
		setGain       (fpos,fx,fy,fgain);
		setRC         (fpos,fx,fy,frc);
		setResolution (fpos,fx,fy,fres);
	}

	// Fill holes in the maps
	FillHoles();
}


/* Public functions */
// Getters
int PID::ERAMMaps::ID(const int& position)
{
  	return channel2iD[position];
}

double PID::ERAMMaps::RC(const int& position, const int& iX, const int& iY)
{
  	return fRC[position][iX][iY] ;
}

double PID::ERAMMaps::Gain(const int& position, const int& iX, const int& iY)
{
  	return fGain[position][iX][iY] ;
}

double PID::ERAMMaps::Resolution(const int& position, const int& iX, const int& iY)
{
  	return fResolution[position][iX][iY] ;
}



/* Private functions */
// Setters
// ERAMs on endplates 1&3 are X-flipped compared to their maps in the file
void PID::ERAMMaps::setGain(const int& position, const int& iX, const int& iY, const double& gain)
{
	if(position%16 < 8) fGain[position][iX][iY]    = gain ;
	else                fGain[position][35-iX][iY] = gain ;
}

void PID::ERAMMaps::setRC(const int& position, const int& iX, const int& iY, const double& RC)
{
	if(position%16 < 8) fRC[position][iX][iY]    = RC ;
	else                fRC[position][35-iX][iY] = RC ;
}

void PID::ERAMMaps::setResolution(const int& position, const int& iX, const int& iY, const double& resolution)
{
	if(position%16 < 8) fResolution[position][iX][iY]    = resolution ;
	else                fResolution[position][35-iX][iY] = resolution ;
}


void PID::ERAMMaps::FillHoles()
{
	for(int i=0;i<32;i++){
		for(int iY = 0 ; iY < 32 ; iY++){
		for(int iX = 0 ; iX < 36 ; iX++){
			double gain = Gain(i,iX,iY) ;
			double rc   = RC(i,iX,iY) ;

			// Fill holes of maps if there is any
			std::vector<float> v_sides;
			if(gain == 0){
			// std::cout << "ERAM#" << ID(i) << ": " << std::setw(4) << "Gain hole in (iX,iY) = (" << iX << "," << iY << ") | "; 
			if(iX>0)  v_sides.push_back(Gain(i, iX-1,iY  )) ;
			if(iX<35) v_sides.push_back(Gain(i, iX+1,iY  )) ;
			if(iY>0)  v_sides.push_back(Gain(i, iX,  iY-1)) ;
			if(iY<31) v_sides.push_back(Gain(i, iX,  iY+1)) ;
			float n_sides = 0;
			for(int i = 0; i<4; i++) if (v_sides[i]!=0){ // additionnal step to discard empty neighbours
				gain += v_sides[i];
				n_sides++;
			}
			gain /= n_sides;
			setGain(i, iX, iY, gain);
			// std::cout << "value reset at " << gain << std::endl ;
			}
			v_sides.clear();
			if(rc == 0){
			// std::cout << "ERAM#" << ID(i) << ": " << std::setw(4) << "RC hole in (iX,iY) = (" << iX << "," << iY << ") | "; 
			if(iX>0)  v_sides.push_back(RC(i, iX-1,iY  )) ;
			if(iX<35) v_sides.push_back(RC(i, iX+1,iY  )) ;
			if(iY>0)  v_sides.push_back(RC(i, iX,  iY-1)) ;
			if(iY<31) v_sides.push_back(RC(i, iX,  iY+1)) ;
			float n_sides = 0;
			for(int i = 0; i<4; i++) if (v_sides[i]!=0){ // additionnal step to discard empty neighbours
				rc += v_sides[i];
				n_sides++;
			}
			rc /= n_sides;
			setRC(i, iX, iY, rc);
			// std::cout << "value reset at " << rc << std::endl ;
			}
			v_sides.clear();
		} // iY
		} // iX
	}
}




/* Look Up Tables for XP method ------------------------------------------------------------------------------------------------------------------ */
// Default constructor
PID::LUT::LUT(const int& transDiffCoeff, const int& peakingTime){
	// fhatReconRoot = getenv("HATRECONROOT");
	// fhatReconConfig = getenv("HATRECONCONFIG");
	// fFile=(fhatReconRoot + "/" + fhatReconConfig + Form("/PID_LUT/LUT_Dt%i_PT412_nphi%i_nd%i_nRC%i_nZ%i.root", fTransDiffCoeff, sN_PHI, sN_D, sN_RC, sN_Z)); // Dt = 310 (B field) or 350 (no B field) | Peaking Time 412ns
	// fFile=(fHATPath + Form("$HOME/public/LUT_dEdx/LUT_Dt%i_PT412_nphi%i_nd%i_nRC%i_nZ%i.root", fTransDiffCoeff, sN_PHI, sN_D, sN_RC, sN_Z));
	fFile = Form("$HOME/Documents/Code/Python/LUT/LUT_Dt%i_PT%i_nphi150_nd150_nRC41_nZ21.root", transDiffCoeff, peakingTime);
    std::cout << "dEdx LUT: LOADING " << fFile << std::endl;
	Load();
	std::cout << "dEdx LUT: LOADED" << std::endl;
}

PID::LUT::LUT(const std::string& file)
	{
	fFile = file;
	std::cout << "dEdx LUT: LOADING " << fFile << std::endl;
	Load();
	std::cout << "dEdx LUT: LOADED" << std::endl;
}

// Destructor
PID::LUT::~LUT()
	{
	pFile->Close();
	delete pFile;
	for(int i=0;i<sN_PHI;i++) for(int j=0;j<sN_D;j++) for(int k=0;k<sN_RC;k++) for(int l=0;l<sN_Z;l++) fValue[i][j][k][l] = 0;
}


// Load the ERAM maps
void PID::LUT::Load()
{  
	pFile                      = TFile::Open(fFile.c_str(),"READ");
	pTree                      = (TTree*) pFile->Get("outTree");
	pTree->                      SetBranchAddress("weight",        &fweight);
	pTree->                      SetBranchAddress("angle",         &fphi);
	pTree->                      SetBranchAddress("impact_param",  &fd);
	pTree->                      SetBranchAddress("RC",            &fRC);
	pTree->                      SetBranchAddress("drift_dist",    &fz);

	
	// Initializing all values to 0
	for(int i=0;i<sN_PHI;i++) for(int j=0;j<sN_D;j++) for(int k=0;k<sN_RC;k++) for(int l=0;l<sN_Z;l++) fValue[i][j][k][l] = 0;

	int nentries = pTree->GetEntries();
	int id, iphi, iRC, iz;
	for (int i=0; i<nentries;i++) {
		pTree->                      GetEntry(i);
		iphi                        = (int)std::round((fphi-1e-6)/sSTEP_PHI);
		id                          = (int)std::round(fd/sSTEP_D);
		iRC                         = (int)std::round((fRC-50)/sSTEP_RC);
		iz                          = (int)std::round(fz/sSTEP_Z);
		fValue[iphi][id][iRC][iz]   = fweight;
	}
}


float PID::LUT::getRatio(const float& phi, const float& d, const float& RC, const float& z)
{
	// Determine the indices of the 8 points surrounding the point of interest
	float iphi     = (phi-1e-6)/sSTEP_PHI;
	float iphi_min = std::min(std::floor((phi-1e-6)/sSTEP_PHI), (double)sN_PHI-1);
	float iphi_max = std::max(std::ceil((phi-1e-6)/sSTEP_PHI), 0.0);
	float id       = d/sSTEP_D;
	float id_min   = std::min(std::floor(d/sSTEP_D), (float)sN_D-1);
	float id_max   = std::max(std::ceil(d/sSTEP_D), (float)0);
	float iRC      = (RC-50)/sSTEP_RC;
	float iRC_min  = std::min(std::floor((RC-50)/sSTEP_RC), (float)(sN_RC-1));
	float iRC_max  = std::max(std::ceil((RC-50)/sSTEP_RC), (float)0);
	float iz       = z/sSTEP_Z;
	float iz_min   = std::min(std::floor(z/sSTEP_Z), (float)(sN_Z-1));
	float iz_max   = std::max(std::ceil(z/sSTEP_Z), (float)0);

	// Determine the weights for the 8 points
	float w_phi, w_d, w_RC, w_z;
	if(iphi_min == iphi_max)  w_phi = 1;
	else                      w_phi = 1 - (iphi - iphi_min)/(iphi_max - iphi_min);
	if(id_min == id_max)      w_d   = 1;
	else                      w_d   = 1 - (id - id_min)/(id_max - id_min);
	if(iRC_min == iRC_max)    w_RC  = 1;
	else                      w_RC  = 1 - (iRC - iRC_min)/(iRC_max - iRC_min);
	if(iz_min == iz_max)      w_z   = 1;
	else                      w_z   = 1 - (iz - iz_min)/(iz_max - iz_min);

	float interpolated_value = 0;
	for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) for (int k = 0; k < 2; ++k) for (int l = 0; l < 2; ++l) 
		interpolated_value += fValue[(int)iphi_min + i][(int)id_min + j][(int)iRC_min + k][(int)iz_min + l] *
							(i == 0 ? w_phi : (1 - w_phi)) * (j == 0 ? w_d : (1 - w_d)) * (k == 0 ? w_RC : (1 - w_RC)) * (l == 0 ? w_z : (1 - w_z));

	return interpolated_value;
}



/* Equivalent of numpy linspace (npoints uniformly spaced between start and end) */
std::vector<double> linspace(double start, double end, int numPoints) {
	std::vector<double> result(numPoints);
	double step = (end - start) / (numPoints - 1);
  
	for (int i = 0; i < numPoints; ++i) {
		result[i] = start + i * step;
  	}
  return result;
}