#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <fstream>
#include "TH1I.h"
#include "TCanvas.h"

using namespace std;

class B1EventAction : public G4UserEventAction
{
  public:
    B1EventAction();
    virtual ~B1EventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    G4double  fEdep;
    ofstream archivo;
    TCanvas *c1= new TCanvas("c1", "" , 800 , 600);
    TH1I* hist = new TH1I("hist", "",200 , 0 , 2) ;
};

#endif

    

