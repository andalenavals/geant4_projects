#include "CsvManager.hh"

#include <string>

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"


#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4WorkerThread.hh"




B1RunAction::B1RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.)
{
  //Un analysis manager por hilo
  
  //auto AManager = MyAnalysisManager::Instance();

  //G4cout << G4Threading::G4GetThreadId() << G4endl ;
  //auto CsvManager = CsvManager::Instance();
 
  //AManager->SetFrame(0,0,2048,500, "","Energy [keV]", "Counts");
  // AManager->CreateTH1("Edep2", 2048, 0 , 2048);
  
 

  
  // add new units for dose
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2); 
}

B1RunAction::~B1RunAction()
{
  //delete AManager;
}

void B1RunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
 
  
  auto AManager = CsvManager::Instance();
  AManager->CreateCSV("out/fEdep_moments.dat"); //0
  AManager->FillCSV("*********************Begin of Run**************************");
  
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}



/**Al final de este método se presenta La fisica despues de que la
   simulacion halla corrido*/
void B1RunAction::EndOfRunAction(const G4Run* run)
{ 
  // auto AManager = MyAnalysisManager::Instance();
  // AManager->GetTH1(0)->Draw("Same");
  //AManager->Print("out/Energy.eps");
  //AManager->FillCSV(0, 200);
  //AManager->FillCSV(0,Form("Dosis Total: : %3.3g MeV ", fEdep.GetValue()) );
  //AManager->FillCSV(0, "*******************End of Run**************************");
  
  
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  //
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue(); 
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

  const DetectorConstruction* detectorConstruction
   = static_cast<const DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated dose per run, in scoring volume : " 
     << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

  

}

/** Suma al fEdep la energia depositada se llama al final de cada
    evento, util para calcular la dosis*/
void B1RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}


