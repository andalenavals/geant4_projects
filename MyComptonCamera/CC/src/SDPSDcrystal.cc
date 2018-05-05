/*! @file SDPSDcrystal.cc
  @brief  Implements sensitive part (Crystals) of simulation.
  @date   March,2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/
/* no-geant4 classes*/
#include "SDPSDcrystal.hh"
#include "PSDcrystalHit.hh"
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

SDPSDcrystal::SDPSDcrystal(G4String name, G4int fNPixels)
  : G4VSensitiveDetector(name),gammaID(-1),NPixels(fNPixels)
{
  fPSDcrystalHitsCollection = NULL;
  collectionName.insert("psdcrystalHCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SDPSDcrystal::~SDPSDcrystal() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDPSDcrystal::Initialize(G4HCofThisEvent* hitsCE){
  //G4cout<<"SDcrystal in"<<G4endl;
  fPSDcrystalHitsCollection = new PSDcrystalHitsCollection
    (SensitiveDetectorName,collectionName[0]);
  //G4cout<<"SDcrystal out"<<G4endl;
  //A way to keep all the hits of this event in one place if needed
  static G4int hitsCID = -1;
  if(hitsCID<0){
    hitsCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitsCID, fPSDcrystalHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SDPSDcrystal::ProcessHits(G4Step* aStep,G4TouchableHistory* ){
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

  /// detector_ID = 0 -> BSDdetector
  if(detector_ID==-1)
    {
      if(thePrePV->GetName().contains("PSD"))
	{
	  G4int y = theTouchable->GetReplicaNumber(0); /* along +Y axis */
	  G4int x = theTouchable->GetReplicaNumber(1); /* along +X axis */
	  detector_ID = x + y*NPixels + 1; /// 1 and NPixels*NPixels are first one and last one
	  if( detector_ID<1 || detector_ID >= NPixels*NPixels+1)
	    {
	      G4cout<<"\nPOSITION: "<<"\tx = "<<x<<"\ty = "<<y<<G4endl;
	      G4cout<<"error with "<<thePrePV->GetName()<<"\t detector_ID = "<<detector_ID<<G4endl;
	      return false;
	    }
	}
      else
	{
	  G4cout<<"SDPSDcrystal::ProcessHits: ERROR: Detector ID has not been assigned -> Detector not found"<<G4endl;
	  return false;
	}
    }
  
  //Get the average position of the hit
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos/=2.;
  // store global and local time
  G4double  local_time = aStep->GetPostStepPoint()-> GetLocalTime();
  G4double global_time = aStep->GetPostStepPoint()->GetGlobalTime();
  
  PSDcrystalHit* crystalHit = new PSDcrystalHit(thePrePV,detector_ID,gammaID,isPrimary);

  crystalHit->SetEdep(edep);
  crystalHit->SetPos(pos);
  crystalHit->SetLocalTime(local_time);
  crystalHit->SetGlobalTime(global_time);
  if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="phot")
    crystalHit->SetFlagPhotoelectric(true);
  //crystalHit->Print();
  fPSDcrystalHitsCollection->insert(crystalHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDPSDcrystal::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDPSDcrystal::clear() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDPSDcrystal::DrawAll() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SDPSDcrystal::PrintAll() {} 
