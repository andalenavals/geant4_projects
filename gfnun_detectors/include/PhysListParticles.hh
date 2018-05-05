/**
 * @file   PhysListParticles.hh
 *
 * @date   1 Jun 2010
 * @author Pico
 *
 * @brief  Physics list: particle definitions and processes.
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysListParticles_h
#define PhysListParticles_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysListParticles : public G4VPhysicsConstructor
{
public: 
  PhysListParticles(const G4String& name = "particles");
  virtual ~PhysListParticles();

public: 
  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method is dummy.
  virtual void ConstructProcess() {};

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif








