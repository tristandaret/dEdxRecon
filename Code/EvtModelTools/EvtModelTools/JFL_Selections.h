#include "Misc/Misc.h"

#include "EvtModel/Sample.h"
#include "EvtModel/Event.h"
#include "EvtModel/Pad.h"

#include "TH1F.h"
#include "TH2D.h"


// All selections
void Selection_JFL_A(Sample& aSample, const int& ModuleNber); // 1+2+3+4+5
void Selection_JFL_B(Sample& aSample, const int& ModuleNber); // 1+6+5

// Stage 1: Remove clusters in first and last columns 
void Selection_JFL_Stage1(Sample& aSample, const int& ModuleNber);


// Stage 2: Remove clusters with less than 2 pads 
void Selection_JFL_Stage2(Sample& aSample, const int& ModuleNber);


// Stage 3: Remove clusters with Leading pad missing one of its immediate neighbor along Y-axis
void Selection_JFL_Stage3(Sample& aSample, const int& ModuleNber);


// Stage 4: Redo cluster keeping only the leading pad and its immediate neighbors along Y-axis
void Selection_JFL_Stage4(Sample& aSample, const int& ModuleNber);


// Stage 5: Reject Events with less than 20 clusters
void Selection_JFL_Stage5(Sample& aSample, const int& ModuleNber);


// Stage 6: Redo cluster keeping only the pads properly ordered wrt Leading pad
void Selection_JFL_Stage6(Sample& aSample, const int& ModuleNber);


//------------------------------------------------/
void Selection_JFL_BN(Sample& aSample, const int& ModuleNber); // 1+14+12+7+8+13+6+  9+10+11+5
void Selection_JFL_AN(Sample& aSample, const int& ModuleNber); // 1+14+12+7+8+13+6+4+9+10+11+5

// Stage 7: Remove Clusters with too high or too small T2-T1, T3-T1 and T3-T2
void Selection_JFL_Stage7(Sample& aSample, const int& ModuleNber);

// Stage 8: Remove Clusters with too hich A3/A1
void Selection_JFL_Stage8(Sample& aSample, const int& ModuleNber);

// Stage 9: Remove clusters with less than 1pads 
void Selection_JFL_Stage9(Sample& aSample, const int& ModuleNber);

// Stage 10: Remove clusters with more than 6 pad 
void Selection_JFL_Stage10(Sample& aSample, const int& ModuleNber);

// Stage 11: Remove clusters out of time (Event Based)
void Selection_JFL_Stage11(Sample& aSample, const int& ModuleNber);

// Stage 12: Remove clusters out of time (Run Based)
void Selection_JFL_Stage12(Sample& aSample, const int& ModuleNber);

// Stage 13: Remove Clusters with too small Tpad-TLeading 
void Selection_JFL_Stage13(Sample& aSample, const int& ModuleNber);

// Stage 14: Remove Clusters with a too high APad 
void Selection_JFL_Stage14(Sample& aSample, const int& ModuleNber);




// All cleanings
void Cleaning_JFL_01(Sample& aSample, const int& ModuleNber); // Eliminate cluster with NAN YT
void Cleaning_JFL_02(Sample& aSample, const int& ModuleNber); // Eliminate cluster failling fit
