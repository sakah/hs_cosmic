#ifndef _RUN_H
#define _RUN_H

#include "TH1F.h"
#include "TH2F.h"

#include "event.h"
#include "chamber.h"
#include "line.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "trackfinder.h"
#include "outputroot.h"

class Run
{
   public:
      Run();
      Run(const char* prog_path, const char* config_path, const char* input_root_path, const char* ouput_root_path);
      void GetNext();
      void GetEntry(Long64_t event_number);
      void SetT0(double t0_0, double t0_1);
      void PrintHits();
      void DrawHits();
      void MakeTangents(int cid1, int cid2, double z1, double z2);
      void DrawTangents();
      bool FindBestTangent(Long64_t event_number, int cid1, int cid2, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer);
      void DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer);
      bool DoFit(Long64_t event_number, int cid1, int cid2, double z_step, double t0_min, double t0_max, double t0_step, int test_cid, bool include_outer_guard_layer);
      void DrawFit(Long64_t event_number, int cid1, int cid2, double z_step, double t0_min, double t0_max, double t0_step, int test_cid, bool include_outer_guard_layer);
      void Loop(Long64_t start_iev, Long64_t last_iev);

   private:
      Config config_;
      XTcurve* xt_ptr_;
      Chamber chamber_;
      Event event_;
      Track track_;
      TrackFinder finder_;
      OutputROOT output_;
};

#endif
