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
   Config("./");
}

Config::Config(const char* top_dir)
{
   strcpy(top_dir_, top_dir);

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

void Config::PrintConfig()
{
   printf("-------------------------\n");
   printf("top_dir                %s\n", top_dir_);
   printf("config_path            %s\n", config_path_);
   printf("xt_curve_name          %s\n", xt_curve_name_);
   printf("xt_param_path          %s\n", xt_param_path_);
   printf("xt_drift_velocity      %6.2f [mm/ns]\n", xt_drift_velocity_);
   printf("xt_sigma_r             %6.2f [mm]\n", xt_sigma_r_);
   printf("fit_func_name          %s\n", fit_func_name_);
   for (int bd=0; bd<num_boards_; bd++) {
      printf("bd %d t0               %5.2f [ns]\n", bd, t0_boards_[bd]);
   }
   printf("track_find_cid1       %d\n", track_find_cid1_);
   printf("track_find_cid2       %d\n", track_find_cid2_);
   printf("use_inner_guard_layer %d\n", use_inner_guard_layer_);
   printf("use_outer_guard_layer %d\n", use_outer_guard_layer_);
   printf("... track_find_range ...\n");
   track_find_range_.PrintTrackFindRange();
   printf("-------------------------\n");
}

void Config::ReadConfig(const char* config_path)
{
   strcpy(config_path_, config_path);

   FILE* fp = fopen(config_path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open %s\n", config_path);
      exit(1);
   }
   char line[128];
   int board;
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
      if (strstr(line, "t0"))                  { sscanf(line, "board %d t0 %lf", &board, &t0); t0_boards_[board] = t0;}
      if (strstr(line, "xt_curve_name"))         sscanf(line, "xt_curve_name %s", xt_curve_name_);
      if (strstr(line, "xt_param_path"))         sscanf(line, "xt_param_path %s", xt_param_path_); // relative path
      if (strstr(line, "xt_drift_velocity"))     sscanf(line, "drift_velocity %lf", &xt_drift_velocity_);
      if (strstr(line, "xt_sigma_r"))            sscanf(line, "xt_sigma_r %lf",     &xt_sigma_r_);
      if (strstr(line, "fit_func_name"))         sscanf(line, "fit_func_name %s", fit_func_name_);
      if (strstr(line, "track_find_cid1"))       sscanf(line, "track_find_cid1 %d", &track_find_cid1_);
      if (strstr(line, "track_find_cid2"))       sscanf(line, "track_find_cid2 %d", &track_find_cid2_);
      if (strstr(line, "track_find_range_z1"))   { sscanf(line, "track_find_range_z1 %lf %lf %lf", &min, &max, &step); track_find_range_.UpdateRange("track_find_range_z1", min, max, step); }
      if (strstr(line, "track_find_range_z2"))   { sscanf(line, "track_find_range_z2 %lf %lf %lf", &min, &max, &step); track_find_range_.UpdateRange("track_find_range_z2", min, max, step); }
      if (strstr(line, "track_find_range_t0"))   { sscanf(line, "track_find_range_t0 %lf %lf %lf", &min, &max, &step); track_find_range_.UpdateRange("track_find_range_t0", min, max, step); }
      if (strstr(line, "track_find_range_drift_radius")) { sscanf(line, "trak_find_drift_radius %lf %lf", &min, &max); track_find_range_.UpdateRange("track_find_drift_radius", min, max); }
      if (strstr(line, "track_find_num_tracks")) { sscanf(line, "trak_find_num_tracks %d %d", &imin, &imax); track_find_range_.UpdateRange("track_find_num_tracks", imin, imax); }
      if (strstr(line, "use_inner_guard_layer")) sscanf(line, "use_inner_guard_layer %d", &use_inner_guard_layer_);
      if (strstr(line, "use_outer_guard_layer")) sscanf(line, "use_outer_guard_layer %d", &use_outer_guard_layer_);
   }
   fclose(fp);

   PrintConfig();
}

