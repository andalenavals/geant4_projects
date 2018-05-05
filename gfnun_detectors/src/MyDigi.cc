/**
 * @file MyDigi.cc
 * @brief implements MyDigi detector output class
 *
 * @date 10 Dec 2009
 * @author adotti
 */

#include "MyDigi.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// -- one more nasty trick for new and delete operator overloading:
G4Allocator<MyDigi> MyDigiAllocator;

MyDigi::MyDigi(const int& detector_ID) :
		DetectorID(detector_ID) ,
		isPrimary(false)
{
  charge=0.0;
  DetectorName = "";
}

bool MyDigi::Zero( const G4double& aCharge ,G4bool DoIt)
{
  //charge is implicitly in keV
  if( aCharge <= 0.) return true; //obvious test
  if(!DoIt) return false; //do not go any further

  /** low energy part of the code meant to be used only with the Ge-signals **/
  if( aCharge < 20 ) 
    return true;
  else if( ( aCharge > 20 && aCharge < (45+10) ) && //range to use the filter 
 	   ( ( CLHEP::RandFlat::shoot(0.,1.) > std::tan(((aCharge-10)*degree)/radian) ) ) )
    {
      return true;
    }
  else
    return false;
}

void MyDigi::Print()
{
  if(charge!=0)
  G4cout<<"Digit: DetectorID = "<<DetectorID
	<<"\n\t with Charge = "<<charge
	<<"\t globaltime = "<<globalTime
	<<"\t localTime = "<<localTime
	<<G4endl;
}
