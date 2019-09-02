#ifndef SteppingAction_h
#define SteppingAction_h 1

/*! @file SteppingAction.hh
  @brief Headers for the class SteppingAction.
  @date December, 2010
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  
  The SteppingAction is one of the 'optional' User Actions. It has
  been implemented to help the measuring times from the TRAPSpec
  experiment but is a general purpose class that comes VERY handy to
  control the tracks.
 */

#include "G4UserSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class Interactions;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  inline   void SetVerbose_flag( const G4bool flag ) { print_flag = flag; }
  inline G4bool GetVerbose_flag() const {return print_flag;};

  inline   void SetInteraction_flag( const G4bool flag ) { interaction_flag = flag;};
  inline G4bool GetInteraction_flag(void) const { return interaction_flag;};
  inline G4int GetNumberOfInterstingGammaRays(void) {return NumberOfInterstingGammaRays;};
  inline   void ResetGammaAnnihilationCounter(void) { GammaAnnihilationCounter=0; };
  inline Interactions* GetRegister_Int(void) {return Register_Int;};

private:
  const G4int NumberOfInterstingGammaRays;
  Interactions* Register_Int; 
  G4int GammaAnnihilationCounter;
  G4bool interaction_flag;//Defines if the Compton interactions and flies time in the interest volumes
                          // (in the Soil anr/or burried object or plates) will be registred.
  G4double iso_time;
  G4bool print_flag;
};

#endif
