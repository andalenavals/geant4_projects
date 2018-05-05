#include "MyAnalysisManager.hh"

MyAnalysisManager* MyAnalysisManager::instance = 0;

MyAnalysisManager::MyAnalysisManager(void)
{
  paso=0;
  DefinirCanvas(can);
  can->cd();
}

MyAnalysisManager::~MyAnalysisManager(void)
{
  can->Destructor();
}

Color_t MyAnalysisManager::Color(int i)
{
  colores[0] = kBlack;
  colores[1] = kRed;
  colores[2] = kGreen;
  colores[3] = kMagenta+2;
  colores[4] = kYellow;
  colores[5] = kMagenta;
  colores[6] = kBlue-2;
  colores[7] = kRed-2;
  colores[8] = kGreen-2;
  colores[9] = kCyan-2;
  colores[10] = kYellow-2;
  colores[11] = kMagenta-2;
  colores[12] = kBlue+2;
  colores[13] = kRed+2;
  colores[14] = kGreen+2;
  colores[15] = kCyan+2;
  colores[16] = kYellow+2;
  return colores[i];
}

void MyAnalysisManager::CreateCSV(const char* name)
{
  ofstream * archivo = new ofstream;
  archivo->open(name);
  archivos.push_back(archivo);
}

void MyAnalysisManager::CreateTGraph(int n , Color_t col)
{
  TGraph *g = new TGraph(n);
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.1);
  g->SetMarkerColor(col);
  g->SetLineColor(col);
  g->SetLineWidth(1);
  
  tgraphs.push_back(g);
}


void MyAnalysisManager::CreateTH1(const char* name, int nbin, double xlow, double xup )
{
  TH1D *h= new TH1D (name , "", nbin , xlow , xup);
  histos.push_back(h);
}

void MyAnalysisManager::DefinirCanvas(TCanvas * c1)
{
  c1->SetBottomMargin( 0.15 );
  c1->SetTopMargin( 0.15 );
  c1->SetLeftMargin( 0.15 );
  c1->SetRightMargin( 0.15 );
}


void MyAnalysisManager::Destroy() 
{
  can->Destructor();
  if (instance != 0) 
    {
      delete instance;
      instance = 0;
    }
}


void MyAnalysisManager::DrawAnimation(int i, int pasos, double x0, double y0,  double x1, double y1 )
{
  paso+=1;
  if(paso%pasos==0){
  frame = gPad->DrawFrame(x0, y0, x1, y1 );
  gPad->Modified();
  histos[i]->Draw("SAME");
  //cout<<"Problema antes"<<endl;
  can->Print("out/animation.gif+10");
  //cout<<"Problema despues"<<endl;
  }
  
}

void MyAnalysisManager::FillTH1(int i, double val)
{
  histos[i]->Fill(val); 
}

MyAnalysisManager * MyAnalysisManager::Instance() 	
{
   if (instance == 0) 
     {
       instance = new MyAnalysisManager();
     }
   
   return instance;
 }

TH1D * MyAnalysisManager::GetTH1(int i)
{
  return histos[i];
}

TGraph * MyAnalysisManager::GetTGraph(int i)
{
  return tgraphs[i];
} 

void MyAnalysisManager::Print()
{
  can->Print("out/Energia.eps") ;
}



double MyAnalysisManager::RandomFWHM(double media , double a , double b, double c )
{
  double r1;
  r1= ran->Gaus(media, FWHM(media, a , b , c)/2.35);
  return r1;
}

void MyAnalysisManager::SetBinContentTH1(int i, double x, double y)
{
  histos[i]->SetBinContent(histos[i]->FindBin(x), y);
}

void MyAnalysisManager::SetFrame(double x0, double y0,  double x1, double y1)
{
  gPad->SetLogx();
  gPad->SetLogy();
  frame = gPad->DrawFrame(x0, y0, x1, y1 );
  DefinirTitulos(frame, "", "Energy [MeV]", "Cross Section [mbarn]");
}



void MyAnalysisManager::SetPointTGraph(int t , int point , double x, double y )
{
  tgraphs[t]->SetPoint(point, x, y);
}



