#include "Tristan/Misc_Functions.h"
#include <cmath>
#include <numeric>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


// FILE HANDLING /////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read CSV file
std::vector<std::vector<float>> readCSV(std::string filename) {
  std::vector<std::vector<float>> data;
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream lineStream(line);
    std::string cell;
    std::vector<float> row;
    while (std::getline(lineStream, cell, ',')) {
      row.push_back(std::stof(cell));
    }
    data.push_back(row);
  }
  return data;
}


// Function to search for a word in a CSV file
bool Get120_CSV(const std::string& filename, const std::string& targetWord, int& value1, int& value2) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string word;
    double val1, val2;

      // Parse the CSV line
      if (std::getline(ss, word, ',') && ss >> val1 && ss.ignore() && ss >> val2) {
        if (word == targetWord) {
          value1 = val1;
          value2 = val2;
          return true; // Word found
      }
    }
  }

  return false; // Word not found
}


// Function to update a CSV file
void Set120_CSV(const std::string& filename, const std::string& targetWord, int value1, int value2) {
    std::ofstream file(filename, std::ios_base::app); // Open in append mode

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Append a new line with the provided values
    file << targetWord << ", " << value1 << ", " << value2 << std::endl;

    file.close();
}



// General functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find if a value is in a vector
bool is_in(std::vector<double> v, double val){
  if(std::find(v.begin(), v.end(), val) != v.end()) return true;
  else return false;
}



// General Math //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mean of vector
float mean(const std::vector<float>& values) {
    float sum = 0.0;
    for (const float& value : values) {
        sum += value;
    }

    return sum / values.size();
}



// Heaviside
float Heaviside(const float& t, const float& t_0){
  if(t_0 >  t)  return 0.0 ;
  if(t_0 == t)  return 0.5 ;
  if(t_0 <  t)  return 1.0 ;
  else          return -999.0 ;
}



TF1* Fit1Gauss(TH1F* h1F){   
  return Fit1Gauss(h1F, 2) ;
}
TF1* Fit1Gauss(TH1F* h1F, float range){   
    TF1* gausn    = new TF1("gausn", "gausn") ;
    float mean    = h1F->GetBinCenter(h1F->GetMaximumBin()) ;
    float std     = h1F->GetRMS() ;
    float max     = mean + 1.8*std ;
    float min     = mean - 1.8*std ;
    h1F->Fit("gausn","RQ","0", min, max) ;

    TF1* g1       = h1F->GetFunction("gausn") ;
    mean          = g1->GetParameter(1) ;
    std           = g1->GetParameter(2) ;
    max           = mean + 1.7*std ;
    min           = mean - 1.7*std ;
    h1F->Fit("gausn","RQ","0", min, max) ;

    TF1* g2       = h1F->GetFunction("gausn") ;
    mean          = g2->GetParameter(1) ;
    std           = g2->GetParameter(2) ;
    max           = mean + 1.6*std ;
    min           = mean - 1.6*std ;
    h1F->Fit("gausn","RQ","0", min, max) ;

    TF1* g3       = h1F->GetFunction("gausn") ;
    mean          = g3->GetParameter(1) ;
    std           = g3->GetParameter(2) ;
    max           = mean + range*std ;
    min           = mean - range*std ;
    h1F->Fit("gausn","RQ","0", min, max) ;

    TF1* tf1     = h1F->GetFunction("gausn") ;
    delete gausn ;

    return tf1 ;
} 




