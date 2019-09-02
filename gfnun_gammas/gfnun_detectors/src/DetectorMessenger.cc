/**
 * @file
 *
 * @date   01 July, 2010
 * @author Pico
 *
 * @brief Implements class DetectorMessenger.
 */

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

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
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyDigitizer.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
:detector(det)
{ 
  detDir = new G4UIdirectory("/GFNUNDetectors/");
  detDir->SetGuidance("GFNUN detectors DetectorConstruction commands");

  /**  Optical photons  **/
  /** default: false**/
  opticalCmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/opticalphotons",this);
  opticalCmd ->SetGuidance("Select whether the optical photon will be taken into account");
  opticalCmd ->SetParameterName("optical_photons",true);
  opticalCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /*********************/
  /** Plates Commands **/
  /*********************/

  // *******  Plate  ***** //

  /** Plate Composition: Default "iron"   **/
  Plate_MaterialCmd = new G4UIcmdWithAString("/GFNUNDetectors/plate/material",this);
  Plate_MaterialCmd->SetGuidance("Select plate material");
  Plate_MaterialCmd->SetParameterName("Plate_Material",true);
  Plate_MaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Plate Thickness: Default 5.*mm   **/
  Plate_thicknessCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/plate/thickness",this);
  Plate_thicknessCmd->SetGuidance("Set Plate thickness.");
  Plate_thicknessCmd->SetParameterName("Plate_thickness",false);
  Plate_thicknessCmd->SetDefaultUnit("mm");
  Plate_thicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  /***********************/
  /** Detector Commands **/
  /***********************/

  ////////////////////////
  /** Ge (10%) detector */
  ////////////////////////

  placeGe10Cmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/Ge10/placeDetector",this);
  placeGe10Cmd ->SetGuidance("Select whether the Ge (10%) detector is placed");
  placeGe10Cmd ->SetParameterName("placeGe10Detector",true);
  placeGe10Cmd ->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  /** Position: Default 0,0,0 **/
  /** measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  Ge10PositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/Ge10/position",this);
  Ge10PositionCmd ->SetGuidance("Set starting position of the Ge (10%) detector.");
  Ge10PositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  Ge10PositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  Ge10rotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Ge10/rotationX",this);
  Ge10rotationXCmd ->SetGuidance("Set Ge (10%) detector tilting angle, around X axis.");
  Ge10rotationXCmd->SetParameterName("rotationX",false);
  Ge10rotationXCmd->SetUnitCategory("Angle");
  Ge10rotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  Ge10rotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Ge10/rotationY",this);
  Ge10rotationYCmd ->SetGuidance("Set Ge (10%) detector tilting angle, around Y axis.");
  Ge10rotationYCmd->SetParameterName("rotationY",false);
  Ge10rotationYCmd->SetUnitCategory("Angle");
  Ge10rotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  ////////////////////////
  /** Ge (40%) detector */
  ////////////////////////
 
  placeGe40Cmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/Ge40/placeDetector",this);
  placeGe40Cmd ->SetGuidance("Select whether the Ge (40%) detector is placed");
  placeGe40Cmd ->SetParameterName("placeGe40Detector",true);
  placeGe40Cmd ->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  Ge40PositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/Ge40/position",this);
  Ge40PositionCmd ->SetGuidance("Set starting position of the Ge (40%) detector.");
  Ge40PositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  Ge40PositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  Ge40rotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Ge40/rotationX",this);
  Ge40rotationXCmd ->SetGuidance("Set Ge (40%) detector tilting angle, around X axis.");
  Ge40rotationXCmd->SetParameterName("rotationX",false);
  Ge40rotationXCmd->SetUnitCategory("Angle");
  Ge40rotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  Ge40rotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Ge40/rotationY",this);
  Ge40rotationYCmd ->SetGuidance("Set Ge (40%) detector tilting angle, around Y axis.");
  Ge40rotationYCmd->SetParameterName("rotationY",false);
  Ge40rotationYCmd->SetUnitCategory("Angle");
  Ge40rotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  ///////////////////////
  /** NaI 2x2 detector */
  ///////////////////////

  placeNaI2x2Cmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/NaI2x2/placeDetector",this);
  placeNaI2x2Cmd ->SetGuidance("Select whether the NaI (2x2 crystal) detector is placed");
  placeNaI2x2Cmd ->SetParameterName("placeNaI2x2Detector",true);
  placeNaI2x2Cmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  NaI2x2PositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/NaI2x2/position",this);
  NaI2x2PositionCmd ->SetGuidance("Set starting position of the NaI (2x2 crystal) Detector.");
  NaI2x2PositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  NaI2x2PositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  NaI2x2rotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/NaI2x2/rotationX",this);
  NaI2x2rotationXCmd ->SetGuidance("Set NaI (2x2 crystal) detector tilting angle, around X axis.");
  NaI2x2rotationXCmd->SetParameterName("rotationX",false);
  NaI2x2rotationXCmd->SetUnitCategory("Angle");
  NaI2x2rotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  NaI2x2rotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/NaI2x2/rotationY",this);
  NaI2x2rotationYCmd ->SetGuidance("Set NaI (2x2 crystal) detector tilting angle, around Y axis.");
  NaI2x2rotationYCmd->SetParameterName("rotationY",false);
  NaI2x2rotationYCmd->SetUnitCategory("Angle");
  NaI2x2rotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  ///////////////////////
  /** NaI 3x3 detector */
  ///////////////////////

  placeNaI3x3Cmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/NaI3x3/placeDetector",this);
  placeNaI3x3Cmd ->SetGuidance("Select whether the NaI (3x3 crystal) detector is placed");
  placeNaI3x3Cmd ->SetParameterName("placeNaI3x3Detector",true);
  placeNaI3x3Cmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  NaI3x3PositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/NaI3x3/position",this);
  NaI3x3PositionCmd ->SetGuidance("Set starting position of the NaI (3x3 crystal) Detector.");
  NaI3x3PositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  NaI3x3PositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  NaI3x3rotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/NaI3x3/rotationX",this);
  NaI3x3rotationXCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around X axis.");
  NaI3x3rotationXCmd->SetParameterName("rotationX",false);
  NaI3x3rotationXCmd->SetUnitCategory("Angle");
  NaI3x3rotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  NaI3x3rotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/NaI3x3/rotationY",this);
  NaI3x3rotationYCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around Y axis.");
  NaI3x3rotationYCmd->SetParameterName("rotationY",false);
  NaI3x3rotationYCmd->SetUnitCategory("Angle");
  NaI3x3rotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

 ///////////////////////
  /** Plastic detector */
  ///////////////////////

  placePlasticCmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/Plastic/placeDetector",this);
  placePlasticCmd ->SetGuidance("Select whether the NaI (3x3 crystal) detector is placed");
  placePlasticCmd ->SetParameterName("placePlasticDetector",true);
  placePlasticCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  PlasticPositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/Plastic/position",this);
  PlasticPositionCmd ->SetGuidance("Set starting position of the NaI (3x3 crystal) Detector.");
  PlasticPositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  PlasticPositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  PlasticrotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Plastic/rotationX",this);
  PlasticrotationXCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around X axis.");
  PlasticrotationXCmd->SetParameterName("rotationX",false);
  PlasticrotationXCmd->SetUnitCategory("Angle");
  PlasticrotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  PlasticrotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Plastic/rotationY",this);
  PlasticrotationYCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around Y axis.");
  PlasticrotationYCmd->SetParameterName("rotationY",false);
  PlasticrotationYCmd->SetUnitCategory("Angle");
  PlasticrotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  ///////////////////////
  /** CsI detector */
  ///////////////////////

  placeCsICmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/CsI/placeDetector",this);
  placeCsICmd ->SetGuidance("Select whether the NaI (3x3 crystal) detector is placed");
  placeCsICmd ->SetParameterName("placeCsIDetector",true);
  placeCsICmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  CsIPositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/CsI/position",this);
  CsIPositionCmd ->SetGuidance("Set starting position of the NaI (3x3 crystal) Detector.");
  CsIPositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  CsIPositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0 **/
  CsIrotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/CsI/rotationX",this);
  CsIrotationXCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around X axis.");
  CsIrotationXCmd->SetParameterName("rotationX",false);
  CsIrotationXCmd->SetUnitCategory("Angle");
  CsIrotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0 **/
  CsIrotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/CsI/rotationY",this);
  CsIrotationYCmd ->SetGuidance("Set NaI (3x3 crystal) detector tilting angle, around Y axis.");
  CsIrotationYCmd->SetParameterName("rotationY",false);
  CsIrotationYCmd->SetUnitCategory("Angle");
  CsIrotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /////////////////////
  /** Dummy detector */
  /////////////////////

  placeDummyCmd = new G4UIcmdWithABool("/GFNUNDetectors/detector/Dummy/placeDetector",this);
  placeDummyCmd ->SetGuidance("Select whether the Dummy detector is placed");
  placeDummyCmd ->SetParameterName("placeDummyDetector",true);
  placeDummyCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  DummyPositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/detector/Dummy/position",this);
  DummyPositionCmd ->SetGuidance("Set starting position of the Dummy Detector.");
  DummyPositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  DummyPositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0,0 **/
  DummyrotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Dummy/rotationX",this);
  DummyrotationXCmd ->SetGuidance("Set dummy detector tilting angle, around X axis.");
  DummyrotationXCmd->SetParameterName("rotationX",false);
  DummyrotationXCmd->SetUnitCategory("Angle");
  DummyrotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0,0 **/
  DummyrotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/detector/Dummy/rotationY",this);
  DummyrotationYCmd ->SetGuidance("Set dummy detector tilting angle, around Y axis.");
  DummyrotationYCmd->SetParameterName("rotationY",false);
  DummyrotationYCmd->SetUnitCategory("Angle");
  DummyrotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  ////////////////////////////////////////////////////////////////////
  /** Castle shielding: paraffin + lead (Shielding of 252Cf source) */
  ////////////////////////////////////////////////////////////////////

  /** Position: Default 0,0,0 **/
  /**  measured to the center of the base  **/
  /*******  Three vector position *******/
  CastleShieldingPositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/ShieldingCastle/position",this);
  CastleShieldingPositionCmd ->SetGuidance("Set starting position of the 252Cf shielding (castle).");
  CastleShieldingPositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  CastleShieldingPositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /*******  Thickness of Lead Wall *******/
  CastleShielding_LeadThicknessCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/ShieldingCastle/wallleadthick",this);
  CastleShielding_LeadThicknessCmd->SetGuidance("Set Thickness of Lead Wall");
  CastleShielding_LeadThicknessCmd->SetParameterName("CSWallThickness",false);
  CastleShielding_LeadThicknessCmd->SetDefaultUnit("mm");
  CastleShielding_LeadThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  /*******  Thickness of Lead Roof *******/
  CastleShielding_LeadRoofThicknessCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/ShieldingCastle/roofleadthick",this);
  CastleShielding_LeadRoofThicknessCmd->SetGuidance("Set Thickness of Lead Roof");
  CastleShielding_LeadRoofThicknessCmd->SetParameterName("CSRoofThickness",false);
  CastleShielding_LeadRoofThicknessCmd->SetDefaultUnit("mm");
  CastleShielding_LeadRoofThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  /*******  Radius of Paraffin block *******/
  CastleShielding_ParaffinRadiusCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/ShieldingCastle/paraffinradius",this);
  CastleShielding_ParaffinRadiusCmd->SetGuidance("Set Radius  of Paraffin block");
  CastleShielding_ParaffinRadiusCmd->SetParameterName("CSParaffinRadius",false);
  CastleShielding_ParaffinRadiusCmd->SetDefaultUnit("mm");
  CastleShielding_ParaffinRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  /******* Height of Paraffin block *******/
  CastleShielding_HeightCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/ShieldingCastle/height",this);
  CastleShielding_HeightCmd->SetGuidance("Set Thickness of Lead Roof");
  CastleShielding_HeightCmd->SetParameterName("CSParaffinHeight",false);
  CastleShielding_HeightCmd->SetDefaultUnit("mm");
  CastleShielding_HeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ///////////////////////////////////////
  /** Source capsule: source container */
  ///////////////////////////////////////

  /** Position: Default 0,0,0 **/
  /**  measured to the frontal face (crystal) **/
  /*******  Three vector position *******/
  SourceCapsulePositionCmd = new G4UIcmdWith3VectorAndUnit("/GFNUNDetectors/source/position",this);
  SourceCapsulePositionCmd ->SetGuidance("Set starting position of the SourceCapsule Detector.");
  SourceCapsulePositionCmd ->SetParameterName("cam_X","cam_Y","cam_Z",true);
  SourceCapsulePositionCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around X axis: Default 0,0 **/
  SourceCapsulerotationXCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/source/capsule/rotationX",this);
  SourceCapsulerotationXCmd ->SetGuidance("Set source capsule tilting angle, around X axis.");
  SourceCapsulerotationXCmd->SetParameterName("rotationX",false);
  SourceCapsulerotationXCmd->SetUnitCategory("Angle");
  SourceCapsulerotationXCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);

  /** Rotation around Y axis: Default 0,0 **/
  SourceCapsulerotationYCmd = new G4UIcmdWithADoubleAndUnit("/GFNUNDetectors/source/capsule/rotationY",this);
  SourceCapsulerotationYCmd ->SetGuidance("Set source capsule tilting angle, around Y axis.");
  SourceCapsulerotationYCmd->SetParameterName("rotationY",false);
  SourceCapsulerotationYCmd->SetUnitCategory("Angle");
  SourceCapsulerotationYCmd ->AvailableForStates(G4State_PreInit,G4State_Idle);


  /** this command MUST be issued before beamOn **/
  updateCmd = new G4UIcmdWithoutParameter("/GFNUNDetectors/update",this);
  updateCmd->SetGuidance("force to recompute geometry.");
  updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  updateCmd->SetGuidance("if you changed geometrical value(s).");
  updateCmd->AvailableForStates(G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  G4cout<<"Haber si es en el detector Messenger: entra"<<G4endl;
  delete detDir; 
  delete opticalCmd;
  delete Ge10PositionCmd;
  delete Ge40PositionCmd;
  delete NaI2x2PositionCmd;
  delete NaI3x3PositionCmd;
  delete PlasticPositionCmd;
  delete CsIPositionCmd;
  delete DummyPositionCmd;
  delete placeGe10Cmd;
  delete placeGe40Cmd;
  delete placeNaI2x2Cmd;
  delete placeNaI3x3Cmd;
  delete placePlasticCmd;
  delete placeCsICmd;
  delete placeDummyCmd;
  delete Ge10rotationXCmd;
  delete Ge10rotationYCmd;
  delete Ge40rotationXCmd;
  delete Ge40rotationYCmd;
  delete NaI2x2rotationXCmd;
  delete NaI2x2rotationYCmd;
  delete NaI3x3rotationXCmd;
  delete NaI3x3rotationYCmd;
  delete PlasticrotationXCmd;
  delete PlasticrotationYCmd;
  delete CsIrotationXCmd;
  delete CsIrotationYCmd;
  delete DummyrotationXCmd;
  delete DummyrotationYCmd;
  delete SourceCapsulePositionCmd;
  delete SourceCapsulerotationXCmd;
  delete SourceCapsulerotationYCmd;
  delete CastleShieldingPositionCmd;
  delete CastleShielding_LeadThicknessCmd;
  delete CastleShielding_LeadRoofThicknessCmd;
  delete CastleShielding_ParaffinRadiusCmd;
  delete CastleShielding_HeightCmd;
  delete updateCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == opticalCmd)
    detector->SetOptical_flag(opticalCmd->GetNewBoolValue(newValue));

  if(command == Plate_MaterialCmd)
    detector->SetPlate_Material(newValue);

  if(command == Plate_thicknessCmd)
    detector->SetPlate_thickness(Plate_thicknessCmd->GetNewDoubleValue(newValue));

 if(command == placeGe10Cmd )
   detector->SetGe10Detector_flag(placeGe10Cmd->GetNewBoolValue(newValue));

 if(command == Ge10PositionCmd )
    detector->SetGe10DetectoPosition(Ge10PositionCmd->GetNew3VectorValue(newValue));

