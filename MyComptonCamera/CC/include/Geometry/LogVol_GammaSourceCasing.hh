#ifndef LogVol_GammaSourceCasing_h 
#define LogVol_GammaSourceCasing_h 1

/*! @file LogVol_GammaSourceCasing.hh
  @brief Defines mandatory user class LogVol_GammaSourceCasing.
  @date August, 2012
  @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)
  @version 1.9
  
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

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_GammaSourceCasing : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_GammaSourceCasing(G4String fname="GammaSourceCasing_log",G4double frad=1.0*cm, G4double fhei=0.3*cm);
  //! Destructor
  ~LogVol_GammaSourceCasing();
public:
  inline void SetRadius(G4double val)             {Radius = val;};
  inline void SetHeight(G4double val)             {Height = val;};
  inline void SetName(G4String pname)             {Name = pname;};
private:
  void ConstructLogVol_GammaSourceCasing(void);
  void SetSolidVol_GammaSourceCasing(void);
private:
  // General solid volume
  G4VSolid* GammaSourceCasing_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double Height;
  G4double Radius;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
