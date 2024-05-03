#ifndef Cluster_H
#define Cluster_H

#include "Misc/Misc.h"
#include "EvtModel/Pad.h"

#include "EvtModel/FitOutput.h"

#include "TVirtualFitter.h"
#include "TMatrixD.h"
#include "TF1.h"


/////////////////////////////////////////////////////////////
class Cluster {
public:
   /** Constructor */
   Cluster(
           const int& EventNber ,
           const int& EntryNber ,
           const int& ModuleNber 
          );
   virtual ~Cluster();
   
   
//------------------------------IDs-----------------------//
   int  Get_EventNber  () const ; // Get Event nber
   int  Get_EntryNber  () const ; // Get Entry nber
   int  Get_ModuleNber () const ; // Get Module nber
   
   
//------------------------------Prints-----------------------//
   void WriteOut() const ; // Big dump
 
   
//------------------------------Clusters Data-----------------------//
   double Get_Acluster() const ; // Sum of Pads amplitude 
   double Get_XWeight()  const ; // Averaged cluster position
   double Get_YWeight()  const ; // Averaged cluster position


//------------------------------Pads-----------------------//
   void Add_Pad(const Pad* pPad) ; // Add Pad
   void DoClosure()        ; // Closure: Order pads, compute Leading pad stuff
   
   int   Get_NberOfPads() const                   ; // Size of Pad set
   const Pad* Get_Pad (const int& Index1D)  const ; // Get Pad

//Leading pad   
   const Pad* Get_LeadingPad () const  ; // Get Leading pad
   double Get_YLeading   () const ; // Get Leading pad Y position (m)
   double Get_TMaxLeading() const ; // Get Leading pad Time at max (Time bin)
   double Get_AMaxLeading() const ; // Get Leading pad Max amplitude (ADC)

   const Pad* Get_NextLeadingPad ()     const  ; // Get Next Leading pad, the hotest of the nearest pads of the leading pad
   const Pad* Get_NextNextLeadingPad () const  ; // Get NextNext Leading pad, the coldest of the nearest pads of the leading pad
       

//------------------------------Data Members-----------------------//
private:
  short int m_NberOfPads ;
  std::vector < const Pad* > V_Pad ;
  const Pad* Pad_Leading ;
  const Pad* Pad_NextLeading ;
  const Pad* Pad_NextNextLeading ;

  double m_Acluster ;
  double m_XWeight   ;
  double m_YWeight   ;

  int m_EventNber  ;
  int m_EntryNber  ;
  short int m_ModuleNber ;

  
//---------------------------Cluster Fit Stuff-----------------------------//   
private:
  double m_XTrack  ; 
  double m_YTrack  ; 
  double m_eXTrack ; 
  double m_eYTrack ; 
  double m_AT      ; 
  double m_eAT     ;
  double m_TT      ; 
  double m_eTT     ;
  bool   m_StatusFit  ; // True: fit failed  ; False: Fit OK
   
  double m_Chi2Min ;
  
  double m_XTrack_BeforeMinimisation  ; 
  double m_YTrack_BeforeMinimisation  ; 
  double m_AT_BeforeMinimisation      ; 
  double m_TT_BeforeMinimisation      ; 
  
public:
   void Set_XTrack(const double& XTrack);
   void Set_YTrack(const double& YTrack);
   void Set_AT    (const double& AT    );
   void Set_TT    (const double& TT    );
   
   void Set_eXTrack(const double& eXTrack);
   void Set_eYTrack(const double& eYTrack);
   
   int  StatusFit();  // 0: OK ; 1: Failed

   double Get_XTrack  () const ;
   double Get_eXTrack () const ;
   double Get_YTrack  () const ;
   double Get_eYTrack () const ;
   
   double Get_AT      () const ;
   double Get_eAT     () const ;
   
   double Get_TT      () const ;
   double Get_eTT     () const ;
   
   double Get_Chi2Min () const ;
   
   double Get_XTrack_BeforeMinimisation() const ;
   double Get_YTrack_BeforeMinimisation() const ;
   double Get_AT_BeforeMinimisation    () const ;
   double Get_TT_BeforeMinimisation    () const ;
  
  
//---------------------PV0_PV1-----------------------------//
public:
   int    StatusFit_PV0_PV1();  // 0: OK ; 1: Failed
   void   SetEval_PV0_PV1(TF1* pTF1 );
   double Eval_PV0_PV1(const double& Xin );
   int    SetParameter_PV0_PV1          (TVirtualFitter* pTVirtualFitter) ;
   void   SetResults_PV0_PV1            (TVirtualFitter* pTVirtualFitter) ;
   void   SetResults_FailledFit_PV0_PV1 (const int& Verbose ) ;
   double Chi2_PV0_PV1                  (double par[])                    ;
   
   int        FitRes_PV0_PV1_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV0_PV1_Get_Pad      (const int& Index1D) const ;
   double     FitRes_PV0_PV1_Get_Residual (const int& Index1D) const ;
   double     FitRes_PV0_PV1_Get_Pull     (const int& Index1D) const ;

private:
   void   Beg_PV0_PV1() ;
   void   End_PV0_PV1() ;
   void   SetParameters_Internal_PV0_PV1(double par[])                    ;

   TF1* p_TF1_PV0_PV1 ;
   bool m_StatusFit_PV0_PV1 ; // True: fit failed  ; False: Fit OK
   short int m_NberOf_V_FitRes_PV0_PV1_Pad ;
   std::vector < const Pad* > V_FitRes_PV0_PV1_Pad      ;
   std::vector < double >     V_FitRes_PV0_PV1_Residual ;
   std::vector < double >     V_FitRes_PV0_PV1_Pull     ;

   FitOutput* p_FitOutput_PV0_PV1 ;
   
   
//---------------------PV0_Diag-----------------------------//
public:
   int    StatusFit_PV0_Diag();  // 0: OK ; 1: Failed
   void   SetEval_PV0_Diag(TF1* pTF1 );
   double Eval_PV0_Diag(const double& Xin );
   int    SetParameter_PV0_Diag          (TVirtualFitter* pTVirtualFitter) ;
   void   SetResults_PV0_Diag            (TVirtualFitter* pTVirtualFitter) ;
   void   SetResults_FailledFit_PV0_Diag (const int& Verbose ) ;
   double Chi2_PV0_Diag                  (double par[])                    ;                 ;
   double m_AngleRot ;
   
   int        FitRes_PV0_Diag_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV0_Diag_Get_Pad      (const int& Index1D) const ;
   double     FitRes_PV0_Diag_Get_Residual (const int& Index1D) const ;
   double     FitRes_PV0_Diag_Get_Pull     (const int& Index1D) const ;

private:
   double m_VTrack ;

   void   Beg_PV0_Diag() ;
   void   End_PV0_Diag() ;
   void   SetParameters_Internal_PV0_Diag(double par[])                    ;

   TF1* p_TF1_PV0_Diag ;
   bool m_StatusFit_PV0_Diag ; // True: fit failed  ; False: Fit OK
   short int m_NberOf_V_FitRes_PV0_Diag_Pad ;
   std::vector < const Pad* > V_FitRes_PV0_Diag_Pad      ;
   std::vector < double >     V_FitRes_PV0_Diag_Residual ;
   std::vector < double >     V_FitRes_PV0_Diag_Pull     ;

   FitOutput* p_FitOutput_PV0_Diag ;
   
   
   
};



#endif

