/**
 * @file   PhysicsList.hh
 *
 * @date   1 Jun 2010
 * @author Pico
 *
 * @brief  Physics list: particle definitions and processes.
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicsConstructor;
class PhysicsListMessenger;
class OpticalPhysics;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();

  void ConstructParticle();

  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);

  void SelectPhysicsList(const G4String& name);
  void ConstructProcess();

  void SetTargetCut(G4double val);
  void SetDetectorCut(G4double val);

private:

  void AddExtraBuilders(G4bool flagHP);

  // hide assignment operator
  PhysicsList & operator=(const PhysicsList &right);
  PhysicsList(const PhysicsList&);

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;

  G4VPhysicsConstructor*  emPhysicsList;
  G4VPhysicsConstructor*  raddecayList;
  G4VPhysicsConstructor*  particleList;
  G4VPhysicsConstructor*  hadPhysicsList;
  OpticalPhysics*         opticalPhysicsList;
  

  std::vector<G4VPhysicsConstructor*>  hadronPhys;
  G4int nhadcomp;  

  PhysicsListMessenger* pMessenger;
  G4ProductionCuts* DetectorCuts;
  G4ProductionCuts* TargetCuts;

  G4bool AbsorptionOn;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

