#include "SampleTools/ReadRCmap.h"
#include "SampleTools/Uploader_ReadRCmap.h"

ReadRCmap::ReadRCmap()
{
  // Default constructor  
}

ReadRCmap::ReadRCmap(std::string EramName)
{
//
  m_Dir = "GainRCMaps/RCfiles/" ;
  m_EramName = EramName ;

  ConstructorEnd() ;
      
}

ReadRCmap::ReadRCmap(std::string Dir,std::string EramName)
{
//
  m_Dir = Dir ;
  m_EramName = EramName ;
    
  ConstructorEnd() ;
      
}

void ReadRCmap::ConstructorEnd()
{
//
  p_V_Data = 0 ;
  p_V_HasBeenSet = 0 ;
  

  m_FileName = "BIDON" ;
  if (m_EramName=="ERAM01" ) m_FileName = m_Dir + "ERAM01_RCdata_01.root" ;
  if (m_EramName=="ERAM02" ) m_FileName = m_Dir + "ERAM02_RCdata_02.root" ;
  if (m_EramName=="ERAM03" ) m_FileName = m_Dir + "ERAM03_RCdata_03.root" ;
  if (m_EramName=="ERAM07" ) m_FileName = m_Dir + "ERAM07_RCdata_04.root" ;
  if (m_EramName=="ERAM09" ) m_FileName = m_Dir + "ERAM09_RCdata_05.root" ;
  if (m_EramName=="ERAM10" ) m_FileName = m_Dir + "ERAM10_RCdata_06.root" ;
  if (m_EramName=="ERAM11" ) m_FileName = m_Dir + "ERAM11_RCdata_07.root" ;
  if (m_EramName=="ERAM12" ) m_FileName = m_Dir + "ERAM12_RCdata_08.root" ;
  if (m_EramName=="ERAM13" ) m_FileName = m_Dir + "ERAM13_RCdata_09.root" ;
  if (m_EramName=="ERAM14" ) m_FileName = m_Dir + "ERAM14_RCdata_10.root" ;
  if (m_EramName=="ERAM15" ) m_FileName = m_Dir + "ERAM15_RCdata_11.root" ;
  if (m_EramName=="ERAM16" ) m_FileName = m_Dir + "ERAM16_RCdata_12.root" ;
  if (m_EramName=="ERAM17" ) m_FileName = m_Dir + "ERAM17_RCdata_13.root" ;
  if (m_EramName=="ERAM18" ) m_FileName = m_Dir + "ERAM18_RCdata_14.root" ;
  if (m_EramName=="ERAM19" ) m_FileName = m_Dir + "ERAM19_RCdata_15.root" ;
  if (m_EramName=="ERAM20" ) m_FileName = m_Dir + "ERAM20_RCdata_16.root" ;
  if (m_EramName=="ERAM21" ) m_FileName = m_Dir + "ERAM21_RCdata_17.root" ;
  if (m_EramName=="ERAM23" ) m_FileName = m_Dir + "ERAM23_RCdata_18.root" ;
  if (m_EramName=="ERAM24" ) m_FileName = m_Dir + "ERAM24_RCdata_19.root" ;
  if (m_EramName=="ERAM26" ) m_FileName = m_Dir + "ERAM26_RCdata_20.root" ;
  if (m_EramName=="ERAM28" ) m_FileName = m_Dir + "ERAM28_RCdata_21.root" ;
  if (m_EramName=="ERAM29" ) m_FileName = m_Dir + "ERAM29_RCdata_22.root" ;
  if (m_EramName=="ERAM30" ) m_FileName = m_Dir + "ERAM30_RCdata_23.root" ;
  
  if (m_FileName=="BIDON"){
    std::cout 
      << "ReadRCmap::ReadRCmap "
      << " ERROR. No file for EramName " << m_EramName
      << std::endl ;
      abort();
  }
  
  // std::cout << "ReadRCmap::ReadRCmap "  
  //   << "m_FileName = " << m_FileName << std::endl ;

  Uploader_ReadRCmap aUploader_ReadRCmap(m_FileName);

  p_V_Data         = new std::vector< double >(100*100,0.) ;
  p_V_HasBeenSet = new std::vector< int    >(100*100,0 ) ;
  
  int iEntry_Max = aUploader_ReadRCmap.Get_NberOfEntries() ;
  for (int iEntry=0; iEntry<iEntry_Max;iEntry++) {
     aUploader_ReadRCmap.GiveMe_Entry(iEntry);
     int    iX   = aUploader_ReadRCmap.Get_iX()  ;
     int    iY   = aUploader_ReadRCmap.Get_iY()  ;
     double Data = aUploader_ReadRCmap.Get_Data();
     SetData(iX,iY,Data);
//      std::cout 
//         << " iX "   << iX 
//         << " iY "   << iY 
//         << " Data " << Data
//         << std::endl ; 
  }
  
//Check
  int iX_Max = 36 ;
  for (int iX=0; iX<iX_Max;iX++) {
    int iY_Max = 32 ;
    for (int iY=0; iY<iY_Max;iY++) {
      int Status  = HasBeenSet( iX, iY) ;
      // if (Status==0){
      //   std::cout << std::endl ;
      //   std::cout 
      //     << "ReadRCmap::ReadRCmap "
      //     << " No Data for iX=" << iX << " and " << " iY=" << iY 
      //     << " in " << m_FileName 
      //     << std::endl ;
      // }
    }
  }
  

}

ReadRCmap::~ReadRCmap()
{
  delete p_V_Data ;
  p_V_Data = 0 ;
  delete p_V_HasBeenSet ;
  p_V_HasBeenSet = 0 ;
}

double ReadRCmap::GetData(const int& iX, const int& iY, int& Status)
{
  Status = 0 ;
  int Linear = GetLinear(iX,iY);
  if (HasBeenSet(iX,iY)==0) return 0 ;
  Status = 1 ;
  return (*p_V_Data)[Linear] ;
  
}

void ReadRCmap::SetData(const int& iX, const int& iY, const double& Data)
{
  int Linear = GetLinear(iX,iY);
  (*p_V_Data        )[Linear] = Data ;
  (*p_V_HasBeenSet)[Linear] = 1    ;
}

int ReadRCmap::GetLinear(const int& iX, const int& iY)
{
  return ( iX + 100* iY) ;
}

int ReadRCmap::HasBeenSet(const int& iX, const int& iY)
{
  int Linear = GetLinear(iX,iY);
  return (*p_V_HasBeenSet)[Linear] ;
}
