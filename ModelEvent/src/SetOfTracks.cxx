#include "SetOfTracks.h"
#include "GaussFunction.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"

SetOfTracks::SetOfTracks()
{
   m_MinimalNberOfEntries = 100;
}

SetOfTracks::~SetOfTracks()
{
   int VerboseDelete = 0;
   if (VerboseDelete == 1)
      std::cout << " SetOfTracks Destructor " << std::endl;
   V_Track.clear();
}

void SetOfTracks::Add_Track(const Track *pTrack)
{
   V_Track.push_back(pTrack);
}

int SetOfTracks::Get_NberOfTrack() const
{
   return V_Track.size();
}

const Track *SetOfTracks::Get_Track(int Index1D) const
{
   return V_Track[Index1D];
}

void SetOfTracks::DumpRec() const
{
   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);
      pTrack->DumpRec();
   }
}

int SetOfTracks::Get_MinimalNberOfEntries()
{
   return m_MinimalNberOfEntries;
}
void SetOfTracks::Set_MinimalNberOfEntries(const int &MinimalNberOfEntries)
{
   m_MinimalNberOfEntries = MinimalNberOfEntries;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Ch2Min(const std::string &TAGFinal, const double &Xmax)
{

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << Xmax;
   std::string Name = "GiveMe_pTH1F_Ch2Min" + TAGFinal + aostringstream.str();
   std::string Title = "Track fit : Ch2Min ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, 0., Xmax);
   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);
      double Ch2Min = pTrack->Get_Chi2Min();
      ToBeReturned->Fill(Ch2Min);
   }
   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Ch2MinPerNODF(const std::string &TAGFinal, const double &Xmax)
{

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << Xmax;
   std::string Name = "GiveMe_pTH1F_Ch2MinPerNODF" + TAGFinal + aostringstream.str();
   std::string Title = "Track fit : Ch2Min per DOF ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, 0., Xmax);
   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);
      double Ch2MinPerDOF = pTrack->Get_Chi2Min() / double(pTrack->Get_NberOfCluster() - pTrack->GetNberOfParameters());
      ToBeReturned->Fill(Ch2MinPerDOF);
   }
   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Residual(const std::string &TAGFinal, double &Mean, double &eMean, double &Sigma,
                                         double &eSigma)
{

   std::string Name = "GiveMe_pTH1F_Residual" + TAGFinal;
   std::string Title = "Track fit : Residual (mm)";

   double XminH = -0.5;
   double XmaxH = 0.5;

   TH1F *ToBeReturned = 0;

   for (int iTurn = 0; iTurn < 3; iTurn++) {
      delete ToBeReturned;

      ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, XminH, XmaxH);

      int NberOfTracks = Get_NberOfTrack();
      for (int iT = 0; iT < NberOfTracks; iT++) {
         const Track *pTrack = Get_Track(iT);
         int NClusters = pTrack->Get_NberOfCluster();
         for (int iC = 0; iC < NClusters; iC++) {
            double Residual = pTrack->Get_Residual(iC) * 1.E3;
            ToBeReturned->Fill(Residual);
         }
      }

      double L_Norm = ToBeReturned->GetEntries();
      double L_Mean = ToBeReturned->GetMean();
      double L_Sigma = ToBeReturned->GetRMS();

      if (L_Norm < 1.) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Pull V1 "
         //		<< " ARGGH! Empty histo "
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      if (L_Norm < m_MinimalNberOfEntries) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Pull V1 "
         //		<< " ARGGH! too few entries ("<< L_Norm << ") "
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      Mean = L_Mean;
      eMean = L_Sigma / std::sqrt(L_Norm);
      Sigma = L_Sigma;
      eSigma = L_Sigma / std::sqrt(2. * L_Norm);

      double XminF = L_Mean - 3. * L_Sigma;
      double XmaxF = L_Mean + 3. * L_Sigma;

      GaussFunction aGaussFunction;
      TF1 *pTF1 = new TF1("GaussFunction", aGaussFunction, XminF, XmaxF, 3);

      pTF1->SetParameter(0, L_Norm * (XmaxH - XminH) / 100.);
      pTF1->SetParameter(1, L_Mean);
      pTF1->SetParameter(2, L_Sigma);

      pTF1->SetParName(0, "Norm");
      pTF1->SetParName(1, "mean");
      pTF1->SetParName(2, "sigma");

      int fitStatus = ToBeReturned->Fit("GaussFunction", "Q", "", XminF, XmaxF);
      if (fitStatus != 0) {
         std::cout << " fitStatus = " << fitStatus << " Name = " << Name << " Title = " << Title
                   << " pTF1->GetParameter(0) = " << pTF1->GetParameter(0) << " L_Norm = " << L_Norm
                   << " L_Norm Scaled = " << L_Norm * (XmaxH - XminH) / 100. << std::endl;
      }

      Mean = pTF1->GetParameter(1);
      eMean = pTF1->GetParError(1);
      Sigma = std::fabs(pTF1->GetParameter(2));
      eSigma = pTF1->GetParError(2);

      XminH = Mean - 5. * Sigma;
      XmaxH = Mean + 5. * Sigma;
   }

   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Residual(const std::string &TAGFinal, const int &iX, double &Mean, double &eMean,
                                         double &Sigma, double &eSigma)
{

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << iX;
   std::string Name = "GiveMe_pTH1F_ResidualV1" + TAGFinal + aostringstream2.str();

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "Track fit : Residual (mm) : iX " << iX;
   std::string Title = aostringstream.str();

   Mean = 0.;
   eMean = 0.;
   Sigma = 0.;
   eSigma = 0.;

   double XminH = -0.5;
   double XmaxH = 0.5;

   TH1F *ToBeReturned = 0;

   for (int iTurn = 0; iTurn < 3; iTurn++) {
      Mean = 0.;
      eMean = 0.;
      Sigma = 0.;
      eSigma = 0.;

      delete ToBeReturned;

      ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, XminH, XmaxH);

      int NberOfTracks = Get_NberOfTrack();
      for (int iT = 0; iT < NberOfTracks; iT++) {
         const Track *pTrack = Get_Track(iT);

         double Residual = 0;
         int Found = 0;
         int NberOfClusters = pTrack->Get_NberOfCluster();
         for (int iC = 0; iC < NberOfClusters; iC++) {
            const Cluster *pCluster = pTrack->Get_Cluster(iC);
            int iXCluster = (pCluster->Get_LeadingPad())->Get_iX();
            if (iXCluster == iX) {
               Residual = pTrack->Get_Residual(iC) * 1.E3;
               Found = 1;
            }
         }

         if (Found == 1) {
            ToBeReturned->Fill(Residual);
         }
      }

      double L_Norm = ToBeReturned->GetEntries();
      double L_Mean = ToBeReturned->GetMean();
      double L_Sigma = ToBeReturned->GetRMS();

      if (L_Norm < 1.) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Residual V1 "
         //		<< " ARGGH! Empty histo "
         //		<< " for iX= " << iX
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      if (L_Norm < m_MinimalNberOfEntries) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Residual V1 "
         //		<< " ARGGH! too few entries ("<< L_Norm << ") "
         //		<< " for iX= " << iX
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      Mean = L_Mean;
      eMean = L_Sigma / std::sqrt(L_Norm);
      Sigma = L_Sigma;
      eSigma = L_Sigma / std::sqrt(2. * L_Norm);

      double XminF = L_Mean - 3. * L_Sigma;
      double XmaxF = L_Mean + 3. * L_Sigma;

      GaussFunction aGaussFunction;
      TF1 *pTF1 = new TF1("GaussFunction", aGaussFunction, XminF, XmaxF, 3);

      pTF1->SetParameter(0, L_Norm * (XmaxH - XminH) / 100.);
      pTF1->SetParameter(1, L_Mean);
      pTF1->SetParameter(2, L_Sigma);

      pTF1->SetParName(0, "Norm");
      pTF1->SetParName(1, "mean");
      pTF1->SetParName(2, "sigma");

      int fitStatus = ToBeReturned->Fit("GaussFunction", "Q", "", XminF, XmaxF);
      if (fitStatus != 0) {
         std::cout << " fitStatus = " << fitStatus << " Name = " << Name << " Title = " << Title
                   << " pTF1->GetParameter(0) = " << pTF1->GetParameter(0) << " L_Norm = " << L_Norm
                   << " L_Norm Scaled = " << L_Norm * (XmaxH - XminH) / 100. << std::endl;
      }

      Mean = pTF1->GetParameter(1);
      eMean = pTF1->GetParError(1);
      Sigma = std::fabs(pTF1->GetParameter(2));
      eSigma = pTF1->GetParError(2);

      XminH = Mean - 5. * Sigma;
      XmaxH = Mean + 5. * Sigma;
   }

   return ToBeReturned;
}

