/**
 * @file   SensitiveDetector.cc
 * @brief  Implements sensitive part of simulation.
 *
 * @date   1 Jun 2010
 * @author Pico
 */

#include "SensitiveDetector.hh"

#include "G4Step.hh"
#include "Randomize.hh"

#include "G4HCofThisEvent.hh"

#include "G4HCtable.hh"
#include "G4SDManager.hh"

#include "G4RunManager.hh"
#include "DetectorConstruction.hh"

#include "G4OpticalPhoton.hh"

SensitiveDetector::SensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  if (verboseLevel>1)
    G4cout << "oooOOOooo SensitiveDetector::SensitiveDetector oooOOOooo"<<G4endl;
  
  G4RunManager* runManager = G4RunManager::GetRunManager();
  Detector =
    (DetectorConstruction*)(runManager->GetUserDetectorConstruction());
  
  DetectorCounter = Detector->GetTotalDetectorNumber();
  maxNumOfDetector = Detector->GetMaximumNumberOfDetectors();
  conter_OpticalPhotons =0;
  // Adding ID numbers

  hit_array  = new G4int[maxNumOfDetector];
  // 'collectionName' is a protected data member of base class G4VSensitiveDetector.
  // Here we declare the name of the collection we will be using.
  collectionName.insert("MyHitCollection");
  // Note that we may add as many collection names we would wish: ie
  // a sensitive detector can have many collections.
}

SensitiveDetector::~SensitiveDetector() {
  if (verboseLevel>1)
    G4cout << "oooOOOooo SensitiveDetector::~SensitiveDetector oooOOOooo"<<G4endl;
  
  delete [] hit_array;
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
  //  if (verboseLevel>1)
  //    G4cout << "oooOOOooo SensitiveDetector::ProcessHits oooOOOooo"<<G4endl;
  // step is guaranteed to be in sensitive volumes : no need to check for volume
  
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName(); 
  // energy deposit in this step 
  G4double edep        = step->GetTotalEnergyDeposit();

  // get step points in world coordinate system
  G4ThreeVector point1 = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector point2 = step->GetPostStepPoint()->GetPosition();
  
  // randomize point of energy deposition
  G4ThreeVector pointE = point1 + G4UniformRand()*(point2 - point1);

  /***************************/
  /** Identify the detector **/
  /***************************/
  G4VPhysicalVolume* detector_phys = touchable->GetVolume();
  G4String detector_name = detector_phys->GetName();

  if( step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
      //conter_OpticalPhotons++; // test: Optical photons registered are?
      edep += step->GetPreStepPoint()->GetTotalEnergy();
      //G4cout<<"LLEGO!!!::"<<step->GetPreStepPoint()->GetTotalEnergy()<<" "<<edep<<" "<<conter_OpticalPhotons<<G4endl;
      //  G4cout<<"E = "<<edep/eV<<" # Op = "<<conter_OpticalPhotons<<G4endl;
    }
    
  //check if step is due to primary particle: it has track ID 1 and parent 0
  // The primary is the track with ID 1 and with no parent
  G4bool isPrimary = ( step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetParentID() == 0 ) ? true : false;

  if (edep <= 0.) return false;
  
  // ***** Detector ID ***** //
  // dummy    = 0  
  // Ge_10    = 1
  // Ge_40    = 2
  // NaI_2x2  = 3
  // NaI_3x3  = 4
  // Plastic  = 5
  // CsI  = 6
  // *********************** //

  /* detector ID */
  G4int detector_ID=-1;

  if(detector_name.contains("dummy"))  
    detector_ID = 0;
  else if(detector_name.contains("Ge_10"))  
    detector_ID = 1;
  else if(detector_name.contains("Ge_40"))  
    detector_ID = 2;
  else if(detector_name.contains("NaI_2x2"))  
    detector_ID = 3;
  else if(detector_name.contains("NaI_3x3"))  
    detector_ID = 4;
  else if(detector_name.contains("Plastic"))  
    detector_ID = 5;
  else if(detector_name.contains("CsI"))  
    detector_ID = 6;
  else
    return false;
  //G4cout<<"detector_name = "<<detector_name<<" detector_ID = "<<detector_ID<<" edep = "<<edep<<" MeV"<<G4endl; // Control line
  

  // This is a new hit             
  if (hit_array[detector_ID]==-1)
    {
      MyHit* hit = new MyHit(detector_ID,isPrimary,detector_name);
      // accumulate energy deposition
      hit->AddEdep(edep);
      // store position of energy deposition
      hit->SetPosition(pointE);
      // store global and local time
      G4double  local_time = step->GetPostStepPoint()-> GetLocalTime();
      G4double global_time = step->GetPostStepPoint()->GetGlobalTime();
      hit->SetLocalTime(local_time);
      hit->SetGlobalTime(global_time);
      
      hit_array[detector_ID] = hitCollection->insert(hit) -1;
    }
  else // This is not new. Probably this leads to build up things! since times are too damn long
    {
      (*hitCollection)
	[hit_array[detector_ID]]->AddEdep(edep);
      /* In order to obtain the freaking depth of the implantation */
      /* we also set the position everytime ;) */			
      (*hitCollection)
	[hit_array[detector_ID]]->SetPosition(pointE);
    }


  return true;
}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
//   if (verboseLevel>1)
//     G4cout << "oooOOOooo SensitiveDetector::Initialize oooOOOooo"<<G4endl;
  // ------------------------------
  // -- Creation of the collection
  // ------------------------------
  // -- collectionName[0] is "HitCollection", as declared in constructor
  // DetectorCounter = Detector->GetTotalDetectorNumber();
  //hit_array  = new G4int[DetectorCounter];
  hitCollection = new MyHitCollection(GetName(), collectionName[0]);

  static G4int HCID = -1;
  if (HCID<0) HCID = GetCollectionID(0); // <<-- this is to get an ID for collectionName[0]
  HCE->AddHitsCollection(HCID, hitCollection);

  for(G4int i=0; i<maxNumOfDetector; i++)
    hit_array[i]=-1;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel>1){
    G4cout << "EndOfEvent method of SD `" << GetName() << "' called." << G4endl;
    for (unsigned int i=0;i<hitCollection->GetSize();i++){ 
      (*hitCollection)[i]->Print();
    }
  }
}
