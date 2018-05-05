#ifndef IMPROVE
#define IMPROVE 1

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

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


/**
Esta clase contiene algunos metodos que no estan definidos en ROOT y
otro que principalmente, resumen la forma en como se escriben los
códigos, para evitar tener códigos muy extensos

 */



class RootImprove{
private:
  Color_t colores[18];
  Style_t markerstyle[15];
public:
  RootImprove(void);
  ~RootImprove(void);
  Color_t Color(int i);
  void DefinirCanvas (TCanvas * c1);
  void DefinirEstilo (TStyle * S);
  void DefinirEjes(TGraphErrors * h, double xlow, double xup);
  void DefinirEjes(TGraphErrors * h, double xlow, double xup, double ylow, double yup);
  void DefinirEjes(TGraph * h, double xlow, double xup);
  void DefinirEjes(TGraph * h, double xlow, double xup, double ylow, double yup);
  void DefinirEjes(TH1 * h, double xlow, double xup);
  void DefinirEjes(TH1 * h, double xlow, double xup, double ylow, double yup);
  void DefinirEjes(TF1 * h, double xlow, double xup);
  void DefinirEjes(TF1 * h, double xlow, double xup, double ylow, double yup);
  void DefinirPalette (TStyle *);
  void DefinirLegend (TLegend * , int l, Color_t , double s );
  void DefinirTF1(TF1 * W, Style_t st, Size_t wd, Color_t co1);
  void DefinirTGraph (TGraph *, Style_t , Size_t,Size_t wd, Color_t, Color_t); 
  void DefinirTGraphE (TGraphErrors *, Style_t , Size_t, Size_t wd,Color_t, Color_t);
  void DefinirTH1 (TH1 *, Style_t , Size_t, Color_t);
  template<typename T>
  void DefinirTitulos (T h, const char * title , const char * titlex ,const char * titley); 
  void DibujarAjuste(double x0, double y0, const char* text );
  void Dibujar(TH2 *, const char * modo, TStyle *S ,Int_t color);
  void Dibujar(TH2 * h , const char * modo, TStyle * S , Int_t color, double xlow, double xup , double ylow, double yup  ); 
  int GenerarSemilla(void);
  int GetMaximumBin(TH1 * h1, double xlow, double xup); //**
  double GetMaximumColumna(const char* argv, const char* col, const char* format);
  double GetMinimumColumna(const char* argv , const char* col, const char* format);
  int GetNumberFilas(const char* argv, const char* col, const char* format);
  double GetBinUpEdge(TH1 * h , int y ){return h->GetBinLowEdge(y) + h->GetBinWidth(y) ;};
  double GetXLow(TH1 * h){return h->GetBinLowEdge(1);};
  double GetXUp(TH1 * h){return GetBinUpEdge( h ,h->GetNbinsX() ) ;};
  //  double GetXLow(TH2 * h){return h->GetBinLowEdge(1);};
  // double GetXUp(TH2 * h){return GetBinUpEdge( h ,h->GetNbinsX() ) ;};
  TH1D * GuardarEnTH1(const char* argv, const char* name);
  TH1D * GuardarEnTH1(const char* argv  , const char* name, int nbinx, double xlow , double xup );
  TH2D * GuardarEnTH2(const char* argv  , const char* name, int nbinx , double xlow , double xup, int nbiny, double ylow, double yup);
  TTree * GuardarEnTTree(const char* argv, const char* name, const char* title, const char* format);
  void ImprimirEnArchivoTexto(TH1 * , const char* name);
  void ImprimirAllPalette(TH2D *h2,  TCanvas * c2,TString modo, TStyle * S );
  double Integrar(TH1 * h);
  double Integrar(TH1 * h, double xlow, double xup);
  Style_t MarkerStyle(int i);
  void SuperponerHistogramas(TH1 * h1 , TH1 * h2);
  void SuperponerHistogramas(TH1 * h1 , TH1 * h2, double xlow, double xup);
  TH1D * ToD( TH1 * h);//Convertir histograma int a double
};

template<typename T>
void RootImprove::DefinirTitulos(T h, const char * title , const char * titlex ,const char * titley)
{
  h->SetTitle(title);

  h->GetYaxis()->SetTitle(titley);
  h->GetYaxis()->CenterTitle();
  h->GetXaxis()->SetTitle(titlex);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleOffset(0.9); //mover titulos ejes
  h->GetYaxis()->SetTitleOffset(0.9);
  h->GetXaxis()->SetTitleSize(0.065); //Tamaño titulos ejes
  h->GetYaxis()->SetTitleSize(0.065);
  
}



#endif
