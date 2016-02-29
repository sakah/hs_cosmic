#include "analysis.h"

#include "TF1.h"
#include "TPad.h"
#include "TPaveStats.h"

Analysis::Analysis()
{
}

void Analysis::OpenRootFile(const char* path)
{
   outroot_.OpenRootFile(path);

   // get basename of root file
   // ../../root_path/run16.root
   // -> name_ = run16
   const char* p1 = strrchr(path, '/');
   const char* p2 = strrchr(path, '.');
   if (p1==NULL) { p1 = (char*)path; }
   else { p1++; }
   p2--;
   int i=0;
   for (const char* p = p1; p<=p2; p++) {
      name_[i++] = *p;
   }
   name_[i] = '\0';
   //printf("name_ => '%s'\n", name_);
}

void Analysis::Loop(int max_events)
{
   BeginOfEvent();
   int num_ents = outroot_.GetEntries();
   if (max_events>num_ents) {
      max_events = num_ents;
   }
   num_events_ = 0;
   for (int iev=0; iev<max_events; iev++) {
      outroot_.GetEntry(iev);
      LoopEvent(iev);
      //outroot_.PrintEntry();
      num_events_++;
   }
   EndOfEvent();
}

int Analysis::GetNumEvents()
{
   return num_events_;
}

void Analysis::BeginOfEvent()
{
   // impliment in derived class
}

void Analysis::LoopEvent(int iev)
{
   // impliment in derived class
}

void Analysis::EndOfEvent()
{
   // impliment in derived class
}

TGraph* Analysis::MakeGraph(int n, double* x, double* y, int style, int color)
{
   TGraph* gr = new TGraph(n, x, y);
   gr->SetMarkerStyle(style);
   gr->SetMarkerColor(color);
   gr->SetLineColor(color);
   return gr;
}

void Analysis::RedrawStatBox(double x1, double y1, double x2, double y2)
{
   gPad->Modified();
   gPad->Update();
   TPaveStats* stats = (TPaveStats*)gPad->GetPrimitive("stats");
   if (stats) {
      stats->SetX1NDC(x1);
      stats->SetY1NDC(y1);
      stats->SetX2NDC(x2);
      stats->SetY2NDC(y2);
      stats->Draw();
   }
}

//______________________________
AnaResXVSFitX::AnaResXVSFitX()
{
}

void AnaResXVSFitX::BeginOfEvent()
{
   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      h2_hitdT_VS_fitX_[test_cid] = new TH2F(Form("%s_hitdT_VS_fitX_%d", name_, test_cid), ";fitX [mm];hitdT [ns]", 200, -10, 10, 500, 0, 500);
      h2_hitX_VS_fitX_[test_cid]  = new TH2F(Form("%s_hitX_VS_fitX_%d",  name_, test_cid), ";fitX [mm];hitX [mm]", 200, -10, 10, 200, -10, 10);
      h2_resX_VS_fitX_[test_cid]  = new TH2F(Form("%s_resX_VS_fitX_%d",  name_, test_cid), ";fitX [mm];fitX-hitX [mm]", 200, -10, 10, 100, -2, 2);
      for (int ifitX=0; ifitX<100; ifitX++) {
         h1_hitdT_[test_cid][ifitX] = new TH1F(Form("%s_hitdT_%d_%d", name_, test_cid, ifitX), ";hitT-T0 [ns];", 500, 0, 500);
         h1_hitX_[test_cid][ifitX]  = new TH1F(Form("%s_hitX_%d_%d",  name_, test_cid, ifitX), ";hitX [mm];",    100, -10, 10);
         h1_resX_[test_cid][ifitX]  = new TH1F(Form("%s_resX_%d_%d",  name_, test_cid, ifitX), ";fitX-hitX [mm];", 100, -2, 2);
      }
   }
}

