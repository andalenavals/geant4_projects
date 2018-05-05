#include "PhysicalPropertiesOfMaterials.hh"
#include "G4PhysicalConstants.hh"

 G4double *    PhysicalPropertiesOfMaterials::GetMolarVolume(G4Material * mat )
 {
   return Avogadro/mat->GetTotNbOfAtomsPerVolume();
 }
