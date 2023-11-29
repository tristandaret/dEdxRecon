#ifndef Obj_H
#define Obj_H

#include "Misc/Misc.h"
#include "TFile.h"
#include "TH2.h"

/////////////////////////////////////////////////////////////
class LUT2 {
public:
   /** Constructor */
   LUT2(TFile* pTFile_LUT, const std::string& FileIn);
   virtual ~LUT2();

   float Interpolate(TFile* pTFile_LUT, float& dconv, float& phiconv, int& iz, int& iRC) ;
   
private:
//
   void Load(TFile* pTFile_LUT, const std::string& FileIn) ;

// 
   TH2F* h2 ;
   
};


/////////////////////////////////////////////////////////////
class LUT3 {
public:
   /** Constructor */
   LUT3(TFile* pTFile_LUT, const std::string& FileIn, int& nZ) ;
   virtual ~LUT3();

   float Interpolate(TFile* pTFile_LUT, float& dconv, float& phiconv, float& zconv, int& iRC) ;

private:
//
   void Load(TFile* pTFile_LUT, const std::string& FileIn, int& nZ) ;

//   
   std::vector<std::string> v_th2_name ;
   std::vector< LUT2* > V_pLUT2 ;
   
};
LUT3 GiveMe_LUT3(TFile* pTFile_LUT, const std::string& folder_name, int& nZ) ;



/////////////////////////////////////////////////////////////
class LUT4 {
public:
   /** Constructor */
   LUT4();
   LUT4(const std::string& FileIn, int& nZ, int& nRC);
   virtual ~LUT4();

   float Interpolate(float& dconv, float& phiconv, float& zconv, float& RCconv) ;

private:
//
   void Load(const std::string& FileIn, int& nZ, int& nRC) ;

//   
   std::vector<std::string>   v_th2_nameRC ;
   std::vector< LUT3* >  V_pLUT3 ;
   TFile*                     pTFile_LUT;
   
};
LUT4 GiveMe_LUT(const std::string& folder_name, int& nZ, int& nRC) ;

#endif

