#include "ClusterFitter.h"
#include "TFitter.h"
#include "Util.h"

//-------------------------------------Horizontal-------------------------------/
void ClusterFit_Horizontal(Sample &aSample, const int &ModuleNber,
                           TF1 *pTF1_ToBeUsed) {
  // Set the fitter
  ClusterFitter_Horizontal aClusterFitter_Horizontal("Minuit");

  int NberOfEvents = aSample.Get_NberOfEvents();
  for (int iE = 0; iE < NberOfEvents; iE++) {
    Event *pEvent = aSample.Get_Event(iE);
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0)
      continue;

    ClusterFit_Horizontal_Event(pEvent, ModuleNber, pTF1_ToBeUsed,
                                aClusterFitter_Horizontal);
  }
}

void ClusterFit_Horizontal_Event(
    Event *pEvent, const int &ModuleNber, TF1 *pTF1_ToBeUsed,
    ClusterFitter_Horizontal &aClusterFitter_Horizontal) {
  std::vector<Cluster *> ClusterSet =
      pEvent->GiveMe_Clusters_ForThisModule(ModuleNber);
  int NClusters = ClusterSet.size();
  for (int iC = 0; iC < NClusters; iC++) {
    Cluster *pCluster = ClusterSet[iC];

    ClusterFit_Horizontal_Cluster(pCluster, ModuleNber, pTF1_ToBeUsed,
                                  aClusterFitter_Horizontal);
  }
}

void ClusterFit_Horizontal_Cluster(
    Cluster *pCluster, const int &ModuleNber, TF1 *pTF1_ToBeUsed,
    ClusterFitter_Horizontal &aClusterFitter_Horizontal) {
  // Tell to the cluster which shape function it should use
  pCluster->SetEval_Horizontal(pTF1_ToBeUsed);

  // Tell to the fitter to fit the cluster position
  aClusterFitter_Horizontal.Set_Cluster(pCluster);
  int FitResult = aClusterFitter_Horizontal.DoMinimisation();
}

//--------------------------------------------------------------------//
// This class is needed to run Minuit
class StaticClusterFitter_Horizontal {
public:
  // Constructor
  StaticClusterFitter_Horizontal();
  virtual ~StaticClusterFitter_Horizontal();
  // Set
  static void Set(ClusterFitter_Horizontal *pClusterFitter_Horizontal);
  // Pointer
  static ClusterFitter_Horizontal *p_ClusterFitter_Horizontal;
};

ClusterFitter_Horizontal
    *StaticClusterFitter_Horizontal::p_ClusterFitter_Horizontal = NULL;

StaticClusterFitter_Horizontal::StaticClusterFitter_Horizontal() {}
StaticClusterFitter_Horizontal::~StaticClusterFitter_Horizontal() {}
void StaticClusterFitter_Horizontal::Set(
    ClusterFitter_Horizontal *pClusterFitter_Horizontal) {
  p_ClusterFitter_Horizontal = pClusterFitter_Horizontal;
}

void ClusterFitter_HorizontalFunction(int &nDim, double *gout, double &result,
                                      double par[], int flg) {
  result =
      StaticClusterFitter_Horizontal::p_ClusterFitter_Horizontal->Chi2(par);
}

//--------------------------------------------------------------------//

ClusterFitter_Horizontal::ClusterFitter_Horizontal(
    const std::string &FitterName) {
  m_FitterName = FitterName;

  p_TVirtualFitter = 0;

  delete p_TVirtualFitter;
  TVirtualFitter::SetDefaultFitter(m_FitterName.c_str());
  p_TVirtualFitter = TVirtualFitter::Fitter(0, 500);

  int ierr;
  double arg[1];

  // Set function pointer
  StaticClusterFitter_Horizontal::Set(this);
  p_TVirtualFitter->SetFCN(ClusterFitter_HorizontalFunction);

  p_TVirtualFitter->ExecuteCommand("clear", arg, 0);

  // Suppress all output
  arg[0] = -1;
  ierr = p_TVirtualFitter->ExecuteCommand("SET PRINT", arg, 1);

  // Ensure no warnings or other messages
  arg[0] = 0;
  ierr = p_TVirtualFitter->ExecuteCommand("SET NOW", arg, 1);

  // Set very small epsilon to avoid detailed logging
  arg[0] = 1.e-9;
  ierr = p_TVirtualFitter->ExecuteCommand("set eps", arg, 1);

  // Disable graphical output
  ierr = p_TVirtualFitter->ExecuteCommand("set nog", arg, 0);
}

