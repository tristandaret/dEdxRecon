#include "FitOutput.h"

#include "TLatex.h"
#include "TMatrixD.h"

FitOutput::FitOutput()
{
   m_NberOfModelParameters = 0;

   p_par = 0;
   p_parName = 0;
   p_eparplus = 0;
   p_eparminus = 0;
   p_eparparab = 0;
   p_CovMatrix = 0;

   m_Description = "No Description";
}
FitOutput::~FitOutput()
{
   int VerboseDelete = 0;
   if (VerboseDelete == 1)
      std::cout << " FitOutput Destructor " << std::endl;
   Reset();
}

void FitOutput::Reset()
{
   delete[] p_par;
   delete[] p_parName;
   delete[] p_eparplus;
   delete[] p_eparminus;
   delete[] p_eparparab;
   delete[] p_CovMatrix;
   p_par = 0;
   p_parName = 0;
   p_eparplus = 0;
   p_eparminus = 0;
   p_eparparab = 0;
   p_CovMatrix = 0;
}

void FitOutput::Set(const int &NberOfModelParameters)
{
   m_NberOfModelParameters = NberOfModelParameters;

   //	std::cout << "FitOutput::Set: "
   //			<< " m_NberOfModelParameters " << m_NberOfModelParameters << std::endl;

   int NdimSquared = m_NberOfModelParameters * m_NberOfModelParameters;

   p_par = new double[m_NberOfModelParameters];
   p_parName = new std::string[m_NberOfModelParameters];
   p_eparplus = new double[m_NberOfModelParameters];
   p_eparminus = new double[m_NberOfModelParameters];
   p_eparparab = new double[m_NberOfModelParameters];
   p_CovMatrix = new double[NdimSquared];
}

FitOutput::FitOutput(const FitOutput &ToBeCopied)
{

   m_NberOfModelParameters = ToBeCopied.m_NberOfModelParameters;
   int NdimSquared = m_NberOfModelParameters * m_NberOfModelParameters;

   p_par = new double[m_NberOfModelParameters];
   p_parName = new std::string[m_NberOfModelParameters];
   p_eparplus = new double[m_NberOfModelParameters];
   p_eparminus = new double[m_NberOfModelParameters];
   p_eparparab = new double[m_NberOfModelParameters];
   p_CovMatrix = new double[NdimSquared];

   m_Description = ToBeCopied.m_Description;

   for (int Item = 0; Item < m_NberOfModelParameters; Item++) {
      p_par[Item] = ToBeCopied.p_par[Item];
      p_parName[Item] = ToBeCopied.p_parName[Item];
      p_eparplus[Item] = ToBeCopied.p_eparplus[Item];
      p_eparminus[Item] = ToBeCopied.p_eparminus[Item];
      p_eparparab[Item] = ToBeCopied.p_eparparab[Item];
   }

   if (ToBeCopied.p_CovMatrix) {
      for (int Item = 0; Item < NdimSquared; Item++)
         p_CovMatrix[Item] = ToBeCopied.p_CovMatrix[Item];
   } else {
      for (int Item = 0; Item < NdimSquared; Item++)
         p_CovMatrix[Item] = 0.;
      std::cout << "===> FitOutput::FitOutput:	"
                << "No covarance matrix available " << std::endl;
   }

   m_MinnLL = ToBeCopied.m_MinnLL;
   m_NameOfTheModel = ToBeCopied.m_NameOfTheModel;
   m_NberOfDataPoints = ToBeCopied.m_NberOfDataPoints;
}
FitOutput &FitOutput::operator=(const FitOutput &ToBeCopied)
{
   if (&ToBeCopied != this) {

      if (m_NberOfModelParameters != ToBeCopied.m_NberOfModelParameters) {
         m_NberOfModelParameters = ToBeCopied.m_NberOfModelParameters;

         int NdimSquared = m_NberOfModelParameters * m_NberOfModelParameters;

         delete p_par;
         delete p_parName;
         delete p_eparplus;
         delete p_eparminus;
         delete p_eparparab;
         delete p_CovMatrix;

         p_par = new double[m_NberOfModelParameters];
         p_parName = new std::string[m_NberOfModelParameters];
         p_eparplus = new double[m_NberOfModelParameters];
         p_eparminus = new double[m_NberOfModelParameters];
         p_eparparab = new double[m_NberOfModelParameters];
         p_CovMatrix = new double[NdimSquared];
      }

      m_Description = ToBeCopied.m_Description;

      for (int Item = 0; Item < m_NberOfModelParameters; Item++) {
         p_par[Item] = ToBeCopied.p_par[Item];
         p_parName[Item] = ToBeCopied.p_parName[Item];
         p_eparplus[Item] = ToBeCopied.p_eparplus[Item];
         p_eparminus[Item] = ToBeCopied.p_eparminus[Item];
         p_eparparab[Item] = ToBeCopied.p_eparparab[Item];
      }

      int NdimSquared = m_NberOfModelParameters * m_NberOfModelParameters;
      if (ToBeCopied.p_CovMatrix) {
         for (int Item = 0; Item < NdimSquared; Item++)
            p_CovMatrix[Item] = ToBeCopied.p_CovMatrix[Item];
      } else {
         for (int Item = 0; Item < NdimSquared; Item++)
            p_CovMatrix[Item] = 0.;
         std::cout << "===> FitOutput::FitOutput:	"
                   << "No covarance matrix available " << std::endl;
      }

      m_MinnLL = ToBeCopied.m_MinnLL;
      m_NameOfTheModel = ToBeCopied.m_NameOfTheModel;
      m_NberOfDataPoints = ToBeCopied.m_NberOfDataPoints;
   }
   return *this;
}