void AnaResXVSFitX::LoopEvent(int iev)
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double hitdT = outroot_.GetTrackHitdT(test_cid, test_cid);
      double fitX = outroot_.GetTrackFitX(test_cid, test_cid);
      double hitR = outroot_.GetTrackHitR(test_cid, test_cid);
      double chi2 = outroot_.GetTrackChi2(test_cid);
      double ndf = outroot_.GetTrackNDF(test_cid);
      if (ndf<0.1) continue;
      if (chi2/ndf>2.0) continue;

      double hitX = outroot_.GetTrackHitX(test_cid, test_cid);
      double resX = outroot_.GetTrackResX(test_cid, test_cid);

      h2_hitdT_VS_fitX_[test_cid]->Fill(fitX, hitdT);
      h2_hitX_VS_fitX_[test_cid]->Fill(fitX, hitX);
      h2_resX_VS_fitX_[test_cid]->Fill(fitX, resX);

      int ifitX = GetIdxFitX(fitX);
      h1_hitdT_[test_cid][ifitX]->Fill(hitdT);
      h1_hitX_[test_cid][ifitX]->Fill(hitX);
      h1_resX_[test_cid][ifitX]->Fill(resX);
   }
}

void AnaResXVSFitX::EndOfEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double x[100];
      double y1_hitdT[100];
      double y2_hitdT[100];
      double y3_hitdT[100];
      double y1_hitX[100];
      double y2_hitX[100];
      double y3_hitX[100];
      double y1_resX[100];
      double y2_resX[100];
      double y3_resX[100];
      for (int ifitX=0; ifitX<100; ifitX++) {
         x[ifitX] = GetFitXAt(ifitX);

         TH1F* h1_hitdT = h1_hitdT_[test_cid][ifitX];
         TH1F* h1_hitX  = h1_hitX_[test_cid][ifitX];
         TH1F* h1_resX  = h1_resX_[test_cid][ifitX];

         y1_hitdT[ifitX] = h1_hitdT->GetMean();
         y1_hitX[ifitX]  = h1_hitX->GetMean();
         y1_resX[ifitX]  = h1_resX->GetMean();

         double rms_hitdT = h1_hitdT->GetRMS();
         double rms_hitX  = h1_hitX->GetRMS();
         double rms_resX  = h1_resX->GetRMS();

         int imax_hitdT = h1_hitdT->GetMaximumBin();
         int imax_hitX  = h1_hitX->GetMaximumBin();
         int imax_resX  = h1_resX->GetMaximumBin();

         y2_hitdT[ifitX] = h1_hitdT->GetXaxis()->GetBinCenter(imax_hitdT);
         y2_hitX[ifitX]  = h1_hitX->GetXaxis()->GetBinCenter(imax_hitX);
         y2_resX[ifitX]  = h1_resX->GetXaxis()->GetBinCenter(imax_resX);

         h1_hitdT->Fit("gaus", "", "", y2_hitdT[ifitX]-rms_hitdT, y2_hitdT[ifitX]+rms_hitdT);
         h1_hitX-> Fit("gaus", "", "", y2_hitX[ifitX]-rms_hitX,   y2_hitX[ifitX]+rms_hitX);
         h1_resX-> Fit("gaus", "", "", y2_resX[ifitX]-rms_resX,   y2_resX[ifitX]+rms_resX);

         TF1* f1_hitdT = h1_hitdT->GetFunction("gaus");
         TF1* f1_hitX  = h1_hitX ->GetFunction("gaus");
         TF1* f1_resX  = h1_resX ->GetFunction("gaus");

         if (f1_hitdT) y3_hitdT[ifitX] = f1_hitdT->GetParameter(1);
         if (f1_hitX)  y3_hitX[ifitX]  = f1_hitX->GetParameter(1);
         if (f1_resX)  y3_resX[ifitX]  = f1_resX->GetParameter(1);
      }
      gr_hitdT_VS_fitX_mean_[test_cid] = MakeGraph(100, x, y1_hitdT, 20, kBlack);
      gr_hitdT_VS_fitX_peak_[test_cid] = MakeGraph(100, x, y2_hitdT, 20, kRed);
      gr_hitdT_VS_fitX_fit_ [test_cid] = MakeGraph(100, x, y3_hitdT, 20, kMagenta);

      gr_hitX_VS_fitX_mean_[test_cid] = MakeGraph(100, x, y1_hitX, 20, kBlack);
      gr_hitX_VS_fitX_peak_[test_cid] = MakeGraph(100, x, y2_hitX, 20, kRed);
      gr_hitX_VS_fitX_fit_ [test_cid] = MakeGraph(100, x, y3_hitX, 20, kMagenta);

      gr_resX_VS_fitX_mean_[test_cid] = MakeGraph(100, x, y1_resX, 20, kBlack);
      gr_resX_VS_fitX_peak_[test_cid] = MakeGraph(100, x, y2_resX, 20, kRed);
      gr_resX_VS_fitX_fit_ [test_cid] = MakeGraph(100, x, y3_resX, 20, kMagenta);
   }
}

