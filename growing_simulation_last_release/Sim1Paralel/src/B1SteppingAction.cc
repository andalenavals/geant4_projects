#include "MyAnalysisManager.hh"

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{
}


B1SteppingAction::~B1SteppingAction()
{}

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();


  //Contar procesos


  
  // auto AManager = MyAnalysisManager::Instance();

  G4double edepStep = step->GetTotalEnergyDeposit();
  // check if we are in scoring volume
  if (volume != fScoringVolume)
    {
      //  AManager->FillCSV(0, Form("%3.3g MeV : Out",edepStep) );    
      return;
    }
  
  const G4VProcess * process = step->GetPostStepPoint()->GetProcessDefinedStep();

  TString nameout= "%3.3g MeV: ";
  nameout += static_cast<TString>(process->GetProcessName());

  // AManager->FillCSV(0, Form(nameout ,edepStep) );


  /**Iteracion para calcular la energia depositada en el scorer*/
  fEventAction->AddEdep(edepStep);
   
}

