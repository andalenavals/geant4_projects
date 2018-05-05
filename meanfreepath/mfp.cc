#include "MyAnalysisManager.hh"
#include "CrossSection.hh"
#include "Material.hh"

#include "G4Gamma.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "TMath.h"
#include "TLegend.h"


int main() {
  
  auto C= CrossSection::Instance();
  auto Mat= Material::Instance();
  auto AM= MyAnalysisManager::Instance();
  
  G4UnitDefinition::BuildUnitsTable();

  C->GetCuts().push_back(1*keV);  
  C->Initialise();
  G4MaterialCutsCouple* couple ;

  //**************************************************
  //************Materiales de Construccion************
  //**************************************************
  
  // TLegend *leg= new TLegend( 0.7,0.5,0.8,0.8);
  TLegend *leg= new TLegend( 0.7,0.3,0.8,0.6);
  TString sleg [24];
  sleg [0]= "Cement";  sleg [4]= "Asphalt"; 
  sleg [1]= "Lime";
  sleg [2]= "Sand";
  sleg [3]= "POP"; 
  
 
 
 G4double MFP, mu, density;
 double minx, maxx;
 int points = pow(10,8);
 minx= pow(10,-4); maxx= pow(10, 4);
 double dx= (maxx-minx) / points;
 
 G4double Emin = minx*MeV, Emax = maxx*MeV, dl = dx*MeV;
 G4double E;
 double l0= log10(Emin) ; double l;
 int i;
 
 gPad->SetLogx();
 gPad->SetLogy();
 // AM->SetFrame(minx, pow(10,-3), maxx, pow(10,5), "", "Energy [MeV]", "#mu/#rho [cm^{2}/g]" );
 AM->SetFrame(minx, pow(10,-5), maxx, pow(10,3), "", "Energy [MeV]", "#lambda [cm]" );

 //-------------
 //---Cement----
 //-------------
 AM->CreateTGraph(1, AM->Color(0) ); 
 G4Material* cement = Mat->GetMaterial("Cement");
 density = cement->GetDensity();
 couple= new G4MaterialCutsCouple(cement);
 couple->SetIndex(0);
 C->GetPhotModel()->SetCurrentCouple(couple);
 for (E = Emin, l=l0,  i=1; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
   {
     mu = C->ComputeMassAttenuationCoefficient(E, cement);
     MFP =  pow(density*mu,-1);
     //AM->SetPointTGraph(0, i, E/MeV, mu*(g/cm2));
     AM->SetPointTGraph(0, i, E/MeV, MFP/cm);
     // G4cout<<E/MeV<<" "<<mu*(g/cm2) <<G4endl;
   }

 //-------------
 //-----Lime----
 //-------------
 AM->CreateTGraph(1, AM->Color(1) ); 
 G4Material* lime = Mat->GetMaterial("Lime");
 density = lime->GetDensity();
 couple= new G4MaterialCutsCouple(lime);
 couple->SetIndex(0);
 C->GetPhotModel()->SetCurrentCouple(couple);
 for (E = Emin, l=l0, i=1 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
   {
     mu = C->ComputeMassAttenuationCoefficient(E, lime);
     MFP =  pow(density*mu,-1);
     //AM->SetPointTGraph(1, i, E/MeV, mu*(g/cm2));
     AM->SetPointTGraph(1, i, E/MeV, MFP/cm);
   }

 
 //-------------
 //-----Sand----
 //-------------
 AM->CreateTGraph(1, AM->Color(2) ); 
 G4Material* sand = Mat->GetMaterial("Sand");
 density = sand->GetDensity();
 couple= new G4MaterialCutsCouple(sand);
 couple->SetIndex(0);
 C->GetPhotModel()->SetCurrentCouple(couple);
 for (E = Emin, l=l0, i=1 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
   {
     mu = C->ComputeMassAttenuationCoefficient(E, sand);
     MFP =  pow(density*mu,-1);
     //AM->SetPointTGraph(2, i, E/MeV, mu*(g/cm2));
     AM->SetPointTGraph(2, i, E/MeV, MFP/cm);
   }

 //-------------
 //-----POP-----
 //-------------
 AM->CreateTGraph(1, AM->Color(3) ); 
 G4Material* pop = Mat->GetMaterial("POP");
 density = pop->GetDensity();
 couple= new G4MaterialCutsCouple(pop);
 couple->SetIndex(0);
 C->GetPhotModel()->SetCurrentCouple(couple);
 for (E = Emin, l=l0, i=1 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
   {
     mu = C->ComputeMassAttenuationCoefficient(E, pop);
     MFP =  pow(density*mu,-1);
     // AM->SetPointTGraph(3, i, E/MeV, mu*(g/cm2));
     AM->SetPointTGraph(3, i, E/MeV, MFP/cm);
   }

 
 //-------------
 //---Asphalt---
 //-------------
 /*
 AM->CreateTGraph(1, AM->Color(4) ); 
 G4Material* asphalt = Mat->GetMaterial("Asphalt");
 couple= new G4MaterialCutsCouple(asphalt);
 couple->SetIndex(0);
 C->GetPhotModel()->SetCurrentCouple(couple);
 for (E = Emin, l=l0, i=1 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
   {
     mu = C->ComputeMassAttenuationCoefficient(E, asphalt);
     MFP = C->ComputeMeanFreePath(E, asphalt);
     //AM->SetPointTGraph(4, i, E/MeV, mu*(g/cm2));
     AM->SetPointTGraph(4, i, E/MeV, MFP/cm);
   }

 */

 

 for (int j = 0; j <= 3; j++) {
   leg->AddEntry(AM->GetTGraph(j) , sleg[j] , "P");
   AM->GetTGraph(j)->Draw("LSAME");
 }
 leg->Draw();
 AM->Print("out/MeanFreePath2.eps");


 
 // G4cout << asphalt->GetNumberOfElements() <<G4endl;
 //G4cout << asphalt->GetElement(1) <<G4endl;
 // G4cout << asphalt->GetFractionVector()[1] <<G4endl;
 G4cout <<"Cemento MFP:"<<" "<< C->ComputeMeanFreePath(511*keV, cement)/cm <<" cm"<<G4endl;
 G4cout <<"Lime MFP:"<<" "<< C->ComputeMeanFreePath(511*keV, lime)/cm <<" cm"<<G4endl;
 G4cout <<"Sand MFP:"<<" "<< C->ComputeMeanFreePath(511*keV, sand)/cm <<" cm"<<G4endl;
 G4cout <<"POP MFP:"<<" "<< C->ComputeMeanFreePath(511*keV, pop)/cm <<" cm"<<G4endl;
 //G4cout <<"Asphalt MFP:"<<" "<< C->ComputeMeanFreePath(511*keV, asphalt)/cm <<" cm"<<G4endl;

 
 


























  
  // Funcion energia.
  /*
  TLegend *leg= new TLegend( 0.7,0.3,0.8,0.6);
  TString sleg [24];
  sleg [0]= "Z = 3 ";  sleg [1]= "Z = 13";  sleg [2]= "Z = 23"; 
  sleg [3]= "Z = 33";  sleg [4]= "Z = 43";  sleg [5]= "Z = 53"; 
  sleg [6]= "Z = 63";  sleg [7]= "Z = 81";  sleg [8]= "Z = 15";
  sleg [9]= "Z = 16";  sleg [10]= "Z = 19"; sleg [11]= "Z = 20";
  sleg [12]= "Z = 21"; sleg [13]= "Z = 22"; sleg [14]= "Z = 23";
  sleg [15]= "Z = 24"; sleg [16]= "Z = 25"; sleg [17]= "Z = 26";
  sleg [18]= "Z = 27"; sleg [19]= "Z = 28"; sleg [20]= "Z = 29";
  sleg [21]= "Z = 30";  
  
  G4double MeanFreePath, n;
  double minx, maxx;
  int points = pow(10,8);
  minx= pow(10,-4); maxx= pow(10, 4);
  double dx= (maxx-minx) / points;

  G4double Emin = minx*MeV, Emax = maxx*MeV, dl = dx*MeV;
  G4double E;
  double l0= log10(Emin) ; double l;
  int i, z, h;
  i=1; z=1; h=0;
 
  gPad->SetLogx();
  gPad->SetLogy();
  AM->SetFrame( minx, pow(10,-6), maxx , pow(10,3) , "", "Energy [MeV]",
		"Mean Free Path [cm]" );
  
  for (int k = 1; k <= 8; ++k)
    {
      if (z==1 || z==2 || z==7 || z==8 || z==9 || z==10 || z==17 ||
	  z==18 || z==35|| z==36 || z==54|| z==86 || z==87) {z+=1; continue;}
      AM->CreateTGraph(1, AM->Color(h) ); 
      G4Material* material = Mat->GetMaterial(z);
      G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
      couple->SetIndex(0);
      C->GetPhotModel()->SetCurrentCouple(couple);
      n = material->GetTotNbOfAtomsPerVolume();
      for (E = Emin, l=l0 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
	{
	  //MeanFreePath =  C->ComputeTotalMeanFreePath(E, material);
	  MeanFreePath =  pow(C->ComputeTotalCrossSectionPerAtom(E, z)*n,-1);
	  AM->SetPointTGraph(h, i, E/MeV, MeanFreePath/cm);
	}
      G4cout << z << G4endl;
      leg->AddEntry(AM->GetTGraph(h) , sleg[h] , "P");
      AM->GetTGraph(h)->Draw("LSAME");
      h+=1; z+=10; 
    }
  leg->Draw();
  AM->Print("out/MeanFreePath.eps");
 */


  //Dependencia con Z
  /*
  TLegend *leg= new TLegend( 0.8,0.65,0.9,0.9);
  TString sleg [24];
  sleg [0]= "E = 100 keV "; sleg [1]= "E = 200 keV ";  sleg [2]="E = 300 keV ";
  sleg [3]= "E = 400 keV "; sleg [4]= "E = 500 keV "; sleg [5]= "E = 600 keV ";
  sleg [6]= "E = 700 keV "; sleg [7]= "E = 800 keV "; sleg [8]= "E = 900 keV "; 


  AM->SetFrame( 0, pow(10,-3), 100 , pow(10,2) );
  G4double MeanFreePath, n;

  int i, z, h;
  i=1; h=0;


  
  G4double E = 100*keV;
  for (int k = 1; k <= 8; ++k)
    { 
      AM->CreateTGraph(1, AM->Color(h) ); 
      z=1;
      for (z = 1; z<=98; z++ )
	{
	  if (z==1 || z==2 || z==7 || z==8 || z==9 || z==10 || z==17 || z==18 || z==35|| z==36 || z==54|| z==86 || z==87) {z+=1; continue;}
	  
	  G4Material* material = Mat->GetMaterial(z);
	  G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	  couple->SetIndex(0);
	  C->GetPhotModel()->SetCurrentCouple(couple);
	  n = material->GetTotNbOfAtomsPerVolume();
	  MeanFreePath =  pow(C->ComputeTotalCrossSectionPerAtom(E, z)*n,-1);
	  AM->SetPointTGraph(h, i-1, z, MeanFreePath/cm);
	  i+=1;
	}
      // G4cout << z << G4endl;
      leg->AddEntry(AM->GetTGraph(h) , sleg[h] , "P");
      AM->GetTGraph(h)->Draw("PLSAME");
      h+=1; E+=100*keV;
       }
  leg->Draw();
  AM->Print("out/MeanFreePath.eps");
  */

  return 0;
}




