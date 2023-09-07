#include "EvtModel/Event.h"
#include "TGraph.h"

Event::Event(
     const int& EventNber ,
     const int& EntryNber ,
     Model_ReadOutGeometry* pModel_ReadOutGeometry  ,
     Model_Electronics*     pModel_Electronics      ,
     Model_ChargeI*         pModel_ChargeI
){
//Initialize Models
  Ini_Models(
             pModel_ReadOutGeometry  ,
             pModel_Electronics      ,
             pModel_ChargeI 
            );

//   
  m_IsValid = 1 ;
  
  m_EventNber = EventNber ;
  m_EntryNber = EntryNber ;
  
  m_NberOfModules = 0 ;
  
}

Event::~Event()
{
 int VerboseDelete = 0 ;
 if (VerboseDelete==1)
 std::cout 
       << " Event Destructor "
       << " m_EventNber " << m_EventNber
       << " m_EntryNber " << m_EntryNber
       << std::endl ;
  Clear_Modules() ;
}

//------------------------------Event Validity-----------------------//
int  Event::IsValid()   const  { return m_IsValid; }
void Event::Validate()  { m_IsValid = 1 ;}
void Event::Invalidate()
{ 
  m_IsValid = 0 ; 
  int iMod_Max = V_Module.size() ;
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = V_Module[iMod] ;
    pModule->Invalidate() ;
  }
//Clear_Modules() ;
}

//------------------------------IDs-----------------------//
int Event::Get_EventNber () const { return m_EventNber ; }
int Event::Get_EntryNber () const { return m_EntryNber ; }


//----------------------------Module----------------------//
void Event::Clear_Modules()
{
  int iM_Max = V_Module.size() ;
  for (int iM = 0 ; iM< iM_Max; iM++){
     delete V_Module[iM];
     V_Module[iM] = 0 ;
  }
  V_Module.clear() ;
  m_NberOfModules = 0 ;
//
  V_Module.clear()     ; std::vector< Module* >().swap( V_Module    );
//
}

void Event::Add_Module(Module* pModule) 
{ 
  V_Module.push_back(pModule); 
  m_NberOfModules += 1 ;
}

Module* Event::Get_ThisModule (const int& ModuleNber)
{
  int iMod_Max = V_Module.size() ;
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = V_Module[iMod] ;
    int ModuleNber_Cur = V_Module[iMod]->Get_ModuleNber() ;
    if ( ModuleNber == ModuleNber_Cur ) return pModule;
  }
  return 0 ;
}

const Module* Event::Get_ThisModuleConst (const int& ModuleNber) const
{
  int iMod_Max = V_Module.size() ;
  for (int iMod = 0 ; iMod< iMod_Max; iMod++){
    Module* pModule = V_Module[iMod] ;
    int ModuleNber_Cur = V_Module[iMod]->Get_ModuleNber() ;
    if ( ModuleNber == ModuleNber_Cur ) return pModule;
  }
  return 0 ;
}

int     Event::Get_NberOfModule() const { return m_NberOfModules   ; }
Module* Event::Get_Module_InArray (const int& Index1D) { return V_Module[Index1D] ; }


//----------------------------Clusters----------------------//
std::vector < Cluster* > Event::GiveMe_Clusters_ForThisModule(const int& ModuleNber)
{
  std::vector < Cluster* > ToBeReturned ;
  
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    int NClusters = pModule->Get_NberOfCluster();
    for (int iC = 0 ; iC< NClusters; iC++){
      Cluster* pCluster = pModule->Get_Cluster(iC);
      ToBeReturned.push_back( pCluster ); 
    }
  }else{
    std::cout << "WARNING from Event::GiveMe_Clusters_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }

  return ToBeReturned ;
}

double Event::GiveMe_CutSlopeXY_ForThisModule(const int& ModuleNber)
{
  std::vector < Cluster* >ClusterSet = GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  double XY_Mean = 0. ;
  double XX_Mean = 0. ;
  double X_Mean  = 0. ;
  double Y_Mean  = 0. ;
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
    double Yi = pCluster->Get_YWeight() *1.E2 ;
    double Xi = pPad_Leading->Get_XPad()*1.E2 ;
    
    XY_Mean += Xi*Yi ;
    XX_Mean += Xi*Xi ;
    X_Mean  +=    Xi ;
    Y_Mean  +=    Yi ;
    
  }  
  XY_Mean = XY_Mean/double(NClusters) ;
  XX_Mean = XX_Mean/double(NClusters) ;
  X_Mean  = X_Mean /double(NClusters) ;
  Y_Mean  = Y_Mean /double(NClusters) ;

  double SlopeXY = 36. * (XY_Mean- X_Mean*Y_Mean)/(XX_Mean - X_Mean*X_Mean) ;
  
  return SlopeXY ;

}

