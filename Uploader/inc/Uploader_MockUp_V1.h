#/**
 * @file Uploader_MockUp_V1.h
 * @brief Mock-up uploader (version 1) declaration for testing.
 *
 * Reads simplified datasets and fills Event structures for development and
 * testing of reconstruction code.
 */
#ifndef Uploader_MOCKUP_V1_H
#define Uploader_MOCKUP_V1_H

#include "Misc.h"
#include "Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_MockUp_V1 : public Uploader {
public:
  /** Constructor */
  Uploader_MockUp_V1(const std::string &SampleFile,
                     Model_ReadOutGeometry *pModel_ReadOutGeometry,
                     Model_Electronics *pModel_Electronics,
                     Model_Charge1D *pModel_Charge1D);
  virtual ~Uploader_MockUp_V1();

  virtual Event *GiveMe_Event(const int &iEvent, const int &ModuleNber_Input,
                              const int &Data_to_Use, const int &CloseWF);

  virtual Event *GiveMe_Event(const int &iEvent, const int &ModuleNber_Input,
                              const int &Data_to_Use);

  //------------------------------Data Members-----------------------//
private:
};

#endif