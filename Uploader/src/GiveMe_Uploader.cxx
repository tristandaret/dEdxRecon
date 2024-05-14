#include "GiveMe_Uploader.h"
#include "Util.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_Charge1D.h"

#include "Uploader_ERAM01.h"
#include "Uploader_Prototype.h"
#include "Uploader_MockUp.h"

Uploader* GiveMe_Uploader(
                          const int& intUploader,
                          const std::string& SampleFile 
){

//Warning: these will feed the Event model and should exist for the whole job
//but pointers on them are lost at the end of this scope (not nice)
  Model_ReadOutGeometry* pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*     pModel_Electronics     = new Model_Electronics     ;
  Model_Charge1D*        pModel_Charge1D        = new Model_Charge1D        ;
  
  Uploader* ToBeReturned = 0 ;

  if ( intUploader == 1 ) ToBeReturned = new Uploader_ERAM01(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );

  if ( intUploader == 2 ) ToBeReturned = new Uploader_Prototype(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
  if ( intUploader == 3 ) {
    pModel_ReadOutGeometry->SquareGeometry() ;
    Uploader_MockUp* pUploader_MockUp = new Uploader_MockUp(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
    pUploader_MockUp->SetBeforeMerging(0) ;                                                    
    pUploader_MockUp->Setwap_XY(0) ;                                                    
    ToBeReturned = pUploader_MockUp;
  }
  return ToBeReturned ;
}
