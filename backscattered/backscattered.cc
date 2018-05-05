#include "MyAnalysisManager.hh"
#include "CrossSection.hh"
#include "Material.hh"
#include "RootImprove.hh"

#include "G4Gamma.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "TMath.h"
#include "TLegend.h"

class RootImprove;

double IntegralPorSimpson(double a, double b, int N, double f(double x, G4double na, G4double da,  G4double Za, G4double E0a) ,G4double n, G4double d,  G4double Z, G4double E0 )
{
  G4double na, da, Za, E0a;
  na= n; da=d; Za=Z; E0a= E0; 
    
  double integral;
  double h=(b-a)/N;
  double xi;
  double suma;
  suma=0;
  for (int i=0; i<=N; i++){
    xi=a+i*h;
    if((i==0) || (i==N) )
      suma +=f(xi, na, da, Za, E0a);
    else if(i%2==0)
      suma += 2*f(xi, na, da, Za, E0a);
    else
      suma += 4*f(xi, na, da, Za, E0a);
  }
  integral=(h/3.)*suma;
  return integral ;
}

double Integrando(double T, G4double n, G4double d,  G4double Z, G4double E0)
{
  double Ae = TMath::Pi()*pow(classic_electr_radius ,2);
  
  auto C= CrossSection::Instance();
  G4double  E1, e, e0, St1, St;
  e0 = 511*keV;  e = E0/e0 ; 
  St =   C->ComputeTotalCrossSectionPerAtom(E0,Z);
  E1= E0/(1+e*(1-cos(T)));
  St1 = C->ComputeTotalCrossSectionPerAtom(E1,Z);

  double Y, a, aux;
  a =( St1/cos(T) )- St  ;
  aux = e*(1-cos(T));
  Y= Ae*Z*(1/a)*( exp(n*a*d) -1)*pow(1+aux, -2)*(1+pow(cos(T),2) +pow(aux,2)/(1+aux))*sin(T);

  return Y;
}


