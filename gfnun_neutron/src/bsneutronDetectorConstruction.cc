//This simulation is mine since 01/09/2010.  Zandra
//I can make like it
#include "bsneutronDetectorConstruction.hh"
#include "DetectorMessenger.hh"

//Mi detector sensible!!
#include "MySensitiveDetector.hh"
//My sensitive DLM2
#include "SensitiveDLM2.hh"

//El controlador del sensitive detector
//#include "G4SDStructure.hh"
#include "G4SDManager.hh"

#include "G4UnitsTable.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//Librerias para el sensitive detector 
//Primero la clase concreta del  sensitive detector
#include "G4MultiFunctionalDetector.hh"

//Despues un primitive scorer para contar corriente
#include "G4PSFlatSurfaceCurrent.hh"

#include "G4SDParticleWithEnergyFilter.hh"

//El controlador del sensitive detector
//#include "G4SDStructure.hh"
#include "G4SDManager.hh"
//#include "G4SDmessenger.hh"
#include "G4NistManager.hh"

//Librerias para limpiar la geometria!! si se cambian los parametros geometricos
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4RunManager.hh"

#include "G4RotationMatrix.hh"

//May 28-2009
#include "G4UserLimits.hh"

bsneutronDetectorConstruction::bsneutronDetectorConstruction()
 :  experimentalHall_log(0), soilBlock_log(0),
    WoodBox_log(0),  AirBox_log(0),
   WetSand_log(0), detector_log(0),  
    ArrayA_log(0), ArrayB_log(0),
    detector1Block_log(0), detector2Block_log(0), 
    detector3Block_log(0), detector4Block_log(0), 
    detector5Block_log(0), detector6Block_log(0), 
    detector7Block_log(0), detector8Block_log(0), 
    plasticbox_log(0),
    mine_log(0),
    experimentalHall_phys(0), soilBlock_phys(0),
    WoodBox_phys(0),  AirBox_phys(0),
    WetSand_phys(0), detector_phys(0),
    detector1Block_phys(0),  detector2Block_phys(0), 
    detector3Block_phys(0),  detector4Block_phys(0), 
    detector5Block_phys(0),  detector6Block_phys(0), 
    detector7Block_phys(0),  detector8Block_phys(0), 
    plasticbox_phys(0),
    mine_phys(0),
    defaultMaterial(0),
    Worldmaterial(0), Soilmaterial(0), 
    Minematerial(0), Plasticboxmaterial(0),
    Detectormaterial(0)


{
  DefineMaterials(); //links with the pointers of the materials
  detectorMessenger = new DetectorMessenger(this);


  //*********ESTAS SON LAS POSICIONES Y TAMAÑOS POR DEFECTO DE LAS COMPONENTES DE LA GEOMETRIA****************  
  //Ancho de la cajita madre de los detectores
  detector_x = 30.0*cm ;//11.75*cm; //CON 11.75 caben pegaditos los detectores
  detector_y = 30.0*cm; //Estaba 11cm lo cambie a 30 para poner los arreglos en y
  detector_z = 1.27*cm; //(radio un tubo de 3He)//
  
  //Posicion de la cajita madre de los detectores (respecto a la caja de aire arriba de la caja de arena)
  detectorPos_x = 0.0*cm;
  detectorPos_y = 0.0*cm;
  detectorPos_z = -20.73*cm;//Julio 29-2008 Nueva posicion pa las nuevas dimensiones (con respecto a la caja de aire).
                           // detectorPos_z=z-23.73, donde z es la altura de los detectores respecto al suelo
                           //a -20.73 queda a 3cm del suelo.
  
  // detectorPos_z = 1.27*cm;//Julio 29-2008 Nueva posicion pa las nuevas dimensiones (con respecto a la caja de madera).  
  //a 1.27 queda a 3cm del suelo  

  //Julio 29-2008 LO que sigue era lo que tenia antes de crear la caja de madera
  //(28.75 cm  queda a 1 cm del suelo desde el extremo inferior, tal cual Brooks)) 
  // 30.75 queda a 3 cm del suelo desde el extremo inferior
  //detectorPos_z=48.0*cm;//**********( a 13 cm del suelo)
  
  //*******************************************************************
  //Mayo 23-2008.. Size of the boxes of the arrays
  ArrayA_x= 5.08*cm; //Size of 4 detectors "pegaditos"/4 
  ArrayA_y= 11.0*cm; 
  ArrayA_z= 1.27*cm; //radio de un tubo de 3He 
  //Mayo 23-2008.. Posicion de las cajitas array con respecto a la cajita madre
  //8.54 para que las cajitas queden separadas 6 cm

  //There are 4 array
  // A and B to locate the mine in axe X
  // B and C to locate the mine in axe Y
  //To follow are the positions of these arrays
  ArrayAPos_x= 8.54*cm;
  ArrayAPos_y= 0*cm;
  ArrayAPos_z= 0*cm;
  
  ArrayBPos_x= -8.54*cm;
  ArrayBPos_y= 0*cm;
  ArrayBPos_z= 0*cm;

  
  
  //Posicion del detector 1 (con respoecto al centro de la cajita madre)
  detector1Pos_x = 3.81*cm;//3 radios. con respecto a la cajita A Array
  detector1Pos_y =  0.0*cm;
  detector1Pos_z =  0.0*cm;

  //Posicion del detector 8 (con respoecto al centro de la cajita madre)
  detector8Pos_x = -3.81*cm;
  detector8Pos_y = 0.0*cm;
  detector8Pos_z = 0.0*cm;




  //Posicion del detector 1 (con respoecto al centro de la cajita madre)

  detector4Pos_x = -3.81*cm;//3 radios. con respecto a la cajita A Array
  detector4Pos_y =  0.0*cm;
  detector4Pos_z =  0.0*cm;

  //Posicion del detector 8 (con respoecto al centro de la cajita madre)

  detector5Pos_x = 3.81*cm;
  detector5Pos_y = 0.0*cm;
  detector5Pos_z = 0.0*cm;


  
  //Posicion del target de polietileno. ¿Esto que es?
  plasticboxPos_x = 0.*m;
  plasticboxPos_y = 0.*m;
  plasticboxPos_z = 38*cm;//CON 38CM QUEDA A NIVEL DEL SUELO

  //Posición de la mina 
  minePos_x=0;
  minePos_y=0*cm;
  minePos_z=0.8*cm; //z=w/2-d-1.7, 0.8 es para w=5 y d=0.
  //April 21-2009 0.8 is d=0 cm by default, with Wet_Sand_width=5cm
  //Julio 29-2008. Nueva posición de la mina respecto a la caja de madera (ver bitacora).
  //minePos_z=0.8*cm;//19.3*cm; //z=21-d-1.7=19.3-d(cm)
  
  //Julio 29-2008 . Si es hija de la capa de arena mojada.


  //minePos_z=19.8*cm;
  //24.8 queda enterrada al nivel del suelo por debajo!!!!
  //Marzo 23 2008
  //si la quiero enterrada a una distancia d desde el extremo superior (como la distancia experimental) 
  //entonces minePos_z=24.8-d
  //19.8 queda enterrada a 5 cm
  


  
  //March 28-2009...width of the wet sand layer....
  Wet_Sand_width=5*cm;
  wetPos_x = 0.0*cm;
  wetPos_y = 0.0*cm;
  wetPos_z = 21*cm-Wet_Sand_width/2; 


  //May 28-2009
  //To include UserLimits and kill protons in the wet sand layer!!
  theUserLimitsForSoil     = 0;
  theMaxStepSize      = DBL_MAX;
  theMaxTimeCuts      = DBL_MAX;
  theMinEkine         = 5.0*MeV; // minimum kinetic energy required in volume

}                                       


bsneutronDetectorConstruction::~bsneutronDetectorConstruction()
{
  G4cout << "oooOOOooo Aqui termina bsneutronDetectorConstruction.cc oooOOOooo"<<G4endl;
  delete detectorMessenger;
}

