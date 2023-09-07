#ifndef Sample_H
#define Sample_H

#include "Misc/Misc.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_ChargeI.h"

#include "EvtModel/Pad.h"
#include "EvtModel/Event.h"
#include "EvtModel/Cluster.h"

#include "EvtModel/FitOutput.h"

#include "EvtModel/Track.h"
#include "EvtModel/SetOfTracks.h"

#include "SignalShape/SC_VS_x_rc_dd_SET_dd.h"

#include "TVirtualFitter.h"
#include "TMatrixD.h"
#include "TF1.h"


/////////////////////////////////////////////////////////////
class Sample {
public:
   /** Constructor */
   Sample();
   Sample(
          Model_ReadOutGeometry*  pModel_ReadOutGeometry  ,
          Model_Electronics*      pModel_Electronics      ,
          Model_ChargeI*          pModel_ChargeI  
         );
   virtual ~Sample();


//------------------------------Prints-----------------------//
   void SmallDump() const ;// Small dump
   void WriteOut()  const ;// Big dump
     
    
//------------------------------Models-----------------------//
//Set Models
   void Set_Model_ReadOutGeometry(Model_ReadOutGeometry* pModel_ReadOutGeometry) ;
   void Set_Model_Electronics    (Model_Electronics    * pModel_Electronics    ) ;
   void Set_Model_Charge         (Model_ChargeI        * pModel_ChargeI        ) ;
   
   
//Get Models
   Model_ReadOutGeometry*  Get_Model_ReadOutGeometry() ;
   Model_Electronics    *  Get_Model_Electronics    () ;
   Model_ChargeI        *  Get_Model_ChargeI        () ;
  
   
//------------------------------Event Addition-----------------------//
   void Add_Event(Event* pEvent) ; // Add Event
   

//------------------------------Event Access-----------------------//
   int    Get_NberOfEvents() const  ; // Size of Event set
   Event* Get_Event(int Index1D)    ; // Get Event


//------------------------------Track Access-----------------------//
   SetOfTracks* Get_SetOfTracks_ForThisModule(const int& ModuleNber) ; //Ownership is passed to user


//------------------------------PRF from input file-----------------------//
  void SetFilePRF(
                  const double& Norm ,
                  const double& a2   ,
                  const double& a4   ,
                  const double& b2   ,
                  const double& b4   
                 );
                  
  void GetFilePRF(
                  int&    FilePRF_Exist ,
                  double& Norm          ,
                  double& a2            ,
                  double& a4            ,
                  double& b2            ,    
                  double& b4   
                 );

//------------------------------Data Members-----------------------//
private:
//Copy constructor
    Sample(const Sample& obj){   }
  
//Copy assignment operator
    Sample& operator=(const Sample& tmp_obj){ return *this;}

private:

  Model_ReadOutGeometry*  p_Model_ReadOutGeometry  ;
  Model_Electronics*      p_Model_Electronics      ;
  Model_ChargeI*          p_Model_ChargeI          ;
  
  int m_NberOfEvents ;
  std::vector < Event* > V_Event ;
    
  short int    m_FilePRF_Exist ;
  double m_FilePRF_Norm  ;
  double m_FilePRF_a2    ;
  double m_FilePRF_a4    ;
  double m_FilePRF_b2    ;
  double m_FilePRF_b4    ;
  

//---------------------RC and DD Fit Stuff-----------------------------//
private:
   double m_RC      ; 
   double m_eRC     ; 
   double m_DD      ; 
   double m_eDD     ; 
   bool   m_StatusFit  ; // True: fit failed  ; False: Fit OK
   
   double m_Chi2Min ;

   double m_RC_BeforeMinimisation      ; 
   double m_DD_BeforeMinimisation      ; 

public:
   void Set_RC(const double& RC);

   void Set_DD(const double& RC);

   int  StatusFit();  // 0: OK ; 1: Failed
   
   double Get_RC      () const ;
   double Get_eRC     () const ;
   
   double Get_DD      () const ;
   double Get_eDD     () const ;
   
   double Get_Chi2Min () const ;
   
   double Get_RC_BeforeMinimisation() const ;
   double Get_DD_BeforeMinimisation() const ;
  
  
//---------------------PV2-----------------------------//
public:
   int    StatusFit_PV2();  // 0: OK ; 1: Failed
   void   SetEval_PV2                (TF1* pTF1, const int& ModuleNber) ;
   double Eval_PV2                  (const double& Xin)                 ;
   int    SetParameter_PV2          (TVirtualFitter* pTVirtualFitter)   ;
   void   SetResults_PV2            (TVirtualFitter* pTVirtualFitter)   ;
   void   SetResults_FailledFit_PV2 (const int& Verbose )               ;
   double Chi2_PV2                  (double par[])                      ;

   int        FitRes_PV2_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV2_Get_Pad      (int Index1D) const ;
   double     FitRes_PV2_Get_Residual (int Index1D) const ;
   double     FitRes_PV2_Get_Pull     (int Index1D) const ;

private:
   void   Beg_PV2() ;
   void   End_PV2() ;
   void   SetParameters_Internal_PV2(double par[])                    ;