TH1F *
SetOfTracks::GiveMe_pTH1F_Pull(const std::string &TAGFinal, double &Mean, double &eMean, double &Sigma, double &eSigma)
{
   return GiveMe_pTH1F_Pull(TAGFinal, Mean, eMean, Sigma, eSigma, -5., 5.);
}
TH1F *SetOfTracks::GiveMe_pTH1F_Pull(const std::string &TAGFinal, double &Mean, double &eMean, double &Sigma,
                                     double &eSigma, const double &XminH, const double &XmaxH)
{

   std::string Name = "GiveMe_pTH1F_Pull" + TAGFinal;
   std::string Title = "Track fit : Pull ";

   Mean = 0.;
   eMean = 0.;
   Sigma = 0.;
   eSigma = 0.;

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, XminH, XmaxH);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);
      int NClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NClusters; iC++) {
         double Pull = pTrack->Get_Pull(iC);
         ToBeReturned->Fill(Pull);
      }
   }

   double L_Norm = ToBeReturned->GetEntries();
   double L_Mean = ToBeReturned->GetMean();
   double L_Sigma = ToBeReturned->GetRMS();

   if (L_Norm < 1.) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Pull V3 "
      //		<< " ARGGH! Empty histo "
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      delete ToBeReturned;
      return 0;
   }

   if (L_Norm < m_MinimalNberOfEntries) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Pull V3 "
      //		<< " ARGGH! too few entries ("<< L_Norm << ") "
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      delete ToBeReturned;
      return 0;
   }

   Mean = L_Mean;
   eMean = L_Sigma / std::sqrt(L_Norm);
   Sigma = L_Sigma;
   eSigma = L_Sigma / std::sqrt(2. * L_Norm);

   double XminF = -3. * L_Sigma;
   double XmaxF = 3. * L_Sigma;

   GaussFunction aGaussFunction;
   TF1 *pTF1 = new TF1("GaussFunction", aGaussFunction, XminF, XmaxF, 3);

   pTF1->SetParameter(0, L_Norm * (XmaxH - XminH) / 100.);
   pTF1->SetParameter(1, L_Mean);
   pTF1->SetParameter(2, L_Sigma);

   pTF1->SetParName(0, "Norm");
   pTF1->SetParName(1, "mean");
   pTF1->SetParName(2, "sigma");

   int fitStatus = ToBeReturned->Fit("GaussFunction", "Q", "", XminF, XmaxF);
   if (fitStatus != 0) {
      std::cout << " fitStatus = " << fitStatus << " Name = " << Name << " Title = " << Title
                << " pTF1->GetParameter(0) = " << pTF1->GetParameter(0) << " L_Norm = " << L_Norm
                << " L_Norm Scaled = " << L_Norm * (XmaxH - XminH) / 100. << std::endl;
   }

   Mean = pTF1->GetParameter(1);
   eMean = pTF1->GetParError(1);
   Sigma = std::fabs(pTF1->GetParameter(2));
   eSigma = pTF1->GetParError(2);

   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Pull(const std::string &TAGFinal)
{

   std::string Name = "GiveMe_pTH1F_Pull" + TAGFinal;
   std::string Title = "Track fit : Pull ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, -5., 5.);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);
      int NClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NClusters; iC++) {
         double Pull = pTrack->Get_Pull(iC);
         ToBeReturned->Fill(Pull);
      }
   }

   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Pull(const std::string &TAGFinal, const int &iX, double &Mean, double &eMean,
                                     double &Sigma, double &eSigma)
{
   return GiveMe_pTH1F_Pull(TAGFinal, iX, Mean, eMean, Sigma, eSigma, -5., 5.);
}
TH1F *SetOfTracks::GiveMe_pTH1F_Pull(const std::string &TAGFinal, const int &iX, double &Mean, double &eMean,
                                     double &Sigma, double &eSigma, const double &XminH, const double &XmaxH)
{

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << iX;
   std::string Name = "GiveMe_pTH1F_Pull" + TAGFinal + aostringstream2.str();

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "Track fit Pull: iX " << iX;
   std::string Title = aostringstream.str();

   Mean = 0.;
   eMean = 0.;
   Sigma = 0.;
   eSigma = 0.;

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 100, XminH, XmaxH);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      double Pull = 0;
      int Found = 0;
      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);
         int iXCluster = (pCluster->Get_LeadingPad())->Get_iX();
         if (iXCluster == iX) {
            Pull = pTrack->Get_Pull(iC);
            Found = 1;
         }
      }

      if (Found == 1) {
         ToBeReturned->Fill(Pull);
      }
   }

   double L_Norm = ToBeReturned->GetEntries();
   double L_Mean = ToBeReturned->GetMean();
   double L_Sigma = ToBeReturned->GetRMS();

   if (L_Norm < 1.) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Pull V1 "
      //		<< " ARGGH! Empty histo for iX= " << iX
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      delete ToBeReturned;
      return 0;
   }

   if (L_Norm < m_MinimalNberOfEntries) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Pull V1 "
      //		<< " ARGGH! too few entries ("<< L_Norm << ") "
      //		<< " for iX= " << iX
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      delete ToBeReturned;
      return 0;
   }

   Mean = L_Mean;
   eMean = L_Sigma / std::sqrt(L_Norm);
   Sigma = L_Sigma;
   eSigma = L_Sigma / std::sqrt(2. * L_Norm);

   double XminF = -3. * L_Sigma;
   double XmaxF = 3. * L_Sigma;

   GaussFunction aGaussFunction;
   TF1 *pTF1 = new TF1("GaussFunction", aGaussFunction, XminF, XmaxF, 3);

   pTF1->SetParameter(0, L_Norm * (XmaxH - XminH) / 100.);
   pTF1->SetParameter(1, L_Mean);
   pTF1->SetParameter(2, L_Sigma);

   pTF1->SetParName(0, "Norm");
   pTF1->SetParName(1, "mean");
   pTF1->SetParName(2, "sigma");

   int fitStatus = ToBeReturned->Fit("GaussFunction", "Q", "", XminF, XmaxF);
   if (fitStatus != 0) {
      std::cout << " fitStatus = " << fitStatus << " Name = " << Name << " Title = " << Title
                << " pTF1->GetParameter(0) = " << pTF1->GetParameter(0) << " L_Norm = " << L_Norm
                << " L_Norm Scaled = " << L_Norm * (XmaxH - XminH) / 100. << std::endl;
   }

   Mean = pTF1->GetParameter(1);
   eMean = pTF1->GetParError(1);
   Sigma = std::fabs(pTF1->GetParameter(2));
   eSigma = pTF1->GetParError(2);

   return ToBeReturned;
}

