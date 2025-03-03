#ifndef Uploader_H
#define Uploader_H

#include "Misc.h"
#include "Event.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_Charge1D.h"

#include "TFile.h"
#include "TTree.h"

/////////////////////////////////////////////////////////////
class Uploader {
public:
   /** Constructor */
   Uploader(const std::string &SampleFile, Model_ReadOutGeometry *pModel_ReadOutGeometry,
            Model_Electronics *pModel_Electronics, Model_Charge1D *pModel_Charge1D);
   virtual ~Uploader();

   std::string Get_SampleFile();

   int Get_NberOfEvent();

   virtual Event *
   GiveMe_Event(const int &iEvent, const int &ModuleNber_Input, const int &Data_to_Use, const int &CloseWF = 1) = 0;
   int Get_PRF_exist();
   double Get_Norm();
   double Get_a2();
   double Get_a4();
   double Get_b2();
   double Get_b4();

   Model_ReadOutGeometry *Get_Model_ReadOutGeometry();
   Model_Electronics *Get_Model_Electronics();
   Model_Charge1D *Get_Model_Charge1D();

   //------------------------------Data Members-----------------------//
protected:
   std::string m_SampleFile;
   int m_NberOfEvent;

   TFile *p_TFile;

   TTree *p_TTree;

   Model_ReadOutGeometry *p_Model_ReadOutGeometry;
   Model_Electronics *p_Model_Electronics;
   Model_Charge1D *p_Model_Charge1D;

   short int m_PRF_exist;
   double m_Norm;
   double m_a2;
   double m_a4;
   double m_b2;
   double m_b4;
};

#endif