void bsneutronDetectorConstruction::DefineMaterials()
{
  
  //Mensaje para mirar donde empieza
  
  G4cout << "oooOOOooo Aqui empieza bsneutronDetectorConstruction.cc oooOOOooo"<<G4endl;
  
  G4String symbol;             //a=mass of a mole;
  G4double a, density;      //z=mean number of protons;  
  G4double z;
  G4int z2;
  
  //  G4int iz, n;                 //iz=number of protons  in an isotope; 
  // n=number of nucleons in an isotope;
  
  G4int ncomponents, natoms,n;
  G4double abundance;
  G4double temperature, pressure, fractionmass;
  //Soil, mine and plastic box material definition (name, symbol, z, a 
  
  G4Element* H  = new G4Element("Hydrogen",symbol="H",z=1.,a=1.00794*g/mole);
  //  G4Element* He = new G4Element("Helium",symbol="He",z=2.,a=4.002602*g/mole);
  
  //Isótopo de helio 3!!!!!!!!!!!!
  G4Isotope* he3 = new G4Isotope("he3", z2=2, n=3, a=3.01603*g/mole);
  G4Element* He3 = new G4Element("He3", symbol="He3", ncomponents=1);
  He3->AddIsotope(he3, abundance=100.*perCent);
  //isotopo de deuterio
  G4Isotope* d = new G4Isotope("d", z2=1, n=2, a=2.01*g/mole);
  G4Element* D = new G4Element("D", symbol="D", ncomponents=1);
  D->AddIsotope(d, abundance=100.*perCent);
  
  G4Element* C  = new G4Element("Carbon",symbol="C",z=6.,a=12.011*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N",z=7.,a=14.007*g/mole);
  G4Element* O  = new G4Element("Oxigen",symbol="O", z=8., a=15.9994*g/mole);
  G4Element* Na = new G4Element("Sodium",symbol="Na", z=11., a=22.9897*g/mole);
  G4Element* Mg = new G4Element("Magnesium",symbol="Mg", z=12., a=24.305*g/mole);
  G4Element* Al = new G4Element("Aluminium",symbol="O", z=13., a=26.9815*g/mole);
  G4Element* Si = new G4Element("Silicon",symbol="Si", z=14., a=28.0855*g/mole);
  G4Element* Ar = new G4Element("Argon",symbol="Ar",z=18.,a=39.948*g/mole);
  G4Element* K  = new G4Element("Potasium",symbol="K", z=19., a=39.0983*g/mole);
  G4Element* Ca = new G4Element("Calcium",symbol="Ca", z=20., a=40.078*g/mole);
  G4Element* Fe = new G4Element("Iron",symbol="Fe", z=26., a=55.865*g/mole);
  G4Element* Ti = new G4Element("Titanium",symbol="Ti", z=22., a=47.867*g/mole);  
  
  //****************************Abril 1 2008********************************************
  //Para el acero...........
  //Ni,Fe,Mn,,Cr
  //EScogemos los materiales del nist......
   G4NistManager* man = G4NistManager::Instance();
 
   G4Material* Hierro  = man->FindOrBuildMaterial("G4_Fe");
   G4Material* Cromo  = man->FindOrBuildMaterial("G4_Cr"); 
   G4Material* Niquel  = man->FindOrBuildMaterial("G4_Ni");
   G4Material* Manganeso  = man->FindOrBuildMaterial("G4_Mn");
   G4Material* Silicio  = man->FindOrBuildMaterial("G4_Si");
   G4Material* Nitrogeno  = man->FindOrBuildMaterial("G4_N");
  //************************** 10.3.2014 ***************
  // Material definition for farm soil --- D:A 
   G4Material* SiO2 =new G4Material("SiO2", density= 2.65*g/cm3, ncomponents=2);
   SiO2->AddElement(Si,natoms = 1); 
   SiO2->AddElement(O,natoms = 2);
   G4Material* Al2O3 =new G4Material("Al2O3", density= 2.42*g/cm3, ncomponents=2);
   Al2O3->AddElement(Al,natoms = 2); 
   Al2O3->AddElement(O,natoms = 3);
   G4Material* Fe2O3 =new G4Material("Fe2O3", density= 5.24*g/cm3, ncomponents=2);
   Fe2O3->AddElement(Fe,natoms = 2); 
   Fe2O3->AddElement(O,natoms = 3);
   G4Material* CaO =new G4Material("CaO", density= 3.35*g/cm3, ncomponents=2);
   CaO->AddElement(Ca,natoms = 1); 
   CaO->AddElement(O,natoms = 1);
   G4Material* MgO =new G4Material("MgO", density= 3.8*g/cm3, ncomponents=2);
   MgO->AddElement(Mg,natoms = 1); 
   MgO->AddElement(O,natoms = 1);
   G4Material* TiO2 =new G4Material("TiO2", density= 2.65*g/cm3, ncomponents=2);
   TiO2->AddElement(Ti,natoms = 1); 
   TiO2->AddElement(O,natoms = 2);    


  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><
  
   
   
   
  //Abril 16 Nuevo material acero 304 (sacado de http://www.lenntech.com/Stainless-steel-304.htm)
  G4Material* Acero304 = 
    new G4Material("Acero304"  , density= 8*g/cm3, ncomponents=6);
  Acero304->AddMaterial(Hierro, fractionmass=0.7115); 
  Acero304->AddMaterial(Cromo, fractionmass=0.18);
  Acero304->AddMaterial(Niquel, fractionmass=0.08);
  Acero304->AddMaterial(Manganeso, fractionmass=0.02);
  Acero304->AddMaterial(Silicio, fractionmass=0.0075);
  Acero304->AddMaterial(Nitrogeno, fractionmass=0.001);
  



  
  //***************************************************************************************
  //Nuevo material agua!!!  
  G4Material* water = new G4Material("water", 1.0*g/cm3, 2);
  water->AddElement(H,natoms=2);
  water->AddElement(O,natoms=1);
  
  //G4Material("GasHeAr",density,ncomponents,kStateGas,temperature,pressure);
  //***********H2 en gas
  //G4Material* GasH2 = new G4Material("GasH2", 0.085*kg/m3,ncomponents=1);
  //GasH2->AddElement(H,natoms=2);
  
  //carbono como grafito***************************************************
  G4Material* Carbono = new G4Material("Carbono", 2260*kg/m3,ncomponents=1);
  Carbono->AddElement(C,natoms=1);
  
  //************H2 en liquido*********************************************************************
  //G4Material* GasH2 = new G4Material("GasH2",70.973*kg/m3,ncomponents=1,
  //kStateLiquid,temperature=20.27*kelvin ,pressure=1*atmosphere);
  //GasH2->AddElement(H,natoms=2);
  //con este material se obtuvo myNtuple_1e6_H2_liquid.root....................................
  //**********************************************************************************************
  
  
  //para los materiales del NIST
  //G4NistManager* man = G4NistManager::Instance();
  //G4Material* Carbono  = man->FindOrBuildMaterial("G4_C");
  //G4Material* GasH2  = man->FindOrBuildMaterial("G4_H");
  //G4Material* water  = man->FindOrBuildMaterial("G4_WATER");
  G4Material* Parafina  = man->FindOrBuildMaterial("G4_PARAFFIN");
  //G4Material* Polyethylene  = man->FindOrBuildMaterial("G4_POLYETHYLENE");                                     
  
  

  // Soil Definition
  // Definicion del suelo, tal cual la puso nancy en su tesis, la densidad la tomo de Ali! (ver macondo 1Er link)
  G4Material* Soil = new G4Material("Soil", density=1.43*g/cm3,ncomponents=8);//se debe revisar la densidad
  Soil->AddElement(O,  fractionmass=51.2*perCent);
  Soil->AddElement(Si, fractionmass=28.0*perCent);  
  Soil->AddElement(Al, fractionmass=7.*perCent);  
  Soil->AddElement(Fe, fractionmass=5.5*perCent);
  Soil->AddElement(Ca, fractionmass=5.0*perCent);
  Soil->AddElement(K,  fractionmass=1.4*perCent);
  Soil->AddElement(Mg, fractionmass=1.3*perCent); 
  Soil->AddElement(Na, fractionmass=0.6*perCent);
  
  //suelo humedo!!!!!!!!!!! al 5% gravimétrico
  G4Material* SoilH5 = new G4Material("SoilH5", density=1.51*g/cm3,ncomponents=2);//revisar densidad pal humedo
  SoilH5->AddMaterial(Soil, fractionmass=95*perCent); 
  SoilH5->AddMaterial(water, fractionmass=5*perCent); 
  
  //suelo humedo!!!!!!!!!!! al 10% gravimétrico
  G4Material* SoilH10 = new G4Material("SoilH10", density=1.59*g/cm3,ncomponents=2);//se debe revisar la densidad
  SoilH10->AddMaterial(Soil, fractionmass=90*perCent); 
  SoilH10->AddMaterial(water, fractionmass=10*perCent);
  
  //suelo humedo!!!!!!!!!!! al 15% gravimétrico
  G4Material* SoilH15 = new G4Material("SoilH15", density=1.68*g/cm3,ncomponents=2);//se debe revisar la densidad
  SoilH15->AddMaterial(Soil, fractionmass=85*perCent); 
  SoilH15->AddMaterial(water, fractionmass=15*perCent);

  G4Material* SoilH20 = new G4Material("SoilH20", density=1.76*g/cm3,ncomponents=2);//se debe revisar la densidad
  SoilH20->AddMaterial(Soil, fractionmass=80*perCent); 
  SoilH20->AddMaterial(water, fractionmass=20*perCent);
 
//****************************************************ARENA SECA!****************************************************
  //Sun Aug 29 19:11:41 COT 2010
  // The densities in the wet sand comes from the cristancho´s model of the soil:
  // theta1_theta_2_Vs_rho_with_delta_theta_0.05.dat

  G4Material* DrySand = new G4Material("DrySand", 1.30*g/cm3, 2);
  DrySand->AddElement(Si,natoms=1);
  DrySand->AddElement(O,natoms=2);
  
  G4Material* SandH5 = new G4Material("SandH5", density=1.365*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH5->AddMaterial(DrySand, fractionmass=95.24*perCent); 
  SandH5->AddMaterial(water, fractionmass=4.76*perCent);
  
  G4Material* SandH10 = new G4Material("SandH10", density=1.43*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH10->AddMaterial(DrySand, fractionmass=90.91*perCent); 
  SandH10->AddMaterial(water, fractionmass=9.09*perCent);

  G4Material* SandH15 = new G4Material("SandH15", density=1.495*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH15->AddMaterial(DrySand, fractionmass=86.96*perCent); 
  SandH15->AddMaterial(water, fractionmass=13.04*perCent);
   
  G4Material* SandH20 = new G4Material("SandH20", density=1.56*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH20->AddMaterial(DrySand, fractionmass=83.34*perCent); 
  SandH20->AddMaterial(water, fractionmass=16.66*perCent);

  G4Material* SandH25 = new G4Material("SandH25", density=1.625*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH25->AddMaterial(DrySand, fractionmass=80*perCent); 
  SandH25->AddMaterial(water, fractionmass=20*perCent);
  
  G4Material* SandH30 = new G4Material("SandH30", density=1.587*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH30->AddMaterial(DrySand, fractionmass=76.93*perCent); 
  SandH30->AddMaterial(water, fractionmass=23.07*perCent);
  
  G4Material* SandH35 = new G4Material("SandH35", density=1.553*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH35->AddMaterial(DrySand, fractionmass=74.08*perCent); 
  SandH35->AddMaterial(water, fractionmass=25.92*perCent);

  G4Material* SandH40 = new G4Material("SandH40", density=1.523*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH40->AddMaterial(DrySand, fractionmass=71.43*perCent); 
  SandH40->AddMaterial(water, fractionmass=28.57*perCent);
  
  G4Material* SandH45 = new G4Material("SandH45", density=1.496*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH45->AddMaterial(DrySand, fractionmass=68.97*perCent); 
  SandH45->AddMaterial(water, fractionmass=31.03*perCent);
 
  G4Material* SandH50 = new G4Material("SandH50", density=1.471*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH50->AddMaterial(DrySand, fractionmass=66.67*perCent); 
  SandH50->AddMaterial(water, fractionmass=33.33*perCent);

  G4Material* SandH55 = new G4Material("SandH55", density=1.449*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH55->AddMaterial(DrySand, fractionmass=64.52*perCent); 
  SandH55->AddMaterial(water, fractionmass=35.48*perCent);
 
  G4Material* SandH60 = new G4Material("SandH60", density=1.429*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH60->AddMaterial(DrySand, fractionmass=62.5*perCent); 
  SandH60->AddMaterial(water, fractionmass=37.5*perCent);
 
  G4Material* SandH65 = new G4Material("SandH65", density=1.411*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH65->AddMaterial(DrySand, fractionmass=60.61*perCent); 
  SandH65->AddMaterial(water, fractionmass=39.39*perCent);
 
  G4Material* SandH70 = new G4Material("SandH70", density=1.394*g/cm3,ncomponents=2);//density from Cristancho´s model
  SandH70->AddMaterial(DrySand, fractionmass=58.83*perCent); 
  SandH70->AddMaterial(water, fractionmass=41.17*perCent);
  
//###################################################################################################################
//>>>>>>>>>>>>>>>>>>>>>>>>  Moisture on Farm Soil definition <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  G4Material* FSoil = new G4Material("FSoil", density= 0.66*g/cm3, ncomponents=6);
  FSoil->AddMaterial(SiO2, fractionmass=0.77);                          
  FSoil->AddMaterial(Al2O3, fractionmass=0.16);                                                 
  FSoil->AddMaterial(Fe2O3, fractionmass=0.03);                                                   
  FSoil->AddMaterial(CaO, fractionmass=0.02);                                                     
  FSoil->AddMaterial(MgO, fractionmass=0.009);                                                     
  FSoil->AddMaterial(TiO2, fractionmass=0.011);




  G4Material* FSoilH5 = new G4Material("FSoilH5", density=0.678*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH5->AddMaterial(FSoil, fractionmass=95.24*perCent); 
  FSoilH5->AddMaterial(water, fractionmass=4.76*perCent);
  
  G4Material* FSoilH10 = new G4Material("FSoilH10", density=0.6793*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH10->AddMaterial(FSoil, fractionmass=90.91*perCent); 
  FSoilH10->AddMaterial(water, fractionmass=9.09*perCent);

  G4Material* FSoilH15 = new G4Material("FSoilH15", density=0.681*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH15->AddMaterial(FSoil, fractionmass=86.96*perCent); 
  FSoilH15->AddMaterial(water, fractionmass=13.04*perCent);
   
  G4Material* FSoilH20 = new G4Material("FSoilH20", density=0.686*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH20->AddMaterial(FSoil, fractionmass=83.34*perCent); 
  FSoilH20->AddMaterial(water, fractionmass=16.66*perCent);

  G4Material* FSoilH25 = new G4Material("FSoilH25", density=0.625*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH25->AddMaterial(FSoil, fractionmass=80*perCent); 
  FSoilH25->AddMaterial(water, fractionmass=20*perCent);
  
  G4Material* FSoilH30 = new G4Material("FSoilH30", density=0.687*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH30->AddMaterial(FSoil, fractionmass=76.93*perCent); 
  FSoilH30->AddMaterial(water, fractionmass=23.07*perCent);
  
  G4Material* FSoilH35 = new G4Material("FSoilH35", density=0.653*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH35->AddMaterial(FSoil, fractionmass=74.08*perCent); 
  FSoilH35->AddMaterial(water, fractionmass=25.92*perCent);

  G4Material* FSoilH40 = new G4Material("FSoilH40", density=0.6523*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH40->AddMaterial(FSoil, fractionmass=71.43*perCent); 
  FSoilH40->AddMaterial(water, fractionmass=28.57*perCent);
  
  G4Material* FSoilH45 = new G4Material("FSoilH45", density=0.6496*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH45->AddMaterial(FSoil, fractionmass=68.97*perCent); 
  FSoilH45->AddMaterial(water, fractionmass=31.03*perCent);
 
  G4Material* FSoilH50 = new G4Material("FSoilH50", density=0.6471*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH50->AddMaterial(FSoil, fractionmass=66.67*perCent); 
  FSoilH50->AddMaterial(water, fractionmass=33.33*perCent);

  G4Material* FSoilH55 = new G4Material("FSoilH55", density=0.6449*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH55->AddMaterial(FSoil, fractionmass=64.52*perCent); 
  FSoilH55->AddMaterial(water, fractionmass=35.48*perCent);
 
  G4Material* FSoilH60 = new G4Material("FSoilH60", density=0.6429*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH60->AddMaterial(FSoil, fractionmass=62.5*perCent); 
  FSoilH60->AddMaterial(water, fractionmass=37.5*perCent);
 
  G4Material* FSoilH65 = new G4Material("FSoilH65", density=0.6411*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH65->AddMaterial(FSoil, fractionmass=60.61*perCent); 
  FSoilH65->AddMaterial(water, fractionmass=39.39*perCent);
 
  G4Material* FSoilH70 = new G4Material("FSoilH70", density=0.6394*g/cm3,ncomponents=2);//density from Cristancho´s model
  FSoilH70->AddMaterial(FSoil, fractionmass=58.83*perCent); 
  FSoilH70->AddMaterial(water, fractionmass=41.17*perCent);

 
//------------------------------------------------------------------------------------------------------------------  
  //*********************************************************************************************************************
  // Mine material definition
  //Definicion del material que puede tener  la mina.
  G4Material* TNT = new G4Material("TNT",density=1.654*g/cm3,ncomponents=4);// Densidad tomada del programa de fortran
  TNT->AddElement(H,natoms=5);
  TNT->AddElement(C,natoms=7);
  TNT->AddElement(N,natoms=3);
  TNT->AddElement(O,natoms=6);
  
  //########################Polietileno#####################
  G4Material* Polyethylene = new G4Material("Polyethylene",density=0.92*g/cm3,ncomponents=2);
  Polyethylene->AddElement(H,natoms=2);
  Polyethylene->AddElement(C,natoms=1);
  
  //#######Acrílico del Container de la dummy-mine
  //La densidad sale del peso que dicen sobre el volumen que dicen en la carta de CAPE TOWN
  //mirar bitacora escrita...es decir el papel
  G4Material* acrilico = new G4Material("acrilico",density=1.15*g/cm3,ncomponents=3);
  acrilico->AddElement(H,natoms=4);
  acrilico->AddElement(C,natoms=2);
  acrilico->AddElement(O,natoms=1);
  

  //Febrero 26
  //Concreto definido por el NIST
  G4Material* Concreto  = man->FindOrBuildMaterial("G4_CONCRETE");
  //Agua pesada!
  G4Material* HeavyWater = new G4Material("HeavyWater", 1.1*g/cm3, 2);
  HeavyWater->AddElement(D,natoms=2);
  HeavyWater->AddElement(O,natoms=1);
  

  
  //######Aire
  G4Material* Air = 
    new G4Material("Air"  , density=1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);
  //######Vacio
  G4Material* Vacuum =
    new G4Material("Vacuum", z=1., a=1.01*g/mole,density= universe_mean_density,
		   kStateGas, 3.e-18*pascal, 2.73*kelvin);
  
  //1 torr=133.322368 pascal, 1 torr=0.00131578947atm, original 3800torr
  //Los calculos de la densidad estan en la bitacora.........!
  //la densidad que tenía nancy era de 2.487*kg/m^3 a mi me dió .. bitácora 0.79*kg/m^3

  //MArzo 5 2008....... le cambié la densiodad a la mala para ver si agarra los neutrones térmicos que tiene que agarrar
  //MArzo 18 2008 ---vuelvo a cambiar la densidad a la que calculé...
  G4Material *GasHeAr = 
    new G4Material("GasHeAr", density=0.79*kg/m3,ncomponents=2,kStateGas,
		   temperature=298*kelvin,pressure=5.0*atmosphere);
  
  GasHeAr->AddElement(He3, fractionmass=0.75);
  GasHeAr->AddElement(Ar, fractionmass=0.25);
  
  
  //Julio 29-2008 . Creación de un nuevo material...madera. ver pdfs/madera y Bookmarks en firefos relacionados con madera.
  //Ojo. La madera tiene cierta humedad residual...approx 10% en peso. le puse 15%
  
  //composicion sacasa de Wiskipedia
  G4Material* Wood = new G4Material("Wood",density=0.5*g/cm3,ncomponents=4);
  Wood->AddElement(H,fractionmass=0.06);
  Wood->AddElement(C,fractionmass=0.5);
  Wood->AddElement(O,fractionmass=0.42);
  Wood->AddElement(N,fractionmass=0.02);
  

  
  
  G4Material* RealWood = new G4Material("RealWood",density=1.2*g/cm3,ncomponents=2);
  RealWood->AddMaterial(Wood, fractionmass=90*perCent); 
  RealWood->AddMaterial(water, fractionmass=10*perCent);
  


  //***************Asignación de materiales*****************
  defaultMaterial  = Vacuum;
  Worldmaterial = Air; //Para definir un material
  //Tubematerial=Al;
  //**************************************************************************************
  Soilmaterial=DrySand;//Vacuum;//Soil;//water;//Parafina;//water;//Carbono;//GasH2;//water;//Soil;//water
  Plasticboxmaterial=Polyethylene;//Vacuum;//Polyethylene;
  Detectormaterial=GasHeAr;
  RecoveringMaterial=Acero304;
  //Febrero 27 les puse de arena para ver solo el fondo es decir sin dummy mine!
  //Marzo 18 pongo la dummy mine tal cual...acrilico +TNT
  Containermaterial=acrilico;//DrySand;//acrilico;
  Minematerial=TNT;//DrySand;//TNT;
  
  //Febrero 28 lo cambie a aire para ver solo el fondo
  //Marzo 5 lo volví a cambiar a aire para ver solo el fondo
  //Marzo 7 lo volví a cambiar a concreto para ver el efecto de la columna
  ColumnaMaterial=Air;//Concreto;//Concreto;//Air;//Concreto;
  //Marzo 18 2008 pongo el blindaje de parafina!
  BlindajeMaterial=Air;//Parafina;//Air;//Parafina;
  
  WetSandMaterial=DrySand;
  
  ReflectorMaterial=HeavyWater;
  
  //Julio 29-2008 Material de la caja de madera. (ver definición de madera arriba..)
  WoodBoxMaterial=Air;//RealWood;//RealWood;

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  //************************************************************************************************************************
  //************************************************************************************************************************
  //Diciembre 6 . Agregada por AngelH para mirar si se calcula el numero de hidrogenos por unidad de volumen igual que yo.

  G4cout <<"**************************************************************   "<<G4endl;
  G4cout <<"El numero de átomos por unidad de volumen es....."<<G4endl;
  G4cout <<DrySand->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<SandH5->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<SandH10->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<SandH15->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<SandH20->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<SandH25->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<TNT->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<acrilico->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  G4cout <<Polyethylene->GetTotNbOfAtomsPerVolume()*cm3<<G4endl;
  
  G4cout <<"**************************************************************   "<<G4endl;
  //************************************************************************************************************************  
  //************************************************************************************************************************
}


G4VPhysicalVolume* bsneutronDetectorConstruction::Construct()
{
  
  //*********** Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)

  //Marzo 12.... Antes estaba 1*1*1 m^3; pero lo aumente para ver si el numero de colisiones cuadra..
  G4double expHall_x = 250*cm;//1*m;//50*cm;
  G4double expHall_y = 250*cm;//1*m;//150*cm;
  G4double expHall_z = 180*cm;//1*m;//50*cm;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             Worldmaterial,"expHall_log",0,0,0);
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                      experimentalHall_log,"expHall",0,false,0);
  //*******************************************************************************************************
  //-**********************Julio 28-2008 Creo un  nuevo volumen que es la caja de madera.
  //
  //Dimensiones de la caja de madera.
  G4double WoodBox_x = 63*cm;
  G4double WoodBox_y = 63*cm;
  G4double WoodBox_z = 47*cm;
  
  WoodBox_box= new G4Box("WoodBox_box",WoodBox_x,WoodBox_y,WoodBox_z);
  
  WoodBox_log = new G4LogicalVolume(WoodBox_box,WoodBoxMaterial,"WoodBox_log",0,0,0);
  
  WoodBox_phys = new G4PVPlacement(0,G4ThreeVector(0*cm,0*cm,0*cm),
				   WoodBox_log,"WoodBox",experimentalHall_log ,false,0);
  

  //Julio 29-2008
  //---------------------------An air Box above de sand (Will be the mother volume of the detectors)
  
  G4double Air_x = 60*cm;
  G4double Air_y = 60*cm;
  G4double Air_z = 25*cm;//Julio 29-2008 ver bitácora.
  
  AirBox_box= new G4Box("AirBox_box",Air_x,Air_y,Air_z);
  
  AirBox_log = new G4LogicalVolume(AirBox_box,Worldmaterial,"AirBox_log",0,0,0);
  
  //Julio 29-2009. Posición de la caja de aire con respectro al centro de la caja madre de Madera.
  G4double AirBoxPos_x = 0.0*cm;
  G4double AirBoxPos_y = 0.0*cm;
  G4double AirBoxPos_z = 22.0*cm; //Ver dibujo de la bitácora Julio 29-2008 (22cm = 25cm-3cm)

  AirBox_phys = new G4PVPlacement(0,G4ThreeVector(AirBoxPos_x,AirBoxPos_y,AirBoxPos_z),
				     AirBox_log,"AirBox",WoodBox_log,false,0);




  
  
  //------------------------------ a soil block (sand box!!!)
  
  //Marzo 12...estaba 75 *75*75^3 lo aumente ..para...ahhh ya se sabe...       
  
  //Julio 28-2008 ..cambio los valores de la caja  de arena (120*120*42) que ahora será una caja hija de la caja de madera.
  
  //Le pongo los valores incluyendo la caja de madera.
  
  G4double soil_x = 60*cm;//Estaban 50,50,35 cm
  G4double soil_y = 60*cm;//Los cambié a los reales del lab: 
  G4double soil_z = 21*cm;//26.5*cm;//65,65,26.5
  
  soilBlock_box= new G4Box("soilBlock_box",soil_x,soil_y,soil_z);

  soilBlock_log = new G4LogicalVolume(soilBlock_box,Soilmaterial,"soilBlock_log",0,0,0);

  //Julio 29-2009. Posición de la arena con respectro al centro de la caja madre de Madera.
  G4double soilPos_x = 0.0*cm;
  G4double soilPos_y = 0.0*cm;
  G4double soilPos_z = -24.0*cm; //Ver dibujo de la bitácora Julio 29-2008 (24 cm= 3cm +21 cm)

  soilBlock_phys = new G4PVPlacement(0,G4ThreeVector(soilPos_x,soilPos_y,soilPos_z),
				     soilBlock_log,"soilBlock",WoodBox_log,false,0);
  
  
  //-----------------------------Mayo 23-2008 capa de arena húmeda!!!
  //Julio 29-2008 Nuevas dimensiones (Ver bitacora.)
  G4double wet_x =60*cm;
  G4double wet_y =60*cm;
  //G4double wet_z =2.5*cm;
  
  WetSand_box= new G4Box("WetSand_box",wet_x,wet_y,Wet_Sand_width/2);
  
  WetSand_log = new G4LogicalVolume(WetSand_box,WetSandMaterial,"WetSand_log",0,0,0);
  //
  //G4double wetPos_z = 18.5*cm; //Julio 29-2008 (21cm-w/2 = 21-2.5=18.5cm....ver bitacora)

  //******Julio 29-2008 Si no quiero capa de arena pues comento el volumen físico.
  WetSand_phys = new G4PVPlacement(0,G4ThreeVector(wetPos_x,wetPos_y,wetPos_z),
				   WetSand_log,"WetSand",soilBlock_log,false,0);
  
  
  
  //-----------------------------Caja madre donde se montarán los arreglos de detectores.....

   
  G4Box* detector_box=new G4Box("detector_box",detector_x,detector_y,detector_z);
  detector_log = new G4LogicalVolume(detector_box, Worldmaterial ,"detector_log",0,0,0);

  //Julio 29-20008. Cambio el volumen madre de la caja madre de los detectores a ahora la caja
  //de aire encima de la arena
  
  detector_phys = new G4PVPlacement(0,
				    G4ThreeVector(detectorPos_x,detectorPos_y,detectorPos_z),
				    detector_log,"detectorBlock",AirBox_log,false,0);
  
  //Mayo 23-2008-------------------------Voy a crear dos cajas "madre" para cada arreglo de detectores...
  
  //***Cajita pal array A
  G4Box* ArrayA_box=new G4Box("ArrayA_box",ArrayA_x,ArrayA_y,ArrayA_z);
  ArrayA_log = new G4LogicalVolume(ArrayA_box, Worldmaterial ,"ArrayA_log",0,0,0);
  ArrayA_phys = new G4PVPlacement(0,G4ThreeVector(ArrayAPos_x,ArrayAPos_y,ArrayAPos_z),
				  ArrayA_log,"ArrayABlock",detector_log,false,0);
  //***Cajita pal array B
  G4Box* ArrayB_box=new G4Box("ArrayB_box",ArrayA_x,ArrayA_y,ArrayA_z);
  ArrayB_log = new G4LogicalVolume(ArrayB_box, Worldmaterial ,"ArrayB_log",0,0,0);
  ArrayB_phys = new G4PVPlacement(0,G4ThreeVector(ArrayBPos_x,ArrayBPos_y,ArrayBPos_z),
				  ArrayB_log,"ArrayBBlock",detector_log,false,0);
  
  //jUNIO 6...estaba caja pero lo cambie qa cilindro
  //G4Box* detectorABlock_box=new G4Box("detectorABlock_box",detector_x,detector_y,detector_z);
  
  //******************FORMA CILINDRICA PARA TODOS LOS DETECTORES******************************************************
  G4RotationMatrix * Rot = new G4RotationMatrix;
  Rot->rotateX(M_PI/2*rad);
  //Rot->rotateX(0.*rad);  
  G4double innerRadiusDetector = 0.*cm;
  G4double outerRadiusDetector = 1.215*cm;//de acuerdo al radio que aparece sn el pdf de los detectores
  G4double hightDetector = 10.0*cm;
  G4double startAngleDetector= 0.*deg;
  G4double spanningAngleDetector = 360.*deg;
  
  G4Tubs* detectorABlock_box=new G4Tubs("detector_tube",innerRadiusDetector,
					outerRadiusDetector,hightDetector,
					startAngleDetector,spanningAngleDetector);
  
  //*************************************************Abril 1*****************************
  //**************************************recubrimiento de acero
  //NOTA!Abril 2 Me tocó cambiarlo a un cilindro sólido por que paila se genera un loop por 
  //tener dos geometrias que comparten una superficie. changos!
  innerRadiusDetector = 0.0*cm; 
  outerRadiusDetector = 1.27*cm;//de acuerdo al radio que aparece en el pdf de los detectores.
  hightDetector = 11.0*cm;
  G4Tubs* detectorABlock_box2=new G4Tubs("detector_recovering",innerRadiusDetector,
					 outerRadiusDetector,hightDetector,
					 startAngleDetector,spanningAngleDetector);
  
  detector1_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector1Block_recovering",0,0,0);
  detector1_recovering_phys = new G4PVPlacement(Rot,
					       G4ThreeVector(detector1Pos_x,detector1Pos_y,detector1Pos_z),
					       detector1_recovering_log,
					       "detector1_recovering",
					       ArrayA_log,
					       false,					       
					       0);
  //*********************************************************Detector 1*********************************************************  
  detector1Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector1Block_log",0,0,0);
  // setXPosition(20.0*cm);
  //G4double detectorPos_x = 0.0*cm;
  // G4double detectorPos_y = 0.0*cm;
  // G4double detectorPos_z =85.0*cm;//10.0*cm; 
  detector1Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector1Block_log,
					  "detector1Block",
					  detector1_recovering_log,
					  false,
					  0);
  


  //*********************************************************Detector 2**********************************************************

   detector2_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector2Block_recovering",0,0,0);
   detector2_recovering_phys = new G4PVPlacement(Rot,
						 G4ThreeVector(1.27*cm,0*cm,0.0*cm),
						 detector2_recovering_log,
						 "detector2_recovering",
						 ArrayA_log,
						 false,						 
						 0);
   //****************************************************************************************************************************

   detector2Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector2Block_log",0,0,0);
   detector2Block_phys = new G4PVPlacement(0,
					   G4ThreeVector(0,0,0),
					   detector2Block_log,
					  "detector2Block",
					  detector2_recovering_log  ,
					  false,
					  0);
  //*********************************************************Detector 3**********************************************************
  //*******************************Abril 1 2008 recubrimiento de acero del detector 3********************************************
  detector3_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector1Block_recovering",0,0,0);
  detector3_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(-1.27*cm,0*cm,0.0*cm),
						detector3_recovering_log,
						"detector3_recovering",
						ArrayA_log,
						false,
						
						0);
  //*****************************************************************************************************************************
  detector3Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector3Block_log",0,0,0);
  detector3Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector3Block_log,
					  "detector3Block",
					  detector3_recovering_log ,
					  false,
					  0);



 //*********************************************************Detector 4********************************************************
  detector4_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector4Block_recovering",0,0,0);
  detector4_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(detector4Pos_x,detector4Pos_y,detector4Pos_z),
						detector4_recovering_log,
						"detector4_recovering",
						ArrayA_log,
						false,
						
						0);
  //*****************************************************************************************************************************
  detector4Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector4Block_log",0,0,0);
  detector4Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector4Block_log,
					  "detector4Block",
					  detector4_recovering_log ,
					  false,
					  0);

  //*********************************************************Detector 5**********************************************************
  //****************************Abril 1 2008 recubrimiento de acero del detector 6***********************************************
  detector5_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector5Block_recovering",0,0,0);
  detector5_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(detector5Pos_x,detector5Pos_y,detector5Pos_z),
						detector5_recovering_log,
						"detector5_recovering",
						ArrayB_log,
						false,
						
						0);
  //*****************************************************************************************************************************
  detector5Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector5Block_log",0,0,0);
  detector5Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector5Block_log,
					  "detector5Block",
					  detector5_recovering_log ,
					  false,
					  0);

  //*********************************************************Detector 6**********************************************************
  //******************************Abril 1 2008 recubrimiento de acero del detector 4*********************************************
  detector6_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector6Block_recovering",0,0,0);
  detector6_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(1.27*cm,0*cm,0.0*cm),
						detector6_recovering_log,
						"detector6_recovering",
						ArrayB_log,
						false,				
						0);
  //*****************************************************************************************************************************
  detector6Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector6Block_log",0,0,0);
  detector6Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector6Block_log,
					  "detector6Block",
					  detector6_recovering_log ,
					  false,
					  0);





  //*********************************************************Detector 2********************************************************** 
  //******************************Abril 1 2008 recubrimiento de acero del detector 2********************************************
  detector7_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector7Block_recovering",0,0,0);
  detector7_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(-1.27*cm,0*cm,0.0*cm),
						detector7_recovering_log,
						"detector7_recovering",
						ArrayB_log,
						false,
						0);
  //*****************************************************************************************************************************
  
  detector7Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector7Block_log",0,0,0);
  detector7Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector7Block_log,
					  "detector7Block",
					  detector7_recovering_log ,
					  false,
					  0);



  //*********************************************************Detector 8**********************************************************
  //*******************************Abril 1 2008 recubrimiento de acero del detector 8
  detector8_recovering_log = new G4LogicalVolume(detectorABlock_box2,RecoveringMaterial,"detector8Block_recovering",0,0,0);
  detector8_recovering_phys = new G4PVPlacement(Rot,
						G4ThreeVector(detector8Pos_x,detector8Pos_y,detector8Pos_z),
						detector8_recovering_log,
						"detector8_recovering",
						ArrayB_log,
						false,				
						0);
  //*****************************************************************************************************************************
  //jUNIO 6...estaba caja pero lo cambie qa cilindro
  //G4Box* detectorBBlock_box=new G4Box("detectorBBlock_box",detector_x,detector_y,detector_z);
  detector8Block_log = new G4LogicalVolume(detectorABlock_box,Detectormaterial,"detector8Block_log",0,0,0);
  // setXPosition(20.0*cm);
  //G4double detectorPos_x = 0.0*cm;
  // G4double detectorPos_y = 0.0*cm;
  // G4double detectorPos_z =85.0*cm;//10.0*cm; 
  detector8Block_phys = new G4PVPlacement(0,
					  G4ThreeVector(0,0,0),
					  detector8Block_log,
					  "detector8Block",
					  detector8_recovering_log,
					  false,			  
					  0);
  


  //*****************************************************************************************************************
  //*****************************************************************************************************************
  //******************************Le voy a asignar "sensitividad" al detector."
  //Es decir se construyen los componentes que van a contar...esta funcion  SetupScoring esta definida..mas abajo...
  //
  //Esta funcion estaba para poner sensible un solo detector!
  // SetupScoring(detectorABlock_log);
  
  // Get pointer to detector manager
  
  G4SDManager* manager = G4SDManager::GetSDMpointer();  
  
  G4String SDname;

  MySensitiveDetector* detector1= new MySensitiveDetector(SDname="/detector1");
  manager->AddNewDetector(detector1);
  detector1Block_log->SetSensitiveDetector(detector1);
    
  MySensitiveDetector* detector2= new MySensitiveDetector(SDname="/detector2");
  manager->AddNewDetector(detector2);
  detector2Block_log->SetSensitiveDetector(detector2);

  MySensitiveDetector* detector3= new MySensitiveDetector(SDname="/detector3");
  manager->AddNewDetector(detector3);
  detector3Block_log->SetSensitiveDetector(detector3);

  MySensitiveDetector* detector4= new MySensitiveDetector(SDname="/detector4");
  manager->AddNewDetector(detector4);
  detector4Block_log->SetSensitiveDetector(detector4);
  
  MySensitiveDetector* detector5= new MySensitiveDetector(SDname="/detector5");
  manager->AddNewDetector(detector5);
  detector5Block_log->SetSensitiveDetector(detector5);

  MySensitiveDetector* detector6= new MySensitiveDetector(SDname="/detector6");
  manager->AddNewDetector(detector6);
  detector6Block_log->SetSensitiveDetector(detector6);

  MySensitiveDetector* detector7= new MySensitiveDetector(SDname="/detector7");
  manager->AddNewDetector(detector7);
  detector7Block_log->SetSensitiveDetector(detector7);
  
  MySensitiveDetector* detector8= new MySensitiveDetector(SDname="/detector8");
  manager->AddNewDetector(detector8);
  detector8Block_log->SetSensitiveDetector(detector8);

  



  //*****************************************************************************************************************
  //********************************************Dummy mine*******************************************************
  //Contenedor cilindrico de acrilico..........  
  G4double radio_interno = 0.0;
  G4double radio_externo = 4.0*cm;
  G4double altura = 1.7*cm;
  G4double startAngle= 0.*deg;
  G4double spanningAngle = 360.*deg;
  G4Tubs* acrilico_box = new G4Tubs("acrilico",radio_interno,radio_externo,altura,startAngle,spanningAngle);
  
  //*****************Recubrimiento de acrilico, es decir una dummy mine tal cual!
  acrilico_log = new G4LogicalVolume(acrilico_box,Containermaterial,"acrilico_log",0,0,0);

  //******************Recubrimiento de Polietileno
  //acrilico_log = new G4LogicalVolume(acrilico_box,Plasticboxmaterial,"acrilico_log",0,0,0);
  
  //Diciembre 6
  //******************Recubrimiento de TNT (es decir una mina de TNT integro)
  //acrilico_log = new G4LogicalVolume(acrilico_box,Minematerial,"acrilico_log",0,0,0);
  
  
  //March 28-2009
  //I commented it because...the mine is daugher volume of the wet sand layer  
  /*acrilico_phys = new G4PVPlacement(0,
				    G4ThreeVector(minePos_x,minePos_y,minePos_z),
				    acrilico_log,"acrilico",soilBlock_log ,false,0);*/
  //Nota!....Le puse como volumen madre el suelo..para que no haya sobrelapamiento de materiales...
  //March 28-2009. I put the mother volume of the acrilico as the wet sand layer.
  //Remember that if I want to put the mine below the wet sand layer
  //the mother volume has to be the soil
  acrilico_phys = new G4PVPlacement(0,
				    G4ThreeVector(minePos_x,minePos_y,minePos_z),
				    acrilico_log,"acrilico",WetSand_log,false,0);
  
  //Simulante de TNT......
  G4double innerRadiusMine = 0.*cm;
  G4double outerRadiusMine = 3.5*cm;
  G4double hightMine = 1.1*cm;
  G4double startAngleMine = 0.*deg;
  G4double spanningAngleMine = 360.*deg;
  G4Tubs* mine_tube = new G4Tubs("mine_tube",innerRadiusMine,
				 outerRadiusMine,hightMine,
				 startAngleMine,spanningAngleMine);
  //Noviembre 30/2007
  //Le cambio el material a la mina para que sea de solo acrilico
  //Diciembre 6
  //se lo vuelvo a cambiar a TNT y cambio el del recubrimiento. mirar arriba
  mine_log = new G4LogicalVolume(mine_tube,Minematerial,"mine_log",0,0,0);
  // mine_log = new G4LogicalVolume(mine_tube,Containermaterial,"mine_log",0,0,0);

  
  //situado en 0,0,0 en el sistema de referencia del acrilico
  mine_phys = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				mine_log,"mine",acrilico_log,false,0);
  

  // ****December 03-12 (Morning 1:32 am) I made the DLM2 sensitive..
  SensitiveDLM2* SDDLM2= new SensitiveDLM2(SDname="/DLM2");
  manager->AddNewDetector(SDDLM2);
  acrilico_log->SetSensitiveDetector(SDDLM2);
 
  //#########################################################################################
  
  //###################################Target de polietileno####################################
  //***************************************Mayo 4 puse el target cubico de polietileno...........................
  
  /*plasticbox_box=new G4Box("plasticbox_box ", 6.0*cm ,6.0*cm,3.0*cm);
    plasticbox_log = new G4LogicalVolume(plasticbox_box,Plasticboxmaterial,"plasticbox_log",0,0,0);
    
    
    
  
  
    //#************Lo comente para poder tener soo agua...sin mina ni nada**************************
    plasticbox_phys = new G4PVPlacement(0, G4ThreeVector(plasticboxPos_x,plasticboxPos_y,plasticboxPos_z),
    plasticbox_log,"plasticbox",soilBlock_log,false,0);  */

  //#########################################################################################
  //#########################################################################################
  //Febrero 26
  //*****************************************************Columna de concreto!
  G4double columna_x =21*cm;//75*cm;//50.0*cm;
  G4double columna_y =11.5*cm;//75*cm;//50.0*cm;
  G4double columna_z =100*cm;//75*cm;//25.0*cm;
  columna_box= new G4Box("columna_box",columna_x,
			   columna_y,columna_z);
  columna_log = new G4LogicalVolume(columna_box,ColumnaMaterial,"columna_log",0,0,0);
  G4double columnaPos_x = 86*cm;
  G4double columnaPos_y = 24*cm;
  G4double columnaPos_z = 52*cm;//-0.25*m;
  columna_phys = new G4PVPlacement(0,
				   G4ThreeVector(columnaPos_x,columnaPos_y,columnaPos_z),
				   columna_log,"Columna",experimentalHall_log,false,0);  
  //#########################################################################################
  //#########################################################################################
   //Febrero 29
  //*****************************************************Blindaje de parafina
  
  G4double blindaje_x =20.5*cm;//75*cm;//50.0*cm;
  G4double blindaje_y =20.5*cm;//75*cm;//50.0*cm;
  G4double blindaje_z =20.5*cm;//75*cm;//25.0*cm;
  blindaje_box= new G4Box("blindaje_box",blindaje_x,
			   blindaje_y,blindaje_z);
  blindaje_log = new G4LogicalVolume(blindaje_box,BlindajeMaterial,"blindaje_log",0,0,0);
  G4double blindajePos_x = 128*cm;
  G4double blindajePos_y = -8*cm;
  G4double blindajePos_z = -29*cm;//-0.25*m;
  blindaje_phys = new G4PVPlacement(0,
				   G4ThreeVector(blindajePos_x,blindajePos_y,blindajePos_z),
				   blindaje_log,"Blindaje",experimentalHall_log,false,0);  


  //################################################################################
  //May 28-2009
  //Attach user limits to avoid proton tracking at the soil
  //copied from the underground example
  if (theUserLimitsForSoil != 0) delete theUserLimitsForSoil;
  
  theUserLimitsForSoil = new G4UserLimits(theMaxStepSize,   // step length max
					  DBL_MAX,          // track length max
					  theMaxTimeCuts,   // Time cut
					  theMinEkine);     // min energy
  
  //For the soil and the wet sand layer!!!
  soilBlock_log->SetUserLimits(theUserLimitsForSoil);
  //For the wet sand and the mine too..because...the User Limits do not propagate to the daughter volumes! 
 
  WetSand_log->SetUserLimits(theUserLimitsForSoil);
  acrilico_log->SetUserLimits(theUserLimitsForSoil);
  mine_log->SetUserLimits(theUserLimitsForSoil);
  
  //Para la caja de madera y para la columna de concreto
  WoodBox_log->SetUserLimits(theUserLimitsForSoil);
  columna_log->SetUserLimits(theUserLimitsForSoil);
  //##################################################################################
  //##################################################################################
  
  //######################################Atributos de visualización 
  experimentalHall_log->SetVisAttributes (G4VisAttributes::Invisible); 
  // The total World is invisible, we can see only the real objects

  detector_log->SetVisAttributes (G4VisAttributes::Invisible);

  //always return physical world
 
  return experimentalHall_phys;
}

void  bsneutronDetectorConstruction::SetupScoring(G4LogicalVolume* scoringVolume) 
{
  // Create a new sensitive detector named "MyDetector"
  
  //Primero lo voy a poner Sensitive detector generico...despues miro como se hace para Multifuncional Detector
  //G4MultiFunctionalDetector* detector = new G4MultiFunctionalDetector("MyDetector");
  
  MySensitiveDetector* detector= new MySensitiveDetector("MyDetector");
  
  // Get pointer to detector manager
  
  G4SDManager* manager = G4SDManager::GetSDMpointer();  
  
  // Register detector with manager
  
  manager->AddNewDetector(detector);
  
  
  // Attach detector to scoring volume
  
  scoringVolume->SetSensitiveDetector(detector);
  
  
  // Create a primitive Scorer named myScorer  
  //G4PSSphereSurfaceCurrent* scorer = new G4PSSphereSurfaceCurrent("myScorer",fCurrent_In);
  //El primer argumento es el nombre. y el segundo argumento indica que se va a medir la corriente de entrada
  //si hay dudas mirar /usr/local/geant4/geant4.8.1.p01/source/digits_hits/utils/src/G4PSFlatSurfaceCurrent.cc
  
  //G4PSFlatSurfaceCurrent* scorer = new G4PSFlatSurfaceCurrent("MyScorer",1);
  

  ////////////////////////Filtro!!!!!!!!!!!!!!!!!
  // Create a filter
  
  //G4SDParticleWithEnergyFilter* filter =
  
  //new G4SDParticleWithEnergyFilter("Neutron Filter");
  
  
  
  // Filtro para solo neutrones
  //filter->add("neutron");
  
  // Establece el rango de energias que el filtro aceptara
  //G4double minEnergy = 0.9*MeV;
  //G4double maxEnergy = 1.1*MeV;
  
  //filter->SetKineticEnergy(minEnergy, maxEnergy);
  
  // Attach filter to scorer. Scorer will only process hits which pass
  // the filter
  
  //Pegarle el filtro al contador (scorer). este solo procesara los hits que
  //fueron aceptados por el filtro
  //scorer->SetFilter(filter);

  // Registrar el scorer al detector  
  //detector->RegisterPrimitive(scorer);
 
  //G4cout<<"Se creo un  G4MultiFunctionalDetector llamado "
    
  //    <<detector->GetName()<<", y un contador G4PSFlatSurfaceCurrent llamado: "<<scorer->GetName()<<G4endl;
  
  //G4cout<<"Created scorer with name "<<scorer->GetName();
  
  //G4cout<<", covering energy range "<<minEnergy*MeV;
  
  //G4cout<<" : "<<maxEnergy*MeV<<" MeV"<<G4endl;
  
  
}

void bsneutronDetectorConstruction::setTargetSize(G4double SVNewSizeZ){
  G4cout << "El target tenia dimensiones [x,y,z]: " << G4BestUnit(2*plasticbox_box->GetXHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*plasticbox_box->GetYHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*plasticbox_box->GetZHalfLength(),"Length") << G4endl;
  
  plasticbox_box->SetZHalfLength(0.5*SVNewSizeZ);
  //fSVLength = soilBlock_box->GetZHalfLength();
  G4cout << "se cambiaron a[x,y,z]: " << G4BestUnit(2*plasticbox_box->GetXHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*plasticbox_box->GetYHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*plasticbox_box->GetZHalfLength(),"Length") << G4endl;
  
  G4RunManager::GetRunManager()-> GeometryHasBeenModified();   
}

void bsneutronDetectorConstruction::setTargetMaterial(G4String  materialName){
  //buscar el material por su nombre
  G4cout << "Antiguo Material: " << plasticbox_log->GetMaterial()->GetName() <<G4endl;
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {
      Plasticboxmaterial  = pttoMaterial;
      plasticbox_log ->SetMaterial(pttoMaterial); 
    }
  G4cout << "Nuevo Material: " << plasticbox_log->GetMaterial()->GetName() <<G4endl;
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}

void bsneutronDetectorConstruction::setSueloSize(G4double SVNewSizeZ){
  G4cout << "El suelo tenia  dimensiones [x,y,z]: " << G4BestUnit(2*soilBlock_box->GetXHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*soilBlock_box->GetYHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*soilBlock_box->GetZHalfLength(),"Length") << G4endl;
  
  soilBlock_box->SetZHalfLength(0.5*SVNewSizeZ);
  
  //fSVLength = soilBlock_box->GetZHalfLength();
  G4cout << "se cambiaron a [x,y,z]: " << G4BestUnit(2*soilBlock_box->GetXHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*soilBlock_box->GetYHalfLength(),"Length") << ", " 
	 << G4BestUnit(2*soilBlock_box->GetZHalfLength(),"Length") << G4endl;
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}
void bsneutronDetectorConstruction::setSueloMaterial(G4String materialName){
  G4cout << "Antiguo Material: " << soilBlock_log->GetMaterial()->GetName() <<G4endl;
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {
      Soilmaterial  = pttoMaterial;
      soilBlock_log ->SetMaterial(pttoMaterial); 
    }
  G4cout << "Nuevo Material: " << soilBlock_log->GetMaterial()->GetName() <<G4endl; 
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}

void bsneutronDetectorConstruction::setXDetectorPosition(G4double newposition){
  //plasticboxPos_x=newposition;
  //minePos_x=newposition;
  //Febrero 27 ahora cambio la posición de los detectores y el el macro la de la fuente
  detectorPos_x=newposition;
  //plasticbox_phys->SetTranslation(G4ThreeVector(plasticboxPos_x,plasticboxPos_y,plasticboxPos_z)); 
  //acrilico_phys->SetTranslation(G4ThreeVector(minePos_x,minePos_y,minePos_z));
  detector_phys->SetTranslation(G4ThreeVector(detectorPos_x,detectorPos_y,detectorPos_z));
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}
//*********Marzo 24 nuevo comando
void bsneutronDetectorConstruction::setTargetDepth(G4double newposition){
  //minePos_z=24.8*cm - newposition;
  //Julio 29-2008. Nueva posición. respecto al suelo si esta enterrada debajo de la capa de arena (ver bitácora)
  //G4cout<<"The width of the wet sand layer is "<<  G4BestUnit(Wet_Sand_width ,"Length")<<G4endl;
  //if(newposition>=Wet_Sand_width){
  //acrilico_phys->SetMotherLogical(soilBlock_log);
  //minePos_z=19.3*cm - newposition; //(19.3 comes from half the suelo height  (in z 21cm) minus  half the DLM2 height (1.7cm))
  //acrilico_phys->SetTranslation(G4ThreeVector(minePos_x,minePos_y,minePos_z)); 
  //}
  //else{
  //  minePos_x=newpositionX;
  minePos_z=(Wet_Sand_width/2)-newposition-1.7*cm;
  acrilico_phys->SetTranslation(G4ThreeVector(minePos_x,minePos_y,minePos_z));
  // }
  G4cout<<"La nueva posición z de la mina es: "<<  G4BestUnit(minePos_z,"Length")<<G4endl;
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
  
}

//********************Abril 1 ...comando para cambiar la altura del detector
void bsneutronDetectorConstruction::setDetectorHeight(G4double newposition){
  //detectorPos_z=27.75*cm+newposition;
  //nota 27.75 viene de la mitad de la caja de arena (26.5) + el radio de los detectores (1.25)...


  //Julio 29-2008. La nueva posición de la mina respecto a la caja de madera.
  //  detectorPos_z=newposition-1.73*cm;

  //Julio 29-2008. La nueva posición de la mina respecto a la caja de aire. (ver bitácora)
  detectorPos_z=newposition-23.73*cm;
 
  detector_phys->SetTranslation(G4ThreeVector(detectorPos_x,detectorPos_y,detectorPos_z));
  G4cout<<"La nueva altura de los detectores es: "<<  G4BestUnit(detectorPos_z,"Length")<<G4endl;
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}

void bsneutronDetectorConstruction::setXDetectorSeparation(G4double newposition){
  double x=newposition/2;
  ArrayBPos_x =-x-5.08*cm;
  ArrayAPos_x = x+5.08*cm;
  //  ArrayCPos_x = x+0.0*cm;
  //  ArrayDPos_x = x+0.0*cm;
  ArrayB_phys -> SetTranslation(G4ThreeVector(ArrayBPos_x,ArrayBPos_y,ArrayBPos_z)); 
  ArrayA_phys -> SetTranslation(G4ThreeVector(ArrayAPos_x,ArrayAPos_y,ArrayAPos_z)); 
  // ArrayC_phys -> SetTranslation(G4ThreeVector(ArrayCPos_x,ArrayCPos_y,ArrayDPos_z)); 
  //ArrayD_phys -> SetTranslation(G4ThreeVector(ArrayDPos_x,ArrayDPos_y,ArrayDPos_z)); 
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}



//March 28-2009
//To change the widht  and the material of the wet sand layer.

void bsneutronDetectorConstruction::setWet_Sand_material(G4String materialName){
  G4cout << "Antiguo Material: " << WetSand_log->GetMaterial()->GetName() <<G4endl;
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
    {
      WetSandMaterial  = pttoMaterial;
      WetSand_log->SetMaterial(pttoMaterial); 
    }
  G4cout << "Nuevo Material: " << WetSand_log->GetMaterial()->GetName() <<G4endl; 
  G4RunManager::GetRunManager()-> GeometryHasBeenModified(); 
}

void bsneutronDetectorConstruction::setWet_Sand_Width(G4double new_width){
  
  Wet_Sand_width=new_width;
  //First change the width of the defined box
  WetSand_box->SetZHalfLength(0.5*Wet_Sand_width);
  //As the width change it has to be changed the position with respect to the soil block.
  WetSand_phys->SetTranslation(G4ThreeVector(wetPos_x,wetPos_y,21*cm-0.5*Wet_Sand_width));
  
  G4cout<<"The new width of the wet sand layer is: "<<  G4BestUnit(new_width,"Length")<<G4endl;
  
  G4RunManager::GetRunManager()-> GeometryHasBeenModified();   
}








