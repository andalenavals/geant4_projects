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
  colores[1] = kGreen;
  colores[2] = kRed;
  colores[3] = kMagenta+2;
  colores[4] = kBlue-1;
  colores[5] = kMagenta;
  colores[6] = kGreen+2;
  colores[7] = kRed-1;
  colores[8] = kYellow-2;
  colores[9] = kCyan-2;
  colores[10] = kGreen-2;
  colores[11] = kMagenta-2;
  colores[12] = kBlue+2;
  colores[13] = kRed+2;
  colores[14] = kGreen-1;
  colores[15] = kCyan+2;
  colores[16] = kRed-2;
  colores[17] = kYellow+2;
  colores[18] = kCyan-1;
  colores[19] = kGreen+1;
  colores[20] = kMagenta+1;
  colores[21] = kBlue+1;
  colores[22] = kYellow+2;
  colores[23] = kYellow+2;
  colores[24] = kYellow+2;
  colores[25] = kYellow+2;
  colores[26] = kYellow+2;
  colores[27] = kYellow+2;
  colores[28] = kYellow+2;
  colores[29] = kYellow+2;
  colores[30] = kYellow+2;
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
  g->SetMarkerSize(0.8);
  g->SetMarkerColor(col);
  g->SetLineColor(col);
  g->SetLineWidth(2);
  
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

void MyAnalysisManager::DefinirEstilo(TStyle * S)
{
  TGaxis::SetMaxDigits(5);
  S->SetOptStat(kFALSE); //NO imprimir la informacion estadistica 
  S->SetOptDate(0); //Fecha
  S->SetOptFile(0);
  // S->SetOptFit(11);  //Informacion de Fit
  // S->SetFitFormat("#11.2e");
  S->SetStatColor(0);
  S->SetStatX(0.82);
  S->SetStatY(0.31);
  S->SetStatBorderSize(0);
  S->SetCanvasColor(0); // Lienzo
  S->SetFrameFillColor(0); //Region entre ejes
  S->SetPadBorderMode(0);  
  S->SetTitleFillColor(0); //Color de relleno cajita titulo
  S->SetTitleBorderSize(0); // activar cajita titulo
  S->SetLegendBorderSize(0); //Sin borde el legend
  S->SetLegendFillColor(0); //Color del Legend
  S->SetNdivisions(500 +10, "x"); //n = primarias + 100*secundarias + 10000*terciarias
  S->SetNdivisions(500 +10, "y");
  S->SetNdivisions(500 +5, "z");
  S->SetLabelSize(0.03 , "x"); //Tamaño de los numeros de los ejes
  S->SetLabelSize(0.03 , "y"); 
  S->SetLabelSize(0.03 , "z");

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

void MyAnalysisManager::Print( const char* name )
{
  can->Print(name) ;
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
  //gPad->SetLogx();
  //gPad->SetLogy();
  frame = gPad->DrawFrame(x0, y0, x1, y1 );
  DefinirTitulos(frame, "", "Energy [keV]" , "R / I_{0}");
}



void MyAnalysisManager::SetPointTGraph(int t , int point , double x, double y )
{
  tgraphs[t]->SetPoint(point, x, y);
}



