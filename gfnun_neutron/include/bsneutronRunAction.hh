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
// $Id: ExN02RunAction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........ooo........oooOO0OOooo........oooOO0OOooo......
#ifndef bsneutronRunAction_h
#define bsneutronRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "bsneutronDetectorConstruction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class bsneutronRunAction : public G4UserRunAction
{
public:
  bsneutronRunAction();
  
  //agrege el virtual 03/14/07... 
  //virtual ~bsneutronRunAction();
  ~bsneutronRunAction();
  //agregue el metodo generate run.....03/14/07
  //virtual  G4Run* GenerateRun();
  //  G4Run* GenerateRun();
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  //MAYO 4.....
  void ContarHits(G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int);
private:
  G4int DiferenciadeHits1;
  G4int DiferenciadeHits2;
  G4int DiferenciadeHits3;
  G4int DiferenciadeHits4;
  
  G4int NHits1;
  G4int NHits2;
  G4int NHits3;
  G4int NHits4;
  G4int NHits5;
  G4int NHits6;
  G4int NHits7;
  G4int NHits8;
  
  bsneutronDetectorConstruction* myDetector;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif





