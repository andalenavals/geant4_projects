#ifndef PhysicalPropertiesOfMaterials
#define PhysicalPropertiesOfMaterials 1

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"

class PhysicalPropertiesOfMaterials{

public:
  PhysicalPropertiesOfMaterials(void);
  ~PhysicalPropertiesOfMaterials(void);
  G4double * GetMolarVolume(G4Material * );


private:


};

#endif
