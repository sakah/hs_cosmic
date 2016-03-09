#include "event.h"
#include "config.h"

Event::Event()
{
   config_ptr_ = NULL;
   wiremap_ptr_ = NULL;
   xtcurve_ptr_ = NULL;
   chamber_ptr_ = NULL;
   input_root_ptr_ = NULL;
}

Event::Event(Config* config_ptr)
{
   config_ptr_ = config_ptr;
   wiremap_ptr_ = config_ptr_->GetWireMap_Ptr();
   xtcurve_ptr_ = config_ptr_->GetXTcurve_Ptr();
   chamber_ptr_ = config_ptr_->GetChamber_Ptr();
   input_root_ptr_ = config_ptr->GetInputROOT_Ptr();
}

void Event::PrintHits()
{
   chamber_ptr_->PrintHits();
}

void Event::DrawHits()
{
   input_root_ptr_->DrawCanvas("run_draw_hits");
   chamber_ptr_->DrawHits();
}

/*
void Run::MakeTangents(int cid1, int cid2, double z1, double z2)
{
   bool alllayerhit=true;
   for (int cid=1; cid<=7; cid++) {
      int n =  chamber_.GetNumHitCells(cid);
      if (n==0) {
         alllayerhit = false;
         break;
      }
   }
   if (!alllayerhit) {
      return;
   }

   for (int cid=1; cid<=7; cid++) {
      int icell = chamber_.GetHitCellNumber(cid, 0);
      if (cid==1) {
         icell = chamber_.GetHitCellNumber(cid, 1);
      }
      Hit& hit = chamber_.GetHit(cid, icell, 0);
      hit.SetT0(chamber_.GetT0(cid, icell));
      track_.SetHit(cid, hit);
   }

   WireMap& wiremap = chamber_.GetWireMap();
   track_.MakeTangents(wiremap, *xt_ptr_, cid1, cid2, z1, z2);
   track_.PrintTangents(wiremap, *xt_ptr_);
}

void Run::DrawTangents()
{
   DrawHits();
   track_.DrawTangents();
}

bool Run::FindBestTangent(Long64_t event_number, int cid1, int cid2, int test_cid, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer)
{
   printf("cid1 %d\n", cid1);
   printf("cid2 %d\n", cid2);
   printf("test_cid %d\n", test_cid);
   printf("z_step %f\n", z_step);
   printf("t0_min %f\n", t0_min);
   printf("t0_max %f\n", t0_max);
   printf("t0_step %f\n", t0_step);
   printf("include_outer_guard_layer %d\n", include_outer_guard_layer);
   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   WireMap& wiremap = chamber_.GetWireMap();
   int t0_num = (t0_max - t0_min)/t0_step;
   double min_chi2 = 1e10;
   double min_t0 = 1e10;
   double chi2_of_small_track = 100;
   for (int it0=0; it0<t0_num; it0++) {
      double t0 = t0_min + it0*t0_step;
      printf("t0 %f\n", t0);
      chamber_.SetT0(0, t0);
      chamber_.SetT0(1, t0);
      int num_tracks = finder_.SetTracks(chamber_, *xt_ptr_, include_outer_guard_layer);
      if (num_tracks==0) {
         continue;
      }
      double chi2 = finder_.FindBestTrack(chamber_, *xt_ptr_, cid1, cid2, test_cid, z_step);
      printf("==> t0 %f -> chi2 %f\n", t0, chi2);
      if (chi2>1e9) {
         continue;
      }
      if (chi2<min_chi2) {
         min_chi2 = chi2;
         min_t0 = t0;
      }
      if (chi2<chi2_of_small_track) {
         printf("min_t0 %f\n", min_t0);
      }
      finder_.PrintTracks(wiremap, *xt_ptr_);
   }
   if (min_chi2>1e9) {
      return false;
   }
   return true;
}

void Run::DrawBestTangent(Long64_t event_number, int cid1, int cid2, int test_cid, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer)
{
   bool found = FindBestTangent(event_number, cid1, cid2, test_cid, z_step, t0_min, t0_max, t0_step, include_outer_guard_layer);
   if (!found) {
      printf("tangent cannot be found...\n");
      return;
   }
   WireMap& wiremap = chamber_.GetWireMap();
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrackWithLine(wiremap, *xt_ptr_, min_track.GetMinTangent(wiremap, *xt_ptr_));
   Line& min_tangent = min_track.GetMinTangent(wiremap, *xt_ptr_);
   chamber_.DrawTrack(event_, *xt_ptr_, min_track, min_tangent);
}

bool Run::DoFit(Long64_t event_number, int cid1, int cid2, int test_cid, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer)
{
   WireMap& wiremap = chamber_.GetWireMap();

   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   bool found = FindBestTangent(event_number, cid1, cid2, test_cid, z_step, t0_min, t0_max, t0_step, include_outer_guard_layer);
   if (!found) {
      printf("tangent cannot be found...\n");
      return false;
   }
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrackWithLine(wiremap, *xt_ptr_, min_track.GetMinTangent(wiremap, *xt_ptr_));
   //printf("### min_track.GetNumHitsUseByFit %d\n", min_track.GetNumHitsUseByFit());

   min_track.InitFit(wiremap, *xt_ptr_, test_cid, config_.GetFitFuncType());
   min_track.DoFit(wiremap, *xt_ptr_);
   min_track.PrintFitResults();
   min_track.PrintTrackWithLine(wiremap, *xt_ptr_, min_track.GetFitLine());
   return true;
}

void Run::DrawFit(Long64_t event_number, int cid1, int cid2, int test_cid, double z_step, double t0_min, double t0_max, double t0_step, bool include_outer_guard_layer)
{
   bool done = DoFit(event_number, cid1, cid2, test_cid, z_step, t0_min, t0_max, t0_step, include_outer_guard_layer);
   if (!done) {
      printf("fit cannot be done\n");
      return;
   }

   WireMap& wiremap = chamber_.GetWireMap();
   Track& min_track = finder_.GetBestTrack();
   Line& fit_line = min_track.GetFitLine();
   chamber_.DrawTrack(event_, *xt_ptr_, min_track, fit_line);
}

void Run::Loop(Long64_t start_iev, Long64_t last_iev)
{
   WireMap& wiremap = chamber_.GetWireMap();

   Long64_t total = event_.GetEntries();
   if (last_iev >= total) {
      last_iev = total -1;
   }
   printf("config_path %s\n", config_.GetConfigPath());
   printf("input_root_path %s\n", event_.GetRootPath());
   printf("output_root_path %s\n", output_.GetOutputRootPath());
   printf("total %lld\n", total);
   printf("start_iev %lld\n", start_iev);
   printf("last_iev %lld\n", last_iev);


   double t0_0  = config_.GetT0_Bd0();
   double t0_1  = config_.GetT0_Bd1();
   int cid1 = config_.GetLayerNumber1();
   int cid2 = config_.GetLayerNumber2();
   double z_step = config_.GetZstep();
   bool include_outer_guard_layer = config_.IncludeOuterGuardLayer();

   SetT0(t0_0, t0_1);

   int start_time = time(NULL);
   int prev_time = start_time;

   for (Long64_t iev=start_iev; iev<=last_iev; iev++) {
      //printf("iev %lld\n", iev);

      GetEntry(iev);
      output_.Clear();

      output_.SetHitData(event_, chamber_);

      int num_tracks = finder_.SetTracks(chamber_, *xt_ptr_, include_outer_guard_layer);
      //finder_.PrintTracks(wiremap, *xt_ptr_);

      output_.SetTrackFinderData(finder_);

      if (num_tracks>0) {
         for (int test_cid=1; test_cid<=8; test_cid++) {
            finder_.FindBestTrack(chamber_, *xt_ptr_, cid1, cid2, test_cid, z_step);
            Track track = finder_.GetBestTrack();
            //printf("test_cid %d\n", test_cid);
            track.InitFit(wiremap, *xt_ptr_, test_cid, config_.GetFitFuncType(), false);
            track.DoFit(wiremap, *xt_ptr_);
            //track.PrintFitResults();
            output_.SetTrackData(chamber_, *xt_ptr_, track);
         }
      }

      int etime = time(NULL)-prev_time;
      int atime = time(NULL)-start_time;
      double rate = (iev-start_iev+1)/(atime+0.1);
      printf("iev %lld num_tracks %d etime %d atime %d rate %3.2f Hz\n", iev, num_tracks, etime, atime, rate);
      output_.SetElapstedTime(etime);
      prev_time = time(NULL);
      output_.Fill();
   }
   output_.Write();
}
*/

