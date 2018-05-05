/**
 * @file   PhysicsList.cc
 *
 * @date   1 Jun 2010
 * @author Pico
 *
 * @brief  Physics list: particle definitions and processes.
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "OpticalPhysics.hh"
#include "PhysListHadron.hh"

#include "PhysicsListMessenger.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

// particles
#include "PhysListParticles.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "QGSP_BERT_HP.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : 
  G4VModularPhysicsList(),
  emPhysicsList(0),
  raddecayList(0),
  particleList(0),
  hadPhysicsList(0),
  opticalPhysicsList(0)
{
  G4LossTableManager::Instance();
  defaultCutValue = 1.*mm;
  /* PK */ 
  cutForGamma     = 10e-6*mm; /* 10e-2 still produces X-rays but there is no real performance penalty, so on the safe side */
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  cutForProton    = 0.00001*mm;

  DetectorCuts = 0;
  TargetCuts   = 0;

  AbsorptionOn = true;

  pMessenger = new PhysicsListMessenger(this);

  SetVerboseLevel(1);

  //decay physics
  particleList = new G4DecayPhysics();
  //RegisterPhysics(new G4DecayPhysics());
  //default physics
  raddecayList = new G4RadioactiveDecayPhysics();
  //RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  emPhysicsList = new G4EmStandardPhysics();
  //emPhysicsList = new G4EmLivermorePhysics();
  //emPhysicsList = new G4EmPenelopePhysics();
  
  // Optical Physics
  opticalPhysicsList = new OpticalPhysics(AbsorptionOn);

  // Had physics // including neutrons
  hadPhysicsList = 0;
  nhadcomp = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
  delete pMessenger;
  delete raddecayList;
  delete emPhysicsList;
  delete opticalPhysicsList;
  if (hadPhysicsList) delete hadPhysicsList;
  if (nhadcomp > 0) {
    for(G4int i=0; i<nhadcomp; i++) {
      delete hadronPhys[i];
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  particleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  // em
  emPhysicsList->ConstructProcess();
  // decays
  particleList->ConstructProcess();
  raddecayList->ConstructProcess();
  // optical
  opticalPhysicsList->ConstructProcess();
  // had // including neutrons
  if (nhadcomp > 0) {
    for(G4int i=0; i<nhadcomp; i++) {
      (hadronPhys[i])->ConstructProcess();
    }
  }
  if (hadPhysicsList) 
    {
      G4cout<<"Y entra como debe ser!!! pero sigue sin funcionar la fisica"<<G4endl;
      hadPhysicsList->ConstructProcess();
    }
  G4cout << "### PhysicsList::ConstructProcess is done" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SelectPhysicsList(const G4String& name)
{
  G4cout << "HABER SI HP ENTRA <" << name << ">" << G4endl;
  if (verboseLevel>1)
  {
    G4cout << "PhysicsList::SelectPhysicsList: <" << name << ">" << G4endl;
  }
  // default  Had physics
  G4VModularPhysicsList* phys = NULL;
  if (name == "Hadron" && !hadPhysicsList) {
    hadPhysicsList = new PhysListHadron("hadron");
  } else if (name == "QGSP_BERT_HP"  && !hadPhysicsList) {
    //AddExtraBuilders(true);
    //phys = new QGSP_BERT_HP;
    G4cout<<"GRAN HP!!! si se mete pero no funciona!!!!"<<G4endl;
    hadPhysicsList = new G4HadronPhysicsQGSP_BERT_HP(1);
  } else if (name == "Standard_EM") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics();
  } else if (name == "LowE_Penelope") {
    delete emPhysicsList;
    emPhysicsList = new G4EmPenelopePhysics();
  } else if (name == "LowE_Livermore") {
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePhysics();
  } else {
    G4cout << "PhysicsList WARNING wrong or unknown <" 
	   << name << "> Physics " << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddExtraBuilders(G4bool flagHP)
{
  /*
  nhadcomp = 3;
  hadronPhys.push_back( new G4EmExtraPhysics("extra EM"));
  //  hadronPhys.push_back( new G4HadronElasticPhysics("elastic",verboseLevel,flagHP));
  //  hadronPhys.push_back( new G4QStoppingPhysics("stopping",verboseLevel));
  hadronPhys.push_back( new G4IonBinaryCascadePhysics("ionBIC"));
  hadronPhys.push_back( new G4NeutronTrackingCut("Neutron tracking cut"));
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4double lowlimit=20*eV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit, 100.*GeV);
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(cutForProton, "proton");
  G4cout << "World cuts are set" << G4endl;

  if( !DetectorCuts ) SetDetectorCut(cutForElectron);
  G4Region* region = (G4RegionStore::GetInstance())->GetRegion("Detector");
  region->SetProductionCuts(DetectorCuts);
  G4cout << "Detector cuts are set" << G4endl;

  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForProton(G4double cut)
{
  cutForProton = cut;
  SetParticleCuts(cutForProton, G4Proton::Proton());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetTargetCut(G4double cut)
{
  if( !TargetCuts ) TargetCuts = new G4ProductionCuts();

  TargetCuts->SetProductionCut(cut, idxG4GammaCut);
  TargetCuts->SetProductionCut(cut, idxG4ElectronCut);
  TargetCuts->SetProductionCut(cut, idxG4PositronCut);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetDetectorCut(G4double cut)
{
  if( !DetectorCuts ) DetectorCuts = new G4ProductionCuts();

  DetectorCuts->SetProductionCut(cut, idxG4GammaCut);
  DetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
  DetectorCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
