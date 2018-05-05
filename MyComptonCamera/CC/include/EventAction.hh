#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4String.hh"
#include "NoiseGenerator.hh"
#include "CrosstalkGenerator.hh"
#include "MeV2ChargeConverter.hh"

class G4Event;
class RootSaver;
class G4EventManager;

class EventAction : public G4UserEventAction
{
public:
  //! Default constructor
  EventAction();
  //! Default destructor
  virtual ~EventAction();
public:
  //! Beginning of event
  void BeginOfEventAction(const G4Event* anEvent);
  //! Digitize hits and store information
  void EndOfEventAction(const G4Event* anEvent);
  
  //! Set the RootSaver
  inline void SetRootSaver( RootSaver* saver ) { rootSaver = saver; }
  
  //! To assign the keepEvent_bool value
  inline void SetKeepEvent(G4bool kEvent=false){ keepEvent_bool=kEvent; };
  inline G4bool GetKeepEvent(void){ keepEvent_bool; };
  
private:
  
  //! pointer to saver object
  RootSaver* rootSaver;
  //! hits collection name 
  G4String PSDcrystalhitsCollName;
  G4String BSDcrystalhitsCollName;
  //G4String CVhitsCollName;
  
  //! digits collection name
  G4String PSDcrystaldigitsCollName;
  G4String BSDcrystaldigitsCollName;
  G4String CVdigitsCollName;
  
   //! Hits collection ID
  G4int PSDcrystalhitsCollID;
  G4int BSDcrystalhitsCollID;
  //  G4int CVhitsCollID;
  
  //! Kept event 
  G4bool keepEvent_bool;
  //! G4EventManager
  G4EventManager* eventManager;
  //! test: Counting edeps
  G4int numofEdep;
  
};

#endif /* EventAction_h 1 */
