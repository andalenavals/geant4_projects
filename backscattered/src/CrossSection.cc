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

G4double  CrossSection::ComputeTotalMeanFreePath(G4double  E,  G4Material* Ma)
{
  G4double MPhoto, MCompt, MConv, Mt;
  MPhoto = phot->ComputeMeanFreePath(particle, E, Ma);
  MCompt = comp->ComputeMeanFreePath(particle, E, Ma);
  MConv = conv->ComputeMeanFreePath(particle, E, Ma);
  Mt =  MPhoto + MCompt + MConv;
  return Mt;
}

CrossSection * CrossSection::Instance() 	
{
   if (instance == 0) 
     {
       instance = new CrossSection();
     }
   
   return instance;
 }
