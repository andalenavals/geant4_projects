/*! @file SDControlVol.hh
  @brief  Defines sensitive part (ControlVols)of simulation.
  Definition of the SDControlVol class
  @date   13 Oct 2015
  @author D. Flechas (dcflechasg@unal.edu.co)
*/

#ifndef SDControlVol_h
#define SDControlVol_h 1

#include "ControlVolHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SDControlVol : public G4VSensitiveDetector
{
public:
  
  SDControlVol(G4String name);
  virtual ~SDControlVol();
 
  virtual void Initialize(G4HCofThisEvent* );
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
  virtual void EndOfEvent(G4HCofThisEvent* );
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();
 
private:

  ControlVolHitsCollection* fControlVolHitsCollection;
  G4int detID;
  G4int gammaID;
};

#endif
