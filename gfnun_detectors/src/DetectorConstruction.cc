/*! @file DetectorConstruction.cc
  @brief Defines mandatory user class DetectorConstruction.
  @date Novermber, 2011
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  @author David Flechas 
  In this source file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "Materials.hh"
#include "Construct_log_vol_detectorGFNUN.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VisAttributes.hh"

#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* for UpdateGeometry */
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

/* Pixel Number_x,y*/
#define PixelNumber 64   //// PixelNumber X PixelNumber grid
#define PixelNumber_vis 6

DetectorConstruction::DetectorConstruction()
{
  //! \li Create a messenger (defines custom UI commands)
  messenger = new DetectorMessenger(this);
  
  /**********************/
  //  Detector Counter  //
  /**********************/
  DetectorCounter=0;
  //! maximum number of sensitive volumes: One per each detector type
  MaximumNumberOfDetectors = 7;
  /* flags */
  constructed = false;
  actual_geometry = false; /* true for round CsI position detector. NOT segmented */
  Pimp_flag = true;
  
  /****************/
  /* OPTICAL FLAG */
  /****************/
  Optical_flag = false;

  /***************/
  /*  MATERIALS  */
  /***************/
  air = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
  materials = NULL;
  
  /* Source capsule -> source container*/

  SourceCapsulePosition = G4ThreeVector();
  SourceCapsulerotationX = 0.;
  SourceCapsulerotationY = 0.;
  /// Castle shielding: paraffin + lead (Shielding of 252Cf source)
  CastleShieldingPosition = G4ThreeVector();
  CastleShielding_LeadThickness = 5.0*cm;
  CastleShielding_LeadRoofThickness= 5.0*cm;
  CastleShielding_ParaffinRadius=55.0/2.0*cm;
  CastleShielding_Height = 45.0*cm;

  /**************************************************/
  /* Detectors: construct, position and orientation */
  /*              default values                    */
  /**************************************************/
  Ge10_flag = false;
  Ge40_flag = false;
  NaI2x2_flag = false;
  NaI3x3_flag = false;
  Plastic_flag = false;
  CsI_flag = false;
  Dummy_flag = false;

  Ge10Position = G4ThreeVector();
  Ge40Position = G4ThreeVector();
  NaI2x2Position = G4ThreeVector();
  NaI3x3Position = G4ThreeVector();
  PlasticPosition = G4ThreeVector();
  CsIPosition = G4ThreeVector();
  DummyPosition = G4ThreeVector();
  
  Ge10rotationX = 0.;
  Ge10rotationY = 0.;
  Ge40rotationX = 0.;
  Ge40rotationY = 0.;
  NaI2x2rotationX = 0.;
  NaI2x2rotationY = 0.;
  NaI3x3rotationX = 0.;
  NaI3x3rotationY = 0.;
  PlasticrotationX = 0.;
  PlasticrotationY = 0.;
  CsIrotationX = 0.;
  CsIrotationY = 0.;
  DummyrotationX = 0.;
  DummyrotationY = 0.;

  Detector_Ge10_log = NULL;
  Detector_Ge40_log = NULL;
  Detector_NaI_2x2_log = NULL;
  Detector_NaI_3x3_log = NULL;
  Detector_Plastic_log = NULL;
  Detector_CsI_log = NULL;
  Detector_dummy_log = NULL;

  /* Sensitive volumes*/
  Detector_Ge10_sensitiveVol_log = NULL;
  Detector_Ge40_sensitiveVol_log = NULL;
  Detector_NaI_2x2_sensitiveVol_log = NULL;
  Detector_NaI_3x3_sensitiveVol_log = NULL;
  Detector_Plastic_sensitiveVol_log = NULL;
  Detector_CsI_sensitiveVol_log = NULL;
  Detector_dummy_sensitiveVol_log = NULL;

  /* Plate material*/
  Plate_Material = "G4_Fe";
  /*Plate thickness*/
  Plate_thickness = 5.0*mm;
}

DetectorConstruction::~DetectorConstruction()
{
  if (materials)         delete materials;
  delete messenger;             
}

/*!
  This function is called by G4 when the detector has to be created\n
  Definitions of Solids, Logical Volumes and Physical Volumes comes here.
  //! \sa ConstructEnclosure(), ConstructIMPLANT(), ConstructSiDetectors(), ConstructGeDetectors() and Sensibilize()
  */

