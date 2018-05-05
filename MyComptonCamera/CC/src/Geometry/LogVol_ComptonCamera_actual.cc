/*! @file LogVol_ComptonCamera_actual.hh
  @brief Defines mandatory user class LogVol_ComptonCamera_actual.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/
/* no-geant4 classes*/
#include "Materials.hh"

#include "LogVol_ComptonCamera_actual.hh"
#include "LogVol_CCHardCovering_Lid.hh"
#include "LogVol_CCHardCovering_Base.hh"
#include "LogVol_GammaSourceCasing.hh"
#include "LogVol_PCBs_Cyl.hh"
#include "LogVol_PCBs_Box.hh"
#include "LogVol_Collimator_ToSample.hh"
#include "LogVol_Collimator_ToPSD.hh"
#include "LogVol_ShieldingPSDCone.hh"
#include "LogVol_Crystal_Cyl.hh"
#include "LogVol_SegmentedCrystal_Box.hh"
#include "LogVol_DynodesArray.hh"
#include "LogVol_PMTbase.hh"
#include "LogVol_PSD_actual.hh"
#include "LogVol_BSD_actual.hh"

#include "PhysVol_SegmentedCrystal_Box.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* geometric objects */
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
/* logic and physical volume */
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
/* geant4 materials */
#include "G4NistManager.hh"
/* visualization */
#include "G4VisAttributes.hh"

LogVol_ComptonCamera_actual:: LogVol_ComptonCamera_actual(G4String fname,G4bool fOpflag,G4bool fadd):
  G4LogicalVolume(new G4Tubs(fname+"_Sol",0,80./2*mm,60.*mm/2.,0.,2*pi),
		  (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),fname,0,0,0)
{ 
  /* set variables */
  SetName(fname);
  SetOpflag(fOpflag);
  SetAddonsflag(fadd);
  materials = NULL;
  /* dimensions */
  base_thick=0.3*cm;
  sourcedis=base_thick+1.5516*cm;
  /* Construct solid volume */
  ConstructLogVol_ComptonCamera_actual();
  /* Visualization */
  G4VisAttributes* red_vis 
    = new G4VisAttributes(true,G4Colour::Red());
  red_vis->SetForceWireframe(false);
  red_vis->SetForceSolid(false);
  this->SetVisAttributes(red_vis);
  //this->SetVisAttributes(G4VisAttributes::GetInvisible());
  
}

LogVol_ComptonCamera_actual::~LogVol_ComptonCamera_actual()
{;}

