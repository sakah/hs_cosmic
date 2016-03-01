#include "config.h"
#include "track.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config::Config()
{
   strcpy(xt_curve_name_, "USE_CONST_XT");
   strcpy(xt_param_path_, "NOT_USED");
   drift_velocity_ = 0.04;
   strcpy(fit_func_name_, "FIT_FUNC_TYPE_FREE_T0");
   t0_bd0_ = -850;
   t0_bd1_ = -850;
   cid1_ = 1;
   cid2_ = 7;
   z_step_ = 10.0; // 10.0 mm

   xt_curve_type_ = XTcurve::USE_CONST_XT;
   fit_func_type_ = Track::FIT_FUNC_TYPE_FREE_T0;
}

void Config::ReadParameters(const char* path)
{
   strcpy(config_path_, path);

   FILE* fp = fopen(path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open %s\n", path);
      exit(1);
   }
   char line[128];
   while(fgets(line, sizeof(line), fp)) {
      if (strstr(line, "xt_curve_name")) sscanf(line, "xt_curve_name %s", xt_curve_name_);
      if (strstr(line, "xt_param_path")) sscanf(line, "xt_param_path %s", xt_param_path_); // relative path
      if (strstr(line, "drift_velocity"))sscanf(line, "drift_velocity %lf", &drift_velocity_);
      if (strstr(line, "fit_func_name")) sscanf(line, "fit_func_name %s", fit_func_name_);
      if (strstr(line, "t0_bd0"))        sscanf(line, "t0_bd0 %lf", &t0_bd0_);
      if (strstr(line, "t0_bd1"))        sscanf(line, "t0_bd1 %lf", &t0_bd1_);
      if (strstr(line, "cid1"))          sscanf(line, "cid1 %d", &cid1_);
      if (strstr(line, "cid2"))          sscanf(line, "cid2 %d", &cid2_);
      if (strstr(line, "z_step"))        sscanf(line, "z_step %lf", &z_step_);
   }
   fclose(fp);

   if (strcmp(xt_param_path_, "NOT_USED")!=0) {
      sprintf(xt_param_path_, "%s/%s", __FILE__, xt_param_path_);
   }

   if      (strcmp(fit_func_name_, "FIT_FUNC_TYPE_FIX_T0")==0)  { fit_func_type_ = Track::FIT_FUNC_TYPE_FIX_T0; } 
   else if (strcmp(fit_func_name_, "FIT_FUNC_TYPE_FREE_T0")==0) { fit_func_type_ = Track::FIT_FUNC_TYPE_FREE_T0; } 

   if      (strcmp(xt_curve_name_, "USE_CONST_XT")==0)   { xt_curve_type_ = XTcurve::USE_CONST_XT; } 
   else if (strcmp(xt_curve_name_, "USE_PARAM_POL4")==0) { xt_curve_type_ = XTcurve::USE_PARAM_POL4; } 

   PrintParameters();
}

void Config::PrintParameters()
{
   printf("xt_curve_name   %s\n", xt_curve_name_);
   printf("xt_param_path   %s\n", xt_param_path_);
   printf("drift_velocity  %6.2f [mm/ns]\n", drift_velocity_);
   printf("fit_func_name   %s\n", fit_func_name_);
   printf("t0_bd0          %5.2f [ns]\n", t0_bd0_);
   printf("t0_bd1          %5.2f [ns]\n", t0_bd1_);
   printf("cid1            %d\n", cid1_);
   printf("cid2            %d\n", cid2_);
   printf("z_step          %6.2f [mm]\n", z_step_);
}

const char* Config::GetConfigPath()
{
   return config_path_;
}

char* Config::GetXTcurveName()
{
   return xt_curve_name_;
}

int Config::GetXTcurveType()
{
   return xt_curve_type_;
}

char* Config::GetFitFuncName()
{
   return fit_func_name_;
}

int Config::GetFitFuncType()
{
   return fit_func_type_;
}

double Config::GetT0_Bd0()
{
   return t0_bd0_;
}

double Config::GetT0_Bd1()
{
   return t0_bd1_;
}

int Config::GetLayerNumber1()
{
   return cid1_;
}

int Config::GetLayerNumber2()
{
   return cid2_;
}

double Config::GetZstep()
{
   return z_step_;
}

double Config::GetDriftVelocity()
{
   return drift_velocity_;
}

char* Config::GetXTParamPath()
{
   return xt_param_path_;
}
