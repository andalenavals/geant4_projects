/////////////////////////////////////////////////////////////
// *********************** Based on ***********************//
// $Id: GB02ActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
// \file GB02ActionInitialization.cc
// \brief Implementation of the GB02ActionInitialization class
// D.Flechas (Sep 2015)
///////////////////////////////////////////////////////////////

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
//#include "TrackingAction.hh"
//#include "SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  EventAction* event = new EventAction();
  SetUserAction(event);
  RunAction* run = new RunAction(event);
  SetUserAction(run);
  //SetUserAction(new TrackingAction());
  //SetUserAction(new SteppingAction());
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
