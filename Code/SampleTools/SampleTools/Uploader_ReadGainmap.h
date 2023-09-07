#ifndef Uploader_ReadGainmap_H
#define Uploader_ReadGainmap_H

#include "Misc/Misc.h"

#include "TFile.h"
#include "TTree.h"

/////////////////////////////////////////////////////////////
class Uploader_ReadGainmap {
public:
   /** Constructor */
   Uploader_ReadGainmap(
            const std::string& GainmapFile 
   );
   virtual ~Uploader_ReadGainmap();

   void GiveMe_Entry(
                               const int& iEntry
                              ) ;

   int Get_NberOfEntries() ;
   
   int    Get_iX() ;
   int    Get_iY() ;
   double Get_Data() ;

//------------------------------Data Members-----------------------//
private:
   std::string m_GainmapFile ;
   int m_NberOfEntries ;
   
   TFile* p_TFile ;
   
   TTree* p_TTree ;

   Int_t           fCurrent;

public:

   // Declaration of leaf types
   Int_t           Xpad;
   Int_t           Ypad;
   Double_t        Gainval;

   // List of branches
   TBranch        *b_Xpad;      //!
   TBranch        *b_Ypad;      //!
   TBranch        *b_Gainval;   //!

   void Init();

};



#endif

