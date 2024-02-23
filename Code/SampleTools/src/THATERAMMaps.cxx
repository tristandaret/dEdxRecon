#include "SampleTools/THATERAMMaps.h"
#include "EvtModelTools/EvtModelTools_Histos.h"




//******************************************************************************
ERAM_map::ERAM_map(){}
//******************************************************************************


ERAM_map::ERAM_map(const std::string& ERAM_id, const std::string& type)
{
  // m_Dir = "..GainRCMaps/" + type + "files/" ;
  m_Dir = "GainRCMaps/" + type + "files/" ;
  m_ERAM_id = ERAM_id ;
  m_type    = type;

  Initialize();
}


ERAM_map::ERAM_map(const std::string& Dir, const std::string& ERAM_id, const std::string& type)
{
  m_Dir     = Dir ;
  m_ERAM_id = ERAM_id ;
  m_type    = type;

  Initialize();
}


ERAM_map::~ERAM_map()
{
  for(int i=0;i<36;i++) for(int j=0;j<32;j++) value[i][j] = 0;
}


void ERAM_map::Initialize()
{
  
  std::vector<std::string> maps = { "01", "02", "03",                   "07",       "09", "10",
                                    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                    "21",       "23", "24",       "26",       "28", "29", "30"};
  if (std::find(maps.begin(), maps.end(), m_ERAM_id) != maps.end()) m_FileName = m_Dir + "ERAM" + m_ERAM_id + "_" + m_type + "data.root" ;
  else {
    std::cout << "ERAM_map::ERAM_map ERROR -> No map for ERAM " << m_ERAM_id << std::endl ;
    abort();
  }

  Load_ERAM_map aLoad_ERAM_map(m_FileName, m_type);
  
  for(int i=0;i<36;i++) for(int j=0;j<32;j++) SetData(i,j,0);
  
  int iEntry_Max = aLoad_ERAM_map.Get_NberOfEntries() ;
  for (int iEntry=0; iEntry<iEntry_Max;iEntry++) {
    aLoad_ERAM_map.GiveMe_Entry(iEntry);
    int    iX   = aLoad_ERAM_map.Get_iX()  ;
    int    iY   = aLoad_ERAM_map.Get_iY()  ;
    double Data = aLoad_ERAM_map.Get_Data();
    SetData(iX,iY,Data);
  }

  DrawOut_ERAMmaps(this, m_ERAM_id, m_type);

  FillHoles();
}


std::string ERAM_map::Get_iD()
{
  return m_ERAM_id;
}


double ERAM_map::GetData(const int& iX, const int& iY)
{
  return value[iX][iY] ;
}


void ERAM_map::SetData(const int& iX, const int& iY, const double& Data)
{
  value[iX][iY] = Data ;
}


void ERAM_map::FillHoles()
{
  for(int iX = 0 ; iX < 36 ; iX++){
    for(int iY = 0 ; iY < 32 ; iY++){
      double val                    = GetData(iX,iY) ; // loading here

      // Fill holes of maps if there is any
      std::vector<float> v_sides;
      if(val == 0){
        std::cout << "ERAM#" << m_ERAM_id << ": " << std::setw(4) << m_type << " hole in (iX,iY) = (" << iX << "," << iY << ") | "; 
        if(iX>0)  v_sides.push_back(GetData(iX-1,iY  )) ;
        if(iX<35) v_sides.push_back(GetData(iX+1,iY  )) ;
        if(iY>0)  v_sides.push_back(GetData(iX,  iY-1)) ;
        if(iY<31) v_sides.push_back(GetData(iX,  iY+1)) ;
        float n_sides = 0;
        for(int i = 0; i<4; i++) if (v_sides[i]!=0){ // additionnal step to discard empty neighbours
          val += v_sides[i];
          n_sides++;
        }
        val /= n_sides;
        SetData(iX, iY, val);
        std::cout << "value reset at " << val << std::endl ;
      }
      v_sides.clear();
    } // iY
  } // iX
}





//******************************************************************************
Load_ERAM_map::Load_ERAM_map(const std::string& mapFile, const std::string& type)
//******************************************************************************
{        
  m_mapFile = mapFile ;
  m_type    = type;

  p_TFile = TFile::Open(m_mapFile.c_str(),"READ");
  if (!p_TFile) {
    std::cout << "Load_ERAM_map::Load_ERAM_map ERROR: Couldn't open root file : " << m_mapFile << std::endl;
    abort() ;
  }
  
  p_TTree   = (TTree*) p_TFile->Get((type + "tree").c_str());
  if (!p_TTree){
    std::cout << "Load_ERAM_map::Load_ERAM_map ERROR: p_TTree==0" << std::endl;
    abort() ;
  }

  m_NberOfEntries = (int) p_TTree->GetEntries();
  Init();

}


Load_ERAM_map::~Load_ERAM_map(){}

void Load_ERAM_map::GiveMe_Entry(const int& iEntry){
  if (iEntry >= Get_NberOfEntries() ) return ;
 
  if (!p_TTree) return ;
  fCurrent = iEntry;
  
  p_TTree->GetEntry(iEntry);
 
}


int    Load_ERAM_map::Get_iX()   { return Xpad  ; }
int    Load_ERAM_map::Get_iY()   { return Ypad  ; }
double Load_ERAM_map::Get_Data() { return val ; }

int Load_ERAM_map::Get_NberOfEntries(){ return m_NberOfEntries; }

void Load_ERAM_map::Init()
{
  // Set branch addresses and branch pointers

  fCurrent = -1;
  p_TTree->SetMakeClass(1);

  p_TTree->SetBranchAddress("Xpad", &Xpad, &b_Xpad);
  p_TTree->SetBranchAddress("Ypad", &Ypad, &b_Ypad);
  p_TTree->SetBranchAddress((m_type + "val").c_str(), &val, &b_val);

}