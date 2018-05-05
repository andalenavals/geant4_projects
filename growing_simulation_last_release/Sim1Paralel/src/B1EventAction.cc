#include "Analysis.hh"
#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"


B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{

} 

B1EventAction::~B1EventAction()
{

}

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  //auto AManager = MyAnalysisManager::Instance();
  // AManager->FillCSV(0,"____________ Begin of Event ______________");//Begin Event
}

void B1EventAction::EndOfEventAction(const G4Event*)
{
  fRunAction->AddEdep(fEdep); /** Este es para calcular la dosis*/
 
  //Salida de Datos
  auto AManager = G4AnalysisManager::Instance();
  if(fEdep!=0)
    {
      AManager->FillH1(0, fEdep*1000 ); 
    }
 
}
