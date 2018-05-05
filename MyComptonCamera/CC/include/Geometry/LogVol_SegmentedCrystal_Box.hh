#ifndef LogVol_SegmentedCrystal_Box_h 
#define LogVol_SegmentedCrystal_Box_h 1

/*! @file LogVol_SegmentedCrystal_Box.hh
  @brief Defines mandatory user class LogVol_SegmentedCrystal_Box.
  @date September, 2015
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 2.0
  
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

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_SegmentedCrystal_Box : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_SegmentedCrystal_Box(G4String fname="SegmentedCrystal_Box_phys",
			      G4double flen=6.40*cm*std::sin(pi/4.),
			      G4double fheight=0.7*cm,
			      G4int fNpixelX=64, G4int fNpixelY=64 ,
			      G4Material* fMaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"));
  //! Destructor
  ~LogVol_SegmentedCrystal_Box();
public:
  inline void SetLength(G4double val)          {Length    = val;};
  inline void SetHeight(G4double val)          {Height    = val;};
  inline void SetXPixels(G4int val)            {XPixelNum = val;};
  inline void SetYPixels(G4int val)            {YPixelNum = val;};
  
  //inline void SetID(G4int pID)                    {ID = pID;};
  inline void SetName(G4String pname)             {Name = pname;};

  inline G4LogicalVolume* GetSegmentedCrystal_Box(void) {return SegmentedCrystal_Box_LogVol;};
private:
  void ConstructLogVol_SegmentedCrystal_Box(void);
private:
  // General solid volume
  G4LogicalVolume* SegmentedCrystal_Box_LogVol;
  //! Material
  G4Material* Material;
  //! Name
  G4String Name;
  // Dimensions
  G4double Length;
  G4double Height;
  G4int    XPixelNum;
  G4int    YPixelNum;
  // G4int ID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
