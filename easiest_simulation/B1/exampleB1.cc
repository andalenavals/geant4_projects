#include "B1DetectorConstruction.hh" //ConstruyeGeometriaDetector
#include "B1ActionInitialization.hh" //El silvatazo inicial
#include "G4RunManager.hh" //Administrador de la carrera
#include "G4UImanager.hh"
#include "B1PhysicsList.hh"
#include "Randomize.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv){
  
  G4Random::setTheEngine(new CLHEP::RanecuEngine); 
  G4RunManager* runManager = new G4RunManager;

  runManager->SetUserInitialization(new B1DetectorConstruction());
  runManager->SetUserInitialization(new B1PhysicsList);
  runManager->SetUserInitialization(new B1ActionInitialization()); //comienza la accion
  runManager->Initialize(); //Iniciar kernel
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive; //G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

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
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

