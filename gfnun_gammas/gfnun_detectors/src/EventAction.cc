/**
 * @file   EventAction.cc
 *
 * @date   17 Dec 2009
 * @author Pico
 * 
 * @brief  Implements user class EventAction.
 */

#include "EventAction.hh"
#include "RootSaver.hh"
#include "SteppingAction.hh"
#include "Interactions.hh"
#include "MyDigi.hh"
#include "MyHit.hh"
#include "MyDigitizer.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

EventAction::EventAction() :
	rootSaver(0),
	hitsCollName("MyHitCollection"),
	digitsCollName("MyDigitCollection"),
	hitsCollID(-1)
{
  //  G4cout<<"EventAction::EventAction"<<G4endl;
  //We build the digitization module
  MyDigitizer* digitizer = new MyDigitizer("MyDigitizer");
  G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
  digiManager->AddNewModule( digitizer );

  /* EventManager */ //
  eventManager = G4EventManager::GetEventManager();
 /* Stepping Action */  
  stepping = (SteppingAction*)(G4RunManager::GetRunManager()->GetUserSteppingAction());
  fussyInformation_flag = false;
}


void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
  keepEvent_bool=false;
  fussyInformation_flag = stepping ?  stepping->GetInteraction_flag() : false;
  if(fussyInformation_flag)
    stepping->ResetGammaAnnihilationCounter();

  //  G4cout<<"EventAction::BeginOfEventAction"<<G4endl;
   if ( anEvent->GetEventID() % 10000000 == 0 )
    {
      G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
      }
  //Retrieve the ID for the hit collection
  if ( hitsCollID == -1 )
    {
      G4SDManager * SDman = G4SDManager::GetSDMpointer();
      hitsCollID = SDman->GetCollectionID(hitsCollName);
    }
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  //  G4cout<<"EventAction::EndOfEventAction"<<G4endl;
  //Digitize!!
  G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
  MyDigitizer* digiModule = static_cast<MyDigitizer*>( digiManager->FindDigitizerModule("MyDigitizer") );
  if ( digiModule )
    {
      digiModule->Digitize();
    }
	
  //Store information
  if ( rootSaver )
    {
      //Retrieve digits collection
      G4int digiCollID = digiManager->GetDigiCollectionID( digitsCollName );
      const MyDigiCollection* digits = static_cast<const MyDigiCollection*>( digiManager->GetDigiCollection(digiCollID) );
      if(rootSaver->Trigger(digits))
	rootSaver->AddEvent(digits);
	
      ///* Keep special Events*///
      if(digits->entries()==100000) //** Multiplicity condition **//
     	SetKeepEvent(true);

      G4double energy_temp=0.;
      for ( G4int d = 0 ; d<digits->entries() ; d++ )
	{
	  const MyDigi* digi = static_cast<const MyDigi*>( digits->GetDigi( d ) );
	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  // ***** Detector ID ***** //
	  // dummy    = 0  
	  // Ge_10    = 1
	  // Ge_40    = 2
	  // NaI_2x2  = 3
	  // NaI_3x3  = 4
	  // Plastic  = 5
	  // CsI      = 6
	  // *********************** //
	  if(digi->GetDetectorID()==3)
	    energy_temp+=digi->GetCharge();
	}
      if(energy_temp>5000.0)//Energy condition in keV
	SetKeepEvent(true);

      if(keepEvent_bool==true)
	eventManager->KeepTheCurrentEvent();   
    }
  if(fussyInformation_flag)
    {
      Interactions* register_int_Comp;
      register_int_Comp = (Interactions*)(stepping->GetRegister_Int());
      for(G4int i=0;i<stepping->GetNumberOfInterstingGammaRays();i++)
	register_int_Comp[i].Clear();
    }
}


