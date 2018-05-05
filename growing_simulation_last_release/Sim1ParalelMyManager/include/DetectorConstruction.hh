/*
Al detector construction le entra las librerias de G4RunManager, para
que?  Importante con este metodo se crean las geometrias del
experimento. Nota Importante: toda la informacion de interes esta en
el volumen logico de almacenamiento, el fScorigVolume, que en este
ejemplo es el cristal del detector.
 */

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  protected:
    G4LogicalVolume*  fScoringVolume;
};

#endif

