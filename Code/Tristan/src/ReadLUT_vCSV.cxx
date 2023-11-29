#include "Tristan/ReadLUT_vROOT.h"
#include "Tristan/Misc_Functions.h"
#include "Misc/Util.h"



Interpol2::Interpol2(const std::string& file_name)
{
  Load(file_name) ;
}
Interpol2::~Interpol2()
{}

void Interpol2::Load(const std::string& file_name)
{
  data        = readCSV(file_name) ;
}

float Interpol2::Interpolated(float& dconv, float& phiconv)
{
  int phi0    = (int)floor(phiconv) ;
  int phi1    = (int)ceil(phiconv) ;
  if(phi0 == 199) phi1 = phi0 ;
  int d0      = (int)floor(dconv) ;
  int d1      = (int)ceil(dconv) ;
  if(d0 == 199) d1 = d0 ;

  float IP0   = data[d0][phi0] + (phiconv-phi0)*(data[d0][phi1] - data[d0][phi0]) ;
  float IP1   = data[d1][phi0] + (phiconv-phi0)*(data[d1][phi1] - data[d1][phi0]) ;

  return IP0 + (dconv-d0)*(IP1 - IP0) ;  
}



/////////////////////////////////////////////////////////////



Interpol3::Interpol3(const std::string& folder_name, int& nZ)
{
  Load( folder_name, nZ) ;
}
Interpol3::~Interpol3()
{}

void Interpol3::Load(const std::string& folder_name, int& nZ)
{
  for(int iZ = 0 ; iZ < nZ ; iZ++) v_FileZ.push_back(folder_name + "LUT_z" + std::to_string(iZ*50) + ".csv") ;
  for(int iFile = 0 ; iFile < nZ; iFile++) V_pInterpol2.push_back(new Interpol2(v_FileZ[iFile])) ;
}

float Interpol3::Interpolated(float& dconv, float& phiconv, float& zconv)
{
  int z0      = (int)floor(zconv) ;
  int z1      = (int)ceil(zconv) ;
  if(z0 == 21) z1 = z0 ;
  float IP0  = V_pInterpol2[z0]->Interpolated(dconv, phiconv) ;
  float IP1  = V_pInterpol2[z1]->Interpolated(dconv, phiconv) ;

  return IP0 + (zconv-z0)*(IP1 - IP0) ; 
}

Interpol3 GiveMe_LUT3(const std::string& folder_name, int& nZ){
  Interpol3 LUT(folder_name, nZ) ;
  return LUT ;
}



/////////////////////////////////////////////////////////////



Interpol4::Interpol4()
{
  // Default constructor
}
Interpol4::Interpol4(const std::string& folder_name, int& nZ, int& nRC)
{
  std::cout << "LUT: LOADING" << folder_name << std::endl ;
  Load(folder_name, nZ, nRC) ;
  std::cout << "LUT: LOADED" << std::endl ;
}
Interpol4::~Interpol4()
{}

void Interpol4::Load(const std::string& folder_name, int& nZ, int& nRC)
{
  for(int iRC = 0 ; iRC < nRC ; iRC++) v_FileRC.push_back(folder_name + "LUT_RC" + std::to_string(iRC*5+50) + "/") ;
  for(int iFile = 0 ; iFile < nRC; iFile++) V_pInterpol3.push_back(new Interpol3(v_FileRC[iFile], nRC)) ;
}

float Interpol4::Interpolated(float& dconv, float& phiconv, float& zconv, float& RCconv)
{
  int RC0     = (int)floor(RCconv) ;
  int RC1     = (int)ceil(RCconv) ;
  if(RC0 == 20) RC1 = RC0 ;
  float IP0  = V_pInterpol3[RC0]->Interpolated(dconv, phiconv, zconv) ;
  float IP1  = V_pInterpol3[RC1]->Interpolated(dconv, phiconv, zconv) ;

  return IP0 + (RCconv-RC0)*(IP1 - IP0) ; 
}

Interpol4 GiveMe_LUT(const std::string& folder_name, int& nZ, int& nRC){
  Interpol4 LUT(folder_name, nZ, nRC) ;
  return LUT ;
}