ClusterFitter_Horizontal::~ClusterFitter_Horizontal() {}

void ClusterFitter_Horizontal::Set_Cluster(Cluster *pCluster) {
  p_Cluster = pCluster;
}

int ClusterFitter_Horizontal::DoMinimisation() {
  int ierReturned = p_Cluster->SetParameter_Horizontal(p_TVirtualFitter);

  //
  double arg[10];

  arg[0] = 10000.; // max number of calls
  arg[1] = 1.;     // tolerance on minimum (1. corresponds to 1.d-3)

  //
  int ierMIGRAD_N = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

  if (ierMIGRAD_N == 0) {

    int ierMINOS_N = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);

    p_Cluster->SetResults_Horizontal(p_TVirtualFitter);

    ierReturned = 0;

  } else {
    int ierMIGRAD_R1 = p_TVirtualFitter->ExecuteCommand("MINMIZE", arg, 2);
    ierMIGRAD_R1 = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

    if (ierMIGRAD_R1 == 0) {

      int ierMINOS_R1 = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);

      p_Cluster->SetResults_Horizontal(p_TVirtualFitter);

      ierReturned = 0;

    } else {

      int ierMIGRAD_R2 = p_TVirtualFitter->ExecuteCommand("SIMPLEX", arg, 2);
      ierMIGRAD_R2 = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

      if (ierMIGRAD_R2 == 0) {

        int ierMINOS_R2 = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);

        p_Cluster->SetResults_Horizontal(p_TVirtualFitter);

        ierReturned = 0;

      } else {

        ierReturned = ierMIGRAD_R2;

        p_Cluster->SetResults_FailledFit_Horizontal(0);
      }
    }
  }
  return ierReturned;
}

