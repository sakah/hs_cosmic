#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "run.h"
#include "trackfindrange.h"
#include "wiremap.h"
#include "xtcurve.h"
#include "chamber.h"
#include "inputroot.h"
#include "event.h"

Config::~Config()
{
   if (wiremap_ptr_) delete wiremap_ptr_;
   if (xtcurve_ptr_) delete xtcurve_ptr_;
   if (chamber_ptr_) delete chamber_ptr_;
   if (input_root_ptr_) delete input_root_ptr_;
   if (event_ptr_) delete event_ptr_;
}


Config::Config()
{
   wiremap_ptr_ = NULL;
   xtcurve_ptr_ = NULL;
   chamber_ptr_ = NULL;
   input_root_ptr_ = NULL;
   event_ptr_ = NULL;

   num_runs_ = 0;
}

Config::Config(const char* top_dir)
   :Config()
{
   strcpy(top_dir_, top_dir);
   SetDefaults();
}

Config::Config(const char* top_dir, const char* config_path)
   :Config()
{
   strcpy(top_dir_, top_dir);
   ReadConfig(config_path);
}

Config::Config(const Config& other)
   :Config()
{
   //printf("A) config copy constuctor is called\n");
   CopyConfig(other);
   //printf("B) config copy constuctor is called\n");
   //ls();
}

void Config::CopyConfig(const Config& other)
{
    strcpy(top_dir_, other.GetTopDir());
    strcpy(config_path_, other.GetConfigPath());
    strcpy(wiremap_path_, other.GetWireMapPath());
    strcpy(input_root_dir_, other.GetInputROOTDir());
    strcpy(input_root_name_templ_, other.GetInputROOTNameTempl());
    strcpy(xt_curve_name_, other.GetXTcurveName());
    strcpy(xt_param_path_, other.GetXTParamPath());
    xt_drift_velocity_ = other.GetXTDriftVelocity();
    xt_sigma_r_ = other.GetXTSigmaR();
    strcpy(fit_func_name_, other.GetFitFuncName());
    num_boards_ = other.GetT0Boards(t0_boards_);
    track_find_cid1_ = other.GetTrackFindLayerNumber1();
    track_find_cid2_ = other.GetTrackFindLayerNumber2();
    track_find_range_ = other.GetTrackFindRange();
    use_inner_guard_layer_ = other.UseInnerGuardLayer();
    use_outer_guard_layer_ = other.UseOuterGuardLayer();
}

void Config::PrintConfig()
{
   printf("--------------------------------------------------\n");
   printf("top_dir                %-6s\n", top_dir_);
   printf("config_path            %-6s\n", config_path_);
   printf("wiremap_path           %-6s\n", wiremap_path_);
   printf("input_root_dir         %-6s\n", input_root_dir_);
   printf("input_root_name_templ  %-6s\n", input_root_name_templ_);
   printf("num_boards             %-6d\n", num_boards_);
   printf("t0_baords              "); for (int bd=0; bd<num_boards_; bd++) { printf("%6.2f  ", t0_boards_[bd]); } printf("[ns]\n");
   printf("xt_curve_name          %-6s\n", xt_curve_name_);
   printf("xt_param_path          %-6s\n", xt_param_path_);
   printf("xt_drift_velocity      %-6.2f [mm/ns]\n", xt_drift_velocity_);
   printf("xt_sigma_r             %-6.2f [mm]\n", xt_sigma_r_);
   printf("fit_func_name          %-6s\n", fit_func_name_);
   printf("track_find_cid1        %-6d\n", track_find_cid1_);
   printf("track_find_cid2        %-6d\n", track_find_cid2_);
   printf("use_inner_guard_layer  %-6d\n", use_inner_guard_layer_);
   printf("use_outer_guard_layer  %-6d\n", use_outer_guard_layer_);
   printf("... track_find_range ...\n");
   track_find_range_.PrintTrackFindRange();
   printf("........................\n");
   printf("--------------------------------------------------\n");
}
void Config::ls()
{
   PrintConfig();
}

