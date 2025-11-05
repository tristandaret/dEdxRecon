#/***************************************************************************
 * File: Cluster.cxx
 * Project: dEdxRecon
 *
 * Brief: Implementation of the Cluster class which groups pads into
 *        detected charge clusters and provides geometry/charge accessors
 *        used by fitting and reconstruction modules.
 *
 * Contents: Cluster methods for managing pads, computing cluster properties
 *           and printing utilities.
 *
 * Notes: See Cluster.h for the public interface.
 ***************************************************************************/

#include "Cluster.h"
#include "AmplitudeError.h"
#include "TimeError.h"
#include "Util.h"

Cluster::Cluster(const int &EventNber, const int &EntryNber,
                 const int &ModuleNber) {
  m_EventNber = EventNber;
  m_EntryNber = EntryNber;
  m_ModuleNber = ModuleNber;

  Pad_Leading = 0;
  Pad_NextLeading = 0;
  Pad_NextNextLeading = 0;

  m_NberOfPads = 0;

  m_StatusFit = 0;

  Beg_Horizontal();
  Beg_Diagonal();
}

Cluster::~Cluster() {
  V_Pad.clear();
  V_Pad.clear();
  std::vector<Pad *>().swap(V_Pad);
  End_Horizontal();
  End_Diagonal();
}

int Cluster::Get_EventNber() const { return m_EventNber; }

int Cluster::Get_EntryNber() const { return m_EntryNber; }

int Cluster::Get_ModuleNber() const { return m_ModuleNber; }

int Cluster::IsValid() const { return m_isValid; }
void Cluster::Validate() { m_isValid = 1; }
void Cluster::Invalidate() {
  m_isValid = 0;
  for (int i = 0; i < m_NberOfPads; i++)
    V_Pad[i]->Invalidate();
}

void Cluster::Add_Pad(Pad *pPad) {
  V_Pad.push_back(pPad);
  m_NberOfPads += 1;
}

void Cluster::DoClosure() {
  m_Acluster = 0.;
  m_XWeight = 0.;
  m_YWeight = 0.;
  int NberOfPads = Get_NberOfPads();

  for (int iPad = 0; iPad < NberOfPads; iPad++) {
    Pad *pPad = Get_Pad(iPad);
    double SMax = pPad->Get_AMax();
    m_Acluster += SMax;
    double XPad = pPad->Get_XPad();
    double YPad = pPad->Get_YPad();
    m_XWeight += XPad * SMax;
    m_YWeight += YPad * SMax;
  }
  m_XWeight = m_XWeight / m_Acluster;
  m_YWeight = m_YWeight / m_Acluster;
  m_XTrack = m_XWeight;
  m_eXTrack = 0.001;
  m_YTrack = m_YWeight;
  m_eYTrack = 0.001;

  std::vector<RankedValue> V_RankedValue;
  for (int iPad = 0; iPad < NberOfPads; iPad++) {
    Pad *pPad = Get_Pad(iPad);
    double SMax = pPad->Get_AMax();
    RankedValue aRankedValue;
    aRankedValue.Value = SMax;
    aRankedValue.Rank = iPad;
    V_RankedValue.push_back(aRankedValue);
  }
  std::sort(V_RankedValue.begin(), V_RankedValue.end());
  std::vector<Pad *> L_Pad;
  for (int iPad = 0; iPad < NberOfPads; iPad++) {
    Pad *pPad = Get_Pad(V_RankedValue[iPad].Rank);
    L_Pad.push_back(pPad);
  }
  V_Pad.clear();
  for (int iPad = 0; iPad < NberOfPads; iPad++) {
    V_Pad.push_back(L_Pad[iPad]);
  }

  Pad_Leading = V_Pad[NberOfPads - 1];

  double Amax_Next = 0.;
  Pad *pPad_Next = 0;
  double Amax_NextNext = 0.;
  Pad *pPad_NextNext = 0;
  for (int iPad = 0; iPad < NberOfPads; iPad++) {
    Pad *pPad = V_Pad[iPad];
    if (std::fabs(pPad->Get_iY() - Pad_Leading->Get_iY()) == 1) {
      double AMax = pPad->Get_AMax();
      if (pPad_Next == 0) {
        pPad_Next = pPad;
        Amax_Next = AMax;
      }
      if (Amax_Next < AMax) {
        pPad_Next = pPad;
        Amax_Next = AMax;
      }
      if (pPad_NextNext == 0) {
        pPad_NextNext = pPad;
        Amax_NextNext = AMax;
      }
      if (Amax_NextNext > AMax) {
        pPad_NextNext = pPad;
        Amax_NextNext = AMax;
      }
    }
  }
  if (pPad_Next)
    Pad_NextLeading = pPad_Next;
  if (pPad_NextNext)
    Pad_NextNextLeading = pPad_NextNext;
  if (Pad_NextNextLeading) {
    if (Pad_NextNextLeading->Get_iY() == Pad_NextLeading->Get_iY())
      Pad_NextNextLeading = 0;
  }
}

