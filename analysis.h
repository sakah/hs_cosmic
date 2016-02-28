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
      virtual void BeginOfEvent();
      virtual bool LoopEvent();
      virtual void EndOfEvent();

   protected:
      OutputROOT outroot_;

   private:
      char name_[128]; // set uniq name according to path
};

class AnaResRVSHitR : public Analysis
{
   public:
      AnaResRVSHitR();
      virtual void BeginOfEvent();
      virtual bool LoopEvent();
      virtual void EndOfEvent();
      TH2F* GetResRVSHitR(int test_cid);
      TGraph* GetResRMeanVSHitR(int test_cid);

   private:
      TH2F* h2_resR_VS_hitR_[MAX_LAYER]; // [test_cid]
      TGraph* gr_resR_mean_VS_hitR_[MAX_LAYER];
      double resR_mean_[MAX_LAYER][20];  // [test_cid][ihitR]
};

#endif
