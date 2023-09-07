#ifndef ReadRCmap_H
#define ReadRCmap_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class ReadRCmap {
public:
  /** Constructor */
  ReadRCmap();
  ReadRCmap(std::string EramName);
  ReadRCmap(std::string Dir,std::string EramName);
  virtual ~ReadRCmap();
   
  double GetData(const int& iX, const int& iY, int& Status);
  void SetData(const int& iX, const int& iY, const double& Data) ;

private:
  std::string m_EramName ;
  std::string m_Dir      ;
  std::string m_FileName ;
  
  std::vector < double >* p_V_Data ;
  std::vector < int    >* p_V_HasBeenSet ;
  
  int GetLinear(const int& iX, const int& iY);
  int HasBeenSet(const int& iX, const int& iY);

  void ConstructorEnd();

};

#endif

