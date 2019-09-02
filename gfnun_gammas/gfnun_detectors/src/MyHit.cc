/**
 * @file
 * @brief Implementation of user class MyHit.
*/

#include "MyHit.hh"
#include "G4SystemOfUnits.hh"
//using namespace CLHEP;

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<MyHit> MyHitAllocator;

MyHit::MyHit(const G4int Detector_ID, const G4bool isPrimary, const G4String name)
  : DetectorID(Detector_ID), DetectorName(name), isPrimary(isPrimary)// <<-- note BTW this is the only way to initialize a "const" member
{
  eDep     = 0.0;
}

MyHit::~MyHit()
{
}

void MyHit::Print()
{
  G4cout<<"Hit: DetectorID= "<<DetectorID<<"\t eDep= "<<eDep/keV<<" keV, isPrimary="<<(isPrimary?"true":"false")<<G4endl;
  G4cout<<"\tGlobal Time = "<<globalTime/ns<<"\tLocal Time = "<<localTime/ns<<G4endl<<G4endl;
}
