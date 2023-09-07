#include "Misc/Misc.h"

#include "Studies/Studies.h"

#include "Samira/Samira.h"
#include "Tristan/Tristan.h"
#include "JeanFrancois/JeanFrancois.h"

#include "Analysis_SH/Analysis_SH.h"
#include "Analysis_TD/Analysis_TD.h"
#include "Analysis_JF/Analysis_JF.h"

#include "GenerateShape/GenerateShapeFunction.h"
#include "GenerateShape/GenerateShape0DFunction.h"
#include "GenerateShape/GenerateShape1DFunction.h"
#include "GenerateShape/Shape_Check.h"

#include "Simulation/Do_Simulation.h"

int main(int argc, char *argv[])
{
  std::cout << std::setiosflags(std::ios::fixed);

//----------------------------Samira stuff
//Samira();
//Analysis_SH() ;


//----------------------------Tristan stuff 
Tristan();
//Analysis_TD();


//----------------------------Jean-Francois stuff
//JeanFrancois() ;
//Analysis_JF() ;

     
//----------------------------Generate Simulation
//Do_Simulation();
     
     
//----------------------------Generate PRF etc.
//GenerateShapeFunction0D_BaseTest();
//GenerateShapeFunction1D_BaseTest();
//GenerateShapeFunction() ;

//Shape_Check() ;

//----------------------------Studies
//Studies() ;


}