TH1F *SetOfTracks::GiveMe_pTH1F_Residual(const std::string &TAGFinal, const int &iX, double YL, double YH,
                                         double YL_Rescaled, double YH_Rescaled, double &Mean, double &eMean,
                                         double &Sigma, double &eSigma)
{

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << iX;
   aostringstream2 << YL;
   aostringstream2 << YH;
   std::string Name = "GiveMe_pTH1F_ResidualV2" + TAGFinal + aostringstream2.str();

   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "Track fit : Residual (mm) : iX " << iX;
   aostringstream << "- Range " << std::setw(8) << std::setprecision(6) << YL_Rescaled;
   aostringstream << "-> " << std::setw(8) << std::setprecision(6) << YH_Rescaled;
   std::string Title = aostringstream.str();

   Mean = 0.;
   eMean = 0.;
   Sigma = 0.;
   eSigma = 0.;

   std::vector<double> V_Data;
   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);
         int iXCluster = (pCluster->Get_LeadingPad())->Get_iX();
         if (iXCluster == iX) {
            double Position = pCluster->Get_YTrack();
            if (YL < Position && Position <= YH) {
               double Residual = pTrack->Get_Residual(iC) * 1.E3;
               if (std::fabs(Residual) < 1.5)
                  V_Data.push_back(Residual);
            }
         }
      }
   }

   int NberOfData = V_Data.size();

   if (NberOfData < 1.) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Residual V2 "
      //		<< " ARGGH! Empty histo "
      //		<< " for iX= " << iX
      //		<< " ,	YL="	<< YL
      //		<< " and	YYHL=" << YH
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      return 0;
   }

   if (NberOfData < m_MinimalNberOfEntries) {
      //	std::cout
      //		<< " SetOfTracks::GiveMe_pTH1F_Residual V2 "
      //		<< " ARGGH! too few entries ("<< NberOfData << ") "
      //		<< " for iX= " << iX
      //		<< " ,	YL="	<< YL
      //		<< " and	YYHL=" << YH
      //		<< " Name = "		<< Name
      //		<< " Title = "	<< Title
      //		<< std::endl;
      return 0;
   }

   double Mean_Data = 0.;
   double RMS_Data = 0.;
   for (int iData = 0; iData < NberOfData; iData++) {
      Mean_Data += V_Data[iData];
      RMS_Data += V_Data[iData] * V_Data[iData];
   }
   Mean_Data = Mean_Data / double(NberOfData);
   RMS_Data = RMS_Data / double(NberOfData);
   RMS_Data = RMS_Data - Mean_Data * Mean_Data;
   RMS_Data = std::sqrt(RMS_Data);

   double XminH = Mean_Data - 5. * RMS_Data;
   double XmaxH = Mean_Data + 5. * RMS_Data;
   XminH = -5. * RMS_Data;
   XmaxH = 5. * RMS_Data;

   TH1F *ToBeReturned = 0;

   for (int iTurn = 0; iTurn < 3; iTurn++) {
      delete ToBeReturned;

      ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 30, XminH, XmaxH);

      for (int iData = 0; iData < NberOfData; iData++) {
         ToBeReturned->Fill(V_Data[iData]);
      }

      double L_Norm = ToBeReturned->GetEntries();
      double L_Mean = ToBeReturned->GetMean();
      double L_Sigma = ToBeReturned->GetRMS();

      if (L_Norm < 1.) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Residual V3 "
         //		<< " ARGGH! Empty histo "
         //		<< " for iX= " << iX
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      if (L_Norm < m_MinimalNberOfEntries) {
         //		std::cout
         //		<< " SetOfTracks::GiveMe_pTH1F_Residual V3 "
         //		<< " ARGGH! too few entries ("<< L_Norm << ") "
         //		<< " for iX= " << iX
         //		<< " Name = "		<< Name
         //		<< " Title = "	<< Title
         //		<< std::endl;
         delete ToBeReturned;
         return 0;
      }

      Mean = L_Mean;
      eMean = L_Sigma / std::sqrt(L_Norm);
      Sigma = L_Sigma;
      eSigma = L_Sigma / std::sqrt(2. * L_Norm);

      double XminF = L_Mean - 3. * L_Sigma;
      double XmaxF = L_Mean + 3. * L_Sigma;
      XminF = -3. * L_Sigma;
      XmaxF = 3. * L_Sigma;

      GaussFunction aGaussFunction;
      TF1 *pTF1 = new TF1("GaussFunction", aGaussFunction, XminF, XmaxF, 3);

      pTF1->SetParameter(0, L_Norm * (XmaxH - XminH) / 100.);
      pTF1->SetParameter(1, L_Mean);
      pTF1->SetParameter(2, L_Sigma);

      pTF1->SetParName(0, "Norm");
      pTF1->SetParName(1, "mean");
      pTF1->SetParName(2, "sigma");

      pTF1->SetParLimits(2, 15.E-3, 10);

      int fitStatus = ToBeReturned->Fit("GaussFunction", "Q", "", XminF, XmaxF);
      if (fitStatus != 0) {
         std::cout << " fitStatus = " << fitStatus << " Name = " << Name << " Title = " << Title
                   << " L_Norm = " << L_Norm << " iTurn = " << iTurn
                   << " pTF1->GetParameter(0) = " << pTF1->GetParameter(0) << " L_Norm = " << L_Norm
                   << " L_Norm Scaled = " << L_Norm * (XmaxH - XminH) / 100. << std::endl;
      } else {
         Mean = pTF1->GetParameter(1);
         eMean = pTF1->GetParError(1);
         Sigma = std::fabs(pTF1->GetParameter(2));
         eSigma = pTF1->GetParError(2);
      }

      double XminH_New = Mean - 5. * Sigma;
      double XmaxH_New = Mean + 5. * Sigma;
      XminH_New = -5. * Sigma;
      XmaxH_New = 5. * Sigma;
      if (XmaxH_New > 1.5) {
         XminH_New = -1.5;
         XmaxH_New = 1.5;
      }

      XminH = XminH_New;
      XmaxH = XmaxH_New;
   }

   return ToBeReturned;
}

