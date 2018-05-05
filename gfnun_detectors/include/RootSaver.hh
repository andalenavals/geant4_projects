/**
 * @file   RootSaver.hh
 *
 * @date   17 Dec 2009
 * @author Pico
 *
 * @brief  Save hits and digits in a ROOT TTree
 */

#ifndef ROOTSAVER_HH_
#define ROOTSAVER_HH_

#include <string>
#include <TTree.h>
#include "MyDigi.hh"
#include "MyHit.hh"
#include "Riostream.h"
class G4RunManager;
class TH1;
class TH2;
class SteppingAction;
class Interactions;

#ifdef G4ANALYSIS_USE
#include <AIDA/AIDA.h>
#endif
/*!
 * \brief Define the hit information per detector.
 *
 * This class is used to store the variables for each detector -or
 * replica- in the array.  It is implemented structure is described
 * below.  The difference between this and the actual hit is that
 * here, we cast the variables for ROOT (G4int -> Int_t) and we
 * discard everything we do not use.\n This can be easily avoided just
 * by reading directly the hit information in the AddEvent routine but
 * if you want to print them -for instance- is useful to use a
 * structure that holds the info already read that will be entered 
 * into the ROOT Tree.
 * \warning I know this might be an overkill but what the hell
 * \sa DetectorConstruction()
 */
class Cal_Event{
public:
  //! Constructor
  Cal_Event(){};
  //! Destructor
  ~Cal_Event(){};
  //! Multiplicity
  Int_t Mult;
  //! Detector IDentification number.
  Int_t detector_ID[100];
  //! Energy deposited
  Double_t energy[100];
  //! Global time (time since beginning of event -primary particle-.
  Double_t globalTime[100];
  //! Local time (time since the creation of the particle -not exclusively primary-)
  Double_t localTime[100];
  //! Detector Name for easy coding =) \warning NOT SAVED TO THE ROOT FILE.
  TString detector[100];

  //! Volumes when Compton interactions occurs for principal gamma rays 
  // will be considered six posibles gamma ray: 
  // (1) 1 x gamma-decay
  // (2),(3) 2 x positron-annihilation (beta+ decay)
  // (4),(5) 2 x positron-annihilation (pair production)
  // (6) 1 x gamma source (primary)
  Int_t Volumes[6][30];
  //! Local time  per volume of each pricipal gamma ray 
  Double_t  localTime_Volumes[6][30];

  //! Print method
  inline void Print()
  {
    G4cout<<"############################\n"
	  <<"Cal event -> Mult = "<<Mult<<G4endl;
    for(Int_t i=0;i<Mult;i++)
      {
	if(energy[i]>0)
	  {
	    G4cout<<"detector_ID = " <<detector_ID[i]
		  <<"\tenergy = "    <<energy[i]<<" keV"
		  <<"\tdetector = "  <<detector[i]
		  <<"\tglobalTime = "<<globalTime[i]<<"ns"
		  <<"\tlocalTime = " <<globalTime[i]<<"ns"
		  <<G4endl;
	  }
      }
  }
  inline void Clear()
  {
    Mult = 100;
    for(Int_t i=0;i<Mult;i++)
      {
	detector_ID[i]=0;
	    energy[i]=0.;
	globalTime[i]=0.;
	 localTime[i]=0.;
	  detector[i]="";
      }
    for(Int_t i=0;i<6;i++)
      for(Int_t j=0;j<30;j++)
	{
	  Volumes[i][j]=-1;
	  localTime_Volumes[i][j]=0.;
	}
  }
};


/*!
 * \brief Save hits and digits to a ROOT TTree.
 *
 * This class can be used to save in a TTree hits
 * and digits.
 * The TTree structure is described below.
 */
class RootSaver
{
public:
	//! Default constructor,
	RootSaver();
	//! Default destructor.
	virtual ~RootSaver();
	/*! \brief Open a TFile and create ROOT TTree.
	 *
	 * Each time that a /run/beamOn command is issued
	 * this method should be called
	 * Every time this method is called the run counter
	 * is incremented and the file name is modified accordingly:
	 * tree_run<n>.root
	 * \sa CloseTree()
	 * @param fileName : The ROOT file name prefix
	 * @param treeName : The name of the TTree
	 */
	virtual void CreateTree( const std::string& fileName = "GFNUNdetectors" , const std::string& treeName = "GFNUNdetectors_tree" );
	/*! Close the file and save ROOT TTree
	 * The ROOT file should be closed at the end of each /run/beamOn
	 * \sa CloseTree
	 */
	virtual void CloseTree();
	//! Add hits and digi container for this event
        virtual bool Trigger(const MyDigiCollection* const digits);
	virtual void AddEvent(const MyDigiCollection* const digits);

  void CreateAscii(const std::string& fileName = "GFNUNdetectors");
  void CloseAscii();

private:
  //! SteppingAction
  SteppingAction* stepping;
  //! Fussy flag event: Store information about number of Compton interactions between other.
  G4bool fussyInformation_flag;
  Interactions*  register_int_Comp;
  G4int NumberOfInterstingGammaRays;

  //!
  G4RunManager* runManager;
  G4bool ascii_flag;
  ofstream ascii_file_raw;
  ofstream ascii_file_cal;
  G4bool optical_flag;

  TTree* Cal_Tree; //!< Pointer to the ROOT TTree
  //! \name Event variables
  //@{
  Cal_Event* cal_event;
  //@}
  //! \name histograms
  //@{

  TH1* Ge_10_detector_spectrum;
  TH1* Ge_40_detector_spectrum;
  TH1* NaI_2x2_detector_spectrum;
  TH1* NaI_3x3_detector_spectrum;
  TH1* CsI_detector_spectrum;
  TH1* dummy_detector_spectrum;
  TH1* Plastic_detector_spectrum;
  TH1* raw_Total_hitPat;	  
  TH1* Total_hitPat;	  
  //@}
  //! \name AI

  //@{
#ifdef G4ANALYSIS_USE
  //  AIDA::IHistogram2D* position_detector_HitPat;
  AIDA::IAnalysisFactory* aida;
  AIDA::IPlotter* Plotter;
#endif
  //@}
};

#endif /* ROOTSAVER_HH_ */
