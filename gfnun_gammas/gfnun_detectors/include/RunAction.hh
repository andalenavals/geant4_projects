/**
 * @file   RunAction.hh
 *
 * @date   17 Dec 2009
 * @author adotti
 */

#ifndef RUNACTION_HH_
#define RUNACTION_HH_

/*!
 * \file
 * \brief User's Run Action
 */
#include "G4UserRunAction.hh"
#include "RootSaver.hh"

class G4Run;
class EventAction;
class RunActionMessenger;

/*!
 * \brief User's RunAction class
 * This class implements the Run Action
 * The relevant method is \sa BeginOfRunAction and \sa EndOfRunAction
 * This class controls the saving facility (\sa RootSaver class), since
 * the handling of ROOT TTree is done at run level (each run one TTree)
 */
class RunAction : public G4UserRunAction
{
public:
	//! constructor
	RunAction( EventAction* evAct );
	//! destructor
	virtual ~RunAction() {};
	//! Called at the beginning of each run
	void BeginOfRunAction(const G4Run*);
	//! Called at the end of each run
	void EndOfRunAction(const G4Run*);

  inline void SetRunName( const G4String name ) { rootfile = name; }
  inline void SetAsciiOutput(const G4bool flag ) {ascii_flag = flag;}
  inline G4bool GetAsciiOutputFlag() {return ascii_flag;}
  
private:
	//! The ROOT TTree handler object
	RootSaver saver;
	//! Pointer to the EventAction
	EventAction* eventAction;

  G4String rootfile;
  G4bool ascii_flag;
  ofstream* ascii_file;
  
  RunActionMessenger* messenger;
  
};

#endif /* RUNACTION_HH_ */
