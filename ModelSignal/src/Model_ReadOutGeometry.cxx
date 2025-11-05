#/***************************************************************************
 * File: Model_ReadOutGeometry.cxx
 * Project: dEdxRecon
 *
 * Brief: Implementation of the readout geometry model. Provides concrete
 *        methods to query pad centres, edges and board layout parameters.
 *
 * Contents: Model_ReadOutGeometry constructor/destructor and geometry accessors.
 *
 * Notes: Geometry parameters are used throughout simulation and
 *        reconstruction stages.
 ***************************************************************************/

#include "Model_ReadOutGeometry.h"

Model_ReadOutGeometry::Model_ReadOutGeometry() {
  m_LX = 11.18E-3;
  m_LY = 10.09E-3;

  m_LX = 11.28E-3;
  m_LY = 10.19E-3;

  m_Xpad_min = 0;
  m_Ypad_min = 0;
  m_Nx = 36;
  m_Ny = 32;
}

Model_ReadOutGeometry::~Model_ReadOutGeometry() {
  int VerboseDelete = 0;
  if (VerboseDelete == 1)
    std::cout << " Model_ReadOutGeometry Destructor " << std::endl;
}

void Model_ReadOutGeometry::SquareGeometry() {
  double k_Nx = m_Nx;
  double k_Ny = m_Ny;

  double The_N = k_Nx;
  if (k_Ny > k_Nx)
    The_N = k_Ny;

  m_Nx = The_N;
  m_Ny = The_N;
}

void Model_ReadOutGeometry::SwapGeometry() {
  double k_LX = m_LX;
  double k_LY = m_LY;
  double k_Nx = m_Nx;
  double k_Ny = m_Ny;

  m_LX = k_LY;
  m_LY = k_LX;
  m_Nx = k_Ny;
  m_Ny = k_Nx;
}
double Model_ReadOutGeometry::Get_LX() const { return m_LX; }
double Model_ReadOutGeometry::Get_LY() const { return m_LY; }
double Model_ReadOutGeometry::Get_Xpad_min() const { return m_Xpad_min; }
double Model_ReadOutGeometry::Get_Ypad_min() const { return m_Ypad_min; }
int Model_ReadOutGeometry::Get_Nx() const { return m_Nx; }
int Model_ReadOutGeometry::Get_Ny() const { return m_Ny; }

void Model_ReadOutGeometry::Set_LX(const double &LX) { m_LX = LX; }
void Model_ReadOutGeometry::Set_LY(const double &LY) { m_LY = LY; }
void Model_ReadOutGeometry::Set_Xpad_min(const double &Xpad_min) {
  m_Xpad_min = Xpad_min;
}
void Model_ReadOutGeometry::Set_Ypad_min(const double &Ypad_min) {
  m_Ypad_min = Ypad_min;
}
void Model_ReadOutGeometry::Set_Nx(const int &Nx) { m_Nx = Nx; }
void Model_ReadOutGeometry::Set_Ny(const int &Ny) { m_Ny = Ny; }

void Model_ReadOutGeometry::WriteOut() const {
  std::cout << "StartModel_ReadOutGeometry" << std::endl;
  std::cout << " m_LX	; " << std::setw(13) << std::setprecision(6) << m_LX
            << std::endl;
  std::cout << " m_LY	; " << std::setw(13) << std::setprecision(6) << m_LY
            << std::endl;
  std::cout << " m_Xpad_min; " << std::setw(13) << std::setprecision(6)
            << m_Xpad_min << std::endl;
  std::cout << " m_Ypad_min; " << std::setw(13) << std::setprecision(6)
            << m_Ypad_min << std::endl;
  std::cout << " m_Nx	; " << std::setw(13) << std::setprecision(0) << m_Nx
            << std::endl;
  std::cout << " m_Ny	; " << std::setw(13) << std::setprecision(0) << m_Ny
            << std::endl;
  std::cout << "EndModel_ReadOutGeometry" << std::endl;
}

double Model_ReadOutGeometry::Get_XcPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  return (m_Xpad_min + 0.5 * m_LX + double(iX) * m_LX);
}
double Model_ReadOutGeometry::Get_YcPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  return (m_Ypad_min + 0.5 * m_LY + double(iY) * m_LY);
}

double Model_ReadOutGeometry::Get_XLPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  double Xpad_C = Get_XcPad(iX, iY, ModuleNber);
  return (Xpad_C - 0.5 * m_LX);
}
double Model_ReadOutGeometry::Get_YLPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  double Ypad_C = Get_YcPad(iX, iY, ModuleNber);
  return (Ypad_C - 0.5 * m_LY);
}

double Model_ReadOutGeometry::Get_XHPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  double Xpad_C = Get_XcPad(iX, iY, ModuleNber);
  return (Xpad_C + 0.5 * m_LX);
}
double Model_ReadOutGeometry::Get_YHPad(const int &iX, const int &iY,
                                        const int &ModuleNber) const {
  double Ypad_C = Get_YcPad(iX, iY, ModuleNber);
  return (Ypad_C + 0.5 * m_LY);
}

void Model_ReadOutGeometry::GetPadEdges(const int &iX, const int &iY,
                                        const int &ModuleNber, double &XL,
                                        double &XH, double &YL,
                                        double &YH) const {
  double Xpad_C = Get_XcPad(iX, iY, ModuleNber);
  double Ypad_C = Get_YcPad(iX, iY, ModuleNber);

  XH = Xpad_C + 0.5 * m_LX;
  XL = Xpad_C - 0.5 * m_LX;

  YH = Ypad_C + 0.5 * m_LY;
  YL = Ypad_C - 0.5 * m_LY;
}
