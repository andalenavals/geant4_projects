#include "B1DetectorConstruction.hh" //ConstruyeGeometriaDetector
#include "B1ActionInitialization.hh" //El silvatazo inicial
#include "B1PhysicsList.hh"
#include "AndresPhysicsList.hh"


#include "G4RunManager.hh" //Administrador de la carrera
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


int main(int argc,char** argv){
  
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  G4RunManager* runManager = new G4RunManager;
 
  runManager->SetUserInitialization(new B1DetectorConstruction());
  runManager->SetUserInitialization(new AndresPhysicsList);
  //runManager->SetUserInitialization(new B1PhysicsList);
  runManager->SetUserInitialization(new B1ActionInitialization()); //comienza la accion
  runManager->Initialize(); //Iniciar kernel

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive; //G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
    ui->SessionStart();
    delete ui;
  }
  
  delete visManager;
  delete runManager;

  return 0;
}