G4VPhysicalVolume* DetectorConstruction::Construct()
{ 
  //------------------------------
  // World
  //------------------------------
  G4double halfWorldLength = 3.3* m;

  G4Box * solidWorld= new G4Box("world",halfWorldLength,halfWorldLength,halfWorldLength);
  logicWorld = new G4LogicalVolume(solidWorld,
				   G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"),
				   "World", 0, 0, 0);
  logicWorld -> SetVisAttributes(G4VisAttributes::GetInvisible()); //never show

  //! \li Must place the \ref logicWorld "World" physical volume unrotated at \f$(0,0,0)\f$.
  physiWorld = new G4PVPlacement(0,               // no rotation
				 G4ThreeVector(), // at (0,0,0)
				 logicWorld,      // its logical volume
				 "World",         // its name
				 0,               // its mother  volume
				 false,           // no boolean operations
				 0);              // copy number


  /*************************************/
  /*****   REAL DEAL FROM HERE ON  *****/
  /*************************************/
  ResetDetectorCounter();
  G4bool vis_flag,solid_flag,Xray_flag,vis_flag_change;
 
  //ConstructCapsule(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(Ge10_flag)
    ConstructGe10Detector(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(Ge40_flag)
    ConstructGe40Detector(vis_flag=true, solid_flag=true, Xray_flag=false);
  if(NaI2x2_flag)
    ConstructNaI2x2Detector(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(NaI3x3_flag)
    ConstructNaI3x3Detector(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(Plastic_flag)
    ConstructPlasticDetector(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(CsI_flag)
    ConstructCsIDetector(vis_flag=true, solid_flag=false, Xray_flag=false);
  if(Dummy_flag)
    ConstructDummyDetector(vis_flag=true, solid_flag=false, Xray_flag=false);
 
  //  ConstructSetup_Transmission(vis_flag=true, solid_flag=true,Xray_flag=false);
  //ConstructPlate(vis_flag=true, solid_flag=true,Xray_flag=false);
  //ConstructSetup_Flechas();
  ConstructCapsule(vis_flag=true, solid_flag=false, Xray_flag=false);
  //Construct_CastleShielding(vis_flag=true, solid_flag=false, Xray_flag=false);
  Sensibilize();

  /** In this routine the logical volumes to score get register with
      the SDmanager **/
  constructed = true;
  
  materials = Materials::GetInstance();
  //! \li Always return the physical World
  return physiWorld;
}

void DetectorConstruction::ConstructCapsule(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  //////----------------------------------------------------------///////
  //////----------- Acrilyc_Capsule :: Source container ----------///////
  //////-----------------------------------------------------------///////
  
  G4VisAttributes* red_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Red());
  red_vis->SetForceSolid(solid_flag);
  red_vis->SetForceWireframe(Xray_flag);
  
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateX(-M_PI/2.);
  Rotation->rotateX(SourceCapsulerotationX);
  Rotation->rotateY(SourceCapsulerotationY);

  G4double Acrilyc_Capsule_Rinner = 0.*cm;
  G4double Acrilyc_Capsule_Router = 0.63*cm;
  G4double Acrilyc_Capsule_length_z = 0.2*cm;

  G4Tubs* solid_Acrilyc_Capsule;
  solid_Acrilyc_Capsule = new G4Tubs("acrilycCapsule", Acrilyc_Capsule_Rinner,Acrilyc_Capsule_Router,
				     Acrilyc_Capsule_length_z/2.,0.*rad,2*M_PI*rad); 
  G4LogicalVolume* logic_Acrilyc_Capsule;
  logic_Acrilyc_Capsule = new G4LogicalVolume(solid_Acrilyc_Capsule,(G4NistManager::Instance())->FindOrBuildMaterial("G4_POLYACRYLONITRILE"), "AcrilycCapsule_source",0,0,0);

  logic_Acrilyc_Capsule->SetVisAttributes(red_vis);
  
  G4VPhysicalVolume* physical_Acrilyc_Capsule;
  physical_Acrilyc_Capsule = new G4PVPlacement(Rotation,         // no rotation
					       SourceCapsulePosition, //Completa
					       logic_Acrilyc_Capsule,    // its logical volume
					       "AcrilycCapsule_source",      // its name
					       logicWorld,
					       false,     // no boolean operations 
					       0,
					       true); 
}

void DetectorConstruction::ConstructCapsule_252Cf(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  //////----------------------------------------------------------///////
  //////----------- 252Cf_Capsule :: Source container ----------///////
  //////-----------------------------------------------------------///////
  
  G4VisAttributes* red_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Red());
  red_vis->SetForceSolid(solid_flag);
  red_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* beige_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.6,0.5,0.0));
  beige_vis->SetForceSolid(solid_flag);
  beige_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* darkgray_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.5,0.5,0.5));
  darkgray_vis->SetForceSolid(solid_flag);
  darkgray_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.9,0.9,0.9));
  gray_vis->SetForceSolid(solid_flag);
  gray_vis->SetForceWireframe(Xray_flag);
  
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateX(-M_PI/2.);
  Rotation->rotateX(SourceCapsulerotationX);
  Rotation->rotateY(SourceCapsulerotationY);

  ///**** Testing::: check these part ****/// 
  G4double Metallic_Capsule_Rinner = 0.0*mm;
  G4double Metallic_Capsule_Router = 7.8*mm;
  G4double Metallic_Capsule_length_z = 10.0*mm;

  G4Tubs* solid_Metallic_Capsule;
  solid_Metallic_Capsule = new G4Tubs("metallicCapsule", Metallic_Capsule_Rinner,
				      Metallic_Capsule_Router,
				      Metallic_Capsule_length_z/2.,0.*rad,2*M_PI*rad); 
  G4LogicalVolume* logic_Metallic_Capsule;
  logic_Metallic_Capsule = new G4LogicalVolume(solid_Metallic_Capsule,(G4NistManager::Instance())->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "MetallicCapsule_source",0,0,0); 

  logic_Metallic_Capsule->SetVisAttributes(darkgray_vis);
  
  G4VPhysicalVolume* physical_Metallic_Capsule;
  physical_Metallic_Capsule = new G4PVPlacement(Rotation,         // no rotation
  						SourceCapsulePosition, //Completa
  						logic_Metallic_Capsule,    // its logical volume
  						"MetallicCapsule_source",      // its name
  						logicWorld,
  						false,     // no boolean operations 
  						0,
  						true); 
  G4double Ceramic_innerCapsule_Rinner = 0.0*mm;
  G4double Ceramic_innerCapsule_Router = 4.6*mm; // estimated
  G4double Ceramic_innerCapsule_length_z = 4.6*mm; // estimated

  G4Tubs* solid_Ceramic_innerCapsule;
  solid_Ceramic_innerCapsule = new G4Tubs("ceramicinnerCapsule", Ceramic_innerCapsule_Rinner,
					  Ceramic_innerCapsule_Router,
					  Ceramic_innerCapsule_length_z/2.,0.*rad,2*M_PI*rad); 
  G4LogicalVolume* logic_Ceramic_innerCapsule;
  logic_Ceramic_innerCapsule = new G4LogicalVolume(solid_Ceramic_innerCapsule,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Ta"), "CeramicinnerCapsule_source",0,0,0);//check

  logic_Ceramic_innerCapsule->SetVisAttributes(beige_vis);
  
  G4VPhysicalVolume* physical_Ceramic_innerCapsule;
  physical_Ceramic_innerCapsule = new G4PVPlacement(0,               // Rotation
						    G4ThreeVector(0.,0.,0.), // Position
						    logic_Ceramic_innerCapsule, // its logical volume
						    "CeramicinnerCapsule_source",      // its name
						    logic_Metallic_Capsule,
						    false,     // no boolean operations 
						    0,
						    true); 
}

