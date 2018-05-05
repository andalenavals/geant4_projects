/**
Este codigo esta basado en el ejemplo/data/data/Andres/Repos/command_source/Geant4/Ejemplos10x3/Geant4-10.3.0/extended/electromagnetic/TestEm0/DirectAccess.cc
 */

#include "MyAnalysisManager.hh"

#include "G4Material.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"

int main() {

  vector <const char *> Elements;
  Elements.push_back("Null");
  Elements.push_back("G4_H") ; 
  Elements.push_back("G4_He") ; 
  Elements.push_back("G4_Li");
  Elements.push_back("G4_Be");
  Elements.push_back("G4_B");
  Elements.push_back("G4_C");
  Elements.push_back("G4_N");
  Elements.push_back("G4_O");
  Elements.push_back("G4_F");
  Elements.push_back("G4_Ne");
  Elements.push_back("G4_Na");
  Elements.push_back("G4_Mg");
  Elements.push_back("G4_Al");
  Elements.push_back("G4_Si");
  Elements.push_back("G4_P");
  Elements.push_back("G4_S");
  Elements.push_back("G4_Cl");
  Elements.push_back("G4_Ar");
  Elements.push_back("G4_K");
  Elements.push_back("G4_Ca");
  Elements.push_back("G4_Sc");
  Elements.push_back("G4_Ti");
  Elements.push_back("G4_V");
  Elements.push_back("G4_Cr");
  Elements.push_back("G4_Mn");
  Elements.push_back("G4_Fe");
  Elements.push_back("G4_Co");
  Elements.push_back("G4_Ni");
  Elements.push_back("G4_Cu");
  Elements.push_back("G4_Zn");
  Elements.push_back("G4_Ga");
  Elements.push_back("G4_Ge");
  Elements.push_back("G4_As");
  Elements.push_back("G4_Se");
  Elements.push_back("G4_Br");
  Elements.push_back("G4_Kr");
  Elements.push_back("G4_Rb");
  Elements.push_back("G4_Sr");
  Elements.push_back("G4_Y");
  Elements.push_back("G4_Zr");
  Elements.push_back("G4_Nb");
  Elements.push_back("G4_Mo");
  Elements.push_back("G4_Tc");
  Elements.push_back("G4_Ru");
  Elements.push_back("G4_Rh");
  Elements.push_back("G4_Pd");
  Elements.push_back("G4_Ag");
  Elements.push_back("G4_Cd");
  Elements.push_back("G4_In");
  Elements.push_back("G4_Sn");
  Elements.push_back("G4_Sb");
  Elements.push_back("G4_Te");
  Elements.push_back("G4_I");
  Elements.push_back("G4_Xe");
  Elements.push_back("G4_Cs");
  Elements.push_back("G4_Ba");
  Elements.push_back("G4_La");
  Elements.push_back("G4_Ce");
  Elements.push_back("G4_Pr");
  Elements.push_back("G4_Nd");
  Elements.push_back("G4_Pm");
  Elements.push_back("G4_Sm");
  Elements.push_back("G4_Eu");
  Elements.push_back("G4_Gd");
  Elements.push_back("G4_Tb");
  Elements.push_back("G4_Dy");
  Elements.push_back("G4_Ho");
  Elements.push_back("G4_Er");
  Elements.push_back("G4_Tm");
  Elements.push_back("G4_Yb");
  Elements.push_back("G4_Lu");
  Elements.push_back("G4_Hf");
  Elements.push_back("G4_Ta");
  Elements.push_back("G4_W");
  Elements.push_back("G4_Re");
  Elements.push_back("G4_Os");
  Elements.push_back("G4_Ir");
  Elements.push_back("G4_Pt");
  Elements.push_back("G4_Au");
  Elements.push_back("G4_Hg");
  Elements.push_back("G4_Tl");
  Elements.push_back("G4_Pb");
  Elements.push_back("G4_Bi");
  Elements.push_back("G4_Po");
  Elements.push_back("G4_At");
  Elements.push_back("G4_Rn");
  Elements.push_back("G4_Fr");
  Elements.push_back("G4_Ra");
  Elements.push_back("G4_Ac");
  Elements.push_back("G4_Th");
  Elements.push_back("G4_Pa");
  Elements.push_back("G4_U");
  Elements.push_back("G4_Np");
  Elements.push_back("G4_Pu");
  Elements.push_back("G4_Am");
  Elements.push_back("G4_Cm");
  Elements.push_back("G4_Bk");   
  Elements.push_back("G4_Cf");
  
  

  auto AM= MyAnalysisManager::Instance();
  AM->CreateTGraph(1, AM->Color(1) ); //Fotoelectrico
  // AM->SetFrame( 0, pow(10,15),  100, pow(10,21) );//n
  AM->SetFrame( 0, pow(10,19),  100, 5*pow(10,23) ); //Vm

  
  G4UnitDefinition::BuildUnitsTable();

  /*
  G4double density, a, temperature, pressure;
  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  pressure    = 100000000*atmosphere;
  temperature = 500.*kelvin;
  G4Material* Al = new G4Material("Aluminum", 13., a, density,kStateGas,temperature,pressure  );
  */
  int i=1;   G4double n, Vm;
  for (int k = 1; k <= 98; ++k)
    {
      // if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
      G4Material* Tem = G4NistManager::Instance()->FindOrBuildMaterial(Elements[k]);
      n = Tem->GetTotNbOfAtomsPerVolume();
      Vm = Avogadro/n;
      G4cout<<k<<" "<<n*cm3<<" "<<Vm/cm3 <<G4endl;
      AM->SetPointTGraph(0, i-1 , k , n*cm3);
      i+=1;
    }


 
  AM->GetTGraph(0)->Draw("PLSAME");
  AM->Print();
  
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  /*
  G4double Z = Al->GetZ();
  G4double A = Al->GetA(); //umas
  G4double n = Al->GetTotNbOfAtomsPerVolume();
  */
  
  // G4cout <<Avogadro*density/A<<" "<<Z<<" "<< G4BestUnit( A , "Mass") <<" "<<n<<G4endl;
  //G4cout <<Z<<" "<< G4BestUnit( A , "Mass") <<" "<<n<<G4endl;
  

  
 
  


  
  
   
  return 0;
}

