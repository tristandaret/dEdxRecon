#ifndef Module_H
#define Module_H

#include "Misc.h"

#include "Pad.h"
#include "Cluster.h"
#include "Track.h"

#include "ROBoard.h"

#include "Model_ReadOutGeometry.h"
#include "Model_Electronics.h"
#include "Model_ChargeI.h"

/////////////////////////////////////////////////////////////
class Module : public ROBoard {
public:
   /** Constructor */
   Module(const int &EventNber, const int &EntryNber, const int &ModuleNber,
          Model_ReadOutGeometry *pModel_ReadOutGeometry,
          Model_Electronics *pModel_Electronics, Model_ChargeI *pModel_ChargeI);
   virtual ~Module();

   //------------------------------IDs-----------------------//
   int Get_EventNber() const;  // Get Event nber
   int Get_EntryNber() const;  // Get Entry nber
   int Get_ModuleNber() const; // Get Module nber

   //------------------------------Prints-----------------------//
   void SmallDump() const; // Small dump
   void WriteOut() const;  // Big dump

   //------------------------------Module Validity-----------------------//
   int IsValid() const; // Get validation status of the Module
   void Validate();     // Validate the Module
   void Invalidate();   // Invalidate the Module

   //------------------------------Pad addition-----------------------//
   virtual void Add_Pad(Pad *pPad); // Add a pad

   //------------------------------Clusters Addition-----------------------//
   void Clear_Clusters();               // Clear existing clusters set
   void Add_Cluster(Cluster *pCluster); // Add a cluster

   //------------------------------Clusters Access-----------------------//
   std::vector<Cluster *> GiveMe_Clusters_ForThisModule();
   int Get_NberOfCluster() const;            // Size of Cluster set
   Cluster *Get_Cluster(const int &Index1D); // Get Cluster

   void
   ReplaceClusters(std::vector<Cluster *> &V_Cluster); // Replace existing clusters set

   Cluster *Get_Cluster_Copy(Cluster *pCluster); // Get copy of the input cluster

   //------------------------------Track-----------------------//
   const Track *GiveMe_ModuleTrack();
   void Set_ModuleTrack(Track *pTrack); // Ownership is passed to the Module
   Track *GiveMe_AnUnfittedTrack();     // Ownership is passed to the user

   //------------------------------Data Members-----------------------//
private:
   short int m_IsValid = 1;

   int m_EventNber;
   int m_EntryNber;
   short int m_ModuleNber;

   short int m_NberOfClusters;
   std::vector<Cluster *> V_Cluster;

   Track *p_TrackOfTheModule;
};

#endif
