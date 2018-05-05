#include "Material.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
using namespace CLHEP;

Material* Material::instance = 0;

Material::Material(void)
{
  G4ElementName.push_back("NULL");
  G4ElementName.push_back("G4_H") ; 
  G4ElementName.push_back("G4_He") ; 
  G4ElementName.push_back("G4_Li");
  G4ElementName.push_back("G4_Be");
  G4ElementName.push_back("G4_B");
  G4ElementName.push_back("G4_C");
  G4ElementName.push_back("G4_N");
  G4ElementName.push_back("G4_O");
  G4ElementName.push_back("G4_F");
  G4ElementName.push_back("G4_Ne");
  G4ElementName.push_back("G4_Na");
  G4ElementName.push_back("G4_Mg");
  G4ElementName.push_back("G4_Al");
  G4ElementName.push_back("G4_Si");
  G4ElementName.push_back("G4_P");
  G4ElementName.push_back("G4_S");
  G4ElementName.push_back("G4_Cl");
  G4ElementName.push_back("G4_Ar");
  G4ElementName.push_back("G4_K");
  G4ElementName.push_back("G4_Ca");
  G4ElementName.push_back("G4_Sc");
  G4ElementName.push_back("G4_Ti");
  G4ElementName.push_back("G4_V");
  G4ElementName.push_back("G4_Cr");
  G4ElementName.push_back("G4_Mn");
  G4ElementName.push_back("G4_Fe");
  G4ElementName.push_back("G4_Co");
  G4ElementName.push_back("G4_Ni");
  G4ElementName.push_back("G4_Cu");
  G4ElementName.push_back("G4_Zn");
  G4ElementName.push_back("G4_Ga");
  G4ElementName.push_back("G4_Ge");
  G4ElementName.push_back("G4_As");
  G4ElementName.push_back("G4_Se");
  G4ElementName.push_back("G4_Br");
  G4ElementName.push_back("G4_Kr");
  G4ElementName.push_back("G4_Rb");
  G4ElementName.push_back("G4_Sr");
  G4ElementName.push_back("G4_Y");
  G4ElementName.push_back("G4_Zr");
  G4ElementName.push_back("G4_Nb");
  G4ElementName.push_back("G4_Mo");
  G4ElementName.push_back("G4_Tc");
  G4ElementName.push_back("G4_Ru");
  G4ElementName.push_back("G4_Rh");
  G4ElementName.push_back("G4_Pd");
  G4ElementName.push_back("G4_Ag");
  G4ElementName.push_back("G4_Cd");
  G4ElementName.push_back("G4_In");
  G4ElementName.push_back("G4_Sn");
  G4ElementName.push_back("G4_Sb");
  G4ElementName.push_back("G4_Te");
  G4ElementName.push_back("G4_I");
  G4ElementName.push_back("G4_Xe");
  G4ElementName.push_back("G4_Cs");
  G4ElementName.push_back("G4_Ba");
  G4ElementName.push_back("G4_La");
  G4ElementName.push_back("G4_Ce");
  G4ElementName.push_back("G4_Pr");
  G4ElementName.push_back("G4_Nd");
  G4ElementName.push_back("G4_Pm");
  G4ElementName.push_back("G4_Sm");
  G4ElementName.push_back("G4_Eu");
  G4ElementName.push_back("G4_Gd");
  G4ElementName.push_back("G4_Tb");
  G4ElementName.push_back("G4_Dy");
  G4ElementName.push_back("G4_Ho");
  G4ElementName.push_back("G4_Er");
  G4ElementName.push_back("G4_Tm");
  G4ElementName.push_back("G4_Yb");
  G4ElementName.push_back("G4_Lu");
  G4ElementName.push_back("G4_Hf");
  G4ElementName.push_back("G4_Ta");
  G4ElementName.push_back("G4_W");
  G4ElementName.push_back("G4_Re");
  G4ElementName.push_back("G4_Os");
  G4ElementName.push_back("G4_Ir");
  G4ElementName.push_back("G4_Pt");
  G4ElementName.push_back("G4_Au");
  G4ElementName.push_back("G4_Hg");
  G4ElementName.push_back("G4_Tl");
  G4ElementName.push_back("G4_Pb");
  G4ElementName.push_back("G4_Bi");
  G4ElementName.push_back("G4_Po");
  G4ElementName.push_back("G4_At");
  G4ElementName.push_back("G4_Rn");
  G4ElementName.push_back("G4_Fr");
  G4ElementName.push_back("G4_Ra");
  G4ElementName.push_back("G4_Ac");
  G4ElementName.push_back("G4_Th");
  G4ElementName.push_back("G4_Pa");
  G4ElementName.push_back("G4_U");
  G4ElementName.push_back("G4_Np");
  G4ElementName.push_back("G4_Pu");
  G4ElementName.push_back("G4_Am");
  G4ElementName.push_back("G4_Cm");
  G4ElementName.push_back("G4_Bk");   
  G4ElementName.push_back("G4_Cf");
  CreateMaterials();
}

