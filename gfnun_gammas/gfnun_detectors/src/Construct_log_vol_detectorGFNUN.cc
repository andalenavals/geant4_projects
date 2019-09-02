/*! @file Construct_log_vol_detectorGFNUN.hh
  @brief Defines mandatory user class Construct_log_vol_detectorGFNUN.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PVPlacement.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "DetectorConstruction.hh"
#include "Construct_log_vol_detectorGFNUN.hh"
#include "Materials.hh"

Construct_log_vol_detectorGFNUN::Construct_log_vol_detectorGFNUN(G4String name, G4String type,
								 G4bool vis_flag,
								 G4bool solid_flag,
								 G4bool Xray_flag):  G4LogicalVolume(new G4Box(name+"_solid",10*mm, 10*mm, 10*mm),(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),"dummy_log",0,0,0)
{
  G4VisAttributes* black_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.2,0.2,0.2));
  black_vis->SetForceSolid(solid_flag);
  black_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* gray_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Gray());
  gray_vis->SetForceSolid(solid_flag);
  gray_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* back_vis 
    = new G4VisAttributes(vis_flag,G4Colour(0.5,1.,0.5));
  back_vis->SetForceWireframe(Xray_flag);
  back_vis->SetForceSolid(solid_flag);
  G4VisAttributes* red_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Red());
  red_vis->SetForceSolid(solid_flag);
  red_vis->SetForceWireframe(Xray_flag);
  G4VisAttributes* green_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Green());
  green_vis->SetForceSolid(solid_flag);
  green_vis->SetForceWireframe(Xray_flag);
 G4VisAttributes* blue_vis 
    = new G4VisAttributes(vis_flag,G4Colour::Blue());
  blue_vis->SetForceSolid(solid_flag);
  blue_vis->SetForceWireframe(Xray_flag);

  DetectorConstruction* Detector =
    (DetectorConstruction*)( G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  materials = Materials::GetInstance();
  optical_flag =  Detector ? Detector->GetOptical_flag() : false;
  
  /**********************************************************/
  /**********************************************************/
  /***************    Detectors  (measures)   ***************/
  /**********************************************************/
  /**********************************************************/
  
  /********************/
  /* Global variables */
  /********************/

  G4double Al_Wrapper_thickness = 0.016*mm; // thickness
 
  /// The enclosure and air into enclousure is a Polycone
  G4Polycone* enclosure_solid_out;
  G4Polycone* inside_space_solid_enclosure;
  G4Material* inside_enclosure_material;
  G4Material* crystal_material;

  G4double origin_z = 0.*cm;

  G4int  numZPlanes_enclosure = 0;
  G4double* zPlane_enclosure;
  G4double* rInner_enclosure;
  G4double* rOuter_enclosure; 

  G4int numZPlanes_inside_space = 0;
  G4double* zPlane_inside_space;
  G4double* rInner_inside_space;
  G4double* rOuter_inside_space;

  G4double thickness_enclosure        = 0.;
  G4double Rmax_crystal               = 0.;
  G4double Rmin_crystal               = 0.;
  G4double lz_crystal                 = 0.;
  G4double Space_crystal_to_enclosure = 0.;

  double dynode_length     = 0.;
  double dynode_width      = 0.;
  double dynode_space      = 0.;
  double dynode_origin     = 0.;

  G4double Rmax_bakelite           = 0.;
  G4double width_bakelite          = 0.;
  G4double lz_bakelite             = 0.;
  G4double bakelite_posz1          = 0.;
  G4double bakelite_posz2          = 0.;
  G4double bakelite_posz3          = 0.;

  /********************/
  /**  Ge (10%) **/
  /********************/

  //! Ge (10%) Crystal //

  G4double Ge10_Rmax_crystal = (4.65/2.)*cm; // outer radius  
  G4double Ge10_Rmin_crystal = 0.*cm;        // inner radius  
  G4double Ge10_lz = 4.75*cm;                // length
  G4double Ge10_space_crystal_to_enclosure = 0.5*mm;

  // enclosure //
  G4double Ge10_thickness_enclosure = 0.05*cm; // thickness
  G4double Ge10_Rmin_enclosure      = 0.*cm;   // inner radius, unique  
  G4double Ge10_Rmax_1_enclosure    = 3.8*cm;  // outer radius 1
  G4double Ge10_Rmax_2_enclosure    = 3.95*cm; // outer radius 2 
  G4double Ge10_Rmax_3_enclosure    = 1.8*cm;  // outer radius 3 
  G4double Ge10_Rmax_4_enclosure    = 11.1*cm; // outer radius 4 
  G4double Ge10_Length_1_enclosure  = 10.9*cm; // length 1
  G4double Ge10_Length_2_enclosure  = 23.6*cm; // length 2 
  G4double Ge10_Length_3_enclosure  = 26.2*cm; // length 3 
  G4double Ge10_Length_4_enclosure  = 60.5*cm; // length 4 

  /********************/
  /**  Ge (40%) **/
  /********************/

  //! Ge (40%) Crystal //

  G4double Ge40_Rmax_crystal = (6.22/2.)*cm; // outer radius  
  G4double Ge40_Rmin_crystal = 0.*cm;        // inner radius  
  G4double Ge40_lz = 5.93*cm;                // length
  G4double Ge40_space_crystal_to_enclosure = 4.89*mm;

  // enclosure //
  G4double Ge40_thickness_enclosure = 0.05*cm; // thickness
  G4double Ge40_Rmin_enclosure      = 0.*cm;   // inner radius, unique  
  G4double Ge40_Rmax_1_enclosure    = 3.8*cm;  // outer radius 1
  G4double Ge40_Rmax_2_enclosure    = 3.95*cm; // outer radius 2 
  G4double Ge40_Rmax_3_enclosure    = 1.8*cm;  // outer radius 3 
  G4double Ge40_Rmax_4_enclosure    = 11.1*cm; // outer radius 4 
  G4double Ge40_Length_1_enclosure  = 10.9*cm; // length 1
  G4double Ge40_Length_2_enclosure  = 23.6*cm; // length 2 
  G4double Ge40_Length_3_enclosure  = 26.2*cm; // length 3 
  G4double Ge40_Length_4_enclosure  = 60.5*cm; // length 4 

  /********************/
  /**     NaI_2x2    **/
  /********************/

  //! NaI_2x2 Crystal //

  G4double NaI_2x2_Rmax_crystal               = (5.1/2.)*cm;  // outer radius  
  G4double NaI_2x2_Rmin_crystal               = 0.*cm;        // inner radius  
  G4double NaI_2x2_lz                         = 5.1*cm;                 // length
  G4double NaI_2x2_space_crystal_to_enclosure = 0.5*cm;

  // enclosure // Model: 802-2x2
  G4double NaI_2x2_thickness_enclosure = 0.05*cm;   // thickness
  G4double NaI_2x2_Rmin_enclosure      = 0.*cm;           // inner radius 1, unique 
  G4double NaI_2x2_Rmax_1_enclosure    = (5.71/2.)*cm; // outer radius 1
  G4double NaI_2x2_Rmax_2_enclosure    = (5.87/2.)*cm; // outer radius 2 
  G4double NaI_2x2_Length_1_enclosure  = 5.40*cm; // length 1
  G4double NaI_2x2_Length_2_enclosure  = 18.41*cm; // length 2 

  // Photomultiplier Tube Base / Preamplifier // Model 2007
  G4double NaI_2x2_Rmax_PMTb    = (5.87/2.)*cm;
  G4double NaI_2x2_Length_PMTb  = 7.6*cm; 

  // dynodes :: 10//
  double NaI_2x2_dynode_length  =  NaI_2x2_Rmax_2_enclosure/2.;
  double NaI_2x2_dynode_width   =  0.05*cm ;
  double NaI_2x2_dynode_space = ( NaI_2x2_Length_2_enclosure - 
				  NaI_2x2_Length_1_enclosure - 3*cm)/5.;

  //  PCB from the electronics
  G4double NaI_2x2_Rmax_bakelite           = NaI_2x2_Rmax_PMTb - NaI_2x2_thickness_enclosure-0.2*cm;
  G4double NaI_2x2_width_bakelite          = 0.15*cm;     
  G4double NaI_2x2_lz_bakelite             = NaI_2x2_Length_PMTb-2.*cm;     
  G4double NaI_2x2_bakelite_posz1          = NaI_2x2_Length_2_enclosure+NaI_2x2_width_bakelite/2.;
  G4double NaI_2x2_bakelite_posz2          = NaI_2x2_Length_2_enclosure+NaI_2x2_Length_PMTb-
                                             NaI_2x2_width_bakelite/2.-NaI_2x2_thickness_enclosure;
  G4double NaI_2x2_bakelite_posz3          = NaI_2x2_Length_2_enclosure+NaI_2x2_Length_PMTb/2.;

  /********************/
  /**     NaI_3x3    **/
  /********************/

  //! NaI_3x3 Crystal //

  G4double NaI_3x3_Rmax_crystal               = (7.6/2.)*cm;  // outer radius  
  G4double NaI_3x3_Rmin_crystal               = 0.*cm;        // inner radius  
  G4double NaI_3x3_lz                         = 7.6*cm;                 // length
  G4double NaI_3x3_space_crystal_to_enclosure = 0.5*cm;

  // enclosure // Model: 802-3x3
  G4double NaI_3x3_thickness_enclosure = 0.05*cm;   // thickness
  G4double NaI_3x3_Rmin_enclosure      = 0.*cm;           // inner radius 1, unique 
  G4double NaI_3x3_Rmax_1_enclosure    = (8.10/2.)*cm; // outer radius 1
  G4double NaI_3x3_Rmax_2_enclosure    = (8.26/2.)*cm; // outer radius 2 
  G4double NaI_3x3_Rmax_3_enclosure    = (5.87/2.)*cm; // outer radius 3 
  G4double NaI_3x3_Length_1_enclosure  = 7.94*cm; // length 1
  G4double NaI_3x3_Length_2_enclosure  = NaI_3x3_Length_1_enclosure + 3.0*cm; // length 2 
  G4double NaI_3x3_Length_3_enclosure  = NaI_3x3_Length_2_enclosure + 1.5*cm; // length 3
  G4double NaI_3x3_Length_4_enclosure  = 22.23*cm;

  // Photomultiplier Tube Base / Preamplifier // Model 2007
  G4double NaI_3x3_Rmax_PMTb    = (5.87/2.)*cm;
  G4double NaI_3x3_Length_PMTb  = 7.6*cm; 

  // dynodes :: 10//
  double NaI_3x3_dynode_length  =  NaI_3x3_Rmax_3_enclosure/2.;
  double NaI_3x3_dynode_width   =  0.05*cm ;
  double NaI_3x3_dynode_space = ( NaI_3x3_Length_4_enclosure - 
				  NaI_3x3_Length_1_enclosure - 4*cm)/5.;

  //  PCB from the electronics
  G4double NaI_3x3_Rmax_bakelite           = NaI_3x3_Rmax_PMTb - NaI_3x3_thickness_enclosure-0.2*cm;
  G4double NaI_3x3_width_bakelite          = 0.15*cm;     
  G4double NaI_3x3_lz_bakelite             = NaI_3x3_Length_PMTb-2.*cm;     
  G4double NaI_3x3_bakelite_posz1          = NaI_3x3_Length_4_enclosure+NaI_3x3_width_bakelite/2.;
  G4double NaI_3x3_bakelite_posz2          = NaI_3x3_Length_4_enclosure+NaI_3x3_Length_PMTb-
                                             NaI_3x3_width_bakelite/2.-NaI_3x3_thickness_enclosure;
  G4double NaI_3x3_bakelite_posz3          = NaI_3x3_Length_4_enclosure+NaI_3x3_Length_PMTb/2.;

  /********************/
  /**     Plastic    **/
  /********************/

  //! Plastic Crystal //

  G4double Plastic_Rmax_crystal               = (5.0/2.)*cm;  // outer radius  !! Master thesis Martha Liliana
  G4double Plastic_Rmin_crystal               = 0.*cm;        // inner radius  !! Master thesis Martha Liliana
  G4double Plastic_lz                         = 5.0*cm;       // length
  G4double Plastic_space_crystal_to_enclosure = 0.5*cm;

  // enclosure // Model: Scionix Holland Type 50 BA 50/2 - P
  G4double Plastic_thickness_enclosure = 0.05*cm;   // thickness
  G4double Plastic_Rmin_enclosure      = 0.*cm;           // inner radius 1, unique 
  G4double Plastic_Rmax_1_enclosure    = (5.805/2.)*cm; // outer radius 1
  G4double Plastic_Rmax_2_enclosure    = (5.64/2.)*cm; // outer radius 2 
  G4double Plastic_Length_1_enclosure  = 18.25*cm; // length 1
  G4double Plastic_Length_2_enclosure  = Plastic_Length_1_enclosure + 1.5*cm; // length 2 

  // Photomultiplier Tube Base / Preamplifier // Scinti Pack Model  296
  G4double Plastic_Rmax_PMTb    = (5.42/2.)*cm;
  G4double Plastic_Length_PMTb  = 15.2*cm; 

  // dynodes :: 10//
  double Plastic_dynode_length  =  Plastic_Rmax_PMTb/2.;
  double Plastic_dynode_width   =  0.05*cm ;
  double Plastic_dynode_space = (Plastic_Length_2_enclosure-Plastic_lz)/8.;

  //  PCB from the electronics
  G4double Plastic_Rmax_bakelite           = Plastic_Rmax_PMTb - Plastic_thickness_enclosure-0.2*cm;
  G4double Plastic_width_bakelite          = 0.15*cm;     
  G4double Plastic_lz_bakelite             = Plastic_Length_PMTb/2.-2.*cm;     
  G4double Plastic_bakelite_posz1          = Plastic_Length_2_enclosure+Plastic_width_bakelite/2.;
  G4double Plastic_bakelite_posz2          = Plastic_Length_2_enclosure+Plastic_Length_PMTb-
                                             Plastic_width_bakelite/2.-Plastic_thickness_enclosure;
  G4double Plastic_bakelite_posz3          = Plastic_Length_2_enclosure+Plastic_Length_PMTb/2.;

  /********************/
  /**     CsI    **/
  /********************/

  //! CsI Crystal //

  G4double CsI_Rmax_crystal               = (5.0/2.)*cm;  // outer radius  !! I do not know the correct value
  G4double CsI_Rmin_crystal               = 0.*cm;        // inner radius  !!
  G4double CsI_lz                         = 5.0*cm;       // length !! I do not know the correct value
  G4double CsI_space_crystal_to_enclosure = 0.5*cm;

  // enclosure // 
  G4double CsI_thickness_enclosure = 0.1*cm;   // thickness // I do not know the correct value
  G4double CsI_Rmin_enclosure      = 0.*cm;           // inner radius 1, unique 
  G4double CsI_Rmax_1_enclosure    = (5.5/2.)*cm; // outer radius 1
  G4double CsI_Rmax_2_enclosure    = (6.0/2.)*cm; // outer radius 2 
  G4double CsI_Rmax_3_enclosure    = (6.5/2.)*cm; // outer radius 2 
  G4double CsI_Length_1_enclosure  = 11.0*cm; // length 1
  G4double CsI_Length_2_enclosure  = CsI_Length_1_enclosure + 4.8*cm; // length 2 
  G4double CsI_Length_3_enclosure  = CsI_Length_2_enclosure + 2.7*cm; // length 2 

  // Photomultiplier Tube Base / Preamplifier //
  G4double CsI_Rmax_PMTb    = CsI_Rmax_crystal;
  G4double CsI_Length_PMTb  = CsI_Length_2_enclosure-CsI_Length_1_enclosure; 

  // dynodes :: 10//
  double CsI_dynode_length  =  CsI_Rmax_PMTb/2.;
  double CsI_dynode_width   =  0.05*cm ;
  double CsI_dynode_space = (CsI_Length_1_enclosure-CsI_lz)/8.;

  //  PCB from the electronics
  G4double CsI_Rmax_bakelite           = CsI_Rmax_1_enclosure - CsI_thickness_enclosure-0.2*cm;
  G4double CsI_width_bakelite          = 0.15*cm;     
  G4double CsI_lz_bakelite             = CsI_Length_2_enclosure-CsI_Length_1_enclosure-5*CsI_width_bakelite;     
  G4double CsI_bakelite_posz1          = CsI_Length_1_enclosure+CsI_width_bakelite/2.;
  G4double CsI_bakelite_posz2          = CsI_Length_2_enclosure-CsI_width_bakelite/2.;
  G4double CsI_bakelite_posz3          = CsI_Length_1_enclosure-CsI_thickness_enclosure+
    (CsI_Length_2_enclosure-CsI_Length_1_enclosure)/2;


  /********************/
  /**      dummy     **/
  /********************/

  //! dummy Crystal //
  G4double dummy_Rmax_crystal   = 0.5*cm;     // outer radius 
  G4double dummy_Rmin_crystal   = 0.*cm;     // inner radius 
  G4double dummy_length_crystal = 5.*cm;     // crystal length

  // enclosure //
  G4double dummy_thickness_enclosure = 0.1*mm; // thickness
  G4double dummy_Rmax_enclosure   = 0.65*cm;     /// outer radius 
  G4double dummy_Rmin_enclosure   = 0.*cm;     /// outer radius 
  G4double dummy_length_enclosure = 7.0*cm;     /// length 
  G4double dummy_space_crystal_to_enclosure = 0.*mm;

  /**********************************************************/
  /**********************************************************/
  /*************    Detectors  (construction)   *************/
  /**********************************************************/
  /**********************************************************/

  if(type == "Ge_10")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_lN2");
      crystal_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_Ge");
      numZPlanes_enclosure = 8;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 
      
      zPlane_enclosure[0] = origin_z;                  rOuter_enclosure[0] = Ge10_Rmax_1_enclosure;
      zPlane_enclosure[1] = Ge10_Length_1_enclosure;   rOuter_enclosure[1] = Ge10_Rmax_1_enclosure;
      zPlane_enclosure[2] = Ge10_Length_1_enclosure;   rOuter_enclosure[2] = Ge10_Rmax_2_enclosure;
      zPlane_enclosure[3] = Ge10_Length_2_enclosure;   rOuter_enclosure[3] = Ge10_Rmax_2_enclosure;
      zPlane_enclosure[4] = Ge10_Length_2_enclosure;   rOuter_enclosure[4] = Ge10_Rmax_3_enclosure;
      zPlane_enclosure[5] = Ge10_Length_3_enclosure;   rOuter_enclosure[5] = Ge10_Rmax_3_enclosure;
      zPlane_enclosure[6] = Ge10_Length_3_enclosure;   rOuter_enclosure[6] = Ge10_Rmax_4_enclosure;
      zPlane_enclosure[7] = Ge10_Length_4_enclosure;   rOuter_enclosure[7] = Ge10_Rmax_4_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 8;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+Ge10_thickness_enclosure;
      zPlane_inside_space[1] = Ge10_Length_1_enclosure+Ge10_thickness_enclosure;
      zPlane_inside_space[2] = Ge10_Length_1_enclosure+Ge10_thickness_enclosure;
      zPlane_inside_space[3] = Ge10_Length_2_enclosure-Ge10_thickness_enclosure;
      zPlane_inside_space[4] = Ge10_Length_2_enclosure-Ge10_thickness_enclosure;
      zPlane_inside_space[5] = Ge10_Length_3_enclosure+Ge10_thickness_enclosure;
      zPlane_inside_space[6] = Ge10_Length_3_enclosure+Ge10_thickness_enclosure;
      zPlane_inside_space[7] = Ge10_Length_4_enclosure-Ge10_thickness_enclosure;

      rOuter_inside_space[0] = Ge10_Rmax_1_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[1] = Ge10_Rmax_1_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[2] = Ge10_Rmax_2_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[3] = Ge10_Rmax_2_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[4] = Ge10_Rmax_3_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[5] = Ge10_Rmax_3_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[6] = Ge10_Rmax_4_enclosure-Ge10_thickness_enclosure;
      rOuter_inside_space[7] = Ge10_Rmax_4_enclosure-Ge10_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_inside_space;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = Ge10_Rmax_crystal;
      Rmin_crystal = Ge10_Rmin_crystal;
      lz_crystal = Ge10_lz;
      Space_crystal_to_enclosure = Ge10_space_crystal_to_enclosure+Ge10_thickness_enclosure;
    }
  else if(type == "Ge_40")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_lN2");
      crystal_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_Ge");
      numZPlanes_enclosure = 8;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 
      
      zPlane_enclosure[0] = origin_z;                  rOuter_enclosure[0] = Ge40_Rmax_1_enclosure;
      zPlane_enclosure[1] = Ge40_Length_1_enclosure;   rOuter_enclosure[1] = Ge40_Rmax_1_enclosure;
      zPlane_enclosure[2] = Ge40_Length_1_enclosure;   rOuter_enclosure[2] = Ge40_Rmax_2_enclosure;
      zPlane_enclosure[3] = Ge40_Length_2_enclosure;   rOuter_enclosure[3] = Ge40_Rmax_2_enclosure;
      zPlane_enclosure[4] = Ge40_Length_2_enclosure;   rOuter_enclosure[4] = Ge40_Rmax_3_enclosure;
      zPlane_enclosure[5] = Ge40_Length_3_enclosure;   rOuter_enclosure[5] = Ge40_Rmax_3_enclosure;
      zPlane_enclosure[6] = Ge40_Length_3_enclosure;   rOuter_enclosure[6] = Ge40_Rmax_4_enclosure;
      zPlane_enclosure[7] = Ge40_Length_4_enclosure;   rOuter_enclosure[7] = Ge40_Rmax_4_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 8;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+Ge40_thickness_enclosure;
      zPlane_inside_space[1] = Ge40_Length_1_enclosure+Ge40_thickness_enclosure;
      zPlane_inside_space[2] = Ge40_Length_1_enclosure+Ge40_thickness_enclosure;
      zPlane_inside_space[3] = Ge40_Length_2_enclosure-Ge40_thickness_enclosure;
      zPlane_inside_space[4] = Ge40_Length_2_enclosure-Ge40_thickness_enclosure;
      zPlane_inside_space[5] = Ge40_Length_3_enclosure+Ge40_thickness_enclosure;
      zPlane_inside_space[6] = Ge40_Length_3_enclosure+Ge40_thickness_enclosure;
      zPlane_inside_space[7] = Ge40_Length_4_enclosure-Ge40_thickness_enclosure;

      rOuter_inside_space[0] = Ge40_Rmax_1_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[1] = Ge40_Rmax_1_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[2] = Ge40_Rmax_2_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[3] = Ge40_Rmax_2_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[4] = Ge40_Rmax_3_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[5] = Ge40_Rmax_3_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[6] = Ge40_Rmax_4_enclosure-Ge40_thickness_enclosure;
      rOuter_inside_space[7] = Ge40_Rmax_4_enclosure-Ge40_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_inside_space;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = Ge40_Rmax_crystal;
      Rmin_crystal = Ge40_Rmin_crystal;
      lz_crystal = Ge40_lz;
      Space_crystal_to_enclosure = Ge40_space_crystal_to_enclosure+Ge40_thickness_enclosure;

    }
  else if(type == "NaI_2x2")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
      crystal_material = optical_flag ? FindMaterial("NaI_Tl") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_SODIUM_IODIDE");
      numZPlanes_enclosure = 6;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 

      zPlane_enclosure[0] = origin_z;                  
      zPlane_enclosure[1] = NaI_2x2_Length_1_enclosure; 
      zPlane_enclosure[2] = NaI_2x2_Length_1_enclosure;
      zPlane_enclosure[3] = NaI_2x2_Length_2_enclosure; 
      zPlane_enclosure[4] = NaI_2x2_Length_2_enclosure; 
      zPlane_enclosure[5] = NaI_2x2_Length_2_enclosure+NaI_2x2_Length_PMTb ; 

      rOuter_enclosure[0] = NaI_2x2_Rmax_1_enclosure;
      rOuter_enclosure[1] = NaI_2x2_Rmax_1_enclosure;
      rOuter_enclosure[2] = NaI_2x2_Rmax_2_enclosure;
      rOuter_enclosure[3] = NaI_2x2_Rmax_2_enclosure;
      rOuter_enclosure[4] = NaI_2x2_Rmax_PMTb;
      rOuter_enclosure[5] = NaI_2x2_Rmax_PMTb;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;


      numZPlanes_inside_space = 6;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+NaI_2x2_thickness_enclosure;
      zPlane_inside_space[1] = NaI_2x2_Length_1_enclosure+NaI_2x2_thickness_enclosure;
      zPlane_inside_space[2] = NaI_2x2_Length_1_enclosure+NaI_2x2_thickness_enclosure;
      zPlane_inside_space[3] = NaI_2x2_Length_2_enclosure+NaI_2x2_thickness_enclosure;
      zPlane_inside_space[4] = NaI_2x2_Length_2_enclosure+NaI_2x2_thickness_enclosure;
      zPlane_inside_space[5] = NaI_2x2_Length_2_enclosure+NaI_2x2_Length_PMTb-NaI_2x2_thickness_enclosure;
      
      rOuter_inside_space[0] = NaI_2x2_Rmax_1_enclosure-NaI_2x2_thickness_enclosure;
      rOuter_inside_space[1] = NaI_2x2_Rmax_1_enclosure-NaI_2x2_thickness_enclosure;
      rOuter_inside_space[2] = NaI_2x2_Rmax_2_enclosure-NaI_2x2_thickness_enclosure;
      rOuter_inside_space[3] = NaI_2x2_Rmax_2_enclosure-NaI_2x2_thickness_enclosure;
      rOuter_inside_space[4] = NaI_2x2_Rmax_PMTb-NaI_2x2_thickness_enclosure;
      rOuter_inside_space[5] = NaI_2x2_Rmax_PMTb-NaI_2x2_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = NaI_2x2_Rmax_crystal;
      Rmin_crystal = NaI_2x2_Rmin_crystal;
      lz_crystal = NaI_2x2_lz;
      Space_crystal_to_enclosure = NaI_2x2_space_crystal_to_enclosure+Al_Wrapper_thickness+NaI_2x2_thickness_enclosure;

      dynode_length     = NaI_2x2_dynode_length;
      dynode_width      = NaI_2x2_dynode_width;
      dynode_space      = NaI_2x2_dynode_space;
      dynode_origin     = NaI_2x2_Length_1_enclosure+2.0*cm;

      Rmax_bakelite        = NaI_2x2_Rmax_bakelite;
      width_bakelite       = NaI_2x2_width_bakelite;
      lz_bakelite  = NaI_2x2_lz_bakelite;
      bakelite_posz1       = NaI_2x2_bakelite_posz1;
      bakelite_posz2       = NaI_2x2_bakelite_posz2;
      bakelite_posz3       = NaI_2x2_bakelite_posz3;

    }
  else if(type == "NaI_3x3")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
      crystal_material = optical_flag ? FindMaterial("NaI_Tl") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_SODIUM_IODIDE");

      numZPlanes_enclosure = 8;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 

      zPlane_enclosure[0] = origin_z;                  
      zPlane_enclosure[1] = NaI_3x3_Length_1_enclosure; 
      zPlane_enclosure[2] = NaI_3x3_Length_1_enclosure;
      zPlane_enclosure[3] = NaI_3x3_Length_2_enclosure; 
      zPlane_enclosure[4] = NaI_3x3_Length_3_enclosure; 
      zPlane_enclosure[5] = NaI_3x3_Length_4_enclosure; 
      zPlane_enclosure[6] = NaI_3x3_Length_4_enclosure; 
      zPlane_enclosure[7] = NaI_3x3_Length_4_enclosure+NaI_3x3_Length_PMTb; 

      rOuter_enclosure[0] = NaI_3x3_Rmax_1_enclosure;
      rOuter_enclosure[1] = NaI_3x3_Rmax_1_enclosure;
      rOuter_enclosure[2] = NaI_3x3_Rmax_2_enclosure;
      rOuter_enclosure[3] = NaI_3x3_Rmax_2_enclosure;
      rOuter_enclosure[4] = NaI_3x3_Rmax_3_enclosure;
      rOuter_enclosure[5] = NaI_3x3_Rmax_3_enclosure;
      rOuter_enclosure[6] = NaI_3x3_Rmax_PMTb;
      rOuter_enclosure[7] = NaI_3x3_Rmax_PMTb;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 8;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+NaI_3x3_thickness_enclosure;
      zPlane_inside_space[1] = NaI_3x3_Length_1_enclosure+NaI_3x3_thickness_enclosure;
      zPlane_inside_space[2] = NaI_3x3_Length_1_enclosure+NaI_3x3_thickness_enclosure;
      zPlane_inside_space[3] = NaI_3x3_Length_2_enclosure-NaI_3x3_thickness_enclosure;
      zPlane_inside_space[4] = NaI_3x3_Length_3_enclosure-NaI_3x3_thickness_enclosure;
      zPlane_inside_space[5] = NaI_3x3_Length_4_enclosure;
      zPlane_inside_space[6] = NaI_3x3_Length_4_enclosure;
      zPlane_inside_space[7] = NaI_3x3_Length_4_enclosure+NaI_3x3_Length_PMTb-
	                       NaI_3x3_thickness_enclosure;
      
      rOuter_inside_space[0] = NaI_3x3_Rmax_1_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[1] = NaI_3x3_Rmax_1_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[2] = NaI_3x3_Rmax_2_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[3] = NaI_3x3_Rmax_2_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[4] = NaI_3x3_Rmax_3_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[5] = NaI_3x3_Rmax_3_enclosure-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[6] = NaI_3x3_Rmax_PMTb-NaI_3x3_thickness_enclosure;
      rOuter_inside_space[7] = NaI_3x3_Rmax_PMTb-NaI_3x3_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = NaI_3x3_Rmax_crystal;
      Rmin_crystal = NaI_3x3_Rmin_crystal;
      lz_crystal = NaI_3x3_lz;
      Space_crystal_to_enclosure = NaI_3x3_space_crystal_to_enclosure+Al_Wrapper_thickness+NaI_3x3_thickness_enclosure;

      dynode_length     = NaI_3x3_dynode_length;
      dynode_width      = NaI_3x3_dynode_width;
      dynode_space      = NaI_3x3_dynode_space;
      dynode_origin     = NaI_3x3_Length_1_enclosure+3.0*cm;

      Rmax_bakelite        = NaI_3x3_Rmax_bakelite;
      width_bakelite       = NaI_3x3_width_bakelite;
      lz_bakelite  = NaI_3x3_lz_bakelite;
      bakelite_posz1       = NaI_3x3_bakelite_posz1;
      bakelite_posz2       = NaI_3x3_bakelite_posz2;
      bakelite_posz3       = NaI_3x3_bakelite_posz3;
    }
