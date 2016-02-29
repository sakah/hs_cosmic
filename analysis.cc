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
   char* p1 = strrchr(path, '/');
   char* p2 = strrchr(path, '.');
   if (p1==NULL) { p1 = (char*)path; }
   else { p1++; }
   p2--;
   int i=0;
   for (char* p = p1; p<=p2; p++) {
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
      h2_hitX_VS_fitX_[test_cid] = new TH2F(Form("%s_hitX_VS_fitX_%d", name_, test_cid), ";fitX [mm];hitX [mm]", 200, -10, 10, 200, -10, 10);
      h2_resX_VS_fitX_[test_cid] = new TH2F(Form("%s_resX_VS_fitX_%d", name_, test_cid), ";fitX [mm];fitX-hitX [mm]", 200, -10, 10, 100, -2, 2);
      for (int ihitX=0; ihitX<100; ihitX++) {
         h1_resX_[test_cid][ihitX] = new TH1F(Form("%s_resX_%d_%d", name_, test_cid, ihitX), ";fitX-hitX [mm];", 100, -2, 2);
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
      h2_resX_VS_fitX_[test_cid]->Fill(fitX, resX);
      h2_hitX_VS_fitX_[test_cid]->Fill(fitX, hitX);

      int ihitX = 0;
      for (int i=0; i<100; i++) {
         if (hitX>=(i-50)*0.2 && hitX<(i-50+1)*0.2) ihitX=i;
      }
      h1_resX_[test_cid][ihitX]->Fill(resX);
   }
}

void AnaResXVSFitX::EndOfEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double x[100];
      double y1[100];
      double y2[100];
      double y3[100];
      for (int ihitX=0; ihitX<100; ihitX++) {
         x[ihitX] = (ihitX-50)*0.2;
         TH1F* h1 = h1_resX_[test_cid][ihitX];
         y1[ihitX] = h1->GetMean();
         int imax = h1->GetMaximumBin();
         y2[ihitX] = h1->GetXaxis()->GetBinCenter(imax);
         h1->Fit("gaus", "0", "", y2[ihitX]-0.5, y2[ihitX]+0.5);
         y3[ihitX] = h1->GetFunction("gaus")->GetParameter(1);
      }
      gr_resX_VS_fitX_mean_[test_cid] = MakeGraph(100, x, y1, 20, kBlack);
      gr_resX_VS_fitX_peak_[test_cid] = MakeGraph(100, x, y2, 20, kRed);
      gr_resX_VS_fitX_fit_ [test_cid] = MakeGraph(100, x, y3, 20, kMagenta);
   }
} 

TH2F* AnaResXVSFitX::GetHitdTVSFitX(int test_cid)
{
   return h2_hitdT_VS_fitX_[test_cid];
}

TH2F* AnaResXVSFitX::GetHitXVSFitX(int test_cid)
{
   return h2_hitX_VS_fitX_[test_cid];
}

TH2F* AnaResXVSFitX::GetResXVSFitX(int test_cid)
{
   return h2_resX_VS_fitX_[test_cid];
}

TH1F* AnaResXVSFitX::GetResX(int test_cid, int ihitX)
{
   return h1_resX_[test_cid][ihitX];
}

TGraph* AnaResXVSFitX::GetResXVSFitXMean(int test_cid)
{
   return gr_resX_VS_fitX_mean_[test_cid];
}

TGraph* AnaResXVSFitX::GetResXVSFitXPeak(int test_cid)
{
   return gr_resX_VS_fitX_peak_[test_cid];
}

TGraph* AnaResXVSFitX::GetResXVSFitXFit(int test_cid)
{
   return gr_resX_VS_fitX_fit_[test_cid];
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

void AnaResXVSFitX::DrawHitXVSFitX(int test_cid)
{
   GetHitXVSFitX(test_cid)->Draw("colz");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}

void AnaResXVSFitX::DrawHitdTVSFitX(int test_cid)
{
   GetHitdTVSFitX(test_cid)->Draw("colz");
   RedrawStatBox(0.1, 0.7, 0.4, 0.9);
}
