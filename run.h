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

class Run
{
   public:
      Run();
      Run(const char* root_file_path);
      void GetNext();
      void GetEntry(Long64_t event_number);
      void SetT0(double t0_0, double t0_1);
      void PrintHits();
      void DrawHits();
      void MakeTangents(int cid1, int cid2, double z1, double z2);
      void PrintTangents();
      void DrawTangents();
      bool FindBestTangent(Long64_t event_number, int cid1, int cid2, double z_step);
      void DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step);
      bool DoFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid);
      void DrawFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid);
      TH1F* FillResidual(const char* hname, int nx, double xmin, double xmax, int cid1, int cid2, double z_step, int cid3, int max_event);
      TH2F* FillXT(const char* hname, int nx, double xmin, double xmax, int ny, double ymin, double ymax, int cid1, int cid2, double z_step, int cid3, int max_event);

   private:
      ConstXTcurve xt_;
      Chamber chamber_;
      Event event_;
      Track track_;
      TrackFinder finder_;

};

#endif
