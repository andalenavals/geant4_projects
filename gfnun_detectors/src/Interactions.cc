/* ! @file Interactions.cc
   @brief Definition of the classnteractions.
   @date July, 2012
   @author Flechas (D. C. Flechas dcflechasg@unal.edu.co)

Is has been implemented to help the register of interest fenomena
in volumens different to sensitive volumes
 */

#include "G4ios.hh"
#include "globals.hh"
#include <iostream>
#include "Interactions.hh"

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

Interactions::Interactions()
{
  Num_Compton_Int = 0;
  gamma_ID = -1;
  gamma_ID_real = -1;
  NumberOfVolumes = 30;
  Volume = new G4int [NumberOfVolumes];
  globalTime = new G4double [NumberOfVolumes];
  localTime = new G4double [NumberOfVolumes];
  for(G4int i=0;i<NumberOfVolumes;i++ )
    {
      Volume[i]=-1;
      globalTime[i]=0.;
      localTime[i]=0.;
    }
  vol_counter=0;
  good_event_flag=false;
}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

Interactions::~Interactions()
{
  delete Volume;
}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::AddComptonInteractionPerVolume(G4int pos,G4int vol)
{
  Volume[pos]=vol;

}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::globalTimePerVolume(G4int pos, G4double gtime)
{
  globalTime[pos]=gtime;
}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::localTimePerVolume(G4int pos, G4double ltime)
{
  localTime[pos]=ltime;
}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::Clear(void)
{
  Num_Compton_Int = 0;
  gamma_ID = -1;
  gamma_ID_real = -1;
 for(G4int i=0;i<NumberOfVolumes;i++ )
    {
      Volume[i]=-1;
      globalTime[i]=0.;
      localTime[i]=0.;
    }
  vol_counter=0;
  good_event_flag=false;
}
// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::Analysis(void)
{
  /*
    Buried object (plates) or soil   == 0
    Backscattering detector (BSD)     == 1
    Sensitive position detector (SPD) == 2
    Acryic                            == 3
    BSD collider                      == 4
    SPD collider                      == 5
    Lead shielding                    == 6
    other volume                      == 7
  */
}

// ........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........oooOO0OOooo.........

void Interactions::Print(void)
{
  G4cout<<"Particle ID = "<<gamma_ID<<" Particle ID (real) = "<<gamma_ID_real<<G4endl;
  G4cout<<"Volumes when Compton interactions occur: (volume, local time [ns], global time [ns])"<<G4endl;
  for(G4int i=0;i<vol_counter;i++)
    {
      G4cout<<"("<<Volume[i]<<", "<<localTime[i]<<", "<<globalTime[i]<<")"<<G4endl;
    }
 G4cout<<"Number of volumes: "<<vol_counter<<G4endl;
  G4cout<<"The number of Compton interactions in Soil and/or burried Object were: "<<Num_Compton_Int<<G4endl;
}
