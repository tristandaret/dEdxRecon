#/***************************************************************************
 * File: Cluster.h
 * Project: dEdxRecon
 *
 * Brief: Declaration of the Cluster class representing a group of pads
 *        forming a detected charge cluster. Provides accessors and
 *        utilities used by fitting and reconstruction code.
 *
 * Contents: Cluster class declaration and helpers.
 *
 * Notes: Implementation in Cluster.cxx.
 ***************************************************************************/

#ifndef Cluster_H
#define Cluster_H

#include "Misc.h"
#include "Pad.h"

#include "FitOutput.h"

#include "TF1.h"
#include "TMatrixD.h"
#include "TVirtualFitter.h"

/////////////////////////////////////////////////////////////
class Cluster {
public:
  /** Constructor */
  Cluster(const int &EventNber, const int &EntryNber, const int &ModuleNber);
  virtual ~Cluster();

  //------------------------------IDs-----------------------//
  int Get_EventNber() const;  // Get Event nber
  int Get_EntryNber() const;  // Get Entry nber
  int Get_ModuleNber() const; // Get Module nber

  //------------------------------Prints-----------------------//
  void WriteOut() const; // Big dump

  //------------------------------Cluster Validity-----------------------//
  int IsValid() const; // Get validation status
  void Validate();     // Validate
  void Invalidate();   // Invalidate

  //------------------------------Clusters Data-----------------------//
  double Get_Acluster() const; // Sum of Pads amplitude
  double Get_XWeight() const;  // Averaged cluster position
  double Get_YWeight() const;  // Averaged cluster position

  //------------------------------Pads-----------------------//
  void Add_Pad(Pad *pPad); // Add Pad
  void DoClosure();        // Closure: Order pads, compute Leading pad stuff

  int Get_NberOfPads() const;             // Size of Pad set
  Pad *Get_Pad(const int &Index1D) const; // Get Pad

  // Leading pad
  Pad *Get_LeadingPad() const;    // Get Leading pad
  double Get_YLeading() const;    // Get Leading pad Y position (m)
  double Get_TMaxLeading() const; // Get Leading pad Time at max (Time bin)
  double Get_AMaxLeading() const; // Get Leading pad Max amplitude (ADC)

  Pad *Get_NextLeadingPad() const; // Get Next Leading pad, the hotest of the
                                   // nearest pads of the leading pad
  Pad *Get_NextNextLeadingPad() const; // Get NextNext Leading pad, the coldest
                                       // of the nearest pads of the leading pad

  //------------------------------Data Members-----------------------//
private:
  short int m_NberOfPads;
  std::vector<Pad *> V_Pad;
  Pad *Pad_Leading;
  Pad *Pad_NextLeading;
  Pad *Pad_NextNextLeading;

  double m_Acluster;
  double m_XWeight;
  double m_YWeight;

  int m_EventNber;
  int m_EntryNber;
  short int m_ModuleNber;

  //---------------------------Cluster Fit Stuff-----------------------------//
private:
  double m_XTrack;
  double m_YTrack;
  double m_eXTrack;
  double m_eYTrack;
  bool m_StatusFit; // True: fit failed; False: Fit OK

  double m_Chi2Min;

  double m_XTrack_BeforeMinimisation;
  double m_YTrack_BeforeMinimisation;

  short int m_isValid = 1;

public:
  void Set_XTrack(const double &XTrack);
  void Set_YTrack(const double &YTrack);

  void Set_eXTrack(const double &eXTrack);
  void Set_eYTrack(const double &eYTrack);

  int StatusFit(); // 0: OK; 1: Failed

  double Get_XTrack() const;
  double Get_eXTrack() const;
  double Get_YTrack() const;
  double Get_eYTrack() const;

  double Get_Chi2Min() const;

  double Get_XTrack_BeforeMinimisation() const;
  double Get_YTrack_BeforeMinimisation() const;

  //---------------------Horizontal-----------------------------//
public:
  int StatusFit_Horizontal(); // 0: OK; 1: Failed
  void SetEval_Horizontal(TF1 *pTF1);
  double Eval_Horizontal(const double &Xin);
  int SetParameter_Horizontal(TVirtualFitter *pTVirtualFitter);
  void SetResults_Horizontal(TVirtualFitter *pTVirtualFitter);
  void SetResults_FailledFit_Horizontal(const int &Verbose);
  double Chi2_Horizontal(double par[]);

  int FitRes_Horizontal_Get_NberOfTermsInChi2() const;
  Pad *FitRes_Horizontal_Get_Pad(const int &Index1D) const;
  double FitRes_Horizontal_Get_Residual(const int &Index1D) const;
  double FitRes_Horizontal_Get_Pull(const int &Index1D) const;

private:
  void Beg_Horizontal();
  void End_Horizontal();
  void SetParameters_Internal_Horizontal(double par[]);

  TF1 *p_TF1_Horizontal;
  bool m_StatusFit_Horizontal; // True: fit failed; False: Fit OK
  short int m_NberOf_V_FitRes_Horizontal_Pad;
  std::vector<Pad *> V_FitRes_Horizontal_Pad;
  std::vector<double> V_FitRes_Horizontal_Residual;
  std::vector<double> V_FitRes_Horizontal_Pull;

  FitOutput *p_FitOutput_Horizontal;

  //---------------------Diagonal-----------------------------//
public:
  int StatusFit_Diagonal(); // 0: OK; 1: Failed
  void SetEval_Diagonal(TF1 *pTF1);
  double Eval_Diagonal(const double &Xin);
  int SetParameter_Diagonal(TVirtualFitter *pTVirtualFitter);
  void SetResults_Diagonal(TVirtualFitter *pTVirtualFitter);
  void SetResults_FailledFit_Diagonal(const int &Verbose);
  double Chi2_Diagonal(double par[]);
  ;
  double m_AngleRot;

  int FitRes_Diagonal_Get_NberOfTermsInChi2() const;
  Pad *FitRes_Diagonal_Get_Pad(const int &Index1D) const;
  double FitRes_Diagonal_Get_Residual(const int &Index1D) const;
  double FitRes_Diagonal_Get_Pull(const int &Index1D) const;

private:
  double m_VTrack;

  void Beg_Diagonal();
  void End_Diagonal();
  void SetParameters_Internal_Diagonal(double par[]);

  TF1 *p_TF1_Diagonal;
  bool m_StatusFit_Diagonal; // True: fit failed; False: Fit OK
  short int m_NberOf_V_FitRes_Diagonal_Pad;
  std::vector<Pad *> V_FitRes_Diagonal_Pad;
  std::vector<double> V_FitRes_Diagonal_Residual;
  std::vector<double> V_FitRes_Diagonal_Pull;

  FitOutput *p_FitOutput_Diagonal;
};

#endif
