#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fParticleSource(0)
{
  //PARTICLE GUN
  //Gamma
  /*
  fParticleGun  = new G4ParticleGun(1);  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
  // G4ParticleDefinition* particle = particleTable->FindParticle("e+");
  fParticleGun->SetParticleDefinition(particle);
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(661.7*keV);
  */
}

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  /*
  fParticleGun  = new G4ParticleGun(1);
  fParticleGun->SetParticleDefinition(Ion);
  fParticleGun->SetParticleMomentum(G4ThreeVector(0.,0.,0.));
  */
  //La aleteorizacion de la direccion debe ir aca particulas no iones como gammas
  //fParticleGun->SetParticleMomentumDirection(G4RandomDirection());

  //Ion
  fParticleGun  = new G4ParticleGun(1);
  G4IonTable* IonTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* Ion = IonTable->GetIon(11,22,0); //22Na
  //G4ParticleDefinition* Ion = IonTable->GetIon(27,60,0); //60Co
  // G4ParticleDefinition* Ion = IonTable->GetIon(55,137,0); //137Cs
  fParticleGun->SetParticleDefinition(Ion);
  fParticleGun->SetParticleMomentum(G4ThreeVector(0.,0.,0.));
    
  G4double r0 = 1.*cm * G4UniformRand();
  G4double angle = G4UniformRand() * 360.0*deg;
  G4double x0 = std::cos(angle)*r0;
  G4double y0 = std::sin(angle)*r0;  
  G4double z0 = 20*cm+1.*cm * G4UniformRand(); 
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  

  
  //PARTICLE SOURCE
  /*
  fParticleSource = new G4GeneralParticleSource();
  G4IonTable* IonTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* Ion = IonTable->GetIon(11,22,0); //22Na
  // G4ParticleDefinition* Ion = IonTable->GetIon(27,60,0); //60Co
  // G4ParticleDefinition* Ion = IonTable->GetIon(55,137,0); //137Cs
  fParticleSource->SetParticleDefinition(Ion);
  //fParticleSource->SetParticleMomentum(G4ThreeVector(0.,0.,0.));

  fParticleSource->SetParticlePosition(G4ThreeVector(0,0,0));
  fParticleSource->GeneratePrimaryVertex(anEvent);
  */
  
 }


