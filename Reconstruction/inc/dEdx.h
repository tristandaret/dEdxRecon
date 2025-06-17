#ifndef DEDX_H
#define DEDX_H

#include "Misc.h"

#include "Variables.h"
#include "LUTs.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TObject.h>

#include "Uploader.h"

namespace Reconstruction {

class RecoPad : public TObject {
public:
   virtual ~RecoPad();

   bool leading = false;
   int ix = 0;
   int iy = 0;
   float xPad = 0; // in cm
   float yPad = 0; // in cm
   float ADCmax = 0;
   float ADCmax_base = 0;
   float charge = 0;
   float dEdxXP = 0;
   float RC = 0;
   float gain = 0;
   float GainCorrection = 0;
   float phi = 0;
   float d = 0;
   float dd = 0;
   float length = 0;
   int TMax = 0;
   int T0 = 0;
   float driftDistance = 0;
   float ratioDrift = 0;
   float ratioFile = 0;
   float ratio = 0;
   float dy = 0; // distance to the track in y

   ClassDef(RecoPad, 1);
};

class RecoCluster : public TObject {
public:
   virtual ~RecoCluster();

   std::vector<RecoPad *> v_pads;
   float length = 0;
   float charge = 0;
   float ADCmax_base = 0;
   float ALead_base = 0;
   float ALead_GCorr = 0;
   int TLead = 0;
   float dEdxWF = 0;
   int NPads = 0;
   float ratioCorr = 0;
   float yCluster = 0;
   float yWeight = 0;
   float LUTrhoLead = 0;

   ClassDef(RecoCluster, 1);
};

class RecoModule : public TObject {
public:
   virtual ~RecoModule();

   std::vector<RecoCluster *> v_clusters;
   bool selected = false;
   int ID = 0;
   int position = 0;
   float dEdxXP = 0;
   float dEdxWF = 0;
   float dEdxXPnoTrunc = 0;
   float dEdxWFnoTrunc = 0;
   int NCrossedPads = 0;
   int NClusters = 0;
   int NPads = 0;
   float lengthXP = 0;
   float lengthWF = 0;
   float phi = 0;
   float avg_pad_mult = 0;
   TF1 *Track = new TF1("pTrackFit", "[0]*x+[1]+[2]*x*x", 0, 2000);

   ClassDef(RecoModule, 1);
};

class RecoEvent : public TObject {
public:
   RecoEvent();
   virtual ~RecoEvent();
   void Clear();

   std::vector<RecoModule *> v_modules;
   std::vector<int> v_modules_position;
   bool selected = false;
   int eventNbr = 0;
   float dEdxXP = 0;
   float dEdxXP4 = 0;
   float dEdxXP5 = 0;
   float dEdxWF = 0;
   float dEdxGPL = 0;
   float dEdxGPC = 0;
   float dEdxGP1 = 0;
   float dEdxGP2 = 0;
   float dEdxGP3 = 0;
   float dEdxGP4 = 0;
   float dEdxGP5 = 0;
   float dEdxXPnoTrunc = 0;
   float dEdxWFnoTrunc = 0;
   int NCrossedPads = 0;
   int NPads = 0;
   int NClusters = 0;
   float lengthXP = 0;
   float lengthWF = 0;
   int numberOfModules = 0;
   float avg_pad_mult = 0;

   // Giga Pad variables
   TH1F *GWF = nullptr;
   TH1F *GWFtruncatedGP1 = nullptr;
   int peakingTime = 0; // in ns
   int timeBinSize = 0; // in ns

   ClassDef(RecoEvent, 1);
};

class dEdx {
public:
   dEdx();
   virtual ~dEdx();
   void Reconstruction();
   float
   ComputedEdxWF(std::vector<float> v_dEdxWF, const int &NClusters, const float &alpha);
   float ComputedEdxXP(const std::vector<float> &v_dEdx, const std::vector<float> &v_dE,
                       const std::vector<float> &v_dx, const int &nCrossedPads,
                       const float &alpha);

   // GP1: sum waveforms of bottom 70% clusters
   float
   ComputedEdxGP1(const std::vector<TH1F> &vClusWF, const std::vector<float> &v_dEdx,
                  const std::vector<float> &v_Aclus, const std::vector<float> &v_Lclus,
                  const int &nClusters, const float &alpha);

