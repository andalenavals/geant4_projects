/*! @file LogVol_Crystal_Cyl.hh
  @brief Defines mandatory user class LogVol_Crystal_Cyl.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_Crystal_Cyl.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Tubs.hh"
#include "G4Polycone.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_Crystal_Cyl:: LogVol_Crystal_Cyl(G4String fname, G4double fIrad, G4double fOrad, G4double flen,G4Material* fMaterial):
  G4LogicalVolume(new G4Tubs(fname+"_Sol",0.0*cm, fOrad, flen/2.0,0.*rad,2*M_PI*rad),fMaterial,fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  //  SetID(fID);
  SetOuterRadius(fOrad);
  SetInnerRadius(fIrad);
  SetHeight(flen);
  /* Construct solid volume */
  ConstructLogVol_Crystal_Cyl();
  /* Visualization */
  G4VisAttributes* yellow_vis 
    = new G4VisAttributes(true,G4Colour(0.7,0.5,0.2));
  yellow_vis->SetForceWireframe(false);
  yellow_vis->SetForceSolid(false);
  this->SetVisAttributes(yellow_vis);
 }

LogVol_Crystal_Cyl::~LogVol_Crystal_Cyl()
{}

void LogVol_Crystal_Cyl::ConstructLogVol_Crystal_Cyl(void)
{
  const G4int  numZPlanes = 2;
  G4double zPlane[numZPlanes] = {0.0*cm,-Height};
  G4double rInner[numZPlanes] = {IRadius,IRadius};
  G4double rOuter[numZPlanes] = {ORadius,ORadius};
  
  Crystal_Cyl_solid = new G4Polycone(Name+"_Sol",0.*rad,2*M_PI*rad,numZPlanes,zPlane,rInner,rOuter);
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_Crystal_Cyl();

}

void LogVol_Crystal_Cyl::SetSolidVol_Crystal_Cyl(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(Crystal_Cyl_solid)
    this->SetSolid(Crystal_Cyl_solid);
}