//-----------------------------------------------------------------//
TH1F *SetOfTracks::GiveMe_pTH1F_TrackDeltaT(const std::string &TAG, const int &iCol)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "GiveMe_pTH1F_TrackPosition" << TAG << "COL" << iCol;

   std::string Name = aostringstream.str();

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << " #Delta_{T} (cm) at col. " << iCol;

   std::string Title = aostringstream2.str();
   if (iCol == -1)
      Title = " #Delta_{T} (cm) all col. ";

   const Track *pTrack = Get_Track(0);
   const Cluster *pCluster = pTrack->Get_Cluster(0);
   Pad *pPad = pCluster->Get_LeadingPad();
   const Model_ReadOutGeometry *pModel_ReadOutGeometry = pPad->Get_Model_ReadOutGeometry();

   double NX = pModel_ReadOutGeometry->Get_Nx();
   double LY = pModel_ReadOutGeometry->Get_LY();
   double Ypad_min = pModel_ReadOutGeometry->Get_Ypad_min();

   //	std::cout << " X0 " << X0 << std::endl;
   //	std::cout << " LY " << LY << std::endl;

   double Ymax = LY / 2.;
   double Ymin = -LY / 2.;
   double DiffY = Ymax - Ymin;
   Ymax = Ymax + 0.1 * DiffY;
   Ymin = Ymin - 0.1 * DiffY;
   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), 50, Ymin * 1.E2, Ymax * 1.E2);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      for (int iX = 0; iX < NX; iX++) {
         if (iX != iCol && iCol != -1)
            continue;
         double X0 = pModel_ReadOutGeometry->Get_XcPad(iX, 0, pTrack->Get_ModuleNber());
         double YTtrack = pTrack->Y_Position(X0);

         int iY0 = std::floor((YTtrack - Ypad_min) / LY);
         double Y0 = pModel_ReadOutGeometry->Get_YcPad(iX, iY0, pTrack->Get_ModuleNber());

         double TheDiff = YTtrack - Y0;
         ToBeReturned->Fill(TheDiff * 1.E2);
      }
   }
   return ToBeReturned;
}
//-----------------------------------------------------------------//