void LogVol_ComptonCamera_actual::ConstructLogVol_ComptonCamera_actual(void)
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

  // dimensions
  
  /////////////////////////////////////////////////////////////////
  ////***** Space where ComptonCamera will be constructed *****////
  /////////////////////////////////////////////////////////////////
  G4double Cyl_height = 19.3*cm;
  G4double Cyl_radius = 23.4/2.0*cm;
  G4double Box_height = 10.0*cm;
  G4double Box_length = 28.0*cm;
  G4double Box_width  = 23.4*cm;

  const G4int numZPlanes_Cyl = 2;
  G4double zPlane_Cyl[numZPlanes_Cyl] = {0.0*cm,Cyl_height};
  G4double rInner_Cyl[numZPlanes_Cyl] = {0.0*cm,0.0*cm};
  G4double rOuter_Cyl[numZPlanes_Cyl] = {Cyl_radius,Cyl_radius};
  
  G4Polycone* Cyl_Space = new G4Polycone(Name+"_CylSpace_Sol",0.*rad,2*M_PI*rad,numZPlanes_Cyl,
					 zPlane_Cyl,rInner_Cyl,rOuter_Cyl);
  G4Box* Box_Space = new G4Box(Name+"_BoxSpace_Sol",Box_width/2.0,Box_length/2.0,Box_height/2.0);

  ///* Trick *////
  if(Addonsflag)
    ComptonCamera_actual_solid = new G4UnionSolid(Name+"_BaseOut_Sol", Cyl_Space, Box_Space,0,
						  G4ThreeVector(0.,Box_length/2.0,Box_height/2.0));
  else
    ComptonCamera_actual_solid = Cyl_Space;
  /*** Main trick here: Set the new solid volume ***/
  SetSolidVol_ComptonCamera_actual();

  /////////////////////////////////////////////////////
  ////////////// Contructing the camera ///////////////
  /////////////////////////////////////////////////////
  ConstructLogVol_MainGeometry();
  if(Addonsflag)
    ConstructLogVol_Addons();
}
void LogVol_ComptonCamera_actual::ConstructLogVol_Addons(void)
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
    
  ///////////////////////////////////////////////
  ///////////**** Aluminum Base ****/////////////
  ///////////////////////////////////////////////
  LogVol_CCHardCovering_Base* logic_CCHardCovering_Base;
  logic_CCHardCovering_Base = new LogVol_CCHardCovering_Base(Name+"_Base_Log");
  G4VPhysicalVolume* physical_CCHardCovering_Base;
  physical_CCHardCovering_Base = new G4PVPlacement(0,         // no rotation
						  G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), //Completa
						  logic_CCHardCovering_Base,    // its logical volume
						  Name+"_Base_Phy",      // its name
						  this,
						  false,     // no boolean operations 
						  0,
  						  true);  
  //////////////////////////////////////////////
  ///////////**** Aluminum Lid ****/////////////
  //////////////////////////////////////////////
  LogVol_CCHardCovering_Lid* logic_CCHardCovering_Lid;
  logic_CCHardCovering_Lid = new LogVol_CCHardCovering_Lid(Name+"_Lid_Log");
  G4VPhysicalVolume* physical_CCHardCovering_Lid;
  physical_CCHardCovering_Lid = new G4PVPlacement(0,         // no rotation
						  G4ThreeVector(0.0*cm,0.0*cm,base_thick), //Completa
						  logic_CCHardCovering_Lid,    // its logical volume
						  Name+"_Lid_Phy",      // its name
						  this,
						  false,     // no boolean operations 
						  0,
  						  true);
  ////////////////////////////////////////////
  ///////**** External electronics ****///////
  ////////////////////////////////////////////
  LogVol_PCBs_Box* pcb_log;
  pcb_log = new LogVol_PCBs_Box(Name+"_PCBs_log",10.0*cm,5.3*cm,0.3*cm);//name,width,length,thick
  G4VPhysicalVolume* physical_pcb;
  physical_pcb = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(0.0,19.0*cm,base_thick+0.15*cm), //Completa
				   pcb_log,    // its logical volume
				   Name+"_PCBs_Phy",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
  ///////////////////////////////////////////////////////////
  ///////**** Aluminum case of the Lead Shielding ****///////
  ///////////////////////////////////////////////////////////
  
  const G4int numZPlanesEnvBlinPoly=6;
  const G4double  zPlaneEnvBlinPoly[]={0.*mm,20*mm,20*mm,43.46*mm,43.46*mm,49.8*mm};
  const G4double  rInnerEnvBlinPoly[]={46.6/2*mm,67.5/2*mm,80.6/2*mm,80.6/2*mm,80.6/2*mm,80.6/2*mm};
  const G4double  rOuterEnvBlinPoly[]={86.72/2*mm,86.72/2*mm,86.72/2*mm,86.72/2*mm,109.4/2*mm,109.4/2*mm};
  
  G4Polycone* solidEnvBlinPoly 
    = new G4Polycone("envBlinPoly",0.*rad,2*M_PI*rad,numZPlanesEnvBlinPoly,zPlaneEnvBlinPoly,rInnerEnvBlinPoly,rOuterEnvBlinPoly);
  G4Trap* solidEnvBlinTrap 
    = new G4Trap("envBlinTrap",52./2*mm,157.2/2*mm,52./2*mm,157.2/2*mm,60./2*mm);
  G4IntersectionSolid* solidEnvBlinDetPos 
    = new G4IntersectionSolid("envBlinDetPos",solidEnvBlinPoly,solidEnvBlinTrap,0,G4ThreeVector(0.,0.,60./2*mm));
  G4LogicalVolume* logicEnvBlinDetPos  = new G4LogicalVolume(solidEnvBlinDetPos,
							     (G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),
							     Name+"_CasePbShi_Log",0,0,0);
  logicEnvBlinDetPos->SetVisAttributes(gray_vis);
  G4VPhysicalVolume* physiEnvBlinDetPos;
  physiEnvBlinDetPos= new G4PVPlacement(0,
					G4ThreeVector(0.,0.,31.5*mm),
					logicEnvBlinDetPos,
					Name+"_CasePbShi_Phy",  
					this,
					false,            
					0,
					true);
}
void LogVol_ComptonCamera_actual::ConstructLogVol_MainGeometry(void)
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
    
  /////////////////////////////////////
  ///////////**** BSD ****/////////////
  /////////////////////////////////////
  LogVol_BSD_actual* BSD_log;
  BSD_log = new LogVol_BSD_actual(Name+"BSD_Log",(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"),true);
  G4VPhysicalVolume* BSD_phy;
  BSD_phy = new G4PVPlacement(0,         // no rotation
			      G4ThreeVector(0,0.0*cm,base_thick), //Completa
			      BSD_log,    // its logical volume
			      Name+"BSD_Phy",      // its name
			      this,
			      false,     // no boolean operations 
			      0,
			      true);
  /////////////////////////////////////
  ///////////**** PSD ****/////////////
  /////////////////////////////////////
  G4double disPSDtoSource = 33.4*mm;
  LogVol_PSD_actual* PSD_log;
  PSD_log = new LogVol_PSD_actual(Name+"PSD_Log",(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"),true);
  G4VPhysicalVolume* PSD_phy;
  PSD_phy = new G4PVPlacement(0,         // no rotation
			      G4ThreeVector(0,0.0*cm,sourcedis+disPSDtoSource), //Completa
			      PSD_log,    // its logical volume
			      Name+"PSD_Phy",      // its name
			      this,
			      false,     // no boolean operations 
			      0,
			      true);
  /////////////////////////////////////////////////////
  ///////////**** Colimator to sample ****/////////////
  /////////////////////////////////////////////////////
   LogVol_Collimator_ToSample* coltos_log;
  coltos_log = new LogVol_Collimator_ToSample(Name+"_CollToSample_Log");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* coltos_phy;
  coltos_phy = new G4PVPlacement(0,         // no rotation
				 G4ThreeVector(0,-0.0*cm,sourcedis), //Completa
				 coltos_log,    // its logical volume
				 Name+"_CollToSample_Log",      // its name
				 this,
				 false,     // no boolean operations 
				 0,
				 true);
  //////////////////////////////////////////////////
  ///////////**** Colimator to PSD ****/////////////
  //////////////////////////////////////////////////
   LogVol_Collimator_ToPSD* coltoPSD_log;
  coltoPSD_log = new LogVol_Collimator_ToPSD(Name+"_CollToPSD_Log");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* coltoPSD_phy;
  coltoPSD_phy = new G4PVPlacement(0,         // no rotation
				   G4ThreeVector(0,-0.0*cm,sourcedis), //Completa
				   coltoPSD_log,    // its logical volume
				   Name+"_CollToPSD_Log",      // its name
				   this,
				   false,     // no boolean operations 
				   0,
				   true);
  ////////////////////////////////////////////////
  ///////////**** Lead Shielding ****/////////////
  ////////////////////////////////////////////////
  G4double posPbS = 3.6*mm;
  LogVol_ShieldingPSDCone* Pbshi_log;
  Pbshi_log = new LogVol_ShieldingPSDCone(Name+"_LeadShielding_Log");//name,radius,height,angular opening, source radius, source height, material
  G4VPhysicalVolume* Pbshi_phy;
  Pbshi_phy = new G4PVPlacement(0,         // no rotation
				G4ThreeVector(0.0,0.0,sourcedis+posPbS), //Completa
				Pbshi_log,    // its logical volume
				Name+"_LeadShielding_Phy",      // its name
				this,
				false,     // no boolean operations 
				0,
				true);
  ///////////////////////////////////////////////
  ///////////**** Source Casing ****/////////////
  ///////////////////////////////////////////////
  // Note:: Into source casing a sphere of source material is placed,
  //        in order to include self-scattering by the source.
  //        Please check this volume to include or not this inner volume
  //        If you change the source type, please change the matrial into logic_GammaSourceCasing
  LogVol_GammaSourceCasing* logic_GammaSourceCasing;
  logic_GammaSourceCasing = new LogVol_GammaSourceCasing(Name+"_GSourceCasing_log",0.63*cm,2.0*mm);//(name,radius,height)
  G4VPhysicalVolume* physical_Acrilyc_Capsule;
  physical_Acrilyc_Capsule = new G4PVPlacement(0,         // no rotation
  					       G4ThreeVector(0.0*cm,0.0*cm,sourcedis-0.1*cm),
  					       logic_GammaSourceCasing,    // its logical volume
  					       Name+"_GSourceCasing_phy",      // its name
  					       this,
  					       false,     // no boolean operations 
  					       0,
  					       true);
}
void LogVol_ComptonCamera_actual::SetSolidVol_ComptonCamera_actual(void)
{
  /*** Main trick here:  Set the new solid volume ***/
  if(ComptonCamera_actual_solid)
    this->SetSolid(ComptonCamera_actual_solid);
}
G4Material* LogVol_ComptonCamera_actual::FindMaterial(G4String name) {
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}

