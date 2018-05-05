#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

/*! @file DetectorConstruction.hh
  @brief Defines mandatory user class DetectorConstruction.
  @date September, 2010
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  @version 1.9
  
  In this header file, the 'physical' setup is defined: materials, geometries and positions. 
  This class defines the experimental hall used in the toolkit.
 */

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4ThreeVector.hh" 

class G4LogicalVolume;
class G4Region;
class DetectorMessenger;
class G4Material;
class Materials;
class Construct_log_vol_detectorGFNUN;
/*! @brief This mandatory user class defines the geometry.
It is responsible for
 @li Construction of geometry

\sa Construct()
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  //! Constructor
  DetectorConstruction();
  //! Destructor
  ~DetectorConstruction();
public:
  //! Construct geometry of the setup
  G4VPhysicalVolume* Construct();

  //! Update geometry
  void UpdateGeometry();

  void ConstructGe10Detector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructGe40Detector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructNaI2x2Detector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructNaI3x3Detector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructPlasticDetector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructCsIDetector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructDummyDetector(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);

  void ConstructCapsule(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructCapsule_252Cf(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructSetup_Transmission(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void Construct_CastleShielding(G4bool vis_flag,G4bool solid_flag,G4bool Xray_flag);
  void ConstructSetup_Flechas(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);
  void ConstructPlate(G4bool vis_flag=true,G4bool solid_flag=false,G4bool Xray_flag=true);

  //! \name some simple set & get functions
  //@{
  //! Get the number of sensitive volumes 
  inline G4int    GetTotalDetectorNumber() const {return DetectorCounter;};
  //! Get the maximum number of sensitive volumes 
  inline G4int    GetMaximumNumberOfDetectors() const {return MaximumNumberOfDetectors;};
  //@}

  inline G4bool   GetOptical_flag(void)      {return Optical_flag;};

private:
  //! \name Construction routines
  //@{

  //! Declare the volumes already constructed as 'sensitive'
  void Sensibilize();
  //! Add detectors to the DetectorCounter
  void Add2DetectorCounter(G4int added) { DetectorCounter+=added;};
  void ResetDetectorCounter() { DetectorCounter=0;};
  
public:
  void SetOptical_flag(G4bool flag)         {Optical_flag    = flag;};
    
  void SetGe10Detector_flag(G4bool flag)            { Ge10_flag      = flag;};
  void SetGe10DetectoPosition(G4ThreeVector pos)    {Ge10Position    = pos; };
  void SetGe10DetectorRotationX(G4double tilt)      {Ge10rotationX   = tilt;};
  void SetGe10DetectorRotationY(G4double tilt)      {Ge10rotationY   = tilt;};

  void SetGe40Detector_flag(G4bool flag)            { Ge40_flag      = flag;};
  void SetGe40DetectoPosition(G4ThreeVector pos)    {Ge40Position    = pos; };  
  void SetGe40DetectorRotationX(G4double tilt)      {Ge40rotationX   = tilt;};
  void SetGe40DetectorRotationY(G4double tilt)      {Ge40rotationY   = tilt;};

  void SetNaI2x2Detector_flag(G4bool flag)          { NaI2x2_flag    = flag;};
  void SetNaI2x2DetectoPosition(G4ThreeVector pos)  {NaI2x2Position  = pos; };
  void SetNaI2x2DetectorRotationX(G4double tilt)    {NaI2x2rotationX   = tilt;};
  void SetNaI2x2DetectorRotationY(G4double tilt)    {NaI2x2rotationY   = tilt;};

  void SetNaI3x3Detector_flag(G4bool flag)          { NaI3x3_flag    = flag;};
  void SetNaI3x3DetectoPosition(G4ThreeVector pos)  {NaI3x3Position  = pos; };
  void SetNaI3x3DetectorRotationX(G4double tilt)    {NaI3x3rotationX   = tilt;};
  void SetNaI3x3DetectorRotationY(G4double tilt)    {NaI3x3rotationY   = tilt;};

  void SetPlasticDetector_flag(G4bool flag)           { Plastic_flag     = flag;};
  void SetPlasticDetectoPosition(G4ThreeVector pos)   {PlasticPosition   = pos; };
  void SetPlasticDetectorRotationX(G4double tilt)     {PlasticrotationX   = tilt;};
  void SetPlasticDetectorRotationY(G4double tilt)     {PlasticrotationY   = tilt;};

  void SetCsIDetector_flag(G4bool flag)           { CsI_flag     = flag;};
  void SetCsIDetectoPosition(G4ThreeVector pos)   {CsIPosition   = pos; };
  void SetCsIDetectorRotationX(G4double tilt)     {CsIrotationX   = tilt;};
  void SetCsIDetectorRotationY(G4double tilt)     {CsIrotationY   = tilt;};

  void SetDummyDetector_flag(G4bool flag)           { Dummy_flag     = flag;};
  void SetDummyDetectoPosition(G4ThreeVector pos)   {DummyPosition   = pos; };
  void SetDummyDetectorRotationX(G4double tilt)     {DummyrotationX   = tilt;};
  void SetDummyDetectorRotationY(G4double tilt)     {DummyrotationY   = tilt;};

  void SetSourceCapsulePosition(G4ThreeVector pos)  {SourceCapsulePosition   = pos; };
  void SetSourceCapsuleRotationX(G4double tilt)     {SourceCapsulerotationX   = tilt;};
  void SetSourceCapsuleRotationY(G4double tilt)     {SourceCapsulerotationY   = tilt;};

  void SetCastleShieldingPosition(G4ThreeVector pos)  {CastleShieldingPosition   = pos; };
  void SetCastleShielding_LeadThickness(G4double val)  {CastleShielding_LeadThickness = val;};
  void SetCastleShielding_LeadRoofThickness(G4double val) {CastleShielding_LeadRoofThickness = val;};
  void SetCastleShielding_ParaffinRadius(G4double val)  {CastleShielding_ParaffinRadius = val;};
  void SetCastleShielding_Height(G4double val)  {CastleShielding_Height = val;};
  
  void SetPlate_Material(G4String mat)              {Plate_Material   = mat; };
  void SetPlate_thickness(G4double val)             {Plate_thickness  = val; };

  G4Material* FindMaterial(G4String mat);

  G4bool        Pimp_flag;
  G4bool        Optical_flag;

  G4String      Plate_Material;
  G4double      Plate_thickness;

private:
  /// detectors position
  G4ThreeVector Ge10Position;
  G4ThreeVector Ge40Position;
  G4ThreeVector NaI2x2Position;
  G4ThreeVector NaI3x3Position;
  G4ThreeVector DummyPosition;
  G4ThreeVector PlasticPosition;
  G4ThreeVector CsIPosition;
  /// detectors orientation
  G4double      Ge10rotationX;
  G4double      Ge10rotationY;
  G4double      Ge40rotationX;
  G4double      Ge40rotationY;
  G4double      NaI2x2rotationX;
  G4double      NaI2x2rotationY;
  G4double      NaI3x3rotationX;
  G4double      NaI3x3rotationY;
  G4double      DummyrotationX;
  G4double      DummyrotationY;
  G4double      PlasticrotationX;
  G4double      PlasticrotationY;
  G4double      CsIrotationX;
  G4double      CsIrotationY;
  // Construct yes or not the detectors
  G4bool        Dummy_flag;
  G4bool        Ge10_flag;
  G4bool        Ge40_flag;
  G4bool        NaI2x2_flag;
  G4bool        NaI3x3_flag;
  G4bool        Plastic_flag;
  G4bool        CsI_flag;
  /// Source capsule
  G4ThreeVector SourceCapsulePosition;
  G4double      SourceCapsulerotationX;
  G4double      SourceCapsulerotationY;
  /// Castle shielding: paraffin + lead (Shielding of 252Cf source)
  G4ThreeVector CastleShieldingPosition;
  G4double      CastleShielding_LeadThickness;
  G4double      CastleShielding_LeadRoofThickness;
  G4double      CastleShielding_ParaffinRadius;
  G4double      CastleShielding_Height;

private:

  //! Defining a region for the detector messenger where special \e cuts can be defined
  G4Region* detectorRegion;

  //! Flag of already constructed geometry used in case of UpdateGeometry method invocation
  
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
  
  //// Detectors
  Construct_log_vol_detectorGFNUN* Detector_Ge10_log;
  Construct_log_vol_detectorGFNUN* Detector_Ge40_log;
  Construct_log_vol_detectorGFNUN* Detector_NaI_2x2_log;
  Construct_log_vol_detectorGFNUN* Detector_NaI_3x3_log;
  Construct_log_vol_detectorGFNUN* Detector_Plastic_log;
  Construct_log_vol_detectorGFNUN* Detector_CsI_log;
  Construct_log_vol_detectorGFNUN* Detector_dummy_log;
  //// sensitive volumes Detectors
  G4LogicalVolume* Detector_Ge10_sensitiveVol_log;
  G4LogicalVolume* Detector_Ge40_sensitiveVol_log;
  G4LogicalVolume* Detector_NaI_2x2_sensitiveVol_log;
  G4LogicalVolume* Detector_NaI_3x3_sensitiveVol_log;
  G4LogicalVolume* Detector_Plastic_sensitiveVol_log;
  G4LogicalVolume* Detector_CsI_sensitiveVol_log;
  G4LogicalVolume* Detector_dummy_sensitiveVol_log;


  G4bool actual_geometry;
  G4double      soil_length;
  
  /// Caracteristics length
  G4double Caracteristic_height;
  //! \name Materials
  //@{
  G4Material* air;
  Materials* materials;
  //@}


  //! \name UI Messenger 
  //@{
  //! Detector messenger to change the geometry
  DetectorMessenger * messenger;
  //@}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
