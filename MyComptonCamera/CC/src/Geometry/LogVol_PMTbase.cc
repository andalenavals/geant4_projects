/*! @file LogVol_PMTbase.hh
  @brief Defines mandatory user class LogVol_PMTbase.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_PMTbase.hh"
#include "LogVol_PCBs_Cyl.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Tubs.hh"
#include "G4Polycone.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_PMTbase:: LogVol_PMTbase(G4String fname, G4double frad, G4double flen):
  G4LogicalVolume(new G4Tubs(fname+"_Sol",0.0*cm, 5.8*cm, 7.6*cm/2.0,0.*rad,2*M_PI*rad),(G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  SetRadius(frad);
  SetLength(flen);
  SetThickness(0.05*cm);
  /* Construct solid volume */
  ConstructLogVol_PMTbase();
  /* Visualization */
  this->SetVisAttributes(G4VisAttributes::GetInvisible());
}

LogVol_PMTbase::~LogVol_PMTbase()
{}

void LogVol_PMTbase::ConstructLogVol_PMTbase(void)
{
  G4VisAttributes* darkgray_vis = new G4VisAttributes(true,G4Colour(0.6,0.6,0.6));
  darkgray_vis->SetForceSolid(false);
  darkgray_vis->SetForceWireframe(false);

  ///**** General solid volume ****///
  const G4int  numZPlanesG = 2;
  G4double zPlaneG[numZPlanesG] = {0.0*cm,Length};
  G4double rInnerG[numZPlanesG] = {0.0*cm,0.0*cm};
  G4double rOuterG[numZPlanesG] = {Radius,Radius};
  
  PMTbase_solid = new G4Polycone(Name+"_Sol",
				 0.*rad,2*M_PI*rad,
				 numZPlanesG,zPlaneG,rInnerG,rOuterG);
  ///*** Main trick here: Set the new solid volume ***///
  SetSolidVol_PMTbase();
  
  ///**** Al enclosure ****///
  const G4int  numZPlanes = 2;
  G4double zPlane[numZPlanes] = {0.0*cm,Length};
  G4double rInner[numZPlanes] = {Radius-Thickness,Radius-Thickness};
  G4double rOuter[numZPlanes] = {Radius,Radius};
  
  G4Polycone* solid_Al_enclousure;
  solid_Al_enclousure = new G4Polycone(Name+"Al_enclousure_Sol",0.*rad,2*M_PI*rad,numZPlanes,zPlane,rInner,rOuter);
  
  G4LogicalVolume* logic_Al_enclousure;
  logic_Al_enclousure = new G4LogicalVolume(solid_Al_enclousure,
					    (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"), Name+"Al_enclousure_Log",
					    0,0,0);
  logic_Al_enclousure->SetVisAttributes(darkgray_vis);
  
  G4VPhysicalVolume* physical_Al_enclousure;
  physical_Al_enclousure = new G4PVPlacement(0,               // Rotation
					     G4ThreeVector(), // Position
					     logic_Al_enclousure, // its logical volume
					     Name+"Al_enclousure_Phys",      // its name
					     this,
					     false,     // no boolean operations 
					     0,
					     true);
  LogVol_PCBs_Cyl* pcb_log;
  pcb_log = new LogVol_PCBs_Cyl(Name+"_PCBs_Cyl_Log",Radius-Thickness,Length);//name, radius, length
  G4VPhysicalVolume* physical_pcb;
  physical_pcb = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(), //Completa
				   pcb_log,    // its logical volume
				   Name+"_PCBs_Cyl_Phys",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
}

void LogVol_PMTbase::SetSolidVol_PMTbase(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(PMTbase_solid)
    this->SetSolid(PMTbase_solid);
}
