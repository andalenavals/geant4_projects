/*! @file LogVol_ShieldingPSDCone.hh
  @brief Defines mandatory user class LogVol_ShieldingPSDCone.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_ShieldingPSDCone.hh"
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

LogVol_ShieldingPSDCone:: LogVol_ShieldingPSDCone(G4String fname,G4double fIrad,G4double fOrad,G4double fheight,G4double fang,G4Material* fmaterial):
  G4LogicalVolume(new G4Box(fname+"_Sol",10*mm, 10*mm, 10*mm),fmaterial,fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  // Dimensions //
  SetOuterRadius(fOrad);
  SetInnerRadius(fIrad);
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
  ConstructSolidVol_ShieldingPSDCone();
  //*** Visualization ***//
  G4VisAttributes* black_vis 
    = new G4VisAttributes(true,G4Colour(0.1,0.1,0.1));
  black_vis->SetForceWireframe(false);
  black_vis->SetForceSolid(false);
  this->SetVisAttributes(black_vis);
}

LogVol_ShieldingPSDCone::~LogVol_ShieldingPSDCone()
{}

void LogVol_ShieldingPSDCone::ConstructSolidVol_ShieldingPSDCone(void)
{
  G4double hcog = 1.0*mm;
  G4double rinner1=InnerRadius+hcog*std::tan(AngOp/rad);
  G4double rinner2=rinner1+(Height-hcog)*std::tan(AngOp/rad);
  G4double MaxRadius2=OuterRadius+(Height-hcog)*std::tan((AngOp-5.0*deg)/rad);
    
  const G4int numZPlanesShieldingPSDCone=3;
  const G4double  zPlaneShieldingPSDCone[]={0.*mm,hcog,Height};
  const G4double  rInnerShieldingPSDCone[]={InnerRadius,rinner1,rinner2};
  const G4double  rOuterShieldingPSDCone[]={OuterRadius,OuterRadius,MaxRadius2};
 
  ShieldingPSDCone_solid = new G4Polycone(Name+"_Sol",0.*rad,2*M_PI*rad,
					     numZPlanesShieldingPSDCone,
					     zPlaneShieldingPSDCone,
					     rInnerShieldingPSDCone,
					     rOuterShieldingPSDCone);

  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_ShieldingPSDCone();
}

void LogVol_ShieldingPSDCone::SetSolidVol_ShieldingPSDCone(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(ShieldingPSDCone_solid)
    this->SetSolid(ShieldingPSDCone_solid);
}

