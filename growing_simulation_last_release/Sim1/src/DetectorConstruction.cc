#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4DisplacedSolid.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();

  //ANDRESVISATT
  //G4VisAttributes* MundoVisAtt = new G4VisAttributes( G4VisAttributes::GetInvisible() );
  //G4VisAttributes* BarrilVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //BarrilVisAtt->SetForceWireframe(true);

  //PARAMETROS Detector
  /*
  G4double cris_alt = 5*cm, cris_dia = 5*cm; //altura, diametro
  G4Material* cris_mat  = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  */
  G4double cris_alt = 7.62*cm, cris_dia = 7.62*cm; //altura, diametro 3x3 in
  G4Material* cris_mat  = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  
  //PARAMETROS BARRIL
  G4double bar_alt = 50*cm, bar_dia = 40*cm; //altura, diametro
  G4double hueco1_alt = 24*cm, hueco1_dia=20*cm; //barrilcentral restado
  G4double hueco2_alt = 13*cm, hueco2_dia= 10*cm ; //barril inferior restado
  G4Material* barril_mat = nist->FindOrBuildMaterial("G4_Pb");
  
  // PARAMETROS DEL MUNDO
  G4double mundo_X = 100*cm;
  G4double mundo_Y = 100*cm;
  G4double mundo_Z  = 200*cm;
  G4Material* mundo_mat = nist->FindOrBuildMaterial("G4_AIR");

  //GEOMETRIA DEL MUNDO
  G4Box* MundoGeometria = new G4Box("Mundo", mundo_X, mundo_Y, mundo_Z);
  G4LogicalVolume* MundoLogico = new G4LogicalVolume(MundoGeometria, mundo_mat, "Mundo");
  //MundoLogico->SetVisAttributes(MundoVisAtt );
  G4VPhysicalVolume* MundoFisico = new G4PVPlacement(0, G4ThreeVector(0*cm,0*cm,0*cm), MundoLogico, "Mundo", 0, false, 0, true);       

  //GEOMETRIA DEL BARRIL
  /*
  G4Tubs* Barril = new G4Tubs("sol", 0*cm, 0.5*bar_dia, 0.5*bar_alt ,  0.*deg, 360.*deg );
  G4Tubs* hueco1 = new G4Tubs("h1", 0*cm, 0.5*hueco1_dia, 0.5*hueco1_alt ,  0.*deg, 360.*deg );
  G4Tubs* hueco2= new G4Tubs("h2", 0*cm, 0.5*hueco2_dia, 0.5*hueco2_alt ,  0.*deg, 360.*deg );
  //G4Tubs* hueco2aux = new G4Tubs("haux", 0*cm, 0.5*hueco2_dia, 0.5*hueco2_alt ,  0.*deg, 360.*deg );
  //G4DisplacedSolid * hueco2 = new G4DisplacedSolid("h2", hueco2aux, 0, G4ThreeVector(0*cm, 0*cm, 0.5*(bar_alt-hueco2_alt)));
  //G4UnionSolid* Hueco = new G4UnionSolid("hueco", hueco1, hueco2);
  //G4SubtractionSolid* BarrilGeometria = new G4SubtractionSolid("Barril", Barril, Hueco);
  G4SubtractionSolid* Barrilaux= new G4SubtractionSolid("Barrilaux", Barril, hueco2, 0, G4ThreeVector(0*cm, 0*cm, 0.5*(bar_alt-hueco2_alt)) );
  G4SubtractionSolid* BarrilGeometria= new G4SubtractionSolid("Barril", Barrilaux, hueco1);
  G4LogicalVolume* BarrilLogico = new G4LogicalVolume(BarrilGeometria, barril_mat, "Barril");
  BarrilLogico->SetVisAttributes(BarrilVisAtt);
  new G4PVPlacement(0, G4ThreeVector(0*cm, 0*cm ,0*cm), BarrilLogico, "Barril", MundoLogico, false, 0,true);
  */
  
  
  G4double zPlane[]={0             , hueco2_alt    , hueco2_alt     , hueco1_alt+hueco2_alt, hueco1_alt+hueco2_alt , bar_alt };
  G4double rInner[]={0.5*hueco2_dia, 0.5*hueco2_dia, 0.5*hueco1_dia , 0.5*hueco1_dia       , 0                     , 0};
  G4double rOuter[]={0.5*bar_dia   , 0.5*bar_dia   , 0.5*bar_dia    , 0.5*bar_dia          , 0.5*bar_dia           ,0.5*bar_dia };
  
  G4Polycone* BarrilGeometria = new G4Polycone("BarrilGeometria", 0.*deg, 360.*deg, 6 , zPlane, rInner, rOuter);
  G4LogicalVolume* BarrilLogico = new G4LogicalVolume(BarrilGeometria, barril_mat, "Barril");
  //BarrilLogico->SetVisAttributes(BarrilVisAtt);
  new G4PVPlacement(0, G4ThreeVector(0*cm, 0*cm ,0*cm), BarrilLogico, "Barril", MundoLogico, false, 0,true);
  


 
  
  
  //GEOMETRIA DEL DETECTOR
  G4Tubs* Cristal = new G4Tubs("Cristal", 0*cm, 0.5*cris_dia, 0.5*cris_alt ,  0.*deg, 360.*deg );
  G4LogicalVolume* CristalLogico = new G4LogicalVolume(Cristal, cris_mat, "Cristal");
  new G4PVPlacement(0, G4ThreeVector(0*cm, 0*cm ,25*cm), CristalLogico, "Cristal", MundoLogico, false, 0,true);
  fScoringVolume = CristalLogico; //contador

  return MundoFisico;
}

  //G4LogicalVolume(solido geometrico, material , nombre);
 // G4PVPlacement(rotation, Posicion, volumen logico, nombre, volumen madre, booleana(tener encuenta los overlaps o no) , numero de copia, overlap)
