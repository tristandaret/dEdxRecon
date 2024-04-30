#ifndef DEDX_H
#define DEDX_H

#include "Misc/Misc.h"

#include "PID/LUTs.hxx"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1F.h>

#include "SampleTools/Uploader.h"



namespace PID{

    class TPad {
    public:
        TH1F* ph1f_WF_pad    = nullptr;
        int   ix             = -999;
        int   iy             = -999;
        float ADC            = -999;
        float RC             = -999;
        float gain           = -999;
        float GainCorrection = -999;
        float phi            = -999;
        float d              = -999;
        float dd             = -999;
        float length         = -999;
        float TMax           = -999;
        float T0             = -999;
        float driftDistance  = -999;
        float ratioDrift     = -999;
        float ratioFile      = -999;
        float ratio          = -999;
    };

    class TCluster {
    public:
        std::vector<TPad> v_pads;
        TH1F   *ph1f_WF_cluster = new TH1F("ph1f_WF_cluster", "ph1f_WF_cluster", 510, -0.5, 509.5);
        float   length          = -999;
        int     NPads           = -999;
        float   ratioCorr       = -999; 
    };

    class TModule {
    public:
        std::vector<TCluster> v_clusters;
        int ID                  = -999;
        int position            = -999;
        int NClusters           = -999;
        float phi               = -999;
        TF1* Track              = new TF1("pTrackFit", "[0]*x+[1]+[2]*x*x", 0, 2000);
    };

    class TEvent {
    public:
        std::vector<TModule> v_modules;
        int eventNbr          = -999;
        float dEdxXP          = -999;
        float dEdxWF          = -999;
        int NCrossedPads      = -999;
        int NClusters         = -999;
        float lengthXP        = -999;
        float lengthWF        = -999;
        int numberOfModules   = -999;
    };

    class dEdx{
        public:
            void Reconstruction();
            float ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters);
            float ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads);


        private:
        PID::ERAMMaps *pERAMMaps = new PID::ERAMMaps();
        Event* pEvent;
        std::vector<double> v_erams;

        // Setup variables
        std::string foutput_log;
        std::vector<int> fERAMs_iD;
        std::vector<int> fERAMs_pos;
        static constexpr float AVG_GAIN = 1492.37; // average gain of the 32 mounted ERAMs
        bool        fgainCorrection     = false;
        const float falpha              = 0.7;
        const float fnParamsTrack       = 3;
        const float fminLength          = 0.002;
        const int   fnbfERAMs           = 1;
        float costheta                  = 1;
        int theta_arr[3]                = {-45, -20, 20};
        const int fnERAMs               = 4;
        const float PHIMAX              = std::atan(10.19/11.28)*180/M_PI;

        // Method histograms
        TH1F *ph1f_WF              = new TH1F("ph1f_WF", "<dE/dx> with WF_{sum};<dE/dx> (ADC count);Number of events", 90, 0, 1800);
        TH1F *ph1f_XP              = new TH1F("ph1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 90, 0, 1800);
        TF1 *ptf1_WF;
        TF1 *ptf1_XP;
    
        // Fitting variables
        TF1 *ptf1PRF;
        int fcounterFit;
        int fcounterFail;

        // Correction function variables
        TF1 *pcorrFunctionWF;
        float fAref;

        // dEdx variables
        int NC;
        float fmodID;
        std::vector<float> v_dx;
        std::vector<float> v_dE;
        std::vector<float> v_dEdxXP;
        std::vector<float> v_dEdxWF;

        TFile* pFile;
        TTree* pTree;
        TEvent tevent;
    };
}

#endif