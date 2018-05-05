/*! @file LogVol_PCBs.hh
  @brief Defines mandatory user class LogVol_PCBs_Box.
  @date August, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2.0
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_PCBs_Box.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
/* geant4 materials */
#include "G4NistManager.hh"
#include "G4Material.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_PCBs_Box:: LogVol_PCBs_Box(G4String fname, G4double fwidth, G4double flen, G4double fthick):
  G4LogicalVolume(new G4Box("PCBs_Box_solid",10*mm, 10*mm, 10*mm),(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  SetWidth(fwidth);
  SetLength(flen);
  SetThickness(fthick);
  Material=(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE");
  /* Construct solid volume */
  ConstructSolidVol_PCBs_Box();
  /* Visualization */
  G4VisAttributes* back_vis 
    = new G4VisAttributes(true,G4Colour(0.1,0.9,0.1));
  back_vis->SetForceWireframe(false);
  back_vis->SetForceSolid(false);
  this->SetVisAttributes(back_vis);
}

LogVol_PCBs_Box::~LogVol_PCBs_Box()
{}

void LogVol_PCBs_Box::ConstructSolidVol_PCBs_Box(void)
{
  //!*** PCB from the electronics ***!//
  
  G4Box* solid_pcb_box;
  solid_pcb_box = new G4Box(Name+"pcb_box_Sol",Width/2.0,Width/2.0,Thickness/2.0);
  G4UnionSolid* solid_sec1;
  solid_sec1 = new G4UnionSolid(Name+"_pcb_sec1_Sol",solid_pcb_box,solid_pcb_box,0,G4ThreeVector(0.,0.,(Length-3*Thickness)/2.0+Thickness));
  PCBs_Box_solid = new G4UnionSolid(Name+"_pcb_Sol",solid_sec1,solid_pcb_box,0,G4ThreeVector(0.,0.,Length-Thickness));
  
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_PCBs_Box();
}

void LogVol_PCBs_Box::SetSolidVol_PCBs_Box(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(PCBs_Box_solid)
    this->SetSolid(PCBs_Box_solid);
}
