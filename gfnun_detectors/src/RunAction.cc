/**
 * @file   RunAction.cc
 *
 * @date   17 Dec 2009
 * @author adotti
 *
 * @brief  Implements user class RunAction.
 */

#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "EventAction.hh"
#include "G4Run.hh"
/* to calculate EM physical values*/
#include "G4EmCalculator.hh"
#include "G4UnitsTable.hh"

RunAction::RunAction(EventAction* theEventAction ) :
  eventAction(theEventAction)
{
//  G4cout<<"RunAction::RunAction"<<G4endl;
eventAction->SetRootSaver( &saver );
messenger = new RunActionMessenger(this);
rootfile="";
ascii_flag = false;
}

void RunAction::BeginOfRunAction(const G4Run* aRun )
{
//  G4cout<<"RunAction::BeginOfRunAction"<<G4endl;
  
if(ascii_flag)
  G4cout<<"\n\t\tASCII output enabled"<<G4endl;
 else
   G4cout<<"\n\t\tASCII output disabled"<<G4endl;

G4int runID = aRun->GetRunID();
G4cout<<"Starting Run: "<<runID<<G4endl;
//For each run a new TTree is created, with default or custom name
if(rootfile=="")
  {
saver.CreateTree();
if(ascii_flag) saver.CreateAscii();
}
 else
   {
saver.CreateTree(rootfile);
if(ascii_flag) saver.CreateAscii(rootfile);
}
}

void RunAction::EndOfRunAction( const G4Run* /*aRun*/ )
{
//  G4cout<<"RunAction::EndOfRunAction"<<G4endl;
saver.CloseTree();
if(ascii_flag)
  saver.CloseAscii();
  
/// Test: to calculate EM physical values //
/*
G4EmCalculator emCalculator;
G4double gamma_E = 511*keV;
G4String material = "G4_Al";
/// Compton scattering
G4double muC = emCalculator.ComputeCrossSectionPerVolume(gamma_E,
							   emCalculator.FindParticle("gamma"),
							   "compt",
							   emCalculator.FindMaterial(material));
/// Rayleigh scattering 
G4double muR = emCalculator.ComputeCrossSectionPerVolume(gamma_E,
							   emCalculator.FindParticle("gamma"),
							   "Rayl",
							   emCalculator.FindMaterial(material));
/// Photo-electric effect 
G4double muPh = emCalculator.ComputeCrossSectionPerVolume(gamma_E,
							    emCalculator.FindParticle("gamma"),
							    "phot",
							    emCalculator.FindMaterial(material));
/// Gamma conversion -->  Pair production
G4double muPp = emCalculator.ComputeCrossSectionPerVolume(gamma_E,
							    emCalculator.FindParticle("gamma"),
							    "conv",
							    emCalculator.FindMaterial(material));
//G4BestUnit(muC, "1/Length");
G4cout<<"The linear attenuation coefficients for a gamma ray of "<<gamma_E/keV<<" kev in "<<material<<" are,   muC = "<<muC*cm<<"/cm    muR ="<<muR*cm<<"/cm,    muPh = "<<muPh*cm<<"/cm,    muPp = "<<muPp*cm<<"/cm"<<"    mu(total) = "<<muC*cm+muR*cm+muPh*cm+muPp*cm<<G4endl;
*/
}