void DetectorConstruction::Construct_CastleShielding(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4VisAttributes* black_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  black_vis->SetForceSolid(solid_flag);
  black_vis->SetForceWireframe(Xray_flag);

  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.9,0.9,0.9));
  gray_vis->SetForceSolid(solid_flag);
  gray_vis->SetForceWireframe(Xray_flag);
  
  G4VisAttributes* beige_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.6,0.5,0.0));
  beige_vis->SetForceSolid(solid_flag);
  beige_vis->SetForceWireframe(Xray_flag);

  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateX(-M_PI/2.);
  
  G4double hole_height = 12.0*cm;
  G4double hole_radius  = 11.0/2.0*cm;
  
  G4double paraffin_height = 50.0*cm;
  G4double paraffin_large  = 50.0*cm;
  G4double paraffin_width  = 10.0*cm;

  G4Box* solid_CS_lead_block = new G4Box("cs_lead_block", CastleShielding_ParaffinRadius+CastleShielding_LeadThickness,CastleShielding_ParaffinRadius+CastleShielding_LeadThickness,CastleShielding_Height/2.0);
  G4Box* solid_CS_lead_hole = new G4Box("cs_lead_hole",CastleShielding_ParaffinRadius,CastleShielding_ParaffinRadius,CastleShielding_Height/2.0+10*cm);
  G4SubtractionSolid*  solid_CS_lead_Wall = new G4SubtractionSolid("cs_lead_Wall", solid_CS_lead_block, solid_CS_lead_hole,0, G4ThreeVector());
  
  G4LogicalVolume* logic_CS_lead_Wall = new G4LogicalVolume(solid_CS_lead_Wall,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"), "CS_Lead_Wall_log",0,0,0);

  G4VPhysicalVolume* physical_CS_lead_Wall =  new G4PVPlacement(Rotation,
								G4ThreeVector(0.,CastleShielding_Height/2.0,0.0),
								logic_CS_lead_Wall,
								"CS_Lead_Wall",
								logicWorld,
								false,  
								0,
								true);
  logic_CS_lead_Wall->SetVisAttributes(black_vis);

  G4Tubs* solid_CS_paraffin_block = new G4Tubs("acrilycCapsule", 0.0*cm,CastleShielding_ParaffinRadius,CastleShielding_Height/2.0,0.*rad,2*M_PI*rad); 

  G4Tubs* solid_CS_paraffin_hole = new G4Tubs("acrilycCapsule", 0.0*cm,11.0/2.0*cm,12.0/2.0*cm,0.*rad,2*M_PI*rad);
  G4SubtractionSolid*  solid_CS_paraffin_Wall = new G4SubtractionSolid("cs_paraffin_Wall", solid_CS_paraffin_block, solid_CS_paraffin_hole, Rotation,G4ThreeVector());
  
  G4LogicalVolume* logic_CS_paraffin_Wall = new G4LogicalVolume(solid_CS_paraffin_Wall,(G4NistManager::Instance())->FindOrBuildMaterial("G4_PARAFFIN"), "CS_Paraffin_Wall_log",0,0,0);

  G4VPhysicalVolume* physical_CS_paraffin_Wall =  new G4PVPlacement(Rotation,
								G4ThreeVector(0.,CastleShielding_Height/2.0,0.0),
								logic_CS_paraffin_Wall,
								"CS_Paraffin_Wall",
								logicWorld,
								false,  
								0,
								true);
  logic_CS_paraffin_Wall->SetVisAttributes(gray_vis);
  
  G4Box* solid_CS_lead_Roof = new G4Box("cs_lead_Roof_block", CastleShielding_ParaffinRadius/std::sqrt(2),CastleShielding_ParaffinRadius/std::sqrt(2),CastleShielding_LeadRoofThickness/2.0);
  
  G4LogicalVolume* logic_CS_lead_Roof = new G4LogicalVolume(solid_CS_lead_Roof,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"), "CS_Lead_Roof_log",0,0,0);
  
  G4VPhysicalVolume* physical_CS_lead_Roof =  new G4PVPlacement(Rotation,
								G4ThreeVector(0.,CastleShielding_Height+CastleShielding_LeadRoofThickness/2.0,0.0),
								logic_CS_lead_Roof,
								"CS_Lead_Roof",
								logicWorld,
								false,  
								0,
								true);
  logic_CS_lead_Roof->SetVisAttributes(black_vis);
}

void DetectorConstruction::ConstructSetup_Transmission(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4VisAttributes* black_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  black_vis->SetForceSolid(solid_flag);
  black_vis->SetForceWireframe(Xray_flag);

  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.9,0.9,0.9));
  gray_vis->SetForceSolid(solid_flag);
  gray_vis->SetForceWireframe(Xray_flag);
  
  G4VisAttributes* beige_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.6,0.5,0.0));
  beige_vis->SetForceSolid(solid_flag);
  beige_vis->SetForceWireframe(Xray_flag);

  // /******************* Floor ********************/ //
  // /*: The experiment was located on a lead base */ //
  
  G4RotationMatrix* Rotation_floor 
    = new G4RotationMatrix();
  Rotation_floor->rotateX(-M_PI/2.);
  
  G4double floor_height = 50.0*cm;
  G4double floor_large  = 50.0*cm;
  G4double floor_width  = 5.0*cm;
  G4double wood_width   = 1.0*cm;

  G4double paraffin_height = 50.0*cm;
  G4double paraffin_large  = 50.0*cm;
  G4double paraffin_width  = 10.0*cm;

  G4Box * solid_floor = new G4Box("floor",floor_height/2.,floor_large/2.,floor_width/2.);

  G4LogicalVolume* logic_floor = new G4LogicalVolume(solid_floor,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"), "Floor",0,0,0);

  G4VPhysicalVolume* physical_floor =  new G4PVPlacement(Rotation_floor,
							 G4ThreeVector(0.,-1.0*mm-floor_width/2.-2*wood_width-2*paraffin_width,0.)
							 + SourceCapsulePosition,
							 logic_floor,
							 "Floor",
							 logicWorld,
							 false,  
							 0,
							 true);
  logic_floor->SetVisAttributes(black_vis);

  G4Box * solid_paraffin_1 = new G4Box("paraffin_1",paraffin_height/2.,paraffin_large/2.,paraffin_width/2.);

  G4LogicalVolume* logic_paraffin_1 = new G4LogicalVolume(solid_paraffin_1,(G4NistManager::Instance())->FindOrBuildMaterial("G4_PARAFFIN"), "Paraffin_1",0,0,0);

  G4VPhysicalVolume* physical_paraffin_1 =  new G4PVPlacement(Rotation_floor,
							      G4ThreeVector(0.,-1.0*mm-paraffin_width/2.-2*wood_width-paraffin_width,0.)
							      + SourceCapsulePosition,
							      logic_paraffin_1,
							      "Paraffin_1",
							      logicWorld,
							      false,  
							      0,
							      true);
  logic_paraffin_1->SetVisAttributes(gray_vis);

  G4Box * solid_wood_1 = new G4Box("wood_1",floor_height/2.,floor_large/2.,wood_width/2.);

  G4LogicalVolume* logic_wood_1 = new G4LogicalVolume(solid_wood_1,(G4NistManager::Instance())->FindOrBuildMaterial("Wood"), "WoodTable1",0,0,0);
 
  G4VPhysicalVolume* physical_wood_1 =  new G4PVPlacement(Rotation_floor,
							  G4ThreeVector(0.,-1.0*mm-wood_width/2.-wood_width-paraffin_width,0.)
							  + SourceCapsulePosition,
							  logic_wood_1,
							  "Wood_1",
							  logicWorld,
							  false,  
							  0,
							  true);

  logic_wood_1->SetVisAttributes(beige_vis);

  G4Box * solid_paraffin_2 = new G4Box("paraffin_2",(paraffin_height-20.0*cm)/2.,(paraffin_large-10.0*cm)/2.,paraffin_width/2.);

  G4LogicalVolume* logic_paraffin_2 = new G4LogicalVolume(solid_paraffin_2,(G4NistManager::Instance())->FindOrBuildMaterial("G4_PARAFFIN"), "Paraffin_2",0,0,0);

  G4VPhysicalVolume* physical_paraffin_2 =  new G4PVPlacement(Rotation_floor,
							      G4ThreeVector(0.,-1.0*mm-paraffin_width/2.-wood_width,0.)
							      + SourceCapsulePosition,
							      logic_paraffin_2,
							      "Paraffin_2",
							      logicWorld,
							      false,  
							      0,
							      true);
  logic_paraffin_2->SetVisAttributes(gray_vis);


  G4Box * solid_wood_2 = new G4Box("wood_2",(floor_height-20.0*cm)/2.,(floor_large-10.0*cm)/2.,wood_width/2.);

  G4LogicalVolume* logic_wood_2 = new G4LogicalVolume(solid_wood_2,(G4NistManager::Instance())->FindOrBuildMaterial("Wood"), "WoodTable2",0,0,0);
 
  G4VPhysicalVolume* physical_wood_2 =  new G4PVPlacement(Rotation_floor,
							  G4ThreeVector(0.,-1.0*mm-wood_width/2.,0.)
							  + SourceCapsulePosition,
							  logic_wood_2,
							  "Wood_2",
							  logicWorld,
							  false,  
							  0,
							  true);

  logic_wood_2->SetVisAttributes(beige_vis);

  G4double collimator_height  = 12.0*cm;
  G4double collimator_large   = 15.0*cm;
  G4double collimator_width   = collimator_large;
  G4double collimator_hole_large   = 2.5*cm;
  G4double collimator_hole_width   = 3.0*cm;


  G4Box * solid_collimator_compact = new G4Box("collimator_compact",collimator_height/2.,collimator_large/2.,collimator_width/2.);

  G4Box * solid_collimator_hole = new G4Box("collimator_hole",collimator_hole_width/2.,collimator_hole_large/2.,(collimator_width+collimator_width/5.)/2.);

  G4SubtractionSolid*  solid_collimator
    = new G4SubtractionSolid("collimator", solid_collimator_compact, solid_collimator_hole,0, G4ThreeVector());

  G4LogicalVolume* logic_collimator = new G4LogicalVolume(solid_collimator,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"), "Collimator",0,0,0);
  G4RotationMatrix* Rotation_collimator 
    = new G4RotationMatrix();
  Rotation_collimator->rotateY(M_PI/2.);

  G4VPhysicalVolume* physical_collimator =  new G4PVPlacement(Rotation_floor,
							      G4ThreeVector(0.,collimator_width/2.,0.)
							      + SourceCapsulePosition,
							      logic_collimator,
							      "Collimator",
							      logicWorld,
							      false,  
							      0,
							      true);
  logic_collimator->SetVisAttributes(black_vis);
  
} 

void DetectorConstruction::ConstructSetup_Flechas(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4VisAttributes* black_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  black_vis->SetForceSolid(true);
  //  black_vis->SetForceWireframe(true);
  
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Gray());
  gray_vis->SetForceSolid(true);
  // gray_vis->SetForceWireframe(true);
  
  G4VisAttributes* red_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.8,0.8));
  red_vis->SetForceSolid(solid_flag);
  // red_vis->SetForceWireframe(true);

  G4VisAttributes* tung_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.7,0.7,0.2));
  tung_vis->SetForceSolid(true);
  //  tung_vis->SetForceWireframe(true);

  ///////------------------------------------------------------///////
  ///////------ Cámara Compton ::: Blindaje de Plomo  ---------///////
  ///////------------------------------------------------------///////
  /*G4double s=0.0*cm;

    G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
    Rotation->rotateX(-M_PI/2.);

    const G4int numZPlanesBlinPbCC=3;
    const G4double  zPlaneBlinPbCC[]={0.*mm,1.*mm,29.76*mm};
    const G4double  rInnerBlinPbCC[]={5.56/2*mm,6.95/2*mm ,47.05/2*mm};
    const G4double  rOuterBlinPbCC[]={33.40/2*mm,33.40/2*mm, 66.49/2*mm};
 
    G4Polycone* solidBlindajePbCC
    = new G4Polycone("blindajePbCC",0.*rad,2*M_PI*rad,numZPlanesBlinPbCC,zPlaneBlinPbCC,rInnerBlinPbCC,rOuterBlinPbCC);
    G4LogicalVolume* logicBlindajePbCC
    = new G4LogicalVolume(solidBlindajePbCC,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"),"BlindajePbCC",0,0,0);
    logicBlindajePbCC->SetVisAttributes(black_vis);
    // logicBlindajePbCC->SetVisAttributes(G4VisAttributes::GetInvisible());
    G4VPhysicalVolume* physiBlindajePbCC;
    physiBlindajePbCC= new G4PVPlacement(0,               // Rotacion
    G4ThreeVector(0.,0.,19.06*mm+s), //Completa
    logicBlindajePbCC,   // its logical volume
    "BlindajePbCC",     // its name	      
    logicWorld,   // its mother  volume
    false,     // no boolean operations 
    0,
    true); 


    ///////-------------------------------------------/////////
    ///////----------------- Colimadores--------------//////////
    ///////-------------------------------------------//////////
 
    ///////-------------------------------------------//////////
    ///////---- Colimador Detectro de Posicion V2 ----//////////
    ///////-------------------------------------------//////////
 
    const G4int numZPlanesColDetPos=6;
    const G4double  zPlaneColDetPos[]={-1.0*mm,0.*mm,0.*mm,3.56*mm,3.56*mm,4.56*mm};
    const G4double  rInnerColDetPos[]={33.41/2*mm,33.41/2*mm,1.28/2*mm,5.56/2*mm,33.41/2*mm,33.41/2*mm};
    const G4double  rOuterColDetPos[]={37.50/2*mm,37.50/2*mm,37.50/2*mm,37.50/2*mm,37.50/2*mm,37.50/2*mm};
 
    G4Polycone* solidColimadorDetPos
    = new G4Polycone("colimadorDetPos",0.*rad,2*M_PI*rad,numZPlanesColDetPos,zPlaneColDetPos,rInnerColDetPos,rOuterColDetPos);
    G4LogicalVolume* logicColimadorDetPos
    = new G4LogicalVolume(solidColimadorDetPos,(G4NistManager::Instance())->FindOrBuildMaterial("G4_W"),"ColimadorDetPos",0,0,0);
    logicColimadorDetPos->SetVisAttributes(tung_vis);
    // logicColimadorDetPos->SetVisAttributes(G4VisAttributes::GetInvisible());
    G4VPhysicalVolume* physiColimadorDetPos;
    physiColimadorDetPos= new G4PVPlacement(0,               // Rotacion
    G4ThreeVector(0.,0.,15.5*mm), //Completa
    logicColimadorDetPos,   // its logical volume
    "ColimadorDetPos",     // its name	      
    logicWorld,   // its mother  volume
    false,     // no boolean operations 
    0,
    true); 
 
    ///////------------------------------------------------//////////
    ///////--- Colimador Detectro de Retrodispersion V2 ---//////////
    ///////------------------------------------------------//////////
 
    const G4int numZPlanesColDetRetro=5;
    const G4double  zPlaneColDetRetro[]={0.*mm,7.*mm,9.*mm,9.*mm,11.*mm};
    const G4double  rInnerColDetRetro[]={56./2.*mm,3.8/2.*mm,1.3/2.*mm,6.3*mm,6.3*mm};
    const G4double  rOuterColDetRetro[]={56./2.*mm,56./2.*mm,56./2.*mm,56./2.*mm,56./2.*mm};
 
 
 
    G4Polycone* solidColimadorDetRetro
    = new G4Polycone("colimadorDetRetro",0.*rad,2*M_PI*rad,numZPlanesColDetRetro,zPlaneColDetRetro,rInnerColDetRetro,rOuterColDetRetro);

    G4SubtractionSolid*  solid_collimator
    = new G4SubtractionSolid("collimator", solidColimadorDetRetro, solidColimadorDetPos,0, G4ThreeVector(0.,0.,11.0*mm));

    G4LogicalVolume* logicColimadorDetRetro
    = new G4LogicalVolume(solid_collimator,(G4NistManager::Instance())->FindOrBuildMaterial("G4_W"),"ColimadorDetRetro",0,0,0);
    logicColimadorDetRetro->SetVisAttributes(tung_vis);

    // logicColimadorDetRetro->SetVisAttributes(G4VisAttributes::GetInvisible());
    G4VPhysicalVolume* physiColimadorDetRetro;
    physiColimadorDetRetro= new G4PVPlacement(0,               // Rotacion
    G4ThreeVector(0.,0.,4.5*mm-s), //Completa
    logicColimadorDetRetro,   // its logical volume
    "ColimadorDetRetro",     // its name	      
    logicWorld,   // its mother  volume
    false,     // no boolean operations 
    0,
    true); 
  */



  //  // /* It is a especific function used to simulate Flechas's experiments */

  //  G4VisAttributes* black_vis 
  //    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  //  black_vis->SetForceSolid(solid_flag);
  //  black_vis->SetForceWireframe(Xray_flag);
  
  //  G4VisAttributes* gray_vis 
  //    = new G4VisAttributes(vis_flag,G4Colour(0.8,0.8,0.8));
  //  gray_vis->SetForceSolid(solid_flag);
  //  gray_vis->SetForceWireframe(Xray_flag);

  //   // /******************* Floor ********************/ //
  //  // /*: The experiment was located on a lead base */ //
  
  // G4RotationMatrix* Rotation_floor 
  //    = new G4RotationMatrix();
  //  Rotation_floor->rotateX(-M_PI/2.);

  //  G4double floor_height   = 55.0*cm;
  //  G4double floor_large    = 50.0*cm;
  //  G4double floor_large_2  = 10.0*cm;
  //  G4double floor_width    = 5.0*cm;

  //  G4Box * solid_floor_1 = new G4Box("floor_1",floor_height/2.,floor_large/2.,floor_width/2.);
  //  G4Box * solid_floor_2 = new G4Box("floor_2",(floor_height-floor_large_2)/2.,
  // 				    floor_large_2/2.,floor_width/2.);
  //  G4Box * solid_floor_3 = new G4Box("floor_3",(floor_height-3*floor_large_2)/2.,
  // 				    floor_large_2/2.,floor_width/2.);

  //  G4UnionSolid * solid_floor_U_1 = new G4UnionSolid("floor_U1",solid_floor_1,solid_floor_2,0,
  // 						    G4ThreeVector(floor_large_2/2.,
  // 								  floor_large/2.+floor_large_2/2.,
  // 								  0.));

  //  G4UnionSolid * solid_floor = new G4UnionSolid("floor",solid_floor_U_1,solid_floor_3,0,
  // 						G4ThreeVector(3*floor_large_2/2.,
  // 							      floor_large/2.+3*floor_large_2/2.,
  // 							      0.));
  
  
  //  G4LogicalVolume* logic_floor = new G4LogicalVolume(solid_floor,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"), "Floor",0,0,0);

  //  G4VPhysicalVolume* physical_floor =  new G4PVPlacement(Rotation_floor,
  // 							 G4ThreeVector(floor_height/2.+25*cm,0.,
  // 								       -floor_large/2.-floor_large_2),
  // 							 logic_floor,
  // 							 "Floor",
  // 							 logicWorld,
  // 							 false,  
  // 							 0,
  // 							 true);
  //  logic_floor->SetVisAttributes(black_vis);

  //  G4double floor_table_height   = 108.0*cm;
  //  G4double floor_table_large    = 75.0*cm;
  //  G4double floor_table_width    = 0.13*cm;

  //  G4Box * solid_floor_table = new G4Box("floor_table",floor_table_height/2.,floor_table_large/2.,floor_table_width/2.);

  //  G4LogicalVolume* logic_floor_table = new G4LogicalVolume(solid_floor_table,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Fe"), "Floor_Table",0,0,0);
  
  //  G4VPhysicalVolume* physical_floor_table =  new G4PVPlacement(Rotation_floor,
  // 							       G4ThreeVector(floor_table_height/2.-20*cm,
  // 									     -floor_width/2.-floor_table_width,
  // 									     -floor_table_large/2.+10*cm),
  // 							       logic_floor_table,
  // 							       "Floor_Table",
  // 							       logicWorld,
  // 							       false,  
  // 							       0,
  // 							       true);
  //  logic_floor_table->SetVisAttributes(gray_vis);
} 


