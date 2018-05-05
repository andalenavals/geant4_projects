
#include "RunActionMessenger.hh"
#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

RunActionMessenger::RunActionMessenger(RunAction* RunAct)
:runAct(RunAct)
{   
  runActDir = new G4UIdirectory("/CCFCF/run/");
  runActDir->SetGuidance("Run Action control.");

  rootNameCmd = new G4UIcmdWithAString("/CCFCF/run/ROOTfilename",this);
  rootNameCmd->SetGuidance("Select ROOT outputfile name.");
  rootNameCmd->SetParameterName("ROOT filename",true);
  rootNameCmd->AvailableForStates(G4State_Idle,G4State_PreInit);
}

RunActionMessenger::~RunActionMessenger()
{
  delete runActDir;
  delete rootNameCmd;
}


void RunActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if( command == rootNameCmd )
    runAct->SetRunName(newValue);
}
