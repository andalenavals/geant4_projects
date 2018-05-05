#include "RootImprove.hh"

using namespace std;

/** Un constructor que no construye */
RootImprove::RootImprove(void)
{
  
}

/** Un destructor que no construye */
RootImprove::~RootImprove(void)
{
  
}

/** Retorna mis colores favoritos */
Color_t RootImprove::Color(int i)
{
  colores[0] = kBlue;
  colores[1] = kRed;
  colores[2] = kGreen+2;
  colores[3] = kBlack;
  colores[4] = kYellow+1;
  colores[5] = kMagenta-2;
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
  colores[17] = kMagenta;
  return colores[i];
}


/** Este metodo define los margenes del Canvas para que no se
    salgan los titulos o ejes de la región de vision */
void RootImprove::DefinirCanvas(TCanvas * c1)
{
  c1->SetBottomMargin( 0.13 );
  c1->SetTopMargin( 0.05 );
  c1->SetLeftMargin( 0.13 );
  c1->SetRightMargin( 0.05 );
}


/** Se define el estilo de las graficas, numero de digitos de los
    ejes, numero de divisiones , si se desea mostrar la fecha el
    directorio o la informacion de ajuste en el canvas, si deseamos
    las graficas en logaritmico */
void RootImprove::DefinirEstilo(TStyle * S)
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
  S->SetNdivisions(500 +10, "x"); //n = primarias + 100*secundarias + 10000*terciarias
  S->SetNdivisions(500 +10, "y");
  S->SetNdivisions(500 +5, "z");
  S->SetLabelSize(0.04 , "x"); //Tamaño de los numeros de los ejes
  S->SetLabelSize(0.04 , "y"); 
  S->SetLabelSize(0.04 , "z");
  //S->SetLabelOffset(0.05 , "x"); //Correr los numeros de los ejes
  //S->SetLabelOffset(0.05 , "y");
  //S->SetLabelOffset(0.05 , "z"); 
  // S->SetOptLogy();
  //S->SetOptLogx();
}

/** */
void RootImprove::DefinirEjes(TGraph * h, double xlow, double xup)
{
  h->GetXaxis()->SetLimits(xlow , xup);
}

void RootImprove::DefinirEjes(TGraph * h, double xlow, double xup , double ylow, double yup  )
{
  h->GetYaxis()->SetRangeUser(ylow , yup);
  h->GetXaxis()->SetLimits(xlow , xup);
}

void RootImprove::DefinirEjes(TGraphErrors * h, double xlow, double xup)
{
  h->GetXaxis()->SetLimits(xlow , xup);
}

void RootImprove::DefinirEjes(TGraphErrors * h, double xlow, double xup , double ylow, double yup  )
{
  h->GetYaxis()->SetRangeUser(ylow , yup);
  h->GetXaxis()->SetLimits(xlow , xup);
}

void RootImprove::DefinirEjes(TH1 * h, double xlow, double xup)
{
  h->GetXaxis()->SetRangeUser(xlow , xup);
  
}

void RootImprove::DefinirEjes(TH1 * h, double xlow, double xup , double ylow, double yup  )
{
  h->GetYaxis()->SetRangeUser(ylow , yup);
  h->GetXaxis()->SetRangeUser(xlow , xup);
}

void RootImprove::DefinirEjes(TF1 * h, double xlow, double xup)
{
  h->GetXaxis()->SetLimits(xlow , xup);
  
}

void RootImprove::DefinirEjes(TF1 * h, double xlow, double xup , double ylow, double yup  )
{
  h->GetYaxis()->SetRangeUser(ylow , yup);
  h->GetXaxis()->SetRangeUser(xlow , xup);
}

void RootImprove::DefinirPalette(TStyle * S)
{
  const Int_t NRGBs = 5;  
  const Int_t NCont = 300; 


  //BANDERA COLOMBIANA
  /*
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  */
  
  //ROJO AZUL NEGRO
  /*
  Double_t stops[NRGBs] = { 0.00, 0.25, 0.5, 0.75, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.0, 0.75, 1.00 };
  Double_t green[NRGBs] = { 0.00, 0.00, 0.0, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.00, 0.2, 1.0, 0.00, 0.00 };
  */
  
  //ROJO AZUL BLANCO
  /*
  Double_t stops[NRGBs] = { 0.00, 0.25, 0.5, 0.75, 1.00 };
  Double_t red[NRGBs]   = { 1.00, 0.6, 0.0, 0.75, 1.00 };
  Double_t green[NRGBs] = { 1.00, 0.6, 0.0, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 1.00, 0.7, 1.0, 0.00, 0.00 };
  */

  //ROJO AZUL BLANCO GRADIENTE
  Double_t stops[NRGBs] = { 0.00, 0.25, 0.5, 0.75, 1.00 };
  Double_t red[NRGBs]   = { 1.00, 0.6, 0.3, 0.20, 0.10 };
  Double_t green[NRGBs] = { 1.00, 0.8, 0.5, 0.40, 0.30 };
  Double_t blue[NRGBs]  = { 1.00, 0.8, 0.6, 0.50, 0.4 };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  S->SetNumberContours(NCont);
}

