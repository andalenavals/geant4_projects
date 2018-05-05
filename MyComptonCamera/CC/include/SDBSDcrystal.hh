/*! @file SDBSDcrystal.hh
  @brief  Defines sensitive part (Crystals)of simulation.
  Definition of the SDBSDcrystal class
  @date   May,2016
  @author D. Flechas (dcflechasg@unal.edu.co)
*/

#ifndef SDBSDcrystal_h
#define SDBSDcrystal_h 1

#include "BSDcrystalHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SDBSDcrystal : public G4VSensitiveDetector
{
public:
  
  SDBSDcrystal(G4String name);
  virtual ~SDBSDcrystal();
 
  virtual void Initialize(G4HCofThisEvent* );
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
  virtual void EndOfEvent(G4HCofThisEvent* );
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();
 
private:

  BSDcrystalHitsCollection* fBSDcrystalHitsCollection;
  G4int gammaID;
};

#endif
