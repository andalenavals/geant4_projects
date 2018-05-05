//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: UVA_TrackerSD.cc,v 1.1 2005/10/18 18:09:14 allison Exp $
// GEANT4 tag $Name: geant4-08-00-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MySensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

#include "G4TouchableHistory.hh"


//para crear las ntuplas
//#include "WriteToEntuple.hh"

//#include "G4UImanager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MySensitiveDetector::MySensitiveDetector(G4String name)
  :G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="detectorCol");
  HCID = -1;
  sum_edep = 0.0;
  firstHit_edep = 0.0;
  stepLength = 0.0;
  pathLength = 0.0;
  
  G4cout << "SD Constructor:" << G4endl;
  firstHit = 0;
  NbtotalHits=0;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MySensitiveDetector::~MySensitiveDetector(){ 
  
  //myNtuple.closeNtuple();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
  hitscollection = new MyHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
    { //HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitscollection); }
  HCE->AddHitsCollection( HCID, hitscollection ); 
  
  // Each time it runs ::Initialize I clear up this variable.
  // It won't call the constructor again, that's why I do it
  // here and make sure i get back to zero.
  sum_edep = 0.0;
  firstHit_edep = 0.0;

  stepLength = 0.0;
  pathLength = 0.0;
  
  firstHit = 0;
  
  //G4cout << "Init hit --> " << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  //G4double edep=aStep->GetDeltaEnergy();
  G4StepPoint* prePoint  = aStep->GetPreStepPoint();
  G4double E = prePoint->GetKineticEnergy();
  //G4double E =aStep->GetTrack()->GetKineticEnergy();
  //Esta vaina fue un intento para coger las particulas secundarias
  //G4cout<<aStep->GetSecondary()<<G4endl
  
  if(edep==0.) return false;
  
  //if(aStep->GetTrack()->GetTrackID()==1){
  //if(E<1e-6){//solo se generan los hits si la energia cinetica es mayor que 0.025 eV
  MyHit* newHit = new MyHit();
  newHit->SetEdep     (edep);
  //newHit->SetEdep     (E);
  newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
  //April 2-2009..new property of the Hit..the name of the particle that deposited the energy.
  G4String aName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  newHit->SetParticleName(aName);
  //April 4-2009
  newHit->SetKineticEnergy(E);  
  hitscollection->insert( newHit );
  //}
  //}
  // este valor viene en milimetros
  stepLength = aStep->GetStepLength();
  pathLength += stepLength;
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1) { 
    G4int NbHits = hitscollection->entries();
    G4cout << "\n-------->En este evento hubieron  " << NbHits 
	   << " hits en el volumen sensible: " << G4endl;
    for (G4int i=0;i<NbHits;i++){ 
      (*hitscollection)[i]->Print();
    }
    G4double Edep=0;
    for (G4int i=0;i<NbHits;i++){ 
      Edep+=(*hitscollection)[i]->GetEdep();
    }
    G4cout << "Total deposited Energy " <<G4BestUnit(Edep,"Energy")<<G4endl;
    
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

