/*! @file LogVol_ShieldingBSD_DFproposal.hh
  @brief Defines mandatory user class LogVol_ShieldingBSD_DFproposal.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_ShieldingBSD_DFproposal.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_ShieldingBSD_DFproposal:: LogVol_ShieldingBSD_DFproposal(G4String fname,G4double fIrad,G4double fOrad,G4double fheight,G4Material* fmaterial):
  G4LogicalVolume(new G4Box(fname+"_Sol",10*mm, 10*mm, 10*mm),fmaterial,fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  // Dimensions //
  SetOuterRadius(fOrad);
  SetInnerRadius(fIrad);
  if(fheight>0.0*mm)
    SetHeight(fheight);
  else
    {
      SetHeight(1.0*mm);
      G4cout<<"\n"<<"\n"<<"***********  WARNING MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"WARNING:: Height of the BSD shielding is zero or negative"<<G4endl;
      G4cout<<"Height of BSD shielding will be defined to 1 mm"<<"\n"<<G4endl;
    }
  // Construct solid volume //
  ConstructSolidVol_ShieldingBSD_DFproposal();
  //*** Visualization ***//
  G4VisAttributes* black_vis 
    = new G4VisAttributes(true,G4Colour(0.1,0.1,0.1));
  black_vis->SetForceWireframe(false);
  black_vis->SetForceSolid(false);
  this->SetVisAttributes(black_vis);
}

LogVol_ShieldingBSD_DFproposal::~LogVol_ShieldingBSD_DFproposal()
{}

void LogVol_ShieldingBSD_DFproposal::ConstructSolidVol_ShieldingBSD_DFproposal(void)
{
  G4double diameterhole = 25.0*mm;
  G4double poshole = 155.2/2*mm;
    
  const G4int numZPlanesShieldingBSD_DFproposal=2;
  const G4double  zPlaneShieldingBSD_DFproposal[]={0.*mm,Height};
  const G4double  rInnerShieldingBSD_DFproposal[]={InnerRadius,InnerRadius};
  const G4double  rOuterShieldingBSD_DFproposal[]={OuterRadius,OuterRadius};

  if(OuterRadius<(poshole-diameterhole/2.0))
    {
      ShieldingBSD_DFproposal_solid = new G4Polycone(Name+"_Sol",0.*rad,2*M_PI*rad,
						     numZPlanesShieldingBSD_DFproposal,
						     zPlaneShieldingBSD_DFproposal,
						     rInnerShieldingBSD_DFproposal,
						     rOuterShieldingBSD_DFproposal);
    }
  else
    {
      G4Polycone* solidShieldingBSD_withoutholes = new G4Polycone(Name+"_sol_base",0.*rad,2*M_PI*rad,
								  numZPlanesShieldingBSD_DFproposal,
								  zPlaneShieldingBSD_DFproposal,
								  rInnerShieldingBSD_DFproposal,
								  rOuterShieldingBSD_DFproposal);
      const G4int     numZPlanesHole=2;
      G4double  zPlaneHole[numZPlanesHole]={-Height,2.0*Height};
      G4double  rInnerHole[numZPlanesHole]={0.0*cm,0.0*cm};
      G4double  rOuterHole[numZPlanesHole]={diameterhole/2.0,diameterhole/2.0};

      G4Polycone* solidHole = new G4Polycone(Name+"_Hole_sol",0.*rad,2*M_PI*rad,
					     numZPlanesHole,zPlaneHole,rInnerHole,rOuterHole);

      G4SubtractionSolid*  ShieldingBSDHole1
	= new G4SubtractionSolid(Name+"_baseHole1_sol",
				 solidShieldingBSD_withoutholes, solidHole, 0,
				 G4ThreeVector(poshole,0.0*cm,0.0*cm));
  
      ShieldingBSD_DFproposal_solid = new G4SubtractionSolid(Name+"_Sol",
							     ShieldingBSDHole1, solidHole, 0,
							     G4ThreeVector(-poshole,0.0*cm,0.0*cm));
    }
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_ShieldingBSD_DFproposal();
}

void LogVol_ShieldingBSD_DFproposal::SetSolidVol_ShieldingBSD_DFproposal(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(ShieldingBSD_DFproposal_solid)
    this->SetSolid(ShieldingBSD_DFproposal_solid);
}

