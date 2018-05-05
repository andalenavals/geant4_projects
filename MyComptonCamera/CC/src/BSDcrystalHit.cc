/*! @file BSDcrystalHit.cc
  @brief  Implementation of user class BSDcrystalHit.
  @date   May,2016
  @author D. Flechas (dcflechasg@unal.edu.co)
*/
/* no-geant4 classes*/
#include "BSDcrystalHit.hh"
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

G4ThreadLocal G4Allocator<BSDcrystalHit>* BSDcrystalHitsAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BSDcrystalHit::BSDcrystalHit()
  : fEdep(0.), fPos(0.), fPhysVol(0), flocalTime(0), fglobalTime(0),fDetectorID(-1),fGammaID(-1),fisPrimary(false),fphotoelectric(false) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BSDcrystalHit::BSDcrystalHit(G4VPhysicalVolume* pVol)
  : fEdep(0.), fPos(0.), fPhysVol(pVol), flocalTime(0), fglobalTime(0),fDetectorID(-1),fGammaID(-1),fisPrimary(false),fphotoelectric(false) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BSDcrystalHit::BSDcrystalHit(G4VPhysicalVolume* pVol,G4int det_ID, G4int Gamma_ID, G4bool ispri)
  : fEdep(0.), fPos(0.), fPhysVol(pVol), flocalTime(0), fglobalTime(0),fDetectorID(det_ID),fGammaID(Gamma_ID),fisPrimary(ispri),fphotoelectric(false) {}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BSDcrystalHit::~BSDcrystalHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BSDcrystalHit::BSDcrystalHit(const BSDcrystalHit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  flocalTime = right.flocalTime;
  fglobalTime = right.fglobalTime;
  fPhysVol = right.fPhysVol;
  fDetectorID = right.fDetectorID;
  fGammaID = right.fGammaID;
  fisPrimary = right.fisPrimary;
  fphotoelectric = right.fphotoelectric;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const BSDcrystalHit& BSDcrystalHit::operator=(const BSDcrystalHit &right){
  fEdep = right.fEdep;
  fPos = right.fPos;
  flocalTime = right.flocalTime;
  fglobalTime = right.fglobalTime;
  fPhysVol = right.fPhysVol;
  fDetectorID = right.fDetectorID;
  fGammaID = right.fGammaID;
  fisPrimary = right.fisPrimary;
  fphotoelectric = right.fphotoelectric;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int BSDcrystalHit::operator==(const BSDcrystalHit&) const{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BSDcrystalHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BSDcrystalHit::Print()
{
  G4cout<<"Hit:DetectorID= "<<fDetectorID<<"\t eDep= "<<fEdep/keV<<" keV"<<(fisPrimary?"true":"false")<<" Gamma ID= "<<fGammaID<<"\t Photoelectric "<<fphotoelectric<<G4endl;
  G4cout<<"\tGlobal Time = "<<G4BestUnit(fglobalTime, "Time")<<"\tLocal Time = "<<G4BestUnit(flocalTime, "Time")<<G4endl<<G4endl;
}
