#ifndef B1PhysicsList_h
#define B1PhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class B1PhysicsList: public G4VModularPhysicsList
{
public:
  B1PhysicsList();
  virtual ~B1PhysicsList();

  virtual void SetCuts();
};

#endif

