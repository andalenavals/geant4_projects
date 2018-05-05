#ifndef Interactions_h
#define Interactions_h 1

class Interactions {
public:
  Interactions();
  ~Interactions();
public:
  void AddComptonInteractionPerVolume(G4int pos, G4int vol);
  void globalTimePerVolume(G4int pos, G4double gtime);
  void localTimePerVolume(G4int pos, G4double ltime);
  void Analysis(void);
  void Clear(void);
  void Print(void);
  //! Increase the Compton interactions counter
  inline void AddComptonInteraction(void){Num_Compton_Int++;};
  inline G4int GetComptonInteraction(void){return Num_Compton_Int;};
  //! Assign the forced ID for gamma of 1275, 511 and 511 keV 
  inline void SetGammaID(G4int gamma_id){gamma_ID=gamma_id;};
  inline void SetGammaID_real(G4int gamma_id){gamma_ID_real=gamma_id;};
  inline G4int GetGammaID(void){return gamma_ID;};
  inline G4int GetGammaID_real(void){return gamma_ID_real;};
  //! Volumes counter
  inline void AddVolumeCounter(void){vol_counter++;};
  inline G4int GetVolumeCounter(void){return vol_counter;};
  inline G4int* GetVolume(void){return Volume;};
  inline G4double* GetglobalTime(void){return globalTime;};
  inline G4double* GetlocalTime(void){return localTime;};
  //! Good event
  inline void Set_good_event_flag(G4bool flag){good_event_flag=flag;};
  inline G4bool Get_good_event_flag(void){return good_event_flag;};
public:
  G4int NumberOfVolumes;
  G4int vol_counter;
private:
  //! Volume when the Compton interaction occurs, each volume has o unique ID
  G4int* Volume; 
  G4double* globalTime; 
  G4double* localTime; 

  //! Number of Compton interactions in Soil or/and object, or plates. 1275, 511 and 511 keV
  G4int Num_Compton_Int; 
  //! Forced ID for gamma of 1275, 511 and 511 keV 
  G4int gamma_ID;
  G4int gamma_ID_real;
  G4bool good_event_flag;
};
#endif