else if(type == "Plastic")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
      crystal_material = optical_flag ? (G4NistManager::Instance())->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

      numZPlanes_enclosure = 6;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 

      zPlane_enclosure[0] = origin_z;                  
      zPlane_enclosure[1] = Plastic_Length_1_enclosure; 
      zPlane_enclosure[2] = Plastic_Length_1_enclosure;
      zPlane_enclosure[3] = Plastic_Length_2_enclosure; 
      zPlane_enclosure[4] = Plastic_Length_2_enclosure;
      zPlane_enclosure[5] = Plastic_Length_2_enclosure + Plastic_Length_PMTb;

      rOuter_enclosure[0] = Plastic_Rmax_1_enclosure;
      rOuter_enclosure[1] = Plastic_Rmax_1_enclosure;
      rOuter_enclosure[2] = Plastic_Rmax_2_enclosure;
      rOuter_enclosure[3] = Plastic_Rmax_2_enclosure;
      rOuter_enclosure[4] = Plastic_Rmax_PMTb;
      rOuter_enclosure[5] = Plastic_Rmax_PMTb;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 6;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+Plastic_thickness_enclosure;
      zPlane_inside_space[1] = Plastic_Length_1_enclosure-Plastic_thickness_enclosure;
      zPlane_inside_space[2] = Plastic_Length_1_enclosure-Plastic_thickness_enclosure;
      zPlane_inside_space[3] = Plastic_Length_2_enclosure-Plastic_thickness_enclosure;
      zPlane_inside_space[4] = Plastic_Length_2_enclosure-Plastic_thickness_enclosure;
      zPlane_inside_space[5] = Plastic_Length_2_enclosure + Plastic_Length_PMTb-Plastic_thickness_enclosure;
      
      rOuter_inside_space[0] = Plastic_Rmax_1_enclosure-Plastic_thickness_enclosure;
      rOuter_inside_space[1] = Plastic_Rmax_1_enclosure-Plastic_thickness_enclosure;
      rOuter_inside_space[2] = Plastic_Rmax_2_enclosure-Plastic_thickness_enclosure;
      rOuter_inside_space[3] = Plastic_Rmax_2_enclosure-Plastic_thickness_enclosure;
      rOuter_inside_space[4] = Plastic_Rmax_PMTb-Plastic_thickness_enclosure;
      rOuter_inside_space[5] = Plastic_Rmax_PMTb-Plastic_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = Plastic_Rmax_crystal;
      Rmin_crystal = Plastic_Rmin_crystal;
      lz_crystal = Plastic_lz;
      Space_crystal_to_enclosure = Plastic_space_crystal_to_enclosure+Al_Wrapper_thickness+Plastic_thickness_enclosure;

      dynode_length     = Plastic_dynode_length;
      dynode_width      = Plastic_dynode_width;
      dynode_space      = Plastic_dynode_space;
      dynode_origin     = Plastic_lz+3.0*cm;

      Rmax_bakelite        = Plastic_Rmax_bakelite;
      width_bakelite       = Plastic_width_bakelite;
      lz_bakelite  = Plastic_lz_bakelite;
      bakelite_posz1       = Plastic_bakelite_posz1;
      bakelite_posz2       = Plastic_bakelite_posz2;
      bakelite_posz3       = Plastic_bakelite_posz3;
    }
 else if(type == "CsI")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
      crystal_material = optical_flag ? (G4NistManager::Instance())->FindOrBuildMaterial("CsI_Tl") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE");

      numZPlanes_enclosure = 6;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 

      zPlane_enclosure[0] = origin_z;                  
      zPlane_enclosure[1] = CsI_Length_1_enclosure; 
      zPlane_enclosure[2] = CsI_Length_1_enclosure;
      zPlane_enclosure[3] = CsI_Length_2_enclosure; 
      zPlane_enclosure[4] = CsI_Length_2_enclosure;
      zPlane_enclosure[5] = CsI_Length_3_enclosure;

      rOuter_enclosure[0] = CsI_Rmax_1_enclosure;
      rOuter_enclosure[1] = CsI_Rmax_1_enclosure;
      rOuter_enclosure[2] = CsI_Rmax_2_enclosure;
      rOuter_enclosure[3] = CsI_Rmax_2_enclosure;
      rOuter_enclosure[4] = CsI_Rmax_3_enclosure;
      rOuter_enclosure[5] = CsI_Rmax_3_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 6;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+CsI_thickness_enclosure;
      zPlane_inside_space[1] = CsI_Length_1_enclosure+CsI_thickness_enclosure;
      zPlane_inside_space[2] = CsI_Length_1_enclosure+CsI_thickness_enclosure;
      zPlane_inside_space[3] = CsI_Length_2_enclosure+CsI_thickness_enclosure;
      zPlane_inside_space[4] = CsI_Length_2_enclosure+CsI_thickness_enclosure;
      zPlane_inside_space[5] = CsI_Length_3_enclosure-CsI_thickness_enclosure;
      
      rOuter_inside_space[0] = CsI_Rmax_1_enclosure-CsI_thickness_enclosure;
      rOuter_inside_space[1] = CsI_Rmax_1_enclosure-CsI_thickness_enclosure;
      rOuter_inside_space[2] = CsI_Rmax_2_enclosure-CsI_thickness_enclosure;
      rOuter_inside_space[3] = CsI_Rmax_2_enclosure-CsI_thickness_enclosure;
      rOuter_inside_space[4] = CsI_Rmax_2_enclosure-CsI_thickness_enclosure;
      rOuter_inside_space[5] = CsI_Rmax_2_enclosure-CsI_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = CsI_Rmax_crystal;
      Rmin_crystal = CsI_Rmin_crystal;
      lz_crystal = CsI_lz;
      Space_crystal_to_enclosure = CsI_space_crystal_to_enclosure+Al_Wrapper_thickness+CsI_thickness_enclosure;

      dynode_length     = CsI_dynode_length;
      dynode_width      = CsI_dynode_width;
      dynode_space      = CsI_dynode_space;
      dynode_origin     = CsI_lz+1.5*cm;

      Rmax_bakelite        = CsI_Rmax_bakelite;
      width_bakelite       = CsI_width_bakelite;
      lz_bakelite  = CsI_lz_bakelite;
      bakelite_posz1       = CsI_bakelite_posz1;
      bakelite_posz2       = CsI_bakelite_posz2;
      bakelite_posz3       = CsI_bakelite_posz3;
    }
  else if(type == "dummy")
    {
      inside_enclosure_material = (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR");
      crystal_material = optical_flag ? FindMaterial("CsI_Tl") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE");
      numZPlanes_enclosure = 2;
      zPlane_enclosure = new G4double [numZPlanes_enclosure];
      rInner_enclosure = new G4double [numZPlanes_enclosure];
      rOuter_enclosure = new G4double [numZPlanes_enclosure]; 

      zPlane_enclosure[0] = origin_z;                  
      zPlane_enclosure[1] = dummy_length_enclosure; 

      rOuter_enclosure[0] = dummy_Rmax_enclosure;
      rOuter_enclosure[1] = dummy_Rmax_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_enclosure[i]=0.*cm;

      numZPlanes_inside_space = 2;
      zPlane_inside_space = new G4double [numZPlanes_inside_space];
      rInner_inside_space = new G4double [numZPlanes_inside_space];
      rOuter_inside_space = new G4double [numZPlanes_inside_space];
      
      zPlane_inside_space[0] = origin_z+dummy_thickness_enclosure;
      zPlane_inside_space[1] = dummy_length_enclosure-dummy_thickness_enclosure;

      rOuter_inside_space[0] = dummy_Rmax_enclosure-dummy_thickness_enclosure;
      rOuter_inside_space[1] = dummy_Rmax_enclosure-dummy_thickness_enclosure;

      for(G4int i=0; i < numZPlanes_enclosure;i++)
	rInner_inside_space[i]=0.*cm;

      Rmax_crystal = dummy_Rmax_crystal;
      Rmin_crystal = dummy_Rmin_crystal;
      lz_crystal = dummy_length_crystal;
      Space_crystal_to_enclosure = dummy_space_crystal_to_enclosure+Al_Wrapper_thickness+dummy_thickness_enclosure;
    }
  else
    {
      G4cout<<"\n"<<"\n"<<"***********  ERROR MESSAGE  ***********"<<"\n"<<G4endl;
      G4cout<<"ERROR:: Detector undefined"<<"\n"<<G4endl;
      G4cout<<"The simulation will be interrupted"<<"\n"<<G4endl;
      G4Exception ("InvalidSetup", "Detector not found", FatalException,"Detector undefined !!");
    }

  enclosure_solid_out = new G4Polycone("enclosure_solid_out",0.*rad,2*M_PI*rad,numZPlanes_enclosure,
				       zPlane_enclosure,rInner_enclosure,rOuter_enclosure);


  /*** MAIN TRICK HERE ***/
  if(enclosure_solid_out)
    this->SetSolid(enclosure_solid_out);
  /*** MAIN TRICK HERE ***/

  this->SetVisAttributes(gray_vis);

  inside_space_solid_enclosure = new G4Polycone("inside_enclosure",0.*rad,2*M_PI*rad,
						numZPlanes_inside_space,zPlane_inside_space,
						rInner_inside_space,rOuter_inside_space);
  G4LogicalVolume*  inside_space_logic_enclosure = new G4LogicalVolume(inside_space_solid_enclosure,
								       inside_enclosure_material,
								       "Inside_enclosure_"+type,0,0,0);
  G4VPhysicalVolume* inside_space_physical_enclosure;
  inside_space_physical_enclosure = new G4PVPlacement(0,               // Rotation
						      G4ThreeVector(), // Position
						      inside_space_logic_enclosure, // its logic vol
						      "Inside_enclosure_"+type, //name
						      this,            // its mother vol
						      false,           // no boolean operations 
						      0,
						      true); 

  inside_space_logic_enclosure->SetVisAttributes(G4VisAttributes::GetInvisible());
   // /! Crystal construction

  G4Tubs* crystal_solid = new G4Tubs("crystal",Rmin_crystal,Rmax_crystal,lz_crystal/2.,0.,2*pi);
  
  G4LogicalVolume*  crystal_logic = new G4LogicalVolume(crystal_solid,
							crystal_material,
							"Crystal_"+type,0,0,0);
  G4VPhysicalVolume* crystal_physical;
  crystal_physical = new G4PVPlacement(0,               // Rotation
				       G4ThreeVector(0.,0.,lz_crystal/2.
						     +Space_crystal_to_enclosure), // Position
				       crystal_logic, // its logic vol
				       "Crystal_"+type, //name
				       inside_space_logic_enclosure, // its mother vol
				       false,           // no boolean operations 
				       0,
				       true);
  crystal_logic->SetVisAttributes(back_vis); 
  sensitiveVolume_log = crystal_logic;

  // /! Aluminium wrapper and photocathode
  
  if(type == "NaI_2x2" || type == "NaI_3x3" || type == "Plastic" || type == "CsI" || type == "dummy")
    {
      
      // /! Aluminium wrapper 
      const G4int numZPlanes_Al_Wrapper = 4;
      const G4double  zPlane_Al_Wrapper[numZPlanes_Al_Wrapper] = {origin_z,
								  Al_Wrapper_thickness,
								  Al_Wrapper_thickness,
								  2*Al_Wrapper_thickness+lz_crystal};
      const G4double  rInner_Al_Wrapper[numZPlanes_Al_Wrapper] = {0.*cm,0.*cm,
								 Rmax_crystal,Rmax_crystal};
      const G4double  rOuter_Al_Wrapper[numZPlanes_Al_Wrapper] = {Rmax_crystal+Al_Wrapper_thickness,
								 Rmax_crystal+Al_Wrapper_thickness,
								 Rmax_crystal+Al_Wrapper_thickness,
								 Rmax_crystal+Al_Wrapper_thickness};
      
      G4Polycone* Al_Wrapper_solid = new G4Polycone ("Al_wrapper",0.*rad,2*M_PI*rad,
						     numZPlanes_Al_Wrapper,zPlane_Al_Wrapper,
						     rInner_Al_Wrapper,rOuter_Al_Wrapper);
      G4LogicalVolume* Al_Wrapper_logic = new G4LogicalVolume(Al_Wrapper_solid,(G4NistManager::Instance())->FindOrBuildMaterial("G4_Al"),"Al_Wrapper"+type,0,0,0);
      Al_Wrapper_logic->SetVisAttributes(gray_vis);
      if(optical_flag ) // Optical property
	{
	  G4OpticalSurface* Al_Wrapper_Surface = new G4OpticalSurface("Al_Wrapper_Surface",
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
	}
      
      G4VPhysicalVolume* physiAl_Wrapper;
      physiAl_Wrapper = new G4PVPlacement(0,                                  // Rotation
					  G4ThreeVector(0.,0.,
							Space_crystal_to_enclosure
							-Al_Wrapper_thickness), // Position
					  Al_Wrapper_logic,                // its logical volume
					  "Al_Wrapper_"+type,                    // its name	      
					  inside_space_logic_enclosure,    // its mother  volume
					  false,                           // no boolean operations 
					  0,
					  true); 

      // /! Photocathode
      G4double Photocathode_Rmax = Rmax_crystal; 
      G4double Photocathode_height = 20.*nm; 
      
      G4Tubs* solid_Photocathode;
      solid_Photocathode = new G4Tubs("photocathode", 0.0*cm, Photocathode_Rmax,Photocathode_height/2.,0,2*M_PI*rad);

      //! *****  Prove solid::: is a volume that cover the crystal ***** //

      // const G4int numZPlanes_Photocathode = 6;
      // const G4double  zPlane_Photocathode[numZPlanes_Photocathode] = {origin_z,
      // 								      Photocathode_height,
      // 								      Photocathode_height,
      //							      lz_crystal-Photocathode_height,
      //							      lz_crystal-Photocathode_height,
      // 								      lz_crystal};
      // const G4double  rInner_Photocathode[numZPlanes_Photocathode] = {0.*cm,0.*cm,
      //							      Rmax_crystal-Photocathode_height,
      //							      Rmax_crystal-Photocathode_height,
      // 								      0.*cm,0.*cm };
      // const G4double  rOuter_Photocathode[numZPlanes_Photocathode] = {Rmax_crystal,Rmax_crystal,
      //							      Rmax_crystal,Rmax_crystal,
      //							      Rmax_crystal,Rmax_crystal};
      //G4Polycone* solid_Photocathode = new G4Polycone ("photocathode",0.*rad,2*M_PI*rad,
      //					       numZPlanes_Photocathode,zPlane_Photocathode,
      //					       rInner_Photocathode,rOuter_Photocathode);

      G4LogicalVolume* logic_Photocathode;
      logic_Photocathode = new G4LogicalVolume(solid_Photocathode,optical_flag ? FindMaterial("BIALKALI") : (G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),"Photocathode_"+type,0,0,0);
      G4VPhysicalVolume* physical_Photocathode;
      physical_Photocathode = new G4PVPlacement(0,
						G4ThreeVector(0.,0.,Space_crystal_to_enclosure
							      -Photocathode_height/2. +lz_crystal),
						/*G4ThreeVector(0.,0.,-Photocathode_height/2+
						  lz_crystal/2.),*/
						 logic_Photocathode,
						 "Photocathode_"+type,                 
						inside_space_logic_enclosure, 
						//crystal_logic,
						false,                  
						0,
						true);
      optical_flag ? logic_Photocathode->SetVisAttributes(red_vis) : logic_Photocathode->SetVisAttributes(G4VisAttributes::GetInvisible());
      if(optical_flag) // The sensitive volume changes from crystal_logic to logic_photocatode
	sensitiveVolume_log = logic_Photocathode;

      ///! Addons::: Bakelite plates and dynodes
      if(type == "NaI_2x2" || type == "NaI_3x3" || type == "Plastic" || type == "CsI")
	{
	  G4RotationMatrix* Rotation1 
	    = new G4RotationMatrix();
	  Rotation1->rotateX(M_PI/3.5*rad);
	  G4RotationMatrix* Rotation2 
	    = new G4RotationMatrix();
	  Rotation2->rotateX((M_PI/3.5 + M_PI/2.)*rad);

	  //! *** Dynodes *** !//
	  G4Box * solid_dynode = new G4Box("dynode",dynode_length/2.,
					   dynode_length/2.,dynode_width/2.);
	  G4LogicalVolume* logic_dynode = new G4LogicalVolume(solid_dynode,(G4NistManager::Instance())->FindOrBuildMaterial("G4_C"), "Dynode",0,0,0);

	  logic_dynode->SetVisAttributes(blue_vis);

	  G4VPhysicalVolume** physical_dynode;
	  physical_dynode = new G4VPhysicalVolume* [10]; /// ten dynodes
	  G4ThreeVector dinode_position;
	  G4int dynode_posz = 0;
	  for(G4int i = 0 ; i<10 ; i++)
	    {
	      if(i%2 == 0)
		dinode_position = G4ThreeVector(0.,rOuter_inside_space[4]/3.8,dynode_origin+
						dynode_posz*dynode_space);
	      else
		{
		  dinode_position = G4ThreeVector(0.,-rOuter_inside_space[4]/3.8,dynode_origin+
						dynode_space/2.+dynode_posz*dynode_space);
		  dynode_posz++;
		}

	      physical_dynode[i] = new G4PVPlacement(i%2 == 0?Rotation1:Rotation2,
						     dinode_position,
						     logic_dynode,
						     "Dynode"+G4UIcommand::ConvertToString(i),
						     inside_space_logic_enclosure,
						     false,  
						     0,
						     true);
	    }

	  //! *** PCB from the electronics ***!//
	  G4Box * solid_pcb_box = new G4Box("pcb_box",width_bakelite/2.,
					    Rmax_bakelite,lz_bakelite/2.);
	  G4Tubs* solid_pcb_tub;
	  solid_pcb_tub = new G4Tubs("pcb_tub", 0., Rmax_bakelite,
				     width_bakelite/2.,0.*rad,2*M_PI*rad); 

	  G4LogicalVolume* logic_pcb_box;
	  logic_pcb_box = new G4LogicalVolume(solid_pcb_box,(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE"), "Pcb_box",0,0,0);
	  G4LogicalVolume* logic_pcb_tub;
	  logic_pcb_tub = new G4LogicalVolume(solid_pcb_tub,(G4NistManager::Instance())->FindOrBuildMaterial("G4_BAKELITE"), "Pcb_tub",0,0,0);

	  logic_pcb_box->SetVisAttributes(green_vis);
	  logic_pcb_tub->SetVisAttributes(green_vis);

	  G4VPhysicalVolume* physical_pcb_tub1;
	  physical_pcb_tub1 = new G4PVPlacement(0,
						G4ThreeVector(0.,0.,bakelite_posz1),
						logic_pcb_tub,    
						"Pcb_tub1",  
						inside_space_logic_enclosure,
						false,    
						0,
						true); 
	  G4VPhysicalVolume* physical_pcb_tub2;
	  physical_pcb_tub2 = new G4PVPlacement(0,
						G4ThreeVector(0.,0.,bakelite_posz2),
						logic_pcb_tub,    
						"Pcb_tub2",  
						inside_space_logic_enclosure,
						false,    
						0,
						true); 
	  G4RotationMatrix* Rotation3 
	    = new G4RotationMatrix();
	  Rotation3->rotateZ(M_PI/2.*rad);

	  G4VPhysicalVolume* physical_pcb_box;
	  physical_pcb_box = new G4PVPlacement(Rotation3,
					       G4ThreeVector(0.,0.,bakelite_posz3),
					       logic_pcb_box,    
					       "Pcb_box",  
					       inside_space_logic_enclosure,
					       false,    
					       0,
					       true); 
	}
    }
  
}

Construct_log_vol_detectorGFNUN::~Construct_log_vol_detectorGFNUN()
{
  if (materials)        
    delete materials;
  //delete sensitiveVolume_log;
}

G4Material* Construct_log_vol_detectorGFNUN::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}
