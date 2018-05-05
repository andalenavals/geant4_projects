#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

double FWHM(G4double E, double a, double b, double c){
  return a*sqrt(E)+b*E+c;
}


B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{
  paso=0;
  archivo.open("Energias_Events.dat");
  c1->cd();
  hist->GetYaxis()->SetRangeUser(0, 700); //Solo para animacion
}

B1EventAction::~B1EventAction()
{
  archivo.close();
  /*
  hist->Draw();
  c1->Print("Energy_Events.eps");
  c1->Destructor();
  */
}

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in B1Run
  fRunAction->AddEdep(fEdep);

  //Salida de Datos
  if(fEdep!=0){
    //hist->Fill(fEdep*1000);

    //FWHM
    r1=ran->Gaus(fEdep*1000 , FWHM(fEdep*1000, 1.2 , 0.027 , 1)/2.35 ); //En keV
    hist->Fill(r1);

    
    //ANIMATION
    if(paso%100==0){
      hist->Draw();
      c1->Print("animation.gif+10");
    }
    
    paso+=1;

  }
  archivo<<fEdep<<endl;
  
}