double Event::GiveMe_CutSlopeXZ_ForThisModule(const int& ModuleNber)
{
  std::vector < Cluster* >ClusterSet = GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  double XY_Mean = 0. ;
  double XX_Mean = 0. ;
  double X_Mean  = 0. ;
  double Y_Mean  = 0. ;
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
    double Yi = pCluster->Get_TMaxLeading()  ;
    double Xi = pPad_Leading->Get_XPad()*1.E2 ;
    
    XY_Mean += Xi*Yi ;
    XX_Mean += Xi*Xi ;
    X_Mean  +=    Xi ;
    Y_Mean  +=    Yi ;
    
  }  
  XY_Mean = XY_Mean/double(NClusters) ;
  XX_Mean = XX_Mean/double(NClusters) ;
  X_Mean  = X_Mean /double(NClusters) ;
  Y_Mean  = Y_Mean /double(NClusters) ;

  double SlopeXZ = 36. * (XY_Mean- X_Mean*Y_Mean)/(XX_Mean - X_Mean*X_Mean) ;
  
  return SlopeXZ ;

}

double Event::GiveMe_CutInterCeptXZ_ForThisModule(const int& ModuleNber)
{
  std::vector < Cluster* >ClusterSet = GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  double XY_Mean = 0. ;
  double XX_Mean = 0. ;
  double X_Mean  = 0. ;
  double Y_Mean  = 0. ;
  int NClusters = ClusterSet.size() ;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
    double Yi = pCluster->Get_TMaxLeading()  ;
    double Xi = pPad_Leading->Get_XPad()*1.E2 ;
    
    XY_Mean += Xi*Yi ;
    XX_Mean += Xi*Xi ;
    X_Mean  +=    Xi ;
    Y_Mean  +=    Yi ;
    
  }  
  XY_Mean = XY_Mean/double(NClusters) ;
  XX_Mean = XX_Mean/double(NClusters) ;
  X_Mean  = X_Mean /double(NClusters) ;
  Y_Mean  = Y_Mean /double(NClusters) ;
  
  double SlopeXZ          = (XY_Mean- X_Mean*Y_Mean)/(XX_Mean - X_Mean*X_Mean) ;
  double InterCeptXZSlope = (XY_Mean*X_Mean-Y_Mean*XX_Mean)/(X_Mean*X_Mean - XX_Mean) ;
  
  return (SlopeXZ*18.+ InterCeptXZSlope);

}
//---------------------------------------------------------------------------------
std::vector <double> Event::GiveMe_CutSlopeXYZ_ForThisModule(const int& ModuleNber)
{
  TGraph gr;
  TGraph gr1;
  std::vector <double> params;
    
  std::vector < Cluster* >ClusterSet = GiveMe_Clusters_ForThisModule (ModuleNber) ;   
  int NClusters = ClusterSet.size() ;
  int EventNber = -1;
  int EntryNber = -1;
  for (int iC = 0 ; iC< NClusters; iC++){
    Cluster* pCluster = ClusterSet[iC];
    double Zi = (pCluster->Get_Pad(0))->Get_TMax() ;
    double Xi = (pCluster->Get_Pad(0))->Get_XPad()*1.E2 ;
    double Yi =  pCluster->Get_YWeight() *1.E2 ;

    const Pad* pPad_Leading = pCluster->Get_LeadingPad() ;
           Zi = pCluster->Get_TMaxLeading()  ;
           Xi = pPad_Leading->Get_XPad()*1.E2 ;
   
    EventNber = pCluster->Get_EventNber () ;
    EntryNber = pCluster->Get_EntryNber () ;

    gr1.SetPoint(gr1.GetN(),Xi,Yi);
    gr.SetPoint(gr.GetN(),Xi,Zi);
  }
  // Perfom a fit in XZ and XY plane to extract theta and phi
  
  TCanvas xzinfo("XZinfo","XZinfo",1024,640);
  xzinfo.Divide(1,2);
  xzinfo.cd(1);
    
  gr.Fit("pol1", "Q");
  TF1* fit = gr.GetFunction("pol1");
  
  if (!fit) params.push_back(-999.);
  
  gr.SetMarkerStyle(20);
  gr.SetMarkerSize(1.);
  gr.SetMarkerColor(4);
  gr.Draw("AP");
  gr.SetMinimum(0.);
  gr.SetMaximum(511.);

  xzinfo.cd(2);  
  gr1.Fit("pol1", "Q");
  TF1* fit1 = gr1.GetFunction("pol1");

  if (!fit1) params.push_back(-999.);
  gr1.SetMarkerStyle(20);
  gr1.SetMarkerSize(1.);
  gr1.SetMarkerColor(1);
  gr1.Draw("AP");
  gr1.SetMinimum(0.);
  gr1.SetMaximum(36.);
    
  if(EventNber<1000)
      xzinfo.SaveAs(TString::Format("/feynman/home/dphp/shassani/work/AOBt_3.06/OUT_JeanFrancois/plots/c_xzinfo_EventNber_%i_EntryNber_%i.png",EventNber, EntryNber));

  //
  params.push_back(fit->GetParameter(1));
  params.push_back(fit1->GetParameter(1));
  //
  return params;
  
}

//-------------------------------------------------------------------------