int Cluster::Get_NberOfPads() const { return m_NberOfPads; }
Pad *Cluster::Get_Pad(const int &Index1D) const { return V_Pad[Index1D]; }

double Cluster::Get_Acluster() const { return m_Acluster; }
double Cluster::Get_XWeight() const { return m_XWeight; }
double Cluster::Get_YWeight() const { return m_YWeight; }

Pad *Cluster::Get_LeadingPad() const { return Pad_Leading; }
double Cluster::Get_YLeading() const { return Pad_Leading->Get_YPad(); }
double Cluster::Get_TMaxLeading() const { return Pad_Leading->Get_TMax(); }

Pad *Cluster::Get_NextLeadingPad() const { return Pad_NextLeading; }
Pad *Cluster::Get_NextNextLeadingPad() const { return Pad_NextNextLeading; }

double Cluster::Get_AMaxLeading() const { return Pad_Leading->Get_AMax(); }

void Cluster::WriteOut() const {
  for (int iPad = 0; iPad < Get_NberOfPads(); iPad++) {
    Pad *pPad = V_Pad[iPad];
    int iX = pPad->Get_iX();
    int iY = pPad->Get_iY();
  }
}

//--------------------------------------------------------------------------//
void Cluster::Set_XTrack(const double &XTrack) { m_XTrack = XTrack; }
void Cluster::Set_YTrack(const double &YTrack) { m_YTrack = YTrack; }

void Cluster::Set_eXTrack(const double &eXTrack) { m_eXTrack = eXTrack; }
void Cluster::Set_eYTrack(const double &eYTrack) { m_eYTrack = eYTrack; }

int Cluster::StatusFit() { return m_StatusFit; }

double Cluster::Get_XTrack() const { return m_XTrack; }
double Cluster::Get_eXTrack() const { return m_eXTrack; }
double Cluster::Get_YTrack() const { return m_YTrack; }
double Cluster::Get_eYTrack() const { return m_eYTrack; }

double Cluster::Get_Chi2Min() const { return m_Chi2Min; }

