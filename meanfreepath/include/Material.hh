#ifndef Mate
#define Mate 1

#include "G4Material.hh"
#include "G4NistManager.hh"

#include <vector>


class Material{
private:
  std::vector <const char *> G4ElementName;
  G4NistManager * nistMan = G4NistManager::Instance();
  static Material* instance;

  G4Material * Wood;   G4Material * Asphalt;
  G4Material * Cement;
  G4Material * Lime;
  G4Material * Sand;
  G4Material * POP;
  
public:
  Material(void);
  ~Material(void);
  G4Material * GetMaterial(const G4String name);
  G4Material * GetMaterial(int Z);
  G4Material * CreateMaterials();  
  static Material* Instance();
};

#endif