TF1* Fit2Gauss(TH1F* h1F){
 return Fit2Gauss(h1F, -5, 1, -1, 2) ;
}
TF1* Fit2Gauss(TH1F* h1F, const float& x1min, const float& x1max, const float& x2min, const float& x2max){
  double par[6] ; 
  float min1 = h1F->GetMean() + x1min*h1F->GetStdDev() ;
  float max1 = h1F->GetMean() + x1max*h1F->GetStdDev() ;
  float min2 = h1F->GetMean() + x2min*h1F->GetStdDev() ;
  float max2 = h1F->GetMean() + x2max*h1F->GetStdDev() ;

  TF1* g1 = new TF1("g1", "gausn") ;
  h1F->Fit(g1, "QR", "0", min1, max1) ;
  g1->GetParameters(&par[0]) ;
  delete g1 ;

  TF1* g2 = new TF1("g2", "gausn") ;
  h1F->Fit(g2, "QR", "0", min2, max2) ; 
  g2->GetParameters(&par[3]) ;
  delete g2 ;

  TF1* g3 = new TF1("g3", "gausn(0)+gausn(3)") ;
  g3->SetParameters(par) ;
  h1F->Fit(g3, "QRS", "0", min1, max2) ;

  TF1* tf1 = h1F->GetFunction("g3") ;
  tf1->SetParName(0, "A_1") ;
  tf1->SetParName(1, "#mu_1") ;
  tf1->SetParName(2, "#sigma_1") ;
  tf1->SetParName(3, "A_2") ;
  tf1->SetParName(4, "#mu_2") ;
  tf1->SetParName(5, "#sigma_2") ;
  return tf1 ;
}



// General Physics ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bethe-Bloch relativistic for heavy charged particles
TF1* BetheBloch(const float& Pmin, const float& Pmax, const double& M, const std::string& particle){
  /*  Input: Pmin & Pmax GeV | m GeV 
      Output: keV/cm */
  // float n                         = 5.357e20; e/cm^3
  // float alpha2                    = pow(1/137, 2);
  // float hbar2c2                   = pow(1.973e-14, 2) (GeV cm)^2
  double coeff                    = 0.2723; // 4 pi n alpha^2 hbar^2 c^2 / (m_e c^2) (keV^2/cm)
  double me                       = 511e-6; // GeV
  double I                        = 188e-9; // GeV

  // PDG general
  const char* formula             = "[1]/(x*x)*(x*x+[0]*[0]) * (log(2*[2]/[3]*x*x/([0]*[0])) - 0.5*log(1 + 2*sqrt(x*x+[0]*[0])/([0]*[0])*[2]/[0] + [2]*[2]/([0]*[0])) - x*x/(x*x+[0]*[0]))" ;
  TF1* dEdx                       = new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->                            SetParameters(M, coeff, me, I);
  dEdx->                            FixParameter(0, M);
  dEdx->                            FixParameter(2, me);
  dEdx->                            FixParameter(3, I);

  dEdx->                            SetTitle(Form("Bethe-Bloch for%s;Energy (GeV);mean (keV/cm)", particle.c_str()));
  return dEdx ; // keV/cm
}



// Bethe-Bloch relativistic for positrons with Bhabha scattering
TF1* BetheBlochBhabha(const float& Pmin, const float& Pmax, const double& m, const std::string& particle){
  /*  Input: Pmin & Pmax GeV | M GeV 
      Output: keV/cm */
  // float n                         = 5.357e20; e/cm^3
  // float alpha2                    = pow(1/137, 2);
  // float hbar2c2                   = pow(1.973e-14, 2) (GeV cm)^2
  double coeff                    = 0.2723; // 4 pi n alpha^2 hbar^2 c^2 / (m_e c^2) (keV^2/cm)
  double me                       = 511e-6; // GeV
  double M2                       = pow(m,2); // GeV^2
  double I                        = 188e-9; // GeV

  // PDG Bhabha
  const char* formula             = "[0]/(x*x)*(x*x+[2]) * (log(2*[1]/[3]*x*x/[2]) - 0.5*log(1 + 2*sqrt(x*x+[2])/[2]*[1]/sqrt([2]) + [1]*[1]/[2]) - x*x/(x*x+[2])/24. * (23 + 14/(sqrt(x*x+[2])/[2]-1) + 10/pow(sqrt(x*x+[2])/[2]-1, 2) + 4/pow(sqrt(x*x+[2])/[2]-1, 3)) )" ;
  TF1* dEdx                       = new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->                            SetParameters(coeff, me, M2, I);

  dEdx->                            SetTitle(Form("Bethe-Bloch with Bhabha Xsec for%s;Energy (GeV);mean (keV/cm)", particle.c_str()));
  return dEdx ; // keV/cm
}



