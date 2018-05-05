#ifndef LogVol_ShieldingBSD_DFproposal_h 
#define LogVol_ShieldingBSD_DFproposal_h 1

/*! @file LogVol_ShieldingBSD_DFproposal.hh
  @brief Defines mandatory user class LogVol_ShieldingBSD_DFproposal.
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
class LogVol_ShieldingBSD_DFproposal : public G4LogicalVolume
{
public:
  //! Constructor
  LogVol_ShieldingBSD_DFproposal(G4String fname="ShieldingBSD_DFproposal_log",G4double fIrad=56.0/2.0*mm,G4double fOrad=100.0*mm,G4double fheight=5.0*mm,G4Material* fmaterial=(G4NistManager::Instance())->FindOrBuildMaterial("G4_Pb"));
  //! Destructor
  ~LogVol_ShieldingBSD_DFproposal();

  inline void SetName(G4String pname)             {Name        = pname;};
  inline void SetOuterRadius(G4double prad)       {OuterRadius = prad;};
  inline void SetInnerRadius(G4double prad)       {InnerRadius = prad;};
  inline void SetHeight(G4double pheigth)         {Height      = pheigth;};
private:
  void ConstructSolidVol_ShieldingBSD_DFproposal(void);
  void SetSolidVol_ShieldingBSD_DFproposal(void);
private:
  // General solid volume
  G4VSolid* ShieldingBSD_DFproposal_solid;
  //! Name
  G4String Name;
  // Dimensions
  G4double Height;
  G4double OuterRadius;
  G4double InnerRadius;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
