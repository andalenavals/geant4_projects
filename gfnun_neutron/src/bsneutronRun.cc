#include "bsneutronRun.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4UnitsTable.hh"

//14/03/07 las agrague por que aparecian en N07...?
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
bsneutronRun::bsneutronRun() : nEvent(0)             
{
  G4SDManager* SDM = G4SDManager::GetSDMpointer();
    totalSurfCurrentID = SDM->GetCollectionID("MyDetector/MyScorer");
}

bsneutronRun::~bsneutronRun(){
}
void bsneutronRun::RecordEvent(const G4Event* evt)
{
  nEvent++;
  
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    eventTotalSurfCurrent = (G4THitsMap<G4double>*)(HCE->GetHC(totalSurfCurrentID));
    totalSurfCurrent += *eventTotalSurfCurrent;
}

void bsneutronRun::DumpScorer(){
  //G4THitsMap<G4double>* RunMap =GetHitsMap(i);
  //if ( totalSurfCurrent ) {
  
  //G4cout << " PrimitiveScorer RUN " 
  //	 << totalSurfCurrent.GetSDname() <<","<< totalSurfCurrent.GetName() << G4endl;
  
  // Print to G4cout and an output file
  std::ofstream fileOut("salida.csv", std::ios::app);
  fileOut.setf( std::ios::scientific, std::ios::floatfield );
  /*G4cout << "Numero de entradas " << totalSurfCurrent.entries() << G4endl;
    
  std::map<G4int,G4double*>::iterator itr = totalSurfCurrent.GetMap()->begin();
  G4double value=0;
  for(; itr != totalSurfCurrent.GetMap()->end(); itr++) {
  G4cout << "  copy no.: " << itr->first
  << "  Corriente (#Particulas/und. area) : " << *(itr->second)*mm*mm<<" mm^-2" 
  << G4endl;
  
  G4cout<<*(itr->second)<<G4endl;
  value=*(itr->second);
  }*/
  //outFile<<value<<G4endl;
  
 
}
