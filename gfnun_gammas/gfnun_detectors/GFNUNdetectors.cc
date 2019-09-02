#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4EventManager.hh"
#include "G4UImanager.hh"

#include "G4Version.hh"

#include "G4VisExecutive.hh"
#if  G4VERSION_NUMBER>=930
#include "G4UIExecutive.hh"
#else
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#endif

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
@file PICompton.cc
@brief Main program
@author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
@version 2.5
@bug NONE so far
@date September, 2010
*/

/*! @brief Main routine

  This is the main routine of the simulation toolkit. It defines the main features of the simulation and drives 
  the overall behavior of the simulation. 
  @param argc the \e command to be entered using the macro files
  @param argv the \e value of the command just entered
 */

int main(int argc,char** argv)
{
  // Random engine
  // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  //! We MUST define the following in the main routine:
  //! \li The <tt>Run manager</tt>
  G4RunManager* runManager = new G4RunManager;

  //! \li Mandatory Initialization classes: <tt>DetectorConstruction</tt> and <tt>PhysicsList</tt>
  DetectorConstruction* Detector = new DetectorConstruction;
  runManager->SetUserInitialization(Detector);
  runManager->SetUserInitialization(new PhysicsList);
   
  //! \li Mandatory User Action classes: <tt>PrimaryGeneratorAction</tt>
  runManager->SetUserAction(new PrimaryGeneratorAction);
  
  runManager->SetUserAction(new SteppingAction);
  //! \li Optional User Actions: <tt>EventAction</tt> and <tt>RunAction</tt>
  EventAction* EvAction = new EventAction;
  runManager->SetUserAction(EvAction);
  runManager->SetUserAction(new RunAction(EvAction));
  

#ifdef G4VIS_USE
  //! \li Visualization manager <tt>G4VisManager</tt>
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1) {  // batch mode  
    
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
  }
  else {           // interactive mode : define UI session
     
#if  G4VERSION_NUMBER>=930
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute macros/visQt.mac");
    }
    else {
      //UImanager->ApplyCommand("/run/initialize");     
    }
#else
  #ifdef G4UI_USE_TCSH
    G4UIsession * ui = new G4UIterminal(new G4UItcsh);      
  #else
    G4UIsession * ui = new G4UIterminal();
  #endif
    //UImanager->ApplyCommand("/run/initialize");
    //UImanager->ApplyCommand("/control/execute macros/vis.mac");     
#endif
    ui->SessionStart();
    delete ui;     
  }
#ifdef G4VIS_USE
  delete visManager;
#endif

  //! @attention Free the store: user actions, physics_list and detector_description are
  //!                 owned and deleted by the run manager, so they should not
  //!                 be deleted in the main() program !
  delete runManager;

  return 0;
}