void RootImprove::DefinirLegend (TLegend * leg , int l,  Color_t col , double s)
{
  leg->SetBorderSize(l);
  leg->SetFillColor(col);
  leg->SetTextSize(s);
  
}

/** Se definen basicamente el estilo de la funcion
\param[in] W Funcion que se quiere peluquiar
\param[in] st Tipo de rayita que quiero, debe entrar un entero, la continua normalita es 1
\param[in] wd Que tan ancha es la rayita, un float.
\param[in] co1 El color que quiero, sino quiere usar , recuerde que kBlue, kRed. Sin comillas.
*/
void RootImprove::DefinirTF1 (TF1 * W, Style_t st, Size_t wd, Color_t co1)
{
  W->SetLineStyle(st);
  W->SetLineWidth(wd); 
  W->SetLineColor(co1);
}

void RootImprove::DefinirTGraph (TGraph * W, Style_t st, Size_t si,  Size_t wd,  Color_t co1, Color_t co2)
{
  W->SetMarkerStyle(st);
  W->SetMarkerSize(si);
  W->SetMarkerColor(co1);
  W->SetLineColor(co2);
  W->SetLineWidth(wd); 
}

void RootImprove::DefinirTGraphE (TGraphErrors * W, Style_t st, Size_t si,  Size_t wd,  Color_t co1, Color_t co2)
{
  W->SetMarkerStyle(st);
  W->SetMarkerSize(si);
  W->SetMarkerColor(co1);
  W->SetLineColor(co2);
  W->SetLineWidth(wd); 
}

void RootImprove::DefinirTH1 (TH1 * W, Style_t st, Size_t wd, Color_t co1)
{
  W->SetLineStyle(st);
  W->SetLineWidth(wd); 
  W->SetLineColor(co1);  
}

void RootImprove::DibujarAjuste(double x0, double y0, const char* text)
{ 
  TLatex latex;
  latex.SetTextSize(0.04);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatex(x0, y0 , text);
}

void RootImprove::Dibujar(TH2 * h, const char * modo, TStyle * S , Int_t color  )
{
  S->SetPalette( color );
  h->Draw(modo);
}

void RootImprove::Dibujar(TH2 * h, const char * modo, TStyle * S , Int_t color, double xlow, double xup, double ylow, double yup )
{
  S->SetPalette( color );
  h->GetYaxis()->SetRangeUser(ylow , yup);
  h->GetXaxis()->SetRangeUser(xlow , xup);
  h->Draw(modo);
}

int RootImprove::GenerarSemilla(void)
{
  time_t t;
  struct tm birthday;
  birthday.tm_hour = 16;
  birthday.tm_min  = 0;
  birthday.tm_sec  = 0;
  birthday.tm_year = 93;
  birthday.tm_mon  = 12;
  birthday.tm_mday = 22;

  time(&t);
  int sec =  difftime( t, mktime( &birthday ) );
  return sec;
}

int RootImprove::GetMaximumBin(TH1 * h, double xlow, double xup)
{
  //Generalizar cuando haya mas de un maximo
  /*
  h->GetXaxis()->SetRangeUser(xlow , xup);
  return   h->GetMaximumBin() ;
  */
  
  int binmax;
  double content=0;
  for (int i = h->FindBin(xlow); i <= h->FindBin(xup) ; i++)
    {
      if(h->GetBinContent(i)> content )
	{
	content = h->GetBinContent(i);
	binmax = i;
	}
    }
  return binmax;
}

double RootImprove::GetMaximumColumna(const char* argv, const char* col, const char* format )
{
  TTree *temp = GuardarEnTTree(argv, "temp" , "temp" , format);
  // temp->Print();
  return temp->GetMaximum(col);
}

double RootImprove::GetMinimumColumna(const char* argv, const char* col, const char* format )
{
  TTree * temp = GuardarEnTTree(argv, "temp" , "temp" , format);
  // temp->Print();
  return temp->GetMinimum(col);
}

int RootImprove::GetNumberFilas(const char* argv,const char* col, const char* format)
{
  int entries;
  TTree * temp = GuardarEnTTree(argv, "temp" , "temp" , format);
  // temp->Print();
  entries= (int) temp->GetEntries();
  return entries;
 
}

