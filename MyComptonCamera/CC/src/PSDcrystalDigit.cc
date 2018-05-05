/**
 * @file PSDcrystalDigit.cc
 * @brief implements PSDcrystalDigit detector output class
 *
 * @date 10 Dec 2009, may 2016
 * @author adotti, modified by D. Flechas
 */

#include "PSDcrystalDigit.hh"
#include "CLHEP/Random/RandFlat.h"
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// -- one more nasty trick for new and delete operator overloading:
G4Allocator<PSDcrystalDigit> PSDcrystalDigitsAllocator;

PSDcrystalDigit::PSDcrystalDigit(const int& detector_ID) :
		DetectorID(detector_ID) ,
		isPrimary(false)
{
  charge=0.0;
  DetectorName = "";
}

bool PSDcrystalDigit::Zero( const G4double& aCharge ,G4bool DoIt)
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

void PSDcrystalDigit::Print()
{
  if(charge!=0)
  G4cout<<"Digit: DetectorID = "<<DetectorID
	<<"\n GammaID = "<<GammaID
    	<<"\n\t with Name = "<<DetectorName
	<<"\n\t with Charge = "<<charge
	<<"\t globaltime = "<<globalTime
	<<"\t localTime = "<<localTime
	<<"\t photoelectric = "<<photoelectric
	<<G4endl;
}
