#include "WriteToEntuple.hh"
writeToNtuple::writeToNtuple(){
  nt = new TFile("myNtuple2.root","RECREATE");
  t1 = new TTree("t1","a ROOT tree for detector 1");
  t2 = new TTree("t2","a ROOT tree for detector 2");
  t3 = new TTree("t3","a ROOT tree for detector 3");
  t4 = new TTree("t4","a ROOT tree for detector 4");
  t5 = new TTree("t5","a ROOT tree for detector 5");
  t6 = new TTree("t6","a ROOT tree for detector 6");
  t7 = new TTree("t7","a ROOT tree for detector 7");
  t8 = new TTree("t8","a ROOT tree for detector 8");
  //April 3-2009. Trees for fast neutrons and gammas
  t_fast  = new TTree("t_fast","a ROOT tree for detector 8, fast neutrons");
  t_gammas = new TTree("t_gammas","a ROOT tree for detector 8, gammas");
  t_thermal = new TTree("t_thermal","a ROOT tree for detector 8, thermal neutrons");
  
  //April 22-2009 new histograms.
  total=new TH1F("total", "total", 200, 0, 2.0);
  fast=new TH1F("fast", "fast", 200, 0, 2.0);
  thermal=new TH1F("thermal", "thermal", 200, 0, 2.0);
  gammas=new TH1F("gammas", "gammas", 200, 0, 2.0);
  
  //Dec. 03-2009
  //Up to 8 MeV
  //KinE_DLM2=new TH1F("KinE", "KinE", 200, 0, 8.0);

  //Up to 40keV
  //KinE_DLM2=new TH1F("KinE", "KinE", 200, 0, 40);
  
  //40000 channels....200 eV
  //KinE_DLM2_before=new TH1F("KinE_before", "KinE_before", 40000, 0, 8.0);  
  //KinE_DLM2_after=new TH1F("KinE_after", "KinE_after", 40000, 0, 8.0);
  
  //****Thu Apr 22 20:30:32 COT 2010
  //With the new logaritmic histogram
  //in order to  get a histogram from 1e-8 to 1

  //****Sat May  1 12:04:11 COT 2010
  //I changed the minimum value to 1e-9...to see cold neutrons...jeje 
  KinE_DLM2_before=new TH1F("KinE_before", "KinE_before", 100, -9, 1);  
  BinLogX(KinE_DLM2_before);
  
  KinE_DLM2_after=new TH1F("KinE_after", "KinE_after", 100, -9, 1);  
  BinLogX(KinE_DLM2_after);
  
  
  //una rama para cada detector......las variables son independientes..............
  t1->Branch("Energia_Depositada_en_1", &Energia_Depositada_en_1, "Energia_Depositada_en_1");
  t2->Branch("Energia_Depositada_en_2", &Energia_Depositada_en_2, "Energia_Depositada_en_2");
  t3->Branch("Energia_Depositada_en_3", &Energia_Depositada_en_3, "Energia_Depositada_en_3");
  t4->Branch("Energia_Depositada_en_4", &Energia_Depositada_en_4, "Energia_Depositada_en_4");
  t5->Branch("Energia_Depositada_en_5", &Energia_Depositada_en_5, "Energia_Depositada_en_5");
  t6->Branch("Energia_Depositada_en_6", &Energia_Depositada_en_6, "Energia_Depositada_en_6");
  t7->Branch("Energia_Depositada_en_7", &Energia_Depositada_en_7, "Energia_Depositada_en_7");
  t8->Branch("Energia_Depositada_en_8", &Energia_Depositada_en_8, "Energia_Depositada_en_8");
  //April 3-2009. Trees for fast neutrons and gammsa
  t_fast->Branch("Energia_Depositada_en_8_fast", &Energia_Depositada_en_8_fast, "Energia_Depositada_en_8_fast");
  t_gammas->Branch("Energia_Depositada_en_8_gammas", &Energia_Depositada_en_8_gammas, "Energia_Depositada_en_8_gammas");
  t_thermal->Branch("Energia_Depositada_en_8_thermal", &Energia_Depositada_en_8_thermal, "Energia_Depositada_en_8_thermal");
}
//Thu Apr 22 20:27:03 COT 2010
//added to obtain a logaritmic histogram..http://root.cern.ch/root/roottalk/roottalk06/1213.html
void writeToNtuple::BinLogX(TH1*h)
{

  TAxis *axis = h->GetXaxis();
  int bins = axis->GetNbins();
  
  Axis_t from = axis->GetXmin();
  Axis_t to = axis->GetXmax();
  Axis_t width = (to - from) / bins;
  Axis_t *new_bins = new Axis_t[bins + 1];
  
  for (int i = 0; i <= bins; i++) {
    new_bins[i] = TMath::Power(10, from + i * width);
    
  }
  axis->Set(bins, new_bins);
  delete new_bins;
} 


