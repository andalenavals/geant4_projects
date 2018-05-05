#define PICO_tree_cal_cxx
#include "PICO_tree_cal.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cmath>

/* PK begin modified */
#include "Riostream.h"
/* PK end modified */

void PICO_tree_cal::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L PICO_tree_cal.C
//      Root > PICO_tree_cal t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  //  nentries = 100;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    /*PK begin modified */
    if((jentry)%10000==0)
      cout<<"loop entry #: "<<jentry<<endl;

    multiplicity->Fill(Mult);

    for(Int_t mul=0;mul<Mult;mul++)
      {
	if(Energy[mul]>1274 && Energy[mul]<1275)
	  activity->Fill(Globaltime[mul]/3.1556926e16);
	
	if(det_ID[mul]==4096) /* BS */
	  {
	    double smear=0.;
	    backscatterred_spectrum->Fill(Energy[mul]);
	    do
	      smear = 6.62*sqrt(Energy[mul])*ran->Gaus(0,1/(2*std::sqrt(2*std::log(2))));
	    while( smear+Energy[mul] < 0) ;
	    
	    backscatterred_spectrum_2->Fill(smear+Energy[mul]);
	    multiplicity_2->Fill(Mult);
	    
	    //	    fChain->Show(jentry);
	    if(Mult<2)
	      break;
	    
	    for(Int_t mul_2=0;mul_2<Mult;mul_2++)
	      {
		if(det_ID[mul_2]!=4096)/* BS */
		  {
		    // cout<<"Posi (x,y) = ("<<det_ID[mul_2]/64<<","<<det_ID[mul_2]%64<<")"<<endl;
		    // cout<<"time diff with trigger = "<<Globaltime[mul_2]-Globaltime[mul]<<endl;
		    if(Mult==2)image->Fill(det_ID[mul_2]/64,det_ID[mul_2]%64);
		    if(Mult>2)image_high_M->Fill(det_ID[mul_2]/64,det_ID[mul_2]%64);
		    trigger_time->Fill(Globaltime[mul_2]-Globaltime[mul]); /* time is stored in ns from the RootSaver */
		  }
	      }
	  }
	
      }
  }
}
