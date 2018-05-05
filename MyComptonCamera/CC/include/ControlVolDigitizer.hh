/**
 * @file   ControlVolDigitizer.hh
 *
 * @date   13 Oct 2015
 * @author DFlechas
 *
 * @brief Defines the digitization process
 */

#ifndef ControlVolDigitizer_h
#define ControlVolDigitizer_h 1

#include "ControlVolDigit.hh"
#include "NoiseGenerator.hh"
#include "MeV2ChargeConverter.hh"
#include "CrosstalkGenerator.hh"
//#include "ControlVolDigitizerMessenger.hh"

//#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4VDigitizerModule.hh"

/*!
 * \brief Simulation of the digitization process
 *
 * This class is responsible of the simulation of the ditis creation. i.e. it is responsible
 * to simulate the read-out of the detector.
 * Geant4 provides hits in the sensitive detector (i.e. energy depositions and position in
 * elements of the detector that have been declared sensitive),
 * this class is responsible to convert these hits in the electronic read-out format.
 * \sa ControlVolDigi
 * 
 * Digitization consists of the following steps:
 *  -# converting the energy deposit in charge
 *  -# simulate the strip cross talk
 *  -# for each strip add a pedestal
 *  -# smear the collected charge with electronic noise
 *  -# add cross talk
 * 
 * All relevant methods are virtual, you can inherit from
 * this base class to overwrite behaviour.
 * This classes uses two support classes to simulate noise and
 * convert energy deposit (in MeV) to the appropriate units.
 * \sa MeV2ChargeConverter and \sa NoiseGenerator
 */
class ControlVolDigitizer : public G4VDigitizerModule
{
public:
  /*! \brief constructor
   *
   * Creates a digitization module
   * @param aName The name of the module
   */
  ControlVolDigitizer(G4String aName);

  //! Empty destructor
  virtual ~ControlVolDigitizer() {};

  /*! \brief Perform digitization
   *
   * This method is declared pure virtual in the base class
   * and thus must be implemented
   */
  virtual void Digitize();
protected:
  //! \name simulate electronics
  //@{
  /*! \brief Simulate crosstalk between strips
   *
   * The charge collected by one strip "leaks" to the adjacent strips
   * Thus the charge collected by the strip that has been "hit"
   * is reduced and part of this goes to the adjacent strips
   * @param digitsMap : the digits collection digitsMap[ planeNumber ][ stripNumber ]
   * Important: crosstalk should be simulated before noise and bedestal
   * is added. \sa Digitize
   */
  virtual void MakeCrosstalk(std::vector< std::vector< ControlVolDigit* > >& digitsMap);
  //@}
public:
  //! \name some simple set & get functions
  //@{
  //TODO: Add setters for the other noise parameters?
  //TODO: make a messanger to set parameters?
  inline void     SetPedestal( const G4double& aValue )         { pedestal = aValue; }
  inline void	  SetNoise( const G4double& aValue )            { noise = NoiseGenerator(aValue); }
  inline void	  SetCrosstalk( const G4double& aValue )        { crosstalk = CrosstalkGenerator(aValue,48); }
  inline void	  SetConversionFactor( const G4double& aValue ) { convert = MeV2ChargeConverter(aValue); }
  inline void     SetCollectionName( const G4String& aName )    { digiCollectionName = aName; }
  inline void     SetHitsCollectionName( const G4String& aName ){ hitsCollName = aName; }
  void ResetValues(void);
  inline void     SetDetectorID( const G4int& detector_id)      { detectorID = detector_id; }
  inline void     SetGammaID( const G4int& gamma_id)            { gammaID = gamma_id; }
  inline void     SetCharge( const G4double& aCharge )          { charge = aCharge; }
  inline void     SetLocalTime(const G4double local_time)       { localTime = local_time; }
  inline void     SetGlobalTime(const G4double global_time)     { globalTime = global_time; }
  inline void     SetDetectorName(G4String name)                { detectorName = name; }
  inline void     SetTimeSignal(G4double ti)                    { timeSignal = ti; }
  inline void     SetFlagPhotoelectric(G4bool photo)            { photoelectricEff = photo; }
  inline void     SetFlagCompton(G4bool comp)                   { ComptonEff = comp; }
  inline void     SetPosition(G4ThreeVector xyz)                { PositionInt = xyz; }
  inline void     SetMomentum(G4ThreeVector pxyz)               { DeltaMomentum = pxyz; }
  //@}
private:
  //! Name of the digits collection
  G4String digiCollectionName;
  //! Name of the hits collection. Should match what has been used in \sa SensitiveDetector
  G4String hitsCollName;
  //! Pedestal level
  G4double pedestal;
  //! The object responsible to generate the electronic noise
  NoiseGenerator noise;
  //! The object that converts the energy deposit in collected charge
  MeV2ChargeConverter convert;
  //! The object that handles cross talk
  CrosstalkGenerator crosstalk;
  //! Messenger to implement some UI commands
  // ControlVolDigitizerMessenger messenger;

  //! Pointer to the Geometry
  //DetectorConstruction* Detector;
  //! Active elements in the Geometry

  // G4bool optical_flag;
  //
  /// Physical variables which will be stored by each detector
  G4double charge; //Total charge collected by the detector between t=0(first deposition) and timeSignal per detector
  G4double localTime; // Local time of first deposition per detector
  G4double globalTime; // Global time of first deposition per detector
  G4int    detectorID; // Detector ID:
  G4int    gammaID; // Gamma ID: 
  G4String detectorName;
  G4double timeSignal; // Time window (in ns) which defines the number of depositions that contribute to final signal per detector
  G4bool   photoelectricEff; //! flag if photoelectric effect 
  G4bool   ComptonEff; //! flag if Compton effect 
  G4ThreeVector PositionInt;//! Position where interaction occurs
  G4ThreeVector DeltaMomentum;//! //! Delta of Momentum during the interaction
};

#endif /* ControlVolDigitizer_h 1 */
