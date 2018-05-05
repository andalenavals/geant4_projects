#include "B1PhysicsList.hh"

//a physics library se define que hereda metodos de G4VModularPhysicsList y Tambien de G4VUserPhysicsList
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

B1PhysicsList::B1PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());

}

B1PhysicsList::~B1PhysicsList()
{ 
}

void B1PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