void FitOutput::PrintFitOutput()
{

   std::cout << std::endl;
   std::cout << "********************************************************" << std::endl;

   std::cout << "*Fit results " << std::endl;
   std::cout << std::endl;

   std::cout << "-Name of the Model " << m_NameOfTheModel << std::endl;
   std::cout << std::endl;

   std::cout << "-Description		" << m_Description << std::endl;
   std::cout << std::endl;

   std::cout << "-Number of Model Parameters " << m_NberOfModelParameters << std::endl;
   std::cout << std::endl;

   std::cout << "-Number of Data points " << m_NberOfDataPoints << std::endl;
   std::cout << std::endl;

   std::cout << "-Best Chi2 " << std::setw(20) << std::setprecision(10) << m_MinnLL
             << std::endl;
   std::cout << std::endl;

   std::cout << "-Parameters: name, values and errors " << std::endl;

   for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
      std::cout
         << " " << ipar << " " << p_parName[ipar] << " " << std::setw(20)
         << std::setprecision(10) << p_par[ipar] << " +/-" << std::setw(20)
         << std::setprecision(10)
         << p_eparparab[ipar]
         //				<< " ( "
         //				<< std::setw(20) << std::setprecision(10) << p_eparplus	[ipar]
         //				<< "; "
         //				<< std::setw(20) << std::setprecision(10) << p_eparminus [ipar]
         //				<< " ) "
         << std::endl;
   }
   std::cout << std::endl;

   std::cout << "********************************************************" << std::endl;
   std::cout << std::endl;
}

