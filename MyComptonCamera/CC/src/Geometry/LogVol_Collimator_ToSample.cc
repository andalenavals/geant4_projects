/*! @file LogVol_Collimator_ToSample.hh
  @brief Defines mandatory user class LogVol_Collimator_ToSample.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "LogVol_Collimator_ToSample.hh"
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

LogVol_Collimator_ToSample:: LogVol_Collimator_ToSample(G4String fname,G4double frad,G4double fheight,G4double fang, G4double fsrad, G4double fsourceh, G4Material* fmaterial):
  G4LogicalVolume(new G4Box(fname+"_Sol",10*mm, 10*mm, 10*mm),fmaterial,fname,0,0,0)
{
  /* set variables */
  SetName(fname);
  // Dimensions //
  SetRadius(frad);
  if(fheight>fsourceh)
    SetHeight(fheight);
  else
    {
      SetHeight(3.0*mm);
      G4cout<<"\n"<<"\n"<<"***********  WARNING MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"WARNING:: Height of the collimator is smaller than source"<<G4endl;
      G4cout<<"Height of collimator ro sample will be 3 mm"<<"\n"<<G4endl;
    }
  SetAngOpening(fang);
  SetSourceRadius(fsrad);
  SetSourceHeight(fsourceh);
  // Construct solid volume //
  ConstructSolidVol_Collimator_ToSample();
  //*** Visualization ***//
  G4VisAttributes* black_vis 
    = new G4VisAttributes(true,G4Colour(0.4,0.4,0.4));
  black_vis->SetForceWireframe(false);
  black_vis->SetForceSolid(false);
  this->SetVisAttributes(black_vis);
}

LogVol_Collimator_ToSample::~LogVol_Collimator_ToSample()
{}

void LogVol_Collimator_ToSample::ConstructSolidVol_Collimator_ToSample(void)
{
  G4double h1=2.0*mm;
  G4double rinner1=1.3/2.0*mm;
  G4double rinner2=rinner1+h1*std::tan(AngOp/rad);

  if(Height<4.0*mm)
    h1=Height-SourceH;
  const G4int numZPlanesCollimator_ToSample=5;
  const G4double  zPlaneCollimator_ToSample[]={0.0, 
					       -SourceH,-SourceH,
					       -SourceH-h1,
					       -Height};
  const G4double  rInnerCollimator_ToSample[]={SourceRad,SourceRad,rinner1,rinner2,MaxRadius};
  const G4double  rOuterCollimator_ToSample[]={MaxRadius,MaxRadius,MaxRadius,MaxRadius,MaxRadius};
 
  Collimator_ToSample_solid = new G4Polycone(Name+"_Sol",0.*rad,2*M_PI*rad,
					     numZPlanesCollimator_ToSample,
					     zPlaneCollimator_ToSample,
					     rInnerCollimator_ToSample,
					     rOuterCollimator_ToSample);

  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_Collimator_ToSample();
}

void LogVol_Collimator_ToSample::SetSolidVol_Collimator_ToSample(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(Collimator_ToSample_solid)
    this->SetSolid(Collimator_ToSample_solid);
}