TH2F* AnaResXVSFitX::GetHitdTVSFitX(int test_cid) { return h2_hitdT_VS_fitX_[test_cid]; }
TH2F* AnaResXVSFitX::GetHitXVSFitX(int test_cid)  { return h2_hitX_VS_fitX_[test_cid]; }
TH2F* AnaResXVSFitX::GetResXVSFitX(int test_cid)  { return h2_resX_VS_fitX_[test_cid]; }

TH1F* AnaResXVSFitX::GetHitdT(int test_cid, double fitX) { return h1_hitdT_[test_cid][GetIdxFitX(fitX)]; }
TH1F* AnaResXVSFitX::GetHitX(int test_cid, double fitX) { return h1_hitX_[test_cid][GetIdxFitX(fitX)]; }
TH1F* AnaResXVSFitX::GetResX(int test_cid, double fitX) { return h1_resX_[test_cid][GetIdxFitX(fitX)]; }

TGraph* AnaResXVSFitX::GetHitdTVSFitXMean(int test_cid) { return gr_hitdT_VS_fitX_mean_[test_cid]; }
TGraph* AnaResXVSFitX::GetHitdTVSFitXPeak(int test_cid) { return gr_hitdT_VS_fitX_peak_[test_cid]; }
TGraph* AnaResXVSFitX::GetHitdTVSFitXFit(int test_cid)  { return gr_hitdT_VS_fitX_fit_[test_cid]; }

TGraph* AnaResXVSFitX::GetHitXVSFitXMean(int test_cid) { return gr_hitX_VS_fitX_mean_[test_cid]; }
TGraph* AnaResXVSFitX::GetHitXVSFitXPeak(int test_cid) { return gr_hitX_VS_fitX_peak_[test_cid]; }
TGraph* AnaResXVSFitX::GetHitXVSFitXFit(int test_cid)  { return gr_hitX_VS_fitX_fit_[test_cid]; }

TGraph* AnaResXVSFitX::GetResXVSFitXMean(int test_cid) { return gr_resX_VS_fitX_mean_[test_cid]; }
TGraph* AnaResXVSFitX::GetResXVSFitXPeak(int test_cid) { return gr_resX_VS_fitX_peak_[test_cid]; }
TGraph* AnaResXVSFitX::GetResXVSFitXFit(int test_cid)  { return gr_resX_VS_fitX_fit_[test_cid]; }

void AnaResXVSFitX::DrawHitdTVSFitXMean(int test_cid)
{
   GetHitdTVSFitX(test_cid)->Draw("colz");
   GetHitdTVSFitXMean(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitdTVSFitXPeak(int test_cid)
{
   GetHitdTVSFitX(test_cid)->Draw("colz");
   GetHitdTVSFitXPeak(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitdTVSFitXFit(int test_cid)
{
   GetHitdTVSFitX(test_cid)->Draw("colz");
   GetHitdTVSFitXFit(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitXVSFitXMean(int test_cid)
{
   GetHitXVSFitX(test_cid)->Draw("colz");
   GetHitXVSFitXMean(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitXVSFitXPeak(int test_cid)
{
   GetHitXVSFitX(test_cid)->Draw("colz");
   GetHitXVSFitXPeak(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitXVSFitXFit(int test_cid)
{
   GetHitXVSFitX(test_cid)->Draw("colz");
   GetHitXVSFitXFit(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawResXVSFitXMean(int test_cid)
{
   GetResXVSFitX(test_cid)->Draw("colz");
   GetResXVSFitXMean(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawResXVSFitXPeak(int test_cid)
{
   GetResXVSFitX(test_cid)->Draw("colz");
   GetResXVSFitXPeak(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawResXVSFitXFit(int test_cid)
{
   GetResXVSFitX(test_cid)->Draw("colz");
   GetResXVSFitXFit(test_cid)->Draw("pl same");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

int AnaResXVSFitX::GetIdxFitX(double fitX)
{
   int ifitX = 0;
   for (int i=0; i<100; i++) {
      if (fitX>=(i-50)*0.2 && fitX<(i-50+1)*0.2) ifitX=i;
   }
   return ifitX;
}

double AnaResXVSFitX::GetFitXAt(int ifitX)
{
   return (ifitX-50)*0.2;
}
