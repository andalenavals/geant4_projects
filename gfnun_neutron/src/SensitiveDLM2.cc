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

#include "SensitiveDLM2.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

#include "G4TouchableHistory.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDLM2::SensitiveDLM2(G4String name)
  :G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="DLM2collection");
  HCID = -1;
  G4cout << "Constructor Sensitive DLM2:1" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDLM2::~SensitiveDLM2(){ 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDLM2::Initialize(G4HCofThisEvent* HCE)
{
  // G4cout << "Constructor Sensitive DLM2:2" << G4endl;
  hitscollection = new HitDLM2sCollection(SensitiveDetectorName,collectionName[0]); 
  if(HCID<0){ 
    // G4cout << "Constructor Sensitive DLM2:3" << G4endl;
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitscollection);
  } 
  HCE->AddHitsCollection( HCID, hitscollection ); 
  //G4cout << "Constructor Sensitive DLM2:4" << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SensitiveDLM2::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4StepPoint* prePoint  = aStep->GetPreStepPoint();
  
  G4double E = prePoint->GetKineticEnergy();
  G4String Volume_name=prePoint->GetPhysicalVolume()->GetName();
  
  //December 10-2009 new property of the Hit..the name of the particle that deposited the energy.
  G4String aName = aStep->GetTrack()->GetDefinition()->GetParticleName();

  
  //if(edep==0) return false;
  //if(Volume_name!="WetSand") return false;
  
  
  HitDLM2* newHit = new HitDLM2();
  
  //fill the new hit with the properties...
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetHitVolume(Volume_name);
  newHit->SetKineticEnergy(E);  
  //December 10-2009 new property of the Hit..the name of the particle that deposited the energy.
  newHit->SetParticleName(aName);
  
  hitscollection->insert( newHit );
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SensitiveDLM2::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1) { 
    G4int NbHits = hitscollection->entries();
    G4cout << "\n-------->In this event  there was " << NbHits 
	   << " hits in the sensitive DLM2 volume " << G4endl;
    for (G4int i=0;i<NbHits;i++){ 
      (*hitscollection)[i]->Print();
    }
    
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

