/*! @file LogVol_BSD_new.hh
  @brief Defines mandatory user class LogVol_BSD_new.
  @date May, 2016
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "Materials.hh"

#include "LogVol_BSD_new.hh"
#include "LogVol_PCBs_Cyl.hh"
#include "LogVol_PCBs_Box.hh"
#include "LogVol_DynodesArray.hh"
#include "LogVol_PMTbase.hh"
#include "LogVol_SegmentedCrystal_Box.hh"
#include "LogVol_Crystal_Cyl.hh"

#include "PhysVol_SegmentedCrystal_Box.hh"
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
/* visualization */
#include "G4VisAttributes.hh"
/* Sensitive detectors */
#include "G4SDManager.hh"
#include "G4RunManager.hh"

LogVol_BSD_new:: LogVol_BSD_new(G4String fname,
				G4Material* fMaterial,
				G4bool det_flag,G4bool fflag):
  G4LogicalVolume(new G4Tubs(fname+"_Sol",0,80./2*mm,60.*mm/2.,0.,2*pi),fMaterial,fname,0,0,0)
{ 
  /* set variables */
  SetName(fname);
  SetOpflag(fflag);
  SetDetectorFlag(det_flag);
  Material=fMaterial;
  materials = NULL;
  /* Dimensions */
  Case_height  = 20.0*mm;
  Case_Iradius = 56.0/2.0*mm;
  Case_Oradius = 216.0/2.0*mm;
  Case_thick   = 2.0*mm;
  origin_z     = 0.*cm;
  crystal_Iradius = 28.05*mm;
  crystal_Oradius = 97.6*mm;
  crystal_height = 15.5*mm;
  distanceToCrystal = 0.0*mm;
  Al_Wrapper_thickness = 0.016*mm;
  /* Construct solid volume */
  ConstructLogVol_BSD_new();
  /* Define sensitive volumes*/
  if(detector_flag)
    SensitiveVolume_BSD_new();
  /* Visualization */
  this->SetVisAttributes(G4VisAttributes::GetInvisible());
}

LogVol_BSD_new::~LogVol_BSD_new()
{;}

