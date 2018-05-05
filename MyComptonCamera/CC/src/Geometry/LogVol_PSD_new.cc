/*! @file LogVol_PSD_new.hh
  @brief Defines mandatory user class LogVol_PSD_new.
  @date May, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "Materials.hh"

#include "LogVol_PSD_new.hh"
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

LogVol_PSD_new:: LogVol_PSD_new(G4String fname,
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
  Cyl_height  = 30.0*mm;
  Cyl_Oradius = 80.22*mm/2.0;
  Cyl_Iradius = 74.22*mm/2.0;
  Box_height  = 59.26*mm;
  Box_length  = 88.24*mm;
  Box_thick   = 1.2*mm;
  origin_z    = 0.*cm;
  crystal_diameter = 66.48*mm;
  crystal_height = 7.0*mm;
  npixels = 64;
  distanceToCrystal = 2.0*mm;
  /* Construct solid volume */
  ConstructLogVol_PSD_new();
  /* Define sensitive volumes*/
  if(detector_flag)
    SensitiveVolume_PSD_new();
  /* Visualization */
  this->SetVisAttributes(G4VisAttributes::GetInvisible());
 }

LogVol_PSD_new::~LogVol_PSD_new()
{;}

void LogVol_PSD_new::ConstructLogVol_PSD_new(void)
{
  /// Visualization
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

  ////***** Space where PSD will be constructed *****//// 

  G4Box* Box_Outer = new G4Box(Name+"_box_Sol",Box_length/2.0,Box_length/2.0,Box_height/2.0);
  
  const G4int numZPlanes_Cyl = 2;
  G4double zPlane_Cyl[numZPlanes_Cyl] = {origin_z,Cyl_height};
  G4double rInner_Cyl[numZPlanes_Cyl] = {0.0*cm,0.0*cm};
  G4double rOuter_Cyl[numZPlanes_Cyl] = {Cyl_Oradius,Cyl_Oradius};
  
  G4Polycone* Cyl_Outer = new G4Polycone(Name+"_cylOut_Sol",0.*rad,2*M_PI*rad,numZPlanes_Cyl,
					 zPlane_Cyl,rInner_Cyl,rOuter_Cyl);

  // trick //
  PSD_new_solid = new G4UnionSolid(Name+"_Solid", Cyl_Outer, Box_Outer,0,
				      G4ThreeVector(0.,0.,Cyl_height+Box_height/2.0));
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_PSD_new();
  
  ////**** Case for the electronics of the PSD ****/////
  G4Box* Box_Inner = new G4Box(Name+"_box_Sol",
			       Box_length/2.0-Box_thick,
			       Box_length/2.0-Box_thick,(Box_height-Box_thick)/2.0);
  G4SubtractionSolid* ElectronicCase_Sol
    = new G4SubtractionSolid(Name+"_ElectronicCase_Sol",Box_Outer,Box_Inner,0,
			     G4ThreeVector(0.,0.,-Box_thick)/2.0);
  G4LogicalVolume* ElectronicCase_Log
    = new G4LogicalVolume (ElectronicCase_Sol,
			   (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),
			   Name+"_ElectronicCase_Log",0,0,0);
  ElectronicCase_Log->SetVisAttributes(gray_vis);

  G4VPhysicalVolume* ElectronicCase_Phys;
  ElectronicCase_Phys = new G4PVPlacement(0,               // Rotacion
					  G4ThreeVector(0.,0.,Cyl_height+(Box_height/2.0)),
					  ElectronicCase_Log,   // its logical volume
					  Name+"_ElectronicCase_Log",     // its name	      
					  this,   // its mother  volume
					  false,     // no boolean operations 
					  0,
					  true);
  ////**** Electronics: PCBs ****////
  G4double pcbthick =0.3*cm;
  LogVol_PCBs_Box* pcb_log;
  pcb_log = new LogVol_PCBs_Box(Name+"_pcd_log",
				4.0/5.0*(Box_length-2.0*Box_thick),
				4.0/5.0*(Box_height-Box_thick),pcbthick);//name,width,length,thick
  G4VPhysicalVolume* physical_pcb;
  physical_pcb = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(0.,0.,Cyl_height+pcbthick/2.0),
				   pcb_log,    // its logical volume
				   Name+"_pcd_phy",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
  ////**** Case for the crystal and dynodes array ****/////
  const G4int numZPlanes_Cyl_PSDcase = 2;
  G4double zPlane_Cyl_PSDcase[numZPlanes_Cyl] = {origin_z,Cyl_height};
  G4double rInner_Cyl_PSDcase[numZPlanes_Cyl] = {Cyl_Iradius,Cyl_Iradius};
  G4double rOuter_Cyl_PSDcase[numZPlanes_Cyl] = {Cyl_Oradius,Cyl_Oradius};
  G4Polycone* Cyl_PSDcase = new G4Polycone(Name+"_cylOut_Sol",0.*rad,2*M_PI*rad,
					   numZPlanes_Cyl_PSDcase,
					   zPlane_Cyl_PSDcase,
					   rInner_Cyl_PSDcase,
					   rOuter_Cyl_PSDcase);
  G4LogicalVolume* PSDCase_Log
    = new G4LogicalVolume (Cyl_PSDcase,
			   (G4NistManager::Instance())->FindOrBuildMaterial("G4_PLEXIGLASS"),
			   Name+"_PSDCase_Log",0,0,0);
  PSDCase_Log->SetVisAttributes(black_vis);

  G4VPhysicalVolume* PSDCase_Phys;
  PSDCase_Phys = new G4PVPlacement(0,               // Rotacion
				   G4ThreeVector(0.,0.,0.),
				   PSDCase_Log,   // its logical volume
				   Name+"_PSDCase_Phys",     // its name	      
				   this,   // its mother  volume
				   false,     // no boolean operations 
				   0,
				   true);
  ///*** Dynodes Array ***///
  LogVol_DynodesArray* DynArray_Log;
  DynArray_Log = new LogVol_DynodesArray(Name+"_DynArray_Log",Cyl_Iradius,1.5*cm,7);
  G4VPhysicalVolume* DynArray_Phys;
  DynArray_Phys = new G4PVPlacement(0,               // Rotacion
				    G4ThreeVector(0.,0.,distanceToCrystal+crystal_height+0.8*cm),
				    DynArray_Log,   // its logical volume
				    Name+"_DynArray_Phys",     // its name	      
				    this,   // its mother  volume
				    false,     // no boolean operations 
				    0,
				    true);
  ///***** Crystal: Depends of Opflag status (with or without optical photons) *****///
  if(Opflag)
    Construct_Crystal_OpTrue();
  else
    Construct_Crystal_OpFalse();
}

