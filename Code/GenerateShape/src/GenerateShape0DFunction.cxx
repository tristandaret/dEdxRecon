#include "GenerateShape/GenerateShape0DFunction.h"
#include "Misc/Util.h"

#include "EvtModel/Pad.h"
#include "EvtModel/ROBoard.h"

#include "SignalModel/Model_ReadOutGeometry.h"
#include "SignalModel/Model_Electronics.h"
#include "SignalModel/Model_Charge0D.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"

void GenerateShapeFunction0D_BaseTest()
{
  int    iOptWS = 1 ;
  double YDeposit          =  0. ;
  double RCin = 50. ;
  double DriftDistanceinmm =  0. ;
  
  GenerateShapeFunction0D_Standard(
    iOptWS            , 
    YDeposit          ,
    RCin              , 
    DriftDistanceinmm
  );
  
}

void GenerateShapeFunction0D_Standard(
  const int&    iOptWS            , 
  const double& YDeposit          ,
  const double& RCin              , 
  const double& DriftDistanceinmm
){
//
  if ( iOptWS!=1 && iOptWS!=2 ){
    std::cout << "GenerateShapeFunction0D_Standard: iOptWS!=1 && iOptWS!=2 " << std::endl ;
    std::cout << "iOptWS= " << iOptWS << std::endl ;
    abort();
  }

//
  std::ostringstream aostringstream ;
  aostringstream << std::setiosflags(std::ios::fixed) ;
  if (iOptWS==1)
  aostringstream << "_400ns"  ;
  if (iOptWS==2)
  aostringstream << "_200ns"  ;
  aostringstream << "_YD" << std::setprecision(6) <<  YDeposit ;
  aostringstream << "_RC" << std::setprecision(2) <<  RCin ;
  aostringstream << "_DD" << std::setprecision(2) <<  DriftDistanceinmm ;
  std::string TAG = aostringstream.str() ;  

  std::cout << " iOptWS              " << iOptWS            << std::endl ;
  std::cout << " YDeposit            " << YDeposit          << std::endl ;
  std::cout << " RCin                " << RCin              << std::endl ;
  std::cout << " DriftDistanceinmm   " << DriftDistanceinmm << std::endl ;
  std::cout << " =>TAG               " << TAG               << std::endl ;
  
  GenerateShapeFunction0D_Base(
    YDeposit          ,
    RCin              , 
    DriftDistanceinmm ,
    iOptWS            , 
    TAG
  );
  
}