void LogVol_BSD_new::ConstructLogVol_BSD_new(void)
{
  /// Visualization
  G4VisAttributes* red_vis 
    = new G4VisAttributes(true,G4Colour::Red());
  red_vis->SetForceWireframe(false);
  red_vis->SetForceSolid(false);
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(true,G4Colour(0.5,0.5,0.5));
  gray_vis->SetForceWireframe(false);
  gray_vis->SetForceSolid(false);
  G4VisAttributes* copper_vis 
    = new G4VisAttributes(true,G4Colour(0.8,0.3,0.2));
  copper_vis->SetForceWireframe(false);
  copper_vis->SetForceSolid(false);
  G4VisAttributes* black_vis 
    = new G4VisAttributes(true,G4Colour(0.1,0.1,0.1));
  black_vis->SetForceWireframe(false);
  black_vis->SetForceSolid(false);
    
  /****** Our Materials List *********/
  materials = Materials::GetInstance();

  ///////////////////////////////////////////////////////
  ////***** Space where BSD will be constructed *****////
  ///////////////////////////////////////////////////////
  const G4int     numZPlanesSpaceCrystal=4;
  G4double  zPlaneSpaceCrystal[]={origin_z,Case_height,Case_height,Case_height+Case_thick};
  G4double  rInnerSpaceCrystal[]={Case_Iradius,Case_Iradius,Case_Iradius,Case_Iradius};
  G4double  rOuterSpaceCrystal[]={ Case_Oradius, Case_Oradius, Case_Oradius, Case_Oradius};

  G4Polycone* solidSpaceCrystal
    = new G4Polycone(Name+"space_Cyl_sol",0.*rad,2*M_PI*rad,numZPlanesSpaceCrystal,zPlaneSpaceCrystal,rInnerSpaceCrystal,rOuterSpaceCrystal);

  G4double PMT_diameter=22.5*mm;
  G4double PMT_height=80.0*mm;
  const G4int     numZPlanesPMT=2;
  G4double  zPlanePMT[]={origin_z,PMT_height};
  G4double  rInnerPMT[]={0.0*cm,0.0*cm};
  G4double  rOuterPMT[]={PMT_diameter/2.0,PMT_diameter/2.0};

  G4Polycone* solidPMT
    = new G4Polycone(Name+"_PMT_sol",0.*rad,2*M_PI*rad,numZPlanesPMT,zPlanePMT,rInnerPMT,rOuterPMT);

  G4double distance_toPMT =155.2/2*mm;
  G4UnionSolid* solidSpaceCrys1PMT = new G4UnionSolid(Name+"_SpaceCrystal1PMT_Solid",
						      solidSpaceCrystal,solidPMT, 0,
						      G4ThreeVector(distance_toPMT,0.,Case_height));
  // trick //
  BSD_new_solid = new G4UnionSolid(Name+"_Solid", solidSpaceCrys1PMT, solidPMT,0,
				      G4ThreeVector(-distance_toPMT,0.,Case_height));
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_BSD_new();
  /////////////////////////////////////////////////////////////
  ////***** Cover of BSD Crystal (polycone with holes)*****////
  /////////////////////////////////////////////////////////////
  const G4int     numZPlanesCrystalCover=4;
  G4double  zPlaneCrystalCover[]={origin_z,Case_height,Case_height,Case_height+Case_thick};
  G4double  rInnerCrystalCover[]={Case_Oradius-Case_thick,Case_Oradius-Case_thick,
				  Case_Iradius,Case_Iradius};
  G4double  rOuterCrystalCover[]={ Case_Oradius, Case_Oradius, Case_Oradius, Case_Oradius};

  G4Polycone* solidCrystalCover_withoutholes
    = new G4Polycone(Name+"_CrystalCoverwithoutholes_sol",0.*rad,2*M_PI*rad,
		     numZPlanesCrystalCover,zPlaneCrystalCover,rInnerCrystalCover,rOuterCrystalCover);
  G4SubtractionSolid*  solidCrystalCoverHole1
    = new G4SubtractionSolid(Name+"_CrystalCoverHole1_sol",
			     solidCrystalCover_withoutholes, solidPMT, 0,
			     G4ThreeVector(distance_toPMT,0.0*cm,0.0*cm));
  G4SubtractionSolid*  solidCrystalCover
    = new G4SubtractionSolid(Name+"_CrystalCover_sol",
			     solidCrystalCoverHole1, solidPMT,0,
			     G4ThreeVector(-distance_toPMT,0.0*cm,0.0*cm));
  
  G4LogicalVolume* logicCrystalCover
    = new G4LogicalVolume(solidCrystalCover,
			  (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),
			  Name+"_CrystalCover_Log",0,0,0);
  logicCrystalCover->SetVisAttributes(gray_vis);
  G4VPhysicalVolume* physiCrystalCover;
  physiCrystalCover= new G4PVPlacement(0,                                  // Rotation
				       G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), // Position
				       logicCrystalCover,                   // its logical volume
				       Name+"_CrystalCover_Phy",                 // its name	      
				       this,                  // its mother  volume
				       false,                              // no boolean operations 
				       0,
				       true);
  //////////////////////////////////////
  //// ********* Crystal ********* /////
  //////////////////////////////////////
  LogVol_Crystal_Cyl* Crystal_Log;
  Crystal_Log = new LogVol_Crystal_Cyl(Name+"_Crystal_Log",crystal_Iradius,crystal_Oradius,crystal_height,Material);//name,Iradius,Oradius,length, material
  G4VPhysicalVolume* Crystal_Phy;
  Crystal_Phy = new G4PVPlacement(0,         // no rotation
				  G4ThreeVector(0.0*cm,0.0*cm,
						distanceToCrystal+crystal_height
						+Al_Wrapper_thickness),
				  Crystal_Log,    // its logical volume
				  Name+"_Crystal_Phy",      // its name
				  this,
				  false,     // no boolean operations 
				  0,
				  true);
  ///*** To define sensitive volume ***///
  BSD_SensitiveVolume = Crystal_Log;
  ///////////////////////////////////////////////
  ///////////**** Photocathodes ****/////////////
  ///////////////////////////////////////////////
  G4double PhCradius = 7.5*mm;
  G4double PhCheight = 20.*nm;
  LogVol_Crystal_Cyl* PhotoCathode1_Log = new LogVol_Crystal_Cyl(Name+"_PhotoC1_Log",0.0*cm,PhCradius,PhCheight,FindMaterial("BIALKALI"));//name,Iradius,Oradius,length, material
  LogVol_Crystal_Cyl* PhotoCathode2_Log = new LogVol_Crystal_Cyl(Name+"_PhotoC1_Log",0.0*cm,PhCradius,PhCheight,FindMaterial("BIALKALI"));//name,Iradius,Oradius,length, material
  
  G4VPhysicalVolume* PhotoCathode1_Phy;
  PhotoCathode1_Phy = new G4PVPlacement(0,         // no rotation
					G4ThreeVector(distance_toPMT,0.0*cm,
						      distanceToCrystal+crystal_height
						      +Al_Wrapper_thickness+PhCheight),
					PhotoCathode1_Log,    // its logical volume
					Name+"_PhotoCathode1_Phy",      // its name
					this,
					false,     // no boolean operations 
					0,
					true);
  G4VPhysicalVolume* PhotoCathode2_Phy;
  PhotoCathode2_Phy = new G4PVPlacement(0,         // no rotation
					G4ThreeVector(-distance_toPMT,0.0*cm,
						      distanceToCrystal+crystal_height
						      +Al_Wrapper_thickness+PhCheight),
					PhotoCathode2_Log,    // its logical volume
					Name+"_PhotoCathode2_Phy",      // its name
					this,
					false,     // no boolean operations 
					0,
					true);
  
  PhotoCathode1_Log->SetVisAttributes(red_vis);
  PhotoCathode2_Log->SetVisAttributes(red_vis);

  ///*** To define sensitive volume ***///
  BSD_SensitiveVolume_PC1 = PhotoCathode1_Log;
  BSD_SensitiveVolume_PC2 = PhotoCathode2_Log;
  
  /////////////////////////////
  // /! Aluminium wrapper !/ //
  /////////////////////////////
  const G4int numZPlanes_Al_Wrapper = 6;
  G4double  zPlane_Al_Wrapper[numZPlanes_Al_Wrapper] = {origin_z,
							Al_Wrapper_thickness,
							Al_Wrapper_thickness,
							Al_Wrapper_thickness+crystal_height,
							Al_Wrapper_thickness+crystal_height,
							2.*Al_Wrapper_thickness+crystal_height};
  G4double  rInner_Al_Wrapper[numZPlanes_Al_Wrapper] = {crystal_Iradius,
							crystal_Iradius,
							crystal_Oradius,
							crystal_Oradius,
							crystal_Iradius,
							crystal_Iradius};
  G4double  rOuter_Al_Wrapper[numZPlanes_Al_Wrapper] = {crystal_Oradius+Al_Wrapper_thickness,
							crystal_Oradius+Al_Wrapper_thickness,
							crystal_Oradius+Al_Wrapper_thickness,
							crystal_Oradius+Al_Wrapper_thickness,
							crystal_Oradius+Al_Wrapper_thickness,
							crystal_Oradius+Al_Wrapper_thickness};
      
  G4Polycone* Al_Wrapper_Outer_solid = new G4Polycone (Name+"_Al_wrapper_Outer_Sol",0.*rad,2*M_PI*rad,
						       numZPlanes_Al_Wrapper,zPlane_Al_Wrapper,
						       rInner_Al_Wrapper,rOuter_Al_Wrapper);

  G4SubtractionSolid*  Al_Wrapper_H1_solid = new G4SubtractionSolid(Name+"_Al_wrapperHole1_Sol",
								    Al_Wrapper_Outer_solid,
								    solidPMT, 0,
								    G4ThreeVector(distance_toPMT,
										  0.0*cm,
										  crystal_height/2.0));
  G4SubtractionSolid*  Al_Wrapper_H2_solid = new G4SubtractionSolid(Name+"_Al_wrapperHole2_Sol",
								    Al_Wrapper_H1_solid,
								    solidPMT, 0,
								    G4ThreeVector(-distance_toPMT,
										  0.0*cm,
										  crystal_height/2.0));

  const G4int numZPlanes_Al_Wrapper_t = 2;
  G4double  zPlane_Al_Wrapper_t[numZPlanes_Al_Wrapper_t] = {origin_z,
							    2.*Al_Wrapper_thickness
							    +crystal_height};
  G4double  rInner_Al_Wrapper_t[numZPlanes_Al_Wrapper_t] = {crystal_Iradius
							    -Al_Wrapper_thickness,
							    crystal_Iradius
							    -Al_Wrapper_thickness};
  G4double  rOuter_Al_Wrapper_t[numZPlanes_Al_Wrapper_t] = {crystal_Iradius,crystal_Iradius};
 
  G4Polycone* Al_Wrapper_Inner_solid = new G4Polycone (Name+"_Al_wrapper_Inner_Sol",0.*rad,2*M_PI*rad,
						       numZPlanes_Al_Wrapper_t,zPlane_Al_Wrapper_t,
						       rInner_Al_Wrapper_t,rOuter_Al_Wrapper_t);
 
 
  G4LogicalVolume* Al_Wrapper_logic = new G4LogicalVolume(Al_Wrapper_H2_solid,
							  (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),Name+"_Al_Wrapper_Log",0,0,0);
  G4LogicalVolume* Al_Wrapper_inner_logic = new G4LogicalVolume(Al_Wrapper_Inner_solid,
								(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),Name+"_Al_Wrapper_Inner_Log",0,0,0);

  //Al_Wrapper_logic->SetVisAttributes(gray_vis);
  //Al_Wrapper_inner_logic->SetVisAttributes(gray_vis);
  Al_Wrapper_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  Al_Wrapper_inner_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  /*    G4OpticalSurface* Al_Wrapper_Surface = new G4OpticalSurface("Al_Wrapper_Surface",
	glisur, // model 
	polished, // surface finish 
	dielectric_metal,// surface-type
	1.);  //surface polish (Sp <=1)
								  
	G4MaterialPropertiesTable* Al_Wrapper_SurfaceProperty = new G4MaterialPropertiesTable();
	  
	G4double Al_Wrapper_Reflectivity=1.0;
	G4double p_Al_Wrapper[2] = {1.50*eV, 5.47*eV};
	G4double refl_Al_Wrapper[2] = {Al_Wrapper_Reflectivity,Al_Wrapper_Reflectivity};
	G4double effi_Al_Wrapper[2] = {0., 0.};
	  
	Al_Wrapper_SurfaceProperty->AddProperty("REFLECTIVITY",p_Al_Wrapper,refl_Al_Wrapper,2);
	Al_Wrapper_SurfaceProperty->AddProperty("EFFICIENCY",p_Al_Wrapper,effi_Al_Wrapper,2);
	  
	Al_Wrapper_Surface -> SetMaterialPropertiesTable(Al_Wrapper_SurfaceProperty);
	new G4LogicalSkinSurface("Al_Wrapper_Surface",Al_Wrapper_logic,Al_Wrapper_Surface);
  */
   
  G4VPhysicalVolume* physiAl_Wrapper;
  physiAl_Wrapper = new G4PVPlacement(0,                                  // Rotation
				      G4ThreeVector(0.,0.,distanceToCrystal), // Position
				      Al_Wrapper_logic,                // its logical volume
				      Name+"_Al_Wrapper_Phy",              // its name	      
				      this,    // its mother  volume
				      false,                           // no boolean operations 
				      0,
				      true);
  G4VPhysicalVolume* physiAl_Wrapper_inner;
  physiAl_Wrapper_inner = new G4PVPlacement(0,                                  // Rotation
				      G4ThreeVector(0.,0.,distanceToCrystal), // Position
				      Al_Wrapper_inner_logic,                // its logical volume
				      Name+"_Al_Wrapper_Inner_Phy",              // its name	      
				      this,    // its mother  volume
				      false,                           // no boolean operations 
				      0,
				      true);
  
  //////////////////////////////////////
  ///////////**** PMTs ****/////////////
  //////////////////////////////////////
  G4double PMT_thick=0.5*mm;
  G4double PMTbase_height=30.0*mm;

  const G4int numZPlanes_Cyl_PMTcase = 2;
  G4double zPlane_Cyl_PMTcase[numZPlanes_Cyl_PMTcase] = {origin_z,PMT_height-PMTbase_height};
  G4double rInner_Cyl_PMTcase[numZPlanes_Cyl_PMTcase] = {PMT_diameter/2.0-PMT_thick,
							 PMT_diameter/2.0-PMT_thick};
  G4double rOuter_Cyl_PMTcase[numZPlanes_Cyl_PMTcase] = {PMT_diameter/2.0,PMT_diameter/2.0};
  G4Polycone* Cyl_PMTcase = new G4Polycone(Name+"_PMTcase_Sol",0.*rad,2*M_PI*rad,
					   numZPlanes_Cyl_PMTcase,
					   zPlane_Cyl_PMTcase,
					   rInner_Cyl_PMTcase,
					   rOuter_Cyl_PMTcase);
  G4LogicalVolume* PMTcase1_Log
    = new G4LogicalVolume (Cyl_PMTcase,
			   (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),
			   Name+"_PMTcase1_Log",0,0,0);
  PMTcase1_Log->SetVisAttributes(gray_vis);

  G4VPhysicalVolume* PMTcase1_Phys;
  PMTcase1_Phys = new G4PVPlacement(0,               // Rotacion
				    G4ThreeVector(distance_toPMT,0.,Case_height),
				    PMTcase1_Log,   // its logical volume
				    Name+"_PMTCase1_Phys",     // its name	      
				    this,   // its mother  volume
				    false,     // no boolean operations 
				    0,
				    true);
  G4LogicalVolume* PMTcase2_Log
    = new G4LogicalVolume (Cyl_PMTcase,
			   (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),
			   Name+"_PMTcase2_Log",0,0,0);
  PMTcase2_Log->SetVisAttributes(gray_vis);

  G4VPhysicalVolume* PMTcase2_Phys;
  PMTcase2_Phys = new G4PVPlacement(0,               // Rotacion
				    G4ThreeVector(-distance_toPMT,0.,Case_height),
				    PMTcase2_Log,   // its logical volume
				    Name+"_PMTCase2_Phys",     // its name	      
				    this,   // its mother  volume
				    false,     // no boolean operations 
				    0,
				    true);
  
  ///*** Dynodes Array ***///
  LogVol_DynodesArray* DynArray1_Log;
  DynArray1_Log = new LogVol_DynodesArray(Name+"_DynArray1_Log",PMT_diameter/2.0-PMT_thick,1.5*cm,7);
  G4VPhysicalVolume* DynArray1_Phys;
  DynArray1_Phys = new G4PVPlacement(0,               // Rotacion
				     G4ThreeVector(distance_toPMT,0.,Case_height+0.75*cm),
				     DynArray1_Log,   // its logical volume
				     Name+"_DynArray1_Phys",     // its name	      
				     this,   // its mother  volume
				     false,     // no boolean operations 
				     0,
				     true);
  LogVol_DynodesArray* DynArray2_Log;
  DynArray2_Log = new LogVol_DynodesArray(Name+"_DynArray2_Log",PMT_diameter/2.0-PMT_thick,1.5*cm,7);
  G4VPhysicalVolume* DynArray2_Phys;
  DynArray2_Phys = new G4PVPlacement(0,               // Rotacion
				     G4ThreeVector(-distance_toPMT,0.,Case_height+0.75*cm),
				     DynArray2_Log,   // its logical volume
				     Name+"_DynArray2_Phys",     // its name	      
				     this,   // its mother  volume
				     false,     // no boolean operations 
				     0,
				     true);
  ///**** PMT bases *****///
  LogVol_PMTbase* PMTbase1_Log;
  PMTbase1_Log = new LogVol_PMTbase(Name+"_PMTbase1_log",PMT_diameter/2.0,PMTbase_height);
  G4VPhysicalVolume* PMTbase1_Phy;
  PMTbase1_Phy = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(distance_toPMT,0.,
						 Case_height+PMT_height-PMTbase_height), 
				   PMTbase1_Log,    // its logical volume
				   Name+"_PMTbase1_Phy",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
  LogVol_PMTbase* PMTbase2_Log;
  PMTbase2_Log = new LogVol_PMTbase(Name+"_PMTbase2_log",PMT_diameter/2.0,PMTbase_height);
  G4VPhysicalVolume* PMTbase2_Phy;
  PMTbase2_Phy = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(-distance_toPMT,0.,
						 Case_height+PMT_height-PMTbase_height), 
				   PMTbase2_Log,    // its logical volume
				   Name+"_PMTbase2_Phy",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
  ///***** Sensitive volume depends of Opflag status (with or without optical photons) *****///
  //if(Opflag)
  //  Construct_Crystal_OpTrue();
  //else
  //  Construct_Crystal_OpFalse();
}
void LogVol_BSD_new::SetSolidVol_BSD_new(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(BSD_new_solid)
    this->SetSolid(BSD_new_solid);
}
G4Material* LogVol_BSD_new::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}

