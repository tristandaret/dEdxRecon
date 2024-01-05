#include <iostream>
#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class ERAM_map {
public:
   /** Constructor */
   ERAM_map();
   ERAM_map(const std::string& ERAM_id, std::string type);
   ERAM_map(std::string Dir, const std::string& ERAM_id, std::string type);
   virtual ~ERAM_map();
   
   double GetData(const int& iX, const int& iY);
   void SetData(const int& iX, const int& iY, const double& Data) ;

   std::string Get_iD(); // ERAM identification number
   std::string Get_iD_public();

public:
   std::string id;

private:
  std::string m_ERAM_id ;
  std::string m_Dir      ;
  std::string m_type     ;
  std::string m_FileName ;

  std::vector<double>* p_V_Data;
  std::vector<int>*    p_V_HasBeenSet;

  int GetLinear(const int& iX, const int& iY);
  int HasBeenSet(const int& iX, const int& iY);

};




class Load_ERAM_map {
public:
   /** Constructor */
   Load_ERAM_map(const std::string& mapFile, const std::string& type);
   virtual ~Load_ERAM_map();

   void GiveMe_Entry(const int& iEntry) ;

   int Get_NberOfEntries() ;
   
   int    Get_iX() ;
   int    Get_iY() ;
   double Get_Data() ;

//------------------------------Data Members-----------------------//
private:
   std::string m_mapFile ;
   std::string m_type;
   int m_NberOfEntries ;
   
   TFile* p_TFile ;
   
   TTree* p_TTree ;

   Int_t  fCurrent;

public:

   // Declaration of leaf types
   Int_t           Xpad;
   Int_t           Ypad;
   Double_t        val;

   // List of branches
   TBranch        *b_Xpad;
   TBranch        *b_Ypad;
   TBranch        *b_val;

   void Init();

};




ERAM_map* Get_Map(const std::string& eram_id, const std::string& type);