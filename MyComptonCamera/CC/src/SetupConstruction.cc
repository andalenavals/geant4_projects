/*! @file SetupConstruction.cc
  @brief Defines mandatory user class SetupConstruction.
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  @author Flechas (D. Flechas dcflechasg@unal.edu.co)
  In this source file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/

/* Sensitive detectors */
#include "G4SDManager.hh"
#include "SDBSDcrystal.hh"
#include "SDPSDcrystal.hh"
#include "SDControlVol.hh"

/* no-geant4 classes*/
#include "SetupConstruction.hh"
#include "SetupMessenger.hh"
#include "Materials.hh"

#include "LogVol_CCHardCovering_Lid.hh"
#include "LogVol_CCHardCovering_Base.hh"
#include "LogVol_GammaSourceCasing.hh"
#include "LogVol_PCBs_Cyl.hh"
#include "LogVol_PCBs_Box.hh"
#include "LogVol_Collimator_ToSample.hh"
#include "LogVol_Collimator_ToPSD.hh"
#include "LogVol_ShieldingPSDCone.hh"
#include "LogVol_Crystal_Cyl.hh"
#include "LogVol_DynodesArray.hh"
#include "LogVol_PMTbase.hh"
#include "LogVol_PSD_actual.hh"
#include "LogVol_BSD_actual.hh"
#include "LogVol_PSD_new.hh"
#include "LogVol_BSD_new.hh"


#include "LogVol_ComptonCamera_actual.hh"
#include "LogVol_ComptonCamera_DFproposal.hh"

#include "PhysVol_SegmentedCrystal_Box.hh"

// #include "LogVol_LeadBarrel.hh"
// #include "LogVol_252CfSourceCasing.hh"
// #include "LogVol_CastleShielding.hh"
// #include "LogVol_Detector_NaI3x3.hh"
// #include "LogVol_Detector_NaI2x2.hh"
// #include "LogVol_Detector_CsI2x2.hh"
// #include "LogVol_Detector_CsI1x5.hh"
// #include "LogVol_Detector_BGO2x2.hh"
// #include "LogVol_Detector_Ge10.hh"
// #include "LogVol_Detector_Ge40.hh"
// #include "LogVol_Detector_Plastic.hh"
// #include "LogVol_LabGround.hh"

/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

/* geometric objects */
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

/* geant4 materials */
#include "G4NistManager.hh"
#include "G4Material.hh"
//#include "G4MaterialTable.hh"
//#include "G4Element.hh"
//#include "G4Isotope.hh"

/* solid volumes */
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"

/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

/* optical process in boundaries */
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalSurface.hh"

/*sensitive volumes */
//#include "SensitiveDetector.hh"
//#include "G4SDManager.hh"

/* for UpdateGeometry */
/*#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else*/
#include "G4RunManager.hh"
//#endif
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

/* visualization */
#include "G4VisAttributes.hh"


SetupConstruction::SetupConstruction()
{
  //! \li Create a messenger (defines custom UI commands)
  messenger = new SetupMessenger(this);
  
  /*  MATERIALS  */
  materials = NULL;
  
  /*  Detector Counter */
  DetectorCounter=0;
  //! maximum number of sensitive volumes: One per each detector type
  MaximumNumberOfDetectors = 2;
  /* flags */
  constructed = false;
  /* Plate  */
  // Plate material
  Plate_Material = "G4_Fe";
  // Plate thickness
  Plate_thickness = 5.0*mm;
}

SetupConstruction::~SetupConstruction()
{
  if (materials)         delete materials;
  delete messenger;             
}

/*!
  This function is called by G4 when the detector has to be created\n
  Definitions of Solids, Logical Volumes and Physical Volumes comes here.
  //! \sa ConstructSetup() and Sensibilize()
  */

G4VPhysicalVolume* SetupConstruction::Construct()
{
  /****** Our Materials List *********/
  materials = Materials::GetInstance();
  
  //------------------------------
  // World
  //------------------------------
  G4double halfWorldLength = 3.3*m;

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
  G4bool vis_flag,solid_flag,Xray_flag;
  Sensibilize(); // This function must to be called before to ConstructSetup(): It constructs the sensitive detectors (PSD and BSD)
  
  ConstructSetup_TEST();
  //ConstructSetup();
  
  /** In this routine the logical volumes to score get register with
      the SDmanager **/
  constructed = true;
  
  //! \li Always return the physical World
  return physiWorld;
}