void Config::SetDefaults()
{
   strcpy(config_path_, "NOT_SET");
   strcpy(wiremap_path_, "NOT_SET");
   strcpy(input_root_dir_, "NOT_SET");
   strcpy(input_root_name_templ_, "NOT_SET");
   num_boards_ = 2;
   for (int bd=0; bd<MAX_BOARD; bd++) {
      t0_boards_[bd] = -850;
   }
   strcpy(xt_curve_name_, "NOT_SET");
   strcpy(xt_param_path_, "NOT_SET");
   xt_drift_velocity_ = 0.04;  // [mm/ns]
   xt_sigma_r_ = 0.2; // 200um
   strcpy(fit_func_name_, "FIT_FUNC_TYPE_FREE_T0");
   use_inner_guard_layer_ = 1;
   use_outer_guard_layer_ = 1;
   track_find_cid1_ = 1;
   track_find_cid2_ = 7;
   //track_find_range_; // defalut
}

void Config::ReadConfig(const char* config_path)
{
   strcpy(config_path_, config_path);

   FILE* fp = fopen(config_path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open %s\n", config_path);
      return;
   }
   char line[128];
   double t0;
   double min;
   double max;
   double step;
   int imin;
   int imax;
   while(fgets(line, sizeof(line), fp)) {
      //printf("line -> '%s'", line);
      if (strstr(line, "wiremap_path"))          sscanf(line, "wiremap_path %s", wiremap_path_);
      if (strstr(line, "input_root_dir"))        sscanf(line, "input_root_dir %s", input_root_dir_);
      if (strstr(line, "input_root_name_templ")) sscanf(line, "input_root_name_templ %s", input_root_name_templ_);
      if (strstr(line, "num_boards"))            sscanf(line, "num_boards %d", &num_boards_);
      if (strstr(line, "t0_all"))               { sscanf(line, "t0_all %lf", &t0); for (int bd=0; bd<MAX_BOARD; bd++) { t0_boards_[bd] = t0;} }
      if (strstr(line, "xt_curve_name"))         sscanf(line, "xt_curve_name %s", xt_curve_name_);
      if (strstr(line, "xt_param_path"))         sscanf(line, "xt_param_path %s", xt_param_path_); // relative path
      if (strstr(line, "xt_drift_velocity"))     sscanf(line, "xt_drift_velocity %lf", &xt_drift_velocity_);
      if (strstr(line, "xt_sigma_r"))            sscanf(line, "xt_sigma_r %lf",     &xt_sigma_r_);
      if (strstr(line, "fit_func_name"))         sscanf(line, "fit_func_name %s", fit_func_name_);
      if (strstr(line, "track_find_cid1"))       sscanf(line, "track_find_cid1 %d", &track_find_cid1_);
      if (strstr(line, "track_find_cid2"))       sscanf(line, "track_find_cid2 %d", &track_find_cid2_);
      if (strstr(line, "track_find_range_z1"))   { sscanf(line, "track_find_range_z1 %lf %lf %lf", &min, &max, &step); track_find_range_.z1().Update(min, max, step); }
      if (strstr(line, "track_find_range_z2"))   { sscanf(line, "track_find_range_z2 %lf %lf %lf", &min, &max, &step); track_find_range_.z2().Update(min, max, step); }
      if (strstr(line, "track_find_range_t0"))   { sscanf(line, "track_find_range_t0 %lf %lf %lf", &min, &max, &step); track_find_range_.t0().Update(min, max, step); }
      if (strstr(line, "track_find_range_drift_radius")) { sscanf(line, "trak_find_drift_radius %lf %lf", &min, &max); track_find_range_.dr().Update(min, max, -1); }
      if (strstr(line, "track_find_range_num_tracks")) { sscanf(line, "trak_find_range_num_tracks %d %d", &imin, &imax); track_find_range_.nt().Update(imin, imax, -1); }
      if (strstr(line, "use_inner_guard_layer")) sscanf(line, "use_inner_guard_layer %d", &use_inner_guard_layer_);
      if (strstr(line, "use_outer_guard_layer")) sscanf(line, "use_outer_guard_layer %d", &use_outer_guard_layer_);
   }
   fclose(fp);

   PrintConfig();
}

//Config Config::NewConfig(int par)
//{
//   Config new_config; new_config.CopyConfig(*this);
//   return new_config;
//}

