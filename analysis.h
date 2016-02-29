#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include "outputroot.h"

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"

class Analysis
{
   public:
      Analysis();
      void OpenRootFile(const char* path);
      void Loop(int max_events=2147483647);
      int GetNumEvents();
      virtual void BeginOfEvent();
      virtual void LoopEvent(int iev);
      virtual void EndOfEvent();

   protected:
      OutputROOT outroot_;
      char name_[128]; // set uniq name according to path

      TGraph* MakeGraph(const char*name, int n, double* x, double* y, int style, int color);
      void RedrawStatBox(double x1, double y1, double x2, double y2);
      void SetCutChi2OVNDF(double cut_chi2_ov_ndf);

   private:
      int num_events_; // total number of analized events
};

class AnaResXVSFitX : public Analysis
{
   public:
      AnaResXVSFitX();
      virtual void BeginOfEvent();
      virtual void LoopEvent(int iev);
      virtual void EndOfEvent();

      TH2F* GetHitdTVSFitX(int test_cid);
      TH2F* GetHitXVSFitX(int test_cid);
      TH2F* GetResXVSFitX(int test_cid);

      TH1F* GetHitdT(int test_cid, double fitX);
      TH1F* GetHitX(int test_cid, double fitX);
      TH1F* GetResX(int test_cid, double fitX);

      TGraph* GetHitdTVSFitXMean(int test_cid);
      TGraph* GetHitdTVSFitXPeak(int test_cid);
      TGraph* GetHitdTVSFitXFit(int test_cid);

      TGraph* GetHitXVSFitXMean(int test_cid);
      TGraph* GetHitXVSFitXPeak(int test_cid);
      TGraph* GetHitXVSFitXFit(int test_cid);

      TGraph* GetResXVSFitXMean(int test_cid);
      TGraph* GetResXVSFitXPeak(int test_cid);
      TGraph* GetResXVSFitXFit(int test_cid);

      void DrawHitdTVSFitXMean(int test_cid);
      void DrawHitdTVSFitXPeak(int test_cid);
      void DrawHitdTVSFitXFit(int test_cid);

      void DrawHitXVSFitXMean(int test_cid);
      void DrawHitXVSFitXPeak(int test_cid);
      void DrawHitXVSFitXFit(int test_cid);

      void DrawResXVSFitXMean(int test_cid);
      void DrawResXVSFitXPeak(int test_cid);
      void DrawResXVSFitXFit(int test_cid);

      TF1* FitHitdTVSFitXFitWithPol4(int test_cid, int left_or_right, double xmin, double xmax);
      void FitHitdTVSFitXFitWithPol4(char* output_txt);

   private:
      TH2F* h2_hitdT_VS_fitX_[MAX_LAYER];
      TH1F* h1_hitdT_[MAX_LAYER][100];

      TH2F* h2_hitX_VS_fitX_[MAX_LAYER];
      TH1F* h1_hitX_[MAX_LAYER][100];

      TH2F* h2_resX_VS_fitX_[MAX_LAYER];
      TH1F* h1_resX_[MAX_LAYER][100];

      TGraph* gr_hitdT_VS_fitX_mean_[MAX_LAYER];
      TGraph* gr_hitdT_VS_fitX_peak_[MAX_LAYER];
      TGraph* gr_hitdT_VS_fitX_fit_[MAX_LAYER];

      TGraph* gr_hitX_VS_fitX_mean_[MAX_LAYER];
      TGraph* gr_hitX_VS_fitX_peak_[MAX_LAYER];
      TGraph* gr_hitX_VS_fitX_fit_[MAX_LAYER];

      TGraph* gr_resX_VS_fitX_mean_[MAX_LAYER];
      TGraph* gr_resX_VS_fitX_peak_[MAX_LAYER];
      TGraph* gr_resX_VS_fitX_fit_[MAX_LAYER];

      int GetIdxFitX(double fitX);
      double GetFitXAt(int ifitX);

};

#endif