void SetupConstruction::ConstructSetup(void)
{}

void SetupConstruction::ConstructSetup_TEST(void)
{

  ///////////////////////////////////
  ////// Setup:: Test ///////////////
  ///////////////////////////////////
  
  G4RotationMatrix* Rotation 
    = new G4RotationMatrix();
  Rotation->rotateX(M_PI/2.);
  
  LogVol_ComptonCamera_actual* CCdf_log;
  CCdf_log = new LogVol_ComptonCamera_actual("CCdf",false,true);//act. posPSD=5.1916cm
  G4VPhysicalVolume* CCdf_phy;
  CCdf_phy = new G4PVPlacement(Rotation,         // no rotation
			       G4ThreeVector(0,0.0*cm,0), //Completa
			       CCdf_log,    // its logical volume
			       "TestCCdf",      // its name
			       logicWorld,
			       false,     // no boolean operations 
			       0,
			       true);
  
  // test volume
  /*
  const G4int numZPlanestVol=2;
  const G4double  zPlanetVol[]={0.*mm,5.2*cm};
  const G4double  rInnertVol[]={0.0*cm,0.0*cm};
  const G4double  rOutertVol[]={0.6*cm,0.6*cm};
 
  G4Polycone* tVol_sol = new G4Polycone("tVol_sol",0.*rad,2*M_PI*rad,
					numZPlanestVol,
					zPlanetVol,
					rInnertVol,
					rOutertVol);
  
  G4LogicalVolume* tVol_log= new G4LogicalVolume(tVol_sol,G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL"),"tvol_Log",0,0,0);
  
  G4VPhysicalVolume* tVol_phy;
  tVol_phy = new G4PVPlacement(Rotation,         // no rotation
			       G4ThreeVector(0.0*cm,-5.2*cm-1.0*cm,0.0*cm), 
			       tVol_log,    // its logical volume
			       "tVol_phy",      // its name
			       logicWorld,
			       false,     // no boolean operations 
			       0,
			       true);
  
  G4double eps_h = 20.0*cm;
  G4double eps_t = 1.0*cm;
  G4double eps_l = 15.0*cm;
  
  G4Box* eps_sol = new G4Box("eps_Sol",eps_h/2.0,eps_l/2.0,eps_t/2.0);
  G4LogicalVolume* eps1_Log= new G4LogicalVolume(eps_sol,FindMaterial("EPS"),"eps1_Log",0,0,0);
  G4VPhysicalVolume* eps1_phy;
  eps1_phy = new G4PVPlacement(Rotation,         // no rotation
			       G4ThreeVector(0.0*cm,-eps_t/2.0,0.0*cm), 
			       eps1_Log,    // its logical volume
			       "EPS1_phy",      // its name
			       logicWorld,
			       false,     // no boolean operations 
			       0,
			       true);
  */
  
  /*
  G4double plate_h = 1.5*cm;
  G4double plate_t = 2.0*cm;//Plate_thickness;
  G4double plate_l = 1.5*cm;
  
  G4Box* plate_sol = new G4Box("plate_Sol",plate_l/2.0,plate_t/2.0,plate_h/2.0);
  
  G4LogicalVolume* plate_Log= new G4LogicalVolume(plate_sol,G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe"),"Plate_Log",0,0,0);
  
  G4VPhysicalVolume* plate_phy;
  plate_phy = new G4PVPlacement(0,         // no rotation
  				G4ThreeVector(0.0*cm,-plate_t/2.0-2.0*cm,0.0*cm), 
  				plate_Log,    // its logical volume
  				"Plate_phy",      // its name
  				logicWorld,
  				false,     // no boolean operations 
  				0,
  				true);
  */
  /* plate_Log->SetSensitiveDetector(G4SDManager::GetSDMpointer()->
  				  FindSensitiveDetector("/ControlVol/Volume"));
  
  
  LogVol_ComptonCamera_actual* CCa_log;
  CCa_log = new LogVol_ComptonCamera_actual("CCa");
  G4VPhysicalVolume* CCa_phy;
  CCa_phy = new G4PVPlacement(Rotation,         // no rotation
			      G4ThreeVector(0,0.0*cm,0), //Completa
			      CCa_log,    // its logical volume
			      "TestCCa",      // its name
			      logicWorld,
			      false,     // no boolean operations 
			      0,
			      true);
  
  LogVol_BSD_actual* BSD_log;
  BSD_log = new LogVol_BSD_actual("BSD_Log",(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"),true);
  G4VPhysicalVolume* BSD_phy;
  BSD_phy = new G4PVPlacement(Rotation,         // no rotation
			      G4ThreeVector(0,0.0*cm,0), //Completa
			      BSD_log,    // its logical volume
			      "TestBSD",      // its name
			      logicWorld,
			      false,     // no boolean operations 
			      0,
			      true);
  
  LogVol_BSD_new* BSD_log;
  BSD_log = new LogVol_BSD_new("BSD_Log",(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"),true);
  G4VPhysicalVolume* BSD_phy;
  BSD_phy = new G4PVPlacement(Rotation,         // no rotation
			      G4ThreeVector(0,0.0*cm,0), //Completa
			      BSD_log,    // its logical volume
			      "TestBSD",      // its name
			      logicWorld,
			      false,     // no boolean operations 
			      0,
  			      true);
  
  LogVol_PSD_new* PSD_log;
  PSD_log = new LogVol_PSD_new("PSD_Log",(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"),true);
  G4VPhysicalVolume* PSD_phy;
  PSD_phy = new G4PVPlacement(Rotation,         // no rotation
			      G4ThreeVector(0,0.0*cm,0), //Completa
			      PSD_log,    // its logical volume
			      "TestPSD",      // its name
			      logicWorld,
			      false,     // no boolean operations 
			      0,
			      true);
  
  PhysVol_SegmentedCrystal_Box* PSDCrystal_Phy;
  PSDCrystal_Phy = new PhysVol_SegmentedCrystal_Box("PSDCrystal", //name
						    Rotation, // Rotational matrix
						    G4ThreeVector(0,0,0), // position
						    logicWorld, // mother volume
						    10*cm, // length
						    1*cm, // height
						    10, 10, // NpixelX, NpixelY
						    (G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE")); // material
						
  
  LogVol_Crystal_Cyl* cry_log;
  cry_log = new LogVol_Crystal_Cyl("pichu4");//name,Iradius,Oradius,length, material
  G4VPhysicalVolume* physical_cry;
  physical_cry = new G4PVPlacement(Rotation,         // no rotation
				   G4ThreeVector(0,-3.0*cm,0), //Completa
				   cry_log,    // its logical volume
				   "TestVol4",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  
  LogVol_ShieldingPSDCone* shi_log;
  shi_log = new LogVol_ShieldingPSDCone("pichu3");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* physical_shi;
  physical_shi = new G4PVPlacement(Rotation,         // no rotation
				   G4ThreeVector(0,3.56*mm,0), //Completa
				   shi_log,    // its logical volume
				   "TestVol3",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  
  LogVol_Collimator_ToSample* col_log;
  col_log = new LogVol_Collimator_ToSample("pichu");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* physical_col;
  physical_col = new G4PVPlacement(Rotation,         // no rotation
				   G4ThreeVector(0,-0.0*cm,0), //Completa
				   col_log,    // its logical volume
				   "TestVol",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  LogVol_Collimator_ToPSD* col2_log;
  col2_log = new LogVol_Collimator_ToPSD("pichu2");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* physical_col2;
  physical_col2 = new G4PVPlacement(Rotation,         // no rotation
				   G4ThreeVector(0,0.0*cm,0), //Completa
				   col2_log,    // its logical volume
				   "TestVol2",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  
  LogVol_PCBs_Box* pcb_log;
  pcb_log = new LogVol_PCBs_Box("test_log",10.0*cm,5.0*cm,0.3*cm);//name,width,length,thick
  G4VPhysicalVolume* physical_pcb;
  physical_pcb = new G4PVPlacement(Rotation,         // no rotation
				   G4ThreeVector(0,0.0*cm,-20*cm), //Completa
				   pcb_log,    // its logical volume
				   "TestVol",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  
  LogVol_PCBs_Cyl* pcb_log;
  pcb_log = new LogVol_PCBs_Cyl("test_log",6.0*cm,10*cm);//name,radius,length
  G4VPhysicalVolume* physical_pcb;
  physical_pcb = new G4PVPlacement(Rotation,         // no rotation
                                   G4ThreeVector(), //Completa
				   pcb_log,    // its logical volume
				   "TestVol",      // its name
				   logicWorld,
				   false,     // no boolean operations 
				   0,
				   true);
  */
  /*
  LogVol_CCHardCovering_Lid* logic_CCHardCovering_Lid;
  logic_CCHardCovering_Lid = new LogVol_CCHardCovering_Lid("CCHardCovering_Lid_log");
  G4VPhysicalVolume* physical_CCHardCovering_Lid;
  physical_CCHardCovering_Lid = new G4PVPlacement(Rotation,         // no rotation
						  G4ThreeVector(0.0*cm,-2.0*cm,0.0), //Completa
						  logic_CCHardCovering_Lid,    // its logical volume
						  "CCHardCovering_Lid_phy",      // its name
						  logicWorld,
						  false,     // no boolean operations 
						  0,
  						  true);
  
  LogVol_CCHardCovering_Base* logic_CCHardCovering_Base;
  logic_CCHardCovering_Base = new LogVol_CCHardCovering_Base("test_log");
  G4VPhysicalVolume* physical_CCHardCovering_Base;
  physical_CCHardCovering_Base = new G4PVPlacement(Rotation,         // no rotation
						   G4ThreeVector(0.0*cm,-2.3*cm,0.0), //Completa
						   logic_CCHardCovering_Base,    // its logical volume
						   "CCHardCovering_Base_phy",      // its name
						   logicWorld,
						   false,     // no boolean operations 
						   0,
						   true);
  
  /// **** Source casing **** ///
  LogVol_GammaSourceCasing* logic_GammaSourceCasing;
  logic_GammaSourceCasing = new LogVol_GammaSourceCasing("GSourceCasing_log",0.63*cm,2.0*mm);//(name,radius,height)
  G4VPhysicalVolume* physical_Acrilyc_Capsule;
  physical_Acrilyc_Capsule = new G4PVPlacement(Rotation,         // no rotation
  					       G4ThreeVector(0.0*cm,-0.1*cm,0.0), //Completa
  					       logic_GammaSourceCasing,    // its logical volume
  					       "GSourceCasing_phy",      // its name
  					       logicWorld,
  					       false,     // no boolean operations 
  					       0,
  					       true);
  */
}


