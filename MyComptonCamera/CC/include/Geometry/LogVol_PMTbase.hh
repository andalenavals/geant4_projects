#ifndef LogVol_PMTbase_h 
#define LogVol_PMTbase_h 1

/*! @file LogVol_PMTbase.hh
  @brief Defines mandatory user class LogVol_PMTbase.
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
class LogVol_PMTbase : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_PMTbase(G4String fname="PMTbase_log", G4double frad=5.8*cm/2.0, G4double flen=7.6*cm);
  //! Destructor
  ~LogVol_PMTbase();
public:
  inline void SetRadius(G4double val)             {Radius = val;};
  inline void SetLength(G4double val)             {Length = val;};
  inline void SetThickness(G4double val)          {Thickness = val;};
  inline void SetName(G4String pname)             {Name = pname;};
  
  inline G4double    GetRadius(void)     {return Radius;};
  inline G4double    GetLength(void)     {return Length;};
  inline G4double    GetThickness(void)  {return Thickness;};
private:
  void ConstructLogVol_PMTbase(void);
  void SetSolidVol_PMTbase(void);
private:
  // General solid volume
  G4VSolid* PMTbase_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double Radius;
  G4double Length;
  G4double Thickness;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