G4Material * Material::GetMaterial(const G4String name)
{
  G4Material* mat ;
  mat= nistMan->FindOrBuildMaterial(name); 
  if(name == "Wood") mat= Material::Wood;
  if(name == "Cement") mat= Material::Cement;
  if(name == "Lime") mat= Material::Lime;
  if(name == "Sand") mat= Material::Sand;
  if(name == "POP") mat= Material::POP;
  if(name == "Asphalt") mat= Material::Asphalt;
  
  if( !mat ) mat = G4Material::GetMaterial(name); 
  if( !mat )
    {
      std::ostringstream o;
      o << "Material " << name << "NOT FOUND!" ;
      G4Exception("Materials::GetMaterial","",FatalException,o.str().c_str());
    }

  return mat;
}

G4Material * Material::GetMaterial(int Z)
{
  G4Material* mat = nistMan->FindOrBuildMaterial(G4ElementName[Z]);
   if( !mat )
    {
      std::ostringstream o;
      o << "Material " << Z << "NOT FOUND!" ;
      G4Exception("Materials::GetMaterial","",FatalException,o.str().c_str());
    }

  return mat;
}


G4Material * Material::CreateMaterials()
{
  G4double density;
 
  //--------------------------------
  // wood
  //--------------------------------
  density = 0.9*g/cm3;
  Wood = new G4Material("Wood", density,3);
  Wood->AddElement(nistMan->FindOrBuildElement("H") , 4);
  Wood->AddElement(nistMan->FindOrBuildElement("O") , 1);
  Wood->AddElement(nistMan->FindOrBuildElement("C") , 2);

  //--------------------------------
  // Cement
  //--------------------------------
  density = 1.362*g/cm3;
  Cement = new G4Material("Cement", density ,9);
  Cement->AddElement(nistMan->FindOrBuildElement("O") , 35.63*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Al") , 2.62*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("K") , 0.82*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Na") , 0.16*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Si") , 9.09*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Ca") , 45.94*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Mg") , 2.19*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("S") , 1.32*perCent);
  Cement->AddElement(nistMan->FindOrBuildElement("Fe") , 2.23*perCent);

  //--------------------------------
  // Lime
  //--------------------------------
  density = 0.561*g/cm3; 
  Lime = new G4Material("Lime", density ,2);
  Lime->AddElement(nistMan->FindOrBuildElement("O") , 28.53*perCent);
  Lime->AddElement(nistMan->FindOrBuildElement("Ca") , 71.47*perCent);

  //--------------------------------
  // Sand
  //--------------------------------
  density = 1.586*g/cm3;
  Sand = new G4Material("Sand", density ,7);
  Sand->AddElement(nistMan->FindOrBuildElement("O"), 51.28*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Si"), 40.06*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Fe") , 3.6*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Mg") , 0.61*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Ca") , 0.26*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Al") , 3.56*perCent);
  Sand->AddElement(nistMan->FindOrBuildElement("Ti") , 0.63*perCent);

  //--------------------------------
  // POP
  //--------------------------------
  density = 0.785*g/cm3; 
  POP = new G4Material("POP", density ,4);
  POP->AddElement(nistMan->FindOrBuildElement("H") , 0.7*perCent);
  POP->AddElement(nistMan->FindOrBuildElement("Ca"), 27.61*perCent);
  POP->AddElement(nistMan->FindOrBuildElement("O") , 49.60*perCent);
  POP->AddElement(nistMan->FindOrBuildElement("S") , 22.09*perCent);

  
  //--------------------------------
  // Asphalt (Concrete)
  //--------------------------------
  density = 0.785*g/cm3;
  Asphalt = new G4Material("Asphalt", density ,2);
  Asphalt->AddMaterial(GetMaterial("POP"), 60*perCent);
  Asphalt->AddMaterial(GetMaterial("Sand"), 40*perCent);
  
  
}


Material * Material::Instance() 	
{
   if (instance == 0) 
     {
       instance = new Material();
     }
   
   return instance;
 }