// Bethe-Bloch with experimental parametrisation
TF1* BetheBlochExp(const float& Pmin, const float& Pmax, const double& M, const std::string& particle){
  /*  Input: Pmin & Pmax GeV | m GeV 
      Output: keV/cm */

  const char* formula             = "[1]/pow(x/sqrt(x*x+[0]*[0]),[4]) * ( [2] - pow(x/sqrt(x*x+[0]*[0]),[4]) - log([3]+ pow(x/[0], [5])) )" ;
  TF1* dEdx                       = new TF1(Form("dEdx_%s", particle.c_str()), formula, Pmin, Pmax, "");
  dEdx->                            FixParameter(0, M);

  dEdx->                            SetTitle(Form("Experimental Bethe-Bloch for%s;Energy (GeV);mean (keV/cm)", particle.c_str()));
  return dEdx ; // keV/cm
}



// Specific Math //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Dirac Pulse Response function
TH1F* DPR(std::string name, const float& tmin, float const& tmax, const float& t_0, const int& nbins, const int& iC, const float& PT, const float& TB){
  float Q  = 2./3.; 
  float ws = 2*TB/PT ;
  // if(PT == 200){ws = 0.4172  ; Q = 0.665  ; }
  // if(PT == 412){ws = 0.19518 ; Q = 0.6368 ; }
  name                            = name + "_" + std::to_string(iC) ;
  TH1F* h1f_DPR                   = new TH1F(name.c_str(), "Electronics pulse response function;time bin;signal (normalized)", nbins, tmin, tmax) ;
  float step                      = (tmax-tmin)/nbins ;
  float DPRmax                    = -999 ;
  for (int t = tmin ; t < tmax ; t+=step){
    float arg                     = ws*(t-t_0)/2 * sqrt(4-1/pow(Q,2)) ;
    float DPR                     = Heaviside(t, t_0)*(exp(-ws*(t-t_0)) + exp(-(ws*(t-t_0)/(2*Q))) * (sqrt((2*Q-1)/(2*Q+1)) * sin(arg) - cos(arg) ) ) ;
    h1f_DPR->                     SetBinContent((t-tmin)/step, DPR) ;
    if(DPR > DPRmax) DPRmax       = DPR ;
  }
  h1f_DPR->                       Scale(1/DPRmax) ;
  return h1f_DPR ;
}



