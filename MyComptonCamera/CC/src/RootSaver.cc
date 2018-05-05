
#include "RootSaver.hh"
#include "BSDcrystalDigit.hh"
#include "PSDcrystalDigit.hh"
#include "BSDcrystalHit.hh"
#include "PSDcrystalHit.hh"
#include "RunAction.hh"
#include "SetupConstruction.hh"
/* geant4 classes */
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
/*  units  and constants */
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
/* root classes */
#include <TH1I.h>
#include <TH2I.h>
#include <TH3I.h>
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>
/* other classes */
//#include <sstream>
//#include <cassert>

RootSaver::RootSaver()
  :fRootFile(0),
   Cal_Tree(0)
{
  NPixels=64;
  Npixels=64;
  
  Cal_Tree = 0;
  
  BSD_Energy_s = 0;
  Diff_localtime_s = 0;
  Total_hitPat = 0;
  PSD_2DHitPat = 0;
  //Control volume: Number of Compton interactions
  for(Int_t i = 0;i<4;i++)
    nc[i] = 0;
  NumberOfInteractions = 0;
  ControlVol_3DHitPat = 0;
  ControlVol_1DHitPat = 0;
  Energy_s_SS = 0;
  Energy_s_MS = 0;
  
  runManager = G4RunManager::GetRunManager();
  cal_event = new Cal_Event();
  cal_event->Clear();
  G4cout<<"sale const. root saver"<<G4endl;
}

RootSaver::~RootSaver()
{
  if ( fRootFile ) delete fRootFile;
}

void RootSaver::Book( const std::string& fileName , const std::string& treeName )
{
  std::ostringstream fn;
  G4int runID = runManager->GetCurrentRun()->GetRunID();
  fn << fileName << "_run" << runID << ".root";
  //Create a new file and open it for writing, if the file already exists the file
  //is overwritten
  fRootFile = new TFile(fn.str().data(),"RECREATE");
  //fRootFile = TFile::Open( fn.str().data() , "recreate" );
  if(!fRootFile || fRootFile->IsZombie() )
    {
      G4cout << " RootSaver::book :" 
	     << " problem creating the ROOT TFile "
	     << G4endl;
      return;
    }
  //SetupConstruction* Detector =
  //  (SetupConstruction*)(runManager->GetUserDetectorConstruction());
  //Int_t maxNumOfDetector = Detector->GetMaximumNumberOfDetectors();

  Int_t maxNumOfDetector = NPixels*NPixels+1;

  // ****************** //
  // Control histograms //
  // ****************** //
  // detectors //
  Total_hitPat = new TH1I("Total_hitPat","TOTAL hit pattern;ID;counts",maxNumOfDetector,
			  0-0.5,maxNumOfDetector-0.5);
  if (!Total_hitPat) G4cout << "\n can't create histo Total_hitPat" << G4endl;
  BSD_Energy_s = new TH1D("BSD Energy_spectra"   ,
			  "BSD Energy spectrum;E [keV];counts/keV",3000,0-0.5,3000-0.5);
  if (!BSD_Energy_s) G4cout << "\n can't create histo Energy_spectrum" << G4endl;
  PSD_Energy_s = new TH1D("PSD Energy_spectra"   ,
			  "PSD Energy spectrum;E [keV];counts/keV",3000,0-0.5,3000-0.5);
  if (!PSD_Energy_s) G4cout << "\n can't create histo Energy_spectrum" << G4endl;
  Diff_localtime_s = new TH1D("localtime_spectra"   ,
			      "local time spectrum;t [ns];counts/ns",1000,0-0.5,10-0.5);
  if (!Diff_localtime_s) G4cout << "\n can't create histo localtime_spectrum" << G4endl;
  PSD_2DHitPat = new TH2I("PSDhitPat2D", "Hit Pattern 2D", NPixels, 0-0.5,
			  NPixels-0.5,NPixels, 0-0.5, NPixels-0.5);
  if (!PSD_2DHitPat) G4cout << "\n can't create histo PSD_2DHitPat" << G4endl;
  
  // control volume //
  Int_t Nbinx,Nbiny,Nbinz;
  Nbinx=Nbiny=Nbinz=200;
  Double_t lx,ly,lz; 
  lx=ly=lz=50.0;// in cm
  ControlVol_3DHitPat = new TH3D("hitPat3D", "Hit Pattern 3D", Nbinx,-lx/2.0-lx/(2.0*Nbinx),lx/2.0-lx/(2.0*Nbinx), Nbiny,-ly+1.0,1.0,Nbinz,-lz/2.0-lz/(2.0*Nbinz),lz/2.0-lz/(2.0*Nbinz));
  ControlVol_1DHitPat = new TH1D("Distance of first interaction (mag(r))", "Hit Pattern 1D", 2*Nbiny,0.0-2,ly/2.0-2);
  NumberOfInteractions = new TH1I("NumberOfInteraction","Number of Compton Interactions",30,0-0.5,30-0.5);
  Energy_s_SS = new TH1D("Energy_spectrumSS"   ,
				"Energy spectrum SS;E [keV];counts/keV",3000,0-0.5,3000-0.5);
  Energy_s_MS = new TH1D("Energy_spectrumMS"   ,
				"Energy spectrum MS;E [keV];counts/keV",3000,0-0.5,3000-0.5);

  // Tree construction //
  Cal_Tree = new TTree( (treeName+"_cal").data() , "TTree contain GFNUNdetectors CAL event info");
  Cal_Tree->Branch("Mult"      ,&cal_event->Mult       ,              "Mult/I");
  Cal_Tree->Branch("det_ID"    , cal_event->detector_ID, "detector_ID[Mult]/I");
  Cal_Tree->Branch("GammaDet_ID" , cal_event->GammaDetector_ID, "GammaDetector_ID[Mult]/I");
  Cal_Tree->Branch("Energy"    , cal_event->energy     ,      "energy[Mult]/D");
  Cal_Tree->Branch("Localtime" , cal_event->localTime  ,  "localTime[Mult]/D");
  Cal_Tree->Branch("Globaltime", cal_event->globalTime ,  "globalTime[Mult]/D");
  Cal_Tree->Branch("IsPhotoelectric", cal_event->photEff ,  "photEff[Mult]/I");

  Cal_Tree->Branch("NumCS"      ,cal_event->NCS       ,              "NCS[4]/I");
  Cal_Tree->Branch("PositionCS" , cal_event->CSpos     ,      "CSpos[3]/D");
}

