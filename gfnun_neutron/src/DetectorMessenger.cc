//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "bsneutronDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(bsneutronDetectorConstruction* myDet)
  :myDetector(myDet)
{ 
  
  bsneutronDir = new G4UIdirectory("/bsneutron/");
  bsneutronDir->SetGuidance("Comandos propios");
  
  detectorDir = new G4UIdirectory("/bsneutron/detector/");
  detectorDir->SetGuidance("comandos para el detector");
  
  TargetDir = new G4UIdirectory("/bsneutron/target/");
  TargetDir->SetGuidance("comandos para el Target o la mina o lo que sea");
  
  SueloDir = new G4UIdirectory("/bsneutron/suelo/");
  SueloDir->SetGuidance("comandos para el Suelo");


  //March 28-2009
  Wet_SandDir = new G4UIdirectory("/bsneutron/suelo/WetSandLayer/");
  Wet_SandDir->SetGuidance("Command to change the properties of the wet sand layer");
  
  
  
  
  
  //**************************************************************************************************************
  //**************************************************************************************************************
  TargetSizeCmd = new G4UIcmdWithADoubleAndUnit("/bsneutron/target/cambieAnchoenZ",this);
  TargetSizeCmd->SetGuidance("Cambia el ancho (z)  del target.");
  TargetSizeCmd->SetParameterName("choice",false);
  TargetSizeCmd->SetUnitCategory("Length");
  TargetSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  TargetMaterialCmd = new G4UIcmdWithAString("/bsneutron/target/TargetMaterial",this);
  TargetMaterialCmd->SetGuidance("Selecciona el material para el target");
  TargetMaterialCmd->SetParameterName("choice",false);
  TargetMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Marzo 24... 2008 nuevo comando
  TargetDepthCmd= new G4UIcmdWithADoubleAndUnit("/bsneutron/target/depth",this);
  TargetDepthCmd->SetGuidance("Cambia la profundidad a la que esta enterrado el target.");
  TargetDepthCmd->SetParameterName("choice",false);
  TargetDepthCmd->SetUnitCategory("Length");
  TargetDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  
  //Marzo 23 2008 cambio el nombre del comando que ahora mueve el arreglo de detectores...........
  DetectorPosCmd= new G4UIcmdWithADoubleAndUnit("/bsneutron/detector/position",this);
  DetectorPosCmd->SetGuidance("Cambia la posicion x del arreglo de detectores");
  DetectorPosCmd->SetParameterName("choice",false);
  DetectorPosCmd->SetUnitCategory("Length");
  DetectorPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Abril 1...2008 nuevo comando para modificar la altura a la que están los detectores
  DetectorHeightCmd= new G4UIcmdWithADoubleAndUnit("/bsneutron/detector/height",this);
  DetectorHeightCmd->SetGuidance("Cambia la altura a la que estan los detectores");
  DetectorHeightCmd->SetParameterName("choice",false);
  DetectorHeightCmd->SetUnitCategory("Length");
  DetectorHeightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  
   //**************************************************************************************************************
  //**************************************************************************************************************
  SueloSizeCmd = new G4UIcmdWithADoubleAndUnit("/bsneutron/suelo/cambieAnchoEnZ",this);
  SueloSizeCmd->SetGuidance("Cambia el ancho (z)  del suelo.");
  SueloSizeCmd->SetParameterName("choice",false);
  SueloSizeCmd->SetUnitCategory("Length");
  SueloSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SueloMaterialCmd = new G4UIcmdWithAString("/bsneutron/suelo/SueloMaterial",this);
  SueloMaterialCmd->SetGuidance("Selecciona el material para el Suelo");
  SueloMaterialCmd->SetParameterName("choice",false);
  SueloMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  



  //March 28-2009// Properties of the wet sand layer
  //Material
  Wet_SandMaterialCmd = new G4UIcmdWithAString("/bsneutron/suelo/WetSandLayer/Material",this);
  Wet_SandMaterialCmd->SetGuidance("Selects the material of the wet sand layer");
  Wet_SandMaterialCmd->SetParameterName("choice",false);
  Wet_SandMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  

  //Width
  Wet_Sand_widthCmd = new G4UIcmdWithADoubleAndUnit("/bsneutron/suelo/WetSandLayer/Width",this);
  Wet_Sand_widthCmd->SetGuidance("Change the width (w) of the wet sand layer");
  Wet_Sand_widthCmd->SetParameterName("choice",false);
  Wet_Sand_widthCmd->SetUnitCategory("Length");
  Wet_Sand_widthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  


  
  
   //**************************************************************************************************************
   //**************************************************************************************************************
  detectorPosCmd= new G4UIcmdWithADoubleAndUnit("/bsneutron/detector/distanciaEntreAB",this);
  detectorPosCmd->SetGuidance("Change the distance between Arrays.");
  detectorPosCmd->SetParameterName("choice",false);
  detectorPosCmd->SetUnitCategory("Length");
  detectorPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  

  



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete bsneutronDir;
    
  delete TargetDir;  
  delete TargetSizeCmd;
  delete TargetDepthCmd;
  delete TargetMaterialCmd;
 
  
  delete SueloDir;
  delete SueloSizeCmd;
  delete SueloMaterialCmd;
  

  //March 28-2009
  delete Wet_SandDir;
  delete Wet_SandMaterialCmd;
  delete Wet_Sand_widthCmd;


  
  delete detectorPosCmd;
  delete DetectorPosCmd;
  delete DetectorHeightCmd;
  
  delete detectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //*************Target
  //ANCHO
  if( command == TargetSizeCmd )
    { 
      myDetector->setTargetSize(TargetSizeCmd->GetNewDoubleValue(newValue));
    } 
  //mATERIAL
  if( command == TargetMaterialCmd )
    { 
      myDetector->setTargetMaterial(newValue);
    } 

  //Marzo 24..2008 nuevo comando
  if( command == TargetDepthCmd )
    { 
      myDetector->setTargetDepth(TargetDepthCmd->GetNewDoubleValue(newValue));
    } 
  
  
  
  //*************Suelo
  //ANCHO
  if( command == SueloSizeCmd )
    { 
      myDetector->setSueloSize(SueloSizeCmd->GetNewDoubleValue(newValue));
    } 
  //mATERIAL
  if( command == SueloMaterialCmd )
    { 
      myDetector->setSueloMaterial(newValue);
    } 
  
  //*************Detector
  //Posicion
  if( command == detectorPosCmd )
    { 
      myDetector->setXDetectorSeparation(detectorPosCmd->GetNewDoubleValue(newValue));
    } 
  
  //cambia la posicion de la cajita madre de los detectores
  if( command == DetectorPosCmd )
    { 
      myDetector->setXDetectorPosition(DetectorPosCmd->GetNewDoubleValue(newValue));
    } 
  
   if( command == DetectorHeightCmd )
    { 
      myDetector->setDetectorHeight(DetectorHeightCmd->GetNewDoubleValue(newValue));
    } 
 
   //March 28-2009.
   //Wet sand layer
   if( command == Wet_SandMaterialCmd)
     { 
       myDetector->setWet_Sand_material(newValue);
     } 
   
   if( command == Wet_Sand_widthCmd)
     { 
       myDetector->setWet_Sand_Width(Wet_Sand_widthCmd ->GetNewDoubleValue(newValue));
     } 
   
   

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
