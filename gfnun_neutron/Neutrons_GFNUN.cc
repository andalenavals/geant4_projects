#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "bsneutronDetectorConstruction.hh"
#include "bsneutronPhysicsList.hh"
#include "bsneutronPrimaryGeneratorAction.hh"

#include "bsneutronEventAction.hh"
#include "bsneutronRunAction.hh"
#include "SteppingVerbose.hh"
#include "Randomize.hh"
#include <cmath>

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_WIN32
#include "G4UIWin32.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
//#include "Randomize.hh"
int main(int argc,char** argv)
{
  //PARA SESION NO INTERACTIVA
  
  //Mi propia clase de verbose
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
  
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  
  //choose the Random engine
  //HepRandom::setTheEngine(new RanecuEngine);
  // set mandatory initialization classes
  runManager->SetUserInitialization(new bsneutronDetectorConstruction);
  runManager->SetUserInitialization(new bsneutronPhysicsList);
  //  writeToNtuple* writeTo = new writeToNtuple();  
  //#########################AngelH 
#ifdef G4VIS_USE
  //visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  //################################
  // Initialize G4 kernel
  
  runManager->Initialize();
  // set mandatory user action class
  runManager->SetUserAction(new bsneutronPrimaryGeneratorAction);

  
  
  //Mayo 4................run action para lo de los hits
  
  runManager->SetUserAction(new bsneutronRunAction);
  
  runManager->SetUserAction(new bsneutronEventAction);
  
  
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  
  if (argc==1)   // Define UI session for interactive mode.
    {
    
      G4UIsession* session = new G4UIterminal(new G4UItcsh);
      session->SessionStart();
      delete session;
      //##
    } 
  
  else  //Batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }
  // job termination 
#ifdef G4VIS_USE
   delete visManager;
#endif
   delete runManager;
   //Ojo se debe comentar todo, 
   
     return 0;
}


