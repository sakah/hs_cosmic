#include "run.h"

Run::Run()
{
}

Run::Run(const char* prog_path, const char* config_path, const char* input_root_path, const char* output_root_path)
{
   config_.SetProgPath(prog_path);
   config_.ReadParameters(config_path);
   output_.ReadConfig(config_);
   output_.SetOutputRootFile(output_root_path);
   output_.FillConfig();

   if (config_.GetXTcurveType() == XTcurve::USE_CONST_XT) {
      xt_ptr_ = new ConstXTcurve;
   } else if (config_.GetXTcurveType() == XTcurve::USE_PARAM_POL4) {
      xt_ptr_ = new XTcurvePol4;
   }
   xt_ptr_->Setup(config_);

   chamber_.ReadWireMap("wirepos.proto4.sw.ver7.txt");
   event_.OpenRootFile(input_root_path);

   double t0_0 = config_.GetT0_Bd0();
   double t0_1 = config_.GetT0_Bd1();
   chamber_.SetT0(0, t0_0);
   chamber_.SetT0(1, t0_1);

}

void Run::GetNext()
{
   bool skip_empty_event = true;
   event_.GetNext(skip_empty_event);
   chamber_.GetEvent(event_);
}

void Run::GetEntry(Long64_t event_number)
{
   event_.GetEntry(event_number);
   chamber_.GetEvent(event_);
}

void Run::SetT0(double t0_0, double t0_1)
{
   chamber_.SetT0(0, t0_0);
   chamber_.SetT0(1, t0_1);
}

void Run::PrintHits()
{
   chamber_.PrintHits(*xt_ptr_);
}

void Run::DrawHits()
{
   chamber_.DrawHits(event_, *xt_ptr_);
}

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

bool Run::FindBestTangent(Long64_t event_number, int cid1, int cid2, double z_step, bool include_outer_guard_layer)
{
   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   WireMap& wiremap = chamber_.GetWireMap();
   int num_tracks = finder_.SetTracks(chamber_, *xt_ptr_, include_outer_guard_layer);
   if (num_tracks==0) {
      return false;
   }
   bool found = finder_.FindBestTrack(chamber_, *xt_ptr_, cid1, cid2, z_step);
   if (!found) return false;
   finder_.PrintTracks(wiremap, *xt_ptr_);
   return true;
}

void Run::DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step, bool include_outer_guard_layer)
{
   bool found = FindBestTangent(event_number, cid1, cid2, z_step, include_outer_guard_layer);
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

bool Run::DoFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid, bool include_outer_guard_layer)
{
   WireMap& wiremap = chamber_.GetWireMap();

   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   bool found = FindBestTangent(event_number, cid1, cid2, z_step, include_outer_guard_layer);
   if (!found) {
      printf("tangent cannot be found...\n");
      return false;
   }
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrackWithLine(wiremap, *xt_ptr_, min_track.GetMinTangent(wiremap, *xt_ptr_));
   printf("### min_track.GetNumHitsUseByFit %d\n", min_track.GetNumHitsUseByFit());

   min_track.InitFit(wiremap, *xt_ptr_, test_cid, config_.GetFitFuncType());
   min_track.DoFit(wiremap, *xt_ptr_);
   min_track.PrintFitResults();
   min_track.PrintTrackWithLine(wiremap, *xt_ptr_, min_track.GetFitLine());
   return true;
}

void Run::DrawFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid, bool include_outer_guard_layer)
{
   bool done = DoFit(event_number, cid1, cid2, z_step, test_cid, include_outer_guard_layer);
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
         finder_.FindBestTrack(chamber_, *xt_ptr_, cid1, cid2, z_step);
         for (int test_cid=1; test_cid<=7; test_cid++) {
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
