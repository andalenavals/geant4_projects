//#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
//#else
#include "G4RunManager.hh"
//#endif

#include "G4UImanager.hh"
#include "Randomize.hh"

#include "G4VModularPhysicsList.hh"

#include "SetupConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc,char** argv) {
 
  
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

   
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  //G4long seed = time(NULL);
  //CLHEP::HepRandom::setTheSeed(seed);
  
  //#ifdef G4MULTITHREADED
  //G4MTRunManager* runManager = new G4MTRunManager;
  //runManager->SetNumberOfThreads(4);
  //#else
  G4RunManager* runManager = new G4RunManager;
  //#endif

  G4VModularPhysicsList* physlist = new PhysicsList();
  runManager->SetUserInitialization(new SetupConstruction);
  runManager->SetUserInitialization(physlist);
  runManager->SetUserInitialization(new ActionInitialization);
  
  G4VisManager*  visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    //interactive mode
    UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}


