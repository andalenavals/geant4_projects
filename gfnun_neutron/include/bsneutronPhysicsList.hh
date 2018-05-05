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

#ifndef bsneutronPhysicsList_h
#define bsneutronPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class bsneutronPhysicsList: public G4VUserPhysicsList
{
public:
  bsneutronPhysicsList();
  ~bsneutronPhysicsList();
public:  
  virtual void SetCuts();  
protected:
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructHad();
  virtual void ConstructOp();
  
  /*
  // these methods Construct particles 
 
  */
  virtual void AddTransportation();
  
private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4int VerboseLevel;
  G4int OpVerbLevel;


  void ConstructBosons();
  void ConstructLeptons();
  void ConstructHadrons();
};


#endif