//-----------------------------------------------------------------//
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackYPadInTracks(const std::string &TAG, const int &iCol)
{
   std::string SubTAG = TAG + "Indirect";
   return GiveMe_pTH1F_YTrackYPadInTracks(SubTAG, iCol, 100, -3., 3.);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackYPadInTracks(const std::string &TAG, const int &iCol, const int &Nbins,
                                                   const double &Xmin, const double &Xmax)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "GiveMe_pTH1F_YTrackYPadInTracks" << TAG << "COL" << iCol;

   std::string Name = aostringstream.str();

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << " Y^{Cluster}_{Track}-Y_{Pad} (cm) at col. " << iCol;

   std::string Title = aostringstream2.str();
   if (iCol == -1)
      Title = " Y^{Cluster}_{Track}-Y_{Pad} (cm) all col. ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), Nbins, Xmin, Xmax);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);

         Pad *pPad_Leading = pCluster->Get_LeadingPad();
         int iXCluster = pPad_Leading->Get_iX();
         if (iXCluster != iCol && iCol != -1)
            continue;

         double YTrack = pCluster->Get_YTrack();

         int NPads = pCluster->Get_NberOfPads();
         for (int iP = 0; iP < NPads; iP++) {
            Pad *pPad = pCluster->Get_Pad(iP);

            double YTrackYPad = YTrack - pPad->Get_YPad();
            ToBeReturned->Fill(YTrackYPad * 1.E2);
         }
      }
   }

   return ToBeReturned;
}
//-----------------------------------------------------------------//

