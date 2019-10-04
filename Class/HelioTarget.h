#ifndef HELIOSTARGET
#define HELIOSTARGET

#include "../Class/GenericPlane.h"

class HeliosTarget : public GenericPlane{
  RQ_OBJECT("HelioTarget");
public:

  HeliosTarget();
  ~HeliosTarget();
  
  void SetXYHistogram(int xMin, int xMax, int yMin, int yMax);
  void SetCanvasDivision();
  
  void Fill(vector<UInt_t> energy);
  void FillHit(int * hit){ for( int i = 0; i < 8; i++){ hHit->Fill(i+1, hit[i]);} }
  
  void Draw();

  void ClearHistograms();

private:
  
  TH1F * hX;
  TH1F * hY;
  TH2F * hXY;
  
  TH1F * hHit;
  
  TH1F * hX1, * hX2, * hY1, * hY2;
  
  int chX1, chX2; // yellow, Red
  int chY1, chY2; // Blue, White
  
};

HeliosTarget::HeliosTarget(){
  
  hX = NULL;
  hY = NULL;
  hXY = NULL;
  
  hHit = NULL;
  
  hX1 = NULL;
  hX2 = NULL;
  hY1 = NULL;
  hY2 = NULL;
  
  chX1 = 0;
  chX2 = 4;
  
  chY1 = 1;
  chY2 = 3;
  
}

HeliosTarget::~HeliosTarget(){
  
  delete hX;
  delete hY;
  delete hXY;
  
  delete hHit;
  
  delete hX1;
  delete hX2;
  delete hY1;
  delete hY2;
  
}

void HeliosTarget::SetXYHistogram(int xMin, int xMax, int yMin, int yMax){
  
  int bin = 200;
  float labelSize = 0.08;
  
  hX = new TH1F("hX", "X; X[ch]; count", bin, xMin, xMax);
  hY = new TH1F("hY", "Y; Y[ch]; count", bin, yMin, yMax);
  
  hXY = new TH2F("hXY", "X-Y; X[ch]; Y[ch]", bin, xMin, xMax, bin, yMin, yMax);
  
  hX->GetXaxis()->SetLabelSize(labelSize);
  hX->GetYaxis()->SetLabelSize(labelSize);
  
  hY->GetXaxis()->SetLabelSize(labelSize);
  hY->GetYaxis()->SetLabelSize(labelSize);
  
  hXY->GetXaxis()->SetLabelSize(labelSize);
  hXY->GetYaxis()->SetLabelSize(labelSize);
  
  hXY->SetMinimum(1);
  
  hHit = new TH1F("hHit", "number of hit", 8, -0.5, 7.5);
  
  hX1 = new TH1F("hX1", Form("X1 (ch=%d)", chX1), bin, 0, 16000);
  hX2 = new TH1F("hX2", Form("X2 (ch=%d)", chX2), bin, 0, 16000);
  hY1 = new TH1F("hY1", Form("Y1 (ch=%d)", chY1), bin, 0, 16000);
  hY2 = new TH1F("hY2", Form("Y2 (ch=%d)", chY2), bin, 0, 16000);
  
  isHistogramSet = true;
  
  printf(" Histogram setted. \n");
  
}

void HeliosTarget::SetCanvasDivision(){
 
  //GenericPlane::SetCanvasDivision();
  
  fCanvas->Divide(1,2);
  fCanvas->cd(1)->Divide(2,1); 
  fCanvas->cd(1)->cd(1)->SetLogz();
  fCanvas->cd(1)->cd(2)->Divide(2,2);
  
  
  fCanvas->cd(2)->Divide(2,1); 
  fCanvas->cd(2)->cd(1)->Divide(2,2);
  fCanvas->cd(2)->cd(2)->Divide(2,2);
  
  /*
  fCanvas->cd(2)->SetGridy();
  fCanvas->cd(2)->SetTicky();
  fCanvas->cd(2)->SetTickx();
  
  fCanvas->cd(1)->cd(2)->cd(3)->SetGridy();
  fCanvas->cd(1)->cd(2)->cd(3)->SetTicky();
  fCanvas->cd(1)->cd(2)->cd(3)->SetTickx(); 
  fCanvas->cd(1)->cd(2)->cd(4)->SetLogy(); 
  */
}