// impact parameter d (in m) & track angle phi (in degrees) computed locally at the level of the pad
void local_params(const Pad* pPad, const Track* pTrack, float& d, float& dd, float& phi, float& trk_len_pad){

  // Get geometry of pad
  float x0                  = pPad->Get_XL() ;                // in meters
  float xc                  = pPad->Get_XPad() ;              // in meters
  float x1                  = pPad->Get_XH() ;                // in meters
  float y0                  = pPad->Get_YL() ;                // in meters
  float yc                  = pPad->Get_YPad() ;              // in meters
  float y1                  = pPad->Get_YH() ;                // in meters

  // Parameters of the fitted Track
  double a                  = pTrack->Get_ParameterValue(0) ; // in meters    (intercept)
  double da                 = pTrack->Get_ParameterError(0) ; // in meters    (intercept)
  double b                  = pTrack->Get_ParameterValue(1) ; // no dimension (slope)
  double db                 = pTrack->Get_ParameterError(1) ; // no dimension (slope)
  double c;
  double dc;

  phi                       = TMath::ATan(b)/TMath::Pi()*180 ;

  // Impact parameter in the pad
  d                         = (b*xc - yc + a) / sqrt(pow(b,2) + 1) ; // if pTrack is a line, then get d from it directly
  float da_d                = 1/sqrt(b*b+1);
  float db_d                = (xc*sqrt(b*b+1) - (b*xc-yc+a)*b/sqrt(b*b+1)) / (b*b+1);
  dd                        = sqrt(da_d*da_d * da*da + db_d*db_d * db*db);

  // Y position of the track at the left & right borders of the pad and X position at top and bottom borders
  float x_y0                = (y0-a)/b ;                      // in meters
  float x_y1                = (y1-a)/b ;                      // in meters
  float y_x0                = b*x0+a ;                        // in meters
  float y_x1                = b*x1+a ;                        // in meters

  float dx_y0                = sqrt(1/b*b * da*da + pow((y0-a)/b*b, 2) * db*db) ; // in meters
  float dx_y1                = sqrt(1/b*b * da*da + pow((y1-a)/b*b, 2) * db*db) ; // in meters
  float dy_x0                = sqrt(da*da + x0*x0*db*db) ;                        // in meters
  float dy_x1                = sqrt(da*da + x1*x1*db*db) ;                        // in meters


  // If parabolic fit <=> Magnetic field
  if(pTrack->GetNberOfParameters() == 3){
    c                       = pTrack->Get_ParameterValue(2) ; // in 1/meters  (curvature)
    dc                      = pTrack->Get_ParameterError(2) ; // in 1/meters  (curvature)

    float Delta_y0          = sqrt(b*b-4*(a-y0)*c);
    float Delta_y1          = sqrt(b*b-4*(a-y1)*c);
    
    // Roots of Pol2 to get X position of track at top and bottom borders of the pad (with parabolic track)
    float xroot1_y0         = (-b-Delta_y0)/(2*c) ;
    float da_xroot1_y0      = 1/Delta_y0;
    float db_xroot1_y0      = 1/(2*c)*(-b/Delta_y0-1);
    float dc_xroot1_y0      = (a-y0)/(c*Delta_y0) + (b+Delta_y0)/(2*c*c);
    float dxroot1_y0        = sqrt(da_xroot1_y0*da_xroot1_y0*da*da +  db_xroot1_y0*db_xroot1_y0*db*db + dc_xroot1_y0*dc_xroot1_y0*dc*dc);

    float xroot2_y0         = (-b+Delta_y0)/(2*c) ;
    float da_xroot2_y0      = -1/Delta_y0;
    float db_xroot2_y0      = 1/(2*c)*(+b/Delta_y0-1);
    float dc_xroot2_y0      = -(a-y0)/(c*Delta_y0) + (b-Delta_y0)/(2*c*c);
    float dxroot2_y0        = sqrt(da_xroot2_y0*da_xroot2_y0*da*da +  db_xroot2_y0*db_xroot2_y0*db*db + dc_xroot2_y0*dc_xroot2_y0*dc*dc);

    float xroot1_y1         = (-b-Delta_y1)/(2*c) ;
    float da_xroot1_y1      = 1/Delta_y1;
    float db_xroot1_y1      = 1/(2*c)*(-b/Delta_y1-1);
    float dc_xroot1_y1      = (a-y1)/(c*Delta_y1) + (b+Delta_y1)/(2*c*c);
    float dxroot1_y1        = sqrt(da_xroot1_y1*da_xroot1_y1*da*da +  db_xroot1_y1*db_xroot1_y1*db*db + dc_xroot1_y1*dc_xroot1_y1*dc*dc);

    float xroot2_y1         = (-b+Delta_y1)/(2*c) ;
    float da_xroot2_y1      = -1/Delta_y1;
    float db_xroot2_y1      = 1/(2*c)*(+b/Delta_y1-1);
    float dc_xroot2_y1      = -(a-y1)/(c*Delta_y1) + (b-Delta_y1)/(2*c*c);
    float dxroot2_y1        = sqrt(da_xroot2_y1*da_xroot2_y1*da*da +  db_xroot2_y1*db_xroot2_y1*db*db + dc_xroot2_y1*dc_xroot2_y1*dc*dc);

    // Select correct root = closest to the center of the pad
    if(fabs(xroot1_y0-xc) < fabs((xroot2_y0-xc))){x_y0 = xroot1_y0 ; dx_y0 = dxroot1_y0 ;}
    else                                         {x_y0 = xroot2_y0 ; dx_y0 = dxroot2_y0 ;}
    if(fabs(xroot1_y1-xc) < fabs((xroot2_y1-xc))){x_y1 = xroot1_y1 ; dx_y1 = dxroot1_y1 ;}
    else                                         {x_y1 = xroot2_y1 ; dx_y1 = dxroot2_y1 ;}

    // Y position of the track at level of left & right borders of the pad (with parabolic track)
    y_x0                    = a + b*x0 + c*x0*x0 ;
    float da_y_x0           = 1.;
    float db_y_x0           = x0;
    float dc_y_x0           = x0*x0;

    y_x1                    = a + b*x1 + c*x1*x1 ;
    float da_y_x1           = 1.;
    float db_y_x1           = x1;
    float dc_y_x1           = x1*x1;


    // Get error on Y
    std::vector<double> errorsy_x0 = {da_y_x0, db_y_x0, dc_y_x0};
    std::vector<double> errorsy_x1 = {da_y_x1, db_y_x1, dc_y_x1};

    double cov_dy_x0 = 0;
    double cov_dy_x1 = 0;
    TMatrixD pCovMat        = pTrack->Get_CovMatrix();
    for(int i = 0; i<(int)pCovMat.GetNrows(); i++){
      for(int j = 0; j<(int)pCovMat.GetNcols(); j++){
        cov_dy_x0 += errorsy_x0[i]*pCovMat(i,j)*errorsy_x0[j];
        cov_dy_x1 += errorsy_x1[i]*pCovMat(i,j)*errorsy_x1[j];
      }
    }
    dy_x0 = sqrt(cov_dy_x0);
    dy_x1 = sqrt(cov_dy_x1);
  }

  int   wall                = 0 ; // count how many walls were crossed
  float x[3]                = {0} ;
  float y[3]                = {0} ;
  float dx[3]               = {0} ;
  float dy[3]               = {0} ;

  // Determine which walls were crossed
  if(y0 <= y_x0 && y_x0 <= y1){   // left
    x[wall]                 = x0 ;
    y[wall]                 = y_x0 ; 
    dx[wall]                = 0 ;
    dy[wall]                = dy_x0 ; 
    wall++ ;
  }
  if(y0 <= y_x1 && y_x1 <= y1){   // right
    x[wall]                 = x1 ;
    y[wall]                 = y_x1 ;
    dx[wall]                = 0 ;
    dy[wall]                = dy_x1 ;
    wall++ ;
  }
  if(x0 <= x_y1 && x_y1 <= x1){   // top
    x[wall]                 = x_y1 ;
    y[wall]                 = y1 ;
    dx[wall]                = dx_y1 ;
    dy[wall]                = 0 ;
    wall++ ;
  }
  if(x0 <= x_y0 && x_y0 <= x1){   // bottom
    x[wall]                 = x_y0 ;
    y[wall]                 = y0 ;
    dx[wall]                = dx_y0 ;
    dy[wall]                = 0 ;
    wall++ ;
  }

  if(wall != 2 and wall > 0){
    std::cout << "ALERT: " << wall << " walls crossed for a pad in entry #" << pPad->Get_EntryNber() << std::endl ;
    std::cout << x0*100 << " " << x1*100 << " " << y0*100 << " " << y1*100 << std::endl ;
    std::cout << x_y0*100 << " " << x_y1*100 << " " << y_x0*100 << " " << y_x1*100 << std::endl ;
    std::cout << a*100 << " " << b << " " << pTrack->Get_ParameterValue(2)/100 << std::endl ;
  }

  trk_len_pad               = sqrt(pow(y[1]-y[0],2) + pow(x[1]-x[0],2)) ; // in meters (track length in the pad)

  float m=0, q=0;
  // float dm=0, dq=0;
  // If parabolic fit <=> Magnetic field
  if(pTrack->GetNberOfParameters() == 3){ 
    TMatrixD pCovMat          = pTrack->Get_CovMatrix();
    // Get intercept from entrance & exit points
    // q                         = (y[0]*x[1]-y[1]*x[0]) / (x[1]-x[0]) ;
    q                         = a-c*x[0]*x[1];
    // float da_q                = 1;
    // float db_q                = 0;
    float dc_q                = -x[0]*x[1];
    // double errorsq[]          = {da_q, db_q, dc_q};

    // Get slope from entrance & exit points
    // m                         =  (y[1]-y[0]) / (x[1]-x[0]) ;
    m                         = b+c*(x[1]+x[0]);
    // float da_m                = 0;
    // float db_m                = 1;
    float dc_m                = x[1]+x[0];
    // double errorsm[]          = {da_m, db_m, dc_m};

    // Get impact parameter from entrance & exit points
    float da_d                = 1/sqrt(m*m+1);
    float db_d                = xc/sqrt(m*m+1) - (m*xc+q-yc)*m/pow(m*m+1, 1.5);
    float dc_d                = (dc_m*xc+dc_q)/sqrt(m*m+1) - (m*xc+q-yc)*dc_m*m/pow(m*m+1, 1.5);
    double errorsd[]          = {da_d, db_d, dc_d};

    TMatrixD row_err_d(1, 3, errorsd);
    TMatrixD d_mat_temp       = row_err_d * pCovMat;
    TMatrixD dd_matrix        = d_mat_temp * row_err_d.Transpose(row_err_d);

    // TMatrixD row_err_q(1, 3, errorsq);
    // d_mat_temp                = row_err_q * pCovMat;
    // TMatrixD dq_matrix        = d_mat_temp * row_err_q.Transpose(row_err_q);

    // TMatrixD row_err_m(1, 3, errorsm);
    // d_mat_temp                = row_err_m * pCovMat;
    // TMatrixD dm_matrix        = d_mat_temp * row_err_m.Transpose(row_err_m);

    phi                       = TMath::ATan(m)/TMath::Pi()*180 ;              // in degrees
    trk_len_pad               = sqrt(pow(y[1]-y[0],2) + pow(x[1]-x[0],2)) ;   // in meters
    d                         = (m*xc - yc + q) / sqrt(pow(m,2) + 1) ;        // in meters
    dd                        = sqrt(dd_matrix(0,0)); // in meters

    // dq                        = sqrt(dq_matrix(0,0)); // in meters
    // dm                        = sqrt(dm_matrix(0,0)); // no dimension
  }
  
  // if(x[0]){ 
  //   std::cout << std::setprecision(3) << "x_in: "  << x[0]*1000 << " +- " << dx[0]*1000 << " | ";
  //   std::cout << std::setprecision(3) << "y_in: "  << y[0]*1000 << " +- " << dy[0]*1000 << " | ";
  //   std::cout << std::setprecision(3) << "x_out: " << x[1]*1000 << " +- " << dx[1]*1000 << " | ";
  //   std::cout << std::setprecision(3) << "y_out: " << y[1]*1000 << " +- " << dy[1]*1000 << " | ";
  //   std::cout << std::setprecision(3) << "q: "     << q*1000    << " +- " << dq*1000    << " | ";
  //   std::cout << std::setprecision(3) << "m: "     << m         << " +- " << dm         << " | ";
  //   std::cout << std::setprecision(3) << "d: "     << d*1000    << " +- " << dd*1000    << std::endl;
  // }  
}