double Cluster::Get_XTrack_BeforeMinimisation() const {
  return m_XTrack_BeforeMinimisation;
}
double Cluster::Get_YTrack_BeforeMinimisation() const {
  return m_YTrack_BeforeMinimisation;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------Horizontal-----------------------------//
void Cluster::Beg_Horizontal() {
  p_FitOutput_Horizontal = new FitOutput;
  p_FitOutput_Horizontal->Set(1);
  m_StatusFit_Horizontal = 0;
  m_NberOf_V_FitRes_Horizontal_Pad = 0;
  V_FitRes_Horizontal_Pad.clear();
  V_FitRes_Horizontal_Residual.clear();
  V_FitRes_Horizontal_Pull.clear();
}
void Cluster::End_Horizontal() {
  delete p_FitOutput_Horizontal;
  p_FitOutput_Horizontal = 0;
  V_FitRes_Horizontal_Pad.clear();
  std::vector<Pad *>().swap(V_FitRes_Horizontal_Pad);
  V_FitRes_Horizontal_Residual.clear();
  std::vector<double>().swap(V_FitRes_Horizontal_Residual);
  V_FitRes_Horizontal_Pull.clear();
  std::vector<double>().swap(V_FitRes_Horizontal_Pull);
}
int Cluster::StatusFit_Horizontal() { return m_StatusFit_Horizontal; }

void Cluster::SetEval_Horizontal(TF1 *pTF1) { p_TF1_Horizontal = pTF1; }

double Cluster::Eval_Horizontal(const double &Xin) {
  // The function Evaluate has its argument in cm
  // std::cout <<	" p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) <<
  // std::endl;
  if (Xin != Xin) {
    std::cout << " Cluster::Eval_Horizontal called with a NAN !" << std::endl;
    return 1.E99;
  } else {
    return (p_TF1_Horizontal->Eval(Xin * 1.E2));
  }
}

int Cluster::SetParameter_Horizontal(TVirtualFitter *pTVirtualFitter) {
  m_NberOf_V_FitRes_Horizontal_Pad = 0;
  V_FitRes_Horizontal_Pad.clear();
  V_FitRes_Horizontal_Residual.clear();
  V_FitRes_Horizontal_Pull.clear();

  int ier = 0;

  ier = pTVirtualFitter->SetParameter(
      0, "YTrack", m_YTrack, 0.0001,
      Pad_Leading->Get_YL() - (Pad_Leading->Get_YH() - Pad_Leading->Get_YL()),
      Pad_Leading->Get_YH() + (Pad_Leading->Get_YH() - Pad_Leading->Get_YL()));

  m_YTrack_BeforeMinimisation = m_YTrack;

  return ier;
}

void Cluster::SetResults_Horizontal(TVirtualFitter *pTVirtualFitter) {
  m_StatusFit_Horizontal = 0;
  m_StatusFit = 0;

  p_FitOutput_Horizontal->SetResults(pTVirtualFitter);

  m_YTrack = p_FitOutput_Horizontal->p_par[0];
  m_eYTrack = (p_FitOutput_Horizontal->p_eparplus[0] -
               p_FitOutput_Horizontal->p_eparminus[0]) /
              2.;

  m_Chi2Min = Chi2_Horizontal(p_FitOutput_Horizontal->p_par);

  m_NberOf_V_FitRes_Horizontal_Pad = 0;
  V_FitRes_Horizontal_Pad.clear();
  V_FitRes_Horizontal_Residual.clear();
  V_FitRes_Horizontal_Pull.clear();

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double YTrackYPad = m_YTrack - pPad->Get_YPad();
    double ThePrediction = Eval_Horizontal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    V_FitRes_Horizontal_Pad.push_back(pPad);
    V_FitRes_Horizontal_Residual.push_back(Residual);
    V_FitRes_Horizontal_Pull.push_back(Pull);
  }
  m_NberOf_V_FitRes_Horizontal_Pad = V_FitRes_Horizontal_Pad.size();
}

void Cluster::SetResults_FailledFit_Horizontal(const int &Verbose) {
  if (Verbose == 1)
    std::cout << " Cluster::SetResults_FailledFit_Horizontal	" << std::endl;

  m_StatusFit_Horizontal = 1;
  m_StatusFit = 1;

  m_YTrack = m_YTrack_BeforeMinimisation;
  m_eYTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL();

  m_Chi2Min = 1.E9;

  std::vector<std::string> V_PARname;
  V_PARname.push_back("YTrack");
  std::vector<double> V_PAR;
  V_PAR.push_back(m_YTrack);
  std::vector<double> V_ePAR;
  V_ePAR.push_back(m_eYTrack);

  p_FitOutput_Horizontal->SetResults(V_PARname, V_PAR, V_ePAR);

  m_NberOf_V_FitRes_Horizontal_Pad = 0;
  V_FitRes_Horizontal_Pad.clear();
  V_FitRes_Horizontal_Residual.clear();
  V_FitRes_Horizontal_Pull.clear();

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double YTrackYPad = m_YTrack - pPad->Get_YPad();
    double ThePrediction = Eval_Horizontal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    V_FitRes_Horizontal_Pad.push_back(pPad);
    V_FitRes_Horizontal_Residual.push_back(Residual);
    V_FitRes_Horizontal_Pull.push_back(Pull);
  }
  m_NberOf_V_FitRes_Horizontal_Pad = V_FitRes_Horizontal_Pad.size();
}

