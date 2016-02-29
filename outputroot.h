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
      void SetRootFile(const char* path);
      void Clear();
      void SetHitData(Event& event, Chamber& chamber);
      void SetTrackFinderData(TrackFinder& finder);
      void SetTrackData(Chamber& chamber, XTcurve& xt, Track& track);
      void Fill();
      void Write();
      void SetElapstedTime(int etime);

      // for reading
      void OpenRootFile(const char* root_path);
      int GetEntries();
      void GetEntry(int event_number);
      void PrintEntry();

      int    GetElapstedTime();
      int    GetEventNumber();
      int    GetNumHitCells(int cid);
      int    GetHitCellNumber(int cid, int ihitcell);
      double GetHitQ(int cid, int ihitcell);
      int    GetHitNumTdcHits(int cid, int ihitcell);
      double GetHitDriftTime(int cid, int ihitcell, int ihittdc);
      int    GetTrackfinderNumTracks();
      double GetTrackChi2(int test_cid);
      double GetTrackNDF(int test_cid);
      int    GetTrackCellNumber(int test_cid, int cid);
      double GetTrackCellQ(int test_cid, int cid);
      int    GetTrackCellNumTdcHits(int test_cid, int cid);
      double GetTrackHitT(int test_cid, int cid);
      double GetTrackFitT0(int test_cid, int cid);
      double GetTrackHitR(int test_cid, int cid);
      double GetTrackFitX(int test_cid, int cid);
      double GetTrackFitZ(int test_cid, int cid);
      double GetTrackHitdT(int test_cid, int cid);
      double GetTrackHitX(int test_cid, int cid);
      double GetTrackFitR(int test_cid, int cid);
      double GetTrackResX(int test_cid, int cid);
      double GetTrackResR(int test_cid, int cid);

      void ReadRunParameters(const char* path);
      void PrintRunParameters();
      const char* GetRootPath();
      const char* GetRunParamsPath();
      int GetFitFuncType();
      double GetT0_Bd0();
      double GetT0_Bd1();
      int GetLayerNumber1();
      int GetLayerNumber2();
      double GetZstep();
      double GetDriftVelocity();
      char* GetXTParamPath();

   private:
      char out_root_path_[1280];
      char run_params_path_[1280];

      TFile* f_;
      TTree* t_;
      int    etime_; // time from last event

      int    iev_; // [-2,147,483,648, 2,147,483,647] = okay until 2G events
      int    hit_num_hitcells_[MAX_LAYER];
      int    hit_icell_[MAX_LAYER][MAX_CELL];
      double hit_q_[MAX_LAYER][MAX_CELL];
      int    hit_num_hittdcs_[MAX_LAYER][MAX_CELL];
      double hit_drift_time_[MAX_LAYER][MAX_CELL][MAX_CELL_HIT];

      int    trackfinder_num_tracks_;

      double track_chi2_[MAX_LAYER]; // [test_cid]
      double track_ndf_[MAX_LAYER]; // [test_cid]
      int    track_icell_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      double track_q_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      int    track_num_hittdcs_[MAX_LAYER][MAX_CELL];
      double track_hitT_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      double track_fitT0_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      double track_hitR_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      double track_fitX_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]
      double track_fitZ_[MAX_LAYER][MAX_LAYER]; // [test_cid][layer]

      TTree* t2_; // for parameters
      int fit_func_type_;
      char fit_func_name_[64];
      double t0_bd0_;
      double t0_bd1_;
      int cid1_;
      int cid2_;
      double z_step_; // [mm]
      double drift_velocity_; // [mm/ns]
      char xt_param_path_[512];
};

#endif
