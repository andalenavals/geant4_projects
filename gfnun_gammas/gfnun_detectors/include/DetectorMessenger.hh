#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

/**
 * @file
 *
 * @date   01 July, 2010
 * @author Pico
 *
 * @brief defines class DetectorMessenger
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\brief This class provides the user interface to DetectorConstruction

It allows for
 - inclusion or not of the radioactive source holder
 
\sa SetNewValue()
*/
class DetectorMessenger: public G4UImessenger
{
public:
  //! Constructor
  DetectorMessenger(DetectorConstruction* );
  //! Destructor
  ~DetectorMessenger();
    
  //! handle user commands
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  DetectorConstruction*      detector;
  G4UIdirectory*             detDir;

  G4UIcmdWithABool*          opticalCmd;

  G4UIcmdWithABool*          placeGe10Cmd;
  G4UIcmdWith3VectorAndUnit* Ge10PositionCmd;
  G4UIcmdWithADoubleAndUnit* Ge10rotationXCmd;
  G4UIcmdWithADoubleAndUnit* Ge10rotationYCmd;

  G4UIcmdWithABool*          placeGe40Cmd;
  G4UIcmdWith3VectorAndUnit* Ge40PositionCmd;
  G4UIcmdWithADoubleAndUnit* Ge40rotationXCmd;
  G4UIcmdWithADoubleAndUnit* Ge40rotationYCmd;

  G4UIcmdWithABool*          placeNaI2x2Cmd;
  G4UIcmdWith3VectorAndUnit* NaI2x2PositionCmd;
  G4UIcmdWithADoubleAndUnit* NaI2x2rotationXCmd;
  G4UIcmdWithADoubleAndUnit* NaI2x2rotationYCmd;

  G4UIcmdWithABool*          placeNaI3x3Cmd;
  G4UIcmdWith3VectorAndUnit* NaI3x3PositionCmd;
  G4UIcmdWithADoubleAndUnit* NaI3x3rotationXCmd;
  G4UIcmdWithADoubleAndUnit* NaI3x3rotationYCmd;

  G4UIcmdWithABool*          placePlasticCmd;
  G4UIcmdWith3VectorAndUnit* PlasticPositionCmd;
  G4UIcmdWithADoubleAndUnit* PlasticrotationXCmd;
  G4UIcmdWithADoubleAndUnit* PlasticrotationYCmd;

  G4UIcmdWithABool*          placeCsICmd;
  G4UIcmdWith3VectorAndUnit* CsIPositionCmd;
  G4UIcmdWithADoubleAndUnit* CsIrotationXCmd;
  G4UIcmdWithADoubleAndUnit* CsIrotationYCmd;

  G4UIcmdWithABool*          placeDummyCmd;
  G4UIcmdWith3VectorAndUnit* DummyPositionCmd;
  G4UIcmdWithADoubleAndUnit* DummyrotationXCmd;
  G4UIcmdWithADoubleAndUnit* DummyrotationYCmd;

  G4UIcmdWith3VectorAndUnit* SourceCapsulePositionCmd;
  G4UIcmdWithADoubleAndUnit* SourceCapsulerotationXCmd;
  G4UIcmdWithADoubleAndUnit* SourceCapsulerotationYCmd;

  G4UIcmdWith3VectorAndUnit* CastleShieldingPositionCmd;
  G4UIcmdWithADoubleAndUnit* CastleShielding_LeadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* CastleShielding_LeadRoofThicknessCmd;
  G4UIcmdWithADoubleAndUnit* CastleShielding_ParaffinRadiusCmd;
  G4UIcmdWithADoubleAndUnit* CastleShielding_HeightCmd;

  G4UIcmdWithAString*         Plate_MaterialCmd;
  G4UIcmdWithADoubleAndUnit*  Plate_thicknessCmd;

  G4UIcmdWithoutParameter*   updateCmd;    
};
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

