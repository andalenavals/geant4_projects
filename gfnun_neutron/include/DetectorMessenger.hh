#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class bsneutronDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(bsneutronDetectorConstruction*);
  ~DetectorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  bsneutronDetectorConstruction* myDetector;

  G4UIdirectory*             bsneutronDir;
  
  //PAra los detectores
  G4UIdirectory*             detectorDir;
  //cambia la distancia entre los detectores.....
  G4UIcmdWithADoubleAndUnit* detectorPosCmd; 
  


  //Para el target
  G4UIdirectory*             TargetDir;
  G4UIcmdWithAString*        TargetMaterialCmd;
  G4UIcmdWithADoubleAndUnit* TargetSizeCmd;     

  G4UIcmdWithADoubleAndUnit* TargetDepthCmd;     
  
  
  //G4UIcmdWithADoubleAndUnit* TargetPosCmd;    
  G4UIcmdWithADoubleAndUnit* DetectorPosCmd;    
  G4UIcmdWithADoubleAndUnit* DetectorHeightCmd;    
  
  //Para el suelo
  G4UIdirectory*             SueloDir;
  G4UIcmdWithAString*        SueloMaterialCmd;
  G4UIcmdWithADoubleAndUnit*      SueloSizeCmd;


  //March 28-2009. Material to the wet sand layer
  G4UIdirectory*             Wet_SandDir;
  G4UIcmdWithAString*        Wet_SandMaterialCmd;
  G4UIcmdWithADoubleAndUnit*  Wet_Sand_widthCmd;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

