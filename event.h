#ifndef EVENT_H
#define EVENT_H

#include "wiremap.h"
#include "xtcurve.h"
#include "chamber.h"
#include "inputroot.h"
//#include "line.h"
//#include "track.h"
//#include "xtcurve.h"
//#include "trackfinder.h"
//#include "outputroot.h"

class Config;

class Event
{
   public:
      Event();
      Event(Config* config_ptr);

      void PrintHits();
      void DrawHits();

      /*
      TrackList GetTrackListByTangent(int cid1, int cid2, FindRange range, Range chi2);
      TrackList GetTrackListByFit(int cid1, int cid2, FindRange range, Range chi2);

      void Loop(const char* prog_path, const char* config_path, Long64_t start_iev, Long64_t last_iev);
      */

   private:
      Config* config_ptr_;
      WireMap* wiremap_ptr_;
      XTcurve* xtcurve_ptr_;
      Chamber* chamber_ptr_;
      InputROOT* input_root_ptr_;

      //TrackFinder track_finder_;
      //TrackFitter* track_fitter_ptr_;
      //OutputROOT output_root_;
};

#endif
