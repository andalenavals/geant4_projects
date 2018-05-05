/*! @file LogVol_DynodesArray.hh
  @brief Defines mandatory user class LogVol_DynodesArray.
  @date August, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_DynodesArray.hh"
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
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
/* geant4 materials */
#include "G4NistManager.hh"
#include "G4Material.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_DynodesArray:: LogVol_DynodesArray(G4String fname, G4double frad, G4double flen, G4int fnum):
  G4LogicalVolume(new G4Box(fname+"_Sol",10*mm, 10*mm, 10*mm),(G4NistManager::Instance())->FindOrBuildMaterial("G4_C"),fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  SetHeight(frad);
  SetLength(flen);
  SetThickness(0.05*cm);
  if(fnum<12)
    SetNumOfDynodes(fnum);
  else
    {
      G4cout<<"Too many dynodes. Set number of dynodes to 11"<<G4endl;
      SetNumOfDynodes(11);
    }
  
  Material=(G4NistManager::Instance())->FindOrBuildMaterial("G4_C");
  /* Construct solid volume */
  ConstructSolidVol_DynodesArray();
  /* Visualization */
  //  this->SetVisAttributes(G4VisAttributes::GetInvisible());//never show
  G4VisAttributes* red_vis 
    = new G4VisAttributes(true,G4Colour::Red());
  red_vis->SetForceWireframe(false);
  red_vis->SetForceSolid(false);
  this->SetVisAttributes(G4VisAttributes::GetInvisible());
}

LogVol_DynodesArray::~LogVol_DynodesArray()
{}

void LogVol_DynodesArray::ConstructSolidVol_DynodesArray(void)
{
  ///**** Geaneral solid: Shielding container ****///
  DynodesArray_solid = new G4Box(Name+"dynArray_box",Height/2.0,Height/2.0,Length/2.);
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_DynodesArray();

  //*** Dynode ***//
  G4Box * solid_dynode = new G4Box(Name+"dynode_Sol",Height/2.0,
				   (Height*std::sqrt(2))/4.0,Thickness/2.);
  G4LogicalVolume* logic_dynode = new G4LogicalVolume(solid_dynode,Material, Name+"Dynode_Log",0,0,0);
  G4VisAttributes* blue_vis 
    = new G4VisAttributes(true,G4Colour::Blue());
  logic_dynode->SetVisAttributes(blue_vis);
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateX(M_PI/4*rad);

  G4VPhysicalVolume* dynodesArray_phys;
    if(NumberOfDynodes>1)
      dynodesArray_phys = new G4PVReplica(
					  Name+"Dynode_Phy",
					  logic_dynode,
					  this,
					  kZAxis, 
					  NumberOfDynodes,
					  Length/NumberOfDynodes);//Thickness); 
    else
      dynodesArray_phys = new G4PVPlacement(Rotation, G4ThreeVector(0.,0.,-Length/2.+Thickness/2.0),
					    logic_dynode,
					    Name+"Dynode_Phy",   
					    this,
					    false,
					    0,
					    true); 
    
}

void LogVol_DynodesArray::SetSolidVol_DynodesArray(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(DynodesArray_solid)
    this->SetSolid(DynodesArray_solid);
}
