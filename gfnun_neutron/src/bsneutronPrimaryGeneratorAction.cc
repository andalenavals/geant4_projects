
#include "bsneutronPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
bsneutronPrimaryGeneratorAction::bsneutronPrimaryGeneratorAction()
{
  particleGun = new G4GeneralParticleSource();
}

bsneutronPrimaryGeneratorAction::~bsneutronPrimaryGeneratorAction()
{

  // G4cout << "oooOOOooo Aqui termina bsneutronPrimaryGeneratorAction.cc oooOOOooo"<<G4endl;

  delete particleGun;
}

void bsneutronPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}


