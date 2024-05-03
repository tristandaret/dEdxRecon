#include "SampleUploader_Data.h"
#include "Util.h"

#include "Uploader.h"
#include "GiveMe_Uploader.h"

void SampleUploader_Data( 
                         const int&         intUploader      , 
                         Sample&            aSample          , 
                         const int&         ModuleNber_Input , 
                         const std::string& SampleFile       , 
                         const int&         Data_to_Use
){
  std::cout << std::endl ;
  std::cout <<"  SampleUploader_Data: "  << std::endl ;
  std::cout <<"    intUploader       = " << intUploader      << std::endl ;
  std::cout <<"    ModuleNber_Input  = " << ModuleNber_Input << std::endl ;
  std::cout <<"    SampleFile        = " << SampleFile       << std::endl ;
  std::cout <<"    Data_to_Use       = " << Data_to_Use      << std::endl ;
  
//
  Uploader* pUploader = GiveMe_Uploader(intUploader,SampleFile) ;

//
  aSample.Set_Model_ReadOutGeometry(pUploader->Get_Model_ReadOutGeometry() ) ;
  aSample.Set_Model_Electronics    (pUploader->Get_Model_Electronics    () ) ;
  aSample.Set_Model_Charge         (pUploader->Get_Model_Charge1D       () ) ;

  if (pUploader->Get_PRF_exist()==1){
    aSample.SetFilePRF(
                       pUploader->Get_Norm (), 
                       pUploader->Get_a2   (), 
                       pUploader->Get_a4   (), 
                       pUploader->Get_b2   (), 
                       pUploader->Get_b4   ()
                      );
  }
  
//  
  int NberOfEvent = pUploader->Get_NberOfEvent() ;
  
  for (Int_t iEvent = 0; iEvent < NberOfEvent; iEvent++) {
    
    Event*  pEvent = pUploader->GiveMe_Event( iEvent, ModuleNber_Input, Data_to_Use );

    if (!pEvent) continue ;
       
    int NberOFModule = pEvent->Get_NberOfModule() ;
    if (NberOFModule!=0) aSample.Add_Event(pEvent);

  }
  delete pUploader ;
}

void SampleUploader_Data( 
                         const int&         MaxNberOfEvents  , 
                         const int&         intUploader      , 
                         Sample&            aSample          , 
                         const int&         ModuleNber_Input , 
                         const std::string& SampleFile       , 
                         const int&         Data_to_Use
){
  std::cout << std::endl ;
  std::cout <<"  SampleUploader_Data: "  << std::endl ;
  std::cout <<"    intUploader       = " << intUploader      << std::endl ;
  std::cout <<"    ModuleNber_Input  = " << ModuleNber_Input << std::endl ;
  std::cout <<"    SampleFile        = " << SampleFile       << std::endl ;
  std::cout <<"    Data_to_Use       = " << Data_to_Use      << std::endl ;
  
//
  Uploader* pUploader = GiveMe_Uploader(intUploader,SampleFile) ;

//
  aSample.Set_Model_ReadOutGeometry(pUploader->Get_Model_ReadOutGeometry() ) ;
  aSample.Set_Model_Electronics    (pUploader->Get_Model_Electronics    () ) ;
  aSample.Set_Model_Charge         (pUploader->Get_Model_Charge1D       () ) ;

  if (pUploader->Get_PRF_exist()==1){
    aSample.SetFilePRF(
                       pUploader->Get_Norm (), 
                       pUploader->Get_a2   (), 
                       pUploader->Get_a4   (), 
                       pUploader->Get_b2   (), 
                       pUploader->Get_b4   ()
                      );
  }
  
//  
  int NberOfEvent = pUploader->Get_NberOfEvent() ;
  
  int iEvent_Max = NberOfEvent ;
  if (iEvent_Max>MaxNberOfEvents) iEvent_Max = MaxNberOfEvents ;
  for (Int_t iEvent = 0; iEvent < iEvent_Max; iEvent++) {
    
    Event*  pEvent = pUploader->GiveMe_Event( iEvent, ModuleNber_Input, Data_to_Use );

    if (!pEvent) continue ;
       
    int NberOFModule = pEvent->Get_NberOfModule() ;
    if (NberOFModule!=0) aSample.Add_Event(pEvent);

  }
  delete pUploader ;
}