void FitOutput::PrintFitOutputInCanvas(double Xstart, double Ystart, double Step)
{
   TLatex aTLatex;
   aTLatex.SetTextAlign(12);
   aTLatex.SetTextSize(0.025);

   std::ostringstream Aostringstream;
   Aostringstream << std::setiosflags(std::ios::fixed);

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "*Fit results " << std::endl;
   aTLatex.DrawLatex(Xstart, Ystart, (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Name of the Model " << m_NameOfTheModel;
   aTLatex.DrawLatex(Xstart, (Ystart - 1 * Step), (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Description " << m_Description;
   aTLatex.DrawLatex(Xstart, (Ystart - 2 * Step), (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Number of Model Parameters " << m_NberOfModelParameters;
   aTLatex.DrawLatex(Xstart, (Ystart - 3 * Step), (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Number of Data points " << m_NberOfDataPoints;
   aTLatex.DrawLatex(Xstart, (Ystart - 4 * Step), (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Best Chi2 " << std::setw(20) << std::setprecision(10) << m_MinnLL;
   aTLatex.DrawLatex(Xstart, (Ystart - 5 * Step), (Aostringstream.str()).c_str());

   Aostringstream.str("");
   Aostringstream.clear();
   Aostringstream << "-Parameters: name, values and errors ";
   aTLatex.DrawLatex(Xstart, (Ystart - 6 * Step), (Aostringstream.str()).c_str());

   for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
      Aostringstream.str("");
      Aostringstream.clear();
      Aostringstream << " " << ipar << " " << p_parName[ipar] << " " << std::setw(20)
                     << std::setprecision(10) << p_par[ipar] << " +/-" << std::setw(20)
                     << std::setprecision(10) << p_eparparab[ipar];
      aTLatex.DrawLatex(Xstart, (Ystart - (7 + ipar) * Step),
                        (Aostringstream.str()).c_str());
   }
}

void FitOutput::SetResults(TVirtualFitter *pTVirtualFitter)
{

   int ier2 = 0;

   //	std::cout << "FitOutput::SetResults: "
   //			<< " m_NberOfModelParameters " << m_NberOfModelParameters << std::endl;

   for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
      double best, ebest;
      Double_t al, bl;
      Char_t parName[60];

      ier2 = pTVirtualFitter->GetParameter(ipar, parName, best, ebest, al, bl);
      if (ier2 < 0)
         std::cout << " ier2 " << ier2 << " for pTVirtualFitter->GetParameter "
                   << std::endl;

      double eplus, eminus, eparab, globcc;
      ier2 = pTVirtualFitter->GetErrors(ipar, eplus, eminus, eparab, globcc);
      if (ier2 < 0)
         std::cout << " ier2 " << ier2 << " for pTVirtualFitter->GetErrors " << std::endl;

      //	std::cout
      //		<< " ebest	" << ebest
      //		<< " eplus	" << eplus
      //		<< " eminus " << eminus
      //		<< std::endl;

      p_par[ipar] = best;
      p_parName[ipar] = std::string(parName);
      p_eparplus[ipar] = eplus;
      p_eparminus[ipar] = eminus;

      p_eparplus[ipar] = ebest;
      p_eparminus[ipar] = -ebest;
   }

   Double_t *pCovReturned = pTVirtualFitter->GetCovarianceMatrix();
   if (pCovReturned) {
      TMatrixD aTMatrixD(m_NberOfModelParameters, m_NberOfModelParameters,
                         pTVirtualFitter->GetCovarianceMatrix());
      for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
         for (int jpar = 0; jpar < m_NberOfModelParameters; jpar++) {
            p_CovMatrix[ipar + jpar * m_NberOfModelParameters] = aTMatrixD(ipar, jpar);
            // std::cout << std::setprecision(4) << aTMatrixD(ipar,jpar) << " ";
         }
         // std::cout << std::endl;
      }
      // std::cout << std::endl;
   } else {
      std::cout << "===> FitOutput::SetResults "
                << "covariance matrix returned is 0; No covarance matrix available "
                << std::endl;
   }
}

void FitOutput::SetResults(const std::vector<std::string> &V_PARname,
                           const std::vector<double> &V_PAR,
                           const std::vector<double> &V_ePAR)
{
   for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
      p_par[ipar] = V_PAR[ipar];
      p_parName[ipar] = V_PARname[ipar];
      p_eparplus[ipar] = V_ePAR[ipar];
      p_eparminus[ipar] = V_ePAR[ipar];
   }

   for (int ipar = 0; ipar < m_NberOfModelParameters; ipar++) {
      for (int jpar = 0; jpar < m_NberOfModelParameters; jpar++) {
         if (ipar != jpar)
            p_CovMatrix[ipar + jpar * m_NberOfModelParameters] = 0.;
         if (ipar == jpar)
            p_CovMatrix[ipar + jpar * m_NberOfModelParameters] =
               1. / (p_eparplus[ipar] * p_eparplus[ipar]);
      }
   }
}
