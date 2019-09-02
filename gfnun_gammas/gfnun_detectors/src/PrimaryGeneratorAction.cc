/**
 * @file
 * @brief implements mandatory user class PrimaryGeneratorAction
 */

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include <cmath>
using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  position = G4ThreeVector();
  gun = new G4ParticleGun(n_particle);
  particleGun = new G4GeneralParticleSource ();

  Energy =0.*keV;
  Theta_min = 0.*rad;
  N_steps = 200;
  KN_AF = new G4double [N_steps+1];

  for(G4int i=0;i<N_steps+1;i++)
    KN_AF[i]=0.;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
  delete particleGun;
  delete KN_AF;
}

////////////////////////////////////////////
///// ********  Particle Gun  ******** /////
////////////////////////////////////////////
/*
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();  
  /////---------- Radioactive sources ---------////////////
  // G4IonTable* ionTable =  const_cast <G4IonTable *> (particleTable->GetIonTable());
  G4IonTable* ionTable =  G4IonTable::GetIonTable();

  gun->SetParticleDefinition(ionTable->GetIon(11,22,0.)); // 22Na
  //gun->SetParticleDefinition(particleTable->GetIon(25,54,0.)); // 54Mn
  //gun->SetParticleDefinition(particleTable->GetIon(27,60,0.)); // 60Co
  //gun->SetParticleDefinition(particleTable->GetIon(55,137,0.)); // 137Cs
  
  gun->SetParticleEnergy(0.*keV);
  // Location   
  gun->SetParticlePosition(position);
  gun->GeneratePrimaryVertex(anEvent);
  
  ///--------- gamma --------///
  
  // gun->SetParticleDefinition(particleTable->FindParticle("gamma"));

  // gun->SetParticleEnergy(1000.0*keV);
  //  gun->SetParticleMomentumDirection(G4ThreeVector(0,1,0));
  // gun->SetParticlePosition(position);
  // gun->GeneratePrimaryVertex(anEvent);
  
    ///----- Cone: gamma -> Klein-Nishina distribution ------///
  
  // gun->SetParticleDefinition(particleTable->FindParticle("gamma"));
  // G4double x = position.getX();
  // G4double E_gamma = 511*keV; 
  // Calculate_Theta_min(x);
  // ConstructAF(E_gamma);
  // G4double Theta=GetTheta();
  // G4double E = GetEnergy(Theta,E_gamma); // in keV
  //   G4double theta = (M_PI-Theta);
  // G4double phi = (G4UniformRand())*M_PI*2*rad;
  // G4double u1=sin(phi);
  // G4double u2=cos(phi);
  // G4double v1=sin(theta);
  // G4double v2=cos(theta);
  // G4ThreeVector direction = G4ThreeVector(v1*u2,v1*u1,v2);
  // direction.rotateY(-M_PI/2);
  
  // gun->SetParticleEnergy(E); // Energy in keV
  // gun->SetParticleMomentumDirection(direction);
  // gun->SetParticlePosition(position);
  // gun->GeneratePrimaryVertex(anEvent);
  
 }
*/

///////////////////////////////////////////////////////
///// ********  General Particle Source  ******** /////
///////////////////////////////////////////////////////

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}

/////////////////////////////////////////////
///// ******* Auxiliar Function ******* /////
/////////////////////////////////////////////

////***** Efficiency correction: NaI(2x2) detector *****////

void PrimaryGeneratorAction::Calculate_Theta_min(double x)
{
  // x is equal to x-position of source
  G4double Crystal_Dmax = 4.65*cm; //4.65 cm --> Ge10

  Theta_min=(M_PI-atan((Crystal_Dmax/2.)/x))*rad;
}

void PrimaryGeneratorAction::ConstructAF(G4double e_gamma)
{
  G4double delta_theta = (M_PI-Theta_min/rad)/N_steps; // (M_PI-Theta_min))/N_steps
  G4double sum;
  G4double theta =0.;
  
  sum=0.;
  for (G4int i=1; i < N_steps+1; ++i)
    {
      theta = Theta_min+(i-1)*delta_theta;
      sum += Calculate_KN(theta,e_gamma);
      KN_AF[i] = sum;
    }

  // It is necessary normalize
 for(G4int i=0;i<N_steps+1;i++)
   {
     KN_AF[i]=KN_AF[i]/KN_AF[N_steps];
     theta = Theta_min+(i-1)*delta_theta;
     //G4cout<<theta/deg<<"\t"<<GetEnergy(theta,511*keV)/keV<<"\t"<<KN_AF[i]<<G4endl;
   }
}

G4double PrimaryGeneratorAction::Calculate_KN(G4double theta,G4double e_gamma)
{
  /// Diferential cross section --> Klein-Nishina
  G4double epsilon = (e_gamma/keV)/511.; /// epsilon = E_gamma/511 keV, for E_gamma = 511 keV -> epsilon = 1
  G4double a  = epsilon*(1-cos(theta));
  G4double b  = 1 + cos(theta)*cos(theta);
  G4double c  = 1 + a;
  G4double P1 = (1./pow(c,2))*(b+(a*a/c));
  /// Cross section --> Klein-Nishina (integrated)
  G4double u  = 2-cos(theta);
  G4double P2 = u-3*log(u)-((6*u+1)/(2*u*u))+5./2.;
  return P1;
}

G4double PrimaryGeneratorAction::GetTheta()
{
  G4double delta_theta = (M_PI-Theta_min/rad)/N_steps;
  G4double Theta=0.;
  G4double r =0.;

  r=G4UniformRand();
  for (G4int i=1; i < N_steps+1; ++i)
    {
      if(r>KN_AF[i-1] && r<=KN_AF[i])
	{
	  Theta=Theta_min+(i-1)*delta_theta;
	  break;
	}
    }
  return Theta/rad;
}

G4double PrimaryGeneratorAction::GetEnergy(G4double theta,G4double e_gamma)
{
  G4double epsilon = (e_gamma/keV)/511.; /// epsilon = E_gamma/511 keV, for E_gamma = 511 keV -> epsilon = 1
  Energy = (e_gamma/keV)/(1+epsilon*(1-cos(theta)));
  return Energy*keV;
}
