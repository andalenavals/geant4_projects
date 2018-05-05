//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 17 19:24:51 2011 by ROOT version 5.30/01
// from TTree PICO_tree_cal/TTree contain PICOSpec CAL event info
// found on file: 213Ra_run0.root
//////////////////////////////////////////////////////////

#ifndef PICO_tree_cal_h
#define PICO_tree_cal_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

/* PK begin modified */
#include "TH1.h"
#include "TH2.h"
#include "Riostream.h"
#include "TRandom.h"
/* PK end modified */

class PICO_tree_cal {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Mult;
   Int_t           det_ID[15];   //[Mult]
   Double_t        Energy[15];   //[Mult]
   Double_t        Globaltime[15];   //[Mult]

   // List of branches
   TBranch        *b_Mult;   //!
   TBranch        *b_det_ID;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_Globaltime;   //!

   PICO_tree_cal(TTree *tree=0, TString root_file_out = TString("Compton.root"));
   virtual ~PICO_tree_cal();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   /* PK begin modified */
   TFile* f_out;

   TH1* activity;
   TH1* multiplicity;
   TH1* multiplicity_2;
   TH1* trigger_time;
   TH1* backscatterred_spectrum;
   TH1* backscatterred_spectrum_2;
   TH2* image;
   TH2* image_high_M;
   TRandom* ran;
   /* PK end modified */
};

#endif

#ifdef PICO_tree_cal_cxx
PICO_tree_cal::PICO_tree_cal(TTree *tree,TString root_file_out)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   Init(tree);

   /* PK begin modified */
   f_out = new TFile(root_file_out.Data(), "recreate");
   if(f_out->IsZombie())
     {
       cout<<"output file "<<root_file_out<<" NOT opened"<<endl;
       return;
     }
   else
     cout<<"file "<<root_file_out<<" opened"<<endl;
   
   activity = new TH1F("activity","activity;time[y];Counts/w",1000,0,20);
   multiplicity = new TH1F("multiplicity","Multiplicity of the events;multiplicity;counts",12,0-0.5,12-0.5);
   multiplicity_2 = new TH1F("multiplicity_2","Multiplicity of the events;multiplicity;counts",12,0-0.5,12-0.5);
   trigger_time = new TH1F("Trigger_diff_time","Time difference with trigger;t[ns];counts/ns",1000,-500-0.5,500-0.5);
   backscatterred_spectrum = new TH1F("Backscattering_spectrum","Backscattering detector, not smeared out;E[kev];counts/keV",2000,0-0.5,2000-0.5);
   backscatterred_spectrum_2 = new TH1F("Backscattering_spectrum_2","Backscattering detector, not smeared out;E[kev];counts/keV",2000,0-0.5,2000-0.5);
   image = new TH2I("Image_in_coincidence","X;Y;counts/pixel",64,0-0.5,64-0.5,64,0-0.5,64-0.5);
   image_high_M = new TH2I("Image_in_coincidence_high","X;Y;counts/pixel",64,0-0.5,64-0.5,64,0-0.5,64-0.5);
   
   ran = new TRandom();
   /* PK end modified  */
}

PICO_tree_cal::~PICO_tree_cal()
{
  /* PK begin modified */
  f_out->Write();
  f_out->Close();
  delete f_out;
  /* PK end modified  */
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t PICO_tree_cal::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PICO_tree_cal::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void PICO_tree_cal::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Mult", &Mult, &b_Mult);
   fChain->SetBranchAddress("det_ID", det_ID, &b_det_ID);
   fChain->SetBranchAddress("Energy", Energy, &b_Energy);
   fChain->SetBranchAddress("Globaltime", Globaltime, &b_Globaltime);
   Notify();
}

Bool_t PICO_tree_cal::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PICO_tree_cal::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PICO_tree_cal::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PICO_tree_cal_cxx