   TF1* p_TF1_PV2 ;
   bool m_StatusFit_PV2 ; // True: fit failed  ; False: Fit OK
   int m_NberOf_V_FitRes_PV2_Pad ;
   std::vector < const Pad* > V_FitRes_PV2_Pad      ;
   std::vector < double >     V_FitRes_PV2_Residual ;
   std::vector < double >     V_FitRes_PV2_Pull     ;
   int m_ModuleNber_PV2 ;

   FitOutput* p_FitOutput_PV2 ;


//---------------------PV3-----------------------------//
public:
   int    StatusFit_PV3();  // 0: OK ; 1: Failed
   void   SetEval_PV3(SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd, const int& ModuleNber) ;
   double Eval_Alpha_PV3            (const double& Xin)                                   ;
   double Eval_Tau_PV3              (const double& Xin)                                   ;
   int    SetParameter_PV3          (TVirtualFitter* pTVirtualFitter)                     ;
   void   SetResults_PV3            (TVirtualFitter* pTVirtualFitter)                     ;
   void   SetResults_FailledFit_PV3 (const int& Verbose )                                 ;
   double Chi2_PV3                  (double par[])                                        ;

   int        FitRes_PV3_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV3_Get_Pad      (int Index1D) const ;
   double     FitRes_PV3_Get_Residual (int Index1D) const ;
   double     FitRes_PV3_Get_Pull     (int Index1D) const ;

private:
   void   Beg_PV3() ;
   void   End_PV3() ;
   void   SetParameters_Internal_PV3(double par[])                    ;

   SC_VS_x_rc_dd_SET_dd* p_PV3_SC_VS_x_rc_dd_SET_dd ;
   bool m_StatusFit_PV3 ; // True: fit failed  ; False: Fit OK
   int m_NberOf_V_FitRes_PV3_Pad ;
   std::vector < const Pad* > V_FitRes_PV3_Pad      ;
   std::vector < double >     V_FitRes_PV3_Residual ;
   std::vector < double >     V_FitRes_PV3_Pull     ;
   int m_ModuleNber_PV3 ;

   FitOutput* p_FitOutput_PV3 ;



//---------------------PV4-----------------------------//
public:
   int    StatusFit_PV4();  // 0: OK ; 1: Failed
   void   SetEval_PV4                (TF1* pTF1, const int& ModuleNber) ;
   double Eval_PV4                  (const double& Xin)                 ;
   int    SetParameter_PV4          (TVirtualFitter* pTVirtualFitter)   ;
   void   SetResults_PV4            (TVirtualFitter* pTVirtualFitter)   ;
   void   SetResults_FailledFit_PV4 (const int& Verbose )               ;
   double Chi2_PV4                  (double par[])                      ;

   int        FitRes_PV4_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV4_Get_Pad      (int Index1D) const ;
   double     FitRes_PV4_Get_Residual (int Index1D) const ;
   double     FitRes_PV4_Get_Pull     (int Index1D) const ;

private:
   void   Beg_PV4() ;
   void   End_PV4() ;
   void   SetParameters_Internal_PV4(double par[])                    ;

   TF1* p_TF1_PV4 ;
   bool m_StatusFit_PV4 ; // True: fit failed  ; False: Fit OK
   int m_NberOf_V_FitRes_PV4_Pad ;
   std::vector < const Pad* > V_FitRes_PV4_Pad      ;
   std::vector < double >     V_FitRes_PV4_Residual ;
   std::vector < double >     V_FitRes_PV4_Pull     ;
   int m_ModuleNber_PV4 ;

   FitOutput* p_FitOutput_PV4 ;



//---------------------PV31-----------------------------//
public:
   int    StatusFit_PV31();  // 0: OK ; 1: Failed
   void   SetEval_PV31(SC_VS_x_rc_dd_SET_dd* pSC_VS_x_rc_dd_SET_dd, const int& ModuleNber) ;
   double Eval_Alpha_PV31            (const double& Xin)                                   ;
   double Eval_Tau_PV31              (const double& Xin)                                   ;
   int    SetParameter_PV31          (TVirtualFitter* pTVirtualFitter)                     ;
   void   SetResults_PV31            (TVirtualFitter* pTVirtualFitter)                     ;
   void   SetResults_FailledFit_PV31 (const int& Verbose )                                 ;
   double Chi2_PV31                  (double par[])                                        ;

   int        FitRes_PV31_Get_NberOfTermsInChi2()            const ;
   const Pad* FitRes_PV31_Get_Pad      (int Index1D) const ;
   double     FitRes_PV31_Get_Residual (int Index1D) const ;
   double     FitRes_PV31_Get_Pull     (int Index1D) const ;

private:
   void   Beg_PV31() ;
   void   End_PV31() ;
   void   SetParameters_Internal_PV31(double par[])                    ;

   SC_VS_x_rc_dd_SET_dd* p_PV31_SC_VS_x_rc_dd_SET_dd ;
   bool m_StatusFit_PV31 ; // True: fit failed  ; False: Fit OK
   int m_NberOf_V_FitRes_PV31_Pad ;
   std::vector < const Pad* > V_FitRes_PV31_Pad      ;
   std::vector < double >     V_FitRes_PV31_Residual ;
   std::vector < double >     V_FitRes_PV31_Pull     ;
   int m_ModuleNber_PV31 ;

   FitOutput* p_FitOutput_PV31 ;
   
   int m_PV31_AddTime_Leading         ;
   int m_PV31_AddTime_NextLeading     ;
   int m_PV31_AddTime_NextNextLeading ;



};



#endif

