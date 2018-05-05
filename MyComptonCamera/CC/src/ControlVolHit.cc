/*! @file ControlVolHit.cc
  @brief  Implementation of user class ControlVolHit.
  @date   13 Oct 2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/
/* no-geant4 classes*/
#include "ControlVolHit.hh"
/* geant4 classes*/
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<ControlVolHit>* ControlVolHitsAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlVolHit::ControlVolHit()
  : fEdep(0.), fPos(0.), fMomentum(0.), fPhysVol(0), flocalTime(0), fglobalTime(0),fDetectorID(-1),fGammaID(-1),fFollowParticle(false),fphotoelectric(false),fcompton(false),entries(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlVolHit::ControlVolHit(G4VPhysicalVolume* pVol)
  : fEdep(0.), fPos(0.), fMomentum(0.), fPhysVol(pVol), flocalTime(0), fglobalTime(0),fDetectorID(-1),fGammaID(-1),fFollowParticle(false),fphotoelectric(false),fcompton(false),entries(0) {}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlVolHit::ControlVolHit(G4VPhysicalVolume* pVol,G4int det_ID, G4int Gamma_ID, G4bool follow)
  :  fEdep(0.), fPos(0.), fMomentum(0.), fPhysVol(pVol), flocalTime(0), fglobalTime(0),fDetectorID(det_ID),fGammaID(Gamma_ID),fFollowParticle(follow),fphotoelectric(false),fcompton(false),entries(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlVolHit::~ControlVolHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlVolHit::ControlVolHit(const ControlVolHit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fMomentum = right.fMomentum;
  flocalTime = right.flocalTime;
  fglobalTime = right.fglobalTime;
  fPhysVol = right.fPhysVol;
  fDetectorID = right.fDetectorID;
  fGammaID = right.fGammaID;
  fFollowParticle = right.fFollowParticle;
  fphotoelectric = right.fphotoelectric;
  fcompton = right.fcompton;
  entries = right.entries;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ControlVolHit& ControlVolHit::operator=(const ControlVolHit &right){
  fEdep = right.fEdep;
  fPos = right.fPos;
  fMomentum = right.fMomentum;
  flocalTime = right.flocalTime;
  fglobalTime = right.fglobalTime;
  fPhysVol = right.fPhysVol;
  fDetectorID = right.fDetectorID;
  fGammaID = right.fGammaID;
  fFollowParticle = right.fFollowParticle;
  fphotoelectric = right.fphotoelectric;
  fcompton = right.fcompton;
  entries = right.entries;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ControlVolHit::operator==(const ControlVolHit&) const{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ControlVolHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ControlVolHit::Print()
{
  G4cout<<"Hit:DetectorID= "<<fDetectorID<<"\t eDep= "<<fEdep/keV<<" keV"<<(fFollowParticle?"true":"false")<<" Gamma ID= "<<fGammaID<<"\t Photoelectric "<<fphotoelectric<<"\n Compton "<<fcompton<<"\t entries = "<<entries<<G4endl;
  G4cout<<"\tGlobal Time = "<<G4BestUnit(fglobalTime, "Time")<<"\tLocal Time = "<<G4BestUnit(flocalTime, "Time")<<G4endl<<G4endl;
}
