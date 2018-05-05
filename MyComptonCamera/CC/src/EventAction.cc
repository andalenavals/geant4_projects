
#include "EventAction.hh"
#include "RootSaver.hh"
#include "BSDcrystalDigit.hh"
#include "PSDcrystalDigit.hh"
#include "BSDcrystalHit.hh"
#include "PSDcrystalHit.hh"
#include "PSDcrystalDigitizer.hh"
#include "BSDcrystalDigitizer.hh"
// #include "ControlVolDigit.hh"
// #include "ControlVolHit.hh"
// #include "ControlVolDigitizer.hh"

/* Geant4 */
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

// This file is a global variable in which we store energy deposition per hit
// and other relevant information

// CVhitsCollName("controlVolHCollection"),CVdigitsCollName("controlVolDCollection"), CVhitsCollID(-1)
EventAction::EventAction() 
  :  PSDcrystalhitsCollName("psdcrystalHCollection"), 	
     BSDcrystalhitsCollName("bsdcrystalHCollection"), 	
     PSDcrystaldigitsCollName("psdcrystalDCollection"),
     BSDcrystaldigitsCollName("bsdcrystalDCollection"),
     PSDcrystalhitsCollID(-1),
     BSDcrystalhitsCollID(-1),
     rootSaver(0),
     numofEdep(0)
{
  //We build the digitization module//

  PSDcrystalDigitizer* PSDdigitizer = new PSDcrystalDigitizer("psdcrystalDigitizer");
  BSDcrystalDigitizer* BSDdigitizer = new BSDcrystalDigitizer("bsdcrystalDigitizer");
  //  ControlVolDigitizer* CVdigitizer = new ControlVolDigitizer("controlVolDigitizer");
  G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
  digiManager->AddNewModule( PSDdigitizer );
  digiManager->AddNewModule( BSDdigitizer );
  //digiManager->AddNewModule( CVdigitizer );
  // EventManager //
  eventManager = G4EventManager::GetEventManager();
}

EventAction::~EventAction()
{
  G4cout<<"EventAction:: Number of edep into the crystal were: "<<numofEdep<<G4endl;
}

void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
  keepEvent_bool = false;
  //  G4cout<<"EventAction::BeginOfEventAction"<<G4endl;
  if ( anEvent->GetEventID() % 100000000 == 0 ) //?
    {
      G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
    }
  //Retrieve the ID for the hit collection
   
  if ( PSDcrystalhitsCollID == -1 )
    {
      G4SDManager * SDman = G4SDManager::GetSDMpointer();
      SDman->SetVerboseLevel(1);
      G4cout<<"Event action:: BeginOfEventAction: PSDcrystalhitsCollID is asigned"<<G4endl;
      PSDcrystalhitsCollID = SDman->GetCollectionID(PSDcrystalhitsCollName);
    }
  if ( BSDcrystalhitsCollID == -1 )
    {
      G4SDManager * SDman = G4SDManager::GetSDMpointer();
      SDman->SetVerboseLevel(1);
      G4cout<<"Event action:: BeginOfEventAction: BSDcrystalhitsCollID is asigned"<<G4endl;
      BSDcrystalhitsCollID = SDman->GetCollectionID(BSDcrystalhitsCollName);
    }
  // if ( CVhitsCollID == -1 )
  //   {
  // 	G4SDManager * SDman = G4SDManager::GetSDMpointer();
  // 	SDman->SetVerboseLevel(1);
  // 	G4cout<<"Event action:: BeginOfEventAction: CVhitsCollID is asigned"<<G4endl;
  // 	CVhitsCollID = SDman->GetCollectionID(CVhitsCollName);
  //   }
    
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  
  //Digitize//
  G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
  PSDcrystalDigitizer* PSDdigiModule = static_cast<PSDcrystalDigitizer*>( digiManager->FindDigitizerModule("psdcrystalDigitizer") );
  BSDcrystalDigitizer* BSDdigiModule = static_cast<BSDcrystalDigitizer*>( digiManager->FindDigitizerModule("bsdcrystalDigitizer") );
  //    ControlVolDigitizer* CVdigiModule = static_cast<ControlVolDigitizer*>( digiManager->FindDigitizerModule("controlVolDigitizer") );
  if ( PSDdigiModule ) PSDdigiModule->Digitize(); 
  if ( BSDdigiModule ) BSDdigiModule->Digitize(); 
  // if ( CVdigiModule )	CVdigiModule->Digitize();
  
  //Store information
  
  if ( rootSaver )
    {
      // Retrieve digits collection
      G4int BSDdigiCollID = digiManager->GetDigiCollectionID( BSDcrystaldigitsCollName );
      G4int PSDdigiCollID = digiManager->GetDigiCollectionID( PSDcrystaldigitsCollName );
      
      //      G4int CVdigiCollID = digiManager->GetDigiCollectionID( CVdigitsCollName );
      const BSDcrystalDigitsCollection* BSDdigits = static_cast<const BSDcrystalDigitsCollection*>( digiManager->GetDigiCollection(BSDdigiCollID) );
      const PSDcrystalDigitsCollection* PSDdigits = static_cast<const PSDcrystalDigitsCollection*>( digiManager->GetDigiCollection(PSDdigiCollID) );
      //      const ControlVolDigitsCollection* CVdigits = static_cast<const ControlVolDigitsCollection*>( digiManager->GetDigiCollection(CVdigiCollID) );
      
      if(rootSaver->Trigger(BSDdigits))
	{
	  //***! Each rootsaver function generates a new entry in the root tree
	  //rootSaver->AddEvent(CVdigits); // Only control volume
	  //rootSaver->AddEvent(BSDdigits); // Only BSD
	  //rootSaver->AddEvent(PSDdigits); // Only PSD 
	  rootSaver->AddEvent(BSDdigits,PSDdigits); // PSD + BSD
	  //if(PSDdigits->entries()>0) 
	  //  SetKeepEvent(true);
	}
      if(keepEvent_bool==true)
	eventManager->KeepTheCurrentEvent(); 
      /// Keep special Events///
      /*
	if(digits->entries()==100000) // Multiplicity condition 
     	SetKeepEvent(true);
      */
      
      // G4double energy_temp=0.0;
      // for ( G4int d = 0 ; d<PSDdigits->entries() ; d++ )
      // 	{
      // 	  const PSDcrystalDigit* digi = static_cast<const PSDcrystalDigit*>(PSDdigits->GetDigi(d));
      // 	  // Energy // remember charge is calibrated energy in keV
      // 	  // ***** Detector ID ***** //
      // 	  // BSD    = 0 
      // 	  // pixels of PSD = 1 to 4096
      // 	  // *********************** //
      // 	  if(digi->GetDetectorID()==3)
      // 	    energy_temp+=digi->GetCharge();
      // 	}
      // 	if(keepEvent_bool==true)
      // 	eventManager->KeepTheCurrentEvent(); 
    }
}

    
