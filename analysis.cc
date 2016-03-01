#include "analysis.h"

#include "TF1.h"
#include "TPad.h"
#include "TPaveStats.h"

Analysis::Analysis()
{
   f1_abs_gaus_ = new TF1("abs_gaus", abs_gaus, 0, 500, 3);
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

TGraph* Analysis::MakeGraph(const char* name, int n, double* x, double* y, int style, int color)
{
   TGraph* gr = new TGraph(n, x, y);
   gr->SetName(name);
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
   fitX_xmin_ = -10;
   fitX_xmax_ = 10; // 10mm
   fitX_num_bins_ = (fitX_xmax_ - fitX_xmin_)/0.2; // 0.5mm/bin

   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {

      h2_hitdT_VS_fitX_[test_cid] = NULL;
      h2_hitX_VS_fitX_[test_cid] = NULL;
      h2_resX_VS_fitX_[test_cid] = NULL;
      h2_fitX_left_VS_hitdT_[test_cid] = NULL;
      h2_fitX_right_VS_hitdT_[test_cid] = NULL;


      gr_hitdT_VS_fitX_mean_[test_cid] = NULL;
      gr_hitdT_VS_fitX_peak_[test_cid] = NULL;
      gr_hitdT_VS_fitX_fit_[test_cid] = NULL;

      gr_hitX_VS_fitX_mean_[test_cid] = NULL;
      gr_hitX_VS_fitX_peak_[test_cid] = NULL;
      gr_hitX_VS_fitX_fit_[test_cid] = NULL;

      gr_resX_VS_fitX_mean_[test_cid] = NULL;
      gr_resX_VS_fitX_peak_[test_cid] = NULL;
      gr_resX_VS_fitX_fit_[test_cid] = NULL;

      gr_fitX_left_VS_hitdT_fit_[test_cid] = NULL;
      gr_fitX_right_VS_hitdT_fit_[test_cid] = NULL;

      for (int ifitX=0; ifitX<GetFitXNumBins(); ifitX++) {
         h1_hitdT_[test_cid][ifitX] = NULL;
         h1_hitX_[test_cid][ifitX] = NULL;
         h1_resX_[test_cid][ifitX] = NULL;
      }
   }
}

void AnaResXVSFitX::BeginOfEvent()
{
   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      h2_hitdT_VS_fitX_[test_cid] = new TH2F(Form("%s_hitdT_VS_fitX_%d", name_, test_cid), ";fitX [mm];hitdT [ns]", 200, -10, 10, 500, 0, 500);
      h2_hitX_VS_fitX_[test_cid]  = new TH2F(Form("%s_hitX_VS_fitX_%d",  name_, test_cid), ";fitX [mm];hitX [mm]", 200, -10, 10, 200, -10, 10);
      h2_resX_VS_fitX_[test_cid]  = new TH2F(Form("%s_resX_VS_fitX_%d",  name_, test_cid), ";fitX [mm];fitX-hitX [mm]", 200, -10, 10, 100, -2, 2);
      h2_fitX_left_VS_hitdT_[test_cid]  = new TH2F(Form("%s_fitX_left_VS_hitdT_%d",  name_, test_cid), ";hitdT [ns];fitX [mm]",  200, 0, 500, 100, 0, 10);
      h2_fitX_right_VS_hitdT_[test_cid] = new TH2F(Form("%s_fitX_right_VS_hitdT_%d",  name_, test_cid), ";hitdT [ns];fitX [mm]", 200, 0, 500, 100, 0, 10);
      for (int ifitX=0; ifitX<GetFitXNumBins(); ifitX++) {
         h1_hitdT_[test_cid][ifitX] = new TH1F(Form("%s_hitdT_%d_ifitX_%d", name_, test_cid, ifitX), ";hitT-T0 [ns];", 100, 0, 500);
         h1_hitX_[test_cid][ifitX]  = new TH1F(Form("%s_hitX_%d_ifitX_%d",  name_, test_cid, ifitX), ";hitX [mm];",    100, -10, 10);
         h1_resX_[test_cid][ifitX]  = new TH1F(Form("%s_resX_%d_ifitX_%d",  name_, test_cid, ifitX), ";fitX-hitX [mm];", 100, -2, 2);
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

      if (fitX<=0) { h2_fitX_left_VS_hitdT_[test_cid]->Fill(hitdT, -fitX); }
      if (fitX>=0) { h2_fitX_right_VS_hitdT_[test_cid]->Fill(hitdT, fitX); }
   }
}


void AnaResXVSFitX::EndOfEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
//   for (int test_cid=2; test_cid<3; test_cid++) {
      double x[100];
      double x_left[100];
      double x_right[100];
      double y1_hitdT[100];
      double y2_hitdT[100];
      double y3_hitdT[100];
      double y1_hitX[100];
      double y2_hitX[100];
      double y3_hitX[100];
      double y1_resX[100];
      double y2_resX[100];
      double y3_resX[100];

      double y3_hitdT_left[100];
      double y3_hitdT_right[100];
      int num_bins_left=0;
      int num_bins_right=0;
      for (int ifitX=0; ifitX<GetFitXNumBins(); ifitX++) {
         x[ifitX] = GetFitXAt(ifitX)+GetFitXdX()/2.0;

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

         // Set range of fitting by [-15, 15] from peak position
         h1_hitdT-> Fit("gaus", "", "", y2_hitdT[ifitX]-15, y2_hitdT[ifitX]+15);
         h1_hitX-> Fit("gaus", "", "", y2_hitX[ifitX]-2.0*rms_hitX,   y2_hitX[ifitX]+2.0*rms_hitX);
         h1_resX-> Fit("gaus", "", "", y2_resX[ifitX]-2.0*rms_resX,   y2_resX[ifitX]+2.0*rms_resX);

         TF1* f1_hitdT  = h1_hitdT ->GetFunction("gaus");
         TF1* f1_hitX  = h1_hitX ->GetFunction("gaus");
         TF1* f1_resX  = h1_resX ->GetFunction("gaus");

         if (f1_hitdT) y3_hitdT[ifitX] = f1_hitdT->GetParameter(1);
         if (f1_hitX)  y3_hitX[ifitX]  = f1_hitX->GetParameter(1);
         if (f1_resX)  y3_resX[ifitX]  = f1_resX->GetParameter(1);

         if (x[ifitX]<=0) { x_left[num_bins_left] = -x[ifitX];  y3_hitdT_left[num_bins_left] = y3_hitdT[ifitX];   num_bins_left++; }
         if (x[ifitX]>=0) { x_right[num_bins_right] = x[ifitX]; y3_hitdT_right[num_bins_right] = y3_hitdT[ifitX]; num_bins_right++; }
      }
      gr_hitdT_VS_fitX_mean_[test_cid] = MakeGraph(Form("%s_gr_hitdT_VS_fitX_mean_%d",name_,test_cid), GetFitXNumBins(), x, y1_hitdT, 20, kBlack);
      gr_hitdT_VS_fitX_peak_[test_cid] = MakeGraph(Form("%s_gr_hitdT_VS_fitX_peak_%d",name_,test_cid), GetFitXNumBins(), x, y2_hitdT, 20, kRed);
      gr_hitdT_VS_fitX_fit_ [test_cid] = MakeGraph(Form("%s_gr_hitdT_VS_fitX_fit_%d", name_,test_cid), GetFitXNumBins(), x, y3_hitdT, 20, kMagenta);

      gr_hitX_VS_fitX_mean_[test_cid] = MakeGraph(Form("%s_gr_hitX_VS_fitX_mean_%d", name_,test_cid), GetFitXNumBins(), x, y1_hitX, 20, kBlack);
      gr_hitX_VS_fitX_peak_[test_cid] = MakeGraph(Form("%s_gr_hitX_VS_fitX_peak_%d", name_,test_cid), GetFitXNumBins(), x, y2_hitX, 20, kRed);
      gr_hitX_VS_fitX_fit_ [test_cid] = MakeGraph(Form("%s_gr_hitX_VS_fitX_fit_%d",  name_,test_cid), GetFitXNumBins(), x, y3_hitX, 20, kMagenta);

      gr_resX_VS_fitX_mean_[test_cid] = MakeGraph(Form("%s_gr_resX_VS_fitX_mean_%d", name_,test_cid), GetFitXNumBins(), x, y1_resX, 20, kBlack);
      gr_resX_VS_fitX_peak_[test_cid] = MakeGraph(Form("%s_gr_resX_VS_fitX_peak_%d", name_,test_cid), GetFitXNumBins(), x, y2_resX, 20, kRed);
      gr_resX_VS_fitX_fit_ [test_cid] = MakeGraph(Form("%s_gr_resX_VS_fitX_fit_%d",  name_,test_cid), GetFitXNumBins(), x, y3_resX, 20, kMagenta);

      gr_fitX_left_VS_hitdT_fit_ [test_cid] = MakeGraph(Form("%s_gr_fitX_left_VS_hitdT_fit_%d", name_,test_cid), num_bins_left, y3_hitdT_left, x_left, 20, kMagenta);
      gr_fitX_right_VS_hitdT_fit_ [test_cid]= MakeGraph(Form("%s_gr_fitX_right_VS_hitdT_fit_%d", name_,test_cid), num_bins_right, y3_hitdT_right, x_right, 20, kMagenta);
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


TH2F*   AnaResXVSFitX::GetFitXVSHitdTLeft(int test_cid)  { return h2_fitX_left_VS_hitdT_[test_cid]; }
TH2F*   AnaResXVSFitX::GetFitXVSHitdTRight(int test_cid) { return h2_fitX_right_VS_hitdT_[test_cid]; }
TGraph* AnaResXVSFitX::GetFitXVSHitdTFitLeft(int test_cid)   { return gr_fitX_left_VS_hitdT_fit_[test_cid]; }
TGraph* AnaResXVSFitX::GetFitXVSHitdTFitRight(int test_cid)  { return gr_fitX_right_VS_hitdT_fit_[test_cid]; }


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

void AnaResXVSFitX::DrawFitXVSHitdTFitLeft(int test_cid)
{
   GetFitXVSHitdTLeft(test_cid)->Draw("colz");
   GetFitXVSHitdTFitLeft(test_cid)->Draw("pl same");
   RedrawStatBox(0.6, 0.7, 0.9, 0.9);
}

void AnaResXVSFitX::DrawFitXVSHitdTFitRight(int test_cid)
{
   GetFitXVSHitdTRight(test_cid)->Draw("colz");
   GetFitXVSHitdTFitRight(test_cid)->Draw("pl same");
   RedrawStatBox(0.6, 0.7, 0.9, 0.9);
}

int AnaResXVSFitX::GetIdxFitX(double fitX)
{
   int numBins = GetFitXNumBins();
   if (fitX<GetFitXAt(0)) return 0;
   if (fitX>=GetFitXAt(numBins-1)) return numBins-1;

   int ifitX = 0;
   for (int i=0; i<numBins; i++) {
      if (fitX>=GetFitXAt(i) && fitX<(GetFitXAt(i+1))) {
         ifitX=i;
         break;
      }
   }
   return ifitX;
}

int AnaResXVSFitX::GetFitXNumBins()
{
   return fitX_num_bins_;
}

double AnaResXVSFitX::GetFitXXmin()
{
   return fitX_xmin_;
}

double AnaResXVSFitX::GetFitXXmax()
{
   return fitX_xmax_;
}

double AnaResXVSFitX::GetFitXdX()
{
   return (fitX_xmax_ - fitX_xmin_)/fitX_num_bins_;
}

void AnaResXVSFitX::SetFitXNumBins(int numBins)
{
   fitX_num_bins_ = numBins;
}

void AnaResXVSFitX::SetFitXXmin(double xmin)
{
   fitX_xmin_ = xmin;
}

void AnaResXVSFitX::SetFitXXmax(double xmax)
{
   fitX_xmax_ = xmax;
}

double AnaResXVSFitX::GetFitXAt(int ifitX)
{
   int numBins = GetFitXNumBins();
   double xmin = GetFitXXmin();
   double xmax = GetFitXXmax();
   double dx = (xmax-xmin)/numBins;
   return (ifitX-numBins/2)*dx + dx/2.0; // at bin center
}

TF1* AnaResXVSFitX::FitHitdTVSFitXFitWithPol4(int test_cid, int left_or_right, double xmin, double xmax)
{
   TGraph* gr = gr_hitdT_VS_fitX_fit_[test_cid];
   if (gr==NULL) return NULL;

   gr->Fit("pol4", "", "", xmin, xmax);
   return gr->GetFunction("pol4");
}

void AnaResXVSFitX::FitHitdTVSFitXFitWithPol4(char* output_txt)
{
   FILE* fp = fopen(output_txt, "w");
   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      for (int left_or_right=0; left_or_right<2; left_or_right++) {
         double xmin;
         double xmax;
         if (left_or_right==XTcurve::LEFT) {
            xmin = -8.0;
            xmax = 0.0;
         } else {
            xmin = 0.0;
            xmax = 8.0;
         }

         TF1* f1 = FitHitdTVSFitXFitWithPol4(test_cid, left_or_right, xmin, xmax);
         if (f1==NULL) continue;
         double pol0 = f1->GetParameter(0);
         double pol1 = f1->GetParameter(1);
         double pol2 = f1->GetParameter(2);
         double pol3 = f1->GetParameter(3);
         double pol4 = f1->GetParameter(4);
         fprintf(fp, "%d %d %f %f %f %f %f\n", test_cid, left_or_right, pol0, pol1, pol2, pol3, pol4);
      }
   }
   fclose(fp);
}