void RootSaver::Save()
{
  //Check if ROOT TTree exists,
  //in case get the associated file and close it.
  //Note that if a TFile goes above 2GB a new file
  //will be automatically opened. We have thus to get,
  //from the TTree the current opened file
  if ( fRootFile )
    {
      G4cout<<"Writing ROOT File: "<<fRootFile->GetName()<<G4endl;
      fRootFile->Print();
      G4cout<<"Writing ROOT Tree: "<<Cal_Tree->GetName()<<G4endl;
      Cal_Tree->Print();

      if ( !fRootFile || fRootFile->IsZombie() )
	{
	  G4cerr<<"Error closing TFile "<<G4endl;
	  return;
	}

      fRootFile->Write();
      fRootFile->Close();
      G4cout << "\n----> Tree is saved \n" << G4endl;
    }
}

bool RootSaver::Trigger(const BSDcrystalDigitsCollection* const BSDdigits)
{
  if (BSDdigits)
    {
      Int_t nDigits = BSDdigits->entries();
      for(Int_t i = 0;i<4;i++)
	nc[i] = 0;
      Double_t bsdE=0.0;
      /* do not process singles */
      if(nDigits < 1)
	return false;
      else
	{
	  for ( G4int d = 0 ; d<nDigits ; d++ )
	    {
	      const BSDcrystalDigit* digi = static_cast<const BSDcrystalDigit*>(BSDdigits->GetDigi(d));
	      // ***** Detector ID: Assigned in SDBSDcrystal ***** //
	      // BSD = 0  
	      // *********************** //
	      if(digi->GetDetectorID() == 0) 
		{
		  timeTrigger=static_cast<Double_t>(digi->GetLocalTime()/ns);
		  bsdE+=static_cast<Double_t>(digi->GetCharge());
		  //return true;
		}
	  }
	  if(bsdE>0.0 && bsdE < 2500.0) // charge is calibrated energy in keV
	    return true;
	}
    }
  else
    {
      G4cout<<"RootSaver::Trigger: No digit collection to trigger"<<G4endl;
    }
  
  return false;
}

