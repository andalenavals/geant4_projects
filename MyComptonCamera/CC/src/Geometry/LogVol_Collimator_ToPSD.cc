/*! @file LogVol_Collimator_ToPSD.hh
  @brief Defines mandatory user class LogVol_Collimator_ToPSD.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_Collimator_ToPSD.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Box.hh"
#include "G4Polycone.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_Collimator_ToPSD:: LogVol_Collimator_ToPSD(G4String fname,G4double frad,G4double fheight,G4double fang,G4Material* fmaterial):
  G4LogicalVolume(new G4Box(fname+"_Sol",10*mm, 10*mm, 10*mm),fmaterial,fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  // Dimensions //
  SetRadius(frad);
  if(fheight>2.0*mm)
    SetHeight(fheight);
  else
    {
      SetHeight(2.0*mm);
      G4cout<<"\n"<<"\n"<<"***********  WARNING MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"WARNING:: Height of the collimator is smaller than source"<<G4endl;
      G4cout<<"Height of collimator ro sample will be 2 mm"<<"\n"<<G4endl;
    }
  SetAngOpening(fang);
  // Construct solid volume //
  ConstructSolidVol_Collimator_ToPSD();
  //*** Visualization ***//
  G4VisAttributes* black_vis 
    = new G4VisAttributes(true,G4Colour(0.4,0.4,0.4));
  black_vis->SetForceWireframe(false);
  black_vis->SetForceSolid(false);
  this->SetVisAttributes(black_vis);
}

LogVol_Collimator_ToPSD::~LogVol_Collimator_ToPSD()
{}

void LogVol_Collimator_ToPSD::ConstructSolidVol_Collimator_ToPSD(void)
{
  G4double hcog = 1.0*mm;
  G4double rinner1=1.3/2.0*mm;
  G4double rinner2=rinner1+(Height-hcog)*std::tan(AngOp/rad);
  G4double rinner3=MaxRadius-2.05*mm;
  
  const G4int numZPlanesCollimator_ToPSD=4;
  const G4double  zPlaneCollimator_ToPSD[]={0.*mm,Height-hcog,Height-hcog,Height};
  const G4double  rInnerCollimator_ToPSD[]={rinner1,rinner2,rinner3,rinner3};
  const G4double  rOuterCollimator_ToPSD[]={MaxRadius,MaxRadius,MaxRadius,MaxRadius};
 
  Collimator_ToPSD_solid = new G4Polycone(Name+"_Sol",0.*rad,2*M_PI*rad,
					     numZPlanesCollimator_ToPSD,
					     zPlaneCollimator_ToPSD,
					     rInnerCollimator_ToPSD,
					     rOuterCollimator_ToPSD);

  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_Collimator_ToPSD();
}

void LogVol_Collimator_ToPSD::SetSolidVol_Collimator_ToPSD(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(Collimator_ToPSD_solid)
    this->SetSolid(Collimator_ToPSD_solid);
}

