#ifndef LogVol_ShieldingPSDCone_h 
#define LogVol_ShieldingPSDCone_h 1

/*! @file LogVol_ShieldingPSDCone.hh
  @brief Defines mandatory user class LogVol_ShieldingPSDCone.
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
class LogVol_ShieldingPSDCone : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_ShieldingPSDCone(G4String fname="ShieldingPSDCone_log",G4double fIrad=5.56/2.0*mm,G4double fOrad=33.40/2.0*mm,G4double fheight=29.76*mm,G4double fang=34.9*deg,G4Material* fmaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"));
  //! Destructor
  ~LogVol_ShieldingPSDCone();

  inline void SetName(G4String pname)             {Name        = pname;};
  inline void SetOuterRadius(G4double prad)       {OuterRadius = prad;};
  inline void SetInnerRadius(G4double prad)       {InnerRadius = prad;};
  inline void SetHeight(G4double pheigth)         {Height      = pheigth;};
  inline void SetAngOpening(G4double pangop)      {AngOp       = pangop;};
private:
  void ConstructSolidVol_ShieldingPSDCone(void);
  void SetSolidVol_ShieldingPSDCone(void);
private:
  // General solid volume
  G4VSolid* ShieldingPSDCone_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double AngOp;
  G4double Height;
  G4double OuterRadius;
  G4double InnerRadius;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