// **** Only BSD ****//
void RootSaver::AddEvent(const BSDcrystalDigitsCollection* const BSDdigits)
{
  // ***** Detector ID: Assigned in SDBSDcrystal and SDPSDcrystal ***** //
  // ***** Detector ID ***** //
  // BSD    = 0 
  // pixels of PSD = 1 to 4096
  // *********************** //
  if (BSDdigits)
    {
      Int_t nDigits = BSDdigits->entries();
      if(nDigits == 0)
	return;
      G4bool repeat=false; // Useful to control if a detector has been touched
      cal_event->Mult=1;
      
      if(cal_event->Mult>MNoD)
	{
	  G4cerr<<"RootSaver::AddEvent: Error: Number of detectors actived in this  larger than Maximum number of detectors"<<G4endl;
	  G4cerr<<"\t cal_event->Mult= "<<cal_event->Mult<<G4endl;
	  G4cerr<<"\t cal_event->Mult = MNoD (4097), some information will be lost"<<G4endl;
	  cal_event->Mult = MNoD;
	}
      /// Filling Branches
      for ( G4int d = 0 ; d< nDigits ; d++ )
	{
	  const BSDcrystalDigit* digi = static_cast<const BSDcrystalDigit*>(BSDdigits->GetDigi(d));
	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  cal_event->energy[0] += static_cast<Double_t>(digi->GetCharge());
	  if(d==0)
	    {
	      /* Time */
	      cal_event ->globalTime[0] = static_cast<Double_t>(digi->GetGlobalTime()/ns);
	      cal_event ->localTime[0]  = static_cast<Double_t>(digi->GetLocalTime()/ns);
	    }
	  if(static_cast<Bool_t>(digi->GetFlagPhotoelectric()))
	    cal_event->photEff[0] = static_cast<Int_t>(1);
	  /// *** Gamma ID *** ///
	  /// 0 : primary particle = gamma rays
	  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
	  /// 2 : origin = positron annihilation
	  /// 3 : other particles
	  cal_event ->GammaDetector_ID[0] =  static_cast<Int_t>(digi->GetGammaID());
	}
      // ************************ //
      // Filled control histogram //
      // ************************ //
      /// Test:: energy
      //if(cal_event->energy[m]>662)
      //  G4cout<<" Deposited Energy ("<<m<<") = "<< cal_event->energy[m]<<G4endl;
      Total_hitPat->Fill(cal_event->detector_ID[0]);
      BSD_Energy_s->Fill(cal_event->energy[0]);
    
      //cal_event->Print();
      //G4cout<<"Filling ROOT Tree: "<<Cal_Tree->GetName()<<G4endl;
      Cal_Tree->Fill();
      cal_event->Clear();
    } 
  else
    G4cerr<<"RootSaver::AddEvent: Error: No digits collection passed to RootSaver"<<G4endl;
}

