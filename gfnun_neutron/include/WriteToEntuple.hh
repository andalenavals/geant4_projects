//////////////////////////////////////////
// writeToEntuple 
// John Idarraga 08/2006
//////////////////////////////////////////


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1F.h>
#include "globals.hh"
#include <CLHEP/Units/SystemOfUnits.h>
//#include "HistoManager.hh"
#include "G4UnitsTable.hh"

class writeToNtuple {

public:
  writeToNtuple();
  ~writeToNtuple(){};
  void fillVars1(Float_t);
  void fillVars2(Float_t);
  void fillVars3(Float_t);
  void fillVars4(Float_t);
  void fillVars5(Float_t);
  void fillVars6(Float_t);
  void fillVars7(Float_t);
  void fillVars8(Float_t);
  void fillVars8_fast(Float_t);
  void fillVars8_gammas(Float_t);
  void fillVars8_thermal(Float_t);
  //April 22-2009, new functions to fill histograms with fixed number of bins.
  void fill_total(Float_t);
  void fill_fast(Float_t);
  void fill_thermal(Float_t);
  void fill_gammas(Float_t);
  
  //Dec 03-2009
  void fillVarsKE_before(Float_t);
  //Dec 10-2009
  void fillVarsKE_after(Float_t);
  
  void closeNtuple();
  
  //Thu Apr 22 20:28:58 COT 2010
  void  BinLogX(TH1*);
  
private:
  
  Float_t Energia_Depositada_en_1;
  Float_t Energia_Depositada_en_2;
  Float_t Energia_Depositada_en_3;
  Float_t Energia_Depositada_en_4;
  Float_t Energia_Depositada_en_5;
  Float_t Energia_Depositada_en_6;
  Float_t Energia_Depositada_en_7;
  Float_t Energia_Depositada_en_8;

  Float_t Energia_Depositada_en_8_fast;
  Float_t Energia_Depositada_en_8_gammas;
  Float_t Energia_Depositada_en_8_thermal;

  //April 22-2009
  Float_t Energia_Depositada_thermal;

  Float_t KinEDLM2_before;

  Float_t KinEDLM2_after;

  
  TFile *nt;
  TTree *t1;
  TTree *t2;
  TTree *t3;
  TTree *t4;
  TTree *t5;
  TTree *t6;
  TTree *t7;
  TTree *t8;
  
  TTree *t_fast;
  TTree *t_gammas;
  TTree *t_thermal;
  
  TH1F *total;
  TH1F *fast;
  TH1F *thermal;
  TH1F *gammas;
  //Dec. 03-2009
  TH1F *KinE_DLM2_before;
  //Dec 10-2009 ...
  TH1F *KinE_DLM2_after;
    
};

