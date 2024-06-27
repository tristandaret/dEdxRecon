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

namespace Reconstruction{

    class RecoPad : public TObject{
    public:
        virtual ~RecoPad();

        // std::vector<int> v_waveform;
        int   ix             = 0;
        int   iy             = 0;
        float ADCmax         = 0;
        float charge         = 0;
        float dEdxXP         = 0;
        float RC             = 0;
        float gain           = 0;
        float GainCorrection = 0;
        float phi            = 0;
        float d              = 0;
        float dd             = 0;
        float length         = 0;
        float TMax           = 0;
        float T0             = 0;
        float driftDistance  = 0;
        float ratioDrift     = 0;
        float ratioFile      = 0;
        float ratio          = 0;

        ClassDef(RecoPad, 1);
    };

    class RecoCluster : public TObject{
    public:
        virtual ~RecoCluster();

        std::vector<RecoPad*> v_pads;
        // std::vector<int> v_waveform;
        float   length          = 0;
        float   charge          = 0;
        float   dEdxWF          = 0;
        int     NPads           = 0;
        float   ratioCorr       = 0; 

        ClassDef(RecoCluster, 1);
    };

    class RecoModule : public TObject{
    public:
        virtual ~RecoModule();
        
        std::vector<RecoCluster*> v_clusters;
        int ID                  = 0;
        int position            = 0;
        float dEdxXP            = 0;
        float dEdxWF            = 0;
        float dEdxXPnoTrunc     = 0;
        float dEdxWFnoTrunc     = 0;
        int NCrossedPads        = 0;
        int NClusters           = 0;
        float lengthXP          = 0;
        float lengthWF          = 0;
        float phi               = 0;
        TF1* Track              = new TF1("pTrackFit", "[0]*x+[1]+[2]*x*x", 0, 2000);

        ClassDef(RecoModule, 1);
    };

    class RecoEvent : public TObject{
    public:
        RecoEvent();
        virtual ~RecoEvent();
        void Clear();

        std::vector<RecoModule*> v_modules;
        int eventNbr          = 0;
        float dEdxXP          = 0;
        float dEdxWF          = 0;
        float dEdxXPnoTrunc   = 0;
        float dEdxWFnoTrunc   = 0;
        int NCrossedPads      = 0;
        int NClusters         = 0;
        float lengthXP        = 0;
        float lengthWF        = 0;
        int numberOfModules   = 0;

        ClassDef(RecoEvent, 1);
    };

    class dEdx{
        public:
            dEdx();
            virtual ~dEdx();
            void Reconstruction();
            float ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters, const float& alpha);
            float ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads, const float& alpha);


        private:
        // Reconstruction::ERAMMaps *pERAMMaps = new Reconstruction::ERAMMaps();
        // Event* pEvent;
        // std::vector<double> v_erams;

        // Setup variables
        std::string foutput_log;
        // std::vector<int> fERAMs_iD;
        // std::vector<int> fERAMs_pos;
        static constexpr float  AVG_GAIN        = 1947.72; // average gain of the 32 mounted ERAMs
        static constexpr float  PHIMAX          = 42.10; //std::atan(10.19/11.28)*180/M_PI;
        static constexpr float  falpha          = 0.7;
        static constexpr float  fnParamsTrack   = 3;
        static constexpr float  fminLength      = 0.002;
        static constexpr float  XPADLENGTH      = 11.28; // mm
        static constexpr float  YPADLENGTH      = 10.19; // mm
        static constexpr int    fnbfERAMs       = 1;
        static constexpr int    fnERAMs         = 4;

        // Method histograms
        // TH1F *ph1f_WF;
        // TH1F *ph1f_XP;
        // TF1 *ptf1_WF;
        // TF1 *ptf1_XP;
    
        // Fitting variables
        // TF1 *ptf1PRF;
        // int fcounterFit;
        // int fcounterFail;

        // Correction function variables
        // float fAref;

        // dEdx variables
        // int NC;
        // // float fmodID;
        // std::vector<float> v_dx;
        // std::vector<float> v_dE;
        // std::vector<float> v_dEdxXP;
        // std::vector<float> v_dEdxWF;

        // TFile* pFile;
        // TTree* pTree;
        Reconstruction::RecoEvent *p_recoevent;
    };

   extern Reconstruction::LUT    *p_lut;
}

#endif