// **** Only PSD ****//
void RootSaver::AddEvent(const PSDcrystalDigitsCollection* const PSDdigits)
{
   // ***** Detector ID: Assigned in SDBSDcrystal and SDPSDcrystal ***** //
  // ***** Detector ID ***** //
  // BSD    = 0 
  // pixels of PSD = 1 to 4096
  // *********************** //
  if (PSDdigits)
    {
      Int_t nDigits = PSDdigits->entries();
      
      if(nDigits == 0)
	return;
      G4int numDetect=0;
      G4bool repeat=false; // Useful to control if a detector has been touched
      cal_event->Mult=0;
      
      // *** First step: define the number of activated detectors *** // 
      for ( G4int d = 0 ; d< nDigits ; d++ )
	{
	  const PSDcrystalDigit* digi = static_cast<const PSDcrystalDigit*>(PSDdigits->GetDigi(d));
	  if(numDetect == 0)
	    {
	      // ***** Detector ID: Assigned in digitizer ***** //
	      /* Detector ID */
	      cal_event->detector_ID[numDetect] = static_cast<Int_t>(digi->GetDetectorID());
	      /* increasing the number of detectors*/
	      numDetect++;
	      repeat=true;
	    }
	  else
	    {
	      for(G4int m = 0 ; m < numDetect ; m++)
		if(cal_event->detector_ID[m] != static_cast<Int_t>(digi->GetDetectorID()))
		  repeat=false;
		else
		  {repeat=true; break;}
	    }
	  if(!repeat)
	    {
	      /* Detector ID */
	      cal_event->detector_ID[numDetect] = static_cast<Int_t>(digi->GetDetectorID());
	      /* increasing the number of detectors*/
	      numDetect++;
	      repeat=true;
	    }
	}
      cal_event->Mult=static_cast<Int_t>(numDetect);
      
      if(cal_event->Mult>MNoD)
	{
	  G4cerr<<"RootSaver::AddEvent: Error: Number of detectors actived in this  larger than Maximum number of detectors"<<G4endl;
	  G4cerr<<"\t cal_event->Mult= "<<cal_event->Mult<<G4endl;
	  G4cerr<<"\t cal_event->Mult = MNoD (4097), some information will be lost"<<G4endl;
	  cal_event->Mult = MNoD;
	}
      
      /// Filling branches
      for ( G4int d = 0 ; d< nDigits ; d++ )
	{
	  const PSDcrystalDigit* digi = static_cast<const PSDcrystalDigit*>(PSDdigits->GetDigi(d));
	  G4int ndet=-1;
	  for(G4int m = 0 ; m < cal_event->Mult; m++)
	    if(cal_event->detector_ID[m] == static_cast<Int_t>(digi->GetDetectorID()))
	      {ndet = m; break;}
	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  cal_event->energy[ndet] += static_cast<Double_t>(digi->GetCharge());
	  if(d==0)
	    {
	      /* Time */
	      cal_event ->globalTime[ndet] = static_cast<Double_t>(digi->GetGlobalTime()/ns);
	      cal_event ->localTime[ndet]  = static_cast<Double_t>(digi->GetLocalTime()/ns);
	    }
	  if(static_cast<Bool_t>(digi->GetFlagPhotoelectric()))
	    cal_event->photEff[ndet] = static_cast<Int_t>(1);
	  /// *** Gamma ID *** ///
	  /// 0 : primary particle = gamma rays
	  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
	  /// 2 : origin = positron annihilation
	  /// 3 : other particles
	  cal_event->GammaDetector_ID[ndet] =  static_cast<Int_t>(digi->GetGammaID());
	}
      
      // ************************ //
      // Filled control histogram //
      // ************************ //
      Double_t psdE =0.0;
      for(G4int m = 0 ; m < numDetect ; m++)
	{
	  Total_hitPat->Fill(cal_event->detector_ID[m]);
	  if(cal_event->detector_ID[m]>0)
	    PSD_2DHitPat->Fill((cal_event->detector_ID[m]-1)%NPixels,(cal_event->detector_ID[m]-1)/NPixels);
	  psdE+=cal_event->energy[m];
	}
      PSD_Energy_s->Fill(psdE);
      
      //cal_event->Print();
      //G4cout<<"Filling ROOT Tree: "<<Cal_Tree->GetName()<<G4endl;
      Cal_Tree->Fill();
      cal_event->Clear();
    } // end if(digits)
  else{
    G4cerr<<"RootSaver::AddEvent: Error: No digits collection passed to RootSaver"<<G4endl;
  }

}

