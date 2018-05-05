/*! @file LogVol_GammaSourceCasing.hh
  @brief Defines mandatory user class LogVol_GammaSourceCasing.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_GammaSourceCasing.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Tubs.hh"
#include "G4Orb.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_GammaSourceCasing:: LogVol_GammaSourceCasing(G4String fname, G4double frad, G4double fhei):
  G4LogicalVolume(new G4Tubs(fname+"_Sol",0.0*cm, 1.0*cm, 0.3*cm/2.0,0.*rad,2*M_PI*rad),(G4NistManager::Instance())->FindOrBuildMaterial("G4_POLYACRYLONITRILE"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  SetRadius(frad);
  SetHeight(fhei);
  /* Construct solid volume */
  ConstructLogVol_GammaSourceCasing();
  /* Visualization */
  G4VisAttributes* red_vis 
    = new G4VisAttributes(true,G4Colour::Red());
  red_vis->SetForceWireframe(false);
  red_vis->SetForceSolid(false);
  this->SetVisAttributes(red_vis);
}

LogVol_GammaSourceCasing::~LogVol_GammaSourceCasing()
{}

void LogVol_GammaSourceCasing::ConstructLogVol_GammaSourceCasing(void)
{
  GammaSourceCasing_solid = new G4Tubs(Name+"_Sol",0.0*cm, Radius, Height/2.0,
				       0.*rad,2*M_PI*rad);
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_GammaSourceCasing();
  ///////////////////////////////
  /// **** Source volume **** ///
  ///////////////////////////////
  G4double source_r = 1.2*mm/2.0;
  G4Orb* source_sol = new G4Orb(Name+"_source_Sol",source_r);
  //Alternatively::  G4Sphere* source_sol = new G4Box("source_Sol",0.0, source_r,0.*rad,2*M_PI*rad,0.*rad,M_PI*rad);
  
  G4LogicalVolume* source_log= new G4LogicalVolume(source_sol,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Na"),Name+"_source_Log",0,0,0);
  G4VisAttributes* blue_vis 
    = new G4VisAttributes(true,G4Colour::Blue());
  source_log->SetVisAttributes(blue_vis);
  
  G4VPhysicalVolume* source_phy;
  source_phy = new G4PVPlacement(0,         // no rotation
				 G4ThreeVector(), 
				 source_log,    // its logical volume
				 Name+"_source_phy",      // its name
				 this,
				 false,     // no boolean operations 
				 0,
				 true);
}

void LogVol_GammaSourceCasing::SetSolidVol_GammaSourceCasing(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(GammaSourceCasing_solid)
    this->SetSolid(GammaSourceCasing_solid);
}
