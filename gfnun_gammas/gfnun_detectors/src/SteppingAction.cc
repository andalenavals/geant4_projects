/*! @file Stepping Action.cc
  @brief Definition of the class SteppingAction.
  @date December, 2010
  @author Pico (L.G. Sarmiento lgsarmientop@unal.edu.co)
  
  The SteppingAction is one of the 'optional' User Actions. It has
  been implemented to help the measuring times from the TRAPSpec
  experiment but is a general purpose class that comes VERY handy to
  control the tracks.
 */

#include "G4ios.hh"

#include "SteppingAction.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4Ions.hh"
#include "G4OpticalPhoton.hh"
#include "Interactions.hh"
#include "EventAction.hh"
#include <iostream>
#include "G4RunManager.hh"
using namespace CLHEP;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SteppingAction::SteppingAction():NumberOfInterstingGammaRays(6)
{ 
  print_flag=false; //default set to false unless modified in the macro file
  interaction_flag = false; //default set to false unless modified in the macro file
  Register_Int = new Interactions[NumberOfInterstingGammaRays];
  GammaAnnihilationCounter = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SteppingAction::UserSteppingAction(const G4Step* fStep) 
{
  G4Track*       fTrack = fStep->GetTrack();
  G4double         time = fStep->GetPreStepPoint()->GetGlobalTime();
  G4double   globaltime = fStep->GetPreStepPoint()->GetGlobalTime();
  G4double    localtime = fStep->GetPostStepPoint()->GetLocalTime();
  G4double     currentE = fStep->GetPreStepPoint()->GetKineticEnergy();
  G4double         edep = fStep->GetTotalEnergyDeposit ();
  G4int          StepNo = fTrack->GetCurrentStepNumber();
  G4String particleName = fTrack->GetDefinition()->GetParticleName();
  G4int      particleID = fTrack->GetTrackID();
  G4StepPoint* point1 = fStep->GetPreStepPoint();
  G4StepPoint* point2 = fStep->GetPostStepPoint();

  G4String VolumeName   = point1->GetTouchableHandle()->GetVolume()->GetName();
  G4bool      isPrimary = fTrack->GetParentID() == 0? true : false;  
  G4String CreatorProcessName = "none";
  G4String ProcessName  = point2->GetProcessDefinedStep()->GetProcessName();
  G4int fgamma_ID       =  0;

  /* avoid stuck tracks */
  if(StepNo >= 10000)
    fTrack->SetTrackStatus(fStopAndKill);
   
  if(VolumeName.contains("Photocathode") && fTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
      //G4RunManager* runManager = G4RunManager::GetRunManager();
      //G4cout<<fStep->GetPreStepPoint()->GetTotalEnergy()/eV<<" "<<runManager->GetCurrentEvent()->GetEventID()<<G4endl;
      
      fTrack->SetTrackStatus(fStopAndKill);

    }

  /*
  if( fTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) 
    {
      //G4cout<<currentE/eV<<G4endl;
      fTrack->SetTrackStatus(fStopAndKill);
    }
  */

  //**********************************************************************//
  //************ Begin: Volumes when Compton scatterings occur ***********// 
  //**********************************************************************//
 if(interaction_flag && particleName.contains("gamma"))
   {
     if(StepNo == 1)
       {
	 if(!isPrimary)
	   {
	     /// INITIALIZATION STEP
	     //!*** ---------- (beta+)-decay ------------- ***
	     //!*** ---  Positron source, example: Na22--- ***
	     /// Register_Int[0] --> 1275 keV gamma ray
	     /// Register_Int[1] --> 511 keV gamma ray (1)
	     /// Register_Int[2] --> 511 keV gamma ray (2)
	     /// Eventually positron creation from 1275 keV
	     /// Register_Int[3] --> 511 keV gamma ray (3)
	     /// Register_Int[4] --> 511 keV gamma ray (4)
	     //!*** --- Gamma-Decay source --- ***
	     /// Register_Int[0] --> Decay via gamma ray
	     CreatorProcessName = fTrack->GetCreatorProcess()->GetProcessName();
	  
	     if(CreatorProcessName == "annihil" )
	       {
		 G4int inner_id = GammaAnnihilationCounter+1;
		 Register_Int[inner_id].SetGammaID(pow(2,inner_id));
		 Register_Int[inner_id].SetGammaID_real(particleID);
		 GammaAnnihilationCounter++;
	       }
	     else if(CreatorProcessName == "RadioactiveDecay" )
	       {
		 Register_Int[0].SetGammaID(1);
		 Register_Int[0].SetGammaID_real(particleID);
	       }
	   }
	 else // Prymary particle::  Gamma source (primary)
	   {
	     //!*** --- Gamma source --- *** 
	     /// Register_Int[5] --> gamma ray (5)
	     Register_Int[5].SetGammaID(5);
	     Register_Int[5].SetGammaID_real(particleID);
	   }
       }
     //------------------------------------//
     //!*** --- register information --- ***
     //------------------------------------//
     G4int gamma_id =-1;
      for(G4int i=0;i<NumberOfInterstingGammaRays;i++)
     	if(particleID == Register_Int[i].GetGammaID_real()) //are we looking at one of OUR gamma-rays?
     	  gamma_id=i;
      
      if(gamma_id!=-1 && 
     	 Register_Int[gamma_id].vol_counter < Register_Int[gamma_id].NumberOfVolumes) //do not store more interactions than needed. For now is set to 30
     	{
	  //G4cout<<" Process name = "<<ProcessName<<G4endl; /// *** test ***
       	  if( ProcessName == "compt" ||  ProcessName == "Rayl" ) 
     	    {
     	      //Register_Int[gamma_id].globalTimePerVolume(Register_Int[gamma_id].vol_counter,globaltime/ns);
     	      Register_Int[gamma_id].localTimePerVolume(Register_Int[gamma_id].vol_counter,localtime/ns);
	      //**** Volumes ID ****//
	      // Sample (Object or/and Plate or/and Soil)        = 0
	      // Acrilyc capsule (Source Container)              = 1
	      // Other volumes (world, electronics, ... , etc)   = 2 
	      //     *different to crystals, sample and acrilyc capsule
	      // Any crystal (not distinguish the detector type) = 3

     	      if(VolumeName.contains("Object")|| 
     		 VolumeName.contains("Plate") || 
     		 VolumeName.contains("Soil"))
     		{
     		  Register_Int[gamma_id].AddComptonInteractionPerVolume(Register_Int[gamma_id].vol_counter,0);
     		  Register_Int[gamma_id].AddVolumeCounter();
     		}
	      else if(VolumeName.contains("CapsulaAcrilico"))
     		{
     		  Register_Int[gamma_id].AddComptonInteractionPerVolume(Register_Int[gamma_id].vol_counter,1);
     		  Register_Int[gamma_id].AddVolumeCounter();
     		}
     	      else if(VolumeName.contains("Crystal"))
     		{
     		  Register_Int[gamma_id].AddComptonInteractionPerVolume(Register_Int[gamma_id].vol_counter,3);
     		  Register_Int[gamma_id].AddVolumeCounter();
     		}
     	      else
     		{
     		  Register_Int[gamma_id].AddComptonInteractionPerVolume(Register_Int[gamma_id].vol_counter,2);
     		  Register_Int[gamma_id].AddVolumeCounter();
     		}
     	    } //non Compton then
     	  else 
	    {
	      if(edep>0.)
		{
		  //Register_Int[gamma_id].globalTimePerVolume(Register_Int[gamma_id].vol_counter,globaltime/ns);
		  Register_Int[gamma_id].localTimePerVolume(Register_Int[gamma_id].vol_counter,localtime/ns);
		  if(VolumeName.contains("Crystal"))
		    {
		      Register_Int[gamma_id].AddComptonInteractionPerVolume(Register_Int[gamma_id].vol_counter,3);
		      Register_Int[gamma_id].AddVolumeCounter();
		    }
		}
	    }
     	}
   }
  //********************************************************************//
  //************ End: Volumes when Compton scatterings occur ***********// 
  //********************************************************************//

  /* Print method for RadioactiveDecay purposes. */
  if(print_flag && StepNo == 1)
    {
      G4String particleType = fTrack->GetDefinition()->GetParticleType();
      G4bool      stability = fTrack->GetDefinition()->GetPDGStable();
      G4double     halfLife = fTrack->GetDefinition()->GetPDGLifeTime()*(std::log(2));
      if(!isPrimary && fTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") //Only printed decay products      
	{
	  G4cout<<"\t"<<setiosflags(std::ios::right)
		<<std::setw(15)<<particleName<<"["
		<<std::setw(7)<<G4BestUnit(currentE,"Energy")<<"] created at "
		<<std::setw(7)<<G4BestUnit(time,"Time")<<"(dt = "
		<<std::setw(7)<<G4BestUnit(time-iso_time,"Time")<<")";
	  if( particleType != "nucleus" || particleName == "alpha")
	    G4cout<<G4endl;
	  else
	    {
	      G4double excitationEnergy = ((const G4Ions*)(fTrack->GetDefinition()))->GetExcitationEnergy();
	      if( !stability )
		G4cout<<"\tUnstable, it should decay."<<G4endl;
	      else
		if(excitationEnergy > 0.)
		  G4cout<<"\tExcited, it should decay."<<G4endl;
		else
		  G4cout<<"\tStable, end of chain."<<G4endl;
	    }
	}
      if( particleType == "nucleus" && particleName != "alpha"  ) // primary particle AND isotopes
	{
	  iso_time = time;
	  if(isPrimary)
	    G4cout<<"###############################"<<G4endl
		  <<"###############################"<<G4endl
		  <<"PRIMARY:";
	  
	  G4cout<<"\nIsotope "
		<<std::setw(15)<<particleName<<" created at "
		<<std::setw(7)<<G4BestUnit(time,"Time")<<" -> ";
	  G4double excitationEnergy = ((const G4Ions*)(fTrack->GetDefinition()))->GetExcitationEnergy();
	  if( !stability )
	    G4cout<<"Unstable, it should decay. HalfLife = "
		  <<G4BestUnit(halfLife,"Time")<<G4endl;
	  else if(excitationEnergy > 0.)
	    G4cout<<"Excited, it should decay "<<G4endl;
	  else
	    G4cout<<"Stable, end of chain"<<G4endl;
	}
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
