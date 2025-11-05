#/***************************************************************************
 * File: GiveMe_Uploader.cxx
 * Project: dEdxRecon
 *
 * Brief: Factory implementation that constructs a concrete Uploader based on
 *        an integer selector. Also creates minimal model objects required by
 *        the Uploader instances.
 *
 * Contents: GiveMe_Uploader factory function.
 *
 * Notes: The created model objects are currently leaked intentionally to
 *        remain valid for the life of the returned Uploader.
 ***************************************************************************/

#include "GiveMe_Uploader.h"

#include "Model_Charge1D.h"
#include "Model_Electronics.h"
#include "Model_ReadOutGeometry.h"

#include "Uploader_ERAM01.h"
#include "Uploader_MockUp_V1.h"
#include "Uploader_MockUp_V2.h"
#include "Uploader_Prototype.h"

Uploader *GiveMe_Uploader(const int &intUploader,
                          const std::string &SampleFile) {

  // Warning: these will feed the Event model and should exist for the whole job
  // but pointers on them are lost at the end of this scope (not nice)
  Model_ReadOutGeometry *pModel_ReadOutGeometry = new Model_ReadOutGeometry;
  Model_Electronics *pModel_Electronics = new Model_Electronics;
  Model_Charge1D *pModel_Charge1D = new Model_Charge1D;

  Uploader *ToBeReturned = 0;

  if (intUploader == 1)
    ToBeReturned = new Uploader_ERAM01(SampleFile, pModel_ReadOutGeometry,
                                       pModel_Electronics, pModel_Charge1D);

  if (intUploader == 2)
    ToBeReturned = new Uploader_Prototype(SampleFile, pModel_ReadOutGeometry,
                                          pModel_Electronics, pModel_Charge1D);
  if (intUploader == 3) {
    pModel_ReadOutGeometry->SquareGeometry();
    ToBeReturned = new Uploader_MockUp_V1(SampleFile, pModel_ReadOutGeometry,
                                          pModel_Electronics, pModel_Charge1D);
  }
  if (intUploader == 4) {
    pModel_ReadOutGeometry->SquareGeometry();
    ToBeReturned = new Uploader_MockUp_V2(SampleFile, pModel_ReadOutGeometry,
                                          pModel_Electronics, pModel_Charge1D);
  }
  return ToBeReturned;
}
