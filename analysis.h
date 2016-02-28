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

   private:
      int num_events_; // total number of analized events
};

class AnaResRVSHitR : public Analysis
{
   public:
      AnaResRVSHitR();
      virtual void BeginOfEvent();
      virtual bool LoopEvent();
      virtual void EndOfEvent();
      TH2F* GetResRVSHitR(int test_cid);
      TH1F* GetResR(int test_cid, int ihitR);
      TGraph* GetResRVSHitRMean(int test_cid);
      TGraph* GetResRVSHitRPeak(int test_cid);
      void DrawResRVSHitRMean(int test_cid);
      void DrawResRVSHitRPeak(int test_cid);

   private:
      TH2F* h2_resR_VS_hitR_[MAX_LAYER]; // [test_cid]
      TH1F* h1_resR_[MAX_LAYER][20]; // [test_cid]
      TGraph* gr_resR_mean_VS_hitR_[MAX_LAYER];
      TGraph* gr_resR_peak_VS_hitR_[MAX_LAYER];
};

#endif
