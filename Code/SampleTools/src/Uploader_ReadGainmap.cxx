#include "SampleTools/Uploader_ReadGainmap.h"

Uploader_ReadGainmap::Uploader_ReadGainmap(
            const std::string& GainmapFile 
)
{  
  std::cout << std::endl ;
  std::cout << "Uploader_ReadGainmap::Uploader_ReadGainmap " 
    << "GainmapFile = " << GainmapFile
    << std::endl ;
      
  m_GainmapFile = GainmapFile ;

  p_TFile = TFile::Open(m_GainmapFile.c_str(),"READ");
  if (!p_TFile) {
    std::cout << "Uploader_ReadGainmap::Uploader_ReadGainmap "
              <<" ERROR. " 
              << "Couldn't open root file : " << m_GainmapFile
              <<std::endl;
    abort() ;
  }
  
  p_TTree   = (TTree*) p_TFile->Get("Gaintree");
  if (!p_TTree){
    std::cout << "Uploader_ReadGainmap::Uploader_ReadGainmap "
              <<" ERROR. " 
              << "p_TTree==0"
              <<std::endl;
    abort() ;
  }

  m_NberOfEntries = (int) p_TTree->GetEntries();
  
  std::cout << "Uploader_ReadGainmap::Uploader_ReadGainmap " 
    << "m_NberOfEntries = " << m_NberOfEntries
    << std::endl ;

  Init();

}


Uploader_ReadGainmap::~Uploader_ReadGainmap(){}

void Uploader_ReadGainmap::GiveMe_Entry(
                               const int& iEntry
){
//   std::cout << " Uploader_ReadGainmap::GiveMe_Event "
//             << " iEntry= " << iEntry
//             << " Get_NberOfEntries() " << Get_NberOfEntries()
//             << std::endl; 
  if (iEntry >= Get_NberOfEntries() ) return ;
 
  if (!p_TTree) return ;
  fCurrent = iEntry;
  
  p_TTree->GetEntry(iEntry);
 
}

int    Uploader_ReadGainmap::Get_iX()   { return Xpad  ; }
int    Uploader_ReadGainmap::Get_iY()   { return Ypad  ; }
double Uploader_ReadGainmap::Get_Data() { return Gainval ; }

int Uploader_ReadGainmap::Get_NberOfEntries(){ return m_NberOfEntries; }

void Uploader_ReadGainmap::Init()
{
   // Set branch addresses and branch pointers

   fCurrent = -1;
   p_TTree->SetMakeClass(1);

   p_TTree->SetBranchAddress("Xpad", &Xpad, &b_Xpad);
   p_TTree->SetBranchAddress("Ypad", &Ypad, &b_Ypad);
   p_TTree->SetBranchAddress("Gainval", &Gainval, &b_Gainval);

}
