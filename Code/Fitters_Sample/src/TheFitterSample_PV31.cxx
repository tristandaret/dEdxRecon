#include "Fitters_Sample/TheFitterSample_PV31.h"
#include "TFitter.h"

//--------------------------------------------------------------------//
/** This class is needed to run Minuit */
class StaticTheFitterSample_PV31 {
public: 
   /** Constructor */
  StaticTheFitterSample_PV31();
  virtual ~StaticTheFitterSample_PV31( );
   /** Set */
  static void Set(TheFitterSample_PV31* pTheFitterSample_PV31); 
   /** Pointer */
  static TheFitterSample_PV31* p_TheFitterSample_PV31 ;
};

TheFitterSample_PV31* StaticTheFitterSample_PV31::p_TheFitterSample_PV31 = NULL;

StaticTheFitterSample_PV31::StaticTheFitterSample_PV31() {}
StaticTheFitterSample_PV31::~StaticTheFitterSample_PV31() {}
void StaticTheFitterSample_PV31::Set(TheFitterSample_PV31* pTheFitterSample_PV31) { p_TheFitterSample_PV31 = pTheFitterSample_PV31; }

void TheFitterSample_PV31Function(int& nDim, double* gout, double& result, double par[], int flg) 
{
  result = StaticTheFitterSample_PV31::p_TheFitterSample_PV31->Chi2(par);
}

//--------------------------------------------------------------------//

TheFitterSample_PV31::TheFitterSample_PV31(
               const std::string& FitterName
)
{

  m_FitterName = FitterName     ;
  
  p_TVirtualFitter = 0 ;
  
  delete p_TVirtualFitter ;
  TVirtualFitter::SetDefaultFitter(m_FitterName.c_str() );
  p_TVirtualFitter= TVirtualFitter::Fitter(0,500);

  int ierr ; double arg[10] ;

  // Set fonction pointer
  StaticTheFitterSample_PV31::Set(this);
  p_TVirtualFitter->SetFCN(TheFitterSample_PV31Function); 

  p_TVirtualFitter->ExecuteCommand("clear"   ,arg, 0) ;
  
  arg[0] = 3 ;// -1/1 no output/standard output
  arg[0] = -1 ;// -1/1 no output/standard output
  ierr = p_TVirtualFitter->ExecuteCommand("SET PRINT",arg,1);
  if (ierr != 0 ) std::cout << " ierr " << ierr << " for SET PRINT " << std::endl;
  
  arg[0] = 0 ;
  ierr = p_TVirtualFitter->ExecuteCommand("SET NOW",arg,1);
  if (ierr != 0 ) std::cout << " ierr " << ierr << " for SET NOW " << std::endl;
  
  arg[0] = 1.e-9;
  ierr = p_TVirtualFitter->ExecuteCommand("set eps" ,arg, 1) ; 
  if (ierr != 0 ) std::cout << " ierr " << ierr << " for set eps " << std::endl;
  ierr = p_TVirtualFitter->ExecuteCommand("set nog" ,arg, 0) ; 
  if (ierr != 0 ) std::cout << " ierr " << ierr << " for set nog " << std::endl;

  arg[0] = 2;
  ierr = p_TVirtualFitter->ExecuteCommand("set str" ,arg, 1) ; 
  if (ierr != 0 ) std::cout << " ierr " << ierr << " for set str " << std::endl;

}

TheFitterSample_PV31::~TheFitterSample_PV31() {}

void TheFitterSample_PV31::Set_Sample (Sample* pSample)
{
  p_Sample      = pSample ;
}

