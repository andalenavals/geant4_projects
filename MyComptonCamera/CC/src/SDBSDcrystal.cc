/*! @file SDBSDcrystal.cc
  @brief  Implements sensitive part (Crystals) of simulation.
  @date   May,2016
  @author D. Flechas (dcflechasg@unal.edu.co)
*/
/* no-geant4 classes*/
#include "SDBSDcrystal.hh"
#include "BSDcrystalHit.hh"
/* geant4 classes*/
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDBSDcrystal::SDBSDcrystal(G4String name)
  : G4VSensitiveDetector(name),gammaID(-1)
{
  fBSDcrystalHitsCollection = NULL;
  collectionName.insert("bsdcrystalHCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDBSDcrystal::~SDBSDcrystal() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDBSDcrystal::Initialize(G4HCofThisEvent* hitsCE){
  //G4cout<<"SDcrystal in"<<G4endl;
  fBSDcrystalHitsCollection = new BSDcrystalHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //G4cout<<"SDcrystal out"<<G4endl;
  //A way to keep all the hits of this event in one place if needed
  static G4int hitsCID = -1;
  if(hitsCID<0){
    hitsCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitsCID, fBSDcrystalHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SDBSDcrystal::ProcessHits(G4Step* aStep,G4TouchableHistory* ){
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false; //No edep so dont count as hit

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable =
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
 
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  // Particle name
  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  // Creator process name
  G4String CreatorProcessName = "NONE";
  // Process name of the interaction, defined at the post estep point
  G4String ProcessName  = thePostPoint->GetProcessDefinedStep()->GetProcessName();
  
  //check if step is due to primary particle: it has track ID 1 and parent 0
  // The primary is the track with ID 1 and with no parent
  G4bool isPrimary = ( aStep->GetTrack()->GetTrackID() == 1 && aStep->GetTrack()->GetParentID() == 0 ) ? true : false;
    
  /// *** Gamma ID *** ///
  gammaID=-1;
  /// 0 : primary particle = gamma rays
  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
  /// 2 : origin = positron annihilation
  /// 3 : other particles
  if(isPrimary && particleName.contains("gamma"))
    gammaID=0;
  else
    {
      CreatorProcessName = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
      //!* Gamma ray emitted due radioactive decay: In addition, an energy condition could be necessary
      if( CreatorProcessName == "RadioactiveDecay" && particleName.contains("gamma") )
	  gammaID = 1;
      //!* Gamma rays from positron annihilation
      else if( CreatorProcessName =="annihil" && particleName.contains("gamma") )
	gammaID = 2; 
      else
	gammaID = 3; 
    }
  /* detector ID */
  G4int detector_ID=-1;

  if(detector_ID==-1)
    {
      if(thePrePV->GetName().contains("BSD"))  
	detector_ID = 0;
      else
	{
	  G4cout<<"SDBSDcrystal::ProcessHits: ERROR: Detector ID has not been assigned -> Detector not found"<<G4endl;
	  return false;
	}
    }
  
  //Get the average position of the hit
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos/=2.;
  // store global and local time
  G4double  local_time = aStep->GetPostStepPoint()-> GetLocalTime();
  G4double global_time = aStep->GetPostStepPoint()->GetGlobalTime();
  
  BSDcrystalHit* crystalHit = new BSDcrystalHit(thePrePV,detector_ID,gammaID,isPrimary);
  crystalHit->SetEdep(edep);
  crystalHit->SetPos(pos);
  crystalHit->SetLocalTime(local_time);
  crystalHit->SetGlobalTime(global_time);
  if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="phot")
    crystalHit->SetFlagPhotoelectric(true);
  //crystalHit->Print();
  fBSDcrystalHitsCollection->insert(crystalHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDBSDcrystal::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDBSDcrystal::clear() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDBSDcrystal::DrawAll() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDBSDcrystal::PrintAll() {} 
