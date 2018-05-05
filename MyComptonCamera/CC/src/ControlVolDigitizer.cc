/**
 * @file  ControlVolDigitizer.cc
 * @brief Implements MyDigitizer class, i.e. the digitization process.
 *
 * @date 13 Oct 2015
 * @author D. Flechas
 */

#include "ControlVolDigitizer.hh"
#include "ControlVolDigit.hh"
#include "ControlVolHit.hh"
#include "NoiseGenerator.hh"
#include "MeV2ChargeConverter.hh"
#include "CrosstalkGenerator.hh"

/* no-geant4 classes*/
#include "G4DigiManager.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include <assert.h>
#include <list>
#include <map>
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

ControlVolDigitizer::ControlVolDigitizer(G4String aName) :
  G4VDigitizerModule(aName) ,
  //These two are names for digits and hits collections
  hitsCollName("controlVolHCollection") ,
  digiCollectionName("controlVolDCollection") ,
  // ************************************************* //
  // *** Digitization requires several components: *** //
  // ************************************************* //
  //1- A pedestal level
  pedestal(0.) ,

  //2- A noise generator: a simple Gaussian noise
  //Noise standard deviation is 1000 e
  //To turn it off put a value < 0
  noise( 1/(2*std::sqrt(2*std::log(2))) ) , // INPUT is sigma, hence with this sigma FWHM is = 1. 

  //3- MeV2Charge converter: converts energy deposits from MeV to Q
  //It needs a parameter: the MeV2Q conversion factor: 3.6 eV/e.
  //  convert( 1./(3.6*eV) ) ,
  convert( 1./(1*keV) ) ,
  //4- Crosstalk Generator:
  //Crosstalk needs two parameters: number of strips in each module
  //and fraction of charge that leaks.
  //To turn off crosstalk put 0.0
  crosstalk( 0.00 , 48 ), //also commented out the line MakeCrossTalk
  //UI cmds
  //  messenger(this)
  //5- Values per digit
  charge(0.0), localTime(0.0), globalTime(0.0), detectorID(0), gammaID(-1), detectorName(""), timeSignal(200),photoelectricEff(false),ComptonEff(false),PositionInt(0),DeltaMomentum(0)
{
  collectionName.push_back( digiCollectionName );
  ResetValues();
}


