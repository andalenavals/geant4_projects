/**
 * @file   RunActionMessenger.cc
 *
 * @date   31 Mar 2011
 * @author Pico
 *
 * @brief  Physics list: particle definitions and processes.
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyDigitizer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* RunAct)
:runAct(RunAct)
{   
  runActDir = new G4UIdirectory("/GFNUNDetectors/run/");
  runActDir->SetGuidance("Run Action control.");

  stepVerboseCmd = new G4UIcmdWithABool("/GFNUNDetectors/run/stepVerbose",this);
  stepVerboseCmd->SetGuidance("Select whether the SteppingAction gets verbose 'Pico Style'");
  stepVerboseCmd->SetParameterName("stepVerbose_flag",true);
  stepVerboseCmd->SetDefaultValue(false);
  stepVerboseCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  rootNameCmd = new G4UIcmdWithAString("/GFNUNDetectors/run/ROOTfilename",this);
  rootNameCmd->SetGuidance("Select ROOT outputfile name.");
  rootNameCmd->SetParameterName("ROOT filename",true);
  rootNameCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  asciiCmd = new G4UIcmdWithABool("/GFNUNDetectors/run/ASCIIoutput",this);
  asciiCmd->SetGuidance("Select whether the output will also be written in ASCII");
  asciiCmd->SetParameterName("ascii_flag",true);
  asciiCmd->SetDefaultValue(false);
  asciiCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  /** Fussy **/
  /** Default: false  **/
  interactionCmd = new G4UIcmdWithABool("/GFNUNDetectors/run/fussy",this);
  interactionCmd->SetGuidance("Select whether to save the information about the number of Compton interactions");
  interactionCmd->SetParameterName("fussy",true);
  interactionCmd->AvailableForStates(G4State_Idle,G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  delete runActDir;
  delete stepVerboseCmd;
  delete rootNameCmd;
  delete interactionCmd;
  delete asciiCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if( command == rootNameCmd )
    runAct->SetRunName(newValue);
  
  if( command == asciiCmd )
    { 
      G4bool flag = asciiCmd->GetNewBoolValue(newValue); 
      runAct->SetAsciiOutput(flag); 
    }

  if( command == stepVerboseCmd )
    {
      G4bool flag = stepVerboseCmd->GetNewBoolValue(newValue);
      G4RunManager* runManager = G4RunManager::GetRunManager();
      SteppingAction* stepping = (SteppingAction*)(runManager->GetUserSteppingAction());
      stepping->SetVerbose_flag(flag);
    }

  if(command == interactionCmd )
    {
      G4RunManager* runManager = G4RunManager::GetRunManager();
      SteppingAction* stepping = (SteppingAction*)(runManager->GetUserSteppingAction());
      stepping->SetInteraction_flag(interactionCmd->GetNewBoolValue(newValue));
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
