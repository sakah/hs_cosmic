#ifndef CONFIG_H
#define CONFIG_H

#include "param.h"

class Config
{
   public:
      Config();
      void ReadParameters(const char* path);
      void PrintParameters();

      const char* GetProgPath();
      const char* GetConfigPath();

      char*  GetXTcurveName();
      int    GetXTcurveType();
      char*  GetFitFuncName();
      int    GetFitFuncType();
      double GetT0_Bd0();
      double GetT0_Bd1();
      int    GetLayerNumber1();
      int    GetLayerNumber2();
      double GetZstep();
      double GetDriftVelocity();
      char*  GetXTParamPath();
      bool   IncludeOuterGuardLayer();

      void SetProgPath(const char* prog_path);
      

   private:
      char prog_path_[512];
      char config_path_[512];
      char xt_curve_name_[32];
      int xt_curve_type_;
      char fit_func_name_[64];
      int fit_func_type_;
      double t0_bd0_;
      double t0_bd1_;
      int cid1_;
      int cid2_;
      double z_step_; // [mm]
      double drift_velocity_; // [mm/ns]
      char xt_param_path_[512];
      bool include_outer_guard_layer_;
};

#endif