// Track length in ERAM in m
float trk_len(Module* pModule, const Track* pTrack){
  int n_param               = pTrack->GetNberOfParameters() ;
  float a                   = 0 ;
  if(n_param == 3) a        = pTrack->Get_ParameterValue(2) ; // curvature
  float b                   = pTrack->Get_ParameterValue(1) ; // slope
  int NC                    = pModule->Get_NberOfCluster() ;
  float start               = pModule->Get_Cluster(0)->Get_LeadingPad()->Get_XL() ;
  float end                 = pModule->Get_Cluster(NC-1)->Get_LeadingPad()->Get_XH() ;

  float L                   = 0 ;
  if(n_param == 2) L        = sqrt(pow(b,2)+1)*(end - start) ;

  if(n_param == 3){
    // computing arc length of P[2] polynomial between start and end points
    float in                = ((b+2*a*start)*sqrt(1+pow(b+2*a*start,2)) + TMath::ASinH(b+2*a*start))/(4*a) ;
    float out               = ((b+2*a*end)  *sqrt(1+pow(b+2*a*end,2))   + TMath::ASinH(b+2*a*end))  /(4*a) ;
    L                       = out - in ;
  }

  return L ; // in m
}



// ROOT /////////////////////////////////////////////////////////////////////////////////////////////////////////

