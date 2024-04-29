#include "Misc/Misc.h"

#include "SampleTools/Uploader.h"
#include "SampleTools/GiveMe_Uploader.h"
#include "EvtModelTools/JFL_Selector.h"
#include "Tristan/THATdEdx.hxx"

#include <TTree.h>


namespace PID{
    std::string outDir;
    std::string dataFile;
    std::string tag;
    std::string comment;
    std::string selectionSet;
    std::string driftMethod;
    Uploader* pUploader;
    PID::LUT* p_lut;
    int moduleCase; // DESY21:0 | CERN22:-1

    int PT;
    int TB;
    float driftDist;
    const float PHIMAX = std::atan(10.19/11.28)*180/M_PI;

    class dEdx{
        public:
            void TreeInitilization(TTree* pTree);
            void Reconstruction();
            float ComputedEdxWF(std::vector<float> v_dEdxWF, const int& nClusters);
            float ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads);

            void PadReset();
            void ClusterReset();
            void ModuleReset();
            void EventReset();


        private:
        PID::ERAMMaps *pERAMMaps = new PID::ERAMMaps();
        Event* pEvent;

        // Setup variables
        std::string foutput_log;
        std::vector<int> fERAMs_iD;
        std::vector<int> fERAMs_pos;
        static constexpr float AVG_GAIN = 1492.37; // average gain of the 32 mounted ERAMs
        bool        fgainCorrection     = false;
        bool        fWFupdated          = false;
        const float falpha              = 0.7;
        const float fnParamsTrack       = 3;
        const float fminLength          = 0.002;
        const int   fnbfERAMs           = 1;
        float costheta                  = 1;
        int theta_arr[3]                = {-45, -20, 20};
        const int fnERAMs               = 4;

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

        // Event variables
        int     feventNbr;          // TTree
        std::vector<float> v_dx;
        std::vector<float> v_dE;
        std::vector<float> v_dEdxXP;
        std::vector<float> v_dEdxWF;
        std::vector<TH1F*> v_trash;
        float   fdEdxXP;            // TTree
        float   fdEdxWF;            // TTree
        int     fNCrossedPads;      // TTree
        int     fNClusters;         // TTree
        float   flengthXP;          // TTree
        float   flengthWF;          // TTree
        int     fnumberOfModules;   // TTree

        // Module variables
        int     fModuleID;          // TTree
        int     fModulePos;         // TTree

        // Cluster variables
        TH1F   *ph1f_WF_cluster;
        float   fclusterLength;     // TTree
        int     fNPads;
        float   fratioCorr;

        // Pad variables
        TH1F* ph1f_WF_pad;
        float fAPad;                // TTree
        float fRCPad;
        float fGainPad;
        float fGainCorrection;
        float fphi;                 // TTree
        float fd;                   // TTree
        float fdd;
        float fpadLength;           // TTree
        float fTPad;                // TTree
        float T0;
        float fdriftDistance;       // TTree
        float fratioDrift;
        float fratioFile;
        float ratio;                // TTree

        TTree   *pTree = new TTree("dEdx_tree", "dEdx TTree");
    };
}