void ControlVolDigitizer::Digitize()
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  // Adding ID numbers

  //We search and retrieve the hits collection
  G4DigiManager* digMan = G4DigiManager::GetDMpointer();
  //digMan->SetVerboseLevel(1);
  //G4cout<<"controlVol digitizer in"<<G4endl;
  G4int HitCollID = digMan->GetHitsCollectionID( hitsCollName );//Number associated to hits collection names hitsCollName
  //G4cout<<"controlVol digitizaer out"<<G4endl;
  const ControlVolHitsCollection* hitCollection = static_cast<const ControlVolHitsCollection*>(digMan->GetHitsCollection(HitCollID));
  
  ControlVolDigitsCollection* digiCollection = 0;
  
  if ( hitCollection )
    {
      G4int hitCol_size = hitCollection->entries();
      //First we create a digits collection...
      digiCollection = new ControlVolDigitsCollection("ControlVolDigitizer",digiCollectionName);
      
      //Initialize with NULL pointers
      std::vector<ControlVolDigit*> digitsMap(hitCol_size);
      /***************************************************************/
      /* NOTE: since this is a vector we can always just append new
	 digits afterwards  */
      /***************************************************************/
      
      
      //G4cout<<"-------------"<<G4endl;
      //G4cout<<"Number of hits: "<<hitCol_size<<G4endl;

      /************************************************************************/
      /* How define each digit? 
         D.Flechas: 
	 1): each digit is defined by all depositions in a given detector 
	 between t=0(first deposition) and timeSignal 
         2): define one digit per each hit */
      /************************************************************************/
      /// *** Method 1 ***///

      ///DFlechas: I need to work in this method. The main problem is the number of different detectors, because hits may be not organized by physical volumen. We will need to define the number of detectors, identify them and generate each digit. 
      
      /// *** Method 2 ***///
      
      for (G4int i=0 ; i<hitCol_size; i++ )
	{

	  //For each Hit we now get to which strip it belongs
          //And convert its edep in charge units
          ControlVolHit* aHit = (*hitCollection)[i];
	  //	  aHit->Print();

	  ControlVolDigit* newDigi = new ControlVolDigit(i);       //Create empty digits
	  digitsMap[i] = newDigi;     
	  digiCollection->insert(newDigi);       //Now insert the digit in the digit collection

	  digitsMap[i]->SetDetectorName(aHit->GetPhysV()->GetName());
	  
	  digitsMap[i]->SetDetectorID(aHit->GetDetectorID());
	  digitsMap[i]->SetGammaID(aHit->GetGammaID());
	  
	  G4double edep = aHit->GetEdep();
          //Converter object accept MeV unit as input and returns the result in keV
          G4double Charge = convert( edep/MeV );
	  //	  G4cout<<"charge assignation NOW in keV ="<<charge<<G4endl;
	  
	  digitsMap[i]->SetCharge(Charge);
	  // * NEW - crossed information * //
	  digitsMap[i]->SetGlobalTime(aHit->GetGlobalTime());
	  digitsMap[i]->SetLocalTime(aHit->GetLocalTime());
	  digitsMap[i]->SetFlagPhotoelectric(aHit->GetFlagPhotoelectric());
	  digitsMap[i]->SetFlagCompton(aHit->GetFlagCompton());
	  digitsMap[i]->SetPosition(aHit->GetPos());
	  digitsMap[i]->SetMomentum(aHit->GetMomentum());
	  digitsMap[i]->SetEntries(aHit->GetEntries());
	 }
      
      
    }
  else //Something really bad happened...
    {
      G4cerr<<" ControlVolDigitizer::Digitize: Could not found Hit collection with name:"<<hitsCollName<<G4endl;
    }
  
  //We can now proceed simulating the crosstalk
  //  MakeCrosstalk( digitsMap );
  //G4cout<<"digiCollection->GetSize() = "<<digiCollection->GetSize()<<G4endl;
  //We can now add, for each detector the noise
  /*
  for ( size_t d = 0 ; d < digiCollection->GetSize(); ++d )
    {
      
      ControlVolDigit* digi = static_cast< ControlVolDigit* >( digiCollection->GetDigi(d) );
      //First we add a pedestal ZERO for now
      digi->Add( pedestal );
      G4int ID = digi->GetDetectorID();
      G4double FWHM = 1.;
      if( ! (digi->Zero(digi->GetCharge())) )
	//We do not want to handle noise
	{ 
	  
	  // ***** Detector ID ***** //
	  // dummy    = 0  
	  // Ge_10    = 1
	  // Ge_40    = 2
	  // NaI_2x2  = 3
	  // NaI_3x3  = 4
	  // Plastic  = 5
	  // CsI2x2   = 6
	  // BGO2x2   = 7
	  // *********************** //
	  
	  /// Is necessary check the FWHM values. by Ge (40%) -new detector- it is necessary to measure the resolution
	  if(ID == 0)
	    FWHM = 0.;
	  else if(ID == 1)
	    FWHM = (0.00043*digi->GetCharge())+0.88;
	  else if(ID == 2)
	    FWHM = 0.00075*digi->GetCharge()+1.24;
	  else if(ID == 3)
	    FWHM = 0.911983*pow(digi->GetCharge(),0.611976);
	  else if(ID == 4)
	    FWHM = 0.911983*pow(digi->GetCharge(),0.611976);
	  else if(ID == 5)
	    FWHM = 0.;
	  else if(ID == 6)
	    FWHM = 3.17605*pow(digi->GetCharge(),0.5);
	  else
	    {
	      G4Exception("Error:: My Digitizer", "Detector ID is not allowed",
			  FatalException," check the MyDigitizer.cc file!!");
	    }
	  // FWHM = 6.62*std::sqrt(digi->GetCharge()); /// CsI

	  //Then we smear for the noise
	  //The charge would be FWHM*(1keV-fwhm Gaussian)

	  //FWHM = 0.;// Ideal Detector: The FWHM can be itroduced in post-analysis (post-processing)

	  // * add noise but do not send negative energies * //
	  G4double smear;
	  do 
	    smear = FWHM*noise();
	  while((digi->GetCharge()+smear)<0 ) ;
	  digi->Add(smear);
	}
      else
	digi->SetCharge(0.);
      //      digi->Print();
    }
  */
  //This line is very important,
  //differently from hits we need to store the digits
  //each event explicitly.
  //This actually gives us quite a lot of flexibility
  //For example it is possible to simulate a malfunctioning
  //detector: you can comment this line and the digits
  //will not be available. Actually this example can be useful
  //for example if you want to study the effect of "dead" channels
  //in the physics measurement you perform.
  StoreDigiCollection(digiCollection);
}

void ControlVolDigitizer::MakeCrosstalk(std::vector< std::vector< ControlVolDigit*> >& digitsMap )
{
  //We need to work heavily on this routine 
  G4cout<<"ControlVolDigitizer::MakeCrosstalk"<<G4endl;
  //We have to make some conversions:
  //1- Take the digits of a detector. With subdetectors
  //2- Make a HepVector of the collected charges, ordered by Strip number
  //3- Apply transformation
  //4- Update digits with the new charge
  /*
    int plane = 1;
    std::vector< MyDigi* >& thisPlane = digitsMap[plane];
    size_t num_stripsY = thisPlane.size();
    size_t strip = 0;
    CLHEP::HepVector in(num_stripsY);
    for ( ; strip<num_stripsY;++strip )
    {
    in[strip] = thisPlane[strip]->GetCharge();
    }
    CLHEP::HepVector out = crosstalk( in );
    for ( strip=0; strip < num_stripsY ; ++strip )
    {
    thisPlane[strip]->SetCharge( out[strip] );
    }
  */
}

void ControlVolDigitizer::ResetValues(void)
{
  SetDetectorID(-1);
  SetCharge(0.0);
  SetLocalTime(0.0);
  SetGlobalTime(0.0);
  SetDetectorName("");
}
