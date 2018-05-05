/**
 * @file ControlVolDigit.cc
 * @brief implements ControlVolDigit detector output class
 *
 * @date 13 Oct 2015
 * @author D. Flechas
 */

#include "ControlVolDigit.hh"
#include "CLHEP/Random/RandFlat.h"
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// -- one more nasty trick for new and delete operator overloading:
G4Allocator<ControlVolDigit> ControlVolDigitsAllocator;

ControlVolDigit::ControlVolDigit(const int& detector_ID) :
		DetectorID(detector_ID) ,
		FollowParticle(false)
{
  charge=0.0;
  DetectorName = "";
}

bool ControlVolDigit::Zero( const G4double& aCharge ,G4bool DoIt)
{
  //charge is implicitly in keV
  if( aCharge <= 0.) return true; //obvious test
  if(!DoIt) return false; //do not go any further

  /** low energy part of the code meant to be used only with the Ge-signals **/
  if( aCharge < 20 ) 
    return true;
  else if( ( aCharge > 20 && aCharge < (45+10) ) &&  //range to use the filter
  	   ( ( CLHEP::RandFlat::shoot(0.,1.) > std::tan(((aCharge-10)*degree)/radian) ) ) )
    {
      return true;
    }
  else
    return false;
}

void ControlVolDigit::Print()
{
  if(charge!=0)
  G4cout<<"Digit: DetectorID = "<<DetectorID
	<<"\n GammaID = "<<GammaID
    	<<"\n\t with Name = "<<DetectorName
	<<"\n\t with Charge = "<<charge
	<<"\t globaltime = "<<globalTime
	<<"\t localTime = "<<localTime
	<<"\t photoelectric = "<<photoelectric
	<<"\n\t compton = "<<compton
	<<"\n N compton = "<<entriesCompton
	<<"\n\t position = "<<Position
	<<"\n\t momentum = "<<Momentum
	<<G4endl;
}