if(command == Ge10rotationXCmd )
   detector->SetGe10DetectorRotationX(Ge10rotationXCmd->GetNewDoubleValue(newValue));

 if(command == Ge10rotationYCmd )
   detector->SetGe10DetectorRotationY(Ge10rotationYCmd->GetNewDoubleValue(newValue));

 if(command == placeGe40Cmd )
   detector->SetGe40Detector_flag(placeGe40Cmd->GetNewBoolValue(newValue));

 if(command == Ge40PositionCmd )
    detector->SetGe40DetectoPosition(Ge40PositionCmd->GetNew3VectorValue(newValue));

 if(command == Ge40rotationXCmd )
   detector->SetGe40DetectorRotationX(Ge40rotationXCmd->GetNewDoubleValue(newValue));

 if(command == Ge40rotationYCmd )
   detector->SetGe40DetectorRotationY(Ge40rotationYCmd->GetNewDoubleValue(newValue));

 if(command == placeNaI2x2Cmd )
   detector->SetNaI2x2Detector_flag(placeNaI2x2Cmd->GetNewBoolValue(newValue));

 if(command == NaI2x2PositionCmd )
    detector->SetNaI2x2DetectoPosition(NaI2x2PositionCmd->GetNew3VectorValue(newValue));

 if(command == NaI2x2rotationXCmd )
   detector->SetNaI2x2DetectorRotationX(NaI2x2rotationXCmd->GetNewDoubleValue(newValue));

 if(command == NaI2x2rotationYCmd )
   detector->SetNaI2x2DetectorRotationY(NaI2x2rotationYCmd->GetNewDoubleValue(newValue));

 if(command == placeNaI3x3Cmd )
   detector->SetNaI3x3Detector_flag(placeNaI3x3Cmd->GetNewBoolValue(newValue));

 if(command == NaI3x3PositionCmd )
    detector->SetNaI3x3DetectoPosition(NaI3x3PositionCmd->GetNew3VectorValue(newValue));

 if(command == NaI3x3rotationXCmd )
   detector->SetNaI3x3DetectorRotationX(NaI3x3rotationXCmd->GetNewDoubleValue(newValue));

 if(command == NaI3x3rotationYCmd )
   detector->SetNaI3x3DetectorRotationY(NaI3x3rotationYCmd->GetNewDoubleValue(newValue));

 if(command == placePlasticCmd )
   detector->SetPlasticDetector_flag(placePlasticCmd->GetNewBoolValue(newValue));

 if(command == PlasticPositionCmd )
    detector->SetPlasticDetectoPosition(PlasticPositionCmd->GetNew3VectorValue(newValue));

 if(command == PlasticrotationXCmd )
   detector->SetPlasticDetectorRotationX(PlasticrotationXCmd->GetNewDoubleValue(newValue));

 if(command == PlasticrotationYCmd )
   detector->SetPlasticDetectorRotationY(PlasticrotationYCmd->GetNewDoubleValue(newValue));

 if(command == placeCsICmd )
   detector->SetCsIDetector_flag(placeCsICmd->GetNewBoolValue(newValue));

 if(command == CsIPositionCmd )
    detector->SetCsIDetectoPosition(CsIPositionCmd->GetNew3VectorValue(newValue));

 if(command == CsIrotationXCmd )
   detector->SetCsIDetectorRotationX(CsIrotationXCmd->GetNewDoubleValue(newValue));

 if(command == CsIrotationYCmd )
   detector->SetCsIDetectorRotationY(CsIrotationYCmd->GetNewDoubleValue(newValue));

 if(command == placeDummyCmd )
   detector->SetDummyDetector_flag(placeDummyCmd->GetNewBoolValue(newValue));

 if(command == DummyPositionCmd )
    detector->SetDummyDetectoPosition(DummyPositionCmd->GetNew3VectorValue(newValue));

 if(command == DummyrotationXCmd )
   detector->SetDummyDetectorRotationX(DummyrotationXCmd->GetNewDoubleValue(newValue));

 if(command == DummyrotationYCmd )
   detector->SetDummyDetectorRotationY(DummyrotationYCmd->GetNewDoubleValue(newValue));

 if(command == CastleShieldingPositionCmd )
   detector->SetCastleShieldingPosition(CastleShieldingPositionCmd->GetNew3VectorValue(newValue));

 if(command == CastleShielding_LeadRoofThicknessCmd )
   detector->SetCastleShielding_LeadRoofThickness(CastleShielding_LeadRoofThicknessCmd->GetNewDoubleValue(newValue));

 if(command == CastleShielding_LeadThicknessCmd )
   detector->SetCastleShielding_LeadThickness(CastleShielding_LeadThicknessCmd->GetNewDoubleValue(newValue));

 if(command == CastleShielding_ParaffinRadiusCmd )
   detector->SetCastleShielding_ParaffinRadius(CastleShielding_ParaffinRadiusCmd->GetNewDoubleValue(newValue));

 if(command == CastleShielding_HeightCmd )
   detector->SetCastleShielding_Height(CastleShielding_HeightCmd->GetNewDoubleValue(newValue));

 if(command == SourceCapsulePositionCmd )
   {
     detector->SetSourceCapsulePosition(SourceCapsulePositionCmd->GetNew3VectorValue(newValue));
      // also set the source position 
      G4RunManager* runManager = G4RunManager::GetRunManager();                                                                            
      PrimaryGeneratorAction* primarygenerator = (PrimaryGeneratorAction*)(runManager->GetUserPrimaryGeneratorAction());
      
      primarygenerator->SetPosition(SourceCapsulePositionCmd->GetNew3VectorValue(newValue));
   }
 if(command == SourceCapsulerotationXCmd )
   detector->SetSourceCapsuleRotationX(SourceCapsulerotationXCmd->GetNewDoubleValue(newValue));

 if(command == SourceCapsulerotationYCmd )
   detector->SetSourceCapsuleRotationY(SourceCapsulerotationYCmd->GetNewDoubleValue(newValue));

 if( command == updateCmd )
    detector->UpdateGeometry();


 /*
  if(command == detectorPositionCmd)
    {
      //detector->SetDetectorPosition(detectorPositionCmd->GetNew3VectorValue(newValue));
      detector->SetDetectorPosition(detectorPositionCmd->GetNewDoubleValue(newValue));
      // also set the source position 
      G4RunManager* runManager = G4RunManager::GetRunManager();                                                                            
      PrimaryGeneratorAction* primarygenerator = (PrimaryGeneratorAction*)(runManager->GetUserPrimaryGeneratorAction());
      
      //primarygenerator->SetPosition(detectorPositionCmd->GetNew3VectorValue(newValue));
      primarygenerator->SetPosition(detectorPositionCmd->GetNewDoubleValue(newValue));
    }

  if(command == detectorDirectionCmd)
    {
      detector->SetDetectorDirection(detectorDirectionCmd->GetNewDoubleValue(newValue));
      // also set the source tilting 
      G4RunManager* runManager = G4RunManager::GetRunManager();
      PrimaryGeneratorAction* primarygenerator = (PrimaryGeneratorAction*)(runManager->GetUserPrimaryGeneratorAction());
      primarygenerator->SetTilting(detectorDirectionCmd->GetNewDoubleValue(newValue));
    }
 */
}

