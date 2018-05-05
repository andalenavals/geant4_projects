#include "CrossSection.hh"

CrossSection* CrossSection::instance = 0;

CrossSection::CrossSection(void)
{
 
}

CrossSection::~CrossSection(void)
{

}

void CrossSection::Initialise( )
{
  phot->Initialise(particle, cuts);
  comp->Initialise(particle, cuts);
  conv->Initialise(particle, cuts);

}

G4double  CrossSection::ComputeTotalCrossSectionPerAtom(G4double  E,  G4double Z)
{
  G4double CrossPhoto, CrossCompt, CrossConv, St;
  CrossPhoto = phot->ComputeCrossSectionPerAtom(particle, E, Z);
  CrossCompt = comp->ComputeCrossSectionPerAtom(particle, E, Z);
  CrossConv = conv->ComputeCrossSectionPerAtom(particle, E, Z);
  St =  CrossPhoto + CrossCompt + CrossConv;
  return St;
}

G4double  CrossSection::ComputeMassAttenuationCoefficient(G4double  E,  G4Material * Material)
{
  G4double CrossPhoto, CrossCompt, CrossConv, CrossTotal, mu, wi, sum =0;
  for (int i = 0; i < Material->GetNumberOfElements(); i++) {
    wi = Material->GetFractionVector()[i] ;
    CrossPhoto = phot->ComputeCrossSectionPerAtom(particle, Material->GetElement(i), E);
    CrossCompt = comp->ComputeCrossSectionPerAtom(particle, Material->GetElement(i), E);
    CrossConv = conv->ComputeCrossSectionPerAtom(particle, Material->GetElement(i) , E);
    CrossTotal = CrossPhoto + CrossCompt + CrossConv;
    sum += wi*CrossTotal/( Material->GetElement(i)->GetA() );
  }
  mu = Avogadro * sum;
  return mu;
  
}

G4double  CrossSection::ComputeMeanFreePath(G4double  E,  G4Material* Ma)
{
  G4double density, mfp;
  density= Ma->GetDensity();
  mfp = pow(density*ComputeMassAttenuationCoefficient(E,Ma),-1);
  return mfp;
}

CrossSection * CrossSection::Instance() 	
{
   if (instance == 0) 
     {
       instance = new CrossSection();
     }
   
   return instance;
 }
