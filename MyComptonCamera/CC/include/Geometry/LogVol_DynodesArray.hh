#ifndef LogVol_DynodesArray_h 
#define LogVol_DynodesArray_h 1

/*! @file LogVol_DynodesArray.hh
  @brief Defines mandatory user class LogVol_DynodesArray.
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

class G4Material;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_DynodesArray : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_DynodesArray(G4String fname="DynodesArray_log", G4double frad=1.0*cm, G4double flen=1.0*cm, G4int fnum=1);
  //! Destructor
  ~LogVol_DynodesArray();
public:
  inline void SetHeight(G4double val)             {Height = val;};
  inline void SetLength(G4double val)             {Length = val;};
  inline void SetThickness(G4double val)          {Thickness = val;};
  inline void SetNumOfDynodes(G4int val)          {NumberOfDynodes = val;};
  inline void SetName(G4String pname)             {Name = pname;};
 
  inline G4double    GetHeight(void)     {return Height;};
  inline G4double    GetLength(void)     {return Length;};
  inline G4double    GetThickness(void)  {return Thickness;};
  inline G4int       GetNumOfDynodes(void) {return NumberOfDynodes;};
  inline G4Material* GetMaterial(void)   {return Material;};
private:
  void ConstructSolidVol_DynodesArray(void);
  void SetSolidVol_DynodesArray(void);
  void SetMaterial_DynodesArray(void);
private:
  // General solid volume
  G4VSolid* DynodesArray_solid;
  //! Name
  G4String Name;
  //! Dimensions and material
  G4int    NumberOfDynodes;
  G4double Height;
  G4double Thickness;
  G4double Length;
  G4Material* Material;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
