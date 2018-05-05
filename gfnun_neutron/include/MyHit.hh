#ifndef MyHit_h
#define MyHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "G4LogicalVolume.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MyHit : public G4VHit
{
public:
  //Le puse un argumento entero que mas tarde va a ser el numero del volumen detector...........A01..........Mayo 1
  //MyHit(G4int i);
  MyHit();
  
  ~MyHit();
  MyHit(const MyHit&);
  const MyHit& operator=(const MyHit&);
  G4int operator==(const MyHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
  
  void SetEdep     (G4double de)      { edep = de; };
  void SetPos      (G4ThreeVector xyz){ pos = xyz; };
  void SetParticleName (G4String Hname) { name = Hname;};
  //April 4-2009
  void SetKineticEnergy (G4double KE) { Kin_E = KE;};
  
  
  G4double GetEdep()    { return edep; };      
  G4ThreeVector GetPos(){ return pos; };
  G4String GetParticleName() {return name;};
  
  //April 4-2009
  G4double GetKineticEnergy()    { return Kin_E; };      
  //G4ThreeVector GetPosOld(){ return posOld; };
  //G4double GetPosDiff(){ return posDiff; };
  
  
private:
  
  G4double      edep;
  G4double      Kin_E;
  G4ThreeVector pos;
  G4String      name;
  
  //G4ThreeVector posOld;
  //G4double posDiff;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<MyHit> MyHitsCollection;

extern G4Allocator<MyHit> MyHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* MyHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) MyHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MyHit::operator delete(void *aHit)
{
  MyHitAllocator.FreeSingle((MyHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