// BSD + PSD
void RootSaver::AddEvent(const BSDcrystalDigitsCollection* const BSDdigits,const PSDcrystalDigitsCollection* const PSDdigits)
{
  // ***** Detector ID: Assigned in SDBSDcrystal and SDPSDcrystal ***** //
  // ***** Detector ID ***** //
  // BSD    = 0 
  // pixels of PSD = 1 to 4096
  // *********************** //
  if (BSDdigits && PSDdigits)
    {
      Int_t nBdigits = BSDdigits->entries();
      Int_t nPdigits = PSDdigits->entries();
      
      if(nPdigits == 0 || nBdigits == 0)
	return;
      G4int numDetect=0;
      G4bool repeat=false; // Useful to control if a detector has been touched
      cal_event->Mult=0;
      
      // *** First step: define the number of activated detectors *** //
      // BSD
      for ( G4int d = 0 ; d< nBdigits ; d++ )
	{
	  const BSDcrystalDigit* digi = static_cast<const BSDcrystalDigit*>(BSDdigits->GetDigi(d));
	  if(d == 0)
	    {
	      // ***** Detector ID: Assigned in digitizer ***** //
	      /* Detector ID */
	      cal_event->detector_ID[d] = static_cast<Int_t>(digi->GetDetectorID()); 
	      /* increasing the number of detectors*/
	      numDetect++;
	    }
	}
      // PSD
      for ( G4int d = 0 ; d< nPdigits ; d++ )
	{
	  const PSDcrystalDigit* digi = static_cast<const PSDcrystalDigit*>(PSDdigits->GetDigi(d));
	  if(numDetect == 0)
	    {
	      // ***** Detector ID: Assigned in digitizer ***** //
	      /* Detector ID */
	      cal_event->detector_ID[numDetect] = static_cast<Int_t>(digi->GetDetectorID());
	      /* increasing the number of detectors*/
	      numDetect++;
	      repeat=true;
	    }
	  else
	    {
	      for(G4int m = 0 ; m < numDetect ; m++)
		if(cal_event->detector_ID[m] != static_cast<Int_t>(digi->GetDetectorID()))
		    repeat=false;
		  else
		    {repeat=true; break;}
	    }
	  if(!repeat)
	    {
	      /* Detector ID */
	      cal_event->detector_ID[numDetect] = static_cast<Int_t>(digi->GetDetectorID());
	      /* increasing the number of detectors*/
	      numDetect++;
	      repeat=true;
	    }
	}
      cal_event->Mult=static_cast<Int_t>(numDetect);
      
      /*G4cout<<"\t cal_event->Mult= "<<cal_event->Mult<<G4endl;
      for(G4int i=0;i<cal_event->Mult;i++)
	G4cout<<"\t ID= "<<cal_event->detector_ID[i]<<G4endl;
      */
      if(cal_event->Mult>MNoD)
	{
	  G4cerr<<"RootSaver::AddEvent: Error: Number of detectors actived in this  larger than Maximum number of detectors"<<G4endl;
	  G4cerr<<"\t cal_event->Mult= "<<cal_event->Mult<<G4endl;
	  G4cerr<<"\t cal_event->Mult = MNoD (100), some information will be lost"<<G4endl;
	  cal_event->Mult = MNoD;
	}
      
      // ******************************** //
      //  Calcular la energia depositada //
      //  tiempo, DetectorID       //
      // ******************************* //
      //BSD
      for ( G4int d = 0 ; d< nBdigits ; d++ )
	{
	  const BSDcrystalDigit* digi = static_cast<const BSDcrystalDigit*>(BSDdigits->GetDigi(d));
	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  cal_event->energy[0] += static_cast<Double_t>(digi->GetCharge()); //aca se llena energy[]
	  if(d==0)
	    {
	      /* Time */
	      cal_event ->globalTime[0] = static_cast<Double_t>(digi->GetGlobalTime()/ns);
	      cal_event ->localTime[0]  = static_cast<Double_t>(digi->GetLocalTime()/ns);
	    }
	      if(static_cast<Bool_t>(digi->GetFlagPhotoelectric()))
		cal_event->photEff[0] = static_cast<Int_t>(1);
	      /// *** Gamma ID *** ///
	      /// 0 : primary particle = gamma rays
	      /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
	      /// 2 : origin = positron annihilation
	      /// 3 : other particles
	      cal_event ->GammaDetector_ID[0] =  static_cast<Int_t>(digi->GetGammaID());
	}
      
      // PSD
      for ( G4int d = 0 ; d< nPdigits ; d++ )
	{
	  const PSDcrystalDigit* digi = static_cast<const PSDcrystalDigit*>(PSDdigits->GetDigi(d));
	  G4int ndet=-1;
	  for(G4int m = 0 ; m < cal_event->Mult; m++)
	    if(cal_event->detector_ID[m] == static_cast<Int_t>(digi->GetDetectorID()))
	      {ndet = m; break;}
	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  cal_event->energy[ndet] += static_cast<Double_t>(digi->GetCharge());
	  if(d==0)
	    {
	      /* Time */
	      cal_event ->globalTime[ndet] = static_cast<Double_t>(digi->GetGlobalTime()/ns);
	      cal_event ->localTime[ndet]  = static_cast<Double_t>(digi->GetLocalTime()/ns);
	    }
	  if(static_cast<Bool_t>(digi->GetFlagPhotoelectric()))
	    cal_event->photEff[ndet] = static_cast<Int_t>(1);
	  /// *** Gamma ID *** ///
	  /// 0 : primary particle = gamma rays
	  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
	  /// 2 : origin = positron annihilation
	  /// 3 : other particles
	  cal_event->GammaDetector_ID[ndet] =  static_cast<Int_t>(digi->GetGammaID());
	}    


      // ****************** //
      // Filling Histograms//
      // ****************** //
      //ESPECTRO ENERGIA BSD
      BSD_Energy_s->Fill(cal_event->energy[0]); 
      Double_t psdE =0.0;

      //CUENTAS POR DETECTOR
      for(G4int m = 0 ; m < numDetect ; m++)
	{
	  Total_hitPat->Fill(cal_event->detector_ID[m]);
	  if(cal_event->detector_ID[m]>0)
	    {
	      PSD_2DHitPat->Fill(static_cast<Int_t>(cal_event->detector_ID[m]-1)%NPixels,
				 static_cast<Int_t>(cal_event->detector_ID[m]-1)/NPixels);
	      psdE+=cal_event->energy[m];
	    }
	  Diff_localtime_s->Fill(cal_event->localTime[m]-timeTrigger);
	 
	}

      //ESPECTRO ENERGIA PSD
      PSD_Energy_s->Fill(psdE);
      
      Cal_Tree->Fill();
      cal_event->Clear();
    }
  else
    G4cerr<<"RootSaver::AddEvent: Error: No digits collection passed to RootSaver"<<G4endl;
}

