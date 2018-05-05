#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/**
Esta clase es llamada por el RUNMANAGER, asi que seguramente la
estructura de esta clase no se puede cambiar mucho. IMPORTANTE: esta
clase hereda los metodos de la clase G4VUserActionInitialization, en
particular SetUserAction, este metodo sin entrar en el detalle, es el
que lee el RunAction, el PrimaryGeneratorAction, el EventAction y el
SteppingAction. Para tener una idea de como funciona la cosa, el
Stepping action llama al EventAction, el EventAction llama al RunAction,
el RunAction llama al PrimaryGeneratorAction

En el fondo lo relevante del constructor de los objetos B1RunAction,
B1PrimaryGeneratorAction, B1EventAction y B1SteppingAction estan
siendo heredados, EL RunManager entendera lo que
G4VUserActionInitialization::SetUserAction hace con los objetos
construidos por

G4VUserPrimaryGeneratorAction() papa del constructor
de B1PrimaryGeneratorAction G4UserRunAction() papa del constructor de
B1RunAction G4UserEventAction() papa del constructor de B1EventAction
G4UserSteppingAction() papa del constructor de B1SteppingAction

 */

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};


#endif

    
