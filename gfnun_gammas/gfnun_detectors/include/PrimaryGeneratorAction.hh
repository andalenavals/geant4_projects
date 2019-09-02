#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

/**
 * @file
 *
 * @date   01 July, 2010
 * @author Pico
 *
 * @brief Defines mandatory user class PrimaryGeneratorAction.
 */

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include <fstream>
#include "G4ThreeVector.hh"

class G4VPrimaryGenerator;
class G4GeneralParticleSource;
/*!
\brief This mandatory user class provides the primary particle generator

Geant4 provides a number of predefined primary particle generator, to be utilised by the user.
 - G4ParticleGun
 - G4GeneralParticleSource

\sa GeneratePrimaries()
*/

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  //! constructor
  PrimaryGeneratorAction();
  //! destructor
  ~PrimaryGeneratorAction();
  //! defines primary particles (mandatory)
  void GeneratePrimaries(G4Event*);

  void SetPosition(G4ThreeVector pos){position = pos;};
  void SetTilting(G4double tilt){CameraTilting = tilt;};

  /// Efficiency correction: NaI(2x2) detector
  G4double GetEnergy(G4double angle, G4double e_gamma); // return the energy of scattering photon an angle Theta
  G4double GetTheta(void); // return the dispersion angle, calculated using the K-N distribution 
  void ConstructAF(G4double e_gamma); // Construct the acumulative function (of K-N) between 180 and (180-Theta_min) degrees.
  void Calculate_Theta_min(G4double x);
  G4double Calculate_KN(G4double theta, G4double e_gamma);
private:
  G4ParticleGun* gun;
  G4GeneralParticleSource* particleGun;
  G4ThreeVector position;
  G4double CameraTilting;

  /// Efficiency correction: NaI(2x2) detector
  G4int N_steps;
  G4double* KN_AF;//  Construct the acumulative function (of K-N), 1000 points between 180 and (180-Theta_min) degrees.
  G4double Energy;
  G4double Theta_min;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
