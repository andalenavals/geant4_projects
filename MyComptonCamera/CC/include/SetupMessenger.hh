#ifndef SetupMessenger_h
#define SetupMessenger_h 1

/**
 * @file
 *
 * @date   04 September, 2015
 * @author D. Flechas dcflechasg@unal.edu.co
 *
 * @brief defines class SetupMessenger
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4UImessenger.hh"
#include "globals.hh"

class SetupConstruction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class SetupMessenger: public G4UImessenger
{
public:
  //! Constructor
  SetupMessenger(SetupConstruction* );
  //! Destructor
  virtual ~SetupMessenger();
    
  //! handle user commands
  virtual void SetNewValue(G4UIcommand*, G4String);
private:

  SetupConstruction*      detector;
  G4UIdirectory*             detDir;

    // Plate 
  G4UIcmdWithAString*         Plate_MaterialCmd;
  G4UIcmdWithADoubleAndUnit*  Plate_thicknessCmd;
  
  G4UIcmdWithoutParameter*   updateCmd;    
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

