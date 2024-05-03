#include "GiveMe_Uploader.h"
#include "Util.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_Charge1D.h"

#include "Uploader_DESY21.h"
#include "Uploader_CERN22.h"
#include "Uploader_CERN22_V4.h"
#include "Uploader_David.h"

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

  if ( intUploader ==   2 ) ToBeReturned = new Uploader_DESY21(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );

  if ( intUploader ==   3 ) ToBeReturned = new Uploader_CERN22(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );

  if ( intUploader ==   4 ) ToBeReturned = new Uploader_CERN22_V4(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );

  if ( intUploader ==   5 ) {
    pModel_ReadOutGeometry->SquareGeometry() ;
    Uploader_David* pUploader_David = new Uploader_David(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
    pUploader_David->SetBeforeMerging(0) ;                                                    
    pUploader_David->Setwap_XY(0) ;                                                    
    ToBeReturned = pUploader_David;
  }
  if ( intUploader ==   6 ) {
    pModel_ReadOutGeometry->SquareGeometry() ;
    Uploader_David* pUploader_David = new Uploader_David(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
    pUploader_David->SetBeforeMerging(1) ;                                                    
    pUploader_David->Setwap_XY(0) ;                                                    
    ToBeReturned = pUploader_David;
  }
  if ( intUploader ==   51 ) {
    pModel_ReadOutGeometry->SquareGeometry() ;
    Uploader_David* pUploader_David = new Uploader_David(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
    pUploader_David->SetBeforeMerging(0) ;                                                    
    pUploader_David->Setwap_XY(1) ;                                                    
    ToBeReturned = pUploader_David;
  }
  if ( intUploader ==   61 ) {
    pModel_ReadOutGeometry->SquareGeometry() ;
    Uploader_David* pUploader_David = new Uploader_David(
                                                         SampleFile             ,
                                                         pModel_ReadOutGeometry ,
                                                         pModel_Electronics     ,
                                                         pModel_Charge1D        
                                                        );
    pUploader_David->SetBeforeMerging(1) ;                                                    
    pUploader_David->Setwap_XY(1) ;                                                    
    ToBeReturned = pUploader_David;
  }
  return ToBeReturned ;
}