void LogVol_PSD_new::SetSolidVol_PSD_new(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(PSD_new_solid)
    this->SetSolid(PSD_new_solid);
}


void LogVol_PSD_new::Construct_Crystal_OpFalse(void)
{
  ///*** PSD Crystal: Version BOX-> Segmented crystal (npixelsXnpixel pixels) ***///
  /*
  /// Alternative way ///
  PhysVol_SegmentedCrystal_Box* PSDCrystal_Phy;
  PSDCrystal_Phy = new PhysVol_SegmentedCrystal_Box(Name+"_Crystal2", //name
						    0, // Rotational matrix
						    G4ThreeVector(0,0,crystal_height/2.0+distanceToCrystal), // position
						    this, // mother volume
						    crystal_diameter* std::sin(pi/4.), // length
						    crystal_height, // height
						    npixels, npixels, // NpixelX, NpixelY
						    Material); // Crystal material
  */
  LogVol_SegmentedCrystal_Box* PSDCrystal_Log;
  PSDCrystal_Log = new LogVol_SegmentedCrystal_Box(Name+"_Crystal",
						   crystal_diameter* std::sin(pi/4.), // length
						   crystal_height, // height
						   npixels, npixels, // NpixelX, NpixelY
						   Material); // Crystal material
  G4VPhysicalVolume* PSDCrystal_Phys;
  PSDCrystal_Phys = new G4PVPlacement(0,               // Rotacion
				      G4ThreeVector(0.,0.,crystal_height/2.0+distanceToCrystal),
				      PSDCrystal_Log,   // its logical volume
				      Name+"_Crystal_Phys",     // its name	      
				      this,   // its mother  volume
				      false,     // no boolean operations 
				      0,
				      true);
  ///*** To define sensitive volume ***///
  PSD_SensitiveVolume = PSDCrystal_Log->GetSegmentedCrystal_Box();
}

