/**
 * @file   RunActionMessenger.hh
 *
 * @date   31 Mar 2011
 * @author Pico
 *
 * @brief  RunActionMessenger.hh
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunActionMessensger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
public:
  
  RunActionMessenger(RunAction* );
  ~RunActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  RunAction* runAct;

  G4UIdirectory*             runActDir;
  G4UIcmdWithAString*        rootNameCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

