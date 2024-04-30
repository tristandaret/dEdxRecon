#ifndef LUT_H
#define LUT_H

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

namespace PID{
   
   /* ERAM MAPS READING --------------------------------------------------------------------------------------------------------------------------- */
   class ERAMMaps {
   public:
      /* Constructor */
      ERAMMaps();
      ERAMMaps(const std::string& file);
      virtual ~ERAMMaps();
      
      int    ID(const int& position);
      double RC(const int& position, const int& iX, const int& iY);
      double Gain(const int& position, const int& iX, const int& iY);
      double Resolution(const int& position, const int& iX, const int& iY);

   private:
    //   std::string fHATPath;
      std::string fFile;
      std::vector<int> fID;
      double fGain        [32][36][32]; // [channel][iX][iY]
      double fRC          [32][36][32];
      double fResolution  [32][36][32]; 

      void Load();
      void setGain       (const int& position, const int& iX, const int& iY, const double& gain) ;
      void setRC         (const int& position, const int& iX, const int& iY, const double& RC) ;
      void setResolution (const int& position, const int& iX, const int& iY, const double& resolution) ;
      void FillHoles();

      std::vector<int> channel2iD = {24, 30, 28, 19, 21, 13,  9,  2, 26, 17, 23, 29,  1, 10, 11,  3,  /*bottom HATPC*/ 
                                     47, 16, 14, 15, 42, 45, 37, 36, 20, 38,  7, 44, 43, 39, 41, 46}; /*top    HATPC*/
   

      //------------------------------Data Members-----------------------//
      TFile    *pFile ;
      TTree    *pTree ;
      TBranch  *pBranch;

      // Leaves
      int            fid;
      int            fpos;
      int            fx;
      int            fy;
      double         frc;
      double         fgain;
      double         fres;
   };



   /* Look Up Tables for XP method ---------------------------------------------------------------------------------------------------------------- */
   class LUT {
   public:
      /* Constructor */
        LUT(const int& transDiffCoeff, const int& peakingTime);
        LUT(const std::string& file);

        virtual ~LUT();
      
        float getRatio(const float& phi, const float& d, const float& RC, const float& z);

   private:
    //   std::string fhatReconRoot;
    //   std::string fhatReconConfig;
      std::string fFile;
    //   int fTransDiffCoeff   = ND::TOARuntimeParams::Get().GetParameterI("hatRecon.TransDiffCoeff");  
      int fTransDiffCoeff   = 350;  

      static constexpr float PAD_DIAG  = 15.20; // sqrt(pow(11.28,2) + pow(10.19, 2))

      static const int sN_PHI      = 150;
      static const int sN_D        = 150;
      static const int sN_RC       = 41;
      static const int sN_Z        = 21;
      
      float fValue[sN_PHI][sN_D][sN_RC][sN_Z];

      static constexpr float sSTEP_PHI   = (90-2e-6)/(sN_PHI-1);
      static constexpr float sSTEP_D     = PAD_DIAG/2/(sN_D-1);
      static constexpr float sSTEP_RC    = 200./(sN_RC-1);
      static constexpr float sSTEP_Z     = 1000./(sN_Z-1);

      void Load();

   
      //------------------------------Data Members-----------------------//

      TFile *pFile ;
      TTree *pTree ;

      // Leaves
      float         fweight;
      float         fd;
      float         fphi;
      float         fRC;
      float         fz;
   };
}

#endif