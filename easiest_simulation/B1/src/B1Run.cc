
#include "B1Run.hh"

B1Run::B1Run()
: G4Run(),
  fEdep(0.), 
  fEdep2(0.)
{} 

B1Run::~B1Run()
{} 
 
void B1Run::Merge(const G4Run* run)
{
  const B1Run* localRun = static_cast<const B1Run*>(run);
  fEdep  += localRun->fEdep;
  fEdep2 += localRun->fEdep2;

  G4Run::Merge(run); 
} 

void B1Run::AddEdep (G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep; //error
}



