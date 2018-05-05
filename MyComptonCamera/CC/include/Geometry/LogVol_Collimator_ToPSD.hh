#ifndef LogVol_Collimator_ToPSD_h 
#define LogVol_Collimator_ToPSD_h 1

/*! @file LogVol_Collimator_ToPSD.hh
  @brief Defines mandatory user class LogVol_Collimator_ToPSD.
  @date August, 2015
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
#include "G4NistManager.hh"

class G4VisAttributes;
class G4Material;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_Collimator_ToPSD : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_Collimator_ToPSD(G4String fname="Collimator_ToPSD_log",G4double frad=37.50/2.0*mm,G4double fheight=4.56*mm,G4double fang=32.0*deg,G4Material* fmaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_W"));
  //! Destructor
  ~LogVol_Collimator_ToPSD();

  inline void SetName(G4String pname)             {Name      = pname;};
  inline void SetRadius(G4double prad)            {MaxRadius = prad;};
  inline void SetHeight(G4double pheigth)         {Height    = pheigth;};
  inline void SetAngOpening(G4double pangop)      {AngOp     = pangop;};
private:
  void ConstructSolidVol_Collimator_ToPSD(void);
  void SetSolidVol_Collimator_ToPSD(void);
private:
  // General solid volume
  G4VSolid* Collimator_ToPSD_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double AngOp;
  G4double Height;
  G4double MaxRadius;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
