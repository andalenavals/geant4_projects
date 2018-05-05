#ifndef LogVol_PSD_new_h 
#define LogVol_PSD_new_h 1

/*! @file LogVol_PSD_new.hh
  @brief Defines mandatory user class LogVol_PSD_new.
  @date May, 2016
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
class LogVol_PSDcrystal;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_PSD_new : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_PSD_new(G4String fname="PSD_new_log",
		 G4Material* fMaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_AIR"),
		 G4bool det_flag=true,
		 G4bool fOpflag=false);
  //! Destructor
  ~LogVol_PSD_new();
public:
  inline void SetName(G4String pname)         {Name    = pname;};
  inline void SetOpflag(G4bool pflag)         {Opflag  = pflag;};
  inline void SetDetectorFlag(G4bool flag) {detector_flag = flag;};
private:
  void ConstructLogVol_PSD_new(void);
  void SensitiveVolume_PSD_new(void);
  void Construct_Crystal_OpFalse(void);
  void Construct_Crystal_OpTrue(void);
  void SetSolidVol_PSD_new(void);
  //! Materials defined in Materials.hh
  G4Material* FindMaterial(G4String mat);
private:
  // General solid volume
  G4VSolid* PSD_new_solid;
  // Sensitive volume
  G4LogicalVolume* PSD_SensitiveVolume;
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
  G4double Cyl_height;
  G4double Cyl_Oradius;
  G4double Cyl_Iradius;
  G4double Box_height;
  G4double Box_length;
  G4double Box_thick;
  G4double origin_z;
  G4double crystal_diameter;
  G4double crystal_height;
  G4int    npixels;
  G4double distanceToCrystal; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
