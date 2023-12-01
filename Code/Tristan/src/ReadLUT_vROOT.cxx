#include "Tristan/ReadLUT_vROOT.h"
#include "Tristan/Misc_Functions.h"
#include "Misc/Util.h"



LUT2::LUT2(TFile* pTFile_LUT, const std::string& file_name)
{
  Load(pTFile_LUT, file_name) ;
}
LUT2::~LUT2()
{}

void LUT2::Load(TFile* pTFile_LUT, const std::string& file_name)
{
  h2          = (TH2F*)pTFile_LUT->Get<TH2F>(file_name.c_str())->Clone();
  h2->          SetDirectory(0);
  // DrawTH2("OUT_Tristan/", h2);
}

float LUT2::Interpolate(float& dconv, float& phiconv)
{
  int phi0    = std::max({(int)floor(phiconv),  0}) ;
  int phi1    = std::min({(int)ceil(phiconv),   199}) ;
  int d0      = std::max({(int)floor(dconv),    0}) ;
  int d1      = std::min({(int)ceil(dconv),     199}) ;
  // float IP0   = h2->GetBinContent(d0, phi0) + (phiconv-phi0)*(h2->GetBinContent(d0, phi1) - h2->GetBinContent(d0, phi0)) ;
  // float IP1   = h2->GetBinContent(d1, phi0) + (phiconv-phi0)*(h2->GetBinContent(d1, phi1) - h2->GetBinContent(d1, phi0)) ;
  float IP0   = h2->GetBinContent(phi0, d0) + (phiconv-phi0)*(h2->GetBinContent(phi1, d0) - h2->GetBinContent(phi0, d0)) ;
  float IP1   = h2->GetBinContent(phi0, d1) + (phiconv-phi0)*(h2->GetBinContent(phi1, d1) - h2->GetBinContent(phi0, d1)) ;

  return IP0 + (dconv-d0)*(IP1 - IP0) ;  
}



/////////////////////////////////////////////////////////////



LUT3::LUT3(TFile* pTFile_LUT, const std::string& th2_name, int& nZ)
{
  Load(pTFile_LUT, th2_name, nZ) ;
}
LUT3::~LUT3()
{}

void LUT3::Load(TFile* pTFile_LUT, const std::string& th2_name, int& nZ)
{
  for(int iZ = 0 ; iZ < nZ ; iZ++) v_th2_name.push_back(th2_name + "Z" + std::to_string(iZ*50)) ;
  for(int iFile = 0 ; iFile < nZ; iFile++) V_pLUT2.push_back(new LUT2(pTFile_LUT, v_th2_name[iFile])) ;
}

float LUT3::Interpolate(float& dconv, float& phiconv, float& zconv)
{
  int z0      = (int)floor(zconv) ;
  int z1      = (int)ceil(zconv) ;
  if(z0 == 21) z1 = z0 ;
  float IP0  = V_pLUT2[z0]->Interpolate(dconv, phiconv) ;
  float IP1  = V_pLUT2[z1]->Interpolate(dconv, phiconv) ;

  return IP0 + (zconv-z0)*(IP1 - IP0) ; 
}

LUT3 GiveMe_LUT3(TFile* pTFile_LUT, const std::string& th2_name, int& nZ){
  LUT3 LUT(pTFile_LUT, th2_name, nZ) ;
  return LUT ;
}



/////////////////////////////////////////////////////////////



LUT4::LUT4()
{
  // Default constructor
}
LUT4::LUT4(const std::string& filename, int& nZ, int& nRC)
{
  std::cout << "LUT: LOADING" << filename << std::endl ;
  Load(filename, nZ, nRC) ;
  std::cout << "LUT: LOADED" << std::endl ;
}
LUT4::~LUT4()
{}

void LUT4::Load(const std::string& filename, int& nZ, int& nRC)
{
  TFile* pTFile_LUT            = TFile::Open(filename.c_str()) ;
  for(int iRC = 0 ; iRC < nRC ; iRC++) v_th2_nameRC.push_back("LUT_2D_RC" + std::to_string(iRC*5+50) + "_") ;
  for(int iFile = 0 ; iFile < nRC; iFile++) V_pLUT3.push_back(new LUT3(pTFile_LUT, v_th2_nameRC[iFile], nZ)) ;
  pTFile_LUT->Close();
  delete pTFile_LUT;
}

float LUT4::Interpolate(float& dconv, float& phiconv, float& zconv, float& RCconv)
{
  int RC0     = (int)floor(RCconv) ;
  int RC1     = (int)ceil(RCconv) ;
  if(RC0 == 20) RC1 = RC0 ;
  float IP0  = V_pLUT3[RC0]->Interpolate(dconv, phiconv, zconv) ;
  float IP1  = V_pLUT3[RC1]->Interpolate(dconv, phiconv, zconv) ;

  return IP0 + (RCconv-RC0)*(IP1 - IP0) ; 
}

LUT4 GiveMe_LUT(const std::string& filename, int& nZ, int& nRC){
  LUT4 LUT(filename, nZ, nRC) ;
  return LUT ;
}