///// Test Classs /////
void RootSaver::AddEvent(const ControlVolDigitsCollection* const digits)
{
  //  G4cout<<"Test: Entry RootSaver:: 1"<<G4endl;
  if (digits)
    {
      Int_t nDigits = digits->entries();
      //  G4cout<<"Test: Entry RootSaver::TestImageCV digits = "<<nDigits<<G4endl;
      if(nDigits == 0)
	return;
      const ControlVolDigit* lastdigit = static_cast<const ControlVolDigit*>( digits->GetDigi(nDigits-1));
      for(Int_t i = 0;i<4;i++)
	nc[i] = 0;

      //if(lastdigit->GetEntries()==1)
      
      for ( G4int d = 0 ; d< nDigits ; d++ )
	{
	  const ControlVolDigit* digi = static_cast<const ControlVolDigit*>( digits->GetDigi( d ) );
	  if(digi->GetFlagCompton())
	    nc[static_cast<Int_t>(digi->GetGammaID())]++;
	}
      // ************************ //
      if(nc[2]>0)
	NumberOfInteractions->Fill(static_cast<Int_t>(nc[2]));// nc[2] -> gamma rays from positron annihilation
      for(Int_t i = 0;i<4;i++)
	cal_event->NCS[i]=static_cast<Int_t>(nc[i]);

      for ( G4int d = 0 ; d< nDigits ; d++ )
	{
	  const ControlVolDigit* digi = static_cast<const ControlVolDigit*>( digits->GetDigi( d ) );
	  if(digi->GetFlagCompton())
	    {
	      Double_t qx = static_cast<Double_t>(digi->GetPosition().getX()/cm);
	      Double_t qy = static_cast<Double_t>(digi->GetPosition().getY()/cm);
	      Double_t qz = static_cast<Double_t>(digi->GetPosition().getZ()/cm);
	      Double_t norm = static_cast<Double_t>(digi->GetPosition().mag()/cm);
	      ControlVol_3DHitPat->Fill(qx,qy,qz);
	      cal_event->CSpos[0]=qx; cal_event->CSpos[1]=qy; cal_event->CSpos[2]=qz;
	      // Fill control histogram //
	      if(nc[2]==1)// nc[2] -> gamma rays from positron annihilation
		{
		  ControlVol_1DHitPat->Fill(norm);
		  //G4cout<<"NC= "<<nc<<" Position= "<<qx<<", "<<qy<<", "<<qz<<" norm= "<<norm<<G4endl;
		}
	    }
	}
      
      // ************************ //
      //cal_event->Print();
      //G4cout<<"Filling ROOT Tree: "<<Cal_Tree->GetName()<<G4endl;
      Cal_Tree->Fill();
      cal_event->Clear();
    } // end if(digits)
  else{
    G4cerr<<"RootSaver::TestImageCV: Error: No digits collection passed to RootSaver"<<G4endl;
  }

}