Config Config::ChangeConfig(const char* name, char* value)
{
   if (strstr(name, "wiremap_path"))          strcpy(wiremap_path_, value);
   if (strstr(name, "input_root_dir"))        strcpy(input_root_dir_, value);
   if (strstr(name, "input_root_name_templ")) strcpy(input_root_name_templ_, value);
   if (strstr(name, "xt_curve_name")) strcpy(xt_curve_name_, value);
   if (strstr(name, "xt_param_path")) strcpy(xt_param_path_, value);
   if (strstr(name, "fit_func_name")) strcpy(fit_func_name_, value);
   return *this;
}

Config Config::ChangeConfig(const char* name, int value)
{
   if (strstr(name, "track_find_cid1"))       track_find_cid1_ = value;
   if (strstr(name, "track_find_cid2"))       track_find_cid2_ = value;
   if (strstr(name, "use_inner_guard_layer")) use_inner_guard_layer_ = value;
   if (strstr(name, "use_outer_guard_layer")) use_outer_guard_layer_ = value;
   return *this;
}

Config Config::ChangeConfig(const char* name, double value)
{
   if (strstr(name, "xt_drift_velocity")) xt_drift_velocity_ = value;
   if (strstr(name, "xt_sigma_r"))        xt_sigma_r_ = value;
   return *this;
}

Config Config::ChangeConfig(const char* name, int idx, double value)
{
   if (strstr(name, "t0")) t0_boards_[idx] = value;
   return *this;
}

Config Config::ChangeConfig(const char* name, double min, double max, double step)
{
   track_find_range_.UpdateRange(name, min, max, step);
   return *this;
}

Config Config::ChangeConfig(const char* name, int min, int max, int step)
{
   track_find_range_.UpdateRange(name, min, max, step);
   return *this;
}

int Config::GetNumBoards()
{
   return num_boards_;
}

double Config::GetT0Board(int board)
{
   return t0_boards_[board];
}

int Config::GetT0Boards(double* t0_boards)
{
   for (int bd=0; bd<num_boards_; bd++) {
      t0_boards[bd] = t0_boards_[bd];
   }
   return num_boards_;
}

char* Config::GetXTcurveName()
{
   return xt_curve_name_;
}

char* Config::GetXTParamPath()
{
   return xt_param_path_;
}

double Config::GetXTDriftVelocity()
{
   return xt_drift_velocity_;
}

double Config::GetXTSigmaR()
{
   return xt_sigma_r_;
}

char* Config::GetFitFuncName()
{
   return fit_func_name_;
}

int Config::UseInnerGuardLayer()
{
   return use_inner_guard_layer_;
}

int Config::UseOuterGuardLayer()
{
   return use_outer_guard_layer_;
}

int Config::GetTrackFindLayerNumber1()
{
   return track_find_cid1_;
}

int Config::GetTrackFindLayerNumber2()
{
   return track_find_cid2_;
}

TrackFindRange& Config::GetTrackFindRange()
{
   return track_find_range_;
}

const char* Config::GetTopDir()
{
   return top_dir_;
}

const char* Config::GetConfigPath()
{
   return config_path_;
}

const char* Config::GetWireMapPath()
{
   return wiremap_path_;
}

const char* Config::GetConfigPathAbs()
{
   return Form("%s/%s", top_dir_, config_path_);
}

const char* Config::GetWireMapPathAbs()
{
   return Form("%s/%s", top_dir_, wiremap_path_);
}

const char* Config::GetXTParamPathAbs()
{
   return Form("%s/%s", top_dir_, xt_param_path_);
}

const char* Config::GetInputROOTPathAbs(int run_number)
{
   const char* input_root_path = Form(input_root_name_templ_, run_number);
   return Form("%s/%s/%s", top_dir_, input_root_dir_, input_root_path);
}

void Config::MakeAll(int run_number)
{
   MakeWireMap();
   MakeXTcurve();
   MakeChamber();
   MakeInputROOT(run_number);
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
   if (strcmp(GetXTcurveName(), "USE_CONST_XT")==0) {
      xtcurve_ptr_ = new XTcurveConst(xt_drift_velocity_, xt_sigma_r_);
   } else if (strcmp(GetXTcurveName(), "USE_PARAM_POL4")==0) {
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


Run Config::GetRun(int run_number)
{
   return Run(this, run_number);
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
