#ifndef Uploader_Simulation_H
#define Uploader_Simulation_H

#include "Misc/Misc.h"
#include "SampleTools/Uploader.h"

/////////////////////////////////////////////////////////////
class Uploader_Simulation : public Uploader  {
public:
   /** Constructor */
   Uploader_Simulation(
            const std::string& SampleFile                 ,
            Model_ReadOutGeometry* pModel_ReadOutGeometry ,
            Model_Electronics*     pModel_Electronics     ,
            Model_Charge1D*        pModel_Charge1D        
   );
   virtual ~Uploader_Simulation();
            
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

