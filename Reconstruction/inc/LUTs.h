#/**
# * @file LUTs.h
# * @brief Lookup table utilities used by reconstruction algorithms.
# *
# * Declares data structures and helpers for building and accessing LUTs
# * (geometry, calibration, etc.) used by dE/dx and track/cluster
# * reconstruction.
# *
# * @details
# * The implementation file LUTs.cxx constructs ERAM maps, fills lookup tables
# * and exposes accessors used across the reconstruction pipeline. The source
# * also contains helpers to fill and repair maps (FillHoles), and code to
# * compute mean gains/RC values used in calibration steps.
# */
#ifndef LUT_H
#define LUT_H

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

class EramInfo : public TObject {
public:
  int Id;
  int Position;
  int XX;
  int YY;
  float RC;
  float Gain;
  float Resolution;

  int Endplate;
  bool InbTPC;
  bool IntTPC;

  EramInfo() {}
  virtual ~EramInfo() {}

  ClassDef(EramInfo, 2)
};

namespace Reconstruction {

/* ERAM MAPS READING
 * ---------------------------------------------------------------------------------------------------------------------------
 */
class ERAMMaps {
public:
  /* Constructor */
  ERAMMaps(
      const std::string &file = "$HOME/Documents/Code/ERAMinfo/ERAMinfo.root");
  virtual ~ERAMMaps();

  int ID(const int &position);
  float RC(const int &position, const int &iX, const int &iY);
  float Gain(const int &position, const int &iX, const int &iY);
  float Resolution(const int &position, const int &iX, const int &iY);
  float MeanGain(const int &position);
  float MeanRC(const int &position);

private:
  bool verbose = false;
  std::string fFile;
  std::vector<int> fID;
  std::vector<float> v_sides;

  float fGain[34][36][32]; // [position][iX][iY]
  float fRC[34][36][32];
  float fResolution[34][36][32];
  std::vector<float> fmean_gain;
  std::vector<float> fmean_RC;

  void Load();
  void setGain(const int &position, const int &iX, const int &iY,
               const float &gain);
  void setRC(const int &position, const int &iX, const int &iY,
             const float &RC);
  void setResolution(const int &position, const int &iX, const int &iY,
                     const float &resolution);
  void setMeanGain(const int &position, const float &meanGain);
  void setMeanRC(const int &position, const float &meanRC);
  void FillHoles();

  std::vector<int> channel2iD = {24, 30, 28, 19, 21, 13, 9,  2,  26,
                                 17, 23, 29, 1,  10, 11, 3, /*bottom HATPC*/
                                 47, 16, 14, 15, 42, 45, 37, 36, 20,
                                 38, 7,  44, 43, 39, 41, 46, /*top	HATPC*/
                                 12, 18}; // CERN22 MockUp and prototype

  //------------------------------Data Members-----------------------//
  TFile *pFile;
  TTree *pTree;
  TBranch *pBranch;

  // Leaves
  int fid;
  int fpos;
  int fx;
  int fy;
  float frc;
  float fgain;
  float fres;
};

/* Look Up Tables for XP method
 * ----------------------------------------------------------------------------------------------------------------
 */
class LUT {
public:
  /* Constructor */
  LUT(const int &transDiffCoeffB, const int &transDiffCoeffnoB,
      const int &peakingTime);
  LUT(const std::string &file);

  virtual ~LUT();

  float getRatio(const int &Dt, const int &RC, const float &drift,
                 const float &impact, const float &angle);

private:
  std::string fFile_LUT;

  static constexpr float PAD_DIAG = 15.20; // sqrt(pow(11.28,2) + pow(10.19, 2))

  static constexpr int RCmin = 80;
  static constexpr int RCmax = 160;

  // Number of discrete steps in each dimension of the Look Up Table
  static const int SNSTEPS_TRANS = 2;
  static const int SNSTEPS_RC = 17;
  static const int SNSTEPS_DRIFT = 21;
  static const int SNSTEPS_IMPACT = 250;
  static const int SNSTEPS_PHI = 250;

  static float LUTValues[SNSTEPS_TRANS][SNSTEPS_RC][SNSTEPS_DRIFT]
                        [SNSTEPS_IMPACT][SNSTEPS_PHI];

  float stepSizeTrans = 37; // 286 -> 323 or 310->350
  static constexpr float sSTEP_RC = 5;
  static constexpr float sSTEP_PHI = 90. / (SNSTEPS_PHI - 1);
  static constexpr float sSTEP_IMPACT = (PAD_DIAG / 2) / (SNSTEPS_IMPACT - 1);
  static constexpr float sSTEP_DRIFT = 1000. / (SNSTEPS_DRIFT - 1);

  void Load();

  //------------------------------Data Members-----------------------//

  TFile *pFile_LUT;
  TTree *pTree_LUT;

  // Branches
  float fweight;
  float fDt;
  float fRC;
  float fdrift;
  double fd;
  double fphi;

  int DtwithB;
  int DtwithoutB;
};

} // namespace Reconstruction

#endif