//-----------------------------------------------------------------//
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackYPadLeadingInTracks(const std::string &TAG, const int &iCol)
{
   std::string SubTAG = TAG + "Indirect";
   return GiveMe_pTH1F_YTrackYPadLeadingInTracks(SubTAG, iCol, 100, -3., 3.);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackYPadLeadingInTracks(const std::string &TAG, const int &iCol, const int &Nbins,
                                                          const double &Xmin, const double &Xmax)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "GiveMe_pTH1F_YTrackYPadLeadingInTracks" << TAG << "COL" << iCol;

   std::string Name = aostringstream.str();

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << " Y^{Cluster}_{Track}-Y_{Pad}^{Leading} (cm) at col. " << iCol;

   std::string Title = aostringstream2.str();
   if (iCol == -1)
      Title = " Y^{Cluster}_{Track}-Y_{Pad}^{Leading} (cm) all col. ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), Nbins, Xmin, Xmax);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);

         Pad *pPad_Leading = pCluster->Get_LeadingPad();
         int iXCluster = pPad_Leading->Get_iX();
         if (iXCluster != iCol && iCol != -1)
            continue;

         double YTrack = pCluster->Get_YTrack();
         double YLeading = pCluster->Get_YLeading();
         double YTrackYPadLeading = YTrack - YLeading;

         ToBeReturned->Fill(YTrackYPadLeading * 1.E2);
      }
   }

   return ToBeReturned;
}
//-----------------------------------------------------------------//