//#################          December-2009 , new histograms      #################
void writeToNtuple::fillVarsKE_before(Float_t Energia_Depositada_toFill){
  KinE_DLM2_before->Fill(Energia_Depositada_toFill);
}
void writeToNtuple::fillVarsKE_after(Float_t Energia_Depositada_toFill){
  KinE_DLM2_after->Fill(Energia_Depositada_toFill);
}


//#################          April 22-2009 , new histograms      #################
void writeToNtuple::fill_total(Float_t Energia_Depositada_toFill){
  total->Fill(Energia_Depositada_toFill);
}
void writeToNtuple::fill_fast(Float_t Energia_Depositada_toFill){
  fast->Fill(Energia_Depositada_toFill);
}
void writeToNtuple::fill_thermal(Float_t Energia_Depositada_toFill){
  thermal->Fill(Energia_Depositada_toFill);
}
void writeToNtuple::fill_gammas(Float_t Energia_Depositada_toFill){
  gammas->Fill(Energia_Depositada_toFill);
}
//################################################################################

void writeToNtuple::fillVars8_thermal(Float_t Energia_Depositada_toFill){
  Energia_Depositada_en_8_thermal=Energia_Depositada_toFill;
  t_thermal->Fill();
}

//#####################################################################3
//April 3-2009. Trees for fast neutrons and gammas
void writeToNtuple::fillVars8_fast(Float_t Energia_Depositada_toFill){
  Energia_Depositada_en_8_fast=Energia_Depositada_toFill;
  t_fast->Fill();
}
void writeToNtuple::fillVars8_gammas(Float_t Energia_Depositada_toFill){
  Energia_Depositada_en_8_gammas=Energia_Depositada_toFill;
  t_gammas->Fill();
}
//#####################################################################3
void writeToNtuple::fillVars1(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_1 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_B =Energia_Depositada_toFill;*/
  t1->Fill();
}
void writeToNtuple::fillVars2(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_2 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_B =Energia_Depositada_toFill;*/
  t2->Fill();
}
void writeToNtuple::fillVars3(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_3 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_1 =Energia_Depositada_toFill;*/
  t3->Fill();
}
void writeToNtuple::fillVars4(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_4 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_2 =Energia_Depositada_toFill;*/
  t4->Fill();
}
void writeToNtuple::fillVars5(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_5 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_B =Energia_Depositada_toFill;*/
  t5->Fill();
}
void writeToNtuple::fillVars6(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_6 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_B =Energia_Depositada_toFill;*/
  t6->Fill();
}
void writeToNtuple::fillVars7(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_7 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_4 =Energia_Depositada_toFill;*/
  t7->Fill();
}
void writeToNtuple::fillVars8(Float_t Energia_Depositada_toFill)
{
  //if(id==0)
  Energia_Depositada_en_8 =Energia_Depositada_toFill;
  /*else
    Energia_Depositada_en_5 =Energia_Depositada_toFill;*/
  t8->Fill();
}

void writeToNtuple::closeNtuple()
{
  t1->Write();
  t2->Write();
  t3->Write();
  t4->Write();
  t5->Write();
  t6->Write();
  t7->Write();
  t8->Write();
  t_fast->Write();
  t_gammas->Write();
  t_thermal->Write();
  
  //April 22-2009... write new histograms
  total->Write();
  fast->Write();
  thermal->Write();
  gammas->Write();
  
  //December 10-2009
  KinE_DLM2_before->Write();
  KinE_DLM2_after->Write();
  
  nt->Close(); 
}
