/*! @file ControlVolHit.hh
  @brief  Definition of the ControlVolHit class
  @date   13 Oct 2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/

#ifndef ControlVolHit_h
#define ControlVolHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

//#include "tls.hh"

class ControlVolHit : public G4VHit
{
public:
  ControlVolHit();
  ControlVolHit(G4VPhysicalVolume* pVol);
  ControlVolHit(G4VPhysicalVolume* pVol, G4int Detector_ID, G4int Gamma_ID, G4bool follow);
  virtual ~ControlVolHit();
  ControlVolHit(const ControlVolHit &right);
  const ControlVolHit& operator=(const ControlVolHit &right);
  G4int operator==(const ControlVolHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
 
  virtual void Draw();
  virtual void Print();

  inline void SetEdep(G4double de)                { fEdep = de; }
  inline void AddEdep(G4double de)                { fEdep += de; }
  inline void AddEntry(void)                      { entries++; }
  inline void SetEntry(G4int en)                  { entries = en; }
  inline void SetPos(G4ThreeVector xyz)           { fPos = xyz; }
  inline void SetMomentum(G4ThreeVector xyz)      { fMomentum = xyz; }
  inline void SetLocalTime(G4double local_time)   { flocalTime = local_time; }
  inline void SetGlobalTime(G4double global_time) { fglobalTime = global_time; }
  inline void SetFlagPhotoelectric(G4bool photo)  { fphotoelectric = photo; }
  inline void SetFlagCompton(G4bool comp)         { fcompton = comp; }
  inline void SetDetectorID(G4int id)             { fDetectorID = id; }
  inline void SetGammaID(G4int id)                { fGammaID = id; }
  
  inline G4double      GetEdep(void)              { return fEdep; }
  inline G4ThreeVector GetPos(void)               { return fPos; }
  inline G4ThreeVector GetMomentum(void)          { return fMomentum; }
  inline G4double      GetLocalTime(void)         { return flocalTime; }
  inline G4double      GetGlobalTime(void)        { return fglobalTime; }
  inline const G4VPhysicalVolume* GetPhysV(void)  { return fPhysVol; }
  inline G4int        GetDetectorID()             { return fDetectorID; }
  inline G4int        GetGammaID()                { return fGammaID; }
  inline G4bool       GetFlagFollowParticle()     { return fFollowParticle; }
  inline G4bool       GetFlagPhotoelectric(void)  { return fphotoelectric; }
  inline G4bool       GetFlagCompton(void)        { return fcompton; }
  inline  G4int       GetEntries()          const { return entries; }
private:
  G4double      fEdep;
  G4ThreeVector fPos;
  G4ThreeVector fMomentum;   //! Delta of Momentum during the interaction
  G4double      flocalTime;
  G4double      fglobalTime;
  const G4VPhysicalVolume* fPhysVol;
  G4int         fDetectorID;
  G4bool        fFollowParticle;
  G4bool        fphotoelectric;
  G4bool        fcompton;
  G4int         entries; /// Number of energy depositions via Compton Effect
  G4int         fGammaID; /// Useful to identify different gamma rays
};

typedef G4THitsCollection<ControlVolHit> ControlVolHitsCollection;

extern G4ThreadLocal G4Allocator<ControlVolHit>* ControlVolHitsAllocator;

inline void* ControlVolHit::operator new(size_t)
{
  if(!ControlVolHitsAllocator)
    ControlVolHitsAllocator = new G4Allocator<ControlVolHit>;
  return (void *) ControlVolHitsAllocator->MallocSingle();
}

inline void ControlVolHit::operator delete(void *aHit)
{
  ControlVolHitsAllocator->FreeSingle((ControlVolHit*) aHit);
}

#endif