int main() {

  RootImprove * R = new RootImprove(); 
  TStyle * AStyle = new TStyle("Andres", "");
  R->DefinirEstilo(AStyle);
  AStyle->cd();
  TCanvas *c2 = new TCanvas ("c2","",800,600);
  R->DefinirCanvas(c2);
  c2->cd();
  
  auto C= CrossSection::Instance();
  auto Mat= Material::Instance();
  
  G4UnitDefinition::BuildUnitsTable();

  //C->GetParticle() == G4Gamma::Gamma();
  C->GetCuts().push_back(0*keV);  
  C->Initialise();

  /*
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
  couple->SetIndex(0);
  C->GetPhotModel()->SetCurrentCouple(couple);
  */
 
  double T0, TF, dT, T;
  T0 = 0.5*TMath::Pi(); TF = TMath::Pi() ;
  int steps;
  steps =100;
  dT=(TF-T0)/steps;
  G4double E0, n, d , Z;



  // ******************************************************************
  // ********************* DEPENDENCIA DEL n **************************
  // ******************************************************************
  
  //DIFERENTES d
  /*
  TH1F *frame = gPad->DrawFrame(pow(10,12), pow(10,-6) , pow(10,21), 0.1);
  gPad->SetLogx();
  gPad->SetLogy();
  const TString sleg []= {"d=1 mm",  "d=1 cm",  "d=10 cm" , "d=1 m"}; 
  R->DefinirTitulos(frame, "","n [m^{-3}]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.2,0.6,0.4,0.9);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  
  Z = material->GetZ();
  E0 = 511*keV;
  TGraph *gr[10];			
  for (int k = 0; k <=3 ; ++k )
      {
	d=pow(10,k)*mm;
	gr[k]  = new TGraph(1);
	R->DefinirTGraph(gr[k] , R->MarkerStyle(1) , 0.2, 2, R->Color(k), R->Color(k) );
	for (int j = 0; j <=steps ; ++j )
	  {
	    n=pow(10,21-0.1*j)/mm3;
	    Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, Z, E0  );
	    gr[k]->SetPoint(j, n , Integral);
	    //G4cout << n << " " << Integral << G4endl;
	  }
	gr[k]->Draw("L");
	leg->AddEntry(gr[k] , sleg[k] , "L");
      }
  leg->Draw();
  
  // TF1 * f = new TF1("test","[0]*TMath::Power(x, [1]) ", pow(10,12),  pow(10,16));
  // R->DefinirTF1(f, 3, 10, R->Color(4) );
  //f->SetParName(0, "Amplitud");
  //f->SetParName(1, "Potencia");
  //f->SetParameters(1, 1);
 
  // gr[3]->Fit("test", "", " ", pow(10,12), pow(10,16));
  c2->Print("out/FE_RvsN_d.tex");
  */
  
  //DIFERENTE Z
  /*
  TH1F *frame = gPad->DrawFrame( pow(10,11) , pow(10,-8), pow(10,21) , .1 );
  gPad->SetLogx();
  gPad->SetLogy();
  const TString sleg []= {"C","Si",  "Fe", "Pb"};
  G4double ZZ[] = {6, 14, 26, 82};
  R->DefinirTitulos(frame, "","n [m^{-3}]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.6,0.3,0.7,0.6);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  d=pow(10,1)*m;
  
  E0=511*keV;
  
  TGraph *gr[10];
  for (int k = 0; k <=3 ; ++k )
      {
	G4Material* material = Mat->GetMaterial(ZZ[k] );
	G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	couple->SetIndex(0);
	C->GetPhotModel()->SetCurrentCouple(couple);
	
	gr[k]  = new TGraph(1);
	R->DefinirTGraph(gr[k] , R->MarkerStyle(1) , 0.2, 2, R->Color(k), R->Color(k) );
	for (int j = 0; j <=steps ; ++j )
	  {
	    n=pow(10,21-0.1*j)/mm3;
	    Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, ZZ[k] , E0  );
	    //cout<< "Sali" <<endl;
	    gr[k]->SetPoint(j, n , Integral);
	    // G4cout << n << " " << Integral << G4endl;
	  }
	gr[k]->Draw("L");
	leg->AddEntry(gr[k] , sleg[k] , "L");
      }
  leg->Draw();
  //TF1 * f = new TF1("test","[0]*TMath::Power(x, [1]) ", pow(10,12),  pow(10,16));
  // R->DefinirTF1(f, 3, 10, R->Color(4) );
  // f->SetParName(0, "Amplitud");
 // f->SetParName(1, "Potencia");
  //f->SetParameters(1, 1);
 
  // gr[3]->Fit("test", "", " ", pow(10,12), pow(10,16));
  c2->Print("out/RvsN_Z.tex");
  */
  
  

  //DIFERENTES E0
  /*
  TH1F *frame = gPad->DrawFrame( pow(10,11) , pow(10,-8), pow(10,21) , .1 );
  gPad->SetLogx();
  gPad->SetLogy();
   R->DefinirTitulos(frame, "","n [m^{-3}]", "R/I_{0}" );
  const TString sleg []= {"E_{#gamma} = 10 keV",  "E_{#gamma} = 100 keV" , "E_{#gamma} = 1 MeV", "E_{#gamma} = 10 MeV"};
  G4double EE0[] = {10*keV, 0.1*MeV, 1*MeV, 10*MeV};
 
  TLegend *leg= new TLegend( 0.6,0.2,0.9,0.5);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  d=10*m;
  Z = material->GetZ();
  TGraph *gr[10];
  for (int k = 0; k <=3 ; ++k )
      {
	gr[k]  = new TGraph(1);
	R->DefinirTGraph(gr[k] , R->MarkerStyle(1) , 0.2, 2, R->Color(k), R->Color(k) );
	for (int j = 0; j <=steps ; ++j )
	  {
	    n=pow(10,21-0.2*j)/mm3;
	    Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, Z, EE0[k]  );
	    gr[k]->SetPoint(j, n , Integral);
	    //G4cout << n << " " << Integral << G4endl;
	  }
	gr[k]->Draw("L");
	leg->AddEntry(gr[k] , sleg[k] , "L");
      }
  leg->Draw();
  
  TF1 * f = new TF1("test","[0]*TMath::Power(x, [1]) ", pow(10,12),  pow(10,16));
  //TF1 * f = new TF1("test","[0]*(1-TMath::Exp(-x* [1])) ", pow(10,12),  pow(10,21));
  R->DefinirTF1(f, 3, 10, R->Color(4) );
  f->SetParName(0, "Amplitud");
  f->SetParName(1, "Potencia");
  f->SetParameters(0.01, 1);
 
  gr[3]->Fit("test", "", " ", pow(10,12), pow(10,16));
   
  c2->Print("out/FE_RvsN_E0.eps");
  */

  // ******************************************************************
  // ********************* DEPENDENCIA DEL Z **************************
  // ******************************************************************
  
  /*
  AM->CreateTGraph(1, AM->Color(0) ); 
  AM->SetFrame( 0, 0, 100 , 0.1 );
  TLegend *leg= new TLegend( 0.2,0.5,0.3,0.8);
  double Integral;
  d=1*m;
  E0=511*keV;

  int i=1;
  for (int k = 1; k <=98 ; ++k )
      {
	if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
	G4Material* material = Mat->GetMaterial(k);
	G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	couple->SetIndex(0);
	C->GetPhotModel()->SetCurrentCouple(couple);
	n = material->GetTotNbOfAtomsPerVolume();
	Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, k, E0  );
	G4cout << k << " " << Integral << G4endl;
	AM->SetPointTGraph(0, i-1 , k, Integral );
	//G4cout << n << " " << Integral << G4endl;
	i+=1;
	  }
  
  AM->GetTGraph(0)->Draw("PSAME");
  AM->Print("out/FE_RvsZ.tex");
  */

  //Diferente E0
  /*
  TH1F *frame = gPad->DrawFrame( 0 , 0 , 100 , .14 );
  // gPad->SetLogx();
  //gPad->SetLogy();
  R->DefinirTitulos(frame, "","Z", "R/I_{0}" );
  const TString sleg []= {"E_{#gamma} = 50 keV",  "E_{#gamma} = 100 keV" , "E_{#gamma} = 500 keV", "E_{#gamma} = 1 MeV"};
  G4double EE0[] = {50*keV, 100*keV, 500*keV, 1*MeV};
  TLegend *leg= new TLegend( 0.6,0.5,0.9,0.9);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  d=pow(10,15)*m;
  TGraph *gr[10];
  for (int j = 0; j <=3 ; ++j )
    {
  	gr[j]  = new TGraph(1);
	R->DefinirTGraph(gr[j] , R->MarkerStyle(0) , 0.7, 2, R->Color(j), R->Color(j) );
	int i=1;
      for (int k = 1; k <=98 ; ++k )
	{
	  // if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
	  G4Material* material = Mat->GetMaterial(k);
	  G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	  couple->SetIndex(0);
	  C->GetPhotModel()->SetCurrentCouple(couple);
	  n = material->GetTotNbOfAtomsPerVolume();
	  Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, k, EE0[j]  );
	  //G4cout << k << " " << Integral << G4endl;
	  gr[j]->SetPoint(i-1, k , Integral);
	  i+=1;
	}
	leg->AddEntry(gr[j] , sleg[j] , "L");    
	gr[j]->Draw("PLSAME");    
    }
     
  leg->Draw();
  c2->Print("out/RvsZ_E0.tex");
  */
  
  //Diferente d
  /*
  TH1F *frame = gPad->DrawFrame( 0, 0, 100 , 0.08 );
  //gPad->SetLogx();
  //gPad->SetLogy();
  const TString sleg []= {"d=1 mm",  "d=1 cm",  "d=10 cm" , "d=1 m"};
  G4double dd[]= {1*mm, 1*cm, 10*cm, 1*m};
  R->DefinirTitulos(frame, "","Z", "R/I_{0}" );

  TLegend *leg= new TLegend( 0.65,0.5,0.9,0.9);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  E0=511*keV;
  TGraph *gr[10];
   for (int j = 0; j <=3 ; ++j )
      {
	gr[j]  = new TGraph(1);
	R->DefinirTGraph(gr[j] , R->MarkerStyle(0) , 0.7, 2, R->Color(j), R->Color(j) );
	int i=1;
	for (int k = 1; k <=98 ; ++k )
	  {
	    //if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
	    G4Material* material = Mat->GetMaterial(k);
	    G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	    couple->SetIndex(0);
	    C->GetPhotModel()->SetCurrentCouple(couple);
	    n = material->GetTotNbOfAtomsPerVolume();
	    Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, dd[j], k, E0  );
	    G4cout << k << " " << Integral << G4endl;
	    gr[j]->SetPoint(i-1, k , Integral);
	    //G4cout << n << " " << Integral << G4endl;
	    i+=1;
	  }
	leg->AddEntry(gr[j] , sleg[j] , "L");    
	gr[j]->Draw("PL");   
      }
   leg->Draw();
   c2->Print("out/RvsZ_d.tex");
  */


  // *********************************************************************
  // ********************* DEPENDENCIA con el d **************************
  // *********************************************************************
  /*
  AM->CreateTGraph(0, AM->Color(0) ); 
  AM->SetFrame( 0, 0, 5 , 0.1 );
  double Integral;
  d=0.1*mm;
  E0=511*keV;
  int i=1;
  for (int k = 1; k <=500 ; ++k )
    {
      //  if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
      G4Material* material = Mat->GetMaterial(26); //hierro
      G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
      couple->SetIndex(0);
      C->GetPhotModel()->SetCurrentCouple(couple);
      n = material->GetTotNbOfAtomsPerVolume();
      // G4cout << C->ComputeTotalMeanFreePath(E0, material) << G4endl;
      //G4cout << G4BestUnit(pow(C->ComputeTotalCrossSectionPerAtom(E0, 26)*n,-1),"Length") << G4endl;
      Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, 26, E0  );
      AM->SetPointTGraph(0, i-1 , d/cm, Integral );
      i+=1;
      d+=0.1*mm;
    }
  AM->GetTGraph(0)->Draw("LSAME");
  AM->Print("out/Rvsd_d.eps");
  */


  //Diferente Z
  /*
  TH1F *frame = gPad->DrawFrame( pow(10,-3), pow(10,-3), 40 , 0.08 );
  // gPad->SetLogx();
  //gPad->SetLogy();
  const TString sleg []= {"C","Si",  "Fe", "Pb"};
  G4double ZZ[] = {6, 14, 26, 82};
  R->DefinirTitulos(frame, "","d [cm]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.6,0.4,0.7,0.7);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  TGraph *gr[10];
  E0=511*keV;
  for (int k = 0; k <=3 ; k++)
       {
	 gr[k]  = new TGraph(1);
	 R->DefinirTGraph(gr[k] , R->MarkerStyle(0) , 0.7, 2, R->Color(k), R->Color(k) );
	 G4Material* material = Mat->GetMaterial(ZZ[k]);
       	 G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	 couple->SetIndex(0);
	 C->GetPhotModel()->SetCurrentCouple(couple);
	 n = material->GetTotNbOfAtomsPerVolume();
	 int i=1;  d=0.1*mm;
	 for (int j= 1; j <=10000 ; ++j )
	   {
	     Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, ZZ[k], E0  );
	     d+=0.1*mm;
	     gr[k]->SetPoint(i-1, d/cm , Integral);
	     //G4cout << n << " " << Integral << G4endl;
	     i+=1;
	   }
	 leg->AddEntry(gr[k] , sleg[k] , "L");    
	 gr[k]->Draw("L"); 
       }
  leg->Draw();
  c2->Print("out/Rvsd_Z.tex");
  */
  
  //Diferente E0
  /*
  TH1F *frame = gPad->DrawFrame( 0, 0, 3, 0.067 );
  // gPad->SetLogx();
  //gPad->SetLogy();
   R->DefinirTitulos(frame, "","d [cm]", "R/I_{0}" );
  const TString sleg []= {"E_{#gamma} = 100 keV",  "E_{#gamma} = 500 keV" , "E_{#gamma} = 1 MeV", "E_{#gamma} = 10 MeV"};
  G4double EE0[] = {100*keV, 0.5*MeV, 1*MeV, 10*MeV};
  R->DefinirTitulos(frame, "","d [cm]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.6,0.25,0.9,0.55);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
 TGraph *gr[10];
  int h=0;
  for (int s = 0; s <=3 ; s++)
       {
	 gr[s]  = new TGraph(1);
	 R->DefinirTGraph(gr[s] , R->MarkerStyle(0) , 0.7, 2, R->Color(s), R->Color(s) );
	 G4Material* material = Mat->GetMaterial(26);
       	 G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	 couple->SetIndex(0);
	 C->GetPhotModel()->SetCurrentCouple(couple);
	 n = material->GetTotNbOfAtomsPerVolume();
	 int i=1;  d=0.1*mm;
	 for (int j= 1; j <=500 ; ++j )
	   {
	     Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, 26, EE0[s]  );
	     gr[s]->SetPoint(i-1, d/cm , Integral);
	     i+=1;
	     d+=0.1*mm;
	   }
	 leg->AddEntry(gr[s] , sleg[s] , "L");    
	 gr[s]->Draw("L"); 
	 // G4cout << E0 << G4endl; 
	 h+=1; 
       }
  leg->Draw();
  c2->Print("out/Rvsd_E0.tex"); 
  */
  
  // ******************************************************************
  // ********************* DEPENDENCIA DE E0 **************************
  // ******************************************************************

  //DIFERENTES Z
  /*
  TH1F *frame = gPad->DrawFrame( 0, 0, 1000 , 0.15);
  //gPad->SetLogx();
  //gPad->SetLogy();
  const TString sleg []= {"C","Si",  "Fe", "Pb"};
  G4double ZZ[] = {6, 14, 26, 82};
  R->DefinirTitulos(frame, "","E_{#gamma} [keV]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.7,0.6,0.8,0.9);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  TGraph *gr[10];
  
  d=pow(10,15)*m;
  int i, j;
  j=0; 
  for(int h=0; h<=3; h++)
    {
      gr[h]  = new TGraph(1);
      R->DefinirTGraph(gr[h] , R->MarkerStyle(0) , 0.7, 2, R->Color(h), R->Color(h) );
      //if (z==1 || z==2 || z==7 || z==8 || z==9 || z==10 || z==17 || z==18 || z==35|| z==36 || z==54|| z==86 || z==87) {z+=1; continue;}
      G4Material* material = Mat->GetMaterial(ZZ[h]);
      G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
      couple->SetIndex(0);
      C->GetPhotModel()->SetCurrentCouple(couple);
      n = material->GetTotNbOfAtomsPerVolume();
      i=1; E0=1*keV;
      for (int k = 1; k <=1000 ; ++k )
	{
	  Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, d, ZZ[h] , E0  );
	  gr[h]->SetPoint(i-1, E0/keV , Integral);
	  i+=1;
	  E0+=1*keV;
	}
      leg->AddEntry(gr[h] , sleg[h] , "L");    
      gr[h]->Draw("L"); 
      j+=1;
    }
  
  leg->Draw();
  c2->Print("out/RvsE_Z.tex");
  */

  
  //Diferentes d
  /*
  TH1F *frame  = gPad->DrawFrame(0, 0 , 1000, 0.08);
  // gPad->SetLogx();
  // gPad->SetLogy();
  const TString sleg []= {"d=1 mm",  "d=5 mm",  "d=1 cm" , "d=5 cm"};
  G4double dd[]= {1*mm, 5*mm, 1*cm, 5*cm};
  R->DefinirTitulos(frame, "","E_{#gamma} [keV]", "R/I_{0}" );
  TLegend *leg= new TLegend( 0.75,0.7,0.94,0.94);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  TGraph *gr[10];
  double Integral;
  int i, z , j, h;
  j=0; z=0;

  for(h=0; h<=3 ; h++)
    {
      gr[h]  = new TGraph(1);
      R->DefinirTGraph(gr[h] , R->MarkerStyle(0) , 0.7, 2, R->Color(h), R->Color(h) );
      G4Material* material = Mat->GetMaterial(26); //hierro
      G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
      couple->SetIndex(0);
      C->GetPhotModel()->SetCurrentCouple(couple);
      n = material->GetTotNbOfAtomsPerVolume();
      i=1; E0=1*keV;
      for (int k = 1; k <=1000 ; ++k )
	{
	  Integral = IntegralPorSimpson(T0+0.001 , TF, steps , Integrando, n, dd[h], 26 , E0  );
	  gr[h]->SetPoint(i-1, E0/keV , Integral);
	  i+=1;
	  E0+=1*keV;
	}
      //G4cout << z << G4endl;
      leg->AddEntry(gr[h] , sleg[h] , "L");    
      gr[h]->Draw("L"); 
      j+=1;
    }
  leg->Draw();
  c2->Print("out/RvsE_d.tex");
  */



  
  // *****************************************************************************************
  // ********************* Camino libre medio diferentes materiales **************************
  // *****************************************************************************************


  // Funcion energia.
  G4double MeanFreePath;
  double minx, maxx;
  int points = pow(10,6);
  minx= pow(10,-4); maxx= pow(10, 4);
  double dx= (maxx-minx) / points;

  G4double Emin = minx*MeV, Emax = maxx*MeV, dl = dx*MeV;
  G4double E;
  double l0= log10(Emin) ; double l;
  int i=1;
  int h =0;
  TH1F *frame = gPad->DrawFrame( minx, pow(10,-6), maxx , pow(10,2) );
  gPad->SetLogx();
  gPad->SetLogy();
  const TString sleg []= {"C","Si",  "Fe", "Pb"};
  G4double ZZ[] = {6, 14, 26, 82};
  R->DefinirTitulos(frame, "","E_{#gamma} [MeV]", "#lambda [cm]" );
  TLegend *leg= new TLegend( 0.7,0.4,0.8,0.7);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  TGraph *gr[10];
   
  
  for (int k = 0; k <= 3; ++k)
    {
      gr[k]  = new TGraph(1);
      R->DefinirTGraph(gr[k] , R->MarkerStyle(0) , 0.7, 2, R->Color(k), R->Color(k) );
      G4Material* material = Mat->GetMaterial(ZZ[k]);
      G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
      couple->SetIndex(0);
      C->GetPhotModel()->SetCurrentCouple(couple);
      n = material->GetTotNbOfAtomsPerVolume();
      for (E = Emin, l=l0 ; E <= Emax; l+=dl, i+=1, E = pow(10, l)  )
	{
	  MeanFreePath =  pow(C->ComputeTotalCrossSectionPerAtom(E, ZZ[k])*n,-1);
	  gr[k]->SetPoint(i-1, E/MeV , MeanFreePath/cm);
	}
      leg->AddEntry(gr[k] , sleg[k] , "L");    
      gr[k]->Draw("L"); 
    }
  leg->Draw();
  c2->Print("out/MFPvsE_Z.tex");
  

  //Dependencia con Z
  /*
  TH1F *frame = gPad->DrawFrame( 0, 0.5*pow(10,-2), 100, pow(10,6) );
  // gPad->SetLogx();
  gPad->SetLogy();
  R->DefinirTitulos(frame, "","Z", "#lambda [cm]" );
  const TString sleg []= {"E_{#gamma} = 100 keV",  "E_{#gamma} = 500 keV" , "E_{#gamma} = 1 MeV", "E_{#gamma} = 10 MeV"};
  G4double EE0[] = {100*keV, 0.5*MeV, 1*MeV, 10*MeV};
  TLegend *leg= new TLegend( 0.6,0.7,0.9,0.99);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  double Integral;
  TGraph *gr[10];


  G4double MeanFreePath;
 
  int i=0;
  int z ;
  int h =0;
  for (int k = 0; k <= 3; ++k)
  {
      gr[k]  = new TGraph(1);
      R->DefinirTGraph(gr[k] , R->MarkerStyle(0) , 0.7, 2, R->Color(k), R->Color(k) );
      z=1;
      for (z = 1; z<=98; z++ )
	{
	  
	  G4Material* material = Mat->GetMaterial(z);
	  G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(material);
	  couple->SetIndex(0);
	  C->GetPhotModel()->SetCurrentCouple(couple);
	  n = material->GetTotNbOfAtomsPerVolume();
	  MeanFreePath =  pow(C->ComputeTotalCrossSectionPerAtom(EE0[k], z)*n,-1);
	  gr[k]->SetPoint(i-1, z , MeanFreePath/cm);
	  i+=1;
	}
      leg->AddEntry(gr[k] , sleg[k] , "L");    
      gr[k]->Draw("PL");
  }
  leg->Draw();
  c2->Print("out/MFPvsZ_E0.tex");
 
*/
  
  // *****************************************************************************************
  // ************************* DENSIDAD DE CENTROS DISPERSORES *******************************
  // *****************************************************************************************
  /*
  // TH1F *frame = gPad->DrawFrame( 0, pow(10,19), 100, 0.2*pow(10,24) ); //n
  TH1F *frame = gPad->DrawFrame( 0, pow(10,0),  100, pow(10,5) );//Vm
  //TH1F *frame = gPad->DrawFrame( 0, 5*pow(10,-5), 100, 0.5*pow(10,2) ); //rho
  // gPad->SetLogx();
  gPad->SetLogy();
  //R->DefinirTitulos(frame, "","Z", "n [cm^{-3}]" );
  R->DefinirTitulos(frame, "","Z", "Vm [cm^{3}/mol]" );
  //R->DefinirTitulos(frame, "","Z", "#rho [g/cm^{3}]" );
  TLegend *leg= new TLegend( 0.6,0.7,0.9,0.99);
  R->DefinirLegend(leg, 2, kWhite , 0.05 );
  TGraph *gr = new TGraph(1);
  R->DefinirTGraph(gr , R->MarkerStyle(0) , 0.9, 2, R->Color(1), R->Color(1) );
  int i=1;
  G4double Vm, rho;
  
  for (int k = 1; k <= 98; ++k)
    {
      // if (k==1 || k==2 || k==7 || k==8 || k==9 || k==10 || k==17 || k==18 || k==35|| k==36 || k==54|| k==86 || k==87) {continue ;}
      G4Material* Tem =  Mat->GetMaterial(k);
      n = Tem->GetTotNbOfAtomsPerVolume();
      rho = Tem->GetDensity();
      Vm = Avogadro/n;
      //G4cout<<k<<" "<<n*cm3<<" "<<Vm/cm3 <<G4endl;
      //gr->SetPoint( i-1 , k , n*cm3);
      gr->SetPoint( i-1 , k , Vm/cm3);
      // gr->SetPoint( i-1 , k , rho/(g/cm3));
      i+=1;
    }
  gr->Draw("PL");
  c2->Print("out/VmvsZ.tex");
  //c2->Print("out/nvsZ.tex");
  //c2->Print("out/rhovsZ.tex");
 
  */
  
  return 0;
}




