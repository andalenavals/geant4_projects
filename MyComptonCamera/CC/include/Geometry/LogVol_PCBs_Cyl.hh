#ifndef LogVol_PCBs_Cyl_h 
#define LogVol_PCBs_Cyl_h 1

/*! @file LogVol_PCBs_Cyl.hh
  @brief Defines mandatory user class LogVol_PCBs_Cyl.
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

class G4Material;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_PCBs_Cyl : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_PCBs_Cyl(G4String fname="PCBs_Cyl_log", G4double frad=1.0*cm, G4double flen=1.0*cm);
  //! Destructor
  ~LogVol_PCBs_Cyl();
public:
  inline void SetRadius(G4double val)             {Radius = val;};
  inline void SetLength(G4double val)             {Length = val;};
  inline void SetThickness(G4double val)          {Thickness = val;};
  inline void SetName(G4String pname)             {Name = pname;};
  
  inline G4double    GetRadius(void)     {return Radius;};
  inline G4double    GetLength(void)     {return Length;};
  inline G4double    GetThickness(void)  {return Thickness;};
private:
  void ConstructSolidVol_PCBs_Cyl(void);
  void SetSolidVol_PCBs_Cyl(void);
private:
  // General solid volume
  G4VSolid* PCBs_Cyl_solid;
  //! Name
  G4String Name;
  //! Dimensions and material
  G4double Radius;
  G4double Thickness;
  G4double Length;
  G4Material* Material; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
