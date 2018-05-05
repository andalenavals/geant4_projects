
#include "HitDLM2.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<HitDLM2> HitDLM2Allocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HitDLM2::HitDLM2() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HitDLM2::~HitDLM2() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HitDLM2::HitDLM2(const HitDLM2& right)
  : G4VHit()
{
  Volume   = right.Volume;
  KinE     = right.KinE;
  pos       = right.pos;
  name      = right.name;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HitDLM2& HitDLM2::operator=(const HitDLM2& right)
{
  Volume   = right.Volume;
  KinE     = right.KinE;
  pos       = right.pos;
  name      = right.name; 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int HitDLM2::operator==(const HitDLM2& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HitDLM2::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Circle circle(pos);
      circle.SetScreenSize(2.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,0.,1.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HitDLM2::Print()
{
  G4cout <<"Particle name :  "<<name<<" " <<"Volume:  "<<Volume<<" Kinetic Energy: " <<G4BestUnit(KinE,"Energy") <<"  position: " << G4BestUnit(pos,"Length") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

