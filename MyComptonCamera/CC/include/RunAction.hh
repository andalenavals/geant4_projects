#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "RootSaver.hh"

class G4Run;
class EventAction;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
public:
  RunAction( EventAction* evAct );
  virtual ~RunAction();
  //! Called at the beginning of each run
  void BeginOfRunAction(const G4Run*);
  //! Called at the end of each run
  void EndOfRunAction(const G4Run*);
  
  inline void SetRunName( const G4String name ) { rootfile = name; }
  
private:
    //! Pointer to the EventAction
  EventAction* eventAction;
  //! The ROOT TTree handler object
  RootSaver saver;
  G4String rootfile;
  RunActionMessenger* messenger;
  
};

#endif
