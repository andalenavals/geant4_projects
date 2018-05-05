/**
Este codigo esta basado en el ejemplo/data/data/Andres/Repos/command_source/Geant4/Ejemplos10x3/Geant4-10.3.0/extended/electromagnetic/TestEm0/DirectAccess.cc
 */

#include "MyAnalysisManager.hh"

#include "G4Material.hh"

#include "G4PEEffectFluoModel.hh" //Fotoelectrico
#include "G4KleinNishinaCompton.hh" //Compton
#include "G4BetheHeitlerModel.hh" //Conversion interna

#include "G4eeToTwoGammaModel.hh"

#include "G4MollerBhabhaModel.hh"
#include "G4SeltzerBergerModel.hh"

#include "G4BetheBlochModel.hh"
#include "G4BraggModel.hh"

#include "G4MuBetheBlochModel.hh"
#include "G4MuBremsstrahlungModel.hh"
#include "G4MuPairProductionModel.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4MuonPlus.hh"

#include "G4DataVector.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"

int main() {
  
  double minx, maxx;
  int nbins = pow(10,8);
  minx= pow(10,-4); maxx= pow(10, 4);
  double dx= (maxx-minx) / nbins;
  auto AM= MyAnalysisManager::Instance();
  AM->CreateTGraph(1, AM->Color(1) ); //Fotoelectrico
  AM->CreateTGraph(1, AM->Color(2) ); //Compton
  AM->CreateTGraph(1, AM->Color(3) ); //Pares
  AM->CreateTGraph(1, AM->Color(0) ); //Total
  
  G4UnitDefinition::BuildUnitsTable();

  G4ParticleDefinition* gamma = G4Gamma::Gamma();

  G4DataVector cuts;
  cuts.push_back(1*keV);

  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
  couple->SetIndex(0);

  // work only for simple materials
  G4double Z = material->GetZ();
  //G4double A = material->GetA(); //umas


  G4VEmModel* phot = new G4PEEffectFluoModel();
  G4VEmModel* comp = new G4KleinNishinaCompton();
  G4VEmModel* conv = new G4BetheHeitlerModel(); 
  phot->Initialise(gamma, cuts);
  comp->Initialise(gamma, cuts);
  conv->Initialise(gamma, cuts);

  // valid pointer to a couple is needed for this model
  phot->SetCurrentCouple(couple);

  // compute CrossSection per atom and MeanFreePath
  G4double Emin = minx*MeV, Emax = maxx*MeV, dz = dx*MeV;
  G4double E;  G4double CrossPhoto; G4double CrossCompt; G4double CrossConv;

  double z0= log10(Emin) ; double z;
  int i=1;
  for (E = Emin, z=z0 ; E <= Emax; z+=dz, i+=1, E = pow(10, z)  )
    {
      CrossPhoto = phot->ComputeCrossSectionPerAtom(gamma,E,Z);
      CrossCompt = comp->ComputeCrossSectionPerAtom(gamma,E,Z);
      CrossConv = conv->ComputeCrossSectionPerAtom(gamma,E,Z);
      AM->SetPointTGraph(0, i , E/MeV, CrossPhoto/millibarn);
      AM->SetPointTGraph(1, i , E/MeV, CrossCompt/millibarn);
      AM->SetPointTGraph(2, i , E/MeV, CrossConv/millibarn);
      AM->SetPointTGraph(3, i , E/MeV, (CrossConv +  CrossCompt + CrossPhoto )/millibarn );
    }

  //G4BestUnit (conv->ComputeCrossSectionPerAtom(gamma,Energy,Z),"Surface")
  //G4BestUnit (phot->ComputeMeanFreePath(gamma,Energy,material),"Length")

  AM->SetFrame( minx, 1,  maxx, pow(10,10) );
  AM->GetTGraph(0)->Draw("PLSAME");
  AM->GetTGraph(1)->Draw("PLSAME");
  AM->GetTGraph(2)->Draw("PLSAME");
  AM->GetTGraph(3)->Draw("PLSAME");
  AM->Print();


  
  
   
  return 0;
}

