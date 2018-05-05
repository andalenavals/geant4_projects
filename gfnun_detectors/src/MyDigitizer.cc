/**
 * @file  MyDigitizer.cc
 * @brief Implements MyDigitizer class, i.e. the digitization process.
 *
 * @date 10 Dec 2009
 * @author adotti
 */

#include "G4DigiManager.hh"
#include "MyDigitizer.hh"
#include "MyDigi.hh"
#include "DetectorConstruction.hh"

#include "NoiseGenerator.hh"
#include "MeV2ChargeConverter.hh"
#include "CrosstalkGenerator.hh"
#include "G4RunManager.hh"
#include "MyHit.hh"
#include <assert.h>
#include <list>
#include <map>
using namespace CLHEP;
MyDigitizer::MyDigitizer(G4String aName) :
  G4VDigitizerModule(aName) ,
  //These two are names for digits and hits collections
  digiCollectionName("MyDigitCollection") ,
  hitsCollName("MyHitCollection") ,

  /*************************************************/
  /*** Digitization requires several components: ***/
  /*************************************************/
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
  messenger(this)
{
  optical_flag=false;
  collectionName.push_back( digiCollectionName );
}


void MyDigitizer::Digitize()
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  Detector =
    (DetectorConstruction*)(runManager->GetUserDetectorConstruction());
  optical_flag = Detector->GetOptical_flag();  
  // Adding ID numbers

  //We search and retrieve the hits collection
  G4DigiManager* digMan = G4DigiManager::GetDMpointer();
  G4int HitCollID = digMan->GetHitsCollectionID( hitsCollName );//Number associated to hits collection names hitsCollName
  const MyHitCollection* hitCollection = static_cast<const MyHitCollection*>(digMan->GetHitsCollection(HitCollID));
  
  MyDigiCollection* digiCollection = 0;
  

  if ( hitCollection )
    {
      G4int hitCol_size = hitCollection->entries();
      
      //First we create a digits collection...
      digiCollection = new MyDigiCollection("MyDigitizer",digiCollectionName);
      
      //Initialize with NULL pointers
      std::vector<MyDigi*> digitsMap(hitCol_size);
      /***************************************************************/
      /* NOTE: since this is a vector we can always just append new
	 digits afterwards  */
      /***************************************************************/
      
      
      //G4cout<<"-------------"<<G4endl;
      //G4cout<<"Number of hits: "<<hitCol_size<<G4endl;
      for (G4int i=0 ; i<hitCol_size; i++ )
	{

	  //For each Hit we now get to which strip it belongs
          //And convert its edep in charge units
          MyHit* aHit = (*hitCollection)[i];
	  //	  aHit->Print();

	  MyDigi* newDigi = new MyDigi(i);       //Create empty digits
	  digitsMap[i] = newDigi;     
	  digiCollection->insert(newDigi);       //Now insert the digit in the digit collection

	  G4int detector_ID = aHit->GetDetectorID();
	  
	  digitsMap[i]->SetDetectorID(detector_ID);
	  G4double edep = aHit->GetEdep();
          //Converter object accept MeV unit as input and returns the result in keV
          G4double Charge = convert( edep/MeV );
	  //	  G4cout<<"charge assignation NOW in keV ="<<charge<<G4endl;
	  
	  digitsMap[i]->SetCharge(Charge);
	  
	  /* NEW - crossed information */
	  digitsMap[i]->SetGlobalTime(aHit->GetGlobalTime());
	  digitsMap[i]->SetLocalTime(aHit->GetLocalTime());
	  digitsMap[i]->SetDetectorName(aHit->GetDetectorName());
	}
    }
  else //Something really bad happened...
    {
      G4cerr<<"Could not found Hit collection with name:"<<hitsCollName<<G4endl;
    }
  
  //We can now proceed simulating the crosstalk
  //  MakeCrosstalk( digitsMap );
      //G4cout<<"digiCollection->GetSize() = "<<digiCollection->GetSize()<<G4endl;
  //We can now add, for each detector the noise
  for ( size_t d = 0 ; d < digiCollection->GetSize(); ++d )
    {
      
      MyDigi* digi = static_cast< MyDigi* >( digiCollection->GetDigi(d) );
      //First we add a pedestal ZERO for now
      digi->Add( pedestal );
      G4int ID = digi->GetDetectorID();
      G4double FWHM = 1.;
      if( ! (digi->Zero(digi->GetCharge())) )
	//We do not want to handle noise
	{ 
	  if(!optical_flag)
	    {
	      // ***** Detector ID ***** //
	      // dummy    = 0  
	      // Ge_10    = 1
	      // Ge_40    = 2
	      // NaI_2x2  = 3
	      // NaI_3x3  = 4
	      // Plastic  = 5
	      // CsI      = 6
	      // *********************** //

	      /// Is necessary check the FWHM values. by Ge (40%) -new detector- it is necessary to measure the resolution
	      if(ID == 0)
		FWHM = 0.;
	      else if(ID == 1)
		FWHM = (0.00043*digi->GetCharge())+0.88;
	      else if(ID == 2)
		FWHM = (0.141986*pow(digi->GetCharge(),0.5))+1.52;
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

	      /* add noise but do not send negative energies */
	      G4double smear;
	      do 
		smear = FWHM*noise();
	      while((digi->GetCharge()+smear)<0 ) ;
	      digi->Add(smear);
	    }
	}
      else
	digi->SetCharge(0.);
      //      digi->Print();
    }
  
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

void MyDigitizer::MakeCrosstalk(std::vector< std::vector< MyDigi* > >& digitsMap )
{
  //We need to work heavily on this routine 
  G4cout<<"MyDigitizer::MakeCrosstalk"<<G4endl;
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
