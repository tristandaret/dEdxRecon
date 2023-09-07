#include "SampleTools/Uploader_ReadRCmap.h"

Uploader_ReadRCmap::Uploader_ReadRCmap(
            const std::string& RCmapFile 
)
{  
  std::cout << std::endl ;
  std::cout << "Uploader_ReadRCmap::Uploader_ReadRCmap " 
    << "RCmapFile = " << RCmapFile
    << std::endl ;
      
  m_RCmapFile = RCmapFile ;

  p_TFile = TFile::Open(m_RCmapFile.c_str(),"READ");
  if (!p_TFile) {
    std::cout << "Uploader_ReadRCmap::Uploader_ReadRCmap "
              <<" ERROR. " 
              << "Couldn't open root file : " << m_RCmapFile
              <<std::endl;
    abort() ;
  }
  
  p_TTree   = (TTree*) p_TFile->Get("RCtree");
  if (!p_TTree){
    std::cout << "Uploader_ReadRCmap::Uploader_ReadRCmap "
              <<" ERROR. " 
              << "p_TTree==0"
              <<std::endl;
    abort() ;
  }

  m_NberOfEntries = (int) p_TTree->GetEntries();
  
  std::cout << "Uploader_ReadRCmap::Uploader_ReadRCmap " 
    << "m_NberOfEntries = " << m_NberOfEntries
    << std::endl ;

  Init();

}


Uploader_ReadRCmap::~Uploader_ReadRCmap(){}

void Uploader_ReadRCmap::GiveMe_Entry(
                               const int& iEntry
){
//   std::cout << " Uploader_ReadRCmap::GiveMe_Event "
//             << " iEntry= " << iEntry
//             << " Get_NberOfEntries() " << Get_NberOfEntries()
//             << std::endl; 
  if (iEntry >= Get_NberOfEntries() ) return ;
 
  if (!p_TTree) return ;
  fCurrent = iEntry;
  
  p_TTree->GetEntry(iEntry);
 
}

int    Uploader_ReadRCmap::Get_iX()   { return Xpad  ; }
int    Uploader_ReadRCmap::Get_iY()   { return Ypad  ; }
double Uploader_ReadRCmap::Get_Data() { return RCval ; }

int Uploader_ReadRCmap::Get_NberOfEntries(){ return m_NberOfEntries; }

void Uploader_ReadRCmap::Init()
{
   // Set branch addresses and branch pointers

   fCurrent = -1;
   p_TTree->SetMakeClass(1);

   p_TTree->SetBranchAddress("Xpad", &Xpad, &b_Xpad);
   p_TTree->SetBranchAddress("Ypad", &Ypad, &b_Ypad);
   p_TTree->SetBranchAddress("RCval", &RCval, &b_RCval);

}
