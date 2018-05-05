/**
 * @file   RootSaver.cc
 *
 * @date   17 Dec 2009
 * @author adotti
 * 
 * @brief  Implements class RootSaver.
 */

#include "RootSaver.hh"
#include "MyDigi.hh"
#include "MyHit.hh"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2I.h"
#include "TMath.h"
#include <sstream>
#include <cassert>
#include "G4RunManager.hh"
#include "Interactions.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4Run.hh"

#include "DetectorConstruction.hh"
using namespace CLHEP;
RootSaver::RootSaver() :
  Cal_Tree(0)
{
  runManager = G4RunManager::GetRunManager();
  ascii_flag = false;
  cal_event = new Cal_Event();
  cal_event->Clear();
  stepping = (SteppingAction*)(runManager->GetUserSteppingAction());
  fussyInformation_flag = false;
}

RootSaver::~RootSaver()
{
  //Close current file if needed
  if ( Cal_Tree )
      CloseTree();

  delete Ge_10_detector_spectrum;
  delete Ge_40_detector_spectrum;
  delete NaI_2x2_detector_spectrum;
  delete NaI_3x3_detector_spectrum;
  delete CsI_detector_spectrum;
  delete dummy_detector_spectrum;
  delete Plastic_detector_spectrum;
  delete raw_Total_hitPat;
  delete Total_hitPat;

  // delete position_detector_2DHitPat; /* for some unknown reason it seems that deleting TH2 pointers segfaults */

  delete[] register_int_Comp;

#ifdef G4ANALYSIS_USE
  delete position_detector_HitPat;
#endif
}

void RootSaver::CreateTree( const std::string& fileName , const std::string& treeName )
{
  DetectorConstruction* Detector =
    (DetectorConstruction*)(runManager->GetUserDetectorConstruction());
  
  optical_flag = Detector->GetOptical_flag();
  Int_t maxNumOfDetector = Detector->GetMaximumNumberOfDetectors();
 
 register_int_Comp = (Interactions*)(stepping->GetRegister_Int());
  NumberOfInterstingGammaRays = (G4int)(stepping->GetNumberOfInterstingGammaRays());
  fussyInformation_flag = stepping ? stepping->GetInteraction_flag() : false;

  Total_hitPat = new TH1F("Total_hitPat"   ,"TOTAL hit pattern;ID;counts",
			    maxNumOfDetector,0-0.5,maxNumOfDetector-0.5);
  raw_Total_hitPat = new TH1F("raw_Total_hitPat"   ,"TOTAL hit pattern;ID;counts" ,
			      maxNumOfDetector,0-0.5,maxNumOfDetector-0.5);
  Ge_10_detector_spectrum = new TH1F("Ge10_detector_spectra"   ,
				     "Ge10 detector spectrum;E [keV];counts/keV",2000,0-0.5,2000-0.5);
  Ge_40_detector_spectrum = new TH1F("Ge40_detector_spectra"   ,
				     "Ge40 detector spectrum;E [keV];counts/keV",2000,0-0.5,2000-0.5);
  NaI_2x2_detector_spectrum = new TH1F("NaI_2x2_detector_spectra"   ,
				     "NaI (2x2) detector spectrum;E [keV];counts/keV",
				     2000,0-0.5,2000-0.5);
  NaI_3x3_detector_spectrum = new TH1F("NaI_3x3_detector_spectra"   ,
				       "NaI (3x3) detector spectrum;E [keV];counts/keV",
				       2000,0-0.5,2000-0.5);
  CsI_detector_spectrum = new TH1F("CsI_detector_spectra"   ,
				     "CsI detector spectrum;E [keV];counts/keV",
				     2000,0-0.5,2000-0.5);
  dummy_detector_spectrum = new TH1F("dummy_detector_spectra"   ,
				     "Dummy detector spectrum;E [keV];counts/keV",
				     2000,0-0.5,2000-0.5);
  Plastic_detector_spectrum = new TH1F("Plastic_detector_spectra"   ,
				     "Plastic detector spectrum;E [keV];counts/keV",
				     2000,0-0.5,2000-0.5);


  if ( Cal_Tree )
    {
      std::cerr<<"TTree already created, first call CloseTree"<<std::endl;
      return;
    }
  
  std::ostringstream fn;
    G4int runID = runManager->GetCurrentRun()->GetRunID();
  fn << fileName << "_run" << runID << ".root";
  //Create a new file and open it for writing, if the file already exists the file
  //is overwritten
  TFile* rootFile = TFile::Open( fn.str().data() , "recreate" );
  if ( rootFile == 0 || rootFile->IsZombie() )
    {
      G4cerr<<"Error opening the file: "<<fn.str() <<" TTree will not be saved."<<G4endl;
      return;
    }
  

  Cal_Tree = new TTree( (treeName+"_cal").data() , "TTree contain GFNUNdetectors CAL event info");
  Cal_Tree->Branch("Mult"      ,&cal_event->Mult       ,              "Mult/I");
  if(fussyInformation_flag)
    {
      Cal_Tree->Branch("Volumes",cal_event->Volumes,"Volumes[6][30]/I");
      Cal_Tree->Branch("Localtime_Volumes",cal_event->localTime_Volumes,"localTime_Volumes[6][30]/D");
    }
  Cal_Tree->Branch("det_ID"    , cal_event->detector_ID, "detector_ID[Mult]/I");
  Cal_Tree->Branch("Energy"    , cal_event->energy     ,      "energy[Mult]/D");
  Cal_Tree->Branch("Localtime" , cal_event->localTime  ,  "localTime[Mult]/D");
  Cal_Tree->Branch("Globaltime", cal_event->globalTime ,  "globalTime[Mult]/D");

  
#ifdef G4ANALYSIS_USE
  aida = 0;
  G4cout << "### AIDA Histo books & histograms " << G4endl;
  // Creating the analysis factory
  aida = AIDA_createAnalysisFactory();
  if(aida)
    {
      // Creating a histogram factory, whose histograms will be handled by the tree
      AIDA::IHistogramFactory* hf = aida->createHistogramFactory(*aida->createTreeFactory()->create());
      if(hf)
	{
          // Creating an 1-dimensional histograms in the root directory of the tree
          //position_detector_HitPat = hf->createHistogram2D("hitPat-2D", "Hit Pattern 2D", PixelNumber, 0-0.5, PixelNumber-0.5,PixelNumber, 0-0.5, PixelNumber-0.5);
          //G4cout << "AIDA histograms are booked" << G4endl;
          delete hf;
        }
      AIDA::IPlotterFactory* pf = aida->createPlotterFactory(0,0);
      if (pf)
        {
          Plotter = pf->create("Plotter");
          if(Plotter)
            {
              /*Plotter->createRegions(1);
	      Plotter->region(0)->plot(*position_detector_HitPat);
              Plotter->show();*/
            }
          delete Plotter;
        }
    }
  else
    {
      G4cout << "ERROR: can't get AIDA." << G4endl;
    }
#endif     

}