//-----------------------------------------------------------------//
TH1F *SetOfTracks::GiveMe_pTH1F_YFitCol(const std::string &TAG, const int &iYBeam)
{
   std::string SubTAG = TAG + "Indirect1";
   return GiveMe_pTH1F_YFitCol(SubTAG, iYBeam, -1);
}
TH1F *
SetOfTracks::GiveMe_pTH1F_YFitCol(const std::string &TAG, const int &Nbins, const double &Ymin, const double &Ymax)
{
   std::string SubTAG = TAG + "Indirect2";
   return GiveMe_pTH1F_YFitCol(SubTAG, Nbins, Ymin, Ymax, -1);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YFitCol(const std::string &TAG, const int &iYBeam, const int &iCol)
{
   const Track *pTrack = Get_Track(0);
   if (pTrack == 0)
      return 0;
   const Cluster *pCluster = pTrack->Get_Cluster(0);
   Pad *pPad = pCluster->Get_LeadingPad();
   const Model_ReadOutGeometry *pModel_ReadOutGeometry = pPad->Get_Model_ReadOutGeometry();

   double Ycen = 1.E2 * pModel_ReadOutGeometry->Get_YcPad(0, iYBeam, pTrack->Get_ModuleNber());
   double Ymin = Ycen - 5.;
   double Ymax = Ycen + 5.;

   std::string SubTAG = TAG + "Indirect3";

   return GiveMe_pTH1F_YFitCol(SubTAG, 100, Ymin, Ymax, iCol);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YFitCol(const std::string &TAG, const int &Nbins, const double &Ymin,
                                        const double &Ymax, const int &iCol)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "GiveMe_pTH1F_YFitCol" << TAG << "COL" << iCol;

   std::string Name = aostringstream.str();

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << " Track Y_{T} (cm) in col. " << iCol;

   std::string Title = aostringstream2.str();
   if (iCol == -1)
      Title = " Track Y_{T} (cm) all col. ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), Nbins, Ymin, Ymax);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);

         Pad *pPad_Leading = pCluster->Get_LeadingPad();
         int iXCluster = pPad_Leading->Get_iX();
         if (iXCluster != iCol && iCol != -1)
            continue;

         double X0 = pPad_Leading->Get_XPad();
         double YTtrack = pTrack->Y_Position(X0);
         ToBeReturned->Fill(YTtrack * 1.E2);
      }
   }
   return ToBeReturned;
}
//-----------------------------------------------------------------//