double Cluster::Chi2_Horizontal(double par[]) {
  SetParameters_Internal_Horizontal(par);

  double ToBeReturned = 0.;

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double YTrackYPad = m_YTrack - pPad->Get_YPad();
    double ThePrediction = Eval_Horizontal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    ToBeReturned += Pull * Pull;
  }

  return ToBeReturned;
}

void Cluster::SetParameters_Internal_Horizontal(double par[]) {
  if (par[0] != par[0]) {
    //	std::cout << " Cluster::SetParameters_Internal_Horizontal called with a
    //NAN !" <<
    // std::endl;
  } else {
    m_YTrack = par[0];
  }
}

int Cluster::FitRes_Horizontal_Get_NberOfTermsInChi2() const {
  return m_NberOf_V_FitRes_Horizontal_Pad;
}
Pad *Cluster::FitRes_Horizontal_Get_Pad(const int &Index1D) const {
  return V_FitRes_Horizontal_Pad[Index1D];
}
double Cluster::FitRes_Horizontal_Get_Residual(const int &Index1D) const {
  return V_FitRes_Horizontal_Residual[Index1D];
}
double Cluster::FitRes_Horizontal_Get_Pull(const int &Index1D) const {
  return V_FitRes_Horizontal_Pull[Index1D];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------Diagonal-----------------------------//
void Cluster::Beg_Diagonal() {
  m_AngleRot = M_PI / 4.;
  p_FitOutput_Diagonal = new FitOutput;
  p_FitOutput_Diagonal->Set(1);
  m_StatusFit_Diagonal = 0;
  m_NberOf_V_FitRes_Diagonal_Pad = 0;
  V_FitRes_Diagonal_Pad.clear();
  V_FitRes_Diagonal_Residual.clear();
  V_FitRes_Diagonal_Pull.clear();
}
void Cluster::End_Diagonal() {
  delete p_FitOutput_Diagonal;
  p_FitOutput_Diagonal = 0;
  V_FitRes_Diagonal_Pad.clear();
  std::vector<Pad *>().swap(V_FitRes_Diagonal_Pad);
  V_FitRes_Diagonal_Residual.clear();
  std::vector<double>().swap(V_FitRes_Diagonal_Residual);
  V_FitRes_Diagonal_Pull.clear();
  std::vector<double>().swap(V_FitRes_Diagonal_Pull);
}
int Cluster::StatusFit_Diagonal() { return m_StatusFit_Diagonal; }

void Cluster::SetEval_Diagonal(TF1 *pTF1) { p_TF1_Diagonal = pTF1; }

double Cluster::Eval_Diagonal(const double &Xin) {
  // The function Evaluate has its argument in cm
  // std::cout <<	" p_TF1->GetParameter(0) " << p_TF1->GetParameter(0) <<
  // std::endl;
  if (Xin != Xin) {
    std::cout << " Cluster::Eval_Diagonal called with a NAN !" << std::endl;
    return 1.E99;
  } else {
    return (p_TF1_Diagonal->Eval(Xin * 1.E2));
  }
}

int Cluster::SetParameter_Diagonal(TVirtualFitter *pTVirtualFitter) {
  m_NberOf_V_FitRes_Diagonal_Pad = 0;
  V_FitRes_Diagonal_Pad.clear();
  V_FitRes_Diagonal_Residual.clear();
  V_FitRes_Diagonal_Pull.clear();

  int ier = 0;

  double CosRot = std::cos(m_AngleRot);
  double SinRot = std::sin(m_AngleRot);
  double Uref = CosRot * m_XWeight + SinRot * m_YWeight;
  double Vref = -SinRot * m_XWeight + CosRot * m_YWeight;
  m_VTrack = Vref;

  // ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 ,
  // Pad_Leading->Get_YL(), Pad_Leading->Get_YH()
  // ); ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 ,
  // m_VTrack-0.10, m_VTrack+0.10 ); ier = pTVirtualFitter->SetParameter(0,
  // "VTrack" , m_VTrack , 0.0001 , Pad_Leading->Get_YL() -
  // (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()), Pad_Leading->Get_YH() +
  // (Pad_Leading->Get_YH()-Pad_Leading->Get_YL()) );
  ier = pTVirtualFitter->SetParameter(0, "VTrack", m_VTrack, 0.0001,
                                      Vref - 5.E-2, Vref + 5.E-2);
  // ier = pTVirtualFitter->SetParameter(0, "VTrack" , m_VTrack , 0.0001 , 0. ,
  // 0. );
  if (ier != 0)
    std::cout << " ier " << ier << " SetParameter " << std::endl;

  m_XTrack_BeforeMinimisation = CosRot * Uref - SinRot * m_VTrack;
  m_YTrack_BeforeMinimisation = SinRot * Uref + CosRot * m_VTrack;

  return ier;
}

void Cluster::SetResults_Diagonal(TVirtualFitter *pTVirtualFitter) {
  m_StatusFit_Diagonal = 0;
  m_StatusFit = 0;

  p_FitOutput_Diagonal->SetResults(pTVirtualFitter);

  m_VTrack = p_FitOutput_Diagonal->p_par[0];

  double CosRot = std::cos(m_AngleRot);
  double SinRot = std::sin(m_AngleRot);
  double Uref = CosRot * Get_LeadingPad()->Get_XPad() +
                SinRot * Get_LeadingPad()->Get_YPad();
  m_XTrack = CosRot * Uref - SinRot * m_VTrack;
  m_YTrack = SinRot * Uref + CosRot * m_VTrack;

  m_eXTrack = (p_FitOutput_Diagonal->p_eparplus[0] -
               p_FitOutput_Diagonal->p_eparminus[0]) /
              2.;
  m_eYTrack = m_eXTrack;

  m_Chi2Min = Chi2_Diagonal(p_FitOutput_Diagonal->p_par);

  m_NberOf_V_FitRes_Diagonal_Pad = 0;
  V_FitRes_Diagonal_Pad.clear();
  V_FitRes_Diagonal_Residual.clear();
  V_FitRes_Diagonal_Pull.clear();

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double XLocal = pPad->Get_XPad();
    double YLocal = pPad->Get_YPad();

    //	double ULocal =	CosRot * XLocal + SinRot * YLocal;
    double VLocal = -SinRot * XLocal + CosRot * YLocal;

    double YTrackYPad = m_VTrack - VLocal;
    //
    double ThePrediction = Eval_Diagonal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    V_FitRes_Diagonal_Pad.push_back(pPad);
    V_FitRes_Diagonal_Residual.push_back(Residual);
    V_FitRes_Diagonal_Pull.push_back(Pull);
  }
  m_NberOf_V_FitRes_Diagonal_Pad = V_FitRes_Diagonal_Pad.size();
}

