#include "B1ActionInitialization.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"

B1ActionInitialization::B1ActionInitialization()
 : G4VUserActionInitialization()
{}

B1ActionInitialization::~B1ActionInitialization()
{}

void B1ActionInitialization::BuildForMaster() const
{
  SetUserAction(new B1RunAction);
}

void B1ActionInitialization::Build() const
{
  SetUserAction(new B1PrimaryGeneratorAction);
  SetUserAction(new B1RunAction);
  
  B1EventAction* eventAction = new B1EventAction;
  SetUserAction(eventAction);
  
  SetUserAction(new B1SteppingAction(eventAction));
}
