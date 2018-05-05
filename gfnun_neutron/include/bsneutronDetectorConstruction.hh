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
// $Id: ExN01DetectorConstruction.hh,v 1.5 2002/01/09 17:23:48 ranjard Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//

#ifndef bsneutronDetectorConstruction_H
#define bsneutronDetectorConstruction_H 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
//02/03/07
class DetectorMessenger;

//May 28-2009..user limits 
class G4UserLimits;



class bsneutronDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
  bsneutronDetectorConstruction();
  ~bsneutronDetectorConstruction();
  
  G4VPhysicalVolume* Construct();

  //02/03/07
  //funciones para definir los comandos para cambiar la dimension z o el material de tal volumen
  //void setSVMaterial(G4String);
  void setTargetSize(G4double);
  void setTargetMaterial(G4String);
  
  void setSueloSize(G4double);
  void setSueloMaterial(G4String);
  
  void setXDetectorSeparation(G4double);
  void setXDetectorPosition( G4double);  

  //Marzo 24 2008
  void setTargetDepth( G4double );  
  //Abril 1 2008
  void setDetectorHeight( G4double );  

  
 
  //Mayo 4 ...por esos dias 2007
  G4double GetDistanciaEntreDetectores(void ){return (detector4Pos_x-detector5Pos_x)/cm;};  
  G4double GetTargetPosition(void ){return  plasticboxPos_x/cm;};  


  
  //March 28-2009
  //To change the widht  and the material of the wet sand layer.
  void setWet_Sand_Width(G4double);
  void setWet_Sand_material(G4String);

  
private:
  //May 04-2009.....to include limits
  G4UserLimits*    theUserLimitsForSoil; 
  G4double         theMaxTimeCuts;
  G4double         theMaxStepSize;
  G4double         theMinEkine;
  
  //Parámetros de la cajita madre de los detectores
  G4double detector_x; 
  G4double detector_y;
  G4double detector_z;
  
  G4double detectorPos_x; 
  G4double detectorPos_y;
  G4double detectorPos_z; 
  

  G4double detector1Pos_x;
  G4double detector1Pos_y;
  G4double detector1Pos_z;
  
  G4double detector8Pos_x;
  G4double detector8Pos_y;
  G4double detector8Pos_z;



  G4double detector4Pos_x;
  G4double detector4Pos_y;
  G4double detector4Pos_z;
  
  G4double detector5Pos_x;
  G4double detector5Pos_y;
  G4double detector5Pos_z;


  
  G4double plasticboxPos_x;
  G4double plasticboxPos_y;
  G4double plasticboxPos_z;

  G4double minePos_x;
  G4double minePos_y;
  G4double minePos_z;


 
   //Mayo 23-2008  tama�os de las cajitas Array
  G4double ArrayA_x; 
  G4double ArrayA_y; 
  G4double ArrayA_z; 

  //  G4double ArrayB_x; 
  // G4double ArrayB_y; 
  //G4double ArrayB_z; 

 //September 8th 2010 size of new arrays C and D 
 // G4double ArrayC_x; 
 // G4double ArrayC_y; 
 // G4double ArrayC_z; 

 // G4double ArrayD_x; 
 // G4double ArrayD_y; 
 // G4double ArrayD_z; 
  
  //Mayo 23-2008  posicioneds de los "Arrays" con respecto a la cajita madre! 
  G4double ArrayAPos_x;
  G4double ArrayAPos_y;
  G4double ArrayAPos_z;
  
  G4double ArrayBPos_x;
  G4double ArrayBPos_y;
  G4double ArrayBPos_z;

  //March 28-2009
  G4double Wet_Sand_width;
  G4double wetPos_x;
  G4double wetPos_y;
  G4double wetPos_z;
  
