#include "Fitters/TheFitterTrack.h"
#include "TFitter.h"

//--------------------------------------------------------------------//
/** This class is needed to run Minuit */
class StaticTheFitterTrack {
public: 
   /** Constructor */
  StaticTheFitterTrack();
  virtual ~StaticTheFitterTrack( );
   /** Set */
  static void Set(TheFitterTrack* pTheFitterTrack); 
   /** Pointer */
  static TheFitterTrack* p_TheFitterTrack ;
};

TheFitterTrack* StaticTheFitterTrack::p_TheFitterTrack = NULL;

StaticTheFitterTrack::StaticTheFitterTrack() {}
StaticTheFitterTrack::~StaticTheFitterTrack() {}
void StaticTheFitterTrack::Set(TheFitterTrack* pTheFitterTrack) { p_TheFitterTrack = pTheFitterTrack; }

void TheFitterTrackFunction(int& nDim, double* gout, double& result, double par[], int flg) 
{
  result = StaticTheFitterTrack::p_TheFitterTrack->Chi2(par);
}

//--------------------------------------------------------------------//

TheFitterTrack::TheFitterTrack(
               const std::string& FitterName,
               const int& NberOfParameters 
)
{

  m_FitterName = FitterName     ;
  
  m_NberOfParameters = NberOfParameters     ;
  
  p_TVirtualFitter = 0 ;
  
  delete p_TVirtualFitter ;
  TVirtualFitter::SetDefaultFitter(m_FitterName.c_str() );
  p_TVirtualFitter= TVirtualFitter::Fitter(0,500);

  int ierr ; double arg[10] ;

  // Set fonction pointer
  StaticTheFitterTrack::Set(this);
  p_TVirtualFitter->SetFCN(TheFitterTrackFunction); 

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

TheFitterTrack::~TheFitterTrack() {}

void TheFitterTrack::Set_Track (Track* pTrack)
{
  p_Track      = pTrack ;
  p_Track->SetNberOfParameters(m_NberOfParameters) ;
}

int TheFitterTrack::DoMinimisation()
{
  int ierReturned = p_Track->SetParameter(p_TVirtualFitter) ;

//  
  double arg[10] ;
  
  arg[0] = 10000. ; // max number of calls
  arg[1] = 1.     ; // tolerance on minimum (1. corresponds to 1.d-3)

//
  int ierMIGRAD_N = p_TVirtualFitter->ExecuteCommand("MIGRAD"  ,arg ,2) ;
  
  if (ierMIGRAD_N==0){

    int ierMINOS_N = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
    if (ierMINOS_N!=0) std::cout << " TheFitterTrack::DoMinimisation() ierMINOS_N  " << ierMINOS_N << std::endl ;

    p_Track->SetResults(p_TVirtualFitter) ;
    
    ierReturned = 0 ;
    
  }else{
  
    std::cout << std::endl ;
    std::cout << " TheFitterTrack::DoMinimisation() normal fitting failed "
              << "->ierMIGRAD_N " << ierMIGRAD_N  
              << std::endl ;

    int ierMIGRAD_R1  = p_TVirtualFitter->ExecuteCommand("MINMIZE"  ,arg ,2) ;
        ierMIGRAD_R1  = p_TVirtualFitter->ExecuteCommand("MIGRAD"   ,arg ,2) ; 

    if (ierMIGRAD_R1==0){

      std::cout << " TheFitterTrack::DoMinimisation() Rescue 1 succeeded "
                << std::endl ;

      int ierMINOS_R1 = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
      if (ierMINOS_R1!=0) std::cout << " TheFitterTrack::DoMinimisation() ->->ierMINOS_R1  " << ierMINOS_R1 << std::endl ;

      p_Track->SetResults(p_TVirtualFitter) ;

      ierReturned = 0 ;
    
    }else{

      std::cout << " TheFitterTrack::DoMinimisation() Rescue 1 failed "
                << "->ierMIGRAD_R1 " << ierMIGRAD_R1  
                << std::endl ;

      int ierMIGRAD_R2  = p_TVirtualFitter->ExecuteCommand("SIMPLEX"  ,arg ,2) ;
          ierMIGRAD_R2  = p_TVirtualFitter->ExecuteCommand("MIGRAD"   ,arg ,2) ; 

      if (ierMIGRAD_R2==0){

        std::cout << " TheFitterTrack::DoMinimisation() Rescue 2 succeeded "
                  << std::endl ;

        int ierMINOS_R2 = p_TVirtualFitter->ExecuteCommand("MINOS"   ,arg ,1) ;
        if (ierMINOS_R2!=0) std::cout << " TheFitterTrack::DoMinimisation() ->->->ierMINOS_R2  " << ierMINOS_R2 << std::endl ;

        p_Track->SetResults(p_TVirtualFitter) ;

        ierReturned = 0 ;
    
      }else{

        ierReturned = ierMIGRAD_R2 ;

        std::cout << " TheFitterTrack::DoMinimisation() Rescue 2 failed "
                  << "->ierMIGRAD_R2 " << ierMIGRAD_R2  
                  << std::endl ;
        std::cout << " TheFitterTrack::DoMinimisation() All Rescues failed : abort "
                  << std::endl ;
       
        int NClusters = p_Track->Get_NberOfCluster();
        for (int iC = 0 ; iC< NClusters; iC++){
          const Cluster* pCluster = p_Track->Get_Cluster(iC);
          std::cout
           << " Y "   << std::setw(16) << std::setprecision(3) <<  pCluster->Get_YTrack()  * 1.E3
           << " +/- " << std::setw(16) << std::setprecision(3) <<  pCluster->Get_eYTrack() * 1.E3
           << std::endl ;
        }
        
        abort();

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

//   p_Track->SetResults(p_TVirtualFitter) ;

//   int STATUS = (((TFitter*)p_TVirtualFitter)->GetMinuit())->GetStatus() ;
// 
//   std::cout 
//     << "TheFitterTrack::DoMinimisation()"  
//     << " STATUS " << STATUS << std::endl ;
  

  return ierReturned ;
      
}

double TheFitterTrack::Chi2(double par[]) { return p_Track->Chi2(par) ; }
