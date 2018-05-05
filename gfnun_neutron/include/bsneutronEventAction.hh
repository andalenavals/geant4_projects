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
// $Id: ExN02EventAction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef bsneutronEventAction_h
#define bsneutronEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
//Mayo 4 sigo con lo de los hits
#include "bsneutronRunAction.hh"

//para las ntuplas de root
#include "WriteToEntuple.hh"

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class bsneutronEventAction : public G4UserEventAction
{
  public:
  bsneutronEventAction();
  ~bsneutronEventAction();
  //identidades de las colecciones de hists para cada detector....... Mayo 2
private:
  G4int HHC1ID;
  G4int HHC2ID;
  G4int HHC3ID;
  G4int HHC4ID;
  G4int HHC5ID;
  G4int HHC6ID;
  G4int HHC7ID;
  G4int HHC8ID;

  G4int HHC_DLM2ID;
  
  
  G4int n11;
  G4int n22;
  G4int n33;
  G4int n44;
  G4int n55;
  G4int n66;
  G4int n77;
  G4int n88;
  
  bsneutronRunAction* runAction;
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  
  //my ntuplas·.
  writeToNtuple myNtuple;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
