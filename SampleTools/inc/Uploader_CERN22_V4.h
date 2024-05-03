#ifndef Uploader_CERN22_V4_H
#define Uploader_CERN22_V4_H

#include "Misc/Misc.h"
#include "SampleTools/Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_CERN22_V4 : public Uploader  {
public:
   /** Constructor */
   Uploader_CERN22_V4(
            const std::string& SampleFile                 ,
            Model_ReadOutGeometry* pModel_ReadOutGeometry ,
            Model_Electronics*     pModel_Electronics     ,
            Model_Charge1D*        pModel_Charge1D        
   );
   virtual ~Uploader_CERN22_V4();
            
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

