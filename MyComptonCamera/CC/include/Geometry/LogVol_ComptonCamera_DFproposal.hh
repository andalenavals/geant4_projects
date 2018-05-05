#ifndef LogVol_ComptonCamera_DFproposal_h 
#define LogVol_ComptonCamera_DFproposal_h 1

/*! @file LogVol_ComptonCamera_DFproposal.hh
  @brief Defines mandatory user class LogVol_ComptonCamera_DFproposal.
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
class Materials;

/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class LogVol_ComptonCamera_DFproposal : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_ComptonCamera_DFproposal(G4String fname="ComptonCamera_DFproposal_log",G4bool fOpflag=false,G4bool faddons=true, G4double fPSdpos=3.34*cm);
  //! Destructor
  ~LogVol_ComptonCamera_DFproposal();
public:
  inline void SetName(G4String pname)         {Name        = pname;};
  inline void SetOpflag(G4bool pflag)         {Opflag      = pflag;};
  inline void SetAddonsflag(G4bool pflag)     {Addonsflag  = pflag;};
  inline void SetPSDposition(G4double pos)    {PSD_pos     = pos;};
private:
  void ConstructLogVol_ComptonCamera_DFproposal(void);
  void ConstructLogVol_MainGeometry(void);
  void ConstructLogVol_Addons(void);
  void SetSolidVol_ComptonCamera_DFproposal(void);
  //! Materials defined in Materials.hh
  G4Material* FindMaterial(G4String mat);
private:
  // General solid volume
  G4VSolid* ComptonCamera_DFproposal_solid;
  // Sensitive volume
  //! Name
  G4String Name;
  //CrystalMaterial
  G4Material* Material;
  Materials*  materials;
  //Optical photons
  G4bool Opflag;
  // Addons
  G4bool Addonsflag;
  // Dimensions;
  G4double base_thick;
  G4double sourcedis;
  G4double PSD_pos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