void Cluster::SetResults_FailledFit_Diagonal(const int &Verbose) {
  if (Verbose == 1)
    std::cout << " Cluster::SetResults_FailledFit_Diagonal	" << std::endl;

  m_StatusFit_Diagonal = 1;
  m_StatusFit = 1;

  m_XTrack = m_XTrack_BeforeMinimisation;
  m_YTrack = m_YTrack_BeforeMinimisation;
  m_eXTrack = Pad_Leading->Get_YH() - Pad_Leading->Get_YL();
  m_eYTrack = m_eXTrack;

  double CosRot = std::cos(m_AngleRot);
  double SinRot = std::sin(m_AngleRot);
  // double Uref =	CosRot * m_XTrack +	SinRot * m_YTrack;
  double Vref = -SinRot * m_XTrack + CosRot * m_YTrack;
  m_VTrack = Vref;
  m_Chi2Min = 1.E9;

  std::vector<std::string> V_PARname;
  V_PARname.push_back("YTrack");
  std::vector<double> V_PAR;
  V_PAR.push_back(m_YTrack);
  std::vector<double> V_ePAR;
  V_ePAR.push_back(m_eYTrack);

  p_FitOutput_Diagonal->SetResults(V_PARname, V_PAR, V_ePAR);

  m_NberOf_V_FitRes_Diagonal_Pad = 0;
  V_FitRes_Diagonal_Pad.clear();
  V_FitRes_Diagonal_Residual.clear();
  V_FitRes_Diagonal_Pull.clear();

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double XLocal = pPad->Get_XPad();
    double YLocal = pPad->Get_YPad();

    //	double ULocal =	CosRot * XLocal + SinRot * YLocal;
    double VLocal = -SinRot * XLocal + CosRot * YLocal;

    double YTrackYPad = m_VTrack - VLocal;
    //
    double ThePrediction = Eval_Diagonal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    V_FitRes_Diagonal_Pad.push_back(pPad);
    V_FitRes_Diagonal_Residual.push_back(Residual);
    V_FitRes_Diagonal_Pull.push_back(Pull);
  }
  m_NberOf_V_FitRes_Diagonal_Pad = V_FitRes_Diagonal_Pad.size();
}

