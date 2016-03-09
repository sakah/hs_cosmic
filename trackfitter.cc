#include "trackfitter.h"

TrackFitter::TrackFitter(Track ini_track, int num_params)
{
   ini_track_ = ini_track;
   num_params_ = num_params;
}

Track& TrackFitter::GetIniTrack()
{
   return ini_track_;
}

Track& TrackFitter::GetFitTrack()
{
   return fit_track_;
}

int TrackFitter::GetNumParams()
{
   return num_params_;
}

int TrackFitter::GetFitFuncType(const char* fit_func_name)
{
   int fit_func_type = -1;
   if      (strcmp(fit_func_name, "FIT_FUNC_TYPE_FIX_T0")==0)  { fit_func_type = Track::FIT_FUNC_TYPE_FIX_T0; } 
   else if (strcmp(fit_func_name, "FIT_FUNC_TYPE_FREE_T0")==0) { fit_func_type = Track::FIT_FUNC_TYPE_FREE_T0; } 
   else {
      fprintf(stderr, "ERROR: unknown fit_func_name %s\n", fit_func_name);
   }
   return fit_func_type;
}
