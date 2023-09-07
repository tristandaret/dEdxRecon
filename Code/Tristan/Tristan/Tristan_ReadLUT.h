#ifndef Obj_H
#define Obj_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class Interpol2 {
public:
   /** Constructor */
   Interpol2(const std::string& FileIn);
   virtual ~Interpol2();

   float Interpolated(float& dconv, float& phiconv) ;
   
private:
//
   void Load(const std::string& FileIn) ;

// 
   std::vector<std::vector<float>> data ;
   
};


/////////////////////////////////////////////////////////////
class Interpol3 {
public:
   /** Constructor */
   Interpol3(const std::string& FileIn, int& nZ) ;
   virtual ~Interpol3();

   float Interpolated(float& dconv, float& phiconv, float& zconv) ;

private:
//
   void Load(const std::string& FileIn, int& nZ) ;

//   
   std::vector<std::string> v_FileZ ;
   std::vector< Interpol2* > V_pInterpol2 ;
   
};
Interpol3 GiveMe_LUT3(const std::string& folder_name, int& nZ) ;



/////////////////////////////////////////////////////////////
class Interpol4 {
public:
   /** Constructor */
   Interpol4();
   Interpol4(const std::string& FileIn, int& nZ, int& nRC);
   virtual ~Interpol4();

   float Interpolated(float& dconv, float& phiconv, float& zconv, float& RCconv) ;

private:
//
   void Load(const std::string& FileIn, int& nZ, int& nRC) ;

//   
   std::vector<std::string> v_FileRC ;
   std::vector< Interpol3* > V_pInterpol3 ;
   
};
Interpol4 GiveMe_LUT(const std::string& folder_name, int& nZ, int& nRC) ;

#endif