void LogVol_PSD_new::Construct_Crystal_OpTrue(void)
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
  
  G4double Al_Wrapper_thickness = 0.016*mm; // thickness of aluminum wrapper
  G4double photocathode_height = 20.0*nm;
  //// ********** Crystal ********** ////

  LogVol_Crystal_Cyl* PSDCrystal_Log;
  PSDCrystal_Log = new LogVol_Crystal_Cyl(Name+"_Crystal",
					  0.0*cm,
					  crystal_diameter/2.0,
					  crystal_height,
					  Material);//name,Iradius,Oradius,length, material
  G4VPhysicalVolume* PSDCrystal_Phys;
  PSDCrystal_Phys = new G4PVPlacement(0,         // no rotation
				      G4ThreeVector(0.,0.,distanceToCrystal+crystal_height),
				      PSDCrystal_Log,    // its logical volume
				      Name+"_Crystal_Phys",      // its name
				      this,
				      false,     // no boolean operations 
				      0,
				      true);
  ////**** Photocathodes array *****/////
  LogVol_SegmentedCrystal_Box* PSDphotocathode_Log;
  PSDphotocathode_Log = new LogVol_SegmentedCrystal_Box(Name+"_Photocathode",
							crystal_diameter*std::sin(pi/4.),//length
							photocathode_height, // height
							npixels, npixels, // NpixelX, NpixelY
							FindMaterial("BIALKALI")); // Crystal material
  G4VPhysicalVolume* PSDphotocathode_Phys;
  PSDphotocathode_Phys = new G4PVPlacement(0,               // Rotacion
					   G4ThreeVector(0.,0.,distanceToCrystal+crystal_height+photocathode_height/2.0),
					   PSDphotocathode_Log,   // its logical volume
					   Name+"_Photocathode_Phys",     // its name	      
					   this,   // its mother  volume
					   false,     // no boolean operations 
					   0,
					   true);
  
  ///*** To define sensitive volume ***///
  PSDphotocathode_Log->GetSegmentedCrystal_Box()->SetVisAttributes(red_vis);
  PSD_SensitiveVolume = PSDphotocathode_Log->GetSegmentedCrystal_Box();
  
  /// ************************************
  // /! Aluminium wrapper and photocathode
  // /! Aluminium wrapper
  const G4int numZPlanes_Al_Wrapper = 4;
  const G4double  zPlane_Al_Wrapper[numZPlanes_Al_Wrapper] = {origin_z,
							      Al_Wrapper_thickness,
							      Al_Wrapper_thickness,
							      2*Al_Wrapper_thickness+crystal_height};
  const G4double  rInner_Al_Wrapper[numZPlanes_Al_Wrapper] = {0.*cm,0.*cm,
							      crystal_diameter/2.0,
							      crystal_diameter/2.0};
  const G4double  rOuter_Al_Wrapper[numZPlanes_Al_Wrapper]
    = {crystal_diameter/2.0+Al_Wrapper_thickness,
       crystal_diameter/2.0+Al_Wrapper_thickness,
       crystal_diameter/2.0+Al_Wrapper_thickness,
       crystal_diameter/2.0+Al_Wrapper_thickness};
      
  G4Polycone* Al_Wrapper_solid = new G4Polycone (Name+"_Al_wrapper_Sol",0.*rad,2*M_PI*rad,
						 numZPlanes_Al_Wrapper,zPlane_Al_Wrapper,
						 rInner_Al_Wrapper,rOuter_Al_Wrapper);
  G4LogicalVolume* Al_Wrapper_logic = new G4LogicalVolume(Al_Wrapper_solid,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),Name+"_Al_Wrapper_Log",0,0,0);

  Al_Wrapper_logic->SetVisAttributes(gray_vis);
  //Al_Wrapper_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  
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
				      G4ThreeVector(0.,0.,
						    distanceToCrystal
						    -Al_Wrapper_thickness), // Position
				      Al_Wrapper_logic,                // its logical volume
				      Name+"_Al_Wrapper_Phy",              // its name	      
				      this,    // its mother  volume
				      false,                           // no boolean operations 
				      0,
				      true); 
  
}

G4Material* LogVol_PSD_new::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}

void LogVol_PSD_new::SensitiveVolume_PSD_new()
{
  
  if ( !PSD_SensitiveVolume) return;
  // Sensitive Volumes
  if(!Opflag)
    {  // Crystal
      G4cout << "LogVol_Detector_CsI2x2::SensitiveVolume_Detector:::"<<G4endl;
      G4cout << "Including Detector:"<<Name<< G4endl;
      //The name of sensitive detectors (general) were assigned in SetupConstruction::Sensibilize()
      if(G4SDManager::GetSDMpointer()->FindSensitiveDetector("/Detectors/PSDcrystal"))
	{
	  PSD_SensitiveVolume->SetSensitiveDetector(G4SDManager::GetSDMpointer()->
					      FindSensitiveDetector("/Detectors/PSDcrystal"));
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