Config Config::track_find_cid1(int new_value)
{
   Config new_config; new_config.CopyConfig(*this);
   return new_config;
}
Config Config::track_find_cid2(int new_value, int aa)
{
   Config new_config; new_config.CopyConfig(*this);
   return new_config;
}
//Config Config::track_find_cid3(Hoge a)
//{
//   Config new_config; new_config.CopyConfig(*this);
//   return new_config;
//}
Config Config::track_find_cid4(char str)
{
   Config new_config; new_config.CopyConfig(*this);
   return new_config;
}
//Config Config::NewConfig(int par, int par2)
//{
//   Config new_config; new_config.CopyConfig(*this);
//   return new_config;
//}
//
//Config Config::NewConfig(TString par)
//{
//   Config new_config; new_config.CopyConfig(*this);
//   return new_config;
//}

/*
Config* Config::ChangeConfig(const char* name, const char* value)
{
   Config *new_config = new Config; new_config->CopyConfig(*this);
   new_config->ls();
   if (strstr(name, "top_dir"))               strcpy(new_config->top_dir_,        value);
   if (strstr(name, "wiremap_path"))          strcpy(new_config->wiremap_path_,   value);
   if (strstr(name, "input_root_dir"))        strcpy(new_config->input_root_dir_, value);
   if (strstr(name, "input_root_name_templ")) strcpy(new_config->input_root_name_templ_, value);
   if (strstr(name, "xt_curve_name"))         strcpy(new_config->xt_curve_name_, value);
   if (strstr(name, "xt_param_path"))         strcpy(new_config->xt_param_path_, value);
   if (strstr(name, "fit_func_name"))         strcpy(new_config->fit_func_name_, value);
   return new_config;
}

Config Config::ChangeConfig(const char* name, int value)
{
   Config new_config; new_config.CopyConfig(*this);
   if (strstr(name, "track_find_cid1"))       new_config.track_find_cid1_ = value;
   if (strstr(name, "track_find_cid2"))       new_config.track_find_cid2_ = value;
   if (strstr(name, "use_inner_guard_layer")) new_config.use_inner_guard_layer_ = value;
   if (strstr(name, "use_outer_guard_layer")) new_config.use_outer_guard_layer_ = value;
   return new_config;
}

Config Config::ChangeConfig(const char* name, double value)
{
   Config new_config; new_config.CopyConfig(*this);
   if (strstr(name, "xt_drift_velocity")) new_config.xt_drift_velocity_ = value;
   if (strstr(name, "xt_sigma_r"))        new_config.xt_sigma_r_ = value;
   return new_config;
}

Config Config::ChangeConfig(const char* name, int idx, double value)
{
   Config new_config; new_config.CopyConfig(*this);
   if (strstr(name, "t0")) new_config.t0_boards_[idx] = value;
   return new_config;
}

Config Config::ChangeConfig(const char* name, double min, double max, double step)
{
   Config new_config; new_config.CopyConfig(*this);
   if (strstr(name, "track_find_range_z1"))            new_config.track_find_range_.z1().Update(min, max, step);
   if (strstr(name, "track_find_range_z2"))            new_config.track_find_range_.z2().Update(min, max, step);
   if (strstr(name, "track_find_range_t0"))            new_config.track_find_range_.t0().Update(min, max, step);
   if (strstr(name, "track_find_range_drift_radius"))  new_config.track_find_range_.dr().Update(min, max, -1);
   return new_config;
}

Config Config::ChangeConfig(const char* name, int min, int max, int step)
{
   Config new_config; new_config.CopyConfig(*this);
   if (strstr(name, "track_find_range_num_tracks")) new_config.track_find_range_.nt().Update(min, max, -1);
   return new_config;
}
*/

int Config::GetNumBoards() const
{
   return num_boards_;
}

double Config::GetT0Board(int board) const
{
   return t0_boards_[board];
}

int Config::GetT0Boards(double* t0_boards) const
{
   for (int bd=0; bd<num_boards_; bd++) {
      t0_boards[bd] = t0_boards_[bd];
   }
   return num_boards_;
}

const char* Config::GetXTcurveName() const
{
   return xt_curve_name_;
}

const char* Config::GetXTParamPath() const
{
   return xt_param_path_;
}

double Config::GetXTDriftVelocity() const
{
   return xt_drift_velocity_;
}

