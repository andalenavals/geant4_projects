//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Materials_h
#define Materials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class Materials
{
public:
  
  ~Materials();
  
  static Materials* GetInstance();
  
  G4Material* GetMaterial(const G4String);
  
private:
  
  Materials();
  
  void CreateMaterials();
  
private:
  
  static Materials* instance;
  
  G4NistManager*     nistMan;
  
  G4Material*        Air;
  G4Material*        LN2;
  G4Material*        PMMA;
  G4Material*        Pethylene;
  G4Material*        FPethylene;
  G4Material*        Polystyrene;
  G4Material*        Silicone;
  G4Material*        Coating;
  G4Material*        CsI_Na;
  G4Material*        CsI_Tl;
  G4Material*        NaI_Tl;
  G4Material*        Bialkali;
  G4Material*        LYSO;
  G4Material*        Wood;
};

#endif /*Materials_h*/
