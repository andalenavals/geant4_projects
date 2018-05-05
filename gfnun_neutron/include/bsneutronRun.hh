

#ifndef bsneutronRun_h
#define bsneutronRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"

class G4Event;

class bsneutronRun : public G4Run{
public:
  bsneutronRun();
  virtual ~bsneutronRun();
  
  virtual void RecordEvent(const G4Event*);
  
private:
  G4int nEvent;
  
  G4int totalSurfCurrentID;
  
  G4THitsMap<G4double> totalSurfCurrent;
  
  G4THitsMap<G4double>* eventTotalSurfCurrent;
  
public:
  //... access methods ...
  void DumpScorer();
};
#endif
                                                     
