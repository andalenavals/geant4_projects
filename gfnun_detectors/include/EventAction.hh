/**
 * @file   EventAction.hh
 *
 * @date   17 Dec 2009
 * @author adotti
 *
 * @brief  User's EventAction.
 */

#ifndef EVENTACTION_HH_
#define EVENTACTION_HH_


#include "G4UserEventAction.hh"
#include "G4String.hh"
#include "NoiseGenerator.hh"
#include "CrosstalkGenerator.hh"
#include "MeV2ChargeConverter.hh"
class G4Event;
class RootSaver;
class G4EventManager;
class SteppingAction;
/*!
 * \brief User's EventAction class
 * This class implements the Event Action.
 * The relevant method is \sa EndOfEventAction where the
 * digitization (\sa SiDigitizer class) and the storage of hits and digits
 * in a ROOT TTree are perfomed (\sa RootSaver class).
 */
class EventAction : public G4UserEventAction
{
public:
  //! Default constructor
  EventAction();
  //! Default destructor
  virtual ~EventAction() {};
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
  G4String hitsCollName;
  //! digits collection name
  G4String digitsCollName;
  //! Hits collection ID
  G4int hitsCollID;
  //! Kept event 
  G4bool keepEvent_bool;
  //! G4EventManager
  G4EventManager* eventManager;
 //! SteppingAction
  SteppingAction* stepping;
  //! Fussy flag event: Store information about number of Compton interactions between other.
  G4bool fussyInformation_flag;
};

#endif /* EVENTACTION_HH_ */
