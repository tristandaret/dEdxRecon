#include "Module.h"

Module::Module(const int &EventNber, const int &EntryNber, const int &ModuleNber,
               Model_ReadOutGeometry *pModel_ReadOutGeometry, Model_Electronics *pModel_Electronics,
               Model_ChargeI *pModel_ChargeI)
   : ROBoard(pModel_ReadOutGeometry, pModel_Electronics, pModel_ChargeI)
{
   m_IsValid = 1;

   m_EventNber = EventNber;
   m_EntryNber = EntryNber;
   m_ModuleNber = ModuleNber;

   m_NberOfClusters = 0;

   p_TrackOfTheModule = 0;
}

Module::~Module()
{
   int VerboseDelete = 0;
   if (VerboseDelete == 1)
      std::cout << " Module Destructor "
                << " m_EventNber " << m_EventNber << " m_EntryNber " << m_EntryNber << " m_ModuleNber " << m_ModuleNber
                << std::endl;
   Clear_Clusters();
   delete p_TrackOfTheModule;
   p_TrackOfTheModule = 0;
}

int Module::IsValid() const
{
   return m_IsValid;
}
void Module::Validate()
{
   m_IsValid = 1;
}
void Module::Invalidate()
{
   m_IsValid = 0;
   delete p_TrackOfTheModule;
   p_TrackOfTheModule = 0;
   // Clear_Clusters();
   for (int i = 0; i < Get_NberOfCluster(); i++) {
      m_IsValid = 0;
      V_Cluster[i]->Invalidate();
   }
   // int NPads = Get_NberOfPads();
   // for (int iP = 0; iP< NPads; iP++){
   // delete V_Pad[iP];
   // V_Pad[iP] = 0;
   // }
   // V_Pad.clear();
   // V_ExisFlag2D.clear();
   // V_Pad.clear(); std::vector< Pad* >().swap( V_Pad	);
   // V_ExisFlag2D.clear(); std::vector<int>().swap( V_ExisFlag2D );
}

void Module::Add_Pad(Pad *pPad)
{
   ROBoard::Add_Pad(pPad);
}

void Module::Clear_Clusters()
{
   for (int iC = 0; iC < Get_NberOfCluster(); iC++) {
      delete V_Cluster[iC];
      V_Cluster[iC] = 0;
   }
   V_Cluster.clear();
   V_Cluster.clear();
   std::vector<Cluster *>().swap(V_Cluster);
   m_NberOfClusters = 0;
}

void Module::Add_Cluster(Cluster *pCluster)
{
   V_Cluster.push_back(pCluster);
   m_NberOfClusters += 1;
}

void Module::ReplaceClusters(std::vector<Cluster *> &VCluster)
{
   Clear_Clusters();
   int NClusters = VCluster.size();
   if (NClusters == 0) {
      Invalidate();
      return;
   }
   for (int iC = 0; iC < NClusters; iC++) {
      Add_Cluster(VCluster[iC]);
   }
}

int Module::Get_NberOfCluster() const
{
   return m_NberOfClusters;
}
Cluster *Module::Get_Cluster(const int &Index1D)
{
   return V_Cluster[Index1D];
}

Cluster *Module::Get_Cluster_Copy(Cluster *pCluster)
{
   Cluster *ToBeReturned =
      new Cluster(pCluster->Get_EventNber(), pCluster->Get_EntryNber(), pCluster->Get_ModuleNber());

   int NPads = pCluster->Get_NberOfPads();
   for (int iP = 0; iP < NPads; iP++) {
      Pad *pPad = pCluster->Get_Pad(iP);
      ToBeReturned->Add_Pad(pPad);
   }
   ToBeReturned->DoClosure();

   return ToBeReturned;
}

int Module::Get_EventNber() const
{
   return m_EventNber;
}
int Module::Get_EntryNber() const
{
   return m_EntryNber;
}
int Module::Get_ModuleNber() const
{
   return m_ModuleNber;
}

const Track *Module::GiveMe_ModuleTrack()
{
   return p_TrackOfTheModule;
}
void Module::Set_ModuleTrack(Track *pTrack)
{
   delete p_TrackOfTheModule;
   p_TrackOfTheModule = pTrack;
}
Track *Module::GiveMe_AnUnfittedTrack()
{
   Track *ToBeReturned = new Track(m_EventNber, m_EntryNber, m_ModuleNber);
   int NClusters = Get_NberOfCluster();
   for (int iC = 0; iC < NClusters; iC++) {
      Cluster *pCluster = Get_Cluster(iC);
      ToBeReturned->Add_Cluster(pCluster);
   }
   ToBeReturned->DoClosure();
   return ToBeReturned;
}

void Module::WriteOut() const
{
   std::cout << "StartModule" << std::endl;
   std::cout << " m_IsValid; " << std::setw(16) << m_IsValid << std::endl;
   std::cout << " m_EventNber; " << std::setw(16) << m_EventNber << std::endl;
   std::cout << " m_EntryNber; " << std::setw(16) << m_EntryNber << std::endl;
   std::cout << " m_ModuleNber; " << std::setw(16) << m_ModuleNber << std::endl;
   for (int iP = 0; iP < Get_NberOfPads(); iP++) {
      V_Pad[iP]->WriteOut();
   }
   for (int iC = 0; iC < Get_NberOfCluster(); iC++) {
      V_Cluster[iC]->WriteOut();
   }
   std::cout << "EndModule" << std::endl;
}

// Small dump
void Module::SmallDump() const
{
   std::cout << std::endl;
   std::cout << "->->Module::SmallDump" << std::endl;
   std::cout << "->->->Module Validity		" << IsValid() << std::endl;
   std::cout << "->->->Get_NberOfCluster() " << Get_NberOfCluster() << std::endl;
}

//----------------------------Clusters----------------------//
std::vector<Cluster *> Module::GiveMe_Clusters_ForThisModule()
{
   return V_Cluster;
}
