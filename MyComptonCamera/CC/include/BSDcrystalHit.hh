/*! @file BSDcrystalHit.hh
  @brief  Definition of the BSDcrystalHit class
  @date   March,2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/

#ifndef BSDcrystalHit_h
#define BSDcrystalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

//#include "tls.hh"

class BSDcrystalHit : public G4VHit
{
public:
 
  BSDcrystalHit();
  BSDcrystalHit(G4VPhysicalVolume* pVol);
  BSDcrystalHit(G4VPhysicalVolume* pVol, G4int Detector_ID, G4int Gamma_ID, G4bool isPrimary);
  virtual ~BSDcrystalHit();
  BSDcrystalHit(const BSDcrystalHit &right);
  const BSDcrystalHit& operator=(const BSDcrystalHit &right);
  G4int operator==(const BSDcrystalHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
 
  virtual void Draw();
  virtual void Print();

  inline void SetEdep(G4double de)                { fEdep = de; }
  inline void AddEdep(G4double de)                { fEdep += de; }
  inline void SetPos(G4ThreeVector xyz)           { fPos = xyz; }
  inline void SetLocalTime(G4double local_time)   { flocalTime = local_time; }
  inline void SetGlobalTime(G4double global_time) { fglobalTime = global_time; }
  inline void SetFlagPhotoelectric(G4bool photo)  { fphotoelectric = photo; }
  inline void SetGammaID(G4int id)                { fGammaID = id; }
  
  inline G4double      GetEdep(void)              { return fEdep; }
  inline G4ThreeVector GetPos(void)               { return fPos; }
  inline G4double      GetLocalTime(void)         { return flocalTime; }
  inline G4double      GetGlobalTime(void)        { return fglobalTime; }
  inline const G4VPhysicalVolume* GetPhysV(void)  { return fPhysVol; }
  inline G4int        GetDetectorID()             { return fDetectorID; }
  inline G4int        GetGammaID()                { return fGammaID; }
  inline G4bool       GetIsPrimary()              { return fisPrimary; }
  inline G4bool       GetFlagPhotoelectric(void)  { return fphotoelectric; }
private:
  G4double      fEdep;
  G4ThreeVector fPos;
  G4double      flocalTime;
  G4double      fglobalTime;
  const G4VPhysicalVolume* fPhysVol;
  G4int         fDetectorID;
  G4bool        fisPrimary;
  G4bool        fphotoelectric;
  G4int         fGammaID; /// Useful to identify different gamma rays
};

typedef G4THitsCollection<BSDcrystalHit> BSDcrystalHitsCollection;

extern G4ThreadLocal G4Allocator<BSDcrystalHit>* BSDcrystalHitsAllocator;

inline void* BSDcrystalHit::operator new(size_t)
{
  if(!BSDcrystalHitsAllocator)
    BSDcrystalHitsAllocator = new G4Allocator<BSDcrystalHit>;
  return (void *) BSDcrystalHitsAllocator->MallocSingle();
}

inline void BSDcrystalHit::operator delete(void *aHit)
{
  BSDcrystalHitsAllocator->FreeSingle((BSDcrystalHit*) aHit);
}

#endif
