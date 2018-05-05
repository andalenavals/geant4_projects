#ifndef LogVol_PCBs_Box_h 
#define LogVol_PCBs_Box_h 1

/*! @file LogVol_PCBs_Box.hh
  @brief Defines mandatory user class LogVol_PCBs_Box.
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
class LogVol_PCBs_Box : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_PCBs_Box(G4String fname="PCBs_Box_log", G4double fwidth=1.0*cm, G4double flen=1.0*cm, G4double fthick = 0.15*cm);
  //! Destructor
  ~LogVol_PCBs_Box();
public:
  inline void SetWidth(G4double val)             {Width = val;};
  inline void SetLength(G4double val)             {Length = val;};
  inline void SetThickness(G4double val)          {Thickness = val;};
  inline void SetName(G4String pname)             {Name = pname;};
  
  inline G4double    GetWidth(void)     {return Width;};
  inline G4double    GetLength(void)     {return Length;};
  inline G4double    GetThickness(void)  {return Thickness;};
private:
  void ConstructSolidVol_PCBs_Box(void);
  void SetSolidVol_PCBs_Box(void);
private:
  // General solid volume
  G4VSolid* PCBs_Box_solid;
  //! Name
  G4String Name;
  //! Dimensions and material
  G4double Width;
  G4double Thickness;
  G4double Length;
  G4Material* Material; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
