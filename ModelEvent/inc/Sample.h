#ifndef Sample_H
#define Sample_H

#include "Misc.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_ChargeI.h"

#include "Pad.h"
#include "Event.h"
#include "Cluster.h"

#include "FitOutput.h"

#include "Track.h"
#include "SetOfTracks.h"

#include "TVirtualFitter.h"
#include "TMatrixD.h"
#include "TF1.h"

/////////////////////////////////////////////////////////////
class Sample {
public:
   /** Constructor */
   Sample();
   Sample(Model_ReadOutGeometry *pModel_ReadOutGeometry, Model_Electronics *pModel_Electronics,
          Model_ChargeI *pModel_ChargeI);
   virtual ~Sample();

   //------------------------------Prints-----------------------//
   void SmallDump() const; // Small dump
   void WriteOut() const;  // Big dump

   //------------------------------Models-----------------------//
   // Set Models
   void Set_Model_ReadOutGeometry(Model_ReadOutGeometry *pModel_ReadOutGeometry);
   void Set_Model_Electronics(Model_Electronics *pModel_Electronics);
   void Set_Model_Charge(Model_ChargeI *pModel_ChargeI);

   // Get Models
   Model_ReadOutGeometry *Get_Model_ReadOutGeometry();
   Model_Electronics *Get_Model_Electronics();
   Model_ChargeI *Get_Model_ChargeI();

   //------------------------------Event Addition-----------------------//
   void Add_Event(Event *pEvent); // Add Event

   //------------------------------Event Access-----------------------//
   int Get_NberOfEvents() const;  // Size of Event set
   Event *Get_Event(int Index1D); // Get Event

   //------------------------------Track Access-----------------------//
   SetOfTracks *Get_SetOfTracks_ForThisModule(const int &ModuleNber); // Ownership is passed to user

   //------------------------------PRF from input file-----------------------//
   void SetFilePRF(const double &Norm, const double &a2, const double &a4, const double &b2, const double &b4);

   void GetFilePRF(int &FilePRF_Exist, double &Norm, double &a2, double &a4, double &b2, double &b4);

   //------------------------------Data Members-----------------------//
private:
   // Copy constructor
   Sample(const Sample &obj) {}

   // Copy assignment operator
   Sample &operator=(const Sample &tmp_obj) { return *this; }

private:
   Model_ReadOutGeometry *p_Model_ReadOutGeometry;
   Model_Electronics *p_Model_Electronics;
   Model_ChargeI *p_Model_ChargeI;

   int m_NberOfEvents;
   std::vector<Event *> V_Event;

   short int m_FilePRF_Exist;
   double m_FilePRF_Norm;
   double m_FilePRF_a2;
   double m_FilePRF_a4;
   double m_FilePRF_b2;
   double m_FilePRF_b4;

   //---------------------RC and DD Fit Stuff-----------------------------//
private:
   double m_RC;
   double m_eRC;
   double m_DD;
   double m_eDD;
   bool m_StatusFit; // True: fit failed; False: Fit OK

   double m_Chi2Min;

   double m_RC_BeforeMinimisation;
   double m_DD_BeforeMinimisation;

public:
   void Set_RC(const double &RC);

   void Set_DD(const double &RC);

   int StatusFit(); // 0: OK; 1: Failed

   double Get_RC() const;
   double Get_eRC() const;

   double Get_DD() const;
   double Get_eDD() const;

   double Get_Chi2Min() const;

   double Get_RC_BeforeMinimisation() const;
   double Get_DD_BeforeMinimisation() const;
};

#endif
