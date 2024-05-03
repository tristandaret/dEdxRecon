#ifndef Uploader_DESY21_H
#define Uploader_DESY21_H

#include "Misc.h"
#include "Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_DESY21 : public Uploader  {
public:
   /** Constructor */
   Uploader_DESY21(
            const std::string& SampleFile                 ,
            Model_ReadOutGeometry* pModel_ReadOutGeometry ,
            Model_Electronics*     pModel_Electronics     ,
            Model_Charge1D*        pModel_Charge1D        
   );
   virtual ~Uploader_DESY21();
            
   virtual Event* GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use,
                               const int& CloseWF
                              ) ;

   virtual Event* GiveMe_Event(
                               const int& iEvent, 
                               const int& ModuleNber_Input, 
                               const int& Data_to_Use
                              ) ;
   

//------------------------------Data Members-----------------------//
private:

};



#endif

