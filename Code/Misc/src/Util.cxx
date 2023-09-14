#include "Misc/Util.h"
#include "TPaveStats.h"
#include "TH1.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int MyMakeDir(const std::string& DirName)
{
 std::string MakeCommand = "mkdir -p " + DirName ;
 return system(MakeCommand.c_str()) ;
}

int ListDir(std::string DirName, std::vector < std::string >& VectorOfStrings)
{
  
  int ToBeReturned = 0 ;

  VectorOfStrings.clear();
  
  DIR *dp;
  struct dirent *ep;  
  std::string   DirNameLocal = DirName ;
  dp = opendir (DirNameLocal.c_str());

  if (dp != NULL)
  {
    while((ep = readdir(dp)) != NULL) 
    {
      std::string FileFound = ep->d_name ;
      std::size_t found = FileFound.find(".csv");
      if (found!=std::string::npos) {
        std::string FileFound2 = DirName + "/" + FileFound ;
        VectorOfStrings.push_back(FileFound2);
      }
    }
    (void) closedir (dp); 
    ToBeReturned = 1 ;
  }else{
//    std::cout << "*Couldn't open the directory"<< std::endl ;
    ToBeReturned = 0 ;
  }
  
  return ToBeReturned ;
  
}

void Free_trim(std::string& s) {
    while(s.compare(0,1," ")==0)
        s.erase(s.begin()); // remove leading whitespaces
    while(s.size()>0 && s.compare(s.size()-1,1," ")==0)
        s.erase(s.end()-1); // remove trailing whitespaces
}



TGraphErrors* Convert_TH2_TGE(const TH2* pTH2)
{
  TGraphErrors* gr = new TGraphErrors() ;
  for (int i = 1 ; i < pTH2->GetXaxis()->GetNbins() ; ++i) {

    TH1D* temp_h = pTH2-> ProjectionY(Form("projections_bin_%i", i), i, i);

    double x = pTH2->GetXaxis()->GetBinCenter(i);
    double y = temp_h->GetBinCenter(temp_h->GetMaximumBin());

    float start = -1.;
    float end   = -1.;
    float max = temp_h->GetMaximum();

    for (Int_t bin = 0; bin < temp_h->GetXaxis()->GetNbins(); ++bin) {
      if (start == -1. && temp_h->GetBinContent(bin) >= max / 2.)
        start = temp_h->GetBinCenter(bin);

      if (end == -1. && start != -1. && temp_h->GetBinContent(bin) <= max / 2.)
        end = temp_h->GetBinCenter(bin);
    }

    float e = end - start;

    gr->SetPoint(gr->GetN(), x, y);
    gr->SetPointError(gr->GetN()-1, 0, e/2.);
  }
  return gr ;
}



TGraphErrors* Convert_TH2_TGE_v2(const TH2* pTH2)
{
  TGraphErrors* gr = new TGraphErrors() ;
  for (int i = 1 ; i < pTH2->GetXaxis()->GetNbins() ; ++i) {

    TH1D* temp_h = pTH2-> ProjectionY(Form("projections_bin_%i", i), i, i);

    double x = pTH2->GetXaxis()->GetBinCenter(i);
    double y = temp_h->GetBinCenter(temp_h->GetMaximumBin());

    gr->SetPoint(gr->GetN(), x, y);
    gr->SetPointError(gr->GetN()-1, 0, 100);
  }
  return gr ;
}



// Set graphic settings of a TGraphErrors
void Graphic_setup(TGraphErrors* ptge, Size_t markersize, Style_t markerstyle, Color_t markercolor, Width_t linewidth, Color_t linecolor){
  ptge->SetMarkerSize(markersize) ;
  ptge->SetMarkerStyle(markerstyle) ;
  ptge->SetMarkerColor(markercolor) ;
  ptge->SetLineWidth(linewidth) ;
  ptge->SetLineColor(linecolor) ;
}



// Set graphic settings of an histogram
void Graphic_setup(TH1* th1, Size_t markersize, Style_t markerstyle, Color_t markercolor, Width_t linewidth, Color_t linecolor){
  th1->SetMarkerSize(markersize) ;
  th1->SetMarkerStyle(markerstyle) ;
  th1->SetMarkerColor(markercolor) ;
  th1->SetLineWidth(linewidth) ;
  th1->SetLineColor(linecolor) ;
}



// Set graphic settings of an TH2
void Graphic_setup(TH2* th2, Size_t markersize, Style_t markerstyle, Color_t markercolor){
  th2->SetMarkerSize(markersize) ;
  th2->SetMarkerStyle(markerstyle) ;
  th2->SetMarkerColor(markercolor) ;
}



// Set the stats box position
void SetStatBoxPosition(TH1* pTH, const double& xmin, const double& xmax, const double& ymin, const double& ymax){
  TPaveStats *st = (TPaveStats*)pTH->FindObject("stats");
  st->SetX1NDC(xmin) ;
  st->SetX2NDC(xmax) ;
  st->SetY1NDC(ymin) ;
  st->SetY2NDC(ymax) ;
}