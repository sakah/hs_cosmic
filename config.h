#ifndef CONFIG_H
#define CONFIG_H

#include "param.h"
#include "TString.h"
//#include <string>
#include "range.h"

class Run;
//class TrackFindRange;
#include "trackfindrange.h"
class WireMap;
class XTcurve;
class Chamber;
class InputROOT;
class Event;

class Config
{
   public:
      ~Config();
      Config();
      Config(const char* top_dir);
      Config(const char* top_dir, const char* config_path);
      Config(const Config& obj);
      void CopyConfig(const Config& other);
      void PrintConfig();
      void ls();
      void SetDefaults();
      void ReadConfig(const char* config_path);
      //Config NewConfig(int par);
      //Config NewConfig(int par, int par2);
      Config track_find_cid1(int new_value);
      Config track_find_cid2(int new_value, int a);
      //Config track_find_cid3(Hoge a);
      Config track_find_cid4(char str);
      //Range<double>& track_find_cid3();
      //Config NewConfig(TString par);
      //Config* ChangeConfig(const char* name, const char* value);
      //Config ChangeConfig(const char* name, int value);
      //Config ChangeConfig(const char* name, double value);
      //Config ChangeConfig(const char* name, int idx, double value);
      //Config ChangeConfig(const char* name, double min, double max, double step);
      //Config ChangeConfig(const char* name, int min, int max, int step);

      int    GetNumBoards() const;
      double GetT0Board(int bd) const;
      int    GetT0Boards(double* t0_boards) const;
      const char*  GetXTcurveName() const;
      const char*  GetXTParamPath() const;
      double GetXTDriftVelocity() const;
      double GetXTSigmaR() const;
      const char*  GetFitFuncName() const;
      int    GetTrackFindLayerNumber1() const;
      int    GetTrackFindLayerNumber2() const;
      const TrackFindRange& GetTrackFindRange() const;
      int    UseInnerGuardLayer() const;
      int    UseOuterGuardLayer() const;

      const char* GetTopDir() const;
      const char* GetConfigPath() const;
      const char* GetWireMapPath() const;
      const char* GetInputROOTDir() const;
      const char* GetInputROOTNameTempl() const;

      const char* GetConfigPathAbs() const;
      const char* GetWireMapPathAbs() const;
      const char* GetXTParamPathAbs() const;
      const char* GetInputROOTPathAbs(int run_number) const;

      Run* GetRun(int run_number);

      void MakeAll(int run_number);
      void MakeWireMap();
      void MakeXTcurve();
      void MakeChamber();
      void MakeInputROOT(int run_number);
      void MakeEvent();

      WireMap* GetWireMap_Ptr();
      XTcurve* GetXTcurve_Ptr();
      Chamber* GetChamber_Ptr();
      InputROOT* GetInputROOT_Ptr();
      Event* GetEvent_Ptr();

   private:
      char top_dir_[512];
      char config_path_[512];
      char wiremap_path_[512];
      char input_root_dir_[512];
      char input_root_name_templ_[32];
      int num_boards_;
      double t0_boards_[MAX_BOARD]; // [board]
      char xt_curve_name_[32];
      char xt_param_path_[512];
      double xt_drift_velocity_; // [mm/ns]
      double xt_sigma_r_;
      char fit_func_name_[64];
      int use_inner_guard_layer_;
      int use_outer_guard_layer_;
      int track_find_cid1_;
      int track_find_cid2_;
      TrackFindRange track_find_range_;

      WireMap* wiremap_ptr_;
      XTcurve* xtcurve_ptr_;
      Chamber* chamber_ptr_;
      InputROOT* input_root_ptr_;
      Event* event_ptr_;

      int num_runs_;
      Run* runs_[MAX_RUN];
};

#endif