// From TH1 get TGraph
TGraph* hist_to_graph(TH1 *h1){
  TGraph *gr                   = new TGraph(h1);
  Int_t nbins                  = gr->GetN();
  Double_t x,y;
  for (int n=0;n<nbins;++n) {
    gr->GetPoint(n,x,y);
    gr->SetPoint(n,x,y);
  }
  return gr;
}



// From TH1 get swapped TGraph (transpose x and y)
TGraph* Swapped_graph(TH1 *h1){
  TGraph *gr                   = new TGraph(h1);
  Int_t nbins                  = gr->GetN();
  Double_t x,y;
  for (int n=0;n<nbins;++n) {
    gr->GetPoint(n,x,y);
    gr->SetPoint(n,y,x);
  }
  return gr;
}




double GetResoError(TF1* tf1){
  return GetResoError(tf1, 1, 2) ;
}
double GetResoError(TF1* tf1, const int& mu, const int& sigma){
    double mean     = tf1->GetParameter(mu) ;
    double std      = tf1->GetParameter(sigma) ;
    double dmean    = tf1->GetParError(mu) ;
    double dstd     = tf1->GetParError(sigma) ;
    return (std*dmean/(mean*mean) + dstd/mean) * 100 ;
}



double GetSeparation(const float& mean1, const float& std1, const float& mean2, const float& std2){
  float separation  = std::fabs(mean1 - mean2) / std::sqrt((std::pow(std1, 2) + std::pow(std2, 2)) / 2) ;
  return separation ;
}