void LogVol_BSD_new::SensitiveVolume_BSD_new()
{
  
  if ( !BSD_SensitiveVolume) return;
  // Sensitive Volumes
  if(!Opflag)
    {  // Crystal
      G4cout << "LogVol_Detector_CsI2x2::SensitiveVolume_Detector:::"<<G4endl;
      G4cout << "Including Detector:"<<Name<< G4endl;
      //The name of sensitive detectors (general) were assigned in SetupConstruction::Sensibilize()
      if(G4SDManager::GetSDMpointer()->FindSensitiveDetector("/Detectors/BSDcrystal"))
	{
	  BSD_SensitiveVolume->SetSensitiveDetector(G4SDManager::GetSDMpointer()->
					      FindSensitiveDetector("/Detectors/BSDcrystal"));
	  G4cout << "Detector Successfully created: "<<Name<< G4endl;
	}
      else
	{ G4cout << "ERROR:: Detector was not created: "<<Name<< G4endl;}
    }
  else
    {
      /*
      // Photocathode  
      if (!fPMT_SD) {
	G4cout << "Construction /Det/SDPMT/"<<Name << G4endl;
	fPMT_SD = new SDPMT("/Det/SDPMT"+Name);
	SDman->AddNewDetector(fPMT_SD);
	G4cout << "Sensitive Detector Successfully created: "<<fPMT_SD->GetName()<< G4endl;
	//pmt_SD->InitPMTs(0); 
	//pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());
      }
      logvolPhotocathode->SetSensitiveDetector(fPMT_SD);
     */
    }
  
}

void LogVol_BSD_new::Construct_Crystal_OpFalse(void)
{ }
void LogVol_BSD_new::Construct_Crystal_OpTrue(void)
{ }

