#ifndef AndresPhysicsList_h
#define AndresPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"

class AndresPhysicsList: public G4VModularPhysicsList
{
public:
  AndresPhysicsList();
  virtual ~AndresPhysicsList();

  virtual void SetCuts();
};

#endif

