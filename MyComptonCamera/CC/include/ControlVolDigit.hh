/**
 * @file   ControlVolDigit.hh
 *
 * @date   13 Oct 2015
 * @author D. Flechas
 *
 * @brief Defines a digit in the control volume
 */
#ifndef ControlVolDigit_h
#define ControlVolDigit_h 1
 

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/*! \brief Definition of a digit
 *
 * This class defines the structure of a digit.
 * A digit is the output of the detector as recorded by the R/O
 * A digit is defined by an identifier: the coutple
 * (plane number , strip number) and the measurement,
 * in this case the collected charge.
 * Hits are collected in a collection of hits: \sa ControlVolDigitsCollection
 */
class ControlVolDigit : public G4VDigi
{
public:
  //! constructor
  ControlVolDigit(const int& detector_id);
  //! Empty destructor
  virtual ~ControlVolDigit() {}
  /*!
   * \brief Add to the digit
   * Add a charge to the digit
   */
  inline void Add ( const G4double& aValue ) { charge+= aValue; if(charge<0) charge=0.; }

  /*!
   * \brief Validity of the Digit
   * Here we check the validity of the digit, in order to avoid noise
   * we will use a tan(E_gamma) filter. Check details inside the
   * routine.
   */
  bool Zero( const G4double& aCharge ,G4bool DoIt=false);
  /*!
   * \brief Print a digit
   *
   * Inherited method. Print some information on the
   * digit
   */
  void Print();
  /*!
   * \brief Draw a digit
   *
   * Inherited method, empty: do not draw digits
   */
  void Draw() {}
  //! \name some simple set & get functions
  //@{
  inline void     SetDetectorID( const G4int& detector_id)    { DetectorID = detector_id; }
  inline void     SetGammaID( const G4int& gamma_id)          { GammaID = gamma_id; }
  inline void     SetCharge( const G4double& aCharge )        { charge = aCharge; }
  inline void     SetLocalTime(const G4double local_time)     { localTime = local_time; }
  inline void     SetGlobalTime(const G4double global_time)   { globalTime = global_time; }
  inline void     SetPosition(G4ThreeVector xyz)              { Position = xyz; }
  inline void     SetMomentum(G4ThreeVector pxyz)             { Momentum = pxyz; }
  inline void     SetDetectorName(G4String name)              { DetectorName = name; }
  inline void     SetFlagPhotoelectric(G4bool photo)          { photoelectric = photo; }
  inline void     SetFlagCompton(G4bool fcomp)                { compton = fcomp; }
  inline void     SetEntries( const G4int& eNtries)           { entriesCompton = eNtries; }
  
  inline G4int    GetDetectorID( )       const { return DetectorID; }
  inline G4int    GetGammaID( )       const { return GammaID; }
  inline G4double GetCharge()            const { return charge; }
  inline G4double GetLocalTime()         const { return localTime; }
  inline G4double GetGlobalTime()        const { return globalTime; }
  inline G4String GetDetectorName()      const { return DetectorName;}
  inline G4bool   GetFlagPhotoelectric() const { return photoelectric; }
  inline G4bool   GetFlagCompton()       const { return compton; }
  inline G4ThreeVector GetPosition(void) const { return Position; }
  inline G4ThreeVector GetMomentum(void) const { return Momentum; }
  inline  G4int        GetEntries()      const { return entriesCompton; }
  //@}

  //! \name Memory management methods
  //@{
  //! Equality operator
  /*! 
   * Two digits are the same if they belong to the same detector
   * i.e. plane and Strip number
   * note that no check is done on the charge, since the logic is that
   * each strip can make a single measurement (the hit).
   */
  inline G4int operator==(const ControlVolDigit& aDigi) const
  { return ( DetectorID == aDigi.GetDetectorID() ); }
  //! The new operator
  /*! 
   * This operator creates efficiently a new hit.
   * Overwriting the default new operators allows for the use
   * of the G4Allocator functionalities.
   *
   * See 3.2.4 "General management classes" paragraph in Application Developer G4 manual
   *
   * \note The use of allocators is not mandatory but recommended (to improve performances).
   * You can safely skip this part.
   */
  inline void* operator new(size_t);
  //! Delete operator
  /*! \sa operator new() */
  inline void  operator delete(void* aDigi);
  //@}

private:
  //! Collected charge
  G4double charge;
  //! Detector ID
  G4int DetectorID;
  //! Gamma ID
  G4int GammaID;
  //! flag if hit is from interestin particle
  G4bool FollowParticle;
  //! Localtime from the Hit class 
  G4double localTime;
  //! Globaltime from the Hit class 
  G4double globalTime;
  //! DetectorName
  G4String DetectorName;
  //! flag if photoelectric effect 
  G4bool photoelectric;
  //! flag if Compton effect 
  G4bool compton;
  //! Position where interaction occurs
  G4ThreeVector Position;
  //! Delta of Momentum during the interaction
  G4ThreeVector Momentum;
  //! Number of Compton interactions
  G4int entriesCompton;
};

/*!
 * \brief A container of digitis
 */
typedef G4TDigiCollection<ControlVolDigit> ControlVolDigitsCollection;

/*!
 * \brief Allocator
 *
 * Creating this objects allows for an efficient use of memory.
 * Operators new and delete for the ControlVolDigi objects have to be
 * defined
 */
extern G4Allocator<ControlVolDigit> ControlVolDigitsAllocator;

//It's not very nice to have these two in .hh and not in .cc
//But if we move these to the correct place we receive a warning at compilation time
//Also these have to appear below the previous declaration of the allocator
//This should be cleaned somehow...
void* ControlVolDigit::operator new(size_t)
{
  return static_cast<void*>( ControlVolDigitsAllocator.MallocSingle() );
}

void ControlVolDigit::operator delete(void* aDigi)
{
  ControlVolDigitsAllocator.FreeSingle( static_cast<ControlVolDigit*>(aDigi) );
}

#endif /* ControlVolDigit_h 1 */
