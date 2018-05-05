#ifndef LogVol_Crystal_Cyl_h 
#define LogVol_Crystal_Cyl_h 1

/*! @file LogVol_Crystal_Cyl.hh
  @brief Defines mandatory user class LogVol_Crystal_Cyl.
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

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_Crystal_Cyl : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_Crystal_Cyl(G4String fname="Crystal_Cyl_log", G4double fIrad=2.805*cm, G4double fOrad=9.76*cm, G4double fheight=1.55*cm,G4Material* fMaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_CESIUM_IODIDE"));
  //! Destructor
  ~LogVol_Crystal_Cyl();
public:
  inline void SetOuterRadius(G4double val)        {ORadius = val;};
  inline void SetInnerRadius(G4double val)        {IRadius = val;};
  inline void SetHeight(G4double val)             {Height  = val;};
  //inline void SetID(G4int pID)                  {ID      = pID;};
  inline void SetName(G4String pname)             {Name    = pname;};
  
  inline G4double    GetOuterRadius(void)     {return ORadius;};
  inline G4double    GetInnerRadius(void)     {return IRadius;};
  inline G4double    GetHeight(void)          {return Height;};
  //inline G4int       GetID(void)            {return ID;};
private:
  void ConstructLogVol_Crystal_Cyl(void);
  void SetSolidVol_Crystal_Cyl(void);
private:
  // General solid volume
  G4VSolid* Crystal_Cyl_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double Height;
  G4double ORadius;
  G4double IRadius;
  // G4int ID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
