#ifndef LogVol_BSD_new_h 
#define LogVol_BSD_new_h 1

/*! @file LogVol_BSD_new.hh
  @brief Defines mandatory user class LogVol_BSD_new.
  @date August, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
 */

#include "globals.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4Material;
class Materials;
class LogVol_BSDcrystal;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_BSD_new : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_BSD_new(G4String fname="BSD_new_log",
		 G4Material* fMaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),
		 G4bool det_flag=true,
		 G4bool fOpflag=false);
  //! Destructor
  ~LogVol_BSD_new();
public:
  inline void SetName(G4String pname)         {Name    = pname;};
  inline void SetOpflag(G4bool pflag)         {Opflag  = pflag;};
  inline void SetDetectorFlag(G4bool flag) {detector_flag = flag;};
private:
  void ConstructLogVol_BSD_new(void);
  void SensitiveVolume_BSD_new(void);
  void SetSolidVol_BSD_new(void);
  void Construct_Crystal_OpFalse(void);
  void Construct_Crystal_OpTrue(void);
  //! Materials defined in Materials.hh
  G4Material* FindMaterial(G4String mat);
private:
  // General solid volume
  G4VSolid* BSD_new_solid;
  // Sensitive volume
  G4LogicalVolume* BSD_SensitiveVolume;
  G4LogicalVolume* BSD_SensitiveVolume_PC1; // photocathode 1: optical photons = true
  G4LogicalVolume* BSD_SensitiveVolume_PC2; // photocathode 1: optical photons = true
  //! Name
  G4String Name;
  //CrystalMaterial
  G4Material* Material;
  Materials*  materials;
  //Optical photons
  G4bool Opflag;
  //! Detector flag: works or not
  G4bool detector_flag;
  //dimensions
  G4double Case_height;
  G4double Case_Oradius;
  G4double Case_Iradius;
  G4double Case_thick;
  G4double origin_z;
  G4double crystal_Oradius;
  G4double crystal_Iradius;
  G4double crystal_height;
  G4double distanceToCrystal;
  G4double Al_Wrapper_thickness;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
