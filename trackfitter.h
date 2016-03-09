#ifndef TRACK_FITTER_H
#define TRACK_FITTER_H

#include "TMinuit.h"
#include "TFitter.h"

class TrackFitter
{
   public:
      enum { FIT_FUNC_TYPE_FIX_T0 = 0, FIT_FUNC_TYPE_FREE_T0 };
      TrackFitter(Track ini_track, int num_params);
      Track& GetIniTrack();
      Track& GetFitTrack();
      int GetNumParams();
      int GetFitFuncType(const char* fit_func_name);
      virtual void   DoFit()=0;
      virtual void   GetIniParams(double* ini_params)=0;
      virtual void   GetFitParams(double* fit_params)=0;
      virtual double GetChi2()=0;
      virtual double GetNDF()=0;
      virtual double GetRedChi2()=0;

   private:
      Track ini_track_;
      Track fit_track_;
      int num_params_;
};

#endif