void SetupConstruction::Sensibilize()
{
  /// *** BSD sensitive detector *** ///
  static SDBSDcrystal* sensitiveBSDcrystal = 0;
  if (!sensitiveBSDcrystal)
    {
      sensitiveBSDcrystal = new SDBSDcrystal("/Detectors/BSDcrystal");
      //CAUTION:: For sensitiveBSDrystal, its name (/Detectors/BSDcrystals) is used to find this detector type by other classes
      //We register now the SD with the manager
      G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveBSDcrystal);
      BSDdetectorRegion   = new G4Region("BSDcrystalR");
    }
  /// *** PSD sensitive detector *** ///
  static SDPSDcrystal* sensitivePSDcrystal = 0;
  if (!sensitivePSDcrystal)
    {
      sensitivePSDcrystal = new SDPSDcrystal("/Detectors/PSDcrystal");
      //CAUTION:: For sensitivePSDrystal, its name (/Detectors/PSDcrystals) is used to find this detector type by other classes
      //We register now the SD with the manager
      G4SDManager::GetSDMpointer()->AddNewDetector(sensitivePSDcrystal);
      PSDdetectorRegion   = new G4Region("PSDcrystalR");
    }
  /// *** Control volume *** ///
  static SDControlVol* sensitiveControlVol = 0;
  if (!sensitiveControlVol)
    {
      sensitiveControlVol = new SDControlVol("/ControlVol/Volume");
      //CAUTION:: For sensitiveControlVol, its name (/ControlVol/Volume) is used to find this detector type by other classes
      //We register now the SD with the manager
      G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveControlVol);
      ControlVolRegion   = new G4Region("ControlVolumeR");
    }
}

//! from the detector messenger you can force a geometry re-computation
void SetupConstruction::UpdateGeometry()
{
  if(physiWorld) 
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

G4Material* SetupConstruction::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}
