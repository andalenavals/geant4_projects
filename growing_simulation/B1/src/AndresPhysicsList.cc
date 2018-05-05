#include "AndresPhysicsList.hh"

//a physics library se define que hereda metodos de G4VModularPhysicsList y Tambien de G4VUserPhysicsList
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmPenelopePhysics.hh" 

AndresPhysicsList::AndresPhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  //RegisterPhysics(new G4EmStandardPhysics());

  //Penelope
  RegisterPhysics(new G4EmPenelopePhysics());

}

AndresPhysicsList::~AndresPhysicsList()
{ 
}

void AndresPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
