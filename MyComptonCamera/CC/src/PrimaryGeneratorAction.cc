/**
 * @file
 * @brief implements mandatory user class PrimaryGeneratorAction
 */

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include <cmath>
using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), 
    particleGun(0),
    gun(0)
{
  G4int n_particle = 1;
  position = G4ThreeVector(0.0*cm,1.8516*cm-0.1*cm,0.0*cm);
  gun = new G4ParticleGun(n_particle);
  particleGun = new G4GeneralParticleSource ();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
  delete particleGun;
}

////////////////////////////////////////////
///// ********  Particle Gun  ******** /////
////////////////////////////////////////////
/*
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();  
  /////---------- Radioactive sources ---------////////////
  // G4IonTable* ionTable =  const_cast <G4IonTable *> (particleTable->GetIonTable());
  G4IonTable* ionTable =  G4IonTable::GetIonTable();

  gun->SetParticleDefinition(ionTable->GetIon(11,22,0.)); // 22Na
  //gun->SetParticleDefinition(particleTable->GetIon(25,54,0.)); // 54Mn
  //gun->SetParticleDefinition(particleTable->GetIon(27,60,0.)); // 60Co
  //gun->SetParticleDefinition(particleTable->GetIon(55,137,0.)); // 137Cs
  
  gun->SetParticleEnergy(0.*keV);
  // Location   
  gun->SetParticlePosition(position);
  gun->GeneratePrimaryVertex(anEvent);
  
  ///--------- gamma --------///
  
  // gun->SetParticleDefinition(particleTable->FindParticle("gamma"));

  // gun->SetParticleEnergy(1000.0*keV);
  //  gun->SetParticleMomentumDirection(G4ThreeVector(0,1,0));
  // gun->SetParticlePosition(position);
  // gun->GeneratePrimaryVertex(anEvent);
  
    ///----- Cone: gamma -> Klein-Nishina distribution ------///
  
  // gun->SetParticleDefinition(particleTable->FindParticle("gamma"));
  // G4double x = position.getX();
  // G4double E_gamma = 511*keV; 
  // Calculate_Theta_min(x);
  // ConstructAF(E_gamma);
  // G4double Theta=GetTheta();
  // G4double E = GetEnergy(Theta,E_gamma); // in keV
  //   G4double theta = (M_PI-Theta);
  // G4double phi = (G4UniformRand())*M_PI*2*rad;
  // G4double u1=sin(phi);
  // G4double u2=cos(phi);
  // G4double v1=sin(theta);
  // G4double v2=cos(theta);
  // G4ThreeVector direction = G4ThreeVector(v1*u2,v1*u1,v2);
  // direction.rotateY(-M_PI/2);
  
  // gun->SetParticleEnergy(E); // Energy in keV
  // gun->SetParticleMomentumDirection(direction);
  // gun->SetParticlePosition(position);
  // gun->GeneratePrimaryVertex(anEvent);
  
 }
*/

///////////////////////////////////////////////////////
///// ********  General Particle Source  ******** /////
///////////////////////////////////////////////////////

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}


