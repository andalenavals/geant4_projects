/*! @file SDControlVol.cc
  @brief  Implements sensitive part (ControlVols) of simulation.
  @date   13 Oct 2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/
/* no-geant4 classes*/
#include "SDControlVol.hh"
#include "ControlVolHit.hh"
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

SDControlVol::SDControlVol(G4String name)
  : G4VSensitiveDetector(name),
    detID(-1),gammaID(-1)
{
  fControlVolHitsCollection = NULL;
  collectionName.insert("controlVolHCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDControlVol::~SDControlVol() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDControlVol::Initialize(G4HCofThisEvent* hitsCE){
  //G4cout<<"SDcontrolVol in"<<G4endl;
  fControlVolHitsCollection = new ControlVolHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //G4cout<<"SDcontrolVol out"<<G4endl;
  //A way to keep all the hits of this event in one place if needed
  static G4int hitsCID = -1;
  if(hitsCID<0){
    hitsCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitsCID, fControlVolHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SDControlVol::ProcessHits(G4Step* aStep,G4TouchableHistory* ){
  G4double edep = aStep->GetDeltaEnergy();
  //if(edep==0.) return false; //No edep so dont count as hit
  // Pre and post step point
  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  // Thouchable volume
  G4TouchableHistory* theTouchable =
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
  // Track
  G4Track* fTrack = aStep->GetTrack();
  // Particle name
  G4String particleName = fTrack->GetDefinition()->GetParticleName();
  // Creator process name
  G4String CreatorProcessName = "NONE";
  // Process name of the interaction, defined at the post estep point
  G4String ProcessName  = thePostPoint->GetProcessDefinedStep()->GetProcessName();
  //!!*** Check if step is due to interesting particle, examples:::::
  G4bool followParticle = false;
  gammaID=-1;
  detID=-1;
  /// *** Gamma ID *** ///
  /// 0 : primary particle = gamma rays
  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
  /// 2 : origin = positron annihilation
  /// 3 : other particles
  
  if( aStep->GetTrack()->GetTrackID() == 1 && aStep->GetTrack()->GetParentID() == 0  && particleName.contains("gamma") )
    {//!!*** primary particle (pure gamma-ray emited(): it has track ID 1 and parent 0)
      followParticle = true;
      gammaID = 0; 
    }
  else
    {
      CreatorProcessName= fTrack->GetCreatorProcess()->GetProcessName();
      if( CreatorProcessName == "RadioactiveDecay" && particleName.contains("gamma") )
	{
	  //!* Gamma ray emitted due radioactive decay: In addition, an energy condition could be necessary
	  followParticle = true;
	  gammaID = 1; 
	}
      else if( CreatorProcessName =="annihil" && particleName.contains("gamma") )
	{
	  //!* Gamma rays from positron annihilation
	  followParticle = true;
	  gammaID = 2; 
	}
      else
	gammaID = 3; 
    }
  
  if(!followParticle) return false; //No interesting particles are tracking and dont count as hit
  /* detector ID */
  G4int detector_ID=-1;
  if(detector_ID==-1)
    {
      if(thePrePV->GetName().contains("Plate"))  
	{
	  detector_ID = -2; 
	  detID = detector_ID;
	}
      else
	{
	  G4cout<<"SDControlVol::ProcessHits: ERROR: ControlVol ID has not been assigned -> Volume not found"<<G4endl;
	  return false;
	}
    }
  //Get the average position of the hit
  G4ThreeVector pos = thePostPoint->GetPosition();
  //G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  //pos/=2.;
  G4ThreeVector momentum = aStep->GetDeltaMomentum();
  // store global and local time
  G4double  local_time = aStep->GetPostStepPoint()-> GetLocalTime();
  G4double global_time = aStep->GetPostStepPoint()->GetGlobalTime();
  
  ControlVolHit* controlVolHit = new ControlVolHit(thePrePV,detID,gammaID,followParticle);

  // For interesting particles, the first step initilize the entries counter //
  //if(aStep->IsFirstStepInVolume())
  //  controlVolHit->SetEntry(0);

  controlVolHit->SetEdep(edep);
  controlVolHit->SetPos(pos);
  controlVolHit->SetMomentum(momentum);
  controlVolHit->SetLocalTime(local_time);
  controlVolHit->SetGlobalTime(global_time);
  if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="phot")
    controlVolHit->SetFlagPhotoelectric(true);
  if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="compt")
    {
      controlVolHit->SetFlagCompton(true);
      controlVolHit->AddEntry();
    }
  //controlVolHit->Print();
  fControlVolHitsCollection->insert(controlVolHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDControlVol::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDControlVol::clear() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDControlVol::DrawAll() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDControlVol::PrintAll() {} 
