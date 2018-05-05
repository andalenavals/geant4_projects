
/**
Esta clase define los guns, las particulas, cuantas particulas por
tiro, es importante tener en cuenta que esta clase hereda metodos de
G4VUserPrimaryGeneratorAction en particular GeneratePrimaries
*/

#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    B1PrimaryGeneratorAction();    
    virtual ~B1PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
 
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
  G4ParticleGun*  fParticleGun; 
  G4GeneralParticleSource*  fParticleSource;
  G4Box* fEnvelopeBox;  
};


#endif