   // GPL: Remove from total GWF the ETF of the top 30% leading pads
   // GPC: Remove from total GWF the ETF of the top 30% crossed pads
   // GP2: Remove from total GWF the ETF of the top 30% clusters
   // GP3: Remove from total GWF the ETF of the top 30% clusters computed as
   // Alead*ratio(y_barycenter)
   // GP4: Remove from total GWF the ETF of the top 30% clusters computed with LUT
   // GP5: Remove from total GWF the ETF of the top 30% crossed pads computed with LUT
   float ComputedEdxGP(const TH1F *GWF, const std::vector<TH1F> &vWF,
                       const std::vector<float> &v_dEdx, const std::vector<float> &v_Amax,
                       const float &eventLength, const std::vector<float> &v_Length,
                       const int &nElements, const float &alpha);
   float ComputedEdxGA(const TH1F *GWF, const std::vector<TH1F> &vWF,
                       const std::vector<float> &v_dEdx, const std::vector<float> &v_Amax,
                       const float &eventLength, const std::vector<float> &v_Length,
                       const int &nElements, const float &alpha);
   float ComputedEdxXP5(const TH1F *GWF, const std::vector<TH1F> &vWF,
                       const std::vector<float> &v_dEdx, const std::vector<float> &v_Amax,
                       const float &eventLength, const std::vector<float> &v_Length,
                       const int &nElements, const float &alpha);

   TH1F *GetGigaWaveform(const std::vector<TH1F> &vClusWF);
   TH1F *GetTruncatedGigaWaveformGP1(const std::vector<TH1F> &vClusWF,
                                     const std::vector<float> &v_dEdx,
                                     const int &nClusters);

   void DiscardedModule();

private:
   // Output file variables
   TFile *fpFile_dEdx;
   TTree *fpTree_dEdx;

   // Setup variables
   static constexpr float AVG_GAIN = 1947.72; // average gain of the 32 mounted ERAMs
   static constexpr float PHIMAX = 42.10;     // std::atan(10.19/11.28)*180/M_PI;
   static constexpr float falpha = 0.7;
   static constexpr float fnParamsTrack = 3;
   static constexpr float fminLength = 0.002;
   static constexpr float XPADLENGTH = 11.28; // mm
   static constexpr float YPADLENGTH = 10.19; // mm

   // Input classes
   Event *pEvent;
   Module *pModule;
   const Track *pTrack;
   Cluster *pCluster;
   Pad *pPad;

   // Reconstruction classes
   Reconstruction::RecoEvent *p_recoevent;
   Reconstruction::RecoModule *p_recomodule;
   Reconstruction::RecoCluster *p_recocluster;
   Reconstruction::RecoPad *p_recopad;

   // ERAM mapping
   Reconstruction::ERAMMaps *pERAMMaps;
   std::vector<int> fERAMs_iD;
   std::vector<int> fERAMs_pos;

   // Diagonal variables
   bool diag;
   float costheta;
   std::vector<int> v_theta;

   // WF correction variables
   TF1 *pcorrFunctionWF;
   float fAref;

   // Fitting variables
   TF1 *ptf1PRF;
   int fcounterFit;
   int fcounterFail;

   // Analysis variables //
   // Iterators
   int NEvents;
   int iEvent;
   float fmodID;
   int NMod;
   int iMod;
   int NClusters;
   int iC;
   int NPads;
   int iP;
   // Modules
   std::vector<float> v_mod_dx;
   std::vector<float> v_mod_dE;
   std::vector<float> v_mod_dEdxXP;
   std::vector<float> v_mod_dEdxWF;
   // Events
   std::vector<float> v_evt_dx;
   std::vector<float> v_evt_dE;
   std::vector<float> v_evt_dEdxXP;
   std::vector<float> v_evt_dEdxWF;
   // Waveforms
   std::vector<float> waveform_cluster;
   std::vector<float> waveform_pad;
   // histograms
   TH1F *ph1f_WF;
   TH1F *ph1f_XP;
   TH1F *ph1f_GPL;
   TH1F *ph1f_GPC;
   TH1F *ph1f_GP1;
   TH1F *ph1f_GP2;
   TH1F *ph1f_GP3;
   TH1F *ph1f_GP4;
   TH1F *ph1f_GP5;
};
} // namespace Reconstruction

#endif