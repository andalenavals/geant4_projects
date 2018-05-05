#ifndef CrossSection
#define CrossSectio 1

#include "G4PEEffectFluoModel.hh" //Fotoelectrico
#include "G4KleinNishinaCompton.hh" //Compton
#include "G4BetheHeitlerModel.hh" //Conversion interna
#include "G4DataVector.hh"
#include "G4Gamma.hh"

class CrossSection{
private:
  G4ParticleDefinition* particle = G4Gamma::Gamma();
  //G4ParticleDefinition* particle;
  G4DataVector cuts;
  G4VEmModel* phot = new G4PEEffectFluoModel();
  G4VEmModel* comp = new G4KleinNishinaCompton();
  G4VEmModel* conv = new G4BetheHeitlerModel();
  static CrossSection* instance;
public:
  CrossSection(void);
  ~CrossSection(void);
  G4VEmModel * GetPhotModel( void ){return phot;};
  G4VEmModel * GetCompModel( void ){return comp;};
  G4VEmModel * GetConvmodel( void ){return conv;};
  G4ParticleDefinition*  GetParticle(void){return particle;};
  G4DataVector GetCuts(void){return cuts;};
  G4double ComputeTotalCrossSectionPerAtom(G4double  E,  G4double Z);
  G4double ComputeTotalMeanFreePath(G4double  E,  G4Material * Ma);
  void Initialise();
  static CrossSection* Instance();
};

#endif