TF1* AnaResXVSFitX::FitFitXVSHitdTFitWithPol4(int test_cid, int left_or_right, double xmin, double xmax)
{
   TGraph* gr = gr_fitX_left_VS_hitdT_fit_[test_cid];
   if (left_or_right==XTcurve::RIGHT) {
      gr = gr_fitX_right_VS_hitdT_fit_[test_cid];
   }
   if (gr==NULL) return NULL;

   gr->Fit("pol4", "", "", xmin, xmax);
   return gr->GetFunction("pol4");
}

void AnaResXVSFitX::FitFitXVSHitdTFitWithPol4(char* output_txt)
{
   FILE* fp = fopen(output_txt, "w");
   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      for (int left_or_right=0; left_or_right<2; left_or_right++) {
         double tmin = 0;
         double tmax = 500;

         TF1* f1 = FitHitdTVSFitXFitWithPol4(test_cid, left_or_right, tmin, tmax);
         if (f1==NULL) continue;
         double pol0 = f1->GetParameter(0);
         double pol1 = f1->GetParameter(1);
         double pol2 = f1->GetParameter(2);
         double pol3 = f1->GetParameter(3);
         double pol4 = f1->GetParameter(4);
         fprintf(fp, "%d %d %f %f %f %f %f\n", test_cid, left_or_right, pol0, pol1, pol2, pol3, pol4);
      }
   }
   fclose(fp);
}
