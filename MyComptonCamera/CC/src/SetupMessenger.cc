/**
 * @file
 *
 * @date   04 September, 2015
 * @author D. Flechas dcflechasg@unal.edu.co
 *
 * @brief Implements class SetupMessenger.
 */

#include "SetupMessenger.hh"
#include "SetupConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"
#include "PrimaryGeneratorAction.hh"

SetupMessenger::SetupMessenger(SetupConstruction * det)
:G4UImessenger(), detector(det), detDir(0),
 Plate_MaterialCmd(0),Plate_thicknessCmd(0),
 updateCmd(0)
{ 
  detDir = new G4UIdirectory("/CCFCF/");
  detDir->SetGuidance("SetupConstruction commands");

  /*********************/
  /** Plates Commands **/
  /*********************/
  // *******  Plate  ***** //
  /** Plate Composition: Default "iron"   **/
  Plate_MaterialCmd = new G4UIcmdWithAString("/CCFCF/plate/material",this);
  Plate_MaterialCmd->SetGuidance("Select plate material");
  Plate_MaterialCmd->SetParameterName("Plate_Material",true);
  Plate_MaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Plate Thickness: Default 5.*mm   **/
  Plate_thicknessCmd = new G4UIcmdWithADoubleAndUnit("/CCFCF/plate/thickness",this);
  Plate_thicknessCmd->SetGuidance("Set Plate thickness.");
  Plate_thicknessCmd->SetParameterName("Plate_thickness",false);
  Plate_thicknessCmd->SetDefaultUnit("mm");
  Plate_thicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

SetupMessenger::~SetupMessenger()
{
  delete Plate_MaterialCmd;
  delete Plate_thicknessCmd;
  delete updateCmd;
  delete detDir;
}

void SetupMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == Plate_MaterialCmd)
    detector->SetPlate_Material(newValue);
  if(command == Plate_thicknessCmd)
    detector->SetPlate_thickness(Plate_thicknessCmd->GetNewDoubleValue(newValue));

  if( command == updateCmd )
    detector->UpdateGeometry();
}