void RootSaver::CreateAscii(const std::string& fileName)
{
  ascii_flag = ((RunAction*)(runManager->GetUserRunAction()))->GetAsciiOutputFlag();
  
  G4int runID = runManager->GetCurrentRun()->GetRunID();
  std::ostringstream ascii_name_cal;
  ascii_name_cal << fileName << "_run" << runID << "_cal.dat";
  
  ascii_file_cal.open(ascii_name_cal.str().data(),ios::out);
  G4cout<<"ASCII output: "<<ascii_name_cal.str().data()<<G4endl;
}

void RootSaver::CloseAscii()
{
  ascii_file_cal.close();
  G4cout<<G4endl;
  G4cout<<"/**********************************************/"<<G4endl;
  G4cout<<"/********  ASCII output created        ********/"<<G4endl;
  G4cout<<"/**********************************************/"<<G4endl;
}


void RootSaver::CloseTree()
{
  //Check if ROOT TTree exists,
  //in case get the associated file and close it.
  //Note that if a TFile goes above 2GB a new file
  //will be automatically opened. We have thus to get,
  //from the TTree the current opened file
  if ( Cal_Tree )
    {
      G4cout<<"Writing ROOT TTree: "<<Cal_Tree->GetName()<<G4endl;
      //Cal_Tree->Print();

      TFile* currentFile = Cal_Tree->GetCurrentFile();
      if ( currentFile == 0 || currentFile->IsZombie() )
	{
	  G4cerr<<"Error closing TFile "<<G4endl;
	  return;
	}
      
      Cal_Tree->Write();

      Ge_10_detector_spectrum->Write();
      Ge_40_detector_spectrum->Write();
      NaI_2x2_detector_spectrum->Write();
      NaI_3x3_detector_spectrum->Write();
      CsI_detector_spectrum->Write();
      dummy_detector_spectrum->Write();
      Plastic_detector_spectrum->Write();
      
      Total_hitPat->Write();
      raw_Total_hitPat->Write();

      currentFile->Close();
      //The root is automatically deleted.
      Cal_Tree = 0;

      Ge_10_detector_spectrum->Reset();
      Ge_40_detector_spectrum->Reset();
      NaI_2x2_detector_spectrum->Reset();
      NaI_3x3_detector_spectrum->Reset();
      CsI_detector_spectrum->Reset();
      dummy_detector_spectrum->Reset();
      Plastic_detector_spectrum->Reset();
      Total_hitPat->Reset();
      raw_Total_hitPat->Reset();
    }
#ifdef G4ANALYSIS_USE
  if(position_detector_HitPat) delete position_detector_HitPat;
#endif
}