//------------------------------------------------------------------------------------------------------------------
  
  //******************************************* Logical volumes
  //
  G4LogicalVolume* experimentalHall_log;
  G4LogicalVolume* soilBlock_log;
  
  //Volumen Logico de la cajita madre..
  G4LogicalVolume* detector_log;
  
  //Mayo 23-2008 creo dos cajitas  donde pondre los arreglos de detectores
  G4LogicalVolume* ArrayA_log;
  G4LogicalVolume* ArrayB_log;
  //September 8th-2010  logic volumens of the boxes of new arrays C and D 
  
  //Abril 1****2008 recubrimiento de acero de los detectores
  G4LogicalVolume* detector1_recovering_log;
  G4LogicalVolume* detector2_recovering_log;
  G4LogicalVolume* detector3_recovering_log;
  G4LogicalVolume* detector4_recovering_log;
  G4LogicalVolume* detector5_recovering_log;
  G4LogicalVolume* detector6_recovering_log;
  G4LogicalVolume* detector7_recovering_log;
  G4LogicalVolume* detector8_recovering_log;

  //*************************************************
  
  G4LogicalVolume* plasticbox_log;
  G4LogicalVolume* acrilico_log;
  G4LogicalVolume* mine_log;

  //*****************OTROS agregados Junio6 
  G4LogicalVolume* detector1Block_log;
  G4LogicalVolume* detector2Block_log;
  G4LogicalVolume* detector3Block_log;
  G4LogicalVolume* detector4Block_log;
  G4LogicalVolume* detector5Block_log;
  G4LogicalVolume* detector6Block_log;
  G4LogicalVolume* detector7Block_log;
  G4LogicalVolume* detector8Block_log;

  //**********************************************
  //Columna agregada Febrero 26
  G4LogicalVolume* columna_log;
  G4LogicalVolume* blindaje_log;
  //Mayo 23-2008. Capa de arena. 
  G4LogicalVolume* WetSand_log;
  
  //Julio 28-2008. Caja de madera que ahora es madre de la caja de arena.
  G4LogicalVolume* WoodBox_log;
  
  //Julio 29-2008. Caja de aire que ahora es madre de lacajita madre de los detectores
  
  G4LogicalVolume* AirBox_log;
  
  //********************************************* Physical volumes
  //
  G4VPhysicalVolume* experimentalHall_phys;
  G4VPhysicalVolume* soilBlock_phys;
  
  //Volumen Físico de la cajita madre..
  G4VPhysicalVolume* detector_phys;

  //**********Volumen fisico de las cajitas hijas de la cajita madre de detectores. Cada una un array
  G4VPhysicalVolume* ArrayA_phys;
  G4VPhysicalVolume* ArrayB_phys;

  //September 8th physic volumen to the arrays C and D
  G4VPhysicalVolume* ArrayC_phys;
  G4VPhysicalVolume* ArrayD_phys;
  //********************************
  

  
  //*****************OTROS agregados Junio 6/ 2008
  G4VPhysicalVolume* detector1Block_phys;
  G4VPhysicalVolume* detector2Block_phys;
  G4VPhysicalVolume* detector3Block_phys;
  G4VPhysicalVolume* detector4Block_phys;
  G4VPhysicalVolume* detector5Block_phys;
  G4VPhysicalVolume* detector6Block_phys;
  G4VPhysicalVolume* detector7Block_phys;
  G4VPhysicalVolume* detector8Block_phys;
  //******************************************
  //******************Columna agregada Febrero 26
  G4VPhysicalVolume* columna_phys;
  G4VPhysicalVolume* blindaje_phys;
  
  //***************Mayo 23-2008. Capa de arena
  G4VPhysicalVolume* WetSand_phys;
  
  //***************Julio 28-2008. Caja de madera
  G4VPhysicalVolume* WoodBox_phys;


  //***************Julio 29-2008. Caja de aire
  G4VPhysicalVolume* AirBox_phys;

  
  
  G4VPhysicalVolume* plasticbox_phys;
  G4VPhysicalVolume* mine_phys;
  G4VPhysicalVolume* acrilico_phys;
  //*********Abril 1 2008 recubrimiento de los detectores
  G4VPhysicalVolume* detector1_recovering_phys;  
  G4VPhysicalVolume* detector2_recovering_phys;  
  G4VPhysicalVolume* detector3_recovering_phys;  
  G4VPhysicalVolume* detector4_recovering_phys;  
  G4VPhysicalVolume* detector5_recovering_phys;  
  G4VPhysicalVolume* detector6_recovering_phys;  
  G4VPhysicalVolume* detector7_recovering_phys;  
  G4VPhysicalVolume* detector8_recovering_phys;  
  //*******************************************************
  

  

  //********************Box definitions********************
  G4Box* soilBlock_box;  
  G4Box* plasticbox_box;
  //Columna agregada Febrero 26-2008
  G4Box* columna_box;
  G4Box* blindaje_box;
  
  //Julio 28-2008. Caja de madera.
  G4Box* WoodBox_box;

  //Julio 29-2008. Caja de aire
  G4Box* AirBox_box;
  
  //March 25-2009
  G4Box* WetSand_box;
  
  
  
private:
  //materials
  G4Material* defaultMaterial;
  G4Material* Worldmaterial;
  G4Material* Soilmaterial;
  G4Material* Minematerial; 
  G4Material* Plasticboxmaterial; 
  G4Material* Detectormaterial; 
  G4Material*  Containermaterial;
  
  //***Febrero 26
  G4Material*  ColumnaMaterial;
  G4Material*  ReflectorMaterial; //(Agua pesada!)
  G4Material*  BlindajeMaterial; //(Agua pesada!)
  
  G4Material*  RecoveringMaterial; 
  
  //Mayo 23-2008
  G4Material*  WetSandMaterial; 

  //Julio 28-2008. Material de la caja de madera.
  G4Material*  WoodBoxMaterial; 
  

  
  
  //messenger....02/03/07
  DetectorMessenger* detectorMessenger;  // pointer to the Messenger
private:
  
  void DefineMaterials();
  //angelh..acuerdese que esta está para definir los componentes sensitivos de la geometria, es decir el berraco detector!!!
  // 15/03/07 ya los defini ...pero aun estoy haciendo lo del G4RUN 
  void SetupScoring(G4LogicalVolume* scoringVolume);
  
};

#endif