double Cluster::Chi2_Diagonal(double par[]) {
  SetParameters_Internal_Diagonal(par);

  double ToBeReturned = 0.;

  double CosRot = std::cos(m_AngleRot);
  double SinRot = std::sin(m_AngleRot);

  int NberOfPad = V_Pad.size();
  for (int iPad = 0; iPad < NberOfPad; iPad++) {
    Pad *pPad = V_Pad[iPad];

    double XLocal = pPad->Get_XPad();
    double YLocal = pPad->Get_YPad();

    //	double ULocal =	CosRot * XLocal + SinRot * YLocal;
    double VLocal = -SinRot * XLocal + CosRot * YLocal;

    double YTrackYPad = m_VTrack - VLocal;
    //
    double ThePrediction = Eval_Diagonal(YTrackYPad);

    double TheObserved = pPad->Get_AMax() / m_Acluster;

    double Residual = TheObserved - ThePrediction;

    double Nume = pPad->Get_AMax();
    double Deno = m_Acluster;

    double Error_Nume = std::sqrt(std::fabs(Nume));
    double Error_Deno = std::sqrt(std::fabs(Deno));

    //	double Error_Nume = 6.;	// Const Amplitude Error
    //	double Error_Deno = 6.;	// Const Amplitude Error

    double TheError = TheObserved * std::sqrt(std::pow(Error_Nume / Nume, 2) +
                                              std::pow(Error_Deno / Deno, 2));

    double Pull = Residual / TheError;

    ToBeReturned += Pull * Pull;
  }

  return ToBeReturned;
}

void Cluster::SetParameters_Internal_Diagonal(double par[]) {
  if (par[0] != par[0]) {
    //	std::cout << " Cluster::SetParameters_Internal_Diagonal called with a
    //NAN !" <<
    // std::endl;
  } else {
    m_VTrack = par[0];
  }
}

int Cluster::FitRes_Diagonal_Get_NberOfTermsInChi2() const {
  return m_NberOf_V_FitRes_Diagonal_Pad;
}
Pad *Cluster::FitRes_Diagonal_Get_Pad(const int &Index1D) const {
  return V_FitRes_Diagonal_Pad[Index1D];
}
double Cluster::FitRes_Diagonal_Get_Residual(const int &Index1D) const {
  return V_FitRes_Diagonal_Residual[Index1D];
}
double Cluster::FitRes_Diagonal_Get_Pull(const int &Index1D) const {
  return V_FitRes_Diagonal_Pull[Index1D];
}
