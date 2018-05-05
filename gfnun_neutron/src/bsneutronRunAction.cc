//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: ExN02RunAction.cc,v 1.8 2004/03/08 15:14:49 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "bsneutronRunAction.hh"
#include "bsneutronRun.hh"
#include "G4Run.hh"

#include "G4RunManager.hh"
//MAyo 4 la agregue para ver si puedo imprimir en el archivo la distancia entre detectores
#include "bsneutronDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bsneutronRunAction::bsneutronRunAction()
  :DiferenciadeHits1(0),DiferenciadeHits2(0),DiferenciadeHits3(0),DiferenciadeHits4(0),NHits1(0),NHits2(0),NHits3(0),NHits4(0),NHits5(0),NHits6(0),NHits7(0),NHits8(0)
{
  G4RunManager* theRunManager =  G4RunManager::GetRunManager();
  myDetector=(bsneutronDetectorConstruction*)theRunManager->GetUserDetectorConstruction();  
  NHits1=0;
  NHits2=0;
  NHits3=0;
  NHits4=0;
  NHits5=0;
  NHits6=0;
  NHits7=0;
  NHits8=0;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bsneutronRunAction::~bsneutronRunAction()
{ 
  // G4cout << "oooOOOooo Aqui termina bsneutronRunAction.cc oooOOOooo"<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//función generate run agregada 14/03/07.....
/*G4Run*  bsneutronRunAction::GenerateRun()
  {
  G4cout<<"Creating user define run class BeamTestRun"<<G4endl;
  //comente el argumento....aun no se porque...pero gueno.. 14/03/07
  //return new bsneutronRun("MyDetector");
  return new bsneutronRun;
  }*/
void bsneutronRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //G4cout << "oooOOOooo Aqui empieza bsneutronRunAction.cc oooOOOooo"<<G4endl;
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;  
  DiferenciadeHits1=0;
  DiferenciadeHits2=0;
  DiferenciadeHits3=0;
  DiferenciadeHits4=0;
  NHits1=0;
  NHits2=0;
  NHits3=0;
  NHits4=0;
  NHits5=0;
  NHits6=0;
  NHits7=0;
  NHits8=0;  
}
//*******************************************************************************************
void bsneutronRunAction::EndOfRunAction(const G4Run* aRun)
{
  //14/03/07.
  G4cout <<"Numero de eventos procesados: " <<aRun->GetNumberOfEvent() << " Eventos. " <<G4endl; 
  
  //14/03/07.Asi esta en el ejemplo HandsOn3 
  //const bsneutronRun* theRun = dynamic_cast<const bsneutronRun*>(aRun);
  
  //14/03/07..Asi esta en el pdf del tuturial de slac.sanford...creo que son lo mismo pero....creer no lo es todo carajo.
  //bsneutronRun* theRun = (bsneutronRun*)aRun;
  //theRun->DumpScorer();
  
  
  //Distancia entre detectores.....................
  G4double D,TD;
  D=myDetector->GetDistanciaEntreDetectores();
  TD=myDetector->GetTargetPosition();
  //G4cout <<"Hits en el detector A.....:" <<NHitsA<<G4endl;
  //G4cout <<"Hits en el detector B.....:" <<NHitsB<<G4endl;
  G4cout <<"Diferencia............:" <<DiferenciadeHits1<<G4endl;
  
  std::ofstream fileABOut("CuentasAB.out", std::ios::app);
  fileABOut.setf( std::ios::scientific, std::ios::floatfield );
  std::ofstream file1Out("Cuentas1-8.out", std::ios::app);
  file1Out.setf( std::ios::scientific, std::ios::floatfield );
  std::ofstream file2Out("Cuentas2-7.out", std::ios::app);
  file2Out.setf( std::ios::scientific, std::ios::floatfield );
  std::ofstream file3Out("Cuentas3-6.out", std::ios::app);
  file3Out.setf( std::ios::scientific, std::ios::floatfield );
  std::ofstream file4Out("Cuentas4-5.out", std::ios::app);
  file4Out.setf( std::ios::scientific, std::ios::floatfield );
  //Distancia entre detectores, Posicion del target,  Cuentas en el detector A, Cuentas en el detector B, Diferencia
  
  //  if(TD==0*cm){
  fileABOut<<"ArrayAyB"<<"  "<< NHits1+NHits2+NHits3+NHits4 <<"  "<< NHits5+NHits6+NHits7+NHits8 << "  "<< (NHits1+NHits2+NHits3 + NHits4)- (NHits5 + NHits6+NHits7+NHits8) << G4endl;   

  //  fileOut<<"P2"<<"  " <<TD <<"  "<<NHits2 NHits7<<"  "<<DiferenciadeHits2<<G4endl;
  //leOut<<"P3"<<"  " <<TD <<"  "<<NHits3 NHits6<<"  "<<DiferenciadeHits3<<G4endl;
  //leOut<<"P4"<<"  " <<TD <<"  "<<NHits4 NHits5<<"  "<<DiferenciadeHits4<<G4endl;
    
  /*
    G4cout<<"Hubieron estos hits :"<<G4endl;
    G4cout<<"P1"<<"  " <<TD <<"  "<<NHits1<<"  "<<NHits8<<"  "<<DiferenciadeHits1<<G4endl;
    G4cout<<"P2"<<"  " <<TD <<"  "<<NHits2<<"  "<<NHits7<<"  "<<DiferenciadeHits2<<G4endl;
    G4cout<<"P3"<<"  " <<TD <<"  "<<NHits3<<"  "<<NHits6<<"  "<<DiferenciadeHits3<<G4endl;
    G4cout<<"P4"<<"  " <<TD <<"  "<<NHits4<<"  "<<NHits5<<"  "<<DiferenciadeHits4<<G4endl;
    
    // }
    //else{
    file1Out<<TD <<"  "<<NHits1<<"  "<<NHits8<<"  "<<DiferenciadeHits1<<G4endl;
    file2Out<<TD <<"  "<<NHits2<<"  "<<NHits7<<"  "<<DiferenciadeHits2<<G4endl;
    file3Out <<TD <<"  "<<NHits3<<"  "<<NHits6<<"  "<<DiferenciadeHits3<<G4endl;
    file4Out<<TD <<"  "<<NHits4<<"  "<<NHits5<<"  "<<DiferenciadeHits4<<G4endl;
    //}

   */    

    G4cout<<"Hubieron estos hits :"<<G4endl;
    G4cout<<"P1"<<"  " <<"  "<<NHits1<<"  "<<NHits8<<"  "<<DiferenciadeHits1<<G4endl;
    G4cout<<"P2"<<"  " <<"  "<<NHits2<<"  "<<NHits7<<"  "<<DiferenciadeHits2<<G4endl;
    G4cout<<"P3"<<"  " <<"  "<<NHits3<<"  "<<NHits6<<"  "<<DiferenciadeHits3<<G4endl;
    G4cout<<"P4"<<"  " <<"  "<<NHits4<<"  "<<NHits5<<"  "<<DiferenciadeHits4<<G4endl;
    
    // }
    //else{
    file1Out<<"  "<<NHits1<<"  "<<NHits8<<"  "<<DiferenciadeHits1<<G4endl;
    file2Out<<"  "<<NHits2<<"  "<<NHits7<<"  "<<DiferenciadeHits2<<G4endl;
    file3Out <<"  "<<NHits3<<"  "<<NHits6<<"  "<<DiferenciadeHits3<<G4endl;
    file4Out<<"  "<<NHits4<<"  "<<NHits5<<"  "<<DiferenciadeHits4<<G4endl;
    //}

    
    
  DiferenciadeHits1=0;
  DiferenciadeHits2=0;
  DiferenciadeHits3=0;
  DiferenciadeHits4=0;

  NHits1=0;
  NHits2=0;
  NHits3=0;
  NHits4=0;
  NHits5=0;
  NHits6=0;
  NHits7=0;
  NHits8=0;
}
void bsneutronRunAction::ContarHits(G4int N1,G4int N2, G4int N3,   G4int N4,   G4int N5,   G4int N6,   G4int N7,   G4int N8){
  NHits1+=N1;
  NHits2+=N2;
  NHits3+=N3;
  NHits4+=N4;
  NHits5+=N5;
  NHits6+=N6;
  NHits7+=N7;
  NHits8+=N8;
  DiferenciadeHits1=NHits1-NHits8;
  DiferenciadeHits2=NHits2-NHits7;
  DiferenciadeHits3=NHits3-NHits6;
  DiferenciadeHits4=NHits4-NHits5;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



