
#ifndef RootSaver_h
#define RootSaver_h 1

#include "BSDcrystalDigit.hh"
#include "PSDcrystalDigit.hh"
#include "ControlVolDigit.hh"

#include <TROOT.h>
#include "globals.hh"

class G4RunManager;
class TFile;
class TTree;
class TH1D;
class TH1I;
class TH2I;
class TH3D;


const G4int MNoD = 4097;//! MNoD = Maximum number of detectors

class Cal_Event{
public:
  //! Constructor
  Cal_Event(){};
  //! Destructor
  ~Cal_Event(){};
  //! Multiplicity
  Int_t Mult;
  //! Detector IDentification number.
  Int_t detector_ID[MNoD];
   //! First Gamma detected, per Detector, IDentification number.
  Int_t GammaDetector_ID[MNoD];
  //! Deposition is produced via photoelectric effect.
  Int_t photEff[MNoD];
  //! Energy deposited
  Double_t energy[MNoD];
  //! Global time (time since beginning of event -primary particle-.
  Double_t globalTime[MNoD];
  //! Local time (time since the creation of the particle -not exclusively primary-)
  Double_t localTime[MNoD];
  
  //! Counting the number of Compton interactions
  Double_t CSpos[3]; // Position where Compton interactions occurs
  Int_t NCS[4]; // Number of Compton interactions
  /// 0 : primary particle = gamma rays
  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
  /// 2 : origin = positron annihilation
  /// 3 : other particles
  
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
		  <<"GammaDetector_ID = " <<GammaDetector_ID[i]
		  <<"\tenergy = "    <<energy[i]<<" keV"
		  <<"\tglobalTime = "<<globalTime[i]<<"ns"
		  <<"\tlocalTime = " <<localTime[i]<<"ns"
		  <<"\tphotEff = "<<photEff[i]
		  <<G4endl;
	  }
      }
  }
  inline void Clear()
  {
    Mult = 0;
    for(Int_t i=0;i<MNoD;i++)
      {
	detector_ID[i]=-1;
	GammaDetector_ID[i]=-1;
	energy[i]=0.;
	globalTime[i]=0.;
	localTime[i]=0.;
	photEff[i]=0;
      }
    for(Int_t i=0;i<4;i++)
      NCS[i]=0;
    for(Int_t i=0;i<3;i++)
      CSpos[i]=0.0;
  }
};


class RootSaver
{
public:
  //! Default constructor,
  RootSaver();
  //! Default destructor.
  virtual ~RootSaver();
 
   virtual void Book( const std::string& fileName = "CCamFCF" , const std::string& treeName = "CCamFCF_tree" );
 
  virtual void Save();
  //! Add hits and digi container for this event
  virtual bool Trigger(const BSDcrystalDigitsCollection* const BSDdigits);
  virtual void AddEvent(const BSDcrystalDigitsCollection* const BSDdigits);// Only BSD
  virtual void AddEvent(const PSDcrystalDigitsCollection* const PSDdigits);// Only PSD
  virtual void AddEvent(const ControlVolDigitsCollection* const CVdigits);// Only control volume
  virtual void AddEvent(const BSDcrystalDigitsCollection* const BSDdigits,const PSDcrystalDigitsCollection* const PSDdigits); // BSD + PSD
  //  virtual void AddEvent(const BSDcrystalDigitsCollection* const BSDdigits,const PSDcrystalDigitsCollection* const PSDdigits,const ControlVolDigitsCollection* CVdigits); // All


private:
  
  G4RunManager* runManager;
  // G4bool optical_flag;

  TFile*   fRootFile; 
  TTree* Cal_Tree; 

  Cal_Event* cal_event;

  Int_t NPixels;
  G4int Npixels;

  //! Control histograms
  TH1D* BSD_Energy_s;
  TH1D* PSD_Energy_s;
  TH1D* Diff_localtime_s;
  TH1I* Total_hitPat;
  TH2I* PSD_2DHitPat;
  //// Control volume: Number of Compton interactions
  TH1I* NumberOfInteractions;
  TH3D* ControlVol_3DHitPat;
  TH1D* ControlVol_1DHitPat;
  TH1D* Energy_s_SS;
  TH1D* Energy_s_MS;
  //@}
  Int_t nc[4]; // Number of Compton interaction counter
  // Number of Compton interactions
  /// 0 : primary particle = gamma rays
  /// 1 : origin = radiactive decay (example: 1275 keV (22Ne), 662 keV (137Cs))
  /// 2 : origin = positron annihilation
  /// 3 : other particles
  Double_t timeTrigger;
};

#endif
