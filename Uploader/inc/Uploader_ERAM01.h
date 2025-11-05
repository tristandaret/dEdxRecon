#/***************************************************************************
 * File: Uploader_ERAM01.h
 * Project: dEdxRecon
 *
 * Brief: Declaration for an Uploader that reads ERAM01-format data files and
 *        converts them into `Event` objects for the reconstruction pipeline.
 *
 * Contents: Uploader_ERAM01 subclass declaration.
 *
 * Notes: Implementation is in Uploader_ERAM01.cxx.
 ***************************************************************************/

#ifndef Uploader_ERAM1_H
#define Uploader_ERAM1_H

#include "Misc.h"
#include "Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_ERAM01 : public Uploader {
public:
  /** Constructor */
  Uploader_ERAM01(const std::string &SampleFile,
                  Model_ReadOutGeometry *pModel_ReadOutGeometry,
                  Model_Electronics *pModel_Electronics,
                  Model_Charge1D *pModel_Charge1D);
  virtual ~Uploader_ERAM01();

  virtual Event *GiveMe_Event(const int &iEvent, const int &ModuleNber_Input,
                              const int &Data_to_Use, const int &CloseWF);

  virtual Event *GiveMe_Event(const int &iEvent, const int &ModuleNber_Input,
                              const int &Data_to_Use);

  //------------------------------Data Members-----------------------//
private:
};

#endif
