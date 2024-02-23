#ifndef erammap_H
#define erammap_H

#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class ERAM_map {
public:
   /** Constructor */
   ERAM_map();
   ERAM_map(const std::string& ERAM_id, const std::string& type);
   ERAM_map(const std::string& Dir, const std::string& ERAM_id, const std::string& type);
   virtual ~ERAM_map();
   
   double GetData(const int& iX, const int& iY);

   std::string Get_iD(); // ERAM identification number

   std::string id;

private:
  std::string m_ERAM_id ;
  std::string m_Dir      ;
  std::string m_type     ;
  std::string m_FileName ;

// float value[36][32] = {0};
   float value[36][32];

  void Initialize();
  void SetData(const int& iX, const int& iY, const double& Data) ;
  void FillHoles();

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

#endif