double Config::GetXTSigmaR() const
{
   return xt_sigma_r_;
}

const char* Config::GetFitFuncName() const
{
   return fit_func_name_;
}

int Config::UseInnerGuardLayer() const
{
   return use_inner_guard_layer_;
}

int Config::UseOuterGuardLayer() const
{
   return use_outer_guard_layer_;
}

int Config::GetTrackFindLayerNumber1() const
{
   return track_find_cid1_;
}

int Config::GetTrackFindLayerNumber2() const
{
   return track_find_cid2_;
}

const TrackFindRange& Config::GetTrackFindRange() const
{
   return track_find_range_;
}

const char* Config::GetTopDir() const
{
   return top_dir_;
}

const char* Config::GetConfigPath() const
{
   return config_path_;
}

const char* Config::GetWireMapPath() const
{
   return wiremap_path_;
}

const char* Config::GetInputROOTDir() const
{
   return input_root_dir_;
}

const char* Config::GetInputROOTNameTempl() const
{
   return input_root_name_templ_;
}

const char* Config::GetConfigPathAbs() const
{
   return Form("%s/%s", top_dir_, config_path_);
}

const char* Config::GetWireMapPathAbs() const
{
   return Form("%s/%s", top_dir_, wiremap_path_);
}

const char* Config::GetXTParamPathAbs() const
{
   return Form("%s/%s", top_dir_, xt_param_path_);
}

const char* Config::GetInputROOTPathAbs(int run_number) const
{
   const char* input_root_path = Form(input_root_name_templ_, run_number);
   return Form("%s/%s/%s", top_dir_, input_root_dir_, input_root_path);
}

void Config::MakeAll(int run_number)
{
   MakeWireMap();
   MakeXTcurve();
   MakeInputROOT(run_number);
   MakeChamber();
   MakeEvent();
}

void Config::MakeWireMap()
{
   if (wiremap_ptr_) {
      delete wiremap_ptr_;
   }
   wiremap_ptr_ = new WireMap(GetWireMapPathAbs());
}

void Config::MakeXTcurve()
{
   if (xtcurve_ptr_) {
      delete xtcurve_ptr_;
   }
   if (strcmp(GetXTcurveName(), "XT_CONST")==0) {
      xtcurve_ptr_ = new XTcurveConst(xt_drift_velocity_, xt_sigma_r_);
   } else if (strcmp(GetXTcurveName(), "XT_PARAM_POL4")==0) {
      xtcurve_ptr_ = new XTcurvePol4(GetXTParamPathAbs(), xt_sigma_r_);
   } else {
      fprintf(stderr, "ERROR: unknown xt_curve_name %s\n", GetXTcurveName());
   }
}

void Config::MakeChamber()
{
   if (chamber_ptr_) {
      delete chamber_ptr_;
   }
   chamber_ptr_ = new Chamber(this);
}

void Config::MakeInputROOT(int run_number)
{
   if (input_root_ptr_) {
      delete input_root_ptr_;
   }
   input_root_ptr_ = new InputROOT(GetInputROOTPathAbs(run_number));
}

void Config::MakeEvent()
{
   if (event_ptr_) {
      delete event_ptr_;
   }
   event_ptr_ = new Event(this);
}


Run* Config::GetRun(int run_number)
{
   if (num_runs_>=MAX_RUN) {
      fprintf(stderr, "Warning: num_runs reach max %d. New run is not genrated.\n", MAX_RUN);
      return NULL;
   }
   for (int irun=0; irun<num_runs_; irun++) {
      int runnum = runs_[irun]->GetRunNumber();
      //printf("irun %d runnum %d\n", irun, runnum);
      if (run_number == runnum) {
         return runs_[irun];
      }
   }

   runs_[num_runs_] = new Run(this, run_number);
   return runs_[num_runs_++];
}

WireMap* Config::GetWireMap_Ptr()
{
   return wiremap_ptr_;
}

XTcurve* Config::GetXTcurve_Ptr()
{
   return xtcurve_ptr_;
}

Chamber* Config::GetChamber_Ptr()
{
   return chamber_ptr_;
}

InputROOT* Config::GetInputROOT_Ptr()
{
   return input_root_ptr_;
}

Event* Config::GetEvent_Ptr()
{
   return event_ptr_;
}
