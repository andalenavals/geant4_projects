#ifndef Construct_log_vol_detectorGFNUN_h 
#define Construct_log_vol_detectorGFNUN_h 1

/*! @file Construct_log_vol_detectorGFNUN.hh
  @brief Defines mandatory user class Construct_log_vol_detectorGFNUN.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
 */

#include "globals.hh"
#include "G4LogicalVolume.hh"

#include "G4ThreeVector.hh" 

class G4VisAttributes;
class Materials;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class Construct_log_vol_detectorGFNUN : public G4LogicalVolume
{
public:
  //! Constructor
  Construct_log_vol_detectorGFNUN(G4String name, G4String type,
				  G4bool vis_flag=false,G4bool solid_flag=false,
				  G4bool Xray_flag=false);
  //! Destructor
  ~Construct_log_vol_detectorGFNUN();
public:
  inline  G4LogicalVolume* GetSensitiveVolume(void){return sensitiveVolume_log;};
  //! Materials defined in Materials.hh
  G4Material* FindMaterial(G4String mat);

private:

  //! Optical flag 
  G4bool optical_flag;
  //! Sensitive volume: Crystal or Photocathode
  G4LogicalVolume* sensitiveVolume_log;

  G4bool actual_geometry;
  G4bool optical_photons;
  
  /// Caracteristics length
  G4double Caracteristic_height;
  //! \name Materials
  //@{
  Materials* materials;
  //@}

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