int TheFitterSample_PV31::DoMinimisation()
{
  int Verbose = 1 ;
  
  int ierReturned = p_Sample->SetParameter_PV31(p_TVirtualFitter) ;

  double arg[10] ;
  
  arg[0] = 10000. ; // max number of calls
  arg[1] = 1.     ; // tolerance on minimum (1. corresponds to 1.d-3)

//
  int ierMIGRAD_N = p_TVirtualFitter->ExecuteCommand("MIGRAD"  ,arg ,2) ;
  
  if (ierMIGRAD_N==0){

    int ierMINOS_N = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
    if (Verbose==1 && ierMINOS_N!=0) std::cout << " TheFitterSample_PV31::DoMinimisation() ierMINOS_N  " << ierMINOS_N << std::endl ;

    p_Sample->SetResults_PV31(p_TVirtualFitter) ;
    
    ierReturned = 0 ;
    
  }else{
  
    if (Verbose==1) 
    std::cout << std::endl ;
    if (Verbose==1) 
    std::cout << " TheFitterSample_PV31::DoMinimisation() normal fitting failed "
              << "->ierMIGRAD_N " << ierMIGRAD_N  
              << std::endl ;

    int ierMIGRAD_R1  = p_TVirtualFitter->ExecuteCommand("MINMIZE"  ,arg ,2) ;
        ierMIGRAD_R1  = p_TVirtualFitter->ExecuteCommand("MIGRAD"   ,arg ,2) ; 

    if (ierMIGRAD_R1==0){

      if (Verbose==1) 
      std::cout << " TheFitterSample_PV31::DoMinimisation() Rescue 1 succeeded "
                << std::endl ;

      int ierMINOS_R1 = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
      if (Verbose==1 && ierMINOS_R1!=0) std::cout << " TheFitterSample_PV31::DoMinimisation() ->->ierMINOS_R1  " << ierMINOS_R1 << std::endl ;

      p_Sample->SetResults_PV31(p_TVirtualFitter) ;

      ierReturned = 0 ;
    
    }else{

      if (Verbose==1) 
      std::cout << " TheFitterSample_PV31::DoMinimisation() Rescue 1 failed "
                << "->ierMIGRAD_R1 " << ierMIGRAD_R1  
                << std::endl ;

      int ierMIGRAD_R2  = p_TVirtualFitter->ExecuteCommand("SIMPLEX"  ,arg ,2) ;
          ierMIGRAD_R2  = p_TVirtualFitter->ExecuteCommand("MIGRAD"   ,arg ,2) ; 

      if (ierMIGRAD_R2==0){

        if (Verbose==1) 
        std::cout << " TheFitterSample_PV31::DoMinimisation() Rescue 2 succeeded "
                  << std::endl ;

        int ierMINOS_R2 = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
        if (Verbose==1 && ierMINOS_R2!=0) std::cout << " TheFitterSample_PV31::DoMinimisation() ->->->ierMINOS_R2  " << ierMINOS_R2 << std::endl ;

        p_Sample->SetResults_PV31(p_TVirtualFitter) ;

        ierReturned = 0 ;
    
      }else{

        ierReturned = ierMIGRAD_R2 ;

        if (Verbose==1) 
        std::cout << " TheFitterSample_PV31::DoMinimisation() Rescue 2 failed "
                  << "->ierMIGRAD_R2 " << ierMIGRAD_R2  
                  << std::endl ;
        if (Verbose==1) 
        std::cout << " TheFitterSample_PV31::DoMinimisation() All Rescues failed : Failure procedure called  "
                  << std::endl ;
       
        p_Sample->SetResults_FailledFit_PV31(Verbose) ;

      }

    }
  }

//   ier = p_TVirtualFitter->ExecuteCommand("MIGRAD"  ,arg ,2) ;
//   ier = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
// 
//   ier = p_TVirtualFitter->ExecuteCommand("MIGRAD"  ,arg ,2) ;
//   ier = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
// 
//   ier = p_TVirtualFitter->ExecuteCommand("MIGRAD"  ,arg ,2) ;
//   ier = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;

///  p_Sample->SetResults_PV31(p_TVirtualFitter) ;

//   int STATUS = (((TFitter*)p_TVirtualFitter)->GetMinuit())->GetStatus() ;
// 
//   std::cout 
//     << "TheFitterSample_PV31::DoMinimisation()"  
//     << " STATUS " << STATUS << std::endl ;
  

  return ierReturned ;
      
}

double TheFitterSample_PV31::Chi2(double par[]) { return p_Sample->Chi2_PV31(par) ; }
