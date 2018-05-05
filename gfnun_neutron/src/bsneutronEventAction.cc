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
// $Id: ExN02EventAction.cc,v 1.10 2004/09/08 16:30:10 johna Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "bsneutronEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"


//agragadas Mayo2........para lo de los hits
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "MyHit.hh"

//December 03-2009 hits of the DLM2
#include "HitDLM2.hh"


#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

//********Mayo 4...............
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bsneutronEventAction::bsneutronEventAction()
{
  //*********Mayo 4 sigo con lo de los hits
  G4RunManager* theRunManager =  G4RunManager::GetRunManager();
  runAction=(bsneutronRunAction*)theRunManager->GetUserRunAction();  
  
  //*********MAyo 2Para agarrar los hits de cada detector.......
  G4String colName;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  HHC1ID = SDman->GetCollectionID(colName="detector1/detectorCol");
  HHC2ID = SDman->GetCollectionID(colName="detector2/detectorCol");
  HHC3ID = SDman->GetCollectionID(colName="detector3/detectorCol");
  HHC4ID = SDman->GetCollectionID(colName="detector4/detectorCol");
  HHC5ID = SDman->GetCollectionID(colName="detector5/detectorCol");
  HHC6ID = SDman->GetCollectionID(colName="detector6/detectorCol");
  HHC7ID = SDman->GetCollectionID(colName="detector7/detectorCol");
  HHC8ID = SDman->GetCollectionID(colName="detector8/detectorCol");

  HHC_DLM2ID = SDman->GetCollectionID(colName="DLM2/DLM2collection");
  
  
  
  
  
  G4cout <<"contructor de eventAction: "<<G4endl;
  G4cout <<"ids  de las colecciones"<<G4endl;
  G4cout <<HHC1ID<<G4endl;
  G4cout <<HHC2ID<<G4endl;
  G4cout <<HHC3ID<<G4endl;
  G4cout <<HHC4ID<<G4endl;
  G4cout <<HHC5ID<<G4endl;
  G4cout <<HHC6ID<<G4endl;
  G4cout <<HHC7ID<<G4endl;
  G4cout <<HHC8ID<<G4endl;
  G4cout <<HHC_DLM2ID<<G4endl;
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bsneutronEventAction::~bsneutronEventAction()
{
  //G4cout << "oooOOOooo Aqui termina bsneutronEventAction.cc oooOOOooo"<<G4endl;
  myNtuple.closeNtuple();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void bsneutronEventAction::BeginOfEventAction(const G4Event*)
{
  //G4cout << "oooOOOooo Aqui empieza bsneutronEventAction.cc oooOOOooo"<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void bsneutronEventAction::EndOfEventAction(const G4Event* evt)
{
  //***************************************************************************************************************
  //*********Mayo 2 agregadas  para lo de los hits
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  G4int n;
  n=HCE->GetNumberOfCollections();
  //G4cout << "Numero de colecciones " << n <<G4endl;
  
  MyHitsCollection* HHC1 = 0;
  MyHitsCollection* HHC2 = 0;
  MyHitsCollection* HHC3 = 0;
  MyHitsCollection* HHC4 = 0;
  MyHitsCollection* HHC5 = 0;
  MyHitsCollection* HHC6 = 0;
  MyHitsCollection* HHC7 = 0;
  MyHitsCollection* HHC8 = 0;
  
  //Dec 3-2009
  HitDLM2sCollection* HHC_DLM2=0;

  

  
  if(HCE)
    {
      HHC1 = (MyHitsCollection*)(HCE->GetHC(HHC1ID));
      HHC2 = (MyHitsCollection*)(HCE->GetHC(HHC2ID));
      HHC3 = (MyHitsCollection*)(HCE->GetHC(HHC3ID));
      HHC4 = (MyHitsCollection*)(HCE->GetHC(HHC4ID));
      HHC5 = (MyHitsCollection*)(HCE->GetHC(HHC5ID));
      HHC6 = (MyHitsCollection*)(HCE->GetHC(HHC6ID));
      HHC7 = (MyHitsCollection*)(HCE->GetHC(HHC7ID));
      HHC8 = (MyHitsCollection*)(HCE->GetHC(HHC8ID));
      //Dec 03-2009.
      HHC_DLM2= (HitDLM2sCollection*)(HCE->GetHC(HHC_DLM2ID));
    }
  
  int n1=0;
  int n2=0;
  int n3=0;
  int n4=0;
  int n5=0;
  int n6=0;
  int n7=0;
  int n8=0;
  
  //########################################################
  //Dec 03-2009
  //If there is any entry on the DLM2 hits collection...
  if(HHC_DLM2)
    {
      int  nhit = HHC_DLM2->entries();
      if(nhit!=0){
	
	G4double kinetic_energy_before=0;//the first entry
	kinetic_energy_before=(*HHC_DLM2)[0]->GetKineticEnergy();
	
	G4double kinetic_energy_after=0;//the last entry.....
	kinetic_energy_after=(*HHC_DLM2)[nhit-1]->GetKineticEnergy();
	
	//kinetic_energy=(*HHC_DLM2)[0]->GetKineticEnergy()/keV;
	G4String Name= (*HHC_DLM2)[0]-> GetParticleName();
	if(Name=="neutron"){
	  myNtuple.fillVarsKE_before((float)kinetic_energy_before);
	  myNtuple.fillVarsKE_after((float)kinetic_energy_after);
	}
	
      }
    }   
  //########################################################
  if(HHC1)
    {
      int  n1_hit = HHC1->entries();
      n1=n1_hit;
      G4double energia_depositada=0;
      for(int i=0;i<n1_hit; i++)
	{energia_depositada +=(*HHC1)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      sigma = FWHM/2.35;
      //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2MeV
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars1((float)energiadepo1);
	n1=1;
      }
      else
	n1=0;
      
      /*if (energia_depositada>0)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVarsA((float)energiadepo1);**/
    }
  
  if(HHC8)
    {
      int  n8_hit = HHC8->entries();
      n8=n8_hit;
      
      G4double energia_depositada=0;
      G4double energia_depositada_gammas=0;
      G4double energia_depositada_fast=0;
      G4double energia_depositada_thermal=0;
 
      for(int i=0;i<n8_hit; i++){
	MyHit* aHit = (*HHC8)[i];
	G4String Name=aHit -> GetParticleName();
	//April 21-2009. I commented this line because what I want is the initial kinetic energy.
	//G4double K=aHit -> GetKineticEnergy();
	G4double K= (*HHC8)[0]-> GetKineticEnergy();
	//energia_depositada +=aHit->GetEdep(); 
	energia_depositada +=aHit->GetEdep(); 
	if(Name=="He3"|| Name=="Argon")
	  energia_depositada_fast +=aHit->GetEdep();
	if((Name=="proton" && K>600.0*keV) || (Name=="triton" && K>200.0*keV) )
	  energia_depositada_fast +=aHit->GetEdep();	
	if((Name=="proton" && K<600.0*keV) || (Name=="triton" && K<200.0*keV) )
	  energia_depositada_thermal +=aHit->GetEdep();	
	
	if(Name=="e-" ||Name=="e+")
	  energia_depositada_gammas +=aHit->GetEdep();
	
	if(Name=="gamma")
	  energia_depositada_gammas +=aHit->GetEdep();
	
       }
      
      
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      double energiadepo1_fast=0.;
      double energiadepo1_gammas=0.;
      double energiadepo1_thermal=0.;
    
      //FWHM = 0.09621*sqrt(energia_depositada/MeV);//para obtener una resolucion del 11% en el pico termico..
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;
      //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2*MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars8((float)energiadepo1);
	myNtuple.fill_total((float)energiadepo1);
	n8=1;
      }
      else
	//nB=0;
	n8=0;
      //Para llenar los histogramas con los neutrones térmicos
      if(energia_depositada_thermal>=0.1*keV){
	//energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	//April 23-2009 . new histogram.
	//
	//without FWHM...(April 17-2009)
	//myNtuple.fillVarsB((float)energia_depositada);
	energiadepo1_thermal = G4RandGauss::shoot( energia_depositada_thermal, sigma);  
	myNtuple.fillVars8_thermal((float)energiadepo1_thermal);
	//April 23-2009 . new histogram.
	myNtuple.fill_thermal((float)energiadepo1_thermal);
	
	//without FWHM...(April 17-2009)
	//myNtuple.fillVarsB_thermal((float)energia_depositada_thermal);
      }
      
      //Para llenar los histogramas con los neutrones rápidos
      if(energia_depositada_fast>=0.1*keV){
	energiadepo1_fast = G4RandGauss::shoot( energia_depositada_fast, sigma);  
	myNtuple.fillVars8_fast((float)energiadepo1_fast);

	//April 23-2009 . new histogram.
	myNtuple.fill_fast((float)energiadepo1_fast);
	}

      //Para llenar los histogramas con los gammas
      if(energia_depositada_gammas>=0.1*keV){
	energiadepo1_gammas = G4RandGauss::shoot(energia_depositada_gammas, sigma);  
	myNtuple.fillVars8_gammas((float)energiadepo1_gammas);
	//April 23-2009 . new histogram.
	myNtuple.fill_gammas((float)energiadepo1_gammas);
      }
      /*if (energia_depositada>0.)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVarsB((float)energiadepo1);*/
      
    }
 if(HHC3)
    {
      int  n3_hit = HHC3->entries();
      n3=n3_hit;
      G4double energia_depositada=0;
      for(int i=0;i<n3_hit; i++)
	{energia_depositada +=(*HHC3)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;

       //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars3((float)energiadepo1);
	n3=1;
      }
      else
	n3=0;

      /*      if (energia_depositada>0.)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVars1((float)energiadepo1);*/
    }
 if(HHC4)
    {
      int  n4_hit = HHC4->entries();
      n4=n4_hit;
       G4double energia_depositada=0;
      for(int i=0;i<n4_hit; i++)
	{energia_depositada +=(*HHC4)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;
      
 //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars4((float)energiadepo1);
	n4=1;
      }
      else
	n4=0;

      /*      if (energia_depositada>0.)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVars2((float)energiadepo1);*/
    }
 if(HHC5)
    {
      int  n5_hit = HHC5->entries();
      n5=n5_hit;
       G4double energia_depositada=0;
      for(int i=0;i<n5_hit; i++)
	{energia_depositada +=(*HHC5)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;
      
 //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars5((float)energiadepo1);
	n5=1;
      }
      else
	n5=0;
      
      /*if (energia_depositada>0.)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVars3((float)energiadepo1);*/
    }
 if(HHC6)
    {
      int  n6_hit = HHC6->entries();
      n6=n6_hit;
       G4double energia_depositada=0;
      for(int i=0;i<n6_hit; i++)
	{energia_depositada +=(*HHC6)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;
      
 //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars6((float)energiadepo1);
	n6=1;
      }
      else
	n6=0;

      /*  if (energia_depositada>0.)
	  energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	  
	  if (energiadepo1>0)
	  myNtuple.fillVars4((float)energiadepo1);*/
    }
 if(HHC7)
    {
      int  n7_hit = HHC7->entries();
      n7=n7_hit;
       G4double energia_depositada=0;
      for(int i=0;i<n7_hit; i++)
	{energia_depositada +=(*HHC7)[i]->GetEdep(); }
      double FWHM = 0.;
      double sigma = 0.;
      double energiadepo1=0.;
      
      FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
      //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
      
      sigma = FWHM/2.35;
      
      
 //*************Marzo 2 2007 pongo a llenar el histograma y las cuentas solo si la energia es mayor que 0.2
      if (energia_depositada>=0.2/MeV){
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	myNtuple.fillVars7((float)energiadepo1);
	n7=1;
      }
      else
	n7=0;
      
      /*if (energia_depositada>0.)
	energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
	
	if (energiadepo1>0)
	myNtuple.fillVars5((float)energiadepo1);*/
    }
 if(HHC2)
   {
     int  n2_hit = HHC2->entries();
     n2=n2_hit;
     G4double energia_depositada=0;
     for(int i=0;i<n2_hit; i++)
       {energia_depositada +=(*HHC2)[i]->GetEdep(); }
     double FWHM = 0.;
     double sigma = 0.;
     double energiadepo1=0.;
     
     FWHM = 0.061185*sqrt(energia_depositada/MeV);//para obtener una resolucion del 7% en el pico termico..
     //FWHM = 0.0281442*sqrt(energia_depositada/MeV);//para obtener el fwhm del espectro de prueba.......  
     
     sigma = FWHM/2.35;

     //****************Marzo 4 de 2007 solo voy a generar los hits si la energia depositada es mayor que 0.2 MeV     
     if (energia_depositada>0.2/MeV){
       energiadepo1 = G4RandGauss::shoot( energia_depositada, sigma);  
       myNtuple.fillVars2((float)energiadepo1);
       n2=1;
     }
     else
       n2=0;
   }
 //G4int DiferenciaDeCuentas;
 //DiferenciaDeCuentas=NA_total-NB_total;
 //  G4cout << "Hubo.. " <<n1_hit<<"  Hits, en el Detector A"<< G4endl;
 // G4cout << "Hubo.. " <<n2_hit <<"  Hits, en el Detector B"<< G4endl;*/
 //G4cout << "Diferencia......." <<DiferenciaDeCuentas<< G4endl;
 //MAyo 4****************************************************************** Para lo de los hits
 if(n1>0)
   n11=1;
 else
   n11=0;
 
 if(n2>0)
   n22=1;
 else
   n22=0;  

 if(n3>0)
   n33=1;
 else
   n33=0;  
 
 if(n4>0)
   n44=1;
 else
   n44=0;  

if(n5>0)
   n55=1;
 else
   n55=0;  

if(n6>0)
   n66=1;
 else
   n66=0; 

 if(n7>0)
   n77=1;
 else
   n77=0;
 
 if(n8>0)
   n88=1;
 else
   n88=0;

 runAction->ContarHits(n11,n22,n33,n44,n55,n66,n77,n88);
  
  //***************************************************************************************************************
  //***************************************************************************************************************
  G4int event_id = evt->GetEventID();
  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
  if (event_id%100000 == 0) {
    // 16/03/07   comente estas lineas para que me muestre solo lo intesante
    G4cout << "#####################################################################################" << G4endl;
    G4cout << ">>> Va en el evento " << evt->GetEventID() << G4endl;
    G4cout << "#####################################################################################" << G4endl;
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