double GetSeparationError(const float& mean1, const float& std1, const float& dmean1, const float& dstd1, const float& mean2, const float& std2, const float& dmean2, const float& dstd2){
  float mu_part     = (pow(dmean1, 2)+pow(dmean2, 2)) / (pow(std1, 2)+pow(std2, 2)) ;
  float sigma_part  = pow(mean1-mean2, 2) * (pow(std1, 2)*pow(dstd1, 2) + pow(std2, 2)*pow(dstd2, 2)) / pow(pow(std1, 2) + pow(std2, 2), 3) ;
  float err         = sqrt(2) * sqrt(mu_part + sigma_part) ;
  return err ;
}




void PrintResolution(TF1* tf1, TCanvas* pCanvas){
  return PrintResolution(tf1, pCanvas, 0.05, 0.7, kBlack, " ") ;
}
void PrintResolution(TF1* tf1, TCanvas* pCanvas, float NDCx, float NDCy, Color_t color, const std::string& title){
  double  xMax          = pCanvas->GetUxmax() ;
  double  yMax          = pCanvas->GetUymax() ;
  TLatex* ptText        = new TLatex ;
  ptText->SetTextSize(0.04) ;
  ptText->SetTextFont(42) ;
  ptText->SetTextAlign(31) ;
  float reso            = tf1->GetParameter(2)/tf1->GetParameter(1) * 100 ;
  float dreso           = GetResoError(tf1) ;
  ptText->SetText(NDCx*xMax, NDCy*yMax, Form("#frac{#sigma}{#mu}(%s) = %.2f #pm %.2f %%", title.c_str(), reso, dreso)) ;
  ptText->SetTextColor(color) ;
  ptText->DrawClone() ;
  delete ptText ;
}