bool RootSaver::Trigger(const MyDigiCollection* const digits)
{
  if (digits)
    {
      Int_t nDigits = digits->entries();
      
      /* do not process singles */
      if(nDigits < 1)
	return false;
      //else
      //return true;
      for ( G4int d = 0 ; d<nDigits ; d++ )
	{
	  const MyDigi* digi = static_cast<const MyDigi*>( digits->GetDigi( d ) );
	  // ***** Detector ID ***** //
	  // dummy    = 0  
	  // Ge_10    = 1
	  // Ge_40    = 2
	  // NaI_2x2  = 3
	  // NaI_3x3  = 4
	  // Plastic  = 5
	  // CsI      = 6
	  // *********************** //
	  if(digi->GetDetectorID() == 2) /* Ge40 Detector defines the triggering condition */
	    return true;
	}
    }
  else
    {
      G4cout<<"No digit collection to trigger"<<G4endl;
    }
  
  return false; /* default no trigger */
}


void RootSaver::AddEvent(const MyDigiCollection* const digits)
{
  if (digits)
    {
      Int_t nDigits = digits->entries();
      
      if(nDigits == 0)
	return;
	
      cal_event->Mult = nDigits;

      if(ascii_flag)
	{
	  G4int eventID = runManager->GetCurrentEvent()->GetEventID();
	  ascii_file_cal<<eventID<<G4endl; //event header for D.Rudolph
	}


      for ( G4int d = 0 ; d<nDigits ; d++ )
	{
	  const MyDigi* digi = static_cast<const MyDigi*>( digits->GetDigi( d ) );
	  
	  /* Detector ID */
	  cal_event->detector_ID[d] = static_cast<Int_t>(digi->GetDetectorID());
	  Total_hitPat->Fill(cal_event->detector_ID[d]);
	    
	  // ***** Detector ID ***** //
	  // dummy    = 0  
	  // Ge_10    = 1
	  // Ge_40    = 2
	  // NaI_2x2  = 3
	  // NaI_3x3  = 4
	  // Plastic  = 5
	  // CsI      = 6
	  // *********************** //

	  /* Energy */
	  /* remember charge is calibrated energy in keV */
	  cal_event->energy[d] = static_cast<Double_t>(digi->GetCharge());
	  // Filled control histogram
	  if(cal_event->detector_ID[d]==0)
	    dummy_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==1)
	    Ge_10_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==2)
	    Ge_40_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==3)
	    NaI_2x2_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==4)
	    NaI_3x3_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==5)
	    Plastic_detector_spectrum->Fill(cal_event->energy[d]);
	  else if(cal_event->detector_ID[d]==6)
	    CsI_detector_spectrum->Fill(cal_event->energy[d]);
	  

	  /* Time */
 	  cal_event ->globalTime[d] = static_cast<Double_t>(digi->GetGlobalTime()/ns);
 	  cal_event ->localTime[d]  = static_cast<Double_t>(digi->GetLocalTime()/ns);

	  /** ASCII OUTPUT **/
	  if(ascii_flag)
	    {
	      ascii_file_cal<<cal_event->detector_ID[d]<<G4endl;
	      ascii_file_cal<<cal_event->energy[d]<<G4endl;
	      ascii_file_cal<<cal_event->globalTime[d]<<G4endl;
	      ascii_file_cal<<cal_event->localTime[d]<<G4endl;
	    }
	}
      
      if(fussyInformation_flag)
	{
	  for(Int_t i=0;i<NumberOfInterstingGammaRays;i++)
	    {
	      for(G4int j=0;j<register_int_Comp[i].GetVolumeCounter();j++)
		{
		  cal_event->Volumes[i][j] = static_cast<Int_t*>(register_int_Comp[i].GetVolume())[j];
		  cal_event->localTime_Volumes[i][j] = static_cast<Double_t*>(register_int_Comp[i].GetlocalTime())[j];
		}
	    }
	  /*	  
       ///// !  test::
	  G4cout<<"1275: ";
	  for(G4int j=0;j<30;j++)
	    G4cout<<cal_event->Volumes[0][j]<<" ";
	  G4cout<<G4endl;
	  G4cout<<"511_1: ";
	  for(G4int j=0;j<30;j++)
	    G4cout<<cal_event->Volumes[1][j]<<" ";
 	  G4cout<<G4endl; 
	  G4cout<<"511_2: ";
	  for(G4int j=0;j<30;j++)
	    G4cout<<cal_event->Volumes[2][j]<<" ";
	    G4cout<<G4endl;*/
	}

      // cal_event->Print();
      Cal_Tree->Fill();
      if(fussyInformation_flag)
	cal_event->Clear();
      if(ascii_flag)
	{
	  ascii_file_cal<<"-1"<<G4endl; //event trailer
	}

    } //end if(digits)
  else{
    G4cerr<<"Error: No digits collection passed to RootSaver"<<G4endl;
  }

}