void GenerateShapeFunction0D_Base(
  const double&      YDeposit          ,
  const double&      RCin              , 
  const double&      DriftDistanceinmm ,
  const int&         iOptWS            , 
  const std::string& TAG
){
//
  if ( iOptWS!=1 && iOptWS!=2 ){
    std::cout << "GenerateShapeFunction0D_Base: iOptWS!=1 && iOptWS!=2 " << std::endl ;
    std::cout << "iOptWS= " << iOptWS << std::endl ;
    abort();
  }

//
  Model_ReadOutGeometry*  pModel_ReadOutGeometry = new Model_ReadOutGeometry ;
  Model_Electronics*      pModel_Electronics     = new Model_Electronics     ;
  Model_Charge0D*         pModel_Charge0D        = new Model_Charge0D        ;
  
  if (iOptWS==2){
    double TheQValue = pModel_Electronics->Get_QValue() ;
    double TheWS     = pModel_Electronics->Get_ws()     ;
    TheWS = TheWS * 2. ;
    pModel_Electronics->Set_TimeShape(TheQValue,TheWS);
  }

  double LocalRC = RCin * 1E-9 / ( 1E-3 * 1E-3) ;
  pModel_Charge0D->Set_RC(LocalRC) ;
  
  double Qprim = 100.*1.602176565E-19  ;
  pModel_Charge0D->Set_Qprim(Qprim) ;
  pModel_Charge0D->Set_Gain( 1000. ) ;

  pModel_Charge0D->Set_WidthFromDriftDistance(DriftDistanceinmm/10.) ;

//
  double LX = pModel_ReadOutGeometry->Get_LX() ;
//double LY = pModel_ReadOutGeometry->Get_LY() ;

  int    Nx = pModel_ReadOutGeometry->Get_Nx() ;  
  int    Ny = pModel_ReadOutGeometry->Get_Ny() ; 

//  
  ROBoard aROBoard(
                   pModel_ReadOutGeometry  ,
                   pModel_Electronics      ,
                   pModel_Charge0D
                  );
                  
  int EventNber = 0 ;
  int EntryNber = 0 ;
  int ModuleNber = 0 ;
  for (int iX = 0 ; iX< Nx; iX++){
    for (int iY = 0 ; iY< Ny; iY++){
       std::ostringstream aostringstream ;
       aostringstream << std::setiosflags(std::ios::fixed) ;
       aostringstream << "( " << iX << " , " << iY << " )" ;
       std::string PadName = aostringstream.str() ; 
       Pad* pPad = new Pad (pModel_ReadOutGeometry , pModel_Electronics , pModel_Charge0D , PadName , EventNber,EntryNber,ModuleNber ,iX ,iY); 
       aROBoard.Add_Pad(pPad);
    }
  }
  
//  
  int iXc =  Nx/2 ;
  int iYc =  Ny/2 ;
  Pad* pPad_Center = aROBoard.Get_Pad(iXc,iYc) ;

  double Xc = pPad_Center->Get_XPad() ;
  double Yc = pPad_Center->Get_YPad() ;
  
//   std::cout
//     << " Xc " << Xc 
//     << " Yc " << Yc 
//     << std::endl ;

  double Time0  = 0. ;

//
  double XDeposit_Min =  0. ;
  double XDeposit_Max =  0.5*LX ;
  int    iXDeposit_Min =  0 ;
  int    iXDeposit_Max= 100 ;
         iXDeposit_Max = int( 0.5*LX*1.E6/10 ) + 1 ;
         std::cout << " iXDeposit_Max " << iXDeposit_Max << std::endl ;
//       iXDeposit_Max= 2 ;
  
//
  double Time_Min =     0. * 1E-9 ;
  double Time_Max = 20000. * 1E-9 ; 
  int    iTime_Min =      0 ;
  int    iTime_Max = 100000 ;
//       iTime_Max = 2      ;
  
//
  std::vector< double > V_X ;      
  std::vector< std::vector< double > > V_V_A;      
  std::vector< std::vector< double > > V_V_T;      
  
  for (int iXDeposit = iXDeposit_Min ; iXDeposit< iXDeposit_Max; iXDeposit++){
    double XDeposit = XDeposit_Min + double(iXDeposit)*(XDeposit_Max -XDeposit_Min)/double(iXDeposit_Max-1) ;
    double XTrue = Xc + XDeposit ;
    double YTrue = Yc + YDeposit ;
    
//     std::cout
//       << " XTrue " << XTrue 
//       << " YTrue " << YTrue
//       << std::endl ;
    
//
    int NberOfPads = aROBoard.Get_NberOfPads() ;
    for (int iPad = 0 ; iPad< NberOfPads; iPad++){
      Pad* pPad = aROBoard.Get_Pad(iPad) ;
      pPad->SetSignalModel( Time0 ,XTrue,YTrue ); 
    }

//
    std::vector< double > V_A(9,0.) ;      
    std::vector< double > V_T(9,0.) ;      
    for (int iDY = 0 ; iDY< 3 ; iDY++){
    for (int iDX = 0 ; iDX< 3 ; iDX++){
      int iX = iXc - 1 + iDX ;
      int iY = iYc - 1 + iDY ;
      int iLinear = iDX + 3*iDY ;
      
      Pad* pPad_Cur  = aROBoard.Get_Pad(iX,iY) ;
      
      double A_Max = 0. ;
      double T_Max = 0. ;
      for (int iTime = iTime_Min ; iTime< iTime_Max ; iTime++){
        double TimeCur = Time_Min + double (iTime)*(Time_Max-Time_Min)/double(iTime_Max-1) ;
        double A_Pad = pPad_Cur->Get_APad(TimeCur) ;
        
        if (iTime==iTime_Min) {
          A_Max = A_Pad   ;
          T_Max = TimeCur ;
        }
        if (A_Max<=A_Pad){
          A_Max = A_Pad   ;
          T_Max = TimeCur ;
        }else{
          break ;
        }
      }
      V_A[iLinear] = A_Max ;
      V_T[iLinear] = T_Max ;
    }
    }
    
//     for (int iDY = 0 ; iDY< 3 ; iDY++){
//     for (int iDX = 0 ; iDX< 3 ; iDX++){
//       int iLinear = iDX + 3*iDY ;
//       std::cout << " " << std::setw(15) << std::setprecision(3) << V_A[iLinear] ;
//     }
//     std::cout << std::endl ;
//     }
//     std::cout << std::endl ;
//     for (int iDY = 0 ; iDY< 3 ; iDY++){
//     for (int iDX = 0 ; iDX< 3 ; iDX++){
//       int iLinear = iDX + 3*iDY ;
//       std::cout << " " << std::setw(15) << std::setprecision(9) << V_T[iLinear] ;
//     }
//     std::cout << std::endl ;
//     }
    
    V_X  .push_back( XDeposit ) ;
    V_V_A.push_back( V_A      ) ;
    V_V_T.push_back( V_T      ) ;
    
  }    

//
  std::string Out_PRF =  "OUT_SignalShape/SignalTime_0D_True" + TAG + ".csv" ;

  std::streambuf *coutbuf = std::cout.rdbuf();       // Save old buf
  std::ofstream OUT_DataFile( Out_PRF.c_str() ); // Set output file
  std::cout.rdbuf(OUT_DataFile.rdbuf());             // Redirect std::cout to output file

    int iTem_Max = V_X.size() ;
    for (int iTem = 0 ; iTem< iTem_Max; iTem++){
      std::cout 
        << std::setw(15) << std::setprecision(9) << V_X[iTem] 
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[0]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[1]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[2]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[3]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[4]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[5]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[6]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[7]
        << " ; "  << std::setw(15) << std::setprecision(3) << (V_V_A[iTem])[8]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[0]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[1]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[2]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[3]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[4]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[5]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[6]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[7]
        << " ; "  << std::setw(15) << std::setprecision(9) << (V_V_T[iTem])[8]
        << std::endl ;
    }

  std::cout.rdbuf(coutbuf); // Reset to standard output again

}