void DetectorConstruction::ConstructPlate(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4VisAttributes* black_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  black_vis->SetForceSolid(solid_flag);
  black_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Gray());
  gray_vis->SetForceSolid(solid_flag);
  gray_vis->SetForceWireframe(Xray_flag);

  G4VSolid* solidPlate;
  G4LogicalVolume* logicPlate;
  G4VPhysicalVolume* physiPlate;

  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateX(pi/2.);
  G4double PlateWidth  = 20.5*cm; 
  G4double PlateLength = 33.*cm;
  G4double caracteristic_length_p = 16.5*cm;
  G4double center_top_p = Plate_thickness/2.;

  if(Plate_thickness > 0. )
    {
      solidPlate = new G4Box("plate", PlateLength/2. , PlateWidth/2., Plate_thickness/2.); 
      logicPlate = new G4LogicalVolume(solidPlate,G4NistManager::Instance()->FindOrBuildMaterial(Plate_Material), "Plate",0,0,0);
      physiPlate = new G4PVPlacement(rm,
				     G4ThreeVector(0.,center_top_p+caracteristic_length_p,0.)
				     + SourceCapsulePosition,
				     logicPlate,  
				     "Plate",     
				     logicWorld,   
				     false,     
				     0,         
				     true);
      logicPlate->SetVisAttributes(gray_vis);
      if(physiPlate->CheckOverlaps()==true)
	{      
	  G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
	  G4cout<<"ERROR:: The plate is outside of limits"<<G4endl;
	  G4cout<<"        Check the position and (or) dimension"<<"\n"<<G4endl;
	  G4cout<<"        The Object position (axis y) must be positive  "<<"\n"<<G4endl;
	  G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
	  G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
	}
    }
  else
    {
      G4cout<<"\n"<<"\n"<<"***********  WARNING MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"WARNING:: Thickness plate is equal to zero, "<<G4endl;
      G4cout<<"          the plate will don't be placed.   "<<"\n"<<G4endl;
      G4cout<<"***********                    ***********"<<G4endl;
    }
}

