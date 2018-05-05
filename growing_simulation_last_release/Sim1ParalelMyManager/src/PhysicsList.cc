#include "PhysicsList.hh"

//a physics library se define que hereda metodos de G4VModularPhysicsList y Tambien de G4VUserPhysicsList
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

PhysicsList::PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(0);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());

}

PhysicsList::~PhysicsList()
{ 
}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
