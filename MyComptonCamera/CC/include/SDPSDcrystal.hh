/*! @file SDPSDcrystal.hh
  @brief  Defines sensitive part (Crystals)of simulation.
  Definition of the SDPSDcrystal class
  @date   May,2016
  @author D. Flechas (dcflechasg@unal.edu.co)
*/

#ifndef SDPSDcrystal_h
#define SDPSDcrystal_h 1

#include "PSDcrystalHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SDPSDcrystal : public G4VSensitiveDetector
{
public:
  
  SDPSDcrystal(G4String name, G4int fNPixels = 64);
  virtual ~SDPSDcrystal();
 
  virtual void Initialize(G4HCofThisEvent* );
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
  virtual void EndOfEvent(G4HCofThisEvent* );
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();
 
private:

  PSDcrystalHitsCollection* fPSDcrystalHitsCollection;
  G4int gammaID;
  G4int NPixels;
};

#endif
