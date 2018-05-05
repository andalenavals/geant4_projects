/*! @file LogVol_CCHardCovering_Base.hh
  @brief Defines mandatory user class LogVol_CCHardCovering_Base.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_CCHardCovering_Base.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
/* geant4 materials */
#include "G4NistManager.hh"
#include "G4Material.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_CCHardCovering_Base:: LogVol_CCHardCovering_Base(G4String fname):
  G4LogicalVolume(new G4Box(fname+"_Sol",65*cm/2.0,65*cm/2.0,50*cm/2.0),(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  //material=(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al");
  Cyl_height = 1.8*cm;
  Cyl_radius = 23.4/2.0*cm;
  Box_height = 1.7*cm;
  Box_length = 28.0*cm;
  Box_width  = 23.4*cm;
  Base_thickness = 0.3*cm;
  
  /* Construct solid volume */
  ConstructSolidVol_CCHardCovering_Base();
  /* Visualization */
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(true,G4Colour(0.5,0.5,0.5));
  gray_vis->SetForceWireframe(false);
  gray_vis->SetForceSolid(false);
  this->SetVisAttributes(gray_vis);
}

LogVol_CCHardCovering_Base::~LogVol_CCHardCovering_Base()
{}

void LogVol_CCHardCovering_Base::ConstructSolidVol_CCHardCovering_Base(void)
{
  /// Outer part ///
  const G4int numZPlanes_Cyl = 2;
  G4double zPlane_Cyl[numZPlanes_Cyl] = {0.0*cm,Cyl_height};
  G4double rInner_Cyl[numZPlanes_Cyl] = {0.0*cm,0.0*cm};
  G4double rOuter_Cyl[numZPlanes_Cyl] = {Cyl_radius,Cyl_radius};
  
  G4Polycone* Cyl_Outer = new G4Polycone(Name+"_CylOut_Sol",0.*rad,2*M_PI*rad,numZPlanes_Cyl,
					 zPlane_Cyl,rInner_Cyl,rOuter_Cyl);
  G4Box* Box_Outer = new G4Box(Name+"_BoxOut_Sol",Box_width/2.0,Box_length/2.0,Box_height/2.0);
  
  G4UnionSolid*  Base_Outer = new G4UnionSolid(Name+"_BaseOut_Sol", Cyl_Outer, Box_Outer,0,
					       G4ThreeVector(0.,Box_length/2.0,Box_height/2.0));
  /// Inner space ////
  const G4int numZPlanes_Cyl_In = 2;
  G4double zPlane_Cyl_In[numZPlanes_Cyl_In] = {0.0*cm,Cyl_height};
  G4double rInner_Cyl_In[numZPlanes_Cyl_In] = {0.0*cm,0.0*cm};
  G4double rOuter_Cyl_In[numZPlanes_Cyl_In] = {Cyl_radius-Base_thickness,
					       Cyl_radius-Base_thickness};
  
  G4Polycone* Cyl_Inner = new G4Polycone(Name+"_CylOut_Sol",0.*rad,2*M_PI*rad,numZPlanes_Cyl_In,
					 zPlane_Cyl_In,rInner_Cyl_In,rOuter_Cyl_In);
  G4Box* Box_Inner = new G4Box(Name+"_BoxOut_Sol",(Box_width-2.0*Base_thickness)/2.0,
			       (Box_length-Base_thickness)/2.0,
			       Box_height/2.0);
  
  G4UnionSolid*  Base_Inner = new G4UnionSolid(Name+"_BaseInn_Sol", Cyl_Inner, Box_Inner,0,
						G4ThreeVector(0.,
							      (Box_length-Base_thickness)/2.0,
							      Box_height/2.0));
  
  CCHardCovering_Base_solid = new G4SubtractionSolid(Name+"_Sol",
						     Base_Outer,Base_Inner,
						     0,G4ThreeVector(0.,0.,Base_thickness));
  
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_CCHardCovering_Base();
}

void LogVol_CCHardCovering_Base::SetSolidVol_CCHardCovering_Base(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(CCHardCovering_Base_solid)
    this->SetSolid(CCHardCovering_Base_solid);
}
