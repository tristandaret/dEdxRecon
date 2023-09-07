#ifndef ReadGainmap_H
#define ReadGainmap_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class ReadGainmap {
public:
   /** Constructor */
   ReadGainmap(std::string EramName);
   ReadGainmap(std::string Dir,std::string EramName);
   virtual ~ReadGainmap();
   
   double GetData(const int& iX, const int& iY, int& Status);

private:
  std::string m_EramName ;
  std::string m_Dir      ;
  std::string m_FileName ;
  
  std::vector < double >* p_V_Data ;
  std::vector < int    >* p_V_HasBeenSet ;
  
  int GetLinear(const int& iX, const int& iY);
  int HasBeenSet(const int& iX, const int& iY);
  void SetData(const int& iX, const int& iY, const double& Data) ;

  void ConstructorEnd();

};

#endif

