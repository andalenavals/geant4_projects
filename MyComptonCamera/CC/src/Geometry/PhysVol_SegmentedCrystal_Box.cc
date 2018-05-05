/*! @file LogVol_SegmentedCrystal_Box.hh
  @brief Defines mandatory user class LogVol_SegmentedCrystal_Box.
  @date September, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2.0
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "PhysVol_SegmentedCrystal_Box.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Tubs.hh"
#include "G4Polycone.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

PhysVol_SegmentedCrystal_Box:: PhysVol_SegmentedCrystal_Box(G4String fname,
							    G4RotationMatrix *pRot,
							    const G4ThreeVector &tlate,
							    G4LogicalVolume *pMotherLogical,
							    G4double flen, G4double fheight,
							    G4int fNpixelX, G4int fNpixelY ,
							    G4Material* fMaterial)
  :G4PVPlacement(pRot,tlate,
		 new G4LogicalVolume(new G4Box(fname+"_tempsol",flen/2.0,flen/2.0,fheight/2.0),
                                     (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),
                                     fname+"_templog",0,0,0),
		 fname+"_physVol",pMotherLogical,0,0,true),Material(fMaterial)
{
  /* set variables */
  SetName(fname);
  SetHeight(fheight);
  SetXPixels(fNpixelX);
  SetYPixels(fNpixelY);
  SetLength(flen);
  /* Construct solid volume */
  SegmentedCrystal_Box_LogVol=NULL;
  ConstructPhysVol_SegmentedCrystal_Box();
  /* Visualization */
  G4VisAttributes* blue_vis 
    = new G4VisAttributes(true,G4Colour::Blue());
  blue_vis->SetForceWireframe(false);
  blue_vis->SetForceSolid(false);
  GetLogicalVolume()->SetVisAttributes(G4VisAttributes::GetInvisible());
}

PhysVol_SegmentedCrystal_Box::~PhysVol_SegmentedCrystal_Box()
{}

void PhysVol_SegmentedCrystal_Box::ConstructPhysVol_SegmentedCrystal_Box(void)
{
  /* Visualization */
  G4VisAttributes* yellow_vis 
    = new G4VisAttributes(true,G4Colour(0.7,0.5,0.2));
  yellow_vis->SetForceWireframe(false);
  yellow_vis->SetForceSolid(false);
  G4VisAttributes* blue_vis 
    = new G4VisAttributes(true,G4Colour::Blue());
  blue_vis->SetForceWireframe(false);
  blue_vis->SetForceSolid(false);
 
  /*! 
    - Create the "pixels"
    - Create a volume that looks like a strip
    - Create a volume that looks like a pixel
    - The CsI detector is \c X x \c Y CsI replicas (\ref WALL_pixel_log "pixels") and place them into \ref vacuum vacuum container
    - To achieve that, we first divide (using the method Replica) the CsI volume into strips
    - Once we have the strips, we can divide them in a similar way to the previous step but now creating pixels
  */
  /* for now a strip along X*/
  G4Box* strip_solid = new G4Box("position_strip_solid",
				 Length/2.0/XPixelNum, Length/2.0, Height/2.0);
  G4LogicalVolume* strip_log = new G4LogicalVolume(strip_solid,
						   Material,
						   Name+"position_strip_log", 0,0,0);
  strip_log->SetVisAttributes(G4VisAttributes::Invisible); //never show
  /* the pixel along Y*/
  G4Box* pixel_solid = new G4Box("position_pixel_solid", 
				 Length/2.0/XPixelNum, Length/2.0/YPixelNum, Height/2.0);
  G4LogicalVolume* pixel_log = new G4LogicalVolume(pixel_solid,
						       Material,
						       Name+"pixel_csi_log", 0,0,0);
  pixel_log->SetVisAttributes(yellow_vis);// back_vis
  
  /*the [Materia] crystal is XxY CsI replicas (pixels) and place them into step 3)*/
  /* for now the strips */
   
  G4VPhysicalVolume* Crystal_strip;
  Crystal_strip = new G4PVReplica(Name+"position_strip_phy",         //name
				  strip_log,   //logical volume
				  this->GetLogicalVolume(),//its logical mother
				  kXAxis,            //replication axis
				  XPixelNum,        //number of replicas
				  Length/XPixelNum);  //width
  
  /* strip division into pixels */
  G4VPhysicalVolume* Crystal_pixel;
  Crystal_pixel = new G4PVReplica(Name+"ArraySegCystal",         //name
				  pixel_log,   //logical volume
				  strip_log,   //its logical mother
				  kYAxis,            //replication axis
				  YPixelNum,        //number of replicas
				  Length/YPixelNum);  //width

  SegmentedCrystal_Box_LogVol=pixel_log;
}


