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
      virtual bool LoopEvent();
      virtual void EndOfEvent();

   protected:
      OutputROOT outroot_;
      char name_[128]; // set uniq name according to path

      TGraph* MakeGraph(int n, double* x, double* y, int style, int color);
      void RedrawStatBox(double x1, double y1, double x2, double y2);

   private:
      int num_events_; // total number of analized events
};

class AnaResXVSHitX : public Analysis
{
   public:
      AnaResXVSHitX();
      virtual void BeginOfEvent();
      virtual bool LoopEvent();
      virtual void EndOfEvent();
      TH2F* GetFitXVSHitX(int test_cid);
      TH2F* GetResXVSHitX(int test_cid);
      TH1F* GetResX(int test_cid, int ihitX);
      TGraph* GetResXVSHitXMean(int test_cid);
      TGraph* GetResXVSHitXPeak(int test_cid);
      TGraph* GetResXVSHitXFit(int test_cid);
      void DrawResXVSHitXMean(int test_cid);
      void DrawResXVSHitXPeak(int test_cid);
      void DrawResXVSHitXFit(int test_cid);
      void DrawFitXVSHitX(int test_cid);

   private:
      TH2F* h2_fitX_VS_hitX_[MAX_LAYER]; // [test_cid]
      TH2F* h2_resX_VS_hitX_[MAX_LAYER]; // [test_cid]
      TH1F* h1_resX_[MAX_LAYER][100]; // [test_cid]
      TGraph* gr_resX_VS_hitX_mean_[MAX_LAYER];
      TGraph* gr_resX_VS_hitX_peak_[MAX_LAYER];
      TGraph* gr_resX_VS_hitX_fit_[MAX_LAYER];
};

#endif