void HeliosTarget::Draw(){
  
  if ( !isHistogramSet ) return;
  //GenericPlane::Draw();
  
  fCanvas->cd(1)->cd(1); hdEtotE->Draw("colz");
  //fCanvas->cd(1)->cd(1); hdEE->Draw("colz");

  if( numCut > 0 ){
    for( int i = 0; i < numCut; i++){
      cutG = (TCutG *) cutList->At(i);
      cutG->Draw("same");
    }
  }

  fCanvas->cd(1)->cd(2)->cd(1); hE->Draw();
  fCanvas->cd(1)->cd(2)->cd(2); hdE->Draw();
  fCanvas->cd(1)->cd(2)->cd(4); hTDiff->Draw(); line->Draw();
  fCanvas->cd(1)->cd(2)->cd(3); rateGraph->Draw("AP"); legend->Draw();
  
  fCanvas->cd(2)->cd(1)->cd(1); hX1->Draw("");
  fCanvas->cd(2)->cd(1)->cd(2); hX2->Draw("");
  fCanvas->cd(2)->cd(1)->cd(3); hY1->Draw("");
  fCanvas->cd(2)->cd(1)->cd(4); hY2->Draw("");
  
  fCanvas->cd(2)->cd(2)->cd(1); hHit->Draw("HIST");
  fCanvas->cd(2)->cd(2)->cd(2); hX->Draw("");
  fCanvas->cd(2)->cd(2)->cd(3); hY->Draw("");
  fCanvas->cd(2)->cd(2)->cd(4); hXY->Draw("colz");
  
  
  fCanvas->Modified();
  fCanvas->Update();
  gSystem->ProcessEvents();
  
}


void HeliosTarget::Fill(vector<UInt_t> energy){
  
  //GenericPlane::Fill(energy);
  
  if ( !isHistogramSet ) return;
  
  //check non-zero
  int count = 0;
  for( int k = 0; k < energy.size() ; k++){
    if( energy[k] != 0 ) count++;
  }
  
  if( count != 5 ) return;
  
  int E = energy[chE] + gRandom->Gaus(0, 500);
  int dE = energy[chY1] + energy[chY2] + gRandom->Gaus(0, 500);
  
  float X = ((float)energy[chX1] - (float)energy[chX2])*1.0/(energy[chX1] + energy[chX2]);
  float Y = ((float)energy[chY1] - (float)energy[chY2])*1.0/(energy[chY1] + energy[chY2]);

  hX1->Fill(energy[chX1]);
  hX2->Fill(energy[chX2]);
  hY1->Fill(energy[chY1]);
  hY2->Fill(energy[chY2]);
    
  hX->Fill(X);
  hY->Fill(Y);
  hXY->Fill(X, Y);
  
  hE->Fill(E);
  hdE->Fill(dE);
  hdEE->Fill(E, dE);
  float totalE = dE * chdEGain + E * chEGain;
  hdEtotE->Fill(totalE, dE);
  
  if( numCut > 0  ){
    for( int i = 0; i < numCut; i++){
      cutG = (TCutG *) cutList->At(i);
      if( cutG->IsInside(totalE, dE)){
        countOfCut[i] += 1;
      }
    }
  }
  
}

void HeliosTarget::ClearHistograms(){
  
  GenericPlane::ClearHistograms();
  
  hX1->Reset();
  hX2->Reset();
  hY1->Reset();
  hY2->Reset();
  
  hX->Reset();
  hY->Reset();
  hXY->Reset();
  
  hHit->Reset();
  
}

#endif
