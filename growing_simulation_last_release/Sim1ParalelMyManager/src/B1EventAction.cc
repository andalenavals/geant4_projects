#include "CsvManager.hh"

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

  //auto AManager = MyAnalysisManager::Instance();
  //AManager->FillCSV(0,Form("Total Deposited In Scorer: %3.3g MeV ",fEdep)); //EndEvent

 
  //Salida de Datos
  /*
   if(fEdep!=0){
    
     // AManager->FillTH1(0, fEdep*1000 );
     //  AManager->FillCSV(0,fEdep*1000);
     
     AManager->FillTH1(0, AManager->RandomFWHM(fEdep*1000, 0.027, 1.2 , 1) );
     // AManager->FillCSV(0, AManager->RandomFWHM(fEdep*1000, 0.027, 1.2 , 1) );


     // Este metodo NO funciona en MULTITHREADED
     AManager->DrawAnimation(0, 1000); 
     }*/
 
}
