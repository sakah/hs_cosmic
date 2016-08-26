#ifndef OUTPUT_ROOT_H
#define OUTPUT_ROOT_H

#include "param.h"
#include "event.h"
#include "chamber.h"
#include "xtcurve.h"
#include "wiremap.h"
#include "trackfinder.h"
#include "track.h"

#include "TFile.h"
#include "TTree.h"

class OutputROOT
{
   public:
      OutputROOT();
      OutputROOT(const char* path);
      void Clear();
      void SetRootFile(const char* path);
      void SetHitData(Event& event, Chamber& chamber);
      void SetTrackFinderData(TrackFinder& finder);
      void SetTrackData(Chamber& chamber, XTcurve& xt, Track& track);
      void Fill();
      void Write();
      void SetElapstedTime(int etime);

   private:
      TFile* f_;
      TTree* t_;
      int    etime_; // time from last event

      int    iev_;
      int    trig_number_;
      int    hit_num_hitcells_[MAX_SIDE][MAX_LAYER];
      int    hit_icell_[MAX_SIDE][MAX_LAYER][MAX_CELL];
      double hit_q_[MAX_SIDE][MAX_LAYER][MAX_CELL];
      int    hit_num_hittdcs_[MAX_SIDE][MAX_LAYER][MAX_CELL];
      double hit_drift_time_[MAX_SIDE][MAX_LAYER][MAX_CELL][MAX_CELL_HIT];

      int    trackfinder_num_tracks_;

      double track_chi2_[MAX_SIDE][MAX_LAYER]; // [test_cid]
      double track_ndf_[MAX_SIDE][MAX_LAYER]; // [test_cid]
      int    track_icell_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_q_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      int    track_num_hittdcs_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_CELL];
      double track_hitT_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_fitT0_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_fitdT_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_hitR_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_fitX_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_resR_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
      double track_fitZ_[MAX_SIDE][MAX_LAYER][MAX_SIDE][MAX_LAYER]; // [test_cid][layer]
};

#endif
