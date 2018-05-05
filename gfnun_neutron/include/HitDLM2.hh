#ifndef HitDLM2_h
#define HitDLM2_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "G4LogicalVolume.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HitDLM2 : public G4VHit
{
public:
  //Le puse un argumento entero que mas tarde va a ser el numero del volumen detector...........A01..........Mayo 1
  //HitDLM2(G4inti);
  HitDLM2();
  
  ~HitDLM2();
  HitDLM2(const HitDLM2&);
  const HitDLM2& operator=(const HitDLM2&);
  G4int operator==(const HitDLM2&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
  
  void SetHitVolume (G4String HVolume) { Volume = HVolume;};
  void SetKineticEnergy (G4double KE) { KinE = KE;};
  void SetPos      (G4ThreeVector xyz){ pos = xyz; };
  
  //December 10-2009 name of the particle that is being procceced in the hit
  void SetParticleName (G4String Hname) { name = Hname;};
  G4String GetParticleName() {return name;};  


  G4String GetHitVolume() {return Volume;};
  G4double GetKineticEnergy()    { return KinE; };      
  G4ThreeVector GetPos(){ return pos; };
  
private:
  
  G4double      KinE;
  G4String      Volume;
  G4ThreeVector pos;
  G4String      name;

  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<HitDLM2> HitDLM2sCollection;

extern G4Allocator<HitDLM2> HitDLM2Allocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* HitDLM2::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HitDLM2Allocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void HitDLM2::operator delete(void *aHit)
{
  HitDLM2Allocator.FreeSingle((HitDLM2*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
