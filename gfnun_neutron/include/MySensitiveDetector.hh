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
// $Id: UVA_TrackerSD.hh,v 1.1 2005/10/18 18:09:14 allison Exp $
// GEANT4 tag $Name: geant4-08-00-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MySensitiveDetector_h
#define MySensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "MyHit.hh"
#include "WriteToEntuple.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
  MySensitiveDetector(G4String);
  ~MySensitiveDetector();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
 
  writeToNtuple myNtuple;

  
  
  //writeToNtuple myNtupleFast;
  
  //writeToNtuple myNtupleGammas;
  
  
private:
  // MyHitsCollection* sensitiveVolume;
  MyHitsCollection* hitscollection;
  G4int HCID;
  // extras
  G4double sum_edep; // to get a sum of the deposit energy in a sensitive volume
  G4double firstHit_edep; // to get a sum of the deposit energy in a sensitive volume
  G4double stepLength; // to get the distance
  G4double pathLength; // to get the distance
  G4int firstHit; // to get the distance
  G4int NbtotalHits;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