void Event::Replace_Clusters_ForThisModule(
                                       std::vector < Cluster* >& VCluster,
                                       const int& ModuleNber
){

  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    pModule->Clear_Clusters() ;
    int NClusters = VCluster.size();
    if (NClusters==0) {
      pModule->Invalidate() ; // If no clusters left, invalidate the Module
      int IOK = 0 ;
      int iMod_Max = V_Module.size() ;
      for (int iMod = 0 ; iMod< iMod_Max; iMod++){
        Module* pModule = V_Module[iMod] ;
        if (pModule->IsValid()==1) IOK = 1 ;
      }
      if (IOK==0) Invalidate() ; // If all modules are invalid, invalidate the event
      return ;
    }
    for (int iC = 0 ; iC< NClusters; iC++){
      pModule->Add_Cluster( VCluster[iC] ) ;
    }
  }else{
    std::cout << "WARNING from Event::Replace_Clusters_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
}

Cluster* Event::Get_Cluster_Copy (Cluster* pCluster) 
{ 
  Cluster* ToBeReturned = new Cluster(
                                      pCluster->Get_EventNber () ,
                                      pCluster->Get_EntryNber () ,
                                      pCluster->Get_ModuleNber() 
                                     );
  
  int NPads = pCluster->Get_NberOfPads();
  for (int iP = 0 ; iP< NPads; iP++){
    const Pad* pPad = pCluster->Get_Pad(iP);
    ToBeReturned->Add_Pad(pPad) ;
  }
  ToBeReturned->DoClosure();

  return ToBeReturned ; 
}


//----------------------------Module Validity----------------------//
int  Event::Validity_ForThisModule (const int& ModuleNber) const  
{ 
  const Module* pModule = Get_ThisModuleConst(ModuleNber);
  if ( pModule ){
    return ( pModule->IsValid() ) ;
  }else{
    std::cout << "WARNING from Event::Validity_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
  return 0; 
}
void Event::Validate_ThisModule    (const int& ModuleNber)
{
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    Validate() ;
    return pModule->Validate() ;
  }else{
    std::cout << "WARNING from Event::Validate_ThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
}
void Event::Invalidate_ThisModule  (const int& ModuleNber)
{
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    pModule->Invalidate() ;
    int IOK = 0 ;
    int iMod_Max = V_Module.size() ;
    for (int iMod = 0 ; iMod< iMod_Max; iMod++){
      Module* pModule = V_Module[iMod] ;
      if (pModule->IsValid()==1) IOK = 1 ;
    }
    if (IOK==0) Invalidate() ; // If all modules are invalid, invalidate the event
  }else{
    std::cout << "WARNING from Event::Invalidate_ThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
}


//----------------------------Module Track----------------------//
Track*       Event::GiveMe_AnUnfittedTrack_ForThisModule(const int& ModuleNber)
{
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    return pModule->GiveMe_AnUnfittedTrack() ;
  }else{
    std::cout << "WARNING from Event::GiveMe_AnUnfittedTrack_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
  return 0 ;
}
void Event::Set_Track_ForThisModule(Track* pTrack,const int& ModuleNber) 
{ 
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    return pModule->Set_ModuleTrack(pTrack) ;
  }else{
    std::cout << "WARNING from Event::Set_Track_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
} 
const Track* Event::GiveMe_Track_ForThisModule(const int& ModuleNber) 
{ 
  Module* pModule = Get_ThisModule(ModuleNber);
  if ( pModule ){
    return pModule->GiveMe_ModuleTrack() ;
  }else{
    std::cout << "WARNING from Event::GiveMe_Track_ForThisModule" 
              << " The Module ModuleNber=" << ModuleNber 
              << " does not exist"
              << std::endl ;
  }
  return 0 ; 
}


//----------------------------Print----------------------//
void Event::WriteOut() const 
{
  std::cout << "StartEvent" << std::endl ;
//   std::cout << " m_IsValid ; " << std::setw(16) << m_IsValid << std::endl ;
  std::cout << " m_EventNber ; " << std::setw(16) << m_EventNber << std::endl ;
  std::cout << " m_EntryNber ; " << std::setw(16) << m_EntryNber << std::endl ;
  for (int iM = 0 ; iM< Get_NberOfModule(); iM++){
    V_Module[iM]->WriteOut() ;
  }
  std::cout << "EndEvent" << std::endl ;
}

// Small dump
void Event::SmallDump() const 
{
  std::cout << std::endl ;
  std::cout << "->->Event::SmallDump" << std::endl ;

}


//----------------------------Models----------------------//
const Model_ReadOutGeometry*  Event::Get_Model_ReadOutGeometry(){ return p_Model_ReadOutGeometry ;}
const Model_Electronics    *  Event::Get_Model_Electronics    (){ return p_Model_Electronics     ;}
const Model_ChargeI        *  Event::Get_Model_ChargeI        (){ return p_Model_ChargeI         ;}

void Event::Ini_Models(
     Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
     Model_Electronics*      pModel_Electronics      ,
     Model_ChargeI*          pModel_ChargeI    
){
//
  p_Model_ReadOutGeometry  = pModel_ReadOutGeometry  ;
  p_Model_Electronics      = pModel_Electronics      ;
  p_Model_ChargeI          = pModel_ChargeI          ;
}

