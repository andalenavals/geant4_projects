/**
 * @file   PhysicsListMessenger.hh
 *
 * @date   1 Jun 2010
 * @author Pico
 *
 * @brief  Physics list Messenger: particle definitions and processes.
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger: public G4UImessenger
{
public:
  
  PhysicsListMessenger(PhysicsList* );
  ~PhysicsListMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  PhysicsList* pPhysicsList;

  G4UIdirectory*             physDir;  
  G4UIcmdWithADoubleAndUnit* gammaCutCmd;
  G4UIcmdWithADoubleAndUnit* electCutCmd;
  G4UIcmdWithADoubleAndUnit* protoCutCmd;    
  G4UIcmdWithADoubleAndUnit* allCutCmd;    
  G4UIcmdWithADoubleAndUnit* mCutCmd;
  G4UIcmdWithADoubleAndUnit* eCutCmd;
  G4UIcmdWithAString*        pListCmd;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
