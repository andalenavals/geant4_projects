#ifndef AnalysisManager
#define AnalysisManager 1

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TLegend.h"
#include "TMath.h"
#include "TF1.h"
#include "TLatex.h"
#include "TString.h"
#include "TMatrix.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TMath.h"
#include "TColor.h"
#include "TFrame.h"

using namespace std;

class MyAnalysisManager{
public:
  MyAnalysisManager(void);
  ~MyAnalysisManager(void);

  double FWHM(double E, double a, double b, double c)
  { return a*E+b*sqrt(E)+c;};
  Color_t Color(int i);
  void CreateTCanvas(const char* name, const char* title , Int_t ww, Int_t wh );
  void CreateTGraph(int n, Color_t col );
  void CreateTH1(const char* name, int nbin, double xlow, double xup );
  void CreateCSV(const char* name);
  void DefinirCanvas (TCanvas * c1);
  void DefinirEstilo (TStyle * S);
  template<typename T>
  void DefinirTitulos (T h, const char * title , const char * titlex ,const char * titley); 
  void Destroy();
  void DrawAnimation(int i, int pasos , double x0, double y0,  double x1, double y1);
  void FillTH1(int i, double val  );
  template<typename T>
  void FillCSV(int i, T val  );
  static MyAnalysisManager* Instance();
  TH1D * GetTH1(int i);
  TGraph * GetTGraph(int i);
  void Print(const char* name);
  double RandomFWHM(double , double, double, double );
  void SetBinContentTH1(int i, double x, double y);
  void SetFrame(double x0, double y0,  double x1, double y1, const char * title , const char * titlex ,const char * titley);
  void SetPointTGraph(int t , int point , double x, double y );

private:
  vector <ofstream *> archivos;
  TCanvas* can= new TCanvas("can", "" , 800 , 600);
  Color_t colores[50];
  TH1F *frame;
  vector  <TH1D *> histos;
  static MyAnalysisManager* instance;
  TRandom* ran = new TRandom(123); 
  int paso;
  vector  <TGraph *> tgraphs;

   
};

template<typename T>
void MyAnalysisManager::FillCSV(int i, T val  )
{
  * archivos[i]<<val<<endl;
}

template<typename T>
void MyAnalysisManager::DefinirTitulos(T h, const char * title , const char * titlex ,const char * titley)
{
  h->SetTitle(title);
  h->GetYaxis()->SetTitle(titley);

  h->GetYaxis()->CenterTitle();
  h->GetXaxis()->SetTitle(titlex);
  h->GetXaxis()->CenterTitle();
  h->SetTitleOffset(1.0 , "x"); //mover titulos ejes
  h->SetTitleOffset(1.0 , "y");
  h->SetTitleOffset(1.5 , "z");
  h->SetTitleSize(0.065, "x"); //Tamaño titulos ejes
  h->SetTitleSize(0.065, "y");
  
}

#endif