void DetectorConstruction::ConstructGe10Detector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(Ge10rotationX);
  Rotation->rotateY(Ge10rotationY);

  Detector_Ge10_log = new Construct_log_vol_detectorGFNUN("Ge10_detector","Ge_10",vis_flag,
							  solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_Ge10_physical;
  Detector_Ge10_physical = new G4PVPlacement(Rotation,               // Rotation
					     Ge10Position, // Position
					     Detector_Ge10_log, // its logic vol
					     "Ge10_detector", //name
					     logicWorld, // its mother vol
					     false,           // no boolean operations 
					     0,
					     true);

  if(Detector_Ge10_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The Ge (10%) detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_Ge10_sensitiveVol_log = Detector_Ge10_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructGe40Detector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(Ge40rotationX);
  Rotation->rotateY(Ge40rotationY);

  Detector_Ge40_log = new Construct_log_vol_detectorGFNUN("Ge40_detector","Ge_40",vis_flag,
							  solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_Ge40_physical;
  Detector_Ge40_physical = new G4PVPlacement(Rotation,               // Rotation
					     Ge40Position, // Position
					     Detector_Ge40_log, // its logic vol
					     "Ge40_detector", //name
					     logicWorld, // its mother vol
					     false,           // no boolean operations 
					     0,
					     true);

  if(Detector_Ge40_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The Ge (40%) detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }

  Detector_Ge40_sensitiveVol_log = Detector_Ge40_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructNaI2x2Detector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(NaI2x2rotationX);
  Rotation->rotateY(NaI2x2rotationY);

  Detector_NaI_2x2_log = new Construct_log_vol_detectorGFNUN("NaI2x2_detector","NaI_2x2",vis_flag,
							     solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_NaI_2x2_physical;
  Detector_NaI_2x2_physical = new G4PVPlacement(Rotation,               // Rotation
						NaI2x2Position,
						//+G4ThreeVector(-Plate_thickness,0.,0.), // Position
						Detector_NaI_2x2_log, // its logic vol
						"NaI2x2_detector", //name
						logicWorld, // its mother vol
						false,           // no boolean operations 
						0,
						true);

  if(Detector_NaI_2x2_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The NaI (crystal of 2x2 in) detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_NaI_2x2_sensitiveVol_log = Detector_NaI_2x2_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructNaI3x3Detector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(NaI3x3rotationX);
  Rotation->rotateY(NaI3x3rotationY);

  Detector_NaI_3x3_log = new Construct_log_vol_detectorGFNUN("NaI3x3_detector","NaI_3x3",vis_flag,
							     solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_NaI_3x3_physical;
  Detector_NaI_3x3_physical = new G4PVPlacement(Rotation,               // Rotation
						NaI3x3Position, // Position
						Detector_NaI_3x3_log, // its logic vol
						"NaI3x3_detector", //name
						logicWorld, // its mother vol
						false,           // no boolean operations 
						0,
						true);

  if(Detector_NaI_3x3_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The NaI (crystal of 3x3 in) detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_NaI_3x3_sensitiveVol_log = Detector_NaI_3x3_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructPlasticDetector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(PlasticrotationX);
  Rotation->rotateY(PlasticrotationY);

  Detector_Plastic_log = new Construct_log_vol_detectorGFNUN("Plastic_detector","Plastic",vis_flag,
							     solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_Plastic_physical;
  Detector_Plastic_physical = new G4PVPlacement(Rotation,               // Rotation
						PlasticPosition, // Position
						Detector_Plastic_log, // its logic vol
						"Plastic_detector", //name
						logicWorld, // its mother vol
						false,           // no boolean operations 
						0,
						true);

  if(Detector_Plastic_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The Plastic detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_Plastic_sensitiveVol_log = Detector_Plastic_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructCsIDetector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(CsIrotationX);
  Rotation->rotateY(CsIrotationY);

  Detector_CsI_log = new Construct_log_vol_detectorGFNUN("CsI_detector","CsI",vis_flag,
							 solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_CsI_physical;
  Detector_CsI_physical = new G4PVPlacement(Rotation,               // Rotation
					    CsIPosition, // Position
					    Detector_CsI_log, // its logic vol
					    "CsI_detector", //name
					    logicWorld, // its mother vol
					    false,           // no boolean operations 
					    0,
					    true);

  if(Detector_CsI_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The CsI detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      //G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_CsI_sensitiveVol_log = Detector_CsI_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::ConstructDummyDetector(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag)
{
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateY(-M_PI/2.);
  Rotation->rotateX(DummyrotationX);
  Rotation->rotateY(DummyrotationY);

  Detector_dummy_log = new Construct_log_vol_detectorGFNUN("Dummy_detector","dummy",vis_flag,
							   solid_flag,Xray_flag);
  G4VPhysicalVolume* Detector_dummy_physical;
  Detector_dummy_physical = new G4PVPlacement(Rotation,               // Rotation
					      DummyPosition, // Position
					      Detector_dummy_log, // its logic vol
					      "Dummy_detector", //name
					      logicWorld, // its mother vol
					      false,           // no boolean operations 
					      0,
					      true);

  if(Detector_dummy_physical->CheckOverlaps()==true)
    {      
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: The dummy detector is overlap"<<G4endl;
      G4cout<<"        Check the detector position"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "G4PVPlacement::CheckOverlaps()", FatalException,"Overlap with other volume !!");
    }
  Detector_dummy_sensitiveVol_log = Detector_dummy_log->GetSensitiveVolume();
  Add2DetectorCounter(1);
}

void DetectorConstruction::Sensibilize()
{
  static SensitiveDetector* sensitive = 0;
  if (!sensitive)
    {
      sensitive = new SensitiveDetector("/Detectors/full");
      //We register now the SD with the manager
      G4SDManager::GetSDMpointer()->AddNewDetector(sensitive);
      detectorRegion   = new G4Region("Detector");
    }

  if(Detector_Ge10_sensitiveVol_log)
    Detector_Ge10_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_Ge40_sensitiveVol_log)
    Detector_Ge40_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_NaI_2x2_sensitiveVol_log)
    Detector_NaI_2x2_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_NaI_3x3_sensitiveVol_log)
    Detector_NaI_3x3_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_Plastic_sensitiveVol_log)
    Detector_Plastic_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_CsI_sensitiveVol_log)
    Detector_CsI_sensitiveVol_log->SetSensitiveDetector(sensitive);
  if(Detector_dummy_sensitiveVol_log)
    Detector_dummy_sensitiveVol_log->SetSensitiveDetector(sensitive);
}

//! from the detector messenger you can force a geometry re-computation
void DetectorConstruction::UpdateGeometry()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RegionStore::GetInstance()->UpdateMaterialList(physiWorld);

}
 
/* pictures to be taken 
   - bottom case with BS detector case.
   + photomultipliers
   + X-ray vision from the inside with bakelites
   ALL of them and please check dimensions and messengers
*/

G4Material* DetectorConstruction::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}