TH1D * RootImprove::GuardarEnTH1(const char* argv, const char* name)
{
  return GuardarEnTH1(argv, name, GetNumberFilas(argv, "x", "x:y"), GetMinimumColumna(argv, "x", "x:y"), GetMaximumColumna(argv, "x", "x:y"));
}

TH1D * RootImprove::GuardarEnTH1(const char* argv, const char* name , int nbin, double xlow, double xup)
{
  TH1D *h= new TH1D (name , "", nbin , xlow , xup);
  ifstream file;
  double column1, column2;  
  file.open (  argv , std::ios::in );
  while( file >> column1 >> column2){
    h->SetBinContent( h->FindBin(column1) ,column2);
    //Si column1 cae en borde se mete a bin de la derecha.
  } 
  return h;
}

TH2D * RootImprove::GuardarEnTH2(const char* argv  , const char* name, int nbinx , double xlow , double xup, int nbiny, double ylow, double yup)
{
  TH2D *h= new TH2D (name , "", nbinx , xlow , xup, nbiny, ylow, yup);
  ifstream file;
  double value;  
  file.open (  argv , std::ios::in );
  for (int j = 1; j <= nbiny; ++j)
    for (int i = 1; i <= nbinx; ++i)
      {
	file >> value;
	h->SetBinContent(i, j, value);

      }
  return h;
}

TTree * RootImprove::GuardarEnTTree(const char* argv, const char* name, const char* title, const char* format)
{
  TTree *t = new TTree(name, title);
  t->ReadFile(argv,format);
  return t;
}

void RootImprove::ImprimirEnArchivoTexto(TH1 * h, const char * name)
{
  ofstream file;
  file.open( name );
  for(int i=1; i<= h->GetNbinsX() ;i++){
    file << i <<" " << h->GetBinContent(i) <<endl;
  }
  file.close();
     
}

void RootImprove::ImprimirAllPalette( TH2D *h2,  TCanvas * c2, TString modo, TStyle * S )
{
  TString name; 
  for (int i = 1; i <= 112; ++i)
    {
      name= "outpaletas/";
      name+=i;
      name.Append(".jpg");
      Dibujar(h2,modo, S ,  i); //BOX, COL2Z, CONT0, CONT4
      c2->Print(name);
      name.Clear();
    }     
}

double RootImprove::Integrar(TH1 * h)
{
  //return h->GetEffectiveEntries();
  double suma=0;
  for (int i = 1; i <= h->GetNbinsX() ; i++)
    suma+=h->GetBinContent(i);
  return suma;
   
}

double RootImprove::Integrar(TH1 * h, double xlow, double xup)
{
  //return h->GetEffectiveEntries();
  double suma=0;
  for (int i = h->FindBin(xlow); i <= h->FindBin(xup) ; i++)
    suma+=h->GetBinContent(i);
  return suma;
   
}

Style_t RootImprove::MarkerStyle(int i)
{
  markerstyle[0]=20;
  markerstyle[1]=21;
  markerstyle[2]=22;
  markerstyle[3]=23;
  markerstyle[4]=24;
  markerstyle[5]=25;
  markerstyle[6]=26;
  markerstyle[7]=27;
  markerstyle[8]=28;
  markerstyle[9]=29;
  markerstyle[10]=30;
  markerstyle[11]=31;
  markerstyle[12]=32;
  markerstyle[13]=33;
  markerstyle[14]=34;
  return markerstyle[i];
}

void RootImprove::SuperponerHistogramas(TH1 * h1 , TH1 * h2 )
{
  
  if(h1->GetMaximum() > h2->GetMaximum())
    {
      h1->SetLineColor(1);
      h2->SetLineColor(2);
      h1->Draw("");
      h2->Draw("same");
    }
  else
    {
      h1->SetLineColor(2);
      h2->SetLineColor(1);
      h2->Draw("");
      h1->Draw("same");
    } 
}

void RootImprove::SuperponerHistogramas(TH1 * h1, TH1 * h2, double xlow, double xup )
{
  
  if(h1->GetMaximum() > h2->GetMaximum())
    {
      h1->GetXaxis()->SetRangeUser(xlow , xup);
      h1->SetLineColor(1);
      h2->SetLineColor(2);
      h1->Draw("");
      h2->Draw("same");
    }
  else
    {
      h2->GetXaxis()->SetRangeUser(xlow , xup);
      h1->SetLineColor(2);
      h2->SetLineColor(1);
      h2->Draw("");
      h1->Draw("same");
    } 
}

TH1D * RootImprove::ToD( TH1 * h)
{
  TH1D *hd= new TH1D ( "", ""  , h->GetNbinsX() , GetXLow(h) , GetXUp(h) );
  for(int i=1; i<= h->GetNbinsX(); i++)
    {
      hd->SetBinContent( i ,h->GetBinContent(i) );
    }
  return hd;
}



