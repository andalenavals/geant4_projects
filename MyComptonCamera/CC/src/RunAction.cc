#include "EventAction.hh"
#include "RunAction.hh"
#include "RunActionMessenger.hh"

/* to calculate EM physical values*/
#include "G4Run.hh"
#include "G4EmCalculator.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//#include "G4UImanager.hh"
//#include "G4VVisManager.hh"
#include "G4ios.hh"

RunAction::RunAction(EventAction* theEventAction ) :
  eventAction(theEventAction)
{
  eventAction->SetRootSaver( &saver );
  messenger = new RunActionMessenger(this);
  rootfile="";
}


RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout<<"Start of Run: "<< aRun->GetRunID()<<G4endl;
  
  //For each run a new TTree is created, with default or custom name
  if(rootfile=="")
  {
    saver.Book();
  }
  else
    {
      saver.Book(rootfile,rootfile+"_Tree");
    }
}

void RunAction::EndOfRunAction( const G4Run* aRun )
{
  G4cout << "End of Run " << aRun->GetRunID() << G4endl;
  
  G4cout<<"RunAction::EndOfRunAction"<<G4endl;

  saver.Save();

}
