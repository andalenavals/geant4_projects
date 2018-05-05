/*! @file LogVol_PCBs.hh
  @brief Defines mandatory user class LogVol_PCBs_Cyl.
  @date August, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2.0
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_PCBs_Cyl.hh"
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

LogVol_PCBs_Cyl:: LogVol_PCBs_Cyl(G4String fname, G4double frad, G4double flen):
  G4LogicalVolume(new G4Box("PCBs_Cyl_solid",10*mm, 10*mm, 10*mm),(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  SetRadius(frad);
  SetLength(flen);
  SetThickness(0.15*cm);
  Material=(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE");
  /* Construct solid volume */
  ConstructSolidVol_PCBs_Cyl();
  /* Visualization */
  G4VisAttributes* back_vis 
    = new G4VisAttributes(true,G4Colour(0.1,0.9,0.1));
  back_vis->SetForceWireframe(false);
  back_vis->SetForceSolid(false);
  this->SetVisAttributes(back_vis);
}

LogVol_PCBs_Cyl::~LogVol_PCBs_Cyl()
{}

void LogVol_PCBs_Cyl::ConstructSolidVol_PCBs_Cyl(void)
{
 //!*** PCB from the electronics ***!//
  const G4int  numZPlanes = 2;
  G4double zPlane[numZPlanes] = {0.0*cm,Thickness};
  G4double rInner[numZPlanes] = {0.0*cm,0.0*cm};
  G4double rOuter[numZPlanes] = {Radius,Radius};
  
  G4Polycone* solid_pcb_base;
  solid_pcb_base = new G4Polycone(Name+"_base_Sol",0.*rad,2*M_PI*rad,numZPlanes,zPlane,rInner,rOuter);
  G4Box* solid_pcb_box;
  solid_pcb_box = new G4Box(Name+"pcb_box_Sol",Thickness/2.,0.9*Radius,Length/2.);
  G4UnionSolid* solid_sec1;
  solid_sec1 = new G4UnionSolid(Name+"_pcb_sec1_Sol",solid_pcb_base,solid_pcb_box,0,G4ThreeVector(0.,0.,Length/2.));
  G4Tubs* solid_pcb_tub;
  solid_pcb_tub = new G4Tubs(Name+"pcb_tub_Sol", 0.,Radius,Thickness/2.0,0.*rad,2*M_PI*rad);
  PCBs_Cyl_solid = new G4UnionSolid(Name+"_pcb_Sol",solid_sec1,solid_pcb_tub,0,G4ThreeVector(0.,0.,Length-Thickness/2.0));
  
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_PCBs_Cyl();
}

void LogVol_PCBs_Cyl::SetSolidVol_PCBs_Cyl(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(PCBs_Cyl_solid)
    this->SetSolid(PCBs_Cyl_solid);
}
