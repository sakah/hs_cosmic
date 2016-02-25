#ifndef _RUN_H
#define _RUN_H

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
      void PrintHits();
      void DrawHits();
      void MakeTangents(int cid1, int cid2, double z1, double z2);
      void PrintTangents();
      bool FindBestTangent(int cid1, int cid2, double z_step);
      void DrawBestTangent();
      void DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step);

   private:
      ConstXTcurve xt_;
      Chamber chamber_;
      Event event_;
      Track track_;
      TrackFinder finder_;
};

#endif
