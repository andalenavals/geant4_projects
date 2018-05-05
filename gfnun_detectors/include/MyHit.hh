#ifndef MyHit_h
#define MyHit_h 1

/**
 * @file
 * @brief Define user class MyHit.
 *
 * We define "our" hit format : it is a generic Hit characterized by 
 * a detector ID, detector name, energy deposited and interacion position
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

/*!
 * \brief This class stores information of a hit.
 *
 * It contains
 *  - detector ID
 *  - detector name
 *  - deposited energy
 */
class MyHit : public G4VHit {
public:
  /// Constructor
  MyHit(const G4int Detector_ID, const G4bool isPrimary, const G4String name);
  /// Destructor
  ~MyHit();
  //! Print on screen a Hit
  void Print();
  
public:
  //! \name The new and delete operators are overloaded for performances reasons:
  /*! -- Tricky business here... :-(, but provided for you below */
  //@{
  inline void *operator    new(size_t);
  inline void  operator delete(void *aHit);
  //@}

public:
  //! \name  simple set and get methods
  //@{
inline  void          AddEdep(const G4double e)                { eDep += e; }
inline  void          SetPosition(const G4ThreeVector & pos) { position = pos; }
inline  void          SetLocalTime(const G4double local_time) { localTime = local_time; }
inline  void          SetGlobalTime(const G4double global_time) { globalTime = global_time; }

inline  G4double      GetEdep()        const { return eDep;}
inline  G4ThreeVector GetPosition()    const { return position; }
inline  G4double      GetLocalTime()   const { return localTime; }
inline  G4double      GetGlobalTime()  const { return globalTime; }
inline  G4int         GetDetectorID()  const { return DetectorID; }
inline  G4String      GetDetectorName()const { return DetectorName;}
inline  G4bool        GetIsPrimary()   const { return isPrimary; }
  
  //@}

private:
  const G4int   DetectorID;
  const G4String DetectorName;
  G4double      eDep;
  G4ThreeVector position;
  G4double localTime;
  G4double globalTime;
  const G4bool  isPrimary;
};

// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<MyHit> MyHitCollection;

// -- new and delete overloaded operators:
extern G4Allocator<MyHit> MyHitAllocator;

inline void* MyHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) MyHitAllocator.MallocSingle();
  return aHit;
}
inline void MyHit::operator delete(void *aHit)
{
  MyHitAllocator.FreeSingle((MyHit*) aHit);
}

#endif