double ClusterFitter_Horizontal::Chi2(double par[]) {
  return p_Cluster->Chi2_Horizontal(par);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------Diagonal-------------------------------/
void ClusterFit_Diagonal(const double &AngleRot, Sample &aSample,
                         const int &ModuleNber, TF1 *pTF1_ToBeUsed) {
  // Set the fitter
  ClusterFitter_Diagonal aClusterFitter_Diagonal("Minuit");

  // Do the fits
  int Kounter_Fit = 0;
  int Kounter_Failure = 0;

  int NberOfEvents = aSample.Get_NberOfEvents();
  for (int iE = 0; iE < NberOfEvents; iE++) {
    Event *pEvent = aSample.Get_Event(iE);
    if (pEvent->Validity_ForThisModule(ModuleNber) == 0)
      continue;

    ClusterFit_Diagonal_Event(AngleRot, pEvent, ModuleNber, pTF1_ToBeUsed,
                              Kounter_Fit, Kounter_Failure,
                              aClusterFitter_Diagonal);
  }
  std::cout << std::endl;
  std::cout << " ClusterFit_Diagonal " << std::endl;
  std::cout << "	Nber of Fits		" << std::setw(20)
            << Kounter_Fit << std::endl;
  std::cout << "	Nber of Failed Fits " << std::setw(20)
            << Kounter_Failure << " ( " << std::setw(10) << std::setprecision(4)
            << 100. * double(Kounter_Failure) / double(Kounter_Fit) << " % ) "
            << std::endl;
}

void ClusterFit_Diagonal_Event(
    const double &AngleRot, Event *pEvent, const int &ModuleNber,
    TF1 *pTF1_ToBeUsed, int &Kounter_Fit, int &Kounter_Failure,
    ClusterFitter_Diagonal &aClusterFitter_Diagonal) {
  std::vector<Cluster *> ClusterSet =
      pEvent->GiveMe_Clusters_ForThisModule(ModuleNber);
  int NClusters = ClusterSet.size();
  for (int iC = 0; iC < NClusters; iC++) {
    Cluster *pCluster = ClusterSet[iC];

    ClusterFit_Diagonal_Cluster(AngleRot, pCluster, ModuleNber, pTF1_ToBeUsed,
                                Kounter_Fit, Kounter_Failure,
                                aClusterFitter_Diagonal);
  }
}

void ClusterFit_Diagonal_Cluster(
    const double &AngleRot, Cluster *pCluster, const int &ModuleNber,
    TF1 *pTF1_ToBeUsed, int &Kounter_Fit, int &Kounter_Failure,
    ClusterFitter_Diagonal &aClusterFitter_Diagonal) {
  // Tell to the cluster which shape function it should use
  pCluster->SetEval_Diagonal(pTF1_ToBeUsed);

  // Tell to the cluster which angle should be used
  pCluster->m_AngleRot = AngleRot;

  // Tell to the fitter to fit the cluster position
  aClusterFitter_Diagonal.Set_Cluster(pCluster);
  int FitResult = aClusterFitter_Diagonal.DoMinimisation();
  Kounter_Fit += 1;
  if (FitResult != 0)
    Kounter_Failure += 1;
}

//--------------------------------------------------------------------//
// This class is needed to run Minuit
class StaticClusterFitter_Diagonal {
public:
  // Constructor
  StaticClusterFitter_Diagonal();
  virtual ~StaticClusterFitter_Diagonal();
  // Set
  static void Set(ClusterFitter_Diagonal *pClusterFitter_Diagonal);
  // Pointer
  static ClusterFitter_Diagonal *p_ClusterFitter_Diagonal;
};

ClusterFitter_Diagonal *StaticClusterFitter_Diagonal::p_ClusterFitter_Diagonal =
    NULL;

StaticClusterFitter_Diagonal::StaticClusterFitter_Diagonal() {}
StaticClusterFitter_Diagonal::~StaticClusterFitter_Diagonal() {}
void StaticClusterFitter_Diagonal::Set(
    ClusterFitter_Diagonal *pClusterFitter_Diagonal) {
  p_ClusterFitter_Diagonal = pClusterFitter_Diagonal;
}

void ClusterFitter_DiagonalFunction(int &nDim, double *gout, double &result,
                                    double par[], int flg) {
  result = StaticClusterFitter_Diagonal::p_ClusterFitter_Diagonal->Chi2(par);
}

//--------------------------------------------------------------------//

ClusterFitter_Diagonal::ClusterFitter_Diagonal(const std::string &FitterName) {

  m_FitterName = FitterName;

  p_TVirtualFitter = 0;

  delete p_TVirtualFitter;
  TVirtualFitter::SetDefaultFitter(m_FitterName.c_str());
  p_TVirtualFitter = TVirtualFitter::Fitter(0, 500);

  int ierr;
  double arg[10];

  // Set fonction pointer
  StaticClusterFitter_Diagonal::Set(this);
  p_TVirtualFitter->SetFCN(ClusterFitter_DiagonalFunction);

  p_TVirtualFitter->ExecuteCommand("clear", arg, 0);

  arg[0] = -1;
  ierr = p_TVirtualFitter->ExecuteCommand("SET PRINT", arg, 1);

  arg[0] = 0;
  ierr = p_TVirtualFitter->ExecuteCommand("SET NOW", arg, 1);

  arg[0] = 1.e-9;
  ierr = p_TVirtualFitter->ExecuteCommand("set eps", arg, 1);
  ierr = p_TVirtualFitter->ExecuteCommand("set nog", arg, 0);

  arg[0] = 2;
  ierr = p_TVirtualFitter->ExecuteCommand("set str", arg, 1);
}

ClusterFitter_Diagonal::~ClusterFitter_Diagonal() {}

void ClusterFitter_Diagonal::Set_Cluster(Cluster *pCluster) {
  p_Cluster = pCluster;
}

int ClusterFitter_Diagonal::DoMinimisation() {
  int Verbose = 0;

  int ierReturned = p_Cluster->SetParameter_Diagonal(p_TVirtualFitter);

  //
  double arg[10];

  arg[0] = 10000.; // max number of calls
  arg[1] = 1.;     // tolerance on minimum (1. corresponds to 1.d-3)

  //
  int ierMIGRAD_N = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

  if (ierMIGRAD_N == 0) {

    int ierMINOS_N = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);
    if (Verbose == 1 && ierMINOS_N != 0)
      std::cout
          << " ClusterFitter_Diagonal::DoMinimisation() ierMINOS_N	"
          << ierMINOS_N << std::endl;

    p_Cluster->SetResults_Diagonal(p_TVirtualFitter);

    ierReturned = 0;

  } else {

    if (Verbose == 1)
      std::cout << std::endl;
    if (Verbose == 1)
      std::cout
          << " ClusterFitter_Diagonal::DoMinimisation() normal fitting failed "
          << "->ierMIGRAD_N " << ierMIGRAD_N << std::endl;

    int ierMIGRAD_R1 = p_TVirtualFitter->ExecuteCommand("MINMIZE", arg, 2);
    ierMIGRAD_R1 = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

    if (ierMIGRAD_R1 == 0) {

      if (Verbose == 1)
        std::cout
            << " ClusterFitter_Diagonal::DoMinimisation() Rescue 1 succeeded "
            << std::endl;

      int ierMINOS_R1 = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);
      if (Verbose == 1 && ierMINOS_R1 != 0)
        std::cout << " ClusterFitter_Diagonal::DoMinimisation() "
                     "->->ierMINOS_R1	"
                  << ierMINOS_R1 << std::endl;

      p_Cluster->SetResults_Diagonal(p_TVirtualFitter);

      ierReturned = 0;

    } else {

      if (Verbose == 1)
        std::cout
            << " ClusterFitter_Diagonal::DoMinimisation() Rescue 1 failed "
            << "->ierMIGRAD_R1 " << ierMIGRAD_R1 << std::endl;

      int ierMIGRAD_R2 = p_TVirtualFitter->ExecuteCommand("SIMPLEX", arg, 2);
      ierMIGRAD_R2 = p_TVirtualFitter->ExecuteCommand("MIGRAD", arg, 2);

      if (ierMIGRAD_R2 == 0) {

        if (Verbose == 1)
          std::cout
              << " ClusterFitter_Diagonal::DoMinimisation() Rescue 2 succeeded "
              << std::endl;

        int ierMINOS_R2 = p_TVirtualFitter->ExecuteCommand("MINOS", arg, 1);
        if (Verbose == 1 && ierMINOS_R2 != 0)
          std::cout << " ClusterFitter_Diagonal::DoMinimisation() "
                       "->->->ierMINOS_R2	"
                    << ierMINOS_R2 << std::endl;

        p_Cluster->SetResults_Diagonal(p_TVirtualFitter);

        ierReturned = 0;

      } else {

        ierReturned = ierMIGRAD_R2;

        if (Verbose == 1)
          std::cout
              << " ClusterFitter_Diagonal::DoMinimisation() Rescue 2 failed "
              << "->ierMIGRAD_R2 " << ierMIGRAD_R2 << std::endl;
        if (Verbose == 1)
          std::cout << " ClusterFitter_Diagonal::DoMinimisation() All Rescues "
                       "failed : Failure procedure called	"
                    << std::endl;

        p_Cluster->SetResults_FailledFit_Diagonal(Verbose);
      }
    }
  }

  return ierReturned;
}

double ClusterFitter_Diagonal::Chi2(double par[]) {
  return p_Cluster->Chi2_Diagonal(par);
}
