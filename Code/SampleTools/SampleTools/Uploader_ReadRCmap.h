#ifndef Uploader_ReadRCmap_H
#define Uploader_ReadRCmap_H

#include "Misc/Misc.h"

#include "TFile.h"
#include "TTree.h"

/////////////////////////////////////////////////////////////
class Uploader_ReadRCmap {
public:
   /** Constructor */
   Uploader_ReadRCmap(
            const std::string& RCmapFile 
   );
   virtual ~Uploader_ReadRCmap();

   void GiveMe_Entry(
                               const int& iEntry
                              ) ;

   int Get_NberOfEntries() ;
   
   int    Get_iX() ;
   int    Get_iY() ;
   double Get_Data() ;

//------------------------------Data Members-----------------------//
private:
   std::string m_RCmapFile ;
   int m_NberOfEntries ;
   
   TFile* p_TFile ;
   
   TTree* p_TTree ;

   Int_t           fCurrent;

public:

   // Declaration of leaf types
   Int_t           Xpad;
   Int_t           Ypad;
   Double_t        RCval;

   // List of branches
   TBranch        *b_Xpad;   //!
   TBranch        *b_Ypad;   //!
   TBranch        *b_RCval;   //!

   void Init();

};



#endif

