#include "Materials.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
using namespace CLHEP;

Materials::Materials()
{
  nistMan = G4NistManager::Instance();
  nistMan->SetVerbose(2);
  CreateMaterials();
}

Materials::~Materials()
{
  delete    Wood;
  delete    CsI_Na;
  delete    CsI_Tl;
  delete    NaI_Tl;
  delete    Bialkali;
  delete    LYSO;
  delete    EPS;
}

Materials* Materials::instance = 0;

Materials* Materials::GetInstance()
{
  if (instance == 0)
    {
      instance = new Materials();
    }
  return instance;
}

G4Material* Materials::GetMaterial(const G4String material)
{
  G4Material* mat =  nistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {
     std::ostringstream o;
     o << "Material " << material << " not found!";
     G4Exception("Materials::GetMaterial","",
                 FatalException,o.str().c_str());
  }

  return mat;
}

void Materials::CreateMaterials()
{
  G4double density;
  G4int ncomponents;

  // Materials Definitions
  // =====================

  //--------------------------
  // wood
  //--------------------------
  density=0.9*g/cm3;
  Wood = new G4Material("Wood", density,3);
  Wood->AddElement(nistMan->FindOrBuildElement("H") , 4);
  Wood->AddElement(nistMan->FindOrBuildElement("O") , 1);
  Wood->AddElement(nistMan->FindOrBuildElement("C") , 2);
  //--------------------------------------------------
  // Expanden polystyrene (EPS)
  //--------------------------------------------------
  density   = 0.020*g/cm3;
  EPS = new G4Material("EPS",density,1);
  EPS->AddMaterial(nistMan->FindOrBuildMaterial("G4_POLYSTYRENE"),100.0*perCent);
  
  //--------------------------------------------------
  // Sodium Iodide doped with Thallium to 0.03 % (is necessary check this value)
  //--------------------------------------------------
  density   = 3.667*g/cm3;
  NaI_Tl = new G4Material("NaI_Tl",density,2);
  NaI_Tl->AddMaterial(nistMan->FindOrBuildMaterial("G4_SODIUM_IODIDE"),99.97*perCent);
  NaI_Tl->AddMaterial(nistMan->FindOrBuildMaterial("G4_Tl"),0.03*perCent);

  //--------------------------------------------------
  // Cesium Iodide doped with Sodium to 0.02 % ( see "Kinetic and scintillation characteristics of CsI(Na) crystals grown under melt mixing"), CsI(Na)
  //--------------------------------------------------

  density   = 4.51*g/cm3;
  CsI_Na    = new G4Material ("CsI_Na",density,ncomponents=2);

  CsI_Na->AddMaterial(nistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE"),99.98*perCent);
  CsI_Na->AddMaterial(nistMan->FindOrBuildMaterial("G4_Na"),0.02*perCent);

  //--------------------------------------------------
  // Cesium Iodide doped with Thallium to 0.03 % ( see "Scintillation characteristics of pure and Tl-doped CsI crystals"), CsI(Tl)
  //--------------------------------------------------

  density   = 4.51*g/cm3;
  CsI_Tl    = new G4Material ("CsI_Tl",density,ncomponents=2);

  CsI_Tl->AddMaterial(nistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE"),99.97*perCent);
  CsI_Tl->AddMaterial(nistMan->FindOrBuildMaterial("G4_Tl"),0.03*perCent);

  //--------------------------------------------------
  // Bialkali K2CsSb (Optical properties of an EMI K2CsSb bialkali photocathode)
  //-------------------------------------------------

  density   = 10.0*g/cm3; // arbitrary value

  Bialkali  = new G4Material ("BIALKALI",density,3);

  Bialkali->AddElement(nistMan->FindOrBuildElement("K") , 2);
  Bialkali->AddElement(nistMan->FindOrBuildElement("Cs") , 1);
  Bialkali->AddElement(nistMan->FindOrBuildElement("Sb") , 1);
  
  //--------------------------------------------------
  ///  LYSO
  //--------------------------------------------------
  
  density=7.4*g/cm3;
								     
  G4Material* Scint_mat = new G4Material("Scint", density,ncomponents=4);
  Scint_mat->AddMaterial(nistMan->FindOrBuildMaterial("G4_Lu"),71*perCent);
  Scint_mat->AddMaterial(nistMan->FindOrBuildMaterial("G4_Si"), 7*perCent);
  Scint_mat->AddMaterial(nistMan->FindOrBuildMaterial("G4_O"), 18*perCent);
  Scint_mat->AddMaterial(nistMan->FindOrBuildMaterial("G4_Y"), 4*perCent);

  LYSO = new G4Material("LYSO", density,2);
  LYSO->AddMaterial(Scint_mat, 99.85*perCent);
  LYSO->AddMaterial(nistMan->FindOrBuildMaterial("G4_Ce"), 0.15*perCent);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //

  //--------------------------------------------------
  //  Sodium Iodide  Optical Properties, Refractive Index 
  //--------------------------------------------------
  
  const G4int nEntries_NaI_Tl = 169;

  G4double PhotonEnergy_NaI_Tl[nEntries_NaI_Tl] =
    { 2.401*eV,2.424*eV,2.441*eV,2.458*eV,2.476*eV,2.488*eV,2.5*eV,2.513*eV,2.525*eV,2.537*eV,2.55*eV,2.556*eV,2.569*eV,
      2.576*eV,2.589*eV,2.595*eV,2.602*eV,2.608*eV,2.622*eV,2.628*eV,2.635*eV,2.642*eV,2.649*eV,2.655*eV,2.662*eV,2.669*eV,
      2.676*eV,2.683*eV,2.69*eV,2.697*eV,2.704*eV,2.704*eV,2.712*eV,2.719*eV,2.726*eV,2.733*eV,2.74*eV,2.74*eV,2.748*eV,
      2.755*eV,2.763*eV,2.763*eV,2.77*eV,2.778*eV,2.785*eV,2.785*eV,2.793*eV,2.8*eV,2.8*eV,2.808*eV,2.816*eV,2.823*eV,
      2.823*eV,2.831*eV,2.839*eV,2.839*eV,2.847*eV,2.855*eV,2.855*eV,2.863*eV,2.863*eV,2.871*eV,2.879*eV,2.879*eV,2.887*eV,
      2.895*eV,2.895*eV,2.903*eV,2.903*eV,2.911*eV,2.92*eV,2.92*eV,2.928*eV,2.936*eV,2.936*eV,2.945*eV,2.945*eV,2.953*eV,
      2.962*eV,2.962*eV,2.97*eV,2.97*eV,2.979*eV,2.988*eV,2.988*eV,2.996*eV,2.996*eV,3.005*eV,3.014*eV,3.014*eV,3.023*eV,
      3.023*eV,3.032*eV,3.041*eV,3.041*eV,3.05*eV,3.05*eV,3.059*eV,3.068*eV,3.068*eV,3.077*eV,3.077*eV,3.086*eV,3.096*eV,
      3.096*eV,3.105*eV,3.114*eV,3.114*eV,3.124*eV,3.124*eV,3.133*eV,3.143*eV,3.143*eV,3.153*eV,3.162*eV,3.162*eV,3.172*eV,
      3.182*eV,3.182*eV,3.192*eV,3.202*eV,3.202*eV,3.212*eV,3.222*eV,3.222*eV,3.232*eV,3.242*eV,3.252*eV,3.252*eV,3.263*eV,
      3.273*eV,3.284*eV,3.284*eV,3.294*eV,3.305*eV,3.315*eV,3.326*eV,3.326*eV,3.337*eV,3.348*eV,3.359*eV,3.37*eV,3.381*eV,
      3.392*eV,3.403*eV,3.415*eV,3.426*eV,3.438*eV,3.449*eV,3.461*eV,3.472*eV,3.484*eV,3.496*eV,3.52*eV,3.532*eV,3.544*eV,
      3.557*eV,3.581*eV,3.594*eV,3.619*eV,3.645*eV,3.658*eV,3.684*eV,3.711*eV,3.738*eV,3.779*eV,3.807*eV,3.85*eV,3.908*eV};

  G4double RefractiveIndex_NaI_Tl[nEntries_NaI_Tl] =
    { 1.788, 1.789, 1.79, 1.791, 1.791, 1.792, 1.793, 1.793, 1.794, 1.795, 1.795, 1.796, 1.796, 
      1.797, 1.797, 1.798, 1.798, 1.798, 1.799, 1.8, 1.8, 1.8, 1.801, 1.801, 1.802, 1.802, 
      1.802, 1.803, 1.803, 1.804, 1.804, 1.804, 1.804, 1.805, 1.805, 1.806, 1.806, 1.806, 1.806, 
      1.807, 1.807, 1.807, 1.808, 1.808, 1.809, 1.809, 1.809, 1.81, 1.81, 1.81, 1.811, 1.811, 
      1.811, 1.812, 1.812, 1.812, 1.813, 1.813, 1.813, 1.814, 1.814, 1.814, 1.815, 1.815, 1.815, 
      1.816, 1.816, 1.816, 1.816, 1.817, 1.817, 1.817, 1.818, 1.818, 1.818, 1.819, 1.819, 1.819, 
      1.82, 1.82, 1.821, 1.821, 1.821, 1.822, 1.822, 1.822, 1.822, 1.823, 1.824, 1.824, 1.824, 
      1.824, 1.825, 1.825, 1.825, 1.826, 1.826, 1.827, 1.827, 1.827, 1.828, 1.828, 1.829, 1.829, 
      1.829, 1.83, 1.831, 1.831, 1.831, 1.831, 1.832, 1.833, 1.833, 1.834, 1.834, 1.834, 1.835, 
      1.836, 1.836, 1.837, 1.837, 1.837, 1.838, 1.839, 1.839, 1.84, 1.84, 1.841, 1.841, 1.842, 
      1.843, 1.844, 1.844, 1.845, 1.845, 1.846, 1.847, 1.847, 1.848, 1.849, 1.85, 1.851, 1.852, 
      1.853, 1.854, 1.855, 1.856, 1.857, 1.858, 1.859, 1.86, 1.861, 1.862, 1.864, 1.865, 1.866, 
      1.868, 1.87, 1.871, 1.874, 1.876, 1.877, 1.88, 1.883, 1.886, 1.89, 1.893, 1.898, 1.905};

  // G4double PhotonEnergy_NaI_Tl[nEntries_NaI_Tl];
  // for(G4int i=0;i<nEntries_NaI_Tl;i++ )
  //   {
  //     if(i<85)
  // 	PhotonEnergy_NaI_Tl[i]=2.987*eV;
  //     else
  // 	PhotonEnergy_NaI_Tl[i]=2.987*eV;
  //   }
  // G4double RefractiveIndex_NaI_Tl[nEntries_NaI_Tl];
  // for(G4int i=0;i<nEntries_NaI_Tl;i++ )
  //  RefractiveIndex_NaI_Tl[i]=1.824;
  

  //--------------------------------------------------
  //  Sodium Iodide NaI_Tl
  //--------------------------------------------------
  
  G4double Abs_NaI_Tl[nEntries_NaI_Tl];
  // Ansorption length = 38.1 cm 
  for(G4int i=0;i<nEntries_NaI_Tl;i++ )
    Abs_NaI_Tl[i]=4.0*m;

  G4double ScintilFast_NaI_Tl[nEntries_NaI_Tl];
  for(G4int i=0;i<nEntries_NaI_Tl;i++ )
    ScintilFast_NaI_Tl[i]=1.0;

  // Add entries into properties table
  G4MaterialPropertiesTable* MPT_NaI_Tl= new G4MaterialPropertiesTable();
  MPT_NaI_Tl->AddProperty("RINDEX",PhotonEnergy_NaI_Tl,RefractiveIndex_NaI_Tl,nEntries_NaI_Tl);
  MPT_NaI_Tl->AddProperty("ABSLENGTH",PhotonEnergy_NaI_Tl,Abs_NaI_Tl,nEntries_NaI_Tl);
  MPT_NaI_Tl->AddProperty("FASTCOMPONENT",PhotonEnergy_NaI_Tl, ScintilFast_NaI_Tl,nEntries_NaI_Tl);
  MPT_NaI_Tl->AddConstProperty("SCINTILLATIONYIELD",40./keV); /// 40/keV Sodium Iodide, see Knoll
  MPT_NaI_Tl->AddConstProperty("RESOLUTIONSCALE",0.5); /// Statistical fluctuation around SCINTILLATION YIELD value
  MPT_NaI_Tl->AddConstProperty("FASTTIMECONSTANT", 253.*ns);// 253 ns (Fast light of NaI(Na) crystals
  NaI_Tl->SetMaterialPropertiesTable(MPT_NaI_Tl);

  // Set the Birks Constant for the Polystyrene scintillator

  NaI_Tl->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  //--------------------------------------------------
  //  Cesium Iodide  Optical Properties, Refractive Index 
  //--------------------------------------------------
  
  const G4int nEntries_CsI = 50;
  
  G4double PhotonEnergy_CsI[nEntries_CsI] =
    {1.265*eV,1.292*eV,1.319*eV,1.348*eV,1.378*eV,
     1.409*eV,1.442*eV,1.476*eV,1.512*eV,1.550*eV,
     1.590*eV,1.631*eV,1.676*eV,1.722*eV,1.771*eV,
     1.824*eV,1.879*eV,1.938*eV,2.000*eV,2.067*eV,
     2.102*eV,2.138*eV,2.175*eV,2.214*eV,2.254*eV,
     2.296*eV,2.340*eV,2.385*eV,2.431*eV,2.840*eV,
     2.531*eV,2.583*eV,2.638*eV,2.697*eV,2.755*eV,
     2.818*eV,2.884*eV,2.952*eV,3.024*eV,3.100*eV,
     3.179*eV,3.263*eV,3.351*eV,3.444*eV,3.543*eV,
     3.647*eV,3.758*eV,3.875*eV,4.000*eV,4.133*eV};      
  
  G4double RefractiveIndex_CsI[nEntries_CsI] =
    { 1.758, 1.759, 1.760, 1.760, 1.761,
      1.762, 1.763, 1.764, 1.765, 1.766,
      1.767, 1.769, 1.770, 1.772, 1.774, 
      1.776, 1.778, 1.780, 1.783, 1.786, 
      1.787, 1.789, 1.791, 1.793, 1.795, 
      1.797, 1.799, 1.801, 1.804, 1.807, 
      1.810, 1.813, 1.816, 1.820, 1.824, 
      1.829, 1.833, 1.839, 1.844, 1.851, 
      1.858, 1.865, 1.874, 1.884, 1.894, 
      1.907, 1.921, 1.937, 1.956, 1.979};
  
  //--------------------------------------------------
  //  Cesium Iodide doped with Sodium, CsI(Na)
  //--------------------------------------------------
  
  G4double Abs_CsINa[nEntries_CsI] =
  {34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm};

  G4double ScintilFast_CsINa[nEntries_CsI] =
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

  // Add entries into properties table
  G4MaterialPropertiesTable* MPTCsI_Na= new G4MaterialPropertiesTable();
  MPTCsI_Na->AddProperty("RINDEX",PhotonEnergy_CsI,RefractiveIndex_CsI,nEntries_CsI);
  MPTCsI_Na->AddProperty("ABSLENGTH",PhotonEnergy_CsI,Abs_CsINa,nEntries_CsI);
  MPTCsI_Na->AddProperty("FASTCOMPONENT",PhotonEnergy_CsI, ScintilFast_CsINa,nEntries_CsI);
  MPTCsI_Na->AddConstProperty("SCINTILLATIONYIELD",41./keV); /// 41/keV (CsI(Tl), CsI(Na) Cesium Iodide Scintillation Material)
  MPTCsI_Na->AddConstProperty("RESOLUTIONSCALE",7.73); /// Statistical fluctuation around SCINTILLATION YIELD value // Reproduce the experimental resolution (FWHM) 100 keV < E < 1 MeV for the CsI(Tl) gfnun detector
  MPTCsI_Na->AddConstProperty("FASTTIMECONSTANT", 670.*ns); 
 
  CsI_Na->SetMaterialPropertiesTable(MPTCsI_Na);

  // Set the Birks Constant for the Polystyrene scintillator

  CsI_Na->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

 //--------------------------------------------------
  //  Cesium Iodide doped with Thallium, CsI(Tl)
  //--------------------------------------------------

  G4double Abs_CsITl[nEntries_CsI] =
  {34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,
   34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm,34.2*cm};

  G4double ScintilFast_CsITl[nEntries_CsI] =
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  
  // Add entries into properties table
  G4MaterialPropertiesTable* MPTCsI_Tl= new G4MaterialPropertiesTable();
  MPTCsI_Tl->AddProperty("RINDEX",PhotonEnergy_CsI,RefractiveIndex_CsI,nEntries_CsI);
  MPTCsI_Tl->AddProperty("ABSLENGTH",PhotonEnergy_CsI,Abs_CsITl,nEntries_CsI);
  MPTCsI_Tl->AddProperty("FASTCOMPONENT",PhotonEnergy_CsI, ScintilFast_CsITl,nEntries_CsI);
  MPTCsI_Tl->AddConstProperty("SCINTILLATIONYIELD",55/keV); /// 55/keV (CsI(Tl), CsI(Na) Cesium Iodide Scintillation Material)
  MPTCsI_Tl->AddConstProperty("RESOLUTIONSCALE",7.73); /// Statistical fluctuation around SCINTILLATION YIELD value // Reproduce the experimental resolution (FWHM) 100 keV < E < 1 MeV for the CsI(Tl) gfnun detector
  MPTCsI_Tl->AddConstProperty("FASTTIMECONSTANT", 1080.*ns);
 
  CsI_Tl->SetMaterialPropertiesTable(MPTCsI_Tl);

  // Set the Birks Constant for the Polystyrene scintillator

  CsI_Tl->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  //--------------------------------------------------
  //  Bialkali K2CsSb (Optical properties of an EMI K2CsSb bialkali photocathode)
  //--------------------------------------------------
   
  const G4int nEntries_Bialkali = 50;
  
  G4double PhotonEnergy_Bialkali[nEntries_Bialkali];

  for(G4int i=0;i < nEntries_Bialkali; i++)
    {
      // from 1.8 eV to 3.27 eV
      PhotonEnergy_Bialkali[i] = (0.03*i+1.8)*eV;
    }

  G4double RefractiveIndex_Bialkali[nEntries_Bialkali];
  for(G4int i=0;i < nEntries_Bialkali; i++)
    {
      // best values at wavelength = 440 nm (~2.816 eV): n = 2.7
      RefractiveIndex_Bialkali[i] = -1.4*pow((0.03*i+1.8),2)+6.4*(0.03*i+1.8)-4.1; 
      // G4cout<<(0.03*i+1.8)<<" "<<RefractiveIndex_Bialkali[i]<<G4endl;
    }
 
  G4double Abs_Bialkali[nEntries_CsI];
  for(G4int i=0;i < nEntries_Bialkali; i++)
    {
      Abs_Bialkali[i] = 0.001*nm;
    }

  // Add entries into properties table
  G4MaterialPropertiesTable* MPTBialkali= new G4MaterialPropertiesTable();
  MPTBialkali->AddProperty("RINDEX",PhotonEnergy_Bialkali,RefractiveIndex_Bialkali,nEntries_Bialkali);
  MPTBialkali->AddProperty("ABSLENGTH",PhotonEnergy_Bialkali,Abs_Bialkali,nEntries_Bialkali);
  Bialkali->SetMaterialPropertiesTable(MPTBialkali);
}
