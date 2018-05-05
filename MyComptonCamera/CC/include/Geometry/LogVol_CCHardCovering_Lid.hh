#ifndef LogVol_CCHardCovering_Lid_h 
#define LogVol_CCHardCovering_Lid_h 1

/*! @file LogVol_CCHardCovering_Lid.hh
  @brief Defines mandatory user class LogVol_CCHardCovering_Lid.
  @date September, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2.0
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
 */

#include "globals.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"

/*  units and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4Material;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_CCHardCovering_Lid : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_CCHardCovering_Lid(G4String fname="CCHardCovering_Lid_log");
  //! Destructor
  ~LogVol_CCHardCovering_Lid();
public:
  //inline void SetMaterial(G4Material* pMaterial)  {material = pMaterial;};
  inline void SetName(G4String pname)             {Name = pname;};

private:
  void ConstructSolidVol_CCHardCovering_Lid(void);
  void SetSolidVol_CCHardCovering_Lid(void);
private:
  // General solid volume
  G4VSolid* CCHardCovering_Lid_solid;
  //! Name
  G4String Name;
  //! Dimensions and material
  G4double Cyl_height;
  G4double Cyl_radius;
  G4double Box_height;
  G4double Box_length;
  G4double Box_width;
  G4double Lid_thickness;
  //G4Material* material;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