//-----------------------------------------------------------------//
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackInTracks(const std::string &TAG, const int &iYBeam)
{
   std::string SubTAG = TAG + "Indirect1";
   return GiveMe_pTH1F_YTrackInTracks(SubTAG, iYBeam, -1);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackInTracks(const std::string &TAG, const int &Nbins, const double &Ymin,
                                               const double &Ymax)
{
   std::string SubTAG = TAG + "Indirect2";
   return GiveMe_pTH1F_YTrackInTracks(SubTAG, Nbins, Ymin, Ymax, -1);
}

TH1F *SetOfTracks::GiveMe_pTH1F_YTrackInTracks(const std::string &TAG, const int &iYBeam, const int &iCol)
{
   const Track *pTrack = Get_Track(0);
   if (pTrack == 0)
      return 0;
   const Cluster *pCluster = pTrack->Get_Cluster(0);
   Pad *pPad = pCluster->Get_LeadingPad();
   const Model_ReadOutGeometry *pModel_ReadOutGeometry = pPad->Get_Model_ReadOutGeometry();

   double Ycen = 1.E2 * pModel_ReadOutGeometry->Get_YcPad(0, iYBeam, pTrack->Get_ModuleNber());
   double Ymin = Ycen - 5.;
   double Ymax = Ycen + 5.;

   std::string SubTAG = TAG + "Indirect3";
   return GiveMe_pTH1F_YTrackInTracks(SubTAG, 100, Ymin, Ymax, iCol);
}
TH1F *SetOfTracks::GiveMe_pTH1F_YTrackInTracks(const std::string &TAG, const int &Nbins, const double &Ymin,
                                               const double &Ymax, const int &iCol)
{
   std::ostringstream aostringstream;
   aostringstream << std::setiosflags(std::ios::fixed);
   aostringstream << "GiveMe_pTH1F_YTrackInTracks" << TAG << "COL" << iCol;

   std::string Name = aostringstream.str();

   std::ostringstream aostringstream2;
   aostringstream2 << std::setiosflags(std::ios::fixed);
   aostringstream2 << " Cluster Y_{T} (cm) at col. " << iCol;

   std::string Title = aostringstream2.str();
   if (iCol == -1)
      Title = " Cluster Y_{T} (cm) all col. ";

   TH1F *ToBeReturned = new TH1F(Name.c_str(), Title.c_str(), Nbins, Ymin, Ymax);

   int NberOfTracks = Get_NberOfTrack();
   for (int iT = 0; iT < NberOfTracks; iT++) {
      const Track *pTrack = Get_Track(iT);

      int NberOfClusters = pTrack->Get_NberOfCluster();
      for (int iC = 0; iC < NberOfClusters; iC++) {
         const Cluster *pCluster = pTrack->Get_Cluster(iC);

         Pad *pPad_Leading = pCluster->Get_LeadingPad();
         int iXCluster = pPad_Leading->Get_iX();
         if (iXCluster != iCol && iCol != -1)
            continue;

         double YTrack = pCluster->Get_YTrack();
         ToBeReturned->Fill(YTrack * 1.E2);
      }
   }
   return ToBeReturned;
}
//-----------------------------------------------------------------//
