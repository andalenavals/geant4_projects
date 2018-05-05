#include "B1EventAction.hh"
#include "B1Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.)
{
  archivo.open("Energias_Events.dat");
  c1->cd();
} 

B1EventAction::~B1EventAction()
{
  archivo.close();
  //No quiero ver event de cambio de volumen
  //razon por la que se llena el primer bin.
  hist->SetBinContent(1,0); 
  hist->Draw();
  c1->Print("Energy_Events.eps");
  c1->Destructor();
}

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in B1Run
  B1Run* run  = static_cast<B1Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddEdep(fEdep);


  //Salida de Datos
  hist->Fill(fEdep);
  archivo<<fEdep<<endl;
  
}



