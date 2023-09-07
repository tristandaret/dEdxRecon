#ifndef Analysis_Files_H
#define Analysis_Files_H

#include "Misc/Misc.h"

/////////////////////////////////////////////////////////////
class Analysis_Files {
public:
   /** Constructor */
   Analysis_Files(const std::string& SampleSet);
   virtual ~Analysis_Files();

public:
  std::string m_SampleSet                             ;
  int m_NberOfFiles                                   ;
  std::string                 m_FilesDir              ;
  std::vector < std::string > m_FilesName             ;
  std::string                 m_VariableName          ;
  std::vector < double >      m_VariableValue         ;
  std::vector < std::string > m_VariableValueAsString ;


};

#endif

