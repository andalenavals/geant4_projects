#ifndef SetupConstruction_h
#define SetupConstruction_h 1

/*! @file SetupConstruction.hh
  @brief Defines mandatory user class SetupConstruction.
  @date September, 2015
  @author  (D. Flechas dcflechasg@unal.edu.co)
  @version 1.0
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
*/

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Region;
class G4Material;
class SetupMessenger;
class Materials;

/*! @brief This mandatory user class defines the geometry.
  It is responsible for
  @li Construction of geometry

  \sa Construct()
*/
class SetupConstruction : public G4VUserDetectorConstruction
{
public:
  //! Constructor
  SetupConstruction();
  //! Destructor
  ~SetupConstruction();
public:
  //! Construct geometry of the setup
  G4VPhysicalVolume* Construct();

  //! Update geometry
  void UpdateGeometry();

private:
  void ConstructSetup(void);
  void ConstructSetup_TEST(void);
  
public:
  //! \name some simple set & get functions
  //@{
  //! Get the number of sensitive volumes 
  inline G4int    GetTotalDetectorNumber() const {return DetectorCounter;};
  //! Get the maximum number of sensitive volumes 
  inline G4int    GetMaximumNumberOfDetectors() const {return MaximumNumberOfDetectors;};
  //@}

private:
  //! \name Construction routines
  //@{

  //! Declare the volumes already constructed as 'sensitive'
  void Sensibilize();
  //! Add detectors to the DetectorCounter
  void Add2DetectorCounter(G4int added) { DetectorCounter+=added;};
  void ResetDetectorCounter() { DetectorCounter=0;};
  //! Our Materials List
  Materials* materials;
  
public:
  //// Plate 
  void SetPlate_Material(G4String mat)              {Plate_Material   = mat; };
  void SetPlate_thickness(G4double val)             {Plate_thickness  = val; };
  
  //! Materials defined in Materials.hh
  G4Material* FindMaterial(G4String mat);

  //!! Variables
private:
  // Plate 
  G4String      Plate_Material;
  G4double      Plate_thickness;
private:

  //! Defining a region for the detector messenger where special \e cuts can be defined
  G4Region* BSDdetectorRegion;
  G4Region* PSDdetectorRegion;
  G4Region* ControlVolRegion;
  //! string used for naming purposes
  char string_name[30];
  //! \name shared data between routines
  //!@{
  //! Physical Global mother volume
  G4VPhysicalVolume* physiWorld;
  //! Logical Global mother volume
  G4LogicalVolume* logicWorld;
  //! Maximum number of detectors than can be created: one per each detector type
  G4int  MaximumNumberOfDetectors;
  //! Counts the number of detectors defined
  G4int DetectorCounter;
  //! Construction flag 
  G4bool constructed;
  //!

  //! \name UI Messenger 
  //@{
  //! Setup messenger to change the geometry
  SetupMessenger * messenger;
  //@}
};

#endif // end SetupConstruction_h
