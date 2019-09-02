/****************************************************************/
/****************************************************************/
/***** File created by Pico (L.G. Sarmiento) for the TASCA  *****/
/***** group @ GSI. August 2011.                            *****/
/***** Contact: lgsarmientop@unal.edu.co                    *****/
/****************************************************************/
/****************************************************************/

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "Riostream.h"

#include "PICO_tree_cal.h"

int main(int argc, char *argv[]){

  PICO_tree_cal* cal_tree;

  /** arguments checking **/
  if ( argc < 2 )
    {
      cout<<"usage: "<< argv[0] <<" <filename_input> [filename_output=Dirk.root]"<<endl;
      return 0;
    }
  
  /** check INPUT file **/
  TFile* f_in = new TFile(TString(argv[1]),"read");
  if(f_in->IsZombie())
    {
      cout<<"INPUT file "<<argv[1]<<" NOT opened for READING"<<endl;
      return 0;
    }
  else
    cout<<"input file "<<argv[1]<<" opened SUCCESSFULLY"<<endl;
  
  TTree* tree = (TTree*)f_in->FindObjectAny("GFNUNdetectors_tree_cal");     
  
  if(!tree)
    {
      cout<<"Error retrieving TTree "<<endl;
      return 0;
    }

  if(argc == 2)
    {
      cal_tree = new PICO_tree_cal(tree);  
    }
  else if (argc == 3)
    {
      cal_tree = new PICO_tree_cal(tree,TString(argv[2]));  
    }
  else
    {
      cout<<"usage: "<< argv[0] <<" <filename_input> [filename_output=Dirk.root]"<<endl;
      return 0;
    }

  cal_tree->Loop();
  //cal_tree->LoopTime();

  delete cal_tree;

  return 0;
}
