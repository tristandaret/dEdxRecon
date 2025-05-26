#include "FuncFromTGraph.h"

FuncFromTGraph::FuncFromTGraph(TGraph *pTGraph)
{
   p_TGraph = pTGraph;

   double *X_pTGraph = p_TGraph->GetX();

   m_StepX = X_pTGraph[1] - X_pTGraph[0];

   m_X_Lowest = X_pTGraph[0];

   m_X_Highest = X_pTGraph[p_TGraph->GetN() - 1];

   //	std::cout << " m_StepX	" << std::setw(14) << std::setprecision(9) << m_StepX	<<
   //std::endl; 	std::cout << " m_X_Lowest	" << std::setw(14) << std::setprecision(9) <<
   //m_X_Lowest	<< std::endl; 	std::cout << " m_X_Highest " << std::setw(14) <<
   //std::setprecision(9) << m_X_Highest << std::endl;
   //
   //	double* Y_pTGraph = p_TGraph ->GetX();
   //	std::cout
   //		<< " X_pTGraph[0] " << std::setw(14) << std::setprecision(9) << X_pTGraph[0]
   //		<< " Y_pTGraph[0] " << std::setw(14) << std::setprecision(9) << Y_pTGraph[0]
   //		<< std::endl;
   //	std::cout
   //		<< " X_pTGraph[1] " << std::setw(14) << std::setprecision(9) << X_pTGraph[1]
   //		<< " Y_pTGraph[1] " << std::setw(14) << std::setprecision(9) << Y_pTGraph[1]
   //		<< std::endl;
}

FuncFromTGraph::~FuncFromTGraph() {}

double FuncFromTGraph::operator()(double *x, double *par)
{
   double vX = x[0];
   return Get_Y(vX);
}

double FuncFromTGraph::Get_Y(const double &vX_in)
{
   if (vX_in > m_X_Highest)
      return 0.;

   double vXLocal = vX_in - m_X_Lowest;
   if (vXLocal < 0)
      return 0.;

   double *X_pTGraph = p_TGraph->GetX();
   double *Y_pTGraph = p_TGraph->GetY();

   int Index = int(vXLocal / m_StepX);

   double XL = X_pTGraph[Index];
   double XH = X_pTGraph[Index + 1];
   double YL = Y_pTGraph[Index];
   double YH = Y_pTGraph[Index + 1];

   //	std::cout << " m_X_Lowest " << m_X_Lowest << std::endl;
   //	std::cout << " m_StepX " << m_StepX << std::endl;
   //
   //	std::cout << " vX_in " << vX_in << std::endl;
   //	std::cout << " vXLocal " << vXLocal	<< std::endl;
   //	std::cout << " Index " << Index << std::endl;
   //	std::cout << " XL " << XL << std::endl;
   //	std::cout << " XH " << XH << std::endl;
   //	std::cout << " YL " << YL << std::endl;
   //	std::cout << " YH " << YH << std::endl;

   return (YL + (vX_in - XL) * (YH - YL) / (XH - XL));
}

void FuncFromTGraph::Dump()
{
   double *X_pTGraph = p_TGraph->GetX();
   double *Y_pTGraph = p_TGraph->GetY();
   int iTem_Max = p_TGraph->GetN();
   for (int iTem = 0; iTem < iTem_Max; iTem++) {
      std::cout << "DUMPFuncFromTGraph" << std::setw(10) << iTem << std::setw(20)
                << std::setprecision(10) << X_pTGraph[iTem] << std::setw(20)
                << std::setprecision(10) << Y_pTGraph[iTem] << std::endl;
   }
}
